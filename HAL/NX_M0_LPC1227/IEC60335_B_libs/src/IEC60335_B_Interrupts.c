/****************************************************************************
 *   $Id:: IEC60335_B_Interrupts.c 7461 2011-06-01 13:01:58Z gerritdewaard  $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B Interrupt testing 
 *	   library.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "IEC60335_B_Interrupts.h"

/***************************************************************************//**
** @brief 		The interrupt check has to be called in fixed equidistant times. You must estimate how many times the 
**				interrupt will occur within this known time slice.
** @param[in]	*pIRQ			Pointer to the interrupt test structure.
** @param[in]	lowerBound		Estimated minimum count of occured interrupts. (Don't take respect to the individualValue).
** @param[in]	upperBound		Estimated maximum count of occured interrupts. (Don't take respect to the individualValue).
** @param[in]	individualValue	The internally used individual up-counting value.
** @return		NONE
** @attention	Ensure to place the function call #IEC60335_InterruptOcurred into the corresponding 
**				interrupt service handler!
*****************************************************************************/
void IEC60335_InitInterruptTest(type_InterruptTest *pIRQ, UINT32 lowerBound, UINT32 upperBound, UINT32 individualValue)
{
	if(pIRQ != 0)
	{
		pIRQ->Count = 0;
	
		pIRQ->lower = lowerBound;
		pIRQ->upper = upperBound;
	
		pIRQ->individualValue = individualValue;

		pIRQ->CountOverflow = FALSE;
	}
}

/***************************************************************************//**
** @brief 		This function must be called from any interrupt service handler which has to be tested.
** @param[in]	*pIRQ			Pointer to the interrupt test structure.
** @return		NONE
*****************************************************************************/
void IEC60335_InterruptOcurred(type_InterruptTest *pIRQ)
{
	if(pIRQ != 0)
	{
		UINT32 cnt = pIRQ->Count;

		pIRQ->Count = pIRQ->Count + pIRQ->individualValue;

		if(cnt > pIRQ->Count)
			pIRQ->CountOverflow = TRUE;
	}
}

/***************************************************************************//**
** @brief 		The interrupt check routine must be called once in known equidistant times.
** @param[in]	*pIRQ			Pointer to the interrupt test structure.
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_InterruptCheck(type_InterruptTest *pIRQ)
{
	type_testResult Result = IEC60335_testFailed;

	if(pIRQ != 0)
	{
        if((pIRQ->Count < pIRQ->lower) || (pIRQ->Count > pIRQ->upper))
            Result = IEC60335_testFailed;
        else
            Result = IEC60335_testPassed;

		if(pIRQ->CountOverflow == TRUE)
		{
			pIRQ->CountOverflow = FALSE;
			Result = IEC60335_testFailed;
		}

		pIRQ->Count = 0;
	}

	return Result;
}
