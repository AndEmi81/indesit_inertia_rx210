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
* Tool Chain : GNU Rx
* H/W Platform : 
* Description : Self Test code.
*               Test ADC12 using ADC12 module diagnostic mode.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
15-May-2012 RTE-PJI Ported to GCC: Added prototype of function to be called when
									when interrupt occurs.
***********************************************************************************/

#ifndef POE_H_INCLUDE
#define POE_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/

/*Error Function pointer */
typedef void(*POE_CALL_BACK)(void);


/***********************************************************************************
 Defines
***********************************************************************************/


/***********************************************************************************
 Function Prototypes
***********************************************************************************/
/*Interrupt_POE2_OEI1 must be called from the handler for POE2_OEI1 */
void Interrupt_POE2_OEI1(void);

void POE_Init(POE_CALL_BACK Callback);
void POE_ClearFlags(void);

#endif
