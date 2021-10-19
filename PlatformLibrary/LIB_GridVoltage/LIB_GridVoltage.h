/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_GridVoltage.h
\n			header file of LIB_GridVoltage.c
\n
\file		LIB_GridVoltage.h 
\ingroup	LIB_GridVoltage
\date		Jan 12, 2011 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n
\n 		01.00.00 
\n		Jan 12, 2011 
\n		stefano.ughi 
\n		changes 
\n 			Creation
\n
*/

#ifndef __GRID_VOLTAGE_H
#define __GRID_VOLTAGE_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "OCADC_drv.h"
#include "LIB_GridVoltageExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

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
bool_t LIB_GV_CheckNominalVoltage(ADC_ValueType Vline);
void LIB_GV_InitNominalGridVoltageMeasure(void);
int16_t LIB_GV_getRMS_Vac( ADC_ValueType measureADvLine );
void ManageVLineRefCrossing(ADC_ValueType vPeakADvalue);
uint8_t LIB_GV_GetNominalGridVoltage(void);
#endif /* __GRID_VOLTAGE_H */

/* *********** */
/* End of file */
/* *********** */
