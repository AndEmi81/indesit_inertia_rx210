/**************************************************************************************
 DISCLAIMER	: We (Renesas Electronics Europe Limited) do NOT warrant that the Software
			  is free from claims by a third party of copyright, patent, trademark,
			  trade secret or any other intellectual property infringement.

			  Under no circumstances are we liable for any of the following:

			  1. third-party claims against you for losses or damages;
			  2. loss of, or damage to, your records or data; or
			  3. economic consequential damages (including lost profits or savings)
			  	 or incidental damages, even if we are informed of their possibility.

			  We do not warrant uninterrupted or error free operation of the Software.
			  We have no obligation to provide service, defect, correction, or any
			  maintenance for the Software. We have no obligation to supply any
			  Software updates or enhancements to you even if such are or later become
			  available.

			  IF YOU DOWNLOAD OR USE THIS SOFTWARE YOU AGREE TO THESE TERMS.

			  THERE ARE NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE IMPLIED
			  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

FILE NAME  	: CRC_Verify.c

DESCRIPTION	: Verify that a newly calculated CRC value is correct by comparing
				it with a value stored in ROM.
***********************************************************************************/

/***********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
27-May-2009 RTE-PJI First Release
28-May-2009	VDE (Version 1.0)
03-August-2009	RTE Added comment - MISRA warning explanation. (Version 1.1)
26-November-2009 RTE Corrected comment in Function header. (Version 1.2)	
03-Feb-2011 VDE (Version 1.2)
***********************************************************************************/

/**********************************************************************************
User Includes
***********************************************************************************/
#include "misratypes.h"
#include "crc_verify.h"


/***********************************************************************************
Function Name : CRC_Verify

Description   :	Test if a CRC value is correct by comparing
				it with a value stored in ROM at a specifed address.

Parameters    : ui16_NewCRCValue: 	CRC value that has just been calculated.
				ui32_AddrRefCRC: 	Address where 16 bit reference CRC value
									is stored.

Return value  : TRUE = Test Passed. FALSE = Test Failed.
***********************************************************************************/
bool_t CRC_Verify(const uint16_t ui16_NewCRCValue, const uint32_t ui32_AddrRefCRC)
{
	bool_t bPassed;
	
	/*Read reference CRC from specified address*/
	/*NOTE: This causes MISRA message 0306 -
	"The size of integer required to hold the result of a pointer cast is implementation-defined."
	This is recognised as unavoidable for certain embedded applications.*/
	uint16_t ui16_RefCRCValue = *(uint16_t*)ui32_AddrRefCRC;
	
	/*Compare with the passed in CRC value*/
	if(ui16_NewCRCValue == ui16_RefCRCValue)
	{
		/*Test Passed*/
		bPassed = TRUE;
	}
	else
	{
		/*Test Failed*/
		bPassed = FALSE;
	}
	
	/*Return the test result*/
	return bPassed;	
}
