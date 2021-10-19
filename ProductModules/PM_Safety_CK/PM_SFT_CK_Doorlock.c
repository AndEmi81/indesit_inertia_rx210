/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_CK_DL
\n
\n
\file		PM_SFT_CK_Doorlock.c
\ingroup	PM_Safety_CK
\date		13/02/2013
\author		Marco Sperandini
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SFT_CK_Common.h"
#include "PM_SFT_CK_Doorlock.h"
#include "PM_SFT_CK_PTCProbe.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "PM_DoorLockExp.h"
#include "LIB_Utilities.h"
#include "LIB_ClassBVar.h"
#include "PM_SFT_CK_SafetyActions.h"
#if defined(CONFIG_PM_DOORLOCK_PULSE_ENABLE)
#include "PM_DoorLock_Pulse.h"
#endif


/*#define TEST_PM_SFT_CK*/

#if defined(TEST_PM_SFT_CK)
#include "TEST_PM_SFT_CK.h"
#else
#define TEST_PM_SFT_CK_MONITOR(n)	{}
#endif

/**
 * 3.12.1	Door Lock Faults Data
		15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
+0000h	Lock Temperature  (°C)
+0001h	Unlock Temperature  (°C)
+0002h	Door Lock Prefault time (100 ms)
+0003h	Max Unlock Pause (120 min)	 | Door Lock Fault time Value
		Delta Temp Pause Unlock activation (def 20 °C)  | 	Num Attempt on Lock Temperature  (default  10 )
 *
 */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum {
	SFT_DL_NONE,
	SFT_DL_ERROR_ENTER,
	SFT_DL_ERROR_EXIT,
	SFT_DL_TIMEOUT,
	SFT_DL_OVER_TEMP,
	SFT_DL_DOWN_TEMP,
	SFT_DL_END_FILTERING,

} SafetyDoorLockEvent_t;
typedef struct {
	sft_module_status_t lastStatus;
	uint8_t  loadPos;
	uint8_t  lastConsistencyValue;
	uint16_t cnt;
	uint16_t warningTimeout;    /* tempo massimo di permanenza in warning */
	uint16_t prefaultTimeout;	/* tempo massimo di permanenza in prefault */
	uint16_t cntResetPrefault;
	uint8_t  waitPrefaultReset;


	/*safty protection */
	uint8_t  substateProtection;
	uint16_t  filterTimeCnt;
	uint16_t  filterTime;

	uint16_t rawLockTemperature;
	uint16_t rawUnlockTemperature;

	/*fault reset */
	uint8_t  substateFault;
	uint8_t  retryCounter;

} DoorLockFaultStatus_t;

/* ********* */
/* Variables */
/* ********* */
static DoorLockFaultStatus_t DL_faultStatus = { LOAD_NOT_FOUND };
typedef enum {
	DL_FLT_LockTemperature,		/**< Lock Temperature  (°C) */
	DL_FLT_UnLockTemperature,	/**< Unlock Temperature  (°C) */
	DL_FLT_PrefaultTime, 		/**< Door Lock Prefault time : tempo per attivazione prefault (default 100 msec = 1* 100ms). */
	DL_FLT_MaxUnlockPause, 		/**< Max Unlock Pause (120 min) */
	DL_FLT_FaultTime, 			/**< Door Lock Fault time: tempo per attivazione fault (default 60 sec) */
	DL_FLT_DTPUA, 				/**< Delta Temp Pause Unlock activation (def 20 °C) */
	DL_FLT_NALT, 				/**< Num Attempt on Lock Temperature  (default  10 ) */

	/* nuovi parametri, decidere se mettere sui parameti di fault o su quelli di safty Probe main */
	DL_SFT_RawLockTemp,
	DL_SFT_RawUnlockTemp,
	DL_SFT_TimeTempFilter, /**< time filter (in Seconds )  on temperature to allow door unlock*/
	DL_NUM_PARAMS /**< numero dei parametri previsti nel setting */
} DL_ParamId;

#define PARAM_OFFSET_LockTemperature	0
#define PARAM_OFFSET_UnLockTemperature	2
#define PARAM_OFFSET_PrefaultTime		4
#define PARAM_OFFSET_MaxUnlockPause		6
#define PARAM_OFFSET_FaultTime			7
#define PARAM_OFFSET_DTPUA				8
#define PARAM_OFFSET_NALT				9

#define PARAM_OFFSET_RawLockTemp		0
#define PARAM_OFFSET_RawMaxTemp			2
#define PARAM_OFFSET_TimeTempFilter		4


