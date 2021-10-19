/****************************************************************************
 *   $Id:: IEC60335_B_defs.h 7460 2011-06-01 11:53:27Z gerritdewaard        $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for in the IEC60335 Class B library definitions.
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

#ifndef IEC60335_DEFS_H_
#define IEC60335_DEFS_H_

#if defined ( __CC_ARM__) /*------------------RealView Compiler -----------------*/
testState  EQU 0
testPassed EQU 4

#elif (defined (__ICCARM__)) /*------------------ ICC Compiler -------------------*/
testState  EQU 0
testPassed EQU 4

#elif (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
  .equ  testState       , 0
  .equ  testPassed      , 4

#endif

#endif /* IEC60335_DEFS_H_ */
