/********************************************************************
 *
 *   Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 406 Assignments
 * MODULE:       3D Box Rotate
 * FILE:         3dbox.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date       Description
 *      ----   ----       -----------
 *       NF    03/11/95   Initial version
 *       NF    03/24/95   Used fixed point and look-up tables.
 *       NF    03/25/95   Added pyramid object.
 *
 * DESCRIPTION:
 *      This program implements a 3D viewing system in which the
 *      user can move around in a 3D universe. This universe
 *      contains a wire-frame cube with a P on the side.
 *      Much of the mathematics and algorithms came from the book
 *      Flights of Fantasy by Christopher Lampton.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <alloc.h>
    #define IBM_PC
    #include "SRGP.h"

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define SCRWD       440 /* Width of SRGP screen. */
    #define SCRHT       440 /* Height of SRGP screen. */
    #define PERSPECTIVE 350 /* Perspective projection value. */
    #define PARALLEL    4   /* Parallel projection value. */
    #define ANGLE_INC   10  /* Angle change for each frame. */
    #define TRANS_INC   5   /* Motion for each frame. */
    #define SENSITIVE   0   /* Sensitivity of the mouse. */
    #define SHIFT9      9   /* Number of fractional binary digits. */
    #define ONE         1L<<SHIFT9 /* One shifted to fixed point. */

/********************************************************************
 *                 Macros
 *******************************************************************/
    #define COS(X) cos_table[X]
    #define SIN(X) sin_table[X]

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    /* Matrix and vector element type */
    typedef long elemType;
    /* Vertex (point) type */
    typedef struct {
      int      wx, wy, wz; /* World coordinates */
      elemType ax, ay, az; /* Aligned coordinates */
      int      sx, sy;     /* Screen coordinates */
    } vertexType;
    /* Line type (connects points) */
    typedef struct {
      int start; /* Vertex # for start of line */
      int end;   /* Vertex # for end of line */
    } lineType;
    /* Shape type (combines vertices and lines) */
    typedef struct {
      int        numVerts;  /* Number of vertices */
      int        numLines;  /* Number of lines */
      vertexType *vertices; /* Pointer to array of vertices */
      lineType   *lines;    /* Pointer to array of lines */
    } shapeType, *shapePtr;
    /* Matrix pointer type */
    typedef elemType *matrixPtr; /* Pointer to matrix */

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    /* Initialize shape structure to that of a cube with a P. */
    void InitShape( shapePtr shape );
    /* Apply the transformation to the shape. */
    void Transform( matrixPtr m, shapePtr shape );
    /* Project world coordinates onto screen coordinates. */
    void Project( shapePtr shape, int dist, boolean para );
    /* Draw shape on screen. */
    void DrawShape( shapePtr shape, int X, int Y );
    /* Translate the matrix using the values x,y,z. */
    void Translate( matrixPtr m, int x, int y, int z );
    /* Rotate the matrix using the values xa,ya,za. */
    void Rotate( matrixPtr m, int xa, int ya, int za );
    /* Allocate space for a matrix. */
    matrixPtr AllocMatrix( void );
    /* Multiply two 4x4 matrixes and return result. */
    void MatrixMult( matrixPtr m1, matrixPtr m2, matrixPtr result );
    /* Copy source matrix into dest matrix. */
    void MatrixCopy( matrixPtr source, matrixPtr dest );
    /* Return the identity matrix. */
    void MatrixIdentity( matrixPtr m );

/********************************************************************
 *                 Globals
 *******************************************************************/
  /* These two tables are used by the COS and SIN macros to find
   * the value of the cosine and sine of an angle.
   * The values are in fixed point format. */
