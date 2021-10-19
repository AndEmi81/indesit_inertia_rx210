#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/**********************************************************************************************************************
* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
*
* File Name     : $Source: r_fcl_env.h $
* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
* Mod. Revision : $Revision: 1.7 $
* Mod. Date     : $Date: 2013/06/25 11:00:16MESZ $
* Device(s)     : RX100 MF3 Flash based microcontrollers
* Description   : Flash programming hardware related definitions
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

#ifndef R_FCL_ENV_H
#define R_FCL_ENV_H


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Flash P/E mode entry register */
#define RX_SFR_REG_ADDR_FENTRYR_U16             (0x007FFFB2uL)

/* ROM P/E mode entry bit */
#define RX_SFR_REG_BIT_FENTRYR_FENTRYI          (0x0001u)
#define RX_SFR_REG_BIT_FENTRYR_KEYCODE          (0xAA00u)

/*********************************************************************************************************************/
/* FCB registers */

/* Flash programming mode control register */ 
#define FCB_REG_ADDR_FLPMC_U08                  (0x007FFF80uL)
/* bits of FLPMC */
#define FCB_REG_BIT_FLPMC_CWEE                  (0x80u)
#define FCB_REG_BIT_FLPMC_LOWSPY                (0x40u)
#define FCB_REG_BIT_FLPMC_EEEMD                 (0x10u)
#define FCB_REG_BIT_FLPMC_FWEDIS                (0x08u)
#define FCB_REG_BIT_FLPMC_FLSPM                 (0x02u)

/* Flash area selection register */
#define FCB_REG_ADDR_FLARS_U08                  (0x007FFF81uL)
/* Area selection bit */
#define FCB_REG_FLARS_USER_AREA                 (0x00u)
#define FCB_REG_BIT_FLARS_EXA                   (0x01u)

/* Flash address pointer */
#define FCB_REG_ADDR_FLAPL_U16                  (0x007FFF82uL)
#define FCB_REG_ADDR_FLAPH_U08                  (0x007FFF84uL)

/* Flash end address pointer */
#define FCB_REG_ADDR_FLSEDL_U16                 (0x007FFF86uL)
#define FCB_REG_ADDR_FLSEDH_U08                 (0x007FFF88uL)

/* Flash initialization register */
#define FCB_REG_ADDR_FLRST_U08                  (0x007FFF89uL)
#define FCB_REG_BIT_FLRST_FLRST                 (0x01u)

/* Flash sequencer status registers */
#define FCB_REG_ADDR_FSASTL_U08                 (0x007FFF8AuL)
#define FCB_REG_ADDR_FSASTH_U08                 (0x007FFF8BuL)
/* bits */
#define FCB_REG_BIT_FSASTH_ESQEND               (0x80u)
#define FCB_REG_BIT_FSASTH_SQEND                (0x40u)
#define FCB_REG_BIT_FSASTH_DRREQ                (0x02u)
#define FCB_REG_BIT_FSASTL_MBTSEL               (0x80u)
#define FCB_REG_BIT_FSASTL_MOPEN                (0x40u)
#define FCB_REG_BIT_FSASTL_ESEQER               (0x20u)
#define FCB_REG_BIT_FSASTL_SEQER                (0x10u)
#define FCB_REG_BIT_FSASTL_BLER                 (0x08u)
#define FCB_REG_BIT_FSASTL_IVER                 (0x04u)
#define FCB_REG_BIT_FSASTL_WRER                 (0x02u)
#define FCB_REG_BIT_FSASTL_ERER                 (0x01u)
#define FCB_REG_BITS_FSASTL_FLASH_OK            (0x00u) 
#define FCB_REG_MASK_FSASTL_ERR                 (0x3Fu)

/* Flash write buffer registers */
#define FCB_REG_ADDR_FLWL_U16                   (0x007FFF8CuL)
#define FCB_REG_ADDR_FLWH_U16                   (0x007FFF8EuL)

/* Flash security flag monitor register */
#define FCB_REG_ADDR_FLSEC_U16                  (0x007FC0B0uL)
#define FCB_REG_BIT_FLSEC_BTFLG                 (0x0100u)

/* AW monitor register start address */
#define FCB_REG_ADDR_FLFSWS_U16                 (0x007FC0B2uL)

/* AW monitor register end address */
#define FCB_REG_ADDR_FLFSWE_U16                 (0x007FC0B4uL)

