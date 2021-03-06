/**
\defgroup ApplPlatform 	Application Platform Modules
*/

/**
\defgroup	APP_Config Modulo Application Platform Config
\ingroup	ApplPlatform

Il modulo contiene le defines che permettono di configurare i moduli in Application Platform
a seconda dell'applicazione, che viene selezionata in base ad una define inserita al livello
piu' alto:

\b CodeWarrior: 	Edit - P&E Settings - Compiler - Options - Language - Preprocessor Definitions \n
\b Hew: 			Build - H8S,H8/300 Standard Toolchain - (project level)C/C++ - Show entries for: Defines \n

Le define attualmente utilizzate sono:

\b WD_ARCADIA:
  wash & dryer machine. 	 \n
\b DW_DW_DISHWASHER:
	lavastoviglie  \n
\b CK_HOTTIMA:			
	forno  \n
\b DR_DRYER:
	nuovo scheda dryer  \n
\b CL_ARTICA:
	piattaforma Cooling \n
\b CK_HOOD:
	nuova scheda cappe con micro AG128 e piattaforma 2.0 \n
*/

/**
Header file del modulo Application Platform Config

\file		APP_Config.h
\ingroup	APP_Config
\date		22/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
/********************************************************************/
/**
\name Distinzione prodotti monovano/multivano
*/
/*@{*/ 
/** 0 -> applicazione monovano, 1 -> applicazione con piu' di un vano/cavita' */
#if  defined (CK_HOTTIMA) || defined (CL_ARTICA)

	#define MULTI_COMPARTMENT_APPL	1

#elif  defined (WD_ARCADIA) || defined (DW_DISHWASHER) || \
	   defined (CK_HOOD) || defined (DR_DRYER)

	#define MULTI_COMPARTMENT_APPL	0

#else

	#error "\n MULTI_COMPARTMENT_APPL property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 


/********************************************************************/
/**
\name Numero massimo di vani/cavita' previsti nell'applicazione
*/
/*@{*/ 
/** Numero massimo di vani */
#if ( MULTI_COMPARTMENT_APPL == 1 )

#if  defined (CL_ARTICA)

	#define APP_N_COMP				4

#elif defined (CK_HOTTIMA)

	#define APP_N_COMP				2
	
#else

	#error "\n APP_N_COMP not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

#else /* ( MULTI_COMPARTMENT_APPL == 1 ) */

	#define APP_N_COMP				1

#endif /* ( MULTI_COMPARTMENT_APPL == 0 ) */
/*@}*/ 


/********************************************************************/
/**
\name Numero massimo di cicli per vano
*/
/*@{*/ 
/** Numero max di cicli per vano */
#if defined (CL_ARTICA)

	#define N_MAX_CYCLES_PC			16

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (CK_HOOD)

	#define N_MAX_CYCLES_PC			64
	
#elif  defined (DR_DRYER) || defined (WD_ARCADIA)

	#define N_MAX_CYCLES_PC			128	/*in Arcadia2 e dryer possiamo avere fino a 128 cicli*/

#else

	#error "\n N_MAX_CYCLES_PC not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 


/********************************************************************/
/**
\name Puntatori e Opcode dei microprogrammi comuni
*/
/*@{*/ 
/** Microprogramma SetState */
#if  defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (CL_ARTICA) || \
	defined (CL_HOOD) || defined (DR_DRYER) || defined (WD_ARCADIA)

	#define APP_PTR_MP_SETSTATE_DATA	0x0010

	#define MP_SETSTATE_OPCODE			0x00

#else

	#error "\n SetState pointer or opcode not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/** Microprogramma JumpIf */
#if defined (CL_ARTICA)

	#define APP_PTR_MP_JUMPIF_DATA		((uint16_t)0x001E)

	#define MP_JUMPIF_OPCODE			0x0E

#elif defined ( CK_HOTTIMA)

	#define APP_PTR_MP_JUMPIF_DATA		((uint16_t)0x0016)

	#define MP_JUMPIF_OPCODE			0x06

#elif defined (WD_ARCADIA)

	#define APP_PTR_MP_JUMPIF_DATA		((uint16_t)0x0017)

	#define MP_JUMPIF_OPCODE			0x07

#elif defined (DW_DISHWASHER)

	#define APP_PTR_MP_JUMPIF_DATA		((uint16_t)0x001B)

	#define MP_JUMPIF_OPCODE			0x0B