long cos_table[360]={
    512, 511, 511, 511, 510, 510, 509, 508, 
    507, 505, 504, 502, 500, 498, 496, 494, 
    492, 489, 486, 484, 481, 478, 474, 471, 
    467, 464, 460, 456, 452, 447, 443, 438, 
    434, 429, 424, 419, 414, 409, 403, 398, 
    392, 386, 380, 374, 368, 362, 355, 349, 
    342, 336, 329, 322, 315, 308, 301, 293, 
    286, 279, 271, 263, 256, 248, 240, 232, 
    224, 216, 208, 200, 192, 183, 175, 166, 
    158, 150, 141, 132, 124, 115, 106, 98, 
    89, 80, 71, 62, 53, 45, 36, 27, 
    18, 9, 0, -8, -17, -26, -35, -44, 
    -53, -61, -70, -79, -88, -97, -105, -114, 
    -123, -132, -140, -149, -157, -166, -174, -183, 
    -191, -199, -207, -215, -223, -231, -239, -247, 
    -255, -263, -270, -278, -285, -293, -300, -307, 
    -314, -321, -328, -335, -342, -348, -355, -361, 
    -367, -374, -380, -385, -391, -397, -403, -408, 
    -413, -419, -424, -429, -433, -438, -443, -447, 
    -451, -455, -459, -463, -467, -471, -474, -477, 
    -480, -483, -486, -489, -491, -494, -496, -498, 
    -500, -502, -504, -505, -506, -508, -509, -509, 
    -510, -511, -511, -511, -511, -511, -511, -511, 
    -510, -510, -509, -508, -507, -505, -504, -502, 
    -500, -499, -497, -494, -492, -489, -487, -484, 
    -481, -478, -475, -471, -468, -464, -460, -456, 
    -452, -448, -443, -439, -434, -429, -425, -419, 
    -414, -409, -404, -398, -392, -387, -381, -375, 
    -369, -362, -356, -349, -343, -336, -329, -323, 
    -316, -308, -301, -294, -287, -279, -272, -264, 
    -256, -249, -241, -233, -225, -217, -209, -201, 
    -192, -184, -176, -167, -159, -150, -142, -133, 
    -124, -116, -107, -98, -90, -81, -72, -63, 
    -54, -45, -36, -28, -19, -10, -1, 7, 
    16, 25, 34, 43, 52, 61, 70, 78, 
    87, 96, 105, 113, 122, 131, 139, 148, 
    156, 165, 173, 182, 190, 198, 207, 215, 
    223, 231, 239, 247, 254, 262, 270, 277, 
    285, 292, 299, 307, 314, 321, 328, 334, 
    341, 348, 354, 361, 367, 373, 379, 385, 
    391, 396, 402, 408, 413, 418, 423, 428, 
    433, 438, 442, 447, 451, 455, 459, 463, 
    467, 470, 474, 477, 480, 483, 486, 489, 
    491, 494, 496, 498, 500, 502, 503, 505, 
    506, 507, 509, 509, 510, 511, 511, 511, 
}; /* cos_table[] */

long sin_table[360]={
    -1, 7, 16, 25, 34, 42, 51, 60, 
    69, 78, 87, 96, 104, 113, 122, 130, 
    139, 148, 156, 165, 173, 181, 190, 198, 
    206, 214, 222, 230, 238, 246, 254, 262, 
    269, 277, 284, 292, 299, 306, 313, 320, 
    327, 334, 341, 347, 354, 360, 367, 373, 
    379, 385, 391, 396, 402, 407, 413, 418, 
    423, 428, 433, 437, 442, 446, 451, 455, 
    459, 463, 466, 470, 473, 477, 480, 483, 
    486, 489, 491, 494, 496, 498, 500, 502, 
    503, 505, 506, 507, 508, 509, 510, 511, 
    511, 511, 511, 511, 511, 511, 510, 510, 
    509, 508, 507, 506, 504, 502, 501, 499, 
    497, 495, 492, 490, 487, 484, 481, 478, 
    475, 472, 468, 464, 461, 457, 453, 448, 
    444, 439, 435, 430, 425, 420, 415, 410, 
    404, 399, 393, 387, 381, 375, 369, 363, 
    357, 350, 344, 337, 330, 323, 316, 309, 
    302, 295, 288, 280, 273, 265, 257, 250, 
    242, 234, 226, 218, 210, 202, 193, 185, 
    177, 168, 160, 151, 143, 134, 126, 117, 
    108, 100, 91, 82, 73, 64, 55, 47, 
    38, 29, 20, 11, 2, -6, -15, -24, 
    -33, -42, -51, -59, -68, -77, -86, -95, 
    -104, -112, -121, -130, -138, -147, -155, -164, 
    -172, -181, -189, -197, -205, -214, -222, -230, 
    -238, -245, -253, -261, -269, -276, -284, -291, 
    -298, -306, -313, -320, -327, -333, -340, -347, 
    -353, -360, -366, -372, -378, -384, -390, -396, 
    -401, -407, -412, -417, -422, -427, -432, -437, 
    -442, -446, -450, -454, -458, -462, -466, -470, 
    -473, -477, -480, -483, -486, -488, -491, -493, 
    -496, -498, -500, -502, -503, -505, -506, -507, 
    -508, -509, -510, -511, -511, -511, -511, -511, 
    -511, -511, -510, -510, -509, -508, -507, -506, 
    -504, -503, -501, -499, -497, -495, -492, -490, 
    -487, -485, -482, -479, -475, -472, -468, -465, 
    -461, -457, -453, -449, -444, -440, -435, -431, 
    -426, -421, -415, -410, -405, -399, -394, -388, 
    -382, -376, -370, -364, -357, -351, -344, -338, 
    -331, -324, -317, -310, -303, -296, -288, -281, 
    -273, -266, -258, -250, -243, -235, -227, -219, 
    -211, -202, -194, -186, -178, -169, -161, -152, 
    -144, -135, -126, -118, -109, -100, -92, -83, 
    -74, -65, -56, -47, -38, -30, -21, -12, 
}; /* sin_table[] */

