/*
 Copyright (c) 2004-2005, Indesit Company, I.D.D..
 Tutti i diritti sono riservati. All rights reserved.
 */

/**
 \n
 \par		PM_Safety_CK_PM_Fan.c
 \n
 \n
 \file		PM_SFT_CK_Fan.c
 \ingroup	PM_Safety_CK
 \date		06/06/2013
 \author		Marco Sperandini
 \n
 */

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_APPLIANCE_CK)
#include "PM_SFT_CK_Fan.h"
#include "PM_SFT_CK_Common.h"
#include "PM_ConsistencyExp.h"
#include "LIB_Utilities.h"
#include "LIB_ClassBVar.h"
#if defined(CONFIG_LIB_CLASS_B_WDTEST)
#include "LIB_ResetExp.h"
#endif
#include "LIB_Trace.h"
/**
 3.12.4	Fan Faults Data

 15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
 +0000h	Fan Prefault time (100 msec)
 +0001h	Fan Fault time (sec)
 Fan Prefault / fault time: tempo per attivazione prefault/fault


 3.13.1.1	Fan Params
 15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
 +0000h		POR fan 	Vel_off_auto	Vel_start Up	Time_StartUp
 +0001h	Fan_TON_auto( 10 sec) 	Fan_TOFF_auto( 10 ms)

 POR fan: l’indice progressivo dell’output all’interno della categoria della ventola da pilotare al  POR.
 Questi parametri di pilotaggio vanno applicati all’accensione del motoventilatore (passaggio OFF-ON)

 Vel_off_auto Indica la velocità di pilotaggio della ventola circolare nel caso di spegnimento (o riduzione)
 periodico per Fan_TOFF_auto.
 La ventola tangenziale invece nella riduzione periodica utilizzerà esclusivamente la velocità 4.

 T_startup Indica la durata (espressa in 100ms) del pilotaggio (se = 0 non necessario startup)
 Vel_startup  Indica la velocità di pilotaggio

 Fan_TON_auto / Fan_TOFF_auto: durante una richiesta di attivazione motoventilatore alla max velocità
 questo viene spenta (o la velocità viene ridotta , vedi Vel_off_auto)
 dal fw per Fan_TOFF_auto ogni Fan_TON_auto.
 Le basi tempi per  Fan_TON_auto  sono in minuti in editor e 10 sec nel file di setting;
 mentre Fan_TOFF_auto è espressa in  10 ms .
 */

/**
 * In ogni caso se, dopo l’inizio della rilevazione di un guasto e prima che venga dichiarato il Fault,
 * il feedback torna a segnalare un valore coerente con lo stato,
 * la procedura di rilevazione del guasto viene resettata immediatamente
 * (se ancora non era stato dichiarato il Prefault)
 * o dopo un tempo di Reset Prefault impostabile da setting
 * (se era già stato dichiarato il Prefault/Fault).
 * Durante la rilevazione di uno stato di fault il motoventilatore
 * viene forzatamente pilotato alla velocità di rilevazione del fault
 * fino a quando o non si ripristina la condizione di errore
 * o non viene dichiarato il fault.
 * Questo perché ci sono casi di errore che vengono determinati
 * solo con alcune velocità delle ventole che quindi
 * devono permanere il tempo sufficiente per dichiarare il fault.
 *
 */

#define FAN_FLT_PRAM_TRIAC_PF_TimeOffset	(0) 		/* offset in byte */
#define FAN_FLT_PRAM_TRIAC_F_TimeOffset		(2)

#define FAN_SFT_PRAM_TON_Offset				(2)
#define FAN_SFT_PRAM_TOFF_Offset			(3)

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
#define FAN_SFT_PRAM_TOUT_RESET_Offset		(0)
#endif

/**
 * stati dell' autoOff_status
 */
#define STATE_AUTOOFF_INACTIVE		0
#define STATE_AUTOOFF_ON			1
#define STATE_AUTOOFF_OFF			2

#define SUB_STATE_WAIT_PF_RESET_NO		0
#define SUB_STATE_WAIT_PF_RESET_YES		1
#define SUB_STATE_WAIT_PF_RESET_TO		2
/**
 *  posizione delle ventole nell'array
 */
#define FAN_TANG_IDX	0
#define FAN_CIRC_IDX	1
#define FAN_MAX_IDX		2

