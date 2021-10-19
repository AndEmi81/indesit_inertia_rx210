/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM
\n
\n
\file		PM_SFT_WM_Common.h
\ingroup	PM_Safety_WM
\date		13/02/2013
\author		Marco Sperandini
*/
#ifndef PM_SFT_WM_COMMON_H_
#define PM_SFT_WM_COMMON_H_
#include "PM_SFT_WM_Motor.h"
/*\addtogroup PM_Safety_WM
@{*/

#define GEN_FLT_RESET_TIMER_VALUE	50	/* Set to 5 sec */
/*#define GEN_FLT_UNLOCKDOOR_TIMER_VALUE	30*/	/* Set to 3 sec */

#define BASETIME_1S_TO_100MS(counter)	(counter*10)  	/**/
#define BASETIME_10S_TO_100MS(counter)	(counter*100)	/**/

#define PRSWH_PUMP_FREE		0
#define PRSWH_PUMP_BLOCKED	1
/**
@name macros to extract Safety Action Param
@{
*/
/**
returns Max value to consider tach in range [us]
*/
#define PM_SFT_WM_GET_TACH_MAX_VAL_TH(_ptr)						MBGET(2, _ptr, TachoThreshold)
/**
returns speed tout [100ms]
*/
#define PM_SFT_WM_GET_SPEED_TOUT(_ptr)							MBGET(1, _ptr, drumSpeedTimeout)
/**
returns min delta to consider slow down [us]
*/
#define PM_SFT_WM_GET_TACH_DELTA(_ptr)							MBGET(2, _ptr, TachoThresholdDelta)

/*@}*/




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* Safety Action Param structure*/
typedef struct SFSafetyActionsParam_s
{
	unsigned char drumSpeedTh0;					/* not used */
	unsigned char drumSpeedTimeout0;
	unsigned char drumSpeedDelta0;				/* not used */
	unsigned char drainTimeout0;
	unsigned char TachoThreshold1;
	unsigned char TachoThreshold0;
	unsigned char TachoThresholdDelta1;
	unsigned char TachoThresholdDelta0;
}PM_SFT_WM_SFSafetyActionsParamType;

/**
 * Info related to the status of a generic load
 */
typedef struct {
	uint8_t pos;				/* Position of the load in the LoadStatus */
	uint8_t status;
	uint8_t statusOld;
}LoadInfos_t;



bool_t PM_SFT_WM_CheckLowWaterTemperature(void);
bool_t PM_SFT_WM_CheckWaterLevel(void);
uint8_t PM_SFT_WM_PressSwitch_GetPumpStatus(void);
void PM_SFT_WM_Init(void);
/*@}*/


#endif /* PM_SFT_WM_COMMON_H_ */
