/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_SafetyActions
\n
\n
\file		PM_SFT_WM_SafetyActions.c
\ingroup	PM_SFT_SafetyActions
\date		18/02/2013
\author		Francesco Barletta
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "StandardDefines.h"
#include "LIB_ProductSafety.h"
#include "PM_SFT_WM_SafetyActions.h"
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_WashRes.h"
#include "PM_DoorLockExp.h"
#include "LoadsRequestExp.h"
#include "SwTimersCounters.h"
#include "LIB_UTilities.h"
#include "LIB_ClassBVar.h"

#if defined(CONFIG_PM_MOTORGATE)
#include "PM_UM_UniversalMotorExp.h"
#endif /* defined(CONFIG_PM_MOTORGATE) */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define SAFETY_DRAIN_TOUT(p)		(OFFS_B((uint8_t*)(p), 3))	/* x10 sec, default 30*/






/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */




/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */
static struct {
	uint16_t cnt;
	uint16_t cntSkip;
} tmrDrainPump;

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
static bool_t PM_SFT_WM_DrainPumpKeptON(void);

/* *************** */
/* Function Bodies */
/* *************** */
/**
\n 			Routine di disattivazione relay resistenza in caso di Fault
\param		...
\return 	TRUE, se la routine é terminata con successo;
			FALSE, se é in esecuzione (in questo caso non lo è perchè
			vengono eseguite azioni di sicurezza direttamente, senza
			transitori);
\author 	Francesco Barletta
\date		Feb 21, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_WM_LoadsOff_Res(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t washResPos;
uint8_t retValue = TRUE;
#if defined(CONFIG_NUM_SR_LOADS)
uint8_t  evWashPos, drainPumpPos, srPos, retLoadOnSR;
#endif
#if defined(CONFIG_APPLIANCE_WD)
uint8_t fanPos, dryResPos;
#endif

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

#if defined(CONFIG_NUM_SR_LOADS)
	/* Si ottiene la posizione di pompa scarico */
	drainPumpPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);
	/* Si ottiene la posizione di EV wash */
	evWashPos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_EV_POS);
#endif
	/* Si ottiene la posizione di Wash Res */
	washResPos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_RES_POS);
#if defined(CONFIG_APPLIANCE_WD)
	/* Si ottiene la posizione di Dry FAN */
	fanPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRY_FAN_POS);
	/* Si ottiene la posizione di Dry Res */
	dryResPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRY_RES_POS);
#endif

	if (status == _ENABLE_)
	{
		retValue = TRUE;
	}

	/* Dato che la resistenza lavaggio viene fisicamente spenta si aggiorna in modo coerente
		anche lo stato richiesto per il carico */
	newLoads->loads_ovr[washResPos] = LOAD_REG_OFF;

#if defined(CONFIG_NUM_SR_LOADS)
	/* Si verifica se resistenza è su ramo ON dello Switch Relay */
	retLoadOnSR = LIB_IOM_FindLoadOnSwitchRelay(washResPos);

	/* Dopo aver controllato la posizione della resistenza lavaggio (disattivata) */
	/* si imposta il relay su ramo in cui essa non è presente */
	if (retLoadOnSR & LOAD_SR_FOUND)
	{
		srPos = LIB_IOM_GetSRLoadPosByRelLdPos(washResPos);

		if (retLoadOnSR & LOAD_SR_POS_ON)
		{
			newLoads->loads_ovr[srPos] = LOAD_REG_OFF;
		}
		else
		{
			newLoads->loads_ovr[srPos] = LOAD_REG_ON;
		}

		/* Drain pump ed EVL vengono mantenuti spenti */
		newLoads->loads_ovr[drainPumpPos] = LOAD_REG_OFF;
		newLoads->loads_ovr[evWashPos] = LOAD_REG_OFF;

#if defined(CONFIG_APPLIANCE_WD)

		/* Si verifica se FAN è collegato allo stesso Switch Relay resistenza */
		retLoadOnSR = LIB_IOM_GetSRLoadPosByRelLdPos(fanPos);

		if ((retLoadOnSR == srPos) && (fanPos != LOAD_NOT_FOUND))
		{
			newLoads->loads_ovr[fanPos] = LOAD_REG_OFF;
		}

		/* Si verifica se Dry Res è è collegato allo stesso Switch Relay resistenza */
		retLoadOnSR = LIB_IOM_GetSRLoadPosByRelLdPos(dryResPos);
		if ((retLoadOnSR == srPos) && (dryResPos != LOAD_NOT_FOUND))
		{
			newLoads->loads_ovr[dryResPos] = LOAD_REG_OFF;
		}

#endif

	}
	else
	{
		/* ARCADIA 1, retrocompatibilità  */
		newLoads->loads_ovr[washResPos] = LOAD_CFG_RELAY_ONLY;
	}