#define SUB_STATE_WAIT_PF_RESET_NO		0
#define SUB_STATE_WAIT_PF_RESET_YES		1
#define SUB_STATE_WAIT_PF_RESET_TO		2

#define SUB_STATE_PROTECTION_OVER_IDLE	0
#define SUB_STATE_PROTECTION_OVER_TEMP	1
#define SUB_STATE_PROTECTION_FILTERING	2

#if 0
#define CONFIG_RESET_FAULT_TIMEOUT				(10*10) 		/* 10 sec */
#define CONFIG_SAFETY_RETRY_UNLOCK_TIMEOUT		(2 *(10*60))	/* 2 min */
#define CONFIG_SAFETY_MAX_RETRY_UNLOCK			(3)				/* numero massimo di tentativi di sblocco in fault F7.02 */
#endif

#define FST_FAULT_UNRECOVERABLE		0
#define FST_WAIT_FAULT_RESET		1
#define FST_WAIT_RETRY_UNLOCK		2

SafetyDoorLockEvent_t generateEvent(void);
void EnterFaultState(void);
/* *************** */
/* Function Bodies */
/* *************** */

static uint16_t PM_SFT_CK_DL_GetParam(DL_ParamId paramId, sft_module_status_t *safetyData)
{
	uint16_t retVal = 0;
	uint8_t err=0;
	Loader_Struct data;
	if (paramId < DL_NUM_PARAMS)
	{
		if(paramId<DL_SFT_RawLockTemp){
			err = BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DOORLOCK, &data);
		}else{
			err = BasicLoader(SF_PTR_SAFETY_PARAMETERS, SF_DISPL_SFT_DOORLOCK, &data);
		}

		if( !err )
		{
			switch (paramId){
#if 0 /* parametri non usati dal modulo */
			case DL_FLT_LockTemperature:		/**< Lock Temperature  (°C) */
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_LockTemperature))));
				break;
			case DL_FLT_UnLockTemperature:	/**< Unlock Temperature  (°C) */
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_UnLockTemperature))));
				break;
			case DL_FLT_MaxUnlockPause: 		/**< Max Unlock Pause (120 min) */
				retVal = (*((uint8_t*)((uint8_t*)data.Data + PARAM_OFFSET_MaxUnlockPause)));
				break;
			case DL_FLT_DTPUA: 				/**< Delta Temp Pause Unlock activation (def 20 °C) */
				retVal = (*((uint8_t*)((uint8_t*)data.Data + PARAM_OFFSET_DTPUA)));
				break;
			case DL_FLT_NALT:
				retVal = (*((uint8_t*)((uint8_t*)data.Data + PARAM_OFFSET_NALT)));
				break;
#endif
			case DL_FLT_PrefaultTime: 		/**< Door Lock Prefault time : tempo per attivazione prefault (default 100 msec = 1* 100ms). */
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_PrefaultTime))));
				break;
			case DL_FLT_FaultTime: 			/**< Door Lock Fault time: tempo per attivazione fault (default 60 sec) */
				retVal = 10*(*((uint8_t*)((uint8_t*)data.Data + PARAM_OFFSET_FaultTime)));
				break;

			case DL_SFT_RawLockTemp:
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_RawLockTemp))));
				break;
			case DL_SFT_RawUnlockTemp:
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_RawMaxTemp))));
				break;
			case DL_SFT_TimeTempFilter:/* valore in secondi */
				retVal = 10*(*((uint16_t*)((uint8_t*)data.Data + PARAM_OFFSET_TimeTempFilter)));
				break;
			default:
				break;
			}
		}
	}

	if(err)
	{
		/* ERRORE */
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_DOORLOCK_BAD_SETTING;
	}
	return retVal;
}


static void ResetFaultStatus(sft_module_status_t *safetyData){
	DL_faultStatus.lastConsistencyValue = PM_CS_NOT_VALID_CODE;
	DL_faultStatus.cnt = 0;
	safetyData->status = SFT_MODULE_IDLE;
	DL_faultStatus.lastStatus.status = SFT_MODULE_IDLE;
	DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
}

static void resetTimeout(void){
	DL_faultStatus.cnt = 0;
}

static void setTimeout(uint16_t value){
	if(value == 0){
		DL_faultStatus.cnt = 1;
	}else{
		DL_faultStatus.cnt = value;
	}
}


