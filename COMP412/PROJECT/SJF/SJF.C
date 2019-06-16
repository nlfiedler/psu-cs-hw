/*****************************************************************************/
/***  This is the Shortest Job First Processing Simulation                 ***/
/***                                                                       ***/
/***  Group Members:  Nathan Fiedler, Rosco Schock, Eric Koons             ***/
/***  Instructor:     Dr. Null                                             ***/
/***  Course:         Comp412  Operating Systems                           ***/
/***  Project:        CPU Scheduling Project                               ***/
/***  Due Date:       10/4/95                                              ***/
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

int main(void) {

	 typedef struct {
		 int arrival_time;
		 int burst_time;
	  } job;

	 job           process;
	 ptrToHeapType job_heap;
	 dataType      curr_job_arriv;
	 int           curr_job_burst;
	 int           count_done;
	 int           count_read;
	 long int      curr_time;
	 long int      wait;
	 long int      total_wait;
	 double        avg_wait;
	 FILE        * infilep;
	 char          tempString[80];
	 long int      tot_burst;
	 char          achFileName[13];
	 int           i;


	 for( i = 1; i < 4; i++ ) {
		 count_done = 0;
		 count_read = 0;
		 curr_time = 0;
		 wait = 0;
		 total_wait = 0;
		 avg_wait = 0;
		 tot_burst = 0;
		 sprintf( achFileName, "stream%d.dat", i );
		 infilep=fopen( achFileName, "rt" );

		 job_heap = MakeEmptyHeap();

		 while( count_done < 100)
		  {
			 if (count_read < 100){
			 do {

				 fgets( tempString, 81, infilep );
				 process.arrival_time = atoi( tempString );

				 fgets( tempString, 81, infilep );
				 process.burst_time = atoi( tempString );
				 count_read++;

				 HeapInsert(job_heap, process.burst_time, process.arrival_time);
				 } while((process.arrival_time <= curr_time) &&
						  (count_read < 100)); // End while
			  }

			  HeapDeleteMin(job_heap, &curr_job_burst, &curr_job_arriv);

			  count_done++;
			  tot_burst += curr_job_burst;
			  wait = curr_time - curr_job_arriv;
			  total_wait += wait;
			  curr_time += curr_job_burst;

		  };   // End while

			avg_wait = total_wait/count_done;
			printf( "For stream%d.dat the results are:\n", i );
			printf("\n  The Average wait time is:  ");
			printf("%lf", avg_wait);
			printf("\n  The total burst time is:  ");
			printf("%ld", tot_burst);
			printf("\n  The total wait time is:  ");
			printf("%ld\n\n", total_wait);

			fclose(infilep);
			FreeHeap(job_heap);
	 } /* for i */

	return(0);
}; //  End main
