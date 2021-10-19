/**
\addtogroup
\ingroup Project
*/
/*
** **************************************************************************
** **************************************************************************
*/ 
/**
 
Gestisce i fault della macchina.
Header file 
 
\file    	WM_Fault.h
 
\version 	1.0.0
\date    	19/09/2006
\authors	Marco Quintini
*/ 
/*
** **************************************************************************
** **************************************************************************
*/

/*
Copyright (c) 2004-2006, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version
\date
\author
*/


/**
\addtogroup
@{*/

#ifndef __APP_FAULT_H
#define __APP_FAULT_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Step procedura rilevazione/reset fault */
enum
{
	FLT_IDLE_STEP =	0 ,			/*.0 */
	ALARM_FLAG_STEP,			/*.1 */
	PREFAULT_WAIT_STEP,			/*.2 */
	PREFAULT_STEP,				/*.3 */
	FAULT_STEP,					/*.4 */
	WAIT_PREFAULT_RESET_STEP,	/*.5 */

};
/**/

/* Mask on field status of struct sft_module_faults_t */
#define SFT_FAULTS_STATUS_MASK				0x7F
#define SFT_FAULT_STATUS_CHANGED_MASK		0x80

/**/
/* Mappatura dei bit nel byte di configurazione passato come parametro alle funzioni FLT_getTimeout() e FLT_prefaultFaultStateManagement() */
#define FLT_PRESCALER_MASK				0x0F	/* bit 0..3 */
#define COMP_MASK						0x30	/* bit 4 e 5 */
#define THEREATTEMPTFLT_BIT				6
#define THEREATTEMPTFLT_MASK			MASK_B(THEREATTEMPTFLT_BIT)
#define FLT_SAFETY_ACTION_BIT			7
#define FLT_SAFETY_ACTION_MASK			MASK_B(FLT_SAFETY_ACTION_BIT)

/* Shift per estrarre il campo compartment dal byte di configurazione della macchina a stati dei fault */
#define COMP_MASK_NUM_RIGHT_SHIFT		4


#define FAULT_RECOVERY_MASK		0x8000
#if (MULTI_COMPARTMENT_APPL == 1)
#define FAULTRESET_TIMEOUT_MSK	0x0FFF
#else
#define FAULTRESET_TIMEOUT_MSK	0x3FFF
#endif

#define COMPARTMENT_DISPLACEMENT	12
#define RESTORABLE_DISPLACEMENT		15
#define SET_FAULT_RESTORABLE		(1 << RESTORABLE_DISPLACEMENT)
#define SET_FAULT_NOT_RESTORABLE	(0)
#define TOUT_PREFAULT_CUSTOM   			((uint16_t)(0))

/* Codici eventi salvataggio dati black box */
#define BB_EVENT_PREFAULT			0
#define BB_EVENT_FAULT_MATCH		1
#define BB_EVENT_FAULT_NOT_MATCH	2

#define STC_BB_MATCHFOUND_MASK    	0x80
#define STC_BB_COMP_MASK			0x7F

/** Codici e sottocodici */
#define CODE_NO_FAULT	    	0
#define SUBCODE_NO_FAULT	    0

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void FLT_lastFaultUpdate(sft_fault_codes_t faultToBeSaved, LastFault_Struct *ptrMostRecentFault);
bool_t FLT_checkForSettingFault(void);
void FLT_declareSettingFault(void);
void FLT_safetyBitmapReset(void);

#if (APP_NEW_FAULT_STATE_MACHINE == 1)

#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_applCurrentFaultReset(uint8_t compartment);
void FLT_faultMgmtDataReset(uint8_t compartment);
bool_t FLT_checkForCurrentFault(uint8_t code, uint8_t subcode, uint8_t compartment);
void FLT_manageFault(uint8_t code, uint8_t subcode, uint8_t compartment);
void FLT_updateGlobalFaultCode(uint8_t compartment);
void FLT_SafetyBitmap_Cstm_Exclusion ( uint8_t compartment);
#ifdef _STATISTICS_ON_
void STC_saveBlackBoxData(sft_fault_codes_t faultToBeSaved, uint8_t compartment);
#endif /* _STATISTICS_ON_ */

#else /* MULTI_COMPARTMENT_APPL == 0 */

void FLT_applCurrentFaultReset(void);
void FLT_faultMgmtDataReset(void);
bool_t FLT_checkForCurrentFault(uint8_t code, uint8_t subcode);
void FLT_manageFault(uint8_t code, uint8_t subcode);
void FLT_updateGlobalFaultCode(void);

void FLT_SafetyBitmap_Cstm_Exclusion ( void);
#ifdef _STATISTICS_ON_
/* Questa funzione deve essere implementata dal prodotto specifico in XX_Statistics.c */
void STC_saveBlackBoxData(sft_fault_codes_t faultToBeSaved);
#endif /* _STATISTICS_ON_ */

#endif /* (MULTI_COMPARTMENT_APPL == 1)*/

void FLT_ResetFaultDetectProc(uint8_t code, uint8_t subcode, uint16_t reset_config);
void FLT_safetyBitmapStandard(void);
void FLT_updatePlatformFaultStatus(void); 
void FLT_manageGlobalFaultStatus(void);
uint8_t FLT_getTimeout(uint8_t code, uint8_t subcode, Timeout_struct *timeoutdata);
#endif /* (APP_NEW_FAULT_STATE_MACHINE == 1)*/
#endif /* __APP_FAULT_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */





