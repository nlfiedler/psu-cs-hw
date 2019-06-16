/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Round Robin Header
 * FILE:         roundr.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/03/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the roundr.c program.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include <alloc.h>

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* Amount of time each job is run while going
        * through the loop. */
    #define TIME_QUANTUM 10

/********************************************************************
 *                 Data Types
 *******************************************************************/

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Read the next job from the file. */
    void ReadFromFile
      (
      FILE *     file_p,
      dataType * buffer_p
      );

