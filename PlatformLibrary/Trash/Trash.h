/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		Trash.h
\n			header file of Trash.h
\n
\file		Trash.h 
\ingroup	[module_name_slave] 
\date		Jul 7, 2010 
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

#ifndef __TRASH_H
#define __TRASH_H

/* ********** */
/* Includes */
/* ********* */
#include "OCADC_drv.h"
#if defined(BUILD_RENESAS_36079)
#include "osek.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#if (PWM_LOAD_ENABLE == 1)
/* for PWM loads and buzzer */
#define FIRST_PWM_CHANNEL_PIN   27
#define FIRST_PWM_CHANNEL_ADR   0x00000065	   /** TPM2C0SC Register Addr */
#endif /* (PWM_LOAD_ENABLE == 1) */

#define VLINE_220V   0
#define VLINE_110V   1

 
 

 

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
#if (PWM_LOAD_ENABLE == 1)
void PWMinit (void);
#endif
void ResetVlineMeasure(void);
uint8_t VlineMeasureUpdate(ADC_ValueType Vline);
uint8_t VlineMeasureType(void);
void ManageVLineRefCrossing(ADC_ValueType actual_vline_value);
#if defined(WBOX) && (CALIBRATION_ENABLE == 1)
void InitCalibrationPointers_trash(void);
uint8_t WboxLoader(uint8_t displacement, Loader_Struct* pDataOut);
#endif /* #if defined(WBOX) && (CALIBRATION_ENABLE == 1) */
void Set_IO_to_Default(void);
ADC_ValueType VlineGetTh(ADC_ValueType line);
#endif /* __TRASH_H */

/* *********** */
/* End of file */
/* *********** */