/********************************************************************
 *                 UserMain
 ********************************************************************
 * SYNOPSIS:     The main function of the program. Contains the
 *               main sampling loop and calls the other functions.
 * PARAMETERS:   ( void )
 * RETURNS:      void
 * CALLED BY:    Operating system
 * SIDE EFFECTS:
 * STRATEGY:     Initialize SRGP, mouse, transformation matrix, and
 *               the shape data. Then enter a mouse sampling loop
 *               in which the user will move through 3D space as
 *               s/he moves the mouse. Quit when both buttons are
 *               pressed.
 *******************************************************************/
void UserMain( void ) {
  matrixPtr      M;       /* Master transformation matrix */
  shapePtr       Shape;   /* The shape data */
  locatorMeasure measure; /* Mouse event record */
  char           keyboard[3]; /* Keyboard measure. */
  boolean        endProgram;  /* T/F flag to end program. */
  boolean        parallel;    /* T/F flag for parallel projection. */
  int            /* Previous sampling of mouse position */
                 lastX, lastY,
                 /* Change in mouse position */
                 changeX, changeY,
                 /* Angle of viewer */
                 xrot = 0, yrot = 0, zrot = 0,
                 /* World coordinates of viewer,
                  * with an initial z value. */
                 xt = 0, yt = 0, zt = 100;
  int            paraDist, perDist; /* Distances to viewer. */

  /* Do all initialization. */
  M = AllocMatrix(); /* Allocate space for master matrix. */
  /* Allocate space for shape. */
  if (( Shape = (shapePtr)malloc( sizeof( shapeType ))) == 0 ) {
    printf( "Error allocating memory in main().\n" );
    exit( 1 );
  } /* if */
  /* Initialize shape as a cube. */
  InitShape( Shape );
  /* Initialize SRGP library. */
  SRGP_begin( "3D Rotating Box", SCRWD, SCRHT, 0, 0 );
  /* Set input mode of mouse. */
  SRGP_setInputMode( LOCATOR, SAMPLE );
  SRGP_setInputMode( KEYBOARD, SAMPLE );
  SRGP_setKeyboardProcessingMode( RAW );
  /* Get initial mouse location. */
  SRGP_sampleLocator( &measure );
  lastX = measure.position.x;
  lastY = measure.position.y;
  /* Assume user doesn't want to quit yet. */
  endProgram = FALSE;
  /* Assume parallel project is off. */
  parallel = FALSE;
  /* Set initial viewer distances. */
  paraDist = PARALLEL;
  perDist = PERSPECTIVE;

  do {
    SRGP_setKeyboardMeasure( "   " );
    /* Changing 3 to 1 will effectively disable keyboard. */
    SRGP_sampleKeyboard( keyboard, 3 );
    /* Check if user changing parallel/perspective projection. */
    if ( keyboard[0] == 'p' ) {
      if ( parallel ) {
        parallel = FALSE;
      } else {
        parallel = TRUE;
      } /* if-else */
    } /* if */
    /* Check if changing focal length (distance). */
    if ( keyboard[0] == '-' ) {
      if ( parallel ) {
        paraDist -= 1;
        if ( paraDist < 0 ) {
          paraDist = 0;
        } /* if */
      } else {
        perDist -= 10;
        if ( perDist < 0 ) {
          perDist = 0;
        } /* if */
      } /* if-else */
    } /* if */
    if ( keyboard[0] == '=' ) {
      if ( parallel ) {
        paraDist += 1;
      } else {
        perDist += 10;
      } /* if-else */
    } /* if */
    SRGP_sampleLocator( &measure );
    /* Check if user wants to quit.
     * User quits by pressing both mouse buttons. */
    if (( measure.buttonChord[0] == DOWN ) &&
        ( measure.buttonChord[2] == DOWN )) {
      endProgram = TRUE;
    } else {
      /* Make adjustments to matrix according to mouse movement. */
      changeX = measure.position.x - lastX;
      changeY = lastY - measure.position.y;
      /* Update last positions. */
      lastX = measure.position.x;
      lastY = measure.position.y;
      if (( abs( changeX ) > SENSITIVE ) ||
          ( abs( changeY ) > SENSITIVE )) {
        if (( measure.buttonChord[0] == UP ) &&
            ( measure.buttonChord[2] == UP )) {
          /* Neither button is pressed.
           * NOTE: If the user presses the right button
           * the loop will not move the box.
           * This is useful for moving the mouse pointer
           * without moving the box. */
          if ( abs( changeX ) > abs( changeY ) ) {
            /* Check if moving around sideways. */
            if ( changeX > 0 ) {
              yrot += ANGLE_INC;
            } else {
              yrot -= ANGLE_INC;
            } /* if-else */
            if ( yrot > 359 ) {
              yrot = 0;
            } /* if */
            if ( yrot < 0 ) {
              yrot = 360 - ANGLE_INC;
            } /* if */
          } else {
            /* Check if moving over or under. */
            if ( changeY > 0 ) {
              xrot += ANGLE_INC;
            } else {
              xrot -= ANGLE_INC;
            } /* if-else */
            if ( xrot > 359 ) {
              xrot = 0;
            } /* if */
            if ( xrot < 0 ) {
              xrot = 360 - ANGLE_INC;
            } /* if */
          } /* if-else */
        } else {
          if ( measure.buttonChord[0] == DOWN ) {
            /* Left button is pressed. */
            /* Check if moving towards or away. */
            if ( changeY > 0 ) {
              zt += TRANS_INC;
            } else {
              zt -= TRANS_INC;
            } /* if-else */
          } /* if */
        } /* if-else */
        /* Initialize transformation matrix to identity. */
        MatrixIdentity( M );
        /* Translate matrix according to mouse movements previously. */
        Translate( M, xt, yt, zt );
        /* Rotate matrix according to mouse. */
        Rotate( M, xrot, yrot, zrot );
        /* Transform shape using matrix. */
        Transform( M, Shape );
        /* Project shape onto screen coordinates. */
        if ( parallel ) {
          Project( Shape, paraDist, parallel );
        } else {
          /* Pass the distance from the screen to the
           * viewer (effects perspective). */
          Project( Shape, perDist, parallel );
        } /* if-else */
        /* Clear viewport */
        SRGP_setColor( COLOR_WHITE );
        SRGP_fillRectangleCoord( 0, 0, SCRWD, SCRHT );
        SRGP_setColor( COLOR_BLACK );
        /* Draw shape on screen. */
        DrawShape( Shape, SCRWD/2, SCRHT/2 );
      } /* if */
    } /* if-else */
  } while ( !endProgram );

  /* Terminate the SRGP library. */
  SRGP_end();
  /* Free allocated memory. */
  free( Shape->vertices );
  free( Shape->lines );
  free( Shape );
  free( M ); /* Free matrix */
} /* UserMain */

