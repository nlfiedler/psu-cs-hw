%{
#include "symtab.h"
#include "syncheck.h"
%}

delim  [ \t]
newln  [\n]
ws     {delim}+
letter [A-Za-z]
digit  [0-9]
id     {letter}({letter}|{digit})*
number {digit}+(\.{digit}+)?(E[+\-]?{digit}+)?
error1 {digit}+\.
error2 {digit}+(\.{digit}+)E
error3 \.{digit}+

%%

{ws}      { }
{newln}   { curLine++; }

error1    { printf( "Missing fraction on real number.\n" ); }
error2    { printf( "Missing exponent on real number.\n" ); }
error3    { printf( "Missing integer portion of real number.\n" ); }

program   { return( TOKEN_KEY_PROGRAM ); }
var       { return( TOKEN_KEY_VAR ); }
array     { return( TOKEN_KEY_ARRAY ); }
of        { return( TOKEN_KEY_OF ); }
integer   { return( TOKEN_KEY_INTEGER ); }
real      { return( TOKEN_KEY_REAL ); }
function  { return( TOKEN_KEY_FUNCTION ); }
procedure { return( TOKEN_KEY_PROCEDURE ); }
begin     { return( TOKEN_KEY_BEGIN ); }
end       { return( TOKEN_KEY_END ); }
if        { return( TOKEN_KEY_IF ); }
then      { return( TOKEN_KEY_THEN ); }
else      { return( TOKEN_KEY_ELSE ); }
while     { return( TOKEN_KEY_WHILE ); }
do        { return( TOKEN_KEY_DO ); }
not       { return( TOKEN_KEY_NOT ); }

or        { yylval = LVAL_OR;
            return( TOKEN_ADDOP ); }
and       { yylval = LVAL_AND;
            return( TOKEN_MULOP ); }
div       { yylval = LVAL_DIV;
            return( TOKEN_MULOP ); }
mod       { yylval = LVAL_MOD;
            return( TOKEN_MULOP ); }

{id}      { yylval = install_id();
            return( TOKEN_ID ); }
{number}  { yylval = install_num();
            return( TOKEN_NUM ); }

"<"       { yylval = LVAL_LT;
            return( TOKEN_RELOP ); }
"<="      { yylval = LVAL_LE;
            return( TOKEN_RELOP ); }
"="       { yylval = LVAL_EQ;
            return( TOKEN_RELOP ); }
"<>"      { yylval = LVAL_NE;
            return( TOKEN_RELOP ); }
">"       { yylval = LVAL_GT;
            return( TOKEN_RELOP ); }
">="      { yylval = LVAL_GE;
            return( TOKEN_RELOP ); }

"/"       { yylval = LVAL_DIV_REAL;
            return( TOKEN_MULOP ); }
"*"       { yylval = LVAL_MUL;
            return( TOKEN_MULOP ); }

"+"       { yylval = LVAL_PLUS;
            return( TOKEN_ADDOP ); }
"-"       { yylval = LVAL_MINUS;
            return( TOKEN_ADDOP ); }

"{"       { while ( input() != '}' ); }

"."       { return( TOKEN_PERIOD ); }
".."      { return( TOKEN_DOTDOT ); }
","       { return( TOKEN_COMMA ); }
";"       { return( TOKEN_SEMICOLON ); }
":"       { return( TOKEN_COLON ); }
":="      { return( TOKEN_ASSIGNOP ); }
"("       { return( TOKEN_PAREN_OPEN ); }
")"       { return( TOKEN_PAREN_CLOSE ); }
"["       { return( TOKEN_BRACKET_OPEN ); }
"]"       { return( TOKEN_BRACKET_CLOSE ); }
.         { printf( "Character not part of a token.\n" ); }

%%

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
 *                 yywrap
 ********************************************************************
 * SYNOPSIS:     Returns 1 when yylex reaches the end of the file.
 * RETURNS:      int - 1 no matter what
 * STRATEGY:     Return 1 no matter what.
 *******************************************************************/
int yywrap
  (
  void
  )
{
  return( 1 );
} /* yywrap */
