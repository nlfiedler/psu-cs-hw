/********************************************************************
 *
 *    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:   COMP 406 Assignments
 * MODULE:    Thermometer Simulator
 * FILE:      thermo.c
 *
 * CLASS:     COMP 406 - Computer Graphics
 * DATE:      1/25/95
 * AUTHOR:    Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date     Description
 *      ----   ----     -----------
 *       NF    01/22/95 Initial version
 *
 * DESCRIPTION:
 *      This is a project for COMP 406. Using the SRGP library this
 *      program attempts to simulate an Celsius thermometer. All
 *      the attributes are set from within the program source code,
 *      but could easily to extended to allow the user to change
 *      them at run-time.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #define IBM_PC
    #include "SRGP.h" 

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define SCRWD 360
    #define SCRHT 360

    #define QUIT  0 /* ID number for Quit button */
    #define QLX   155    /* Quit left side */
    #define QRX   QLX+50 /* Quit right side */
    #define QBY   20     /* Quit bottom */
    #define QTY   QBY+30 /* Quit top */

    #define SLX  100     /* Left side of slider */
    #define SRX  SLX+200 /* Right side of slider */
    #define SBY  100     /* Bottom of slider */
    #define STY  SBY+20  /* Top of slider */

/********************************************************************
 *                 Typedefs
 *******************************************************************/

/********************************************************************
 *                 Globals
 *******************************************************************/

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
    /* Draws the quit button */
    void MakeQuitButton( rectangle buttonRect );
    /* Draws the thermometer */
    void MakeThermometer( rectangle buttonRect );
    /* Draws the mercury in the thermometer */
    void DrawLiquid( point pt );
    /* Checks if mouse is in a rectangle */
    boolean IsMouseInRegion( point position, rectangle rect );
    /* Checks if mouse click over quit button */
    boolean PickedQuitButton( point location,
                              rectangle buttonRect );
    /* Checks if mouse click over slider */
    boolean PickedSlider( point location,
                          rectangle buttonRect );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Has the main event
 *               wait loop and some prolog/epilog code.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * STRATEGY:     Using the SRGP_waitEvent function main() loops
 *             continuously, checking any mouse events (particularly
 *             mouse clicks on the two objects: Quit button, and
 *             thermometer).
 *******************************************************************/
void UserMain() {
        rectangle quitRect,   /* Area for quit button */
                  slideRect;  /* Area for slider region */
          boolean terminate;  /* T/F flag to end program */
      inputDevice inputDev;   /* Indicates which input device */
   locatorMeasure measure;    /* Holds position of locator */
        rectangle rect;       /* Used in draw */

  /* Initialize SRGP library */
  SRGP_begin( "Thermometer", SCRWD, SCRHT, 0, 0 );

  /* Define quit and slider rectangles */
  quitRect = SRGP_defRectangle( QLX, QBY, QRX, QTY );
  slideRect = SRGP_defRectangle( SLX, SBY, SRX, STY );

  /* Draw quit button and thermometer */
  MakeQuitButton( quitRect );
  /* This one also draws the slider gadget */
  MakeThermometer( slideRect );

  /* Initialize mouse for wait event loop */
  SRGP_setLocatorButtonMask( LEFT_BUTTON_MASK );
  SRGP_setInputMode( LOCATOR, EVENT );
  terminate = FALSE; /* Assume we don't want to end program yet */
  do {  /* This is the indefinite loop in which the user selects
         * the slider or quit button. Quit ends the loop */
    inputDev = SRGP_waitEvent( INDEFINITE );
    /* Now we wait until mouse event */

    SRGP_getLocator( &measure );
    /* Test if mouse button down */
    if ( measure.buttonChord[0] == DOWN ) {
      /* Check if user clicked on quit button */
      if ( PickedQuitButton( measure.position, quitRect )) {
        terminate = TRUE; /* Yes, we must quit program */
      } else { /* No, now check if user clicked on the slider control */

        if ( PickedSlider( measure.position, slideRect )) {
          /* Sample loop here */
          SRGP_setInputMode( LOCATOR, SAMPLE );

          do { /* This is the sampling loop in which the user
                * moves the slider gadget back and forth */
            if ( PickedSlider( measure.position, slideRect )) {
              DrawLiquid( measure.position );
            } /* if */
            SRGP_sampleLocator( &measure );
          } while( measure.buttonChord[0] == DOWN );

          /* Done sampling, set locator back to event mode */
          SRGP_setInputMode( LOCATOR, EVENT );
        } /* if */
      } /* if-else */
    } /* if */
  } while( !terminate );

  /* Load out the SRGP library. */
  SRGP_end();
} /* main */

