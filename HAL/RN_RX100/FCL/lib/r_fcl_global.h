#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_global.h $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.17 $
* Mod. Date     : $Date: 2013/06/25 11:00:16MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : Library internal defines, types, function prototypes and variables
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

#ifndef R_FCL_GLOBAL_H
#define R_FCL_GLOBAL_H


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define FCL_NULL                                (0x00000000u)
#define FCL_ADDR_POINTER_UNDEFINED              (uint32_t *)(0x00000000uL)
#define FCL_FCT_RAM_POINTER_UNDEF               (fcl_fct_ptr_t)(0xFFFFFFFFuL)
/* Operation types */
#define FCL_NO_OPERATION                        (0x00u)
#define FCL_FLASH_OP                            (0xAAu)
#define FCL_EXTRA_AREA_OP                       (0xBBu)
/* Data flash access is activated by the library */
#define FCL_DATA_FLASH_ACCESS_BY_LIBRARY        (0x01u)
/* Operating modes */
#define FCL_LOW_SPEED_OP_MODE                   (0u)
#define FCL_MIDDLE_SPEED_OP_MODE                (1u)
#define FCL_HIGH_SPEED_OP_MODE                  (2u)

/* Library version string */
#ifndef PRECOMPILED
    #define FCL_VERSION_STRING                  "SX100T01xxxxxxV100"
#else
    #ifndef PRECOMPILED_LIB_VER
        #error "PRECOMPILED_LIB_VER is not defined. Aborting build."
    #endif

    /* ref for REC: http://c-faq.com/ansi/stringize.html */
    #define STRINGIZE(str)                      #str
    #define STRINGIZE_VALUE_OF(x)               STRINGIZE(x)

    #define FCL_VERSION_STRING                  STRINGIZE_VALUE_OF(PRECOMPILED_LIB_VER)
#endif

/* Static functions definition */ 
#ifndef FCL_STATIC
    /* GCC does not place static functions into sections specified by __attribute__ suffix */
    #ifndef __GNUC__
        #define FCL_STATIC                      static
    #else
        #define FCL_STATIC
    #endif
#endif

/* testing purposes only */
#ifdef FCL_TEST_PATCH
    #undef  FCL_STATIC
    #define FCL_STATIC
#endif

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/**  
   @enum r_fcl_flag_t 
   standard FCL internal flag definition 
*/
typedef enum R_FCL_FLAG_T 
{     
    R_FCL_FALSE,                            /**< Flag false */
    R_FCL_TRUE                              /**< Flag true  */
} r_fcl_flag_t; 


/**  
   @enum r_fcl_fcb_mode_t 
   Sequencer operating modes 
*/
typedef enum R_FCL_FCB_MODE_T 
{     
    R_FCL_FCB_MODE_USER,                    /**< User mode or read mode  */
    R_FCL_FCB_MODE_PE                       /**< Programing / Erase mode */
} r_fcl_fcb_mode_t; 


/**  
   @struct r_fcl_multi_op_t 
   structure containing the data required for multi operations (erase multiple blocks, 
   write/read multiple words data) 
*/ 
typedef struct R_FCL_MULTI_OP_T 
{ 
    uint32_t      srcAddr_u32;              /**< source data address      */
    uint32_t *    dstAddr_pu32;             /**< destination data pointer */
    uint32_t      srcAddrEnd_u32;           /**< last source address      */
    uint8_t       fcbCmd_u08;               /**< FCB command              */
} r_fcl_multi_op_t; 


/** 
   @enum r_fcl_int_lib_state_t 
   Library internal states 
*/ 
typedef enum R_FCL_INT_LIB_STATE_T 
{ 
    R_FCL_LIB_ISTATE_NOINIT  = 0x00u,        /**< Library not initialized (default and reset state)*/
    R_FCL_LIB_ISTATE_INIT    = 0x01u,        /**< Library initialized, further proceding possible  */
    R_FCL_LIB_ISTATE_IDLE    = 0x02u,        /**< All initialization done. Operation possible      */
    R_FCL_LIB_ISTATE_BUSY    = 0xffu         /**< Any Flash operation is ongoing                   */
} r_fcl_int_lib_state_t; 


/** 
   @struct r_fcl_internal_t 
   Library internal debug information
*/ 
typedef struct R_FCL_INTERNAL_T 
{

    r_fcl_int_lib_state_t   fclInternalState_enu;       /**< FCL operational status variable      */           
    r_fcl_multi_op_t        multiOp_str;                /**< multi operation (erase/write/read) 
                                                             variables. Set when a multi operation  
                                                             is started, checked/updated when a  
                                                             single erase/write/read is finished  */
    uint8_t                 fclCpuFreqMHz_u08;          /**< CPU Frequency provided by user       */
    uint8_t                 fclFlashIfFreqMHz_u08;      /**< Calculated Flash IF Frequency        */
    uint8_t                 fclDataFlashMemAccess_u08;  /**< Data flash is activated by library's
                                                             internal operations(Read and Extra 
                                                             area writting) or not                */
    uint8_t                 fclOpType_u08;              /**< Used to distinguish between operation 
                                                             performed on extra area or on flash  */
    rBool                   resetReg_FLPMC_Flag_b;      /**< reset register FLPMC value, needed to
                                                             set the temporary start-up program swap flags */
} r_fcl_internal_t; 

/* FCL function pointers for internal use (distinguish ROM / RAM usage) */
typedef r_fcl_status_t (*fcl_fct_ptr_t)( uint32_t start_addr_u32, 
                                         uint32_t end_addr_u32, 
                                         uint8_t  command_u08 );

