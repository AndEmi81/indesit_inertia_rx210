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
* File Name : Voltage.c
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : Self Test code.
*               Monitors voltage level.
*               Generate an NMI Interrupt if voltage drops below
*				a specified voltage level.
*				User must supply NMI handler.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
01-Sep-2011 RTE-PJI First Release
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "RN_RX100_reg.h"
#include "voltage.h"

/**********************************************************************
defines
**********************************************************************/

/**********************************************************************
Variables
**********************************************************************/

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : VoltageMonitor_Init

Description   :	Start monitoring the VCC voltage level.

				If low voltage is detected a Non-Maskable Interrupt (NMI)
				will be generated that the user must handle.
				
				This uses the LVD1 circuit.
				Note that there is also LVD0 and LVD2 circuits avaialable.
				If an NMI is not the desired behaviour, the HW can generate
				a reset or maskable interrupt instead.
				The HW also supports a digital filter which this sample does not use.		
				
Parameters    : eVoltage: The volatge level that if VCC drops below will cause
							a low voltage detection.
Return value  : 
***********************************************************************************/
void VoltageMonitor_Init(VOLTAGE_MONITOR_LEVEL eVoltage)
{
	volatile uint32_t delay;
	
	/*Disable PRC3 write protection of registers*/
	SYSTEM.PRCR.WORD = 0xA508;
	
	/*Select voltage level*/
	SYSTEM.LVDLVLR.BIT.LVD1LVL = eVoltage;


	/*Internal reference voltage*/
	SYSTEM.LVCMPCR.BIT.EXVREFINP1 = 0;
	
	/*Monitor Vcc */
	SYSTEM.LVCMPCR.BIT.EXVCCINP1 = 0;

	/*Interrupt if Vcc < Vdet1 */
	SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 1;
	
	/*Select interrupt not reset*/
	SYSTEM.LVD1CR0.BIT.LVD1RI = 0;
	
	/*Select NMI interrupt not maskable*/
	SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 0;
	
	/*Enable Interrupt  (or Reset)*/
	SYSTEM.LVD1CR0.BIT.LVD1RIE = 1;
	
	/*Enable LVD1*/
	SYSTEM.LVCMPCR.BIT.LVD1E = 1;
	
	/*Wait at least td(E-A) = MAX 100uS*/
	delay = 1000; /*At 50MHz this will generate a long enough delay.*/
	while(0 != delay){delay--;}
	
	/*Enable output of the results */
	SYSTEM.LVD1CR0.BIT.LVD1CMPE = 1;
	
	/*Enable write protection of registers*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Enable Interrupt (NMI) on ICU*/
	ICU.NMIER.BIT.LVD1EN = 1;
}