/**
 * Indice dei profili utilizzati per convenzione dalle ventole
 */
#define ID_POFILE_MAX_VEL  (3)
#define ID_TANG_POFILE_AUTO_OFF (4)

/**
 * Identificatore dei parametri che puo fornire la geParam
 */
#define TO_PF				0
#define TO_F				1
#define TO_AUTOOFF_TON		2		/**< autoOff Ton	*/
#define TO_AUTOOFF_TOFF		3		/**< autooff Toff	*/
#define VEL_OFF_AUTO		4

/* #define FILTER_MR_CHANGE_STATUS */
#define MR_CONSISTENCY_FILTER	8		/**< filtro per stabilizzare la consistency x100 ms */
#define CK_FAN_ERROR_ENTER_FILTER	15	/**< filtro per entrare in warning, valido quando non in auto off */
#define CONSISTENCY_DONTCARE 	(PM_CS_NOT_VALID_CODE & 0x7)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum
{
	SFT_FAN_NONE,
	SFT_FAN_ERROR_ENTER,
	SFT_FAN_ERROR_EXIT,
	SFT_FAN_TIMEOUT,
} SafetyFanEvent_t;

typedef struct
{
	uint8_t lastLoadReq;
	uint8_t loadPos;
	uint8_t mrLoadPos;

	uint8_t lastConsistencyValue;
	uint8_t lastConsistencyFilterCnt;
#if defined(FILTER_MR_CHANGE_STATUS)
	/* filtro per il cambiamento di consistency dovuto al MR */
	uint8_t lastMRStatus;
	uint8_t cntMRStatus;
#endif

	uint8_t  autoOff_status;
	uint16_t autoOff_cnt;
	uint16_t autoOff_TOff;
	uint16_t autoOff_TOn;
	uint8_t autoOff_profile;
	uint8_t forcedLoadStatus;

} FanFaultStatus_t;

typedef struct
{
	uint8_t fanIdx;
	FanFaultStatus_t fan;
	sft_module_status_t safetyStatus;
	uint16_t cnt;
	uint16_t cntResetPrefault;
	uint8_t waitPrefaultReset;
	uint16_t timeoutPrefault;
	uint16_t timeoutFault;

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
	uint8_t mrResLoadPos;
	uint32_t resetTimer;	/* Reset for periodic self-test and monitoring */
	uint8_t delayResetCnt;
#endif

} FansFaultStatus_t;
/* ********* */
/* Local Functions */
/* ********* */
static void setTimeout(uint16_t* pCnt, uint16_t value);
static uint16_t getParam(sft_module_status_t *safetyData, uint8_t param);
static void PM_SFT_CK_SingleFan_Monitor(FansFaultStatus_t *pFanStatus, iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags);
void PM_SFT_CK_Fan_AutoOffMngr(FansFaultStatus_t* pFanStatus, iom_loads_struct_t *newLoads, uint8_t applReqFlags);
uint8_t getFaultPrio( uint8_t faultSrc);
#if defined(CONFIG_LIB_CLASS_B_WDTEST)
static bool_t PM_SFT_CK_Reset_Monitor(FansFaultStatus_t *pFanStatus, sft_module_status_t *_safetyData);
#endif

/* ********* */
/* Variables */
/* ********* */
static FansFaultStatus_t FAN_faultStatus[FAN_MAX_IDX]; /* reinizzializzata  a 0 con il reset freddo */


/* *************** */
/* Function Bodies */
/* *************** */
void setTimeout(uint16_t* pCnt, uint16_t value)
{
	if(value == 0){
		*pCnt = 1;
	}else{
		*pCnt = value;
	}
}
static void ResetFaultStatus(FansFaultStatus_t* pFanStatus)
{
	pFanStatus->fan.lastConsistencyValue = PM_CS_NOT_VALID_CODE;

	pFanStatus->cnt = 0;
	pFanStatus->waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
	pFanStatus->safetyStatus.status = SFT_MODULE_IDLE;
	pFanStatus->safetyStatus.faultSrc = 0;
}

uint8_t getFaultPrio( uint8_t faultSrc) /*TODO */
{
	uint8_t retVal;
	switch(faultSrc){
	case FAULTSRC_FAN_ERR_DM:
		retVal = 5;
	break;
	case FAULTSRC_FAN_ERR_FEEDBACK_KO:
		retVal = 4;
	break;
	case FAULTSRC_FAN_ERR_SC:
		retVal = 3;
	break;
	case FAULTSRC_FAN_ERR_OC:
		retVal=2;
	break;
	case FAULTSRC_FAN_MR:
	default:
		retVal = 1;
	}
	return retVal;
}

