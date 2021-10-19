#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_hw_access.c $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.22 $
* Mod. Date     : $Date: 2013/08/05 17:00:27MESZ $
* Device(s)     : MF3 Flash based RX100 microcontroller
* Description   : FCL hardware interface functions
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
* MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0303)
* Reason:       For effective embedded programming, integer to pointer conversions are used
* Verification: The converted addresses are essential for complete code execution. Incorrect conversion would 
*               result in test fails.
**********************************************************************************************************************/


/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"  
**********************************************************************************************************************/
#include "r_typedefs.h"
#include "r_fcl_types.h"
#include "fcl_cfg.h"
#include "r_fcl.h"
#include "r_fcl_env.h"
#include "r_fcl_global.h"

#if   FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    #include "RN_RX100_reg.h"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #include "iorx111.h"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #include "rx111_iodefine_v050.h"
#endif

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define FCL_SPA_SIZE_SHIFT_BIT               (14u)

/**********************************************************************************************************************
Private global variables and functions 
**********************************************************************************************************************/
FCL_STATIC void     R_FCL_FCB_WriteRegFLPMC   ( const uint8_t val_u08 )   GCC_SECTION(".PRAM");
FCL_STATIC void     R_FCL_WriteMemoryU16      ( const uint32_t addr_u32, 
                                                const uint16_t val_u16 )  GCC_SECTION(".PRAM");
FCL_STATIC uint8_t  R_FCL_ReadMemoryU08       ( const uint32_t addr_u32 ) GCC_SECTION(".PRAM");
FCL_STATIC uint16_t R_FCL_ReadMemoryU16       ( const uint32_t addr_u32 ) GCC_SECTION(".PRAM");
FCL_STATIC void     R_FCL_CheckSetBitEEEMD    ( void ) 					  GCC_SECTION(".PRAM");
FCL_STATIC void     R_FCL_CheckResetBitEEEMD  ( void )                    GCC_SECTION(".PRAM");


/**********************************************************************************************************************
 * Helper functions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_FCB_WriteRegFLPMC 
 *********************************************************************************************************************/
/**
 *  Helper function that writes the FLPMC register, that is secured by a special sequence. The write must be ckecked 
 *  in a loop, as an interrupt handling can lead to a wrong sequence, thus preventing the write. 
 *  In this case the write must be repeated.  
 *
 * @param     val_u08:      The value to be written in register FLPMC
 *
 * @return    ---
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC void R_FCL_FCB_WriteRegFLPMC ( const uint8_t val_u08 )
{
    volatile uint8_t reg_FLPMC; 

    do
    {
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_PFCMD_U08, FCB_REG_PFCMD_CMD);
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLPMC_U08, val_u08);
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLPMC_U08, (~val_u08));
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLPMC_U08, val_u08);

        reg_FLPMC = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FLPMC_U08);
    }
    while (reg_FLPMC != val_u08);
}
/**********************************************************************************************************************
 * End of function R_FCL_FCB_WriteRegFLPMC  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_WriteMemoryU16 
 *********************************************************************************************************************/
/**
 * Helper function that writes a 16-bit IO register or memory
 *
 * @param     addr_u32:      write destination address
 * @param     val_u16:       16-bit write data
 *
 * @return    ---
 */ 
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC void R_FCL_WriteMemoryU16 ( const uint32_t addr_u32, const uint16_t val_u16 )
{
    ( *( (volatile uint16_t *)(addr_u32) ) ) = val_u16;                                            /* PRQA S 0303 */
}
/**********************************************************************************************************************
 End of function R_FCL_WriteMemoryU16  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ReadMemoryU08 
 *********************************************************************************************************************/
/**
 * Helper function to read a 8-bit IO register or memory
 *
 * @param     addr_u32:      Source address
 * 
 * @return    8-bit register contents    
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC uint8_t R_FCL_ReadMemoryU08 ( const uint32_t addr_u32 )
{
    return ( *( (volatile uint8_t *)(addr_u32) ) );                                                /* PRQA S 0303 */
}
/**********************************************************************************************************************
 End of function R_FCL_ReadMemoryU08  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ReadMemoryU16 
 *********************************************************************************************************************/
/**
 * Helper function to read a 16-bit IO register or memory
 *
 * @param     addr_u32:      Source address
 * 
 * @return    16-bit register contents    
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC uint16_t R_FCL_ReadMemoryU16 ( const uint32_t addr_u32 )
{
    return ( *( (volatile uint16_t *)(addr_u32) ) );                                               /* PRQA S 0303 */
}
/**********************************************************************************************************************
 End of function R_FCL_ReadMemoryU16  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_CheckSetBitEEEMD 
 *********************************************************************************************************************/
