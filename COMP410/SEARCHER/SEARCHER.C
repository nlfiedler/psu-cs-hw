/********************************************************************
 *
 *    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:       COMP 410 Assignments
 * MODULE:        Searcher
 * FILE:          searcher.c
 *
 * AUTHOR:        Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    01/25/95  Initial version
 *
 * DESCRIPTION:
 *      This is an assignment for COMP 410 which compares two
 *      searching algorithms, a recursive binary search and a
 *      move-to-front linear search.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <conio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dos.h>
    #include <alloc.h>
    #include <time.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
    #define TRUE         -1
    #define FALSE         0
    #define MAX_SIZE      1000
    #define START         1000
    #define STEP          100
    #define MAX_NUM_WIDTH 10

/********************************************************************
 *                 Typedefs
 *******************************************************************/
    typedef unsigned char boolean;
    typedef int dataType;
    typedef struct node {
	      dataType num;
      struct node *nextNode;
    } nodeType, *ptrType;
    typedef int *arrayType;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
        int qsort_compare( const void *a, const void *b );
      float CalcTime( struct time startp, struct time endp );
    boolean InitLists( arrayType array, int N );
    boolean BinarySearch( arrayType array, int lower, int upper,
			                 dataType x );
    boolean LinearSearch( dataType x );

/********************************************************************
 *                 Globals
 *******************************************************************/
    ptrType Root; /* Points to number list */

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     This is the main function of the program. It calls
 *               the search functions in a loop and prints out the
 *               running time of each.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( int argc, char *argv[] )
 * RETURNS:      int  Error condition
 * SIDE EFFECTS:
 * STRATEGY:     Open necessary files, read in random numbers,
 *               generate random number list and then use each
 *               algorithm to search for numbers, keeping the time
 *               it takes to run each one.
 *******************************************************************/
