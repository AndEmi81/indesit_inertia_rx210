/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name : IWDT.c
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : Independent Watchdog.
*				Used to detect abnormal programme operation.
*               Resets system if not kicked (refreshed) often enough.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include "RN_RX100_reg.h"
#include "IWDT.h"

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : IWDT_Init

Description   :	Configure the IWDT and Start it runnning.
				The Clock,Timeout Period and Window position should be set
				so that your application can manage to call the IWDT_Kick at
				the correct time so that the IWDT counter does not underflow.
				However it is preferable to make the window/period as short
				as it can be so that any code runaway is detected quickly.
				
				NOTE: This function can only be used once after a reset as the
				IWDTCR register can only be written to once

Parameters    : TimeOutperiod - See IWDT_TOP declaration for details.
			  :	ClockSelection - See IWDT_CKS_DIV declaration for details. 
			  : WindowStart - See IWDT_WINDOW_START declaration for details.
			  : WindowEnd - See IWDT_WINDOW_END declaration for details.	
Return value  : 
***********************************************************************************/
void IWDT_Init(IWDT_TOP TimeOutperiod,
				IWDT_CKS_DIV ClockSelection,
				IWDT_WINDOW_START WindowStart,
				IWDT_WINDOW_END WindowEnd,
				IWDT_ACTION Action)
{
	uint16_t iwdtcr_value;
	
	/*Disable PRC0 write protection of registers*/
	SYSTEM.PRCR.WORD = 0xA501;
	
	/* Enable the IWDT dedicated clock (IWDTLOCO / IWDTCLK) */
	SYSTEM.ILOCOCR.BIT.ILCSTP = 0;
	
	/*Enable PRC0 and PRC1 write protection of registers*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	iwdtcr_value = 0x0000u;
	iwdtcr_value |= (uint16_t)TimeOutperiod;
	iwdtcr_value |= (uint16_t)(ClockSelection << 4);
	iwdtcr_value |= (uint16_t)(WindowEnd << 8);
	iwdtcr_value |= (uint16_t)(WindowStart << 12);
	IWDT.IWDTCR.WORD = iwdtcr_value;
	
	/*Set NMI or Reset action*/
	if(Action == IWDT_ACTION_RESET)
	{
		/*Reset*/
		IWDT.IWDTRCR.BIT.RSTIRQS = 1;
	}
	else
	{
		/*NMI*/
		/*USER must handle the NMI interrupt.*/
		IWDT.IWDTRCR.BIT.RSTIRQS = 0;
		
		/*Enable the NMI for IWDT*/
		ICU.NMIER.BIT.IWDTEN = 1;
	}
	
	/*Start the count going by doing initial refresh*/
	IWDT_Kick();
}

/***********************************************************************************
Function Name : IWDT_Kick

Description   :	Kick (refesh) the watch dog count
				Must get called before count overflows.
				NOTE: The first time this is called it starts the counter.
Parameters    : 
Return value  :  
***********************************************************************************/
void IWDT_Kick(void)
{
	/*To refresh the count:
	1. Write h'00
	2. Write h'FF*/
	IWDT.IWDTRR = 0x00;
	IWDT.IWDTRR = 0xFF;
}

/***********************************************************************************
Function Name : IWDT_SleepMode_CountStop

Description   :	Count stop control.
				From reset count stop is enabled, meaning the IWDT counter
				is stopped during sleep mode.
				Call this to disable count stop.
				Once disabled it can not be re-enabled as IWDTCSTPR register
				can only be written to once.

Parameters    :
Return value  :  
***********************************************************************************/
void IWDT_SleepMode_CountStop_Disable(void)
{
	IWDT.IWDTCSTPR.BIT.SLCSTP = 0;
}

/***********************************************************************************
Function Name : IWDT_DidReset

Description   :	This can be used after a reset to see if the IWDT caused the reset.
				Clears IWDT status flags.

Parameters    : 
Return value  :  TRUE if watchdog has underflowed or generated a refresh error,
				 otherwise FALSE.
***********************************************************************************/
bool_t IWDT_DidReset(void)
{
	bool_t bReturn = FALSE;
	
	/*Read status register to see if underflow has occured*/
	if(1 == IWDT.IWDTSR.BIT.UNDFF)
	{
		/*Clear flag*/
		IWDT.IWDTSR.BIT.UNDFF = 0;
		bReturn = TRUE;
	}
	
	/*Read status register to see if underflow has occured*/
	if(1== IWDT.IWDTSR.BIT.REFEF)
	{
		/*Clear flag*/
		IWDT.IWDTSR.BIT.REFEF = 0;
		bReturn = TRUE;
	}
	
	return bReturn;
}