/********************************************************************
 *                 MakeQuitButton
 ********************************************************************
 * SYNOPSIS:     Draws the quit button on the screen at the location
 *               passed. This is straight out of the book
 *               Introduction to Computer Graphics on page 35.
 * CALLED BY:    main
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
 *                 MakeThermometer
 ********************************************************************
 * SYNOPSIS:     Draws the thermometer on the screen.
 * CALLED BY:    main
 * PARAMETERS:   ( rectangle buttonRect )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it draws the
 *             thermometer by calling several drawing primitives.
 *******************************************************************/
void MakeThermometer( rectangle buttonRect ) {
  rectangle bulbRect; /* Temporary rect for bulb */
        int sliderX;
      float value;

  /* Draw outside of bulb (case) */
  SRGP_setColor( COLOR_BLACK );
  SRGP_setFillStyle( SOLID );
  bulbRect = SRGP_defRectangle( buttonRect.bottom_left.x - 40,
                                buttonRect.bottom_left.y - 10,
                                buttonRect.bottom_left.x,
                                buttonRect.top_right.y + 10 );
  SRGP_fillEllipse( bulbRect );
  /* Draw outside of rod (case) */
  SRGP_setLineWidth( 2 );
  SRGP_rectangleCoord( buttonRect.bottom_left.x - 4,
                       buttonRect.bottom_left.y - 2,
                       buttonRect.top_right.x + 2,
                       buttonRect.top_right.y + 2 );
  SRGP_setColor( COLOR_WHITE );
  SRGP_fillRectangleCoord( buttonRect.bottom_left.x - 2,
                           buttonRect.bottom_left.y,
                           buttonRect.top_right.x - 2,
                           buttonRect.top_right.y );
  /* Draw slider gadget */
  SRGP_setColor( COLOR_BLACK );
  SRGP_fillRectangleCoord( buttonRect.bottom_left.x - 20,
                           buttonRect.bottom_left.y + 2,
                           buttonRect.bottom_left.x + 50,
                           buttonRect.top_right.y - 2 );
} /* MakeThermometer */

/********************************************************************
 *                 DrawLiquid
 ********************************************************************
 * SYNOPSIS:     Draws the mercury in the thermometer.
 * CALLED BY:    main
 * PARAMETERS:   ( point pt )
 * RETURNS:      
 * SIDE EFFECTS:
 * STRATEGY:     Draw the liquid part then draw the part absent of
 *               liquid.
 *******************************************************************/
void DrawLiquid( point pt ) {
  SRGP_setColor( COLOR_BLACK );
  SRGP_fillRectangleCoord( SLX, SBY + 2, pt.x, STY - 2 );
  SRGP_setColor( COLOR_WHITE );
  SRGP_fillRectangleCoord( pt.x, SBY + 2, SRX, STY - 2 );
} /* DrawLiquid */

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

/********************************************************************
 *                 PickedSlider
 ********************************************************************
 * SYNOPSIS:     Checks to see if user clicked on the slider.
 * CALLED BY:    main
 * PARAMETERS:   ( point location,
 *                 rectangle buttonRect )
 * RETURNS:      boolean  TRUE if mouse in region
 * SIDE EFFECTS:
 * STRATEGY:     Using the SRGP library functions it checks to see
 *             if the mouse is located over the slider.
 *******************************************************************/
boolean PickedSlider( point location, rectangle buttonRect ) {
  boolean inFlag;

  inFlag = FALSE;
  if ( IsMouseInRegion( location, buttonRect )) {
    inFlag = TRUE;
  } /* if */
  return( inFlag );
} /* PickedSlider */
