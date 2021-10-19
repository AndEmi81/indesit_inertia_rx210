/**
\defgroup	APP_Defines Modulo Application Platform Defines
\ingroup	ApplPlatform

Il modulo contiene le defines comuni a tutte le applicazioni.
*/

/**
Header file del modulo Application Platform Defines

\file		APP_Defines.h
\ingroup	APP_Defines
\date		18/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_DEFINES_H
#define __APP_DEFINES_H

/* ******** */
/* Includes */
/* ******** */
#include "APP_Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* ********** DATA TYPES RANGE ********** */
/**
\name Data Type Ranges
*/
/*@{*/ 
#define INT8_T_MAX				127					/**< Valore massimo signed char */
#define INT8_T_MIN				(-INT8_T_MAX - 1)	/**< Valore minimo signed char */
#define UINT8_T_MAX				0xFF				/**< Valore massimo unsigned char */
#define UINT8_T_MIN				0U					/**< Valore minimo unsigned char */
#define INT16_T_MAX				32767				/**< Valore massimo signed short */
#define INT16_T_MIN				(-INT16_T_MAX - 1)	/**< Valore minimo signed short */
#define UINT16_T_MAX			0xFFFF				/**< Valore massimo unsigned short */
#define UINT16_T_MIN			0U					/**< Valore minimo unsigned short */
#define INT32_T_MAX				2147483647L			/**< Valore massimo signed long */
#define INT32_T_MIN				(-INT32_T_MAX - 1)	/**< Valore minimo signed long */
#define UINT32_T_MAX			0xFFFFFFFFUL		/**< Valore massimo unsigned long */
#define UINT32_T_MIN			0UL					/**< Valore minimo unsigned long */
/*@}*/

/* ********** BITMASK ********** */
/**
\name Bitmask
*/
/*@{*/ 
#define b0  0x0001	/**< */
#define b1  0x0002	/**< */
#define b2  0x0004	/**< */
#define b3  0x0008	/**< */
#define b4  0x0010	/**< */
#define b5  0x0020	/**< */
#define b6  0x0040	/**< */
#define b7  0x0080	/**< */
#define b8  0x0100	/**< */
#define b9  0x0200	/**< */
#define b10 0x0400	/**< */
#define b11 0x0800	/**< */
#define b12 0x1000	/**< */
#define b13 0x2000	/**< */
#define b14 0x4000	/**< */
#define b15 0x8000	/**< */
/*@}*/ 

/* ********** PUNTATORI FILE DI SETTING ********** */
/**
\name Puntatori comuni nel file di setting
*/
/*@{*/ 
#define APP_PTR_SERIAL_NUMBER_DATA			((uint16_t)0x0000)	/**< Dati identificativi dell'elettrodomestico */
#define APP_PTR_MAIN_EXT_PROCESSOR			((uint16_t)0x0001)	/**< Parametri schede esterne */
#define APP_PTR_EXPANSION_BOARD				((uint16_t)0x0002)	/**< Dati schede di espansione */
#define APP_PTR_EXPANSION_FACTORY_TEST_DATA	((uint16_t)0x0003)	/**< Dati di calibrazione schede di espansione */
#define APP_PTR_EXPANSION_BOARD_SPEC_DATA	((uint16_t)0x0004)	/**< Parametri specifici schede di espansione */
#define APP_PTR_FACTORY_TEST_DATA			((uint16_t)0xFFFE)	/* NOTA: non posso mettere #if defined (WBOX) perch� la define WBOX non viene vista qui */
#define APP_PTR_MAP_LENGTH					((uint16_t)0x0007)	/**< Prima locazione successiva al file di setting (== lunghezza file di setting) */

#define APP_PTR_CHECKSUM_POSITION			((uint16_t)0x0009)	/**< Checksum file di setting */
#define APP_PTR_HW_CONFIG_DATA				((uint16_t)0x000A)	/**< Hardware Configuration */
		
#define APP_PTR_HI_STATEMENTS				((uint16_t)0x000C)	/**< High Statement microprogrammi */
#define APP_PTR_LO_STATEMENTS				((uint16_t)0x000D)	/**< Low Statement microprogrammi */

#define APP_PTR_IO_CONFIG_DATA			 	((uint16_t)0x0020)	/**< Mappe ingressi e uscite */
#define APP_PTR_IO_DEVICE_DATA				((uint16_t)0x0021)	/**< Parametri di configurazione ingressi e uscite */
#define APP_PTR_LOADS_PROFILE_DATA			((uint16_t)0x0022)	/**< Profili di pilotaggio carichi Profiler e PLP */
#define APP_PTR_UI_MAIN_REG_PARAM_TABLE		((uint16_t)0x0023)	/**< Tabelle di regolazione main per le funzioni UI */

