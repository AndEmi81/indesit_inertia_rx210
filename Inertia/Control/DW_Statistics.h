/**
\defgroup DW_Statistics	  Modulo Statistiche DW

Il modulo contiene le funzioni di gestione delle statistiche

\par \b PCLint: x Errors, 2 Warnings (indentation), x Info 12/06/2008 Valeria Catani


\date    11/09/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_Statistics.c

\file    DW_Statistics.h
\ingroup DW_Statistics
\date    11/09/2008
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
\author  
*/


/**
\addtogroup DW_Statistics
@{*/

#ifndef __DW_STATISTICS_H
#define __DW_STATISTICS_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define DUMMY_CLOCK_VALUE   0x00



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

void STC_manageStatResetRequest(void);
void STC_superVisorUpdateStatistics(void);
void STC_LastCycleStat_Init(void);
void STC_LastCyclesArray_Init(void);
void STC_functionValueUpdateStatistics(void);
void STC_userDishRack_H2oHardness_Update(void);
void STC_regenerate_spaklingAid_Update(void);
void STC_Add_LastCycle_turbPerc_turbCalType(void);
void STC_updateLastCycleStat_END(uint8_t mode);
#endif /* __DW_STATISTICS_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
