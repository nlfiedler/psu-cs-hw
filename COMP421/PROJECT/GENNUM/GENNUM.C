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
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
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
     /* For loop index. */
  int i;
     /* Output file variables. */
  FILE *  hOutputFile;
  int arrival;

  hOutputFile = fopen( "input.txt", "wt" );
  arrival = 0;
  for ( i = 1; i < MAXJOBS + 1; i++ ) {
    fprintf( hOutputFile, "0 0 0 %d %d 0 0\n",
             arrival, random( 576 ) );
    if (( i % 10 ) == 0 ) {
      arrival++;
    }
  }
  fprintf( hOutputFile, "\n" );
  if ( fclose( hOutputFile ) ) {
    printf( "Error closing output file %d!\n", i );
  }

  return( 0 );
} /* main */

