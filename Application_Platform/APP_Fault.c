/*
** **************************************************************************
** **************************************************************************
*/ 
/**
 
Gestisce la macchina a stati dei fault e l'architettura ad essi correlati.
Source file 
 
\file    	App_Fault.c
 
\version 	1.0.0
\date    	01/08/2013
\authors	Emanuela Antognoli
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

/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"
#include "LIB_ProductSafetyExp.h"
#include "StandardTypes.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
Gestione specifica Fault Motore
*/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void FLT_prefaultFaultStateManagement(uint16_t n_max_prefault, uint8_t faultTimeTO, uint8_t config);  

static void FLT_prefaultResetManagement(uint16_t config);


#if (MULTI_COMPARTMENT_APPL == 1)

static void FLT_initFaultDetectProc(uint8_t compartment);
static void FLT_checkFltStateMachineRestart( uint8_t code, uint8_t subcode, uint8_t compartment) ;
static void FLT_updateFaultStatistics(sft_fault_codes_t previousFaultCodes, sft_fault_codes_t currentFaultCodes, uint8_t compartment);
static void FLT_faultUpdateStep(uint8_t step, uint8_t compartment);

#else /* !MULTI_COMPARTMENT_APPL */

static void FLT_initFaultDetectProc(void);
static void FLT_checkFltStateMachineRestart( uint8_t code, uint8_t subcode) ;
static void FLT_updateFaultStatistics(sft_fault_codes_t previousFaultCodes, sft_fault_codes_t currentFaultCodes);
static void FLT_faultUpdateStep(uint8_t step);
#endif /*(MULTI_COMPARTMENT_APPL == 1)*/

#ifdef _STATISTICS_ON_ 

static void STC_updateBlackBoxEvent(uint8_t i, uint8_t codesMatch);

#endif

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */


/* ***************** */
/* Exported function */
/* ***************** */
/**/

/**
Aggiorna lo step della procedura di rilevazione fault e setta stepChanged a TRUE.
 
\param [in] 	step
\param [out] 	FaultMgmt_Data (global)
 
\return 		none

\notes:

\version		1.0.0
\date 			18/12/2006
\author			Alberto Camerin
*/
#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_faultUpdateStep(uint8_t step, uint8_t compartment)
#else
void FLT_faultUpdateStep(uint8_t step)
#endif /*(MULTI_COMPARTMENT_APPL == 0)*/
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif
	FaultMgmt_Data[compartment].step = step;
	FaultMgmt_Data[compartment].stepChanged = TRUE;
}
/**/


/**
Resetta la struttura ApplCurrentFault
 
\param [in] 	
\param [out]	ApplCurrentFault
 
\return 		none
\warning		

\notes:

\version		1.0.0
\date 			25/09/2006
\author			Marco Quintini
*/
#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_applCurrentFaultReset(uint8_t compartment)
#else
void FLT_applCurrentFaultReset(void)
#endif
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment=0;
#endif
	ApplCurrentFault[compartment].prefaultCodes = No_Fault;
	ApplCurrentFault[compartment].faultCodes = No_Fault;
}
/**/


/**
Resetta la struttura FaultMgmt_Data e le variabili specifiche per la rilevazione
dei singoli guasti.
 
\param [in] 	
\param [out] 	FaultMgmt_Data
 
\return 		none
\warning		

\notes:

\version		1.0.0
\date 			25/09/2006
\author			Marco Quintini
*/
#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_faultMgmtDataReset(uint8_t compartment)
#else
void FLT_faultMgmtDataReset(void)
#endif
{
#if (MULTI_COMPARTMENT_APPL == 0)
	uint8_t compartment =0;
#endif

#if (SW_TIMER_NORM_MODE	== FALSE) 
  Reset_MediumCounter((SWTC_CountersType)(MAIN_FAULT_MGMT_COUNTER_M + compartment));
#else  
	StopCounterToZero(MEDIUM_COUNTER, (SWTC_CountersType)(MAIN_FAULT_MGMT_COUNTER_M + compartment));
#endif
				
	LKT_FIELD_S(FaultMgmt_Data[compartment].filterCount) = 0;

	FaultMgmt_Data[compartment].step = FLT_IDLE_STEP;

	FaultMgmt_Data[compartment].faultCodes = No_Fault;

	/** NOTA: il campo stepChanged viene gestito all'interno delle routines 
		FLT_prefaultFaultStateManagement() e FLT_prefaultResetManagement() e deve essere 
		controllato solo dopo aver chiamato una delle due routines. */
	
}
/**/


/**
Aggiorna il buffer puntato da ptrMostRecentFault, eliminando il fault più vecchio
e introducendo in testa faultToBeSaved. Inoltre aggiorna il campo faultOccurencies
relativo all'ultimo fault salvato.
 
\param [in] 	faultToBeSaved, struttura con codice e sottocodice del nuovo fault
				da inserire
\param [in/out]	ptrMostRecentFault punta al primo elemento del buffer da aggiornare.
 
\return 		none
\warning		la dimensione del buffer da shiftare dev'essere N_MAX_LAST_FAULT

\notes:			se faultToBeSaved coincide ptrMostRecentFault->faultCodes, il buffer
				non viene shiftato, ma viene comunque incrementato il campo 
				ptrMostRecentFault->faultOccurencies

\version		1.2.0
\date 			05/07/2007
\author			Marco Quintini
*/
void FLT_lastFaultUpdate(sft_fault_codes_t faultToBeSaved, LastFault_Struct *ptrMostRecentFault)
{
uint8_t i;
LastFault_Struct *ptrLocal;

	/* Il buffer viene fatto scorrere solo se il nuovo fault è diverso dal primo
	   della lista */
	if( (faultToBeSaved.code != ptrMostRecentFault->faultCodes.code) || 
		(faultToBeSaved.subCode != ptrMostRecentFault->faultCodes.subCode) )
	{
		/* Usato il while invece del for per evitare l'unrolling loop e risparmiare codice */
		ptrLocal = (ptrMostRecentFault + (N_MAX_LAST_FAULT - 1));
		i = N_MAX_LAST_FAULT-1;
		while(i>0)
		{
			*ptrLocal = *(ptrLocal - 1);
			ptrLocal --;
			i--;
		}

		/* Dopo il loop ptrLocal punta al primo elemento del buffer da aggiornare*/
		ptrLocal->faultCodes = faultToBeSaved;
		ptrLocal->faultOccurencies = 0;
	}

	IncByteWithCheck(&(ptrMostRecentFault->faultOccurencies));
}
/**/


