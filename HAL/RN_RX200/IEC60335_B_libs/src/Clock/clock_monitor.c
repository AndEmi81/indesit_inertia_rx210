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
* File Name : Clock_Monitor.c
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* H/W Platform : 
* Description : Self Test code.
*               
*				Configure the Clock Frequency Accuracy Measurement Circuit (CAC) to
* 				monitor the Main Clock.
*				IWDTCLK or an external clock can be used as a reference.
*				
*				Oscillation stop Detection is also configured to generate
*				an NMI interrupt. User must handle the NMI interrupt.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
12-September-2011 REE-PJI First Release
15-Nov-2011 VDE (Version 1.0) (RX210)
15-May-2012 RTE-PJI Ported to GCC: Removed #pragma interrupt.
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "RN_RX200_reg.h"

#include "Clock_Monitor.h"

/**********************************************************************
defines
**********************************************************************/
/*Percentage tolerance of main clock allowed before an
error is reported.*/
/*USER: Calculate a tolerance based on the environment you are operating in.*/
#define CLOCK_TOLERANCE_PERCENT 10

/*Interrupt priority of Clock Tolerence Exceeded interrupt.*/
#define CLOCK_MONITOR_INTERRUPT_PRIORITY 15

#ifndef CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
	/*This value is calculated for the case where:
	Reference = IWDTCLK / 1024 = 125KHZ / 1024.
	Clock Count = Main / 32  = 20MHZ / 32*/
	#define CLOCK_COUNT_EXPECTED 0x2800
#endif

/**********************************************************************
Variables
**********************************************************************/
/*User registered callback function*/
static CLOCK_MONITOR_ERROR_CALL_BACK g_CallBackClock = NULL;

/**********************************************************************
Prototypes
**********************************************************************/
static void OscillationStopDetection_Enable(void);

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : ClockMonitor_Init

				NOTE: There are two versions of this function:
				This is used if CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
				is not defined.

Description   :	Configure the Clock Frequency Acuracy Measurement Circuit (CAC) to
 				monitor the Main Clock using IWDTCLK.
				
				NOTE: *** The IWDTCLK must be enabled for this to work.
				If IWDTCLK is not present an error will not be detected.
				User must ensure the IWDTCLK stabalisation time (20uS)
				has elapsed berfore calling this.
				
				NOTE: This is written for the case where the Main clock
				is 20MHz. See CLOCK_COUNT_EXPECTED definition for details.
				
				The allowed range is calculated as the expected
				count +- the tolerence specifed by CLOCK_TOLERANCE_PERCENT.
				
				Oscillation Stop Detection.
				This also enables the Oscillation Stop detection NMI interrupt.
				User code must handle any resulting NMI interrupt and check
				the NMISR.OSTST flag. 
				
Parameters    : CallBack: Function to call if the main clock deviates
						  from the allowed range.

Return value  : 
***********************************************************************************/
#ifndef CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
void ClockMonitor_Init(CLOCK_MONITOR_ERROR_CALL_BACK CallBack)
{
	uint16_t tolerence;
		
	/*Save callback*/
	g_CallBackClock = CallBack;
	
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable the module*/
	SYSTEM.MSTPCRC.BIT.MSTPC19 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Clock select =  Main / 32*/
	CAC.CACR1.BYTE = 0x34;
	
	/*Reference clock - IWDTCLK / 1024 */
	CAC.CACR2.BYTE = 0x29;
	
	/*Set count Limits.*/
	tolerence = CLOCK_COUNT_EXPECTED * CLOCK_TOLERANCE_PERCENT / 100;
	CAC.CALLVR = (uint16_t)(CLOCK_COUNT_EXPECTED - tolerence);
	CAC.CAULVR = (uint16_t)(CLOCK_COUNT_EXPECTED + tolerence);
	
	/*Ensure all status flags are cleared by writing 1 to them.*/
	CAC.CASTR.BYTE = 0x07;
	
	/*Enable Interrupt for Frequency Error and Overflow*/
	CAC.CAICR.BYTE = 0x05;
	
	/*Configure Interrupt Control Unit:*/
	/*Overflow (OVFF)*/
	/*Make sure IR flag is clear*/
	ICU.IR[34].BIT.IR = 0;
	/*ICU Interrupt priority*/
	ICU.IPR[34].BIT.IPR = CLOCK_MONITOR_INTERRUPT_PRIORITY;	
	/*Enable the interrupt with the ICU*/
	ICU.IER[0x04].BIT.IEN2 = 1;
	
	/*Frequency error (FERRF)*/
	/*Make sure IR flag is clear*/
	ICU.IR[32].BIT.IR = 0;
	/*ICU Interrupt priority*/
	ICU.IPR[32].BIT.IPR = CLOCK_MONITOR_INTERRUPT_PRIORITY;	
	/*Enable the interrupt with the ICU*/
	ICU.IER[0x04].BIT.IEN0 = 1;
	
	/*Clock frequency measurement enable*/
	CAC.CACR0.BIT.CFME = 1;	
	
	/*Also enable Oscillation Stop. */
	/* OscillationStopDetection_Enable(); */
}
#endif