#endif

#endif

	return retValue;

}

/**
\n This function switches the switch relay off
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] status			TODO



\return Result of the operation
\retval	TRUE


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
uint8_t PM_SFT_WM_SwitchRelayOff(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t retValue = TRUE;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) && defined(CONFIG_APPLIANCE_WD)
uint8_t loadPos;
uint8_t retLoadOnSR;

	loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRY_FAN_POS);

	if (loadPos != LOAD_NOT_FOUND)
	{

		newLoads->loads_ovr[loadPos] = LOAD_REG_ON;

		/* Si verifica se motoventilatore è su ramo ON dello Switch Relay */
		retLoadOnSR = LIB_IOM_FindLoadOnSwitchRelay(loadPos);

		/* Dopo aver controllato la posizione del carico in oggetto su ramo ON dello
		 * Switch Relay, si imposta il relay su ramo in cui esso non è presente, ovvero
		 * lato Caricamento/Lavaggio */
		if (retLoadOnSR & LOAD_SR_FOUND)
		{
			uint8_t srPos = LIB_IOM_GetSRLoadPosByRelLdPos(loadPos);

			if (retLoadOnSR & LOAD_SR_POS_ON)
			{
				newLoads->loads_ovr[srPos] = LOAD_REG_OFF;
			}
			else
			{
				newLoads->loads_ovr[srPos] = LOAD_REG_ON;
			}
		}
		else
		{
			/* ARCADIA 1, retrocompatibilità */
			newLoads->loads_ovr[loadPos] |= LOAD_CFG_TRIAC_ONLY;
		}
	}

#endif

	return retValue;
}

/**
\n TODO
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] status			TODO



\return Result of the operation
\retval	TRUE


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
uint8_t PM_SFT_WM_FanOn(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t retValue = TRUE;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) && defined(CONFIG_APPLIANCE_WD)
uint8_t loadPos;

	loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRY_FAN_POS);

	if (loadPos != LOAD_NOT_FOUND)
	{
		/* Si impone il blocco porta,si ativa motoventilatore, */
		newLoads->loads_ovr[LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS)] = LOAD_REG_ON;

		newLoads->loads_ovr[loadPos] = LOAD_REG_ON;
	}

#endif

	return retValue;

}