/********************************************************************
 *                 InitShape
 ********************************************************************
 * SYNOPSIS:     Initialize the shape to a cube with a P on the
 *               front of it.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( shapePtr shape )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Inserts coordinates for that of a cube and P.
 *******************************************************************/
void InitShape( shapePtr shape ) {
  int vertex[] = { -20, -20,  20,   /* Vertex 0 of cube */
                    20, -20,  20,   /* Vertex 1 of cube */
                    20,  20,  20,   /* Vertex 2 of cube */
                   -20,  20,  20,   /* Vertex 3 of cube */
                   -20, -20, -20,   /* Vertex 4 of cube */
                    20, -20, -20,   /* Vertex 5 of cube */
                    20,  20, -20,   /* Vertex 6 of cube */
                   -20,  20, -20,   /* Vertex 7 of cube */

                    -5, -10, -20,   /* Vertex 8 of P */
                    -5,   0, -20,   /* Vertex 9 of P */
                    -5,  10, -20,   /* Vertex 10 of P */
                    10,  10, -20,   /* Vertex 11 of P */
                    10,   0, -20,   /* Vertex 12 of P */

                    40, -20, -10,   /* Vertex 13 of pyramid */
                    50, -20, -10,   /* Vertex 14 of pyramid */
                    50, -20,  10,   /* Vertex 15 of pyramid */
                    40, -20,  10,   /* Vertex 16 of pyramid */
                    45,   0,   0 }; /* Vertex 17 of pyramid */
  int line[] = { 0, 1,  /* Lines connecting cube vertices. */
                 1, 2,
                 2, 3,
                 3, 0,
                 4, 5,
                 5, 6,
                 6, 7,
                 7, 4,
                 0, 4,
                 1, 5,
                 2, 6,
                 3, 7,

                 8, 9,  /* Lines connecting P vertices. */
                 9, 10,
                10, 11,
                11, 12,
                12, 9,

                13, 14, /* Lines connecting pyramid vertices. */
                14, 15,
                15, 16,
                16, 13,
                13, 17,
                14, 17,
                15, 17,
                16, 17 };
  int i, j; /* Used in for loop */

  shape->numVerts = 18;
  shape->numLines = 25;
  /* Allocate space of vertices. */
  if (( shape->vertices = (vertexType*)calloc( shape->numVerts,
        sizeof( vertexType ))) == 0 ) {
    printf( "Error allocating memory in InitShape().\n" );
    exit( 1 );
  } /* if */
  /* Allocate space for lines. */
  if (( shape->lines = (lineType*)calloc( shape->numLines,
        sizeof( lineType ))) == 0 ) {
    printf( "Error allocating memory in InitShape().\n" );
    exit( 1 );
  } /* if */
  /* Insert vertex data into shape. */
  for ( i = 0, j = 0; i < shape->numVerts; i++, j += 3 ) {
    shape->vertices[i].wx = vertex[j+0];
    shape->vertices[i].wy = vertex[j+1];
    shape->vertices[i].wz = vertex[j+2];
  } /* for */
  /* Insert line connector data into shape. */
  for ( i = 0, j = 0; i < shape->numLines; i++, j += 2 ) {
    shape->lines[i].start = line[j];
    shape->lines[i].end = line[j+1];
  } /* for */
} /* InitShape */

