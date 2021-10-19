/****************************************************************************
 *   $Id:: IEC60335_B_ProgramCounterTest.h 7460 2011-06-01 11:53:27Z gerrit#$
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for the Program Counter test in the IEC60335 Class B library.
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

#ifndef __cb_pc_
#define __cb_pc_

#include "IEC60335.h"

/*!
 * @brief IEC60335 Program counter test (POST)
 *        Main function call for the PC POST test
 */
type_testResult IEC60335_B_PCTest_POST(void);

/*!
 * @brief IEC60335 Program counter test (BIST)
 *        Main function call for the PC BIST test
 */
type_testResult IEC60335_B_PCTest_BIST(void);

#endif
