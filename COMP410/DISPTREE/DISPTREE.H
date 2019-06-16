/********************************************************************
 *
 * Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       DisplayTree Header file
 * FILE:         disptree.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      04/13/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the DisplayTree program.
 *
 *      To use DisplayTree you must #include the disptree.h header
 *      file and include the disptree.c file in your project file
 *      using Borland's Project Manager.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <conio.h>
    #include <ctype.h>
    #include <string.h>
    #include <graphics.h>
    #include <alloc.h>
    #include "queue.h"

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define MAX_ROWS     100 /* Max number of levels. */
    #define MAX_COLUMNS  100 /* Max number of nodes on a level. */
    #define LABEL_LENGTH  11 /* Length of node labels.
                              * This is duplicated in queue.h */

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    /* This holds information about a node.
     * This is defined in queue.h. */
    typedef dataType nodeRecordType;

    /* This is a pointer to an array of nodeRecordTypes.
     * It is required that the array be allocated using
     * calloc() so it can be free()'d. */
    typedef nodeRecordType *childrenArrayType;

    /* This is the record that GetChildren must return. */
    typedef struct {
      int               numChildren;
      childrenArrayType childrenArray;
    } childrenRecordType;

    /* We implemented the following record for a node to hold the
     * necessary information for implementing our special tree
     * of type TREETYPE. It also holds the x,y coordinates of the
     * nodes, allowing fast tree display, as afforded by the
     * array implementation. */
    typedef struct {
      char    Label[LABEL_LENGTH];
                       /* Label representing the node. */
      boolean Sibling; /* True if this node has a sibling. */
      int     Parent,  /* Column index to parent. */
              Child,   /* Column index to first child. */
              Width,   /* The "width" of this node. */
              X, Y;    /* World x and y coordinates. */
    } NodeRecordType;

    /* The level type is simply an array of NodeRecords. Each
     * level represents a level in the tree. A level can be of
     * any size from 1 to MAX_COLUMNS. */
    typedef NodeRecordType *LevelType;

    /* The tree type is also an array, but of Levels. It can
     * range from size of 1 to MAX_ROWS. Each record of the tree
     * holds the number of nodes on the level and a pointer to
     * the first record of that level. */
    typedef struct {
      int       NumNodes; /* Number of nodes in this level. */
      LevelType Level;    /* Pointer to first element in level. */
    } TreeType, *ptrToTreeType;

    /* nodePtr
     * Pointer to a node in user program's tree. */
    typedef void *nodePtr;

/********************************************************************
 *                 Prototypes
 *******************************************************************/

    /* GetChildren
     * This is the prototype of the GetChildren function.
     * To use DisplayTree you must provide this function. */
    childrenRecordType *GetChildren( nodePtr P );

             void DisplayTree( childrenArrayType TheTree,
                               boolean LabelsOn );
    ptrToTreeType BuildArrays( childrenArrayType Tree );
             void MakeWidths( ptrToTreeType tree );
             void MakeCoords( ptrToTreeType tree);
             void DrawTree( ptrToTreeType tree, boolean LabelsOn );

