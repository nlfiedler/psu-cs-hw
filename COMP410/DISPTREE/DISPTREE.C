/********************************************************************
 *
 * Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       DisplayTree
 * FILE:         disptree.c
 *
 * AUTHORS:      Thomas Denn
 *               Nathan Fiedler
 *               Dean   Winkelspecht (did not help)
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      04/12/95  Initial version
 *
 * DESCRIPTION:
 *      This is the DisplayTree program. It will display any tree
 *      graphically using the BGI DOS graphics library.
 *
 *      To use you must #include <disptree.h> and include the
 *      disptree.c in your project file.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "disptree.h"

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define DELTA_X     100 /* Distance between nodes. */
    #define DELTA_Y      50 /* Distance between layers in pixels. */
    #define LABEL_WIDTH  80 /* Width of label in pixels. */
    #define NODE_WIDTH   90 /* Width of node in pixels. */
    #define NODE_HEIGHT  20 /* Height of node in pixels. */

/********************************************************************
 *                 Globals
 *******************************************************************/
    int treeDepth,   /* Depth of tree (number of levels). */
        curX, curY,  /* Position of viewer. */
        maxX, maxY,  /* Screen size. */
        spaceWidth,  /* Width of tree space, equal to x position
                      * of rightmost node. */
        spaceHeight; /* Height of tree space, equal to y position
                      * of bottommost node. */

/********************************************************************
 *                 DisplayTree
 ********************************************************************
 * SYNOPSIS:     Creates the tree array and places the node labels
 *               and parent/child relationship data in each node.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   void ( childrenArrayType TheTree, boolean LabelsOn )
 * REQUIREMENTS:
 *               DisplayTree requires the user to supply a function
 *               GetChildren which when given the pointer to a
 *               node in the tree, must return a record with
 *               the following fields:
 *                 ChildRecordType (struct)
 *                   int               NumberOfChildren;
 *                   ChildrenArrayType *ChildrenArray;
 *                 ChildrenArrayType is Array of NOdeRecordType
 *                 NodeRecordType (struct)
 *                   void *child;
 *                   char Label[LABEL_LENGTH];
 *
 *               rootLabel is the null-terminated label of the
 *               root node of the tree.
 * SIDE EFFECTS:
 * STRATEGY:     Call the functions to build our copy of the tree,
 *               then start an interaction loop to scroll the tree
 *               on the screen.
 *******************************************************************/
