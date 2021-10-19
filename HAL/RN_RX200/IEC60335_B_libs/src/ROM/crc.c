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
* File Name : CRC.c
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

/**********************************************************************
Includes
**********************************************************************/
#include "RN_RX200_reg.h"
#include "CRC.h"


/**********************************************************************
Functions
**********************************************************************/

/***********************************************************************************
Function Name : CRC_Init
Description   : Initialise the CRC module for CRC-CCITT
				This function must be used before any other function in this module.
				
Parameters    :
Return value  : 
***********************************************************************************/
void CRC_Init(void)
{
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable CRC module*/
	SYSTEM.MSTPCRB.BIT.MSTPB23 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
	
	/*Configure CRC Control Register:
	1.Set Polynomial to CRC-CCITT XModem (h'1021)
		(p(x) = x ** 16 + x ** 12 + x ** 5 + 1)
	2.Set MSB First. (This makes result agree with online CRC calculators.)
	3.Clear CRCDOR register*/	
	CRC.CRCCR.BYTE = 0x87;
}

/***********************************************************************************
Function Name : CRC_Calculate
Description   : Calculate CRC on supplied data. (Single address range)
				
Parameters    : pui8_Data: Start address of dta
			    ui32_Length : Number of bytes of data.
				
Return value  : Calculated CRC
***********************************************************************************/
uint16_t CRC_Calculate(const uint8_t* pui8_Data,
					   uint32_t ui32_Length)
{
	CRC_Start();
	
	CRC_AddRange(pui8_Data, ui32_Length);
	
	return CRC_Result();
}

/***********************************************************************************
Function Name : CRC_Start
Description   : Call this function before using the CRC_AddRange function to
				add the first address range.
				Re-sets the CRC module to a start condition.
				
Parameters    :
Return value  : 
***********************************************************************************/
void CRC_Start(void)
{
	/*Clear any previous result.*/
	CRC.CRCCR.BIT.DORCLR = 1;
	
	/*Initial value */
	CRC.CRCDOR = 0xFFFF;
}

/***********************************************************************************
Function Name : CRC_AddRange
Description   : Use this function rather than CRC_Calculate if wanting
				to calculate the CRC on data made up of more than one address range.
				Call CRC_Start first then CRC_AddRange for each address range
				required and then call CRC_Result to get the CRC value.
				
Parameters    : pui8_Data: Start address of dta
			    ui32_Length : Number of bytes of data.
Return value  : 
***********************************************************************************/					  
void CRC_AddRange(const uint8_t* pui8_Data,
					  uint32_t ui32_Length)
{
	/*Write the data a byte at a time to the CRC Data Input register.*/
	while(0 != ui32_Length)
	{	
		CRC.CRCDIR = *pui8_Data;
		/*Onto the next byte*/
		pui8_Data++;
		ui32_Length--;
	}
}

/***********************************************************************************
Function Name : CRC_Result
Description   : use this function to get a CRC result after adding supplying data
				using CRC_AddRange function.			
Parameters    :
Return value  : CRC value
***********************************************************************************/
uint16_t CRC_Result(void)
{
	/*Return the CRC result.
	This is as read from CRC Data Ouput register and bit inversed*/
	return (uint16_t)~CRC.CRCDOR;
}
