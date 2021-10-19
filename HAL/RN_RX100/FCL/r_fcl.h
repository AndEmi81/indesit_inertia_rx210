#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl.h $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.8 $
* Mod. Date     : $Date: 2013/06/25 11:00:17MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : Header file containing function prototype
**********************************************************************************************************************/

/**********************************************************************************************************************
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  intended for use with Renesas products. 
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED 
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
* NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS 
* AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY 
* REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH 
* DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2012-2013 Renesas Electronics Corporation. All rights reserved.     
**********************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_ON 0292
#endif

/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
* Reason:       To support automatic insertion of revision, module name etc. by the source revision control system 
*               it is necessary to violate the rule, because the system uses non basic characters as placeholders.
* Verification: The placeholders are used in commentars only. Therefore rule violation cannot influency code 
*               compilation.
**********************************************************************************************************************/

#ifndef R_FCL_H
#define R_FCL_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Global compiler definition */
#define FCL_LIB_COMP_GCC            (1u)
#define FCL_LIB_COMP_IAR            (2u)
#define FCL_LIB_COMP_REC            (3u)

#if (defined __GNUC__)
    #define FCL_LIB_COMPILER     FCL_LIB_COMP_GCC
    #define GCC_SECTION(s)       __attribute__((section(s)))
#elif (defined __IAR_SYSTEMS_ASM__) || (defined __IAR_SYSTEMS_ICC__)
    #define FCL_LIB_COMPILER     FCL_LIB_COMP_IAR
    #define GCC_SECTION(s)
#elif (defined __RX200)
    #define FCL_LIB_COMPILER     FCL_LIB_COMP_REC
    #define GCC_SECTION(s)
#else
    #error "Unknown compiler"
#endif

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
**********************************************************************************************************************/

/**********************************************************************************************************************
Exported global functions (to be accessed by other files) 
**********************************************************************************************************************/
/**********************************************************************************************************************
 * Initialization 
 *********************************************************************************************************************/
/* FCL initialization function */
void               R_FCL_Init               ( void ) /* GCC_SECTION("R_FCL_CODE_ROM") */;
/* Calculate RAM address of copied function */ 
r_fcl_status_t     R_FCL_CalcFctAddr        ( void const *   fct_addr_ptr,
                                              const uint32_t dest_addr_u32,
                                              uint32_t *     ram_addr_pu32 )  /*  GCC_SECTION("R_FCL_CODE_ROM") */;
/* FCB frequency set */
r_fcl_status_t     R_FCL_SetFrequency       ( const uint8_t cpu_freq_MHz_u08 ) GCC_SECTION(".PRAM");



/**********************************************************************************************************************
 * Normal operations 
 *********************************************************************************************************************/
/* Erase blocks */
r_fcl_status_t     R_FCL_Erase              ( const uint32_t block_no_start_u32,
                                              const uint32_t block_no_end_u32 ) GCC_SECTION(".PRAM");
/* Write flash content to consecutive flash addresses */
r_fcl_status_t     R_FCL_Write              ( uint32_t *     src_addr_pu32,
                                              const uint32_t dest_addr_u32,
                                              const uint32_t length_u32 )       GCC_SECTION(".PRAM");
/* Blankcheck address range */
r_fcl_status_t     R_FCL_BlankCheck         ( const uint32_t start_addr_u32,
                                              const uint32_t length_u32 )       GCC_SECTION(".PRAM");
/* Read words from code flash */
r_fcl_status_t     R_FCL_Read               ( const uint32_t start_addr_u32,
                                              uint32_t *     dest_addr_pu32,
                                              const uint32_t length_u32 )       GCC_SECTION(".PRAM");
/* Current status of an operation */
#if (FCL_STATUS_CHECK == FCL_STATUS_CHECK_USER)
    r_fcl_status_t R_FCL_StatusCheck        ( void )                            GCC_SECTION(".PRAM");
#endif



/**********************************************************************************************************************
 * Administration operations
 *********************************************************************************************************************/
/* Return of library version string */
const uint8_t *  R_FCL_GetVersionString   ( void ) /* GCC_SECTION("R_FCL_CODE_ROM") */;
/* Return device number of blocks */
uint32_t         R_FCL_GetBlockCnt        ( void ) GCC_SECTION(".PRAM");
/* Return block start address */
r_fcl_status_t   R_FCL_GetBlockStartAddr  ( const uint32_t   block_no_u32,
                                            uint32_t * const block_start_addr_pu32 ) GCC_SECTION(".PRAM");
/* Get current start-up program swap status */
r_fcl_status_t   R_FCL_GetSwapState       ( r_fcl_sps_status_t * const sps_stat_enu ) 
                                          GCC_SECTION(".PRAM");
/* Temporary change of start-up programs */
r_fcl_status_t   R_FCL_ChangeSwapState    ( void ) GCC_SECTION(".PRAM");
/* Get current value of the start-up program swap flag */
r_fcl_status_t   R_FCL_GetSwapFlag        ( r_fcl_sps_flag_status_t * const sps_flag_status_penu ) 
                                          GCC_SECTION(".PRAM");
/* Change start-up program swap flag */
r_fcl_status_t   R_FCL_ChangeSwapFlag     ( const uint8_t immediate_sp_swap_u08 ) GCC_SECTION(".PRAM");
/* Get current start-up program area size */
uint8_t          R_FCL_GetStartupAreaSize ( void ) GCC_SECTION(".PRAM");



/**********************************************************************************************************************
 * Security
 *********************************************************************************************************************/
/* Get the Access window range */
r_fcl_status_t   R_FCL_GetAW              ( uint32_t * const block_no_start_pu32,
                                            uint32_t * const block_no_end_pu32 ) GCC_SECTION(".PRAM");
/* Set access window new range */
r_fcl_status_t   R_FCL_SetAW              ( const uint32_t block_no_start_u32,
                                            const uint32_t block_no_end_u32 ) GCC_SECTION(".PRAM");

#endif /* ifndef R_FCL_H */
