/********************************************************************
 *
 * Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved.
 *
 * PROJECT:      COMP 421 LAN Analyzer
 * MODULE:       ASCII Table
 * FILE:         ascii.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name    Date      Description
 *      ----    ----      -----------
 *       NF     11/07/95  Initial version
 *
 * DESCRIPTION:
 *      This displays all 256 ASCII characters and their
 *      associated values.
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the work.
 * RETURNS:      int - Error condition
 * STRATEGY:     Use a for loop to output all the ASCII charactes
 *               and their associated values.
 *******************************************************************/
int main
  (
  void
  )
{
     /* For loop index. */
  int i;

     /*
      * Loop through the ASCII characters and output the
      * value and it's character.
      * Avoid printing characters that screw up display,
      * and don't bother with the alphabet letters.
      */
  for ( i = 0; i < 256; i++ ) {
    if ( (  i ==  0 )                  ||
         (  i == 26 )                  ||
         (( i >= 65 ) && ( i <=  90 )) ||
         (( i >= 97 ) && ( i <= 122 )) ||
         (( i >=  7 ) && ( i <=  13 ))    ) {
      /* Do nothing */
    }
    else {
      printf( "%3d %c   ", i, i );
    }
  }

  return( 0 );
} /* main */
