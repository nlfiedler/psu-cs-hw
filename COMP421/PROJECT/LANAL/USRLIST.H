//***********************************************************************
//
// Program    : CmpEt 401, Networking Ethernet Analyzer
// Module     : UserList.cpp
//
// Date       : 11/14/95
// Programer  : Ken Sievers, User Id kxs161@psuvm.psu.edu
//
// Compiler   : Borland C++
//
//***********************************************************************

#ifndef __USRLIST_H
#define __USRLIST_H

//  *****  Data Structures  *****
    typedef struct  node{

       int userID;

       countType packetCountPerSec;
       countType errorCountPerSec;
       countType byteCountPerSec;

       countType packetCountTotal;
       countType errorCountTotal;
       countType byteCountTotal;

       struct node *next;

       int index;

       Boolean updated;
    } userStatsType, * userStatsPtr;


// *****  User Stats List Operations  ******
    void Initialize( userStatsPtr *head );
    userStatsPtr Allocate( void );
    void Insert( userStatsPtr *head, userStatsType rec );
    void Destroy( userStatsPtr *head );
    userStatsPtr FindUser ( userStatsPtr head, int key );
    void SortUser ( userStatsPtr *head, char key );

#endif

