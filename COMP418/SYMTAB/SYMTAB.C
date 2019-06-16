/********************************************************************
 *
 *  Copyright (C) Blue Marsh Softworks 1995 -- All rights reserved
 *
 * PROJECT:      COMP 418 Compiler
 * MODULE:       Symbol Table Manager
 * FILE:         symtab.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      01/24/96  Initial version
 *
 * DESCRIPTION:
 *      The symbol table manager handles insertion and lookup
 *      operations on any kind of symbols that are stored in
 *      the table.
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
    #include <stdlib.h>
    #include <string.h>
    #include "symtab.h"

/********************************************************************
 *                 Global Variables
 *******************************************************************/
       /* The symbol table. */
    SymbolTable symbolTable_g;

       /* Number of symbols in table.
        * Zero when table is empty */
    Index symbolCount_g;

/********************************************************************
 *                 Function Defintions
 *******************************************************************/

/********************************************************************
 *                 SymTabInitialize
 ********************************************************************
 * SYNOPSIS:     Initialize the symbol table.
 * RETURNS:      Error code, if any.
 * STRATEGY:     Set the symbol count to zero, to indicate the
 *               table is empty. Allocate a table for the maximum
 *               number of symbols.
 *******************************************************************/
int SymTabInitialize
  (
  void
  )
{

     /*
      * Set symbol count to zero, then allocate for the
      * maximum number of symbols. If memory can't be allocated,
      * report appropriate error. Else return 0 for no error.
      */
  symbolCount_g = 0;
  symbolTable_g = calloc( SYMBOL_MAX_NUMBER, sizeof( Symbol ) );
  if ( symbolTable_g == NULL ) {
    return( MEMORY_RUNNING_LOW );
  }
  else {
    return( 0 );
  }
} /* SymTabInitialize */

/********************************************************************
 *                 insert
 ********************************************************************
 * SYNOPSIS:     Insert a new symbol into the table.
 * RETURNS:      Index to where symbol was stored. Will be negative
 *               if an error occurred. See error constants.
 * STRATEGY:     If the symbol count is less than the maximum allowed,
 *               then store the lexeme and value as a symbol in the
 *               symbol table. Increment the number of symbols.
 *               If the symbol count has reached the maximum, then
 *               return a INSERT_SYMBOL_TABLE_OVERFLOW.
 *               Returns INSERT_SYMBOL_TOO_LONG if symbol too long.
 *******************************************************************/
Index insert
  (
  lexeme_p, /* Lexeme name to insert (in). */
  token_p   /* Value associated with lexeme (in). */
  )
  Lexeme lexeme_p;
  Token  token_p;
{
     /* Used to make new symbol before inserting into table. */
  Symbol newSymbol;

     /*
      * Check length of lexeme.
      *   If okay, check  if symbol table overflowed.
      *     If okay, create new symbol from parameters and store
      *       at current position in table.
      *     Else, return appropriate error.
      *   Else, return appropriate error.
      */
  if ( strlen( lexeme_p ) <= LEXEME_MAX_LENGTH ) {
    if ( symbolCount_g < SYMBOL_MAX_NUMBER ) {
      strcpy( newSymbol.S_lexeme, lexeme_p );
      newSymbol.S_token = token_p;
      symbolTable_g[symbolCount_g] = newSymbol;
      symbolCount_g++;
      return( symbolCount_g - 1 );
    }
    else {
      return( SYMBOL_TABLE_OVERFLOW );
    }
  }
  else {
    return( SYMBOL_TOO_LONG );
  }
} /* insert */

/********************************************************************
 *                 lookup
 ********************************************************************
 * SYNOPSIS:     Lookup a token for a particular lexeme.
 * RETURNS:      Index to where symbol was found. Will be -1
 *               if symbol not in table.
 * STRATEGY:     Starting at the end of the list, compare each
 *               symbol to the lexeme. When the first match is
 *               found, return it's index into the table.
 *               If the beginning is reached, then return -1.
 * ASSUMES:      That most recent copy of a lexeme will be after
 *               the first occurrence.
 *******************************************************************/
Index lookup
  (
  lexeme_p /* Lexeme to find (in). */
  )
  Lexeme lexeme_p;
{
  Index curSymbol;

  if ( strlen( lexeme_p ) <= LEXEME_MAX_LENGTH ) {
    curSymbol = symbolCount_g - 1;
    while (( curSymbol >= 0 ) &&
           ( strcmp( symbolTable_g[curSymbol].S_lexeme,
                     lexeme_p ) != 0 )) {
      curSymbol--;
    }
    if ( curSymbol >= 0 ) {
      return( curSymbol );
    }
    else {
      return( SYMBOL_NOT_FOUND );
    }
  }
  else {
    return( SYMBOL_TOO_LONG );
  }
} /* lookup */

/********************************************************************
 *                 SymTabGetSymbol
 ********************************************************************
 * SYNOPSIS:     Return the contents of the symbol table entry.
 * RETURNS:      Error code, if any.
 * STRATEGY:     Check the index for validity, then return the
 *               contents of that entry in the parameter.
 *******************************************************************/
int SymTabGetSymbol
  (
  index_p, /* Symbol to get (in). */
  sym_p    /* Return value (out). */
  )
  Index    index_p;
  Symbol * sym_p;
{

  if (( index_p >= 0 ) && ( index_p < symbolCount_g )) {
    strcpy( sym_p->S_lexeme, symbolTable_g[index_p].S_lexeme );
    sym_p->S_token = symbolTable_g[index_p].S_token;
    return( 0 );
  }
  else {
    return( SYMBOL_NOT_FOUND );
  }
} /* SymTabGetSymbol */

/********************************************************************
 *                 SymTabDeleteSymbol
 ********************************************************************
 * SYNOPSIS:     Removes the specified symbol from the table.
 * RETURNS:      Error code, if any.
 * STRATEGY:     Check the index for validity, then removes the
 *               symbol by shifting all others up towards the top.
 *               Decrement the symbol count.
 *******************************************************************/
int SymTabDeleteSymbol
  (
  index_p /* Symbol to delete (in). */
  )
  Index index_p;
{
     /* Used to move symbols after index_p up towards top. */
  Index shiftSym;

     /*
      * If valid index, start shifting all symbols after the
      * deleted symbol up one, ending at the last symbol.
      * Clear out the extra copy of the last symbol.
      * Decrement the number of symbols in the table.
      */
  if (( index_p >= 0 ) && ( index_p < symbolCount_g )) {
    for ( shiftSym = index_p + 1;
          shiftSym < symbolCount_g;
          shiftSym++ ) {
      symbolTable_g[shiftSym - 1] = symbolTable_g[shiftSym];
    }
    symbolTable_g[shiftSym].S_lexeme[0] = '\0';
    symbolCount_g--;
    return( 0 );
  }
  else {
    return( SYMBOL_NOT_FOUND );
  }
} /* SymTabDeleteSymbol */

/********************************************************************
 *                 SymTabFree
 ********************************************************************
 * SYNOPSIS:     Free the symbol table.
 * STRATEGY:     Free the symbol table that was allocated in
 *               SymTabInitialize.
 *******************************************************************/
void SymTabFree
  (
  void
  )
{

  free( symbolTable_g );
} /* SymTabFree */