#define APP_PTR_COND_TABLE_DATA				((uint16_t)0x0026)	/**< Tabelle delle condizioni */

#define APP_PTR_FAULT_DATA					((uint16_t)0x0027)	/**< Parametri relativi ai fault */
	
#define APP_PTR_MICROPROG_MODIFIERS_DATA	((uint16_t)0x0029)	/**< Tabelle modificatori dati microprogrammi */
#define APP_PTR_SAFETY_PARAM_DATA			((uint16_t)0x002A)	/**< Parametri sicurezze e allarmi */
#define APP_PTR_ALGORITHM_PARAM_DATA		((uint16_t)0x002B)	/**< Parametri algoritmi */

#define APP_PTR_ALGORITHM_INDEX_DATA		((uint16_t)0x002E)	/**< Indice algoritmi */
#define APP_PTR_ALGORITHM_EXPR_DATA			((uint16_t)0x002F)	/**< Espressione algoritmi */
#define APP_PTR_SELECTOR_TARGET_0			((uint16_t)0x0030)	/**< Dati composizione fasi vano 0 */
#define APP_PTR_SELECTOR_TARGET_1			((uint16_t)0x0031)	/**< Dati composizione fasi vano 1 */
#define APP_PTR_SELECTOR_TARGET_2			((uint16_t)0x0032)	/**< Dati composizione fasi vano 2 */
#define APP_PTR_SELECTOR_TARGET_3			((uint16_t)0x0033)	/**< Dati composizione fasi vano 3 */
#define APP_PTR_SELECTOR_TARGET_4			((uint16_t)0x0034)	/**< Dati composizione fasi vano 4 */
#define APP_PTR_SELECTOR_TARGET_5			((uint16_t)0x0035)	/**< Dati composizione fasi vano 5 */
#define APP_PTR_SELECTOR_TARGET_6			((uint16_t)0x0036)	/**< Dati composizione fasi vano 6 */
#define APP_PTR_SELECTOR_TARGET_7			((uint16_t)0x0037)	/**< Dati composizione fasi vano 7 */
#define APP_PTR_ALGORITHM_DATA				((uint16_t)0x0038)	/**< Dati algoritmi */
#define APP_PTR_STATISTICS_DATA				((uint16_t)0x0039)	/**< Parametri statistiche */
#define APP_PTR_UI_HW_CONFIG_DATA			((uint16_t)0x003A)	/**< Hardware Configuration User Interface */
#define APP_PTR_UI_SELECTOR_TARGET_0		((uint16_t)0x003B)	/**< Dati composizione fasi UI vano 0 */
#define APP_PTR_UI_SELECTOR_TARGET_1		((uint16_t)0x003C)	/**< Dati composizione fasi UI vano 1 */
#define APP_PTR_UI_SELECTOR_TARGET_2		((uint16_t)0x003D)	/**< Dati composizione fasi UI vano 2 */
#define APP_PTR_UI_HI_STATEMENTS			((uint16_t)0x003E)	/**< High Statement microprogrammi UI */
#define APP_PTR_UI_LO_STATEMENTS			((uint16_t)0x003F)	/**< Low Statement microprogrammi UI */
#define APP_PTR_UI_MP_NOP					((uint16_t)0x0040)
#define APP_PTR_UI_MP_SETFUNCTION			((uint16_t)0x0041)
#define APP_PTR_UI_MP_FASE_DATA				((uint16_t)0x0042)

#define APP_PTR_UI_MP_SET_FUNCTION_NEW_DATA ((uint16_t)0x0046)  /**< Microprogramma UI Set Function New */
#define APP_PTR_UI_MP_SPECIAL_ACTIONS_DATA	((uint16_t)0x0047)	/**< Microprogramma UI Special Actions */

