/********************************************************************
 *
 * Copyright (C) 1996 Blue Marsh Softworks - All rights reserved
 *
 * PROEJCT:      COMP 418 Compiler Design
 * MODULE:       Lexical Analyzer
 * FILE:         lex.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      02/06/96  Initial version
 *
 * DESCRIPTION:
 *      This is a lexical analyzer for the compiler. It finds and
 *      tokenizes all valid strings in the source file.
 *      Any errors encountered are displayed to stdout. It uses
 *      a panic mode error recovery, in which any out-of-place
 *      characters are stuffed back into the input stream and
 *      will be prepended to the next token.
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
    #include "mylex.h"

/********************************************************************
 *                 Global Variables
 *******************************************************************/
       /* Input file. */
    FILE * yyin;
       /* String containing the current identifier, keyword,
        * or number. */
    char yytext[LEXEME_MAX_LENGTH * 2];
       /* Lexical value of token, such as EQ or the index
        * into the symbol table. */
    int yylval;

/********************************************************************
 *                 Function Definitions
 *******************************************************************/

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
     /* Current token being processed. */
  Token curToken;

     /*
      * Check command-line arguments. Display message
      * if incorrect number of arguments.
      * Display message if /? is entered (standard
      * DOS feature).
      */
  if ( ( argc == 1 ) ||
       ( ( argc == 2 ) && ( strcmp( argv[1], "/?" ) == 0 ) ) ) {
    printf( "Lex program for COMP 418, Spring '96\n" );
    printf( "Usage: lex FILENAME.EXT\n" );
    return( 0 );
  }

     /*
      * Start symbol table.
      * Insert all the keywords.
      * Open the input file.
      * While not end of file,
      *   display each token and it's corresponding lexeme.
      *   If it's a bad token,
      *     display appropriate error message.
      * Free symbol table.
      */
  if ( SymTabInitialize() ) {
    printf( "ERROR: Low on memory.\n" );
    exit( 1 );
  }
  LoadKeyWords();
  yyin = fopen( argv[1], "rt" );
  if ( yyin == NULL ) {
    printf( "Error opening file %s!", argv[1] );
    exit( 1 );
  }
  while ( ( curToken = yylex() ) != TOKEN_DONE ) {
    if ( curToken >= 0 ) {
      printf( "yylval: %d curToken: %d yytext: %s\n",
              yylval, curToken, yytext );
    }
    else {
      PrintError( curToken );
    }
  }
  SymTabFree();

     /*
      * Close the input file.
      */
  fclose( yyin );
  return( 0 );
} /* main */

/********************************************************************
 *                 yylex
 ********************************************************************
 * SYNOPSIS:     Return the next token from the input stream.
 * RETURNS:      Token - the next token, 0 if a keyword, < 0 if
 *               an error (see lex.h for values).
 * STRATEGY:     A BIG case statement. Each case represents a state
 *               in the transition diagram. See diagram.
 *******************************************************************/