/**
Inizializza la procedura di rilevazione fault.
 
\param [in] 	
\param [out] 	
 
\return 		none
\warning		

\notes:

\version		1.0.0
\date 			03/10/2006
\author			Marco Quintini
*/
#if (MULTI_COMPARTMENT_APPL == 1)
static void FLT_initFaultDetectProc(uint8_t compartment)
#else
static void FLT_initFaultDetectProc(void)
#endif
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif

	FaultMgmt_Data[compartment].step = ALARM_FLAG_STEP;
}
/**/


/**
Gestisce la procedura di rilevazione fault, con le transizioni tra gli stati Alarm -> 
Prefault -> Fault.
 
\param [in/out] FaultMgmt_Data, ApplCurrentFault
\param [out] 	
 
\return 		none
\warning		

\notes:

\version		1.0.0
\date 			25/09/2006
\author			Marco Quintini
*/
static void FLT_prefaultFaultStateManagement(uint16_t n_max_prefault, uint8_t faultTimeTO, uint8_t config)
{

#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t compartment=(config & COMP_MASK)>>COMP_MASK_NUM_RIGHT_SHIFT;
#else
uint8_t compartment =0;
#endif

FaultMgmt_Data_Struct *pFaultmgmt_Data= &FaultMgmt_Data[compartment];
Fault_Compl_Struct *pApplCurrentFault = &ApplCurrentFault[compartment];
SWTC_CountersType counterIndex = MAIN_FAULT_MGMT_COUNTER_M + compartment;

	pFaultmgmt_Data->stepChanged = FALSE;

	switch (pFaultmgmt_Data->step)
	{
		case ALARM_FLAG_STEP:

			SWTC_Stop((SWTC_CountersType) counterIndex);

			/* Init contatore Alarm -> Prefault */
			LKT_FIELD_S(pFaultmgmt_Data->filterCount) = 0;

			pFaultmgmt_Data->step = PREFAULT_WAIT_STEP;

		break;

		
		case PREFAULT_WAIT_STEP:
		
	
			LKT_FIELD_S(pFaultmgmt_Data->filterCount) ++;

			if(( LKT_FIELD_S(pFaultmgmt_Data->filterCount) >= n_max_prefault )||(n_max_prefault== 0 ))
			{
				/* End conteggio Alarm -> Prefault: viene attivato il Prefault */
				LKT_FIELD_S(pFaultmgmt_Data->filterCount) = 0;
#if (MULTI_COMPARTMENT_APPL == 1)
				FLT_faultUpdateStep( (uint8_t)PREFAULT_STEP, compartment);
#else				
				FLT_faultUpdateStep((uint8_t)PREFAULT_STEP);
#endif				
				pApplCurrentFault->prefaultCodes = pFaultmgmt_Data->faultCodes;

				/* Salvataggio codici di prefault per le statistiche del ciclo di collaudo */
#ifdef _STATISTICS_ON_
#ifndef _NO_STC_TESTCYCLE_
				MachineStatistics.testCycleStats.prefaultCodes = pApplCurrentFault->prefaultCodes;
#endif
#endif /* _STATISTICS_ON_ */
				/* Aggiornamento buffer Last Prefault */
#if (MULTI_COMPARTMENT_APPL == 1)
				FLT_lastFaultUpdate(pApplCurrentFault->prefaultCodes, &MachineStatistics.lastPreFault[compartment][0]);
#else
				FLT_lastFaultUpdate(pApplCurrentFault->prefaultCodes, &MachineStatistics.lastPreFault[0]);
#endif
#ifdef _STATISTICS_ON_ 
				/* Salvataggio dati nella Black Box (se non c'è già un fault di applicazione o di piattaforma attivo) */
				if( (pApplCurrentFault->faultCodes.code == CODE_NO_FAULT) && (PltFaultCodes.code == CODE_NO_FAULT) )
				{
				
#if (MULTI_COMPARTMENT_APPL == 1)
					STC_saveBlackBoxData(pApplCurrentFault->prefaultCodes, compartment);
#else	
					STC_saveBlackBoxData(pApplCurrentFault->prefaultCodes);
#endif					
				}
#endif /* _STATISTICS_ON_ */

				/* Init timer Prefault -> Fault 
				*/
				
				if( (config & THEREATTEMPTFLT_MASK) == 0 )
				{
					#if (SW_TIMER_NORM_MODE	== FALSE) 
				    Set_MediumCounter((SWTC_CountersType)counterIndex, (SWTC_PrescalerType)(config & FLT_PRESCALER_MASK ), (SWTC_TimeType)faultTimeTO)  ;
					#else
					  Set_DownCounter(MEDIUM_COUNTER, (SWTC_CountersType)counterIndex, (SWTC_PrescalerType)(config & FLT_PRESCALER_MASK ), (SWTC_TimeType)faultTimeTO);
					#endif  
				}
				else
				{
					/*
					Il contatore non è utilizzato nei fault gestiti mediante tentativi: in questo caso per il passaggio da prefault
					a fault si contano i tentativi, mediante pFaultmgmt_Data->filterCount, che risulta già azzerato
					*/
				}
			}

		break;


		case PREFAULT_STEP:

			if (config & THEREATTEMPTFLT_MASK)
			{
				/* EA: nella gestione di eventuali tentativi anche in fault, questa variabile viene incrementata ogni volta che viene chiamata la funzione*/

				LKT_FIELD_S(pFaultmgmt_Data->filterCount) ++;
			}
			else
			{
				SWTC_Run((SWTC_CountersType) counterIndex);
			}

			if( (SWTC_IsExpired((SWTC_CountersType)counterIndex)&& ((config & THEREATTEMPTFLT_MASK)==0) ) ||
					(( LKT_FIELD_S(pFaultmgmt_Data->filterCount) >= faultTimeTO )&&(config & THEREATTEMPTFLT_MASK)) )
			{	
				/* End conteggio Prefault -> Fault: viene attivato il Fault */
				SWTC_Stop((SWTC_CountersType) counterIndex);

#if (MULTI_COMPARTMENT_APPL == 1)
				FLT_faultUpdateStep((uint8_t)FAULT_STEP, compartment );
#else
				FLT_faultUpdateStep((uint8_t)FAULT_STEP);
#endif /* (MULTI_COMPARTMENT_APPL == 0) */
				pApplCurrentFault->faultCodes = pFaultmgmt_Data->faultCodes;
			}					

		break;	

		case WAIT_PREFAULT_RESET_STEP:

			pFaultmgmt_Data->step = PREFAULT_STEP;
			
			if( (config & THEREATTEMPTFLT_MASK) == 0 )
			{
				#if (SW_TIMER_NORM_MODE	== FALSE) 
				   Set_MediumCounter((SWTC_CountersType)counterIndex, (SWTC_PrescalerType)(config & FLT_PRESCALER_MASK ), (SWTC_TimeType)faultTimeTO)  ;
				#else
				  Set_DownCounter(MEDIUM_COUNTER, (SWTC_CountersType) counterIndex, (SWTC_PrescalerType)(config & FLT_PRESCALER_MASK ), (SWTC_TimeType)faultTimeTO);
				#endif  
			}
			else
			{
				/* Fault gestito mediante tentativi: viene azzerato il contatore usato per i tentativi.	*/
				LKT_FIELD_S(pFaultmgmt_Data->filterCount) = 0;

				/* Solo per pulizia, perchè comunque il timer non viene utilizzato */
				SWTC_Stop((SWTC_CountersType) counterIndex);
			}
		
		break;
		

		default:
		
			/* Include FLT_IDLE_STEP, FAULT_STEP */

		break;
	
	}

	/*
	Test per esecuzione azioni di sicurezza: va effettuato qui, e non in uno specifico case del switch, perchè così
	è eseguito sin dalla prima iterazione in cui si la macchina a stati è in fault, e non dalla successiva.
	Infatti se succedesse che nell'iterazione successiva la macchina a stati non fosse più chiamata per la scomparsa
	della condizione di guasto, si resterebbe in una situazione in cui la macchina a stati è in fault, l'applicazione
	è in fault, ma le azioni di sicurezza non sono eseguite.
	*/
	if( pFaultmgmt_Data->step == FAULT_STEP )
	{
		if (TESTBIT(config, FLT_SAFETY_ACTION_BIT))
		{
			FLT_safetyBitmapStandard();
#if (MULTI_COMPARTMENT_APPL == 1)
			FLT_SafetyBitmap_Cstm_Exclusion(compartment);
#else
			FLT_SafetyBitmap_Cstm_Exclusion();
#endif
		}
	}
}
/**/


