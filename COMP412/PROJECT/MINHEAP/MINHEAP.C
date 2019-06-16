/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Priority Heap
 * FILE:         minheap.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/01/95  Initial version
 *
 * DESCRIPTION:
 *      Priority queue ADT, implemented as a min heap.
 *
 * NOTE:
 *      Because the way the heap uses the indexes, we must waste
 *      the first element of the array. You will notice that
 *      all of the functions will maintain this dummy element.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "minheap.h"

/********************************************************************
 *                 MakeEmptyHeap
 ********************************************************************
 * SYNOPSIS:     Creates a new heap.
 * PARAMETERS:   ( void )
 * RETURNS:      ptrToHeapType - New heap.
 * STRATEGY:     Allocate memory for the heap and set it's initial
 *               values. Then return a pointer to it.
 *******************************************************************/
ptrToHeapType MakeEmptyHeap
  (
  void
  )
{
  ptrToHeapType heap; /* Used temporarily to allocate heap. */

     /*
      * Allocate memory for the heap type.
      * If error, display message.
      * Initialize heap variables.
      * Return pointer to heap.
      */
  if (( heap = malloc( sizeof( heapType ))) == NULL ) {
    printf( "Could not allocate memory for heap type!\n" );
    exit( 1 );
  }
  else {
    heap->numElements = 0;
    heap->head = NULL;
  }
  return( heap );
} /* MakeEmptyHeap */

/********************************************************************
 *                 HeapInsert
 ********************************************************************
 * SYNOPSIS:     Inserts new element onto the heap.
 * PARAMETERS:   void ( ptrToHeapType heap,
 *                      int           key,
 *                      dataType      data )
 * STRATEGY:     Increment the number of elements. If this is the
 *               first one, use calloc. If not, use realloc to make
 *               the heap one element larger. Insert the data.
 *               Percolate the data up the heap as needed.
 *******************************************************************/
void HeapInsert
  (
  ptrToHeapType heap, /* Heap to work on. (in) */
  int           key,  /* Key to add to heap. (in) */
  dataType      data  /* Data to add to heap. (in) */
  )
{
  int i; /* Used to percolate element up the heap. */

     /*
      * Increment number of elements.
      * Make heap one element larger to accomodate new data.
      */
  heap->numElements++;
  heap->head = realloc( heap->head,
                        ( heap->numElements + 1 ) *
                        sizeof( nodeType ));
  if ( heap->head == NULL ) {
    printf( "Could not allocate memory for heap element!\n" );
    exit( 1 );
  }

     /*
      * Percolate element up the heap.
      */
  i = heap->numElements;
  while (( i > 1 ) && ( heap->head[floor(i/2)].key > key )) {
    heap->head[i] = heap->head[floor(i/2)];
    i = floor( i / 2 );
  }
  heap->head[i].key = key;
  heap->head[i].data = data;
} /* HeapInsert */

/********************************************************************
 *                 HeapDeleteMin
 ********************************************************************
 * SYNOPSIS:     Inserts new element onto the heap.
 * PARAMETERS:   void ( ptrToHeapType heap,
 *                      int *         key,
 *                      dataType *    data )
 * STRATEGY:     Copy the data from the first element into the
 *               return parameters. Then moves the last element to
 *               the first position. Then it percolates the element
 *               down the heap to the correct position by calling
 *               HeapHeapify.
 *******************************************************************/
void HeapDeleteMin
  (
  ptrToHeapType heap, /* Heap to work on. (in) */
  int *         key,  /* Key of first element. (out) */
  dataType *    data  /* Data of first element. (out) */
  )
{
  if ( IsEmptyHeap( heap ) ) {
    printf( "Cannot DeleteMin from an empty heap!\n" );
  }
  else {
    *key = heap->head[1].key;
    *data = heap->head[1].data;
    heap->head[1] = heap->head[heap->numElements];
    heap->numElements--;
    if ( heap->numElements == 0 ) {
      free( heap->head );
      heap->head = NULL;
    }
    else {
      heap->head = realloc( heap->head,
                            ( heap->numElements + 1 ) *
                            sizeof( nodeType ));
      Heapify( heap, 1 );
    }
  }
} /* HeapDeleteMin */

/********************************************************************
 *                 Heapify
 ********************************************************************
 * SYNOPSIS:     Inserts new element onto the heap.
 * PARAMETERS:   void ( ptrToHeapType heap,
 *                      int *         key,
 *                      dataType *    data )
 * STRATEGY:     Start with the element passed and compare it to
 *               the two nodes below it. Swap it with the smaller
 *               of the two. Then call itself with the swapped
 *               element as the pivot.
 *******************************************************************/
void Heapify
  (
  ptrToHeapType heap, /* Heap to heapify. (in) */
  int           i     /* Pivot element in array. (in) */
  )
{
     /* Used to percolate element down heap. */
  int      l;
  int      r;
  int      smallest;
     /* Used to swap elements. */
  nodeType swapElement;

  if ( !IsEmptyHeap( heap ) ) {
    l = 2 * i;
    r = 2 * i + 1;
    if (( l <= heap->numElements ) &&
        ( heap->head[l].key < heap->head[i].key )) {
      smallest = l;
    }
    else {
      smallest = i;
    }
    if (( r <= heap->numElements ) &&
        ( heap->head[r].key < heap->head[smallest].key )) {
      smallest = r;
    }
    if ( smallest != i ) {
      swapElement = heap->head[i];
      heap->head[i] = heap->head[smallest];
      heap->head[smallest] = swapElement;
      Heapify( heap, smallest );
    }
  }
} /* Heapify */

/********************************************************************
 *                 IsEmptyHeap
 ********************************************************************
 * SYNOPSIS:     Checks to see if heap is empty.
 * PARAMETERS:   ( ptrToHeapType heap )
 * RETURNS:      boolean - TRUE if heap is empty.
 * STRATEGY:     Check if number of elements is zero, and if so
 *               return TRUE, else return FALSE.
 *******************************************************************/
boolean IsEmptyHeap
  (
  ptrToHeapType heap /* Heap to check. (in) */
  )
{
  if ( heap->numElements ) {
    return( FALSE );
  }
  else {
    return( TRUE );
  }
} /* IsEmptyHeap */

/********************************************************************
 *                 TopOfHeap
 ********************************************************************
 * SYNOPSIS:     Returns the data in the element at the top of the
 *               heap.
 * PARAMETERS:   void ( ptrToHeapType heap,
 *                      int *         key,
 *                      dataType *    data )
 * STRATEGY:     Simply copies the data from the first element into
 *               the return parameters.
 *******************************************************************/
void TopOfHeap
  (
  ptrToHeapType heap, /* Heap to check. (in) */
  int *         key,  /* Key of first element. (out) */
  dataType *    data  /* Data of first element. (out) */
  )
{
  *key = heap->head[1].key;
  *data = heap->head[1].data;
} /* TopOfHeap */

/********************************************************************
 *                 FreeHeap
 ********************************************************************
 * SYNOPSIS:     Frees the memory associated with the heap. Also
 *               removes any remaining elements on the heap.
 * PARAMETERS:   void ( ptrToHeapType heap )
 * STRATEGY:     Simply free the heap array, then free the heap.
 *******************************************************************/
void FreeHeap
  (
  ptrToHeapType heap /* Heap to free. (in) */
  )
{
     /*
      * Free the heap array.
      * Free the heap structure.
      */
  if ( heap->head != NULL ) {
    free( heap->head );
  }
  free( heap );
} /* FreeHeap */

