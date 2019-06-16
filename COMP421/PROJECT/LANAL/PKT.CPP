/********************************************************************
 *
 * PROGRAM:      LAN Analyzer
 * MODULE:       Packet Access Routines
 * FILE:         access.cpp
 *
 * AUTHOR:       Garret Kress
 *
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       GK      12/06/95  Initial version
 *
 * DESCRIPTION:
 *      This file contains the routines for starting and talking to
 *      the packet driver, written by Garret Kress.
 *
 *******************************************************************/

/********************************************************************
 *                 Header Files
 *******************************************************************/
    #include <stdio.h>
    #include <dos.h>      /* Need this for int86x. */
    #include <process.h>
    #include "stats.h"
    #include "pkt.h"

/********************************************************************
 *                 Globals
 *******************************************************************/
       /* These hold the address, size, offset, and time for
        * each packet that is retrieved. */
    long PKT_SEG;
    long PKT_OFF;
    long PKT_SIZE;
    long BIOS_TIME;

       /* These are the CPU registers represented in C form. */
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;


/********************************************************************
 *                 Enable_Capture
 ********************************************************************
 * SYNOPSIS:     Starts the packet capturing program.
 * CALLED BY:    main()
 * STRATEGY:     Initialize the CPU registers and set an interrupt
 *               to wake up the packet driver.
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       GK      12/06/95  Initial version
 *******************************************************************/
void Enable_Capture()
{

   inregs.h.ah = 0x00;
   inregs.h.al = CAPTURE;
   int86x(CONTROL_INT, &inregs, &outregs, &segregs);
}

/********************************************************************
 *                 getPacket
 ********************************************************************
 * SYNOPSIS:     Retrieves a packet from the packet driver.
 * CALLED BY:    main
 * STRATEGY:     Set the CPU registers and set an interrupt to
 *               occur. When copy the contents of the registers so
 *               we can move the packet into stable memory.
 * REVISION HISTORY:
 *      Name     Date      Description
 *      ----     ----      -----------
 *       GK      12/06/95  Initial version
 *       NF      12/06/95  Added test portion.
 *******************************************************************/
Boolean getPacket
  (
  packetData * packet // Returns packet info here (out).
  )
{

#ifdef TEST

   // Read next line from dummy data file.
   // Indicate if we've run dry on packets.
   if (( fscanf( inFile, "%d %d %d %d %lu %d %d\n",
           &(packet->destination),
           &(packet->source),
           &(packet->userID),
           &(packet->arrival),
           &(packet->length),
           &(packet->toDo),
           &(packet->error))) == 7 ) {
      return( TRUE );
   }
   else {
      return( FALSE );
   }

#else

   inregs.h.ah = 0x00;
   inregs.h.al = GET_POINTER;
   int86x( CONTROL_INT, &inregs, &outregs, &segregs );
   PKT_SEG = segregs.es;
   PKT_OFF = outregs.x.di;
   PKT_SIZE = outregs.x.bx;
   BIOS_TIME = outregs.x.dx;

   // Retrieve information from packet and store
   // in the passed "packet" structure.
// XXX Fill these in.
//   packet->destination
//   packet->source
//   packet->userID
   packet->arrival = (int)( BIOS_TIME & 0x0000ffff );
   packet->length = (int)( PKT_SIZE & 0x0000ffff );
   packet->toDo = 0;
   packet->error = 0;

   // Always indicate that a packet is ready.
   return( TRUE );

#endif

} // getPacket

