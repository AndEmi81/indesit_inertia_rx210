/*  DISCLAIMER:                                                               */
/*  We (Renesas Electronics Europe Limited) do not warrant that the Software   */
/*  is free from claims by a third party of copyright, patent, trademark,     */
/*  trade secret or any other intellectual property infringement.             */
/*                                                                            */
/*  Under no circumstances are we liable for any of the following:            */
/*                                                                            */
/*  1. third-party claims against you for losses or damages;                  */
/*  2. loss of, or damage to, your records or data; or                        */
/*  3. economic consequential damages (including lost profits or savings)     */
/*     or incidental damages, even if we are informed of their possibility.   */
/*                                                                            */
/*  We do not warrant uninterrupted or error free operation of the Software.  */
/*  We have no obligation to provide service, defect, correction, or any      */
/*  maintenance for the Software. We have no obligation to supply any         */
/*  Software updates or enhancements to you even if such are or later become  */
/*  available.                                                                */
/*                                                                            */
/*  IF YOU DOWNLOAD OR USE THIS SOFTWARE YOU AGREE TO THESE TERMS.            */
/*                                                                            */
/*  THERE ARE NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE IMPLIED        */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.       */

/***********************************************************************************
FILE NAME  		ramtest_march_HW.h
DESCRIPTION		Initialise the HW (DOC and DMAC) used for the HW versions of
				the RAM memory tests.
***********************************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
26-Aug-201 RTE-PJI First Release
15-Nov-2011 VDE (Version 1.0) (RX210)
***********************************************************************************/
#ifndef _RAMTEST_MARCH_HW_H_INCLUDED_
#define _RAMTEST_MARCH_HW_H_INCLUDED_

/***********************************************************************************
 User Includes
***********************************************************************************/
#include "misratypes.h"

/**********************************************************************************
 #defines
***********************************************************************************/

/*A DMAC Channel can be used by the RAM Tests.
If you don't want to use the DMAC then comment this out.*/
/*#define RAMTEST_USE_DMAC*/

/*Define the DMAC Channel to use.
This must be as defined in iodefine.h (DMAC0, DMAC1, DMAC2 or DMAC3)*/
#define DMAC_CHANNEL 	DMAC0

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
/*Initialise the HW (DOC and DMAC) required for the "HW" versions
of the RAM Tests.*/
void RamTest_March_HW_Init(void);
/*Check if DOC (and DMAC) are functionally working.*/
bool_t RamTest_March_HW_PreTest(void);
/*Check if RamTest_March_HW_Init has been called.*/
bool_t RamTest_March_HW_Is_Init(void);

#ifdef RAMTEST_USE_DMAC
	/*Performs multiple DMAC transfers to transfer specified count*/
	void RamTest_March_HW_DMAC_Multi_Transfer(uint32_t ui32_NumBlocks);
	/*Function blocks until DMAC has completed a transfer.*/
	void RamTest_March_HW_Wait_DMAC(void);
#endif

#endif
