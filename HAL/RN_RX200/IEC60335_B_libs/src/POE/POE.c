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
* File Name : POE.c
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* H/W Platform : 
* Description : Sample of usage of the Port Ouput Enable (POE) module.
*				See POE_Init for details.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
30-July-2011 RTE-PJI First Release
15-May-2012 RTE-PJI Ported to GCC: Removed #pragma interrupt.
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "RN_RX200_reg.h"
#include "POE.h"


/**********************************************************************
Defines
**********************************************************************/
/*Interrupt priority of POE*/
#define POE_INTERRUPT_PRIORITY 15

/**********************************************************************
Variables
**********************************************************************/
/*Callbacks*/
static POE_CALL_BACK g_POE_CallBack = NULL;

/********************************************************************
Prototypes
**********************************************************************/

/**********************************************************************
Functions
**********************************************************************/

/***********************************************************************************
Function Name : POE_Init

Description   :	Configures the POE to put the the following
				pins in the high impedance state if a falling edge on
				the POE0 (PD_7 pin) input pin is detected.
				An interrupt is also generated.
				Pins: MTIOC3B, MTIOC3D, MTIOC4A, MTIOC4B, MTIOC4C, and MTIOC4D
				
				Configures the POE to put the the following
				pins in the high impedance state if
				Oscillation Stop is detected.
				Pins: MTIOC0A, MTIOC0B, MTIOC0C, MTIOC0D,
				MTIOC3B, MTIOC3D, MTIOC4A, MTIOC4B, MTIOC4C, and MTIOC4D.
				
Parameters    : Callback from interrupt. Set NULL if callback is not required.
Return value  : 
***********************************************************************************/
void POE_Init(POE_CALL_BACK Callback)
{
	/*Store callback*/
	g_POE_CallBack = Callback;
	
	POE_ClearFlags();
	
	/*Select PD_7 pin as POE0 pin*/
	/*Remove protection of register.*/
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PD7PFS.BIT.PSEL = 0x07;
	PORTD.PMR.BIT.B7 = 1;
	/*Restore protection of register.*/
	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
	
	
	/* Select the pins that can be put in the high impendace state */
	POE.POECR1.BIT.PE0ZE = 1; /*MTIOC0A*/
	POE.POECR1.BIT.PE1ZE = 1; /*MTIOC0B*/
	POE.POECR1.BIT.PE2ZE = 1; /*MTIOC0C*/
	POE.POECR1.BIT.PE3ZE = 1; /*MTIOC0D*/
	POE.POECR2.BIT.P1CZEA = 1; /*MTU2A Port 1*/
	POE.POECR2.BIT.P2CZEA = 1; /*MTU2A Port 2*/
	POE.POECR2.BIT.P3CZEA = 1; /*MTU2A Port 3*/
	
	/*Oscillation stop detection enable*/
	POE.ICSR3.BIT.OSTSTE = 1;
	
	/*Enable the POE interrupt on the ICR if the callback is not NULL*/
	if(NULL != g_POE_CallBack)
	{
		/*Enable Interrupt on detection of high impedance request.*/
		POE.ICSR1.BIT.PIE1 = 1;
	
		/*Make sure IR flag is clear*/
		ICU.IR[170].BIT.IR = 0;
		/*ICU Interrupt priority*/
		ICU.IPR[170].BIT.IPR = POE_INTERRUPT_PRIORITY;	
		/*Enable the interrupt with the ICU*/
		ICU.IER[0x15].BIT.IEN2 = 1;
	}
	else
	{
		/*Disable Interrupt on detection of high impedance request.*/
		POE.ICSR1.BIT.PIE1 = 0;
	}
	
}

/***********************************************************************************
Function Name : POE_ClearFlags

Description   :	Clear all high impedance request flags.
				This will release the pins from the high impedance state.

Parameters    : 
Return value  : 
***********************************************************************************/
void POE_ClearFlags(void)
{
	/*Ensure all POE flags are clear.
	To clear the flags read and then write zero*/
	if((POE.ICSR1.WORD & 0xF000) != 0)
	{
		POE.ICSR1.WORD &= (uint16_t)~0xF000u;
	}
	
	if(POE.ICSR3.BIT.OSTSTF != 0)
	{
		POE.ICSR3.BIT.OSTSTF = 1;
	}
}

/***********************************************************************************
Function Name : Interrupt_POE2_OEI1

Description   :	Interrupt handler for POE.

				TODO User: Ensure this function is called when a POE2_OEI1
				interrupt is generated.

				Call the registered callback function.
				NOTE: It is the users responsibility to ensure a valid function
				has been registered with the POE_Init function.
Parameters    :
Return value  : 
***********************************************************************************/
void Interrupt_POE2_OEI1(void)
{
	/*Call registered callback.*/
	g_POE_CallBack();
}

