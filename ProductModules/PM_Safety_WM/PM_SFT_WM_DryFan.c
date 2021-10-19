/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_PM_DryFan.c
\n
\n
\file		PM_SFT_WM_DryFan.c
\ingroup	PM_SFT_DryFan
\date		02/04/2013
\author		Marco Sperandini
\n
*/

/**
 4.2.1.1.2.2	Fault Motoventilatore
La gestione di questo Fault avviene attraverso la funzione FLT_dryFanFaultManagement() chiamata in FLT_faultManagement() ed è monitorato solo quando la porta è bloccata.

SSRS_WDMT_0008		Il software del sistema ARCADIA 2 deve gestire il “Fault Motoventilatore”
					quando la porta è bloccata (segnale di Zero Crossing attivo)
SSRS_WDMT_0009		Il software del sistema ARCADIA 2 deve verificare che i seguenti due check
 	 	 	 	 	 siano soddisfatti contemporaneamente:
					CHECK1: Controlla che il motoventilatore sia pilotato/non pilotato
					CHECK2: Controllo dell’effettivo pilotaggio/non pilotaggio del motoventilatore attraverso il  FEEDBACK MOTOVENTILATORE
SSRS_WDMT_0010		Se il CHECK 1 descritto in SSRS_WDMT_0009 è verificato mentre il CHECK2 è
					fallito rilevando una frequenza di 50Hz o una condizione di pilotaggio del motoventilatore
					attraverso il FEEDBACK MOTOVENTILATORE,
					il software del sistema ARCADIA 2 deve attendere un periodo temporale pari
					a DryFanTriacCCorDiodeModePrefaultTime (default 5 secondi) e, se tale condizione persiste,
					dichiarare lo stato di PREFAULT
SSRS_WDMT_0011		Dall’istante di dichiarazione dello stato di PREFAULT,
					il software del sistema ARCADIA 2 deve attendere un periodo temporale pari a
					DryFanTriacCCorDiodeModeFaultTime secondi (default 30 secondi)
					e dichiarare lo stato di FAULT
SSRS_WDMT_0012		Se dichiarato lo stato di FAULT motoventilatore,
					il software del sistema ARCADIA 2 deve attivare la procedura di sicurezza
					attraverso le seguenti azioni:
						- Disattivazione carichi
						- Posizionamento relay deviatore verso lato lavaggio
						- Attivazione carico acqua (EVP) per un periodo pari a DrainTimeout
						- Sblocco porta
SSRS_WDMT_0013		Il FAULT del motoventilatore non deve essere ripristinabile
SSRS_WDMT_0014		Qualora il FEEDBACK MOTOVENTILATORE torni a segnalare un valore coerente
					con il pilotaggio, il software del sistema ARCADIA 2 deve resettare la procedura
					di rilevazione del guasto immediatamente,
					se ancora non è stato dichiarato il PREFAULT,
					o dopo 5 secondi, se dichiarato lo stato di PREFAULT


Considero anche il caso in cui il consistency non fornisca una valore.
Fault generati

F01.61	Guasto feedback motoventilatore
F01.64	Triac pilotaggio motoventilatore in modo diodo
F19.01	motoventilatore scablato
		/ triac pilotaggio motoventilatore in corto
		/ switch relè incollato lato resistenza lavaggio (non di pertinenza di questo modulo )
		/ pin feedback motoventilatore in corto con Vcc

 */

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_APPLIANCE_WD)
#include "PM_SFT_WM_DryFan.h"
#include "PM_SFT_WM_Common.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "PM_DoorLockExp.h"
#include "PM_SFT_WM_Motor.h"
#include "Analog.h"
#include "LIB_Utilities.h"

#include "LIB_ClassBVar.h"


