/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCLPM_drv.h
\n			Header file of OCLPM_drv.c
\n
\file		OCLPM_drv.h 
\ingroup	LPM
\date		Jul 02, 2012 
\version	01.01.01 
\author		Leonardo Sparti
\n
\n
\par\b	History:
\n
\n 		01.00.00 
\n		17/09/2010 
\n		Jaroslav Musil 
\n		- Creation
\n
\n
\n etc.
*/

#ifndef __OCLPM_DRV_H
#define __OCLPM_DRV_H

#define __DRIVER_H

/* ********** */
/* Includes */
/* ********* */

#include "Hal_Interface.h"
#include HAL_CFG

#include "StandardDefines.h"
#include "IO_driver.h"

#if defined(_HAL_LPM_PERIOD)
  #include "OCADC_drv.h"
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define LPM_ID_I2C                         0x01
#define LPM_ID_UART                        0x02
#define LPM_ID_DIO                         0x03
#define LPM_ID_PERIOD                      0x04
                                           
#define LPM_CRITERIA_I2C                   (1 << (LPM_ID_I2C    - 1))
#define LPM_CRITERIA_UART                  (1 << (LPM_ID_UART   - 1))
#define LPM_CRITERIA_DIO                   (1 << (LPM_ID_DIO    - 1))
#define LPM_CRITERIA_PERIOD                (1 << (LPM_ID_PERIOD - 1))
                                           

#define LPM_WAKE_UP_PERIOD_EDGE_MASK       0x80
#define LPM_WAKE_UP_PERIOD_MODULE_MASK     0x7F
#define LPM_WAKE_UP_PERIOD_TYPE_MASK       0xF0
#define LPM_WAKE_UP_PERIOD_CONDITION_MASK  0x8000
#define LPM_WAKE_UP_PERIOD_DELTA_MASK      0x7FFF

#define LPM_WAKE_UP_PERIOD_TYPE_ANDELTA    1
#define LPM_WAKE_UP_PERIOD_TYPE_ANRANGE    2
#define LPM_WAKE_UP_PERIOD_TYPE_DIGITAL    3
#define LPM_WAKE_UP_PERIOD_TYPE_SHIFT      4


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

#if defined(_HAL_LPM_PERIOD)
  /* Result sum after the resolution correction */
  #if (ADC_RESOLUTION <= 8)
  typedef uint16_t ADC_SumType;
  #else
  typedef uint32_t ADC_SumType;
  #endif
#endif /* defined(_HAL_LPM_PERIOD) */


#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
typedef struct OCLPM_WakeUpPinType_tag
{
    uint8_t  modulePin;
    uint8_t  expectedPinValue;
    uint16_t timeFilter;
} OCLPM_WakeUpPinType;
#endif /* defined(_HAL_LPM_DIO) */


#if defined(_HAL_LPM_PERIOD)
  typedef struct OCLPM_WakeUpPeriodType_tag
  {
    uint8_t modulePin;
    uint8_t timeFilter;
    uint8_t type;
    uint8_t sleepTime;
    ADC_ValueType adcSetPoint;
    ADC_ValueType adcDelta;
  } OCLPM_WakeUpPeriodType;
#endif /* defined(_HAL_LPM_PERIOD) */


/* ****************** */
/* Exported variables */
/* ****************** */


#if defined(_HAL_LPM_PERIOD)
  extern OCLPM_WakeUpPeriodType OCLPM_WakeUpPeriod;
#endif


/* ******************* */
/* Function prototypes */
/* ******************* */
IO_ErrorType OCLPM_LowPowerModeInit(void);
IO_ErrorType OCLPM_GoLowPowerMode(uint16_t wakeupCriteria);
void OCLPM_PeriodIsr(void);

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
extern  void OCLPM_WakeUpIsr(void);
#endif

#if defined(_HAL_LPM_PERIOD)
  IO_ErrorType OCLPM_SetWakeUpPeriodParam(uint8_t *pStruct);
#endif
IO_ErrorType OCLPM_Callback(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error);
#endif /* __OCLPM_DRV_H */

/* *********** */
/* End of file */
/* *********** */

