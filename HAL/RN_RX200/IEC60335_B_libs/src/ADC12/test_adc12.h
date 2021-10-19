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
* File Name : Test_ADC12.h
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

#ifndef TEST_ADC12_H_INCLUDE
#define TEST_ADC12_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/

/*Error Function pointer */
typedef void(*ADC12_ERROR_CALL_BACK)(void);

/***********************************************************************************
 Defines
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/
void Test_ADC12_Init(void);
bool_t Test_ADC12_Wait(void);
void Test_ADC12_Start(const ADC12_ERROR_CALL_BACK Callback);
bool_t Test_ADC12_CheckResult(const bool_t bCallErrorHandler);

#endif