/**
 *  SF_DISPL_FAULT_DRYING
 *  Drying Fan Triac ca Prefault time: tempo attivazione prefault per guasto triac  ca su motoventilatore (default 60 sec = 600 * 100 msec).
 *	Drying Fan Triac ca Fault time: tempo attivazione fault per guasto triac ca su motoventilatore (default 240 sec).
 *	Drying Fan Triac CC or diode mode Prefault time: tempo attivazione prefault per guasto triac motoventilatore cc o in modo diodo (default 5 sec = 50 * 100 msec).
 *  Drying Fan Triac CC or diode mode Fault time: tempo attivazione fault per guasto triac motoventilatore cc o in modo diodo (default 55 sec).
 *
 */
#define DF_FLT_PRAM_TRIAC_CA_PF_TimeOffset		16 		/* offset in byte */
#define DF_FLT_PRAM_TRIAC_CA_F_TimeOffset		(16+3)
#define DF_FLT_PRAM_TRIAC_CC_PF_TimeOffset		(16+4)
#define DF_FLT_PRAM_TRIAC_CC_F_TimeOffset		(16+7)

/**
 * Safety Drying  Params

+0000h	Fan_Ton_auto (10 sec.) 1 Byte				| Fan_Toff_auto (0,1 sec.) 1 Byte |
+0001h	Fan_Ton_auto_AutotestCycle (10 sec.) 1 Byte | unused 1 Byte					  |

Fan_Ton_auto              : tempo massimo di attivazione motoventilatore (Default 6)
Fan_Toff_auto             : tempo di disattivazione forzata motoventilatore dopo tempo max di accensione (Default 5)
Fan_Ton_auto_AutotestCycle: tempo massimo di attivazione motoventilatore in autotest (Default 6)
 *
 */
#define DF_SFT_PRAM_TON_Offset				(0)
#define DF_SFT_PRAM_TOFF_Offset				(1)
#define DF_SFT_PRAM_TON_AUTOTEST_Offset		(2)



#define STATE_AUTOOFF_OFF	0
#define STATE_AUTOOFF_ON	1

#define SUB_STATE_WAIT_PF_RESET_NO		0
#define SUB_STATE_WAIT_PF_RESET_YES		1
#define SUB_STATE_WAIT_PF_RESET_TO		2

#define TO_PF_CA			0
#define TO_F_CA				1
#define TO_PF_CC			2
#define TO_F_CC				3
#define TO_AUTOOFF_TON		4		/**< autoOff Ton	*/
#define TO_AUTOOFF_TOFF		5		/**< autooff Toff	*/
#define TO_AUTOOFF_TON_AT	6	/**< AutoOff TOn AutoTest */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum {
	SFT_DF_NONE,
	SFT_DF_ERROR_ENTER,
	SFT_DF_ERROR_EXIT,
	SFT_DF_TIMEOUT,
	SFT_DF_AUTOOFF_TIMEOUT,
} SafetyDryFanEvent_t;

typedef struct {
	uint8_t	 lastLoadReq;
	uint8_t  loadPos;
	uint8_t  drPumpPos;
	uint8_t  lastConsistencyValue;
	uint16_t cnt;
	uint8_t  autoOff_status;
	uint16_t autoOff_cnt;
	uint16_t cntResetPrefault;
	uint8_t  waitPrefaultReset;
} DryFanFaultStatus_t;

/* ********* */
/* Local Functions */
/* ********* */
static void resetTimeout(void);
static void setTimeout(uint16_t value);
static void setAutoOffTimeout(uint16_t value);
static uint16_t getParamTimeout( sft_module_status_t *safetyData, uint8_t param);
static void ResetFaultStatus(sft_module_status_t *safetyData);

/* ********* */
/* Variables */
/* ********* */
static DryFanFaultStatus_t DF_faultStatus = { LOAD_NOT_FOUND };

/* *************** */
/* Function Bodies */
/* *************** */

