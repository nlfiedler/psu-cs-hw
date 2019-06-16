/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       Pancakes
 * FILE:         pancake.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    02/05/95  Initial version
 *
 * DESCRIPTION:
 *      Implementation of the ACM programming contest problem
 *      involving the construction of a data type similar to a
 *      stack of pancakes, with a "flip" operation to reverse
 *      the order of the elements starting at a particular position.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <alloc.h>
    #include <dos.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE         -1
    #define FALSE         0
    #define MAX_NUM_WIDTH 10
    #define START         50
    #define STEP          50
    #define MAX_SIZE      5000

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Type for element data */
    typedef int dataType;

       /* Type for stack node type */
    typedef struct node {
      dataType      num;
      struct node * next;
    } nodeType, * ptrToNodeType;

       /* Type for stack type */
    typedef struct {
      int           numItems;
      ptrToNodeType Head;
    } stackType, * ptrToStackType;

    typedef unsigned char boolean;

    typedef int * arrayType;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Used with qsort to compare two items. */
    int qsort_compare
      (
      const void *a,
      const void *b
      );

       /* Finds the difference in time. */
    float CalcTime
      (
      struct time startp,
      struct time endp
      );

       /* Initializes the lists by loading the data from file. */
    boolean InitLists
      (
      arrayType array,
      int N,
      ptrToStackType PS
      );

       /* Makes a new pancake stack. */
    ptrToStackType MakeEmptyPStack
      (
      void
      );

       /* Frees a pancake stack. */
    void FreePStack
      (
      ptrToStackType PS
      );

       /* Returns TRUE if pancake stack is empty. */
    boolean IsEmptyPStack
      (
      ptrToStackType PS
      );

       /* Puts a new element on the stack. */
    void Push
      (
      ptrToStackType PS,
      dataType x
      );

       /* Removes the top element from the stack. */
    dataType Pop
      (
      ptrToStackType PS
      );

       /* Flips the stack at a given position. */
    void Flip
      (
      ptrToStackType PS,
      int pos
      );

       /* Finds the maximum element in the stack. */
    int FindMax
      (
      ptrToStackType PS,
      int pos
      );

       /* Sorts the stack by flipping. */
    void PStackSort
      (
      ptrToStackType PS
      );

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int    argc,
 *                 char * argv[] )
 * RETURNS:      int - Error condition
 * STRATEGY:
 *******************************************************************/
int main
  (
  int argc,
  char * argv[]
  )
{
     /* Used in main for loop, # of elements in list. */
  int N;
     /* Stack to be used in test. */
  ptrToStackType aStack;
     /* Array to be used in test. */
  arrayType numberArray;
     /* Start and end time structs. */
  struct time start,
              end;
     /* File for qsort and pancake sort results. */
  FILE * QSortFile,
       * PanFile;

  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "Pancakes  Copyright (C) 1995 Blue Marsh Softworks\n" );
    printf( "Implements a stack that can be flipped at a particular\n" );
    printf( "position in the stack. COMP 410 - DSAA\n" );
    printf( "Usage: pancake\n" );
    exit( 0 );
  }
  if ( argc > 1 ) {
    printf( "For help, type: pancake /?\n" );
    exit( 1 );
  }
     /*
      * Create and open the output files for writing.
      */
  if (( QSortFile = fopen( "prob22-1.out", "wt" )) == 0 ) {
    printf( "Error creating prob22-1.out.\n" );
    exit( 1 );
  }
  if (( PanFile = fopen( "prob22-2.out", "wt" )) == 0 ) {
    printf( "Error creating prob22-2.out.\n" );
    exit( 1 );
  }
     /*
      * Allocate an array.
      */
  if (( numberArray = (arrayType)calloc( MAX_SIZE,
        sizeof( int ))) == 0 ) {
    printf( "Memory allocation failed -- program terminated.\n" );
    exit( 1 );
  }

     /*
      * Perform tests on the two algorithms.
      */
  for ( N = START; N <= MAX_SIZE; N+=STEP ) {
	  printf( "N = %d\n", N );
       /* Create an empty test stack */
    aStack = MakeEmptyPStack();
       /* Open and read in random numbers. */
    if ( InitLists( numberArray, N, aStack ) ) {
      break; /* Error, break out of loop */
    }
       /* Sort the array in increasing order */
    gettime( &start );
    qsort( numberArray, N, sizeof( dataType), qsort_compare );
    gettime( &end );
       /* Output size and time */
    fprintf( QSortFile, "%d %.4f\n", N, CalcTime( start, end ));
    fflush( QSortFile );
    gettime( &start );
       /* Sort the pancake stack in increasing order */
    PStackSort( aStack );
    gettime( &end );
       /* Output size and time */
    fprintf( PanFile, "%d %.4f\n", N, CalcTime( start, end ));
    fflush( PanFile );
       /* Free the stack so we can start over */
    FreePStack( aStack );
  }
  fclose( QSortFile );
  fclose( PanFile );
  return( 0 );
} /* main */

