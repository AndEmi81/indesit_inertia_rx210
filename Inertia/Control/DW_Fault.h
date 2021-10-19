/**
\defgroup DW_Fault	Modulo Gestione Faults DW

Il modulo contiene le funzioni di gestione dei Fault applicazione

\par \b PCLint: x Errors, 24 Warnings: indentation, x Info 24/09/2008 Valeria Catani


\date    14/04/2008
\author  Roberto Fioravanti - Valeria Catani
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_Fault.c

\file    DW_Fault.h
\ingroup DW_Fault
\date    30/01/2008
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
\addtogroup DW_Fault
@{*/



#ifndef __DW_FAULT_H
#define __DW_FAULT_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define TOUT_RESET_PREFAULT 	((uint16_t)(0))

#define FLTN_INCONSISTENCY   1
#define FLTN_LOST_WATER      0

/*timeout overflow */
#define TOUT_OVERFLOW_FAULT_AUTOTEST     (OFFS_B(Tab_LoadFault_Special.Data,0)& 0x7F)	 /* 100 ms */
#define TOUT_OVERFLOW_PREFAULT_AUTOTEST  (OFFS_B(Tab_LoadFault_Special.Data,5)& 0x7F)	 /* 100 ms */
#define TOUT_OVERFLOW_FAULT     (OFFS_B(Tab_LoadFault.Data,0)& 0x7F)	 /* 100 ms */
#define TOUT_OVERFLOW_PREFAULT  (OFFS_B(Tab_LoadFault.Data,5)& 0x7F)	 /* 100 ms */
/*timeout EV*/
#define TOUT_BASE_LOAD_EV_FAULT    11  /* 100ms */
#define TOUT_LOAD_EV_FAULT(p)         OFFS_B(p,1)/*OFFS_B(LoadFaultData,1)	 sec */

#define TOUT_LOAD_EV_PILOT(p)         (OFFS_B(p,6)& 0x7F)	   /* 100
(OFFS_B(LoadFaultData,6)& 0x7F)	   100 msec */

/*timeout probe */
#define TOUT_PROBE_FAULT(p)        (OFFS_B(p,0)& 0x7F) /* 10 sec / 1 sec */
#define TOUT_PROBE_PREFAULT(p)     (OFFS_B(p,3)& 0x7F)	/* 10 sec / 1 sec */

/*timeout caricamento + warning */
/*#define TOUT_LOAD_FAULT         OFFS_B(LoadFaultData,2)	  sec */
/* EA modifica brevetto Bosch*/
#define TOUT_LOAD_FAULT(p)			((HWC_Flags.bit.vs_wash_presence)?(OFFS_W(p,5)& 0x7FFF):OFFS_B(p,2))
#define TOUT_WARNING_LOAD(p)       OFFS_B(p,3)	 /* sec */

/*timeout turbina bloccata */
#define TOUT_PULSE_COUNTER_STILL(p)  OFFS_B(p,4) /* 100 ms  da considerare il caso autotest*/
#define TOUT_PULSE_COUNT_CHECK_DELAY (OFFS_B(Tab_LoadFault.Data,9) & 0x7F)/*da considerare il caso autotest*/

/*timeout riscaldamento*/
#define TOUT_HEAT_FAULT(p)         OFFS_B(p,1) /* min */
#define TOUT_HEAT_PREFAULT(p)      OFFS_B(p,4) /* min */


#define TOUT_HEAT_BY_UV_PREFAULT_THRESHOLD(p)	OFFS_W(p, 3) /* sec */
#define TOUT_HEAT_BY_UV_FAULT_THRESHOLD(p)		OFFS_W(p, 4) /* sec */


#define TOUT_HEAT_BY_OH_PREFAULT_THRESHOLD(p)	OFFS_W(p, 5) /* sec */
#define TOUT_HEAT_BY_OH_FAULT_THRESHOLD(p)		OFFS_W(p, 6) /* sec */

/* timeout res open */
#define TOUT_RES_OPEN_FAULT(p)    OFFS_B(p,2) /* 10 sec / 1 sec */
#define TOUT_RES_OPEN_PREFAULT(p)  OFFS_B(p,5)  /* 10 sec / 1 sec */

/*timeout BLDC comm */
#define TOUT_BLDC_COMM_FAULT			(OFFS_B(ExtDevFaultData,0) & 0x7F)		/* 1 sec */
#define TOUT_BLDC_COMM_PREFAULT			(OFFS_B(ExtDevFaultData,1) & 0x7F)		/* 1 sec */

#define TOUT_DOOR_CIRCUIT_FAULT			(OFFS_B(Tab_MainBoardFault.Data,2) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_DOOR_CIRCUIT_PREFAULT		(OFFS_B(Tab_MainBoardFault.Data,5) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_DOOR_CIRCUIT_FAULT_AUTOTEST		(OFFS_B(Tab_MainBoardFault_Special.Data,2) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_DOOR_CIRCUIT_PREFAULT_AUTOTEST		(OFFS_B(Tab_MainBoardFault_Special.Data,5) & 0x7F)	/* 10 sec / 1 sec */

/*timeout press out of range */
#define TOUT_PRESS_CIRCUIT_FAULT		(OFFS_B(MainBoardFaultData,1) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_PRESS_CIRCUIT_PREFAULT		(OFFS_B(MainBoardFaultData,4) & 0x7F)	/* 10 sec / 1 sec */

