/********************************************************************
 *
 *  Copyright (C) Blue Marsh Softworks 1995 -- All rights reserved
 *
 * PROJECT:      COMP 418 Compiler
 * MODULE:       Symbol Table Manager Header
 * FILE:         symtab.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      01/24/96  Initial version
 *
 * DESCRIPTION:
 *      Contains the constants, data types, and function prototypes
 *      for the symbol table manager module.
 *
 * NAMING CONVENTION:
 *      Data types:
 *        _p is for parameter variables
 *        _g is for global variables
 *      Constants:
 *        First comes the general classification of the constant.
 *        For instance, constants related to lexemes all start with
 *        LEXEME_.
 *        Constants are all uppercase.
 *      Functions:
 *        Named as they are in the specifications, using the
 *        case that was used in the printed specs.
 *        Functions not listed in the specs are named first with
 *        the general class, such as functions dealing with symbols
 *        will start with Symbol (eg SymbolCreate, or SymbolFind).
 *
 *******************************************************************/

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Lexeme constants. */
    #define LEXEME_MAX_LENGTH 20

       /* Symbol constants. */
    #define SYMBOL_MAX_NUMBER 100

       /* General error constants.
        * Zero indicates no error. */
    #define MEMORY_RUNNING_LOW    -1

       /* Symbol table error constants.
        * Zero indicates no error. */
    #define SYMBOL_TABLE_OVERFLOW -2
    #define SYMBOL_TOO_LONG       -3
    #define SYMBOL_NOT_FOUND      -4

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Type for lexeme name (string).
        * Add one for null terminator. */
    typedef char Lexeme[LEXEME_MAX_LENGTH + 1];

       /* Type for lexeme token value. */
    typedef int Token;

       /* Type for index into symbol table. */
    typedef int Index;

       /* Symbol type, which is a combination of lexeme and token. */
    typedef struct {
      Lexeme S_lexeme;
      Token  S_token;
    } Symbol;

       /* Symbol table type. */
    typedef Symbol * SymbolTable;

/********************************************************************
 *                 Function Prototypes and Macros
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
      );

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
      Lexeme, /* Lexeme name to insert (in). */
      Token   /* Value associated with lexeme (in). */
      );

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
      Lexeme /* Lexeme to find (in). */
      );

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
      Index,   /* Symbol to get (in). */
      Symbol * /* Return value (out). */
      );

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
      Index index_p /* Symbol to delete (in). */
      );

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
      );

    /********************************************************************
     *                 get_token
     ********************************************************************
     * SYNOPSIS:     Return the token for the symbol in the indexed
     *               table entry.
     * RETURNS:      Token - token from table
     * STRATEGY:     Using the passed index, return the token from
     *               that entry in the symbol table.
     *******************************************************************/
    Token get_token
      (
      Index index_p /* Index to entry to get token for. (in) */
      );

