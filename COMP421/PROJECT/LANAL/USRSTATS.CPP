//*******************************************************************
//
// Program    : CmpEt 401, Networking Ethernet Analyzer
// Module     : UserStats.cpp
// Date       : 11/14/95
//
// Programer  : Ken Sievers; kxs161@psuvm.psu.edu
//
// Compiler   : Borland C++
//
// REVISION HISTORY:
//      Name     Date      Description
//      ----     ----      -----------
//       KS      11/14/95  Initial version
//       NF      12/05/95  Interfaced w/ packet access code.
//                         Added output functions.
//
// DESCRIPTION:
//      This is responsible for collecting the statistics from the
//      packet driver. It maintains a list of users and associates
//      a set of statistics with each one.
//
//*******************************************************************


//  *****  Included System Header Files  ******
    #include    <stdio.h>
    #include    <conio.h>
    #include    <ctype.h>
    #include    "stats.h"
    #include      "pkt.h"
    #include  "usrlist.h"


//  ***** Global Variables  *****
    FILE * inFile;


//*******************************************************************
//*******************************************************************
int main( void ) {

   // Statistics for all users totaled together.
   countType totalPacketCountPerSec = 0;
   countType totalPacketCount       = 0;
   countType totalErrorCountPerSec  = 0;
   countType totalErrorCount        = 0;
   countType totalByteCountPerSec   = 0;
   countType totalByteCount         = 0;

   // Various moments in time.
   int currentSec;
   int baseSec;
// NF  int timeAtRESET;

   // Indicates when to reset the counts.
   Boolean RESET = FALSE;
   // Indicates when to quit.
   Boolean QUIT = FALSE;
   // Displays current stats in a separate display.
   Boolean LAP = FALSE;

   // User information.
   int           totalUsers = 0;
   packetData    currentPacket;
   userStatsType newUser;
   userStatsPtr  userStatsHead;
   userStatsPtr  userStatsCurrent;

   // User input variables.
   char userCommand;

   // Output file.
   FILE * Stats;

   // Enable the packet driver.
//   Enable_Capture();

   // Create the output file.
   // The user statistics will go to this file.
   if (( Stats = fopen( "stats.txt", "wt" )) == NULL ) {
      printf ( "Error opening 'STATS.TXT'.\n" );
      return ( 0 );
   }

   // Open the input file.
   inFile = fopen( "input.txt", "rt" );

   // Print title.
   clrscr();
   gotoxy( TITLE_X, TITLE_Y );
   printf( "LAN Analyzer\n" );
   gotoxy( 16, wherey() );
   printf( "Rob Burns, Mounir Elabridi, Nathan Fiedler,\n" );
   gotoxy( 16, wherey() );
   printf( "Kevin Forrester, Garret Kress, Ken Sievers.\n" );

   // Initialize the user list.
   Initialize( &userStatsHead );
   //****************************************************************
   // Run until user presses a key.
   do {

      // Get the next packet from the driver.
      getPacket( &currentPacket );

      // Set the time when we started after a reset.
// NF     timeAtRESET = currentPacket.arrival;

      //*************************************************************
      // Totals stats until RESET.
      do{

         // Set start time for first arriving packet.
         // This will be used to detect the packets
         // arriving in the next second.
         currentSec = currentPacket.arrival;
         baseSec = currentSec;

         //**********************************************************
         // Totals stats for the current second.
         do{

            // Walk down the list until the current user is found.
            userStatsCurrent = userStatsHead;
            while (( userStatsCurrent != NULL ) &&
                   ( userStatsCurrent->userID !=
                     currentPacket.userID )) {
               userStatsCurrent = userStatsCurrent->next;
            }  //  end while

            // If we didn't find the user ID, then that means
            // this is a new user. Then make a new node
            // and insert it into the list.
            if ( userStatsCurrent == NULL ) {

               newUser.userID =  currentPacket.userID;

               newUser.packetCountPerSec = 0;
               newUser.errorCountPerSec  = 0;
               newUser.byteCountPerSec   = 0;

               newUser.packetCountTotal = 0;
               newUser.errorCountTotal  = 0;
               newUser.byteCountTotal   = 0;

               newUser.index = totalUsers;
               newUser.updated = FALSE;

               totalUsers++;

               Insert( &userStatsHead, newUser );

               userStatsCurrent = userStatsHead;

            }  //  end if

            // If we found the user, then update user packet
            // counts, and set updated flag.
            userStatsCurrent->packetCountPerSec++;
            userStatsCurrent->packetCountTotal++;

            // Update total packet counts.
            totalPacketCountPerSec++;
            totalPacketCount++;
            userStatsCurrent->updated = TRUE;

            // If there's an error, update the error counts.
            // Else, update the length counts.
            if ( currentPacket.error ) {
               userStatsCurrent->errorCountPerSec++;
               userStatsCurrent->errorCountTotal++;
               totalErrorCountPerSec++;
               totalErrorCount++;
            } else {
               userStatsCurrent->byteCountPerSec += currentPacket.length;
               userStatsCurrent->byteCountTotal  += currentPacket.length;
               totalByteCountPerSec += currentPacket.length;
               totalByteCount       += currentPacket.length;
            }  //  end if

            // Wait for the next packet, or
            // when user quits by pressing a key.
            do {
               // Key fetching routine.
               if ( kbhit() ) {
                  userCommand = toupper( getch() );
                  switch ( userCommand ) {
                     case 'Q' :
                        QUIT = TRUE;
                        break;
                     case 'R' :
                        RESET = TRUE;
                        break;
                     case 'L' :
                        LAP = TRUE;
                        break;
                  }
               }
            } while ((( !QUIT )||( !RESET )||( !LAP )) &&
                     ( !getPacket( &currentPacket )));

            if ( LAP ) {
               // Display current user stats.
               gotoxy( 39, LAP_Y );
               printf( "Lap\n" );
               gotoxy( LAP_X, wherey() );
               printf( "UserID  Packets  Pack/s  Errors  Errors/s" );
               printf( "    Bytes  Bytes/s\n" );
               gotoxy( LAP_X, wherey() );
               printf( "%6d %8lu %7lu %7lu %9lu %8lu %8lu\n",
                       userStatsCurrent->userID,
                       userStatsCurrent->packetCountTotal,
                       userStatsCurrent->packetCountPerSec,
                       userStatsCurrent->errorCountTotal,
                       userStatsCurrent->errorCountPerSec,
                       userStatsCurrent->byteCountTotal,
                       userStatsCurrent->byteCountPerSec );
               LAP = FALSE;
            }

            // Finish loop when current second is over, or
            // when user wants to quit.
            currentSec = currentPacket.arrival;
         } while (( !QUIT ) && (( baseSec + 1 ) > currentSec ));
         //**********************************************************


         // Walk the list and visually update any users who
         // have changed recently.
         gotoxy( 38, USERS_Y );
         printf( "Users\n" );
         gotoxy( USERS_X, wherey() );
         printf( "UserID  Packets  Pack/s  Errors  Errors/s" );
         printf( "    Bytes  Bytes/s\n" );
         userStatsCurrent = userStatsHead;
         while ( userStatsCurrent != NULL ) {
            if ( userStatsCurrent->updated ) {
               gotoxy( USERS_X, wherey() );
               printf( "%6d %8lu %7lu %7lu %9lu %8lu %8lu\n",
                       userStatsCurrent->userID,
                       userStatsCurrent->packetCountTotal,
                       userStatsCurrent->packetCountPerSec,
                       userStatsCurrent->errorCountTotal,
                       userStatsCurrent->errorCountPerSec,
                       userStatsCurrent->byteCountTotal,
                       userStatsCurrent->byteCountPerSec );
               userStatsCurrent->updated = FALSE;
               userStatsCurrent = userStatsCurrent->next;
            } // if
         } // while

         // Output total counts.
         gotoxy( 37, TOTALS_Y );
         printf( "Totals\n" );
         gotoxy( TOTALS_X, wherey() );
         printf( "Packets  Pack/s  Errors  Errors/s" );
         printf( "    Bytes  Bytes/s\n" );
         gotoxy( TOTALS_X, wherey() );
         printf( "%7lu %7lu %7lu %9lu %8lu %8lu\n",
                 totalPacketCount,
                 totalPacketCountPerSec,
                 totalErrorCount,
                 totalErrorCountPerSec,
                 totalByteCount,
                 totalByteCountPerSec );

         // Reset the total per second counts.
         totalPacketCountPerSec = 0;
         totalErrorCountPerSec  = 0;
         totalByteCountPerSec   = 0;

         // Walk down the list and reset per second
         // counts for each user.
         userStatsCurrent = userStatsHead;
         while ( userStatsCurrent != NULL ) {

            userStatsCurrent->packetCountPerSec = 0;
            userStatsCurrent->errorCountPerSec  = 0;
            userStatsCurrent->byteCountPerSec   = 0;

            userStatsCurrent = userStatsCurrent->next;

         }  //  while

      // Finish loop when user wants to reset the counters, or
      // when user wants to quit.
      } while (( !RESET ) && ( !QUIT ));
      //*************************************************************


      // Perform resets.
      RESET = FALSE;

      // Reset the total counts.
      totalPacketCount = 0;
      totalErrorCount  = 0;
      totalByteCount   = 0;

      // Reset the users by destroying the entire user list.
      Destroy( &userStatsHead );

   //****************************************************************
   } while ( !QUIT );
   //****************************************************************

   // Close output file.
   fclose( Stats );

   // Close input file.
   fclose( inFile );

   // Return to OS.
   return( 0 );

}  // main