/***********************************************************************************
Function Name : ClockMonitor_Init

				NOTE: There are two versions of this function:
				This is used if CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
				is defined.

Description   :	Configure the Clock Frequency Acuracy Measurement Circuit (CAC) to
 				monitor the Main Clock using external CACREF clock.
				
				The allowed range is calculated as the expected
				count +- the tolerence specifed by CLOCK_TOLERANCE_PERCENT.
				
				This function calculates a suitable dividers for the
				main clock. If a suitable divider is not avaialable
				the supplied callback will be called.
				
				The specified CRCREF pin is configured by setting
				the appropriate PSEL value and by setting the PMR bit.
				
				NOTE: If no external clock is present on the specified CRCREF pin
				this will not detect an error.
				
				NOTE: A digital filter can be applied to CRCREF.
				This function does not do this.
				
				Oscillation Stop Detection.
				This also enables the Oscillation Stop detection NMI interrupt.
				User code must handle any resulting NMI interrupt and check
				the NMISR.OSTST flag. 
				
Parameters    : 	MainClockFrequency: Main Clock frequency in Hz.
					ExternalRefClockFrequency: CRCREF clock in Hz
					ePin: The pin that will be used as CRCREF.
					CallBack: Function to call if the main clock deviates
						  		from the allowed range.

Return value  : 
***********************************************************************************/
#ifdef CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK

void ClockMonitor_Init(uint32_t MainClockFrequency,
						uint32_t ExternalRefClockFrequency,
						CLOCK_MONITOR_CACREF_PIN ePin,
						CLOCK_MONITOR_ERROR_CALL_BACK CallBack)
{
	#define NUM_MAIN_DIV 4
	#define NUM_REF_DIV 4
	
	uint32_t count = 0;
	uint32_t tolerence = 0;
	uint16_t main_div[NUM_MAIN_DIV] = {1,4,8,32};
	uint8_t index_main_div;
	bool_t bFound = FALSE;
		
	/*Save callback*/
	g_CallBackClock = CallBack;
	
	/*Configure the CACREF pin*/
	/*Remove protection of register.*/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	switch(ePin)
	{
		case eCLOCK_MONITOR_CACREF_PIN_PA0:
			MPC.PA0PFS.BIT.PSEL = 0x07;
			PORTA.PMR.BIT.B0 = 1;
			break;
		case eCLOCK_MONITOR_CACREF_PIN_PC7:
			MPC.PC7PFS.BIT.PSEL = 0x07;
			PORTC.PMR.BIT.B7 = 1;
			break;
		case eCLOCK_MONITOR_CACREF_PIN_PH0:
			MPC.PH0PFS.BIT.PSEL = 0x07;
			PORTH.PMR.BIT.B0 = 1;
			break;
	};
	/*Restore protection of register.*/
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable the module*/
	SYSTEM.MSTPCRC.BIT.MSTPC19 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Calculate divider on main clock*/
	/*Start with lowest divide*/
	for(index_main_div = 0; index_main_div < NUM_MAIN_DIV; index_main_div++)
	{
		count = (uint32_t)(((double)MainClockFrequency / (double)main_div[index_main_div]) /
				((double)ExternalRefClockFrequency)-1);
		
		tolerence = (count * CLOCK_TOLERANCE_PERCENT) / 100;
		
		/*If the ref. clock is very slow so that the calculated clock
		is very small the tolerence calculation can round to zero.
		Deal with this by ensuring the tolerence is at least 1.*/
		if(tolerence == 0)
		{
			tolerence = 1;
		}
				
		if(((count + tolerence) <= 0xFFFF) && (count >= tolerence))
		{
			/*Found suitable dividers*/
			bFound = TRUE;
			break;
		}		 
	}
	
	if(FALSE == bFound)
	{
		/*Error setting up the clock monitor */
		g_CallBackClock();
	}
	else
	{
		/*Measure Main clock */
		CAC.CACR1.BIT.FMCS = 0;
		/*Divide Main*/
		CAC.CACR1.BIT.TCSS = index_main_div;
		
		/*Use CACREF pin*/
		CAC.CACR1.BIT.CACREFE = 1;
		CAC.CACR2.BIT.RPS = 0;
		/*Divide Ref (This is not applicable if using CACREF)*/
		CAC.CACR2.BIT.RCDS = 0;
		
		/*Set count limits.*/
		CAC.CALLVR = (uint16_t)(count - tolerence);
		CAC.CAULVR = (uint16_t)(count + tolerence);
	
		/*Ensure all status flags are cleared by writing 1 to them.*/
		CAC.CASTR.BYTE = 0x07;
	
		/*Enable Interrupt for Frequency Error and Overflow*/
		CAC.CAICR.BYTE = 0x05;
	
		/*Configure Interrupt Control Unit:*/
		/*Overflow (OVFF)*/
		/*Make sure IR flag is clear*/
		ICU.IR[34].BIT.IR = 0;
		/*ICU Interrupt priority*/
		ICU.IPR[34].BIT.IPR = CLOCK_MONITOR_INTERRUPT_PRIORITY;	
		/*Enable the interrupt with the ICU*/
		ICU.IER[0x04].BIT.IEN2 = 1;
	
		/*Frequency error (FERRF)*/
		/*Make sure IR flag is clear*/
		ICU.IR[32].BIT.IR = 0;
		/*ICU Interrupt priority*/
		ICU.IPR[32].BIT.IPR = CLOCK_MONITOR_INTERRUPT_PRIORITY;	
		/*Enable the interrupt with the ICU*/
		ICU.IER[0x04].BIT.IEN0 = 1;
	
		/*Clock frequency measurement enable*/
		CAC.CACR0.BIT.CFME = 1;
		
		OscillationStopDetection_Enable();
	}
}
#endif

/***********************************************************************************
Function Name : OscillationStopDetection_Enable

Description   :	Enable NMI Interrupt when Oscillation Stop is detected.
Parameters    :
Return value  : 
***********************************************************************************/
static void OscillationStopDetection_Enable(void)
{
	/***Oscillation Stop detection****/
	/*Disable register protection (PRC0)*/
	SYSTEM.PRCR.WORD = 0xA501;	
	/*Enable Stop Detection Interrupt */
	SYSTEM.OSTDCR.BIT.OSTDIE = 1;
	/*Enable Oscillation Stop detection*/
	SYSTEM.OSTDCR.BIT.OSTDE = 1;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Enable Interrupt (NMI) on ICU*/
	ICU.NMIER.BIT.OSTEN = 1;
}


/***********************************************************************************
Function Name : Interrupt_CAC_FERRF

Description   :	Interrupt handler for CAC Frequency Error.

				TODO User: Ensure this function is called when a CAC_FERRF
				interrupt is generated.

				Call the registered callback function.
				NOTE: It is the users responsibility to ensure a valid function
				has been registered with the ClockMonitor_Init function.
Parameters    :
Return value  : 
***********************************************************************************/
void Interrupt_CAC_FERRF(void)
{
	/*Call registered calback.*/
	g_CallBackClock();
	
	/*Clear flag by writing 1 to clear bit.*/
	CAC.CAICR.BIT.FERRFCL = 1;
}

/***********************************************************************************
Function Name : Interrupt_CAC_OVFF

Description   :	Interrupt handler for CAC Overflow Error

				TODO User: Ensure this function is called when a CAC_OVFF
				interrupt is generated.

				Call the registered callback function.
				NOTE: It is the users responsibility to ensure a valid function
				has been registered with the ClockMonitor_Init function.
Parameters    :
Return value  : 
***********************************************************************************/
void Interrupt_CAC_OVFF(void)
{
	/*Call registered calback.*/
	g_CallBackClock();
	
	/*Clear flag by writing 1 to clear bit.*/
	CAC.CAICR.BIT.OVFFCL = 1;
}
