/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Measure.h
\brief
\addtogroup MotorGate
@{*/

#ifndef __PM_UM_MEASUREEXP_H
#define __PM_UM_MEASUREEXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*#define PM_UM_MEASURES_NUM					2*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* codici di ritorno della funzione getEngineMeasures() */

#define	ENGINE_MEASURE_IS_NOT_READY		0
#define	ENGINE_MEASURE_IS_UP			1
#define	ENGINE_MEASURE_IS_DOWN			2

typedef struct engine_measures_s
{
    uint32_t sum_alpha;		/* motore universale */
    uint16_t average;		/* motore universale */
    uint8_t  num_samples_alpha;  /* motore universale */
    uint8_t  on_off;		/* 0 -> OFF, !0 -> ON motore universale */
    uint8_t  result;		/* nel caso di gestione sia del motore universale che
				   del trifase questa variabile e' condivisa */
    uint16_t alphalimit; 		/* angolo limite motore universale */
} engine_measures_t;


/** Type of allowed measurements */
typedef enum {
	PM_UM_MEASURE_PHASE,		/**< phase average : 3ph, brush */
	PM_UM_MEASURE_ANTIFOAM,     /**< antifoam     : 3ph, brush  */
	PM_UM_MEASURE_HL_PHASE,		/**< phase average for half load case : 3ph, brush */
#if defined(CONFIG_PM_MOTORGATE)
	PM_UM_MEASURE_SPEED_LIM,	/**< speedLim   : brush  */
#endif
	PM_UM_NUM_MEASURES, 		/**< Number of measurements stored in engine_measures */
#if defined(CONFIG_PM_MOTORGATE)
	PM_UM_MEASURE_MID_FIELD,	/**< mid field : brush */
	PM_UM_MEASURE_APLHA_MIN,
#endif
} PM_UM_Meas_t;

/**
Result
*/
typedef enum {
	PM_UM_MEASURE_IS_NOT_READY=0,		/**< phase average : 3ph, brush */
	PM_UM_MEASURE_IS_UP,				/**< antifoam     : 3ph, brush  */
	PM_UM_MEASURE_IS_DOWN
} PM_UM_MeasureResult_Type;

/* **************** */
/* Global variables */
/* **************** */
#if !defined(MODULE_BODY_PM_UNIVMOTORMEASUER)
extern engine_measures_t engine_measures[PM_UM_NUM_MEASURES];
#endif


/* ******************* */
/* Function prototypes */
/* ******************* */
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
void PM_UM_SetTorque( uint16_t torqueMeasure, uint8_t whichMeasure );
#endif

PM_UM_MeasureResult_Type PM_UM_GetMotorMeasures( PM_UM_Meas_t whichMeasure );

#endif /* __PM_UM_MEASUREEXP_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
