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
* File Name : IWDT.h
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

#ifndef IWDT_H_INCLUDE
#define IWDT_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/
/*Time-Out Period selection*/
typedef enum IWDT_TOP
{
	IWDT_TOP_1024 = 0,
	IWDT_TOP_4096 = 1,
	IWDT_TOP_8192 = 2,
	IWDT_TOP_16384 = 3
}IWDT_TOP;

/*Clock selection. (OCOCLK/x) */
typedef enum IWDT_CKS_DIV
{
	IWDT_CKS_DIV_1 = 0,
	IWDT_CKS_DIV_16 = 4,
	IWDT_CKS_DIV_32 = 5,
	IWDT_CKS_DIV_64 = 6,
	IWDT_CKS_DIV_128 = 7,
	IWDT_CKS_DIV_256 = 8
}IWDT_CKS_DIV;

/*Windowing: The percentages here are based on:
100% = count starting,
0% = count timeout.*/

/*Window start Position*/
typedef enum IWDT_WINDOW_START
{
	IWDT_WINDOW_START_25 = 0,
	IWDT_WINDOW_START_50 = 1,
	IWDT_WINDOW_START_75 = 2,
	IWDT_WINDOW_START_NO_START = 3 /*100%*/
}IWDT_WINDOW_START;

/*Window end Position*/
typedef enum IWDT_WINDOW_END
{
	IWDT_WINDOW_END_75 = 0,
	IWDT_WINDOW_END_50 = 1,
	IWDT_WINDOW_END_25 = 2,
	IWDT_WINDOW_END_NO_END = 3 /*0%*/
}IWDT_WINDOW_END;

/*Action when underflow or refresh error */
typedef enum IWDT_ACTION
{
	IWDT_ACTION_RESET,
	IWDT_ACTION_NMI	
}IWDT_ACTION;

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
void IWDT_Init(IWDT_TOP TimeOutperiod, IWDT_CKS_DIV ClockSelection,
				IWDT_WINDOW_START WindowStart, IWDT_WINDOW_END WindowEnd,
				IWDT_ACTION Action);
void IWDT_Kick(void);
bool_t IWDT_DidReset(void);

void IWDT_SleepMode_CountStop_Disable(void);

#endif