/**
 * Helper function:
 * In order to modify the temporary start-up program swap flags, the bits EEEMD or FLSPM have to be set. 
 * This function checks if at least one of the mentioned bits are set, if not sets the bit EEEMD.
 *
 * @param     ---
 * @return    ---   
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
FCL_STATIC void R_FCL_CheckSetBitEEEMD( void )
{
    volatile uint8_t    reg_FLPMC;

    reg_FLPMC = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FLPMC_U08);
    if (0 == (reg_FLPMC & (FCB_REG_BIT_FLPMC_EEEMD | FCB_REG_BIT_FLPMC_FLSPM)))
    {
        reg_FLPMC |= FCB_REG_BIT_FLPMC_EEEMD;
        R_FCL_FCB_WriteRegFLPMC (reg_FLPMC);
        g_fclInternal_str.resetReg_FLPMC_Flag_b = true;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_CheckSetBitEEEMD  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_CheckResetBitEEEMD 
 *********************************************************************************************************************/
/**
 * Helper function:
 * In order to modify the temporary start-up program swap flags, the bits EEEMD or FLSPM have to be set. 
 * This function checks if bit EEEMD was set by function R_FCL_CheckSetBitEEEMD, if so restores 
 * the previous value of FLPMC register.
 *
 * @param     ---
 * @return    ---   
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
FCL_STATIC void R_FCL_CheckResetBitEEEMD( void )
{
    volatile uint8_t    reg_FLPMC;

    if (true == g_fclInternal_str.resetReg_FLPMC_Flag_b)
    {
        reg_FLPMC = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FLPMC_U08);
        reg_FLPMC &= (~FCB_REG_BIT_FLPMC_EEEMD);
        R_FCL_FCB_WriteRegFLPMC (reg_FLPMC);
        g_fclInternal_str.resetReg_FLPMC_Flag_b = false;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_CheckResetBitEEEMD  
 *********************************************************************************************************************/



/**********************************************************************************************************************
 * Interface functions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_WriteMemoryU08 
 *********************************************************************************************************************/
/**
 * Helper function that writes a 8-bit IO register or memory
 *
 * @param     addr_u32:      write destination address
 * @param     val_u8:        8-bit write data
 *
 * @return    ---
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
void R_FCL_WriteMemoryU08 ( const uint32_t addr_u32, const uint8_t val_u8 )
{
    ( *( (volatile uint8_t *)(addr_u32) ) ) = val_u8;                                              /* PRQA S 0303 */
}
/**********************************************************************************************************************
 End of function R_FCL_WriteMemoryU08  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_CheckReady 
 *********************************************************************************************************************/
/**
 *  Returns sequencer busy/ready status 
 *  The sequencer might be busy or not busy (not configured or configured operation already finished) 
 *
 * @param         ---
 *
 * @return        Status of the sequencer:
 * @li            R_FCL_TRUE  - Sequencer is not busy with a Flash operation  
 * @li            R_FCL_FALSE - Sequencer is busy with a Flash operation
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
r_fcl_flag_t R_FCL_FCB_CheckReady ( void )
{
    r_fcl_flag_t        fcb_ready_flag = R_FCL_FALSE;
    volatile uint8_t    reg_FSASTH;

    reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
    
    if (FCL_EXTRA_AREA_OP == g_fclInternal_str.fclOpType_u08)
    {
        if (FCB_REG_BIT_FSASTH_ESQEND == (reg_FSASTH & FCB_REG_BIT_FSASTH_ESQEND))
        {
            fcb_ready_flag = R_FCL_TRUE;
        }
    }
    else
    {
        /* Flash Operation */
        if (  (FCB_REG_BIT_FSASTH_SQEND  == (reg_FSASTH & FCB_REG_BIT_FSASTH_SQEND))
           || (FCB_REG_BIT_FSASTH_DRREQ  == (reg_FSASTH & FCB_REG_BIT_FSASTH_DRREQ))
           )
        {
            fcb_ready_flag = R_FCL_TRUE;
        }
    }

    return fcb_ready_flag;
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_CheckReady  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_ReadOperation 
 *********************************************************************************************************************/
/**
 * Reads out data to output buffer, resets the data read request flag and starts a new read command.
 * Called by R_FCL_HandleMultiOperation, to handle multiple READ operation. 
 *
 * @param         ---
 * @return        ---
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
void R_FCL_FCB_ReadOperation ( void )
{
    volatile uint8_t   reg_FSASTH;
    volatile uint16_t  reg_data;

    /* Read information from registers and save it into output buffer */
    reg_data = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLRH_U16);
    *((g_fclInternal_str.multiOp_str).dstAddr_pu32) = (uint32_t)(reg_data) << 16;
    reg_data = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLRL_U16);
    *((g_fclInternal_str.multiOp_str).dstAddr_pu32) |=  reg_data ;

    /* Clear data read request flag */
    R_FCL_WriteMemoryU08 ( FCB_REG_ADDR_FSSQ_U08,
                           (FCB_REG_BIT_FSSQ_SQST | FCB_REG_BIT_FSSQ_DSETEN | FCB_REG_BIT_FSSQ_CMD_READ) );
    do
    {
        reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
    } while (FCB_REG_BIT_FSASTH_DRREQ == (reg_FSASTH & FCB_REG_BIT_FSASTH_DRREQ));

    /* Start a new read command*/
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSQ_U08, (FCB_REG_BIT_FSSQ_SQST | FCB_REG_BIT_FSSQ_CMD_READ));
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_ReadOperation  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_GetOpStatus 
 *********************************************************************************************************************/