/********************************************************************
 *                 qsort_compare
 ********************************************************************
 * SYNOPSIS:     Does the comparison necessary for the qsort function.
 * CALLED BY:    qsort
 * PARAMETERS:   ( const void *a,
 *                 const void *b )
 * RETURNS:      int - Comparison result
 * STRATEGY:     Does subtraction to return comparison value.
 *******************************************************************/
int qsort_compare( const void *a, const void *b ) {
  return( *(int*)a - *(int*)b );
} /* qsort_compare */

/********************************************************************
 *                 CalcTime
 ********************************************************************
 * SYNOPSIS:     Calculate execution time using start and end
 *               time and return value in seconds.
 * CALLED BY:    main
 * PARAMETERS:   ( struct time startp, struct time endp )
 * RETURNS:      long double  Difference in seconds
 * STRATEGY:     Use simple arithmetic to calculate time difference.
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      01/28/95  Initial version
 *******************************************************************/
float CalcTime
  (
  struct time startp,
  struct time endp
  )
{
  float startt, /* Start as float */
        endt;   /* End as float */

     /* Convert start time to seconds */
  startt = startp.ti_hund; /* Get hund into a float */
  startt = startt / 100; /* Now divide */
  startt = startt + startp.ti_hour * 3600;
  startt = startt + startp.ti_min * 60;
  startt = startt + startp.ti_sec;
     /* Convert end time to seconds */
  endt = endp.ti_hund; /* Get hund into a float */
  endt = endt / 100; /* Now divide */
  endt = endt + endp.ti_hour * 3600;
  endt = endt + endp.ti_min * 60;
  endt = endt + endp.ti_sec;
     /* Calculate time difference */
  endt = endt - startt;
  return( endt );
} /* CalcTime */

/********************************************************************
 *                 InitLists
 ********************************************************************
 * SYNOPSIS:     Initializes the number array and linked list.
 * CALLED BY:    main
 * PARAMETERS:   ( arrayType array, int N, ptrToStackType PS )
 * RETURNS:      boolean  TRUE if error
 * STRATEGY:     Opens random.num file and reads in numbers.
 *******************************************************************/
boolean InitLists
  (
  arrayType array,
  int N,
  ptrToStackType PS
  )
{
     /* The input file of random numbers */
  FILE * RandomFile;
     /* Used in converting numbers */
  char   convStr[MAX_NUM_WIDTH];
     /* Used in for loop */
  int    i;

  if (( RandomFile = fopen( "random.num", "rt" )) == 0 ) {
    printf( "Error opening random number file.\n" );
    return( TRUE );
  }
  for ( i = 0; i < N; i++ ) {
    fgets( convStr, MAX_NUM_WIDTH, RandomFile );
       /* We won't worry about EOF here, since the file is
        * guaranteed to be 15000+ elements, more than enough. */
    array[i] = atoi( convStr );
    Push( PS, array[i] );
  }
  fclose( RandomFile );
  return( FALSE );
} /* InitLists */

