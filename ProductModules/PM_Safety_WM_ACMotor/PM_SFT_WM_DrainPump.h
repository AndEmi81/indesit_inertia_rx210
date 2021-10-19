/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_Drainpump
\n
\n
\file		PM_SFT_WM_Drainpump.h
\ingroup	PM_SFT_DrainPump
\date		04/03/2013
\author		Alessandro Sabbatini
\n
*/

#ifndef PM_Safety_DrainPump_H_
#define PM_Safety_DrainPump_H_

#include "Tools.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafetyExp.h"
#include "PM_ConsistencyExp.h"


#define FAULTSRC_DRAINPUMP_TRIAC_SC			1
#define FAULTSRC_DRAINPUMP_TRIAC_DM_ON		2
#define FAULTSRC_DRAINPUMP_TRIAC_DM_OFF		3
#define FAULTSRC_DRAINPUMP_FB_KO			4
#define FAULTSRC_DRAINPUMP_BLOCKED			5
#define FAULTSRC_DRAINPUMP_TRIAC_OC			6
#define FAULTSRC_DRAINPUMP_BAD_SETTING		7


#define DRAIN_CS_OFF_OK				(PM_CS_STATUS_OFF | PM_CS_VALUE_OK)
#define DRAIN_CS_OFF_ERR_GENERIC	(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_GENERIC)
#define DRAIN_CS_OFF_ERR_SC	 		(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_SC)
#define DRAIN_CS_OFF_ERR_DM			(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_DM)

#define DRAIN_CS_ON_OK				(PM_CS_STATUS_ON | PM_CS_VALUE_OK)
#define DRAIN_CS_ON_ERR_GENERIC		(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_GENERIC)
#define DRAIN_CS_ON_ERR_OC			(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_OC)
#define DRAIN_CS_ON_ERR_DM			(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_DM)

#define	SFT_DRAIN_FLT_OC_PREFAULT		0
#define	SFT_DRAIN_FLT_OC_FAULT_PS		2
#define	SFT_DRAIN_FLT_OC_FAULT_VALUE	3

#define	SFT_DRAIN_FLT_SC_PREFAULT		4
#define	SFT_DRAIN_FLT_SC_FAULT_PS		6
#define	SFT_DRAIN_FLT_SC_FAULT_VALUE	7
#define	SFT_DRAIN_FLT_DM_OFF_CHK		8

#define SFT_DRAIN_PARM_TON_AUTO			2
#define SFT_DRAIN_PARM_TOFF_AUTO		3
#define SFT_DRAIN_PARM_TON_AUTO_TEST	8
#define SFT_DRAIN_PARM_VT_MAX_TEMP		12
#define SFT_DRAIN_PARM_VT_RST_TEMP		13
#define SFT_DRAIN_PARM_VT_MIN_TEMP		14
#define SFT_DRAIN_PARM_VT_PREFILTER		15
#define SFT_DRAIN_PARM_VT_BLK_SLOPE		16
#define SFT_DRAIN_PARM_VT_BLK_INTCP		18
#define SFT_DRAIN_PARM_VT_FRE_SLOPE		20
#define SFT_DRAIN_PARM_VT_FRE_INTCP		22
#define SFT_DRAIN_PARM_VT_UNP_SLOPE		24
#define SFT_DRAIN_PARM_VT_UNP_INTCP		26
#define SFT_DRAIN_PARM_VT_FST_TEMP		28


#define SFT_DRAIN_FLTR_TIMER_VALUE	5	/* Set to 500 ms */

#define SFT_DRAIN_TEST_PHASE0		0
#define SFT_DRAIN_TEST_PHASE1		1
#define SFT_DRAIN_TEST_PHASE2		2
#define SFT_DRAIN_TEST_PHASE3		3
#define SFT_DRAIN_TEST_COMPLETE		4
#define SFT_DRAIN_TEST_DISABLED		0xff
/**/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct {
	uint8_t loadPos;
	uint8_t lastloadStatus;
	uint8_t sftSecondTmr;
#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
	uint8_t pinChk;
#endif
	uint16_t cnt;
	uint16_t fltrCnt;
	uint16_t autoOffToutCntr;
	uint8_t	*sftParams;
	uint8_t	*fltParams;
} DrainPumpFaultStatus_t;

typedef struct {
	uint16_t virTempPump;
	uint16_t virTempFreePump;
} DrainPumpVirtualTemp_t;


/* ********* */
/* Variables */
/* ********* */

#if !defined(PM_SFT_WM_DRAIN_PUMP_MODULE)
extern const DrainPumpFaultStatus_t	sftDrainPumpStatus;
extern const DrainPumpVirtualTemp_t	sftDrainPumpVirtualTemp;
#endif

uint16_t PM_SFT_WM_Drainpump_GetTime(sft_module_status_t *status);
void PM_SFT_WM_Drainpump_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_WM_Drainpump_Init( sft_module_status_t *safetyData );

#endif /* PM_Safety_DrainPump_H_ */