/**
 * Return FCB Flash operation result. Called by R_FCL_StatusCheck.
 *
 * @param         ---
 * @return        Operation status:
 * @li            R_FCL_OK:             Operation terminated successfully 
 * @li            R_FCL_BUSY:           Operation is still on-going 
 * @li            R_FCL_ERR_WRITE:      Flash write error
 * @li            R_FCL_ERR_ERASE:      Flash erase error
 * @li            R_FCL_ERR_BLANKCHECK: Flash blank check error
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
r_fcl_status_t R_FCL_FCB_GetOpStatus ( void )
{
    r_fcl_status_t      status;
    volatile uint8_t    reg_FSASTL;

    /* Read background operation result */
    reg_FSASTL =  R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTL_U08);
    reg_FSASTL &= FCB_REG_MASK_FSASTL_ERR;

    #ifdef FCL_TEST_PATCH
        if (test_patch.act_flag_FSASTL != false)
        {
            reg_FSASTL = test_patch.reg_FSASTL;
            test_patch.act_flag_FSASTL = false;
        }
    #endif

    /* Check if the operation finished with an error */
    if (FCB_REG_BITS_FSASTL_FLASH_OK == reg_FSASTL)
    {
        /* No error */
        status = R_FCL_OK;
    }
    else
    {
        if (  (FCB_REG_BIT_FSASTL_BLER == reg_FSASTL)
           && (FCB_REG_BIT_FSSQ_CMD_BC == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
           )
        {
            status = R_FCL_ERR_BLANKCHECK;
        }
        else if (  (FCB_REG_BIT_FSASTL_WRER == reg_FSASTL)
                && (FCB_REG_BIT_FSSQ_CMD_WRITE == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
                )
        {
            status = R_FCL_ERR_WRITE;
        }
        else if (  (FCB_REG_BIT_FSASTL_ERER == reg_FSASTL)
                && (FCB_REG_BIT_FSSQ_CMD_ERASE == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
                )
        {
            status = R_FCL_ERR_ERASE;
        }
        else if (  (FCB_REG_BIT_FSASTL_SEQER == reg_FSASTL)
                && (  (FCB_REG_BIT_FSSQ_CMD_ERASE == (g_fclInternal_str.multiOp_str).fcbCmd_u08) 
                   || (FCB_REG_BIT_FSSQ_CMD_WRITE == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
                   )
                )
        {
            /* For write and erase operations, in case the specified area is outside of the AW 
               return R_FCL_ERR_PROTECTION */
            status = R_FCL_ERR_PROTECTION;
        }
        else
        {
            /* Sequence error */
            status = R_FCL_ERR_INTERNAL;
        }
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_GetOpStatus  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_ClearControlReg 
 *********************************************************************************************************************/
/**
 * Resets the control registers after an operation is finished. 
 *
 * @param         ---
 *
 * @return        ---
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
void R_FCL_FCB_ClearControlReg ( void )
{
    volatile uint8_t    reg_FSASTH;

    if (FCL_EXTRA_AREA_OP == g_fclInternal_str.fclOpType_u08)
    {
        /* Reset sequencer control registers */
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSE_U08, FCB_CONTROL_REG_RESET);
        
        do
        {
            reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
        }
        while (FCB_REG_BIT_FSASTH_ESQEND == (FCB_REG_BIT_FSASTH_ESQEND & reg_FSASTH));
    }
    else
    {
        /* Reset sequencer control registers for Flash operations */
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSQ_U08, FCB_CONTROL_REG_RESET);
        
        do
        {
            reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
        }
        while (FCB_REG_BIT_FSASTH_SQEND  == (FCB_REG_BIT_FSASTH_SQEND & reg_FSASTH));
    }

    /* For extra area operation and Read operation, the data flash access needs to be modified by the library,
       return to previous data access state, if activated by library */
    if (FCL_DATA_FLASH_ACCESS_BY_LIBRARY == g_fclInternal_str.fclDataFlashMemAccess_u08)
    {
        FLASH.DFLCTL.BIT.DFLEN = 0;                                                                /* PRQA S 0303 */
        g_fclInternal_str.fclDataFlashMemAccess_u08 = 0x0;
    }
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_ClearControlReg  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_Reset 
 *********************************************************************************************************************/
/**
 * Reset Flash programming sequencer. As the reset does not work on running sequencer and in case of transient        
 * conditions, it must be stopped first. 
 *
 * @param         ---
 * 
 * @return        ---
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROM
#endif 
void R_FCL_FCB_Reset ( void )
{
    volatile uint8_t   reg_FSSQ;
    volatile uint8_t   reg_FSSE;
    volatile uint8_t   reg_FSASTH;

    reg_FSSQ = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSQ_U08);
    reg_FSSE = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSE_U08);

    /* Check if extra area operation is on */
    if (FCB_REG_BIT_FSSE_ESQST == (reg_FSSE & FCB_REG_BIT_FSSE_ESQST))
    {
        /* Wait for the extra area operation to finish */
        do 
        { 
            reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
        }
        while (FCB_REG_BIT_FSASTH_ESQEND != (FCB_REG_BIT_FSASTH_ESQEND & reg_FSASTH));

        R_FCL_WriteMemoryU08(FCB_REG_ADDR_FSSE_U08, FCB_CONTROL_REG_RESET);

        do 
        { 
            reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
        }
        while (FCB_REG_BIT_FSASTH_ESQEND == (FCB_REG_BIT_FSASTH_ESQEND & reg_FSASTH));
    }
    else
    {
        /* Stop the sequencer if it is busy */
        if (FCB_REG_BIT_FSSQ_SQST == (reg_FSSQ & FCB_REG_BIT_FSSQ_SQST))
        {
            reg_FSSQ |= FCB_REG_BIT_FSSQ_FSSTP;
            R_FCL_WriteMemoryU08 (FCB_REG_BIT_FSSQ_SQST, reg_FSSQ);
            
            do
            {
                reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
            }
            while (FCB_REG_BIT_FSASTH_SQEND != (FCB_REG_BIT_FSASTH_SQEND & reg_FSASTH));

            R_FCL_WriteMemoryU08(FCB_REG_ADDR_FSSQ_U08, FCB_CONTROL_REG_RESET);

            do 
            { 
                reg_FSASTH = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSASTH_U08);
            }
            while (FCB_REG_BIT_FSASTH_SQEND == (FCB_REG_BIT_FSASTH_SQEND & reg_FSASTH));
        }
    }

    /* Issue reset signal on stopped sequencer */
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLRST_U08, FCB_REG_BIT_FLRST_FLRST);
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLRST_U08, 0x00);
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_Reset  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_SwitchMode 
 *********************************************************************************************************************/