/**
\n TODO
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] status			TODO



\return TODO
\retval	TODO


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
uint8_t PM_SFT_WM_LoadsOff(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t retValue = TRUE;
uint8_t i;
uint8_t loadPos;


#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

#ifdef __UNIVERSAL_MOTOR_H
int16_t deltaSpeed;
#endif

/** SPEGNIMENTO CARICHI (ad eccezione del blocco porta e, a seconda dei bit di
	FltSafetyBitmapReq,	della pompa scarico e del motoventilatore) */

	for (i=0; i<LD_MAX_NUMBER; i++)
	{

		/* Nell'ottenere la posizione di un carico, si effettua il controllo */
		/* su eventuale ritorno di "carico non trovato" */
		loadPos = LIB_IOM_GetLoadPos(i);
		if (loadPos < N_MAXLOADS)
		{

			switch(i)
			{
				case LD_DOOR_LOCK_POS:
					if (LIB_SFT_GetFaultSourceStatus(SFT_FLT_DRLCK_CANNOT_CLOSE) != SFT_MODULE_PREFAULT)
					{
						/* Il blocco porta deve rimanere attivo */
						newLoads->loads_ovr[loadPos] = LOAD_REG_ON;
					}
				break;

#ifdef CONFIG_PM_DRUMLOCK
				/*
				Il blocco cesto rimane attivo:
				se avessi un guasto sul motore come triac in CC
				meglio che il blocco cesto rimanga pilotato.
				*/
				case LD_DRUM_LOCK_POS:
#endif
				case LD_LAMP_POS:
#if defined(CONFIG_NUM_SR_LOADS)
				case LD_SWITCH_RELAY_POS:
#endif
					/*
					 *	Swtich relay position will be set anyway - no need to force it
					 *	Lamp: not needed to force it OFF
					*/
				break;

				case LD_DRAIN_PUMP_POS:
					if (!PM_SFT_WM_DrainPumpKeptON())
					{
					newLoads->loads_ovr[loadPos] = LOAD_REG_OFF;
					}


				break;

#if defined(CONFIG_APPLIANCE_WD)
				case LD_DRY_FAN_POS:
				{
					/* Mantiene lo stato richiesto fino a che c'è ZC
					 */
					if(((LIB_IOM_GetLoadStatus(LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS)) & LOAD_REG_MASK) == LOAD_REG_OFF)
						&& !goodZCSignal())
					{
						newLoads->loads_ovr[loadPos] = LOAD_REG_OFF;
					}

				}

				break;
#endif

				default:
					newLoads->loads_ovr[loadPos] = LOAD_REG_OFF;
				break;

			}	/* end switch(i) */
		}	/* end else */
	}	/* end for */

/** SPEGNIMENTO MOTORE */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	if (IS_MOTOR_3PH)
	{
		memset(&newLoads->extdloads_ovr[EXTLD_ENGINE][0], 0, sizeof(newLoads->extdloads_ovr[EXTLD_ENGINE]));
	}
#endif
#if defined(CONFIG_PM_MOTORGATE)
	if (!IS_MOTOR_3PH)
	{
		PM_SFT_WM_AC_MotorProt(newLoads, status);
	}
#endif

#endif /* (CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */

	return retValue;

}

/**
\n Routine generica per evitare l'uso della pompa scarico
\n
\n
\param None




\return Result of the operation
\retval	TRUE,FALSE


\par Architecture Requirements ID:
\li		TODO
\n\li

\author 	Alessandro Sabbatini
\date		Feb 21, 2013
\version	1.0.0

*/
uint8_t PM_SFT_WM_ForceDrainOff(iom_loads_struct_t *newLoads, uint8_t status)
{
	uint8_t drainPumpPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);
#if defined(CONFIG_NUM_SR_LOADS)
	uint8_t cfgDrain = LIB_IOM_FindLoadOnSwitchRelay(drainPumpPos);
#endif
	(void) status;

	newLoads->loads_ovr[drainPumpPos] = LOAD_REG_OFF;

#if defined(CONFIG_NUM_SR_LOADS)
	if (cfgDrain & LOAD_SR_FOUND)
	{
		uint8_t srPos = LIB_IOM_GetSRLoadPosByRelLdPos(drainPumpPos);

		/* Drain pump configured on switch relay: place in other position
		 * to prevent power supply
		 */

		if (cfgDrain & LOAD_SR_POS_ON)
		{
			newLoads->loads_ovr[srPos] = LOAD_REG_OFF;
		}
		else
		{
			newLoads->loads_ovr[srPos] = LOAD_REG_ON;
		}
	}
#endif

	return TRUE;
}


/**
\n Routine generica di scarico in caso di Fault
\n
\n
\param None




\return Result of the operation
\retval	TRUE,FALSE


\par Architecture Requirements ID:
\li		TODO
\n\li

\author 	Francesco Barletta
\date		Feb 21, 2013
\version	1.0.0

*/

