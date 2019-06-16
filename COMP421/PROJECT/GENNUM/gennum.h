/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Generate Numbers Header
 * FILE:         gennum.h
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    09/29/95  Initial version
 *
 * DESCRIPTION:
 *      This is the header file for the gennum.c program.
 *
 *******************************************************************/

/********************************************************************
 *                 Constants
 *******************************************************************/
       /* These define the boolean values. */
    #define TRUE  -1
    #define FALSE  0

       /* Maximum number of jobs to create. */
    #define MAXJOBS 10000

/********************************************************************
 *                 Data Types
 *******************************************************************/
       /* Boolean data type. INTERNAL */
    typedef char boolean;

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* A better random function. */
    double BetterRandom
      (
      void
      );

