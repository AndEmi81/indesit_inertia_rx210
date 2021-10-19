/**
Header file del modulo Interprete

\file		APP_Interpreter.h
\ingroup	APP_Interpreter
\date		23/07/2008
\version	01.00.00
\author		Valeria Catani, Simone Costantini, Stefano Ughi, Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_INTERPRETER_H 
#define __APP_INTERPRETER_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
\name Codici di ritorno della routine ITP_programFlowInterpreter()
*/
/*@{*/ 
#define END_STEP			0x00	/**< Fine passo */
#define END_PHASE			0x01	/**< Fine fase */
#define END_CYCLE			0x02	/**< Fine ciclo naturale */
#define END_CYCLE_RESET		0x03	/**< Fine ciclo per microprogramma Reset */
#define SAME_STEP			0x04	/**< Rimane in esecuzione lo stesso passo */
#define PROGRAM_FLOW_BREAK	0x05	/**< Interruzione del flusso normale (per JumpIf) */
/*@}*/ 

/**
\name Inizializzazione interprete
*/
/*@{*/ 
/** Ricaricare la fase, ripartire da passo 0, inizializzare tutti i uProgrammi */
#define ITP_FULL_INIT		((uint8_t) 0 )
/** Ricaricare la fase, passo invariato, inizializzare selettivamente i uProgrammi */
#define ITP_PARTIAL_INIT	((uint8_t) 1 )
/*@}*/ 

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */
#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)

	extern const uint8_t Tab_phase2byte[];

#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */

/* ******************* */
/* Function prototypes */
/* ******************* */
#if (MULTI_COMPARTMENT_APPL == 1)		
/* Applicazione multivano */
  uint8_t ITP_programFlowInterpreter(uint8_t compartment);
  void ITP_interpreterFlagsInit(uint8_t compartment, uint8_t mode);
  uint8_t DummyMP( uint8_t highStatement, uint8_t lowStatement, uint8_t compartment);
#else
/* Applicazione monovano */
  uint8_t ITP_programFlowInterpreter(void);
  void ITP_interpreterFlagsInit(uint8_t mode);
  uint8_t DummyMP( uint8_t highStatement, uint8_t lowStatement);

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
	void ITP_syncUICyclePos(uint8_t compartment);
	void ITP_phaseTrackerReset(void);
#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */	
	
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

#endif /* __APP_INTERPRETER_H */

/* *********** */
/* End of file */
/* *********** */
