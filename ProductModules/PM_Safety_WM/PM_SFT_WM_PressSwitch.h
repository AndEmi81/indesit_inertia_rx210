/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_PressSwitch
\n
\n
\file		PM_SFT_WM_PressSwitch.h
\ingroup	PM_SFT_PressSwitch
\date		19/03/2013
\author		Salvatore Fimiani
\n
*/

#ifndef PM_SFT_WM_LINEARPRESENSOR_H_
#define PM_SFT_WM_LINEARPRESENSOR_H_

#include "Config.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafety.h"
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_PressSwitchExp.h"




#define ALERTSRC_PRSWH_NONE 						0			/* The press switch operates correctly */
#define ALERTSRC_PRSWH_OUT_OF_RANGE					1
#define ALERTSRC_PRSWH_BAD_EMPTYLEV					2
#define ALERTSRC_PRSWH_LITTLE_WATER					3
#define ALERTSRC_PRSWH_FILL_BLOCKED					4
#define ALERTSRC_PRSWH_DRAIN_BLOCKED				5
#define ALERTSRC_PRSWH_BAD_SPEED_INFO				6
#define ALERTSRC_PRSWH_TO_BE_CALIBRATED				7
#define ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF		8


#define ALERTSRC_PRSWH_BAD_SETTING	255		/* Problems with parameters from the Setting File */



typedef struct
{
	LPS_ValueType val;
	uint8_t ready;
}prsZeroRefInfo_t;
typedef struct
{
	bool_t	 calibOK;				/* Status of the autocalibration (TRUE/FALSE)*/
	bool_t 	cycleInProgr;
	LPS_ValueType zeroRefLev;			/* Pressure switch value for the empty level */
	bool_t isWashResEnabled;			/* This flag says if the Waster Safe Level has been exceeded with at least one EV open */
}CalibStatusInfo_t;



extern LoadInfos_t prsDoorInfos;
extern LoadInfos_t prsEVWashInfos;
extern LoadInfos_t prsEVPrewashInfos;
extern LoadInfos_t prsEVHotwashInfos;
extern LoadInfos_t prsDrainPumpInfos;


/* Variables to be inserted into Backup */
extern CalibStatusInfo_t prsCalibStatus;
extern prsZeroRefInfo_t prsZeroRefInfo;

/* ******************* */
/* Function prototypes */
/* ******************* */
uint16_t PM_SFT_WM_PressSwitch_GetTime(sft_module_status_t *status);
void PM_SFT_WM_PressSwitch_Init(sft_module_status_t *status);
void PM_SFT_WM_PressSwitch_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);



#endif /* PM_SFT_WM_LINEARPRESENSOR_H_ */


/* *********** */
/* End of file */
/* *********** */