void DisplayTree( childrenArrayType TheTree, boolean LabelsOn ) {
  char          keyPress;   /* Used in user interaction. */
  int           gdriver = DETECT,
                gmode,
                errorcode,  /* Used in BGI graphics. */
                i,          /* Used in for loop. */
                lastX, lastY; /* Used in user interaction,
                               * tells if we need to
                               * redraw the tree. */
  ptrToTreeType Tree;       /* Our tree. */
  boolean       endProgram; /* T/F flag indicating if user
                             * wants to end program. */


  /*Construct the main array to hold the data for the tree. */
  Tree = BuildArrays( TheTree );  /* Returns tree structure. */

  /* Now set the width of each of the nodes in OurTree. */
  MakeWidths( Tree );  /* Modifies the width fields of OurTree. */

  /* Next create the X and Y coordinates of all the nodes. */
  MakeCoords( Tree );  /* Modifies X and Y fields. */

  /* Register the EGAVGA video driver. */
  errorcode = registerbgidriver( EGAVGA_driver );
  if ( errorcode < 0 ) {
    printf( "Graphics error: %s\n", grapherrormsg( errorcode ) );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  /* Initialize graphics mode. */
  initgraph( &gdriver, &gmode, "" );
  /* Read result of initialization. */
  errorcode = graphresult();
  if ( errorcode != grOk ) {
    printf( "Graphics error: %s\n", grapherrormsg( errorcode ) );
    exit(1); /* Exit to DOS */
  } /* if */
  maxX = getmaxx(); /* Get screen size. */
  maxY = getmaxy();
  spaceWidth = spaceWidth - maxX + ( NODE_WIDTH / 2 );
  if ( spaceWidth < 0 ) {
    spaceWidth = 0;
  } /* if */
  spaceHeight = spaceHeight - maxY + ( NODE_HEIGHT / 2 );
  if ( spaceHeight < 0 ) {
    spaceHeight = 0;
  } /* if */
  /* Clear screen to dark on light gray. */
  setbkcolor( LIGHTGRAY ); /* Set background color. */
  setcolor( DARKGRAY );    /* Set paint color. */
  cleardevice(); /* Clear screen. */

  /*Draw the tree using the parent/child relationship. */
  curX = 0; /* Set initial viewer position. */
  curY = 0;
  lastX = curX;
  lastY = curY;
  DrawTree( Tree, LabelsOn );

  /* Start main loop. */
  endProgram = FALSE;
  do {
    while (!kbhit()) { } /* Wait till keypress. */
    keyPress = getch();
    fflush( stdin );
    if ( toupper( keyPress ) == 'I' ) {
      curY -= 10;
      if ( curY < 0 ) {
        curY = 0;
      } /* if */
    } /* if */
    if ( toupper( keyPress ) == 'K' ) {
      curY += 10;
      if ( curY > spaceHeight ) {
        curY = spaceHeight;
      } /* if */
    } /* if */
    if ( toupper( keyPress ) == 'J' ) {
      curX -= 10;
      if ( curX < 0 ) {
        curX = 0;
      } /* if */
    } /* if */
    if ( toupper( keyPress ) == 'L' ) {
      curX += 10;
      if ( curX > spaceWidth ) {
        curX = spaceWidth;
      } /* if */
    } /* if */
    /* Check if user wants to quit. */
    /* Check if both mouse buttons are down. */
    if ( toupper( keyPress ) == 'Q' ) {
      endProgram = TRUE;
    } /* if */
    if (( curX != lastX ) || ( curY != lastY )) {
      DrawTree( Tree, LabelsOn );
      lastX = curX;
      lastY = curY;
    } /* if */
  } while( endProgram == FALSE );

  /* Exit BGI library. */
  closegraph();

  /* Free allocated memory. */
  for ( i = 0; i < treeDepth; i++ ) {
    free( Tree[i].Level );
  } /* for */
  free( Tree );
} /* DisplayTree */

/********************************************************************
 *                 BuildArrays
 ********************************************************************
 * SYNOPSIS:     Creates the tree array and places the node labels
 *               and parent/child relationship data in each node.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   ptrToTreeType ( childrenArrayType Tree )
 * SIDE EFFECTS:
 * STRATEGY:     Lots of stuff
 *******************************************************************/
ptrToTreeType BuildArrays( childrenArrayType Tree ) {
  ptrToTreeType      tree;       /* Our tree that will be built. */
  ptrToQueueType     Q;          /* Queue that is used temporarily. */
  nodeRecordType     P,          /* Current node being processed. */
                     marker,     /* Used to mark end of siblings. */
                     temp;       /* Used temporarily. */
  childrenRecordType *childList; /* Struct built by GetChildren. */
  int                numProcessed, /* Number of nodes done so far
                                    * on this level. */
                     curLevel,     /* Current level */
                     parent,       /* Used to set parent pointers. */
                     firstParent,  /* Avoids "column 1" problem. */
                     i;            /* Used in for loop. */

  /* Initialize tree structure for MAX_ROWS of levels. */
  tree = (ptrToTreeType)calloc( MAX_ROWS, sizeof( TreeType ));
  if ( tree == NULL ) {
    printf( "Memory allocation failed in BuildArrays.\n" );
    exit( 1 );
  } /* if */
  /* Allocate an empty queue. */
  Q = MakeEmptyQueue();
  /* Initialize some variables and counters. */
  numProcessed = 0; /* Number of nodes processed on this level. */
  curLevel = 0;     /* Current level of tree being processed. */
  parent = 0;       /* Used to set parent field of nodes. */
  firstParent = -1; /* Used to set parent field of nodes.
                     * This makes sure the first node with a
                     * child is correctly flagged, since the
                     * parent node is not necessarily the one
                     * in column 1. */
  marker.Node = (childrenArrayType)1;  /* Set sibling marker. */
  temp.Node = NULL;
  P.Node = NULL; /* Satisfy compiler. */
  Enqueue( Q, *Tree );  /* Start the queue. */
  Enqueue( Q, marker ); /* Add sibling marker */
  Enqueue( Q, temp );   /* Add level marker */
  FrontOfQueue( Q, &temp );
  while ( temp.Node != NULL ) {
    tree[curLevel].Level = (LevelType)calloc( MAX_COLUMNS,
                             sizeof( NodeRecordType ));
    if ( tree[curLevel].Level == NULL ) {
      printf( "Memory allocation failed in BuildArrays.\n" );
      exit( 1 );
    } /* if */
    FrontOfQueue( Q, &temp );
    while ( temp.Node != NULL ) {
      Dequeue( Q, &P );
      /* Set label of current node. */
      strncpy( tree[curLevel].Level[numProcessed].Label, P.Label,
               LABEL_LENGTH );
      /* Set sibling flag. */
      FrontOfQueue( Q, &temp );
      if ( temp.Node == marker.Node ) {
        tree[curLevel].Level[numProcessed].Sibling = FALSE;
      } else {
        tree[curLevel].Level[numProcessed].Sibling = TRUE;
      } /* if-else */
      /* Set parent pointer. */
      if ( curLevel == 0 ) {
        /* Root has no parent. */
        tree[curLevel].Level[numProcessed].Parent = -1;
      } else {
        tree[curLevel].Level[numProcessed].Parent = parent;
      } /* if-else */
      /* Set child pointer of this Level. */
      /* This acts as a flag which is checked in the next line. */
      tree[curLevel].Level[numProcessed].Child = -1;
      /* Set child pointer of previous level. */
      if ( curLevel > 0 ) {
        /* Do only when past root level. */
        if ( tree[curLevel-1].Level[parent].Child == -1 ) {
          tree[curLevel-1].Level[parent].Child = numProcessed;
        } /* if */
      } /* if */
      /* Now increment parent counter if at end of siblings. */
      FrontOfQueue( Q, &temp );
      if ( temp.Node == marker.Node ) {
        parent += 1;
        /* Check if this "parent" has a child or not.
         * If not, move to next node until one has a child. */
        if ( curLevel > 0 ) {
          /* Do only if past root level. */
          while (( parent < tree[curLevel-1].NumNodes ) &&
                 ( tree[curLevel-1].Level[parent].Y == 0 )) {
            /* Advance until we find a parent that has kids. */
            parent += 1;
          } /* while */
        } /* if */
        Dequeue( Q, &temp );  /* Remove the sibling marker. */
      } /* if */
      childList = GetChildren( P.Node );
      /* Set number of children node has.
       * We're using Y field temporarily. */
      tree[curLevel].Level[numProcessed].Y = childList->numChildren;
      /* Enqueue node's children. */
      if ( childList->numChildren > 0 ) {
        for ( i = 0; i < childList->numChildren; i++ ) {
          Enqueue( Q, childList->childrenArray[i] );
        } /* for */
/*        printf( "%ld\n", coreleft() ); */
        free( childList->childrenArray ); /* Free child list. */
        free( childList );
/*        printf( "%ld\n", coreleft() );
        getchar(); */
        Enqueue( Q, marker ); /* Indicate end of siblings */
        /* Set first parent index. */
        if ( firstParent == -1 ) {
          firstParent = numProcessed;
        } /* if */
      } /* if */
      numProcessed += 1; /* One more down, many more to go... */
      FrontOfQueue( Q, &temp );
    } /* while */
    Dequeue( Q, &temp );
    Enqueue( Q, temp ); /* Move level marker to end of queue. */
    /* Reallocate Level array to numProcessed size. */
    tree[curLevel].Level = (LevelType)realloc( tree[curLevel].Level,
                            ( numProcessed * sizeof( NodeRecordType )));
    if ( tree[curLevel].Level == NULL ) {
      printf( "Memory allocation failed in BuildArrays.\n" );
      exit( 1 );
    } /* if */
    tree[curLevel].NumNodes = numProcessed;
    curLevel += 1;  /* Increment level counter. */
    numProcessed = 0; /* Reset since done with this level. */
    parent = firstParent;
    firstParent = -1;
    FrontOfQueue( Q, &temp );
  } /* while */
  FreeQueue( Q ); /* Deallocate queue structure.
                   * This will remove any leftover elements. */
  /* Reallocate tree structure to curLevel value. */
  tree = (ptrToTreeType)realloc( tree, ( curLevel * sizeof( TreeType )));
  if ( tree == NULL ) {
    printf( "Memory allocation failed in BuildArrays.\n" );
    exit( 1 );
  } /* if */
  /* Set tree depth. */
  treeDepth = curLevel;
  return( tree ); /* Return tree pointer. */
} /* BuildArrays */

/********************************************************************
 *                 MakeWidths
 ********************************************************************
 * SYNOPSIS:     Find the widths of the nodes.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   void ( ptrToTreeType tree )
 * SIDE EFFECTS:
 * STRATEGY:     Starting at the leaves, go back up the tree
 *               filling in the widths with the widths of the
 *               children of any particular node.
 *******************************************************************/
void MakeWidths( ptrToTreeType tree ) {
  int curLevel, /* Current level being processed. */
      colIndex, /* Column index. */
      sibCount, /* Sibling counter. */
      parent,   /* Used temporarily to save source line space. */
      width;    /* Used in setting width field. */

  for ( curLevel = treeDepth-1; curLevel >= 0; curLevel-- ) {
    colIndex = 0; /* Column index */
    sibCount = 0; /* Sibling counter */
    /* Process each node on this level. */
    while ( colIndex < tree[curLevel].NumNodes ) {
      width = 0; /* Width of the current set of siblings. */
      do { /* For each sibling. */
        /* Check if node has a child. */
        if ( tree[curLevel].Level[sibCount].Child == -1 ) {
          /* If no child, set width to 1. */
          tree[curLevel].Level[sibCount].Width = 1;
          /* Set number of children to 0. */
/*          tree[curLevel].Level[sibCount].Y = 0; */
        } /* if */
        /* Add node width to siblings' width. */
        width += tree[curLevel].Level[sibCount].Width;
        /* Advance sibling column index. */
        sibCount += 1;
        /* Do until reach end of this set of siblings. */
      } while ( tree[curLevel].Level[sibCount-1].Sibling == TRUE );
      /* Find parent of those siblings and set its width. */
      if ( curLevel > 0 ) {
        /* Do only if not on root level. */
        parent = tree[curLevel].Level[sibCount-1].Parent;
        tree[curLevel-1].Level[parent].Width = width;
        /* Set the number of children the parent has.
         * We're using the Y field temporarily. */
/*        tree[curLevel-1].Level[parent].Y = ( sibCount - colIndex ); */
      } /* if */
      /* Set column index to sibling column index. */
      colIndex = sibCount;
    } /* while */
  } /* for */
} /* MakeWidths */

/********************************************************************
 *                 MakeCoords
 ********************************************************************
 * SYNOPSIS:     Calculate the coordinates of each node within the
 *               world space using the widths and parent/child
 *               relationships.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   void ( ptrToTreeType tree )
 * SIDE EFFECTS:
 * STRATEGY:     Starting at the root, go down the tree
 *               filling in the x,y coordinate pairs with
 *               world coordinates appropriate for each node.
 *******************************************************************/
void MakeCoords( ptrToTreeType tree ) {
  int   row,    /* Current level being processed. */
        col,    /* Current node in level being processed. */
        x,      /* Running x position. */
        y,      /* Running y position. */
        parent; /* Used to save source code. */
  float temp;  /* Used in floating point calculations. */

  /* Set tree space boundaries to initial values. */
  spaceWidth = 0;
  spaceHeight = 0;
  /* Special case: Set the root node. */
  row = 0;
  col = 0;
  temp = (float)tree[row].Level[col].Width / 2;
  tree[row].Level[col].X = temp * DELTA_X;
  y = DELTA_Y * 2; /* y must be at least DELTA_Y to start. */
  for ( row = 1; row < treeDepth; row++ ) {
    x = 0;
    col = 0;
    while ( col < tree[row].NumNodes ) {
      parent = tree[row].Level[col].Parent;
      x = tree[row-1].Level[parent].X -
          (((float)tree[row-1].Level[parent].Width / 2 ) * DELTA_X );
      do { /* For each sibling */
        /* Set node's x coordinate. */
        tree[row].Level[col].X =
          ((float)tree[row].Level[col].Width / 2 ) * DELTA_X + x;
        /* Update tree space width. */
        if ( tree[row].Level[col].X > spaceWidth ) {
          spaceWidth = tree[row].Level[col].X;
        } /* if */
        /* If node has no children its y will not get set
         * down below, so must do it here. */
        if ( tree[row].Level[col].Child == -1 ) {
          tree[row].Level[col].Y = y;
          /* Update tree space height. */
          if ( tree[row].Level[col].Y > spaceHeight ) {
            spaceHeight = tree[row].Level[col].Y;
          } /* if */
        } /* if */
        x += tree[row].Level[col].Width * DELTA_X;
        col += 1;
      } while( tree[row].Level[col-1].Sibling == TRUE );
      /* Now we can set the y position of the parent. */
      tree[row-1].Level[tree[row].Level[col-1].Parent].Y = y - DELTA_Y;
    } /* while */
    y += DELTA_Y; /* Move to next level. */
  } /* for */
} /* MakeCoords */

/********************************************************************
 *                 DrawTree
 ********************************************************************
 * SYNOPSIS:     Draw the tree using the BGI routines and the
 *               data provided in the nodes.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   void ( ptrToTreeType tree, boolean LabelsOn )
 * SIDE EFFECTS:
 * STRATEGY:     Starting at the root, go down the tree
 *               drawing any nodes that are visible.
 *******************************************************************/
void DrawTree( ptrToTreeType tree, boolean LabelsOn ) {
  int x1, y1,
      x2, y2, /* Running coordinates. */
      tx,     /* Text x coordinate. */
      ly,     /* Line y coordinate. */
      row,    /* Row index. */
      col,    /* Column index. */
      child;  /* Column index to current child being drawn. */

  cleardevice(); /* Clear graphics screen. */
  /* Draw the tree. */
  for ( row = 0; row < treeDepth; row++ ) {
    col = 0;
    do {
      x1 = tree[row].Level[col].X;
      y1 = tree[row].Level[col].Y;
      /* Draw node dot at x1,y1. */
      if ((( x1 - curX ) <= maxX ) &&
          (( y1 - curY ) <= maxY )) {
        /* Draw only if on screen. Also, subtract current
         * viewer position from coordinates. */
        if ((( x1 - curX ) >= -NODE_WIDTH ) &&
            (( y1 - curY ) >= -NODE_HEIGHT )) {
          fillellipse( x1 - curX, y1 - curY,
                       NODE_WIDTH / 2, NODE_HEIGHT / 2 );
          /* Draw label and node at x1,y1. */
          if ( LabelsOn == TRUE ) {
            tx = x1 - ( LABEL_WIDTH / 2 );
            /* Draw label at x1,y1. */
            outtextxy( tx - curX, y1-3 - curY,
                       tree[row].Level[col].Label );
          } /* if */
        } /* if */
      } /* if */
      /* Check if child exists. */
      if ( tree[row].Level[col].Child != -1 ) {
        child = tree[row].Level[col].Child;
        do {
          x2 = tree[row+1].Level[child].X;
          y2 = tree[row+1].Level[child].Y;
          ly = y1 + ( NODE_HEIGHT / 2 );
          /* Draw line from x1,ly to x2,y2. */
          if ((( ly - curY ) <= maxY ) &&
              ((( x1 - curX ) <= maxX ) ||
               (( x2 - curX ) <= maxX ))) {
            /* Draw only if some part of the line is on screen. */
            line( x1 - curX, ly - curY,
                  x2 - curX, y2 - curY );
          } /* if */
          child += 1;
        } while( tree[row+1].Level[child-1].Sibling == TRUE );
      } /* if */
      col += 1;
    } while( col < tree[row].NumNodes );
  } /* for */
} /* DrawTree */

