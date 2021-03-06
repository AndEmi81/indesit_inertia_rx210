/**
\defgroup	APP_callback Modulo Application Platform Callback
\ingroup	ApplPlatform

Il modulo definisce le callback utilizzate nei moduli di Application Platform.
*/

/**
Source file del modulo Application Platform Callback

\file		APP_callback.c
\ingroup	APP_callback
\date		17/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

\par		PCLint
			Report 17/06/2009: 0 Errors, 0 Warnings, 0 Info

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

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
bool_t CBF_CurrentLoadedWater(uint32_t * LoadedWater);

/* ****************** */
/* Exported variables */
/* ****************** */
#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
/*extern const uint8_t Tab_phase2byte[];*/
#endif
/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Interpreter Callback per gestione di inizializzazioni specifiche dell'applicazione (es. CL_2008)
che vengono applicate prima dell'esecuzione di ogni iterazione del passo.

\warning		none		

\version		1.1.0
\date 			24/07/2009
\author			Valeria Catani
*/

void ITP_preStepInit(void)
{

 
}
/**
Interpreter Callback per gestione della continuit� dei carichi specifica per applicazione (es. HOTTIMA per var di pilotaggio
ventole o door lock)


\warning		none	
 se la richieste hanno valore dont'care anche Restore_loads_mng deve essere don't care , altimenti si richia di sovrascrivere un valore non corretto 
 e per la lampada dove ci sono priorità sulle richieste anche il valore zero è errato.
 quindi il bit più significativo di ogni campo indica se la regolazione ha senso o meno.  	
\note			inserita gestione di Restore_loads_mng per la continuità dei carichi
\version		1.1.1
\date 			11/09/2009
				Emanuela  Antognoli
				
\version		1.1.0
\date 			29/07/2009
				Catani valeria
*/
#if (ITP_LOADS_MGMT_ENABLE == 1)

#if (MULTI_COMPARTMENT_APPL == 1)	
void ITP_loadsMgmt(uint8_t compartment)
#else
void ITP_loadsMgmt(void)

#endif
{
  uint8_t cnt;
  	memcpy(&LoadsReq_New[0], &LoadsReq_Old[0], sizeof(LoadsReq_New));
	for(cnt=0 ;cnt<NR_OF_BLDC_PUMPS; cnt++)
	{
		BLDC_PumpReq_New[cnt] = BLDC_PumpReq_Old[cnt];
	}
}


#endif  /* (ITP_LOADS_MGMT_ENABLE == 1) */
/**
Interpreter Callback per gestione di inizializzazioni specifiche dell'applicazione
che vengono applicate nel momento del passaggio ad un nuovo step


\warning		none		

\version		1.1.0
\date 			30/07/2009
\author			Valeria Catani
*/
void ITP_newStepInit(void)
{

	/* Reset esecuzione Activate Algorithm - Lev To Time Ref Update */
	Flag_levTOtimeRef_update = FALSE; 

	SETBIT(TerminatedAlgorithmBmp[0], AAL_LEV_TO_TIME_REF_UPDATE_ALG);
	CLRBIT(ActiveAlgorithmBmp[0], AAL_LEV_TO_TIME_REF_UPDATE_ALG);
	CLRBIT(ActiveAlgorithmBmpOld[0], AAL_LEV_TO_TIME_REF_UPDATE_ALG);

	CLRBIT(Flags_1,JIF_FLAG1_MAINTAIN_TARGET_OK_POS);	/* reset bit Mantieni come Timeout */

	CLRBIT(uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT); /* reset flag caricamento dinamico */
    
	uP_AltCtrl_Data.SV_invalid_4alt=FALSE;
	
	uP_AltCtrl_Data.seq_done_could_esc=FALSE;
	
	uP_Load_Data.pump_act=FALSE;
	
	SETBIT(TerminatedAlgorithmBmp[0], AAL_READVIRTUALSENSOR_ALG);
	CLRBIT(ActiveAlgorithmBmp[0],AAL_READVIRTUALSENSOR_ALG);
	
}