uint8_t PM_SFT_WM_Drain(iom_loads_struct_t *newLoads, uint8_t status)
{
	uint8_t retValue = TRUE;
	uint8_t locSafetyDrain_Tout = 3;
	uint8_t locSafetySkip_Tout = 50;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t door_pos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);
	uint8_t drain_pos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);
	uint8_t drain_cons;

	if (status == _ENABLE_)
	{
		Loader_Struct tmpSafetyData;

		if ((drain_pos != LOAD_NOT_FOUND) &&
			( BasicLoader( SF_PTR_SAFETY_PARAMETERS, SF_DISPL_SFT_FAULT_ACTIONS, &tmpSafetyData) == SF_OK ))
		{
			locSafetyDrain_Tout = SAFETY_DRAIN_TOUT(tmpSafetyData.Data);
		}
		else
		{
			locSafetyDrain_Tout = 0;
			locSafetySkip_Tout = 0;
		}
		/* il timer per safety drain nel setting è definito con base tempi 10s */
		tmrDrainPump.cnt = BASETIME_10S_TO_100MS(locSafetyDrain_Tout);
		tmrDrainPump.cntSkip = locSafetySkip_Tout;
		SAFETY_DATA_UPDATE(tmrDrainPump);

	}

	SAFETY_DATA_CHECK(tmrDrainPump);

	if ( PM_CS_NO_ERROR == PM_Consistency_IsValueReady(drain_pos, &drain_cons))
	{
		if ((drain_cons == PM_CS_NOT_VALID_CODE) ||
				((drain_cons & PM_CS_VALUE_MASK) == PM_CS_VALUE_OK))
		{
			tmrDrainPump.cntSkip = locSafetySkip_Tout;
		}
		else if (!tmrDrainPump.cntSkip)
		{
			tmrDrainPump.cnt = 0;
		}
		else if ((LIB_SFT_GetFaultSourceStatus(SFT_FLT_DRAIN_TRIAC_DM_ON) == SFT_MODULE_FAULT) )
		{
			tmrDrainPump.cntSkip--;
		}
		else
		{
			/* do nothing */
		}
	}

	if (tmrDrainPump.cnt > 0)
	{
		/* Si ottiene la posizione di blocco porta */
		/* Si impone il blocco porta */
		newLoads->loads_ovr[door_pos] = LOAD_REG_ON;
			newLoads->loads_ovr[drain_pos] = LOAD_REG_ON;
			tmrDrainPump.cnt--;
			retValue = FALSE;

	}
	else
	{
		/* Check if washing heater is in trouble
		 * in this case keep drain pump ON until ZC is present.
		 */
		if ( PM_SFT_WM_DrainPumpKeptON() )
		{
			newLoads->loads_ovr[door_pos] = LOAD_REG_OFF;
			newLoads->loads_ovr[drain_pos] = LOAD_REG_ON;
		}
	}

	SAFETY_DATA_UPDATE(tmrDrainPump);

#endif

	return retValue;

}


/**
\n TODO
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] status			TODO



\return TODO
\retval	TODO


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
uint8_t PM_SFT_WM_UnlockDoor(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t retValue = TRUE;
uint8_t doorLockPos;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	/* Nell'ottenere la posizione di blocco porta tra i carichi, il controllo */
	/* su eventuale ritorno di "carico non trovato" non si esegue perché già fatto */
	doorLockPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

	/* Se motore risulta in movimento */
	if ( PM_SFT_WM_DrumSpeedIsZero() == FALSE )
	{
		/* Si impone il blocco porta */
		newLoads->loads_ovr[doorLockPos] = LOAD_REG_ON;

		retValue = FALSE;
	}
	/* Se motore risulta fermo (velocità di rotazione nulla) */
	else
	{
		/* Si impone la apertura della porta */
		newLoads->loads_ovr[doorLockPos] = LOAD_REG_OFF;
	}

	if (LIB_SFT_GetFaultSourceStatus(SFT_FLT_DRLCK_CANNOT_CLOSE) == SFT_MODULE_FAULT)
	{
		PM_DoorLock_ForceUnlockDoor();
	}

