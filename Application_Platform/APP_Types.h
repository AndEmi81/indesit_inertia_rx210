/**
\defgroup	APP_Types Modulo Application Platform Types
\ingroup	ApplPlatform

Il modulo contiene le dichiarazioni dei tipi comuni a tutte le applicazioni.
*/

/**
Header file del modulo Application Platform Types

\file		APP_Types.h
\ingroup	APP_Types
\date		18/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_TYPES_H
#define __APP_TYPES_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "APP_Config.h"
#include "APP_Defines.h"
#include "SwTimersCounters.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if (APP_NEW_FAULT_STATE_MACHINE == 1)
/* La seguente define permette di stabilire in che modo devono essere impacchettate le strutture dati:
 * se PACK_USING_MACROS == FALSE allora viene utilizzato il metodo di impacchettamento messo a disposizione
 * dal compilatore (es: __attribute__((packed)) per il GCC, #pragma pack per altri compilatori).
 *
 * Qualora il compilatore non metta a disposizione alcun metodo di impacchettamento allora si puo'
 * definire PACK_USING_MACROS == TRUE
 * In questo modo la macro qui sotto permettono di definire le strutture dati in modo da garantire
 * il corretto impacchettamento e di accedere ai campi in modo astratto.
 */
#define PACK_USING_MACROS FALSE

#if !defined(PACK_USING_MACROS)
	#error "\n PACK_USING_MACROS must be defined TRUE or FALSE "
#endif /* PACK_USING_MACROS */

#if (PACK_USING_MACROS == FALSE)

#if defined(RN_RX200) || defined(RN_RX100)
	#define PACKED __attribute__((packed))
#else
	#error "\n PACKED must be defined for the current compiler "
#endif

/*  Le seguenti macro ripristinano il codice originale */
#define UINT16_FIELD(varname) uint16_t varname
#define UINT32_FIELD(varname) uint32_t varname
#define INT16_FIELD(varname)  int16_t varname
#define INT32_FIELD(varname)  int32_t varname
#define M_COUNTER_FIELD(varname)  SWTC_MCounterVariableType varname
#define UINT8_PTR(varname)    uint8_t *varname
#define UINT16_ARRAY(arrayname, arraysize) uint16_t arrayname arraysize
#define UINT32_ARRAY(arrayname, arraysize) uint32_t arrayname arraysize
#define INT16_ARRAY(arrayname, arraysize)   int16_t arrayname arraysize
#define INT32_ARRAY(arrayname, arraysize)   int32_t arrayname arraysize
#define LKT_SIZEOF_S(varname) sizeof(varname)
#define LKT_SIZEOF_L(varname) sizeof(varname)
#define LKT_FIELD_S(varname)  varname
#define LKT_FIELD_L(varname)  varname
#define LKT_FIELD_M_COUNTER(varname)  varname
#define LKT_FIELD_PTR(varname)  varname
#define LKT_ITEM_S(arrayname, arrayindex) arrayname arrayindex
#define LKT_ITEM_L(arrayname, arrayindex) arrayname arrayindex
#define LKT_INIT_S(initval) initval
#define LKT_INIT_L(initval) initval
#define LKT_INIT_M_COUNTER(initval) {initval}
#define LKT_INIT_PTR(varname)  varname

#elif (PACK_USING_MACROS == TRUE)

#define PACKED

/*  Macro da usare per definire strutture packed in lookup table */
#define UINT16_FIELD(varname) uint8_t varname##_S0; uint8_t varname##_S1
#define UINT32_FIELD(varname) uint8_t varname##_L0; uint8_t varname##_L1; uint8_t varname##_L2; uint8_t varname##_L3
#define INT16_FIELD(varname)  uint8_t varname##_S0; uint8_t varname##_S1
#define INT32_FIELD(varname)  uint8_t varname##_L0; uint8_t varname##_L1; uint8_t varname##_L2; uint8_t varname##_L3
#define M_COUNTER_FIELD(varname)  uint8_t varname##_S0; uint8_t varname##_S1
#define UINT16_ARRAY(arrayname, arraysize) uint8_t arrayname##_AS0 arraysize; uint8_t arrayname##_AS1 arraysize
#define UINT32_ARRAY(arrayname, arraysize) uint8_t arrayname##_AL0 arraysize; uint8_t arrayname##_AL1 arraysize; \
	                                       uint8_t arrayname##_AL2 arraysize; uint8_t arrayname##_AL3 arraysize
#define INT16_ARRAY(arrayname, arraysize) int8_t arrayname##_AS0 arraysize; int8_t arrayname##_AS1 arraysize
#define INT32_ARRAY(arrayname, arraysize) int8_t arrayname##_AL0 arraysize; int8_t arrayname##_AL1 arraysize; \
	                                       int8_t arrayname##_AL2 arraysize; int8_t arrayname##_AL3 arraysize
/*  Puntatore a uint8_t - viene espanso su 4 bytes */
#define UINT8_PTR(varname) uint8_t varname##_L0; uint8_t varname##_L1; uint8_t varname##_L2; uint8_t varname##_L3

/*  Macro per calcolare la sizeof di un campo a 16 o 32 bit definito come sopra */
#define LKT_SIZEOF_S(varname) (sizeof(varname##_S0)+sizeof(varname##_S1))
#define LKT_SIZEOF_L(varname) (sizeof(varname##_L0)+sizeof(varname##_L1)+sizeof(varname##_L2)+sizeof(varname##_L3))

/*  Macro da usare per accedere alle variabili 16 e 32 bit in LT */
#define LKT_FIELD_S(varname) ( ((uint16_t *)&(varname##_S0))[0] )
#define LKT_FIELD_L(varname) ( ((uint32_t *)&(varname##_L0))[0] )
#define LKT_FIELD_M_COUNTER(varname) ( ((uint16_t *)&(varname##_S0))[0] )
#define LKT_FIELD_PTR(varname) ( ((uint32_t *)&(varname##_L0))[0] )

/*  Macro da usare per accedere agli elemnti di un array in LT */
#define LKT_ITEM_S(arrayname, arrayindex) ( ((uint16_t *)arrayname##_AS0)arrayindex )
#define LKT_ITEM_L(arrayname, arrayindex) ( ((uint32_t *)arrayname##_AL0)arrayindex )

/*  Macro da usare per inizializzare le strutture globali */
/*  NOTA: usare solo per inizializzare a 0 o a -1 (0xFFFF) in quanto viene banalmente replicato il valore */
#define LKT_INIT_S(initval) initval, initval
#define LKT_INIT_L(initval) initval, initval, initval, initval
#define LKT_INIT_M_COUNTER(initval) initval, initval
#define LKT_INIT_PTR(initval) initval, initval, initval, initval

#else /* (PACK_USING_MACROS != FALSE) && (PACK_USING_MACROS != TRUE) */

	#error "\n PACK_USING_MACROS must be defined TRUE or FALSE "

#endif /* PACK_USING_MACROS */

#endif /*(APP_NEW_FAULT_STATE_MACHINE == 1*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Caricamento fasi */
typedef struct _PhaseLoader_Struct
{
	uint8_t nCyclePhases;		/**< Numero di fasi del ciclo */
	uint8_t nPhaseStatements;	/**< Numero di statement della fase */
	uint8_t *pHiStat;			/**< Punta all'inizio degli High Statement per il ciclo
									 e la fase corrente */
	uint8_t *pLoStat;			/**< Punta all'inizio dei Low Statement per il ciclo
									 e la fase corrente */
} PhaseLoader_Struct;

/** Function type routines microprogrammi */ 
#if (MULTI_COMPARTMENT_APPL == 1)
	typedef uint8_t (*MICROPROG) (uint8_t, uint8_t, uint8_t);
#else
	typedef uint8_t (*MICROPROG) (uint8_t, uint8_t);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */  

/** Function type routines microprogrammi */ 
#if (MULTI_COMPARTMENT_APPL == 1)
	typedef void (*ALGORITHM_PTR) (uint8_t,uint8_t);
#else
	typedef void (*ALGORITHM_PTR) (uint8_t);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */  


