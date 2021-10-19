/**
\defgroup DW_Supervisor	Modulo Supervisore DW

Il modulo contiene funzioni di gestione delle transizioni 
di stato della macchina

\par \b PCLint: x Errors, 6 Warnings (indentation), x Info 24/09/2008 Valeria Catani


\date    08/04/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_Supervisor.c

\file    DW_Supervisor.h
\ingroup DW_Supervisor
\date    08/04/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date     
\author  Marco Quintini - Arcadia / Stefano Ughi - DEA601
*/



/**
\addtogroup DW_Supervisor
@{*/

#ifndef __DW_SUPERVISOR_H 
#define __DW_SUPERVISOR_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Numero di fasi del ciclo di PROGRAMMING */
#define PHASES_PROGRAMMING_NUM	CYCLES_NUM	/* Essendo il ciclo creato dall'Editor, è garantito 
											   che il numero delle sue fasi coincida con il numero
											   dei cicli selettore */

/* HarakiriManager */
#define SBY_SB_RESET			0
#define SBY_SB_ON				  1
#define SBY_FAST_SB_ON		2
/**/
										   

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */



/* ******************* */
/* Function prototypes */
/* ******************* */

void SPV_superVisor(void);
uint8_t SPV_getCycleHeaderPtr(uint8_t cycle, const uint8_t **pCycleHeading);
uint8_t SPV_searchCycleByLabel(uint8_t completeLabel, uint8_t *pCycle);
uint8_t SPV_getRefCycleForFuncValue(void);
void SPV_setupDelayCounterVal(void);
void SPV_setupDelayCounter(void);
bool_t SPV_isCycleRunning(void);
bool_t SPV_isAutotestRunning(void);
bool_t SPV_isAutotestOrTestRunning(void);
void SPV_manageRequestFromRemote(void);
bool_t SPV_goingToSleepManager(void);
void SPV_resetSensVariables(void);


#endif	/* __DW_SUPERVISOR_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */

