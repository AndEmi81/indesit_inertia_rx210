#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_types.h $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.8 $
* Mod. Date     : $Date: 2013/06/25 11:00:16MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : User interface type definitions
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

#ifndef R_FCL_TYPES_H
#define R_FCL_TYPES_H

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/**
   @enum r_fcl_sps_status_t 
   Start-up program swap status 
*/
typedef enum R_FCL_SPS_STATUS_T
{
    R_FCL_NO_SP_SWAP,                     /**< Start-up program swap was not performed */
    R_FCL_SP_SWAPPED                      /**< Either a temporary or a permanent start-up program swap was performed */
} r_fcl_sps_status_t;


/**
   @enum r_fcl_sps_flag_status_t 
   Start-up program swap flag state 
*/
typedef enum R_FCL_SPS_FLAG_STATUS_T
{
    R_FCL_NO_SPS_FLAG_SWAP,               /**< No permanent swap performed */
    R_FCL_SPS_FLAG_SWAPPED                /**< Permanent start-up program swap performed after reset */
} r_fcl_sps_flag_status_t;


/**
   @enum r_fcl_status_t 
   FCL status return values 
*/ 
typedef enum R_FCL_STATUS_T 
{
    R_FCL_OK,                             /**< Operation terminated successfully */ 
    R_FCL_BUSY,                           /**< Operation is still ongoing */
    R_FCL_ERR_PARAMETER,                  /**< A parameter of the FCL function call was wrong */ 
    R_FCL_ERR_PROTECTION,                 /**< Operation blocked due to wrong parameters */ 
    R_FCL_ERR_REJECTED,                   /**< Flow error, e.g. another operation is still busy */ 
    R_FCL_ERR_WRITE,                      /**< Flash write error */ 
    R_FCL_ERR_ERASE,                      /**< Flash erase error */ 
    R_FCL_ERR_BLANKCHECK,                 /**< Flash blank check error */ 
    R_FCL_ERR_INTERNAL                    /**< Library internal error */ 
} r_fcl_status_t;

#endif /* ifndef R_FCL_TYPES_H */