/** Struttura base per l'interprete */
typedef struct _Interpreter_Struct 
{
	/* byte 0 */
	union
	{
		uint8_t	b;						/**< insieme di label + subLabel */
		
		struct
		{
#if defined(__BIT_ORDER_MSB_FIRST__)
			uint8_t subLabel		:4;	/**< subLabel del ciclo in corso */
			uint8_t label			:4;	/**< label del ciclo in corso */
#else /* __BIT_ORDER_MSB_FIRST__ */
			uint8_t label			:4;	/**< label del ciclo in corso */
			uint8_t subLabel		:4;	/**< subLabel del ciclo in corso */
#endif /* !__BIT_ORDER_MSB_FIRST__ */
		}s; 

	} completeLabel;					/**< Complete Label */
	
	/* byte 1 */
	uint8_t cycle;				/**< Ciclo in esecuzione */
	/* byte 2 */
	uint8_t phase;				/**< Fase in esecuzione */
	/* byte 3 */
	uint8_t first_statement;	/**< Statement iniziale del passo in esecuzione */

	/* byte 4 */
#if defined(__BIT_ORDER_MSB_FIRST__)

	uint8_t unusedB4b67				:2;	/**< */

#if (ITP_LOADS_MGMT_ENABLE == 1)
	uint8_t loads_mgmt_activation	:1; /**< '1' richiesta gestione continuit� carichi - la routine � fuori da Interpreter */
#else
	uint8_t unusedB4b5				:1;	/**< */
#endif /*(ITP_LOADS_MGMT_ENABLE == 1)*/

#if (ITP_ZC_ENABLE == 1)
	uint8_t ZC_not_good				:1;	/**< '1' Zero Crossing valido (utilizzato da uP e gestione ingressi */
#else
	uint8_t unusedB4b4				:1;	/**< */
#endif /* (ITP_ZC_ENABLE == 1) */

#if (ITP_JIF_PHASE_ENABLE == 1)
	uint8_t jumpIF_phase_ctrl		:1;	/**< flag di controllo per Jump IF valido per fase */
#else
	uint8_t unusedB4b3				:1;	/**< */
#endif /* (ITP_JIF_PHASE_ENABLE == 1) */

	uint8_t	optionalStepInitialized	:1;	/**< '0' => i uProgrammi del passo devono essere
											inizializzati. Azzerato al power on e ad ogni
											cambio passo. I uProgrammi, pur prevedendo uno
											stato di inizializzazione, possono anche non
											processarlo (ad esempio se non richiedono
											inizializzazione al restore da backup). */

	uint8_t	stepInitialized			:1;	/**< '0' => i uProgrammi del passo devono essere
											inizializzati. Azzerato ad ogni cambio passo. Al
											restore da backup, se il ciclo salvato viene
											ripristinato, stepInitialized rimane invariato.
											Tutti i uProgrammi che prevedono uno stato di
											inizializzazione devono processarlo. */

	uint8_t phaseDataInitialized	:1;	/**< '0' => l'interprete carica i dati della fase
											 indicata in Interpreter_Data. Azzerato al power
											 on e ogni volta che per qualunque motivo si
											 cambia fase. */

#else /* __BIT_ORDER_MSB_FIRST__ */

	uint8_t phaseDataInitialized	:1;	/**< '0' => l'interprete carica i dati della fase
											 indicata in Interpreter_Data. Azzerato al power
											 on e ogni volta che per qualunque motivo si
											 cambia fase. */
	uint8_t	stepInitialized			:1;	/**< '0' => i uProgrammi del passo devono essere
											inizializzati. Azzerato ad ogni cambio passo. Al
											restore da backup, se il ciclo salvato viene
											ripristinato, stepInitialized rimane invariato. 
											Tutti i uProgrammi che prevedono uno stato di 
											inizializzazione devono processarlo. */
	uint8_t	optionalStepInitialized	:1;	/**< '0' => i uProgrammi del passo devono essere
											inizializzati. Azzerato al power on e ad ogni
											cambio passo. I uProgrammi, pur prevedendo uno
											stato di inizializzazione, possono anche non
											processarlo (ad esempio se non richiedono
											inizializzazione al restore da backup). */

#if (ITP_JIF_PHASE_ENABLE == 1)											
	uint8_t jumpIF_phase_ctrl		:1;	/**< flag di controllo per Jump IF valido per fase */
#else
	uint8_t unusedB4b3				:1;	/**< */
#endif /* (ITP_JIF_PHASE_ENABLE == 1) */  	

#if (ITP_ZC_ENABLE == 1)
	uint8_t ZC_not_good				:1;	/**< '1' Zero Crossing valido (utilizzato da uP e gestione ingressi */
#else
	uint8_t unusedB4b4				:1;	/**< */
#endif /* (ITP_ZC_ENABLE == 1) */

#if (ITP_LOADS_MGMT_ENABLE == 1)
	uint8_t loads_mgmt_activation	:1; /**< '1' richiesta gestione continuit� carichi - la routine � fuori da Interpreter */
#else
	uint8_t unusedB4b5				:1;	/**< */		
#endif /*(ITP_LOADS_MGMT_ENABLE == 1)*/
											
	uint8_t unusedB4b67				:2;	/**< */

#endif /* !__BIT_ORDER_MSB_FIRST__ */
	
  /* byte 5 */
	uint8_t mSetting;             /**< mSetting Selector. '0' mean primary SF. */

	
} Interpreter_Struct;

