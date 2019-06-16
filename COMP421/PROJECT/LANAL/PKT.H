//***********************************************************************
// Program    : CmpEt 401, Networking Ethernet Analyzer
// Module     : pkt.h
// Date       : 11/14/95
// Programer  : Ken Sievers; Email kxs161@psuvm.psu.edu
// Compiler   : Borland C++
//***********************************************************************

#ifndef __PACKET_H
#define __PACKET_H

/********************************************************************
 *                 Structures
 *******************************************************************/
       // Shared ADT with packet catcher.
    struct packetData {
       int       destination;
       int       source;
       int       userID;
       int       arrival;
       countType length;
       int       toDo;    // ?
       int       error;
    };

/********************************************************************
 *                 Constants
 *******************************************************************/
       // Interrupt for packet driver.
    #define CONTROL_INT 0x61
    #define CAPTURE     0x00
    #define GET_POINTER 0x01

/********************************************************************
 *                 Function Prototypes
 *******************************************************************/
       /* Call this to enable the packet driver. */
    void Enable_Capture();

       /* This gets the next available packet and returns
        * a pointer to a copy of the packet block. */
    Boolean getPacket( packetData * packet );

#endif

