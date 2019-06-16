//*******************************************************************
// Program  : CmpEt 401, Networking Ethernet Analyzer
// Module   : UserStats.cpp
// Date     : 06/14/95
// Author   : Ken Sievers; Email: kxs161@psuvm.psu.edu
// Compiler : Borland C++
// DESCRIPTION:
//      This is an implemenation of a linked list, made
//      especially for the LAN Analyzer project.
//*******************************************************************

#include   <stdio.h>
#include  <stdlib.h>
#include   "stats.h"
#include "usrlist.h"

//*******************************************************************
// Function   : Initialize
// Date       :
// Purpose    : Set the head pointer to NULL.
//*******************************************************************

void Initialize( userStatsPtr *head ) {
   *head = NULL;

}   //  end Initialize


//*******************************************************************
// Function   : Allocate
// Date       :
// Purpose    : Allocate another node, without putting it in the
//              list.
//*******************************************************************

userStatsPtr Allocate( void ) {
   userStatsPtr ptr;

   ptr = (userStatsPtr)malloc( sizeof( userStatsType ) );
   if ( ptr == NULL ) {
      printf( "Error: Cannot allocate userStatsType in Allocate().\n" );
   }
   return( ptr );

}   //  end Allocate()



//*******************************************************************
// Function   : Insert
// Date       :
// Purpose    : Given some data, make a new node and insert the
//              data into that node. Place the node and the head
//              of the list.
//*******************************************************************

void Insert( userStatsPtr *head, userStatsType rec ) {

   userStatsPtr newnode;

   newnode = Allocate();

   if ( newnode == NULL ) {
      printf( "insert(): Error: cannot get new node.\n" );
      return;

   } else {
      newnode->userID = rec.userID;

      newnode->packetCountPerSec = rec.packetCountPerSec;
      newnode->errorCountPerSec  = rec.errorCountPerSec;
      newnode->byteCountPerSec   = rec.byteCountPerSec;

      newnode->packetCountTotal  = rec.packetCountTotal;
      newnode->errorCountTotal   = rec.errorCountTotal;
      newnode->byteCountTotal    = rec.byteCountTotal;

      newnode->next = (*head);
      (*head)       = newnode;

   }  // end if

   return;

}   // end Insert()



//*******************************************************************
// Function   : Destroy
// Date       :
// Purpose    : Deallocate all the nodes in the list, then set the
//              head of the list to null.
//*******************************************************************

void Destroy( userStatsPtr *head ) {
   userStatsPtr ptr;
   userStatsPtr prev;

   ptr = *head;

   while ( ptr != NULL ) {
      prev = ptr;
      ptr  = ptr->next;
      free( prev );

   }
   Initialize( head );

}   //  end Destroy()


//*******************************************************************
// Function   : FindUser
// Date       :
// Purpose    : Search the list looking for a particular user.
//*******************************************************************

userStatsPtr FindUser( userStatsPtr head, int key ) {

   userStatsPtr current = head;
   int          found   = FALSE;

   while (( !found ) && ( current != NULL )) {
      if ( current->userID == key )
         found = TRUE;

      else {
         current = current->next;

      } //end if

   }  // end while

   return ( current );

}  // end FindUser


//*******************************************************************
// Function   : SortUser
// Date       :
// Purpose    : Sorts the list by the user ID number.
//*******************************************************************

void SortUser
   (
   userStatsPtr * head, // Head of list to sort.
   char           key   // Key to sort by:
                        //   P = packet/second descending,
                        //   p = packet/second ascending
                        //   E = error/second descending
                        //   e = error/second ascending
                        //   B = bytes/second descending
                        //   b = bytes/second ascending
   )
{

   userStatsPtr last = NULL, // the last  node moved to the head
   first   =  NULL,    // the first node moved to the head
   start   = *head,    // start node of current search
   mmPrev  = *head,
   mm      = *head,    // current max OR min
   curPrev = *head,
   current = *head;    // start at head

   while ( start != NULL ) {

      current = start;
      mm      = start;
      mmPrev  = first;

      while ( current != NULL ) {

         switch ( key ) {

         case 'P' :
            if ( current->packetCountPerSec > mm->packetCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         case 'E' :
            if ( current->errorCountPerSec > mm->errorCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         case 'B' :
            if ( current->byteCountPerSec > mm->byteCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         case 'p' :
            if ( current->packetCountPerSec < mm->packetCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         case 'e' :
            if ( current->errorCountPerSec < mm->errorCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         case 'b' :
            if ( current->byteCountPerSec < mm->byteCountPerSec ) {
               mmPrev = curPrev;
               mm     = current;
            }  //  end if
            break;

         }  //  end switch

         curPrev = current;
         current = current->next;

      }  // end while

      mmPrev->next = mm->next;
      mm->next     = last;
      last         = mm;

      if ( first == NULL ) {
         first = last;
      }

   }  // end while

   *head = last;

}  // end SortUser