/**
Gestisce la procedura di reset dallo stato di Prefault.
 
\param [in]/[out]	FaultMgmt_Data, ApplCurrentFault 	
\param [out] 	
 
\return 		none
\warning		

\notes:

\version		1.0.0
\date 			25/09/2006
\author			Marco Quintini
*/

static void FLT_prefaultResetManagement(uint16_t config)
{

#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t compartment = (uint8_t)((config >> COMPARTMENT_DISPLACEMENT) & 0x03);
#else
uint8_t compartment =0;
#endif

FaultMgmt_Data_Struct *pFaultmgmt_Data= &FaultMgmt_Data[compartment];
Fault_Compl_Struct *pCurrentFault = &ApplCurrentFault[compartment];
SWTC_CountersType counterIndex = MAIN_FAULT_MGMT_COUNTER_M + compartment;

	pFaultmgmt_Data->stepChanged = FALSE;

	switch (pFaultmgmt_Data->step)
	{
	
		case WAIT_PREFAULT_RESET_STEP:
	/* EA prevedere il reset del fault della singola cavità in caso di più compartment */
			SWTC_Run((SWTC_CountersType)counterIndex);
			
			if ( ! SWTC_IsExpired((SWTC_CountersType)counterIndex) )
			{
				break;
			}	
		
		/*lint -fallthrough : i due case si possono accorpare */
		case ALARM_FLAG_STEP:
		case PREFAULT_WAIT_STEP:
			
			/* Se non ho ancora raggiunto lo stato di prefault la procedura di reset viene
			   eseguita subito */

			/*
			Se c'è un prefault attivo lo resetto comunque, anche se la procedura di reset
			è relativa ad un guasto diverso da quello che ha portato al prefault.
			*/
			pCurrentFault->prefaultCodes = No_Fault;

#if defined(FAULT_ARE_RESTORABLE)
			if ( config  & FAULT_RECOVERY_MASK)
			{
#if (MULTI_COMPARTMENT_APPL == 1)			
				if(FLT_checkForCurrentFault(pCurrentFault->faultCodes.code, pCurrentFault->faultCodes.subCode, compartment))
				{
					FLT_applCurrentFaultReset(compartment);
				}
#else
				if (FLT_checkForCurrentFault( pCurrentFault->faultCodes.code, pCurrentFault->faultCodes.subCode))
				{
					FLT_applCurrentFaultReset();
				}
#endif				
			}
#endif /* defined(FAULT_ARE_RESTORABLE) */
#if (MULTI_COMPARTMENT_APPL == 1)
			FLT_faultMgmtDataReset(compartment);
#else
			FLT_faultMgmtDataReset();
#endif /*if (MULTI_COMPARTMENT_APPL == 1)            */

			pFaultmgmt_Data->stepChanged = TRUE;

			if(pCurrentFault->faultCodes.code != CODE_NO_FAULT)
			{
				/*
				Se c'è un fault attivo, vengono ripristinati i suoi codici (sono il riferimento nel caso
				intervenissero nuovi guasti) e lo step.

				Va ripresa la priorità del fault reale ma non di quello virtuale altrimenti verrebbero
				mascherati tutti i faults meno prioritari mentre la macchina sta ancora funzionando
				*/
				pFaultmgmt_Data->faultCodes = pCurrentFault->faultCodes ;

				pFaultmgmt_Data->step = FAULT_STEP;
			}
			else
			{/* reset bitmap per routine di sicurezza in caso di Fault */
				FLT_safetyBitmapReset();
			}
		
		break;
		
#if defined(FAULT_ARE_RESTORABLE)
		case  FAULT_STEP:
			if ( ( config  & FAULT_RECOVERY_MASK)==0)
			{/* posso cancellare il fault anche quando sono già in fault. */
				break;
			}
#endif /* defined(FAULT_ARE_RESTORABLE) */

		case PREFAULT_STEP:
		
			/* Init Procedura di Reset */
			#if (SW_TIMER_NORM_MODE	== FALSE) 
			  Set_MediumCounter((SWTC_CountersType)counterIndex, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_TimeType)(config & FAULTRESET_TIMEOUT_MSK)) ;
			#else
			  Set_DownCounter(MEDIUM_COUNTER, (SWTC_CountersType) counterIndex, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_TimeType)(config & FAULTRESET_TIMEOUT_MSK));
			#endif  
							
			pFaultmgmt_Data->step = WAIT_PREFAULT_RESET_STEP;
		
		break;
				

		default:

			/* Include FLT_IDLE_STEP e, se FAULT_ARE_RESTORABLE non è definita, FAULT_STEP */
			
			break;
	}
}

