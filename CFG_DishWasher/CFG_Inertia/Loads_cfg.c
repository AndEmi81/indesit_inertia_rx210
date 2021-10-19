/*
 * SafetyModules_Cfg.c
 *
 *  Created on: Jan 14, 2013
 *      Author: alessandro.sabbatini
 */

#define _LOADS_CFG_

#include "Config.h"
#include "LoadsRequest.h"
#include "OutputManagerExp.h"

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

const uint8_t IOM_LoadID_Table[LD_MAX_NUMBER] = {
LD_ENGINE,

LD_DRAIN_PUMP,
LD_RECYCLE_PUMP,

LD_WASH_RES,
LD_DRY_RES,

LD_WASH_EV,
LD_PRE_WASH_EV,
LD_SOFT_EV,
LD_HOT_WASH_EV,
LD_DRY_EV,
LD_AQUASTOP_EV,

LD_DOOR_LOCK,

LD_STANDBY_REG,
LD_EXT_WATCHDOG_REG,

LD_DRY_FAN,

LD_LAMP,

LD_MUX_FEEDBACK,
LD_MUX_PROBE_READ,

LD_DRUM_LOCK,

LD_SWITCH_RELAY,
LD_DYNAQSTOP_EV

};

#if defined(CONFIG_NUM_EXTDLOADS)

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
uint8_t ExtMotor_InfoBuffer[IOM_EXTERNAL_0_INFOBUFFER_SIZE];
#endif

const ext_load_config IOM_extdLoadID_Table[CONFIG_NUM_EXTDLOAD_CFG] = {
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) 	&& defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
		{EXTLD_ENGINE, LD_ENGINE, EXTERN_MODULE_L, IOM_EXTERNAL_0_INFOBUFFER_SIZE, ExtMotor_InfoBuffer},
#endif
#if defined (CONFIG_PM_MOTORGATE)
		{EXTLD_ENGINE, LD_ENGINE, COLLECTORMOTOR_L, 0, NULL}
#endif
};

#endif

#if defined(CONFIG_NUM_SR_LOADS)
const uint8_t IOM_LoadsID_SRTypeLoads_Table[CONFIG_NUM_SR_LOADS] = {
		LD_SWITCH_RELAY,
		LD_DYNAQSTOP_EV
};

uint8_t LIB_IOM_GetSRDelayTime(uint8_t srItem, uint8_t newLoadStatus)
{
	uint8_t retVal = 0;
	(void) newLoadStatus;

	switch(srItem)
	{
	case SRLD_SWITCH_RELAY_POS:
	{
		retVal = 4;
	}
	break;

	default:
		break;
	}

	return retVal;

}

#endif

#if defined(CONFIG_NUM_MR_LOADS)
const uint8_t IOM_LoadsID_MRTypeLoads_Table[CONFIG_NUM_MR_LOADS] = {
		LD_DOOR_LOCK
};
#endif

bool_t LIB_IOM_CFG_SetAllLoadsFilter(uint8_t loadPos)
{
	bool_t	retVal = TRUE;

	if (loadPos < N_MAXLOADS)
	{
		if ( (loadPos == LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS)) ||
				(loadPos == LIB_IOM_GetLoadPos((uint8_t) LD_STANDBY_REG_POS)) )
		{
			retVal = FALSE;
		}
	}

	return retVal;
}
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
