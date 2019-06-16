/********************************************************************
 *
 * Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 * Permission to change this source and update it is hereby
 * granted to any student of The Pennsylvania State University.
 *
 * PROJECT:      COMP 421 LAN Analyzer
 * MODULE:       Statistics Viewer
 * FILE:         statview.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    11/07/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the statview.c program.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "list.h"  /* Has: stdio.h
                               stdlib.h */
    #include <conio.h> /* gotoxy, wherex, wherey */
    #include <ctype.h> /* */

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Number of rows and columns of data. */
    #define STAT_VIEW_NUM_COLUMNS  7
    #define STAT_VIEW_COLUMN_WIDTH 10
    #define STAT_VIEW_NUM_ROWS     7

       /* Position of stat-view window. */
    #define STAT_VIEW_WIN_TOP    10
    #define STAT_VIEW_WIN_BOTTOM STAT_VIEW_WIN_TOP + \
                                 STAT_VIEW_NUM_ROWS + \
                                 3 /* Extra space */
    #define STAT_VIEW_WIN_LEFT   4
    #define STAT_VIEW_WIN_RIGHT  STAT_VIEW_WIN_LEFT + \
                                 ( STAT_VIEW_COLUMN_WIDTH * \
                                 STAT_VIEW_NUM_COLUMNS )

       /* Stat-view text colors */
    #define STAT_VIEW_TEXT_COLOR   YELLOW
    #define STAT_VIEW_BKGRND_COLOR BLUE

/********************************************************************
 *                 Data Types
 *******************************************************************/

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Display window with statistics. */
    void viewstats
      (
      ptrToListType list_p,
      int           numUsers_p
      );

       /* Draw stat-view window border. */
    void drawbox
      (
      void
      );

       /* Displays list of users. */
    void displaylist
      (
      ptrToListType list_p,
      int           startPos_p,
      int           numToDisplay_p
      );