/**/

/**/


/**
Dichiara il fault per il file di setting.

\param [out]	Fault (global)

 
\return 		none.

\version		1.0.0
\date			02/10/2006
\author			Marco Quintini
*/
void FLT_declareSettingFault(void)
{
uint8_t compartment;
	if( FLT_checkForSettingFault() == FALSE )
	{	/* Il fault file di setting non è ancora attivo */
		/*
		MQ AN Perché chiamarla? Non viene chiamata quando scatta un fault, ma quando rientra (e non agisce neanche su variabili
		backuppate). Tra l'altro, anche nell'ipotesi che la macchina a stati dell'applicazione girasse ancora, all'inizio della
		rilevazione di un fault ci pensa la chiamata a FLT_initFaultDetectProc() a resettare le variabili significative.
		*/
	/*	FLT_faultMgmtDataReset(); */
		for (compartment =0;compartment < APP_N_COMP; compartment++)
		{		

			/* MQ AN Rivedere in base alle scelte sull'F09 */
			ApplCurrentFault[compartment].faultCodes.code = FLT_CODE_SETTING_FILE;
			ApplCurrentFault[compartment].faultCodes.subCode = FLT_SUBCODE_MAIN_BOARD_SETTING_FILE;
			ApplCurrentFault[compartment].prefaultCodes = No_Fault;

			/*
			Per coerenza viene aggiornata la macchina a stati, anche se con F09.01 attivo le routine
			di rilevazione guasti dell'applicazione non sono eseguite.
			*/
			FaultMgmt_Data[compartment].step = FAULT_STEP;
			FaultMgmt_Data[compartment].faultCodes = ApplCurrentFault[compartment].faultCodes;

			/* MQ AN Da verificare se è il caso si inserire qui il salvataggio dell'F09 nei Last fault */

			/*
			All'ingresso in FAULT viene resettato comunque il valore del delay e del numero di attivazioni dell'Antishock
			in un passo, per non ritrovare queste variabili sporche dopo un eventuale mancanza rete
			*/
			/* MQ AN Potrebbe non servire più, se l'F09.0x01 viene trattato come tutti gli altri fault e quindi fa andare in FAULT la macchina */
#if (MULTI_COMPARTMENT_APPL == 1)	
			SPV_resetSensVariables(compartment);
		
			if ( TESTBIT(N_Application, FLAG_DOUBLE)==0)
			{
				compartment=APP_N_COMP;
			}
#else
			SPV_resetSensVariables(	);		
#endif			
		}
	}
}
/**/