/********************************************************************
 *                 MakeEmptyPStack
 ********************************************************************
 * SYNOPSIS:     Creates a new, empty pancake stack.
 * CALLED BY:    main
 * PARAMETERS:   ( void )
 * RETURNS:      ptrToStackType - Pointer to newly created stack
 * STRATEGY:     Simply creates an empty stack data structure.
 * RUNNING TIME: O(1)
 *******************************************************************/
ptrToStackType MakeEmptyPStack
  (
  void
  )
{
  ptrToStackType PS; /* Temporary stack pointer */

     /*
      * Allocate space for the stack structure.
      * Set the number of elements to zero and null out
      * the head pointer, to indicate that there are no
      * elements on the stack yet.
      */
  if (( PS = malloc( sizeof( stackType ))) == 0 ) {
    printf( "Error allocating more memory.\n" );
    exit( 1 );
  }
  PS->numItems = 0;
  PS->Head = NULL;
  return( PS );
} /* MakeEmptyPStack */

/********************************************************************
 *                 FreePStack
 ********************************************************************
 * SYNOPSIS:     Frees all the nodes of the stack and then frees the
 *               stack itself.
 * CALLED BY:    main
 * PARAMETERS:   void ( ptrToStackType PS )
 * STRATEGY:     First free each of the nodes by calling Pop until
 *               there are no more items, then free the stack.
 * RUNNING TIME: O(N)
 *******************************************************************/
void FreePStack
  (
  ptrToStackType PS /* Stack to free. */
  )
{
  while ( !IsEmptyPStack( PS ) ) {
    Pop( PS );
  }
  free( PS );
} /* FreePStack */

/********************************************************************
 *                 IsEmptyPStack
 ********************************************************************
 * SYNOPSIS:     Checks if the pancake stack is empty.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      boolean - TRUE if stack is empty
 * STRATEGY:     Checks PS->numItems == 0 and returns result.
 * RUNNING TIME: O(1)
 *******************************************************************/
boolean IsEmptyPStack( ptrToStackType PS ) {
  return( PS->numItems == 0 );
} /* IsEmptyPStack */

/********************************************************************
 *                 Push
 ********************************************************************
 * SYNOPSIS:     Pushes the data onto the stack.
 * CALLED BY:    main
 * PARAMETERS:   void ( ptrToStackType PS,
 *                      dataType       x )
 * STRATEGY:     Allocates a new node type and places the data into
 *               the num field of that node.
 * RUNNING TIME: O(1)
 *******************************************************************/
void Push
  (
  ptrToStackType PS,
  dataType x
  )
{
     /* Temporary pointer */
  ptrToNodeType ptr;

     /*
      * Allocate a new element.
      * Put this element in front of the head of the stack, ie
      * put it on top.
      * Set the data and increment the number of elements on
      * the stack.
      */
  if (( ptr = malloc( sizeof( nodeType ))) == NULL ) {
    printf( "Error allocating more memory.\n" );
    exit( 1 );
  }
  ptr->next = PS->Head;
  PS->Head = ptr;
  ptr->num = x;
  PS->numItems++;
} /* Push */

/********************************************************************
 *                 Pop
 ********************************************************************
 * SYNOPSIS:     Pops the top element from the stack.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      dataType - Element's data
 * STRATEGY:     Pops the top element from the stack and returns it,
 *               first checking to see if the stack is empty.
 * RUNNING TIME: O(1)
 *******************************************************************/
dataType Pop
  (
  ptrToStackType PS
  )
{
     /* Temporary data */
  dataType x;
     /* Temporary pointer */
  ptrToNodeType ptr;

     /*
      * Store the information of the head element in a temp
      * variable.
      * Point the head to the next element, making it
      * the new top of the stack.
      * Free the old head.
      * Decrement the number of elements.
      * Return the data from the old head.
      */
  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot pop from an empty stack.\n" );
    exit( 1 );
  }
  x = PS->Head->num;
  ptr = PS->Head;
  PS->Head = ptr->next;
  free( ptr );
  PS->numItems--;
  return( x );
} /* Pop */

