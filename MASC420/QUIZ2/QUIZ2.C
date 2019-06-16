/********************************************************************
 *
 *    Copyright (c) 1995 Blue Marsh Softworks -- All rights reserved.
 *
 * PROJECT:      MASC 420 Quizzes
 * MODULE:       Quiz #2
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      09/26/95  Initial version
 *
 * DESCRIPTION:
 *      Computes the probability that there will be a match in
 *      day of birth amoung a class of 27 students.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the
 *               calculations.
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * STRATEGY:     Use a for loop to compute the probabitlity that
 *               there won't be a match in birthdays, then subtract
 *               that result from one.
 *******************************************************************/
int main
  (
  void
  )
{
  double i;
  double answer = 1.0;

  for( i = 365.0;
       i > 365.0 - 27.0;
       i = i - 1.0 ) {
    answer = answer * ( i / 365.0 );
  }

  printf( "Probability of match: %lf\n", ( 1.0 - answer ) );

  return( 0 );
}

