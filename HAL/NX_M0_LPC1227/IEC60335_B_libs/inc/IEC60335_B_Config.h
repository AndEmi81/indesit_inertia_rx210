/****************************************************************************
 *   $Id:: IEC60335_B_Config.h 7460 2011-06-01 11:53:27Z gerritdewaard      $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Configurations of the IEC60335 Class B library.
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
#ifndef __IEC60335_USER_CFG_H
#define __IEC60335_USER_CFG_H

/*****************************************************************************/
/* common defines for flash sizes                                            */
/*****************************************************************************/
#define SIZE1K	    0x000003FF
#define SIZE2K	    0x000007FF
#define SIZE4K	    0x00000FFF
#define SIZE8K	    0x00001FFF
#define SIZE16K	    0x00003FFF
#define SIZE24K	    0x00005FFF
#define SIZE32K	    0x00007FFF
#define SIZE64K	    0x0000FFFF
#define SIZE128K    0x0001FFFF
#define SIZE256K    0x0003FFFF
#define SIZE512K    0x0007FFFF

/*****************************************************************************/
/* definitions for supported CRC signature types                             */
/*****************************************************************************/
#define CCITT_ALGO    1
#define CRC16_ALGO    2
#define CRC32_ALGO    3
#define MISRHW_ALGO   4

/*****************************************************************************/
/* the following macro defines the pattern to be used for the BIST ram test  */
/*****************************************************************************/
#define PATTERN	(0x55555555U)
  
/*****************************************************************************/
/* the following macros are used to define the addresses for the blocks used */
/* for flash crc signature check. These can be redefined within the specific */
/* target header file in case these are different on other derivate targets  */
/*****************************************************************************/
/*------------------RealView Compiler -----------------*/
/*------------------ ICC Compiler -------------------*/
#if (defined(__CC_ARM__) || defined(__ICCARM__) || defined(__IASMARM__))

    /* Flash signature generation engine on LPC11x */
	#define FMSSTART_ADDR  TARGET_FMSSTART   EQU 0x4003C020
	#define FMSSTOP_ADDR   TARGET_FMSSTOP    EQU 0x4003C024
	#define FMSW0_ADDR     TARGET_FMSW0      EQU 0x4003C02C
	#define FMSW1_ADDR     TARGET_FMSW1      EQU 0x4003C030
	#define FMSW2_ADDR     TARGET_FMSW2      EQU 0x4003C034
	#define FMSW3_ADDR     TARGET_FMSW3      EQU 0x4003C038
	#define FMSTAT_ADDR    TARGET_FMSTAT     EQU 0x4003CFE0
	#define FMSTATCLR_ADDR TARGET_FMSTATCLR  EQU 0x4003CFE8

	/* Crc generation engine on LPC12x */
	#define CRC_MODE_ADDR     TARGET_CRC_MODE_ADDR EQU 0x50070000
	#define CRC_SEED_ADDR     TARGET_CRC_SEED_ADDR EQU 0x50070004
	#define CRC_SUM_ADDR      TARGET_CRC_SUM_ADDR  EQU 0x50070008
	#define CRC_WR_DATA_ADDR  TARGET_WD_DATA_ADDR  EQU 0x50070008

#elif (defined(__GNUC__)) /*------------------ GNU Compiler ---------------------*/

    /* Flash signature generation engine on LPC11x */
	#define FMSSTART_ADDR  .equ TARGET_FMSSTART   , 0x4003C020
	#define FMSSTOP_ADDR   .equ TARGET_FMSSTOP    , 0x4003C024
	#define FMSW0_ADDR     .equ TARGET_FMSW0      , 0x4003C02C
	#define FMSW1_ADDR     .equ TARGET_FMSW1      , 0x4003C030
	#define FMSW2_ADDR     .equ TARGET_FMSW2      , 0x4003C034
	#define FMSW3_ADDR     .equ TARGET_FMSW3      , 0x4003C038
	#define FMSTAT_ADDR    .equ TARGET_FMSTAT     , 0x4003CFE0
	#define FMSTATCLR_ADDR .equ TARGET_FMSTATCLR  , 0x4003CFE8

	/* Crc generation engine on LPC12x */
	#define CRC_MODE_ADDR     .equ TARGET_CRC_MODE_ADDR , 0x50070000
	#define CRC_SEED_ADDR     .equ TARGET_CRC_SEED_ADDR , 0x50070004
	#define CRC_SUM_ADDR      .equ TARGET_CRC_SUM_ADDR  , 0x50070008
	#define CRC_WR_DATA_ADDR  .equ TARGET_WD_DATA_ADDR  , 0x50070008
  
#endif
	  
/*****************************************************************************/
/* User target configuration                                                 */
/*****************************************************************************/
/* the following includes are target dependant and provide the necessary     */
/* system level definitions. They need to be modified by the end user to     */
/* specify the target device. Default build rules for LPC1227 and LPC114     */
/* are provided in the project setup                                         */
                      
#ifdef LPC1114
	#include "LPC1114_TargetConfig.h"
#endif
#ifdef LPC1227
	#include "LPC1227_TargetConfig.h"
#endif

/*****************************************************************************/
/* End of user target configuration                                          */
/*****************************************************************************/

/*****************************************************************************/
/* sanity checks for user chosen configuration                               */
/*****************************************************************************/
#ifdef LPC1227
  #if (SELECTED_CRC_TYPE == MISRHW_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif

  #if ((SELECTED_CRC_TYPE != CCITT_ALGO) && (SELECTED_CRC_TYPE != CRC16_ALGO) && (SELECTED_CRC_TYPE != CRC32_ALGO))
    #error "Invalid CRC calculation type selected"
  #endif
#endif

#ifdef LPC1114
  #if (SELECTED_CRC_TYPE == CCITT_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE == CRC16_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE == CRC32_ALGO)
    #error "CRC calculation type non supported on target hardware"
  #endif
  #if (SELECTED_CRC_TYPE != MISRHW_ALGO)
    #error "Invalid CRC calculation type selected"
  #endif
#endif
#endif

