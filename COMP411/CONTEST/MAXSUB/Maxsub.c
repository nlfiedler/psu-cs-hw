/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 411 Contest Project
 * MODULE:       Maximum Subsequence
 * FILE:         maxsub.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/10/95  Initial version
 *
 * DESCRIPTION:
 *      Find the maximum subsequence in a sequence of positive
 *      and negative integers.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "maxsub.h"

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Finds the maximum
 *               subsequence in a list of integers.
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * STRATEGY:     Open the input file, then allocate an array for
 *               the largest size we can use. Then read in the
 *               elements and call MaxSubSeq on them. When all the
 *               arrays have been processed via the while loop,
 *               we will have a final answer.
 *******************************************************************/
int main
  (
  int    argc,  /* Number of arguments. */
  char * argv[] /* The agruments as strings. */
  )
{
     /* Variables for accessing the files. */
  FILE * hInputFile;
  char   achNumber[81];
     /* Variables concerning the array. */
  indexType totalElements;
  indexType numToRead;
  indexType i;
  indexType base;
  arrayType array;
     /* Maximum subsequence found so far. */
  sumType   maxSum;
  indexType BestI;
  indexType BestJ;

     /*
      * Check if user invoked program properly.
      * If not, output message.
      * Else, run program.
      */
  if ( argc <= 1 ) {
    printf( "MaxSub - Finds maximum subsequence.\n" );
    printf( "Usage: maxsub.exe DATAFILE.EXT\n" );
    exit( 1 );
  }
  if ( argc == 2 ) {
       /*
        * Initialize variables.
        * Open the input file, get number of elements to process.
        * Allocate an array for largest chunk we can handle.
        */
    base = 0;
    maxSum = 0;
    if (( hInputFile = fopen( argv[1], "rt" )) == NULL ) {
      printf( "Error opening input file %s!\n", argv[1] );
      exit( 1 );
    }  /* XXX - Change this to fscanf */
    fgets( achNumber, 80, hInputFile );
    totalElements = atoi( achNumber );
    array = calloc( MAX_ELEMENTS, sizeof( numberType ));
    if ( array == NULL ) {
      printf( "Could not allocate memory for array!\n" );
      exit( 1 );
    }
       /*
        * While there are elements to process:
        *   Read in the numbers to the subarray.
        *   Run MaxSubSeq on the subarray, which will give back
        *   the best subsequence at that stage of the search.
        *   Decrement number of elements to process.
        */
    while ( totalElements > 0 ) {
      if ( totalElements > MAX_ELEMENTS ) {
        numToRead = MAX_ELEMENTS;
      }
      else {
        numToRead = totalElements;
      }
      for ( i = 0; i < numToRead; i++ ) {
           /* XXX - Use fscanf */
        fgets( achNumber, 80, hInputFile );
        array[ i ] = atoi( achNumber );
      }
      maxSum = MaxSubSeq( array, numToRead, base, &BestI, &BestJ );
      totalElements -= numToRead;
      base += numToRead;
    } /* while */
  }

     /*
      * Free the array.
      * Output the results.
      * Return to DOS.
      */
  free( array );
  printf( "Range: %u, %u\n", BestI, BestJ );
  printf( "Maximum sum: %ld\n", maxSum );
  return( 0 );
} /* main */

/********************************************************************
 *                 MaxSubSeq
 ********************************************************************
 * SYNOPSIS:     Finds the maximum subsequence in an array of
 *               integers.
 * PARAMETERS:   ( arrayType   a,
 *                 indexType   n,
 *                 indexType   base_p,
 *                 indexType * i,
 *                 indexType * j )
 * RETURNS:      unsigned long int - Maximum subsequence sum.
 *               Will be zero if the array is all negative integers.
 * STRATEGY:     Uses Algorithm 4 from the book Data Structures and
 *               Algorithm Analysis by Mark Allen Weiss (p29).
 *******************************************************************/
sumType MaxSubSeq
  (
  arrayType   a,      /* The array to examine. */
  indexType   n,      /* The number of elements in the array. */
  indexType   base_p, /* The logical base offset to the array. */
  indexType * i_p,    /* The start of the best sequence. */
  indexType * j_p     /* The end of the best sequence. */
  )
{
     /* Maximum sums found so far. */
  static sumType MaxSum = 0;
  static sumType ThisSum = 0;
     /* The maximum sum subsequence so far. */
  static indexType BestI = 0;
  static indexType BestJ = 0;
     /* Loop indices. */
  static indexType i = 0;
  indexType j;

     /*
      * Run through the array, keeping track of the largest
      * sum found so far. If the sum ever goes negative, then
      * reset it to zero.
      */
  for ( j = base_p; j < n + base_p; j++ ) {
    ThisSum += a[ j - base_p ];
    if ( ThisSum > MaxSum ) {
      MaxSum = ThisSum;
      BestI = i;
      BestJ = j;
    }
    else {
      if ( ThisSum < 0 ) {
        i = j + 1;
        ThisSum = 0;
      }
    }
  } /* for */

     /*
      * Return largest sum found.
      */
  *i_p = BestI + 1;
  *j_p = BestJ + 1;
  return( MaxSum );
} /* MaxSubSeq */

