/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\n
\par		PM_Safety_WM_DL
\n
\n
\file		PM_SFT_WM_Doorlock.c
\ingroup	PM_SFT_DoorLock
\date		13/02/2013
\author		Marco Sperandini
\n


F06
	0x01	Blocco porta non si chiude
			triac blocco porta PTC aperto
			segnale frequenza di rete guasto
			segnale tensione di rete guasto

	0x02	Blocco porta non si apre
			triac blocco porta IMP in corto
			triac blocco porta IMP aperto
*/




/* ******** */
/* Includes */
/* ******** */


#include "PM_SFT_WM_Doorlock.h"
#include "PM_SFT_WM_Common.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "PM_DoorLockExp.h"
#include "PM_SFT_WM_Motor.h"
#include "LIB_Utilities.h"

#include "LIB_ClassBVar.h"

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum {
	SFT_DL_NONE,
	SFT_DL_ERROR_ENTER,
	SFT_DL_ERROR_EXIT,
	SFT_DL_TIMEOUT,
} SafetyDoorLockEvent_t;
typedef struct {
	uint8_t  loadPos;
	uint8_t  lastConsistencyValue;
	uint8_t  lastLoadStatus;
	uint16_t cnt;
	uint16_t warningTimeout;   /* tempo massimo di permanenza in warning */
	uint16_t prefaultTimeout;	/* tempo massimo di permanenza in prefault */
	uint16_t cntResetPrefault;
	uint8_t  waitPrefaultReset;
	uint8_t  filterTime;
} DoorLockFaultStatus_t;

/* ********* */
/* Variables */
/* ********* */
static DoorLockFaultStatus_t DL_faultStatus = { LOAD_NOT_FOUND };

