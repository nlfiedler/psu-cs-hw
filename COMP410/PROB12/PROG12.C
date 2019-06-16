/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:   Data Structures class
 * MODULE:    ProbSet 1-2
 * FILE:      prog12.c
 *
 * AUTHOR:    Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date     Description
 *      ----   ----     -----------
 *       NF    01/22/95 Initial version
 *
 * DESCRIPTION:
 *      This program is a test-wrapper for the Problem 1-2 in which
 *      we are to estimate the running time of an algorithm. This
 *      Was written to check my guesses by examining the difference
 *      in running times for different size sets.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <dos.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE  1 /* Value for TRUE */
    #define FALSE 0 /* Value for FALSE */
    #define N1 10
    #define N2 100
    #define N3 1000
    #define N4 10000
    #define N5 100000

/********************************************************************
 *                 Globals
 *******************************************************************/

/********************************************************************
 *                 Function prototypes
 *******************************************************************/
    float CalcTime( struct time startp, struct time endp );
    void StrangeSum( int n );
    int StrangerSum( int n );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     This is the test wrapper portion of the program.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * STRATEGY:     Uses the gettime function in time.h and runs the
 *             algorithms each with the different sizes of sets,
 *             N1, N2, N3, N4, N5 and outputs the running time of
 *             the algorithms with each set.
 *******************************************************************/
int main( int argc, char *argv[] ) {
  struct time startTime; /* Time at start */
  struct time endTime;   /* Time at end */
  int i; /* Used in for loop */
  int n[5] = { N1, N2, N3, N4, N5 };
  /* Holds values for N1, N2, N3, N4, N5 */

  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "Test wrapper  Copyright(C) 1995 Blue Marsh Softworks\n" );
    printf( "This is a generic testing program for trying out various\n" );
    printf( "algorithms and checking their running times on different\n" );
    printf( "sized sets of numbers.\n" );
    printf( "Usage: prog12\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  if ( argc > 1 ) {
    printf( "For help, type prog12 /?\n" );
    exit( 1 ); /* Exit to DOS with error */
  } /* if */
  for ( i = 0; i < 5; i++ ) {
    gettime( &startTime );  /* Get system time */
    StrangeSum( n[i] ); /* Run algorithm A */
    gettime( &endTime ); /* Get system time */
    printf( "The running time is %f seconds\n",
            CalcTime( startTime, endTime ));
    printf( "on the set of size %d for A\n", n[i] );
    gettime( &startTime );  /* Get system time */
    StrangerSum( n[i] ); /* Run algorithm Bhere */
    gettime( &endTime ); /* Get system time */
    printf( "The running time is %f seconds\n",
            CalcTime( startTime, endTime ));
    printf( "on the set of size %d for B\n\n", n[i] );
  } /* for */
  return( 0 );
} /* main */

/********************************************************************
 *                 CalcTime
 ********************************************************************
 * SYNOPSIS:     Calculate execution time using start and end
 *               time and return value in seconds.
 * CALLED BY:    main
 * PARAMETERS:   ( struct time startp, struct time endp )
 * RETURNS:      float  Difference in seconds
 * SIDE EFFECTS:
 * STRATEGY:     Use simple arithmetic to calculate time difference.
 *******************************************************************/
float CalcTime( struct time startp, struct time endp ) {
	float startt; /* Start as float */
	float endt;   /* End as float */
  /* Convert start time to hundredths of seconds */
	startt = startp.ti_min * 6000 + startp.ti_sec * 100;
	startt = startt + startp.ti_hund;
  /* Convert end time to hundredths of seconds */
	endt = endp.ti_min * 6000 + endp.ti_sec * 100;
  endt = endt + endp.ti_hund;
  /* Calculate difference and convert to seconds */
	endt = ( endt - startt ) / 100;
	return( endt );
} /* CalcTime */

/********************************************************************
 *                 StrangeSum
 ********************************************************************
 * SYNOPSIS:     The first algorithm to be tested.
 * CALLED BY:    main
 * PARAMETERS:   ( int n )
 * RETURNS:      void
 * STRATEGY:     Confuse us
 *******************************************************************/
void StrangeSum( int n ) {
  int x = 0,
      y = 1,
      i,
      j;

  for ( i = 1; i <= n; i++ ) {
    if (( i % 3 ) == 0 ) {
      for ( j = i; j <= n; j++ ) {
        x = x + 1;
      } /* for */
      for ( j = 1; j <= i; j++ ) {
        y = y + 1;
      } /* for */
    } /* if */
  } /* for */
} /* StrangeSum */

/********************************************************************
 *                 StrangerSum
 ********************************************************************
 * SYNOPSIS:     The second algorithm to be tested.
 * CALLED BY:    main
 * PARAMETERS:   ( int n )
 * RETURNS:      int  Sum
 * STRATEGY:     Confuse us
 *******************************************************************/
int StrangerSum( int n ) {
  int i,
      j,
      k,
      Sum = 0;

  for ( i = 1; i <= n; i++ ) {
    for ( j = 1; j <= (i*i); j++ ) {
      if (( j % i ) == 0 ) {
        for ( k = 1; k <= j; k++ ) {
          Sum = Sum + 1;
        } /* for */
      } /* if */
    } /* for */
  } /* for */
  return( Sum );
} /* StrangerSum */
