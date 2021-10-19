/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/*
 * PM_Safety_DR_SafetyActions.c
 *
 *  Created on: Jun 05, 2013
 *      Author: Alessandro Sabbatini
 */


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SFT_DR_SafetyActions.h"
#include "PM_SFT_DR_Common.h"
#include "PM_SFT_DR_Heaters.h"
#include "LoadsRequestExp.h"
#include "SwTimersCounters.h"
#include "LIB_UTilities.h"
#include "LIB_ClassBVar.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define SAFETY_COOLTUMBLE_TOUT(p)		(OFFS_B((uint8_t*)(p), 3))	/* x10 sec, default 30*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */
typedef struct sftCoolTumble_s
{
	uint32_t tmrCoolTumble;
	uint16_t txRatio;
} sftCoolTumble_t;

static sftCoolTumble_t sftCoolTumble;

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/**
\n 			Force Heater Master Relay OFF if MRR check is not in progress
\param		...
\return 	TRUE	always

\author 	Alessandro Sabbatini
\date		Jun 13, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_DR_OffHeatersMRelay(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

	/* Disabled only if single pin MR check is in progress */
	if (!PM_SFT_DR_Heater_isResMRCheckInProgr())
	{
		LIB_IOM_UpdateLoadRequestByItem(LD_RES_MASTER_RELAY_ITM, LOAD_REG_OFF, newLoads->loads_ovr);
	}

	return TRUE;

}

/**
\n 			Force Heater Master Relay always ON only - it prevents to activate related loads
\param		...
\return 	TRUE	always

\author 	Alessandro Sabbatini
\date		Jun 13, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_DR_KeepHeaterMRelayON(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

	LIB_IOM_UpdateLoadRequestByItem(LD_RES_MASTER_RELAY_ITM,(LOAD_CFG_MR_FORCE_DELAY | LOAD_REG_ON), newLoads->loads_ovr);

	return TRUE;

}

/**
\n 			Force Motor Master Relay ON
\param		...
\return 	TRUE	always

\author 	Alessandro Sabbatini
\date		Jun 13, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_DR_KeepMotorMRelayON(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

	LIB_IOM_UpdateLoadRequestByItem(LD_MOTOR_MASTER_RELAY_ITM,LOAD_REG_ON, newLoads->loads_ovr);
	return TRUE;

}

/**
\n 			Force Motor Master Relay OFF
\param		...
\return 	TRUE	always

\author 	Alessandro Sabbatini
\date		Jun 13, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_DR_OffMotorMRelay(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

	LIB_IOM_UpdateLoadRequestByItem(LD_MOTOR_MASTER_RELAY_ITM,LOAD_REG_OFF, newLoads->loads_ovr);

	return TRUE;

}


/**
\n 			Cool Tumble - keeps motor moving at a given speed for a specific time
\param		...
\return 	TRUE	when time elapsed

\author 	Alessandro Sabbatini
\date		Jun 05, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_DR_CoolTumble(iom_loads_struct_t *newLoads, uint8_t status)
{
	uint8_t retValue = TRUE;
	uint8_t locCoolTumble_Tout = 5;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

		if (status == _ENABLE_)
		{
			Loader_Struct tmpSafetyData;

			if( BasicLoader( SF_PTR_FAULT, SF_DISPL_FAULT_GENERAL, &tmpSafetyData) == SF_OK )
			{
				locCoolTumble_Tout = SAFETY_COOLTUMBLE_TOUT(tmpSafetyData.Data);
			}

			if( BasicLoader( SF_PTR_HW_CONFIG, SF_DSPL_ENGINE_DATA, &tmpSafetyData) == SF_OK )
			{
				if (tmpSafetyData.Data[0] & 0x80)
				{
					sftCoolTumble.txRatio = EndianU16_BToB(*((uint16_t *)(tmpSafetyData.Data + 0x02)));
				}
			}

			/* Cool tumble timer is set in minutes base time */
			sftCoolTumble.tmrCoolTumble = BASETIME_1MIN_TO_100MS(locCoolTumble_Tout);
			SAFETY_DATA_UPDATE(sftCoolTumble);

		}

		SAFETY_DATA_CHECK(sftCoolTumble);

		if (sftCoolTumble.tmrCoolTumble > 0)
		{
			uint8_t engPos = LIB_IOM_GetLoadPosByExtdLoadItem(EXTLD_ENGINE);

			/* Forces MRM ON - if present and even if not set by action */
			PM_SFT_DR_KeepMotorMRelayON(newLoads, status);

			if (engPos != LOAD_NOT_FOUND)
			{
				newLoads->loads_ovr[engPos] = LOAD_REG_ON;

				/* Clear buffer */
				memset(newLoads->extdloads_ovr[EXTLD_ENGINE], 0, sizeof(newLoads->extdloads_ovr[EXTLD_ENGINE]));

				/* Motor is set at fixed speed - convert it using a more readable way */
				newLoads->extdloads_ovr[EXTLD_ENGINE][0] = 0;
				newLoads->extdloads_ovr[EXTLD_ENGINE][1] = 55;
				newLoads->extdloads_ovr[EXTLD_ENGINE][3] = 1;

				/* External motor: set TX-Ratio */
				if (sftCoolTumble.txRatio)
				{
					LIB_IOM_UpdateLoadRequestByItem(LD_MOTOR_POWER_ITM,LOAD_REG_ON, newLoads->loads_ovr);

					/* BPM motor drum speed invertion needed: -55 rpm */
					newLoads->extdloads_ovr[EXTLD_ENGINE][0] = 0xff;
					newLoads->extdloads_ovr[EXTLD_ENGINE][1] = 0xc9;
					newLoads->extdloads_ovr[EXTLD_ENGINE][6] = BF_16_GET(sftCoolTumble.txRatio,8,8);
					newLoads->extdloads_ovr[EXTLD_ENGINE][7] = BF_16_GET(sftCoolTumble.txRatio,0,8);
				}

			}

			if (LIB_SFT_GetFaultSourceStatus(SFT_COOLT_DOORFLTSRC) == SFT_MODULE_PROTECTION)
			{
				sftCoolTumble.tmrCoolTumble = 0;
			}
			else
			{
				sftCoolTumble.tmrCoolTumble--;
				retValue = FALSE;
			}

		}

		SAFETY_DATA_UPDATE(sftCoolTumble);