/**
Controlla la priorità del fault ed eventualmente inizializza la procedura di rilevazione.

\param [in]		code e subcode del fault da rilevare
\param [in/out]	aggiorna la macchina a stati FaultMgmt_Data se il fault è prioritario

\return 		none

\version		1.0.0
\date
\author			Emanuela Antognoli

*/
#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_manageFault( uint8_t code, uint8_t subcode, uint8_t compartment)
#else
void FLT_manageFault( uint8_t code, uint8_t subcode)
#endif /* (MULTI_COMPARTMENT_APPL == 1)*/
{
Timeout_struct time_data = {0, 0, 0};

#if (MULTI_COMPARTMENT_APPL == 0)
  uint8_t compartment = 0;
#endif /* (MULTI_COMPARTMENT_APPL == 0) */  
Fault_Compl_Struct *pCurrentFault= &ApplCurrentFault[compartment];

	/*lint -save -e960 */ /* There are not side effects */
	if( ( pCurrentFault->faultCodes.code == CODE_NO_FAULT )
		||
		( LIB_SEF_GetFaultToDisplay((bool_t)TRUE, pCurrentFault->faultCodes.code, pCurrentFault->faultCodes.subCode) != 0 )
	/*lint -restore */
	  )
	{
		/*
		Non è presente un fault di applicazione o ne è presente uno di quelli che NON vanno visualizzati: si può
		valutare l'ingresso nella macchina a stati di un nuovo fault.
		*/
#if (MULTI_COMPARTMENT_APPL == 1)
		FLT_checkFltStateMachineRestart( code,subcode, compartment);
#else
		FLT_checkFltStateMachineRestart( code,subcode);
#endif /*(MULTI_COMPARTMENT_APPL == 1)*/

	}
	else
	{
		/*
		Se è presente un fault di applicazione di quelli che vanno visualizzati nessun altro fault di applicazione
		può entrare nella macchina a stati. La sicurezza è comunque garantita dalla piattaforma, e si evita che un
		fault da non visualizzare a priorità maggiore possa soppiantare un fault da visualizzare, facendo uscire
		la macchina dallo stato di FAULT.
		*/
	}
#if (MULTI_COMPARTMENT_APPL == 1)
	if( FLT_checkForCurrentFault( code, subcode, compartment) )
#else
	if( FLT_checkForCurrentFault( code, subcode) )
#endif	
	{
#if (MULTI_COMPARTMENT_APPL == 1)
		time_data.config |= (compartment << COMP_MASK_NUM_RIGHT_SHIFT);
#endif
		if(FLT_getTimeout(code, subcode, &time_data))
		{
			/*
			A questo punto in time_data.config c'è l'indicazione del prescaler, se ci sono i tentativi e se
			sono richieste le azioni di sicurezza standard in FAULT.
			*/
			FLT_prefaultFaultStateManagement(time_data.prefaultTime, time_data.faultTime,time_data.config);
		}
		else
		{
			/*
			Questo caso in realtà non può accadere. Se un fault occupa la macchina a stati vuol dire che quando si
			è verificato valeva una delle due seguenti condizioni:
			- aveva priorità maggiore del fault corrente in quel momento.
			- aveva priorità uguale a quella del fault corrente in quel momento, ma gli è subentrato perché ha un tempo
			  di prefault minore.
			In entrambi i casi, all'interno di FLT_checkFltStateMachineRestart(), la funzione la funzione FLT_getTimeout()
			deve avere restituito OK, quindi FLT_getTimeout() restituisce OK con i codici correnti.
			*/
		}
	}
}

/**
Controlla se il fault in esecuzione ha priorità maggiore di quello in ingresso
o se ci vuole meno tempo per rilevarlo, SOLO se non si è già in prefault (hottima style).

In questo caso aggiorna il il fault corrente da analizzare e re-inizializza la procedura di rilevazione

\param [in] 	code subcode
\param [out]	FaultMgmt_Data-> STATO ALARM

\return 		noone
\warning

\notes:

\version		1.0.0
\date 			31/01/2013
\author			Emanuela Antognoli
*/
#if (MULTI_COMPARTMENT_APPL == 1)
static void FLT_checkFltStateMachineRestart( uint8_t code, uint8_t subcode, uint8_t compartment)
#else	
static void FLT_checkFltStateMachineRestart( uint8_t code, uint8_t subcode)
#endif
{
uint8_t faultPriority,currPriorityMonitor;
Timeout_struct time_Data;
uint16_t currCodeTimeout;

#if (MULTI_COMPARTMENT_APPL ==0)
uint8_t compartment = 0;
#endif
FaultMgmt_Data_Struct *pFaultmgmt_Data= &FaultMgmt_Data[compartment];

	if (LIB_SEF_GetFaultPriority((bool_t)TRUE,code,subcode, &faultPriority) != SF_OK)
	{
		faultPriority =0;
	}
	faultPriority=LIB_SEF_FaultPriorityField(faultPriority);

	if (pFaultmgmt_Data->faultCodes.code!=0)
	{
		if (LIB_SEF_GetFaultPriority((bool_t)TRUE,pFaultmgmt_Data->faultCodes.code,pFaultmgmt_Data->faultCodes.subCode, &currPriorityMonitor) != SF_OK)
		{
			currPriorityMonitor =0;
		}
		currPriorityMonitor=LIB_SEF_FaultPriorityField(currPriorityMonitor);
	}
	else
	{
		currPriorityMonitor=0;
	}

	if ( faultPriority > currPriorityMonitor )
	{
		/* La priorità del fault da dichiarare è maggiore di quella del fault eventualmente già presente */

		if(FLT_getTimeout(code, subcode, &time_Data))
		{
			/*
			In questo caso la funzione FLT_getTimeout() viene chiamata solo per verificare che il timeout
			del nuovo fault sia gestito.
			*/
			pFaultmgmt_Data->faultCodes.code = code;
			pFaultmgmt_Data->faultCodes.subCode = subcode;
			/*imposta la macchina a stati dei Fault in ALARM, non è un prefault.*/
#if (MULTI_COMPARTMENT_APPL == 1)
			FLT_initFaultDetectProc(compartment);
#else
			FLT_initFaultDetectProc();
#endif
		}
		else
		{
			/*
			Codice/sottocodice del nuovo fault non previsti nella funzione FLT_getTimeout(),
			il nuovo fault non può entrare nella macchina a stati.
			*/
		}
	}
	else
	{
		if ( faultPriority == currPriorityMonitor )
		{
			/* se la priorità del fault è uguale a quello analizzato si controllano i timeout */
			if ( ( pFaultmgmt_Data->step <= PREFAULT_WAIT_STEP) &&
				 (pFaultmgmt_Data->faultCodes.subCode!= subcode) &&
				 (pFaultmgmt_Data->faultCodes.code!= code) )
			{
				/* non siamo ancora nello stato di prefault */

				currCodeTimeout=0;

				if (pFaultmgmt_Data->faultCodes.code != CODE_NO_FAULT)
				{
					if(FLT_getTimeout(pFaultmgmt_Data->faultCodes.code,pFaultmgmt_Data->faultCodes.subCode, &time_Data))
					{
						currCodeTimeout=time_Data.prefaultTime;
					}
					else
					{
						/*
						Questo caso in realtà non può accadere. Se un fault occupa la macchina a stati vuol dire che quando si
						è verificato valeva una delle due seguenti condizioni:
						- aveva priorità maggiore del fault corrente in quel momento.
						- aveva priorità uguale a quella del fault corrente in quel momento, ma gli è subentrato perché ha un tempo
						  di prefault minore.
						In entrambi la funzione la funzione FLT_getTimeout() deve avere restituito OK, quindi FLT_getTimeout()
						restituisce OK con i codici pFaultmgmt_Data->faultCodes.
						*/
					}
				}

				if( FLT_getTimeout( code, subcode,&time_Data) )
				{
					if (time_Data.prefaultTime < currCodeTimeout)
					{
						pFaultmgmt_Data->faultCodes.code = code;
						pFaultmgmt_Data->faultCodes.subCode = subcode;
						/*imposta la macchina a stati dei Fault in ALARM, non è un prefault.*/
#if (MULTI_COMPARTMENT_APPL == 1)	
						FLT_initFaultDetectProc(compartment);
#else
						FLT_initFaultDetectProc();
#endif /*(MULTI_COMPARTMENT_APPL == 1)*/
					}
				}
				else
				{
					/*
					Codice/sottocodice del nuovo non fault previsti nella funzione FLT_getTimeout(),
					il nuovo fault non può subentrare nella macchina a stati.
					*/
				}
			}
		}
	}/* end else*/
}


/**
Verifica se le risorse per la rilevazione fault sono allocate per il fault in oggetto.

\param [in]		FaultMgmt_Data (global)


\return 		TRUE/FALSE.

\version		1.0.0
\date			14/01/2010
\author			Emanuela Antongnoli
*/

#if (MULTI_COMPARTMENT_APPL == 1)
bool_t FLT_checkForCurrentFault(uint8_t code, uint8_t subcode, uint8_t compartment)
#else
bool_t FLT_checkForCurrentFault(uint8_t code, uint8_t subcode)
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment=0;
#endif
FaultMgmt_Data_Struct *pFaultmgmt_Data= &FaultMgmt_Data[compartment];


	if(	( pFaultmgmt_Data->faultCodes.subCode == subcode) &&
		( pFaultmgmt_Data->faultCodes.code== code) )
	{
		return TRUE;
	}
    else
    {
    	return FALSE;
    }
}




void FLT_ResetFaultDetectProc(uint8_t code, uint8_t subcode, uint16_t reset_config)
{
#if (MULTI_COMPARTMENT_APPL == 1)
	if(FLT_checkForCurrentFault(code, subcode, ((reset_config >> COMPARTMENT_DISPLACEMENT) & 0x03)))
#else
	if ( FLT_checkForCurrentFault( code,  subcode))
#endif		
	{ /* si sta analizzando il fault corrente*/
		
		 FLT_prefaultResetManagement(reset_config );
	}
	else 
	{
		/* do nothing */
	}
}