void PM_SFT_CK_Doorlock_Init( sft_module_status_t *safetyData )
{  /* initialize safety module */
	uint8_t err=0;
	SAFETY_REGION_START();

	ResetFaultStatus(safetyData);

	DL_faultStatus.loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

	if( LOAD_NOT_FOUND != DL_faultStatus.loadPos ){

		err = PM_DoorLock_CheckConfig();

		PLT_ASSERT(PM_SFT_CK_DL_GetParam(DL_FLT_PrefaultTime, safetyData)>0)
		PLT_ASSERT(PM_SFT_CK_DL_GetParam(DL_FLT_FaultTime, safetyData)>0)

		PLT_ASSERT(PM_SFT_CK_DL_GetParam(DL_SFT_RawLockTemp, safetyData) < 1024)
		PLT_ASSERT(PM_SFT_CK_DL_GetParam(DL_SFT_RawUnlockTemp, safetyData)  < 1024)
		PLT_ASSERT(PM_SFT_CK_DL_GetParam(DL_SFT_TimeTempFilter, safetyData)>0)


	}

	/* Prefault time x100ms, timeout warning, per poi entrare in prefault, FLT_DL_N_DOORLOCK_PREFAULT */
	DL_faultStatus.warningTimeout = PM_SFT_CK_DL_GetParam(DL_FLT_PrefaultTime, safetyData);


	/* Prefault time in sec, timeout prefault, per poi entrare in fault FLT_DL_TOUT_DOORLOCK_FAULT_TO */
	DL_faultStatus.prefaultTimeout = PM_SFT_CK_DL_GetParam(DL_FLT_FaultTime, safetyData);

	DL_faultStatus.substateProtection = SUB_STATE_PROTECTION_OVER_IDLE;
	DL_faultStatus.filterTime = PM_SFT_CK_DL_GetParam(DL_SFT_TimeTempFilter, safetyData);

	DL_faultStatus.rawLockTemperature   = PM_SFT_CK_DL_GetParam(DL_SFT_RawLockTemp,   safetyData);
	DL_faultStatus.rawUnlockTemperature = PM_SFT_CK_DL_GetParam(DL_SFT_RawUnlockTemp, safetyData);

	if (err) {
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_DOORLOCK_BAD_SETTING;
	}
	DL_faultStatus.lastStatus = *safetyData;

	SAFETY_DATA_UPDATE( DL_faultStatus );
	SAFETY_REGION_END();
}

SafetyDoorLockEvent_t generateEvent(void)
{
	uint32_t consistencyValue;
	SafetyDoorLockEvent_t event = SFT_DL_NONE;

	consistencyValue = PM_DoorLock_InternalConsistency();

	if (DL_faultStatus.lastConsistencyValue != consistencyValue) { /* change status */
		if(consistencyValue != DOORLOCK_NOTAVAILABLE){
		   if (	(consistencyValue > DOORLOCK_OPENDOOR)
				&& (consistencyValue < DOORLOCK_WARNING_DOOR)
		   ) {
			   event = SFT_DL_ERROR_ENTER;
		   } else {
			   event = SFT_DL_ERROR_EXIT;
		   }
		}
	} else {
		if (DL_faultStatus.cnt > 0) {
			DL_faultStatus.cnt--;
			if (0 == DL_faultStatus.cnt) {
				event = SFT_DL_TIMEOUT;
			}
		}
	}
	if(consistencyValue != DOORLOCK_NOTAVAILABLE){
		DL_faultStatus.lastConsistencyValue    = consistencyValue;
	}

	return event;
}

