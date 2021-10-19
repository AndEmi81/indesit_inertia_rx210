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
* File Name : Temperature.h
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

#ifndef TEST_TEMPERATURE_H_INCLUDE
#define TEST_TEMPERATURE_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/

/*Error Function pointer */
typedef void(*TEMPERATURE_ERROR_CALL_BACK)(void);

/*Select gain applicable for your system based on the A/D reference voltage(AVCC0)
and required resolution.
This will be used to set the TSCR.PGAGAIN register.*/
typedef enum TEMPERATURE_GAIN_SELECT
{
	TEMPERATURE_GAIN_SELECT_AVCC0_1_8_VOLTS = 0, /* 1.8V < AVCC0 < 2.7V (Lowest Gain)*/
	TEMPERATURE_GAIN_SELECT_AVCC0_2_7_VOLTS, 	 /* 2.7V < AVCC0 < 3.6V */
	TEMPERATURE_GAIN_SELECT_AVCC0_3_6_VOLTS, 	 /* 3.6V < AVCC0 < 4.5V */
	TEMPERATURE_GAIN_SELECT_AVCC0_4_5_VOLTS  	 /* 4.5V < AVCC0 < 5.5V (Highest Gain)*/
}TEMPERATURE_GAIN_SELECT;

/***********************************************************************************
 Defines
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/
void Temperature_Init(TEMPERATURE_GAIN_SELECT eGain,
					  uint16_t Temperature_ADC_Value_Min,
					  uint16_t Temperature_ADC_Value_Max,
					  TEMPERATURE_ERROR_CALL_BACK Error_callback);

void Temperature_Start(void);
bool_t Temperature_Is_Finished(void);
void Temperature_Wait_Finish(void);
uint16_t Temperature_Read_Wait(void);
/*NOTE: Temperature_CheckResult will wait if required for the ADC conversion to finish/ */
bool_t Temperature_CheckResult(bool_t bCallErrorHandler);

#endif
