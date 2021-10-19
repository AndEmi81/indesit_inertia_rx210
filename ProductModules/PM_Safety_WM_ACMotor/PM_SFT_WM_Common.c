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
#include "LIB_Utilities.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

#define EEP_LLI_TABPTR_OFFSET			0
#define LLI_TABLE_ITEM_SIZE_IN_BYTES	4

#define LLI_READ_TYPE(pByte)			*(pByte)

#if defined(CONFIG_PM_MOTORGATE) && defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#define INT_IS_MOTOR_3PH		(PM_SFT_WM_Cfg.EngineType_ThreePhase == TRUE)
#else
#define INT_IS_MOTOR_3PH		(IS_MOTOR_3PH)
#endif

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
#if defined(CONFIG_PM_MOTORGATE) && defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	bool_t EngineType_ThreePhase;
#endif
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
#if defined(CONFIG_PM_MOTORGATE) && defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	PM_SFT_WM_Cfg.EngineType_ThreePhase = LIB_IOM_ExtdLoadIsExternal(EXTLD_ENGINE);
#endif
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

/**
\n returns if Motor is threephase
\n
\param		none

\return		bool_t
\retvalue 	TRUE
\retvalue 	FALSE

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/

bool_t PM_SFT_WM_Is3phMotor(void)
{
	return (bool_t) INT_IS_MOTOR_3PH;
}

/**
\n This function tests the Low Level Input Type.

\param 		[in] lliTypeID Low Level Input Type ID

\return 	uint8_t The result of research
\retval 	TRUE
\retval 	FALSE
\retval 	BASICLOADER_ERR

\author		Salvatore Fimiani
\date		01/04/2014
\version

\note
*/

uint8_t PM_SFT_WM_IsLLIPresent(uint8_t lliTypeID)
{
	struct Loader lliTabPtr;
	uint8_t *pByte;  /* Local pointer used to scan inputs */

	uint8_t retVal = FALSE;

	if ( 0 != BasicLoader(SF_PTR_IO_CONFIG, EEP_LLI_TABPTR_OFFSET, &lliTabPtr) )
	{
		retVal = COMMON_BLOADER_ERR; /* Serious Error */
	}
	else
	{
		/* For all generic inputs in the setting file */
		for ( pByte = lliTabPtr.Data;
					pByte < (lliTabPtr.Data + (lliTabPtr.DataLenght*2));
					pByte += LLI_TABLE_ITEM_SIZE_IN_BYTES )
		{

			if( LLI_READ_TYPE(pByte) == lliTypeID )
			{
				retVal = TRUE;
				break;
			}
		}
	}
	return retVal;
}



/* ------------------------------------------------------------*/



/* Local Functions */





