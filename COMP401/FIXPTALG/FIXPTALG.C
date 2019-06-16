/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 401 Assignments
 * MODULE:       Fixed Point Iteration Algorithm
 * FILE:         fixptalg.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *        NF     02/25/95  Initial version
 *
 * DESCRIPTION:
 *      This is part of Problem Set 3 of COMP 401 - Numerical
 *      Analysis. It implements the Fixed Point Iteration algorithm.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <conio.h>
    #include <math.h>

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    long double myabs( long double x );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * SIDE EFFECTS:
 * STRATEGY:     Uses math.h functions to implement the fixed point
 *               iteration algorithm.
 *******************************************************************/
int main( void ) {
  long double p0,  /* Previous approximation */
              p,   /* Current approximation */
              TOL; /* Tolerance */
          int i,   /* Used in while loop */
              N0;  /* Number of iterations to perform */
         char func[2]; /* Which function to use (+ or -) */

  printf( "Fixed Point Iteration Algorithm\n" );
  printf( "Copyright (C) 1995 Blue Marsh Softworks\n\n" );
  /* Get values from user */
  printf( "Please enter p0: " );
  scanf( "%Lf", &p0 );
  do {
    printf( "Enter the number of iterations: " );
    scanf( "%d", &N0 );
    if ( N0 < 1 ) {
      printf( "Number of iterations must be >= 1.\n" );
    } /* if */
  } while ( N0 < 1 );
  printf( "Enter the tolerance: " );
  scanf( "%Lf", &TOL );
  do {
    printf( "Which function [+/-] ? " );
    scanf( "%2s", &func );
    if (( func[0] != '+' ) && ( func[0] != '-' )) {
      printf( "Please enter a + or -.\n" );
    } /* if */
  } while (( func[0] != '+' ) && ( func[0] != '-' ));

  /* Fixed Point Iteration implementation */
  i = 1;
  while ( i <= N0 ) {

    /* This is the function you want to examine. */
    if ( func[0] == '+' ) {
      p = log( 3 * pow( p0, 2 ) );
    } else {
      p = log( 3 * pow( p0, 2 ) );
    } /* if-else */

    if ( myabs( p - p0 ) < TOL ) {
      printf( "Solution found: %Lf ", p );
      printf( "after %d iterations.\n", i );
      break;
    } /* if */
    i = i + 1;
    p0 = p;
  } /* while */
  /* Test if solution found. */
  if ( i > N0 ) {
    printf( "No solution found.\n" );
  } /* if */
  return( 0 ); /* Return to DOS */
} /* main */

/********************************************************************
 *                 myabs
 ********************************************************************
 * SYNOPSIS:     Finds the absolute value of a number.
 * CALLED BY:    main
 * PARAMETERS:   ( long double x ) - Number of absolute
 * RETURNS:      long double - Absolute value of x
 * SIDE EFFECTS:
 * STRATEGY:     Uses simple logic to find absolute value of x.
 *******************************************************************/
long double myabs( long double x ) {
  if ( x < 0 ) {
    return( 0 - x );
  } else {
    return( x );
  } /* if-else */
} /* myabs */