/**
Tiene aggiornata una bitmap costituita dalle fasi
mandate in esecuzione dall'interprete e il ciclo di RUN 
correntemente in esecuzione 

\param [out] (global) UICycle
\param [out] (global) UIPhase
\param [out] (global) UIBackupData[]

\return     	none

\author			Simone Costantini
\date   		26/03/2008
\version    	1.0.0

*/
void ITP_phaseTrackerUI(void)
{
	ITP_syncUICyclePos(0);

	if (LBL_RUN == Interpreter_Data[0].completeLabel.s.label)
	{
		/* E' in esecuzione un ciclo di RUN */

	  /* Marca la fase */
	  SETBIT(UIBackupData[Tab_phase2byte[(Interpreter_Data[0].phase >> 3)]], (Interpreter_Data[0].phase & 0x07));

	  /* Indica il ciclo */
	  UIBackupData[INT_CYCLE_ACTIVE_POS] = Interpreter_Data[0].cycle;
	}

}

/**
Interpreter Callback per verificare se l'applicazione � in PROGRAMMING o END

\return			TRUE se sono in PROGRAMMING o END

\warning		none		

\version		1.1.0
\date 			18/03/2010
\author			Francesco Corvo 

\review			Emanuela Antognoli
\version		1.1.0
\date 			19/03/2010

\note logica invertita rispetto ad Arcadia per Hottima
*/
uint8_t STB_getStatusAppliance(void)
{	
	if ( (IsOvenRunningDelay() )|| (  IsOvenPauseFAult()) )
	{
			/* RUN, PAUSE, DELAY, FAULT : stati in cui non è ammesso Standby */
	
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
/**
Dato il puntatore pInd ad una struttura di tipo Condition_Struct,
restituisce il puntatore pData alla variabile identificata nella condizione
ed il puntatore pOpnd al valore con cui verra' effettuato il confronto al
momento di valutare la condizione.

\param [in]		pInd punta in Flash alla condizione
\param [out]	pData puntatore in RAM alla variabile specificata nella condizione
\param [out]	pOpnd punta in RAM al valore da confrontare
\attention 		JIF_DataTable (global, buffer puntatori alle varie categorie delle
				condizioni) e' usata come input

\return 		S_OK se i dati sono corretti, un codice di errore altrimenti.

\warning		none		

\version		1.1.0
\date 			18/12/2007
\author			Marco Quintini
*/
uint8_t SelectOperands(const Condition_Struct* pCondition, int32_t* pData,  int32_t* pOpnd)
{
	uint8_t dataType, dataIndex,mask;

	dataIndex = pCondition->dataSel.dataIndex;
	dataType =pCondition->dataSel.dataType;

	if((NULL == JIF_DataTable[dataType]) || (dataType >= SIZE(JIF_DataTable)))
	{
		return ERR_CODE;
	}

	*pOpnd = pCondition->operand.uw;
	switch (dataType)
	{
		case JIF_TAB_GI_PROBE_SEG:
		case JIF_TAB_GI_TURBIDITY_SEG:
		case JIF_TAB_FUNCTIONS_SEG:
		case JIF_TAB_BOOLEANS_SEG:
        case JIF_TAB_WASH_VS:
        case JIF_TAB_DRAIN_VS:
		/* array di byte */
		
			*pData = (uint16_t)((*((uint8_t **)(JIF_DataTable[dataType])))[dataIndex]);
			/* 
			NOTA:		
			(*((uint8_t **)(JIF_DataTable[dataType])))
			è un puntatore a uint8_t, essendo l'oggetto puntato da un puntatore a puntatore
			a uint8_t.
			E' possibile quindi applicargli l'operatore [DataIndex] oppure, in modo equivalente,
			sommargli DataIndex e poi estrarre il contenuto della somma, come nell'espressione
			sotto che equivale alla precedente:
			*pData = (uint16_t)(*((*((uint8_t **)(JIF_DataTable[dataType]))) + dataIndex));
			*/
					
		break;
		
		case JIF_TAB_GI_PRESSSENSOR_SEG:
		case JIF_TAB_GI_FLOW_METER_SEG:
		case JIF_TAB_LAST_FAULT:
		/* "array" (monodimensionale) di word */
		
			*pData = (uint16_t)( (*((uint16_t **)(JIF_DataTable[dataType]))) [dataIndex]);
			/* 
			NOTA:		
			(*((uint16_t **)(JIF_DataTable[dataType])))
			è un puntatore a uint16_t, essendo l'oggetto puntato da un puntatore a puntatore
			a uint16_t.
			E' possibile quindi applicargli l'operatore [dataIndex] oppure, in modo equivalente,
			sommargli dataIndex e poi estrarre il contenuto della somma, come nell'espressione
			sotto che equivale alla precedente:
			*pData = (uint16_t)(*((*((uint16_t **)(JIF_DataTable[dataType]))) + dataIndex));
			*/
		
		break;
		
		case JIF_TAB_GI_CRITERION_SEG:
		case JIF_TAB_FLAGS_SEG:
		/* bitmap */
			
			
			*pData = (uint16_t)(*(*((uint8_t **)(JIF_DataTable[dataType]) + dataIndex)));
			*pOpnd = pCondition->operand.uc.value;
			/* 
			NOTA:		
			(uint8_t **)(JIF_DataTable[dataType])
			è un puntatore a puntatore a uint8_t, dunque sommandogli DataIndex si ottiene
			sempre una grandezza omogenea: 
			((uint8_t **)(JIF_DataTable[dataType]) + dataIndex)
			che punta DataIndex puntatori più avanti. A questo punto:
			(*((uint8_t **)(JIF_DataTable[dataType]) + dataIndex))
			è un puntatore a uint8_t, essendo l'oggetto puntato da un puntatore a puntatore
			a uint8_t.
			*/
			 mask = pCondition->operand.uc.mask;
			*pOpnd &= mask;
			*pData &= mask;
			
		break;
		case JIF_TAB_ALPHA_COEFFICIENTS:
		/*alpha coefficients*/
			*pData = (uint16_t)(*(*((uint16_t **)(JIF_DataTable[dataType]) + dataIndex)));
			*pOpnd = dataIndex;
			 mask = pCondition->operand.uc.mask;
			*pOpnd &= mask;
			*pData &= mask;
		break;
		default:
		break;
	
	}

	return S_OK;
}

/**
Dato il puntatore pInd ad una struttura di tipo Condition_Struct,
restituisce il puntatore pData alla variabile identificata nella condizione
ed il puntatore pOpnd al valore con cui verra' effettuato il confronto al
momento di valutare la condizione.

\param [in]		pInd punta in Flash alla condizione
\param [out]	pData puntatore in RAM alla variabile specificata nella condizione
\param [out]	pOpnd punta in RAM al valore da confrontare
\attention 		JIF_DataTable (global, buffer puntatori alle varie categorie delle
				condizioni) e' usata come input

\return 		S_OK se i dati sono corretti, un codice di errore altrimenti.

\warning		none

\version		1.1.0
\date 			18/12/2007
\author			Marco Quintini
*/
uint8_t SelectOperandsAPP2_0(const Condition_Struct* pCondition, LIB_Fixed_ValueType* pData)
{
 uint8_t dataType, dataIndex;

	dataIndex = pCondition->dataSel.dataIndex;
	dataType =pCondition->dataSel.dataType;
	switch (dataType)
	{
		case JIF_TAB_ALPHA_COEFFICIENTS:
		/*alpha coefficients*/
			*pData = (LIB_Fixed_ValueType)(*(*((LIB_Fixed_ValueType **)(JIF_DataTable[dataType]) + dataIndex)));
		break;
		default:
			return S_FALSE;
		break;
	}
	return S_OK;
}

/**
Return the value of total estimated water loaded.

\return S_OK

\warning

\author         Pietro Loschiavo
\date           04.12.2013
\version        1.0.0

\note
*/

bool_t CBF_CurrentLoadedWater(uint32_t * LoadedWater)
{

        if (cycle_loaded_water < UINT32_T_MAX )
        {
                *LoadedWater = MachineStatistics.globalStats.water_loaded + cycle_loaded_water;
        }
        else
        {
                *LoadedWater = UINT32_T_MAX;
        }
        return (S_OK);
}


/* *************** */
/* Local Functions */
/* *************** */

/* *********** */
/* End of file */
/* *********** */
