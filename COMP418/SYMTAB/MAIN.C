/********************************************************************
 *
 *  Copyright (C) Blue Marsh Softworks 1995 -- All rights reserved
 *
 * PROJECT:      COMP 418 Compiler
 * MODULE:       Symbol Table Testwrapper
 * FILE:         main.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      01/25/96  Initial version
 *
 * DESCRIPTION:
 *      This tests the symbol table manager.
 *
 * ERROR HANDLING:
 *      When a function is passed an illegal parameter, it will do
 *      one of two things:
 *      1. Provide a default value in it's place, where applicable.
 *      2. Flag the error in the return value.
 *
 *******************************************************************/

/********************************************************************
 *                 Header Files
 *******************************************************************/
    #include <stdio.h>
    #include "symtab.h"

/********************************************************************
 *                 Global Variables for main
 *******************************************************************/

/********************************************************************
 *                 Function Defintions
 *******************************************************************/

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Testwrapper for symbol table.
 * RETURNS:      Error code, if any.
 * STRATEGY:     Insert five entries, two of which have the same
 *               lexeme. Lookup each entry and print it out.
 *******************************************************************/
int main
  (
  void
  )
{
     /* Used to test if symbol was stored alright. */
  Symbol testSym;
  Index  testIndex;

     /* Create symbol table. */
  if ( SymTabInitialize() ) {
    printf( "Error initializing the symbol table in main().\n" );
  }

     /* Insert some symbols. */
  insert( "lex2", 30 );
  insert( "lex3", 4 );
  insert( "lex4", 10000 );
  insert( "lex1", 4 );
  insert( "lex1", 500 );

     /* Look up the symbols and print them out. */
  testIndex = lookup( "lex1" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

  testIndex = lookup( "lex1" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

  testIndex = lookup( "lex2" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

  testIndex = lookup( "lex3" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

  testIndex = lookup( "lex4" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

     /*
      * Remove one of the duplicate symbols and
      * lookup the one that's left.
      */
  SymTabDeleteSymbol( lookup( "lex1" ) );
  testIndex = lookup( "lex1" );
  SymTabGetSymbol( testIndex, &testSym );
  printf( "%s %d\n", testSym.S_lexeme, testSym.S_token );

     /*
      * Free the symbol table and return to OS.
      */
  SymTabFree();
  return( 0 );
} /* main */