/**
 * Switch FCB mode to Programming/User mode 
 *
 * @param         mode_enu:               Modes:
 * @li                                    R_FCL_FCB_MODE_USER
 * @li                                    R_FCL_FCB_MODE_PE
 *
 * @return        Operation status:
 * @li            R_FCL_OK:             Operation terminated successfully
 * @li            R_FCL_ERR_PROTECTION: Low speed operating mode, flash operations are prohibited
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
r_fcl_status_t R_FCL_FCB_SwitchMode ( const r_fcl_fcb_mode_t mode_enu )
{
    r_fcl_status_t       status = R_FCL_OK;
    volatile uint8_t     reg_FSSET;

    if (R_FCL_FCB_MODE_USER == mode_enu)
    {
        /* Enter user mode: special sequence needed */
        R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_CWEE  |
                                   FCB_REG_BIT_FLPMC_EEEMD |
                                   FCB_REG_BIT_FLPMC_FLSPM);

        R_FCL_WaitUs (WAIT_TDIS_TIME_Us, g_fclInternal_str.fclCpuFreqMHz_u08);

        R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_EEEMD | FCB_REG_BIT_FLPMC_FLSPM);

        R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_FWEDIS);

        /* operating mode: high speed or middle speed */ 
        if (REG_OPCCR_MASK_MODE_HIGH_SPEED == (uint8_t)SYSTEM.OPCCR.BIT.OPCM)                      /* PRQA S 0303 */
        {
            R_FCL_WaitUs (WAIT_TMS_TIME_HS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
        }
        else
        {
            R_FCL_WaitUs (WAIT_TMS_TIME_MS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
        }

        /* set bit FENTRYRI = 0 */
        R_FCL_WriteMemoryU16 (RX_SFR_REG_ADDR_FENTRYR_U16, RX_SFR_REG_BIT_FENTRYR_KEYCODE);
    }
    else
    {
        /* Start programming/erase mode: special sequence needed */
        /* check the operating mode: if low speed then operations are not allowed */
        if (0 == SYSTEM.SOPCCR.BIT.SOPCM)                                                          /* PRQA S 0303 */
        {
            /* enable code flash operations */
            R_FCL_WriteMemoryU16 ( RX_SFR_REG_ADDR_FENTRYR_U16,
                                    (RX_SFR_REG_BIT_FENTRYR_KEYCODE | RX_SFR_REG_BIT_FENTRYR_FENTRYI));

            R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_EEEMD | FCB_REG_BIT_FLPMC_FLSPM);

            R_FCL_WaitUs (WAIT_TDIS_TIME_Us, g_fclInternal_str.fclCpuFreqMHz_u08);

            if (REG_OPCCR_MASK_MODE_HIGH_SPEED == (uint8_t)SYSTEM.OPCCR.BIT.OPCM)                  /* PRQA S 0303 */
            {
                /* High speed operating mode */
                R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_CWEE  |
                                           FCB_REG_BIT_FLPMC_EEEMD |
                                           FCB_REG_BIT_FLPMC_FLSPM);

                R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_CWEE | FCB_REG_BIT_FLPMC_FLSPM);

                R_FCL_WaitUs (WAIT_TMS_TIME_HS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
            }
            else
            {
                /* Middle speed operating mode */
                R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_CWEE   |
                                           FCB_REG_BIT_FLPMC_LOWSPY |
                                           FCB_REG_BIT_FLPMC_EEEMD  |
                                           FCB_REG_BIT_FLPMC_FLSPM);

                R_FCL_FCB_WriteRegFLPMC (FCB_REG_BIT_FLPMC_CWEE   |
                                           FCB_REG_BIT_FLPMC_LOWSPY |
                                           FCB_REG_BIT_FLPMC_FLSPM);

                R_FCL_WaitUs (WAIT_TMS_TIME_MS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
            }

            /* Frequency set */
            reg_FSSET = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSET_U08);
            reg_FSSET &= (~FCB_REG_MASK_FSSET_FSET);
            reg_FSSET |= (uint8_t)(g_fclInternal_str.fclFlashIfFreqMHz_u08 - 1);
            R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSET_U08, reg_FSSET);
        }
        else
        {
            status = R_FCL_ERR_PROTECTION;
        }

    }

    return status;
} 
/**********************************************************************************************************************
 End of function R_FCL_FCB_SwitchMode  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_StartFlashOperation 
 *********************************************************************************************************************/
