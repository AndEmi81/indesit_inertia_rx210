/*
 * PM_SafetyPTCProbe.h
 *
 *  Created on: 04/feb/2013
 *      Author: consul.fimiani
 */

#ifndef _PM_SFT_CK_PTCPROBE_H_
#define _PM_SFT_CK_PTCPROBE_H_

/* ********** */
/* Includes */
/* ********* */

#include "Config.h"
#include "PM_SFT_CK_PTCProbeExp.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafety.h"
#include "PM_SFT_CK_Common.h"
#include "Analog.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define FAULTSRC_PTCPROBE_NOFAULT 		0		/* The PTC probe operates correctly */
#define FAULTSRC_PTCPROBE_SHORTED 		1		/* The PTC probe is shorted */
#define FAULTSRC_PTCPROBE_OPEN 			2		/* The PTC probe is open */
#define FAULTSRC_PTCPROBE_BLOCKED		3		/* The PTC probe is blocked */
#define FAULTSRC_PTCPROBE_BAD_SETTING	4		/* Problems with parameters from the Setting File */

#define FAULTSRC_PTCPROBE_HIGHTEMP		5		/* This is not a fault source but only a warning source for high temperature */
#define FAULTSRC_PTCPROBE_HIGHTEMP_OL	6		/* high temp Over Limit */
#define FAULTSRC_PTCPROBE_MISMATCH		7
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/


typedef struct {
	uint8_t testDelayCnt;
	uint8_t startDelayCnt;
	uint8_t prbPos;					/* Location in ram for the value of PTC Probe */
	uint8_t currentAlertType;
	uint8_t lastAlertType;
	uint8_t numLoadON;				/* Number of loads related to this probe which are ON */
	uint16_t cnt; 					/* Counter for measuring how long an anomaly on the probe lasts */
	uint16_t prbBlockedTestCnt;
	uint16_t fltRstTimer;
	ePTCProbeType prbType;


	struct
	{
		uint8_t  deltaCheck;		/* Minimum Delta value to be collected in order to declare the probe is not blocked with wash resistor ON */
		uint16_t shrtFaultADC_Thr;
		uint16_t openFltADC_Thr;
		uint16_t minSafeADC_Thr;
		uint16_t maxSafeADC_Thr;
		uint16_t testTimeout;		/* Time during which testing the blocked probe (time base: 10s) */
		uint16_t ovrtPrefTime;		/* Time to the over temperature probe prefault activation (time base: 100ms) */
		uint16_t ovrtFltTime;		/* Time to the over temperature probe fault activation (time base: 100ms) */
		uint16_t fltTimeout;
	} fltParams;



}  SafetyPTCProbe_t;

typedef struct {
	uint8_t  statusBuf;
	uint8_t  currCycleStatus;
	uint8_t  lastCycleStatus;
	uint8_t  protFrom;				/* Says if it was active an alert */
	uint8_t  prbBlockedTestON;		/* control request for blocked probe - 1: control ON, 0: control OFF */
	uint8_t  prbTestResult;
	uint8_t  wasInFault;			/* Flag useful to decide if there was a fault for blocked probe */
	uint16_t filterCnt;

	uint8_t  status;
	uint8_t  lastStatus;
	uint16_t faultHightTempCounter;
	uint16_t limitFaultHightTempCounter;
}  PTC_SftCntrlData_t;


struct
{
	uint16_t prefTime;			/* Time to the general probe prefault activation (time base: 100ms) */
	uint16_t fltTime;			/* Time to the general probe fault activation (time base: 1s)		*/

}genFltParams;


/**
 * Values sampled by PTC probe
 */
typedef struct {

	ADC_ValueType currVal;	/* Current value measured by the PTC probe */
	ADC_ValueType oldVal;	/* Previous value measured by the PTC probe */
	ADC_ValueType minVal;	/* Current minimum value measured by the  PTC probe */
	ADC_ValueType maxVal;	/* Current maximum value measured by the  PTC probe*/

} SamplePTCProbe_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/* To be inserted into Backup */
extern SafetyPTCProbe_t topPTCProbe;
extern SafetyPTCProbe_t bottomPTCProbe;
extern PTC_SftCntrlData_t persTopPTCProbe;
extern PTC_SftCntrlData_t persBottomPTCProbe;

/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_SFT_CK_PTCProbe_TopInit(sft_module_status_t *probeMod);
void PM_SFT_CK_PTCProbe_BottomInit(sft_module_status_t *probeMod);
void PM_SFT_CK_PTCProbe_TopMonitor(iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_CK_PTCProbe_BottomMonitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags);

uint16_t PM_SFT_CK_PTCProbe_TopGetTime(sft_module_status_t *status);
uint16_t PM_SFT_CK_PTCProbe_BottomGetTime(sft_module_status_t *status);
uint16_t PM_SFT_CK_PTCProbe_GetRawTemperature( uint8_t *err);




#endif /* _PM_SFT_CK_PTCPROBE_H_ */


/* *********** */
/* End of file */
/* *********** */
