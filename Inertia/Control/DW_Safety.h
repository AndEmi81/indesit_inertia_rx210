/**
\defgroup DW_Safety	  Modulo Sicurezze DW

Il modulo contiene le funzioni di gestione delle azioni di sicurezza

\par \b PCLint: x Errors, 2 Warnings (indentation), x Info 12/06/2008 Valeria Catani


\date    11/04/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_Safety.c

\file    DW_Safety.h
\ingroup DW_Safety
\date    11/04/2008
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
\addtogroup DW_Safety
@{*/

#ifndef __DW_SAFETY_H
#define __DW_SAFETY_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define TOUT_DOOR_CLOSE					(OFFS_B(Tab_SFT_Door.Data, 0) & 0x7F)

#define TOUT_PRESS_EMPTY_FOR_RES_OFF	(OFFS_B(Tab_SFT_PressRes.Data,0) & 0x7F)    
#define TOUT_PRESS_FULL_FOR_RES_ON		(OFFS_B(Tab_SFT_PressRes.Data,1)& 0x7F)


#define TOUT_PULSE_WARN_FOR_EV_PILOT	(OFFS_B(Tab_SFT_EV.Data,0) & 0x7F)/* 1 sec */

#define CNT_DRAIN_EMPTY					OFFS_B(Tab_SFT_EV.Data,1) 

#define CNT_DRAIN_DELAY					((OFFS_B(Tab_SFT_EV.Data,2))* 10)	/* QI 28.10.09 ritardo su scarico in pieno, tempo in sec, contatore 100 ms
 */
/* define per nuova procedura di attivazione pompa scarico in caso di impulsi out of range */ 
#define VS_DRAINSTATUS_IDLE				0
#define VS_DRAINSTATUS_ON				1
#define VS_DRAINSTATUS_OFF				2
#define VS_DRAIN_TIMEON					((Tab_SFT_EV.DataLenght >3)?(OFFS_B(Tab_SFT_EV.Data,5)& 0x7F):40) /*def 40sec */
#define VS_DRAIN_TIMEOFF				((Tab_SFT_EV.DataLenght >3)?(OFFS_B(Tab_SFT_EV.Data,6)& 0x7F):60) /*ogni  (600) sec*/

/* Safety EV */

/* stati */
#define SFT_PULSE_WARN_NO    0 
#define SFT_PULSE_WARN_INIT  1
#define SFT_PULSE_WARN_EXE   2

/* modalità */
#define SFT_PULSE_WARN_SAFETY_MODE 0
#define SFT_PULSE_WARN_FAULT_MODE  1

/* Safety Zero crossing */
#if (ITP_ZC_ENABLE == 1)
#define SFT_ZC_GOOD_FILTER_TIME		50 /* x 100 msec */
#endif /* ITP_ZC_ENABLE */

/* stati check EV */
#define LOAD_EV_OK			 0
#define LOAD_EV_PAUSE_ON	 1
#define LOAD_EV_PAUSE_END	 2
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* stati per tentativi pilotaggio */
enum
{
  LOAD_EV_PILOT_INIT= 0,
  LOAD_EV_PILOT_1,
  LOAD_EV_PILOT_PAUSE_1,
  LOAD_EV_PILOT_2,
  LOAD_EV_PILOT_PAUSE_2,
  LOAD_EV_PILOT_END,
  LOAD_EV_PILOT_OK,

};

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

void SFT_safetyManagement(void);
void SFT_uPdoorStatus_Mgmt(void);
void SFT_pilotLoadEV_Mgmt(uint8_t mode);
uint8_t SFT_checkLoadEV(uint8_t mode);

#endif /* __DW_SAFETY_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
