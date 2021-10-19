/****************************************************************************
 *   $Id:: IEC60335.h 7460 2011-06-01 11:53:27Z gerritdewaard               $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Main include file for the IEC60335 Class B library.
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

#ifndef __IEC60335_
#define __IEC60335_

typedef enum tag_testResult
{
	IEC60335_testFailed = 0,
	IEC60335_testPassed = 1
} type_testResult;

#include "IEC60335_typedefs.h"
#include "IEC60335_B_Config.h"
#include "IEC60335_B_SecureDataStorage.h"
#include "IEC60335_B_Interrupts.h"
#include "IEC60335_B_ProgramCounterTest.h"
#include "IEC60335_B_RAMTestBIST.h"
#include "IEC60335_B_FLASHTest.h"
#include "IEC60335_B_ClockTest.h"
#include "IEC60335_B_CPUregTests.h"

/*	End of user configuration */
#endif
