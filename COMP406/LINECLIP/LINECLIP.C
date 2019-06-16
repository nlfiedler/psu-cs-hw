/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:     COMP 406 Assignments
 * MODULE:      LineClipper
 * FILE:        lineclip.c
 *
 * AUTHOR:      Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    01/29/95  Initial version
 *
 * DESCRIPTION:
 *      Assignment for 2/1/95 for Computer Graphics class. This
 *      program will demonstrate line clipping by a method similar
 *      to the Cohen-Sutherland algorithm for clipping lines to a
 *      rectangle.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #define IBM_PC
    #include "SRGP.h"

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define SCRHT    360
    #define SCRWD    360
    #define CLIPLINE 180

/********************************************************************
 *                 Typedefs
 *******************************************************************/

/********************************************************************
 *                 Globals
 *******************************************************************/

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       void ClipAndDraw( point p0, point p1, int yClip );
    boolean CompOutCode( int p, int clip );
    boolean GoodCoords( point p0, point p1 );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     This is the main function of the program.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( void )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Call ClipAndDraw with several lines as parameters
 *               to demonstrate the line clipping algorithm.
 *******************************************************************/
void UserMain( void ) {
  point pt0, pt1;

  /* Initialize SRGP library */
  SRGP_begin( "LineClipper", SCRWD, SCRHT, 0, 0 );

  /* Draw the horizontal clip line */
  /* Set style to dashed so it is set apart */
  SRGP_setLineStyle( DASHED );
  SRGP_lineCoord( 0, CLIPLINE, SCRWD, CLIPLINE );
  /* Turn style back to normal */
  SRGP_setLineStyle( CONTINUOUS );

  /* Define two points and draw a line clipped */
  pt0 = SRGP_defPoint( 190, 230 );
  pt1 = SRGP_defPoint( 240, 70 );
  ClipAndDraw( pt0, pt1, CLIPLINE );
  /* Define two points and draw a line clipped */
  pt0 = SRGP_defPoint( 210, 50 );
  pt1 = SRGP_defPoint( 260, 250 );
  ClipAndDraw( pt0, pt1, CLIPLINE );

  /* Now let's test some more lines */
  /* This line will be rejected */
  pt0 = SRGP_defPoint( 50, 50 );
  pt1 = SRGP_defPoint( 240, 50 );
  ClipAndDraw( pt0, pt1, CLIPLINE );
  /* This line will be accepted entirely */
  pt0 = SRGP_defPoint( 50, 280 );
  pt1 = SRGP_defPoint( 240, 280 );
  ClipAndDraw( pt0, pt1, CLIPLINE );
  /* This line is vertical, testing the formula */
  pt0 = SRGP_defPoint( 80, 50 );
  pt1 = SRGP_defPoint( 80, 250 );
  ClipAndDraw( pt0, pt1, CLIPLINE );
  

  /* Wait for user to close window */
  SRGP_waitEvent( INDEFINITE );

  /* Tell SRGP to close itself */
  SRGP_end();
} /* UserMain */

/********************************************************************
 *                 ClipAndDraw
 ********************************************************************
 * SYNOPSIS:     Clips and draws the given line to the clip line.
 * CALLED BY:    main
 * PARAMETERS:   ( point p0, point p1, int yClip )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using a method similar to that of the Cohen-
 *               Sutherland algorithm for line clipping to a
 *               rectangle, this function first rejects or clips
 *               the line and then draws the portion above the
 *               specified clip line.
 *******************************************************************/
void ClipAndDraw( point p0, point p1, int yClip ) {
      int x0, y0,
          x1, y1; /* Coordinates of clipped line */
  boolean outcode0,
          outcode1, /* Outcodes for p0 and p1 */
          accept;   /* Code to accept line */

  /* Assume line is acceptable and set variables */
  accept = TRUE;
  x0 = p0.x;
  y0 = p0.y;
  x1 = p1.x;
  y1 = p1.y;
  /* Get code for point 0 */
  outcode0 = CompOutCode( p0.y, yClip );
  /* Get code for point 1 */
  outcode1 = CompOutCode( p1.y, yClip );
  if (( outcode0 ) && ( outcode1 )) {
    accept = FALSE; /* Line past clip, reject it */
  } else {
    if ( outcode0 ) {
      y0 = yClip; /* Point 0 below clip line */
      x0 = p0.x + ( p1.x - p0.x ) * ( yClip - p0.y ) / ( p1.y - p0.y );
        /* Note we don't need to check for division by zero since a
         * horizontal line would not cross the clip line. */
    } else {
      if ( outcode1 ) {
        y1 = yClip; /* Point 1 below clip line */
        x1 = p0.x + ( p1.x - p0.x ) * ( yClip - p0.y ) / ( p1.y - p0.y );
      } /* if */
    } /* if-else */
  } /* if-else */
  if ( accept ) {
    /* Line is acceptable, draw it */
    SRGP_lineCoord( x0, y0, x1, y1 );
  } /* if */
} /* ClipAndDraw */

/********************************************************************
 *                 CompOutCode
 ********************************************************************
 * SYNOPSIS:     Computes the outcode for a given point.
 * CALLED BY:    main
 * PARAMETERS:   ( int p, int clip )
 * RETURNS:      boolean  TRUE if point below clip line
 * SIDE EFFECTS:
 * STRATEGY:     Compares the values and returns TRUE if p is
 *               greater than clip.
 *******************************************************************/
boolean CompOutCode( int p, int clip ) {
  boolean out; /* Return value */

  if ( p > clip ) {
    out = FALSE; /* P is greater than clip value */
  } else {
    out = TRUE; /* P is less than or equal to clip value */
  } /* if-else */
  return( out );
} /* CompOutCode */
