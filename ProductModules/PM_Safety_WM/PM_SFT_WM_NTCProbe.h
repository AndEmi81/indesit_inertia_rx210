/**
\n
\par		PM_Safety_WM_NTCProbe
\n
\n
\file		PM_SFT_WM_NTCProbe.h
\ingroup	PM_SFT_NTCProbe
\date		04/02/2013
\author		Salvatore Fimiani
\n
*/

#ifndef PM_SAFETYNTCPROBE_H_
#define PM_SAFETYNTCPROBE_H_

/* ********** */
/* Includes */
/* ********* */

#include "Config.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafety.h"
#include "PM_SFT_WM_Common.h"
#include "Analog.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define FAULTSRC_NTCPROBE_NOFAULT 		0		/* The NTC probe operates correctly */
#define FAULTSRC_NTCPROBE_SHORTED 		1		/* The NTC probe is shorted */
#define FAULTSRC_NTCPROBE_OPEN 			2		/* The NTC probe is open */
#define FAULTSRC_NTCPROBE_BLOCKED		3		/* The NTC probe is blocked */
#define FAULTSRC_NTCPROBE_BAD_SETTING	4		/* Problems with parameters from the Setting File */

#define FAULTSRC_NTCPROBE_HIGHTEMP		5		/* This is not a fault source but only a warning source for high temperature */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/**
 * Info related to the control of probe's safety
 * */
typedef enum
{
	wash,
	dryer

}eNTCProbeType;


typedef struct {
	uint8_t startDelayCnt;
	/** Location in ram for the value of NTC Probe */
	uint8_t prbPos;
	/** Alert type for the current cycle */
	uint8_t currentAlertType;
	/** Alert type during the previous cycle */
	uint8_t lastAlertType;
	/** Counter for measuring how long an anomaly on the probe lasts */
	uint16_t cnt;
	uint16_t fltRstTimer;
	/** Probe type (Washing / Drying)*/
	eNTCProbeType prbType;

	struct
	{
		/** Minimum Delta value to be collected in order to declare the probe is not blocked with wash resistor ON */
		uint8_t  deltaCheck;
		/** Time to the probe prefault activation (time base: 100ms) */
		uint16_t prefaultTime;
		/** Time to the probe fault activation (time base: 1s)		*/
		uint16_t faultTime;
		/** Threshold value for shorted NTC probe */
		uint16_t shrtFaultADC_Thr;
		/** Threshold value for open NTC probe */
		uint16_t openFltADC_Thr;
		/** Min ADC value below which you can enable again the washing resistor */
		uint16_t minSafeADC_Thr;
		/** Max ADC value beyond which you must disable the washing resistor */
		uint16_t maxSafeADC_Thr;
		/** Time to update the adc value for temperature with wash resistor ON (time base: 10s) */
		uint16_t checkTimeout;
		/** Minimum time waiting the resistor's cool down when the probe is blocked (minutes)*/
		uint16_t coolDownTimeout;


	} genFltParams;

}  SafetyNTCProbe_t;

typedef struct {
	uint8_t  isTestAlreadyExec;
	uint8_t  protFrom;				/* Says if it is active a protection from blocked probe */
	uint8_t  prbIsBlocked;			/* This flag is TRUE if the probe is blocked, FALSE if not */
	uint8_t  prbBlockedControlON;	/* control request for blocked probe - 1: control ON, 0: control OFF */
	uint16_t prbBlockedControlCnt;

}  SafetyBkNTCProbe_t;



/**
 * Values sampled by NTC probe
 */
typedef struct {

	ADC_ValueType currVal;	/* Current value measured by the NTC probe */
	ADC_ValueType oldVal;	/* Previous value measured by the NTC probe */
	ADC_ValueType minVal;	/* Current minimum value measured by the  NTC probe */
	ADC_ValueType maxVal;	/* Current maximum value measured by the  NTC probe*/

} SampleNTCProbe_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* To be inserted into Backup */
extern uint16_t coolDownCnt;			/* Counter for measuring time between door opening and closing */

extern SafetyBkNTCProbe_t persWashNTCProbe;
extern SafetyNTCProbe_t washNTCProbe;
extern SampleNTCProbe_t washProbeSample;

#if defined (CONFIG_APPLIANCE_WD)
extern SafetyBkNTCProbe_t persDryNTCProbe;
extern SafetyNTCProbe_t dryNTCProbe;
extern SampleNTCProbe_t dryProbeSample;
#endif



/* ******************* */
/* Function prototypes */
/* ******************* */
uint8_t PM_SFT_WM_NTCProbe_GetWashProbePos(void);
uint16_t PM_SFT_WM_washNTCProbe_GetTime(sft_module_status_t *status);
void PM_SFT_WM_washNTCProbe_Init(sft_module_status_t *status);
void PM_SFT_WM_washNTCProbe_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
#if defined (CONFIG_APPLIANCE_WD)
uint16_t PM_SFT_WM_dryNTCProbe_GetTime(sft_module_status_t *status);
void PM_SFT_WM_dryNTCProbe_Init(sft_module_status_t *status);
void PM_SFT_WM_dryNTCProbe_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags);
#endif

bool_t PM_SFT_WM_NTCProbe_isHighTemp(eNTCProbeType probe);

#endif /* PM_SAFETYNTCPROBE_H_ */


/* *********** */
/* End of file */
/* *********** */