#endif

	return retValue;

}




/**
\n TODO
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] status			TODO



\return TODO
\retval	TODO


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
uint8_t PM_SFT_WM_LoadsOffTotal(iom_loads_struct_t *pNewLoads, uint8_t status)
{
uint8_t i;
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
uint8_t drain_pos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);

	/** SPEGNIMENTO CARICHI Tranne pompa scarico se in guasto modo diodo
	 * e ZC presente - in questo caso va mantenuta ON */
	for(i=0; i<N_MAXLOADS; i++)
	{
		if ((i == drain_pos) && (PM_SFT_WM_DrainPumpKeptON()))
		{
			pNewLoads->loads_ovr[i] = LOAD_ON;
		}
		else
		{
			pNewLoads->loads_ovr[i] = LOAD_OFF;
		}
	}

	/** PILOTAGGIO MOTORE OFF */
#if ( defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS))
	if (IS_MOTOR_3PH)
	{
		memset(&pNewLoads->extdloads_ovr[EXTLD_ENGINE][0], 0, sizeof(pNewLoads->extdloads_ovr[EXTLD_ENGINE]));
	}
#endif
#if defined(CONFIG_PM_MOTORGATE)
	if (!IS_MOTOR_3PH)
	{
		PM_SFT_WM_AC_MotorProt(pNewLoads, status);
	}
#endif

#endif /* defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */


	/** RETURN */
	return TRUE;

}


#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)


/**
\n safety action to request to force call with external board
\n
\n
\param [in,out] pNewLoads		Not used
\param [in] 	init			Initial step



\return Result of the operation
\retval	TRUE safety action completed,FALSE safety action executing


\par Architecture Requirements ID:
\li
\n\li

\author		Roberto Fioravanti
\date		Mar 14, 2013
\version	1.0.0



*/

uint8_t PM_SFT_WM_RestoreCommWithExternalBoard(iom_loads_struct_t *pNewLoads, uint8_t init)
{
	(void) pNewLoads;
	uint8_t res = FALSE; /* by default return safety action as executing */

	if(init)
	{
		PM_SFT_WM_RestoreCommunicationWithExternalBoard();
		res = FALSE;
	}
	else
	{
		res = PM_SFT_WM_Motor_CommManagerIsIdle();
	}

	return res;
}

/**
\n Safety action to request to suspend temporarily the communication with external board
\n
\n
\param [in,out] pNewLoads		Not used
\param [in] 	init			Initial step



\return Result of the operation
\retval	TRUE safety action completed,FALSE safety action executing


\par Architecture Requirements ID:
\li
\n\li

\author		Roberto Fioravanti
\date		Mar 14, 2013
\version	1.0.0
*/
uint8_t PM_SFT_WM_DisableTmpCommWithExternalBoard(iom_loads_struct_t *pNewLoads, uint8_t init)
{
	(void) pNewLoads;
	if(init)
	{
		/* issue disable communication command */
		PM_SFT_WM_Motor_DisableTmpComm();
	}
	return TRUE; /* by default return safety action as completed to allow execution of other safety actions */
}
#endif

/**
\n Safety action to prevent driving doorlock from application or platform
\n
\n
\param [in,out] pNewLoads		Not used
\param [in] 	init			Initial step

\return Result of the operation
\retval	TRUE safety action completed,FALSE safety action executing

\par Architecture Requirements ID:
\li
\n\li

\author		Roberto Fioravanti
\date		Nov 20, 2013
\version	1.0.0
*/
uint8_t PM_SFT_WM_SuspendDoorlock(iom_loads_struct_t *pNewLoads, uint8_t init)
{
	uint8_t doorLockPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

	if(doorLockPos != LOAD_NOT_FOUND)
	{
		/* don't execute setLoad */
		pNewLoads->loads_ovr[doorLockPos] = LOAD_DONT_EXE_SETLOAD;
	}
	return TRUE; /* by default return safety action as completed to allow execution of other safety actions */
}




