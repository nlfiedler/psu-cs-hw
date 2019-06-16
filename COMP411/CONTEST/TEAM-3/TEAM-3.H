/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 411 Contest Project
 * MODULE:       Zero-sum Subsequence Header
 * FILE:         team-3.h
 *
 * AUTHORS:      Thomas Denn
 *               Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/10/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the team-3.c program. It
 *      contains the included headers, constants, type
 *      definitions, and function prototypes.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Maximum number of elements our array can handle. */
    #define MAX_ELEMENTS 32000

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Type for the numbers in the array. */
    typedef int numberType;

       /* The array that holds the numbers. */
    typedef numberType *arrayType;

       /* Type result of the maximum sum. */
    typedef long int sumType;

       /* Index into the array of numbers. */
    typedef unsigned int indexType;

       /* Index into the array of numbers. */
    typedef long int returnType;

/********************************************************************
 *                 Macros
 *******************************************************************/
       /* Create the returnType from two numbers. */
    #define MAKE_RANGE( l, r ) \
              ((returnType)(( (returnType)(l) << 16 ) + (r) ))
       /* Extract the left or right boundary from the
        * return type of ZeroSumSeq. */
    #define LEFT( val )  ((unsigned)( (val) >> 16    ))
    #define RIGHT( val ) ((unsigned)( (val) & 0xffff ))

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Finds the maximum subsequence in an array. */
    returnType ZeroSumSeq
      (
      arrayType   a, /* The array to examine. (in) */
      indexType   n  /* The number of elements. (in) */
      );

