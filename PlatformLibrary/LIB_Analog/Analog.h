/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Analog.h
\n			[Brief description]
\n
\file		Analog.h
\ingroup	LIB_Analog
\date		Jun 16, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
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

#ifndef __ANALOG_H
#define __ANALOG_H

/* ********** */
/* Includes */
/* ********* */
/*lint -e537*/
#include "Config.h"
#include "OCADC_drv.h"
#include "AnalogExp.h"
/*lint +e537*/
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
/**
Not defined Ch_Vline value
*/
#define CH_VLINE_IS_NOT_DEFINED				(uint8_t)0xFF
/**
Not defined Ch_Comp value
*/
#define CH_FOR_COMPENSATION_IS_NOT_DEFINED	(uint8_t)0xFF
/**
Channel Not Found
*/
#define CH_NOT_FOUND    (uint8_t)0xFF
/**
Position Not Found
*/
#define AD_POS_NOT_FOUND    (uint8_t)0xFF
/**
Index of AnalogInputParam[] related to channel Not Found
*/
#define AD_INDEX_NOT_FOUND  (uint8_t)0xFF

#define VOLT_TO_ADC(VREF)	(ADC_ValueType)((ONE_AD_SAMPLE_MAX_VALUE*R_PULL_DN*(VREF))/(VCC*(R_PULL_UP+R_PULL_DN)))

/* V ref (RTN validation range A/D units)*/
#define	REF_MIN_ADC_UNITS 			VOLT_TO_ADC(RTN_ACC_MIN)
#define	REF_MAX_ADC_UNITS 			VOLT_TO_ADC(RTN_ACC_MAX)

/* V ref (RTN compensation valid range A/D units)*/
#define	COMP_MIN_ADC_UNITS 			VOLT_TO_ADC(RTN_COMP_MIN)
#define	COMP_MAX_ADC_UNITS 			VOLT_TO_ADC(RTN_COMP_MAX)

/* V ref (RTN tolerance range A/D units)*/
#define	UPPER_BOUND_RTN_ADC_UNITS 	VOLT_TO_ADC(UPPER_BOUND_RTN)
#define	BOTTOM_BOUND_RTN_ADC_UNITS 	VOLT_TO_ADC(BOTTOM_BOUND_RTN)

/* Compensation Value */
#define AD_COMP_VAL     VOLT_TO_ADC(RTN)

#if (VLINEMEASURE_COMP_FACTOR != 0)
#if ((((1<<(SIZE_IN_BYTE_OF_ADC_ValueType*8)))-1) < (ONE_AD_SAMPLE_MAX_VALUE+VLINEMEASURE_COMP_FACTOR))
//"Possible overflow on V line Compensation"
#endif
#endif /* #if (VLINEMEASURE_COMP_FACTOR != 0) */

/*
parameter related to LIB_ADC_DeltaCtrl( uint8_t index, ADC_ValueType newADvalue)
*/
#define ANALOG_SYNC_THR         2
#if (ANALOG_SYNC_THR < 2)
#error "ANALOG_SYNC_THR must be greater than 1"       
#endif
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
void LIB_ADC_InitSync (void);
void LIB_ADC_RegisterADchannel( uint8_t channel, uint8_t posResult, const uint8_t *pADlowLevelParams );
void LIB_ADC_UnregisterAllADch(void);
uint8_t LIB_ADC_AnalogChRamPos(uint8_t channel);

void LIB_ADC_SetAnalogValue( uint8_t pos, ADC_ValueType value );
void LIB_ADC_Manager(void);
ADC_ValueType LIB_ADC_Conv_F  ( uint8_t index, uint8_t ad_sample_num_pot2, uint8_t samples_to_discard);
void LIB_ADC_ResetAnalogMeasure(uint8_t index);
void LIB_ADC_ForceAutoRead(uint8_t _index, bool_t newVal);
void LIB_ADC_InitMeasureForCh ( uint8_t channel );
bool_t LIB_ADC_IsNoAutoMeasureReady( uint8_t channel );

uint8_t LIB_ADC_AnalogInputParamIndexVsPos(uint8_t pos);
uint8_t LIB_ADC_ADposVsIndex(uint8_t index);
uint8_t LIB_ADC_ADChVsAnalogInputParamIndex(uint8_t channel);
bool_t LIB_ADC_CheckRef(void);
bool_t LIB_ADC_VLineMeasure(ADC_ValueType *pVlinePeak);
uint8_t LIB_ADC_GetChCompPos(void);
#endif	/* __ANALOG_H */

/* *********** */
/* End of file */
/* *********** */
