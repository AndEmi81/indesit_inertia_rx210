#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_user_if.c $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.16 $
* Mod. Date     : $Date: 2013/08/05 17:01:19MESZ $
* Device(s)     : MF3 Flash based RX100 microcontroller
* Description   : FCL user interface functions
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
* MISRA Rule:   MISRA-C 2004 rule 14.1 (QAC message 1503)
* Reason:       This function is not used within the library, it is only used by the user application.
* Verification: This function is part of the library API and therefore essential on the one hand. On the other hand 
*               the QAC warning is correct, because this function is never called within the library. To solve this 
*               dillemma it is neccessary and suitable to disable the message.
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
Exported global variables (to be accessed by other files) 
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_DATA"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section B R_FCL_DATA 
#endif
/**********************************************************************************************************************
* MISRA Rule:   QAC message 2022
* Reason:       These variables are used for the library management. Initialization will be  made separately in the 
*               function R_FCL_Init.
* Verification: Initialization is made separately in function R_FCL_Init, which should be called before any other 
*               library operation is started.  
**********************************************************************************************************************/
r_fcl_internal_t    g_fclInternal_str;                                                             /* PRQA S 2022 */
fcl_fct_ptr_t       g_fclRomRamInterfacePtr_fp;                                                    /* PRQA S 2022 */
/**********************************************************************************************************************
Private global variables and functions 
**********************************************************************************************************************/
FCL_STATIC r_fcl_status_t R_FCL_HandleMultiOperation ( void ) GCC_SECTION(".PRAM");
FCL_STATIC r_fcl_status_t R_FCL_OperationStatusCheck ( void ) GCC_SECTION(".PRAM");
FCL_STATIC r_fcl_status_t R_FCL_ROM_RAM_Interface( const uint32_t start_addr_u32,
                                                   const uint32_t end_addr_u32,
                                                   const uint8_t command_u08 ) GCC_SECTION(".PRAM");
FCL_STATIC rBool          R_FCL_ParamCheckOk ( const uint32_t start_addr_u32, 
                                               const uint32_t length_u32 ) GCC_SECTION(".PRAM");

/**********************************************************************************************************************
 * Helper functions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_StartOperation 
 *********************************************************************************************************************/
/**
 * Helper function that switches the FCB to programing\erasing mode and depending on the operation type,
 * will start the coresponding operation (for extra area or for normal flash)
 *
 * @param         start_addr_u32 - Start address (start block address for Erase/AW, 1st address for BC/Write/Read) 
 * @param         end_addr_u32   - End address (of the 1st block address for Erase, last address for BC/Read,
 *                                 data value for Write, end block address for AW)
 * @param         command_u08    - Type of command: Erase,BC,Read,Write,AW, change start-up program swap flag   
 *
 * @return        The operation status:
 * @li            R_FCL_BUSY:           Operation started
 * @li            R_FCL_ERR_PROTECTION: Protection in case of low speed operating mode
 * @li            In Internal mode:     See the R_FCL_OperationStatusCheck return values
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
r_fcl_status_t R_FCL_StartOperation( const uint32_t start_addr_u32,                                /* PRQA S 1505 */
                                      const uint32_t end_addr_u32,
                                      const uint8_t  command_u08 )
{
    volatile r_fcl_status_t    status;

    /* switch FCB to programming/erase mode */
    status = R_FCL_FCB_SwitchMode (R_FCL_FCB_MODE_PE);

    if (R_FCL_OK == status)
    {
        if (FCL_EXTRA_AREA_OP == g_fclInternal_str.fclOpType_u08)
        {
            R_FCL_FCB_StartExtraAreaOperation (start_addr_u32, end_addr_u32, command_u08);
        }
        else
        {
            R_FCL_FCB_StartFlashOperation (start_addr_u32, end_addr_u32, command_u08);
        }

        status = R_FCL_BUSY;

        #if (FCL_STATUS_CHECK_INTERNAL == FCL_STATUS_CHECK)
            while (R_FCL_BUSY == status) 
            { 
                status = R_FCL_OperationStatusCheck(); 
            }
        #endif
    }

    return status;
} 
/**********************************************************************************************************************
 * End of function R_FCL_StartOperation  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_HandleMultiOperation 
 *********************************************************************************************************************/
