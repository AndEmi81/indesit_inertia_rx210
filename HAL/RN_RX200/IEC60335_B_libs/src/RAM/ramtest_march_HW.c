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
FILE NAME  		ramtest_march_HW.c
DESCRIPTION		Initialise the HW (DOC and DMAC) used for the HW versions of
				the RAM memory tests.
***********************************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
26-Aug-201 RTE-PJI First Release
15-Nov-2011 VDE (Version 1.0) (RX210)
***********************************************************************************/

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"
#include "ramtest_march_HW.h"

/*Acccess to the registers of the device.*/
#include "RN_RX200_reg.h"

/***********************************************************************************
User Program Code
***********************************************************************************/

/***********************************************************************************
Function Name : RamTest_March_HW_Init
Description   :	Initialise the DOC and DMAC module so the RAM tests can use it.

				* NOTE: The DMAC is only used if 'RAMTEST_USE_DMAC' is defined.

				Call this function before using any other function in
				RamTest_March_C_HW.c,RamTest_March_X_WOM_HW.c
				and this file.
Parameters    : 				
Return Value  : 
***********************************************************************************/
void RamTest_March_HW_Init(void)
{
	/*The DOC module is used to perform the comparison of an expected RAM value with
	a value read from RAM.*/
	
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable DOC*/
	SYSTEM.MSTPCRB.BIT.MSTPB6 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;	
	
	/*Configure DOC for comparison mode, detecting mismatch.*/
	DOC.DOCR.BYTE = 0x40;
	
	#ifdef RAMTEST_USE_DMAC
		/*DMAC channel 3 is used for memory copy and memory initialisation.*/
		
		/*Disable register protection (PRC1)*/
		SYSTEM.PRCR.WORD = 0xA502;	
		/*Enable DMAC*/
		SYSTEM.MSTPCRA.BIT.MSTPA28 = 0;
		/*Re-Enable register protection*/
		SYSTEM.PRCR.WORD = 0xA500;
		
		/*DMA top level enable*/
		DMAC.DMAST.BIT.DMST = 1;
		/*Ensure Disabled*/
		DMAC_CHANNEL.DMCNT.BIT.DTE = 0;
		/*Set Normal mode and software request source.*/
		DMAC_CHANNEL.DMTMD.WORD = 0x0000;
		/*No Interrupts*/
		DMAC_CHANNEL.DMINT.BYTE = 0x00;
		/*Software Start, Do not clear SW Start*/
		DMAC_CHANNEL.DMREQ.BYTE = 0x11;
	#endif
}

/***********************************************************************************
Function Name : RamTest_March_HW_PreTest
Description   :	Check if DOC (and DMAC are functionally working.
				(DMAC only tested if RAMTEST_USE_DMAC is #defined)
				
				NOTE: If this returns FALSE then, unless the RAM has
				already been tested using the non-HW versions of the RAM tests,
				it can not be said if it is a RAM or DOC/D<AC problem.			
Parameters    : 				
Return Value  : TRUE = OK, FALSE = Error.
***********************************************************************************/
bool_t RamTest_March_HW_PreTest(void)
{
	#define ARRAY_SIZE 8
	#define EXPECTED_VALUE 0x55u
	
	bool_t bOK = TRUE;
	uint8_t index;
	uint8_t Array_src[ARRAY_SIZE];
	#ifdef RAMTEST_USE_DMAC
		uint8_t Array_dest[ARRAY_SIZE];
	#endif
	
	
	#ifdef RAMTEST_USE_DMAC
		/*Initialise Arrays */
		for(index = 0; index < ARRAY_SIZE; index++)
		{
			Array_src[index] = index;
			Array_dest[index] = 0xA5;
		}
	
		/*DMAC transfer*/
		/*Set source */
		DMAC_CHANNEL.DMSAR = (uint32_t)Array_src;
		/*Set destination */
		DMAC_CHANNEL.DMDAR = (uint32_t)Array_dest;
		/*Count*/
		DMAC_CHANNEL.DMCRA = ARRAY_SIZE;
		/*Source and Destination are incremented*/
		DMAC_CHANNEL.DMAMD.WORD = 0x8080;
		/*Set transfer data width as 8 bit*/
		DMAC_CHANNEL.DMTMD.BIT.SZ = 0;
		/*Enable*/
		DMAC_CHANNEL.DMCNT.BIT.DTE = 1;
	
		/*Wait for completion*/
		while(1 == DMAC_CHANNEL.DMCNT.BIT.DTE){;}
	
		/*Check destination now equalls src */
		for(index = 0; index < ARRAY_SIZE; index++)
		{
			if(Array_src[index] != Array_dest[index])
			{
				/*DMAC Error*/
				bOK = FALSE;
			}
		}
	#endif /*RAMTEST_USE_DMAC*/
	
	/*Set all Array to EXPECTED_VALUE */
	for(index = 0; index < ARRAY_SIZE; index++)
	{
		Array_src[index] = EXPECTED_VALUE;
	}
	
	/*Ensure the DOC flag (DOPCF) is clear before starting.
	Clear by writing to 1 to DOPCFCL */
	DOC.DOCR.BIT.DOPCFCL = 1;
	
	/*Set the DOC expected compare value*/
	DOC.DODSR = EXPECTED_VALUE;
	
	/*Pass all data to DOC*/
	for(index = 0; index < ARRAY_SIZE; index++)
	{
		DOC.DODIR = Array_src[index];
	}
	
	/*Confirm it has NOT detected a mis-match*/
	if(0 != DOC.DOCR.BIT.DOPCF)
	{
		/*DOC detected mismatch, when not expected.*/
		bOK = FALSE;
	}
	
	/*Alter a value in the array and check the DOC detects it as a mismatch*/
	Array_src[ARRAY_SIZE-1] = (uint8_t)~EXPECTED_VALUE;
	
	/*Pass all data to DOC*/
	for(index = 0; index < ARRAY_SIZE; index++)
	{
		DOC.DODIR = Array_src[index];
	}
	
	/*Confirm it has detected the mis-match*/
	if(0 == DOC.DOCR.BIT.DOPCF)
	{
		/*DOC hasn't detected a mismatch when it should have.*/
		bOK = FALSE;
	}
	
	return bOK;
}

