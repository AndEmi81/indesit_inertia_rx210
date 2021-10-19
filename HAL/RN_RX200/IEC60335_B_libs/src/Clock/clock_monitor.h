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
* File Name : Clock_Monitor.h
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* H/W Platform : RSK RX210
* Description : Self Test code.
*               Configure the Clock Frequency Acuracy Measurement Circuit (CAC) to
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
15-May-2012 RTE-PJI Ported to GCC: Added prototype of function to be called when
									when interrupt occurs.
***********************************************************************************/

#ifndef CLOCKMONITOR_H_INCLUDE
#define CLOCKMONITOR_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
#defines
***********************************************************************************/
/*If this (CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK) is defined then
the clock monitor will use an external reference clock.
If it is not defined the reference clock will be the on chip IWDTCLK clock.*/
/*#define CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK*/

/***********************************************************************************
 Typedefs
***********************************************************************************/
typedef void(*CLOCK_MONITOR_ERROR_CALL_BACK)(void);

#ifdef CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
	typedef enum CLOCK_MONITOR_CACREF_PIN
	{
		eCLOCK_MONITOR_CACREF_PIN_PA0,
		eCLOCK_MONITOR_CACREF_PIN_PC7,
		eCLOCK_MONITOR_CACREF_PIN_PH0
	}CLOCK_MONITOR_CACREF_PIN;
#endif

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
/*Interrupt_CAC_OVFF must be called from the handler for CAC_OVFF */
void Interrupt_CAC_OVFF(void);
/*Interrupt_CAC_FERRF must be called from the handler for CAC_FERRF */
void Interrupt_CAC_FERRF(void);

#ifdef CLOCK_MONITOR_USE_EXTERNAL_REFERENCE_CLOCK
	void ClockMonitor_Init(uint32_t MainClockFrequency,
						uint32_t ExternalRefClockFrequency,
						CLOCK_MONITOR_CACREF_PIN ePin,
						CLOCK_MONITOR_ERROR_CALL_BACK CallBack);
#else
	void ClockMonitor_Init(CLOCK_MONITOR_ERROR_CALL_BACK CallBack);
#endif

#endif