#elif defined (CK_HOOD) || defined (DR_DRYER) 

	#define APP_PTR_MP_JUMPIF_DATA		0x0014

	#define MP_JUMPIF_OPCODE			0x04
#else

	#error "\n JumpIf pointer or opcode not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 

/** Microprogramma ActivateAlgorhythm */
#if  defined (CL_ARTICA)

	#define APP_PTR_MP_ACTIVATEALGO_DATA		((uint16_t)0x0017)

#elif defined ( CK_HOTTIMA)

	#define APP_PTR_MP_ACTIVATEALGO_DATA		((uint16_t)0x0017)

#elif  defined (WD_ARCADIA)

	#define APP_PTR_MP_ACTIVATEALGO_DATA		((uint16_t)0x0018)

#elif defined (DW_DISHWASHER)

	#define APP_PTR_MP_ACTIVATEALGO_DATA		((uint16_t)0x001A)

#elif defined (CK_HOOD) || defined (DR_DRYER)

	#define APP_PTR_MP_ACTIVATEALGO_DATA		((uint16_t)0x0015)

#else

	#error "\n ActivateAlgo pointer not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 



/********************************************************************/
/**
\name Funzionalita' del microprogramma JumpIf
*/
/*@{*/ 
/**
0 -> gestione condizioni combinate in AND/OR disabilitata, 1 -> gestione
condizioni combinate in AND/OR abilitata
*/
#if defined (CL_ARTICA)

	#define JIF_AND_OR_ENABLE	1

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (DR_DRYER) || \
	defined (WD_ARCADIA) || defined (CK_HOOD)

	#define JIF_AND_OR_ENABLE	0

#else

	#error "\n JIF_AND_OR_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/


/********************************************************************/
/**
\name Funzionalita' del microprogramma SetState
*/
/*@{*/ 
/**
0 -> nel mp SetState NON e' abilitata la modalita' IDROSTOP,
1 -> nel mp SetState e' abilitata la modalita' IDROSTOP (solo per piattaforma ARCADIA)
*/
#if defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || \
 defined (CL_ARTICA) || defined (CK_HOOD) || defined (DR_DRYER)

	#define SST_IDROSTOP_ENABLE		0

#elif defined (WD_ARCADIA)

	#define SST_IDROSTOP_ENABLE		1

#else

	#error "\n SST_IDROSTOP_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/**
0 -> nel mp SetState NON e' abilitata la modalita' NOP-NOP,
1 -> nel mp SetState e' abilitata la modalita' IDROSTOP
*/
#if defined (CL_ARTICA)

	#define SST_NOPNOP_ENABLE		0

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (DR_DRYER) || \
	defined (WD_ARCADIA) || defined (CK_HOOD)

	#define SST_NOPNOP_ENABLE		1

#else

	#error "\n SST_NOPNOP_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/**
0 -> nel mp SetState NON e' abilitata la modalita' COOL STATE,
1 -> nel mp SetState e' abilitata la modalita' COOL STATE
*/
#if defined (CL_ARTICA)  || defined  (CK_HOTTIMA) || defined (WD_ARCADIA) || defined (DW_DISHWASHER) || defined (CK_HOOD)

	#define SST_COOLSTATE_ENABLE		0

#elif defined (DR_DRYER)

	#define SST_COOLSTATE_ENABLE		1

#else

	#error "\n SST_COOLSTATE_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 


/********************************************************************/
/**
\name Funzionalita' del microprogramma JumpIf - Modulo Interprete
*/
/*@{*/ 
/** 0 -> JumpIf con ritorno (JSR) disabilitato, 1 -> JumpIf con ritorno (JSR) abilitato */
#if defined (CK_HOTTIMA) ||  defined (DR_DRYER) || \
	defined (CL_ARTICA) || defined (WD_ARCADIA) || defined (CK_HOOD)

	#define ITP_JIF_RET_ENABLE	0

#elif defined (DW_DISHWASHER)

	#define ITP_JIF_RET_ENABLE	1

#else

	#error "\n ITP_JIF_RET_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/**
0 -> gestione JUMP IF valido per fase disabilitata, 1 -> gestione
JUMP IF valido per fase abilitata
 */
#if defined (DR_DRYER) || 	defined (CL_ARTICA) || defined (WD_ARCADIA)

	#define ITP_JIF_PHASE_ENABLE	0

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (CK_HOOD)

	#define ITP_JIF_PHASE_ENABLE	1

#else

	#error "\n ITP_JIF_PHASE_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/**
