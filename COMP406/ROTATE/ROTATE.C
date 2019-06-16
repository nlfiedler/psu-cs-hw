/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 406 Assignments
 * MODULE:       Rotate
 * FILE:         rotate.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    02/10/95  Initial version
 *       NF    02/16/95  Change in specifications
 *
 * DESCRIPTION:
 *      This program rotates a set of points (a polygon) around
 *      a user specified point, counter-clockwise. The user can
 *      repeatedly click the mouse to have the polygon move
 *      around that point.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #define IBM_PC
    #include "SRGP.h"

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define NUM_POINTS 5 /* Number of points in polygon */
    #define ANGLE_INC  0.087266462 /* Degrees of angle increase
                                   * in radians (5 degrees) */

    #define SCRWD 360 /* Screen size */
    #define SCRHT 360

    #define QLX   5    /* Quit left side */
    #define QRX   QLX+50 /* Quit right side */
    #define QBY   5     /* Quit bottom */
    #define QTY   QBY+30 /* Quit top */

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    typedef struct realPoint {
      long double x,
                  y;
    };
    typedef struct realPoint *realArrayType;
    typedef point *arrayType;

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    /* Draw polygon primitive */
    void DrawShape( arrayType vertices );
    /* Draw the picture (house) */
    void DrawMyHouse( arrayType vertices, realArrayType reals,
                      point rotPt );
    /* Draws the quit button */
    void MakeQuitButton( rectangle buttonRect );
    /* Checks if mouse is in a rectangle */
    boolean IsMouseInRegion( point position, rectangle rect );
    /* Checks if mouse click over quit button */
    boolean PickedQuitButton( point location,
                              rectangle buttonRect );

/********************************************************************
 *                 Globals
 *******************************************************************/

/********************************************************************
 *                 UserMain
 ********************************************************************
 * SYNOPSIS:     The main function of the program. Contains the
 *               locator sampling loop and program initialization.
 * CALLED BY:    Operating system
 * PARAMETERS:   void
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Rotate a set of points around a point (initially
 *               set to the center of the object) until the user
 *               clicks on another point, then rotate around that
 *               new point. Exit the program when the user clicks
 *               on the quit button.
 *******************************************************************/
void UserMain( void ) {
           point rotatePoint; /* Current center of rotation */
       arrayType vertices; /* Set of points for polygon */
   realArrayType realVertices; /* Set of points in real format */
         boolean endProgram; /* T/F flag to end program */
       rectangle quitRect; /* Rectangle of quit button */
  locatorMeasure measure; /* Mouse event record */

  /* Start up the SRGP library */
  SRGP_begin( "Rotate", SCRWD, SCRHT, 0, 0 );
  /* Initialize rotation point */
  /* Allocate space for vertices array */
  if (( vertices = (arrayType)calloc( NUM_POINTS,
                   sizeof( point ) )) == 0 ) {
    printf( "Error allocating memory for polygon points.\n" );
    exit( 1 ); /* Exit to OS with error */
  } /* if */
  if (( realVertices = (realArrayType)calloc( NUM_POINTS,
                        sizeof( struct realPoint ) )) == 0 ) {
    printf( "Error allocating memory for polygon points.\n" );
    exit( 1 ); /* Exit to OS with error */
  } /* if */
  /* Set points for a house */
  vertices[0] = SRGP_defPoint( 250, 180 );
  realVertices[0].x = vertices[0].x;
  realVertices[0].y = vertices[0].y;
  vertices[1] = SRGP_defPoint( 300, 180 );
  realVertices[1].x = vertices[1].x;
  realVertices[1].y = vertices[1].y;
  vertices[2] = SRGP_defPoint( 300, 220 );
  realVertices[2].x = vertices[2].x;
  realVertices[2].y = vertices[2].y;
  vertices[3] = SRGP_defPoint( 275, 240 );
  realVertices[3].x = vertices[3].x;
  realVertices[3].y = vertices[3].y;
  vertices[4] = SRGP_defPoint( 250, 220 );
  realVertices[4].x = vertices[4].x;
  realVertices[4].y = vertices[4].y;
  /* Define quit rectangle */
  quitRect = SRGP_defRectangle( QLX, QBY, QRX, QTY );
  /* Draw quit button */
  MakeQuitButton( quitRect );
  /* Draw the polygon */
  DrawShape( vertices );
  /* Initialize mouse for event wait */
  SRGP_setLocatorButtonMask( LEFT_BUTTON_MASK );
  SRGP_setInputMode( LOCATOR, EVENT );
  /* Wait for user click */
  SRGP_waitEvent( INDEFINITE );

  /* Get mouse position */
  SRGP_getLocator( &measure );
  rotatePoint = measure.position;

  /* Initialize mouse for sample loop */
  SRGP_setInputMode( LOCATOR, SAMPLE );
  /* Assume user doesn't want to quit yet */
  endProgram = FALSE;
  do {
    /* Draw picture, also rotates it */
    DrawMyHouse( vertices, realVertices, rotatePoint );
    SRGP_sampleLocator( &measure );
    /* Test if mouse button down */
    if ( measure.buttonChord[0] == DOWN ) {
      /* Check if user clicked on quit button */
      if ( PickedQuitButton( measure.position, quitRect )) {
        endProgram = TRUE; /* Yes, we must quit program */
      } else {
        /* Set new rotation point */
        rotatePoint = measure.position;
      } /* if-else */
    } /* if */
  } while( !endProgram );

  /* Kill the mouse */
  SRGP_setInputMode( LOCATOR, INACTIVE );
  /* Load out the SRGP library. */
  SRGP_end();
} /* main */

/********************************************************************
 *                 DrawShape
 ********************************************************************
 * SYNOPSIS:     Draws the polygon.
 * CALLED BY:    DrawMyHouse, UserMain
 * PARAMETERS:   ( arrayType points )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the function SRGP_polygon it draws the
 *               shape described in the points array.
 *******************************************************************/
void DrawShape( arrayType points ) {
  /* Draw new polygon */
  SRGP_setColor( COLOR_BLACK );
  SRGP_polygon( NUM_POINTS, points );
} /* DrawShape */

/********************************************************************
 *                 DrawMyHouse
 ********************************************************************
 * SYNOPSIS:     Draws the house and rotates it.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( arrayType points, realArrayType reals,
 *                 point rotPt )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it draws a
 *               polygon.
 *******************************************************************/
void DrawMyHouse( arrayType points, realArrayType reals,
                  point rotPt ) {
          int x1 = SCRWD, y1 = SCRHT,
              x2 = 0, y2 = 0, /* Boundary containing polygon */
              i; /* Used in for loop */
  long double rtx,
              rty, /* Temporary copy of the rotate point */
              c, s, dx, dy; /* Used in parallel assignment */

  for ( i = 0; i < NUM_POINTS; i++ ) {
    if ( points[i].x < x1 ) {
      x1 = points[i].x;
    } /* if */
    if ( points[i].x > x2 ) {
      x2 = points[i].x;
    } /* if */
    if ( points[i].y < y1 ) {
      y1 = points[i].y;
    } /* if */
    if ( points[i].y > y2 ) {
      y2 = points[i].y;
    } /* if */
  } /* for */
  /* Clear out old polygon image */
  SRGP_setColor( COLOR_WHITE );
  SRGP_setFillStyle( SOLID );
  SRGP_fillRectangleCoord( x1-1, y1-1, x2+1, y2+1 );
  /* (Add one to boundary to make up for line thickness of 2) */
  /* Rotate the points */
  rtx = rotPt.x;
  rty = rotPt.y;
  c = cosl( ANGLE_INC );
  s = sinl( ANGLE_INC );
  for ( i = 0; i < NUM_POINTS; i++ ) {
    dx = reals[i].x - rtx;
    dy = reals[i].y - rty;
    reals[i].x = rtx + (( c * dx ) - ( s * dy ));
    reals[i].y = rty + (( s * dx ) + ( c * dy ));
    points[i].x = (int)reals[i].x;
    points[i].y = (int)reals[i].y;
  } /* for */
  DrawShape( points );
} /* DrawMyHouse */

/********************************************************************
 *                 MakeQuitButton
 ********************************************************************
 * SYNOPSIS:     Draws the quit button on the screen at the location
 *               passed. This is straight out of the book
 *               Introduction to Computer Graphics on page 35.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( rectangle buttonRect )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it clears the
 *             rectangle and the frames it, finally drawing the word
 *             'quit' in the center of it.
 *******************************************************************/
void MakeQuitButton( rectangle buttonRect ) {
  point centerOfButton,
        textOrigin;
  int width,
      height,
      descent;

  SRGP_setFillStyle( SOLID );
  SRGP_setColor( COLOR_WHITE );
  SRGP_fillRectangle( buttonRect );
  SRGP_setColor( COLOR_BLACK );
  SRGP_setLineWidth( 2 );
  SRGP_rectangle( buttonRect );
  SRGP_inquireTextExtent( "Quit", &width, &height, &descent );
  centerOfButton.x = ( buttonRect.bottom_left.x +
                       buttonRect.top_right.x ) / 2;
  centerOfButton.y = ( buttonRect.bottom_left.y +
                       buttonRect.top_right.y ) / 2;
  textOrigin.x = centerOfButton.x - ( width / 2 );
  textOrigin.y = centerOfButton.y - ( height / 2 );
  SRGP_text( textOrigin, "Quit" );
} /* MakeQuitButton */

/********************************************************************
 *                 IsMouseInRegion
 ********************************************************************
 * SYNOPSIS:     Checks to see if mouse press occurred over the
 *               rectangle. 
 * CALLED BY:    main
 * PARAMETERS:   ( point position, rectangle rect )
 * RETURNS:      boolean  TRUE if mouse in region
 * SIDE EFFECTS:
 * STRATEGY:     Compare mouse positon to rectangle boundaries.
 *******************************************************************/
boolean IsMouseInRegion( point position, rectangle rect ) {
  boolean TFflag;

  TFflag = TRUE; /* Assume mouse is in region. */
  if ( position.x < rect.bottom_left.x ) {
    TFflag = FALSE;
  } else {
    if ( position.x > rect.top_right.x ) {
      TFflag = FALSE;
    } /* if */
  } /* if-else */
  if ( position.y < rect.bottom_left.y ) {
    TFflag = FALSE;
  } else {
    if ( position.y > rect.top_right.y ) {
      TFflag = FALSE;
    } /* if */
  } /* if-else */
  return( TFflag );
} /* IsMouseInRegion */

/********************************************************************
 *                 PickedQuitButton
 ********************************************************************
 * SYNOPSIS:     Checks to see if user clicked on quit button.
 * CALLED BY:    main
 * PARAMETERS:   ( point location,
 *                 rectangle buttonRect )
 * RETURNS:      boolean  TRUE if mouse in region
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it checks to see
 *             if the mouse is located over the quit button.
 *******************************************************************/
boolean PickedQuitButton( point location, rectangle buttonRect ) {
  boolean inFlag;

  inFlag = FALSE;
  if ( IsMouseInRegion( location, buttonRect )) {
    inFlag = TRUE;
  } /* if */
  return( inFlag );
} /* PickedQuitButton */