static void resetTimeout(void){
	DF_faultStatus.cnt = 0;
}

 static void setTimeout(uint16_t value){
	 if(value == 0){
		DF_faultStatus.cnt = 1;
	 }else{
		DF_faultStatus.cnt = value;
	 }
}
 static void setAutoOffTimeout(uint16_t value){
	DF_faultStatus.autoOff_cnt = value;
}

 static void ResetFaultStatus(sft_module_status_t *safetyData){
 	DF_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
 	DF_faultStatus.autoOff_status = STATE_AUTOOFF_ON;
 	setAutoOffTimeout(getParamTimeout(safetyData, TO_AUTOOFF_TON));
 	DF_faultStatus.lastConsistencyValue = PM_CS_NOT_VALID_CODE;
 	safetyData->status = SFT_MODULE_IDLE;
 	resetTimeout();
 }

static uint16_t getParamTimeout(sft_module_status_t *safetyData, uint8_t param)
{
	uint16_t retVal = 0;

	Loader_Struct data;

	if (param < TO_AUTOOFF_TON)
	{
		if (BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &data))
		{
			/* ERRORE */
			safetyData->status = SFT_MODULE_FAULT;
			safetyData->faultSrc = FAULTSRC_DRYFAN_BAD_SETTING;
		}
		else
		{
			switch (param)
			{
			case TO_PF_CA:/* Prefault time x100ms, timeout warning, per poi entrare in prefault,  */
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + DF_FLT_PRAM_TRIAC_CA_PF_TimeOffset))));
				break;
			case TO_F_CA:/* Prefault time in sec, timeout prefault, per poi entrare in fault */
				retVal = *(uint8_t*) ((uint8_t*) data.Data + DF_FLT_PRAM_TRIAC_CA_F_TimeOffset) * 10;
				break;
			case TO_PF_CC:
				retVal = EndianU16_BToN((*((uint16_t*)((uint8_t*)data.Data + DF_FLT_PRAM_TRIAC_CC_PF_TimeOffset))));
				break;
			case TO_F_CC:
				retVal = *(uint8_t*) ((uint8_t*) data.Data + DF_FLT_PRAM_TRIAC_CC_F_TimeOffset) * 10;
				break;
			}
		}
	}
	else
	{
		if (BasicLoader(SF_PTR_SAFETY_PARAMETERS, SF_DISPL_SFT_DRYING, &data))
		{
			/* ERRORE */
			safetyData->status = SFT_MODULE_FAULT;
			safetyData->faultSrc = FAULTSRC_DRYFAN_BAD_SETTING;
		}
		else
		{
			switch (param)
			{
			case TO_AUTOOFF_TON:
				retVal = *((uint8_t*)data.Data + DF_SFT_PRAM_TON_Offset); /**< x 10sec */
				retVal *= 100;
				break;
			case TO_AUTOOFF_TOFF:
				retVal = *((uint8_t*)data.Data + DF_SFT_PRAM_TOFF_Offset);/**< x 100 ms */
				break;
			case TO_AUTOOFF_TON_AT:
				retVal = *((uint8_t*)data.Data + DF_SFT_PRAM_TON_AUTOTEST_Offset); /**< x 10sec */
				retVal *= 100;
				break;
			}
		}
	}
	return retVal;
}

/* ------------------------------------------------------------*/


void PM_SFT_WM_DryFan_Init( sft_module_status_t *safetyData )
{  /* initialize safety module */

	SAFETY_REGION_START();

	ResetFaultStatus(safetyData);
	DF_faultStatus.loadPos = LIB_IOM_GetLoadPos(LD_DRY_FAN_POS);
	if( LOAD_NOT_FOUND == DF_faultStatus.loadPos ){
		safetyData->status = SFT_MODULE_IDLE;
	}
	else
	{
		uint8_t tmpDPPos = LIB_IOM_GetLoadPos(LD_DRAIN_PUMP_POS);

		/* Assuming Drain pump configured as a coupled load into a switch relay
		 * means it's related to EVP
		 */
		if ( (LIB_IOM_FindLoadOnSwitchRelay(tmpDPPos) & (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE))
			!= (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE))
		{
			tmpDPPos = LOAD_NOT_FOUND;
		}

		DF_faultStatus.drPumpPos = tmpDPPos;

	}

	PLT_ASSERT(getParamTimeout(safetyData, TO_PF_CA));
	PLT_ASSERT(getParamTimeout(safetyData, TO_F_CA));
	PLT_ASSERT(getParamTimeout(safetyData, TO_PF_CC));
	PLT_ASSERT(getParamTimeout(safetyData, TO_F_CC));

	PLT_ASSERT(getParamTimeout(safetyData, TO_AUTOOFF_TON));
	PLT_ASSERT(getParamTimeout(safetyData, TO_AUTOOFF_TOFF));
	PLT_ASSERT(getParamTimeout(safetyData, TO_AUTOOFF_TON_AT));

	PLT_ASSERT(safetyData->faultSrc == 0);

	SAFETY_DATA_UPDATE( DF_faultStatus );
	SAFETY_REGION_END();
}

#define SFT_DF_CONSISTENCY_TIMEOUT (1200)

void PM_SFT_WM_DryFan_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	uint8_t consistencyValue = PM_CS_NOT_VALID_CODE;
	SafetyDryFanEvent_t event=SFT_DF_NONE;

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(DF_faultStatus);

	if( LOAD_NOT_FOUND != DF_faultStatus.loadPos )
	{
		(void) PM_Consistency_IsValueReady(DF_faultStatus.loadPos, &consistencyValue);

		if (goodZCSignal() == TRUE)
		{ /** solo se c'è segnale di ZC */

			if (consistencyValue != PM_CS_NOT_VALID_CODE)
			{
				uint8_t drPumpCons;

				if ((PM_CS_NO_ERROR == PM_Consistency_IsValueReady(DF_faultStatus.drPumpPos, &drPumpCons)))
				{
					if ( (drPumpCons == PM_CS_VALUE_ERR_SC) &&
						((consistencyValue & PM_CS_STATUS_MASK) == 0) )
					{
						/* Drain pump consistency has to be checked if it's due to coupled
						 * load (Prewash EV) ON; only if so cousistency OK and SC can be swapped
						 */
						if (LIB_IOM_GetLoadStatus(LIB_IOM_GetLoadPos(LD_PRE_WASH_EV_POS)) != LOAD_REG_OFF)
						{
							if (consistencyValue == DRYFAN_OK)
							{
								consistencyValue = PM_CS_VALUE_ERR_SC;
							}
							else if (consistencyValue == PM_CS_VALUE_ERR_SC)
							{
								consistencyValue = DRYFAN_OK;
							}
							else
							{
								/* All other cases are OK */
							}
						}
					}
				}

				/* Filter consistency value only - regardless of load status */
				consistencyValue &= PM_CS_VALUE_MASK;

			}

		}
		else
		{
			/* When ZC is missing, force to OK case; if FB is configured and provides a
			 * valid consistency value, force Auto OFF status to be checked when ZC goes back 
			 * If FB is not configured, Auto OFF has to not be set ever! */
			if (consistencyValue != PM_CS_NOT_VALID_CODE)
			{
				/* Riparte la gestione da OFF */
				setAutoOffTimeout(getParamTimeout(safetyData, TO_AUTOOFF_TOFF));
				DF_faultStatus.autoOff_status = STATE_AUTOOFF_OFF;
			}
			consistencyValue = DRYFAN_OK;

		}

		if (consistencyValue != PM_CS_NOT_VALID_CODE)
		{
			if( DF_faultStatus.lastConsistencyValue != consistencyValue )
			{	/* change status */
				/** lo stato puo cambiare perche':
				 * - cambia il pilotaggio ON/OFF ( non rilevante )
				 * - cambia il valore del consistency
				 * - cambia il pilotaggio del bloccoporta o dello ZC ( non rilevante perche il controllo  non e' attivo )
				 */
				if(	consistencyValue != DRYFAN_OK )
				{
					event = SFT_DF_ERROR_ENTER;
				}
				else
				{
					event = SFT_DF_ERROR_EXIT;
				}
			}
			else
			{
				if ( (DF_faultStatus.autoOff_cnt > 0)
					&& (safetyData->status < SFT_MODULE_WARNING))
				{
					DF_faultStatus.autoOff_cnt--;
					if (0 == DF_faultStatus.autoOff_cnt)
					{
						event = SFT_DF_AUTOOFF_TIMEOUT;
					}
				}
				if( DF_faultStatus.cnt > 0){
					DF_faultStatus.cnt--;
					if( 0 == DF_faultStatus.cnt ){
						event = SFT_DF_TIMEOUT;
					}
				}
			}
		}

		/** AutoOff management -------------------------------------*/
		if (newLoads->loads_req[DF_faultStatus.loadPos] == LOAD_OFF)
		{
			/* Riparte la gestione di auto OFF quando la richiesta carico è OFF */
			setAutoOffTimeout(getParamTimeout(safetyData, TO_AUTOOFF_TON));
			DF_faultStatus.autoOff_status = STATE_AUTOOFF_ON;

		}
		else
		{ /* se la richiesta è di carico acceso */
			if(safetyData->status ==  SFT_MODULE_IDLE)
			{ /* se non sono in allerta */
				uint16_t timeout;
				if(SFT_DF_AUTOOFF_TIMEOUT == event){
					if( DF_faultStatus.autoOff_status == STATE_AUTOOFF_ON){
					/* se scade il timeout di T_On spegne */
						DF_faultStatus.autoOff_status = STATE_AUTOOFF_OFF;/**< cambia stato */
						timeout = getParamTimeout(safetyData, TO_AUTOOFF_TOFF ); /**< legge il valore del timepout */
						setAutoOffTimeout(timeout);	/**< imposta il timeout */
					}
					else
					{ /* STATE_AUTOOFF_OFF */
					/* se scade il timeout di T_Off accende */
						DF_faultStatus.autoOff_status = STATE_AUTOOFF_ON; /**< cambia stato */
						timeout = getParamTimeout(safetyData, TO_AUTOOFF_TON);
						setAutoOffTimeout(timeout);
					}
				}
			}
		}

		/* -------------------------------------------------------------*/


		switch(safetyData->status){
		case SFT_MODULE_IDLE:
			if( SFT_DF_ERROR_ENTER == event || SFT_DF_TIMEOUT == event ){
				safetyData->status = SFT_MODULE_WARNING;
				if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ){ /** seltest */
					setTimeout(SFT_TEST_PREFAULT_TIMEOUT_100MS);
				}else{
					uint16_t timeout;
					if( consistencyValue == PM_CS_VALUE_ERR_OC ){
						timeout = getParamTimeout(safetyData, TO_PF_CA );
					}else{
						timeout = getParamTimeout(safetyData, TO_PF_CC );
					}
					setTimeout(timeout);
				}
			}
			else
			{
				consistencyValue = DRYFAN_OK;
			}

			/* se persiste il consistency a 0xFF, valore non disponibile
			 * allora è considerato un errore.
			 * Resetta continuamente il timer  altrimeti genera un timeout.
			 */
			if((DF_faultStatus.lastConsistencyValue == 0xFF) && (consistencyValue == 0xFF))
			{
				setTimeout(SFT_DF_CONSISTENCY_TIMEOUT);
			}

		break;
		case SFT_MODULE_WARNING:
		case SFT_MODULE_PROTECTION:
			if( SFT_DF_TIMEOUT == event)
			{
				safetyData->status = SFT_MODULE_PREFAULT;
				if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ){ /** seltest */
					setTimeout(SFT_TEST_FAULT_TIMEOUT_100MS);
				}else{
					uint16_t timeout;
					if (consistencyValue == PM_CS_VALUE_ERR_OC)
					{
						timeout = getParamTimeout(safetyData, TO_F_CA);
					}
					else
					{
						timeout = getParamTimeout(safetyData, TO_F_CC);
					}
					setTimeout(timeout);
				}
			}
			else if( SFT_DF_ERROR_EXIT == event)
			{
				ResetFaultStatus(safetyData);
			}
		break;
		case SFT_MODULE_PREFAULT:
			if( SFT_DF_TIMEOUT == event )
			{
				if( DF_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_NO){
					safetyData->status = SFT_MODULE_FAULT;
				}else{
					DF_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_TO;
				}
			}else
			if (SFT_DF_ERROR_EXIT == event)
			{
				DF_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_YES;
				DF_faultStatus.cntResetPrefault = GEN_FLT_RESET_TIMER_VALUE;
			}else
			if (SFT_DF_ERROR_ENTER == event)
			{
				if (DF_faultStatus.waitPrefaultReset == SUB_STATE_WAIT_PF_RESET_TO)
				{ /* rientra in condizine di errore ed il timeout del prefault è gia scaduto */
					safetyData->status = SFT_MODULE_FAULT;
				}
				DF_faultStatus.cntResetPrefault = 0;
				DF_faultStatus.waitPrefaultReset = SUB_STATE_WAIT_PF_RESET_NO;
			}

			if (DF_faultStatus.waitPrefaultReset) {
				if (DF_faultStatus.cntResetPrefault > 0) {
					DF_faultStatus.cntResetPrefault--;
				} else {
					ResetFaultStatus(safetyData);
				}
			}
			break;
		case SFT_MODULE_FAULT:
			/* stato senza uscita fino a richiesta esterna */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				safetyData->status = SFT_MODULE_RESET;
			}
			consistencyValue = safetyData->faultSrc;
		break;
		case SFT_MODULE_RESET:
		case SFT_MODULE_MONITORING:
		default:
			/* MS TODO: va in fault ??? */
			safetyData->status = SFT_MODULE_IDLE;
		break;
		}

		/* Auto OFF */
		if(safetyData->status <= SFT_MODULE_WARNING)
		{
			if( (newLoads->loads_req[DF_faultStatus.loadPos] == LOAD_ON) && (DF_faultStatus.autoOff_status == STATE_AUTOOFF_OFF))
			{
				uint8_t cfgOnSR = LIB_IOM_FindLoadOnSwitchRelay(DF_faultStatus.loadPos) & (LOAD_SR_FOUND | LOAD_SR_POS_ON);

				newLoads->loads_ovr[DF_faultStatus.loadPos] = OFF;

				if (cfgOnSR & LOAD_SR_FOUND)
				{
					newLoads->loads_ovr[LIB_IOM_GetSRLoadPosByRelLdPos(DF_faultStatus.loadPos)] = ((cfgOnSR & LOAD_SR_POS_ON) ? LOAD_ON : LOAD_OFF);
				}
			}
		}

		/* get fault source */

		if ( (consistencyValue != DRYFAN_OK) && ((consistencyValue != PM_CS_NOT_VALID_CODE)))
		{
			/** definire la fault source */
			/** definito come i valori assunti per convenzione dal consistency quindi non rimappo */
			safetyData->faultSrc = consistencyValue;
		}

		DF_faultStatus.lastConsistencyValue = consistencyValue;

		DF_faultStatus.lastLoadReq = newLoads->loads_req[DF_faultStatus.loadPos];

	}

	SAFETY_DATA_UPDATE( DF_faultStatus );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);

}

uint16_t PM_SFT_WM_DryFan_GetTime(sft_module_status_t *status)
{
	uint16_t retVal = 0;

	if (status->status == SFT_MODULE_IDLE)
	{
		retVal = DF_faultStatus.autoOff_cnt;
	}
	else
	{
	 retVal = DF_faultStatus.cnt;
	}

	return retVal;
}

#endif /* #if defined(CONFIG_APPLIANCE_WD) */
