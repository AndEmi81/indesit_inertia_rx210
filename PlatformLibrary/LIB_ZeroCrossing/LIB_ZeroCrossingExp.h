/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_ZeroCrossingExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		LIB_ZeroCrossingExp.h
\ingroup	LIB_ZeroCrossing
\date		Dec 20, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Dec 20, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __ZERO_CROSSING_EXP_H
#define __ZERO_CROSSING_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "OCADC_drv.h"
#include "OCTPM_drv.h"
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
#include "syncSvc.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* <belonging to the module, visible to the Application project> */
/** define on NominalGridFrequency */
#define NOM_FREQ_IS_50_HZ       1
/** define on NominalGridFrequency */
#define NOM_FREQ_IS_60_HZ       0
/** define on NominalGridFrequency */
#define NOM_FREQ_IS_UNDEFINED   0xFF

/* Returns values for LIB_ZC_GetGridFrequencyWarning() */
/** Grid Frequency in range */
#define GRID_FRQ_IN_RANGE   0
/** Grid Frequency is low */
#define GRID_FRQ_TOO_LOW    1
/** Grid Frequency is high */
#define GRID_FRQ_TOO_HIGH   2
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
/** Variables used to check if Zero Crossing signal is right */
typedef struct checkIfZC_Is_Good_Tag
{
	/** V peak measure A/D */
	ADC_ValueType lineVoltage;
	/** If TRUE V line input is available */
	bool8_t vLineIsPresent;
	/** If TRUE Zero Crossing signal is right */
	bool8_t	goodZC;
	/** If TRUE Grid Frequency in range */
	bool8_t freqValid;
	uint8_t stability;
} checkIfZC_Is_Good_t;

typedef struct {
	/** Negative Half Wave [uSec] */
	uint16_t HalfWaveLengthNegUsec;
	/** Positive Half Wave [uSec] */
	uint16_t HalfWaveLengthPosUsec;
	/** Counter used for Number of Zero Crossing Edges to discharge before entering in syncronization phase */
	uint8_t CntrEdgesToDisc;
	/** Counter used for Syncronization Phase */
	uint8_t NumZC;
	/** Counter used to define Nominal Grid Frequency */
	int8_t GridFreqFlt;
	/** Counter used to check too low Grid Frequency */
	uint8_t CntrFreqTooLow;
	/** Counter used to check too high Grid Frequency */
	uint8_t CntrFreqTooHigh;
	/** If '1' Grid Frequency is too low */
	bool_t FreqTooLowWarning;
	/** If '1' Grid Frequency is too high */
	bool_t FreqTooHighWarning;
	/** Previous timer's value in ticks(type verified) */
	IO_HwTimerType OldHalfPeriodMeasure;
	/** This variable is used to store nominal grid frequency, 50Hz or 60 Hz */
	uint8_t NominalGridFrequency;
	OS_ALARM_TICK ZcRefTick;
	OS_U32 SysTime;	/* ZC sysTime*/
#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
	IO_ICU_CfgType IcuZC_cfg;
#endif
#if defined(IRQ_IS_UNMASKABLE_INT)
	bool_t go_on;
	IO_NotificationType edge;
#endif /* defined(IRQ_IS_UNMASKABLE_INT) */
} LIB_ZC_Control;

typedef enum
{
ZC_EDGE_FALLING=0,          /**< falling edge or negative halfwave    */
ZC_EDGE_RISING             /**< rising edge or positive halfwave   */
}LIB_ZcEdgeType;

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
#if !defined(__ZERO_CROSSING)
extern const checkIfZC_Is_Good_t IsZC_Good;
extern const LIB_ZC_Control LIB_ZC_Control_Var;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
uint8_t LIB_ZC_GetNominalFrequency(void);
uint16_t LIB_ZC_GetNominalHalfWave(void);
bool_t goodZCSignal( void );
uint8_t  getZCStability(void);
bool_t IsZCSyncOngoing( void );
uint8_t LIB_ZC_GetGridFrequencyWarning( void );
uint16_t LIB_ZC_GetPartialWidth(void);
uint16_t LIB_ZC_GetHalfWaveLengthPos(void);
uint16_t LIB_ZC_GetHalfWaveLengthNeg(void);

void LIB_ZC_InitPeriodicEvents(IO_NotificationType notifVal);
uint32_t LIB_ZC_GetReference_us(void);
uint32_t LIB_ZC_GetExtCntrReference_us(void);

uint16_t LIB_ZC_GetSemiPeriod(void);
uint16_t LIB_ZC_GetPeriod(void);
bool_t LIB_ZC_IsOnGood(void);
bool_t LIB_ZC_IsOnNotGood(void);

uint32_t LIB_ZC_GetPeriodoMedio(void);
uint32_t LIB_ZC_GetPeriodoReteNominale(void);
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
extern NotifyDataEvent ZcEvent;
#endif

#endif /* __ZERO_CROSSING_EXP_H */

/* *********** */
/* End of file */
/* *********** */

