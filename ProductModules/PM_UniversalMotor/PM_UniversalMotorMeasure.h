/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\par    Software Energy Meter
\n
\n
\file    PM_UM_Measure.h
\brief
\addtogroup PM_UniversalMotor
*/

#ifndef __PM_UM_MEASURE_H
#define __PM_UM_MEASURE_H

/* ********** */
/* Includes   */
/* ********** */
#include "PM_UniversalMotorExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* SF pointer and displacements */

/* ENGINE_PARAM_DATA_PTR (0x24) displacements */
#define PM_UM_COMMON_PARAM_DISPL			((uint8_t)0x00)
#define PM_UM_TAB_SPEED_RANGE_DISPL			((uint8_t)0x01)
#define PM_UM_ALPHA_MIN_SOFT_START_DISPL	((uint8_t)0x02)
#define PM_UM_MID_FIELD_DISPL				((uint8_t)0x03)
#define PM_UM_ENGINE_PHASE_DISPL			((uint8_t)0x04)
#define PM_UM_ANTIFOAMING_DISPL				((uint8_t)0x05)
#define PM_UM_TORQUE_LIM_TAB_DISPL			((uint8_t)0x06)
#define PM_UM_ENGINE_HL_PHASE_DISPL			((uint8_t)0x07)

#define VAC_OFFSET              			4
#define SIZE_ENGINE_DATA_BLOCK  			6
#define M_D_OFFSET              			1
#define Q_OFFSET                			(2/sizeof(uint16_t))

/* state */
#define PM_UM_MEASURE_IS_ON					(uint8_t)0
#define PM_UM_MEASURE_IS_OFF				(uint8_t)(!PM_UM_MEASURE_IS_ON)

/* inizio dati m_n,m_d,q,Vac */
#define MID_FIELD_DATA_OFFSET       		0x04


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct getEngineData_s
{
    uint16_t dato;
    uint8_t error;
} getEngineData_t;

typedef enum {
	FORMULA_PARAM_ERR_OK,		/**< phase average : 3ph, brush */
	FORMULA_PARAM_ERR_m_d,		/**< antifoam     : 3ph, brush  */
} PM_UM_FormulaError_Type;
/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_UM_GetMotorData( PM_UM_Meas_t formulaid, getEngineData_t *engineData );
PM_UM_FormulaError_Type ApplyFormulaParam(const uint8_t * pparamset, uint16_t * result);
#endif /* __PM_UM_MEASURE_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
