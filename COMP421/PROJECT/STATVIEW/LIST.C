/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       List ADT
 * FILE:         list.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/03/95  Initial version
 *
 * DESCRIPTION:
 *      Doubly-linked list ADT.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "list.h"

/********************************************************************
 *                 MakeEmptyList
 ********************************************************************
 * SYNOPSIS:     Makes an empty list structure.
 * PARAMETERS:   ( void )
 * RETURNS:      ptrToListType - Newly created list.
 * STRATEGY:     Allocate memory for new list. Initialize its
 *               variables.
 *******************************************************************/
ptrToListType MakeEmptyList
  (
  void
  )
{
  ptrToListType newList;

  if ( ( newList = malloc( sizeof( listType ))) == NULL ) {
    printf( "Could not allocate memory for list!\n" );
    exit( 1 );
  }
  newList->head = NULL;
  newList->tail = NULL;
  newList->numElements = 0;
  return( newList );
} /* MakeEmptyList */

/********************************************************************
 *                 FreeList
 ********************************************************************
 * SYNOPSIS:     Frees a list structure.
 * PARAMETERS:   void ( ptrToListType list )
 * STRATEGY:     Free the memory used by the list, after removing
 *               all of the elements.
 *******************************************************************/
void FreeList
  (
  ptrToListType list
  )
{
  ptrToNodeType eDelete;

  if ( list != NULL ) {
    if ( list->head != NULL ) {
      eDelete = list->head;
      while( !IsEmptyList( list ) ) {
        ListDeleteNode( list, &eDelete );
      }
    }
    free( list );
  }
} /* FreeList */

/********************************************************************
 *                 ListDeleteNode
 ********************************************************************
 * SYNOPSIS:     Frees a node in the list.
 * PARAMETERS:   void ( ptrToListType   list,
 *                      ptrToNodeType * node )
 * STRATEGY:     Free the memory used by the node. Set the pointers
 *               appropriately.
 *******************************************************************/
void ListDeleteNode
  (
  ptrToListType   list,
  ptrToNodeType * node
  )
{
  ptrToNodeType prev;
  ptrToNodeType next;

  if ( !IsEmptyList( list ) ) {
       /*
        * Make previous node point around this node.
        * Make next node point around this node.
        */
    prev = (*node)->prev;
    next = (*node)->next;
    prev->next = next;
    next->prev = prev;
       /*
        * If this node is the head or tail, set those
        * pointers appropriately.
        */
    if ( *node == list->head ) {
      list->head = next;
    }
    if ( *node == list->tail ) {
      list->tail = prev;
    }
       /*
        * Free the node.
        * Decrement the number of elements.
        * If there are no elements left, set the head
        * and tail to NULL.
        */
    free( *node );
    *node = prev;
    list->numElements--;
    if ( list->numElements == 0 ) {
      list->head = NULL;
      list->tail = NULL;
      *node = NULL;
    }
  } /* if */
} /* ListDeleteNode */

/********************************************************************
 *                 ListAddNode
 ********************************************************************
 * SYNOPSIS:     Add new element to the end of the list.
 * PARAMETERS:   void ( ptrToListType list,
 *                      dataType      data )
 * STRATEGY:
 *******************************************************************/
void ListAddNode
  (
  ptrToListType list,
  dataType      data
  )
{
  ptrToNodeType newNode;

  if ( list != NULL ) {
    if (( newNode = malloc( sizeof( nodeType ))) == NULL ) {
      printf( "Could not allocate memory for node!\n" );
      exit( 1 );
    }
    newNode->data = data;
    if ( list->numElements > 0 ) {
      list->tail->next = newNode;
      list->head->prev = newNode;
    }
    else {
      list->head = newNode;
      list->tail = newNode;
    }
    newNode->next = list->head;
    newNode->prev = list->tail;
    list->tail = newNode;
    list->numElements++;
  }
} /* ListAddNode */

/********************************************************************
 *                 ListHead
 ********************************************************************
 * SYNOPSIS:     Returns the pointer to the head node of a list.
 * PARAMETERS:   void ( ptrToListType   list,
 *                      ptrToNodeType * node )
 * STRATEGY:     Simply access the head field of the list.
 *******************************************************************/
void ListHead
  (
  ptrToListType   list, /* (in) */
  ptrToNodeType * node  /* (out) */
  )
{
  if (( !IsEmptyList( list ) ) && ( *node != NULL )) {
    *node = list->head;
  }
  else {
    *node = NULL;
  }
} /* ListHead */

/********************************************************************
 *                 ListTail
 ********************************************************************
 * SYNOPSIS:     Returns the pointer to the tail node of a list.
 * PARAMETERS:   void ( ptrToListType   list,
 *                      ptrToNodeType * node )
 * STRATEGY:     Simply access the tail field of the list.
 *******************************************************************/
void ListTail
  (
  ptrToListType   list, /* (in) */
  ptrToNodeType * node  /* (out) */
  )
{
  if (( !IsEmptyList( list ) ) && ( *node != NULL )) {
    *node = list->tail;
  }
  else {
    *node = NULL;
  }
} /* ListTail */

/********************************************************************
 *                 ListDataOfNode
 ********************************************************************
 * SYNOPSIS:     Returns the data of a given node.
 * PARAMETERS:   void ( ptrToNodeType node,
 *                      dataType *    data )
 * STRATEGY:     Simply access the data field of the node.
 *******************************************************************/
void ListDataOfNode
  (
  ptrToNodeType node, /* (in) */
  dataType *    data  /* (out) */
  )
{
  if ( node != NULL ) {
    *data = node->data;
  }
} /* ListDataOfNode */

/********************************************************************
 *                 ListSetNodeData
 ********************************************************************
 * SYNOPSIS:     Returns the data of a given node.
 * PARAMETERS:   void ( ptrToNodeType node,
 *                      dataType      data )
 * STRATEGY:     Simply access the data field of the node.
 *******************************************************************/
void ListSetNodeData
  (
  ptrToNodeType node, /* (in) */
  dataType      data  /* (in) */
  )
{
  if ( node != NULL ) {
    node->data = data;
  }
} /* ListSetNodeData */

/********************************************************************
 *                 ListNextNode
 ********************************************************************
 * SYNOPSIS:     Returns the pointer to the next node.
 * PARAMETERS:   void ( ptrToNodeType * node )
 * STRATEGY:     Simply access the next field of the node.
 *******************************************************************/
void ListNextNode
  (
  ptrToNodeType * node /* (in/out) */
  )
{
  if ( *node != NULL ) {
    *node = (*node)->next;
  }
} /* ListNextNode */

/********************************************************************
 *                 ListPrevNode
 ********************************************************************
 * SYNOPSIS:     Returns pointer to previous node.
 * PARAMETERS:   void ( ptrToNodeType * node )
 * STRATEGY:     Simply access the prev field of the node.
 *******************************************************************/
void ListPrevNode
  (
  ptrToNodeType * node /* (in/out) */
  )
{
  if ( *node != NULL ) {
    *node = (*node)->prev;
  }
} /* ListPrevNode */

/********************************************************************
 *                 IsEmptyList
 ********************************************************************
 * SYNOPSIS:     Returns true if list is empty.
 * PARAMETERS:   ( ptrToListType list )
 * RETURNS:      boolean - TRUE if list is empty.
 * STRATEGY:     Simply access the numElements field of the list.
 *******************************************************************/
boolean IsEmptyList
  (
  ptrToListType list /* (in) */
  )
{
  if ( list != NULL ) {
    return( list->numElements == 0 );
  }
  else {
    return( TRUE );
  }
} /* IsEmptyList */

