/********************************************************************
 *
 * Copyright (C) 1996 Blue Marsh Softworks - All rights reserved
 *
 * PROEJCT:      COMP 418 Compiler Design
 * MODULE:       Syntax Checker
 * FILE:         syncheck.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      02/06/96  Initial mylex version
 *       NF      02/18/96  Converted to test lex output
 *       NF      04/07/96  Converted to syntax checker
 *
 * DESCRIPTION:
 *      This is the syntax checker for the subset of Pascal described
 *      on p. 745 of the text.
 *
 *******************************************************************/

/********************************************************************
 *                 Header Files
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    #include "symtab.h"
    #include "syncheck.h"

/********************************************************************
 *                 Function Definitions
 *******************************************************************/

/********************************************************************
 *                 External Variables
 *******************************************************************/
       /* Input file that main() will open and yylex uses. */
    extern FILE * yyin;

/********************************************************************
 *                 Global Variables
 *******************************************************************/
       /* Lexical value. */
    int  yylval;
       /* Text string containing current token. */
    char yytext[];
       /* Length of string in yytext. */
    int  yyleng;
       /* Current token being processed. */
    Token lookahead;
       /* Current line number being parsed. */
    int curLine;

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Testwrapper for the yylex function.
 * RETURNS:      int - error code
 * STRATEGY:     Open the input file passed on the command line.
 *               Start calling yylex in a loop, outputting the
 *               results to the screen. Close the file when done.
 *******************************************************************/
int main
  (
  int    argc,  /* Number of command-line arguments. (in) */
  char * argv[] /* Array of argument strings. (in) */
  )
{

     /*
      * Check command-line arguments. Display message
      * if incorrect number of arguments.
      * Display message if /? is entered (standard
      * DOS feature).
      */
  if ( ( argc == 1 ) ||
       ( ( argc == 2 ) && ( strcmp( argv[1], "/?" ) == 0 ) ) ) {
    printf( "Syntax checker program for COMP 418, Spring '96\n" );
    printf( "Usage: syncheck FILENAME.EXT\n" );
    return( 0 );
  }

     /*
      * Start symbol table.
      * Open the input file.
      * While not end of file,
      *   Call program to parse the input.  
      * Free symbol table.
      */
  if ( SymTabInitialize() ) {

    printf( "ERROR: Low on memory.\n" );
    exit( 1 );
  }
  yyin = fopen( argv[1], "rt" );
  if ( yyin == NULL ) {
    printf( "Error opening file %s!", argv[1] );
    exit( 1 );
  }
  curLine = 1;
  lookahead = yylex();
  if ( ( lookahead >= 0 ) && ( lookahead != TOKEN_DONE ) ) {
    program();
  }
  else {
    printf( "Not a valid token: %d\n", lookahead );
  }
  SymTabFree();

     /*
      * Close the input file.
      */
  fclose( yyin );
  return( 0 );
} /* main */

/********************************************************************
 *                 match
 ********************************************************************
 * SYNOPSIS:     Matches a token to the token in the lookahead
 *               buffer.
 * RETURNS:      boolean - TRUE if error
 * STRATEGY:     Check if lookahead is equal to the passed parameter
 *               and if it is, set lookahead to the next token.
 *******************************************************************/
boolean match
  (
  Token t
  )
{
  if ( lookahead == t ) {
    lookahead = yylex();
    yylval = 0;
    return( FALSE );
  }
  else {
    return( TRUE );
  }
} /* match */

/********************************************************************
 *                 program
 ********************************************************************
 * SYNOPSIS:     This starts the parsing for the program.
 * RETURNS:      void
 * STRATEGY:     It begins by looking for "program" and the
 *               required parts of a program.
 *******************************************************************/
void program
  (
  void
  )
{
  if ( lookahead == TOKEN_KEY_PROGRAM ) {
    match( TOKEN_KEY_PROGRAM );
    if ( match( TOKEN_ID ) ) {
      printf( "%d SYNTAX ERROR: Missing program name\n", curLine );
    }
    if ( match( TOKEN_PAREN_OPEN ) ) {
      printf( "%d SYNTAX ERROR: Missing ( in program heading.\n", 
	      curLine );
    }
    identifier_list();
    if ( match( TOKEN_PAREN_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ) in parameter list.\n",
	      curLine );
    }
    if ( match( TOKEN_SEMICOLON ) ) {
      printf( "%d SYNTAX ERROR: Missing ; after program heading\n",
	      curLine );
    }
    declarations();
    subprogram_declarations();
    compound_statement();
  }
  else {
    printf( "%d SYNTAX ERROR: Missing keyword program\n", curLine );
  }
} /* program */

