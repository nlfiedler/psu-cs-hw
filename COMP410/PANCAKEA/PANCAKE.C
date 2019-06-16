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
    #include <time.h>

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
 *                 Typedefs
 *******************************************************************/
    typedef unsigned char boolean;
    /* Type for element data */
    typedef int dataType;
    /* Type for array (a pointer) */
    typedef dataType *arrayType;
    /* Type for stack type */
    typedef struct {
            int numItems;
      arrayType Head;
    } stackType, *ptrToStackType;

/********************************************************************
 *                 Prototypes
 *******************************************************************/
               int qsort_compare( const void *a, const void *b );
           boolean InitLists( arrayType array, int N,
                              ptrToStackType PS );
    ptrToStackType MakeEmptyPStack( void );
              void FreePStack( ptrToStackType PS );
           boolean IsEmptyPStack( ptrToStackType PS );
              void Push( ptrToStackType PS, dataType x );
          dataType Pop( ptrToStackType PS );
              void Flip( ptrToStackType PS, int pos );
               int FindMax( ptrToStackType PS, int pos );
              void PStackSort( ptrToStackType PS );

/********************************************************************
 *                 Globals
 *******************************************************************/

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * SIDE EFFECTS:
 * STRATEGY:
 *******************************************************************/
int main( int argc, char *argv[] ) {
				 int N; /* Used in main for loop,
                     * # of elements in list */
  ptrToStackType aStack; /* Stack to be used in test */
       arrayType numberArray; /* Array to be used in test */
           float start,
                 end;  /* Start and end times */
            FILE *QSortFile, /* File for qsort results */
                 *PanFile; /* File for pancake sort results */

  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "Pancakes  Copyright (C) 1995 Blue Marsh Softworks\n" );
    printf( "Implements a stack that can be flipped at a particular\n" );
    printf( "position in the stack. COMP 410 - DSAA\n" );
    printf( "Usage: pancake\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  if ( argc > 1 ) {
    printf( "For help, type: pancake /?\n" );
    exit( 1 ); /* Exit to DOS with error */
  } /* if */
  /* Create and open the output files for writing. */
  if (( QSortFile = fopen( "prob22-1.out", "wt" )) == 0 ) {
    printf( "Error creating prob22-1.out.\n" );
    exit( 1 );
  } /* if */
  if (( PanFile = fopen( "prob22-2.out", "wt" )) == 0 ) {
    printf( "Error creating prob22-2.out.\n" );
    exit( 1 );
  } /* if */
  /* Allocate an array */
  if (( numberArray = (arrayType)calloc( MAX_SIZE,
        sizeof( int ))) == 0 ) {
    printf( "Memory allocation failed -- program terminated.\n" );
    exit( 1 );
  } /* if */

  /* Perform tests on the two algorithms */
  for ( N = START; N <= MAX_SIZE; N+=STEP ) {
	  printf( "N = %d\n", N );
    /* Create an empty test stack */
    aStack = MakeEmptyPStack();
    /* Open and read in random numbers. */
    if ( InitLists( numberArray, N, aStack ) ) {
      break; /* Error, break out of loop */
    } /* if */
    /* Sort the array in increasing order */
    start = (( clock() / CLK_TCK ) / 100 );
    qsort( numberArray, N, sizeof( dataType), qsort_compare );
    end = (( clock() / CLK_TCK ) / 100 );
    /* Output size and time */
    fprintf( QSortFile, "%d %.4f\n", N, end - start );
    fflush( QSortFile );
    start = (( clock() / CLK_TCK ) / 100 );
    /* Sort the pancake stack in increasing order */
    PStackSort( aStack );
    end = (( clock() / CLK_TCK ) / 100 );
    /* Output size and time */
    fprintf( PanFile, "%d %.4f\n", N, end - start );
    fflush( PanFile );
    /* Free the stack so we can start over */
    FreePStack( aStack );
  } /* for */
  fclose( QSortFile );
  fclose( PanFile );
  return( 0 ); /* Return to DOS */
} /* main */

/********************************************************************
 *                 qsort_compare
 ********************************************************************
 * SYNOPSIS:     Does the comparison necessary for the qsort function.
 * CALLED BY:    qsort
 * PARAMETERS:   ( const void *a, const void *b )
 * RETURNS:      int  Comparison result
 * SIDE EFFECTS:
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
 * SIDE EFFECTS:
 * STRATEGY:     Use simple arithmetic to calculate time difference.
 * CREATED:      01/28/95
 *******************************************************************/
