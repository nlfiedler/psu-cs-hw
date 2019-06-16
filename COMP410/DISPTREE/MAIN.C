/********************************************************************
 *
 * Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 410 Assignments
 * MODULE:       DisplayTree Test program
 * FILE:         main.c
 *
 * AUTHORS:      Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       NF      04/12/95  Initial version
 *
 * DESCRIPTION:
 *      This is a test program for the DisplayTree program.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "disptree.h"

/********************************************************************
 *                 Prototypes
 *******************************************************************/
    childrenRecordType *GetChildren( nodePtr P );

/********************************************************************
 *                 Globals
 *******************************************************************/
    const int  list1[] = { 4, 2, 0, 0, 2, 0, 1, 3, 0, 1, 0, 0,
                           0, 1, 2, 0, 1, 2, 2, 0, 1, 2, 1, 0,
                           0, 0 };
    const char list2[][LABEL_LENGTH] = {
                 "Root\0",
                 "First\0", "Second\0", "Third\0", "Fourth\0",
                 "First\0", "Second\0",
                 "First\0", "Second\0",
                 "First\0", "Second\0", "Third\0",
                 "MoreOfSame\0", "Further\0", "Down\0",
                 "We\0", "Go...\0", "Eighteen\0", "Nineteen\0",
                 "Twenty\0", "One\0", "Two\0", "Three\0",
                 "Four\0", "Five\0", "Six\0" };
    int  list1Count = 0,
         list2Count = 0;

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     This is the main function of the program. It calls
 *               DisplayTree.
 * CALLED BY:    Operating system
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * SIDE EFFECTS:
 * STRATEGY:     Call DisplayTree.
 ****************************************************************/
int main( void ) {
  nodeRecordType root;
  unsigned long  mem;

  /* Print free memory available. */
  mem = coreleft();

  /* Set up root node for DisplayTree. */
  root.Node = &root;
  strncpy( root.Label, list2[list2Count], LABEL_LENGTH );
  list2Count += 1;
  /* Call DisplayTree. */
  DisplayTree( &root, TRUE );

  /* Free any memory allocated by main(). */

  /* Print free memory available. */
  printf( "Memory left allocted: %ld\n", ( mem - coreleft() ) );
  return( 0 ); /* Return to DOS */
} /* main */

/********************************************************************
 *                 GetChildren
 ********************************************************************
 * SYNOPSIS:     Builds a list of children nodes and returns them
 *               to DisplayTree.
 * CALLED BY:    DisplayTree
 * PARAMETERS:   ( nodePtr P, childrenRecordType childrenRecord )
 * RETURNS:      void
 * SIDE EFFECTS:
 * STRATEGY:     Make a list of dummy values and return them.
 ****************************************************************/
childrenRecordType *GetChildren( nodePtr P ) {
  int                i, /* Number of phony children. */
                     j; /* Used in for loop. */
  childrenArrayType  childrenArray;
  childrenRecordType *children;

  if ( P == NULL ) { } /* Do something with P. */
  i = list1[list1Count];
  list1Count += 1;
//  printf( "%ld\n", coreleft() );
  childrenArray = (childrenArrayType)calloc( i,
                   sizeof( nodeRecordType ));
  children = (childrenRecordType*)malloc( sizeof( childrenRecordType ));
//  printf( "%ld\n", coreleft() );
//  getchar();
  if ((( childrenArray == NULL ) || ( children == NULL )) &&
      ( i != 0 )) {
    printf( "Memory allocation failed in GetChildren.\n" );
    exit( 1 );
  } /* if */
  for ( j = 0; j < i; j++ ) {
    childrenArray[j].Node = (void*)2; /* Something non-NULL and
                                       * not marker value. */   
    strncpy( childrenArray[j].Label, list2[list2Count], LABEL_LENGTH );
    list2Count += 1;
  } /* for */
  children->numChildren = i;
  children->childrenArray = childrenArray;
  return( children );
} /* GetChildren */