/**
 * This function configures the Flash programming sequencer and starts the operation 
 *
 * @param         flash_addr_u32:       Start address (start block address for erase, 1st address for BC/Write/IV/Read)
 * @param         data_or_end_addr_u32: End address of the Flash operation on Erase/BC/IV/Read or data for Write
 * @param         fcb_cmd_u08:          Type of command. Erase / BC / IV / Write / Read
 *
 * @return        ---
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
void R_FCL_FCB_StartFlashOperation ( const uint32_t flash_addr_u32,
                                     const uint32_t data_or_end_addr_u32,
                                     const uint8_t  fcb_cmd_u08 )
{
    volatile uint8_t    reg_FSSQ;

    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLARS_U08, FCB_REG_FLARS_USER_AREA);

    if (FCB_REG_BIT_FSSQ_CMD_READ == fcb_cmd_u08)
    {
        if (0 == FLASH.DFLCTL.BIT.DFLEN)                                                           /* PRQA S 0303 */
        {
            g_fclInternal_str.fclDataFlashMemAccess_u08 = FCL_DATA_FLASH_ACCESS_BY_LIBRARY;
            FLASH.DFLCTL.BIT.DFLEN = 1;                                                            /* PRQA S 0303 */
            if (REG_OPCCR_MASK_MODE_HIGH_SPEED == (uint8_t)SYSTEM.OPCCR.BIT.OPCM)                  /* PRQA S 0303 */
            {
                /* High speed operating mode */
                R_FCL_WaitUs (WAIT_TSTOP_TIME_HS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
            }
            else
            {
                /* Middle speed operating mode */
                R_FCL_WaitUs (WAIT_TSTOP_TIME_MS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
            }
        }
    }

    R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLAPL_U16, (uint16_t)(flash_addr_u32 & 0xFFFFu));
    R_FCL_WriteMemoryU08   (FCB_REG_ADDR_FLAPH_U08, (uint8_t)(flash_addr_u32 >> 16u) & 0x0Fu);

    if (FCB_REG_BIT_FSSQ_CMD_WRITE == fcb_cmd_u08)
    {
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLSEDL_U16, 0x0000u);
        R_FCL_WriteMemoryU08   (FCB_REG_ADDR_FLSEDH_U08, 0x00u);
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWL_U16, (uint16_t)(data_or_end_addr_u32 & 0xFFFFu));
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWH_U16, (uint16_t)(data_or_end_addr_u32 >> 16u));
    }
    else
    {
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLSEDL_U16, (uint16_t)(data_or_end_addr_u32 & 0xFFFFu));
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLSEDH_U08, (uint8_t)(data_or_end_addr_u32 >> 16u) & 0x0Fu);
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWL_U16, 0x0000u);
        R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWH_U16, 0x0000u);
    }

    /* Save command and start sequencer operation */
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSQ_U08, fcb_cmd_u08);
    reg_FSSQ =  R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSQ_U08);
    reg_FSSQ |= FCB_REG_BIT_FSSQ_SQST;
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSQ_U08, reg_FSSQ);
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_StartFlashOperation  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_FCB_StartExtraAreaOperation 
 *********************************************************************************************************************/