void PM_SFT_CK_Doorlock_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{


	SafetyDoorLockEvent_t event=SFT_DL_NONE;
	uint8_t  ntcError;
	uint16_t rawTemperature = PM_SFT_CK_PTCProbe_GetRawTemperature( &ntcError );

	LIB_SFT_EnterSafetyModule(safetyData);

	/* se non è configurato il il DoorLock, il modulo non controlla nulla */
	if(    (LOAD_NOT_FOUND == DL_faultStatus.loadPos)
		|| (FAULTSRC_DOORLOCK_BAD_SETTING == safetyData->faultSrc))
	{
		LIB_SFT_LeaveSafetyModule(safetyData);
		return;
	}

	if (ntcError) {
			/* sonde in errore, scablate/in corto ...
			 * non attivo la protection della reta virtuale
			 * ma blocco la porta
			 */
			rawTemperature = 0;
			newLoads->loads_ovr[DL_faultStatus.loadPos] = LOAD_REG_ON;
	}

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(DL_faultStatus);

	event = generateEvent();

	/* protection -----------------------------------------*/


	/** gestione filtro temperatura per la richiesta di sblocco porta */
	if (rawTemperature > DL_faultStatus.rawLockTemperature) {
		/* SFT_DL_OVER_TEMP */
		DL_faultStatus.filterTimeCnt = DL_faultStatus.filterTime;
		DL_faultStatus.substateProtection = SUB_STATE_PROTECTION_OVER_TEMP;
	} else {
		if (DL_faultStatus.filterTimeCnt > 0) {
				DL_faultStatus.filterTimeCnt--;
				/* SFT_DL_DOWN_TEMP */
				DL_faultStatus.substateProtection = SUB_STATE_PROTECTION_FILTERING;
		}else{
			if (rawTemperature < DL_faultStatus.rawUnlockTemperature)
			{
				/* END_FILTERING */
				DL_faultStatus.substateProtection = SUB_STATE_PROTECTION_OVER_IDLE;
			}
		}
	}

	/*-----------------------------------------------------*/

	switch(DL_faultStatus.lastStatus.status){
	case SFT_MODULE_IDLE:
		if( SFT_DL_ERROR_ENTER == event ){
			DL_faultStatus.lastStatus.status = SFT_MODULE_WARNING;

			if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ){ /** seltest */
				setTimeout(SFT_TEST_PREFAULT_TIMEOUT_100MS);
			}else{
				setTimeout(DL_faultStatus.warningTimeout);
			}
		}
		break;
	case SFT_MODULE_WARNING:
		if( SFT_DL_ERROR_EXIT == event ){
				ResetFaultStatus(&DL_faultStatus.lastStatus);
		} else if( SFT_DL_TIMEOUT == event  ){
			DL_faultStatus.lastStatus.status = SFT_MODULE_PREFAULT;

			if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ){ /** seltest */
				setTimeout(SFT_TEST_FAULT_TIMEOUT_100MS);
			}else{
				setTimeout(DL_faultStatus.prefaultTimeout);
			}
		}
	break;
	case SFT_MODULE_PREFAULT:
		if (SFT_DL_TIMEOUT == event && DL_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_NO)
		{
			DL_faultStatus.lastStatus.status = SFT_MODULE_FAULT;
			EnterFaultState();
		}
		else if (SFT_DL_ERROR_EXIT == event)
		{
			DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_YES;
			DL_faultStatus.cntResetPrefault = FLT_RESET_TIMEOUT_VALUE;
		}
		else if (SFT_DL_ERROR_ENTER == event)
		{
			if (DL_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_TO)
			{ /* rientra in condizine di errore ed il timeout del prefault è gia scaduto */
				EnterFaultState();
			}
			DL_faultStatus.cntResetPrefault = 0;
			DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
		}

		if (DL_faultStatus.waitPrefaultReset)
		{
			if (DL_faultStatus.cntResetPrefault > 0)
			{
				DL_faultStatus.cntResetPrefault--;
			}
			else
			{
				ResetFaultStatus(&DL_faultStatus.lastStatus);
			}
		}
	break;
	case SFT_MODULE_FAULT:
		/* stato senza uscita fino a richiesta esterna */
		if (applReqFlags & SFT_FLAGS_RESET_REQ)
		{
			DL_faultStatus.lastStatus.status = SFT_MODULE_RESET;
		}else if( DL_faultStatus.lastStatus.faultSrc == FAULTSRC_DOORLOCK_CANNOT_OPEN){

			/* gestione ripristino fault F7.02
			 * 1) osservo che lo stato sia coerente per T1 cnt
			 * 2) se lo stato permane ok dopo CONFIG_RESET_FAULT_TIMEOUT allora torno in idle
			 * 3) se permane in fault per CONFIG_SAFETY_RETRY_UNLOCK_TIMEOUT cnt faccio un tentativo di sblocco
			 * 4) ripeto il passo 3 per CONFIG_SAFETY_MAX_RETRY_UNLOCK volte, poi non ritento piu
			 */

			switch(DL_faultStatus.substateFault){
			case FST_WAIT_RETRY_UNLOCK:
				if( SFT_DL_ERROR_EXIT == event ){ /* passa nello stato di attesa timeout per il reset del fault */
					DL_faultStatus.substateFault = FST_WAIT_FAULT_RESET;
					setTimeout(CONFIG_RESET_FAULT_TIMEOUT);
				}else if (SFT_DL_TIMEOUT == event){
					if(DL_faultStatus.retryCounter < CONFIG_SAFETY_MAX_RETRY_UNLOCK )
					{
						/* Retry UNLOCK*/
						PM_DoorLock_SetPosition(BPGEN_CLOSE);
						PM_DoorLock_SetPosition(BPGEN_OPEN);

						DL_faultStatus.retryCounter++;
						setTimeout(CONFIG_SAFETY_RETRY_UNLOCK_TIMEOUT);
					}
				}
			break;
			case FST_WAIT_FAULT_RESET:
				if (SFT_DL_TIMEOUT == event){
					ResetFaultStatus(&DL_faultStatus.lastStatus);
				}else if( SFT_DL_ERROR_ENTER == event ){

					DL_faultStatus.substateFault = FST_WAIT_RETRY_UNLOCK;
					setTimeout(CONFIG_SAFETY_RETRY_UNLOCK_TIMEOUT);
					DL_faultStatus.retryCounter  = 0;
				}
			break;
			case FST_FAULT_UNRECOVERABLE:
				break;
			}
		}
	break;
	case SFT_MODULE_RESET:
	case SFT_MODULE_MONITORING:
	default:
		ResetFaultStatus(&DL_faultStatus.lastStatus);
	break;
	}

	if(DL_faultStatus.lastStatus.status == SFT_MODULE_IDLE){
		DL_faultStatus.lastStatus.faultSrc = 0;
	}else if(DL_faultStatus.lastStatus.status != SFT_MODULE_FAULT){ /* non cambia fault source quando è in fault */
		/* get fault source */

		if(DL_faultStatus.lastConsistencyValue == DOORLOCK_FAULT_FB){
			DL_faultStatus.lastStatus.faultSrc = FAULTSRC_DOORLOCK_FEEDBACK_KO;
		}else if(LIB_IOM_GetLoadStatus(DL_faultStatus.loadPos) == LOAD_REG_OFF){
			DL_faultStatus.lastStatus.faultSrc = FAULTSRC_DOORLOCK_CANNOT_OPEN;
		}else{
			DL_faultStatus.lastStatus.faultSrc = FAULTSRC_DOORLOCK_CANNOT_CLOSE;
		}
	}

	safetyData->faultSrc = DL_faultStatus.lastStatus.faultSrc;
	safetyData->status = DL_faultStatus.lastStatus.status;

	/* La protection ha la priorità! da definire */
	if ( (safetyData->status < SFT_MODULE_WARNING )) /* se non è in fault */
	{
		if(DL_faultStatus.substateProtection != SUB_STATE_PROTECTION_OVER_IDLE)
		{
			safetyData->status = SFT_MODULE_PROTECTION;
			if (SUB_STATE_PROTECTION_OVER_TEMP == DL_faultStatus.substateProtection) {
				safetyData->faultSrc = FAULTSRC_DOORLOCK_OVER_TEMP;
			} else {
				safetyData->faultSrc = FAULTSRC_DOORLOCK_TEMP_MON;
			}
		}
	}

	if(DL_faultStatus.substateProtection != SUB_STATE_PROTECTION_OVER_IDLE){

		newLoads->loads_ovr[DL_faultStatus.loadPos] = LOAD_REG_ON;

		if(DL_faultStatus.substateProtection == SUB_STATE_PROTECTION_FILTERING){
			if(rawTemperature < DL_faultStatus.rawUnlockTemperature){
				PM_SFT_CK_HeatersOff(newLoads,0);
			}
		}

#if defined(CONFIG_PM_DOORLOCK_PULSE_ENABLE)
		if(PM_DoorLock_GetDoorLockType() == BLOCCO_PORTA_IMPULSI){
			PM_DoorLock_Pulse_DisableRetry(TRUE);
		}
#endif
	}
