/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 406 Assignments
 * MODULE:       Rotate by Matrix
 * FILE:         rotate2.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name    Date      Description
 *      ----    ----      -----------
 *       NF     03/01/95  Initial version
 *
 * DESCRIPTION:
 *      This program rotates a set of points (a polygon) around
 *      a user specified point, counter-clockwise. The user can
 *      repeatedly click the mouse to have the polygon move
 *      around that point.
 *      This new version uses the original position of the
 *      polygon and a transformation matrix to move it to
 *      whereever it belongs.
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

    #define QLX   5      /* Quit box boundaries */
    #define QRX   QLX+50
    #define QBY   5
    #define QTY   QBY+30

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    typedef point *arrayType;
    typedef struct matrixMType {
      long double a,
                  b,
                  phi;
    };

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    /* Draws the picture */
    void DrawMyHouse( arrayType vertices, arrayType original,
                      point rotPt, struct matrixMType *M );
    /* Apply the transformation to the points */
    void ApplyTransform( arrayType points, arrayType original,
                         point rotPt, struct matrixMType *M );
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
 * PARAMETERS:   ( void )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Rotate a set of points around a point until the
 *               user clicks on another point, then rotate around
 *               that new point. Exit the program when the user
 *               clicks on the quit button.
 *******************************************************************/
void UserMain( void ) {
           point rotatePoint;  /* Current center of rotation */
       arrayType vertices,     /* Set of points for polygonv view */
                 origPoints;   /* Original points */
         boolean endProgram;   /* T/F flag to end program */
       rectangle quitRect;     /* Rectangle of quit button */
  locatorMeasure measure;      /* Mouse event record */
  struct matrixMType abphi;

  /* Start up the SRGP library */
  SRGP_begin( "Rotate", SCRWD, SCRHT, 0, 0 );
  /* Initialize rotation point */
  rotatePoint.x = 275; /* Center of object */
  rotatePoint.y = 210;
  /* Initialize the abphi struct */
  abphi.a = 0;
  abphi.b = 0;
  abphi.phi = 0;
  /* Allocate space for vertices array */
  if (( vertices = (arrayType)calloc( NUM_POINTS,
                   sizeof( point ))) == 0 ) {
    printf( "Error allocating memory for polygon points.\n" );
    exit( 1 ); /* Exit to OS with error */
  } /* if */
  if (( origPoints = (arrayType)calloc( NUM_POINTS,
                     sizeof( point ))) == 0 ) {
    printf( "Error allocating memory for original points.\n" );
    exit( 1 ); /* Exit to OS with error */
  } /* if */
  /* Set points for a house */
  vertices[0] = SRGP_defPoint( 250, 180 );
  origPoints[0] = vertices[0];
  vertices[1] = SRGP_defPoint( 300, 180 );
  origPoints[1] = vertices[1];
  vertices[2] = SRGP_defPoint( 300, 220 );
  origPoints[2] = vertices[2];
  vertices[3] = SRGP_defPoint( 275, 240 );
  origPoints[3] = vertices[3];
  vertices[4] = SRGP_defPoint( 250, 220 );
  origPoints[4] = vertices[4];
  /* Define quit rectangle */
  quitRect = SRGP_defRectangle( QLX, QBY, QRX, QTY );
  /* Draw quit button */
  MakeQuitButton( quitRect );
  /* Initialize mouse for sample loop */
  SRGP_setLocatorButtonMask( LEFT_BUTTON_MASK );
  SRGP_setInputMode( LOCATOR, SAMPLE );
  /* Assume user doesn't want to quit yet */
  endProgram = FALSE;
  do {
    /* Draw picture, also rotates it */
    DrawMyHouse( vertices, origPoints, rotatePoint, &abphi );
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
  /* Load out the SRGP library */
  SRGP_end();
} /* main */

/********************************************************************
 *                 DrawMyHouse
 ********************************************************************
 * SYNOPSIS:     Draws the polygon.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( arrayType points, realArrayType reals,
 *                 point rotPt )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it draws a
 *               polygon.
 *******************************************************************/
void DrawMyHouse( arrayType points, arrayType original,
                  point rotPt, struct matrixMType *M ) {
          int x1 = SCRWD, y1 = SCRHT,
              x2 = 0, y2 = 0, /* Boundary containing polygon */
              i; /* Used in for loop */

  /* Find outer boundaries of rotated polygon image */
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
  /* (Add one to boundary to make up for line thickness of two) */
  /* Rotate the points */
  ApplyTransform( points, original, rotPt, M );
  /* Draw new polygon */
  SRGP_setColor( COLOR_BLACK );
  SRGP_polygon( NUM_POINTS, points );
} /* DrawMyHouse */

/********************************************************************
 *                 ApplyTransform
 ********************************************************************
 * SYNOPSIS:     Applies the transformation to the set of points.
 * CALLED BY:    DrawMyHouse
 * PARAMETERS:   ( arrayType points, arrayType original,
 *                 point rotPt, struct matrixMType *M )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Uses simple mathematics to alter the position of
 *               the set of points passed. The apparent result is
 *               the object being moved and rotated from it's
 *               original position to the view position.
 *******************************************************************/
void ApplyTransform( arrayType points, arrayType original,
                     point rotPt, struct matrixMType *M ) {
  long double rtx,
              rty, /* Temporary copy of the rotate point */
              c, s, dx, dy; /* Used in parallel assignment */
          int i; /* Used in for loop */

  rtx = rotPt.x; /* Copy point to real variables */
  rty = rotPt.y; /* for FP manipulation */
  c = cosl( ANGLE_INC ); /* Precalculate some values */
  s = sinl( ANGLE_INC );
  dx = M->a - rtx;
  dy = M->b - rty;
  M->phi += ANGLE_INC; /* Increase angle */
  /* Find new a and b values */
  M->a = ( c * dx ) - ( s * dy ) + rtx;
  M->b = ( s * dx ) + ( c * dy ) + rty;
  /* Apply the transformation to the set of points */
  for ( i = 0; i < NUM_POINTS; i++ ) {
    points[i].x = cosl( M->phi ) * original[i].x -
		  sinl( M->phi ) * original[i].y + M->a;
    points[i].y = sinl( M->phi ) * original[i].x +
		  cosl( M->phi ) * original[i].y + M->b;
  } /* for */
} /* ApplyTransform */

/********************************************************************
 *                 MakeQuitButton
 ********************************************************************
 * SYNOPSIS:     Draws the quit button on the screen at the
 *               location passed. This is from the book
 *               Introduction to Computer Graphics (p35).
 * CALLED BY:    UserMain
 * PARAMETERS:   ( rectangle buttonRect )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it clears the
 *               rectangle and then frames it, finally draw the
 *               word 'Quit' in the center of it.
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
                       buttonRect.top_right.x ) /2;
  centerOfButton.y = ( buttonRect.bottom_left.y +
                       buttonRect.top_right.y ) /2;
  textOrigin.x = centerOfButton.x - ( width / 2 );
  textOrigin.y = centerOfButton.y - ( height / 2 );
  SRGP_text( textOrigin, "Quit" );
} /* MakeQuitButton */

/********************************************************************
 *                 IsMouseInRegion
 ********************************************************************
 * SYNOPSIS:     Checks to see if mouse press occurred over the
 *               rectangle.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( point position, rectangle rect )
 * RETURNS:      boolean - TRUE if mouse in region
 * SIDE EFFECTS:
 * STRATEGY:     Compare mouse position to rectangle boundaries.
 *******************************************************************/
boolean IsMouseInRegion( point position, rectangle rect ) {
  boolean TFflag;

  TFflag = TRUE; /* Assume mouse is in region */
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
 * CALLED BY:    UserMain
 * PARAMETERS:   ( point location, rectangle buttonRect )
 * RETURNS:      boolean - TRUE if mouse on button
 * SIDE EFFECTS:
 * STRATEGY:     Using the IsMouseInRegion function it checks to
 *               see if the mouse is located over the quit button.
 *******************************************************************/
boolean PickedQuitButton( point location, rectangle buttonRect ) {
  boolean inFlag;

  inFlag = FALSE;
  if ( IsMouseInRegion( location, buttonRect )) {
    inFlag = TRUE;
  } /* if */
  return( inFlag );
} /* PickedQuitButton */