#if defined(CONFIG_PM_MOTORGATE)
/**
\n 			safety action to request to suspend temporarily the motor
\param		[in] init initial step
\param 		[out] pNewLoads not used
\return 	TRUE safety action completed
			FALSE safety action executing;
\author 	Roberto Fioravanti
\date		Mar 14, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by Roberto Fioravanti
*/
uint8_t PM_SFT_WM_AC_MotorProt(iom_loads_struct_t *pNewLoads, uint8_t init)
{
	uint8_t bRel4DoorLock = 0;
	uint8_t loadPos=LIB_IOM_GetLoadPosByExtdLoadItem(EXTLD_ENGINE);
	uint8_t tmpCurStatus[CONFIG_EXT_LOAD_PARAMDATA_SZ];

	if(init)
	{
		PM_SFT_WM_FSM_SlowDownCheckResync();
	}

	/* Not needed to check function result */
	(void) LIB_IOM_GetExtdLoadStatus(EXTLD_ENGINE, tmpCurStatus);
	bRel4DoorLock = tmpCurStatus[5];

	/* Force switch the motor relay ... */
	if (PM_SFT_WM_FSM_IsTachReady() && PM_SFT_WM_FSM_GetIsSlowDownKO())
	{
		bRel4DoorLock = 1<<(RELAY_CFG_FOR_DOOR_LOCK_BIT);
	}

	if (loadPos < N_MAXLOADS)
	{
		pNewLoads->loads_ovr[loadPos] = LOAD_REG_OFF;
		memset(pNewLoads->extdloads_ovr[EXTLD_ENGINE], 0, sizeof(pNewLoads->extdloads_ovr[0]));
		pNewLoads->extdloads_ovr[EXTLD_ENGINE][5] |= bRel4DoorLock;
	}

	return TRUE; /* by default return safety action as executing */
}
#endif /* defined(CONFIG_PM_MOTORGATE) */


uint8_t PM_SFT_WM_ElectrovAllOff(iom_loads_struct_t *newLoads, uint8_t status)
{
	uint8_t ev1Pos, ev2Pos, ev3Pos;
	ev1Pos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_EV_POS);
	ev2Pos = LIB_IOM_GetLoadPos((uint8_t) LD_PRE_WASH_EV_POS);
	ev3Pos = LIB_IOM_GetLoadPos((uint8_t) LD_HOT_WASH_EV_POS);
	if(ev1Pos != LOAD_NOT_FOUND)
	{
		newLoads->loads_ovr[ev1Pos] = LOAD_OFF;
	}
	if(ev2Pos != LOAD_NOT_FOUND)
	{
		newLoads->loads_ovr[ev2Pos] = LOAD_OFF;
	}
	if(ev3Pos != LOAD_NOT_FOUND)
	{
		newLoads->loads_ovr[ev3Pos] = LOAD_OFF;
	}

	return TRUE;
}


/**
\n 			Check if drain pump should be forced ON until ZC
\param		none
\return 	TRUE 	if diode mode fault is in place, ZC present and drain pump works properly during safety action
			FALSE	otherwise
\author 	Roberto Fioravanti
\date		Mar 14, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by Roberto Fioravanti
*/
static bool_t PM_SFT_WM_DrainPumpKeptON(void)
{
	return  (goodZCSignal() && tmrDrainPump.cntSkip &&
			((LIB_SFT_GetFaultSourceStatus(SFT_FLT_DRAIN_TRIAC_DM_OFF) == SFT_MODULE_FAULT) ||
				(LIB_SFT_GetFaultSourceStatus(SFT_FLT_DRAIN_TRIAC_DM_ON) == SFT_MODULE_FAULT) ) );

}