uint16_t getParam(sft_module_status_t *safetyData, uint8_t param)
{
	uint16_t retVal = 0;

	Loader_Struct data;

	if (param < TO_AUTOOFF_TON)
	{
		if (BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_FAN, &data))
		{
			/* ERRORE */
			safetyData->status = SFT_MODULE_FAULT;
			safetyData->faultSrc = FAULTSRC_FAN_BAD_SETTING;
		}
		else
		{
			switch (param)
			{
			case TO_PF:/* t time x100ms, timeout warning, per poi entrare in prefault,  */
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + FAN_FLT_PRAM_TRIAC_PF_TimeOffset))));
				break;
			case TO_F:/* time in sec, timeout prefault, per poi entrare in fault */
				retVal = *(uint8_t*) ((uint8_t*) data.Data + FAN_FLT_PRAM_TRIAC_F_TimeOffset) * 10;
				break;
			}
		}
	}
	else
	{
		if (BasicLoader(SF_PTR_SAFETY_PARAMETERS, SF_DISPL_SFT_FAN, &data))
		{
			/* ERRORE */
			safetyData->status = SFT_MODULE_FAULT;
			safetyData->faultSrc = FAULTSRC_FAN_BAD_SETTING;
		}
		else
		{
			switch (param)
			{
			case TO_AUTOOFF_TON:
				retVal = *((uint8_t*) data.Data + FAN_SFT_PRAM_TON_Offset); /**< x 10sec */
				retVal *= 100;
				break;
			case TO_AUTOOFF_TOFF:
				retVal = *((uint8_t*) data.Data + FAN_SFT_PRAM_TOFF_Offset);/**< x 10 ms */
				retVal /= 10;
				if (retVal == 0)
					retVal = 1;
				break;
			case VEL_OFF_AUTO: /* indice del profilo da usare in auto off, solo per la fan circolare */
				retVal = ((*((uint8_t*) data.Data))>>2) & 0x03;
				PLT_ASSERT(retVal < 4)
				break;
			}
		}
	}

	return retVal;
}

/* ------------------------------------------------------------*/

void PM_SFT_CK_FanTang_Init(sft_module_status_t *safetyData)
{ /* initialize safety module */

	SAFETY_REGION_START();
	
	safetyData->status = SFT_MODULE_IDLE;

	FAN_faultStatus[FAN_TANG_IDX].fanIdx = FAN_TANG_IDX;
	ResetFaultStatus(&FAN_faultStatus[FAN_TANG_IDX]);
	FAN_faultStatus[FAN_TANG_IDX].fan.loadPos = LIB_IOM_GetLoadPos(LD_TANGENZIAL_FAN_POS);
	FAN_faultStatus[FAN_TANG_IDX].fan.mrLoadPos = LIB_IOM_GetLoadPos(LD_MASTER_RELAY_FAN_POS);

	PLT_ASSERT(getParam(safetyData, TO_PF));
	PLT_ASSERT(getParam(safetyData, TO_F));
	PLT_ASSERT(getParam(safetyData, TO_AUTOOFF_TON));
	PLT_ASSERT(getParam(safetyData, TO_AUTOOFF_TOFF));
	PLT_ASSERT(getParam(safetyData, VEL_OFF_AUTO));

	PLT_ASSERT(safetyData->faultSrc == 0);

	FAN_faultStatus[FAN_TANG_IDX].timeoutPrefault = getParam(safetyData, TO_PF);
	FAN_faultStatus[FAN_TANG_IDX].timeoutFault = getParam(safetyData, TO_F);

	FAN_faultStatus[FAN_TANG_IDX].fan.autoOff_TOff = getParam(safetyData, TO_AUTOOFF_TOFF);
	FAN_faultStatus[FAN_TANG_IDX].fan.autoOff_TOn  = getParam(safetyData, TO_AUTOOFF_TON);
	FAN_faultStatus[FAN_TANG_IDX].fan.autoOff_profile  = ID_TANG_POFILE_AUTO_OFF;
	FAN_faultStatus[FAN_TANG_IDX].fan.autoOff_status = STATE_AUTOOFF_INACTIVE;

	FAN_faultStatus[FAN_TANG_IDX].safetyStatus = *safetyData;

	SAFETY_DATA_ELEM_UPDATE( FAN_faultStatus, FAN_TANG_IDX );
	SAFETY_REGION_END();

}

void PM_SFT_CK_FanCirc_Init(sft_module_status_t *safetyData)
{ /* initialize safety module */

	SAFETY_REGION_START();

	safetyData->status = SFT_MODULE_IDLE;

	FAN_faultStatus[FAN_CIRC_IDX].fanIdx = FAN_CIRC_IDX;
	ResetFaultStatus(&FAN_faultStatus[FAN_CIRC_IDX]);
	FAN_faultStatus[FAN_CIRC_IDX].fan.loadPos = LIB_IOM_GetLoadPos(LD_CIRCULAR_FAN_POS);
	FAN_faultStatus[FAN_CIRC_IDX].fan.mrLoadPos = LIB_IOM_GetLoadPos(LD_MASTER_RELAY_FAN_POS);

	PLT_ASSERT(getParam(safetyData, TO_PF));
	PLT_ASSERT(getParam(safetyData, TO_F));
	PLT_ASSERT(getParam(safetyData, TO_AUTOOFF_TON));
	PLT_ASSERT(getParam(safetyData, TO_AUTOOFF_TOFF));
	PLT_ASSERT(getParam(safetyData, VEL_OFF_AUTO));

	PLT_ASSERT(safetyData->faultSrc == 0);
	FAN_faultStatus[FAN_CIRC_IDX].timeoutPrefault = getParam(safetyData, TO_PF);
	FAN_faultStatus[FAN_CIRC_IDX].timeoutFault = getParam(safetyData, TO_F);

	FAN_faultStatus[FAN_CIRC_IDX].fan.autoOff_TOff = getParam(safetyData, TO_AUTOOFF_TOFF);
	FAN_faultStatus[FAN_CIRC_IDX].fan.autoOff_TOn  = getParam(safetyData, TO_AUTOOFF_TON);
	FAN_faultStatus[FAN_CIRC_IDX].fan.autoOff_profile  = getParam(safetyData, VEL_OFF_AUTO);
	FAN_faultStatus[FAN_CIRC_IDX].fan.autoOff_status = STATE_AUTOOFF_INACTIVE;
	FAN_faultStatus[FAN_CIRC_IDX].safetyStatus = *safetyData;

	SAFETY_DATA_ELEM_UPDATE( FAN_faultStatus, FAN_CIRC_IDX );
	SAFETY_REGION_END();
}


void PM_SFT_CK_Fan_AutoOffMngr(FansFaultStatus_t* pFanStatus, iom_loads_struct_t *newLoads, uint8_t applReqFlags)
{
	uint8_t loadStatus = newLoads->loads_req[ pFanStatus->fan.loadPos];
	/**
	 * quando il carico passa da spento ad acceso inizia il timeout per il T_on
	 * se il T_on è scaduto spegnere lo il carico e far partire il timeout per T_off
	 * se il T_off è scaduto accendere il carico e attivare il timeout per T_on
	 *
	 */
	if (loadStatus != pFanStatus->fan.lastLoadReq )
	{
		if (loadStatus == ID_POFILE_MAX_VEL)
		{ /* attiva solo se è nel profilo  HIGH SPEED*/
			pFanStatus->fan.autoOff_status = STATE_AUTOOFF_ON;
			pFanStatus->fan.autoOff_cnt = pFanStatus->fan.autoOff_TOn; /*setAutoOffTimeout*/
		}
	}else{
		 if(ID_POFILE_MAX_VEL != loadStatus){ /* disattiva l'auto off */
			 pFanStatus->fan.autoOff_status = STATE_AUTOOFF_INACTIVE;
		 }
	}

	if(loadStatus) /* solo in caso di carico on */
	{
		if (pFanStatus->fan.autoOff_cnt > 0)
		{
			pFanStatus->fan.autoOff_cnt--;
		}

		if (pFanStatus->fan.autoOff_cnt == 0)
		{ /* timeout scaduto */
			switch(pFanStatus->fan.autoOff_status){
			case STATE_AUTOOFF_ON:
				pFanStatus->fan.autoOff_status = STATE_AUTOOFF_OFF;
				pFanStatus->fan.autoOff_cnt = pFanStatus->fan.autoOff_TOff;

			break;
			case STATE_AUTOOFF_OFF:
				pFanStatus->fan.autoOff_status = STATE_AUTOOFF_ON;
				pFanStatus->fan.autoOff_cnt = pFanStatus->fan.autoOff_TOn;

			break;
			}
		}
	}

	if (pFanStatus->fan.autoOff_status == STATE_AUTOOFF_OFF)
	{
		newLoads->loads_ovr[pFanStatus->fan.loadPos] = pFanStatus->fan.autoOff_profile;
	}

	pFanStatus->fan.lastLoadReq = loadStatus;
}