/********************************************************************
 *                 identifier_list
 ********************************************************************
 * SYNOPSIS:     Parses an identifier list.
 * RETURNS:      void
 * STRATEGY:     Looks for identifiers separated by commas.
 *******************************************************************/
void identifier_list
  (
  void
  )
{
  if ( lookahead == TOKEN_ID ) {
    match( TOKEN_ID );
    identifier_listp();
  }
  else {
    printf( "%d SYNTAX ERROR: Missing identifier in identifier list.\n",
            curLine );
  }
} /* identifier_list */

/********************************************************************
 *                 identifier_listp
 ********************************************************************
 * SYNOPSIS:     Second part of identifier list parser.
 * RETURNS:      void
 * STRATEGY:     Looks for commas separating identifiers.
 *******************************************************************/
void identifier_listp
  (
  void
  )
{
  if ( lookahead == TOKEN_COMMA ) {
    match( TOKEN_COMMA );
    if ( match( TOKEN_ID ) ) {
      printf( "%d SYNTAX ERROR: Comma cannot end identifier list.\n",
	      curLine );
    }
    identifier_listp();
  }
} /* identifier_listp */

/********************************************************************
 *                 declarations
 ********************************************************************
 * SYNOPSIS:     Parses variable declarations.
 * RETURNS:      void
 * STRATEGY:     Looks for the keyword "var" followed by identifiers
 *               and types.
 *******************************************************************/
void declarations
  (
  void
  )
{
  if ( lookahead == TOKEN_KEY_VAR ) {
    match( TOKEN_KEY_VAR );
    identifier_list();
    if ( match( TOKEN_COLON ) ) {
      printf( "%d SYNTAX ERROR: Missing : in variable declarations\n",
	      curLine );
    }
    type();
    if ( match( TOKEN_SEMICOLON ) ) {
      printf( "%d SYNTAX ERROR: Missing ; after declaration\n",
	      curLine );
    }
    declarations();
  }
} /* declarations */

/********************************************************************
 *                 type
 ********************************************************************
 * SYNOPSIS:     Parses the variable data type.
 * RETURNS:      void
 * STRATEGY:     Matches either standard types or arrays. Arrays
 *               are made of the keyword "array" followed by
 *               [, a range specified by two numbers separated by
 *               .., followed by ] and the keyword "of" and a
 *               standard type.
 *******************************************************************/
void type
  (
  void
  )
{
  if ( ( lookahead == TOKEN_KEY_INTEGER ) ||
       ( lookahead == TOKEN_KEY_REAL    ) ) {
    standard_type();
  }
  else if ( lookahead == TOKEN_KEY_ARRAY ) {
    match( TOKEN_KEY_ARRAY );
    if ( match( TOKEN_BRACKET_OPEN ) ) {
      printf( "%d SYNTAX ERROR: Missing [ in array definition\n",
	      curLine );
    }
    if ( match( TOKEN_NUM ) ) {
      printf( "%d SYNTAX ERROR: Missing lower array bound\n",
	      curLine );
    }
    if ( match( TOKEN_DOTDOT ) ) {
      printf( "%d SYNTAX ERROR: Missing array bound separator (..)\n",
	      curLine );
    }
    if ( match( TOKEN_NUM ) ) {
      printf( "%d SYNTAX ERROR: Missing upper array bound\n",
	      curLine );
    }
    if ( match( TOKEN_BRACKET_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ] in array definition\n",
	      curLine );
    }
    if ( match( TOKEN_KEY_OF ) ) {
      printf( "%d SYNTAX ERROR: Missing keyword of in array \
definition\n", curLine );
    }
    standard_type();
  }
  else {
    printf( "%d SYNTAX ERROR: Not a supported data type\n", curLine );
  }
} /* type */

/********************************************************************
 *                 standard_type
 ********************************************************************
 * SYNOPSIS:     Parses a standard data type (number).
 * RETURNS:      void
 * STRATEGY:     A standard type is a number, either a real or an
 *               integer.
 *******************************************************************/
void standard_type
  (
  void
  )
{
  if ( lookahead == TOKEN_KEY_INTEGER ) {
    match( TOKEN_KEY_INTEGER );
  }
  else if ( lookahead == TOKEN_KEY_REAL ) {
    match( TOKEN_KEY_REAL );
  }
  else {
    printf( "%d SYNTAX ERROR: Not a supported standard data type.\n",
            curLine );
  }
} /* standard_type */

