/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_DR_Heaters
\n
\n
\file		PM_SFT_DR_Heaters.h
\ingroup	PM_Safety_DR
\date		26/06/2013
\author		Salvatore Fimiani
\n
*/

#ifndef _PM_SAFETY_DR_HEATERS_H_
#define _PM_SAFETY_DR_HEATERS_H_

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define FAULTSRC_HEATER_NOFAULT 		0		/* All is OK, no fault */
#define FAULTSRC_HEATER_SHORTED 		1		/* The resistor relay is shorted */
#define FAULTSRC_HEATER_FDB_KO			2		/* The feedback for the resistor is KO */
#define FAULTSRC_HEATER_CIRCUIT_KO		3		/* There is a generic fault on the heater resistor's circuit */
#define FAULTSRC_HEATER_MRR_STUCK		4		/* The resistors' Master Relay is stuck closed */
#define FAULTSRC_HEATER_OPEN			5
#define FAULTSRC_HEATER_BAD_SETTING		6		/* Problems with parameters from the Setting File */


#define HEATER_CS_ON_OK						(PM_CS_STATUS_ON | PM_CS_VALUE_OK)
#define HEATER_CS_ON_ERR_GENERIC			(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_GENERIC)
#define HEATER_CS_ON_ERR_FEEDBACK_KO		(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_FEEDBACK_KO)
#define HEATER_CS_ON_ERR_SC					(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_SC)
#define HEATER_CS_ON_ERR_OC					(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_OC)

#define HEATER_CS_OFF_OK					(PM_CS_STATUS_OFF | PM_CS_VALUE_OK)
#define HEATER_CS_OFF_ERR_GENERIC			(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_GENERIC)
#define HEATER_CS_OFF_ERR_FEEDBACK_KO		(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_FEEDBACK_KO)
#define HEATER_CS_OFF_ERR_SC				(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_SC)
#define HEATER_CS_OFF_ERR_OC				(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_OC)

#define HEATER_CS_TRANS_OK					(PM_CS_STATUS_TRANS | PM_CS_VALUE_OK)
#define HEATER_CS_TRANS_ERR_GENERIC			(PM_CS_STATUS_TRANS | PM_CS_VALUE_ERR_GENERIC)
#define HEATER_CS_TRANS_ERR_FEEDBACK_KO		(PM_CS_STATUS_TRANS | PM_CS_VALUE_ERR_FEEDBACK_KO)
#define HEATER_CS_TRANS_ERR_SC				(PM_CS_STATUS_TRANS | PM_CS_VALUE_ERR_SC)
#define HEATER_CS_TRANS_ERR_OC				(PM_CS_STATUS_TRANS | PM_CS_VALUE_ERR_OC)


#define MR_MOTOR_CS_ON_OK					(PM_CS_STATUS_ON | PM_CS_VALUE_OK)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
typedef enum
{
	STATIC_HEATER,
	DYNAMIC_HEATER

}eHeaterResType;


typedef struct {
	uint8_t pos;				/* Position of the load in the LoadStatus */
	uint8_t curStatus;
	uint8_t lastStatus;
}LoadInfos_t;

typedef struct {
	uint8_t chkExec;
	uint8_t checkCnt;
	uint8_t bmpHtrCfg;
	uint8_t bmpHtrChk;
	LoadInfos_t resMrRele;
	LoadInfos_t motMrRele;
} SafetyMRHtr_t;

typedef struct {

	uint8_t monitorAlertType;
	uint16_t cnt; 					/* Counter for measuring how long an anomaly on the probe lasts */
	uint16_t fltRstTimer;
	eHeaterResType resType;
	LoadInfos_t info;
	struct
	{
		uint16_t prefaultTimeCC;			/* Time to the prefault activation for Shorted Circuit (time base: 100ms) */
		uint16_t faultTimeCC;				/* Time to the fault activation for Shorted Circuit (time base: 1s)		*/
		uint16_t prefaultTimeCA;			/* Time to the prefault activation for Open Circuit (time base: 100ms) */
		uint16_t faultTimeCA;				/* Time to the fault activation Open Circuit (time base: 1s)		*/

	} genFltParams;

}  SafetyHeater_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/


/* ******************* */
/* Function prototypes */
/* ******************* */
uint16_t PM_SFT_DR_StaHeater_GetTime(sft_module_status_t *status);
uint16_t PM_SFT_DR_DynHeater_GetTime(sft_module_status_t *status);
void PM_SFT_DR_StaHeater_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_DR_DynHeater_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_DR_StaHeater_Init( sft_module_status_t *pModInfo );
void PM_SFT_DR_DynHeater_Init( sft_module_status_t *pModInfo );
void PM_SFT_DR_Heater_Common_Init(void);

bool_t PM_SFT_DR_Heater_isResMRCheckInProgr(void);

#endif	/* _PM_SAFETY_DR_HEATERS_H_ */
