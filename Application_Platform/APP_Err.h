/**
\defgroup	APP_Err Modulo Application Platform Errors
\ingroup	ApplPlatform

Il modulo contiene le definizioni dei codici di ritorno e di errore
comuni a tutte le applicazioni.
*/

/**
Header file del modulo Application Platform Errors

\file		APP_Err.h
\ingroup	APP_Err
\date		07/07/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_ERR_H
#define __APP_ERR_H

/* ******** */
/* Includes */
/* ******** */
#include "APP_Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
\name Codici di ritorno standard per l'Applicazione
*/
/*@{*/ 
#define S_OK						0x00	/**< Ok */
#define S_FALSE						0x01	/**< Busy */
/*@}*/ 

/**
\name Maschera per codici di ritorno con errore o warning
*/
/*@{*/ 
#define ERROR_WARNING_CODE_MASK		0xFE	/**< */
/*@}*/ 

/**
\name Offset per la generazione dei codici di errore
*/
/*@{*/ 
#define ERR_CODE					0x80	/**< */
/*@}*/ 

/**
\name Macro per la generazione dei codici di errore e warning
*/
/*@{*/ 
/** Macro base */
#define MAKE_CODE(FN, TYP, cod)		((uint8_t)((FN##_##TYP##_START_CODE) + (cod)))
/** Macro codice di warning */
#define MAKE_WRN_CODE(FN, cod)		MAKE_CODE(FN, WRN, (cod))
/** Macro codice di errore */
#define MAKE_ERR_CODE(FN, cod)		MAKE_CODE(FN, ERR, (cod)) 
/*@}*/ 

#ifdef _NEW_ERR_CODES_
/**
\name Valore di partenza dei codici dei moduli comuni a tutte le applicazioni
*/
/*@{*/ 
#define LDR_WRN_START_CODE			0x02	/**< Modulo CL_Loader, max 10 codici */
#define LDR_ERR_START_CODE			(ERR_CODE + LDR_WRN_START_CODE)		/**< */

#define ITP_WRN_START_CODE			0x0C 	/**< Modulo CL_Interpreter, max 6 codici */    
#define ITP_ERR_START_CODE			(ERR_CODE + ITP_WRN_START_CODE)		/**< */

#define UP_SST_WRN_START_CODE		0x12	/**< Modulo SetState, max 2 codici */
#define UP_SST_ERR_START_CODE		(ERR_CODE + UP_SST_WRN_START_CODE)	/**< */

#define UP_JIF_WRN_START_CODE		0x14	/**< Modulo JumpIf, max 6 codici */
#define UP_JIF_ERR_START_CODE		(ERR_CODE + UP_JIF_WRN_START_CODE)	/**< */

#define UP_AAL_WRN_START_CODE		0x1A	/**< Modulo ActivateAlgorythm, max 4 codici */
#define UP_AAL_ERR_START_CODE		(ERR_CODE + UP_AAL_WRN_START_CODE)	/**< */
/*@}*/ 


/**
\name Codici di output per il modulo CL_Loader (max 10)
*/ 
/*@{*/ 
/* Warning */
#define LDR_WRN_LENGHT					MAKE_WRN_CODE(LDR, 0)	/**< Lunghezza campo nulla !! */

/* Errori */
#define LDR_ERR_INVALID_DISPL			MAKE_ERR_CODE(LDR, 0)	/**< Displacement = 0xFF */
#define LDR_ERR_OVFL_CYCLES				MAKE_ERR_CODE(LDR, 1)	/**< Il ciclo specificato e' >= N_MAX_CYCLES_PC */
#define LDR_ERR_OVFL_PHASES				MAKE_ERR_CODE(LDR, 2)	/**< La fase specificata e' >= N_MAX_PHASES */
#define LDR_ERR_INVALID_PHASE			MAKE_ERR_CODE(LDR, 3)	/**< La fase specificata e' > del numero di fasi appartenenti al ciclo */
#define LDR_ERR_MACRO_NULL				MAKE_ERR_CODE(LDR, 4)	/**< Macro nulla (le macro iniziano da 1 ) */
#define LDR_ERR_STATEMENT_MISMATCH		MAKE_ERR_CODE(LDR, 5)	/**< Per la stessa macro il n° degli highstatements differisce dal n° dei lowstatements */
#define LDR_ERR_OVFL_EXTRAPTR			MAKE_ERR_CODE(LDR, 6)	/**< Overflow del numero di extra-pointer */
#define LDR_ERR_INVALID_POINTER			MAKE_ERR_CODE(LDR, 7)	/**< E' stato trovato un puntatore NULL o 0xFFFF durante il caricamento */
#if (MULTI_COMPARTMENT_APPL == 1)
	#define LDR_ERR_OVFL_SELECTORS		MAKE_ERR_CODE(LDR, 8)	/**< Il vano specificato e' >= APP_N_COMP */
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
/*@}*/ 

/**
\name Codici di output per il modulo CL_Interpreter (max 6)
*/ 
/*@{*/ 
/* Errori */
#define ITP_ERR_UNKNOWN_OPCODE			MAKE_ERR_CODE(ITP, 0)	/**< Opcode microprogramma non noto */
#define ITP_ERR_PH_STAT_OUT_OF_RANGE	MAKE_ERR_CODE(ITP, 1)	/**< Numero statement fase out of range */
#define ITP_ERR_PHASE_LOADER			MAKE_ERR_CODE(ITP, 2)	/**< Errore nel caricamento dati relativi alla fase */
#if ( ITP_JIF_RET_ENABLE == 1 )
	#define ITP_ERR_WRONG_COMMAND		MAKE_ERR_CODE(ITP, 3)	/**< Comando non ammesso */
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */
/*@}*/ 

/**
\name Codici di output per il modulo SetState (max 2)
*/ 
/*@{*/ 
/* Errori */
#define SST_ERROR_WRONG_PARAM			MAKE_ERR_CODE(UP_SST, 0)	/**< La combinazione F1F0 non e' tra quelle previste */
/*@}*/ 

/**
\name Codici di output per il modulo JumpIf (max 6)
*/ 
/*@{*/ 
/* Errori */
#define JIF_ERR_LD_DATA_TAB				MAKE_ERR_CODE(UP_JIF, 0)	/**< Errore nel caricamento della tabella dati del JumpIF */
#define JIF_ERR_COND_DATA_LEN			MAKE_ERR_CODE(UP_JIF, 1)	/**< Errore nella lunghezza del capo dati di una condizione */
#define JIF_ERROR_LD_COND_DATA			MAKE_ERR_CODE(UP_JIF, 2)	/**< Errore nel caricamento dei dati di una condizione */
#define JIF_ERR_OP_OR_DATA_SEL			MAKE_ERR_CODE(UP_JIF, 3)	/**< Errore nella selezione del dato e/o dell'operando di una condizione */
/*@}*/ 


/**
\name Codici di output per il modulo ACTIVATE ALGORYTHM (max 4)
*/ 
/*@{*/ 
/* Errori */
#define AAL_ERR_LD_DATA_TAB				MAKE_ERR_CODE(UP_AAL, 0)	/**< Errore nel caricamento delle tabelle del up */

/*@}*/ 


#endif /* _NEW_ERR_CODES_ */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __APP_ERR_H */

/* *********** */
/* End of file */
/* *********** */
