/**
\defgroup Application configuration

Products modules configuration

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\date    06/04/2011
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL
*/

/**
Header file 

\file    ProductModules_Cfg.h
\ingroup WizardPrj
\date    06/04/2011
\author  Roberto Fioravanti
\version 00.00.01
*/

/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
#ifndef __PRODUCT_MODULES_CFG_H
#define __PRODUCT_MODULES_CFG_H



/* ****************** */
/* Defines and Macros */
/* ****************** */
/* Product modules - PM_Library*/
#define IO_MANAGER_BASICLOADER_CSTM		FALSE
/* ******** */
/* Includes */
/* ******** */
/* no include */

/* ****** */
/* Errors */
/* ****** */


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* Product modules - SwTimersCounters*/
#define PLATFORM_SW_COUNTERS	\
	&Time2Death_Counter,				/** Contatore reset PC Control */				\
	&Time_Since_Last_Reset_Counter,		/** Contatore dall'ultimo reset */				\

#define PLATFORM_SW_COUNTERS_CFG	\
	&CfgTime2Death_Counter,				/** Contatore reset PC Control */				\
	&CfgTime_Since_Last_Reset_Counter,	/** Contatore dall'ultimo reset */				\

#define TIME2DEATH_COUNTER_ID			            (0)
#define TIME_SINCE_LAST_RESET_COUNTER_ID			(1)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
#endif /* __PRODUCT_MODULES_CFG_H */
/*@}*/

/* *********** */
/* End of file */
/* *********** */
