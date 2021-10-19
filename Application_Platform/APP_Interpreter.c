/**
\defgroup	APP_Interpreter Modulo Interprete
\ingroup	ApplPlatform

Il modulo processa l'esecuzione dei microprogrammi per i cicli scritti
nel file di setting.
*/

/**
Source file del modulo Interprete

\file		APP_Interpreter.c
\ingroup	APP_Interpreter
\date		23/07/2008
\version	01.00.00
\author		Valeria Catani, Simone Costantini, Stefano Ughi, Marco Quintini

\par		History

\par		PCLint
			Report 16/01/2009: 0 Errors, 0 Warnings, 0 Info

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/**
\name Posizione bit interruzione flusso interprete (per JumpIf o Reset)
*/
/*@{*/ 
#define B_JUMPIF_FLAG		0	/**< Bit interruzione per JumpIf */
#define B_RESET_FLAG		1	/**< Bit interruzione per Reset */
#if ( ITP_JIF_RET_ENABLE == 1 )
	#define B_RETURN_FLAG   2	/**< Bit Return JumpIF */
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
/* Posizione nel buffer dei dati di backup UI del ciclo corrente*/
	#define ITP_CYCLE_ACTIVE_POS		4
#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */

/*@}*/ 

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
#if ( ITP_JIF_RET_ENABLE == 1 )
uint8_t ITP_saveReturnPoint(uint8_t compartment);
uint8_t ITP_getReturnPoint(uint8_t compartment);
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)

	const uint8_t Tab_phase2byte[] = {
	/*0*/ 1,	/* Phase: 0..7 */
	/*1*/ 0,	/* Phase: 8..15 */
	/*2*/ 3,	/* Phase: 16..23 */
	/*3*/ 2,	/* Phase: 24..31 */
	/*4*/ 7,	/* Phase: 32..39 */
	/*5*/ 6,	/* Phase: 40..47 */
	/*6*/ 9,	/* Phase: 48..55 */
	/*7*/ 8,	/* Phase: 56..63 */
	};

#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */

/** Struttura per il caricamento dei dati della fase corrente */
static PhaseLoader_Struct TableData[APP_N_COMP];
/** Puntatore agli High Statement per il passo corrente */
static uint8_t *pStepHigh[APP_N_COMP];
/** Puntatore ai Low Statement per il passo corrente */
static uint8_t *pStepLow[APP_N_COMP];

#if (ITP_JIF_RET_ENABLE == 1)
/** Variabile lunghezza passo corrente */
static uint8_t StepLength[APP_N_COMP];
#endif /* (ITP_JIF_RET_ENABLE == 1) */

/** Buffer delle funzioni che implementano i vari microprogrammi (definito in CstmGlobal.c)*/
extern const MICROPROG Microprog_array[N_MAX_MICROPROG];

/* ***************** */
/* Exported function */
/* ***************** */

/**
E' il cuore dell'applicazione: gestisce il flusso e l'esecuzione dei microprogrammi.

\param [in]		compartment e' il vano di cui si esegue l'interprete ( se MULTI_COMPARTMENT_APPL == 1 )
\attention		Interpreter_Data (global, struttura con i dati sullo stato
				corrente dell'interprete) e' usata come input/output

\attention		TableData (global, struttura per il caricamento dei dati della
				fase corrente) e' usata come input/output
\attention		pStepHigh (global, puntatore agli High Statement per il passo
				corrente) e' usata come input/output
\attention		pStepLow (global, puntatore ai Low Statement per il passo
				corrente) e' usata come input/output
\attention		Microprog_array (global, buffer delle funzioni che implementano
				i vari microprogrammi) e' usata come input
\attention		Table_JumpIf (global, buffer tabelle destinazione Jump If per i
				diversi vani) e' usata come input

\return			un codice che specifica lo stato di esecuzione del passo/fase/ciclo o
				eventualmente l'errore incontrato

\warning		none		

\version		1.1.0
\date			06/03/2008
\author			Valeria Catani - Marco Quintini
*/

/*lint -save -e946 */ /* Suppress messages about operation on pointers for ITP_programFlowInterpreter */

#if (MULTI_COMPARTMENT_APPL == 1)	
  uint8_t ITP_programFlowInterpreter(uint8_t compartment)