0 -> gestione JUMP IF per salto a fase relativo disabilitata, 
1 -> gestione JUMP IF per salto a fase relativo abilitata
 */
#if defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (CL_ARTICA) || defined (CK_HOOD)

	#define ITP_JIF_RELATIVE_PHASE_ENABLE	0

#elif defined (DR_DRYER) || defined (WD_ARCADIA)

	#define ITP_JIF_RELATIVE_PHASE_ENABLE	1

#else

	#error "\n ITP_JIF_RELATIVE_PHASE_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/*@}*/ 


/********************************************************************/
/**
\name Funzionalita' Interprete
*/
/*@{*/ 
/**
0 -> gestione carichi (continuita') disabilitata, 1 -> gestione
carichi (continuita') abilitata
*/
#if defined (CL_ARTICA)

	#define ITP_LOADS_MGMT_ENABLE	0

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || defined (DR_DRYER) || \
	defined (WD_ARCADIA) ||  defined (CK_HOOD)

	#define ITP_LOADS_MGMT_ENABLE	1

#else

	#error "\n ITP_LOADS_MGMT_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

/**
0 -> gestione Zero Crossing valido per uPs disabilitata, 1 -> gestione
Zero Crossing valido per uPs abilitata
*/
#if defined (CL_ARTICA)

	#define ITP_ZC_ENABLE	0

#elif defined (CK_HOTTIMA)  || defined (DR_DRYER) || defined (WD_ARCADIA) ||  defined (CK_HOOD)||defined (DW_DISHWASHER)

	#define ITP_ZC_ENABLE	1

#else

	#error "\n ITP_ZC_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */

#if ( MULTI_COMPARTMENT_APPL == 0 )
/**
0 -> gestione UI Phase Tracker per UI disabilitata, 1 -> gestione
UI Phase Tracker per UI abilitata
*/

#if defined (DW_DISHWASHER) || defined (DR_DRYER) || defined (WD_ARCADIA)

	#define ITP_UI_PHASE_TRAKER_ENABLE  1

#elif  defined (CK_HOOD) || defined (CL_ARTICA)

	#define ITP_UI_PHASE_TRAKER_ENABLE  0

#else

	#error "\n ITP_UI_PHASE_TRAKER_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */ 

#else /* ( MULTI_COMPARTMENT_APPL == 0 ) */

	#define ITP_UI_PHASE_TRAKER_ENABLE  0

#endif /* ( MULTI_COMPARTMENT_APPL != 0 ) */
/*@}*/ 


/********************************************************************/
/**
\name Abilitazione modulo StandBy
*/
/*@{*/ 
/** 0 -> modulo standby disabilitato, 1 -> modulo standby abilitato */
#if defined (CL_ARTICA)

	#define SBY_STANDBY_ENABLE		0
	#define STANDBY_RESTORE_ONLY_IN_SLEEP  0
	#define STANDBY_EXCEPTION_PROCEDURE    0

#elif defined (CK_HOTTIMA) || defined (DR_DRYER) || defined (DW_DISHWASHER) || \
   defined (WD_ARCADIA) ||  defined (CK_HOOD) 

	#define SBY_STANDBY_ENABLE		1

	#if defined (WD_ARCADIA) || defined (DW_DISHWASHER) || defined (DR_DRYER)
/* Hood e Hottima hanno la stessa gestione: salvano sempre lo stato dello standby non solo quando si ? in sleep mode gli altri prodotti no.*/

  	#define STANDBY_RESTORE_ONLY_IN_SLEEP  1
  	#define STANDBY_EXCEPTION_PROCEDURE   0
	
	#elif defined (CK_HOTTIMA) 
	
	   #define STANDBY_EXCEPTION_PROCEDURE    1
	   #define STANDBY_RESTORE_ONLY_IN_SLEEP  0
	   
	 #else
	 /* qui ci finisce anche HOOD*/
	   #define STANDBY_EXCEPTION_PROCEDURE   0
	   #define STANDBY_RESTORE_ONLY_IN_SLEEP  0
	   
	#endif
	
	#if  defined (WD_ARCADIA) || defined (CK_HOTTIMA) || defined (DR_DRYER)
		#define PILOT_NORM_STANDBY_PIN      1
	#else
		#define PILOT_NORM_STANDBY_PIN      0
	#endif
		
#else

	#error "\n SBY_STANDBY_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 




/********************************************************************/
/**
\name Abilitazione modulo ActivateAlgorhythm
*/
/*@{*/ 
/** 0 -> modulo ActivateAlgorhythm disabilitato, 1 -> modulo ActivateAlgorhythm abilitato */
#if defined (CL_ARTICA)

	#define AAL_ACTIVATEALGORHYTHM_ENABLE		0