/********************************************************************
 *                 Transform
 ********************************************************************
 * SYNOPSIS:     Apply the transformation matrix to the shape's
 *               world coordinates to produce the aligned
 *               coordinates.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( matrixPtr m, shapePtr shape )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Uses a 4x1 matrix multiplication method to find
 *               the aligned coordinates.
 *******************************************************************/
void Transform( matrixPtr m, shapePtr shape ) {
  vertexType *vptr; /* Used in for loop. */
  int i; /* Used in for loop. */

  for ( i = 0; i < shape->numVerts; i++ ) {
    vptr = &(shape->vertices[i]);
    vptr->ax = ( (long)vptr->wx * m[0]
               + (long)vptr->wy * m[4]
               + (long)vptr->wz * m[8]
               + m[12] ) >> SHIFT9;
    vptr->ay = ( (long)vptr->wx * m[1]
               + (long)vptr->wy * m[5]
               + (long)vptr->wz * m[9]
               + m[13] ) >> SHIFT9;
    vptr->az = ( (long)vptr->wx * m[2]
               + (long)vptr->wy * m[6]
               + (long)vptr->wz * m[10]
               + m[14] ) >> SHIFT9;
  } /* for */
} /* Transform */

/********************************************************************
 *                 Project
 ********************************************************************
 * SYNOPSIS:     Project the world coordinates onto the screen.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( shapePtr shape, int dist, boolean para )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Projects the shape onto the screen coordinates.
 *******************************************************************/