#else
  uint8_t ITP_programFlowInterpreter(void)
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
{
uint8_t phLoader_Result;
uint8_t *pTempHighStat, *pTempLowStat;
uint8_t *pEndPhaseHighStat;
uint8_t T_Flag, phaseFlags, newStep;
uint8_t opCode, completeOpcode;
uint8_t resultStm = S_FALSE;

#if (MULTI_COMPARTMENT_APPL == 0)
  uint8_t compartment = 0;
#endif /* (MULTI_COMPARTMENT_APPL == 0) */  	

#if (ITP_JIF_PHASE_ENABLE == 1)
  uint8_t i;
  bool_t jumpAlreadyIn;
  JumpIfPhase_Struct *pCompJmpPhase = &JmpPhase[compartment];
#endif  /* (ITP_JIF_PHASE_ENABLE == 1) */

PhaseLoader_Struct *pCompTableData = &TableData[compartment];
Interpreter_Struct *pCompItpData = &Interpreter_Data[compartment];
uint8_t **ppCompStepHigh = &pStepHigh[compartment];
uint8_t **ppCompStepLow = &pStepLow[compartment];

#if (ITP_JIF_RET_ENABLE == 1) 
RetPoint_Struct *pCompRetPoint = &ReturnPoint[compartment];
#endif /* ITP_JIF_RET_ENABLE == 1 */

	/* 
	---------------------------------------------------------------
		Caricamento dei dati della fase indicata dal Supervisor
	---------------------------------------------------------------
	 */
	/*lint -save -e912 */
	if( pCompItpData->phaseDataInitialized == 0)
	/*lint -restore */
	{ 

#if (ITP_JIF_PHASE_ENABLE == 1)
    	
		/* Gestione JumpIF valido per fase in init */    	
		if( pCompItpData->jumpIF_phase_ctrl == 0 )
		{/*Reset condizioni JumpIF valide per fase*/ 
			for( i=0; i<MAX_NUM_JIF_PHASE; i++ )
			{	
  				pCompJmpPhase->statements[i][0] = (0xFF - UP_JUMPIF);	
				pCompJmpPhase->statements[i][1] = 0xFF;			/* High Statement a 0xFF not possible !! */		
			}
			pCompJmpPhase->index = 0; 		 	
		}
    	
    	pCompItpData->jumpIF_phase_ctrl = 0;
    	
#endif /*INT_JIF_PHASE_ENABLE	*/

#if (MULTI_COMPARTMENT_APPL == 1)	
		phLoader_Result = LDR_phaseLoader(compartment, pCompItpData->cycle, pCompItpData->phase, pCompItpData->mSetting, pCompTableData);
#else
    phLoader_Result = LDR_phaseLoader(pCompItpData->cycle, pCompItpData->phase, pCompItpData->mSetting ,pCompTableData);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */    

		if( phLoader_Result != S_OK )
		{
#ifdef _ITP_CHECKERRORS_	
			DBG_CheckErr(phLoader_Result);
#endif /* _ITP_CHECKERRORS_ */

			return ITP_ERR_PHASE_LOADER;
		}
		
		/* Inizializzazione puntatori del passo corrente (con puntatori iniziali fase) */
		*ppCompStepHigh = pCompTableData->pHiStat;
		*ppCompStepLow = pCompTableData->pLoStat;
		
		if(pCompItpData->first_statement < pCompTableData->nPhaseStatements)
		{
			/*
			Statement iniziale del passo in range rispetto al numero di statement
			previsti per la fase corrente, i puntatori al passo vengono allineati
			allo statement iniziale dello stesso.
			*/
		 	*ppCompStepHigh += pCompItpData->first_statement;
		 	*ppCompStepLow += pCompItpData->first_statement;
		}
		else
		{
			/* Errore: statement iniziale del passo out of range */
#ifdef _ITP_CHECKERRORS_	
			DBG_CheckErr(ITP_ERR_PH_STAT_OUT_OF_RANGE);
#endif /* _ITP_CHECKERRORS_ */

			return ITP_ERR_PH_STAT_OUT_OF_RANGE;
		}

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
/* attualmente solo per applicazioni mono-vano*/
	
		ITP_syncUICyclePos(compartment); /* comune */
			
		ITP_phaseTrackerUI(); /* custom*/
		
#endif		
		
		pCompItpData->phaseDataInitialized = 1;
	}
	/* End caricamento dati fase */

	/*
	Callback per funzioni specifiche dell'applicazione da eseguire
	prima di processare il passo
	*/
	ITP_preStepInit();
	 
	/*
	I puntatori allo statement corrente vengono inizializzati allo statement
	iniziale del passo corrente
	*/
	pTempHighStat = *ppCompStepHigh;
	pTempLowStat = *ppCompStepLow;

	/*
	Di default è sempre abilitato l'avanzamento di passo (newStep = TRUE)
   	solo un microprogramma con N a '1' e ritorno FALSE può bloccare tale avanzamento
   	*/ 
	newStep = TRUE;
	phaseFlags = 0;

	/* Get T Flag per il passo corrente */	
	T_Flag = ( *pTempHighStat & T_MASK ); 

	/* Viene calcolato il puntatore all'ultimo statement della fase */
	pEndPhaseHighStat = pCompTableData->pHiStat + (pCompTableData->nPhaseStatements - 1);

#if (ITP_JIF_RET_ENABLE == 1) 
	/* Inizializzazione variabile lunghezza passo corrente */	
	StepLength[compartment] = 0;
#endif /* (ITP_JIF_RET_ENABLE == 1) */  

	/* 
	----------------------------------------------------------------------
		Esecuzione del passo corrente: microprogrammi con stesso T Flag
	----------------------------------------------------------------------
	*/
	while( ( pTempHighStat <= pEndPhaseHighStat ) && ( ( ( *pTempHighStat ) & T_MASK ) == T_Flag ) )
	{

		opCode = OPCODE(*pTempHighStat);
		completeOpcode = COMPLETE_OPCODE(*pTempHighStat);	/* Opcode + Flags F0-F1 */

		/*lint -save -e912 */
		if ( opCode < SIZE(Microprog_array) )
		/*lint -restore */
		{
#if (ITP_JIF_PHASE_ENABLE ==1)

			if ( completeOpcode == UP_JUMPIF_PHASE )
			{/*JumpIF valido per fase */
			
				if ( pCompItpData->stepInitialized == 0 )
				{/*Init uP del passo : la prima volta che incontro il uP vengono sistemate le strutture dati*/
				
					if( pCompJmpPhase->index < MAX_NUM_JIF_PHASE )
					{
						jumpAlreadyIn = FALSE;
						/* Controllo che il Jump If non sia già presente */
						for( i=0; i < pCompJmpPhase->index; i++ )
						{
							if( pCompJmpPhase->statements[i][1] == *pTempLowStat )
							{
								jumpAlreadyIn = TRUE;	
							}
						
						}
						if( jumpAlreadyIn == FALSE )
						{/*Nuovo JumpIF : Salvo HighStatment e LowStatment */
							pCompJmpPhase->statements[pCompJmpPhase->index][0] = *pTempHighStat;	 
							pCompJmpPhase->statements[pCompJmpPhase->index][1] = *pTempLowStat;
							pCompJmpPhase->index ++;
						}
					}
				}
			}
			else if ( completeOpcode == UP_JUMPIF_PHASE_RESET )
			{/*Reset JumpIF valido per fase - prima volta che lo incontro elimino le strutture dati*/
			
				if (pCompItpData->stepInitialized == 0)
				{/*Init uP del passo : la prima volta che incontro il uP vengono sistemate le strutture dati*/
				
					if( pCompJmpPhase->index != 0 )
					{/*un Jump IF è presente nell'array*/
						pCompJmpPhase->index --;
						pCompJmpPhase->statements[pCompJmpPhase->index][0] = (0xFF - UP_JUMPIF);	
						pCompJmpPhase->statements[pCompJmpPhase->index][1] = 0xFF;
					}
				}	
			}
			else
			{
#endif /* ITP_JIF_PHASE_ENABLE == 1	*/
				
			/* Eseguo nel loop tutti i uP tranne JumpIF e Reset JumpIF valido per fase */	

			/* Esecuzione uP individuato da pTempHighStat */
#if (MULTI_COMPARTMENT_APPL == 1)
			resultStm = Microprog_array[opCode](*pTempHighStat, *pTempLowStat, compartment);
#else
			resultStm = Microprog_array[opCode](*pTempHighStat, *pTempLowStat);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

#if (ITP_JIF_PHASE_ENABLE == 1)
			}		
#endif /* ITP_JIF_PHASE_ENABLE == 1*/
			
			/* Gestione uP particolari */
			if( completeOpcode == UP_SETSTATE_RESET )	/* uP RESET */
			{
				/* SetState->Reset -> si esce dall'esecuzione del passo */
				/*
				N.B.A.: resultStm vale sicuramente S_OK perchè UP_SETSTATE_RESET può
				restituire solo questo valore
				*/
				pTempHighStat = pEndPhaseHighStat;	/* sufficiente pEndPhaseHighStat perchè ho ++ al termine dell'iterazione */
				SETBIT(phaseFlags, B_RESET_FLAG);
				
				newStep = TRUE;	
			}
			
#if (ITP_JIF_RET_ENABLE == 1)
			else if ( completeOpcode == UP_SETSTATE_RETURN )	 
			{/* RICORDA: IL RETURN DEVE ESSERE INSERITO IN UN PASSO COME UNICO UP!! */

				/* resultStm = TRUE;	//mi serve solo se non lo processo come uP, ma nell'array c'è */

				pTempHighStat = pEndPhaseHighStat;

				SETBIT(phaseFlags, B_RETURN_FLAG);

				newStep = TRUE;
			}
#endif /* (ITP_JIF_RET_ENABLE == 1) */		
	
			else if ( (completeOpcode == UP_JUMPIF_STEP) && (resultStm == S_OK) )	/* uP JUMPIF valido per PASSO */
			{
				/* JumpIf OK -> si esce dall'esecuzione del passo */
				/*
				N.B.A.: se nello stesso passo sono presenti un JumpIf e un SetState->Reset,
				il JumpIf non viene valutato perchè posto (dall'Editor) sempre dopo
				il SetState->Reset.
				*/
				pTempHighStat = pEndPhaseHighStat;	/* sufficiente pEndPhaseHighStat perchè ho ++ al termine dell'iterazione */
				SETBIT(phaseFlags, B_JUMPIF_FLAG);

				newStep = TRUE;		
			}
			else
			{
				/* Nothing to do */
			}
		}
		else
		{
			/*
			Il codice del uP non è tra quelli noti: se il uP ha l'N-flag settato
			non viene abilitato l'avanzamento del passo, ma in generale il
			processamento del passo prosegue
			*/
#ifdef _ITP_CHECKERRORS_			    	
			DBG_CheckErr(ITP_ERR_UNKNOWN_OPCODE);
#endif /* _ITP_CHECKERRORS_ */

			resultStm = S_FALSE;
		}

		if( ( resultStm != S_OK ) && ((( *pTempHighStat ) & N_MASK ) != 0 ) )
		{	
			/* uP bloccante (N=1) && resultStm!=S_OK => non abilita avanzamento passo */
			newStep = FALSE;		
		}
		else
		{
			/* newStep rimane TRUE => abilitato il passaggio allo step successivo */
		}

		pTempHighStat ++;	
		pTempLowStat ++;
		
#if (ITP_JIF_RET_ENABLE == 1) 
		/* incremento var. lunghezza passo corrente */
		StepLength[compartment]++;
#endif /* (ITP_JIF_RET_ENABLE == 1) */		
		
	}
	/* Scansione dei microprogrammi con identico T (passo) effettuata */


#if (ITP_JIF_PHASE_ENABLE == 1)
	/*
	Controlla se ci sono JumpIF validi per fase da processare prioritari rispetto a quelli
	del passo.
	Per ora N flag non viene preso in considerazione.
	*/

	for( i=0; i<MAX_NUM_JIF_PHASE; i++ )
	{
		if( (COMPLETE_OPCODE (pCompJmpPhase->statements[i][0])) == UP_JUMPIF_PHASE )
		{/*Esecuzione uP JumpIF con gli statement salvati da JumpIF valido per Fase*/

#if (MULTI_COMPARTMENT_APPL == 1) 			
			resultStm = JIF_JumpIf_Exe(pCompJmpPhase->statements[i][0],pCompJmpPhase->statements[i][1],compartment); 
#else
			resultStm = JIF_JumpIf_Exe(pCompJmpPhase->statements[i][0],pCompJmpPhase->statements[i][1]);
#endif /* (MULTI_COMPARTMENT_APPL == 1)  */			

			if ( resultStm == S_OK )
			{/*condizione ok: salto abilitato*/
				pTempHighStat = (pEndPhaseHighStat+1);

				SETBIT(phaseFlags, B_JUMPIF_FLAG);

				newStep = TRUE;
			}
		}	
	} 
	
#endif /* ITP_JIF_PHASE_ENABLE == 1	*/

#if (ITP_LOADS_MGMT_ENABLE == 1)
	/*lint -save -e912 */
	if ( pCompItpData->stepInitialized == FALSE )
	/*lint -restore */
	{/* continuità dei carichi nella prima iterazione del passo rispetto al passo precedente (new = old)*/

		pCompItpData->loads_mgmt_activation = TRUE;	
#if (MULTI_COMPARTMENT_APPL == 1) 		    
		ITP_loadsMgmt(compartment);		
#else
		ITP_loadsMgmt();	
#endif
	}
#endif /* (ITP_LOADS_MGMT_ENABLE == 1) */	

	/* Inizializzazione eseguita per tutti i uProgrammi del passo corrente	*/
	pCompItpData->optionalStepInitialized = 1;

	if( newStep == FALSE )
	{
		/* Il passo non è terminato */
		pCompItpData->stepInitialized = 1;	

		return SAME_STEP;    
	}

	/* Il passo è terminato (newStep = TRUE => abilitazione a processare nuovo passo) */

	/*
	Si forza la richiesta di inizializzazione dei uProgrammi del nuovo passo.
	Non serve azzerare anche optionalStepInitialized, se un uProgramma processa
	optionalStepInitialized processa anche stepInitialized (non è vero il viceversa).
	*/	
	pCompItpData->stepInitialized = 0;
	
	/* Callback per inizializzazione variabili nel passaggio ad un nuovo step */
	ITP_newStepInit();
	
	if( pTempHighStat <= pEndPhaseHighStat )
	{
		/* La Fase corrente NON è stata processata completamente */

		/* Avanzamento al nuovo passo */
		/* Aggiornamento puntatori al passo (HighStat, LowStat) */
		*ppCompStepHigh = pTempHighStat;
		*ppCompStepLow = pTempLowStat; 
		
		/* Aggiornamento indice statement iniziale del passo */
		pCompItpData->first_statement = (uint8_t)(*ppCompStepHigh - pCompTableData->pHiStat);

		return END_STEP;
	}

	/*
	La Fase corrente è stata processata completamente: conclusione naturale,
	o per JumpIF o per Reset.
	*/

	if( TESTBIT(phaseFlags, B_RESET_FLAG) )
	{
		/*
		Sarebbe superfluo, phaseFlags è locale e non viene più usata.
		Infatti il compilatore ottimizza eliminandola.
		*/
		CLRBIT(phaseFlags, B_RESET_FLAG);
		
		/*
		Il ciclo termina forzatamente.
		N.B.A.: questo codice di ritorno deve essere necessariamente processato dalla
		funzione che chiama ITP_programFlowInterpreter, perchè va stabilito il
		nuovo ciclo da eseguire (non può essere l'Interprete a farlo): in quel
		punto va anche opportunamente reimpostata la fase (non necessariamente
		azzerata), e di conseguenza va azzerato phaseDataInitialized.
		Per questo phaseDataInitialized non viene esplicitamente azzerato in questo punto.
		*/
		return END_CYCLE_RESET;	
	}

	/* Gestione fase conclusa per Jump IF */

	if (TESTBIT(phaseFlags, B_JUMPIF_FLAG))
	{
#if (ITP_JIF_RET_ENABLE == 1)

		if( Table_JumpIf[compartment].ret == TRUE )
		{/* Processato un JumpIF con ritorno */
		
			Table_JumpIf[compartment].ret = FALSE;	/*resetto la variabile (VEDI SE SPOSTARE IL RESET NEL UP)*/

			if( ( Table_JumpIf[compartment].cycle == pCompItpData->cycle) && ( Table_JumpIf[compartment].phase == pCompItpData->phase ) )
			{/* è stato richiesto un JSR allo stesso ciclo/fase: non ammesso! */

				return ITP_ERR_WRONG_COMMAND;	
			}
			
			if ( ITP_saveReturnPoint(compartment)!= S_OK )
			{/* Salvataggio punto di ritorno nella var Jsr */

				return ITP_ERR_WRONG_COMMAND;
			}

#if ( ITP_JIF_PHASE_ENABLE == 1)
			
			JmpPhase_Return[compartment] = JmpPhase[compartment];
					
#endif /* ITP_JIF_PHASE_ENABLE == 1 */

		}
#endif /* ITP_JIF_RET_ENABLE == 1 */

#if ( ITP_JIF_PHASE_ENABLE == 1)

		if( (Table_JumpIf[compartment].cycle == pCompItpData->cycle ) && (Table_JumpIf[compartment].phase == pCompItpData->phase) )
		{/* se, con un JumpIF valido per passo, salto ad un passo interno al ciclo-fase corrente,
			non devo resettare JumpIF valido per fase eventualmente presente, 
			nel momento dell'inizializzazione del passo di destinazione */

			pCompItpData->jumpIF_phase_ctrl = 1;
		}

#endif /* ITP_JIF_PHASE_ENABLE == 1 */		

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
        /* Backup poichè nel caso in cui si deve attivare il Timeout tra le fasi e riportarsi alla
           "fase precedente" saranno questi i valori corretti da caricare */
        bkp_CompItpDataCycle = Interpreter_Data[0].cycle;
        bkp_CompItpDataPhase = Interpreter_Data[0].phase;
        bkp_CompItpDatamSetting  = Interpreter_Data[0].mSetting;
        bkp_CompItpDataFirstStatement = Interpreter_Data[0].first_statement;
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
	
		/*
		Fase conclusa per JumpIF
		Aggiornamento indici di Ciclo/Fase/Passo in base alle indic. del up Jump If
		*/
		pCompItpData->cycle = Table_JumpIf[compartment].cycle;
		pCompItpData->phase = Table_JumpIf[compartment].phase;
		pCompItpData->first_statement = Table_JumpIf[compartment].first_statement;

		/*
		Viene richiesto comunque di ricaricare i dati della fase, anche se nel
		caso di salto ad un altro passo della stessa ciclo+fase basterebbe
		aggiornare first_statement.
		*/	
		pCompItpData->phaseDataInitialized = 0;

		/*
		Sarebbe superfluo, phaseFlags è locale e non viene più usata. 
		Infatti il compilatore ottimizza eliminandola.
		*/
		CLRBIT(phaseFlags, B_JUMPIF_FLAG);

		/* 
		E' utile ritornare un codice diverso dal semplice END_PHASE, in generale 
		il Supervisor potrebbe aver bisogno di distinguere questa sistuazione (ad esempio
		nella gestione delle fasi dei cicli di PROGRAMMING, END, ecc, per le applicazioni
		in cui sono presenti)
		*/
  		return PROGRAM_FLOW_BREAK;
	}
	/* END gestione fase conclusa per Jump IF */

#if ( ITP_JIF_RET_ENABLE == 1 )

	/* Gestione fase conclusa con RETURN */

	if ( TESTBIT(phaseFlags, B_RETURN_FLAG) )
	{
		if ( ITP_getReturnPoint(compartment)!= S_OK )
		{/* Recupero punto di ritorno nella var ReturnPoint */

			return ITP_ERR_WRONG_COMMAND;		
		}
		
		if( ( pCompRetPoint->cycle == pCompItpData->cycle ) && ( pCompRetPoint->phase == pCompItpData->phase ) )
		{/* Non ammesso Return allo stesso ciclo-fase */

			return ITP_ERR_WRONG_COMMAND;
		}

		if (pCompRetPoint->cycle == RETURN_END_CYCLE)
		{/* Return point dopo ultima fase: vado in END */

			CLRBIT(phaseFlags, B_RETURN_FLAG);

			return END_CYCLE;
		}

		/*Aggiornamento indici di Ciclo/Fase/Passo in base a quanto salvato in Return Point*/
		pCompItpData->cycle = pCompRetPoint->cycle;
		pCompItpData->phase = pCompRetPoint->phase;
		pCompItpData->first_statement = pCompRetPoint->first_statement;
		
		pCompItpData->phaseDataInitialized = 0;	/* ritorno nell'inizializzazione ciclo-fase
													per richiamare il PhaseLoader alla prox iterazione */

#if (ITP_JIF_PHASE_ENABLE == 1)
		JmpPhase[compartment] = JmpPhase_Return[compartment];  /* ripristino dati relativi al JumpIF valido per fase eventualmente presente */	

		pCompItpData->jumpIF_phase_ctrl = 1;	/* non devo cancellare i dati relativi al JUMPIF 
												valido per fase eventualmente presente */			
#endif /* ITP_JIF_PHASE_ENABLE == 1 */

		CLRBIT(phaseFlags, B_RETURN_FLAG);

		return PROGRAM_FLOW_BREAK;
	}
	/* END Gestione fase conclusa con RETURN */	

#endif /* ITP_JIF_RET_ENABLE == 1 */
	
	if (pCompItpData->phase >= (pCompTableData->nCyclePhases - 1) )
	{
		/*
		Il ciclo termina naturalmente.
		Vedi nota a prposito di END_CYCLE_RESET.
		*/
		return END_CYCLE;
	}

	/* Avanzamento alla Fase successiva */
	pCompItpData->phase ++;
	pCompItpData->first_statement = 0;

	pCompItpData->phaseDataInitialized = 0;

	return END_PHASE;
}
/*lint -restore */