#define APP_PTR_UI_IO_CONFIG_DATA			((uint16_t)0x0050)	/**< Mappe ingressi e uscite User Interface */
#define APP_PTR_UI_IO_DEVICE_DATA			((uint16_t)0x0051)	/**< Parametri di configurazione ingressi e uscite User Interface */
#define APP_PTR_FUNCTION_HEADING			((uint16_t)0x0052)	/**< Intestazione funzioni User Interface */
#define APP_PTR_UI_VISUAL_REG_PARAM_TABLE	((uint16_t)0x0053)	/**< Tabelle di regolazione visual per le funzioni UI */
#define APP_PTR_PROGRAM_HEADING_TARGET_0	((uint16_t)0x0054)	/**< Intestazione cicli vano 0 */
#define APP_PTR_PROGRAM_HEADING_TARGET_1	((uint16_t)0x0055)	/**< Intestazione cicli vano 1 */
#define APP_PTR_PROGRAM_HEADING_TARGET_2	((uint16_t)0x0056)	/**< Intestazione cicli vano 2 */
#define APP_PTR_FUNC_REG_MODIFIER			((uint16_t)0x0057)	/**< Tabelle di regolazione alternative */
#define APP_PTR_UI_GLOBAL_SELECTOR_DATA		((uint16_t)0x0058)	/**< UI Global Macro */
#define APP_PTR_SEQUENCER_DATA				((uint16_t)0x0059)	/**< Tabelle sequenze Visual */
#define APP_PTR_SEQUENCER_STEPS				((uint16_t)0x005A)	/**< Tabelle dei passi delle sequenze Visual */

#define APP_PTR_UI_SELECTOR_TARGET_3		((uint16_t)0x005C)	/**< Dati composizione fasi UI vano 3 */
#define APP_PTR_PROGRAM_HEADING_TARGET_3	((uint16_t)0x005D)	/**< Intestazione cicli vano 3 */
#define APP_PTR_TIME_MODIFIER				((uint16_t)0x005E)	/**< Modificatori tempo a finire */
#define APP_PTR_ECO_POWER_SAVE				((uint16_t)0x005F)	/**< Funzione Eco power save */
#define APP_PTR_WS_LEVEL_DATA				((uint16_t)0x0060)	/**< Tabelle sensore peso */
#define APP_PTR_DEFAULT_PARAM_DATA			((uint16_t)0x0061)  /**< Tabelle valori di default per ciclo e aree geografiche*/
/*@}*/ 

/* ********** ACTIVATE ALGORHYTHM ********** */
/**
\name Microprogrammi
*/
/*@{*/ 
/** Numero massimo di microprogrammi supportati */
#if (AAL_16_ALGO	 == 1)
#define N_MAX_ALG			16 
#else
#define N_MAX_ALG			8 
#endif
/*@}*/ 


/* ***************** LOADER **************** */
/**
\name Valori limite per numero fasi ed extrapointer
*/
/*@{*/ 
#define N_MAX_PHASES			N_MAX_CYCLES_PC	/**< Numero massimo di fasi in un ciclo */
#define N_MAX_EXTRAPTR_WORDS	127	/**< Numero massimo di extra-pointer in un'area dati */
/*@}*/ 



/* ********** INTERPRETE E MICROPROGRAMMI ********** */
/**
\name Microprogrammi
*/
/*@{*/ 
/** Numero massimo di microprogrammi supportati */
#define N_MAX_MICROPROG			16
/*@}*/ 

/**
\name Microprogrammi: struttura High Statements
*/
/*@{*/ 
#define T_MASK					b7	/**< Flag T (raggruppamento microprogrammi in un passo) */
#define N_MASK					b6	/**< Flag N (controllo avanzamento passo) */
#define F1						b5	/**< Opzioni microprogramma, bit 1 */
#define F0						b4	/**< Opzioni microprogramma, bit 0 */
#define OP3						b3	/**< Opcode bit 3 */
#define OP2						b2	/**< Opcode bit 2 */
#define OP1						b1	/**< Opcode bit 1 */
#define OP0						b0	/**< Opcode bit 0 */

/** Opcode */
#define OPCODE(b)				(uint8_t)( ((uint8_t)(b)) & (OP3|OP2|OP1|OP0) )
/** Opcode allargato (comprende le opzioni F1-F0) */
#define COMPLETE_OPCODE(b)		(uint8_t)( ((uint8_t)(b)) & (F1|F0|OP3|OP2|OP1|OP0) )
/*@}*/ 

/**
\name Microprogramma SetState
*/
/*@{*/ 
#define UP_SETSTATE					MP_SETSTATE_OPCODE			/**< Opcode */
	#define UP_SETSTATE_NOP			MP_SETSTATE_OPCODE			/**< Complete Opc. NOP */
	#define UP_SETSTATE_RESET		(F0|MP_SETSTATE_OPCODE)		/**< Complete Opc. RESET */

#if (SST_IDROSTOP_ENABLE == 1)
	#define UP_SETSTATE_IDROSTOP	(F1|MP_SETSTATE_OPCODE)		/**< Complete Opc. IDROSTOP */
#elif (SST_COOLSTATE_ENABLE == 1)
	#define UP_SETSTATE_COOLSTATE	(F1|MP_SETSTATE_OPCODE)		/**< Complete Opc. COOLSTATE */
