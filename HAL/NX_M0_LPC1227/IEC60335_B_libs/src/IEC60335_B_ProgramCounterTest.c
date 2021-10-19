/****************************************************************************
 *   $Id:: IEC60335_B_ProgramCounterTest.c 7461 2011-06-01 13:01:58Z gerrit#$
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B Program counter testing 
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

#include "IEC60335_B_ProgramCounterTest.h"

#define RET_FCT_A	1
#define RET_FCT_B	2
#define RET_FCT_C	3
#define RET_FCT_D	5
#define RET_FCT_E	7
#define RET_FCT_F	11

/** Last tested function storage */
UINT32 IEC60335_B_PCTest_lastFctTested = RET_FCT_F;

/***************************************************************************//**
** @brief 		Function A, first PC test function. 
**				To be placed at section ".IEC60335_B_behindVectors"
** @param[in]	NONE
** @return		RET_FCT_A 	Function A fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_behindVectors"
#else
__attribute__((section(".IEC60335_B_behindVectors")))
#endif
UINT32 IEC60335_B_PCTest_FctA(void)
{
	return RET_FCT_A;
}

/***************************************************************************//**
** @brief 		Function B, first PC test function. 
**				To be placed at section ".IEC60335_B_beforeText"
** @param[in]	NONE
** @return		RET_FCT_B 	Function B fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_beforeText"
#else
__attribute__((section(".IEC60335_B_beforeText")))
#endif
UINT32 IEC60335_B_PCTest_FctB(void)
{
	return RET_FCT_B;
}

/***************************************************************************//**
** @brief 		Function C, first PC test function. 
**				To be placed at section ".IEC60335_B_behindText"
** @param[in]	NONE
** @return		RET_FCT_C 	Function C fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_behindText"
#else
__attribute__((section(".IEC60335_B_behindText")))
#endif
UINT32 IEC60335_B_PCTest_FctC(void)
{
	return RET_FCT_C;
}

/***************************************************************************//**
** @brief 		Function D, first PC test function. 
**				To be placed at section ".IEC60335_B_lastFashSection"
** @param[in]	NONE
** @return		RET_FCT_D 	Function D fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_lastFashSection"
#else
__attribute__((section(".IEC60335_B_lastFashSection")))
#endif
UINT32 IEC60335_B_PCTest_FctD(void)	
{
	return RET_FCT_D;
}

/***************************************************************************//**
** @brief 		Function E, first PC test function. 
**				To be placed at section ".IEC60335_B_beforeRAMcode"
** @param[in]	NONE
** @return		RET_FCT_E 	Function E fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_beforeRAMcode"
#else
__attribute__((section(".IEC60335_B_beforeRAMcode"),long_call))
#endif
UINT32 IEC60335_B_PCTest_FctE(void)
{
	return RET_FCT_E;
}

/***************************************************************************//**
** @brief 		Function F, first PC test function. 
**				To be placed at section ".IEC60335_B_behindRAMcode"
** @param[in]	NONE
** @return		RET_FCT_F 	Function F fixed return value
*****************************************************************************/
#if 	(defined (__ICC_ARM__)) /*------------------ ICC Compiler -------------------*/
#pragma location = ".IEC60335_B_behindRAMcode"
#else
__attribute__((section(".IEC60335_B_behindRAMcode"),long_call))
#endif
UINT32 IEC60335_B_PCTest_FctF(void)
{
	return RET_FCT_F;
}

/***************************************************************************//**
** @brief 		Function call the test functions throughout the program and data 
**				memory and check the return value against the expected value.
** @param[in]	NONE
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_B_PCTest_POST(void)
{
	type_testResult Result = IEC60335_testFailed;

	if(IEC60335_B_PCTest_FctA() == RET_FCT_A)
		Result = IEC60335_testPassed;
		
	if((Result == IEC60335_testPassed) && (IEC60335_B_PCTest_FctB() != RET_FCT_B))
		Result = IEC60335_testFailed;

	if((Result == IEC60335_testPassed) && (IEC60335_B_PCTest_FctC() != RET_FCT_C))
		Result = IEC60335_testFailed;

	if((Result == IEC60335_testPassed) && (IEC60335_B_PCTest_FctD() != RET_FCT_D))
		Result = IEC60335_testFailed;

	if((Result == IEC60335_testPassed) && (IEC60335_B_PCTest_FctE() != RET_FCT_E))
		Result = IEC60335_testFailed;

	if((Result == IEC60335_testPassed) && (IEC60335_B_PCTest_FctF() != RET_FCT_F))
		Result = IEC60335_testFailed;

	return Result;
}

/***************************************************************************//**
** @brief 		 executes at every call one PC test, saves the current executed test and returns a PASS/FAIL. 
** @param[in]	NONE
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_B_PCTest_BIST(void)
{
	type_testResult Result = IEC60335_testFailed;

	switch(IEC60335_B_PCTest_lastFctTested)
	{
		case RET_FCT_A:
			if(IEC60335_B_PCTest_FctB() == RET_FCT_B)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_B;
			break;

		case RET_FCT_B:
			if(IEC60335_B_PCTest_FctC() == RET_FCT_C)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_C;
			break;

		case RET_FCT_C:
			if(IEC60335_B_PCTest_FctD() == RET_FCT_D)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_D;
			break;

		case RET_FCT_D:
			if(IEC60335_B_PCTest_FctE() == RET_FCT_E)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_E;
			break;

		case RET_FCT_E:
			if(IEC60335_B_PCTest_FctF() == RET_FCT_F)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_F;
			break;

		case RET_FCT_F:
		default:
			if(IEC60335_B_PCTest_FctA() == RET_FCT_A)
				Result = IEC60335_testPassed;
			
			IEC60335_B_PCTest_lastFctTested = RET_FCT_A;
			break;

	}
	return Result;
}