/********************************************************************
 *                 Flip
 ********************************************************************
 * SYNOPSIS:     Flips the elements in the stack above the specified
 *               position (relative to the bottom of the stack).
 * CALLED BY:    main
 * PARAMETERS:   void ( ptrToStackType PS,
 *                      int            pos )
 * STRATEGY:     Uses the link reversal technique covered in COMP
 *               410 on 02/02/95.
 * RUNNING TIME: O(N)
 *******************************************************************/
void Flip
  (
  ptrToStackType PS,
  int pos
  )
{
     /* Temporary node pointers, used in the link reversal */
  ptrToNodeType P, Q, T;
     /* How many links to reverse */
  int stopAfter;

  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot flip an empty stack.\n" );
    exit( 1 );
  }
     /*
      * Initialize the pointers.
      * Walk down the links until we reach the flipping point,
      * reversing the links as we go.
      */
  if ( PS->numItems != pos ) {
    P = 0;
    Q = PS->Head;
    stopAfter = PS->numItems - pos + 1;
    do {
      T = Q->next;
      Q->next = P;
      P = Q;
      Q = T;
      stopAfter--;
    } while ( stopAfter );
    PS->Head->next = Q;
    PS->Head = P;
  }
} /* Flip */

/********************************************************************
 *                 FindMax
 ********************************************************************
 * SYNOPSIS:     Finds the position of the largest element after
 *               the specified position (from the bottom) in the stack.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS,
 *                 int            pos )
 * RETURNS:      int - Position of maximum item (from bottom)
 * STRATEGY:     Searches the stack for the largest item until the
 *               the position is reached by keeping a running max
 *               and comparing each node's data to that max.
 * RUNNING TIME: O(N)
 *******************************************************************/
int FindMax
  (
  ptrToStackType PS, /* Stack to find max element in. */
  int            pos /* Position to start from. */
  )
{
     /* Used in while loop. */
  int j, i;
     /* Position of maximum element so far. */
  int maxPos;
     /* Maximum element so far. */
  dataType max;
     /* Used in while loop, points to node being examined. */
  ptrToNodeType ptr;

  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot find maximum of an empty stack.\n" );
    exit( 1 );
  }
     /*
      * Set number of elements to examine.
      * Set max index to head element index.
      * Set max data to head element data.
      * Point to element after head.
      * Set index to "max" element.
      */
  j = PS->numItems - pos;
  maxPos = PS->numItems;
  max = PS->Head->num;
  ptr = PS->Head->next;
  i = PS->numItems - 1;
     /*
      * Walk the list, comparing each element with the max
      * so far. If a new max is found, set the index.
      * Return the maximum found.
      */
  while (( ptr != 0 ) && ( j != 0 )) {
    if ( ptr->num > max ) {
      max = ptr->num;
      maxPos = i;
    }
    ptr = ptr->next;
    j--;
    i--;
  }
  return( maxPos );
} /* FindMax */

/********************************************************************
 *                 PStackSort
 ********************************************************************
 * SYNOPSIS:     Sorts the pancake stack in increasing order.
 * CALLED BY:    main
 * PARAMETERS:   void ( ptrToStackType PS )
 * STRATEGY:     Sorts the pancake stack using a series of flips.
 *               This is based on the algorithm in Problem 2-2.
 * RUNNING TIME: O(N^2)
 *******************************************************************/
void PStackSort( ptrToStackType PS ) {
  int i, /* Used in for loop */
      pos; /* Position of maximum element in sub-stack */

     /*
      * Find the maximum in the sub-stack.
      * Flip from that point.
      * Flip the sub-stack.
      */
  if ( !IsEmptyPStack( PS ) ) {
    for ( i = 1; i <= PS->numItems; i++ ) {
      pos = FindMax( PS, i );
      if ( pos != i ) {
        Flip( PS, pos );
        Flip( PS, i );
      }
    }
  }
} /* PStackSort */