/********************************************************************
 *                 subprogram_declarations
 ********************************************************************
 * SYNOPSIS:     Parses the subprogram declarations, made up of
 *               procedures and functions.
 * RETURNS:      void
 * STRATEGY:     Match either the keyword "function" or "procedure"
 *               and parse appropriately.
 *******************************************************************/
void subprogram_declarations
  (
  void
  )
{
  if ( ( lookahead == TOKEN_KEY_FUNCTION ) ||
       ( lookahead == TOKEN_KEY_PROCEDURE ) ) {
    subprogram_declaration();
    if ( match( TOKEN_SEMICOLON ) ) {
      printf( "%d SYNTAX ERROR: Missing ; after end of subprogram \
declaration\n", curLine );
    }
    subprogram_declarations();
  }
} /* subprogram_declarations */

/********************************************************************
 *                 subprogram_declaration
 ********************************************************************
 * SYNOPSIS:     Parse a single subprogram declaration, either a
 *               function or a procedure.
 * RETURNS:      void
 * STRATEGY:     Match either the "function" or "procedure" keyword
 *               and parse the subprogram heading, followed by
 *               declarations and the subprogram body.
 *******************************************************************/
void subprogram_declaration
  (
  void
  )
{
  if ( ( lookahead == TOKEN_KEY_FUNCTION ) ||
       ( lookahead == TOKEN_KEY_PROCEDURE ) ) {
    subprogram_head();
    declarations();
    compound_statement();
  }
  else {
    printf( "%d SYNTAX ERROR: Invalid subprogram declaration\n",
            curLine );
  }
} /* subprogram_declaration */

/********************************************************************
 *                 subprogram_head
 ********************************************************************
 * SYNOPSIS:     Parse the subprogram heading.
 * RETURNS:      void
 * STRATEGY:     Match either "function" or "procedure" and parse
 *               appropriately. If function, expect a return type.
 *               If procedure, no return type is provided.
 *******************************************************************/
void subprogram_head
  (
  void
  )
{
  if ( lookahead == TOKEN_KEY_FUNCTION ) {
    match( TOKEN_KEY_FUNCTION );
    if ( match( TOKEN_ID ) ) {
      printf( "%d SYNTAX ERROR: Missing function name\n", curLine );
    }
    arguments();
    if ( match( TOKEN_COLON ) ) {
      printf( "%d SYNTAX ERROR: Missing : in function heading\n",
	      curLine );
    }
    standard_type();
    if ( match( TOKEN_SEMICOLON ) ) {
      printf( "%d SYNTAX ERROR: Missing ; after function heading\n",
	      curLine );
    }
  }
  else if ( lookahead == TOKEN_KEY_PROCEDURE ) {
    match( TOKEN_KEY_PROCEDURE );
    if ( match( TOKEN_ID ) ) {
      printf( "%d SYNTAX ERROR: Missing procedure name\n", curLine );
    }
    arguments();
    if ( match( TOKEN_SEMICOLON ) ) {
      printf( "%d SYNTAX ERROR: Missing ; after procedure heading\n", 
	      curLine );
    }
  }
  else {
    printf( "%d SYNTAX ERROR: Missing subprogram heading\n", curLine );
  }
} /* subprogram_head */

/********************************************************************
 *                 arguments
 ********************************************************************
 * SYNOPSIS:     Parse a list of arguments.
 * RETURNS:      void
 * STRATEGY:     Match on an opening parenthesis, followed by a
 *               parameter list, then a closing parenthesis.
 *******************************************************************/
void arguments
  (
  void
  )
{
  if ( lookahead == TOKEN_PAREN_OPEN ) {
    match( TOKEN_PAREN_OPEN );
    parameter_list();
    if ( match( TOKEN_PAREN_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ) in arguments list\n",
	      curLine );
    }
  }
} /* arguments */

/********************************************************************
 *                 parameter_list
 ********************************************************************
 * SYNOPSIS:     Parse a parameter list.
 * RETURNS:      void
 * STRATEGY:     First look for an identifier list, followed by a
 *               colon, a type, and more of the parameter list.
 *******************************************************************/
void parameter_list
  (
  void
  )
{
  if ( lookahead == TOKEN_ID ) {
    identifier_list();
    if ( match( TOKEN_COLON ) ) {
      printf( "%d SYNTAX ERROR: Missing : in parameter list\n",
	      curLine );
    }
    type();
    parameter_listp();
  }
  else {
    printf( "%d SYNTAX ERROR: Missing parameter list between ()\n",
            curLine );
  }
} /* parameter_list */

