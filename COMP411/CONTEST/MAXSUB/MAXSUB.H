/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 411 Contest Project
 * MODULE:       Maximum Subsequence Header
 * FILE:         maxsub.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/10/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the maxsub.c program. It
 *      contains the included headers, constants, type
 *      definitions, and function prototypes.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <alloc.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* These define the boolean values. */
    #define TRUE  -1
    #define FALSE  0

       /* Maximum number of elements our array can handle. */
    #define MAX_ELEMENTS 10000

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Boolean data type. */
    typedef char boolean;

       /* Type for the numbers in the array. */
    typedef int numberType;

       /* The array that holds the numbers. */
    typedef numberType *arrayType;

       /* Type result of the maximum sum. */
    typedef long int sumType;

       /* Index into the array of numbers. */
    typedef unsigned int indexType;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Finds the maximum subsequence in an array. */
    sumType MaxSubSeq
      (
      arrayType a,
      indexType n,
      indexType base_p,
      indexType * i,
      indexType * j
      );