/**
 * This function configures the Flash programming sequencer and starts the extra area operation 
 *
 * @param         start_addr_u32:       Start address for AW 
 * @param         end_addr_u32:         End address for AW
 * @param         fcb_cmd_u08:          Type of command: Set AW range or change start-up program swap flag
 *
 * @return        ---
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
void R_FCL_FCB_StartExtraAreaOperation ( const uint32_t start_addr_u32,
                                         const uint32_t end_addr_u32, 
                                         const uint8_t  fcb_cmd_u08 ) 
{
    uint16_t             start_addr;
    uint16_t             end_addr;
    volatile uint8_t     reg_FSSE;
    volatile uint16_t    reg_FLSEC;

    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FLARS_U08, FCB_REG_BIT_FLARS_EXA);

    if (0 == FLASH.DFLCTL.BIT.DFLEN)                                                               /* PRQA S 0303 */
    {
        g_fclInternal_str.fclDataFlashMemAccess_u08 = FCL_DATA_FLASH_ACCESS_BY_LIBRARY;
        FLASH.DFLCTL.BIT.DFLEN = 1;                                                                /* PRQA S 0303 */
        if (REG_OPCCR_MASK_MODE_HIGH_SPEED == (uint8_t)SYSTEM.OPCCR.BIT.OPCM)                      /* PRQA S 0303 */
        {
            /* High speed operating mode */
            R_FCL_WaitUs (WAIT_TSTOP_TIME_HS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
        }
        else
        {
            /* Middle speed operating mode */
            R_FCL_WaitUs (WAIT_TSTOP_TIME_MS_Us, g_fclInternal_str.fclCpuFreqMHz_u08);
        }
    }

    R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLAPL_U16, 0x0000u);
    R_FCL_WriteMemoryU08   (FCB_REG_ADDR_FLAPH_U08, 0x00u);
    R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLSEDL_U16, 0x0000u);
    R_FCL_WriteMemoryU08   (FCB_REG_ADDR_FLSEDH_U08, 0x00u);

    if (fcb_cmd_u08 == FCB_REG_BIT_FSSE_CMD_WRT_AW)
    {
        /* Calculate start and end block addresses */
        /* Only bits 19-10 are needed */
        start_addr = (uint16_t)(start_addr_u32 >> 10);
        end_addr   = ((uint16_t)(end_addr_u32 >> 10)) + 1;
    }
    else
    {
        reg_FLSEC  = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLSEC_U16);
        start_addr = reg_FLSEC ^ FCB_REG_BIT_FLSEC_BTFLG; /* Invert start-up program swap flag */
        end_addr   = 0xFFFF; 
    }

    /* Set start address or data information */
    R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWL_U16, start_addr);
    R_FCL_WriteMemoryU16 (FCB_REG_ADDR_FLWH_U16, end_addr);

    /* Save command and start sequencer operation */
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSE_U08, fcb_cmd_u08);
    reg_FSSE =  R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSE_U08);
    reg_FSSE |= FCB_REG_BIT_FSSE_ESQST;
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSE_U08, reg_FSSE);
}
/**********************************************************************************************************************
 End of function R_FCL_FCB_StartExtraAreaOperation  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_TempSwapChange 
 *********************************************************************************************************************/
/**
 * Function swaps the start-up program area 0 and start-up program area 1 physically without setting 
 * the start-up program swap flag. After reset, the start-up program area will be activated regarding the 
 * start-up program swap flag.
 * Called by R_FCL_ChangeSwapState.
 *
 * @param         ---
 *
 * @return        ---
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
void R_FCL_TempSwapChange ( void )
{
    volatile uint8_t    reg_FSSET;

    /* In order to modify the temporary start-up program swap state, the bits EEEMD or FLSPM have to be set */
    R_FCL_CheckSetBitEEEMD();

    reg_FSSET = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSET_U08);
    reg_FSSET ^= FCB_REG_BIT_FSSET_TMBTSEL;
    reg_FSSET |= FCB_REG_BIT_FSSET_TMSPMD;
    R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSET_U08, reg_FSSET);

    /* reset the EEEMD bit after operation finished */
    R_FCL_CheckResetBitEEEMD();
}
/**********************************************************************************************************************
 End of function R_FCL_TempSwapChange  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetStartupProgramSwapFlag 
 *********************************************************************************************************************/
/**
 * Checks if a start-up program swap will be performed after reset.
 *
 * @param         ---
 *
 * @return        R_FCL_NO_SPS_FLAG_SWAP: no start-up program swap
 *                R_FCL_SPS_FLAG_SWAPPED: start-up program swap will be performed after reset
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_sps_flag_status_t R_FCL_GetStartupProgramSwapFlag ( void )
{
    volatile uint16_t         reg_FLSEC;
    r_fcl_sps_flag_status_t   flag ;

    flag      = R_FCL_NO_SPS_FLAG_SWAP;
    reg_FLSEC = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLSEC_U16);

    if (FCB_REG_BIT_FLSEC_BTFLG != (reg_FLSEC & FCB_REG_BIT_FLSEC_BTFLG))
    {
        flag = R_FCL_SPS_FLAG_SWAPPED;
    }

    return flag;
}
/**********************************************************************************************************************
 End of function R_FCL_GetStartupProgramSwapFlag  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetOperatingMode 
 *********************************************************************************************************************/