#elif (ITP_JIF_RET_ENABLE == 1)
	#define UP_SETSTATE_RETURN		(F1|MP_SETSTATE_OPCODE)		/**< Complete Opc. RETURN */
#endif

#if (SST_NOPNOP_ENABLE == 1)
	#define UP_SETSTATE_NOP_NOP		(F1|F0|MP_SETSTATE_OPCODE)	/**< Complete Opc. NOP-NOP */
#endif /* (SST_NOPNOP_ENABLE == 1) */

#if (SST_IDROSTOP_ENABLE == 1) && (ITP_JIF_RET_ENABLE == 1)
	#error "\n SetState options Idrostop and Return are mutually exclusive! "
#endif

/*@}*/ 

/**
\name Microprogramma JumpIf
*/
/*@{*/ 
#define UP_JUMPIF						MP_JUMPIF_OPCODE		/**< Opcode */
	#define UP_JUMPIF_PHASE				MP_JUMPIF_OPCODE		/**< Complete Opc. JumpIf
																	 valido per fase */
	#define UP_JUMPIF_STEP				(F0|MP_JUMPIF_OPCODE)	/**< Complete Opc. JumpIf
																	 valido per passo */
	#define UP_JUMPIF_PHASE_RESET		(F1|MP_JUMPIF_OPCODE)	/**< Complete Opc. Reset JumpIf
																	 valido per fase */
/** Numero di operatori di condizione */
#define N_OPERATORS						4
/** Dimensione in word della parte selezione dati di una condizione */
#define CONDITION_SELDATA_SIZE_IN_W		( sizeof(Condition_SelDataStruct) >> 1)
/** Dimensione in word di una condizione */
#define CONDITION_SIZE_IN_W				( sizeof(Condition_Struct) >> 1)
/** Numero di categorie dati su cui fare il JumpIf */
#define NDATATYPES						32

#if (ITP_JIF_PHASE_ENABLE == 1)
	/** N� massimo di Jump If validi per fase supportati */
	#define MAX_NUM_JIF_PHASE		2	
#endif /*(ITP_JIF_PHASE_ENABLE == 1) */


#if (ITP_JIF_RET_ENABLE == 1)
	/** N� massimo JSR annidati supportati */
	#define NUM_MAX_JSR				2
	/** Valore convenzionale per ritorno in END */
	#define RETURN_END_CYCLE    	0xFF
#endif /* (ITP_JIF_RET_ENABLE == 1) */
/*@}*/ 

/**
\name Microprogramma SetVariable
*/
/*@{*/
#define UP_SETVARIABLE

#define N_MAX_STATE_VAR 4 /* Max number of state variables */

/*@}*/

/* ********** SUPERVISOR ********** */
/**
\name Intestazione cicli: defines su Heading (== I.C.)
*/
/*@{*/ 
#define LBL_PROGRAMMING			0	/**< PROGRAMMING */
#define LBL_RUN					1	/**< RUN */
#define LBL_DELAY				2	/**< DELAY */
#define LBL_END					3	/**< END */
#define LBL_PAUSE				4	/**< PAUSE */
#define LBL_FAULT				5	/**< FAULT */
/*@}*/ 


/* ********** COUNTERS ********** */
/**
\name Configurazione contatori di PLT2.0
*/
/*@{*/ 
/*
ATTENZIONE!!!!!!!

il numero di contatori definiti per l'applicazione vanno indicati
nella #define "SWTC_APP_PLT_COUNTERS_NUM" nel file di applicazione

*/

 #if (SBY_STANDBY_ENABLE == 1)
 
#define APP_PLTF_SBY_SW_COUNTERS  \
      &StbHarakiri_MCounter,			/** Contatore StandBy */
      
#define APP_PLT_SBY_SW_COUNTERS_CFG  \
      &StbHarakiri_ConfigCounter, 		/** Contatore StandBy */
      
#else

#define APP_PLTF_SBY_SW_COUNTERS

#define APP_PLT_SBY_SW_COUNTERS_CFG

#endif /* (SBY_STANDBY_ENABLE == 0) */


