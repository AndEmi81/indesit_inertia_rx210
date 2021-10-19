/****************************************************************************
 *   $Id:: IEC60335_B_CPUregTests.c 7461 2011-06-01 13:01:58Z gerritdewaard $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B CPU register testing 
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

#include <stdint.h>
#include "IEC60335.h"
IEC60335_CPUreg_struct CPUregTestBIST_struct;
unsigned int CPUregTestPOSTStatus;
/***************************************************************************//**
** @brief 		BIST CPU register function call
** @param[in]	NONE
** @return		type_testResult IEC60335 Pass/Fail return
*****************************************************************************/
type_testResult IEC60335_CPUregTest_BIST (void)
{

	/* Clear the current test states */
	CPUregTestBIST_struct.testState = 0;
	CPUregTestBIST_struct.testPassed = 0;

	/* Test the low registers r0-r7 */
	 _CPUregTestLOW(); 
         if (CPUregTestBIST_struct.testPassed != IEC60335_testPassed)
            return (IEC60335_testFailed); 

	/* Clear the PASS bit */
	CPUregTestBIST_struct.testPassed = 0;

	/* Test the mid registers r4-r10 */
	_CPUregTestMID();
	if (CPUregTestBIST_struct.testPassed != IEC60335_testPassed) 
          return (IEC60335_testFailed);

	/* Clear the PASS bit */
	CPUregTestBIST_struct.testPassed = 0;

	/* Test the high registers r8-r12 */
	_CPUregTestHIGH();
        if (CPUregTestBIST_struct.testPassed != IEC60335_testPassed) 
          return (IEC60335_testFailed);

	/* Clear the PASS bit */
	CPUregTestBIST_struct.testPassed = 0;

	/* Test the stack pointer registers
	 * NOTE: DISABLES ALL INTERRUPTS!
	 */
	_CPUregTestSP();
        if (CPUregTestBIST_struct.testPassed != IEC60335_testPassed) 
          return (IEC60335_testFailed);

	/* Clear the PASS bit */
	CPUregTestBIST_struct.testPassed = 0;

	/* Test the Special registers */
	_CPUregTestSPEC();
        if (CPUregTestBIST_struct.testPassed != IEC60335_testPassed) 
          return (IEC60335_testFailed);

	/* Indicate all tests passed */
	CPUregTestBIST_struct.testPassed = IEC60335_testPassed;

	return (IEC60335_testPassed);
}