/**
Richiede alla piattaforma il suo stato di fault, aggiorna le statistiche relative ai prefault di piattaforma e
aggiorna PltFaultCodes. Aggiorna lo stato del WARNING DOOR sulla base del consistency del blocco porta.

\return		void

\retval 	none

\warning	none

\version	1.0.0
\date		14/02/2013
\author		Marco Quintini

\bug		none

\todo		nothing

\note		none
*/
void FLT_updatePlatformFaultStatus(void)
{
uint8_t moduleIndex;
uint8_t i=0;
sft_module_faults_t moduleSafetyStatus;

	/* Loop di ricerca di eventuali nuovi prefault di sicurezza */
	for(moduleIndex = 0; moduleIndex < SFT_NUM_SAFETY_MODULES; moduleIndex++ )
	{
		if( LIB_SFT_GetSafetyModFaultStatus(moduleIndex, &moduleSafetyStatus) == 0 )
		{

			if( ( ( moduleSafetyStatus.status & SFT_FAULTS_STATUS_MASK ) == SFT_MODULE_PREFAULT )
				&&
				( ( moduleSafetyStatus.status & SFT_FAULT_STATUS_CHANGED_MASK ) != 0 )
				&&
				( moduleSafetyStatus.fault.code != CODE_NO_FAULT ) /* Dovrebbe essere un controllo superfluo se status == SFT_MODULE_PREFAULT, ma nel dubbio... */
				)
			{
				/* Nuovo prefault di piattaforma */

				/* Salvataggio codici di prefault per le statistiche del ciclo di collaudo */
#ifdef _STATISTICS_ON_
#ifndef _NO_STC_TESTCYCLE_				
				MachineStatistics.testCycleStats.prefaultCodes = moduleSafetyStatus.fault;
#endif
#endif /*  _STATISTICS_ON_ */
#if (MULTI_COMPARTMENT_APPL == 1)
				for(i = 0; i < APP_N_COMP; i++)
#endif						
				{
					/* Aggiornamento buffer Last Prefault */
#if (MULTI_COMPARTMENT_APPL == 1)
					FLT_lastFaultUpdate(moduleSafetyStatus.fault, &MachineStatistics.lastPreFault[i][0]);
#else
					FLT_lastFaultUpdate(moduleSafetyStatus.fault, &MachineStatistics.lastPreFault[0]);
#endif					
#ifdef _STATISTICS_ON_
					/* Salvataggio dati nella Black Box (se non c'è già un fault di applicazione o di piattaforma attivo) */
					if( (ApplCurrentFault[i].faultCodes.code == CODE_NO_FAULT) && (PltFaultCodes.code == CODE_NO_FAULT) )
					{
#if (MULTI_COMPARTMENT_APPL == 1)
						STC_saveBlackBoxData( moduleSafetyStatus.fault,i);
#else
						STC_saveBlackBoxData(moduleSafetyStatus.fault);
#endif					
					}
#endif
#if (MULTI_COMPARTMENT_APPL == 1)
					if(TESTBIT(N_Application, FLAG_DOUBLE) == 0)
					{
						i = APP_N_COMP;
					}
#endif				
				}
			}

			/* Salvataggio stato PEC nella variabile globale */
			FltSafetyModuleStatus[moduleIndex] = moduleSafetyStatus;
		}
	}

	LIB_SFT_GetOverallFaultStatus(&moduleSafetyStatus);

	if( ( moduleSafetyStatus.status & SFT_FAULTS_STATUS_MASK ) == SFT_MODULE_FAULT )
	{
		PltFaultCodes = moduleSafetyStatus.fault;
	}
	else
	{
		PltFaultCodes = No_Fault;
	}

	/* Azzera le notifiche di variazione di stato dei moduli di sicurezza */
	LIB_SFT_ResetStatusChangeNotification();
}


/**
Aggiorna il codice di fault globale della macchina

\return		void

\retval 	none

\warning	none

\version	1.0.0
\date		18/02/2013
\author		Marco Quintini

\bug		none

\todo		nothing

\note		none
*/
#if (MULTI_COMPARTMENT_APPL == 1)
void FLT_updateGlobalFaultCode(uint8_t compartment)
#else
void FLT_updateGlobalFaultCode(void)
#endif
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif
sft_fault_codes_t *pCurrentFault = &CurrentFault[compartment] ;	
Fault_Compl_Struct *pApplCurrentFault= &ApplCurrentFault[compartment];


	if( PltFaultCodes.code != CODE_NO_FAULT )
	{
		*pCurrentFault = PltFaultCodes;
		/* MQ AN Prevedere anche per i fault di Plat la possibilità di non essere visualizzati? */
	}
	/*lint -save -e960 */ /* There are not side effects */
	else if ( (pApplCurrentFault->faultCodes.code != CODE_NO_FAULT )
			  &&
			  ( LIB_SEF_GetFaultToDisplay((bool_t)TRUE, pApplCurrentFault->faultCodes.code, pApplCurrentFault->faultCodes.subCode) == 0 )
			 )
	/*lint -restore */
	{
		/* C'è un fault di applicazione e deve essere visualizzato */
		*pCurrentFault = pApplCurrentFault->faultCodes;
	}
	else
	{
		*pCurrentFault = No_Fault;
	}
}


/**
Confronta i codici di fault all'iterazione precedente con quelli attuali e nel caso sia intervenuto un nuovo fault
lo salva nei Last Fault ed aggiorna l'evento della Black Box.

\param [in] previousFaultCodes sono codice e sottocodice all'iterazione precedente
\param [in] currentFaultCodes sono codice e sottocodice all'iterazione corrente

\return		void

\retval 	none

\warning	none

\version	1.0.0
\date		20/02/2013
\author		Marco Quintini

\bug		none

\todo		nothing

\note		none
*/
#if (MULTI_COMPARTMENT_APPL == 1)
static void FLT_updateFaultStatistics(sft_fault_codes_t previousFaultCodes, sft_fault_codes_t currentFaultCodes, uint8_t compartment)
#else
static void FLT_updateFaultStatistics(sft_fault_codes_t previousFaultCodes, sft_fault_codes_t currentFaultCodes)
#endif 
{
uint8_t i;
uint8_t matchFound =0;

	if( ( previousFaultCodes.code != currentFaultCodes.code ) || ( previousFaultCodes.subCode != currentFaultCodes.subCode ) )
	{
		if( currentFaultCodes.code != CODE_NO_FAULT )
		{
			/* Nuovo fault */

			/* Aggiornamento Last Fault */
			if( SPV_isAutotestRunning() == FALSE )
			{
				/* Non è in corso l'Autotest (i fault che si verificano in Autotest non devono essere salvati nei LastFault) */
#if (MULTI_COMPARTMENT_APPL == 1)
				FLT_lastFaultUpdate(currentFaultCodes, &MachineStatistics.lastFault[compartment][0]);
#else	
				FLT_lastFaultUpdate(currentFaultCodes, &MachineStatistics.lastFault[0]);
#endif
			}

#ifdef _STATISTICS_ON_
		/* Aggiornamento evento Black Box */
#if (MULTI_COMPARTMENT_APPL == 1)
			matchFound=(compartment & STC_BB_COMP_MASK);						
#endif
			for(i = 0; i < SIZE(MachineStatistics.blackBox_Data); i++)
			{
				if( MachineStatistics.blackBox_Data[i].event == BB_EVENT_PREFAULT )
				{
					/*
					La Black Box è in "stato di prefault": se è in stato di fault vuol dire che si è già passati per
					un aggiornamento, quindi non viene sovrascritta.
					*/
					if( (matchFound & STC_BB_MATCHFOUND_MASK ) == FALSE )
					{
						if( ( MachineStatistics.blackBox_Data[i].faultCodes.code == currentFaultCodes.code )
							&&
							( MachineStatistics.blackBox_Data[i].faultCodes.subCode == currentFaultCodes.subCode )
						  )
						{
							/* Il codice e sottocodice di prefault salvati nella Black Box coincidono con quelli del fault attuale */

							/*
							Per come è scritto il ciclo for, nel caso ci fossero più eventi salvati il cui codice e sottocodice coincidono
							con quelli del fault corrente e tutti in stato di prefault, viene aggiornato considerato come "match" solo
							l'evento più recente (MachineStatistics.blackBox_Data[i] è più recente di MachineStatistics.blackBox_Data[i+1]).
							*/
							matchFound|= STC_BB_MATCHFOUND_MASK;
						}

						STC_updateBlackBoxEvent(i, matchFound);
					}
					else
					{
						STC_updateBlackBoxEvent(i, ( matchFound & STC_BB_COMP_MASK) );
					}
				}
				else
				{
					/* Stato Black Box != prefault: la Black Box viene lasciata invariata */
				}
			}
#endif /* _STATISTICS_ON_ */

		}
	}
}