/**
 *  Part of the R_FCL_OperationStatusCheck function. 
 *  In case of multi-operations (erase multiple blocks or write/read multiple words), after end of a single Flash 
 *  erase/write/read Flash operation, the next operation is started. 
 *
 * @param         ---
 *
 * @return        The multi-operation status:
 * @li            R_FCL_OK:    No multi-operation needed
 * @li            R_FCL_BUSY:  Multi-operation started 
 */
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 17.4 (QAC message 0489)
* Reason:       Pointer arithmetic is needed to calculate the next address to be written or read.
* Verification: Out of boundaries checks are made during the testing phase.
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC r_fcl_status_t R_FCL_HandleMultiOperation ( void )
{
    r_fcl_status_t    status = R_FCL_OK;
    uint32_t          end_block_addr;

    if (FCB_REG_BIT_FSSQ_CMD_ERASE == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
    {
        end_block_addr = ((g_fclInternal_str.multiOp_str).srcAddr_u32 + FCL_BLOCK_SIZE) - 1;

        /* Check if any blocks left to erase */
        if (end_block_addr != (g_fclInternal_str.multiOp_str).srcAddrEnd_u32)
        {
            /* Next block to erase */
            (g_fclInternal_str.multiOp_str).srcAddr_u32 -= FCL_BLOCK_SIZE;
            R_FCL_FCB_StartFlashOperation( (g_fclInternal_str.multiOp_str).srcAddr_u32,
                                           (((g_fclInternal_str.multiOp_str).srcAddr_u32 + FCL_BLOCK_SIZE) - 1),
                                           (g_fclInternal_str.multiOp_str).fcbCmd_u08);

            status = R_FCL_BUSY;
        }
    } 
    else if (FCB_REG_BIT_FSSQ_CMD_WRITE == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
    {
        /* Check if any words left for writting */
        if (((g_fclInternal_str.multiOp_str).srcAddr_u32 - 1) != (g_fclInternal_str.multiOp_str).srcAddrEnd_u32)
        {
            /* Write */
            R_FCL_FCB_StartFlashOperation( (g_fclInternal_str.multiOp_str).srcAddr_u32,
                                           *((g_fclInternal_str.multiOp_str).dstAddr_pu32),
                                           (g_fclInternal_str.multiOp_str).fcbCmd_u08);
            status = R_FCL_BUSY;

            (g_fclInternal_str.multiOp_str).srcAddr_u32 += FCL_WRITE_SIZE;
            (g_fclInternal_str.multiOp_str).dstAddr_pu32++;                                        /* PRQA S 0489 */
        }
    } 
    else if (FCB_REG_BIT_FSSQ_CMD_READ == (g_fclInternal_str.multiOp_str).fcbCmd_u08)
    {
        if ((g_fclInternal_str.multiOp_str).srcAddr_u32 < (g_fclInternal_str.multiOp_str).srcAddrEnd_u32)
        {
            R_FCL_FCB_ReadOperation();
            status = R_FCL_BUSY;
            (g_fclInternal_str.multiOp_str).srcAddr_u32 += FCL_WRITE_SIZE;
            (g_fclInternal_str.multiOp_str).dstAddr_pu32++;                                        /* PRQA S 0489 */
        }
    }
    else
    {
        /* Nothing to do for operations: BC/AW/change start-up program swap flag */
    }

    return status;
}
/**********************************************************************************************************************
 * End of function R_FCL_HandleMultiOperation  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ROM_RAM_Interface 
 *********************************************************************************************************************/
/**
 * Helper function called by each operation.
 * Depending on the status check mode, this function determines if the start operation code will be executed 
 * from ROM or RAM. 
 *
 * @param         start_addr_u32:     Start address (start block address for Erase/AW, 1st address for  BC/Write/Read)
 * @param         end_addr_u32:       End address (of the 1st block address for Erase, last address for BC/Read,
 *                                    data value for Write, end block address for AW)
 * @param         command_u08:        Type of command: Erase,BC,Read,Write,AW, change start-up program swap flag
 * 
 * @return        The operation status:
 * @li            R_FCL_BUSY:           Operation started
 * @li            R_FCL_ERR_PROTECTION: Protection in case of low speed operating mode
 * @li            In Internal mode:     See the R_FCL_OperationStatusCheck return values
 */  
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 11.3 (QAC message 0305)
* Reason:       The converted addresses are essential for correct code execution.(QAC message 0305)
* Verification: Incorrect conversion would result in test fails.(QAC message 0305)
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
FCL_STATIC r_fcl_status_t R_FCL_ROM_RAM_Interface (const uint32_t start_addr_u32,
                                                   const uint32_t end_addr_u32,
                                                   const uint8_t  command_u08 )
{
    r_fcl_status_t    status;

    if (FCL_FCT_RAM_POINTER_UNDEF == g_fclRomRamInterfacePtr_fp)                                   /* PRQA S 0305 */
    {
        status = R_FCL_StartOperation(start_addr_u32, end_addr_u32, command_u08);
    }
    else 
    { 
        status = g_fclRomRamInterfacePtr_fp(start_addr_u32, end_addr_u32, command_u08);
    } 

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_ROM_RAM_Interface  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_OperationStatusCheck 
 *********************************************************************************************************************/
/**  
 * Helper function: 
 * This function forwards an initiated Flash operation until it is finished.
 * It checks the FCB status, handles the result operation and multi-operations (Write/Erase/Read).
 *
 * @param          ---
 *
 * @return         Operation status:
 * @li             R_FCL_OK:             Operation terminated successfully 
 * @li             R_FCL_BUSY:           Operation is still ongoing 
 * @li             R_FCL_ERR_PARAMETER:  Wrong parameter 
 * @li             R_FCL_ERR_PROTECTION: Operation forbidden
 * @li             R_FCL_ERR_REJECTED:   Flow error 
 * @li             R_FCL_ERR_WRITE:      Flash write error
 * @li             R_FCL_ERR_ERASE:      Flash erase error
 * @li             R_FCL_ERR_BLANKCHECK: Flash blank check error
 * @li             R_FCL_ERR_INTERNAL    Library internal error
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_RAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_RAM
#endif
FCL_STATIC r_fcl_status_t R_FCL_OperationStatusCheck ( void )
{
    r_fcl_status_t    status = R_FCL_BUSY;
    r_fcl_flag_t      fcb_ready;

    /* Operation possible only in Idle or Busy state */
    if (  (R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu)
       || (R_FCL_LIB_ISTATE_BUSY == g_fclInternal_str.fclInternalState_enu)
       )
    {
        fcb_ready = R_FCL_FCB_CheckReady();
        if (R_FCL_TRUE == fcb_ready)
        {
            R_FCL_FCB_ClearControlReg();
            status = R_FCL_FCB_GetOpStatus();
            if (R_FCL_OK == status)
            {
                status = R_FCL_HandleMultiOperation();
            }
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    if (R_FCL_BUSY != status)
    {
        /* Operation finished */
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_IDLE;
        g_fclInternal_str.fclOpType_u08 = FCL_NO_OPERATION;
        /* switch FCB to user mode */
        (void)R_FCL_FCB_SwitchMode (R_FCL_FCB_MODE_USER);
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_OperationStatusCheck  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ParamCheckOk 
 *********************************************************************************************************************/
/**
 * Helper function: 
 * It does parameter checks: starting address outside of range, arithmetical address overflow and granularity
 * check. 
 * Called by R_FCL_Write, R_FCL_Read and R_FCL_BlankCheck. 
 *
 * @param         start_addr_u32:      Start address of the range to be checked
 * @param         length_u32:          Number of words that must be checked
 *
 * @return        Operation status:
 * @li            true:                Operation finished successfully, parameters are ok
 * @li            false:               Start address outside of range,
 *                                     Arithmetical address overflow,
 *                                     Wrong address granularity 
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
FCL_STATIC rBool R_FCL_ParamCheckOk( const uint32_t start_addr_u32, const uint32_t length_u32 )
{
    rBool       param_ok;
    uint32_t    max_no_blocks;
    uint32_t    cf_start_addr;
    uint32_t    end_addr;

    param_ok      = true;
    max_no_blocks = R_FCL_GetBlockCnt();
    cf_start_addr = (FCL_CF_READ_BASE_END_ADDR - (max_no_blocks * FCL_BLOCK_SIZE)) + 1;
    end_addr      = (start_addr_u32 + (length_u32 * FCL_WRITE_SIZE)) - 1;

    if (  (start_addr_u32 < cf_start_addr)                                /* Start address outside of range */
       || (start_addr_u32 > (FCL_CF_READ_BASE_END_ADDR - FCL_WRITE_SIZE)) /* Start address outside of range */
       || (end_addr <= start_addr_u32)                                    /* Arithmetical address overflow  */
       || (0x00000000uL != (start_addr_u32 & (FCL_WRITE_SIZE - 1)))       /* Address granularity */
        )
    {
        param_ok = false;
    }
    
    return param_ok;
}
/**********************************************************************************************************************
 End of function R_FCL_ParamCheckOk  
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Normal operations
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_Erase 
 *********************************************************************************************************************/
/**
 * User interface function: 
 * Erases a range of blocks.
 * It does parameter checks, extracts information required for the Erase operation, prepare for multi-operation 
 * and calls the function which starts the Erase operation.
 * Block numbering is in the opposite order as the address numbering 
 *
 * @param         block_no_start_u32      - First block number to be erased
 * @param         block_no_end_u32        - Last block number to be erased
 *
 * @return        Operation status:
 * @li            R_FCL_OK:             Operation finished successfully
 * @li            R_FCL_BUSY:           Operation started successfully
 * @li            R_FCL_ERR_REJECTED:   Library not initialized, frequency not set, other command on-going
 * @li            R_FCL_ERR_PARAMETER:  Erase block range is not as expected
 * @li            R_FCL_ERR_ERASE:      Flash block could not be erased
 * @li            R_FCL_ERR_PROTECTION: Block protection by Access Window 
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_Erase ( const uint32_t block_no_start_u32, const uint32_t block_no_end_u32 )  /* PRQA S 1503 */
{
    uint32_t          max_no_blocks;
    uint32_t          start_addr;
    uint32_t          end_addr;
    r_fcl_status_t    status;
    rBool             df_access;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        max_no_blocks = R_FCL_GetBlockCnt();

        /* Check Range and parameters */
        if ( (block_no_start_u32 <= block_no_end_u32) && (block_no_end_u32 < max_no_blocks) )
        {
            /* Calculate start and end erase addresses */ 
            start_addr = (FCL_CF_WRITE_BASE_END_ADDR - ((block_no_start_u32 + 1) * FCL_BLOCK_SIZE)) + 1;
            end_addr   = FCL_CF_WRITE_BASE_END_ADDR - (block_no_end_u32 * FCL_BLOCK_SIZE);
            
            g_fclInternal_str.fclOpType_u08 = FCL_FLASH_OP;

            /* Prepare for multi-block erase */
            (g_fclInternal_str.multiOp_str).srcAddr_u32    = start_addr;
            (g_fclInternal_str.multiOp_str).srcAddrEnd_u32 = end_addr;
            (g_fclInternal_str.multiOp_str).fcbCmd_u08     = FCB_REG_BIT_FSSQ_CMD_ERASE;

            status = R_FCL_ROM_RAM_Interface( start_addr,
                                             ((start_addr + FCL_BLOCK_SIZE) - 1), /* end address */
                                             FCB_REG_BIT_FSSQ_CMD_ERASE );
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    /* Library state change */
    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_Erase  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_Write 
 *********************************************************************************************************************/
/**  
 * User interface function: 
 * Writes the specified number of words from a buffer to consecutive Flash addresses 
 * starting at the specified address.  
 *
 * @param[in]      src_addr_pu32:         Pointer to data buffer to be written
 * @param          dest_addr_u32:         Destination address of data to be written
 * @param          length_u32:            Number of words that must be written
 *
 * @return         Operation status:
 * @li             R_FCL_OK:              Operation finished successfully
 * @li             R_FCL_BUSY:            Operation started successfully
 * @li             R_FCL_ERR_PARAMETER:   Parameter checks, alignment to granularity ...
 * @li             R_FCL_ERR_REJECTED:    Library not initialized, frequency not set, other command on-going
 * @li             R_FCL_ERR_PROTECTION:  Protection by Access Window
 * @li             R_FCL_ERR_WRITE:       A flash word could not be written 
 */
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 17.4 (QAC message 0489)
* Reason:       Pointer arithmetic is needed to calculate the next address to be written.
* Verification: Out of boundaries checks are made during the testing phase.
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_Write ( uint32_t *     src_addr_pu32,                                         /* PRQA S 1503 */ 
                             const uint32_t dest_addr_u32, 
                             const uint32_t length_u32 )
{
    r_fcl_status_t    status;
    rBool             df_access;
    rBool             param_ok;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        param_ok = R_FCL_ParamCheckOk (dest_addr_u32, length_u32);

        /* Parameter checks*/
        if (  (false == param_ok)
           || (FCL_NULL == src_addr_pu32)
           )
        {
            status = R_FCL_ERR_PARAMETER;
        }
        else
        {
            g_fclInternal_str.fclOpType_u08 = FCL_FLASH_OP;
        
            /* Prepare for multi-word write */ 
            /* Next write address */
            (g_fclInternal_str.multiOp_str).srcAddr_u32    = (dest_addr_u32 - FCL_ADDR_OFFSET) + FCL_WRITE_SIZE;
            /* Write end address  */
            (g_fclInternal_str.multiOp_str).srcAddrEnd_u32 = ((dest_addr_u32 + (length_u32 * FCL_WRITE_SIZE)) - 1)
                                                             - FCL_ADDR_OFFSET;
            /* Next addr to be written*/
            /* PRQA S 0489 1*/
            (g_fclInternal_str.multiOp_str).dstAddr_pu32   = src_addr_pu32 + 1;
            /* Command */
            (g_fclInternal_str.multiOp_str).fcbCmd_u08     = FCB_REG_BIT_FSSQ_CMD_WRITE;

            status = R_FCL_ROM_RAM_Interface ( (dest_addr_u32 - FCL_ADDR_OFFSET), 
                                               *src_addr_pu32, 
                                               FCB_REG_BIT_FSSQ_CMD_WRITE );
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    /* Library state change */
    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_Write  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_BlankCheck 
 *********************************************************************************************************************/
/**  
 * User interface function: 
 * Checks if an address range is blank
 *
 * @param          start_addr_u32:        Start address of the range to be checked
 * @param          length_u32:            Number of words that must be checked
 *
 * @return         Operation status:
 * @li             R_FCL_OK:              Operation finished successfully
 * @li             R_FCL_BUSY:            Operation started successfully
 * @li             R_FCL_ERR_PARAMETER:   Parameter checks, alignment to granularity ...
 * @li             R_FCL_ERR_REJECTED:    Library not initialized, frequency not set, other command on-going
 * @li             R_FCL_ERR_BLANKCHECK:  Flash not blank on the given address range 
 */  
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_BlankCheck ( const uint32_t start_addr_u32, const uint32_t length_u32 )       /* PRQA S 1503 */
{
    r_fcl_status_t    status;
    rBool             df_access;
    rBool             param_ok;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        param_ok = R_FCL_ParamCheckOk (start_addr_u32, length_u32);
        /* Parameter checks */
        if (false ==  param_ok)
        {
            status = R_FCL_ERR_PARAMETER;
        }
        else
        {
            g_fclInternal_str.fclOpType_u08 = FCL_FLASH_OP;
        
            /* For multi operation - do nothing */
            /* Command set only to help return the correct error, in case of failure */
            (g_fclInternal_str.multiOp_str).fcbCmd_u08 = FCB_REG_BIT_FSSQ_CMD_BC;

            status = R_FCL_ROM_RAM_Interface( (start_addr_u32 - FCL_ADDR_OFFSET), 
                                              ((start_addr_u32 + (length_u32 * FCL_WRITE_SIZE)) - 1) - FCL_ADDR_OFFSET, 
                                              FCB_REG_BIT_FSSQ_CMD_BC );
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    /* Library state change */
    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_BlankCheck  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_Read 
 *********************************************************************************************************************/
/**
 * User interface function: 
 * Reads the specified number of words from consecutive Flash addresses starting at the specified address.  
 *
 * @param          start_addr_u32:        Source address of data to be read
 * @param[in]      dest_addr_pu32:        Pointer to buffer of read data
 * @param          length_u32:            Number of words that must be read out
 *
 * @return         Operation status:
 * @li             R_FCL_OK:              Operation finished successfully
 * @li             R_FCL_BUSY:            Operation started successfully
 * @li             R_FCL_ERR_PARAMETER:   Parameter checks, alignment to granularity ...
 * @li             R_FCL_ERR_REJECTED:    Library not initialized, frequency not set, other command on-going
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_Read ( const uint32_t start_addr_u32,                                         /* PRQA S 1503 */
                            uint32_t *     dest_addr_pu32, 
                            const uint32_t length_u32 )
{
    r_fcl_status_t    status;
    rBool             df_access;
    rBool             param_ok;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        param_ok = R_FCL_ParamCheckOk (start_addr_u32, length_u32);
        /* Parameter checks */
        if (  (false == param_ok)
           || (FCL_NULL == dest_addr_pu32)
           )
        {
            status = R_FCL_ERR_PARAMETER;
        }
        else
        {
            g_fclInternal_str.fclOpType_u08 = FCL_FLASH_OP;
            
            /* Prepare for multi-word read */
            (g_fclInternal_str.multiOp_str).srcAddr_u32    = start_addr_u32 - FCL_ADDR_OFFSET;
            (g_fclInternal_str.multiOp_str).srcAddrEnd_u32 = ((start_addr_u32 + (length_u32 * FCL_WRITE_SIZE)) - 1)
                                                             - FCL_ADDR_OFFSET;
            (g_fclInternal_str.multiOp_str).dstAddr_pu32   = dest_addr_pu32;
            (g_fclInternal_str.multiOp_str).fcbCmd_u08     = FCB_REG_BIT_FSSQ_CMD_READ;

            status = R_FCL_ROM_RAM_Interface( start_addr_u32 - FCL_ADDR_OFFSET,
                                              ((start_addr_u32 + (length_u32 * FCL_WRITE_SIZE)) - 1) - FCL_ADDR_OFFSET,
                                              FCB_REG_BIT_FSSQ_CMD_READ );
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_Read  
 *********************************************************************************************************************/

#if (FCL_STATUS_CHECK == FCL_STATUS_CHECK_USER)
/**********************************************************************************************************************
 * Function name: R_FCL_StatusCheck 
 *********************************************************************************************************************/
/**
 * User interface function: 
 * Forwards an initiated Flash operation until it is finished.
 * It checks the FCB status, handles the result operation and multi-operations (Write/Erase/Read).
 * After the operation is started, this function must be called regularly to finish the operation. 
 *
 * @param          ---
 *
 * @return         Operation status:
 * @li             R_FCL_OK:             Operation terminated successfully 
 * @li             R_FCL_BUSY:           Operation is still ongoing 
 * @li             R_FCL_ERR_PARAMETER:  Wrong parameter 
 * @li             R_FCL_ERR_PROTECTION: Operation forbidden
 * @li             R_FCL_ERR_REJECTED:   Flow error 
 * @li             R_FCL_ERR_WRITE:      Flash write error
 * @li             R_FCL_ERR_ERASE:      Flash erase error
 * @li             R_FCL_ERR_BLANKCHECK: Flash blank check error
 * @li             R_FCL_ERR_INTERNAL    Library internal error
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_StatusCheck ( void )                                                          /* PRQA S 1503 */
{
    r_fcl_status_t    status;

    status = R_FCL_OperationStatusCheck();

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_StatusCheck  
 *********************************************************************************************************************/
#endif


/**********************************************************************************************************************
 * Administration operations
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_GetVersionString 
 *********************************************************************************************************************/
/**
 * User interface function: 
 * Returns the pointer to the library version string, stored in the library code section. 
 *
 * @param      ---
 * 
 * @return     pointer to the version string in Flash 
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section C R_FCL_CONST
    #pragma section P R_FCL_CODE_ROM
#endif
const uint8_t * R_FCL_GetVersionString ( void )                                                    /* PRQA S 1503 */
{

    #if FCL_LIB_COMPILER == FCL_LIB_COMP_IAR 
        static const uint8_t fcl_version_str[] @ "R_FCL_CONST" = FCL_VERSION_STRING;
    #else
        static const uint8_t fcl_version_str[] GCC_SECTION("R_FCL_CONST") = FCL_VERSION_STRING;
    #endif

    return (&fcl_version_str[0]);

}
/**********************************************************************************************************************
 End of function R_FCL_GetVersionString  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetBlockCnt 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Calculates the device number of blocks.
 * Block numbering is in the opposite order as the address numbering
 *
 * @param         ---
 * 
 * @return        Device number of blocks
 */
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 8.10 (QAC message 1505)
* Reason:       This function is not only used within the library, but it is used by the user application also.
* Verification: This function is part of the library API and therefore essential on the one hand. On the other hand 
*               the QAC warning is correct, because this function is called  once within the library. To solve this 
*               dillemma it is neccessary and suitable to disable the message.
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint32_t R_FCL_GetBlockCnt ( void )                                                                /* PRQA S 1505 */
{
    uint16_t    size;
    uint32_t    block_cnt;

    block_cnt = FCL_MAX_BLOCK_CNT;
    size      = R_FCL_GetCodeFlashSize();
    block_cnt = block_cnt >> size;

    return block_cnt;
}
/**********************************************************************************************************************
 End of function R_FCL_GetBlockCnt  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetBlockStartAddr 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Returns the starting address of the specified block.
 * Block numbering is in the opposite order as the address numbering
 *
 * @param      block_no_u32:           Block number
 * @param[out] block_start_addr_pu32:  Pointer to buffer for block starting address information
 *
 * @return     Operation status:
 * @li         R_FCL_OK:             Operation terminated successfully
 * @li         R_FCL_ERR_PARAMETER:  Block outside of the range, invalid RAM address
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_GetBlockStartAddr ( const uint32_t   block_no_u32,                            /* PRQA S 1503 */
                                         uint32_t * const block_start_addr_pu32 )
{
    uint32_t          max_no_blocks;
    r_fcl_status_t    status = R_FCL_OK;

    if (FCL_NULL != block_start_addr_pu32)
    {
        max_no_blocks = R_FCL_GetBlockCnt();
        if (block_no_u32 < max_no_blocks)
        {
            *block_start_addr_pu32 = (FCL_CF_READ_BASE_END_ADDR - ((block_no_u32 + 1) * FCL_BLOCK_SIZE)) + 1;
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_PARAMETER;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_GetBlockStartAddr  
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Function name: R_FCL_GetSwapState 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Returns the current start-up program swap state, no matter if a temporary or a permanent swap 
 * was performed or not.
 *
 * @param[out]    sps_stat_enu: Current start-up program swap status
 * @li            R_FCL_NO_SP_SWAP: Start-up program swap was not performed
 * @li            R_FCL_SP_SWAPPED: Either a temporary or a permanent start-up program swap was performed
 *
 * @return        Current operation status: 
 * @li            R_FCL_OK: Operation finished succesfully
 * @li            R_FCL_ERR_REJECTED: on-going command
 * @li            R_FCL_ERR_PARAMETER: invalid address is used 
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_GetSwapState (r_fcl_sps_status_t * const sps_stat_enu)                        /* PRQA S 1503 */
{
    r_fcl_status_t    status = R_FCL_OK;
    
    if (  (R_FCL_LIB_ISTATE_INIT == g_fclInternal_str.fclInternalState_enu)
       || (R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu)
       )
    {
        if (FCL_NULL != sps_stat_enu)
        {
            *sps_stat_enu = R_FCL_StartupProgramSwapState();
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_GetSwapState  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ChangeSwapState 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Swaps the start-up program area 0 and  start-up program area 1 physically without setting the start-up program 
 * swap flag. After reset, the the start-up program area will be activated regarding start-up program swap flag. 
 *
 * @param         ---
 *
 * @return        Operation status:
 * @li            R_FCL_OK:           Operation terminated successfully
 * @li            R_FCL_ERR_REJECTED: Library not initialized, frequency not set, other command on-going
 */
/*********************************************************************************************************************/
/**********************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 8.10 (QAC message 1505)
* Reason:       This function is not only used within the library, but it is used by the user application also.
* Verification: This function is part of the library API and therefore essential on the one hand. On the other hand 
*               the QAC warning is correct, because this function is called  once within the library. To solve this 
*               dillemma it is neccessary and suitable to disable the message.
**********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_ChangeSwapState ( void )                                                      /* PRQA S 1505 */
{
    r_fcl_status_t    status = R_FCL_OK;
    rBool             df_access;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        R_FCL_TempSwapChange();
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_ChangeSwapState  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetSwapFlag 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Returns the current value of the start-up program swap flag, which represents the permanent start-up 
 * program swap status. 
 *
 * @param[out]    sps_flag_status_penu: Current start-up program swap flag value
 * @li            R_FCL_NO_SPS_FLAG_SWAP: No permanent swap performed
 * @li            R_FCL_SPS_FLAG_SWAPPED: Permanent start-up program swap performed after reset
 *
 * @return        Current operation status: 
 * @li            R_FCL_OK: Operation finished succesfully
 * @li            R_FCL_ERR_REJECTED: on-going command
 * @li            R_FCL_ERR_PARAMETER: invalid address is used 
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_GetSwapFlag( r_fcl_sps_flag_status_t * const sps_flag_status_penu )           /* PRQA S 1503 */
{
    r_fcl_status_t    status = R_FCL_OK;
    
    if (R_FCL_LIB_ISTATE_BUSY != g_fclInternal_str.fclInternalState_enu)
    {
        if (FCL_NULL != sps_flag_status_penu)
        {
            *sps_flag_status_penu = R_FCL_GetStartupProgramSwapFlag();
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_GetSwapFlag  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_ChangeSwapFlag 
 *********************************************************************************************************************/
/**
 * User interface function:
 * The function inverts the start-up program swap flag in the extra area. 
 * This flag controls the start-up program swap state after reset. 
 * Depending on the input parameter, an immediate swap may be performed. 
 *
 * @param         immediate_sp_swap_u08:   Swap start-up program areas immediatelly or not
 *                                         0x00 - Start-up program swap will be performed after reset
 *                                         0x01 - Start-up program swap will be performed immediately
 *
 * @return        Operation status:
 * @li            R_FCL_OK:           Operation terminated successfully
 * @li            R_FCL_BUSY:         Operation started
 * @li            R_FCL_ERR_REJECTED: Library not initialized, frequency not set, other command on-going
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_ChangeSwapFlag ( const uint8_t immediate_sp_swap_u08 )                        /* PRQA S 1503 */
{
    r_fcl_status_t status = R_FCL_OK;
    rBool          df_access;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        if (0x00 != immediate_sp_swap_u08)
        {
            status = R_FCL_ChangeSwapState();
        }

        if (R_FCL_OK == status)
        {
            g_fclInternal_str.fclOpType_u08 = FCL_EXTRA_AREA_OP;

            /* For multi operation - do nothing */
            (g_fclInternal_str.multiOp_str).fcbCmd_u08 = 0x00;

            status = R_FCL_ROM_RAM_Interface (0, 0, FCB_REG_BIT_FSSE_CMD_WRT_SECFLG );
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    /* Library state change */
    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_ChangeSwapFlag  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: R_FCL_GetStartupAreaSize 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Reads current size of start-up program area. 
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
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
uint8_t R_FCL_GetStartupAreaSize ( void )                                                          /* PRQA S 1503 */
{
    uint8_t spa_size;

    spa_size = R_FCL_GetSPASize();

    return spa_size;
}
/**********************************************************************************************************************
 End of function R_FCL_GetStartupAreaSize  
 *********************************************************************************************************************/



/**********************************************************************************************************************
 * Security operations
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function name: R_FCL_GetAW 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Function returns the start and end block number for the Access Window
 * Block numbering is in the opposite order as the address numbering
 *
 * @param[out] block_no_start_pu32:    Pointer to buffer for starting block number of AW
 * @param[out] block_no_end_pu32:      Pointer to buffer for ending block number of AW
 *
 * @return    Operation status:
 * @li        R_FCL_OK:                Operation finished successfully
 * @li        R_FCL_ERR_PARAMETER:     Invalid RAM address passed as parameter
 * @li        R_FCL_ERR_REJECTED:      Other command on-going
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_GetAW ( uint32_t * const block_no_start_pu32,                                 /* PRQA S 1503 */ 
                             uint32_t * const block_no_end_pu32  )
{
    r_fcl_status_t    status = R_FCL_OK;
    uint32_t          start_addr;
    uint32_t          end_addr;
    uint32_t          max_no_blocks;

    if (R_FCL_LIB_ISTATE_BUSY != g_fclInternal_str.fclInternalState_enu)
    {
        if ((FCL_NULL != block_no_start_pu32) && (FCL_NULL != block_no_end_pu32))
        {
            max_no_blocks = R_FCL_GetBlockCnt();
            start_addr    = R_FCL_GetAWStartAddr();
            end_addr      = R_FCL_GetAWEndAddr();

            /* In case the addresses are equal all blocks are programable/erasable */
            if (start_addr == end_addr)
            {
                *block_no_start_pu32 = 0;
                *block_no_end_pu32   = max_no_blocks - 1;
            }
            else
            {
                start_addr = (FCL_CF_WRITE_BASE_END_ADDR + 1) - start_addr;
                end_addr   = (FCL_CF_WRITE_BASE_END_ADDR + 1) - end_addr;

                /* Calculate block number */
                *block_no_start_pu32 = (start_addr / FCL_BLOCK_SIZE);
                *block_no_end_pu32   = (end_addr / FCL_BLOCK_SIZE) - 1;
            }
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }
    
    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_GetAW  
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name: R_FCL_SetAW 
 *********************************************************************************************************************/
/**
 * User interface function:
 * Sets the new range for Access Window
 * Block numbering is in the opposite order as the address numbering
 *
 * @param     block_no_start_u32:      Starting block number
 * @param     block_no_end_u32:        Ending block number
 *
 * @return    Operation status:
 * @li        R_FCL_OK:              Operation finished successfully
 * @li        R_FCL_BUSY:            Operation started successfully
 * @li        R_FCL_ERR_PARAMETER:   Blocks outside of the range
 * @li        R_FCL_ERR_REJECTED:    Library not initialized, frequency not set, other command on-going
 */
/*********************************************************************************************************************/
#if FCL_LIB_COMPILER == FCL_LIB_COMP_GCC
    /* Pragma is not available, but the functionality can be achieved using __attribute__ in function declaration */
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_IAR
    #pragma location="R_FCL_CODE_ROMRAM"
#elif FCL_LIB_COMPILER == FCL_LIB_COMP_REC
    #pragma section P R_FCL_CODE_ROMRAM
#endif
r_fcl_status_t R_FCL_SetAW ( const uint32_t block_no_start_u32,                                    /* PRQA S 1503 */ 
                             const uint32_t block_no_end_u32 )
{
    uint32_t          max_no_blocks;
    uint32_t          start_addr;
    uint32_t          end_addr;
    r_fcl_status_t    status;
    rBool             df_access;
    
    df_access = R_FCL_OngoingDataFlashOp();

    /* Operation possible only in Idle state and if no ongoing data flash access */
    if ((R_FCL_LIB_ISTATE_IDLE == g_fclInternal_str.fclInternalState_enu) && (false == df_access))
    {
        max_no_blocks = R_FCL_GetBlockCnt();

        /* Check Range and parameters */
        if (  (block_no_start_u32 <= block_no_end_u32)
           && (block_no_end_u32   <  max_no_blocks)
           )
        {
            g_fclInternal_str.fclOpType_u08 = FCL_EXTRA_AREA_OP;
        
            /* For multi operation - do nothing */
            (g_fclInternal_str.multiOp_str).fcbCmd_u08 = 0x00;

            /* Calculate the start and end address for AW */
            start_addr = (FCL_CF_WRITE_BASE_END_ADDR - ((block_no_end_u32 + 1) * FCL_BLOCK_SIZE)) + 1;
            end_addr   = FCL_CF_WRITE_BASE_END_ADDR - (block_no_start_u32 * FCL_BLOCK_SIZE);

            status = R_FCL_ROM_RAM_Interface (start_addr, end_addr, FCB_REG_BIT_FSSE_CMD_WRT_AW );
        }
        else
        {
            status = R_FCL_ERR_PARAMETER;
        }
    }
    else
    {
        status = R_FCL_ERR_REJECTED;
    }

    /* Library state change */
    if (R_FCL_BUSY == status)
    {
        g_fclInternal_str.fclInternalState_enu = R_FCL_LIB_ISTATE_BUSY;
    }

    return status;
}
/**********************************************************************************************************************
 End of function R_FCL_SetAW  
 *********************************************************************************************************************/
