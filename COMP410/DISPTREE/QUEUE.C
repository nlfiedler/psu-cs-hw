/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       Queue ADT
 * FILE:         queue.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    02/05/95  Initial version of pancake stack
 *       NF    04/13/95  Changed to a Queue ADT
 *
 * DESCRIPTION:
 *      An implementation of a simple queue data structure. For use
 *      with the DisplayTree program.
 *
 *      To use this module, add it to your project file in
 *      Borland's Project Manager. Also make sure to #include
 *      the queue.h header file.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "queue.h"

/********************************************************************
 *                 MakeEmptyQueue
 ********************************************************************
 * SYNOPSIS:     Creates a new, empty queue.
 * PARAMETERS:   ( void )
 * RETURNS:      ptrToQueueType - Pointer to newly created queue
 * SIDE EFFECTS:
 * STRATEGY:     Simply creates an empty queue data structure.
 *******************************************************************/
ptrToQueueType MakeEmptyQueue( void ) {
  ptrToQueueType Q; /* Temporary queue pointer */

  if (( Q = malloc( sizeof( queueType ))) == 0 ) {
    printf( "Error allocating memory in MakeEmptyQueue().\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  Q->Head = NULL; /* Initialize head pointer */
  Q->Tail = NULL; /* Initialize tail pointer */
  return( Q ); /* Return pointer to new queue */
} /* MakeEmptyQueue */

/********************************************************************
 *                 FreeQueue
 ********************************************************************
 * SYNOPSIS:     Frees all the nodes of the queue and then frees the
 *               queue itself.
 * PARAMETERS:   ( ptrToQueueType Q )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     First free each of the nodes by calling Dequeue until
 *               there are no more items, then free the queue.
 *******************************************************************/
void FreeQueue( ptrToQueueType Q ) {
  dataType x; /* Temporary */

  x.Node = NULL; /* Satisfy compiler. */
  while ( !IsEmptyQueue( Q ) ) {
    Dequeue( Q, &x );
  } /* while */
  free( Q );
} /* FreePQueue */

/********************************************************************
 *                 IsEmptyQueue
 ********************************************************************
 * SYNOPSIS:     Checks if the queue is empty.
 * PARAMETERS:   ( ptrToQueueType Q )
 * RETURNS:      boolean - TRUE if queue is empty
 * SIDE EFFECTS:
 * STRATEGY:     Checks Q->Head = NULL and returns result.
 *******************************************************************/
boolean IsEmptyQueue( ptrToQueueType Q ) {
  return( Q->Head == NULL );
} /* IsEmptyPQueue */

/********************************************************************
 *                 Enqueue
 ********************************************************************
 * SYNOPSIS:     Enqueues the data onto the queue.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToQueueType Q, dataType x )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Allocates a new node type and places the data into
 *               the data field of that node.
 *******************************************************************/
void Enqueue( ptrToQueueType Q, dataType x ) {
  ptrToNodeType ptr; /* Temporary pointer */

  if (( ptr = malloc( sizeof( nodeType ))) == 0 ) {
    printf( "Error allocating memory in Enqueue().\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  ptr->data.Node = x.Node;  /* Set new node's data */
  strcpy( ptr->data.Label, x.Label );
  ptr->next = NULL; /* New node points nothing. */
  /* Add node to back of list. */
  if ( Q->Tail == NULL ) {
    Q->Head = ptr;
    Q->Tail = ptr;
  } else {
    Q->Tail->next = ptr;
    Q->Tail = ptr;
  } /* if-else */
} /* Enqueue */

/********************************************************************
 *                 Dequeue
 ********************************************************************
 * SYNOPSIS:     Dequeues the front element from the queue.
 * PARAMETERS:   ( ptrToQueueType Q, dataType *x )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Dequeues the front element from the queue and
 *               returns it, first checking to see if the queue is
 *               empty.
 *******************************************************************/
void Dequeue( ptrToQueueType Q, dataType *x ) {
  ptrToNodeType ptr; /* Temporary pointer */

  if ( IsEmptyQueue( Q ) ) {
    printf( "Cannot dequeue from an empty queue.\n" );
  } /* if */
  x->Node = Q->Head->data.Node;   /* Get data of head element. */
  strncpy( x->Label, Q->Head->data.Label, LABEL_LENGTH );
  ptr = Q->Head;       /* Set pointer to head. */
  Q->Head = ptr->next; /* Advance head to next element. */
  if ( Q->Head == NULL ) {
    Q->Tail = NULL;    /* Set tail pointer appropriately. */
  } /* if */
  free( ptr );         /* Free the old head element. */
} /* Dequeue */

/********************************************************************
 *                 FrontOfQueue
 ********************************************************************
 * SYNOPSIS:     Returns value of the front element from the queue.
 * PARAMETERS:   ( ptrToQueueType Q, dataType *x )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Returns the value of the element at the front of
 *               the queue.
 *******************************************************************/
void FrontOfQueue( ptrToQueueType Q, dataType *x ) {
  if ( IsEmptyQueue( Q ) ) {
    printf( "Cannot get front item value of an empty queue.\n" );
  } /* if */
  x->Node = Q->Head->data.Node;   /* Get data of head element. */
  strncpy( x->Label, Q->Head->data.Label, LABEL_LENGTH );
} /* FrontOfQueue */