void Project( shapePtr shape, int dist, boolean para ) {
  vertexType *vptr; /* Used in for loop. */
  int        i; /* Used in for loop. */
  elemType   z; /* Used in division. */

  for ( i = 0; i < shape->numVerts; i++ ) {
    vptr = &(shape->vertices[i]);
    if ( para ) {
      vptr->sx = (int)( (long)dist * vptr->ax );
      vptr->sy = (int)( (long)dist * vptr->ay );
    } else {
      /* Divide x and y by z for persective. */
      z = vptr->az;
      if ( z == 0 ) {
        z = 0 + TRANS_INC;
      } /* if */
      vptr->sx = (int)( (long)dist * vptr->ax / z );
      vptr->sy = (int)( (long)dist * vptr->ay / z );
    } /* if-else */
  } /* for */
} /* Project */

/********************************************************************
 *                 DrawShape
 ********************************************************************
 * SYNOPSIS:     Draws the shape on the screen.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( shapePtr shape, int X, int Y ) The X and Y are
 *               the origin of the world as it would be on the
 *               screen, usually the center of the screen.
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Using the vertex and line arrays draw a series
 *               of lines on the screen.
 *******************************************************************/
void DrawShape( shapePtr shape, int X, int Y ) {
  int i; /* Used in for loop */

  for ( i = 0; i < shape->numLines; i++ ) {
    SRGP_lineCoord(
      shape->vertices[shape->lines[i].start].sx + X,
      shape->vertices[shape->lines[i].start].sy + Y,
      shape->vertices[shape->lines[i].end].sx + X,
      shape->vertices[shape->lines[i].end].sy + Y );
  } /* for */
} /* DrawShape */

/********************************************************************
 *                 Translate
 ********************************************************************
 * SYNOPSIS:     Using the passed values, return the translated
 *               matrix.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( matrixPtr m, int x, int y, int z )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Creates a translation matrix and multiplies it
 *               with the matrix passed.
 *******************************************************************/
void Translate( matrixPtr m, int x, int y, int z ) {
  matrixPtr t;      /* Translation matrix */
  matrixPtr result; /* Need separate one for multiply. */

  /* Allocate some temporary matrixes. */
  t = AllocMatrix();
  result = AllocMatrix();
  /* Assign translation coordinates to translation matrix. */
  MatrixIdentity( t );
  t[12] = (long)x << SHIFT9;
  t[13] = (long)y << SHIFT9;
  t[14] = (long)z << SHIFT9;
  /* Translate matrix passed. */
  MatrixMult( t, m, result );
  /* Copy result into matrix passed. */
  MatrixCopy( result, m );
  /* Free the temporary matrixes. */
  free( t );
  free( result );
} /* Translate */

/********************************************************************
 *                 Rotate
 ********************************************************************
 * SYNOPSIS:     Using the passed values, return the rotated
 *               matrix.
 * CALLED BY:    main
 * PARAMETERS:   ( matrixPtr m, int xa, int ya, int za )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Creates a rotation matrix and multiplies it
 *               with the matrix passed.
 *******************************************************************/
void Rotate( matrixPtr m, int xa, int ya, int za ) {
  matrixPtr r; /* Temporary matrix */
  matrixPtr result; /* Temporary matrix */

  /* Allocate space for matrices. */
  r = AllocMatrix();
  result = AllocMatrix();
  /* Apply rotation about the x axis. */
  r[0] = ONE;
  r[5] = COS( xa );
  r[6] = SIN( xa );
  r[9] = -SIN( xa );
  r[10] = COS( xa );
  r[15] = ONE;
  MatrixMult( r, m, result );
  /* Apply rotation about the y axis. */
  r[0] = COS( ya );
  r[2] = -SIN( ya );
  r[5] = ONE;
  r[6] = 0;
  r[8] = SIN( ya );
  r[9] = 0;
  r[10] = COS( ya );
  MatrixMult( r, result, m );
  /* Apply rotation about the z axis. */
  r[0] = COS( za );
  r[1] = SIN( za );
  r[2] = 0;
  r[4] = -SIN( za );
  r[5] = COS( za );
  r[8] = 0;
  r[10] = ONE;
  MatrixMult( r, m, result );
  MatrixCopy( result, m );
  free( r );
  free( result );
} /* Rotate */