float CalcTime( struct time startp, struct time endp ) {
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
 * SIDE EFFECTS:
 * STRATEGY:     Opens random.num file and reads in numbers.
 *******************************************************************/
boolean InitLists( arrayType array, int N, ptrToStackType PS ) {
     FILE *RandomFile; /* The input file of random numbers */
     char convStr[MAX_NUM_WIDTH]; /* Used in converting numbers */
      int i; /* Used in for loop */

  if (( RandomFile = fopen( "random.num", "rt" )) == 0 ) {
    printf( "Error opening random number file.\n" );
    return( TRUE );
  } /* if */
  for ( i = 0; i < N; i++ ) {
    fgets( convStr, MAX_NUM_WIDTH, RandomFile );
    /* We won't worry about EOF here, since the file is
     * guaranteed to be 15000+ elements, more than enough. */
    array[i] = atoi( convStr );
    Push( PS, array[i] );
  } /* for */
  fclose( RandomFile ); /* Close input file */
  return( FALSE ); /* No error occured */
} /* InitLists */

/********************************************************************
 *                 MakeEmptyPStack
 ********************************************************************
 * SYNOPSIS:     Creates a new, empty pancake stack.
 * CALLED BY:    main
 * PARAMETERS:   ( void )
 * RETURNS:      ptrToStackType  Pointer to newly created stack
 * SIDE EFFECTS:
 * STRATEGY:     Simply creates an empty stack data structure.
 * RUNNING TIME: O(1)
 *******************************************************************/
ptrToStackType MakeEmptyPStack( void ) {
    ptrToStackType PS; /* Temporary stack pointer */
  static arrayType A; /* Array, stays around awhile */

  if (( PS = malloc( sizeof( stackType ))) == NULL ) {
    printf( "Error allocating memory for stack header.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  if (( A = (arrayType)calloc( MAX_SIZE, sizeof( stackType )))
        == NULL ) {
    printf( "Error allocating memory for stack array.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  PS->numItems = 0; /* Initialize number of elements */
  PS->Head = A; /* Initialize head pointer */
  return( PS ); /* Return pointer to new stack */
} /* MakeEmptyPStack */

/********************************************************************
 *                 FreePStack
 ********************************************************************
 * SYNOPSIS:     Frees all the nodes of the stack and then frees the
 *               stack itself.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     First free each of the nodes by calling Pop until
 *               there are no more items, then free the stack.
 * RUNNING TIME: O(N)
 *******************************************************************/
void FreePStack( ptrToStackType PS ) {
  free( PS->Head ); /* Free stack array */
  free( PS ); /* Free stack header */
} /* FreePStack */

/********************************************************************
 *                 IsEmptyPStack
 ********************************************************************
 * SYNOPSIS:     Checks if the pancake stack is empty.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      boolean  TRUE if stack is empty
 * SIDE EFFECTS:
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
 * PARAMETERS:   ( ptrToStackType PS, dataType x )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Allocates a new node type and places the data into
 *               the num field of that node.
 * RUNNING TIME: O(1)
 *******************************************************************/
void Push( ptrToStackType PS, dataType x ) {
  if ( PS->numItems < MAX_SIZE ) {
    PS->Head[PS->numItems] = x;
    PS->numItems++;
  } else {
    printf( "Stack array overflow in Pancake.exe.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if-else */
} /* Push */

/********************************************************************
 *                 Pop
 ********************************************************************
 * SYNOPSIS:     Pops the top element from the stack.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      dataType  Element's data
 * SIDE EFFECTS:
 * STRATEGY:     Pops the top element from the stack and returns it,
 *               first checking to see if the stack is empty.
 * RUNNING TIME: O(1)
 *******************************************************************/
dataType Pop( ptrToStackType PS ) {
  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot pop from an empty stack.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  PS->numItems--;
  return( PS->Head[PS->numItems] ); /* Return old head data */
} /* Pop */

/********************************************************************
 *                 Flip
 ********************************************************************
 * SYNOPSIS:     Flips the elements in the stack above the specified
 *               position (relative to the bottom of the stack).
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS, int pos )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Uses the link reversal technique covered in COMP
 *               410 on 02/02/95.
 * RUNNING TIME: O(N)
 *******************************************************************/
void Flip( ptrToStackType PS, int pos ) {
  int temp, top, bot;

  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot flip an empty stack.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  bot = pos;
  top = PS->numItems-1;
  while ( bot < top ) {
    temp = PS->Head[top];
    PS->Head[top] = PS->Head[bot];
    PS->Head[bot] = temp;
    bot++;
    top--;
  } /* while */
} /* Flip */

/********************************************************************
 *                 FindMax
 ********************************************************************
 * SYNOPSIS:     Finds the position of the largest element after
 *               the specified position (from the bottom) in the stack.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS, int pos )
 * RETURNS:      int  Position of maximum item (from bottom)
 * SIDE EFFECTS:
 * STRATEGY:     Searches the stack for the largest item until the
 *               the position is reached by keeping a running max
 *               and comparing each node's data to that max.
 * RUNNING TIME: O(N)
 *******************************************************************/
int FindMax( ptrToStackType PS, int pos ) {
       int j, /* Used in while loop, # of elements to look at */
           i,    /* Used in while loop, index of node
                  * being examined */
           maxPos; /* Position of maximum element so far */
  dataType max; /* Maximum element so far */

  if ( IsEmptyPStack( PS ) ) {
    printf( "Cannot find maximum of an empty stack.\n" );
    exit( 1 ); /* Exit to DOS */
  } /* if */
  j = PS->numItems - pos; /* Set # of elements to examine */
  maxPos = pos; /* Set max index to head element index */
  max = PS->Head[pos]; /* Set max data to head element data */
  i = pos + 1; /* Set index to "max" element */
  while ( j != 0 ) {
    if ( PS->Head[i] > max ) {
      max = PS->Head[i]; /* Set new maximum data */
      maxPos = i; /* Set new index */
    } /* if */
    j--; /* Decrement number of elements to examine */
    i++; /* Advance index to "max" element */
  } /* while */
  return( maxPos );
} /* FindMax */

/********************************************************************
 *                 PStackSort
 ********************************************************************
 * SYNOPSIS:     Sorts the pancake stack in increasing order.
 * CALLED BY:    main
 * PARAMETERS:   ( ptrToStackType PS )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Sorts the pancake stack using a series of flips.
 *               This is based on the algorithm in Problem 2-2.
 * RUNNING TIME: O(N^2)
 *******************************************************************/
void PStackSort( ptrToStackType PS ) {
  int i, /* Used in for loop */
      pos; /* Position of maximum element in sub-stack */

  if ( !IsEmptyPStack( PS ) ) {
    for ( i = 0; i < PS->numItems; i++ ) {
      pos = FindMax( PS, i ); /* Find max in sub-stack */
      if ( pos != i ) {
        Flip( PS, pos ); /* Flip at max */
        Flip( PS, i ); /* Flip at i */
      } /* if */
    } /* for */
  } /* if */
} /* PStackSort */