#if defined(CONFIG_PM_DOORLOCK_PULSE_ENABLE)
	else{
		if(PM_DoorLock_GetDoorLockType() == BLOCCO_PORTA_IMPULSI){
				PM_DoorLock_Pulse_DisableRetry(FALSE);
		}
	}
#endif


	TEST_PM_SFT_CK_MONITOR(0)

	SAFETY_DATA_UPDATE( DL_faultStatus );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);
}

uint16_t PM_SFT_CK_Doorlock_GetTime (sft_module_status_t *safetyData)
{
	uint16_t timerValue;
	if(safetyData->status == SFT_MODULE_PROTECTION){
		timerValue = DL_faultStatus.filterTimeCnt;
	}else{
		timerValue = DL_faultStatus.cnt;
	}
	return timerValue;
}

void EnterFaultState(void){
	DL_faultStatus.lastStatus.status = SFT_MODULE_FAULT;
	/* decide in che sottostato andare */
	if(FAULTSRC_DOORLOCK_CANNOT_OPEN == DL_faultStatus.lastStatus.faultSrc ){ /* ripristinabile */
		DL_faultStatus.substateFault = FST_WAIT_RETRY_UNLOCK;
		DL_faultStatus.retryCounter  = 0;
		setTimeout(CONFIG_SAFETY_RETRY_UNLOCK_TIMEOUT);
	}
}