/********************************************************************
 *                 parameter_listp
 ********************************************************************
 * SYNOPSIS:     Parse the remaining portion of the parameter list.
 * RETURNS:      void
 * STRATEGY:     Match a semicolon, followed by an identifier list,
 *               colon, type, and more parameter list.
 *******************************************************************/
void parameter_listp
  (
  void
  )
{
  if ( lookahead == TOKEN_SEMICOLON ) {
    match( TOKEN_SEMICOLON );
    identifier_list();
    if ( match( TOKEN_COLON ) ) {
      printf( "%d SYNTAX ERROR: Missing : in parameter list\n",
	      curLine );
    }
    type();
    parameter_listp();
  }
} /* parameter_listp */

/********************************************************************
 *                 compound_statement
 ********************************************************************
 * SYNOPSIS:     Parse a compound statement.
 * RETURNS:      void
 * STRATEGY:     Compound statements begin with keyword "begin",
 *               followed by optional statements, ending with
 *               keyword "end".
 *******************************************************************/
void compound_statement
  (
  void
  )
{
  if ( lookahead == TOKEN_KEY_BEGIN ) {
    match( TOKEN_KEY_BEGIN );
    optional_statements();
    if ( match( TOKEN_KEY_END ) ) {
      printf( "%d SYNTAX ERROR: Missing keyword end in compound \
statement\n", curLine );
    }
  }
  else {
    printf( "%d SYNTAX ERROR: Missing keyword begin in compound statement.\n",
            curLine );
  }
} /* compound_statement */

/********************************************************************
 *                 optional_statements
 ********************************************************************
 * SYNOPSIS:     Parse the optional statements associated with a
 *               compound statement.
 * RETURNS:      void
 * STRATEGY:     Look for a statement list (which means it must
 *               start with a statement).
 *******************************************************************/
void optional_statements
  (
  void
  )
{
  if ( isstatement( lookahead ) ) {
    statement_list();
  }
} /* optional_statements */

/********************************************************************
 *                 statement_list
 ********************************************************************
 * SYNOPSIS:     Parse a statement list.
 * RETURNS:      void
 * STRATEGY:     Look for a statement, followed by more of a
 *               statement list.
 *******************************************************************/
void statement_list
  (
  void
  )
{
  if ( isstatement( lookahead ) ) {
    statement();
    statement_listp();
  }
  else {
    printf( "%d SYNTAX ERROR: Expecting a statement list.\n", curLine );
  }
} /* statement_list */

/********************************************************************
 *                 statement_listp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of the statement list.
 * RETURNS:      void
 * STRATEGY:     Look for semicolons separating the statements.
 *******************************************************************/
void statement_listp
  (
  void
  )
{
  if ( lookahead == TOKEN_SEMICOLON ) {
    match( TOKEN_SEMICOLON );
    statement();
    statement_listp();
  }
} /* statement_listp */

/********************************************************************
 *                 isstatement
 ********************************************************************
 * SYNOPSIS:     Check if the token could start a statement.
 * RETURNS:      boolean - TRUE if token is beginning of a statement.
 * STRATEGY:     Compare token to an identifier, keywords begin,
 *               if, and while. Return TRUE if a match, FALSE
 *               otherwise.
 *******************************************************************/
boolean isstatement
  (
  Token t
  )
{
  if ( ( t == TOKEN_ID        ) ||
       ( t == TOKEN_KEY_BEGIN ) ||
       ( t == TOKEN_KEY_IF    ) ||
       ( t == TOKEN_KEY_WHILE ) ) {
    return( TRUE );
  }
  else {
    return( FALSE );
  }
} /* isstatement */

/********************************************************************
 *                 statement
 ********************************************************************
 * SYNOPSIS:     Parse a statement, which is made up of several
 *               possible components.
 * RETURNS:      void
 * STRATEGY:     Check for an identifier, or a begin, an if, or a
 *               while.
 *******************************************************************/
