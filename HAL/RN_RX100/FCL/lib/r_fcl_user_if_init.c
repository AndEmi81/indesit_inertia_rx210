#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_user_if_init.c $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.10 $
* Mod. Date     : $Date: 2013/08/05 17:01:11MESZ $
* Device(s)     : MF3 Flash based RX100 microcontroller
* Description   : Source code for FCL initialization
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

/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0306)
* Reason:       To address a device specific address conversions are neccessary.
* Verification: The converted addresses are essential for correct code execution. Incorrect conversion would 
*               result in test fails.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"  
**********************************************************************************************************************/
#include "r_typedefs.h"
#include "r_fcl_types.h"
#include "fcl_cfg.h"
#include "r_fcl.h"
#include "r_fcl_global.h"
#include "r_fcl_env.h"

/**********************************************************************************************************************
Private global variables and functions 
**********************************************************************************************************************/


/**********************************************************************************************************************
 * Initialization functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_Init 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Function is executed before any execution of other FCL function. It initializes internal self-programming           
 * environment and internal variables. 
 *
 * @param         ---
 *
 * @return        ---
 */  
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 14.1 (QAC message 1503)
*               MISRA-C 2004 rule 11.3 (QAC message 0305)
* Reason:       This function is not used within the library, it is only used by the user application.
*               (QAC message 1503)
*               Conversion needed to corectly initialize the function pointer.(QAC message 0305)
* Verification: This function is part of the library API and therefore essential on the one hand. On the other hand 
*               the QAC warning is correct, because this function is never called within the library. To solve this 
*               dillemma it is neccessary and suitable to disable the message.(QAC message 1503)
*               Incorrect conversion would result in test fails.(QAC message 0305)
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROM
#endif

void R_FCL_Init( void )                                                                            /* PRQA S 1503 */
{
    uint32_t             cnt;
    volatile uint32_t             fcl_addr;

    CopyFlashRoutinesToRAM();

    /* Initially clear the complete fcl_data structure */
    fcl_addr = (uint32_t)(&g_fclInternal_str);                                                     /* PRQA S 0306 */
    for (cnt = 0; cnt < sizeof(g_fclInternal_str); cnt++)
    {
        R_FCL_WriteMemoryU08 (fcl_addr, 0u);
        fcl_addr++;
    }
    
    /* Parameter independent initializations */
    g_fclRomRamInterfacePtr_fp                   = FCL_FCT_RAM_POINTER_UNDEF;                      /* PRQA S 0305 */
    g_fclInternal_str.fclInternalState_enu       = R_FCL_LIB_ISTATE_INIT;
    g_fclInternal_str.multiOp_str.dstAddr_pu32   = FCL_ADDR_POINTER_UNDEFINED;
    g_fclInternal_str.resetReg_FLPMC_Flag_b      = false;
    g_fclInternal_str.fclOpType_u08              = FCL_NO_OPERATION;

    /* Initialize the swap state flag: copy value from extra area(BTFLG bit) to the  Flash sequencer and 
       initialization register (FSSET) */
    R_FCL_InitSwapStateFlag();
    
    /* Reset the Flash environment after a "warm Reset" */
    R_FCL_FCB_Reset();
}
/**********************************************************************************************************************
 End of function R_FCL_Init  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_SetFrequency 
 *********************************************************************************************************************/
/** 
 * User interface function:
 * Calculates the FCB frequency, based on the informations available in the SCKCR register, bits ICK[3-0] and FCK[3-0]
 * It also saves the cpu frequency to be used internally for waiting time loops
 * 
 * @param          cpu_freq_MHz_u08:  Rounded up CPU frequency in the range of 1MHz - 32MHz 
 *
 * @return         Operation status:
 * @li             R_FCL_OK:             Operation finished successfully
 * @li             R_FCL_ERR_PARAMETER:  Calculated frequency is not in the expected range
 * @li             R_FCL_ERR_REJECTED:   Library not initialized
 * @li             R_FCL_ERR_PROTECTION: Low speed operating mode active, FCB operations are prohibited
 */
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 14.1 (QAC message 1503)
* Reason:       This function is not used within the library, it is only used by the user application.
* Verification: This function is part of the library API and therefore essential on the one hand. On the other hand 
*               the QAC warning is correct, because this function is never called within the library. To solve this 
*               dillemma it is neccessary and suitable to disable the message.
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_SetFrequency (const uint8_t cpu_freq_MHz_u08)                                 /* PRQA S 1503 */
{
    r_fcl_status_t    status = R_FCL_OK;
    uint8_t           operating_mode;

    /* Operation possible only in init or idle state */
    if (  (R_FCL_LIB_ISTATE_INIT == g_fclInternal_str.fclInternalState_enu)
       || (R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu)
       )
    {
        operating_mode = R_FCL_GetOperatingMode();

        /* Check if in high-speed or middle-speed operating mode */
        if (FCL_LOW_SPEED_OP_MODE != operating_mode)
        {
            R_FCL_SetFlashFrequency(cpu_freq_MHz_u08, &g_fclInternal_str.fclFlashIfFreqMHz_u08);
            if (FCL_HIGH_SPEED_OP_MODE == operating_mode)
            {
                /* High-speed operating mode, range is 1:32 MHz */
                if (  (FCL_FCB_CLK_MIN_MHz         > g_fclInternal_str.fclFlashIfFreqMHz_u08)
                   || (FCL_FCB_CLK_IN_HSOM_MAX_MHz < g_fclInternal_str.fclFlashIfFreqMHz_u08) )
                {
                    status = R_FCL_ERR_PARAMETER;
                }
            }
            else
            {
                /* Middle-speed operating mode, range is 1:12 MHz */
                if (  (FCL_FCB_CLK_MIN_MHz         > g_fclInternal_str.fclFlashIfFreqMHz_u08)
                   || (FCL_FCB_CLK_IN_MSOM_MAX_MHz < g_fclInternal_str.fclFlashIfFreqMHz_u08) )
                {
                    status = R_FCL_ERR_PARAMETER;
                }
            }

            if (R_FCL_OK == status)
            {
                g_fclInternal_str.fclCpuFreqMHz_u08    = cpu_freq_MHz_u08;
                g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_IDLE;
            }
        }
        else
        {
            /* Low-speed operating mode, P/E prohibited */
            status = R_FCL_ERR_PROTECTION;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_SetFrequency  
 *********************************************************************************************************************/