/**
Viene eseguita quando occorre inizializzare l'interprete. Impone sempre di
ricaricare la fase. E' possibile indicare in ingresso se l'inizializzazione
della fase comporti anche la ripartenza dallo statement iniziale o no.
 
\param [in]		compartment e' il vano in cui viene eseguito l'interprete (solo per ( MULTI_COMPARTMENT_APPL == 1))
\param [in] 	mode specifica la modalita' di inizializzazione (ITP_FULL_INIT o ITP_PARTIAL_INIT)
\attention		Interpreter_Data (global, struttura con i dati sullo stato
				corrente dell'interprete) e' usata come output

\return			none

\warning		ATTENZIONE all'uso di ITP_PARTIAL_INIT se nella deviazione del flusso
				di esecuzione ci puo' essere una sovrapposizione tra dati di uP con
				lo stesso OpCode.!!!!

\version		1.0.0
\date			30/07/2009
\author			Valeria Catani
*/
#if ( MULTI_COMPARTMENT_APPL == 1)
void ITP_interpreterFlagsInit(uint8_t compartment, uint8_t mode)
#else
void ITP_interpreterFlagsInit(uint8_t mode)
#endif /* MULTI_COMPARTMENT_APPL == 1 */
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif /* (MULTI_COMPARTMENT_APPL == 0) */  	
Interpreter_Struct *pCompItpData = &Interpreter_Data[compartment];

	/* Forza il caricamento della fase */
	pCompItpData->phaseDataInitialized = 0;

	if( mode == ITP_FULL_INIT )
	{
		/* Per ripartire dal passo 0 */
		pCompItpData->first_statement = 0;
		/* Forza l'inizializzazione dei uProgrammi: processato da tutti */
		pCompItpData->stepInitialized = 0;
		/*
		Non serve azzerare optionalStepInitialized se già stepInitialized = 0.
		Infatti tutti i uProgrammi che processano optionalStepInitialized processano
		anche stepInitialized, mentre non è vero il viceversa.
		*/

#if (ITP_JIF_PHASE_ENABLE == 1)
		pCompItpData->jumpIF_phase_ctrl = 0;
#endif /* ITP_JIF_PHASE_ENABLE == 1 */
    }
	else if( mode == ITP_PARTIAL_INIT )
	{
		/* Forza l'inizializzazione dei uProgrammi: potrebbe non essere processato da tutti */
	  pCompItpData->optionalStepInitialized = 0;
	}
	else
	{
		/* Do nothing */
	}

