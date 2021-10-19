/**
\addtogroup StandBy
\ingroup Project
*/

/*
** **************************************************************************
** **************************************************************************
*/ 
/**
 
Si occupa della gestione dello stand by dell'elettrodomestico
 
\file    	APP_StandBy.h

\version 	1.0.0
\date    	27/03/2008
\authors	Simone Costantini
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
\addtogroup StandBy
@{*/

#ifndef __STANDBY_H
#define __STANDBY_H

#if	(SBY_STANDBY_ENABLE == 1)

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* HarakiriManager */
#define HKM_SB_RESET			0
#define HKM_SB_ON				1
#define HKM_FAST_SB_ON			2

/**/

/*Sby Low Power 22.01.10 */
#define HKM_SBY_NO_SLEEP_MODE		0x03 /* per il mode indica la che non andrà main in sleep */
#define HKM_SBY_HW_MODE				0x02 /* per il mode indica la cella di memoria Hw */
#define HKM_SBY_LOW_POWER_MODE		0x01 /* per il mode indica lo sleep mode*/
#define HKM_SBY_OFF_MODE			0x00 /* non c'è lo standby */

#define HKM_LOW_POWER_REQ			0x01 	
#define HKM_LOW_POWER_RESET			0x00

/* Flags position in setting file bitmap to disable standby */
#if defined (CK_HOTTIMA) 
	#define SBY_WAIT_DISABLE_FLAG	  3
	#define SBY_DISABLE_FLAG		  2
#elif defined (DW_DISHWASHER)
 	#define SBY_WAIT_DISABLE_FLAG	  10
	#define SBY_DISABLE_FLAG		  11
#else
	#define SBY_WAIT_DISABLE_FLAG		((uint8_t) 1 )	/**< Disable wait mode */
	#define SBY_DISABLE_FLAG		  	((uint8_t) 2 )	/**< Disable standby */
#endif
	#define SBY_FLAGS_BITMASK			( MASK_B(SBY_WAIT_DISABLE_FLAG) | MASK_B(SBY_DISABLE_FLAG) )

/** StandBy */
#define TOUT_RESET_STANDBY(p)		((SWTC_TimeType)((p == NULL)? 10 : OFFS_B(p, 0)))	/* TimeOut reset StandBy, Se il parametro non è definito in setting prende 10x100ms = 1s */

#define TOUT_PRE_STANDBY(p)			((SWTC_TimeType)((p == NULL)? 150 : OFFS_B(p, 1)))	/* TimeOut prima di andare in StandBy, Se il parametro non è definito in setting prende 150x100ms = 15s */

#define HKM_N_MAX_HITPOINTS			3

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void STB_HarakiriManager(void);
void STB_CheckStandByCounter(void);
void STB_InitStandBy_AfterBackUp (void);
#ifdef STANDBY_EXCEPTION_PROCEDURE
  void STB_Standby_Exception_Proc(void);
  uint8_t STB_Standby_Exception_Restore(void);
#endif /*STANDBY_EXCEPTION_PROCEDURE */	

#endif /* (SBY_STANDBY_ENABLE == 1) */

#endif /* __STANDBY_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