#if (ITP_JIF_PHASE_ENABLE == 1)
/* Struttura per gestione JUMPIF valido per Fase*/
typedef struct _JumpIfPhase_Struct
{
	uint8_t statements[MAX_NUM_JIF_PHASE][2];	/**< Hi / Lo statement del JumpIF valido per fase. Max 2 JIF gestiti */
	uint8_t index;				/**< indice JUMPIF valido per fase */ 

} JumpIfPhase_Struct;
#endif  /* (ITP_JIF_PHASE_ENABLE == 1) */

#if ( ITP_JIF_RET_ENABLE == 1 )
/* Struttura per gestione JumpIF con ritorno */
typedef struct _JsrStack_Struct
{
	unsigned char stack[3*NUM_MAX_JSR];	/**< Stack punto di ritorno JUMPIF (ciclo, fase, statement inziale passo)  */
	unsigned char index;				/**< indice per JSR */

} JsrStack_Struct;

/* Struttura per gestione punto di Return JumpIF */
typedef struct _RetPoint_Struct
{
	uint8_t cycle;				/**< ciclo di ritorno */
	uint8_t phase;				/**< fase di ritorno */
	uint8_t first_statement;	/**< indice statement iniziale passo di ritorno*/
	
} RetPoint_Struct;
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */

/** Destinazione microprogramma JumpIf */ 
typedef struct _JumpIF_Struct
{
	uint8_t cycle;				/**< Ciclo */
	uint8_t phase;				/**< Fase */
	uint8_t first_statement;	/**< Primo statement del passo */
#if (ITP_JIF_RET_ENABLE == 1)
	uint8_t ret;				/**< JumpIf con ritorno (JSR) */
#endif /* (ITP_JIF_RET_ENABLE == 1) */
} JumpIF_Struct;

/** Tabella microprogramma JumpIf nel setting file */ 
typedef struct _JumpIfTableData_Struct
{
#if defined(__BIT_ORDER_MSB_FIRST__)

	/* byte 0 */
#if (ITP_JIF_RET_ENABLE == 1)
	uint8_t ret						:1;	/**< 1 -> JumpIf con ritorno (JSR) */
#else
	uint8_t unusedB0b7				:1;	/**< */
#endif /* (ITP_JIF_RET_ENABLE != 1) */
	uint8_t sameCycle				:1;	/**< 1 -> il ciclo e' lo stesso di partenza */
	uint8_t destinationCycle		:6;	/**< Ciclo di destinazione */

	/* byte 1 */
	uint8_t unusedB1b57				:3;	/**< */
	uint8_t destinationStatement	:5;	/**< Primo statement del passo di destinazione */

	/* byte 2 */
	uint8_t conditionIndex;				/**< Indice dello spiazzamento per accedere alla
											 tabella che descrive la condizione attraverso
											 il puntatore PTR_COND_TABLE_DATA */
	/* byte 3 */
	uint8_t samePhase				:1;	/**< 1 -> la fase e' la stessa di partenza */
#if (ITP_JIF_RELATIVE_PHASE_ENABLE == 1)
	uint8_t signOffset				:1;	/**< In caso di salto relativo, indica il segno (1 -> offset negativo) */
#else
	uint8_t unusedB3b6				:1;	/**< */
#endif	/* (ITP_JIF_RELATIVE_PHASE_ENABLE != 1) */
	uint8_t destinationPhase		:6;	/**< Fase di destinazione */

#else /* __BIT_ORDER_MSB_FIRST__ */

	/* byte 0 */
	uint8_t destinationCycle		:6;	/**< Ciclo di destinazione */
	uint8_t sameCycle				:1;	/**< 1 -> il ciclo e' lo stesso di partenza */
#if (ITP_JIF_RET_ENABLE == 1)
	uint8_t ret						:1;	/**< 1 -> JumpIf con ritorno (JSR) */
#else
	uint8_t unusedB0b7				:1;	/**< */
#endif /* (ITP_JIF_RET_ENABLE != 1) */

	/* byte 1 */
	uint8_t destinationStatement	:5;	/**< Primo statement del passo di destinazione */
	uint8_t unusedB1b57				:3;	/**< */

	/* byte 2 */
	uint8_t conditionIndex;				/**< Indice dello spiazzamento per accedere alla
											 tabella che descrive la condizione attraverso
											 il puntatore PTR_COND_TABLE_DATA */
	/* byte 3 */
	uint8_t destinationPhase		:6;	/**< Fase di destinazione */
#if (ITP_JIF_RELATIVE_PHASE_ENABLE == 1)
	uint8_t signOffset				:1;	/**< In caso di salto relativo, indica il segno (1 -> offset negativo) */
#else
	uint8_t unusedB3b6				:1;	/**< */
#endif	/* (ITP_JIF_RELATIVE_PHASE_ENABLE != 1) */
	uint8_t samePhase				:1;	/**< 1 -> la fase e' la stessa di partenza */

#endif /* !__BIT_ORDER_MSB_FIRST__ */
} JumpIfTableData_Struct;