/********************************************************************
 *                 AllocMatrix
 ********************************************************************
 * SYNOPSIS:     Allocate space for a matrix and return pointer.
 * CALLED BY:    UserMain, Translate
 * PARAMETERS:   ( void )
 * RETURNS:      matrixPtr
 * SIDE EFFECTS:
 * STRATEGY:     Allocate space using calloc and clear matrix to all
 *               zeros and return pointer to it.
 *******************************************************************/
matrixPtr AllocMatrix( void ) {
  matrixPtr m; /* This takes up space after return. */
  int i; /* Used in for loop */

  if (( m = (matrixPtr)calloc( 16, sizeof( elemType ))) == 0 ) {
    printf( "Error allocating memory in AllocMatrix().\n" );
    exit( 1 );
  } /* if */
  /* Clear matrix to all 0's */
  for( i = 0; i < 16; i+=4 ) {
    m[i+0] = 0; /* Optimize using loop un-rolling */
    m[i+1] = 0;
    m[i+2] = 0;
    m[i+3] = 0;
  } /* for */
  /* This leaves some allocated memory which must be
   * freed before exiting the program. */
  return( m );
} /* AllocMatrix */

/********************************************************************
 *                 MatrixIdentity
 ********************************************************************
 * SYNOPSIS:     Return identity matrix.
 * CALLED BY:    UserMain, Translate
 * PARAMETERS:   ( matrixPtr m )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Clear the matrix then set main diagonal to 1's.
 *******************************************************************/
void MatrixIdentity( matrixPtr m ) {
  int i; /* Used in for loop */

  /* First clear matrix to all 0's */
  for( i = 0; i < 16; i+=4 ) {
    m[i+0] = 0; /* Optimize using loop un-rolling */
    m[i+1] = 0;
    m[i+2] = 0;
    m[i+3] = 0;
  } /* for */
  /* Now assign the main diagonal to 1's */
  m[0] = ONE;
  m[5] = ONE;
  m[10] = ONE;
  m[15] = ONE;
} /* MatrixIdentity */

/********************************************************************
 *                 MatrixMult
 ********************************************************************
 * SYNOPSIS:     Multiply two matrixes and return the result.
 * CALLED BY:    UserMain
 * PARAMETERS:   ( matrixPtr m1, matrixPtr m2, matrixPtr result )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Multiply elements of m1 to m2 using two for loops.
 *******************************************************************/
void MatrixMult( matrixPtr m1, matrixPtr m2, matrixPtr result ) {
  int i, j; /* Used in for loops */

  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 4; j++ ) {
      /* Optimize using loop un-rolling */
      result[i*4+j] = ( m1[i*4+0] * m2[0+j]
                      + m1[i*4+1] * m2[4+j]
                      + m1[i*4+2] * m2[8+j]
                      + m1[i*4+3] * m2[12+j] ) >> SHIFT9;
    } /* for */
  } /* for */
} /* MatrixMult */

/********************************************************************
 *                 MatrixCopy
 ********************************************************************
 * SYNOPSIS:     Copy one matrix into the other.
 * CALLED BY:    Translate
 * PARAMETERS:   ( matrixPtr source, matrixPtr dest )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Use for loop to copy values from source into dest.
 *******************************************************************/
void MatrixCopy( matrixPtr source, matrixPtr dest ) {
  int i; /* Used in for loop */

  /* Use optimized for loop using un-rolling. */
  for ( i = 0; i < 16; i+=4 ) {
    dest[i+0] = source[i+0];
    dest[i+1] = source[i+1];
    dest[i+2] = source[i+2];
    dest[i+3] = source[i+3];
  } /* for */
} /* MatrixCopy */

