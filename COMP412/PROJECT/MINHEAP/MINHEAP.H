/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Priority Heap Header File
 * FILE:         minheap.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/01/95  Initial version
 *
 * DESCRIPTION:
 *      Header file for the priority heap program.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <alloc.h>
    #include <math.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE  -1
    #define FALSE  0

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Type for element data.
        * Change this to whatever data you want stored in the
        * queue. Usually this is a number or a pointer to a node. */
    typedef struct {
      int myData;
    } dataType;

       /* Type for each element in the heap. INTERNAL */
    typedef struct {
      int      key;
      dataType data;
    } nodeType;

       /* Holds number of elements in heap. INTERNAL */
    typedef struct {
      int        numElements;
      nodeType * head;
    } heapType, * ptrToHeapType;

       /* Boolean data type: TRUE or FALSE. */
    typedef char boolean;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Creates a new empty heap. */
    ptrToHeapType MakeEmptyHeap
      (
      void
      );

       /* Insert new element into heap. */
    void HeapInsert
      (
      ptrToHeapType heap, /* Heap to insert element onto. */
      int           key,  /* Key to insert onto heap. */
      dataType      data  /* Data to insert with key. */
      );

       /* Remove the first element from the heap (the smallest). */
    void HeapDeleteMin
      (
      ptrToHeapType heap, /* Heap to remove minimum from. */
      int *         key,  /* Key of minimum element. */
      dataType *    data  /* Data associated with minimum key. */
      );

       /* Percolate the pivot element down the heap. */
    void Heapify
      (
      ptrToHeapType heap, /* Heap to operate on. */
      int           i     /* Pivot element to percolate down. */
      );

       /* Check if heap is empty and return TRUE if so. */
    boolean IsEmptyHeap
      (
      ptrToHeapType heap /* Heap to check. */
      );

       /* Returns data at minimum position. */
    void TopOfHeap
      (
      ptrToHeapType heap, /* Heap to get value from. */
      int *         key,  /* Key of minimum element. */
      dataType *    data  /* Data associated with minimum key. */
      );

       /* Free the heap and all the elements on it. */
    void FreeHeap
      (
      ptrToHeapType heap /* Heap to free. */
      );