#elif defined (CK_HOTTIMA) || defined (DW_DISHWASHER) || \
	 defined (DR_DRYER) || defined (WD_ARCADIA) ||  defined (CK_HOOD)

	#define AAL_ACTIVATEALGORHYTHM_ENABLE		1
	#if defined (CK_HOOD) || defined (DR_DRYER)
		#define AAL_16_ALGO						1
	#else
		#define AAL_16_ALGO						0
	#endif
 
#else

	#error "\n AAL_ACTIVATEALGORHYTHM_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 

/* nuove define applicazione */
#if defined (CK_HOOD) 

	#define SWTC_PLATFORM_COUNTERS_NUM 2
	#define SW_TIMER_NORM_MODE			FALSE
	#define APP_NEW_FAULT_STATE_MACHINE  0
	#define FAULT_ARE_RESTORABLE
#elif  defined (CK_HOTTIMA)
		#define FAULT_ARE_RESTORABLE
		#define APP_NEW_FAULT_STATE_MACHINE  1
		#define SWTC_PLATFORM_COUNTERS_NUM 0
		#define SW_TIMER_NORM_MODE			TRUE 
#elif defined (DR_DRYER)
#define APP_NEW_FAULT_STATE_MACHINE  1
		#define SWTC_PLATFORM_COUNTERS_NUM 0
		#define SW_TIMER_NORM_MODE			FALSE
#elif   defined (DW_DISHWASHER)
	  #define APP_NEW_FAULT_STATE_MACHINE  1
		#define SWTC_PLATFORM_COUNTERS_NUM 2
		#define SW_TIMER_NORM_MODE			FALSE 
		
#else
		#define APP_NEW_FAULT_STATE_MACHINE  1
		#define SWTC_PLATFORM_COUNTERS_NUM 0
		#define SW_TIMER_NORM_MODE			TRUE 
#endif		

/********************************************************************/
/**
\name Abilitazione globale controllo errori di esecuzione nei moduli applicazione
*/
/*@{*/ 
/** 0 -> controllo errori disabilitato, 1 -> controllo errori abilitato */
#if defined (CK_HOTTIMA) ||	defined (DR_DRYER) || defined (CL_ARTICA) || \
	defined (WD_ARCADIA) ||  defined (CK_HOOD) || defined (DW_DISHWASHER)

	#define CHECKERRORS_ENABLE		1

#else

	#error "\n CHECKERRORS_ENABLE property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 


#if (CHECKERRORS_ENABLE == 1)
/********************************************************************/
/**
\name Abilitazione controllo errori per i singoli moduli in Application Platform
*/
/*@{*/ 
/**
Se le define sono presenti il controllo degli errori nei rispettivi moduli
e' abilitato
*/
/**
Se la define e' presente sono utilizzati codici di errore standard
*/
#define _NEW_ERR_CODES_

#if  defined (CK_HOTTIMA) || defined (CL_ARTICA)

	#define _JIF_CHECKERRORS_	/**< Errori modulo Jump If */

	#define _ITP_CHECKERRORS_	/**< Errori modulo Interprete */

	#define _SST_CHECKERRORS_	/**< Errori modulo SetState */

/*	#define _AFL_CHECKERRORS_ */ /* Se serve aggiungere una define specifica J_J */

#elif defined (WD_ARCADIA) || defined (CK_HOOD) || defined (DR_DRYER) || defined (DW_DISHWASHER)


#else

	#error "\n Application Platform Modules Error Enable not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 


/********************************************************************/
/**
\name Dimensionamento del buffer Debug_Error[]
*/
/*@{*/ 
/** Lunghezza del buffer in cui vengono raccolti eventuali errori run-time */
#if defined (CK_HOTTIMA) || defined (WD_ARCADIA) ||  defined (CK_HOOD) || defined (DR_DRYER) || defined (DW_DISHWASHER) 

	#define DEBUG_BUFFER_LEN				20

#elif defined (CL_ARTICA)

	#define DEBUG_BUFFER_LEN				10

#else

	#error "\n DEBUG_BUFFER_LEN property not defined in APP_Config.h "

#endif	/* Elenco applicazioni */
/*@}*/ 

#endif /* (CHECKERRORS_ENABLE == 1) */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __APP_CONFIG_H */

/* *********** */
/* End of file */
/* *********** */