#define SUB_STATE_WAIT_PF_RESET_NO		0
#define SUB_STATE_WAIT_PF_RESET_YES		1
#define SUB_STATE_WAIT_PF_RESET_TO		2
/* *************** */
/* Function Bodies */
/* *************** */
static void ResetFaultStatus(sft_module_status_t *safetyData){
	DL_faultStatus.lastConsistencyValue = PM_CS_NOT_VALID_CODE;
	DL_faultStatus.cnt = 0;
	safetyData->status = SFT_MODULE_IDLE;
	DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
	PM_DoorLock_ResetForceUnlockDoor();
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


/* ------------------------------------------------------------*/
static void PM_SFT_WM_DoorLock_SafetyCheck(iom_loads_struct_t *newLoads, sft_module_status_t *safetyData)
{

	if (
			(PM_DoorLock_GetLockStatusFiltered() == BPGEN_FEED_CLOSE ) &&
			(newLoads->loads_req[DL_faultStatus.loadPos] == LOAD_REG_OFF) &&
			(PM_SFT_WM_DrumSpeedIsZero() == FALSE)
	)
	{
		/* L'Applicazione richiede l'apertura ma il motore non è
		 * ancora stabilmente fermo */
		newLoads->loads_ovr[DL_faultStatus.loadPos] = LOAD_REG_ON;
	}

}

void PM_SFT_WM_Doorlock_Init( sft_module_status_t *safetyData )
{  /* initialize safety module */
	Loader_Struct fltFaultDoorLockData;


	SAFETY_REGION_START();

	ResetFaultStatus(safetyData);

	DL_faultStatus.loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

	if( LOAD_NOT_FOUND == DL_faultStatus.loadPos ){
		safetyData->status   = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_DOORLOCK_BAD_SETTING;
	}

	if( BasicLoader( SF_PTR_FAULT, SF_DISPL_FAULT_DOORLOCK, &fltFaultDoorLockData) )
	{
		/* ERRORE */
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_DOORLOCK_BAD_SETTING;
	}

	/* Prefault time x100ms, timeout warning, per poi entrare in prefault, FLT_DL_N_DOORLOCK_PREFAULT */
	DL_faultStatus.warningTimeout = EndianU16_BToN((*((uint16_t*)((uint8_t*)fltFaultDoorLockData.Data + DL_FLT_PRAM_PrefaultTimeOffset))));

	/* Prefault time in sec, timeout prefault, per poi entrare in fault
	 * FLT_DL_TOUT_DOORLOCK_FAULT_TO		*/
	DL_faultStatus.prefaultTimeout = *(uint8_t*)((uint8_t*)fltFaultDoorLockData.Data+DL_FLT_PRAM_FaultTimeOffset) * 10;

	SAFETY_DATA_UPDATE( DL_faultStatus );
	SAFETY_REGION_END();
}


/**
\nThis function implements the state machine which manages safety of the Door Lock
\n module.
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] safetyData		Info related to the safety status of this module
\param [in] 	applReqFlags	Flags of requests by the application

\return void
\retval	N.A.

\par Architecture Requirements ID:
\li
\n\li

*/
void PM_SFT_WM_Doorlock_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	uint8_t consistencyValue;
	SafetyDoorLockEvent_t event=SFT_DL_NONE;

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(DL_faultStatus);

	bool_t zc = goodZCSignal();
	uint8_t loadStatus = LIB_IOM_GetLoadStatus(DL_faultStatus.loadPos);
	consistencyValue = PM_DoorLock_InternalConsistency();

	/*----------------- */
	PM_SFT_WM_DoorLock_SafetyCheck(newLoads, safetyData);

	if( DL_faultStatus.lastConsistencyValue != consistencyValue
			&& DOORLOCK_NOTAVAILABLE != consistencyValue
		)
	{	/* change status */
		if (consistencyValue != DOORLOCK_LOCKED_FAIL_ZC)
		{
			if((consistencyValue > DOORLOCK_OPENDOOR)
				&& (consistencyValue < DOORLOCK_WARNING_DOOR) )
			{
				event = SFT_DL_ERROR_ENTER;
			}
			else
			{
				event = SFT_DL_ERROR_EXIT;
			}
		}

	}else{
		if( DL_faultStatus.cnt > 0){
			DL_faultStatus.cnt--;
			if( 0 == DL_faultStatus.cnt ){
				event = SFT_DL_TIMEOUT;
			}
		}
	}

	switch(safetyData->status){
	case SFT_MODULE_IDLE:
		if (SFT_DL_ERROR_ENTER == event) {
			safetyData->status = SFT_MODULE_PROTECTION;
		}
	break;
	case SFT_MODULE_PROTECTION:
		if (SFT_DL_ERROR_EXIT == event) {
			ResetFaultStatus(safetyData);
		} else {

			if ( (zc || LOAD_REG_ON == loadStatus)
					&& (PM_DoorLock_GetStatus() != BPGEN_DEVICE_RUNNING)
			) {
				/* enter warning */
				uint16_t timeout;

				safetyData->status = SFT_MODULE_WARNING;
				if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
				{ /** seltest */
					if( DL_faultStatus.warningTimeout < SFT_TEST_PREFAULT_TIMEOUT_100MS ){
						timeout = DL_faultStatus.warningTimeout;
					}else{
						timeout = SFT_TEST_PREFAULT_TIMEOUT_100MS;
					}
					setTimeout(timeout);
				} else {
#if defined(CONFIG_PM_DOORLOCK_WAX_ENABLE)
					uint8_t dlType=PM_DoorLock_GetDoorLockType();

					if( ((dlType == BLOCCO_PORTA_CERA) || (dlType == BLOCCO_PORTA_CERA_2PIN))
							&& (LOAD_REG_OFF == loadStatus)
							&& (PM_DoorLock_GetStatus() != BPGEN_DEVICE_STOPPED_FOR_ERROR)
							)
					{
						/* se warning in unlock e situazione stabile */
						timeout = PM_DoorLock_Wax_GetPrefaultTimeout()/5;
					}else
#endif
					{
						timeout = DL_faultStatus.warningTimeout;
					}
					setTimeout(timeout);
				}
			}
		}
		break;
	case SFT_MODULE_WARNING:
		if( SFT_DL_TIMEOUT == event){
			uint16_t timeout=DL_faultStatus.prefaultTimeout;

			safetyData->status = SFT_MODULE_PREFAULT;

			if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ){ /** seltest */
				if(SFT_TEST_FAULT_TIMEOUT_100MS < DL_faultStatus.prefaultTimeout){
					timeout =SFT_TEST_FAULT_TIMEOUT_100MS;
				}
			}

			setTimeout(timeout);

		}else if (SFT_DL_ERROR_EXIT == event){
			ResetFaultStatus(safetyData);
		} else {
			if ( (!zc && (DOORLOCK_FAULT_FB == consistencyValue))
				||  (PM_DoorLock_GetStatus() == BPGEN_DEVICE_RUNNING))
			{
				safetyData->status = SFT_MODULE_PROTECTION;
			}
		}
	break;
	case SFT_MODULE_PREFAULT:
		if( SFT_DL_TIMEOUT == event )
		{
			if( DL_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_NO){
				safetyData->status = SFT_MODULE_FAULT;
			}else{
				DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_TO;
			}
		}
		else if (SFT_DL_ERROR_EXIT == event)
		{
			DL_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_YES;
			DL_faultStatus.cntResetPrefault  = GEN_FLT_RESET_TIMER_VALUE;
		}
		else if (SFT_DL_ERROR_ENTER == event)
		{
			if (DL_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_TO)
			{ /* rientra in condizine di errore ed il timeout del prefault è gia scaduto */
				safetyData->status = SFT_MODULE_FAULT;
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
				ResetFaultStatus(safetyData);
			}
		}

		if ( (safetyData->faultSrc == FAULTSRC_DOORLOCK_CANNOT_CLOSE) && (LOAD_REG_OFF == loadStatus)) {
			safetyData->status = SFT_MODULE_PROTECTION;
			resetTimeout();
		}
	break;
	case SFT_MODULE_FAULT:
		/* stato senza uscita fino a richiesta esterna */
		if (applReqFlags & SFT_FLAGS_RESET_REQ)
		{
			safetyData->status = SFT_MODULE_RESET;
			/* Restart unlock door */
			PM_DoorLock_SetPosition(BPGEN_CLOSE);
			PM_DoorLock_SetPosition(BPGEN_OPEN);
		}
	break;
	case SFT_MODULE_RESET:
	case SFT_MODULE_MONITORING:
	default:
		ResetFaultStatus(safetyData);
	break;
	}
	if(safetyData->status == SFT_MODULE_IDLE)
	{
		safetyData->faultSrc = 0;
	}
	else if(safetyData->status < SFT_MODULE_PREFAULT)
	{
		/* get fault source */
		if(LIB_IOM_GetLoadStatus(DL_faultStatus.loadPos) == LOAD_REG_OFF){
			safetyData->faultSrc = FAULTSRC_DOORLOCK_CANNOT_OPEN;
		}else{
			safetyData->faultSrc = FAULTSRC_DOORLOCK_CANNOT_CLOSE;
		}
	}
	else
	{
		/* Fault source is kept same in FAULT status */
	}

	if(DOORLOCK_NOTAVAILABLE != consistencyValue)
	{
		DL_faultStatus.lastConsistencyValue    = consistencyValue;
	}
	DL_faultStatus.lastLoadStatus = newLoads->loads_req[DL_faultStatus.loadPos] ;

	SAFETY_DATA_UPDATE( DL_faultStatus );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);
}

uint16_t PM_SFT_WM_Doorlock_GetTime(sft_module_status_t *status)
{
	return DL_faultStatus.cnt;
}
