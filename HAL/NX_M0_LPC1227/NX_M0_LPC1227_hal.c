/**
\defgroup HAL_NX_M0_LPC1227  NXP M0 LPC1227 HAL
\ingroup NX_M0_LPC1227
*/

/**
\defgroup HAL_NX_M0_LPC1227_CLS  NXP M0 LPC1227 supported drivers Classes

This module includes the driver local variables supported by this HAL.
\ File NX_M0_LPC1227_hal.c
\ ingroup HAL_NX_M0_LPC1227
\ 03/09/2011
\ author Giancarlo Parodi
\ version 00.00.01



*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    NX_M0_LPC1229_hal.h
\ingroup HAL_NX_M0_LPC1229
\date    12/03/2009
\author  Roberto Fioravanti
\version 00.00.01


\addtogroup HAL_NX_M0_LPC1229_HISTORY_HAL
@{
\n 		
\n		
\n		
\n	        00.00.01	
\n 		04/26/2011
\n		Giancarlo Parodi
\n		Changes
\n		- created file for tracking variables which are local to HAL
@}
*/

/*
Copyright (c) 2009-2010, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup HAL_NX_M0_LPC1227_CLS
@{*/



#define __DRIVER_H

/* ******** */
/* Includes */
/* ******** */
#include <stdlib.h>
#include "Config.h"                               /* For: Application configuration */
#include "NX_M0_LPC1227_cfg.h"
#include "NX_M0_LPC1227_hal.h"

//#include <stdint.h>

/* this variable is used to track the mode of the channel (input capture / output compare) */
/* 0 is output compare, 1 is input capture */ 
/* each bit is associated with a channel [bit 0 <> channel 0, bit 1 <> channel 1 etc]*/
//uint32_t _hal_channelMode = 0;

/* this variable is used to track the PWM mode of the channel */
/* 1 is PWM mode enabled */ 
/* each bit is associated with a channel [bit 0 <> channel 0, bit 1 <> channel 1 etc]*/
uint32_t _hal_pwmEnabled = 0; //TODO_LPC1227

/* this variable is used to track which channel is used as "modulo" within a specific timer */
/* on LPC1227 there is no dedicated "modulo" channel, each of the 4 available can be used */
/* the variable is used for enabling the PWM mode also for the "modulo" channel if */
/* another channel on the same counter is configured (at a later stage) in PWM mode. */
/* "PWM mode" is setup in a separated register, PWMC - pwm control register */
/* The variable tracks the selected modulo channel on the counter, assigned in OCHWCNTR_InitSync > TMR_SetModulo */
/* This info is used when TMR_SetChannelPWM is called, to make sure at this point the HAL knows which other channel */
/* on the same timer was selected for modulo operation */
/* This means that within OCPWM_InitSync > TMR_SetChannelPWM configures both the PWM channel AND the "modulo" channel in "PWM mode" */
/* each bit couple is associated with a timer [bit 1:0 <> timer 0, bit 3:2 <> timer 1 etc]*/
/* the modulo channel number is directly coded in the bitfields (00:channel 0, 01:channel 1, 10: channel 2), 11: channel 3*/
uint8_t _hal_tmrModuloChannel = 0; //TODO_LPC1227



bool_t isChannelInputCapture(uint8_t channelNum) {
  DIO_PortAddressExtendedType chFunc = TmrChToPinMap[channelNum].extAddress;

  if (channelNum < NUM_HW_CNTR_CH)
  {
   
    switch(channelNum)
    {
    case 0:
      if ( ((portreg_ext_struct_t *) chFunc)->function == 0x03)
      { 
        return TRUE;
      }
      break;
  
    case 2:
    case 3:
    case 5:
    case 6:
      break;
      
    default:    
      if ( ((portreg_ext_struct_t *) chFunc)->function == 0x02)
      { 
        return TRUE;
      }
      break;
     
    }

  }

  return FALSE;
  
}





void DIO_SetDigitalFltClkFreq(portreg_ext_struct_t *port,uint16_t filterFrequency)
{
    if ((BUS_FREQUENCY_Hz/(1000*filterFrequency*port->digitalFilter)<=255))
    {
        LPC_SYSCON->FILTERCLKCFG0=BUS_FREQUENCY_Hz/(1000*filterFrequency*port->digitalFilter);
        port->digitalFilterControl=0; /*Select divisor 0*/
        
    }
    else
    {
        LPC_SYSCON->FILTERCLKCFG0=255;
        port->digitalFilterControl=0;  /*Select divisor 0*/
    }
}


uint32_t _getMatchIntChMask(uint32_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_IR_MR0_INT);
      
    case 1: 
      return(_IR_MR1_INT);
      
    case 2:
      return(_IR_MR2_INT);
      
    case 3:
      return(_IR_MR3_INT);
  
    default:
      return(0); // should never get here
  }
}