void PM_SFT_CK_FanTang_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	bool_t resetExe = 0;

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK(FAN_faultStatus,FAN_TANG_IDX);

	/** deve essere presente la ventola tangenziale e il master rele delle ventole,
	 * altrimenti la safety è inattiva, notificare al supervisor???????
	**/
	if (       (LOAD_NOT_FOUND == FAN_faultStatus[FAN_TANG_IDX].fan.loadPos)
			|| (LOAD_NOT_FOUND == FAN_faultStatus[FAN_TANG_IDX].fan.mrLoadPos))
	{
		/** load not present */
	}else{
		PM_SFT_CK_SingleFan_Monitor(&FAN_faultStatus[FAN_TANG_IDX], newLoads, safetyData, applReqFlags);
	}

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
	resetExe = PM_SFT_CK_Reset_Monitor(&FAN_faultStatus[FAN_TANG_IDX],safetyData);
#endif

	SAFETY_DATA_ELEM_UPDATE( FAN_faultStatus,FAN_TANG_IDX );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
	if (resetExe) {
		LIB_Reset_ForceReset();
	}
#endif


}

void PM_SFT_CK_FanCirc_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK(FAN_faultStatus,FAN_CIRC_IDX);

	/** deve essere presente la ventola e il master rele delle ventole,
	 * altrimenti la safety è inattiva, notificare al supervisor???????
	**/
	if (       (LOAD_NOT_FOUND == FAN_faultStatus[FAN_CIRC_IDX].fan.loadPos)
			|| (LOAD_NOT_FOUND == FAN_faultStatus[FAN_CIRC_IDX].fan.mrLoadPos))
	{
		/** load not present */
	}else{
		PM_SFT_CK_SingleFan_Monitor(&FAN_faultStatus[FAN_CIRC_IDX], newLoads, safetyData, applReqFlags);
	}

	SAFETY_DATA_ELEM_UPDATE( FAN_faultStatus,FAN_CIRC_IDX );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);
}

void PM_SFT_CK_SingleFan_Monitor(FansFaultStatus_t *pFanStatus, iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	uint8_t consistencyValue;
	SafetyFanEvent_t event = SFT_FAN_NONE;
#if defined(FILTER_MR_CHANGE_STATUS)
	uint8_t currentMRStatus;
#endif

	PM_Consistency_IsValueReady(pFanStatus->fan.loadPos, &consistencyValue);

#if defined(FILTER_MR_CHANGE_STATUS)
	/* filtro per il cambiamento di consistency dovuto al MR */
	currentMRStatus = LIB_IOM_GetLoadStatus(pFanStatus->fan.mrLoadPos);
	if(	(currentMRStatus  != pFanStatus->fan.lastMRStatus)
			|| ( LIB_IOM_FindLoadOnMasterRelay(pFanStatus->fan.mrLoadPos) & LOAD_MR_TRANSITION))
	{
		pFanStatus->fan.cntMRStatus = MR_CONSISTENCY_FILTER;
	}
	pFanStatus->fan.lastMRStatus = currentMRStatus;

	if( pFanStatus->fan.cntMRStatus > 0 ){
		pFanStatus->fan.cntMRStatus--;
		consistencyValue = PM_CS_NOT_VALID_CODE;
	}
#endif

	consistencyValue &= 0x07;

		/*NOTA: ricalcolo il consistency tenendo conto dello stato del MR */
	if(    LIB_IOM_GetLoadStatus(pFanStatus->fan.mrLoadPos)  == LOAD_REG_ON		/*MR FAN ON */
		&& LIB_IOM_GetLoadStatus(pFanStatus->fan.loadPos)    == LOAD_REG_OFF 	/*UNPILOT   */
	)
	{
		/* (MR FAN OFF)+UNPILOT	   |	2	|	0|	6	|	3 	    |*/
		/* (MR FAN ON) +UNPILOT	   |	2	|	3|	6	|	0		|*/
		if (consistencyValue == 0) {
			consistencyValue = 3;
		} else if (consistencyValue == 3) {
			consistencyValue = 0;
		}
	}

	if( pFanStatus->fan.lastConsistencyValue != consistencyValue
			&& consistencyValue != CONSISTENCY_DONTCARE)
	{
		/* change status */
		/** lo stato puo cambiare perche':
		 * - cambia lo stato (OFF/ON) del MR FAN con carico che resta UNPILOT (OFF);
		 * - in generale se cambia valore Consistency;
		 * lo stato invece non cambia se:
		 * - cambia il pilotaggio PILOT/UNPILOT con MR FAN ON (non rilevante).
		 * Si faccia riferimento alla seguente tabella di valori rilevati dal consistency carico
		 * ventola Circolare, uguale a quelli della ventola Tangenziale,
		 * col 3, riportato tra parentesi:
		 * FFB					|	0	|	1|	2	|	3		|
		 * FB					|	L	|	H|	50Hz|	100Hz	|
		 *
		 * (MR FAN OFF)+UNPILOT	|	2	|	0|	6	|	3 	    |
		 * (MR FAN ON) +PILOT	|	2	|	1|	6	|	0		|
		 * (MR FAN ON) +HI SPEED|	2	|	0|	6	|	4	 	|
		 *
		 * aggiunta via FW
		 * (MR FAN ON) +UNPILOT	|	2	|	3|	6	|	0		|
		 */

		if (consistencyValue == PM_CS_VALUE_OK) {
			event = SFT_FAN_ERROR_EXIT;
			pFanStatus->fan.lastConsistencyFilterCnt=0;
		} else {
			if (pFanStatus->fan.autoOff_status != STATE_AUTOOFF_OFF)
			{  /* Quando non è in auto off per verificare il triac incorto applia un filtro*/
				pFanStatus->fan.lastConsistencyFilterCnt++;
				if(pFanStatus->fan.lastConsistencyFilterCnt >= CK_FAN_ERROR_ENTER_FILTER)
				{
					event = SFT_FAN_ERROR_ENTER;
					pFanStatus->fan.lastConsistencyFilterCnt=0;
				}else{
					consistencyValue = pFanStatus->fan.lastConsistencyValue;
				}
			}else{ /* se è in auto off per verificare il modo diodo non viene applicato il filtro */
				event = SFT_FAN_ERROR_ENTER;
			}
		}
	}
	else
	{
		if (pFanStatus->cnt > 0)
		{
			pFanStatus->cnt--;
			if (0 == pFanStatus->cnt)
			{
					event = SFT_FAN_TIMEOUT;
				}
			}
		}

	PM_SFT_CK_Fan_AutoOffMngr(pFanStatus, newLoads, applReqFlags);

	switch (pFanStatus->safetyStatus.status)
	{
	case SFT_MODULE_IDLE:

		if (SFT_FAN_ERROR_ENTER == event )
		{
			pFanStatus->safetyStatus.status = SFT_MODULE_WARNING;
			pFanStatus->fan.forcedLoadStatus = LIB_IOM_GetLoadStatus(pFanStatus->fan.loadPos);

			if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
			{ /** selftest */
				setTimeout(&pFanStatus->cnt,  SFT_TEST_PREFAULT_TIMEOUT_100MS);
			}
			else
			{
				uint16_t timeout;
				timeout = pFanStatus->timeoutPrefault;
				setTimeout(&pFanStatus->cnt, timeout);
			}
		}

		break;
	case SFT_MODULE_WARNING:
	case SFT_MODULE_PROTECTION:

		if (SFT_FAN_TIMEOUT == event)
		{
			pFanStatus->safetyStatus.status = SFT_MODULE_PREFAULT;

			if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
			{ /** seltest */
				setTimeout(&pFanStatus->cnt, SFT_TEST_FAULT_TIMEOUT_100MS);
			}
			else
			{
				setTimeout(&pFanStatus->cnt, pFanStatus->timeoutFault);
			}
		}
		else if (SFT_FAN_ERROR_EXIT == event)
		{
			ResetFaultStatus(pFanStatus);
		}

		break;
	case SFT_MODULE_PREFAULT:

		if (SFT_FAN_TIMEOUT == event      && SUB_STATE_WAIT_PF_RESET_NO == pFanStatus->waitPrefaultReset )
		{
			pFanStatus->safetyStatus.status = SFT_MODULE_FAULT;
		}else if((SFT_FAN_TIMEOUT == event) && ( SUB_STATE_WAIT_PF_RESET_YES == pFanStatus->waitPrefaultReset) ){
			if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
			{ /** seltest */
				setTimeout(&pFanStatus->cnt, SFT_TEST_FAULT_TIMEOUT_100MS);
			}else{
				pFanStatus->waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_TO; /* stato waitPrefaultReset con PF Timeout scaduto */
			}
		}
		else if (SFT_FAN_ERROR_EXIT == event)
		{
			pFanStatus->waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_YES;
			pFanStatus->cntResetPrefault = FLT_RESET_TIMEOUT_VALUE;
		}
		else if (SFT_FAN_ERROR_ENTER == event)
		{
			if(SUB_STATE_WAIT_PF_RESET_TO == pFanStatus->waitPrefaultReset ){ /* rientra in condizine di errore ed il timeout del prefault è gia scaduto */
				pFanStatus->safetyStatus.status = SFT_MODULE_FAULT;
			}
			pFanStatus->cntResetPrefault = 0;
			pFanStatus->waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
		}

		if (pFanStatus->waitPrefaultReset)
		{
			if (pFanStatus->cntResetPrefault > 0)
			{
				pFanStatus->cntResetPrefault--;
			}
			else
			{
				ResetFaultStatus(pFanStatus);
			}
		}
		break;
	case SFT_MODULE_FAULT:

		/* stato senza uscita fino a richiesta esterna */
		if (applReqFlags & SFT_FLAGS_RESET_REQ)
		{
			pFanStatus->safetyStatus.status = SFT_MODULE_RESET;
		}
		break;
	case SFT_MODULE_RESET:
	case SFT_MODULE_MONITORING:
	default:
		ResetFaultStatus(pFanStatus);
		break;
	}

	if (pFanStatus->safetyStatus.status == SFT_MODULE_IDLE)
	{
		pFanStatus->safetyStatus.faultSrc = 0;
	}
	else if(pFanStatus->safetyStatus.status != SFT_MODULE_FAULT  /* in fault non cambia fault source */
			&& (CONSISTENCY_DONTCARE != consistencyValue ) /* consistency invalida , non cambia fault source */
			)
	{
		uint8_t tmp_faultSrc=0;
		/* get fault source */

		if (consistencyValue == PM_CS_VALUE_ERR_FEEDBACK_KO) 	/* consistency == 2 quindi FB == 0 */
		{
			tmp_faultSrc = FAULTSRC_FAN_ERR_FEEDBACK_KO;	/*0x74/0x84   Fb guasto*/
		}else
		if (consistencyValue == PM_CS_VALUE_ERR_DM) 			/*consistency == 6 quindi FB == 2 */
		{
			tmp_faultSrc = FAULTSRC_FAN_ERR_DM; 			/*0x71/0x81  triac in modo diodo.*/
		}else
		if(consistencyValue == PM_CS_VALUE_ERR_GENERIC) 		/* consistency == 1 quindi FB == 1*/
		{
				tmp_faultSrc = FAULTSRC_FAN_MR; 			/*0x75/0x85 generic error*/
		}else
		if(consistencyValue == PM_CS_VALUE_ERR_SC) 				/* consistency == 3 quindi FB == 3 */
		{
			tmp_faultSrc = FAULTSRC_FAN_ERR_SC; 			/*0x72/0x82 triac in corto circuito. */
		}else
		if ( (consistencyValue == PM_CS_VALUE_ERR_OC) /* consistency == 4 quindi FB==3 */
				&& ( ID_POFILE_MAX_VEL == newLoads->loads_req[ pFanStatus->fan.loadPos] )
		)
		{
			tmp_faultSrc = FAULTSRC_FAN_ERR_OC;				/*0x73/0x83 triac aperto */
		}else{ /* se non corrisponde ad un valore previsto per la tab del consistency */
			tmp_faultSrc = FAULTSRC_FAN_MR;				/*0x75/0x85 generic error*/
		}

		if(pFanStatus->safetyStatus.faultSrc == 0){
			pFanStatus->safetyStatus.faultSrc = tmp_faultSrc;
		}

		if(getFaultPrio(tmp_faultSrc) > getFaultPrio(pFanStatus->safetyStatus.faultSrc)){
			pFanStatus->safetyStatus.faultSrc = tmp_faultSrc;
		}
	}

	if(CONSISTENCY_DONTCARE != consistencyValue){
		pFanStatus->fan.lastConsistencyValue = consistencyValue;
	}

	if(pFanStatus->safetyStatus.status != SFT_MODULE_IDLE){
		newLoads->loads_ovr[pFanStatus->fan.loadPos] = pFanStatus->fan.forcedLoadStatus;
	}

	safetyData->faultSrc = pFanStatus->safetyStatus.faultSrc;
	safetyData->status = pFanStatus->safetyStatus.status;
}

uint16_t PM_SFT_CK_FanTang_GetTime(sft_module_status_t *status)
{
	return FAN_faultStatus[FAN_TANG_IDX].cnt;
}

uint16_t PM_SFT_CK_FanCirc_GetTime(sft_module_status_t *status)
{
	return FAN_faultStatus[FAN_CIRC_IDX].cnt;
}


#if defined(CONFIG_LIB_CLASS_B_WDTEST)
/* Reset for periodic self-test and monitoring */
static bool_t PM_SFT_CK_Reset_Monitor(FansFaultStatus_t *pFanStatus, sft_module_status_t *_safetyData)
{
	bool_t resetActive = 0;
	uint16_t retVal = 0;
	Loader_Struct data;

	static uint8_t firsTime = 0;

	if( (MR_RES_OPEN) && (MR_FAN_OPEN) )
	{
		/* inserito un meccanismo di esecuzione prima volta per vincolare l'esecuzione del self-reset via reset caldo
		 * alla chiusura di almeno uno dei due MR: condizione di start ciclo PLT */
		if(firsTime == 1)
		{
			/* inserito un delay-timer che consenta l'esecuzione del reset non in concomitanza alla simultanea
			 * apertura dei due MR, che avviene al termine della fase di programming del sistema */
			pFanStatus->delayResetCnt++;
			if(pFanStatus->delayResetCnt == DELAY_RESET_1s)
			{
				pFanStatus->delayResetCnt = 0;
				firsTime = 0;
				pFanStatus->resetTimer = 0;

				resetActive = 1;
			}

		}
	}
	/* Se uno dei due MR è ON (condizione START ciclo): */
	else if(MR_RES_CLOSED || MR_FAN_CLOSED)
	/*if( (LIB_IOM_GetLoadStatus(pFanStatus->mrResLoadPos))	||
		(LIB_IOM_GetLoadStatus(pFanStatus->fan.mrLoadPos)) )	*/
	{
		/* Se Timer Reset ha valore nullo): */
		if(pFanStatus->resetTimer == 0)
		{
			/* Impostazione e start timeout */
			/* xxx[min]*60[s]=xxx[s]*10[100ms]=xxx[0.1s] => fattore moltiplicativo = 600 per
			 * convertire l'unità di misura minuti, in SF, in decimi di secondo */
			pFanStatus->mrResLoadPos = LIB_IOM_GetLoadPos(LD_MASTER_RELAY_RES_POS);
			if( (BasicLoader(SF_PTR_SAFETY_PARAMETERS, SF_DISPL_SFT_SYSTEM, &data)) ||
				(LOAD_NOT_FOUND == pFanStatus->mrResLoadPos) )
			{
				_safetyData->status = SFT_MODULE_FAULT;
				_safetyData->faultSrc = FAULTSRC_FAN_BAD_SETTING;
			}
			else
			{
			retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + FAN_SFT_PRAM_TOUT_RESET_Offset))));
			}

			pFanStatus->resetTimer = (uint32_t)(retVal*600);

		}

		firsTime = 1;

	}

	if(pFanStatus->resetTimer)
	{
		pFanStatus->resetTimer--;

		/* Se valore Timer Reset == 1 */
		if(pFanStatus->resetTimer == 1)
		{
			resetActive = 1;
		}
	}

	return resetActive;

}
#endif


#endif /* #if defined(CONFIG_APPLIANCE_CK) */
