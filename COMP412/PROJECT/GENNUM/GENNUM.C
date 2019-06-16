/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Generate Numbers
 * FILE:         gennum.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    09/29/95  Initial version
 *
 * DESCRIPTION:
 *      Generates the random numbers used by the CPU schedulers.
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
 * STRATEGY:     Use two for loops; one loops 3 times to generate
 *               the three data streams. The second loop is inside
 *               the first and it generates each of the data pairs
 *               for that stream.
 *******************************************************************/
int main
  (
  void
  )
{
     /* For loop indices. */
  int i;
  int j;
     /* Output file variables. */
  FILE *  hOutputFile;
  char    achFileName[12];
  boolean errorOccurred;
     /* Determines number of short jobs. */
  double randomNum;
  double shortJob[3] = { 0.95, 0.50, 0.05 };
     /* Data pair: arrival time, burst time. */
  double arrivalTime;
  int    jobSize;
     /* Lambda constant. */
  const double lambda = -1 / 3.46;

    /*
     * Start primary loop. The first time through the loop
     * we will generate the first data stream. The second
     * time we will generate the second stream, and so on.
     */
  for ( i = 1; i < 4; i++ ) {
       /*
        * Initialize arrival time.
        * Create binary output file.
        */
    arrivalTime = 0;
    sprintf( achFileName, "stream%d.dat", i );
    hOutputFile = fopen( achFileName, "w" );
       /*
        * Start loop
        *   Generate output data pair.
        *   Write to output file in binary form.
        *
        * First stream : 95% short jobs
        * Second stream: 50% short jobs
        * Third stream : 5% short jobs
        */
    j = 0;
    errorOccurred = FALSE;
    while ( ( errorOccurred == FALSE ) && ( j < MAXJOBS ) ) {
         /*
          * Interarrival time distribution:
          *  -( 1 / 3.46 ) * ln( 1 - r )
          */
      arrivalTime += lambda * log( 1 - BetterRandom() );
         /*
          * Short jobs: 1-3 seconds
          * Long jobs : 4-90 seconds
          */
      randomNum = BetterRandom();
      if ( randomNum <= shortJob[i-1] ) {
        jobSize = BetterRandom() * 3 + 1;
      }
      else {
        jobSize = BetterRandom() * 86 + 4;
      }
      if ( fprintf( hOutputFile, "%d\n", (int)arrivalTime ) == EOF ) {
        printf( "Error writing to output file stream%d.dat!\n", i );
        errorOccurred = TRUE;
      }
      if ( fprintf( hOutputFile, "%d\n", jobSize ) == EOF ) {
        printf( "Error writing to output file stream%d.dat!\n", i );
        errorOccurred = TRUE;
      }
      j++;
    } /* while */
    fprintf( hOutputFile, "\n" );

       /*
        * Close output file.
        */
    if ( fclose( hOutputFile ) ) {
      printf( "Error closing output file %d!\n", i );
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

