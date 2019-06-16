/********************************************************************
 *
 *    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 406 Assignments
 * MODULE:       TextLine
 * FILE:         textln1.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    01/27/95  Initial version
 *
 * DESCRIPTION:
 *      Using Bresenham's line drawing algorithm, this program will
 *      draw a line in the text mode using the two coordinates that
 *      the user enters upon starting the program.
 *      This program also includes the midpoint recursive line
 *      drawing algorithm.
 *      The Bresenham line is drawn using + while the other line is
 *      drawn with -.
 *      Both of these algorithms are limited to lines with a slope
 *      between 0 and -1.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <conio.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE -1
    #define FALSE 0

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    typedef unsigned char boolean;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
    void BresenhamLine( int x0, int y0, int x1, int y1 );
    void MidpointLine( float x0, float y0, float x1, float y1 );
    void RecursiveLine( int x0, int y0, int x1, int y1 );
    void WritePixel( int x, int y, char ch );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     This is the main function of the program. It calls
 *               the two line drawing functions.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * SIDE EFFECTS:
 * STRATEGY:     Gets the line coordinates from the user and then
 *               calls the appropriate line functions to draw the
 *               lines on the screen.
 *******************************************************************/
int main( int argc, char *argv[] ) {
      int x0, y0,
	  x1, y1; /* Coordinates of user specified line */
  boolean badInput;

  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "TextLine  Copyright (C) 1995 Blue Marsh Softworks\n" );
    printf( "This program draws two lines using the Bresenham and\n" );
    printf( "a recursive midpoint algorithm to compare the two.\n" );
    printf( "Usage: textln1\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  if ( argc > 1 ) {
    printf( "For help, type: textln1 /?" );
    exit( 1 ); /* Exit to DOS with error */
  } /* if */
  printf( "This program will draw an approximate line between two\n" );
  printf( "arbitrary points. The line is specified by two sets of\n" );
  printf( "coordinates. The first two specify x0 and y0, and the\n" );
  printf( "second two specify x1 and y1. The slope of the line\n" );
  printf( "must be between 0 and -1. The maximum x value is 79,\n" );
  printf( "and the maximum y value is 24.\n" );
  do {
    printf( "Enter the coordinates (Ex: 5 5 60 20 ): " );
    scanf( "%d %d %d %d", &x0, &y0, &x1, &y1 );
    badInput = FALSE;
    if ((( x1 - x0 ) < 0 ) || (( y1 - y0 ) < 0 )) {
      printf( "The slope is not between 0 and -1.\n" );
      badInput = TRUE;
    } /* if */
    if (( x0 > 79 ) || ( x1 > 79 )) {
      printf( "One of the x coordinates is greater than 79.\n" );
      badInput = TRUE;
    } /* if */
    if (( y0 > 24 ) || ( y1 > 24 )) {
      printf( "One of the y coordinates is greater than 24.\n" );
      badInput = TRUE;
    } /* if */
  } while ( badInput );
  clrscr();
  BresenhamLine( x0, y0, x1, y1 );
  MidpointLine( x0, y0, x1, y1 );
  if (( x1 - x0 ) > ( y1 - y0 )) {
    RecursiveLine( x0, y0, x1, y1 );
  } /* if */
  return( 0 );
} /* main */

/********************************************************************
 *                 BresenhamLine
 ********************************************************************
 * SYNOPSIS:     Given four points this function draws a line in
 *               text mode.
 * CALLED BY:    main
 * PARAMETERS:   ( int x0, int y0, int x1, int y1 )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using Bresenham's midpoint line drawing algorithm,
 *               this function approximates a line in the text mode.
 *               This code comes from Introduction to Computer Graphics
 *               by Foley, et. al. on page 75.
 *               The slope of the line must be between 0 and -1.
 * USES:         WritePixel
 * ANALYSIS:     This function is very nice in that it does not use
 *               any real numbers and only uses multiplication in it's
 *               initialization of three of the variables. It is also
 *               extremely fast.
 * AUTHOR:  Bresenham
 *******************************************************************/
void BresenhamLine( int x0, int y0, int x1, int y1 ) {
  int dx, dy,
      incrE, incrNE,
      d, x, y;

  dx = x1 - x0;
  dy = y1 - y0;
  d = dy * 2 - dx;
  incrE = dy * 2;
  incrNE = ( dy - dx ) * 2;
  x = x0;
  y = y0;
  WritePixel( x, y, '+' );
  while ( x < x1 ) {
    if ( d <= 0 ) {
      d += incrE;
      x++;
    } else {
      d += incrNE;
      x++;
      y++;
    } /* if-else */
    WritePixel( x, y, '+' );
  } /* while */
  if ( x == x1 ) {
    while ( y < y1 ) {
      WritePixel( x, y, '+' );
      y++;
    } /* while */
  } /* if */
} /* BresenhamLine */