/* Flash sequencer initialization register*/
#define FCB_REG_ADDR_FSSET_U08                  (0x007FC0B6uL)
/* bits */
#define FCB_REG_BIT_FSSET_TMSPMD                (0x80u)
#define FCB_REG_BIT_FSSET_TMBTSEL               (0x40u)
#define FCB_REG_MASK_FSSET_FSET                 (0x1Fu)

/*  Flash sequencer control register */
#define FCB_REG_ADDR_FSSQ_U08                   (0x007FFF85uL)
/* bits */
#define FCB_REG_BIT_FSSQ_SQST                   (0x80u)
#define FCB_REG_BIT_FSSQ_FSSTP                  (0x40u)
#define FCB_REG_BIT_FSSQ_DSETEN                 (0x10u)
#define FCB_REG_BIT_FSSQ_CMD_WRITE              (0x01u)
#define FCB_REG_BIT_FSSQ_CMD_BC                 (0x0Bu) /* BC mode + MRG12 level */
#define FCB_REG_BIT_FSSQ_CMD_ERASE              (0x04u)
#define FCB_REG_BIT_FSSQ_CMD_READ               (0x05u)

/* Sequencer control register for flash and Extra area */
#define FCB_REG_ADDR_FSSE_U08                   (0x007FC0B7uL)
/* bits */
#define FCB_REG_BIT_FSSE_ESQST                  (0x80u)
#define FCB_REG_BIT_FSSE_CMD_WRT_SECFLG         (0x01u)
#define FCB_REG_BIT_FSSE_CMD_WRT_AW             (0x02u)

/* mask used to reset the FSSQ and FSSE registers */
#define FCB_CONTROL_REG_RESET                   (0x00u)

/* Flash protecting command register */
#define FCB_REG_ADDR_PFCMD_U08                  (0x007FC0C0uL)
#define FCB_REG_PFCMD_CMD                       (0xA5u)    

/* Flash read buffer registers */
#define FCB_REG_ADDR_FLRL_U16                   (0x007FC0C2uL)
#define FCB_REG_ADDR_FLRH_U16                   (0x007FC0C4uL)

/* Product information capture register */
#define FCB_REG_ADDR_PORGA_U16                  (0x007FFFB6uL)
/* bits 2 - 0: Code ROM size */
#define FCB_REG_PORGA_ROM_SIZE_MASK             (0x0007u)
/* bits 15-14: Start-up program area size */
#define FCB_REG_MASK_PORGA_SPA_SIZE             (0xC000u)
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Other Code Flash related defines */ 
#define FCL_BLOCK_SIZE                          (0x0400u)       /* Code Flash block size in bytes */
#define FCL_WRITE_SIZE                          (0x00000004uL)  /* Write granularity */
#define FCL_CF_SIZE_MAX_LIMIT                   (0x00200000uL)  /* The maximum ROM size available : 2MBytes*/
#define FCL_MAX_BLOCK_CNT                       (FCL_CF_SIZE_MAX_LIMIT / FCL_BLOCK_SIZE)
#define FCL_CF_READ_BASE_END_ADDR               (0xFFFFFFFFuL)
#define FCL_CF_WRITE_BASE_END_ADDR              (0x0007FFFFuL)
#define FCL_ADDR_OFFSET                         (FCL_CF_READ_BASE_END_ADDR - FCL_CF_WRITE_BASE_END_ADDR)

/* CPU Frequency ranges: */
#define FCL_FCB_CLK_MIN_MHz                     (1u)
#define FCL_FCB_CLK_IN_HSOM_MAX_MHz             (32u) /* max frequency in high speed operating mode   */
#define FCL_FCB_CLK_IN_MSOM_MAX_MHz             (12u) /* max frequency in middle speed operating mode */

/* Mask used to determine the high speed operating mode */
#define REG_OPCCR_MASK_MODE_HIGH_SPEED          (0x00u)

/* Waiting times:*/
#define WAIT_TDIS_TIME_Us                       (2u)  
#define WAIT_TMS_TIME_HS_Us                     (5u) /* tMS for high speed operating mode */
#define WAIT_TMS_TIME_MS_Us                     (3u) /* tMS for middle speed operating mode */
#define WAIT_TSTOP_TIME_HS_Us                   (5u) /* tSTOP for high speed operating mode */
#define WAIT_TSTOP_TIME_MS_Us                   (1u) /* tSTOP for middle speed operating mode */

#endif /* ifndef R_FCL_ENV_H */