void statement
  (
  void
  )
{
  if ( lookahead == TOKEN_ID ) {
    match( TOKEN_ID );
    statementp();
  }
  else if ( lookahead == TOKEN_KEY_BEGIN ) {
    compound_statement();
  }
  else if ( lookahead == TOKEN_KEY_IF ) {
    match( TOKEN_KEY_IF );
    expression();
    if ( match( TOKEN_KEY_THEN ) ) {
      printf( "%d SYNTAX ERROR: Missing keyword then in if statement\n",
	      curLine );
    }
    statement();
    if ( lookahead == TOKEN_SEMICOLON ) {
      printf( "%d SYNTAX ERROR: Cannot have ; before else\n", curLine );
    }
    if ( match( TOKEN_KEY_ELSE ) ) {
      printf( "%d SYNTAX ERROR: Missing keyword else in if statement\n",
	      curLine );
    }
    statement();
  }
  else if ( lookahead == TOKEN_KEY_WHILE ) {
    match( TOKEN_KEY_WHILE );
    expression();
    if ( match( TOKEN_KEY_DO ) ) {
      printf( "%d SYNTAX ERROR: Missing keyword do in while loop\n",
	      curLine );
    }
    statement();
  }
  else {
    printf( "%d SYNTAX ERROR: Not a valid statement\n", curLine );
  }
} /* statement */

/********************************************************************
 *                 statementp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of a statement.
 * RETURNS:      void
 * STRATEGY:     Check for an assignment operator, or an opening
 *               bracket, or an opening parenthesis.
 *******************************************************************/
void statementp
  (
  void
  )
{
  if ( lookahead == TOKEN_ASSIGNOP ) {
    match( TOKEN_ASSIGNOP );
    expression();
  }
  else if ( lookahead == TOKEN_BRACKET_OPEN ) {
    match( TOKEN_BRACKET_OPEN );
    expression();
    if ( match( TOKEN_BRACKET_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ] after expression\n",
	      curLine );
    }
    if ( match( TOKEN_ASSIGNOP ) ) {
      printf( "%d SYNTAX ERROR: Missing := after expression\n",
	      curLine );
    }
    expression();
  }
  else if ( lookahead == TOKEN_PAREN_OPEN ) {
    match( TOKEN_PAREN_OPEN );
    expression_list();
    if ( match( TOKEN_PAREN_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ) after expression\n",
	      curLine );
    }
  }
} /* statementp */

/********************************************************************
 *                 expression_list
 ********************************************************************
 * SYNOPSIS:     Parse a list of expressions.
 * RETURNS:      void
 * STRATEGY:     Check for an expression followed by more of an
 *               expression list.
 *******************************************************************/
void expression_list
  (
  void
  )
{
  if ( isexpression( lookahead ) ) {
    expression();
    expression_listp();
  }
  else {
    printf( "%d SYNTAX ERROR: Expecting an expression\n", curLine );
  }
} /* expression_list */

/********************************************************************
 *                 expression_listp
 ********************************************************************
 * SYNOPSIS:     Rest of an expression list.
 * RETURNS:      void
 * STRATEGY:     Look for a comma separating the expressions.
 *******************************************************************/
void expression_listp
  (
  void
  )
{
  if ( lookahead == TOKEN_COMMA ) {
    match( TOKEN_COMMA );
    expression();
    expression_listp();
  }
} /* expression_listp */

/********************************************************************
 *                 isexpression
 ********************************************************************
 * SYNOPSIS:     Check to see if this token could start an
 *               expression.
 * RETURNS:      void
 * STRATEGY:     Compare the token to an identifier, or a number,
 *               or an opening parenthesis, or keyword not, or
 *               an addition operator. If it matches, return TRUE,
 *               otherwise return FALSE.
 *******************************************************************/
boolean isexpression
  (
  Token t
  )
{

  if ( ( t == TOKEN_ID         ) ||
       ( t == TOKEN_NUM        ) ||
       ( t == TOKEN_PAREN_OPEN ) ||
       ( t == TOKEN_KEY_NOT    ) ||
       ( t == TOKEN_ADDOP      ) ) {
    return( TRUE );
  }
  else {
    return( FALSE );
  }
} /* isexpression */

/********************************************************************
 *                 expression
 ********************************************************************
 * SYNOPSIS:     Parse an expression.
 * RETURNS:      void
 * STRATEGY:     Look for a simple expression.
 *******************************************************************/
void expression
  (
  void
  )
{
  if ( isexpression( lookahead ) ) {
    simple_expression();
    expressionp();
  }
} /* expression */

/********************************************************************
 *                 expressionp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of the expression.
 * RETURNS:      void
 * STRATEGY:     Look for a relational operator followed by a
 *               simple expression.
 *******************************************************************/
void expressionp
  (
  void
  )
{
  if ( lookahead == TOKEN_RELOP ) {
    match( TOKEN_RELOP );
    simple_expression();
  }
} /* expressionp */

