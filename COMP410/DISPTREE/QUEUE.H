/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       Queue ADT Header file
 * FILE:         queue.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    04/14/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the queue.c program. Any
 *      program wishing to use the Queue should #include this
 *      header.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <alloc.h>
    #include <string.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE         -1
    #define FALSE         0
    #define LABEL_LENGTH 11 /* This is also in the
                             * disptree.h file. */

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    /* Type for element data.
     * Change this to whatever data you want stored in the
     * queue. Usually this is a number or a pointer to a node.
     * Structs can be supported with minor changes. */
    typedef struct {
      void *Node;
      char Label[LABEL_LENGTH];
    } dataType;
    /* Type for queue node */
    typedef struct node {
      dataType    data;
      struct node *next;
    } nodeType, *ptrToNodeType;
    /* Type for queue */
    typedef struct {
      ptrToNodeType Head;
      ptrToNodeType Tail;
    } queueType, *ptrToQueueType;
    typedef char boolean;

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    ptrToQueueType MakeEmptyQueue( void );
              void FreeQueue( ptrToQueueType Q );
           boolean IsEmptyQueue( ptrToQueueType Q );
              void Enqueue( ptrToQueueType Q, dataType x );
              void Dequeue( ptrToQueueType Q, dataType *x );
              void FrontOfQueue( ptrToQueueType Q, dataType *x );

