/*****************************************************************************/
/***  This is the First Come First Serve Processing Simulation             ***/
/***                                                                       ***/
/***  Group Members: Nathan Fiedler, Rosco Schock, Eric Koons              ***/
/***  Instructor:    Dr. Null                                              ***/
/***  Course:        Comp412  Operating Systems                            ***/
/***  Project:       CPU Scheduling Project                                ***/
/***  Due Date:      10/4/95                                               ***/
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(void) {

	 typedef struct {
		 int arrival_time;
		 int burst_time;
	  } job;

	 job      process;
	 int      count;
	 long int curr_time;
	 long int wait;
	 long int total_wait;
	 double   avg_wait;
	 FILE *   infilep;
	 long int skip_time;
	 char     tempString[80];
	 long int tot_burst;
	 char     achFileName[13];
	 int      i;


	 for( i = 1; i < 4; i++ ) {
		 count = 0;
		 curr_time = 0;
		 wait = 0;
		 total_wait = 0;
		 avg_wait = 0;
		 skip_time = 0;
		 tot_burst = 0;
		 sprintf( achFileName, "stream%d.dat", i );
		 infilep=fopen( achFileName, "rt" );

		 while( count < 1000)
		  { // begin while

			  fgets( tempString, 81, infilep );
			  process.arrival_time = atoi( tempString );

			  fgets( tempString, 81, infilep );
			  process.burst_time = atoi( tempString );


			  if(process.arrival_time > curr_time) { // begin if

				  skip_time = process.arrival_time - curr_time;
				  curr_time += skip_time;
				 }; //  End if

			  count++;
			  tot_burst += process.burst_time;
			  wait = curr_time - process.arrival_time;

			  total_wait += wait;
			  curr_time += process.burst_time;

		  };   // End while

			avg_wait = total_wait/count;
			printf( "For stream%d.dat the results are:\n", i );
			printf("\n  The Average wait time is:  ");
			printf("%lf", avg_wait);
			printf("\n  The total burst time is:  ");
			printf("%ld", tot_burst);
			printf("\n  The total wait time is:  ");
			printf("%ld\n\n", total_wait);

			fclose(infilep);
	 } /* for i */

	return(0);
}; //  End main

