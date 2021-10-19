/****************************************************************************
 *   $Id:: IEC60335_B_RAMTestBIST.h 7460 2011-06-01 11:53:27Z gerritdewaard $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for the RAM test in the IEC60335 Class B library.
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

#ifndef IEC60335_B_RAMTEST_H_
#define IEC60335_B_RAMTEST_H_

/*!
 * @brief IEC60335 RAM test
 *        Function call for RAM testing on the passed memory range
 */
extern type_testResult IEC60335_RAMtest (UINT32 startAddrs, UINT32 length);

/*!
 * @brief IEC60335 RAM test (BIST)
 *        Main function call for the BIST RAM testing
 */
extern type_testResult IEC60335_RAMtest_BIST (UINT32 startAddrs, UINT32 length);

/*!
 * @brief IEC60335 RAM test (POST)
 *        Main function call for the POST RAM testing
 */
extern void _RAMTestPOST(void);

#endif /* IEC60335_B_RAMTEST_H_ */
