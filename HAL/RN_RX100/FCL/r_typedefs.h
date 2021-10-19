#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
    /* PRQA S 4600 EOF */
    /* PRQA S 4602 EOF */
    /* PRQA S 4620 EOF */
    /* PRQA S 4623 EOF */
#endif
/**********************************************************************************************************************
* Library       : Flash Libraries for Renesas devices
*
* File Name     : $Source: r_typedefs.h $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.6 $
* Mod. Date     : $Date: 2013/06/25 11:00:17MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : This file contains standard C99 type definitions used by FCL and FDL 
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
 * MISRA Rule:   QAC messages 4600, 4602, 4620, 4623
 * Reason:       This file is designed as a light replacement for headers in the standard C lib
 * Verification: The user can use either this file or the standard C library files. 
                 Used defines are the same.
**********************************************************************************************************************/

#ifndef R_TYPEDEFS_H
#define R_TYPEDEFS_H

typedef signed char         int8_t;     /**< Signed 8 bit integer values */
typedef signed short        int16_t;    /**< Signed 16 bit integer values */
typedef signed long         int32_t;    /**< Signed 32 bit integer values */

typedef unsigned char       uint8_t;    /**< Unsigned 8 bit integer values */
typedef unsigned short      uint16_t;   /**< Unsigned 16 bit integer values */
typedef unsigned long       uint32_t;   /**< Unsigned 32 bit integer values */

typedef unsigned char       rBool;      /**< Boolean value is 8 bit unsigned */
typedef unsigned char       bool;       /**< Boolean value is 8 bit unsigned */

#define INT8_MIN            (-128)
#define INT16_MIN           (-32768)
#define INT32_MIN           (-2147483647 - 1)
#define INT64_MIN           (-9223372036854775807LL - 1LL)

#define INT8_MAX            (127)
#define INT16_MAX           (32767)
#define INT32_MAX           (2147483647)
#define INT64_MAX           (9223372036854775807LL)

#define UINT8_MAX           (255)
#define UINT16_MAX          (65535)
#define UINT32_MAX          (4294967295U)
#define UINT64_MAX          (18446744073709551615ULL)

#define false               (0)
#define true                (!false)


#endif /* ifndef R_TYPEDEFS_H */
