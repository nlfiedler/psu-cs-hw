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
 *       NF      02/18/96  Converted to test lex output
 *
 * DESCRIPTION:
 *      This is the testwrapper for the code that lex creates
 *      using the mylex.l file.
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
    #include "lex.h"

/********************************************************************
 *                 Function Definitions
 *******************************************************************/

/********************************************************************
 *                 Global Variables
 *******************************************************************/
       /* Lexical value. */
    int  yylval;
       /* Text string containing current token. */
    char yytext[];
       /* Length of string in yytext. */
    int  yyleng;

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
      * Open the input file.
      * While not end of file,
      *   display each token and it's corresponding lexeme.
      *   If it's a bad token,
      *     display appropriate error message.
      *   Clear lexical value, since yylex doesn't, and we
      *     don't want to misled by old values.
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
  while ( ( curToken = yylex() ) != TOKEN_DONE ) {
    if ( curToken >= 0 ) {
      printf( "yylval: %d\ttoken: %d\tyytext: %s\n",
              yylval, curToken, yytext );
    }
    else {
      printf( "Not a valid token: %d\n", curToken );
    }
    yylval = 0;
  }
  SymTabFree();

     /*
      * Close the input file.
      */
  fclose( yyin );
  return( 0 );
} /* main */
