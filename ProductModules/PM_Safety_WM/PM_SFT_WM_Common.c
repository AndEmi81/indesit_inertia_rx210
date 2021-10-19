/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Safety Common
\n
\n
\file		PM_SFT_WM_Common.c
\ingroup	PM_Safety_WM
\date		13/02/2013
\author		Marco Sperandini
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "Analog.h"
#include "LIB_Generic.h"
#include "LIB_ClassBVar.h"
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_NTCProbe.h"
#if defined(CONFIG_PM_MOTORGATE)
#include "PM_UM_UniversalMotorExp.h"
#endif



/* ****************** */
/* Defines and Macros */
/* ****************** */





/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Info buffer bitfields: It maps only those info invloved with security */
typedef struct InfoExtMotor
{
#if 0 /* temporary disable */
	unsigned int sftcnt1			:8;		/**< Safety counter			*/
	unsigned int sftcnt0			:8;		/**<                  */
#endif
	unsigned int fault1				:8;		/**< Fault					*/
	unsigned int fault0				:8;		/**<                  */
	unsigned int speed1				:8;		/**< Speed					*/
	unsigned int speed0				:8;		/**<                  */
	unsigned int torque1			:8;		/**< Torque                 */
	unsigned int torque0			:8;		/**<                  */
	unsigned int unbalance1			:8;		/**< Unbalance              */
	unsigned int unbalance0			:8;		/**<                  */
	unsigned int flags1				:8;		/**< Flags					*/
	unsigned int flags0				:8;		/**<                  */
	unsigned int chk1				:8;		/**< Modified checksum		*/
	unsigned int chk0				:8;		/**<                  */

}PM_SFT_WM_InfoExtMotorType;


typedef struct {
	uint8_t analogTemeraturePos;
	uint8_t rawTemperatureBound;
	bool_t EngineType_ThreePhase;
} CommonSafetyConf_t;
static CommonSafetyConf_t PM_SFT_WM_Cfg; /* TODO apply DataCare */


/* ********* */
/* Variables */
/* ********* */
/* Global */
extern uint8_t drainPumpStatus;
/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */


/* TODO PM_SFT_WM_Init */
void PM_SFT_WM_Init(void)
{
	PM_SFT_WM_Cfg.analogTemeraturePos = PM_SFT_WM_NTCProbe_GetWashProbePos();
	PM_SFT_WM_Cfg.rawTemperatureBound = 10 ; /* leggere dal file di settings, ancora da definire nell'editor */
	PM_SFT_WM_Cfg.EngineType_ThreePhase = LIB_IOM_ExtdLoadIsExternal(EXTLD_ENGINE);
	SAFETY_DATA_UPDATE( PM_SFT_WM_Cfg );
}

bool_t PM_SFT_WM_CheckLowWaterTemperature(void)
{
	bool_t retVal = TRUE;
	ADC_ValueType rawTemperature;

	rawTemperature = LIB_ADC_GetAnalogValue(PM_SFT_WM_Cfg.analogTemeraturePos);

	if( rawTemperature >  PM_SFT_WM_Cfg.rawTemperatureBound)
	{
		retVal = FALSE;
	}
	return retVal;
}
/* TODO PM_SFT_WM_CheckWaterLevel */
bool_t PM_SFT_WM_CheckWaterLevel(void)
{
	return TRUE;
}


uint8_t PM_SFT_WM_PressSwitch_GetPumpStatus(void)
{
	return drainPumpStatus;
}

/* ------------------------------------------------------------*/



/* Local Functions */





