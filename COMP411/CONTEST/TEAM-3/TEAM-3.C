/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 411 Contest Project
 * MODULE:       Zero-Sum Subsequence
 * FILE:         team-3.c
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
 *      Find the largest zero-sum subsequence in a sequence of
 *      positive and negative integers. The sequence can be of
 *      any size.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "team-3.h"

/********************************************************************
 *                 Globals
 *******************************************************************/
       /* The temporary file containing the elements that
        * could not fit in the array. */
    FILE * tempFile;

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the hard
 *               work, such as file access and memory management.
 * PARAMETERS:   ( int    argc,
 *                 char * argv[] )
 * RETURNS:      int - Error condition
 * STRATEGY:     Open the input file, then allocate an array for
 *               the largest size we can use. Then read in the
 *               elements and call ZeroSumSeq on them. Then free
 *               the array and delete the temporary file. Output
 *               the results of the search to out-3.dat.
 *******************************************************************/
int main
  (
  int    argc,  /* Number of arguments. (in) */
  char * argv[] /* The agruments as strings. (in) */
  )
{
     /* Variables for accessing the files. */
  FILE *     hInputFile;
  FILE *     hOutFile;
  char       achNumber[81];
  numberType number;
     /* Variables concerning the array. */
  indexType totalElements;
  indexType numToRead;
  indexType i;
  arrayType array;
     /* Zero-sum subsequence range. */
  returnType Best;

     /*
      * Open the input file, get number of elements to process.
      * Allocate an array for largest chunk we can handle.
      */
  if (( hInputFile = fopen( argv[1], "rt" )) == NULL ) {
    printf( "Error opening input file %s!\n", argv[1] );
    exit( 1 );
  }
  fgets( achNumber, 80, hInputFile );
  totalElements = atoi( achNumber );
  array = calloc( MAX_ELEMENTS, sizeof( numberType ));
  if ( array == NULL ) {
    printf( "Could not allocate memory for array!\n" );
    fclose( hInputFile );
    exit( 1 );
  }

     /*
      * Read in up to MAX_ELEMENTS numbers to the array.
      * Read those numbers past MAX_ELEMENTS into a binary file.
      * Run ZeroSumSeq.
      */
  if ( totalElements > MAX_ELEMENTS ) {
    numToRead = MAX_ELEMENTS;
  }
  else {
    numToRead = totalElements;
  }
  for ( i = 0; i < numToRead; i++ ) {
    fgets( achNumber, 80, hInputFile );
    array[ i ] = atoi( achNumber );
  }
  if ( totalElements > MAX_ELEMENTS ) {
    totalElements -= MAX_ELEMENTS;
       /*
        * Create temporary file.
        * Read numbers from input file and write to binary file.
        */
    tempFile  = fopen( "team-3.tmp", "w+b" );
    for ( i = 0; i < totalElements; i++ ) {
      fgets( achNumber, 80, hInputFile );
      number = atoi( achNumber );
      fwrite( &number, sizeof( numberType ), 1, tempFile );
    }
    totalElements += MAX_ELEMENTS;
  }
  fclose( hInputFile );

     /*
      * Call the zero-sum finder function. (It's n*n, why
      * worry about overhead?)
      */
  Best = ZeroSumSeq( array, totalElements );

     /*
      * Free the array.
      * Delete the temporary file, without any concern
      * for whether it was there or not.
      * Output the results to file.
      */
  free( array );
  fclose( tempFile );
  remove( "team-3.tmp" );
  hOutFile = fopen( "out-3.dat", "wt" );
  if ( LEFT( Best ) == 50001U ) {
    fprintf( hOutFile, "-1\n\n" );
  }
  else {
    fprintf( hOutFile, "1\n" );
    fprintf( hOutFile, "%u\n", RIGHT( Best ) - LEFT( Best ) + 1 );
    fprintf( hOutFile, "%u %u\n\n", LEFT( Best ), RIGHT( Best ));
  }
  fclose( hOutFile );

     /*
      * Return to DOS.
      */
  return( 0 );
} /* main */

/********************************************************************
 *                 ZeroSumSeq
 ********************************************************************
 * SYNOPSIS:     Finds the zero-sum subsequence in an array of
 *               positive and negative integers.
 * PARAMETERS:   ( arrayType   a,
 *                 indexType   n )
 * RETURNS:      returnType - Range of the zero sum sequence.
 * STRATEGY:     Uses Algorithm 2 from the book Data Structures and
 *               Algorithm Analysis by Mark Allen Weiss (p29), with
 *               slight modifications so that it will look for
 *               subsequences that add up to zero, rather than the
 *               maximum sum subsequence.
 *******************************************************************/
returnType ZeroSumSeq
  (
  arrayType   a, /* The array to examine. (in) */
  indexType   n  /* The number of elements. (in) */
  )
{
     /* Running range and best range so far. *
      *   start   end                        */
  indexType i,     j;
  indexType BestI, BestJ;
     /* Size of range = j - i + 1. */
  indexType rangeSize;
     /* Total sum. */
  sumType sum;
     /* An element fetched from file. */
  numberType number;

     /*
      * Initialize variables.
      */
  i = 0;
  BestI = 50001U;
  BestJ = 50001U;
  rangeSize = 0;

     /*
      * For each element in the file:
      *   If that number is zero, see if we've found a zero-sum
      *     yet, and if not, set that index as the first
      *     zero-sum range.
      *   Starting from the next number, walk down the list:
      *     Add the next number to the running sum.
      *     If the running sum is now zero, check if it's range
      *       is larger than one we've found so far. If so, set
      *       that as the largest range so far.
      *   Stop when a zero-sum range has been found that is
      *   larger than the number of elements left to check.
      */
  while (( i < n ) && ( rangeSize < ( n - i ))) {
    if ( i < MAX_ELEMENTS ) {
      sum = a[ i ];
    }
    else {
      fseek( tempFile,
             (( i - MAX_ELEMENTS ) * sizeof( numberType )),
             SEEK_SET );
      fread( &sum, sizeof( numberType ), 1, tempFile );
    }
    if (( sum == 0 ) && ( rangeSize == 0 )) {
      BestI = i;
      BestJ = i;
      rangeSize = 1;
    }
    for ( j = i + 1; j < n; j++ ) {
      if ( j < MAX_ELEMENTS ) {
        sum += a[ j ];
      }
      else {
        fseek( tempFile,
               (( j - MAX_ELEMENTS ) * sizeof( numberType )),
               SEEK_SET );
        fread( &number, sizeof( numberType ), 1, tempFile );
        sum += number;
      }
      if (( sum == 0 ) && (( j - i + 1 ) > rangeSize )) {
        BestI = i;
        BestJ = j;
        rangeSize = j - i + 1;
      }
    } /* for */
    i++;
  } /* while */

  return( MAKE_RANGE( BestI, BestJ ) );
} /* ZeroSumSeq */

