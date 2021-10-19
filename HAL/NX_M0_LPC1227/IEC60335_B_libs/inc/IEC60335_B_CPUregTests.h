/****************************************************************************
 *   $Id:: IEC60335_B_CPUregTests.h 7460 2011-06-01 11:53:27Z gerritdewaard $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for the CPU register test in the IEC60335 Class B library.
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
#ifndef IEC60335_B_CPUREGS_H_
#define IEC60335_B_CPUREGS_H_

#include "IEC60335.h"

#if defined ( __CC_ARM   )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm                                       /*!< asm keyword for IAR Compiler           */
  #define __INLINE        inline                                      /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */

#elif defined   (  __GNUC__  )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */

#endif


typedef struct {
  /* base 0 */
  unsigned int testPassed;      /*!< CPU register POST test status (PASS/FAIL) */
  /* base 4 */
  unsigned int testState;       /*!< CPU register POST tests passed */
}IEC60335_CPUreg_struct;

/*!
 * @brief IEC60335 CPU register test POST (Pre-Operation Self Test)
 *          Main function call
 */
extern void _CPUregTestPOST (void);
extern unsigned int CPUregTestPOSTStatus;
/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Low-range registers tests r0-r7
 */
extern void _CPUregTestLOW (void);

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Mid-range registers tests r4-r10
 */
extern void _CPUregTestMID (void);

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Mid-range registers tests r8-r12
 */
extern void _CPUregTestHIGH (void);

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Stack pointer registers tests MSP,PSP
 */
extern void _CPUregTestSP (void);

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Other special register tests
 */
extern void _CPUregTestSPEC (void);

/*!
 * @brief IEC60335 CPU register test BIST (Build in Self Test)
 *          Main function call
 */
type_testResult IEC60335_CPUregTest_BIST(void);

#endif /* IEC60335_B_CPUREGS_H_ */