/**
Gestisce lo stato globale di fault della macchina e le relative statistiche (Last Fault, Black Box)

\return		void

\retval 	none

\warning	none

\version	1.0.0
\date		20/02/2013
\author		Marco Quintini

\bug		none

\todo		nothing

\note		none
*/
void FLT_manageGlobalFaultStatus(void)
{
uint8_t compartment = 0;
static sft_fault_codes_t previousPltFaultCodes = { CODE_NO_FAULT, SUBCODE_NO_FAULT };

#if (MULTI_COMPARTMENT_APPL == 1)
static sft_fault_codes_t previousApplFaultCodes[APP_N_COMP] =  {{ CODE_NO_FAULT, SUBCODE_NO_FAULT },
		{ CODE_NO_FAULT, SUBCODE_NO_FAULT }};
#else
static sft_fault_codes_t previousApplFaultCodes = { CODE_NO_FAULT, SUBCODE_NO_FAULT };
#endif

	/* Aggiornamento codice fault globale */
#if (MULTI_COMPARTMENT_APPL == 1)	
	for (compartment =0;compartment < APP_N_COMP; compartment++)
	{
		FLT_updateGlobalFaultCode(compartment);
#else
	FLT_updateGlobalFaultCode();
#endif
	/*
	MQ AN Per non salvare nei Last Fault i fault di applicazione che non diventano attivi, rivedere le chiamate
	seguenti limitandole ai fault di applicazione che non vanno visualizzati e ai fault globali.
	Per l'evento della Black Box non dovrebbe essere un problema lasciare fuori i fault di applicazione non attivi,
	perché se non sono attivi è perché c'è già un fault di piattaforma, e quindi la Black Box è già bloccata.
	*/
	/* Aggiornamento Last Fault e Black Box fault applicazione */
	
#if (MULTI_COMPARTMENT_APPL == 1)
		FLT_updateFaultStatistics(previousApplFaultCodes[compartment], ApplCurrentFault[compartment].faultCodes, compartment);
		/* Aggiornamento Last Fault e Black Box fault piattaforma */
		FLT_updateFaultStatistics(previousPltFaultCodes, PltFaultCodes, compartment);
		previousApplFaultCodes[compartment] = ApplCurrentFault[compartment].faultCodes;
#else	
	FLT_updateFaultStatistics(previousApplFaultCodes, ApplCurrentFault[compartment].faultCodes);
	/* Aggiornamento Last Fault e Black Box fault piattaforma */
	FLT_updateFaultStatistics(previousPltFaultCodes, PltFaultCodes);
#endif
	
	
#if (MULTI_COMPARTMENT_APPL == 1)
		if ( TESTBIT(N_Application, FLAG_DOUBLE)==0){
				compartment=APP_N_COMP;
		}
	}/* parentesi del for*/

#else
	previousApplFaultCodes = ApplCurrentFault[compartment].faultCodes;
#endif
	previousPltFaultCodes = PltFaultCodes;
}




/**
Aggiorna il campo event della struttura MachineStatistics.blackBox_Data, per indicare
che il prefault in corrispondenza del quale sono stati salvati i dati è degenerato
in un fault.

\param [in] 	
\param [out]	
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		04/07/2007
\version    	1.0.0

*/
/**
Aggiorna il campo event della struttura MachineStatistics.blackBox_Data, per indicare
che il prefault in corrispondenza del quale sono stati salvati i dati è degenerato
in un fault.

\param [in] 	
\param [out]	
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		04/07/2007
\version    	1.0.0

*/
#ifdef _STATISTICS_ON_
static void STC_updateBlackBoxEvent(uint8_t bbIndex, uint8_t  codesMatch)
{
	if( codesMatch & STC_BB_MATCHFOUND_MASK )
	{
#if (MULTI_COMPARTMENT_APPL == 1)
	if ( MachineStatistics.blackBox_Data[bbIndex].cycle_ref== (codesMatch & STC_BB_COMP_MASK))
#endif
		{
			MachineStatistics.blackBox_Data[bbIndex].event = BB_EVENT_FAULT_MATCH;
		}		
		
	}
	else
	{
		MachineStatistics.blackBox_Data[bbIndex].event = BB_EVENT_FAULT_NOT_MATCH;
	}
}
#endif
/**/

/* *********** */
/* End of file */
/* *********** */
