;/*********************************************************************************************************************
;* Library       : Code Flash Access Library for Renesas RX100 devices, based on the MF3 Flash technology
;*
;* File Name     : $Source: r_fcl_basic_fct_asm.asm $
;* Lib. Version  : $RX111_FCL_LIB_VERSION_T01: V1.00 $
;* Mod. Revision : $Revision: 1.4 $
;* Mod. Date     : $Date: 2013/06/25 11:00:32MESZ $
;* Device(s)     : RX100 MF3 Flash based microcontrollers
;* Description   : Assembler code of basic functions used during self-programming
;*********************************************************************************************************************/

;/*********************************************************************************************************************
;* DISCLAIMER 
;* This software is supplied by Renesas Electronics Corporation and is only  intended for use with Renesas products. 
;* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
;* applicable laws, including copyright laws. 
;* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED 
;* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
;* NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
;* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS 
;* AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY 
;* REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH 
;* DAMAGES.
;* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
;* this software. By using this software, you agree to the additional terms and conditions found by accessing the
;* following link: 
;* http://www.renesas.com/disclaimer 
;* 
;* Copyright (C) 2012-2013 Renesas Electronics Corporation. All rights reserved.     
;*********************************************************************************************************************/

.SECTION    .PRAM, CODE,ALIGN=4
.SECTION    R_FCL_CODE_RAM_USRINT, CODE,ALIGN=4
.SECTION    R_FCL_CODE_RAM_USR,    CODE,ALIGN=4
.SECTION    R_FCL_CODE_ROMRAM,     CODE,ALIGN=4
.SECTION    R_FCL_CODE_ROM,        CODE,ALIGN=4

;**********************************************************************************************************************
; Function name: R_FCL_WaitUs 
;**********************************************************************************************************************
;     Software wait loop.
;     The function wait for x us @ the frequency configured by the parameter.
;     Note: The accuracy is rather low for low frequencies and low wait times because the loop count 
;     is an integer value of time*freq/8
;    
;     @param[in]:    time_u32:   us time to wait 
;                    freq_u08:   CPU frequency 
; 
;     @return:       --- 
;********************************************************************************************************************** 
.SECTION .PRAM,CODE
.GLB _R_FCL_WaitUs

_R_FCL_WaitUs:
;   clocks = freq*time (us)
    MUL r1, r2, r2
;   the wait loop shall last 8 clocks
    SHLR #3, r2

    _R_FCL_WaitUs_Loop:
    BZ  _R_FCL_WaitUs_End
    NOP
    NOP
    NOP
    ADD #-1, r2
    BRA _R_FCL_WaitUs_Loop

    _R_FCL_WaitUs_End:
    RTS
;**********************************************************************************************************************
; End function R_FCL_WaitUs 
;**********************************************************************************************************************

.end
