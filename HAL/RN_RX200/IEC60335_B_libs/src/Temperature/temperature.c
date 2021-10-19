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
* File Name : Temperature.c
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : Self Test code.
*               Use the ADC12 module to read the MCU temperature.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
30-July-2011 RTE-PJI First Release
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "RN_RX200_reg.h"
#include "Temperature.h"


/**********************************************************************
Defines
**********************************************************************/

/**********************************************************************
Variables
**********************************************************************/
static uint16_t g_Temperature_ADC_Value_Min;
static uint16_t g_Temperature_ADC_Value_Max;
/*Error Callbacks*/
static TEMPERATURE_ERROR_CALL_BACK g_Temp_ErrorCallBack = NULL;

/********************************************************************
Prototypes
**********************************************************************/

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : Temperature_Init

Description   :	Initialise the Temperature sensor and
				enable the ADC12 module.
				
				*** NOTE ***
				After calling this the Temperature_Start function must
				not be called until the temperature sensor reference voltage
				becomes stable. (tSTBL). A delay is not added here as
				it is thought the user can call this function and then schedule
				the Temperature_Start to be called later.

Parameters    : eGain: The temperature sensor gain.
				Temperature_ADC_Value_Min: Min. allowable value of ADTSDR register.
				Temperature_ADC_Value_Max: Max. allowable value of ADTSDR register. 
				Error_callback: Function to be called if temperature (ADTSDR) falls out
				 				of specified range.

				NOTE: The ADC output is 12bit so do not specify a ADC_Value greater
				than h'FFF.

Return value  : void
***********************************************************************************/
void Temperature_Init(TEMPERATURE_GAIN_SELECT eGain,
					  uint16_t Temperature_ADC_Value_Min,
					  uint16_t Temperature_ADC_Value_Max,
					  TEMPERATURE_ERROR_CALL_BACK Error_callback)
{
	/*Store temperature allowed limits*/
	g_Temperature_ADC_Value_Min = Temperature_ADC_Value_Min;
	g_Temperature_ADC_Value_Max = Temperature_ADC_Value_Max;
	
	/*Store callback*/
	g_Temp_ErrorCallBack = Error_callback;
	
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable the Temperature Sensor Module*/
	SYSTEM.MSTPCRB.BIT.MSTPB8 = 0;
	/*Enable the ADC12 Module*/
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Ensure PGA is disabled*/
	TEMPS.TSCR.BIT.PGAEN = 0;
	
	/*Set Gain*/
	TEMPS.TSCR.BIT.PGAGAIN = eGain;
	
	/*Enable Sensor. NOTE A conversion will not start until PGAEN is set.*/
	TEMPS.TSCR.BIT.TSEN = 1;
}

/***********************************************************************************
Function Name : Temperature_Start

Description   :	Configure ADC12 to read the Temperature sensor and
				start a conversion.
				
				NOTE: The user must ensure the ADC12 has been stopped
				before using this. The ADC can not perform conversion of analogue
				pins and temperature sensor output at the same time.
				
				This function will destroy the settings in the following ADC
				registers: ADEXICR,ADCSR and ADSTRGR.
				It is the users responsibility when resuming normal
				operation to ensure these are re-configured as required.
				
				NOTE: To avoid this function altering the ADC interrupts
				it is left for the user to ascertain when the temperature
				sensor conversion has completed so that normal ADC
				operation can be resumed.
				(Function Temperature_Read_Wait can be used for this purpose.)
				
				After calling this function use Temperture_Read_Wait
				to get the result or Temperature_CheckResult to
				verify if the result is within the pre-defined limits.
				
Parameters    :
Return value  : 
***********************************************************************************/
void Temperature_Start(void)
{
	/*Output of temperature sensor is target for AD Conversion*/
	S12AD.ADEXICR.BIT.TSS = 1;
	/*Single scan mode*/
	S12AD.ADCSR.BIT.ADCS = 0;
	/*Set ADC trigger as Temperature Sensor*/
	S12AD.ADSTRGR.BIT.TRSA = 0xA;
	S12AD.ADCSR.BIT.TRGE = 1;
	S12AD.ADCSR.BIT.EXTRG = 0;
	
	/*Start conversion*/
	TEMPS.TSCR.BIT.PGAEN = 1;	
}

/***********************************************************************************
Function Name : Temperature_Is_Finished

Description   :	Determines if a temperature conversion
				(started with Temperature_Start) has completed.
				
Parameters    :
Return value  : TRUE = Finished. FALSE = Busy
***********************************************************************************/
bool_t Temperature_Is_Finished(void)
{
	if(1 == TEMPS.TSCR.BIT.PGAEN)
	{
		/*Busy*/
		return FALSE;
	}
	else
	{
		/*Finished*/
		return TRUE;
	}
}

/***********************************************************************************
Function Name : Temperature_Wait_Finish

Description   :	Function blocks until a temperature conversion
				(started with Temperature_Start) has completed.
				
Parameters    :
Return value  : 
***********************************************************************************/
void Temperature_Wait_Finish(void)
{
	/*If the temperature sensor conversion is still in operation
	then wait for it to finish. */
	while(FALSE == Temperature_Is_Finished()){;}
}

/***********************************************************************************
Function Name : Temperature_Read_Wait

Description   :	Call this following a call to Temperature_Start.
				Read the result of an ADC conversion, waiting for it to
				finish if required.
				
Parameters    :
Return value  : The read 12-bit ADC value.
***********************************************************************************/
uint16_t Temperature_Read_Wait(void)
{
	uint16_t Value;
	
	Temperature_Wait_Finish();
	
	Value = S12AD.ADTSDR;
	
	/*The Temperature Sensor Data Register can be right or left aligned*/
	if(1 == S12AD.ADCER.BIT.ADRFMT)
	{
		/*Left aligned*/
		Value >>= 4;	
	}
	
	return Value;	
}

/***********************************************************************************
Function Name : Temperature_CheckResult

Description   :	Call this following a call to Temperature_Start.
				Read the result of an ADC conversion (waiting for it to
				 finish if required).
				Check if the result is within the limits supplied to the
				Temperature_Init function.
				
Parameters    : bCallErrorHandler: Set true to get the callback registered in
				Temperature_Init called if the temperature falls outside the
				specified limits.
				
Return value  : TRUE: Result falls within specified limits.
				FALSE: Result falls outside specified limits.
***********************************************************************************/
bool_t Temperature_CheckResult(bool_t bCallErrorHandler)
{
	bool_t bResult = TRUE;
	uint16_t Value;
	
	Value = Temperature_Read_Wait();
	
	if((Value < g_Temperature_ADC_Value_Min) ||
		 (Value > g_Temperature_ADC_Value_Max))
	{
		bResult = FALSE;
		
		/*Temperature is out of range.*/
		if(TRUE == bCallErrorHandler)
		{
			g_Temp_ErrorCallBack();
		}
	}
	
	return bResult;
}