#endif

		return retValue;
}

#if 0
/**
\n 			safety action implementing the switch relay test procedure (functional, not safety)
\param		[in] init initial step
\param 		[out] pNewLoads not used
\return 	TRUE safety action completed
			FALSE safety action executing;
\author 	Roberto Fioravanti
\date		Mar 19, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by Roberto Fioravanti
*/
uint8_t PM_SFT_WM_SwitchRelayTestProcedure(iom_loads_struct_t *pNewLoads, uint8_t init)
{
	(void) pNewLoads;
	uint8_t res = FALSE; /* by default return safety action as executing */
	sft_module_status_t DrainSafetyStatus={0,0};

	/* TODO delete.  procedure not needed */
	return TRUE;

	/* check for drain pump safety module status */
	if(0 == LIB_SFT_GetSafetyModStatus(SFT_MOD_DRAINPUMP, &DrainSafetyStatus))
	{
		if(DrainSafetyStatus.status < SFT_MODULE_MONITORING)
		{
			if(init)
			{
				/* start the switch relay test procedure */
				PM_SFT_WM_WashRes_SwitchTest_Start();
			}
			else
			{
				res = TRUE;	/* DBG: terminate the test */
			}
		}
		else
		{
			/* there are some problems affecting drain pump, then signal the safety action as terminated ... */
			res = TRUE;
			/* ... and stop relay test procedure */
			PM_SFT_WM_WashRes_SwitchTest_Halt();
		}
	}
	else
	{
		/* cannot retrieve drain pump module status, then terminate prematurely the test ... */
		res = TRUE;
		/* ... and stop relay test procedure */
		PM_SFT_WM_WashRes_SwitchTest_Halt();
	}
	return res;
}
#endif
