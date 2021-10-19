/**
\defgroup	APP_Global Modulo Application Platform Variables
\ingroup	ApplPlatform

Il modulo contiene la dichiarazione e, laddove prevista, l'inizializzazione
delle variabili e delle costanti comuni a tutte le applicazioni.
*/

/**
Source file del modulo Application Platform Variables

\file		APP_Global.c
\ingroup	APP_Global
\date		18/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

\par		PCLint
			Report 08/07/2009: 0 Errors, 0 Warnings, 0 Info

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#define __APPL_GLOBAL

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "App_Global.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
#if (APP_NEW_FAULT_STATE_MACHINE == 1)
/* Stuttura fault applicazione */
Fault_Compl_Struct ApplCurrentFault[APP_N_COMP] =
{
	{ CODE_NO_FAULT, SUBCODE_NO_FAULT ,	/* sft_fault_codes_t faultCodes;*/
	 CODE_NO_FAULT, SUBCODE_NO_FAULT },	/* sft_fault_codes_t prefaultCodes; */

#if (MULTI_COMPARTMENT_APPL == 1)	

	{ CODE_NO_FAULT, SUBCODE_NO_FAULT ,	/* sft_fault_codes_t faultCodes;*/
	 CODE_NO_FAULT, SUBCODE_NO_FAULT },	/* sft_fault_codes_t prefaultCodes; */

#endif	
};

/* Stuttura fault piattaforma */
sft_fault_codes_t PltFaultCodes =
{
	CODE_NO_FAULT,
	SUBCODE_NO_FAULT
};

/* Stuttura fault globale */
sft_fault_codes_t CurrentFault[APP_N_COMP] =
{
	{ CODE_NO_FAULT, SUBCODE_NO_FAULT },
#if (MULTI_COMPARTMENT_APPL == 1)	
	{ CODE_NO_FAULT, SUBCODE_NO_FAULT },
#endif	
};


/** Fault initialization */
_AP_GLOBAL_ FaultMgmt_Data_Struct FaultMgmt_Data[APP_N_COMP] =
{
	{
		{ 0, 0 }, /* sft_fault_codes_t faultCodes; */
		0, /* uint8_t step; */			/** step macchina a stati gestione fault */
		0, /* uint8_t stepChanged; */
		LKT_INIT_S(0), 	/** contatore alarm->prefault */
		LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */
		SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	},
#if (MULTI_COMPARTMENT_APPL == 1)		
	{
		{ 0, 0 }, /* sft_fault_codes_t faultCodes; */
		0, /* uint8_t step; */			/** step macchina a stati gestione fault */
		0, /* uint8_t stepChanged; */
		LKT_INIT_S(0), 	/** contatore alarm->prefault */
		LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */
		SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	},
#endif	
};

const sft_fault_codes_t No_Fault =
{
	CODE_NO_FAULT,
	SUBCODE_NO_FAULT
};

#endif
/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */


/* ***************** */
/* Exported function */
/* ***************** */

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