Token yylex
  (
  void
  )
{
     /* The current state of the lexical analyzer.
      * Determines what to do next. */
  int state;
     /* Current character from stream. */
  char c;

     /*
      * Start state at beginning of diagram.
      * Loop until a case returns.
      */
  state = 0;
  while ( 1 ) {
    switch ( state ) {
         /*
          * Flush the text buffer, yytext.
          * Set lexical value to zero.
          * This gets us into the right state.
          * Get the next character and begin switching.
          * Return TOKEN_DONE when end of file reached.
          */
      case 0 :
        flushtext();
        yylval = 0;
        c = nextchar();
        if ( c == EOF ) {
          return( TOKEN_DONE );
        }
        if ( isspace( c ) ) {
          state = 1;
        }
        else if ( isalpha( c ) ) state = 3;
        else if ( isdigit( c ) ) state = 5;
        else if ( c == '<' ) state = 14;
        else if ( c == '=' ) state = 18;
        else if ( c == '>' ) state = 19;
        else if ( c == '{' ) state = 22;
        else if ( c == '.' ) state = 24;
        else if ( c == ',' ) state = 27;
        else if ( c == '/' ) state = 28;
        else if ( c == ';' ) state = 29;
        else if ( c == '(' ) state = 30;
        else if ( c == ')' ) state = 31;
        else if ( c == '[' ) state = 32;
        else if ( c == ']' ) state = 33;
        else if ( c == '*' ) state = 34;
        else if ( c == '+' ) state = 35;
        else if ( c == '-' ) state = 36;
        else if ( c == ':' ) state = 37;
        break;
         /* Middle of whitespace. */
      case 1 :
        while ( isspace( c = nextchar() ) ) { }
        state = 2;
        break;
         /* End of whitespace. */
      case 2 :
        retract( c );
        state = 0;
        break;

         /* Start of identifier. */
      case 3 :
        while ( isalpha( c ) || isdigit( c ) ) {
          c = nextchar();
        }
        state = 4;
        break;
         /* End of identifier. */
      case 4 :
        retract( c );
        yylval = install_id();
        return( get_token( yylval ) );

         /* Start of a number. */
      case 5 :
        while ( isdigit( c = nextchar() ) ) { }
        if ( c == '.' ) {
          state = 7;
        }
        else {
          if ( toupper( c ) == 'E' ) {
            state = 10;
          }
          else {
            state = 6;
          }
        }
        break;
         /* End of number. */
      case 6 :
        retract( c );
        yylval = install_num();
        return( TOKEN_NUM );
         /* Start of real fraction. */
      case 7 :
        c = nextchar();
        if ( isdigit( c ) == 0 ) {
          retract( c );
          return( LEX_BAD_REAL );
        }
        else {
          state = 8;
        }
        break;
         /* Fraction of real number. */
      case 8 :
        while ( isdigit( c = nextchar() ) ) { }
        if ( toupper( c ) == 'E' ) {
          state = 10;
        }
        else {
          state = 9;
        }
        break;
         /* End of real number. */
      case 9 :
        retract( c );
        yylval = install_num();
        return( TOKEN_NUM );
         /* Start of exponent to real. */
      case 10 :
        c = nextchar();
        if ( ( c == '+' ) || ( c == '-' ) ) {
          state = 11;
        }
        else {
          if ( isdigit( c ) ) {
            state = 12;
          }
          else {
            retract( c );
            return( LEX_BAD_EXPONENT );
          }
        }
        break;
         /* Rest of exponent. */
      case 11 :
        if ( isdigit( c = nextchar() ) ) {
          state = 12;
        }
        else {
          retract( c );
          return( LEX_BAD_EXPONENT );
        }
        break;
         /* Digits of exponent. */
      case 12 :
        while ( isdigit( c = nextchar() ) ) { }
        state = 13;
        break;
         /* End of exponent of real number. */
      case 13 :
        retract( c );
        yylval = install_num();
        return( TOKEN_NUM );

         /* A < symbol. */
      case 14 :
        c = nextchar();
        if ( c == '=' ) {
          state = 15;
        }
        else {
          if ( c == '>' ) {
            state = 16;
          }
          else {
            state = 17;
          }
        }
        break;
         /* It's a <= */
      case 15 :
        yylval = LVAL_LE;
        return( TOKEN_RELOP );
         /* It's a <> */
      case 16 :
        yylval = LVAL_NE;
        return( TOKEN_RELOP );
         /* It's a < only. */
      case 17 :
        retract( c );
        yylval = LVAL_LT;
        return( TOKEN_RELOP );

         /* It's an = */
      case 18 :
        yylval = LVAL_EQ;
        return( TOKEN_RELOP );

         /* It's a > */
      case 19 :
        c = nextchar();
        if ( c == '=' ) {
          state = 20;
        }
        else {
          state = 21;
        }
        break;
         /* It's a <= */
      case 20 :
        yylval = LVAL_GE;
        return( TOKEN_RELOP );
         /* It's just a > */
      case 21 :
        retract( c );
        yylval = LVAL_GT;
        return( TOKEN_RELOP );

         /* It's a comment. */
      case 22 :
        while ( ( c = nextchar() ) != '}' ) {
          if ( c == '{' ) {
            return( LEX_BAD_COMMENT );
          }
        }
        state = 23;
        break;
         /* End of comment. */
      case 23 :
        state = 0;
        break;

         /* It's a . */
      case 24 :
        c = nextchar();
        if ( c == '.' ) {
          state = 25;
        }
        else {
          state = 26;
        }
        break;
         /* It's an array range. */
      case 25 :
        yylval = 0;
        return( TOKEN_DOTDOT );
         /* It's just a . */
      case 26 :
        retract( c );
        yylval = 0;
        return( TOKEN_PERIOD );

         /* It's a , */
      case 27 :
        yylval = 0;
        return( TOKEN_COMMA );

         /* It's a / */
      case 28 :
        yylval = LVAL_DIV_REAL;
        return( TOKEN_MULOP );

         /* It's a ; */
      case 29 :
        yylval = 0;
        return( TOKEN_SEMICOLON );

         /* It's a ( */
      case 30 :
        yylval = 0;
        return( TOKEN_PAREN_OPEN );

         /* It's a ) */
      case 31 :
        yylval = 0;
        return( TOKEN_PAREN_CLOSE );

         /* It's a [ */
      case 32 :
        yylval = 0;
        return( TOKEN_BRACKET_OPEN );

         /* It's a ] */
      case 33 :
        yylval = 0;
        return( TOKEN_BRACKET_CLOSE );

         /* It's a * */
      case 34 :
        yylval = LVAL_MUL;
        return( TOKEN_MULOP );

         /* It's a + */
      case 35 :
        yylval = LVAL_PLUS;
        return( TOKEN_ADDOP );

         /* It's a - */
      case 36 :
        yylval = LVAL_MINUS;
        return( TOKEN_ADDOP );

         /* It's a : */
      case 37 :
        c = nextchar();
        if ( c == '=' ) {
          state = 38;
        }
        else {
          state = 39;
        }
        break;
         /* It's an := */
      case 38 :
        yylval = LVAL_ASSIGN;
        return( TOKEN_ASSIGNOP );
         /* It's just a : */
      case 39 :
        retract( c );
        yylval = 0;
        return( TOKEN_COLON );

         /* Error, not a valid state. */
      default :
        printf( "ERROR: Unknown state in yylex() %d.\n", state );
        while ( !isspace( c = nextchar() ) ) { }
        state = 0;
        break;
    } /* switch */
  } /* while */
} /* yylex */

/********************************************************************
 *                 nextchar
 ********************************************************************
 * SYNOPSIS:     Get the next character from the stream.
 * RETURNS:      char - the next character
 * STRATEGY:     Call fgetc and put character in yytext. Then
 *               return the character. If it's an EOF, then don't
 *               do anything to yytext.
 *******************************************************************/
char nextchar
  (
  void
  )
{
  char ch;

  ch = fgetc( yyin );
  if ( ch != EOF ) {
    yytext[ strlen( yytext ) ] = ch;
    yytext[ strlen( yytext ) + 1 ] = '\0';
  }
  return( ch );
} /* nextchar */

/********************************************************************
 *                 retract
 ********************************************************************
 * SYNOPSIS:     Put the unwanted character back on the input stream.
 * STRATEGY:     Call ungetc and remove the last character from
 *               the text buffer.
 *******************************************************************/
void retract
  (
  char ch_p /* Character to put back into stream. (in) */
  )
{

  ungetc( ch_p, yyin );
  yytext[ strlen( yytext ) - 1 ] = '\0';
} /* retract */

/********************************************************************
 *                 flushtext
 ********************************************************************
 * SYNOPSIS:     Delete the current text buffer.
 * STRATEGY:     Set all characters of yytext to null.
 *******************************************************************/
