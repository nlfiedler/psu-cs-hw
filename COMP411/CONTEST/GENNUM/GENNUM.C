/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 411 Contest Project
 * MODULE:       Generate Numbers
 * FILE:         gennum.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/11/95  Initial version
 *
 * DESCRIPTION:
 *      Generates the random numbers used by the contest solution.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "gennum.h"

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the work.
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * STRATEGY:     Use a for loop to generate the input sequence.
 *******************************************************************/
int main
  (
  void
  )
{
     /* Loop indices. */
  int      i;
  long int j;
     /* File variables. */
  FILE *  hOutputFile;
  char    achFileName[13];
  boolean fileOkay;
     /* Used to generate number sequence. */
  int      randomNum;
  long int jobSize[3] = { 100, 10000, 50000L };

    /*
     * Start primary loop. The first time through the loop
     * we will generate the first data stream. The second
     * time we will generate the second stream, and so on.
     */
  for ( i = 0; i < 3; i++ ) {
       /*
        * Create text output file.
        * Write number of elements to file.
        */
    sprintf( achFileName, "stream%d.dat", i+1 );
    hOutputFile = fopen( achFileName, "wt" );
    fprintf( hOutputFile, "%ld\n", jobSize[ i ] );
       /*
        * Start loop
        *   Generate output numbers.
        *   Write to output file in text form.
        */
    j = 0;
    fileOkay = TRUE;
    while (( fileOkay ) && ( j < jobSize[ i ] )) {
      randomNum = BetterRandom() * 65535L - 32768L;
      if ( fprintf( hOutputFile, "%d\n", randomNum ) == EOF ) {
        printf( "Error writing to stream%d.dat!\n", i + 1 );
        fileOkay = FALSE;
      }
      j++;
    }
    fprintf( hOutputFile, "\n" );

       /*
        * Close output file.
        */
    if ( fclose( hOutputFile ) ) {
      printf( "Error closing stream%d!\n", i + 1 );
    }
  } /* for i */

     /*
      * Return to DOS.
      */
  return( 0 );
} /* main */

/********************************************************************
 *                 BetterRandom
 ********************************************************************
 * SYNOPSIS:     Calculates a new random number based on the seed
 *               value (which starts at 1).
 * PARAMETERS:   ( void )
 * RETURNS:      double - New random number between 0 and 1.
 * STRATEGY:     Uses the algorithm from Data Structures and
 *               Algorithm Analysis by Mark Allen Weiss, p389.
 *******************************************************************/
double BetterRandom
  (
  void
  )
{
  long        tempSeed; /* Used to check if number is negative. */
  static long Seed = 1; /* Seed value, saved each time. */

  tempSeed = 48271L * ( Seed % 44488L ) - 3399L * ( Seed / 44488L );
  if ( tempSeed >= 0L ) {
    Seed = tempSeed;
  }
  else {
    Seed = tempSeed + 2147483647L;
  }
  return( (double)Seed / (double)2147483647L );
} /* BetterRandom */

