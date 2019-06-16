/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       List ADT Header
 * FILE:         list.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/03/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the list.c program.
 *      The list is a circular list. In other words, calling
 *      ListPrevNode on the ListHead node will point you to
 *      the ListTail node. If there is only one element in the
 *      list, then you will get the same node back when you
 *      call ListNextNode and ListPrevNode.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* These define the boolean values. */
    #define TRUE  -1
    #define FALSE  0

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Boolean data type. */
    typedef char boolean;

       /* Data type.
        * You change this to whatever you want your data to
        * be, from an int to a pointer to a struct. */
    typedef struct {
      unsigned int arrival;
      unsigned int origBurst;
      unsigned int burst;
    } dataType;

       /* List node type. */
    typedef struct node {
      struct node * prev;
      dataType      data;
      struct node * next;
    } nodeType, * ptrToNodeType;

       /* List structure. */
    typedef struct {
      unsigned int  numElements;
      ptrToNodeType head;
      ptrToNodeType tail;
    } listType, * ptrToListType;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Creates a new list. */
    ptrToListType MakeEmptyList
      (
      void
      );

       /* Free the list structure, and any elements in it. */
    void FreeList
      (
      ptrToListType list
      );

       /* Delete a given node from the list.
        * This leaves node pointing to ListNodePrev.
        * If no elements left, node is null. */
    void ListDeleteNode
      (
      ptrToListType   list,
      ptrToNodeType * node
      );

       /* Add a new node to the end of the list. */
    void ListAddNode
      (
      ptrToListType list,
      dataType      data
      );

       /* Return pointer to head node of list. */
    void ListHead
      (
      ptrToListType   list, /* (in) */
      ptrToNodeType * node  /* (out) */
      );

       /* Return pointer to tail node of list. */
    void ListTail
      (
      ptrToListType   list, /* (in) */
      ptrToNodeType * node  /* (out) */
      );

       /* Return data of the node. */
    void ListDataOfNode
      (
      ptrToNodeType node, /* (in) */
      dataType *    data  /* (out) */
      );

       /* Set the data of a node. */
    void ListSetNodeData
      (
      ptrToNodeType node, /* (in) */
      dataType      data  /* (in) */
      );

       /* Points node to the next node in the list. */
    void ListNextNode
      (
      ptrToNodeType * node /* (in/out) */
      );

       /* Points node to the previous node in the list. */
    void ListPrevNode
      (
      ptrToNodeType * node /* (in/out) */
      );

       /* Returns true if list is empty. */
    boolean IsEmptyList
      (
      ptrToListType list /* (in) */
      );