#if (SST_COOLSTATE_ENABLE == 1)

	/* stato di OFF */

	Cool_Status = COOLSTATE_OFF;

#endif /* (SST_COOLSTATE_ENABLE == 1) */
}


/**
Viene eseguita nel caso in cui l'interprete incontri un microprogramma con
un codice sconosciuto, dunque normalmente mai.
In caso contrario significa che il file di setting presenta dei problemi.

\param [in]		highStatement e' l'High Statement del microprogramma (T flag +
				N flag + modificatori F1-F0 + opcode).
\param [in]		lowStatement e' il Low Statement del microprogramma (indice
				dello spiazzamento della tabella dati associata)
\param [in]		compartment e' il vano in cui viene eseguito il microprogramma (solo per ( MULTI_COMPARTMENT_APPL == 1))

\return			S_FALSE

\warning		none		

\version		1.1.0
\date			08/05/2008
\author			Marco Quintini
*/
/*lint -save -e715 */
#if (MULTI_COMPARTMENT_APPL == 1)		
uint8_t DummyMP( uint8_t highStatement, uint8_t lowStatement, uint8_t compartment)
#else
uint8_t DummyMP( uint8_t highStatement, uint8_t lowStatement)
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
{
#ifdef _ITP_CHECKERRORS_			    	
	DBG_CheckErr(ITP_ERR_UNKNOWN_OPCODE);
#endif /* _ITP_CHECKERRORS_ */

	return S_FALSE;
}
/*lint -restore */


