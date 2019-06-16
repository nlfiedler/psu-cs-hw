/********************************************************************
 *
 * Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 * Permission to change this source and update it is hereby
 * granted to any student of The Pennsylvania State University.
 *
 * PROJECT:      COMP 421 LAN Analyzer
 * MODULE:       Statistics Viewer
 * FILE:         statview.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    11/07/95  Initial version
 *
 * DESCRIPTION:
 *      This is the statistics view module for the statistics
 *      object. It provides a scrolling display of each user
 *      and his/her associated statistics.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "statview.h"

/********************************************************************
 *                 Functions
 *******************************************************************/

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Tests the stat_view function.
 * RETURNS:      int - Error condition
 * STRATEGY:     Make a list of phony users, then call the stat_view
 *               function, then free the list. Finally, return to DOS.
 *******************************************************************/
int main
  (
  void
  )
{
     /* Phony list to test stat_view. */
  ptrToListType list;
     /* Data for a particular user. */
  dataType      data;
     /* Input file handle. */
  FILE *        inputFile;
     /* Number of fields read by fscanf. */
  int numRead;
     /* Number of users read in. */
  int numUsers;

     /*
      * Make empty list and fill it with data from
      * input file.
      */
  list = MakeEmptyList();
  inputFile = fopen( "users.lst", "rt" );
  numUsers = 0;
  do {
    numRead = fscanf( inputFile, "%s %ld %ld %ld %ld %ld %ld\n",
                      data.userName,      &data.packetsPerSec,
                      &data.errorsPerSec, &data.kbytesPerSec,
                      &data.numPackets,   &data.numErrors,
                      &data.numKbytes );
    if ( numRead == 7 ) {
      ListAddNode( list, data );
      numUsers++;
    }
  } while ( numRead == 7 );
  fclose( inputFile );

     /*
      * Display stats.
      */
  viewstats( list, numUsers );

     /*
      * Free the list.
      * Return to DOS.
      */
  FreeList( list );
  return( 0 );
} /* main */

/********************************************************************
 *                 viewstats
 ********************************************************************
 * SYNOPSIS:     Provides the window in which the statistics will
 *               be viewed.
 * RETURNS:      void - nothing
 * STRATEGY:     First establish a window to draw in, then fill it
 *               with a nice color, draw a border and columns.
 *               Then walk the linked list and display the users
 *               until we've displayed enough. Then wait for the
 *               user to scroll the display or quit.
 *******************************************************************/
void viewstats
  (
  ptrToListType list_p,
  int           numUsers_p
  )
{
     /* What user wants to do, scroll list or quit. */
  char chUserAction;
     /* Position of first user displayed. */
  int cPosition;

     /*
      * Create viewing window. Clear it to new colors.
      * Draw the box for displaying.
      */
  window( STAT_VIEW_WIN_LEFT, STAT_VIEW_WIN_TOP,
          STAT_VIEW_WIN_RIGHT, STAT_VIEW_WIN_BOTTOM );
  textcolor( STAT_VIEW_TEXT_COLOR );
  textbackground( STAT_VIEW_BKGRND_COLOR );
  clrscr();
  drawbox();

     /*
      * Display the list.
      * Start letting user scroll the list.
      * Stop when user presses 'Q'.
      */
  cPosition = 0;
  displaylist( list_p, cPosition, STAT_VIEW_NUM_ROWS );
  do {
    chUserAction = toupper( getch() );
    switch ( chUserAction ) {
      case 'I' : if ( cPosition > 0 ) {
                   cPosition--;
                   displaylist( list_p, cPosition, STAT_VIEW_NUM_ROWS );
                 }
                 break;
      case 'K' : if ( cPosition < ( numUsers_p - STAT_VIEW_NUM_ROWS )) {
                   cPosition++;
                   displaylist( list_p, cPosition, STAT_VIEW_NUM_ROWS );
                 }
                 break;
    } /* switch */
  } while ( chUserAction != 'Q' );
} /* viewstats */

/********************************************************************
 *                 drawbox
 ********************************************************************
 * SYNOPSIS:     Draws the view border and column titles.
 * RETURNS:      void - nothing
 * STRATEGY:     Simply draw the border using some for loops.
 *******************************************************************/