/** Function type routines JIF_Equal, JIF_Different, JIF_Greater, JIF_Smaller */ 
typedef bool_t (*FUNCOP) (int32_t, int32_t);

/** Struttura di una condizione: word di selezione del dato  */ 
typedef struct _Condition_SelDataStruct
{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* byte 0 */
		uint8_t andOr			:1;
		uint8_t unusedB0b6		:1;
		uint8_t dataIndex		:6;

		/* byte 1 */
		uint8_t operatorCode	:2;
		uint8_t dataType		:6;
#else /* __BIT_ORDER_MSB_FIRST__ */
		/* byte 0 */
		uint8_t dataIndex		:6;
		uint8_t unusedB0b6		:1;
		uint8_t andOr			:1;

		/* byte 1 */
		uint8_t dataType		:6;
		uint8_t operatorCode	:2;
#endif /* !__BIT_ORDER_MSB_FIRST__ */
} Condition_SelDataStruct;

/** Struttura di una condizione  */ 
typedef struct _Condition_Struct
{
	Condition_SelDataStruct dataSel;	/**< word di selezione del dato */
	
	union
	{
		int16_t sw;						/* accesso a word con segno */
		uint16_t uw;					/* accesso a word senza segno */

		struct
		{	
			uint8_t mask;
			uint8_t value;
		} uc;							/* accesso a byte senza segno */

		struct
		{	
			int8_t unused;
			int8_t value;
		} sc;							/* accesso a byte con segno */
		
	} operand;							/**< word dell'operando */
	
} Condition_Struct;


#if (APP_NEW_FAULT_STATE_MACHINE == 1)
typedef struct PACKED _FaultMgmt_Data_Struct
{
	/* byte 0 - 1 */
	sft_fault_codes_t faultCodes;
	/* byte 2 */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t stepChanged :1;
	uint8_t step :7;			/** step macchina a stati gestione fault */
#else /* !(defined(__BIT_ORDER_MSB_FIRST__)) */
  uint8_t step :7;			/** step macchina a stati gestione fault */
  uint8_t stepChanged :1;
#endif /* defined(__BIT_ORDER_MSB_FIRST__) */
	/* byte 3 - 4 */
	UINT16_FIELD( filterCount );	/** contatore alarm->prefault */
	/* byte 5 - 6 */	
	M_COUNTER_FIELD( timer ); 		/** timer prefault->fault e prefault->reset */
	
	/* byte 7 */
	SWTC_CounterConfigurationType cfgTimer;
}FaultMgmt_Data_Struct;

typedef struct _Fault_Compl_Struct
{
	sft_fault_codes_t faultCodes;		/** codici di fault */
	sft_fault_codes_t prefaultCodes;	/** codici prefault (non necessariamente coincidono con i codici di fault) */
} Fault_Compl_Struct;

typedef struct PACKED _LastFault_Struct{
	sft_fault_codes_t faultCodes;		/** codici di fault */
	uint8_t	faultOccurencies;			/** numero di volte che il guasto si � ripetuto */
}LastFault_Struct;

typedef struct _Timeout_struct{
    UINT16_FIELD( prefaultTime);
    uint8_t faultTime;
    uint8_t config;

}Timeout_struct;

#endif /*( APP_NEW_FAULT_STATE_MACHINE == 1)*/

#if	(SBY_STANDBY_ENABLE == 1)
/* Sby Low Power 22.01.10 */
typedef struct _Sby_LowPower_Struct
{
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t Mode		:2;	/* Modalit� Standby Low Power o cella HW  */
	uint8_t Hitpoints	:3;
	uint8_t Req			:3;	/* Richiesta Standby  */
#else /* __BIT_ORDER_MSB_FIRST__ */
	uint8_t Req			:3;	/* Richiesta Standby  */
	uint8_t Hitpoints	:3;
	uint8_t Mode		:2;	/* Modalit� Standby Low Power o cella HW  */
#endif /* !__BIT_ORDER_MSB_FIRST__ */

} Sby_LowPower_Struct;
#endif	/*(SBY_STANDBY_ENABLE == 1)*/
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __APP_TYPES_H */

/* *********** */
/* End of file */
/* *********** */

