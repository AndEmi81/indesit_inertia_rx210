#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* File Name     : $Source: fcl_cfg.h $
* Mod. Revision : $Revision: 1.7 $
* Mod. Date     : $Date: 2013/08/05 16:56:01MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : Header file with definitions for library setup at compile time
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

#ifndef FCL_CFG_H
#define FCL_CFG_H

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/*===========================================================================*/
/* --------------------------- DO NOT CHANGE !!! --------------------------- */
/*===========================================================================*/
#define FCL_STATUS_CHECK_INTERNAL            (1)
#define FCL_STATUS_CHECK_USER                (2)

/*===========================================================================*/
/* ----------------------- USER CHANGEABLE SECTION ------------------------- */
/*===========================================================================*/

/*------------------------------------------------------------------*/
/*   Application specific library compile options                   */
/*   Please refer to the FCL documentation for MF3 based            */
/*   microcontroller for the correct settings.                      */
/*   NOTE:                                                          */
/*   Wrong setting results in wrong or non functioning FCL          */
/*------------------------------------------------------------------*/


/* define whether the status check should be performed by */
/* the Firmware or by the user to allow execution of user */
/* code in between the status checks */
/*MS orig #define FCL_STATUS_CHECK                     FCL_STATUS_CHECK_USER */
#define FCL_STATUS_CHECK FCL_STATUS_CHECK_INTERNAL


#endif /* ifndef FCL_CFG_H */