/**
 * Determines what is the current operating mode.
 * Called by R_FCL_SetFrequency.
 *
 * @param         ---
 *
 * @return        power operating mode:
 * @li            FCL_LOW_SPEED_OP_MODE
 * @li            FCL_MIDDLE_SPEED_OP_MODE
 * @li            FCL_HIGH_SPEED_OP_MODE
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint8_t R_FCL_GetOperatingMode ( void )
{
    uint8_t operating_mode;

    /* Check if in high-speed or middle-speed operating mode */
    if (0 == SYSTEM.SOPCCR.BIT.SOPCM)                                                              /* PRQA S 0303 */
    {
        if (REG_OPCCR_MASK_MODE_HIGH_SPEED == (uint8_t)SYSTEM.OPCCR.BIT.OPCM)                      /* PRQA S 0303 */
        {
            operating_mode = FCL_HIGH_SPEED_OP_MODE;
        }
        else
        {
            operating_mode = FCL_MIDDLE_SPEED_OP_MODE;
        }
    }
    else
    {
        operating_mode = FCL_LOW_SPEED_OP_MODE;
    }

    return operating_mode;
}
/**********************************************************************************************************************
 End of function R_FCL_GetOperatingMode  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetCodeFlashSize 
 *********************************************************************************************************************/
/**
 * Outputs the code flash size. The information is read from the PORGA register, bits 2-0.
 * Called by R_FCL_GetBlockCnt.
 *
 * @param         ---
 *
 * @return        code flash size: 
 * @li            0: 2MByte
 * @li            1: 1MByte
 * @li            2: 512KByte
 * @li            3: 256KByte
 * @li            4: 128KByte
 * @li            5: 64KByte
 * @li            6: 32KByte
 * @li            7: 16KByte
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint16_t R_FCL_GetCodeFlashSize ( void )
{
    volatile uint16_t    size;

    size = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_PORGA_U16);
    size &= FCB_REG_PORGA_ROM_SIZE_MASK;

    #ifdef FCL_TEST_PATCH
        if (test_patch.act_flag_dev_flash_size != false)
        {
            size = test_patch.flash_size;
            test_patch.act_flag_dev_flash_size = false;
        }
    #endif

    return size;
}
/**********************************************************************************************************************
 End of function R_FCL_GetCodeFlashSize  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_StartupProgramSwapState 
 *********************************************************************************************************************/
/**
 * Function returns the current start-up program swap state, no matter if a temporary or a permanent swap 
 * was performed or not.
 * Called by R_FCL_GetSwapState.
 *
 * @param         ---
 *
 * @return        current start-up program swap state:
 * @li            R_FCL_NO_SP_SWAP: Start-up program swap was not performed
 * @li            R_FCL_SP_SWAPPED: Either a temporary or a permanent start-up program swap was performed
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_sps_status_t R_FCL_StartupProgramSwapState ( void )
{
    r_fcl_sps_status_t   sps_status;
    volatile uint8_t     reg_FSSET;

    reg_FSSET = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSET_U08);
    /* Bit TMSPMD already set by R_FCL_Init function, no need to check it again */
    if (FCB_REG_BIT_FSSET_TMBTSEL == (reg_FSSET & FCB_REG_BIT_FSSET_TMBTSEL))
    {
        sps_status = R_FCL_SP_SWAPPED;
    }
    else
    {
        sps_status = R_FCL_NO_SP_SWAP;
    }

    return sps_status;
}
/**********************************************************************************************************************
 End of function R_FCL_StartupProgramSwapState  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetSPASize 
 *********************************************************************************************************************/
/**
 * Function gets the current size of start-up program area.
 * Called by R_FCL_GetStartupAreaSize.
 *
 * @param         ---
 *
 * @return        Start-up program area size:
 * @li            0x00: Start-up program area size is 4KB
 * @li            0x01: Start-up program area size is 8KB
 * @li            0x02: Start-up program area size is 16KB
 * @li            0x03: Start-up program area size is the same as for 0x00, in our case 4KB
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint8_t R_FCL_GetSPASize ( void )
{
    uint8_t           spa_size;
    volatile uint16_t reg_PORGA;

    reg_PORGA =   R_FCL_ReadMemoryU16 (FCB_REG_ADDR_PORGA_U16);
    reg_PORGA &=  FCB_REG_MASK_PORGA_SPA_SIZE;
    reg_PORGA >>= FCL_SPA_SIZE_SHIFT_BIT;

    spa_size = (uint8_t)reg_PORGA;

    return spa_size;
}
/**********************************************************************************************************************
 End of function R_FCL_GetSPASize  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_GetAWStartAddr 
 *********************************************************************************************************************/
/**
 * Calculates the starting address of the AW range.
 * Called by R_FCL_GetAW.
 *
 * @param         ---
 *
 * @return        starting address of AW
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint32_t R_FCL_GetAWStartAddr ( void )
{
    volatile uint32_t    reg_FLFSWE;
    uint32_t             start_addr;

    reg_FLFSWE = (uint32_t)R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLFSWE_U16);
    /* only bits 19-10 are saved */
    start_addr = (reg_FLFSWE << 10);

    return start_addr;
}
/**********************************************************************************************************************
 End of function R_FCL_GetAWStartAddr  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetAWEndAddr 
 *********************************************************************************************************************/