/*timeout overflow out of range */
#define TOUT_OVERFLOW_CIRCUIT_FAULT		(OFFS_B(Tab_MainBoardFault.Data,6) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_OVERFLOW_CIRCUIT_PREFAULT	(OFFS_B(Tab_MainBoardFault.Data,7) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_OVERFLOW_CIRCUIT_FAULT_AUTOTEST		(OFFS_B(Tab_MainBoardFault_Special.Data,6) & 0x7F)	/* 10 sec / 1 sec */
#define TOUT_OVERFLOW_CIRCUIT_PREFAULT_AUTOTEST 	(OFFS_B(Tab_MainBoardFault_Special.Data,7) & 0x7F)	/* 10 sec / 1 sec */



/*timeout res cc - ca */
#define TOUT_RES_CIRCUIT_FAULT(p)    OFFS_B(p,0)  /* 10 sec / 1 sec */
#define TOUT_RES_CIRCUIT_PREFAULT(p)  OFFS_B(p,3) /* 10 sec / 1 sec */

#if ( ITP_ZC_ENABLE== 1)
#define TOUT_ZC_CIRCUIT_FAULT			30	/* 10 sec */
#define TOUT_ZC_CIRCUIT_AUTOTEST_FAULT	60	/* 1 sec */
#define TOUT_ZC_CIRCUIT_PREFAULT		50	/* 100 msec */
#endif /* ITP_ZC_ENABLE */


/** Valori consistency */

/* 
Nei codici dei consistency i bit meno significativi indicano il tipo di guasto rilevato (o
eventualmente il corretto funzionamento), i bit 3 e 4 indicano lo stato di pilotaggio del
carico.
*/

#define CONSISTENCY_FAILURE_MASK			(b0 | b1 | b2)
#define CONSISTENCY_STATE_MASK				(b3 | b4)

/* Codice base di corretto funzionamento */
#define CONSISTENCY_OK						    0x00
/* Codici base di guasto */
#define CONSISTENCY_GENERIC_ERROR			0x01
#define CONSISTENCY_FEEDBACK_KO_ERROR		0x02	/*feedback guasto */
#define CONSISTENCY_ERROR_RELE_CC 	  		0x03	/* rel� incollato */
#define CONSISTENCY_ERROR_RELE_CA		  	0x04	/* rel� che non si chiude */
#define CONSISTENCY_ERROR_RES_OPEN	  		0x05	/* resistenza interrotta o non cablata */

/* Codifica stati di pilotaggio */
#define CNSTCY_LOAD_OFF				0		/* carico non pilotato  */
#define CNSTCY_LOAD_ON				(b3)	/* carico pilotato  */


/* Codici composti */
#define CONSISTENCY_OFF_OK						(CNSTCY_LOAD_OFF | CONSISTENCY_OK)
#define CONSISTENCY_OFF_GENERIC_ERROR			(CNSTCY_LOAD_OFF | CONSISTENCY_GENERIC_ERROR)
#define CONSISTENCY_OFF_FEEDBACK_KO_ERROR		(CNSTCY_LOAD_OFF | CONSISTENCY_FEEDBACK_KO_ERROR)
#define CONSISTENCY_OFF_ERROR_RELE_CC 	  		(CNSTCY_LOAD_OFF | CONSISTENCY_ERROR_RELE_CC)	/* rilevabile solo in OFF */
#define CONSISTENCY_OFF_ERROR_RES_OPEN			(CNSTCY_LOAD_OFF | CONSISTENCY_ERROR_RES_OPEN)		/*rilevabile solo in OFF */

#define CONSISTENCY_ON_OK						(CNSTCY_LOAD_ON | CONSISTENCY_OK)
#define CONSISTENCY_ON_GENERIC_ERROR			(CNSTCY_LOAD_ON | CONSISTENCY_GENERIC_ERROR)	
#define CONSISTENCY_ON_FEEDBACK_KO_ERROR		(CNSTCY_LOAD_ON | CONSISTENCY_FEEDBACK_KO_ERROR)
#define CONSISTENCY_ON_ERROR_RELE_CA		  	(CNSTCY_LOAD_ON | CONSISTENCY_ERROR_RELE_CA)	/* rilevabile solo in ON */

/**/

#define FLT_RES_CIRCUIT_FAULT 0
#define FLT_RES_CIRCUIT_ON    1
#define FLT_RES_CIRCUIT_PREFAULT  2


#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT

#define FLT_BLDC_CODE_NO_FAULT			0x00	/* No Fault BLAC */

#define FLT_BLDC_CODE_F01				0x01	/* fault F01 BLAC */
#define FLT_BLDC_CODE_F02				0x02	/* fault F02 BLAC */


#define FLT_BLDC_CODE_LOW_VOLTAGE_WARN		0x01	/* warning BLDC: Vline under threshold -BLDC non pilotato */
#define FLT_BLDC_CODE_OH_WARN		0x88	/* warning BLDC: overheating */

#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/

/* stati gestione fault Ev sfondata */
#define FLT_LOAD_EV_KO_INIT 0
#define FLT_LOAD_EV_KO_EXE  1
#define FLT_LOAD_EV_KO_END  2


#define CHECK_EV_PULSE_CTRL 0
#define CHECK_EV_WARN_CTRL  1
#define CHECK_EV_VS_FAULT_CTRL 2


#define WRN_FAULT		1
#define WRN_PREFAULT	0
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void FLT_faultManagement(void);

uint8_t FLT_CheckPulseCounterStill(uint8_t mode);
void FLT_PulseCounterCheckDelay_Mgmt(void);
void FLT_pilotLoadEV_Mgmt(uint8_t mode);
void FLT_warningSave(uint8_t code_To_Add, uint8_t subcode_To_Add, uint8_t where_to_save);
void SFT_Timeout_4Loadrestore_Mgmt (uint8_t mode);

#endif /* __WM_FAULT_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */





