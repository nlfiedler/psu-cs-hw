/********************************************************************
 *
 * Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      LAN Analyzer
 * MODULE:       Tickertape Display
 * FILE:         marquee.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    11/05/95  Initial version
 *
 * DESCRIPTION:
 *      This is a marquee-type display routine to display a string
 *      on the screen in a scrolling fashion.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <conio.h>
    #include <string.h>
    #include <dos.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Position and size of display window. */
    #define WINDOW_WIDTH 30
    #define WINDOW_X_POS 25
    #define WINDOW_Y_POS 15

       /* Maximum length of the message. */
    #define MESSAGE_LENGTH 100

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the work.
 * RETURNS:      int - Error condition
 * STRATEGY:     Clear screen, and prepare variables. Start loop
 *               that displays the scrolling message. Wait until
 *               user presses a key to quit.
 *******************************************************************/
int main
  (
  void
  )
{
     /* Message to display. */
  char achMessage[ MESSAGE_LENGTH + 1 ];
  char achOutput[ WINDOW_WIDTH + 1 ];
     /* Current starting position within message to print. */
  int iPrintPos;
     /* Number of characters printed. */
  int cPrintWidth;
     /* For loop index. */
  int i;

     /*
      * Initialize variables and prepare message strings.
      * Clear screen.
      * Print window border.
      */
  strncpy( achMessage, "This is a scrolling message in the window " \
                      "of Marquee.  ", MESSAGE_LENGTH );
  achOutput[ WINDOW_WIDTH ] = 0;
  iPrintPos = 0;
  clrscr();
  gotoxy( WINDOW_X_POS, WINDOW_Y_POS + 0 );
  printf( "É" );
  for ( i = 0; i < WINDOW_WIDTH; i++ ) {
    printf( "Í" );
  }
  printf( "»\n" );
  gotoxy( WINDOW_X_POS, WINDOW_Y_POS + 1 );
  printf( "º" );
  for ( i = 0; i < WINDOW_WIDTH; i++ ) {
    printf( " " );
  }
  printf( "º\n" );
  gotoxy( WINDOW_X_POS, WINDOW_Y_POS + 2 );
  printf( "È" );
  for ( i = 0; i < WINDOW_WIDTH; i++ ) {
    printf( "Í" );
  }
  printf( "¼\n" );

     /*
      * Main loop. Does the scrolling.
      * Create scrolled message, applying a "wrap" if needed.
      * Display message. Delay for fraction of a second.
      * Increment string index and check bounds.
      */
  do {
       /*
        * Find number of characters that will be printed.
        * Could be less than WINDOW_WIDTH when end of string
        * is reached.
        * Copy those characters to output string.
        * If less than WINDOW_WIDTH characters were copied,
        * get some more characters from the beginning of the
        * message, to fill the window.
        */
    cPrintWidth = strlen( achMessage) - iPrintPos;
    if ( cPrintWidth > WINDOW_WIDTH ) {
      cPrintWidth = WINDOW_WIDTH;
    }
    strncpy( achOutput, achMessage + iPrintPos, cPrintWidth );
    if ( cPrintWidth < WINDOW_WIDTH ) {
      strncpy( achOutput + cPrintWidth, achMessage,
               WINDOW_WIDTH - cPrintWidth );
    }
    gotoxy( WINDOW_X_POS + 1, WINDOW_Y_POS + 1 );
    printf( "%s", achOutput );
    delay( 200 );
    iPrintPos++;
    if ( iPrintPos > strlen( achMessage ) ) {
      iPrintPos = 0;
    }
  } while ( !kbhit() );

     /*
      * Return to DOS.
      */
  return( 0 );
} /* main */

