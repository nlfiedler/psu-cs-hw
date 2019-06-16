/********************************************************************
 *
 *    Copyright 1995 (C) Blue Marsh Softworks -- All rights reserved
 *
 * PROJECT:      COMP 412 CPU Scheduling Project
 * MODULE:       Round Robin Scheduler
 * FILE:         roundr.c
 *
 * AUTHOR:       Nathan Fiedler
 *
 * REVISION HISTORY:
 *      Name   Date      Description
 *      ----   ----      -----------
 *       NF    10/03/95  Initial version
 *
 * DESCRIPTION:
 *      Implementation of the round robin CPU scheduler.
 *
 *******************************************************************/

/********************************************************************
 *                 Headers
 *******************************************************************/
    #include "list.h"
    #include "roundr.h"

/********************************************************************
 *                 main
 ********************************************************************
 * SYNOPSIS:     Main function of the program. Does all the work.
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * STRATEGY:     Using a while loop, run through each job in the
 *               ready queue (list) until all jobs are finished.
 *******************************************************************/
int main
  (
  void
  )
{
     /* Job list. */
  ptrToListType jobList;
     /* Current job node and data. */
  ptrToNodeType curJobNode;
  dataType      curJobData;
     /* File variables. */
  dataType      fileBuffer;
  FILE *        inputFile;
  char          fileName[13];
  unsigned long maxJobs[3] = { 100, 500, 1000 };
     /* The statistics. */
  unsigned long totalWait;
  unsigned long avgWait;
  unsigned long curTime;
  unsigned long totalBurst;
  unsigned long numJobsRan;
     /* Loop variables. */
  int i, j;

     /*
      * Master loop. This runs the program for various input
      * sizes: 100, 500, and 1000 jobs.
      */
  printf( "For a time quantum of: %d\n\n", TIME_QUANTUM );
  for ( j = 0; j < 3; j++ ) {
       /*
        * Inside loop. This runs the program for each
        * data file: STREAM1.DAT, STREAM2.DAT, and STREAM3.DAT.
        */
    for ( i = 1; i < 4; i++ ) {
         /*
          * Initialize all the variables.
          */
      jobList = MakeEmptyList();
      curTime = 0;
      avgWait = 0;
      totalWait = 0;
      totalBurst = 0;
      numJobsRan = 0;
      sprintf( fileName, "stream%d.dat", i );
      inputFile = fopen( fileName, "rt" );
      if ( inputFile == NULL ) {
        printf( "Could not open file stream%d.dat!\n", i );
        exit( 1 );
      }
         /*
          * Start reading from the input file and putting
          * the jobs on the list.
          */
      ReadFromFile( inputFile, &fileBuffer );
      ListAddNode( jobList, fileBuffer );
      totalBurst += fileBuffer.burst;
      numJobsRan++;
      ListHead( jobList, &curJobNode );
      ReadFromFile( inputFile, &fileBuffer );
         /*
          * This loops as long as there are jobs on the
          * ready queue. After that, it's all done.
          */
      while ( !IsEmptyList( jobList )) {
           /*
            * Bring in jobs that are ready to be run.
            */
        if ( numJobsRan < maxJobs[j] ) {
          while (( fileBuffer.arrival <= curTime ) &&
                 ( numJobsRan < maxJobs[j] )) {
            ListAddNode( jobList, fileBuffer );
            totalBurst += fileBuffer.burst;
            numJobsRan++;
            ReadFromFile( inputFile, &fileBuffer );
          }
        }
           /*
            * Run the current job for one time quantum.
            */
        ListDataOfNode( curJobNode, &curJobData );
        if ( curJobData.burst > TIME_QUANTUM ) {
          curJobData.burst -= TIME_QUANTUM;
          curTime += TIME_QUANTUM;
        }
        else {
          curTime += curJobData.burst;
          curJobData.burst = 0;
        }
        ListSetNodeData( curJobNode, curJobData );
           /*
            * If the job is finished, add it's wait time
            * to the total wait time.
            * Remove the job.
            */
        if ( curJobData.burst == 0 ) {
          totalWait += ( curTime -
                         curJobData.arrival -
                         curJobData.origBurst );
          ListDeleteNode( jobList, &curJobNode );
        }
           /*
            * Move to the next job.
            */
        ListNextNode( &curJobNode );
      } /* while */
         /*
          * We're all done. Free the list.
          * Calculate the statistics.
          * Output the results.
          */
      FreeList( jobList );
      avgWait = totalWait / numJobsRan;
      fclose( inputFile );
      printf( "AvgWait TotalWait TotalBurst on %ld jobs.\n",
              numJobsRan );
      printf( "%7ld %9ld %10ld\n",
              avgWait, totalWait, totalBurst );
    } /* for i */
  } /* for j */
  return( 0 );
} /* main */

/********************************************************************
 *                 ReadFromFile
 ********************************************************************
 * SYNOPSIS:     Reads the next job from the file.
 * PARAMETERS:   ( void )
 * RETURNS:      int - Error condition
 * STRATEGY:     Uses fgets and atoi to read the next two numbers
 *               from the file and return them in the data buffer.
 *******************************************************************/
void ReadFromFile
  (
  FILE *     file_p,
  dataType * buffer_p
  )
{
  char tempStr[81];

     /*
      * Read in the arrival time.
      * Convert it to an integer.
      * Read in the burst time.
      * Convert it to an integer.
      * Copy the burst time to the original burst time,
      * which we will use in calculating the wait time.
      */
  fgets( tempStr, 80, file_p );
  (*buffer_p).arrival = atoi( tempStr );
  fgets( tempStr, 80, file_p );
  (*buffer_p).burst = atoi( tempStr );
  (*buffer_p).origBurst = (*buffer_p).burst;
} /* ReadFromFile */