/**
 * Calculates the ending address of the AW range.
 * Called by R_FCL_GetAW.
 *
 * @param         ---
 *
 * @return        ending address of AW
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint32_t R_FCL_GetAWEndAddr ( void )
{
    volatile uint32_t    reg_FLFSWS;
    uint32_t             end_addr;

    reg_FLFSWS = (uint32_t)R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLFSWS_U16);
    /* only bits 19-10 are saved */
    end_addr = (reg_FLFSWS << 10);

    return end_addr;
}
/**********************************************************************************************************************
 End of function R_FCL_GetAWEndAddr  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_SetFlashFrequency 
 *********************************************************************************************************************/
/**
 * Calculates the Flash IF frequency to be used by FCB.
 * Called by R_FCL_SetFrequency.
 *
 * @param         cpu_freq_u08:  CPU frequency
 * @param[out]    fcb_freq_pu08: Calculated Flash IF frequency
 *
 * @return        Operation status:
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
void R_FCL_SetFlashFrequency ( const uint8_t cpu_freq_u08, uint8_t * const fcb_freq_pu08 )
{
    *fcb_freq_pu08 = (uint8_t)((uint32_t)(cpu_freq_u08) << SYSTEM.SCKCR.BIT.ICK);              /* PRQA S 0303 */
    *fcb_freq_pu08 >>= SYSTEM.SCKCR.BIT.FCK;                                                   /* PRQA S 0303 */
}
/**********************************************************************************************************************
 End of function R_FCL_SetFlashFrequency  
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_OngoingDataFlashOp 
 *********************************************************************************************************************/
/** 
 *  Function checks if a data flash access is ongoing 
 *
 *  @param          --- 
 *  @return         true:  ongoing data flash access
 *                  false: no data flash access 
 */ 
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
rBool R_FCL_OngoingDataFlashOp ( void )
{
    rBool               df_ongoing;
    volatile uint8_t    reg_FLPMC;
    
    df_ongoing = false;

    /* check if a Data Flash operation is on-going */
    reg_FLPMC = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FLPMC_U08);

    if ( FCB_REG_BIT_FLPMC_EEEMD == (reg_FLPMC & FCB_REG_BIT_FLPMC_EEEMD) )
    {
        df_ongoing = true;
    }
    
    return df_ongoing;
}
/**********************************************************************************************************************
 End of function R_FCL_OngoingDataFlashOp  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_InitSwapStateFlag 
 *********************************************************************************************************************/
/** 
 *  Initializes the swap state flag by copying the value from extra area(BTFLG bit) to the  Flash sequencer and 
 *  initialization register (FSSET)
 *  Called by R_FCL_Init.
 *
 *  @param          --- 
 *  @return         --- 
 */ 
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GNU
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROM
#endif
void R_FCL_InitSwapStateFlag( void )
{
    uint8_t              new_val;
    volatile uint16_t    reg_FLSEC;
    volatile uint8_t     reg_FSSET;

    reg_FLSEC = R_FCL_ReadMemoryU16 (FCB_REG_ADDR_FLSEC_U16);
    /* Extract BTFLG bit and invert value */
    reg_FLSEC &= FCB_REG_BIT_FLSEC_BTFLG;
    reg_FLSEC ^= FCB_REG_BIT_FLSEC_BTFLG;

    /*MS reg_FSSET = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSET_U08); */
    reg_FSSET =  *( (volatile uint8_t *)(FCB_REG_ADDR_FSSET_U08) );

    /* Update the FCB_REG_BIT_FSSET_TMSPMD only once */
    if (FCB_REG_BIT_FSSET_TMSPMD != (reg_FSSET & FCB_REG_BIT_FSSET_TMSPMD))
    {
        /* In order to modify the temporary start-up program swap state, 
           the bits EEEMD or FLSPM have to be set */
        R_FCL_CheckSetBitEEEMD();

        /* Calculate new value for temporary start-up program swap state*/
        new_val = ((uint8_t)(reg_FLSEC >> 2)) & FCB_REG_BIT_FSSET_TMBTSEL;

        /* Reset temporary start-up program swap state */
        reg_FSSET &= (~FCB_REG_BIT_FSSET_TMBTSEL);
        reg_FSSET |= new_val;
        
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSET_U08, reg_FSSET);
        reg_FSSET = R_FCL_ReadMemoryU08 (FCB_REG_ADDR_FSSET_U08);
        reg_FSSET |= FCB_REG_BIT_FSSET_TMSPMD;
        R_FCL_WriteMemoryU08 (FCB_REG_ADDR_FSSET_U08, reg_FSSET);

        /* reset the EEEMD bit after operation finished */
        R_FCL_CheckResetBitEEEMD();
    }
}
/**********************************************************************************************************************
 End of function R_FCL_InitSwapStateFlag  
 *********************************************************************************************************************/
