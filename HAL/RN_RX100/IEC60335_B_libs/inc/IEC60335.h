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

#define  IEC60335_testFailed 0
#define  IEC60335_testPassed 1 
typedef int type_testResult;

#define PATTERN	(0x55555555)

typedef signed char			INT8;
typedef unsigned char		UINT8;

typedef signed short		INT16;
typedef unsigned short		UINT16;

typedef signed int			INT32;
typedef unsigned int		UINT32;
typedef unsigned int		ADDR_TYPE;

typedef signed long long	INT64;
typedef unsigned long long	UINT64;

typedef	float				FLOAT32;
typedef	double				FLOAT64_;

typedef unsigned char		BOOL;

#define FALSE				0
#define TRUE				1

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CUT(a,b,c)			(MAX(a,MIN(b,c)))
#define ifIsInRange(a,b,c)		((CUT(a,b,c) == b)?TRUE:FALSE)

#define isOdd_up(a,b)			((a & 0x01) ? (b=a):(b=a+4))
#define isEven_up(a,b)			((a & 0x01) ? (b=a+4):(b=a))
#define isOdd_down(a,b)			((a & 0x01) ? (b=a):(b=a-4))
#define isEven_down(a,b)		((a & 0x01) ? (b=a-4):(b=a))





/*	End of user configuration */
/* --- CPU TEST ------------------------------------------------------*/
/*!
 * @brief IEC60335 CPU register test POST (Pre-Operation Self Test)
 *          Main function call
 */
extern void _CPUregTestPOST (void);
extern unsigned int CPUregTestPOSTStatus;

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Main function call
 */
void IEC60335_CPUregTest_BIST(void);
/* --- RAM TEST -------------------------------------------------------*/
/*!
 * @brief IEC60335 RAM test
 *        Function call for RAM testing on the passed memory range
 */
extern type_testResult IEC60335_RAMtest (uint32_t startAddrs, uint32_t length);

/*!
 * @brief IEC60335 RAM test (BIST)
 *        Main function call for the BIST RAM testing (stack check too)
 */
extern type_testResult IEC60335_RAMtest_BIST (uint32_t startAddrs, uint32_t length);
bool_t IEC60335_RamMarchC (const uint32_t startAddrs, const uint32_t length, bool_t isBIST);
bool_t IEC60335_RamMarchC_UserStack (const uint32_t startAddrs, const uint32_t length, bool_t isBIST);
bool_t IEC60335_RamMarchC_IntStack (const uint32_t startAddrs, const uint32_t length, bool_t isBIST);
bool_t IEC60335_RamMarchC_SafeRam (void);

/*!
 * @brief IEC60335 RAM test (POST)
 *        Main function call for the POST RAM testing
 */
extern void _RAMTestPOST(void);
/* --- WDT TEST -------------------------------------------------------*/
/*!
 * @brief IEC60335 WD Test
 */
type_testResult IEC60335_WDTest(void);
/* --- CLOCK TEST -----------------------------------------------------*/
void IEC60335_initClockTestHW(void);
type_testResult IEC60335_Clocktest_MainLoopHandler(void);
/* --- ... TEST -------------------------------------------------------*/

extern type_testResult IEC60335_ADCtest_POST(void);
extern void IEC60335_ADCtest_BITS_start(void);
extern type_testResult IEC60335_ADCtest_BITS_check(void);

#endif
