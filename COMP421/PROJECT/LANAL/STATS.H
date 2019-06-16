//***********************************************************************
// Program    : CmpEt 401, Networking Ethernet Analyzer
// Module     : UserStats.h
// Date       : 11/14/95
// Programer  : Ken Sievers, User Id kxs161@psuvm.psu.edu
// Compiler   : Borland C++
//***********************************************************************

#ifndef __STATS_H
#define __STATS_H

//  *****  Data Structures  *****

    typedef char Boolean;

    typedef unsigned long int countType;

// *****  Constants  *****

    #define TRUE  1
    #define FALSE 0

    // Screen positions for various elements.
    #define TITLE_X 29
    #define TITLE_Y 2

    // List of user stats.
    #define USERS_X 10
    #define USERS_Y 10

    // Lap display.
    #define LAP_X   USERS_X
    #define LAP_Y   20

    // Totals for all users.
    #define TOTALS_X USERS_X + 8
    #define TOTALS_Y 15

    // Turns on test code.
    #define TEST

//  ***** Global Variables  *****

    extern FILE * inFile; // XXX remove this when packet driver done

#endif