#ifdef FCL_TEST_PATCH
    typedef struct TEST_PATCH_T
    {
        bool        act_flag_FSASTL;
        uint8_t     reg_FSASTL;
        bool        act_flag_dev_flash_size;
        uint8_t     flash_size;
    } test_patch_t;
#endif
/**********************************************************************************************************************
Exported global variables
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /**< fcl global variable structure */
    extern  r_fcl_internal_t  g_fclInternal_str          ;
    extern  fcl_fct_ptr_t     g_fclRomRamInterfacePtr_fp ;
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_DATA"
    extern  r_fcl_internal_t  g_fclInternal_str;
    #pragma location="R_FCL_DATA"
    extern  fcl_fct_ptr_t     g_fclRomRamInterfacePtr_fp;
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section B R_FCL_DATA
    extern  r_fcl_internal_t  g_fclInternal_str;
    extern  fcl_fct_ptr_t     g_fclRomRamInterfacePtr_fp;
#endif

#ifdef FCL_TEST_PATCH
    extern test_patch_t test_patch;
#endif
/**********************************************************************************************************************
Exported global functions (to be accessed by other files) 
**********************************************************************************************************************/
r_fcl_status_t     R_FCL_StartOperation    ( const uint32_t start_addr_u32,
                                             const uint32_t end_addr_u32,
                                             const uint8_t  command_u08 )      GCC_SECTION(".PRAM");
r_fcl_status_t     R_FCL_FCB_SwitchMode    ( const r_fcl_fcb_mode_t mode_enu ) GCC_SECTION(".PRAM");
void               R_FCL_SetFlashFrequency ( const uint8_t   cpu_freq_u08,
                                             uint8_t * const fcb_freq_pu08 )   GCC_SECTION(".PRAM");
void               R_FCL_BasicFct_CodeRamUsrInt_CalcRange ( uint32_t * sec_start_pu32,
                                                            uint32_t * sec_size_pu32 );
void               R_FCL_BasicFct_CodeRamUsr_CalcRange    ( uint32_t * sec_start_pu32,
                                                            uint32_t * sec_size_pu32 );
void               R_FCL_BasicFct_CodeRam_CalcRange       ( uint32_t * sec_start_pu32,
                                                            uint32_t * sec_size_pu32 );
void               R_FCL_BasicFct_CodeRomRam_CalcRange    ( uint32_t * sec_start_pu32,
                                                            uint32_t * sec_size_pu32 );
void               R_FCL_WaitUs                           ( const uint32_t time_u32,
                                                            const uint8_t  freq_u08 );
void               R_FCL_WriteMemoryU08                   ( const uint32_t addr_u32,
                                                            const uint8_t  val_u8 ) GCC_SECTION(".PRAM");
r_fcl_flag_t       R_FCL_FCB_CheckReady                   ( void ) GCC_SECTION(".PRAM");
r_fcl_status_t     R_FCL_FCB_GetOpStatus                  ( void ) GCC_SECTION(".PRAM");
void               R_FCL_FCB_ReadOperation                ( void ) GCC_SECTION(".PRAM");
void               R_FCL_FCB_ClearControlReg              ( void ) GCC_SECTION(".PRAM");
void               R_FCL_FCB_Reset                        ( void ) /* GCC_SECTION("R_FCL_CODE_ROM") */;
void               R_FCL_InitSwapStateFlag                ( void ) /* GCC_SECTION("R_FCL_CODE_ROM")*/;
void               R_FCL_FCB_StartFlashOperation          ( const uint32_t flash_addr_u32,
                                                            const uint32_t data_or_end_addr_u32,
                                                            const uint8_t  fcb_cmd_u08) GCC_SECTION(".PRAM");
void               R_FCL_FCB_StartExtraAreaOperation      ( const uint32_t start_addr_u32,
                                                            const uint32_t end_addr_u32,
                                                            const uint8_t  fcb_cmd_u08) GCC_SECTION(".PRAM");
void               R_FCL_TempSwapChange                   ( void ) GCC_SECTION(".PRAM");
r_fcl_sps_flag_status_t R_FCL_GetStartupProgramSwapFlag   ( void ) GCC_SECTION(".PRAM");
uint8_t            R_FCL_GetOperatingMode                 ( void ) GCC_SECTION(".PRAM");
uint16_t           R_FCL_GetCodeFlashSize                 ( void ) GCC_SECTION(".PRAM");
r_fcl_sps_status_t R_FCL_StartupProgramSwapState          ( void ) GCC_SECTION(".PRAM");
uint8_t            R_FCL_GetSPASize                       ( void ) GCC_SECTION(".PRAM");
uint32_t           R_FCL_GetAWStartAddr                   ( void ) GCC_SECTION(".PRAM");
uint32_t           R_FCL_GetAWEndAddr                     ( void ) GCC_SECTION(".PRAM");
rBool              R_FCL_OngoingDataFlashOp               ( void ) GCC_SECTION(".PRAM");

/**********************************************************************************************************************
Detect wrong compile options 
**********************************************************************************************************************/
#if ((FCL_STATUS_CHECK != FCL_STATUS_CHECK_USER) && (FCL_STATUS_CHECK != FCL_STATUS_CHECK_INTERNAL))
    #error "Wrong FCL_STATUS_CHECK define" 
#endif

#endif /* ifndef R_FCL_GLOBAL_H */