#if (APP_NEW_FAULT_STATE_MACHINE == 1)

  #if (MULTI_COMPARTMENT_APPL == 1)

  	#define APP_PLTF_FAULT_SW_COUNTERS  \
        &LKT_FIELD_M_COUNTER(FaultMgmt_Data[0].timer),    	/* 	Contatore procedura di set/reset fault */\
        &LKT_FIELD_M_COUNTER(FaultMgmt_Data[1].timer),     /* 	Contatore procedura di set/reset fault */
        
  	#define APP_PLTF_FAULT_SW_COUNTERS_CFG  \
             &FaultMgmt_Data[0].cfgTimer,		/* 	Contatore procedura di set/reset fault */ \
             &FaultMgmt_Data[1].cfgTimer,		/* 	Contatore procedura di set/reset fault */
      
        
  #else /* !MULTI_COMPARTMENT_APPL */

  	#define APP_PLTF_FAULT_SW_COUNTERS \
          &LKT_FIELD_M_COUNTER(FaultMgmt_Data[0].timer),    /* 	Contatore procedura di set/reset fault */
          
 	 #define APP_PLTF_FAULT_SW_COUNTERS_CFG \
          &FaultMgmt_Data[0].cfgTimer,						 /* 	Contatore procedura di set/reset fault */
                                       
        

  #endif /* MULTI_COMPARTMENT_APPL */

#else /* APP_NEW_FAULT_STATE_MACHINE */

    #define APP_PLTF_FAULT_SW_COUNTERS 
    
    #define APP_PLTF_FAULT_SW_COUNTERS_CFG
    
#endif    /*(APP_NEW_FAULT_STATE_MACHINE == 1)*/




#define APP_PLT_SW_COUNTERS \
		APP_PLTF_SBY_SW_COUNTERS \
		APP_PLTF_FAULT_SW_COUNTERS

#define APP_PLT_SW_COUNTERS_CFG	\
		APP_PLT_SBY_SW_COUNTERS_CFG \
		APP_PLTF_FAULT_SW_COUNTERS_CFG


/*
Questo elenco definisce gli ID per indicizzare le variabili contatore
memorizzate in "BEGIN__SWTC_CONFIG_TABLE" e "BEGIN__SWTC_COUNTERS_TABLE"
*/
#if (SBY_STANDBY_ENABLE == 1)

	#define STB_HARAKIRI_COUNTER_M		SWTC_PLATFORM_COUNTERS_NUM
	
	#if (APP_NEW_FAULT_STATE_MACHINE == 1)
  
	    #define    MAIN_FAULT_MGMT_COUNTER_M      (STB_HARAKIRI_COUNTER_M + 1)
    
	    #if (MULTI_COMPARTMENT_APPL == 1) 
      
	      #define  TOP_FAULT_MGMT_COUNTER_M   (MAIN_FAULT_MGMT_COUNTER_M+1)
      
	      #define SWTC_APP_PLT_COUNTERS_NUM	(TOP_FAULT_MGMT_COUNTER_M +1)
	    #else
      
	      #define SWTC_APP_PLT_COUNTERS_NUM	(MAIN_FAULT_MGMT_COUNTER_M +1)
	    #endif  /*(MULTI_COMPARTMENT_APPL == 1)*/
  #else   

	  #define SWTC_APP_PLT_COUNTERS_NUM	(STB_HARAKIRI_COUNTER_M + 1) 
	
  #endif  /* APP_NEW_FAULT_STATE_MACHINE*/
  

#else /* (SBY_STANDBY_ENABLE == 0) */

  #if (APP_NEW_FAULT_STATE_MACHINE == 1)
 
    #define    MAIN_FAULT_MGMT_COUNTER_M      SWTC_PLATFORM_COUNTERS_NUM 
    
    #if (MULTI_COMPARTMENT_APPL == 1) 
      
      #define  TOP_FAULT_MGMT_COUNTER_M   (MAIN_FAULT_MGMT_COUNTER_M+1)
      
      #define SWTC_APP_PLT_COUNTERS_NUM	(TOP_FAULT_MGMT_COUNTER_M +1)
    #else
      
      #define SWTC_APP_PLT_COUNTERS_NUM	(MAIN_FAULT_MGMT_COUNTER_M +1)
    #endif  
  #else /*  (APP_NEW_FAULT_STATE_MACHINE == 1) */
 
	  #define SWTC_APP_PLT_COUNTERS_NUM 	0
	
  #endif /*(APP_NEW_FAULT_STATE_MACHINE == 1)*/
  
#endif /* (SBY_STANDBY_ENABLE == 0) */
  
    
  
  

/* ********** CYCLE DOWNLOAD ********** */
/**
\name The follow defines are used in CYCLE DOWNLOAD
*/
/*@{*/
#define MAIN_SF_INDEX     ((uint8_t) 0 ) /**< Main Setting File Index */
/*@}*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __APP_DEFINES_H */

/* *********** */
/* End of file */
/* *********** */