/* ************** */
/* Local Function */
/* ************** */

#if ( ITP_JIF_RET_ENABLE == 1 )
/**
Questa funzione salva il punto di ritorno (ciclo, fase, passo)
per un microprogramma JumpIF con ritorno (JSR).
Viene gestito uno Stack di possibili JSR.
Si ritornerà al punto salvato incontrando il uP dedicato RETURN

\param [in]		compartment e' il vano/ cavità che si sta processando 
\(per funzioni mono vano compartment =0)

\param [out] (global) Jsr[compartment]

\return		S_OK o S_FALSE in caso di out of range rispetto allo stack dedicato
 
\version	1.0.0
\date 		30/07/2009
\author	Valeria Catani
*/
uint8_t ITP_saveReturnPoint(uint8_t compartment)
{
uint8_t endPhaseReturn,endCycleReturn;
JsrStack_Struct *pCompJsr = &Jsr[compartment];
Interpreter_Struct *pCompItpData = &Interpreter_Data[compartment];
PhaseLoader_Struct *pCompTableData = &TableData[compartment];
uint8_t *pStepLength = &StepLength[compartment];

	endPhaseReturn = FALSE;
	endCycleReturn = FALSE;

	if( pCompJsr->index < sizeof( pCompJsr->stack ) )
	{
		/*
		Salva statement iniziale passo successivo per avere immediatamente
		il punto di ritorno
		*/

		if ( (pCompItpData->first_statement + *pStepLength) ==  pCompTableData->nPhaseStatements )	/* verifica *pStepLength!!!! */
		{/* return point al termine della fase -> punto alla fase successiva */

			pCompJsr->stack[pCompJsr->index] = 0;

			endPhaseReturn = TRUE;
		}
		else
		{
			pCompJsr->stack[pCompJsr->index] = ( pCompItpData->first_statement + *pStepLength );	/* verifica *pStepLength!!!! */
		}

		pCompJsr->index ++;
	}
	else
	{
		return S_FALSE;
	}

	if( pCompJsr->index < sizeof( pCompJsr->stack ) )
	{
		/* Salva fase di ritorno */

		if (endPhaseReturn == TRUE)
		{/* return point al termine della fase -> punto alla fase successiva */
			if ( (pCompItpData->phase + 1) == pCompTableData->nCyclePhases )
			{/* return point al termine del ciclo -> devo andare in END */

				pCompJsr->stack[pCompJsr->index] = 0;
				endCycleReturn = TRUE;
			}
			else
			{/* posso incrementare la fase */
				pCompJsr->stack[pCompJsr->index] = (pCompItpData->phase + 1);
			}
		}
		else
		{
			pCompJsr->stack[pCompJsr->index] = pCompItpData->phase;
		}

		pCompJsr->index ++;
	}					   
	else
	{
		return S_FALSE;
	}

	if( pCompJsr->index < sizeof( pCompJsr->stack ) )
	{
		/* Salva ciclo di ritorno */

		if (endCycleReturn == TRUE)
		{
			pCompJsr->stack[pCompJsr->index] = RETURN_END_CYCLE;
		}
		else
		{
			pCompJsr->stack[pCompJsr->index] = pCompItpData->cycle;
		}

		pCompJsr->index ++;
	    
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


/**
Questa funzione restituisce il punto di ritorno (ciclo, fase, passo)
in corrispondenza del microprogramma RETURN, conseguenza di un JSR.
Viene gestito uno Stack di possibili JSR.
 
\param [in]		compartment e' il vano/ cavità che si sta processando 
\(per funzioni mono vano compartment =0)
 
\param [out] (global) ReturnPoint[compartment]

\return		S_OK o S_FALSE in caso di out of range rispetto allo stack dedicato
 
\version    	1.0.0
\date 			30/07/2009
\author			Valeria Catani
*/
uint8_t ITP_getReturnPoint(uint8_t compartment)
{
JsrStack_Struct *pCompJsr = &Jsr[compartment];
RetPoint_Struct *pCompRetPoint = &ReturnPoint[compartment];

	if( pCompJsr->index )
	{
		pCompJsr->index --;
		pCompRetPoint->cycle = pCompJsr->stack[pCompJsr->index];
	}
	else
	{
		return S_FALSE;
	}

	if( pCompJsr->index )
	{
		pCompJsr->index --;
		pCompRetPoint->phase = pCompJsr->stack[pCompJsr->index];
	}
	else
	{
		return S_FALSE;
	}	

	if( pCompJsr->index )
	{
		pCompJsr->index --;
		pCompRetPoint->first_statement = pCompJsr->stack[pCompJsr->index];

		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */


#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)

/**
Sincronizza ciclo e fase che vengono passati alla UI
con quelli dell'Interprete.

\param [in]		compartment e' il vano/ cavità che si sta processando 
\(per funzioni mono vano compartment =0)

\param [out] (global) UICycle
\param [out] (global) UIPhase

\return     	none

\version    	1.0.0
\date   		24/09/2009
\author			Valeria Catani
*/
void ITP_syncUICyclePos(uint8_t compartment)
{
	UICycle = Interpreter_Data[compartment].cycle;
	UIPhase = Interpreter_Data[compartment].phase;
}
/**/


/**

\return     	none

\version    	1.0.0
\date   		24/09/2009
\author			Valeria Catani
*/
void ITP_phaseTrackerReset(void)
{
	memset(&UIBackupData[0], 0, N_UI_PHASE_BMP);
}
#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */


/* *********** */
/* End of file */
/* *********** */