/********************************************************************
 *                 simple_expression
 ********************************************************************
 * SYNOPSIS:     Parse a simple expression.
 * RETURNS:      void
 * STRATEGY:     Look for a term (or a sign followed by a term)
 *               followed by the rest of the simple expression.
 *******************************************************************/
void simple_expression
  (
  void
  )
{
  if ( ( lookahead == TOKEN_ID ) ||
       ( lookahead == TOKEN_NUM ) ||
       ( lookahead == TOKEN_PAREN_OPEN ) ||
       ( lookahead == TOKEN_KEY_NOT ) ) {
    term();
    simple_expressionp();
  }
  else if ( lookahead == TOKEN_ADDOP ) {
    sign();
    term();
    simple_expressionp();
  }
  else {
    printf( "%d SYNTAX ERROR: Expecting a term or sign in expression\n",
            curLine );
  }
} /* simple_expression */

/********************************************************************
 *                 simple_expressionp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of a simple expression.
 * RETURNS:      void
 * STRATEGY:     Look for a sign followed by a term, followed by
 *               more of the simple expression.
 *******************************************************************/
void simple_expressionp
  (
  void
  )
{
  if ( lookahead == TOKEN_ADDOP ) {
    match( TOKEN_ADDOP );
    term();
    simple_expressionp();
  }
} /* simple_expressionp */

/********************************************************************
 *                 term
 ********************************************************************
 * SYNOPSIS:     Parse a term.
 * RETURNS:      void
 * STRATEGY:     Look for a factor followed by more of a term.
 *******************************************************************/
void term
  (
  void
  )
{
  if ( ( lookahead == TOKEN_ID ) ||
       ( lookahead == TOKEN_NUM ) ||
       ( lookahead == TOKEN_PAREN_OPEN ) ||
       ( lookahead == TOKEN_KEY_NOT ) ) {
    factor();
    termp();
  }
  else {
    printf( "%d SYNTAX ERROR: Expecting a factor in term\n", curLine );
  }
} /* term */

/********************************************************************
 *                 termp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of the term.
 * RETURNS:      void
 * STRATEGY:     Look for a multiplication operator followed by a
 *               factor, followed by more of a term.
 *******************************************************************/
void termp
  (
  void
  )
{
  if ( lookahead == TOKEN_MULOP ) {
    match( TOKEN_MULOP );
    factor();
    termp();
  }
} /* termp */

/********************************************************************
 *                 factor
 ********************************************************************
 * SYNOPSIS:     Parse a factor.
 * RETURNS:      void
 * STRATEGY:     Look for an identifier or a number, or an opening
 *               parenthesis, or a not operator.
 *******************************************************************/
void factor
  (
  void
  )
{
  if ( lookahead == TOKEN_ID ) {
    match( TOKEN_ID );
    factorp();
  }
  else if ( lookahead == TOKEN_NUM ) {
    match( TOKEN_NUM );
  }
  else if ( lookahead == TOKEN_PAREN_OPEN ) {
    match( TOKEN_PAREN_OPEN );
    expression();
    if ( match( TOKEN_PAREN_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ) after expression\n", curLine );
    }
  }
  else if ( lookahead == TOKEN_KEY_NOT ) {
    match( TOKEN_KEY_NOT );
    factor();
  }
  else {
    printf( "%d SYNTAX ERROR: Invalid factor in expression\n", curLine );
  }
} /* factor */

/********************************************************************
 *                 factorp
 ********************************************************************
 * SYNOPSIS:     Parse the rest of a factor.
 * RETURNS:      void
 * STRATEGY:     Look for an opening parenthesis followed by an
 *               expression list, followed by a closing parenthesis.
 *******************************************************************/
void factorp
  (
  void
  )
{
  if ( lookahead == TOKEN_PAREN_OPEN ) {
    match( TOKEN_PAREN_OPEN );
    expression_list();
    if ( match( TOKEN_PAREN_CLOSE ) ) {
      printf( "%d SYNTAX ERROR: Missing ) after expression\n", curLine );
    }
  }
} /* factorp */

/********************************************************************
 *                 sign
 ********************************************************************
 * SYNOPSIS:     Parse a sign.
 * RETURNS:      void
 * STRATEGY:     Look for an addition operator (+ or -).
 *******************************************************************/
void sign
  (
  void
  )
{
  if ( lookahead == TOKEN_ADDOP ) {
    match( TOKEN_ADDOP );
  }
  else {
    printf( "%d SYNTAX ERROR: Expecting a + or - in expression\n",
            curLine );
  }
} /* sign */

