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
* File Name : Voltage.h
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : 
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
01-Sep-2011 RTE-PJI First Release
***********************************************************************************/

#ifndef VOLTAGE_H_INCLUDE
#define VOLTAGE_H_INCLUDE

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/
typedef enum VOLTAGE_MONITOR_LEVEL
{
	VOLTAGE_MONITOR_LEVEL_4_15 = 0, /*4.15V*/
	VOLTAGE_MONITOR_LEVEL_4_00,
	VOLTAGE_MONITOR_LEVEL_3_85,
	VOLTAGE_MONITOR_LEVEL_3_70,
	VOLTAGE_MONITOR_LEVEL_3_55,
	VOLTAGE_MONITOR_LEVEL_3_40,
	VOLTAGE_MONITOR_LEVEL_3_25,
	VOLTAGE_MONITOR_LEVEL_3_10,
	VOLTAGE_MONITOR_LEVEL_2_95,
	VOLTAGE_MONITOR_LEVEL_2_80,
	VOLTAGE_MONITOR_LEVEL_2_65,
	VOLTAGE_MONITOR_LEVEL_2_50,
	VOLTAGE_MONITOR_LEVEL_2_35,
	VOLTAGE_MONITOR_LEVEL_2_20,
	VOLTAGE_MONITOR_LEVEL_2_05,
	VOLTAGE_MONITOR_LEVEL_1_90
}VOLTAGE_MONITOR_LEVEL;

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
void VoltageMonitor_Init(VOLTAGE_MONITOR_LEVEL eVoltage);

#endif
