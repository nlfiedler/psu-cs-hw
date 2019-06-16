/********************************************************************
 *
 *    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:   COMP 406 Assignments
 * MODULE:    Moving Rectangles
 * FILE:      rectangl.c
 *
 * CLASS:     COMP 406 - Computer Graphics
 * DATE:      1/25/95
 * AUTHOR:    Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date     Description
 *      ----   ----     -----------
 *       NF    01/20/95 Initial version
 *
 * DESCRIPTION:
 *      This is a project for COMP 406. It moves a random number of
 *      rectangles around the graphics screen. It changes their colors
 *      randomly. Their initial positions and movement and growth
 *      rates are also random. After that, they bounce of the edges
 *      of the screen.
 *      This version is implemented using the Borland Graphics library.
 *
 * NOTE:
 *      Most of the graphics function calls and usage was taken
 *      from examples supplied in the TurboC++ online help system.
 *
 * REQUIRES:
 *      You must turn on "[X] Graphics Libraries" in the
 *      Options/Linker/Libraries. Otherwise, the source will
 *      link with errors on the calls to the BGI functions.
 *
 *******************************************************************/

/********************************************************************
 *                 Header Files
 *******************************************************************/
    #include <stdio.h>
    #include <string.h>
    #include <dos.h>
    #include <graphics.h>
    #include <stdlib.h>
    #include <conio.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE -1
    #define FALSE 0
    #define MAXRECTS 20  /* Maximum number of rectangles */
    #define MAXB4COLORCHANGE 100 /* Delay before color change */
    #define A 48271L      /* Number that works */
    #define M 2147483647L /* 2^31 - 1 */
    #define Q 44488L      /* M div A  */
    #define R 3399L       /* M mod A  */

/********************************************************************
 *                 Structures and Types
 *******************************************************************/
    typedef unsigned char boolean;
    /* Type definition for array record type */
    typedef struct {
      int xPos,
          yPos;       /* X and Y position of top-left */
      int width,
          height;    /* width and height of rectangle */
      int color,
          ticksB4change; /* color and time before change */
      char widthGrowDir,
           heightGrowDir;   /* Grow directions (Expand or Contract) */
      char vertDir,
           horzDir;         /* Move directions (Up or Down, L or R */
      int widthGrowthRate,
          heightGrowthRate; /* Growth rates (1-5 pixels) */
      int vertMoveRate,
          horzMoveRate;     /* Movement rates (1-5 pixels) */
    } arrayRecType;
    /* Type definition for array type */
    typedef arrayRecType arrayType[MAXRECTS];
    /* Type definition for list type */
    typedef struct {
      int numRects;
      arrayType array;
    } listType;

/********************************************************************
 *                 Globals
 *******************************************************************/
    int scrWidth,
        scrHeight; /* Screen width and height */
    int numColors; /* Maximum number of available colors */

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
    void InitList( listType *list );
    void InitGraphics( void );
    void DrawRects( listType *list );
    void MoveRects( listType *list );
    void ClipToEdges( arrayRecType *arrayRec );
    void CheckBounds( arrayRecType *arrayRec );
    void CheckSmall( arrayRecType *arrayRec );
    int myrandom( int num );
    boolean keyPressed( void );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     The main function of the program. Calls the other
 *               functions to do the work.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * SIDE EFFECTS:
 * STRATEGY:     Call other functions to produce several rectangles
 *             and move them around the screen.
 *******************************************************************/
