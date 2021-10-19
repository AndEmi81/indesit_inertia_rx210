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
* File Name : Test_ADC12.c
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : Self Test code.
*               Test ADC12 using ADC12 module diagnostic mode.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
12-September-2011 REE-PJI First Release
03-Feb-2011 VDE (Version 1.0) (RX62T)
15-Nov-2011 VDE (Version 1.1) (RX210)
***********************************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "Config.h"
#include "RN_RX200_reg.h"
#include "Test_ADC12.h"


/**********************************************************************
Defines
**********************************************************************/
#define ADC12_INTERRUPT_PRIORITY 0x05

/*BIT values for DIAGVAL (Voltage Select) and DIAGST (Status)*/
#define DIAG_NONE	0
#define DIAG_ZERO	1
#define DIAG_HALF	2
#define DIAG_ONE	3

/*Expected results*/
#define VALUE_12_BIT_FULL_SCALE 0xFFF
#define VALUE_12_BIT_HALF_SCALE 0x800
#define VALUE_12_BIT_ZERO_SCALE 0x000

/*The actual result is allowed to be within
a +/- tolerence (ADC12_TOLERANCE) of the expected.
Note: This is initially set as the maximum possible absoulute inaccuracy.
A calibrated system will be able to tighten this tolerence.*/

/** IND: HAL_RN_RX200_ADC12_TOLERANCE may be used to override default
 * tolerance
 */

#if defined(HAL_RN_RX200_ADC12_TOLERANCE)
#define ADC12_TOLERANCE HAL_RN_RX200_ADC12_TOLERANCE
#else
#define ADC12_TOLERANCE 8
#endif

/** IND: ADC12_ENABLE_FAIL_DIAGNOSTICS may be Enabled it to add diagnostic feature:
 * ADC_MaxDelta		contain max delta read at each test (0 - Zero, 1 - Half, 2 - Full)
 * ADC_LastOccur	contain last occurrence time (in 20ms units)
 */
/* #define ADC12_ENABLE_FAIL_DIAGNOSTICS */

/**********************************************************************
Variables
**********************************************************************/
/*Error Callbacks*/
static ADC12_ERROR_CALL_BACK g_ADC12_ErrorCallBack = NULL;

#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
/* Diagnostic variables */
uint16_t ADC_MaxDelta[3];
uint16_t ADC_LastOccur;
uint16_t ADC_LastOccur8ADC;
static uint16_t dbgCntr[5];
#endif

/********************************************************************
Prototypes
**********************************************************************/

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : Test_ADC12_Init

Description   :	Initialise the ADC12 module.

Parameters    : 
Return value  : 
***********************************************************************************/
void Test_ADC12_Init(void)
{
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable the ADC12 Module*/
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
}

/***********************************************************************************
Function Name : Test_ADC12_Wait

Description   :	This function waits while two ADC conversions are made. (0V and VREF)
				This test does not preserve ADC configuration and is therefore
				suitable as a power on test rather than as a periodic test.
				
Parameters    :
				 

Return value  : TRUE = test passed. FALSE = test failed.
***********************************************************************************/
bool_t Test_ADC12_Wait(void)
{	
	bool_t bResult = TRUE;
	uint16_t Value;
	
	/*Set Right Alignment for Data Register*/
	S12AD.ADCER.BIT.ADRFMT = 0;
	
	/*Set single mode*/
	S12AD.ADCSR.BIT.ADCS = 0;
	
	/*Set DIAGLD so DIAGVAL setting is used*/
	S12AD.ADCER.BIT.DIAGLD = 1;
	/*Enable Diagnoistic mode*/
	S12AD.ADCER.BIT.DIAGM = 1;
	
	
	/*** Select test type = 0V ***/
	S12AD.ADCER.BIT.DIAGVAL = DIAG_ZERO;
	
	/*Start conversion */
	S12AD.ADCSR.BIT.ADST = 1;
	
	/*Wait for conversion to finish */
	while(1 == S12AD.ADCSR.BIT.ADST){;}
		
	/*Check result = expected*/
	Value = S12AD.ADRD.BIT.RIGHT.AD;
	if(Value > (VALUE_12_BIT_ZERO_SCALE + ADC12_TOLERANCE))
	{
		bResult = FALSE;
	}
	else
	{
		/*Test passed so far.*/
		/*** Select test type = VREF*1 ***/
		S12AD.ADCER.BIT.DIAGVAL = DIAG_ONE;
	
		/*Start conversion */
		S12AD.ADCSR.BIT.ADST = 1;
	
		/*Wait for conversion to finish */
		while(1 == S12AD.ADCSR.BIT.ADST){;}
		
		/*Check result = expected*/
		Value = S12AD.ADRD.BIT.RIGHT.AD;
		if(Value <= (VALUE_12_BIT_FULL_SCALE - ADC12_TOLERANCE))
		{
			bResult = FALSE;
		}
	
		/*Disable Diagnostic mode*/
		S12AD.ADCER.BIT.DIAGM = 1;
	}
	
	return bResult;
}