/********************************************************************
 *                 MidpointLine
 ********************************************************************
 * SYNOPSIS:     Given four points this function draws a line in
 *               text mode.
 * CALLED BY:    main
 * PARAMETERS:   ( float x0, float y0, float x1, float y1 )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using a recursive midpoint calculating algorithm
 *               this function will draw an approximate line on the
 *               screen.
 *               The base case is when the difference between the
 *               coordinates is one or less.
 *               The value of 0.33 used in the second if statement
 *               was chosen because 0 didn't work and any higher
 *               value would leave blank spaces. A smaller value
 *               will only make the function run more iterations.
 * USES:         WritePixel
 * ANALYSIS:     This function is unfortunately slow and uses real
 *               numbers. It is also difficult to determine a stopping
 *               point in the recursion. If 1 is picked as a good
 *               stopping distance, then there will be blank spots.
 *               Choosing a value of 0 causes the recursion to never
 *               end. Thus, a value between .5 and 0 was chosen.
 *               This algorithm does have an advantage over the
 *               Bresenham algorithm, however. It is able to draw
 *               any line in the lower-right quadrant, without any
 *               special condition checking.  
 *******************************************************************/
void MidpointLine( float x0, float y0, float x1, float y1 ) {
  float mx, my,
        dx, dy;
  static boolean endsDrawn = FALSE; /* TRUE if endpoints drawn */

  if ( !endsDrawn ) {
    WritePixel( (int)x0, (int)y0, '-' );
    WritePixel( (int)x1, (int)y1, '-' );
    endsDrawn = TRUE;
  } /* if */
  if ((( x1 - x0 ) > 0.33 ) || (( y1 - y0 ) > 0.33 )) {
    dx = x1 - x0;
    dy = y1 - y0;
    mx = x0 + dx / 2;
    my = y0 + dy / 2;
    WritePixel( (int)mx, (int)my, '-' );
    MidpointLine( x0, y0, mx, my ); /* Draw top left half. */
    MidpointLine( mx, my, x1, y1 ); /* Draw bottom right half. */
  } /* if */
} /* MidpointLine */

/********************************************************************
 *                 RecursiveLine
 ********************************************************************
 * SYNOPSIS:     Given four points this function draws a line in
 *               text mode.
 * CALLED BY:    main
 * PARAMETERS:   ( int x0, int y0, int x1, int y1 )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using Bresenham's midpoint line drawing algorithm,
 *               this function approximates a line in the text mode.
 *               This code comes from Introduction to Computer Graphics
 *               by Foley, et. al. on page 75.
 *               The slope of the line must be between 0 and -1.
 *               This function is implemented recursively.
 * USES:         WritePixel
 * ANALYSIS:     This function basically is worthless. It is totally
 *               off the true line and is a waste of memory.
 * AUTHOR:  Bresenham & Fiedler
 *******************************************************************/
void RecursiveLine( int x0, int y0, int x1, int y1 ) {
  int dx, dy,
      incrE, incrNE,
      d, x, y;

  dx = x1 - x0;
  dy = y1 - y0;
  d = dy * 2 - dx;
  incrE = dy * 2;
  incrNE = ( dy - dx ) * 2;
  x = x0;
  y = y0;
  WritePixel( x, y, '%' );
  if (( x0 != x1 ) || ( y0 != y1 )) {
    if ( d <= 0 ) {
      d += incrE;
      x++;
    } else {
      d += incrNE;
      x++;
      y++;
    } /* if-else */
    WritePixel( x, y, '%' );
    RecursiveLine( x, y, x1, y1 );
  } /* if */
} /* RecursiveLine */

/********************************************************************
 *                 WritePixel
 ********************************************************************
 * SYNOPSIS:     Given four points this function draws a line in
 *               text mode.
 * CALLED BY:    
 * PARAMETERS:   ( int x, int y, char ch )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using gotoxy and printf this function will place a
 *               character ch at the x and y location on the text
 *               mode screen.
 *******************************************************************/
void WritePixel( int x, int y, char ch ) {
  gotoxy( x, y );
  printf( "%c", ch );
} /* WritePixel */