/***********************************************************************************
Function Name : RamTest_March_HW_Is_Init
Description   :	Checks if RamTest_March_HW_Init has been called.
				This function was thought important as if RamTest_March_HW_Init
				has not been called before performing a test then errors will
				not be detected!
				
				NOTE: This does not use a flag (variable) set in
				RamTest_March_HW_Init so that these functions can be
				used before RAM has been initialised (INITSCT).
Parameters    : 				
Return Value  : TRUE: DOC module is enabled therefore assume RamTest_March_HW_Init
					  has been called: FALSE: DOC not enabled.
***********************************************************************************/
bool_t RamTest_March_HW_Is_Init(void)
{
	bool_t bReturn;
	
	/*See if DOC is enabled.
	This is the important thing to check so as not to get a false positive.
	If DMAC is not initialised then the tests will fail anyway.*/
	if(0 == SYSTEM.MSTPCRB.BIT.MSTPB6)
	{
		bReturn = TRUE;
	}
	else
	{
		bReturn = FALSE;
	}
	
	return bReturn;
}

/***********************************************************************************
Function Name : RamTest_March_HW_DMAC_Multi_Transfer
Description   :	Call this function to perform a DMAC transfer once the DMAC
				has already been set up with source, destination, width etc.
				This then performs (possibly multiple) DMAC transfers
				until all data has been transfered.
				 
Parameters    : ui32_NumBlocks: Number of transfers to perform.
				
Return Value  : void
***********************************************************************************/
#ifdef RAMTEST_USE_DMAC
void RamTest_March_HW_DMAC_Multi_Transfer(uint32_t ui32_NumBlocks)
{
	#define MAX_DMAC_TRANSFER_SIZE 0xFFFFu
	uint32_t ui32_NumBlocks_Split = MAX_DMAC_TRANSFER_SIZE;
	
	/*Check if this can be done with a single transfer.*/
	if(ui32_NumBlocks < MAX_DMAC_TRANSFER_SIZE)
	{
		ui32_NumBlocks_Split = ui32_NumBlocks; 
	}
	
	while(0 != ui32_NumBlocks)
	{
		/*Count*/
		DMAC_CHANNEL.DMCRA = ui32_NumBlocks_Split;

		/*Enable*/
		DMAC_CHANNEL.DMCNT.BIT.DTE = 1;

		/*Wait for DMAC to finish*/
		RamTest_March_HW_Wait_DMAC();
		
		/*Update the count of blocks left to transfer*/
		ui32_NumBlocks -= ui32_NumBlocks_Split;
		
		/*Calculate next transfer size*/
		if((ui32_NumBlocks < MAX_DMAC_TRANSFER_SIZE) &&
		   (0 != ui32_NumBlocks))
		{
			/*Final transfer required:*/
			ui32_NumBlocks_Split = ui32_NumBlocks;
		}	
	}
}
#endif

/***********************************************************************************
Function Name : RamTest_March_HW_Wait_DMAC
Description   :	Wait for the DMAC channel to complete a transfer.
				
				NOTE: In theory a user could add some code into this blocking loop.
				However, as this is called during RAM testing, they would need to
				be very careful not to use any RAM that is involved in the
				current RAM test.

Parameters    : 				
Return Value  : 
***********************************************************************************/
#ifdef RAMTEST_USE_DMAC
	void RamTest_March_HW_Wait_DMAC(void)
	{
		/*Wait for completion*/
		while(1 == DMAC_CHANNEL.DMCNT.BIT.DTE)
		{
		}
	}
#endif