int main( int argc, char *argv[] ) {
    arrayType numberArray, /* Holds sorted number array */
              randomArray; /* Test array of numbers to look for in
                            * numArray. */
  struct time start, end;  /* Start and stop times */
          int i, N;        /* Used in for loops */
         FILE *BinaryFile, /* Used in outputing results */
              *LinearFile;
      ptrType ptr;         /* Used in freeing linked list */

  /* Check for arguments on commandline */
  if (( argc > 1 ) && ( strcmp( argv[1], "/?" ) == 0 )) {
    printf( "Searcher  Copyright (C) 1995 Blue Marsh Softworks\n" );
    printf( "Compares two searching algorithms for COMP 410.\n" );
    printf( "Usage: searcher\n" );
    exit( 0 ); /* Exit to DOS */
  } /* if */
  if ( argc > 1 ) {
    printf( "For help, type: searcher /?\n" );
    exit( 1 ); /* Exit to DOS with error */
  } /* if */

  /* Allocate space for two arrays of MAX_SIZE */
  if (( numberArray = (arrayType)calloc( MAX_SIZE,
        sizeof( int ))) == 0 ) {
    printf( "Memory allocation failed -- program terminated.\n" );
    exit( 1 );
  } /* if */
  if (( randomArray = (arrayType)calloc( MAX_SIZE,
        sizeof( int ))) == 0 ) {
    printf( "Memory allocation failed -- program terminated.\n" );
    exit( 1 );
  } /* if */
  /* Create and open the output files for writing. */
  if (( BinaryFile = fopen( "prob13-1.out", "wt" )) == 0 ) {
    printf( "Error creating prob13-1.out.\n" );
    exit( 1 );
  } /* if */
  if (( LinearFile = fopen( "prob13-2.out", "wt" )) == 0 ) {
    printf( "Error creating prob13-2.out.\n" );
    exit( 1 );
  } /* if */

  Root = 0; /* Initialize root to linked list */
  /* Perform tests on the two algorithms */
  for ( N = START; N <= MAX_SIZE; N+=STEP ) {
	 printf( "N = %d\n", N );
    /* Open and read in random numbers. */
    if ( InitLists( numberArray, N ) ) {
      break;
    } /* if */
    /* Sort the array in increasing order */
    qsort( numberArray, N, sizeof( dataType), qsort_compare );
    /* Initialize the test array of random numbers. */
    randomize();
    for ( i = 0; i < N; i++ ) {
      randomArray[i] = random( 32767 );
    } /* for */
    gettime( &start );
    for ( i = 0; i < N; i++ ) {
      BinarySearch( numberArray, 0, N-1, randomArray[i] );
    } /* for */
    gettime( &end );
    /* Output size and time */
    fprintf( BinaryFile, "%d %.2f\n", N, CalcTime( start, end ));
    fflush( BinaryFile );
    gettime( &start );
    for ( i = 0; i < N; i++ ) {
      LinearSearch( randomArray[i] );
    } /* for */
    gettime( &end );
    /* Output size and time */
    fprintf( LinearFile, "%d %.2f\n", N, CalcTime( start, end ));
    fflush( LinearFile );
  } /* for */

  /* Deallocate the linked list */
  while ( Root != 0 ) {
    ptr = Root->nextNode;
    free( Root );
    Root = ptr;
  } /* while */
  fclose( BinaryFile );
  fclose( LinearFile ); /* Close output files */
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
 * RETURNS:      float  Difference in seconds
 * SIDE EFFECTS:
 * STRATEGY:     Use simple arithmetic to calculate time difference.
 * CREATED:      01/28/95
 *******************************************************************/
float CalcTime( struct time startp, struct time endp ) {
  float startt; /* Start as float */
  float endt;   /* End as float */

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
 * PARAMETERS:   ( arrayType array, int N )
 * RETURNS:      boolean  TRUE if error
 * SIDE EFFECTS:
 * STRATEGY:     Opens random.num file and reads in numbers.
 *******************************************************************/
boolean InitLists( arrayType array, int N ) {
  ptrType ptr; /* Temp pointer */
     FILE *RandomFile; /* The input file of random numbers */
     char convStr[MAX_NUM_WIDTH]; /* Used in converting numbers */
      int i; /* Used in for loop */
  boolean allocating;

  if (( RandomFile = fopen( "random.num", "rt" )) == 0 ) {
    printf( "Error opening random number file.\n" );
    return( TRUE );
  } /* if */
  ptr = Root;
  allocating = FALSE;
  for ( i = 0; i < N; i++ ) {
    fgets( convStr, MAX_NUM_WIDTH, RandomFile );
    /* We won't worry about EOF here, since the file is
     * guaranteed to be 15000+ elements, more than enough. */
    array[i] = atoi( convStr );
    if (( ptr != 0 ) && ( !allocating )) {
      ptr->num = array[i];
      ptr = ptr->nextNode;
    } else {
      if (( ptr = malloc( sizeof( nodeType ))) == 0 ) {
        printf( "Out of memory error!\n" );
        return( TRUE );
      } /* if */
      ptr->num = array[i];
      ptr->nextNode = Root;
      Root = ptr; /* Note allocating in reverse order */
      allocating = TRUE;
    } /* if-else */
  } /* for */
  fclose( RandomFile ); /* Close input file */
  return( FALSE ); /* No error occured */
} /* InitLists */

/********************************************************************
 *                 BinarySearch
 ********************************************************************
 * SYNOPSIS:     Searches an array using a recursive binary search.
 *
 * CALLED BY:    main
 * PARAMETERS:   ( arrayType array, int lower, int upper,
 *                 dataType x )
 * RETURNS:      boolean  TRUE if item in array
 * SIDE EFFECTS:
 * STRATEGY:     With two base cases and a conditional, this function
 *               calls itself repeatedly until one of the base cases
 *               causes the function to return. If the item is in the
 *               list the function will return TRUE. If not, it will
 *               return FALSE.
 *******************************************************************/
boolean BinarySearch( arrayType array, int lower, int upper,
                      dataType x ) {
  int mid; /* Midpoint index into array */

  if ( lower > upper ) {
    return( FALSE ); /* Not in list, base case #1 */
  } else {
    mid = ( lower + upper ) / 2; /* Calc middle */
    if ( x == array[mid] ) {
      return( TRUE ); /* In list, base case #2 */
    } else {
      if ( x < array[mid] ) {
        /* Search first half */
        BinarySearch( array, lower, mid-1, x );
      } else {
        /* Search second half */
        BinarySearch( array, mid+1, upper, x );
      } /* if-else */
    } /* if-else */
  } /* if-else */
} /* BinarySearch */

/********************************************************************
 *                 LinearSearch
 ********************************************************************
 * SYNOPSIS:     Searches the linked list for an element.
 * CALLED BY:    main
 * PARAMETERS:   ( dataType x )
 * RETURNS:      boolean  TRUE if item in list
 * SIDE EFFECTS:
 * STRATEGY:     Traverses the list until it finds the element and
 *             when it does it moves the element to the front of the
 *             list. This eventually causes the "popular" elements
 *             to move to the front of the list, and thereby speed
 *             up search time.
 *******************************************************************/
boolean LinearSearch( dataType x ) {
  ptrType cur, prev;
  boolean found;

  cur = Root;
  prev = cur;
  found = FALSE; /* Assume not in list */
  do {
    if ( cur->num == x ) {
      found = TRUE;
    } else {
      prev = cur;
      cur = cur->nextNode;
    } /* if-else */
  } while(( !found ) && ( cur != 0 ));
  /* Found it, now move to front of list */
  if (( found ) && ( cur != Root )) {
    prev->nextNode = cur->nextNode;
    cur->nextNode = Root;
    Root = cur;
  } /* if */
  return( found ); /* Return result */
} /* LinearSearch */

