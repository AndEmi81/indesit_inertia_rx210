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
* File Name : CRC.h
* Version : 1.00
* Device : RX210
* Tool Chain :
* H/W Platform : 
* Description : Functions for calculating CRC for a range or ranges of memory.
*
*	Calculates the CRC of a specified memory area or memory areas.
*	NOTE:
*	This produces same CRC-CCITT result as Renesas RX Toolchain does
*	using -crc option.  (Initial Value = 0xFFFF, Final XOR with 0xFFFF.)
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
03-Feb-2011 VDE (Version 1.0) (RX62T)
15-Nov-2011 VDE (Version 1.1) (RX210)
***********************************************************************************/

#ifndef CRC_H_INCLUDE
#define CRC_H_INCLUDE

#include "misratypes.h"

void CRC_Init(void);

uint16_t CRC_Calculate(const uint8_t* pui8_Data,
					  uint32_t ui32_Length);
			
void CRC_Start(void);					  
void CRC_AddRange(const uint8_t* pui8_Data,
					  uint32_t ui32_Length);
uint16_t CRC_Result(void);

#endif