void flushtext
  (
  void
  )
{
     /* Used to clear out yytext. */
  int i;

     /*
      * For length of string,
      *   set each character to null.
      */
  for ( i = strlen( yytext ); i >= 0; i-- ) {
    yytext[ i ] = '\0';
  }
} /* flushtext */

/********************************************************************
 *                 install_id
 ********************************************************************
 * SYNOPSIS:     Insert the current lexeme and token into the
 *               symbol table.
 * RETURNS:      Index - index into symbol table, 0 if keyword.
 * STRATEGY:     See if the symbol is already in the table. If so,
 *               return either it's index, or 0 if it's a keyword.
 *******************************************************************/
Index install_id
  (
  void
  )
{
  Index  index;

  index = lookup( yytext );
  if ( index == SYMBOL_NOT_FOUND ) {
    return( insert( yytext, TOKEN_ID ) );
  }
  else {
    return( index );
  }
} /* install_id */

/********************************************************************
 *                 install_num
 ********************************************************************
 * SYNOPSIS:     Insert the current number and appropriate token
 *               into the symbol table.
 * RETURNS:      Index - index into symbol table.
 * STRATEGY:     See if the number is already there. If not,
 *               insert it and return index. If it's there,
 *               return the index.
 *******************************************************************/
Index install_num
  (
  void
  )
{
     /* Index to entry containing number. */
  Index index;

  index = lookup( yytext );
  if ( index == SYMBOL_NOT_FOUND ) {
    return( insert( yytext, TOKEN_NUM ) );
  }
  else {
    return( index );
  }
} /* install_num */

/********************************************************************
 *                 LoadKeyWords
 ********************************************************************
 * SYNOPSIS:     Loads the keywords into the symbol table.
 * STRATEGY:     Call insert a lot.
 *******************************************************************/
void LoadKeyWords
  (
  void
  )
{

     /*
      * Add addops and mulops. Add these first so the index in
      * the table will act as the attribute (lexical value).
      */
  insert( "or", TOKEN_ADDOP );
  insert( "and", TOKEN_MULOP );
  insert( "div", TOKEN_MULOP );
  insert( "mod", TOKEN_MULOP );

     /*
      * Add keywords.
      */
  insert( "program", TOKEN_KEY_PROGRAM );
  insert( "var", TOKEN_KEY_VAR );
  insert( "array", TOKEN_KEY_ARRAY );
  insert( "of", TOKEN_KEY_OF );
  insert( "integer", TOKEN_KEY_INTEGER );
  insert( "real", TOKEN_KEY_REAL );
  insert( "function", TOKEN_KEY_FUNCTION );
  insert( "procedure", TOKEN_KEY_PROCEDURE );
  insert( "begin", TOKEN_KEY_BEGIN );
  insert( "end", TOKEN_KEY_END );
  insert( "if", TOKEN_KEY_IF );
  insert( "then", TOKEN_KEY_THEN );
  insert( "else", TOKEN_KEY_ELSE );
  insert( "while", TOKEN_KEY_WHILE );
  insert( "do", TOKEN_KEY_DO );
  insert( "not", TOKEN_KEY_NOT );
} /* LoadKeyWords */

/********************************************************************
 *                 PrintError
 ********************************************************************
 * SYNOPSIS:     Displays the appropriate error message.
 * STRATEGY:     Do a switch statement, with a case for each
 *               error constant.
 *******************************************************************/
void PrintError
  (
  Token badToken_p /* Bad token error value. (in) */
  )
{

  switch ( badToken_p ) {
    case LEX_BAD_REAL :
      printf( "ERROR: Real number missing digits after \".\"\n" );
      break;
    case LEX_BAD_EXPONENT :
      printf( "ERROR: Exponent must have +,-, or digits after \"E\".\n" );
      break;
    case LEX_BAD_COMMENT :
      printf( "ERROR: Can't have \"{\" inside comment.\n" );
      break;
    default :
      printf( "ERROR: Unknown error value.\n" );
      break;
  } /* switch */
} /* PrintError */

