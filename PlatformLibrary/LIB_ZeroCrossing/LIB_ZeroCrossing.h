/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_ZeroCrossing.h
\n			header file of LIB_ZeroCrossing.c
\n
\file		LIB_ZeroCrossing.h
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

#ifndef __ZERO_CROSSING_H
#define __ZERO_CROSSING_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "Lib_ZeroCrossingExp.h"
#include "OCADC_drv.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define USEC                    1
#define MAX_DEV_ON_HALF_PERIOD  7    /* rate */

#if defined(CONFIG_LIB_ZC_CUSTOM_THRES)
#define LIB_ZC_MAX_FREQ_POS_DEV     (CONFIG_LIB_ZC_CSTM_POS_THRES / 10.0) /* in %*/
#define LIB_ZC_MAX_FREQ_NEG_DEV     (CONFIG_LIB_ZC_CSTM_NEG_THRES / 10.0) /* in %*/
#else
#define LIB_ZC_MAX_FREQ_POS_DEV     (7.0) /* in %*/
#define LIB_ZC_MAX_FREQ_NEG_DEV     (9.0) /* in %*/
#endif

#if defined(CONFIG_LIB_ZC_CUSTOM_SYNC_THRES)
#define LIB_ZC_SYNC_MAX_FREQ_POS_DEV     (CONFIG_LIB_ZC_CSTM_SYNC_POS_THRES / 10.0) /* in %*/
#define LIB_ZC_SYNC_MAX_FREQ_NEG_DEV     (CONFIG_LIB_ZC_CSTM_SYNC_NEG_THRES / 10.0) /* in %*/
#else
#define LIB_ZC_SYNC_MAX_FREQ_POS_DEV     LIB_ZC_MAX_FREQ_POS_DEV /* in %*/
#define LIB_ZC_SYNC_MAX_FREQ_NEG_DEV     LIB_ZC_MAX_FREQ_NEG_DEV /* in %*/
#endif

#if !defined(OS_EmbOS)
#define LIB_ZC_SET_OFFSET_VPEAK_MEASURE(x)
#define LIB_ZC_SET_OFFSET_ANALOGINPUTS_MEASURE(x)
#define LIB_ZC_SET_OFFSET_OUTPUTS(x, y, z)
#endif

/*----------- don't touch below -------------*/
#define NOM_HALF_PERIOD_60HZ    ((1000000/(2*60))*USEC)
#define BOUND_HALF_PERIOD_60HZ  (((NOM_HALF_PERIOD_60HZ*MAX_DEV_ON_HALF_PERIOD)/100)*USEC)


#define MIN_FREQ_60HZ    (60.0*(1.0-LIB_ZC_MAX_FREQ_NEG_DEV/100))
#define MAX_FREQ_60HZ    (60.0*(1.0+LIB_ZC_MAX_FREQ_POS_DEV/100))

#define MIN_HALF_PERIOD_60HZ    ((1000000/(2*MAX_FREQ_60HZ))*USEC)
#define MAX_HALF_PERIOD_60HZ    ((1000000/(2*MIN_FREQ_60HZ))*USEC)

#define MEAN_HALF_PERIOD_60HZ   ((MIN_HALF_PERIOD_60HZ+MAX_HALF_PERIOD_60HZ)/2)
#define DEV_HALF_PERIOD_60HZ    (MEAN_HALF_PERIOD_60HZ - MIN_HALF_PERIOD_60HZ)

#define MIN_FREQ_SYNC_60HZ    (60.0*(1.0-LIB_ZC_SYNC_MAX_FREQ_NEG_DEV/100))
#define MAX_FREQ_SYNC_60HZ    (60.0*(1.0+LIB_ZC_SYNC_MAX_FREQ_POS_DEV/100))

#define MIN_HALF_PERIOD_SYNC_60HZ    ((1000000/(2*MAX_FREQ_SYNC_60HZ))*USEC)
#define MAX_HALF_PERIOD_SYNC_60HZ    ((1000000/(2*MIN_FREQ_SYNC_60HZ))*USEC)



#define NOM_HALF_PERIOD_50HZ    ((1000000/(2*50))*USEC)
#define BOUND_HALF_PERIOD_50HZ  (((NOM_HALF_PERIOD_50HZ*MAX_DEV_ON_HALF_PERIOD)/100)*USEC)


#define MIN_FREQ_50HZ    (50.0*(1.0-LIB_ZC_MAX_FREQ_NEG_DEV/100))
#define MAX_FREQ_50HZ    (50.0*(1.0+LIB_ZC_MAX_FREQ_POS_DEV/100))

#define MIN_HALF_PERIOD_50HZ    ((1000000/(2*MAX_FREQ_50HZ))*USEC)
#define MAX_HALF_PERIOD_50HZ    ((1000000/(2*MIN_FREQ_50HZ))*USEC)

#define MEAN_HALF_PERIOD_50HZ   ((MIN_HALF_PERIOD_50HZ+MAX_HALF_PERIOD_50HZ)/2)
#define DEV_HALF_PERIOD_50HZ    (MEAN_HALF_PERIOD_50HZ - MIN_HALF_PERIOD_50HZ)


#define MIN_FREQ_SYNC_50HZ    (50.0*(1.0-LIB_ZC_SYNC_MAX_FREQ_NEG_DEV/100))
#define MAX_FREQ_SYNC_50HZ    (50.0*(1.0+LIB_ZC_SYNC_MAX_FREQ_POS_DEV/100))

#define MIN_HALF_PERIOD_SYNC_50HZ    ((1000000/(2*MAX_FREQ_SYNC_50HZ))*USEC)
#define MAX_HALF_PERIOD_SYNC_50HZ    ((1000000/(2*MIN_FREQ_SYNC_50HZ))*USEC)



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
void LIB_ZC_InitSync(void);
bool_t LIB_ZC_SyncCompleted(void);
void LIB_ZC_ResetSyncPhase(void);
bool_t LIB_ZC_PeriodicAlarm(void);
void LIB_ZC_SetLineVoltageParams( bool_t isPresentVLine, ADC_ValueType vLine );
void LIB_ZC_CheckIfZC_Is_Good( void );
void SetgoodZCSignalTo(bool_t value);
void LIB_ZC_RegisterICUFromPin(uint8_t pin);
#if defined(IRQ_IS_UNMASKABLE_INT)
void LIB_ZC_MaskableHandler(void);
#endif /* defined(IRQ_IS_UNMASKABLE_INT)*/
bool_t LIB_ZC_GetCurrentEdge(void);

void OnZc_OK(LIB_ZcEdgeType edge, uint16_t usHalfWave);
void OnZc_KO(LIB_ZcEdgeType edge, uint16_t usHalfWave);

OS_U32 Get_ZCSysTime(void);

#if defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD)
void LIB_ZC_PeriodoReteMedioManager(void);
#endif /* defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD) */
#endif /* __ZERO_CROSSING_H */


/* *********** */
/* End of file */
/* *********** */