void drawbox
  (
  void
  )
{
     /* For loop indices. */
  int col, row;

     /*
      * Draw top of box.
      */
  gotoxy( 1, 1 );
  printf( "É" );
  for ( col = 0;
        col < ( STAT_VIEW_WIN_RIGHT - STAT_VIEW_WIN_LEFT - 1 );
        col++ ) {
    printf( "Í" );
  }
  printf( "»\n" );

     /*
      * Draw line that will have the column titles.
      */
  gotoxy( 1, wherey() );
  printf( "º" );
  for ( col = 0;
        col < ( STAT_VIEW_WIN_RIGHT - STAT_VIEW_WIN_LEFT - 1 );
        col++ ) {
    printf( " " );
  }
  printf( "º" );
     /*
      * Draw column titles.
      */
  gotoxy( 2, wherey() );
  printf( " User     Packets/s  Errors/s  Kbytes/s " );
  printf( "# Packets  # Errors      # Kb\n" );
     /*
      * Draw the title underline.
      * For every so many places, put a Â to match the
      * column separators.
      */
  gotoxy( 1, wherey() );
  printf( "Ç" );
  for ( col = 0;
        col < ( STAT_VIEW_WIN_RIGHT - STAT_VIEW_WIN_LEFT - 1 );
        col++ ) {
    if ((( col + 1 ) % STAT_VIEW_COLUMN_WIDTH ) == 0 ) {
      printf( "Â" );
    }
    else {
      printf( "Ä" );
    }
  }
  printf( "¶\n" );
     /*
      * Draw lines that will have the values.
      */
  for ( row = 0;
        row < ( STAT_VIEW_WIN_BOTTOM - STAT_VIEW_WIN_TOP - 3 );
        row++ ) {
    gotoxy( 1, wherey() );
    printf( "º" );
    for ( col = 0;
          col < ( STAT_VIEW_WIN_RIGHT - STAT_VIEW_WIN_LEFT - 1 );
          col++ ) {
      printf( " " );
    }
    printf( "º\n" );
  }

     /*
      * Draw bottom of box.
      */
  gotoxy( 1, wherey() );
  printf( "È" );
  for ( col = 0;
        col < ( STAT_VIEW_WIN_RIGHT - STAT_VIEW_WIN_LEFT - 1 );
        col++ ) {
    if ((( col + 1 ) % STAT_VIEW_COLUMN_WIDTH ) == 0 ) {
      printf( "Ï" );
    }
    else {
      printf( "Í" );
    }
  }
  printf( "¼" );
} /* drawbox */

/********************************************************************
 *                 displaylist
 ********************************************************************
 * SYNOPSIS:     Displays the list of users, printing each item
 *               in the appropriate row and column.
 * RETURNS:      void - nothing
 * STRATEGY:     Walk the list until startPos_p is found, then
 *               start displaying stats, then stop when
 *               numToDisplay_p is reached.
 *******************************************************************/
void displaylist
  (
  ptrToListType list_p,
  int           startPos_p,
  int           numToDisplay_p
  )
{
     /* For loop index to walk list. */
  int user;
     /* Temporary node for walking list. */
  ptrToNodeType node;
     /* Data of node being displayed. */
  dataType data;

     /*
      * Start at the head of the list and walk down to where
      * we should start displaying users.
      * then start displaying the user stats until we've
      * displayed all we can.
      */
  ListHead( list_p, &node );
  for ( user = 0; user < startPos_p; user++ ) {
    ListNextNode( &node );
  }
  gotoxy( 2, 3 );
  for ( user = 0; user < numToDisplay_p; user++ ) {
    ListDataOfNode( node, &data );
    gotoxy( 2, wherey() + 1 );
       /* Print column separators inline.
        * Don't remove the 3's. */
    printf( "%-9s³ %8ld³ %8ld³ %8ld³ %8ld³ %8ld³ %8ld",
            data.userName,     data.packetsPerSec,
            data.errorsPerSec, data.kbytesPerSec,
            data.numPackets,   data.numErrors,
            data.numKbytes );
    ListNextNode( &node );
  }
} /* displaylist */

