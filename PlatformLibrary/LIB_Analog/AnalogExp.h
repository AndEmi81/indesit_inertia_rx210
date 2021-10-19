/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		AnalogExp.h
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		AnalogExp.h
\ingroup	LIB_Analog
\date		Jun 16, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n 		01.03.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

#ifndef __ANALOG_EXP_H
#define __ANALOG_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OCADC_drv.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
/* These variables are exported only for LookUpTable's use, don't perform any access to these
variables directly, use functions below, if avaible */

#if !defined(MODULE_BODY_LIB_ANALOG)
extern const analogInput_t AnalogInputDisable;
extern const analogInput_t AnalogInputReady;
extern const ADC_ValueType AnalogInput[ADC_NUM_CHANNELLS_USED];
#endif
/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
bool_t LIB_ADC_AreAnalogInputsReady(void);
bool_t LIB_ADC_IsAnalogReady(uint8_t pos);
ADC_ValueType LIB_ADC_GetAnalogValue(uint8_t pos);
void LIB_ADC_InitMeasureForChInPos (uint8_t pos);
bool_t LIB_ADC_IsNoAutoMeasureForChInPosReady (uint8_t pos);
bool_t LIB_ADC_IsApplAnalogReady(uint8_t pos);
void LIB_ADC_ResetApplAnalogReady(uint8_t pos);
ADC_ValueType LIB_ADC_GetRefInput(void);
bool_t LIB_ADC_IsRefInputReady(void);
bool_t LIB_ADC_Is_Ref_Good(void);
bool_t LIB_ADC_Is_RTN_Out_Range(void);
#if defined(CONFIG_VLINE_ON_NEG_LEVEL)
ADC_ValueType LIB_ADC_GetVlineOnNegLevel(void);

/* Deprecated - for backward compatibility */
#define SFC_GetVlineOnNegativeLevel()	LIB_ADC_GetVlineOnNegLevel()
#define AD_CONVERTER_CHECK				CONFIG_VLINE_ON_NEG_LEVEL

#endif /* defined(CONFIG_VLINE_ON_NEG_LEVEL) */
#endif /* __ANALOG_EXP_H */

/* *********** */
/* End of file */
/* *********** */

