/********************************************************************
 *
 * Copyright (C) 1996 Blue Marsh Softworks - All rights reserved
 *
 * PROEJCT:      COMP 418 Compiler Design
 * MODULE:       Lexical Analyzer Header
 * FILE:         lex.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      02/06/96  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the lexical analyzer.
 *
 * NAMING CONVENTION:
 *      Data types:
 *        _p is for parameter variables
 *        _g is for global variables (unless specs say otherwise)
 *      Constants:
 *        The first part is the general classification of the constant.
 *        For instance, constants related to lexemes all start with
 *        LEXEME_
 *        Constants are all uppercase.
 *      Functions:
 *        Named as they are in the specifications, using the
 *        case that was used in the printed specs.
 *        Functions not listed in the specs are named first with
 *        the general class; for example, functions dealing with symbols
 *        will start with Symbol (eg SymbolCreate, or SymbolFind).
 *
 *******************************************************************/

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Tokens */
    #define TOKEN_DONE          0
    #define TOKEN_ID            258
    #define TOKEN_NUM           259
    #define TOKEN_RELOP         260
    #define TOKEN_MULOP         261
    #define TOKEN_ADDOP         262
    #define TOKEN_ASSIGNOP      263
    #define TOKEN_KEY_PROGRAM   264
    #define TOKEN_KEY_VAR       265
    #define TOKEN_KEY_ARRAY     266
    #define TOKEN_KEY_OF        267
    #define TOKEN_KEY_INTEGER   268
    #define TOKEN_KEY_REAL      269
    #define TOKEN_KEY_FUNCTION  270
    #define TOKEN_KEY_PROCEDURE 271
    #define TOKEN_KEY_BEGIN     272
    #define TOKEN_KEY_END       273
    #define TOKEN_KEY_IF        274
    #define TOKEN_KEY_THEN      275
    #define TOKEN_KEY_ELSE      276
    #define TOKEN_KEY_WHILE     277
    #define TOKEN_KEY_DO        278
    #define TOKEN_KEY_NOT       279
    #define TOKEN_DOTDOT        280
    #define TOKEN_PERIOD        281
    #define TOKEN_COMMA         282
    #define TOKEN_SEMICOLON     283
    #define TOKEN_PAREN_OPEN    284
    #define TOKEN_PAREN_CLOSE   285
    #define TOKEN_BRACKET_OPEN  286
    #define TOKEN_BRACKET_CLOSE 287
    #define TOKEN_COLON         288

       /* Lexical values */
    #define LVAL_LE            101
    #define LVAL_NE            102
    #define LVAL_LT            103
    #define LVAL_EQ            104
    #define LVAL_GE            105
    #define LVAL_GT            106
    #define LVAL_DIV_REAL      107
    #define LVAL_MUL           108
    #define LVAL_PLUS          109
    #define LVAL_MINUS         110
    #define LVAL_ASSIGN        111
    #define LVAL_AND           112
    #define LVAL_OR            113
    #define LVAL_DIV           114
    #define LVAL_MOD           115

       /* Error constants */
    #define LEX_BAD_REAL             -1
    #define LEX_BAD_EXPONENT         -2
    #define LEX_BAD_COMMENT          -3

/********************************************************************
 *                 External Variables
 *******************************************************************/
       /* Input file that main() will open and yylex uses. */
    extern FILE * yyin;
       /* Lexical value, defined in lex.c, using in lex.yy.c */
    extern int yylval;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/

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
      );

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
      );

    void LoadKeyWords
      (
      void
      );