int main( int argc, char *argv[] ) {
  listType rectList; /* List of rectangles */

  /* User wants some help or information */
  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "Rectangles  Copyright (C) 1995 Blue Marsh Softworks\n" );
    printf( "Draws rectangles that move around the screen.\n" );
    printf( "To start, type: rectangl\n" );
    printf( "To stop, press a key.\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  /* User invoked program improperly */
  if ( argc > 1 ) {
    printf( "For help, type: rectangl /?\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  InitGraphics(); /* Initialize graphics adaptor */
  InitList( &rectList ); /* Initialize rectangle list */
  do { /* Main drawing and moving loop */
    cleardevice(); /* Clear screen */
    DrawRects( &rectList ); /* Draw rectangles */
    MoveRects( &rectList ); /* Move rectangles */
    delay( 50 ); /* Short delay */
  } while ( keyPressed() == FALSE );
  closegraph(); /* Restore text screen */
  printf( "Thank you for using Rectangles.\n" );
  printf( "Copyright (C) 1995 Blue Marsh Softworks\n" );
  printf( "To start again, type: rectangl\n" );
  return( 0 );
} /* main */

/********************************************************************
 *                 InitGraphics
 ********************************************************************
 * SYNOPSIS:     Initialize the graphics adaptor using BGI functions.
 * CALLED BY:    main
 * PARAMETERS:   ( void )
 * RETURNS:      void
 * SIDE EFFECTS: Exits to DOS if error in graphics.
 * STRATEGY:     Call BGI functions and set global variables.
 *******************************************************************/
void InitGraphics( void ) {
  /* Request auto detection from graphics library */
  int gdriver = DETECT,
      gmode,
      errorcode;

  /* Register all the video drivers */
  errorcode = registerbgidriver( EGAVGA_driver );
  if ( errorcode < 0 ) {
    printf( "Graphics error: %s\n", grapherrormsg( errorcode ));
    exit( 1 );
  }
  /* Initialize graphics and local variables */
  initgraph( &gdriver, &gmode, "" );
  /* Read result of initialization */
  errorcode = graphresult();
  if ( errorcode != grOk ) { /* An error occurred */
    printf( "Graphics error: %s\n", grapherrormsg( errorcode ));
    exit( 1 ); /* Exit to DOS with error code */
  } /* if */
  scrWidth = getmaxx();      /* Get screen width */
  scrHeight = getmaxy();     /* Get screen height */
  numColors = getmaxcolor(); /* Get maximum number of colors */
  /* Set line style for all the rectangles */
  setlinestyle( SOLID_LINE, EMPTY_FILL, NORM_WIDTH );
} /* InitGraphics */

/********************************************************************
 *                 InitList
 ********************************************************************
 * SYNOPSIS:     Initialize the rectangle list
 * CALLED BY:    main
 * PARAMETERS:   ( listType *list )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Use random numbers to start of the rectangles in
 *             various positions, sizes, and directions.
 *******************************************************************/
void InitList( listType *list ) {
  int i; /* Used in for loop */
  int j; /* Holds random number */

  list->numRects = myrandom( MAXRECTS ) + 1;
  for ( i = 0; i < list->numRects; i++ ) {
    list->array[i].xPos = myrandom( scrWidth );
    list->array[i].yPos = myrandom( scrHeight );
    list->array[i].width = myrandom( scrWidth -
                             list->array[i].xPos );
    list->array[i].height = myrandom( scrHeight -
                             list->array[i].yPos );
    list->array[i].color = myrandom( numColors );
    list->array[i].ticksB4change = myrandom( 1000 );
    j = myrandom( 100 );
    if ( j < 50 ) {
      list->array[i].widthGrowDir = 'E';
    } else {
      list->array[i].widthGrowDir = 'C';
    } /* if-else */
    j = myrandom( 100 );
    if ( j < 50 ) {
      list->array[i].heightGrowDir = 'E';
    } else {
      list->array[i].heightGrowDir = 'C';
    } /* if-else */
    j = myrandom( 100 );
    if ( j < 50 ) {
      list->array[i].vertDir = 'U';
    } else {
      list->array[i].vertDir = 'D';
    } /* if-else */
    j = myrandom( 100 );
    if ( j < 50 ) {
      list->array[i].horzDir = 'L';
    } else {
      list->array[i].horzDir = 'R';
    } /* if-else */
    /* Make sure it moves faster than it grows */
    list->array[i].widthGrowthRate = myrandom( 3 ) + 1;
    list->array[i].heightGrowthRate = myrandom( 3 ) + 1;
    list->array[i].vertMoveRate = myrandom( 12 ) + 1;
    list->array[i].horzMoveRate = myrandom( 12 ) + 1;
  } /* for */
} /* InitList */

/******************************************************************
 *                 DrawRects
 ******************************************************************
 * SYNOPSIS:     Draws the rectangles on the screen.
 * CALLED BY:    main
 * PARAMETERS:   ( listType *list )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using a for loop, DrawRects will draw each of the
 *             rectangles as they are currently defined in the
 *             list.
 *****************************************************************/
void DrawRects( listType *list ) {
  int i; /* Used in for loop */

  for ( i = 0; i < list->numRects; i++ ) {
    setcolor( list->array[i].color );
    rectangle( list->array[i].xPos,
               list->array[i].yPos,
               list->array[i].xPos + list->array[i].width,
               list->array[i].yPos + list->array[i].height );
  } /* for */
} /* DrawRects */

/******************************************************************
 *                 MoveRects
 ******************************************************************
 * SYNOPSIS:     Moves the rectangles around the screen.
 * CALLED BY:    main
 * PARAMETERS:   ( listType *list )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using a for loop, MoveRects will move the
 *             rectangles as they are currently defined to move
 *             in the list.
 *****************************************************************/
void MoveRects( listType *list ) {
  int i; /* Used in for loop */
  arrayRecType arrayRec;

  for ( i = 0; i < list->numRects; i++ ) {
    arrayRec = list->array[i];
    CheckBounds( &arrayRec );
    if ( arrayRec.widthGrowDir == 'E' ) {
      arrayRec.width += arrayRec.widthGrowthRate;
    } else {
      arrayRec.width -= arrayRec.widthGrowthRate;
    } /* if-else */
    if ( arrayRec.heightGrowDir == 'E' ) {
      arrayRec.height += arrayRec.heightGrowthRate;
    } else {
      arrayRec.height -= arrayRec.heightGrowthRate;
    } /* if-else */
    if ( arrayRec.vertDir == 'U' ) {
      arrayRec.yPos -= arrayRec.vertMoveRate;
    } else {
      arrayRec.yPos += arrayRec.vertMoveRate;
    } /* if-else */
    if ( arrayRec.horzDir == 'L' ) {
      arrayRec.xPos -= arrayRec.horzMoveRate;
    } else {
      arrayRec.xPos += arrayRec.horzMoveRate;
    } /* if-else */
    ClipToEdges( &arrayRec );
    CheckSmall( &arrayRec );
    arrayRec.ticksB4change--;
    if ( arrayRec.ticksB4change == 0 ) {
      arrayRec.color = myrandom( numColors );
      arrayRec.ticksB4change = myrandom( MAXB4COLORCHANGE );
    } /* if */
    list->array[i] = arrayRec;
  } /* for */
} /* MoveRects */

/******************************************************************
 *                 ClipToEdges
 ******************************************************************
 * SYNOPSIS:     Makes sure rectangle boundaries do not exceed
 *               screen edges.
 * CALLED BY:    MoveRects
 * PARAMETERS:   ( arrayRecType *arrayRec )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Uses the screen boundaries and some simple
 *               arithmetic to assure that the rectangle will not
 *               draw outside of the screen edges.
 *****************************************************************/
void ClipToEdges( arrayRecType *arrayRec ) {
  if ( arrayRec->xPos < 0 ) {
    arrayRec->xPos = 0;
  } else {
    if (( arrayRec->xPos + arrayRec->width ) > scrWidth ) {
      arrayRec->width = scrWidth - arrayRec->xPos;
    } /* if */
  } /* if-else */
  if ( arrayRec->yPos < 0 ) {
    arrayRec->yPos = 0;
  } else {
    if (( arrayRec->yPos + arrayRec->height ) > scrHeight ) {
      arrayRec->height = scrHeight - arrayRec->yPos;
    } /* if */
  } /* if-else */
} /* ClipToEdges */

/******************************************************************
 *                 CheckBounds
 ******************************************************************
 * SYNOPSIS:     Causes rectangle to reverse direction and growth
 *               if it hits the edge of the screen.
 * CALLED BY:    MoveRects
 * PARAMETERS:   ( arrayRecType *arrayRec )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Simply checks the boundaries and if they match
 *             CheckBounds will reverse the appropriate direction
 *             and growth direction of the rectangle.
 *****************************************************************/
void CheckBounds( arrayRecType *arrayRec ) {
  if ( arrayRec->xPos == 0 ) {
    arrayRec->horzDir = 'R';
  } /* if */
  if ( arrayRec->yPos == 0 ) {
    arrayRec->vertDir = 'D';
  } /* if */
  if (( arrayRec->xPos + arrayRec->width ) >= scrWidth ) {
    arrayRec->horzDir = 'L';
    arrayRec->widthGrowDir = 'C';
  } /* if */
  if (( arrayRec->yPos + arrayRec->height ) >= scrHeight ) {
    arrayRec->vertDir = 'U';
    arrayRec->heightGrowDir = 'C';
  } /* if */
} /* CheckBounds */

/******************************************************************
 *                 CheckSmall
 ******************************************************************
 * SYNOPSIS:     Checks to see if rectangle is too small and
 *               fixes it by swapping the rates.
 * CALLED BY:    MoveRects
 * PARAMETERS:   ( arrayRecType *arrayRec )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Simply check the width and height and switch the
 *               rates around to make the rectangle operate properly.
 *****************************************************************/
void CheckSmall( arrayRecType *arrayRec ) {
/*  int tempRate;  Holds growth rate for swapping */

  if ( arrayRec->width <= 0 ) {
    arrayRec->width = 0 - arrayRec->width;
    arrayRec->widthGrowDir = 'E';
/*    tempRate = arrayRec->horzMoveRate;
    arrayRec->horzMoveRate = arrayRec->widthGrowthRate;
    arrayRec->widthGrowthRate = tempRate; */
  } /* if */
  if ( arrayRec->height <= 0 ) {
    arrayRec->height = 0 - arrayRec->height;
    arrayRec->heightGrowDir = 'E';
/*    tempRate = arrayRec->vertMoveRate;
    arrayRec->vertMoveRate = arrayRec->heightGrowthRate;
    arrayRec->heightGrowthRate = tempRate; */
  } /* if */
} /* CheckSmall */

/******************************************************************
 *                 myrandom
 ******************************************************************
 * SYNOPSIS:     Produce a random number between 0 and num-1
 *               using the function given in Data Structures
 *               and Algorithm Analysis by Mark Allen Weiss.
 * CALLED BY:
 * PARAMETERS:   ( int num )
 * RETURNS:      int  Random number between 0 and num-1
 * SIDE EFFECTS:
 * STRATEGY:     Using a seed value and the / and % operators,
 *             the function will produce a random number. It is
 *             a much more useful algorithm than the typical
 *             linear congruential functions that are widely used.
 *             This formula is broken up to allow it to work on
 *             most machines. There is a simpler formula but does
 *             not work on most machines.
 *               The problem with this function is that the values
 *             always start with the same seed. If you try to use
 *             randomize and random you get the same thing anyway.
 *             You could also get a situation where Seed will
 *             always be zero. So, an arbitrary value was choosen.
 * NEEDS:        The #defines for A = 48271, M = 2147483647,
 *               Q = 44488, and R = 3399
 *****************************************************************/
int myrandom( int num ) {
  static long Seed = 32853L; /* A static seed value
                             * You could use random, but does
                             * same thing, or worse */
  long TmpSeed; /* Used in calculation */
  float returnValue; /* Here for calculation of return value */

  TmpSeed = A * ( Seed % Q ) - R * ( Seed / Q );
  if ( TmpSeed >= 0 ) {
    Seed = TmpSeed;
  } else {
    Seed = TmpSeed + M;
  } /* if-else */
  returnValue = Seed;
  returnValue = (( returnValue / M ) * num );
  return( (int)returnValue );
} /* myrandom */

/********************************************************************
 *                 keyPressed
 ********************************************************************
 * SYNOPSIS:     Detects if a key was pressed. Operates similar to
 *               the TurboPascal equivalent. This function came from
 *               C: The Complete Reference by Herbert Schildt.
 * CALLED BY:
 * PARAMETERS:   ( void )
 * RETURNS:      boolean  TRUE if key was pressed
 * SIDE EFFECTS:
 * STRATEGY:     Uses bdos function to detect if a key was pressed.
 *******************************************************************/
boolean keyPressed( void ) {
  char kp;

  kp = (char)bdos( 0xB, 0, 0 );
  if ( kp == 0 ) {
    return( FALSE );
  } else {
    return( TRUE );
  } /* if-else */
} /* keyPressed */

