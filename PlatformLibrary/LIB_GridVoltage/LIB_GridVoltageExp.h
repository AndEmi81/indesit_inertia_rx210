/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_GridVoltageExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		LIB_GridVoltageExp.h
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

#ifndef __GRID_VOLTAGE_EXP_H
#define __GRID_VOLTAGE_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */
#define NOMINAL_VLINE_IS_230V   0
#define NOMINAL_VLINE_IS_110V   1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
void LIB_GV_SetVLineRef(uint16_t refl, uint16_t refh);
uint16_t LIB_GV_ReadVLineCrossing_under_refl(void);
uint16_t LIB_GV_ReadVLineCrossing_over_refh(void);
void LIB_GV_SetVLineRefCntInitValueRefL(uint16_t init_value);
void LIB_GV_SetVLineRefCntInitValueRefH(uint16_t init_value);

#endif /* __GRID_VOLTAGE_EXP_H */

/* *********** */
/* End of file */
/* *********** */