/***********************************************************************************
Function Name : Test_ADC12_Start

Description   :	Setup the ADC module so diagnostic test will be performed each time
				ADC is used for reading analogue input pins.
				The Reference voltage is automatically rotated,
				0V, VREF/2, VREF.

				User code must now call the Test_ADC12_CheckResult function
				either periodically or following every ADC completion to
				check the diagnostic result.
				
Parameters    : Callback: A function to call if an error is detected. 
Return value  :
***********************************************************************************/
void Test_ADC12_Start(const ADC12_ERROR_CALL_BACK Callback)
{	
	/*Store error callback*/
	g_ADC12_ErrorCallBack = Callback;
	
	/*Set DIAGLD so voltages are manually rotated*/
	S12AD.ADCER.BIT.DIAGLD = 1;
	
	/*Enable Diagnoistic mode*/
	S12AD.ADCER.BIT.DIAGM = 1;
}

/***********************************************************************************
Function Name : Test_ADC12_CheckResult

Description   :	Check the ADC diagnostic result is as expected.
				This must be called after Test_ADC12_Start.
				It must be called periodically or whenever an ADC conversion completes.
		
Parameters    :  bCallErrorHandler: Set True if want error handler to be called
				 					if an error is detected.
									Note it is the users responsibility to ensure
									the error callback has been set correctly using
									Test_ADC12_Start function.
									
Return value  : TRUE = test passed. FALSE = test failed.
***********************************************************************************/
bool_t Test_ADC12_CheckResult(const bool_t bCallErrorHandler)
{
	uint8_t Status;
	uint16_t Value;
	uint16_t Expected_Max;
	uint16_t Expected_Min;
#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
	uint16_t tmpDelta, tmpExpected;
#endif
	bool_t bResult;
	
	/*The Data Register Diag (ADRD) can be right or left aligned*/
	if(1 == S12AD.ADCER.BIT.ADRFMT)
	{
		/*Left aligned*/
		
		/*Diagnostic Status*/
		Status = (uint8_t)S12AD.ADRD.BIT.LEFT.DIAGST;
		/*Data value*/
		Value = S12AD.ADRD.BIT.LEFT.AD;
	}
	else
	{
		/*Right aligned*/
		
		/*Diagnostic Status*/
		Status = (uint8_t)S12AD.ADRD.BIT.RIGHT.DIAGST;
		/*Data value*/
		Value = S12AD.ADRD.BIT.RIGHT.AD;
	}
	
	
	/*Expected result depends upon test voltage used.
	Calculate an allowable range based on the given TOLERANCE.*/
	switch(Status)
	{
		case DIAG_ZERO:
#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
			tmpExpected = VALUE_12_BIT_ZERO_SCALE;
#endif
			Expected_Min = VALUE_12_BIT_ZERO_SCALE;
			Expected_Max = VALUE_12_BIT_ZERO_SCALE + ADC12_TOLERANCE;
			break;
		case DIAG_HALF:
#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
			tmpExpected = VALUE_12_BIT_HALF_SCALE;
#endif
			Expected_Min = VALUE_12_BIT_HALF_SCALE - ADC12_TOLERANCE;
			Expected_Max = VALUE_12_BIT_HALF_SCALE + ADC12_TOLERANCE;
			break;
		case DIAG_ONE:
#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
			tmpExpected = VALUE_12_BIT_FULL_SCALE;
#endif
			Expected_Min = VALUE_12_BIT_FULL_SCALE - ADC12_TOLERANCE;
			Expected_Max = VALUE_12_BIT_FULL_SCALE;
			break;
		case DIAG_NONE:
		default:
			/*No test run yet - nothing to check*/
			return TRUE;
	}

#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
	if (tmpExpected > Value)
	{
		tmpDelta = tmpExpected - Value;
	}
	else
	{
		tmpDelta = Value - tmpExpected;
	}


	if ((tmpDelta > ADC_MaxDelta[Status-1]) || (dbgCntr[Status-1] == 0))
	{
		ADC_MaxDelta[Status-1] = tmpDelta;
		dbgCntr[Status-1] = 100;
	}
	else if (dbgCntr[Status-1] > 0)
	{
		dbgCntr[Status-1]--;
	}
#endif

	/*Check value is within expected range*/
	if((Value > Expected_Max) || (Value < Expected_Min))
	{
		/*Test Failed*/
		bResult = FALSE;
		
		/*Call error callback?*/
		if(TRUE == bCallErrorHandler)
		{
			g_ADC12_ErrorCallBack();
		}
	}
	else
	{
		/*Test Passed*/
		bResult = TRUE;
	}	

#if defined(ADC12_ENABLE_FAIL_DIAGNOSTICS)
	if (bResult == FALSE)
	{
		ADC_LastOccur = dbgCntr[3];
		dbgCntr[3] = 0;
	}
	else
	{
		if (dbgCntr[3] != 0xFFFF)
		{
			dbgCntr[3]++;
		}
	}

	if (tmpDelta > 8)
	{
		ADC_LastOccur8ADC = dbgCntr[4];
		dbgCntr[4] = 0;
	}
	else
	{
		if (dbgCntr[4] != 0xFFFF)
		{
			dbgCntr[4]++;
		}
	}

#endif
	
	/* Set new diag test */
	if (Status >= DIAG_ONE)
	{
		Status = 1;
	}
	else
	{
		Status++;
	}

	S12AD.ADCER.BIT.DIAGVAL = Status;

	return bResult;
}
