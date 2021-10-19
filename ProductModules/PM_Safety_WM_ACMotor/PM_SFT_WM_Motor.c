/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_Motor
\n
\file		PM_SFT_WM_Motor.c
\ingroup	PM_SFT_Motor
\date		08/03/2013
\author		Roberto Fioravanti
\n

\par Architecture Requirements ID:
\n\li SARS_XTM_MN_SMI_1
\n\li SARS_XCM_MN_SCM_1
*/

/* ******** */
/* Includes */
/* ******** */
#include "PM_SFT_WM_Common.h"
#include "LIB_Utilities.h"
#if defined(SFT_MOTOR_PROTO_ARCADIA3)
#include "LIB_CrcExp.h"
#else
#include "LIB_Generic.h"
#endif

#include "PM_ConsistencyExp.h"
#include "LIB_ClassBVar.h"
#include "Tools.h"

#if defined(CONFIG_PM_MOTORGATE)
#include "PM_UM_UniversalMotorExp.h"
#include "PM_UM_Tachometer.h"
#include "PM_DoorLockExp.h"
#endif

#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
/**
@name macros to extract data from Info buffer
Data in Info buffer is encoded or takes more byte. these macro do all the work for you.
They rely on PM_SFT_WM_InfoExternMotorType definition
@{
*/
/**
macro returning Safety Counter
*/
#define PM_SFT_WM_GET_EXTMOTOR_SFTCNT(_ptr)					MBGET(2, _ptr, sftcnt)
/**
macro returning the Fault
*/
#define PM_SFT_WM_GET_EXTMOTOR_FAULT(_ptr)					MBGET(1, _ptr, fault)
/**
macro returning the Subcode or the Fault
*/
#define PM_SFT_WM_GET_EXTMOTOR_FAULTSUB(_ptr)				MBGET(1, _ptr, faultsub)
/**
macro returning the drum speed in rpm
*/
#define PM_SFT_WM_GET_EXTMOTOR_SPEED(_ptr)					MBGET(2, _ptr, speed)
/**
macro returning the Modified Checksum
*/
#define PM_SFT_WM_GET_EXTMOTOR_CHKMOD(_ptr)					MBGET(2, _ptr, chk)
/*@}*/
#endif

#if defined(CONFIG_PM_MOTORGATE)
/**
@name macros to extract data from Motor Fault Param setting structure
@{
*/
/**
returns prefault time filter validation of feedback [100ms]
*/
#define PM_SFT_WM_GET_FBK_VALIDATION_TIME(_ptr)				MBGET(2, _ptr, fbkPrefaultTime)

/**
returns phase angle threshold [us]
used to filter consistency: values of phase anlge under this th mean triac in short circuit (but this is normal)
*/
#define PM_SFT_WM_GET_PHASE_ANGLE_TH(_ptr)					MBGET(2, _ptr, phaseangleTh)

/**
minimum motor off command duration [s]
*/
#define PM_SFT_WM_GET_PROTECTION_TIME(_ptr)					MBGET(1, _ptr, MotorOffTime)

/* not used  yet*/
#define PM_SFT_WM_GET_DOOR_LOCK_OFF_TIME(_ptr)				MBGET(1, _ptr, DLTime)

/* not used yet */
#define PM_SFT_WM_GET_REL_FAULT_FILTER_TIME(_ptr)			MBGET(1, _ptr, RelFaultFilterTime)
/*@}*/
#endif

/*
@name macros to extract data from motor command
@{

retrieve drum speed in rpm from a pointer _ptr to MotorCmdType */
#define PM_SFT_WM_GET_MOTOR_SPEED_REQ(_ptr)					MBGET(2, _ptr, speed)

/*@}*/

/*
@name Debug-related defines
@{
enables debug support to simultate by sw fault conditions
*/
#define PM_SFT_WM_DEBUG_SUPPORT
/* enables debug support to event logging */
#define PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT

/*@}*/


/*
defines for the support to the debug

highest to lowest priority
- force_match
- match,force_no_match
@{
*/
#define PM_SFT_WM_MATCH_DEBUG(match, force_no_match, force_match)		(((match) && (!(force_no_match))) || (force_match))
#define PM_SFT_WM_MATCH(match)											(match)

#if defined(PM_SFT_WM_DEBUG_SUPPORT)
/*
 to make debug easier special conditions have been implemented to force results making use of debug variables
 format: 					original condition --- if true forces condition to be false --- if true forces condition to be true
 priority:					<--		Low					---			Medium				---							High	-->
*/
#define PM_SFT_WM_MATCH_SFT_CNT(sftcnt_match)							PM_SFT_WM_MATCH_DEBUG((sftcnt_match), WM_MotorDebug.simulate_updated , WM_MotorDebug.simulate_not_updated)
#define PM_SFT_WM_MATCH_SIGN(sign_match)								PM_SFT_WM_MATCH_DEBUG((sign_match), WM_MotorDebug.simulate_integrityKO , WM_MotorDebug.simulate_integrityOK)
#define PM_SFT_WM_MATCH_FAULT(fault_match)								PM_SFT_WM_MATCH_DEBUG((fault_match), FALSE , WM_MotorDebug.simulate_fault)
#define PM_SFT_WM_MATCH_ZC_GOOD(zc_good)								PM_SFT_WM_MATCH_DEBUG((zc_good), WM_MotorDebug.simulate_NoZC, FALSE)
#define PM_SFT_WM_MATCH_MISSING_TACH(missingTach)						PM_SFT_WM_MATCH_DEBUG((missingTach), FALSE, WM_MotorDebug.simulate_missingTach)
#define PM_SFT_WM_MATCH_MISSING_TACH_AT_SSTART(missingTach)				PM_SFT_WM_MATCH_DEBUG((missingTach), FALSE, WM_MotorDebug.simulate_missingTachAtSStart)
#define PM_SFT_WM_MATCH_DRUM_STOP(bStop)								PM_SFT_WM_MATCH_DEBUG((bStop), FALSE, WM_MotorDebug.simulate_drumStop)

#else
/*release version conditions not using debug variables */
#define PM_SFT_WM_MATCH_SFT_CNT(sftcnt_match)							PM_SFT_WM_MATCH((sftcnt_match))
#define PM_SFT_WM_MATCH_SIGN(sign_match)								PM_SFT_WM_MATCH((sign_match))
#define PM_SFT_WM_MATCH_FAULT(fault_match)								PM_SFT_WM_MATCH((fault_match))

#define PM_SFT_WM_MATCH_ZC_GOOD(zc_good)								PM_SFT_WM_MATCH((zc_good))
#define PM_SFT_WM_MATCH_MISSING_TACH(missingTach)						PM_SFT_WM_MATCH((missingTach))
#define PM_SFT_WM_MATCH_MISSING_TACH_AT_SSTART(missingTach)				PM_SFT_WM_MATCH((missingTach))
#define PM_SFT_WM_MATCH_DRUM_STOP(bStop)								PM_SFT_WM_MATCH((bStop))
#endif /* defined(PM_SFT_WM_DEBUG_SUPPORT) */
/*@}*/


/* AC motor related defines */
#if defined(CONFIG_PM_MOTORGATE)
	/* empty */
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
Bit for motor safety event bitmap SFT_MotorEventBmpType
*/
enum eSFT_MotorSftEvent
{
	SFT_MOTOR_EVT_NO_ZC=0,								/**< no ZC */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	SFT_MOTOR_EVT_EXTERNAL_MONITOR_REQ,					/**< External board info buffer monitor request */
	SFT_MOTOR_EVT_INTEGRITY_FAULT,						/**< info buffer integrity issue */
	SFT_MOTOR_EVT_COMM_FAULT,							/**< communication not working fine, issued until safety counter is ok */
	SFT_MOTOR_EVT_EXTERNAL_FAULT,						/**< fault by external board*/
	SFT_MOTOR_EVT_FAULT_TIMEOUTED,
#endif
	SFT_MOTOR_EVT_ZC_GOOD,								/**< Door Locked*/
	SFT_MOTOR_EVT_INVALID_SETTING_PARAMS,
#if defined(CONFIG_PM_MOTORGATE)
	/* missing tach */
	SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,			/**< SoftStart KO: missing tach during soft start*/
	SFT_MOTOR_EVT_MISSING_TACH,							/**< missing tach after soft start */
	SFT_MOTOR_EVT_DRUM_STOP,							/**< drum not running */
	SFT_MOTOR_EVT_AUX1,
	SFT_MOTOR_EVT_AUX2,

	/*slow down */
	SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO,					/**< slow down KO , means tach in range */
	SFT_MOTOR_EVT_TACH_OUT_OF_RANGE,					/**< tach out of range, slow down check is not not available */
	SFT_MOTOR_EVT_TACH_SLOW_DOWN_READY,					/**< new slow down check available */
	SFT_MOTOR_EVT_TACH_IN_RANGE,						/**< tach in of range */

	/* actions*/
	SFT_MOTOR_EVT_RESET_PREFAULT_REQ,
	SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,				/**< protection requested by tach KO */
	SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,					/**< protection requested by FBK error */
	SFT_MOTOR_EVT_REL_MOTOR_PREFAULT_REQ,				/**< relay prefault FAULT */

	/*consistency-relatec events */
	SFT_MOTOR_EVT_FBK_ERR_INIT,							/**< start of FBK KO */
	SFT_MOTOR_EVT_FBK_GEN_ERR,							/**< FBK generic error */
	SFT_MOTOR_EVT_FBK_T_DM_ERR,							/**< FBK error : triac in diode mode */
	SFT_MOTOR_EVT_FBK_RT_SC_DETECTED,					/**< temp detection of triack in short circuit */
	SFT_MOTOR_EVT_FBK_RT_SC_ERR,						/**< FBK error : triac in short circuit */
	SFT_MOTOR_EVT_FBK_MOTOR_OFF_OK,						/**< motor off is ok */
	SFT_MOTOR_EVT_FBK_MOTOR_ON_OK,						/**< motor ON is ok */
	SFT_MOTOR_EVT_REL_FAULT_INIT,						/**< relay FAULT init */
	SFT_MOTOR_EVT_REL_FAULT,							/**< relay FAULT */
#endif
	/*loads */
	SFT_MOTOR_EVT_LOCK_DOOR_REQ,						/**< current door lock req is on */
	SFT_MOTOR_EVT_MOTOR_OVR_OFF,						/**< turn off motor request  by safety */
	SFT_MOTOR_EVT_MOTOR_REQ_ON,							/**< tunr on motor request by application */

	/* safety status */
	SFT_MOTOR_EVT_PROT,									/**< current safety status protection */
	SFT_MOTOR_EVT_IDLE,									/**< current safety status idle  */
	SFT_MOTOR_EVT_MONIT,								/**< current safety status monitoring  */

	/* number of items */
	SFT_MOTOR_EVT_NUM

};

/*
Motor safety events bitmap
\ref eSFT_MotorSftEvent
*/
typedef uint32_t SFT_MotorEventBmpType;

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
/** Protection state machine states
*/
typedef enum eSFT_CommProtection
{
	PM_SFT_WM_MOTOR_COMMPROT_IDLE=0,					/**< no problem */
	PM_SFT_WM_MOTOR_COMMPROT_WAIT_4_COMM_ACTIVE,		/**< not used at now */
	PM_SFT_WM_MOTOR_COMMPROT_FAULT_FILTER,				/**< filter */
	PM_SFT_WM_MOTOR_COMMPROT_NO_COMM					/**< communication inactive */
}PM_SFT_WM_CommProtectionStepType;

/** safety action communication stare machine states */
typedef enum eSFT_CommManager
{
	PM_SFT_WM_MOTOR_COMM_IDLE=0,						/**< no problem */
	PM_SFT_WM_MOTOR_COMMSUSPEND_COMM					/**< communication suspended */
}PM_SFT_WM_CommManagerStepType;
#endif

/**
Info buffer safety counter
*/
typedef uint16_t	InfoCntType;

/**
fault structure
*/
typedef struct ExternalMotorFault_s
{
	uint8_t uCode;
	uint8_t uSubCode;
}PM_SFT_WM_ExternalMotorFaultType;

/** motor state */
enum eSFT_MotorState
{
	PM_SFT_WM_MOTOR_NO_ERROR=0,								/**< NB. this is not a bit, this label is used as value and means no error : communication is working fine, data integrity, ensured, and non fault by external*/
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	/* 3ph motor*/
	PM_SFT_WM_MOTOR_GENERIC,								/**< bit, stable, generic error. e.g. invalid pointer passed as argument of a function */
	PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,				/**< bit, unstable, data you are trying to get has been corrupted */
	PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID,			/**< bit, stable, data corruption validated */
	PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,						/**< bit, unstable, not receiving updated data from external*/
	PM_SFT_WM_MOTOR_EXTERNAL_FAULT,							/**< bit, stable, fault notification by external board */
	PM_SFT_WM_MOTOR_EXTERNAL_FAULT_HAPPENED,				/**< bit, stable, fault notification has been notified by external board, remains active for some seconds  */
	PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,				/**< bit, virtual ramp timeouted */
#endif
	/* Common */
	PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING,						/**< bit	1, unexpected no ZC while motor running if set implies also SFT_MOTOR_EVT_NO_ZC is set */
	/* AC motor */
	PM_SFT_WM_MOTOR_TACH_READY,								/**< bit	2, tacho ready  */
	PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,						/**< bit	3, wait 4 stop missing tacho */
	PM_SFT_WM_MOTOR_NOTACH_SSTART,							/**< bit	4, no tach @ soft start  */
	PM_SFT_WM_WAIT_FOR_MOTOR_STOP,							/**< bit	5, wait 4 stop with tach OK */
	PM_SFT_WM_MOTOR_FORCE_FBK_PROT,							/**< bit	6,  */
	PM_SFT_WM_MOTOR_TRIAC_SC_WARN,							/**< bit	7,  */
	PM_SFT_WM_MOTOR_FORCE_REL_SC_PROT,						/**< bit	8,  */
	PM_SFT_WM_MOTOR_THERMAL_PROT,							/**< bit	9,  */
	/*num of states */
	PM_SFT_WM_STATES_NUM
};


typedef struct MotorData_s
{
	/* data */
	DrumSpeedType						iDrumSpeed;			/* current drum speed: valid if state is PM_SFT_WM_MOTOR_NO_ERROR*/
	PM_SFT_WM_ExternalMotorFaultType 	fault;				/* current fault valid if bit PM_SFT_WM_MOTOR_EXTERNAL_FAULT in state is high */
	PM_SFT_WM_ExternalMotorFaultType 	prot;				/* global safety protection (comm fault and external board fault)*/
	PM_SFT_WM_StateType					state;				/* state*/
	TOOLS_TIMER							speedFilter;		/* motor speed filter */
	TOOLS_TIMER							monitorTimer;		/* 	3ph-Motor: MONITORING, fault filter
																AC-Motor: MONITORING
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	*/
	TOOLS_TIMER							unlockDL;			/* timeout to unlock door if ZC drops while motor is running */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	/* state */
	InfoCntType							nfoCntLastValue;	/* last safety counter */

	PM_SFT_WM_CommProtectionStepType	commProtStep;		/* 'protection' state machine monitor for communication*/


	PM_SFT_WM_CommManagerStepType		commStep;			/* state machine for communication : temp suspension of communication*/
	bool8_t								bSuspendCommReq;	/* if not zero, request to suspend communication */
	bool8_t								bRestoreCommReq;	/* if not zero, communication restored */
	bool8_t								bCommActive;		/* communication active */
	TOOLS_TIMER							timer;

#endif /*defined(CONFIG_EXTERN_MODULE_L_SUPPORT)*/

#if defined(CONFIG_PM_MOTORGATE)
	uint8_t								MotorPos;
	Loader_Struct 						faultParam;
	TOOLS_TIMER							timerVirtualStop;
#endif /*defined(CONFIG_PM_MOTORGATE)*/

	TOOLS_TIMER							timer2;				/* 	3ph-Motor: 'protection' state machine monitor for communication
																AC-Motor: REL fault
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	*/
}
PM_SFT_WM_MotorDataType;




/** Info buffer bit-fields: It maps only those info involved with security
This structure definition reflects how data are encoded physically.
To avoid errors in decoding these fields, use macros provided in this module.
*/
typedef struct InfoExtMotor
{
	unsigned char sftcnt1;				/**< Safety counter - 2 bytes, unsigned , BE				*/
	unsigned char sftcnt0;				/*                 											*/
	unsigned char fault0;				/**< Fault													*/
	unsigned char faultsub0;			/**< Fault subcode          								*/
	unsigned char speed1;				/**< normalized motor speed [rpm] - 2 bytes, signed, BE  	*/
	unsigned char speed0;				/*                  										*/
#if !defined(SFT_MOTOR_PROTO_ARCADIA3)
	unsigned char unused[6];			/* unused - not relevant data in this context				*/
#endif
	unsigned char chk1;					/**< Modified checksum	- 2 bytes, unsigned, BE				*/
	unsigned char chk0;					/*                  										*/
}PM_SFT_WM_InfoExternMotorType;


/* AC motor related defines */
#if defined(CONFIG_PM_MOTORGATE)
/** Motor fault Setting's parameters
*/
typedef struct SFMotorFault_s
{
	unsigned char fbkPrefaultTime1;				/**< FB Based Prefault Filter time [100 ms], 2 bytes unsigned , BE	, default 500ms*/
	unsigned char fbkPrefaultTime0;				/*																	*/
	unsigned char unused[7];					/* unused - not relevant data in this context						*/
	unsigned char MotorOffTime0;				/**< Pause Time NoTacho Attempts [s] 								*/
	unsigned char DLTime0;						/**< Time DL OFF DM Fault 											*/
	unsigned char RelFaultFilterTime0;			/**<  																*/
	unsigned char unused2[4];					/* unused - not relevant data in this context						*/
	unsigned char phaseangleTh1;				/**< phase angle th , 2 bytes unsigned , BE							*/
	unsigned char phaseangleTh0;				/*																	*/
}PM_SFT_WM_SFMotorFaultType;
#endif

/* the whole structure is 6 bytes but in this context only the firt two byte are relevant */
typedef struct MotorCmd_s
{
	unsigned char speed1;						/**< target speed [rpm], 2 bytes unsigned , BE	*/
	unsigned char speed0;
}MotorCmdType;
/**
debug struct
*/
#if defined(PM_SFT_WM_DEBUG_SUPPORT)
typedef struct WM_MotorDebug_s
{
/* Common fields*/
	/*ZC*/
	bool8_t	simulate_NoZC;

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
/* 3ph-Motor specific debug */
	/* Seq ID */
	bool8_t	simulate_updated;
	bool8_t	simulate_not_updated;
	/* Intetrity */
	bool8_t	simulate_integrityKO;
	bool8_t	simulate_integrityOK;
	/* fault*/
	bool8_t	simulate_fault;
#endif /* defined(CONFIG_EXTERN_MODULE_L_SUPPORT)*/
#if defined(CONFIG_PM_MOTORGATE)
/* AC-Motor specific debug*/
	bool8_t simulate_missingTach;				/* set to TRUE to simulate missing tach */
	bool8_t	simulate_missingTachAtSStart;		/* set to TRUE to simulate missing tach at soft start */
	bool8_t	simulate_drumStop;					/* set to TRUE to simulate drum stop */
#endif
	PM_SFT_WM_ExternalMotorFaultType	fault;
}WM_MotorDebugType;
#endif /* defined(PM_SFT_WM_DEBUG_SUPPORT) */

#if defined(CONFIG_PM_MOTORGATE)

/* AC motor related structures */

/***************/
/* SlowDown FSM*/
typedef enum ePM_SFT_WM_FSMSlowDownTachCheck
{
	PM_SFT_WM_FSM_SLOWDOWN_INIT = 0,
	PM_SFT_WM_FSM_SLOWDOWN_WAIT_4_EVAL
}PM_SFT_WM_FSM_SlowDownTachType;

typedef struct PM_SFT_WM_FSM_SlowDown_s{
	PM_SFT_WM_FSM_SlowDownTachType	fsm_state;
	TOOLS_TIMER Tout;
	OS_TIME	Period;				/* Period for SlowDown check in ms */
	int32_t TachValue;			/* prev tach value */
	int32_t TachTh;				/* Tach threshold value under which SlowDown check is active */
	int32_t DeltaTachTh;		/* delta Tach threshold value under which SlowDown is accepted */
	uint32_t evt;				/**/
}PM_SFT_WM_FSM_SlowDownType;

/****************/
/* tach monitor */

typedef struct PM_SFT_WM_FSM_TachMonitor_s{
	SFT_MotorEventBmpType			evt;
	TOOLS_TIMER						tachValid;
}PM_SFT_WM_FSM_TachMonitorType;

/******************/
/* Soft Start FSM */
typedef enum ePM_SFT_WM_FSMSoftStart
{
	PM_SFT_WM_FSM_SOFTSTART_INIT = 0,
	PM_SFT_WM_FSM_SOFTSTART_WAIT_4_NOTACH,
	PM_SFT_WM_FSM_SOFTSTART_MOTOR_OFF_PAUSE
}PM_SFT_WM_FSMSoftStartType;

typedef struct PM_SFT_WM_FSM_SoftStart_s{
	PM_SFT_WM_FSMSoftStartType	fsm_state;
	TOOLS_TIMER Tout;
}PM_SFT_WM_FSM_SoftStartType;

/*************/
/*Consistency*/
typedef enum ePM_SFT_WM_FSMConsistency
{
	PM_SFT_WM_FSM_CONSISTENCY_INIT = 0,
	PM_SFT_WM_FSM_CONSISTENCY_WAIT_4_ZC_GOOD,
	PM_SFT_WM_FSM_CONSISTENCY_WAIT_4_PROT_COMPLETED
}PM_SFT_WM_FSMConsistencyType;

typedef enum ePM_SFT_WM_FSM_SC
{
	PM_SFT_WM_FSM_SC_INIT = 0,
	PM_SFT_WM_FSM_SC_IDLE = 1, /*  detection */
}PM_SFT_WM_FSM_SC_Type;


typedef struct PM_SFT_WM_FSM_Consistency_s{
	PM_SFT_WM_FSMConsistencyType	fsm_state;

	PM_SFT_WM_FSM_SC_Type			fsm_state_sc;
	SFT_MotorEventBmpType			evt;
	TOOLS_TIMER Tout;
	int validationTime;
	int TimeOut;
}PM_SFT_WM_FSM_ConsistencyType;


typedef struct PM_SFT_WM_FSM_ThermalProt_s{

	TOOLS_TIMER Tout;

}PM_SFT_WM_FSM_ThermalProtType;
#endif

#ifdef PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT
typedef struct PM_SFT_WM_EventAcq{
	SFT_MotorEventBmpType	eventbmp;
	PM_SFT_WM_StateType		state;
	OS_TIME					time;
}PM_SFT_WM_EventAcqType;
#endif /* PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT */
/* ********* */
/* Variables */
/* ********* */

/* Global */


/* Static */
/* safety motor data*/
static SFT_MotorEventBmpType 					Events;			/* unsecure varriable, general purpose */
static PM_SFT_WM_MotorDataType				MotorData;		/* subject to  Data care */
#ifdef PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT
static uint8_t MonitorCnt[SFT_MOTOR_EVT_NUM], StatesMonitorCnt[PM_SFT_WM_STATES_NUM];
static PM_SFT_WM_EventAcqType EventBuff[64];
int8_t pos=0 , pos_in=0;
PM_SFT_WM_StateType	StateMsk = 0;
SFT_MotorEventBmpType EventMsk= 0;
OS_TIME	startTime;
#endif

#if defined(PM_SFT_WM_DEBUG_SUPPORT)
static volatile WM_MotorDebugType			WM_MotorDebug ={
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
															1, /* simulate_updated */
															0, /* simulate_not_updated */
															0, /* simulate_integrityKO */
															1, /* simulate_integrityOK */
															0, /* simulate_fault*/
															0, /* simulate_NoZC*/
															{0,0}
#endif
#if defined(CONFIG_PM_MOTORGATE)
															0, /* simulate missing tach*/
															0, /* simulate missing tach @ soft start*/
															0  /* simulate drum stop*/
#endif
															}; /* */
#endif /* defined(PM_SFT_WM_DEBUG_SUPPORT) */

/* AC motor related structures */
#if defined(CONFIG_PM_MOTORGATE)
static PM_SFT_WM_FSM_SlowDownType			PM_SFT_WM_Motor_SlowDown; 		/* slown-down fsm variable, subject to Data care */
static PM_SFT_WM_FSM_SoftStartType			PM_SFT_WM_Motor_SoftStart;		/* soft-start fsm variable, subject to Data care */
static PM_SFT_WM_FSM_ConsistencyType		PM_SFT_WM_Motor_Consistency;	/* consistency fsm variable, subject to Data care */
static PM_SFT_WM_FSM_TachMonitorType		PM_SFT_WM_Motor_TachMonitor;	/* */

static PM_SFT_WM_FSM_ThermalProtType		PM_SFT_WM_Motor_ThermalProt;	/* consistency fsm variable, subject to Data care */

#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
static bool_t PM_SFT_WM_CommunicationWithExternalBoardIsGood(void);
static void PM_SFT_WM_Motor_CommProt(PM_SFT_WM_StateType	state);
static void PM_SFT_WM_Motor_CommManager(void);
static void PM_SFT_WM_ResetCommManager(void);
static void PM_SFT_WM_ProcessExternMotorInfo(void);

static bool_t PM_SFT_WM_ExternalBoardIsFault(void);
#endif /* defined(CONFIG_EXTERN_MODULE_L_SUPPORT) */

/* common */
static void PM_SFT_WM_Motor_InitState(void);
static bool_t PM_SFT_WM_SwitchToProtection(SFT_MotorEventBmpType event);
static bool_t PM_SFT_WM_SwitchToWarning(SFT_MotorEventBmpType event);

#if defined(CONFIG_PM_MOTORGATE)
/*SlowDownCheck FSM*/
static SFT_MotorEventBmpType PM_SFT_WM_FSM_SlowDownCheck(SFT_MotorEventBmpType evt, int32_t i32TachValue);
static bool_t PM_SFT_WM_FSM_IsTachOutOfRange(int32_t i32TachValue);
static bool_t PM_SFT_WM_FSM_IsDeltaTachOutOfRange(int32_t i32DeltaTachValue);
static int32_t PM_SFT_WM_FSM_DeltaTachEval(int32_t i32NewTachValue);
static void PM_SFT_WM_VirtualStop(void);
/*SoftStart FSM*/
static SFT_MotorEventBmpType PM_SFT_WM_SoftStartFSM(SFT_MotorEventBmpType evt);
static void PM_SFT_WM_SoftStart_ResetFSM(void);
static SFT_MotorEventBmpType PM_SFT_WM_FSM_ACMotorMonitor(SFT_MotorEventBmpType evt);
static SFT_MotorEventBmpType PM_SFT_WM_FSM_TachMonitor(SFT_MotorEventBmpType evt);
static void PM_SFT_WM_FSM_TachMonitorResetFSM(void);
/*consistency FSM */
static void PM_SFT_WM_ConsistencyFSM_Resync(void);
static SFT_MotorEventBmpType PM_SFT_WM_ConsistencyFSM(SFT_MotorEventBmpType evt, uint8_t consistency);
static SFT_MotorEventBmpType PM_SFT_WM_FbkGenericErrFSM(SFT_MotorEventBmpType evt);
static SFT_MotorEventBmpType PM_SFT_WM_FbkRelTriacSCErrFSM(SFT_MotorEventBmpType evt);
static bool_t PM_SFT_WM_ConsistencyEnable(void);
static bool_t PM_SFT_WM_ACMotorKeepProtection(bool8_t bKeep);
#endif
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */



/**
\n This function initializes the status of the safety and
\n the safety module of motor module.
\n Called at cold reset.
\n access:

\param [in,out]	pSafetyData safety status

\author		Roberto Fioravanti
\date		28/04/2014
\version

\par Architecture Requirements ID:
\n\li SARS_XTM_MN_SMI_1

\n\li SARS_XCM_MN_SCM_2
\n\li SARS_XCM_MN_SCM_3
\n\li SARS_XCM_MN_SCM_4

*/
void PM_SFT_WM_Motor_Init(sft_module_status_t*  pSafetyStatus)
{
	SAFETY_REGION_START();

	/* init the global variable of the safety module */
	PM_SFT_WM_Motor_InitState();

	/* start monitor timer */
	Tools_StartTimer(&(MotorData.monitorTimer));


	/* overwrite the safety module fault code */
	MotorData.prot.uCode = 0x01;
	MotorData.prot.uSubCode = 0x06;

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	if(IS_MOTOR_3PH)
	{
		/* overwrite the state to start with comm error */
		MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1);
		/* Reset comm manager */
		PM_SFT_WM_ResetCommManager();
	}
#endif

	SAFETY_DATA_UPDATE(MotorData);

#if defined(CONFIG_PM_MOTORGATE)
	PM_SFT_WM_Motor_SlowDown.fsm_state = PM_SFT_WM_FSM_SLOWDOWN_INIT;
	PM_SFT_WM_Motor_SoftStart.fsm_state = PM_SFT_WM_FSM_SOFTSTART_INIT;

	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SoftStart );
	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SlowDown );
	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_Consistency );
	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_TachMonitor );
#endif
	SAFETY_REGION_END();

	/* init state */
	pSafetyStatus->status = SFT_MODULE_MONITORING;
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	if(IS_MOTOR_3PH)
	{
		pSafetyStatus->faultSrc = FAULTSRC_WM_EXTERNAL_BOARD_KO;
	}
#endif
#if defined(CONFIG_PM_MOTORGATE)
	if(!IS_MOTOR_3PH)
	{
		pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_KO;
	}
#endif
}

#if defined(CONFIG_PM_MOTORGATE)
/**
enables VirtualStop logic
 */
static void PM_SFT_WM_VirtualStop(void)
{
	if( !(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1)))
	{
		MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1);
		Tools_StartTimer(&(MotorData.timerVirtualStop));
		PM_SFT_WM_FSM_TachMonitorResetFSM();
	}
	else
	{
		/* nothing to do */
	}
}
#endif

/**
\n This function implements the safety of the Motor
\n module.
\n It must be called within a task

\param [in]		newLoads	not used
\param [out]	pSafetyStatus	status
\param [in]		applReqFlags	not used

\return

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note	Current states managed by the monitor
\n this monitor manages the following states:
\li SFT_MODULE_IDLE, normal
\li SFT_MODULE_RESET
\li SFT_MODULE_MONITORING, init state
\li SFT_MODULE_WARNING, F18.x
\li SFT_MODULE_PROTECTION, external board fault


\par Architecture Requirements ID:
\n\li SARS_XTM_MN_SMI_1

\n\li SARS_XCM_MN_SCM_1
\n\li SARS_XCM_MN_SCM_2
\n\li SARS_XCM_MN_SCM_3
\n\li SARS_XCM_MN_SCM_4
\n\li SARS_XCM_MN_SCM_5
\n\li SARS_XCM_MN_SCM_6
\n\li SARS_XCM_MN_SCM_7
\n\li SARS_XCM_MN_SCM_8
*/
void PM_SFT_WM_Motor_Monitor(iom_loads_struct_t *pNewLoads, sft_module_status_t *pSafetyStatus, uint8_t applReqFlags)
{
	SFT_MotorEventBmpType event = 0; /* clear event bitmap */
	uint8_t loadPos=LOAD_NOT_FOUND;
	uint8_t doorLockPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

	LIB_SFT_EnterSafetyModule(pSafetyStatus);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( MotorData );

	/* eval zero crossing */
	if( PM_SFT_WM_MATCH_ZC_GOOD( goodZCSignal() ) )
	{
		event |= BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1); /* assumed door locked */
	}
	else
	{
		event |= BFMASK(SFT_MOTOR_EVT_NO_ZC,1);
	}

	/* door lock request */
	if(LOAD_REG_ON == LIB_IOM_GetLoadStatus(doorLockPos))
	{
		event |= BFMASK(SFT_MOTOR_EVT_LOCK_DOOR_REQ,1);
	}

	/* motor */
	loadPos = LIB_IOM_GetLoadPosByExtdLoadItem(0);
	if(pNewLoads->loads_ovr[loadPos] != LOAD_REG_OFF)
	{
		if(0 != PM_SFT_WM_GET_MOTOR_SPEED_REQ((MotorCmdType*)(pNewLoads->extdloads_req)))
		{
			event |= BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1);
#ifdef PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT
			if(pos_in == 0)
			{
				pos_in=1;
				pos = 0;
				memset(EventBuff, 0, sizeof(EventBuff));
				startTime = OS_GetTime();
			}
#endif
		}else if(pSafetyStatus->status == SFT_MODULE_IDLE)
		{
#ifdef PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT
			pos_in = 0;
#endif
		}

	}
	else
	{
		event |= BFMASK(SFT_MOTOR_EVT_MOTOR_OVR_OFF,1);
	}

#if defined(CONFIG_PM_MOTORGATE)
	if(!IS_MOTOR_3PH)			/* only one fault source */
	{
		/* tach monitor, consistency */
		event |= PM_SFT_WM_FSM_ACMotorMonitor(event);

		/* check for invalid param */
		if(event & BFMASK(SFT_MOTOR_EVT_INVALID_SETTING_PARAMS,1))
		{
			pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_KO;
			pSafetyStatus->status = SFT_MODULE_FAULT;
		}
	}
#endif
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	if(IS_MOTOR_3PH)
	{
		if(MotorData.bCommActive)
		{
			/* communication not suspended , then process data retrieved from external board  */
			PM_SFT_WM_ProcessExternMotorInfo();
		}

		/* ZC or motor process comm */
		if((event & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1)) || (MotorData.iDrumSpeed) )
		{
			PM_SFT_WM_Motor_CommProt(MotorData.state);
		}
		else
		{
			/* As no ZC and motor speed is zero then
			 * reset comm protection state machine
			 * */
			MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_IDLE;
			MotorData.state &= ~(BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID, 1));
		}

		/* comm manager always on */
		PM_SFT_WM_Motor_CommManager();

		/* iff comm no good, then signal comm fault */
		if( !PM_SFT_WM_CommunicationWithExternalBoardIsGood() )
		{
			event |= BFMASK(SFT_MOTOR_EVT_COMM_FAULT,1);
		}

		/* external board fault, then signal fault */
		if(PM_SFT_WM_ExternalBoardIsFault())
		{
			event |= BFMASK(SFT_MOTOR_EVT_EXTERNAL_FAULT,1);
		}
	}
#endif /* defined(CONFIG_EXTERN_MODULE_L_SUPPORT)*/


	if (MotorData.iDrumSpeed)
	{
		/* if running start speedFilter timer to filter 0 rpm */
		Tools_StartTimer(&(MotorData.speedFilter));
	}

	/* State machine for the safety module state */
	switch (pSafetyStatus->status)
	{
		case SFT_MODULE_WARNING:
		case SFT_MODULE_PROTECTION:
		{
			bool_t bWarning = PM_SFT_WM_SwitchToWarning(event)?TRUE:FALSE;
			bool_t bProtection =  PM_SFT_WM_SwitchToProtection(event)?TRUE:FALSE;


			/* protection has an higher priority than warning:
			 * protection overwrites the status
			 * pSafetyStatus->status
			 * */
			if(bWarning)
			{
				pSafetyStatus->status = SFT_MODULE_WARNING;
			}
			if(bProtection)
			{
				pSafetyStatus->status = SFT_MODULE_PROTECTION;
			}

#if defined(CONFIG_PM_MOTORGATE)
			if(!IS_MOTOR_3PH)			/* only one fault source */
			{
				pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_KO;

				if(event & (BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1)))
				{
					if(event & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1))
					{
					/*	Start timeout virtual stop if tach KO and fbk is faulty */
					PM_SFT_WM_VirtualStop();
					}
					else
					{
						/* nothing to do */
					}
				}

				/* by default , wait for motor to stop */
				if(event & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1) )
				{
					/* really needed only if tach is not faulty */
					MotorData.state |= BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1);
				}
			}
#endif /* defined(CONFIG_PM_MOTORGATE) */

			/* if all is fine update the fault source */
			if( FALSE == ( bProtection || bWarning ) )
			{
				/* no protection or warning required, then back to idle status */
				pSafetyStatus->status = SFT_MODULE_IDLE;
				pSafetyStatus->faultSrc = SFT_NO_FAULT_SRC;

				/* reset fault and sub code*/
				MotorData.prot.uCode=0;
				MotorData.prot.uSubCode=0;

#if defined(CONFIG_PM_MOTORGATE)
				if(!IS_MOTOR_3PH)
				{
					/* reset errors of motor control module*/
					PM_UM_ResetPhaseAngleMinAtSoftStart();
					/* ensuring no tach @ soft start is reset */
					MotorData.state &= ~(BFMASK(PM_SFT_WM_MOTOR_NOTACH_SSTART,1));
				}

#endif /* defined(CONFIG_PM_MOTORGATE) */
			}

#if defined(CONFIG_PM_MOTORGATE)
			if(!IS_MOTOR_3PH)
			{
				SFT_MotorEventBmpType evt= (event & (BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1)|BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1)));

				/* force protection */
				if(FALSE == PM_SFT_WM_ACMotorKeepProtection( (bool8_t)((evt == (BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1)|BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1)) ))))
				{
					PM_UM_ResetPhaseAngleMinAtSoftStart();
					MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_FORCE_FBK_PROT,1);
					MotorData.state &= ~BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1);
				}
			}
#endif /* defined(CONFIG_PM_MOTORGATE) */

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
			/* monitor timer to unlock door when data are not reliable */
			if(IS_MOTOR_3PH)
			{
				if( !(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,1))
					&& Tools_IsTimerElapsed(&(MotorData.unlockDL), 1000*CONFIG_PM_SAFETY_UNLOCK_DOOR_TIMEOUT))
				{
					MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,1);
					MotorData.iDrumSpeed=0; /* forcing speed @ zero in order to assume motor stopped */
				}
			}
#endif	/* defined(CONFIG_EXTERN_MODULE_L_SUPPORT) */


			/* current state */
			event |= (BFMASK(SFT_MOTOR_EVT_PROT,1));
		}
			break;


		case SFT_MODULE_IDLE:
		case SFT_MODULE_RESET:
		{
			bool_t bProtection =  PM_SFT_WM_SwitchToProtection(event);
			bool_t bWarning =  PM_SFT_WM_SwitchToWarning(event);
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
			if ((IS_MOTOR_3PH) && (FALSE != bWarning))
			{
				/* communication with external board missing */
				pSafetyStatus->status = SFT_MODULE_WARNING;
				pSafetyStatus->faultSrc = FAULTSRC_WM_EXTERNAL_BOARD_KO;
			}
#endif

			/* protection overwrites warning */
			if(FALSE != bProtection)
			{
				/* fault by external board  */
				pSafetyStatus->status = SFT_MODULE_PROTECTION;
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
				if(IS_MOTOR_3PH)
				{
					pSafetyStatus->faultSrc = FAULTSRC_WM_EXTERNAL_BOARD_KO;
				}
#endif
#if defined(CONFIG_PM_MOTORGATE)
				if(!IS_MOTOR_3PH)
				{
					pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_KO;
				}
#endif
			}

#if defined(CONFIG_PM_MOTORGATE)
			if(!IS_MOTOR_3PH)
			{
				if(event & (BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1)))
				{
					/*Start timeout virtual stop*/
					if(!(event & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1)) || (event & BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_DETECTED,1)))
					{
						PM_SFT_WM_VirtualStop();
					}

					if(event & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1))
					{
						/* Soft start */
						MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_NOTACH_SSTART,1);
					}
				}

				/* by default, wait for motor to stop */
				if(event & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1) )
				{
					/* really needed only if tach is not faulty */
					MotorData.state |= BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1);
				}

				if (FALSE == ( bProtection || bWarning ))
				{
					if(event & (BFMASK(SFT_MOTOR_EVT_REL_MOTOR_PREFAULT_REQ,1)))
					{
						pSafetyStatus->status = SFT_MODULE_PREFAULT;
						pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_REL_KO;

						Tools_StartTimer(&(MotorData.timer2));
					}
					else
					{
						PM_SFT_WM_ACMotorKeepProtection(FALSE);
					}
				}
			}
#endif /* defined(CONFIG_PM_MOTORGATE) */

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
			if(IS_MOTOR_3PH)
			{
				/* 3ph: Restart timer unlock Door only if not external fault
				*/
				if ((MotorData.state & BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_HAPPENED,1)) == 0)
				{
					Tools_StartTimer(&(MotorData.unlockDL));
					MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,1);
				}
			}
#endif
			event |= (BFMASK(SFT_MOTOR_EVT_IDLE,1));
		}
			break;

		case SFT_MODULE_MONITORING:
		{
			/* Initial state after cold reset.
			 * Safety module cannot rely on data as they have
			 * not yet retrieved at least once after initialization
			 */
			static bool8_t	bReady = 1;
			bool_t bProtection = 0;

			event |= (BFMASK(SFT_MOTOR_EVT_MONIT,1));

#if defined(CONFIG_PM_MOTORGATE)
			if(!IS_MOTOR_3PH)
			{
				/* evaluate if it OK for 3ph motor too */
				bProtection = PM_SFT_WM_SwitchToProtection(event);
			}
#endif
			if(bReady == 1)
			{
				if( Tools_IsTimerElapsed(&(MotorData.monitorTimer), 6000) )
				{
					bReady = 3;
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
					if ((IS_MOTOR_3PH) && ( event & BFMASK(SFT_MOTOR_EVT_NO_ZC,1) ))
					{
						/* no comm in the meanwhile reset no comm*/
						MotorData.commProtStep = PM_SFT_WM_MOTOR_COMMPROT_IDLE;
					}
#endif
				}else
				{
					/* ZC */
#if defined(CONFIG_PM_MOTORGATE)
					if((!IS_MOTOR_3PH) && (MotorData.iDrumSpeed))
					{
						bReady = 3;
					}
#endif /* */

#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
					if (IS_MOTOR_3PH)
					{
						if(MotorData.commProtStep == PM_SFT_WM_MOTOR_COMMPROT_IDLE)
						{
							/* communication is OK, then restart timer  */
							Tools_StartTimer(&(MotorData.monitorTimer));
							/* and wait for reliable data */
							bReady = 2;
						}else
						{
							/* 3ph - comm still KO */
							if(Tools_IsTimerElapsed(&(MotorData.monitorTimer), 4000) )
							{
								/* timeout expired */
								if(event & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1))
								{
									/* comm is expected OK but is not , force no communication */
									MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1);
								}
								bReady = 3;
							}
						}
					}
#endif
				}
			}
			else if (bReady == 2)
			{
				 if ( MotorData.iDrumSpeed || (Tools_IsTimerElapsed(&(MotorData.monitorTimer), 6000)))
				 {
					 bReady = 3;
				 }
			}

			if( (bReady == 3) || (bProtection))
			{
				Tools_DisableTimeout(&(MotorData.monitorTimer));
				/* door locked is assumed */
			}

			if(bProtection)
			{
				pSafetyStatus->status = SFT_MODULE_PROTECTION;
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
				if(IS_MOTOR_3PH)
				{
					pSafetyStatus->faultSrc = FAULTSRC_WM_EXTERNAL_BOARD_KO;
				}
#endif
#if defined(CONFIG_PM_MOTORGATE)
				if(!IS_MOTOR_3PH)
				{
					pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_KO;
				}
#endif
			}
			else if(bReady == 3)
			{
				pSafetyStatus->status = SFT_MODULE_IDLE;
				pSafetyStatus->faultSrc = SFT_NO_FAULT_SRC;
			}

#if defined(CONFIG_PM_MOTORGATE)
			if ((!IS_MOTOR_3PH) && (event & (BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1))))
			{
				/*Start timeout virtual stop*/
				if(!(event & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1)))
				{
					MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1);
					Tools_StartTimer(&(MotorData.timerVirtualStop));
					PM_SFT_WM_FSM_TachMonitorResetFSM();
				}
				else
				{
					/* Soft start */
					MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_NOTACH_SSTART,1);

					if(event & BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_DETECTED,1))
					{
						/*	Start timeout virtual stop if
						 * 	- virtual stop is not already set AND 'no tacho @ soft start' is not signaled
						 * 	*/
						if( !(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1)))
						{
							MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1);
							Tools_StartTimer(&(MotorData.timerVirtualStop));
							PM_SFT_WM_FSM_TachMonitorResetFSM();
						}
						else
						{
							/* nothing to do */
						}
					}
				}
			}
#endif
		}
			break;
		case SFT_MODULE_PREFAULT:
#if defined(CONFIG_PM_MOTORGATE)
			if (!IS_MOTOR_3PH)
			{
				if( !((event & BFMASK(SFT_MOTOR_EVT_REL_FAULT,1)) || (MotorData.state & BFMASK(PM_SFT_WM_MOTOR_FORCE_REL_SC_PROT,1))))
				{
					pSafetyStatus->status = SFT_MODULE_IDLE;
					pSafetyStatus->faultSrc = SFT_NO_FAULT_SRC;
				}
				else if(Tools_IsTimerElapsed(&(MotorData.timer2), 1000*PM_SFT_WM_GET_REL_FAULT_FILTER_TIME((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data)))
				{
					pSafetyStatus->status = SFT_MODULE_FAULT;
					pSafetyStatus->faultSrc = FAULTSRC_WM_ACMOTOR_REL_KO;
				}
			}
#endif /*defined(CONFIG_PM_MOTORGATE)*/
			break;

		case SFT_MODULE_FAULT:
			/* Handle reset fault request */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				pSafetyStatus->status = SFT_MODULE_IDLE;
				pSafetyStatus->faultSrc = SFT_NO_FAULT_SRC;
			}
			break;

		default:
			/* unexpected condition
			 * or
			 * safety manager configured this safety module in fault
			 * */
			;
	}

	SAFETY_DATA_UPDATE( MotorData );
	SAFETY_REGION_END();

#if defined(CONFIG_PM_MOTORGATE)
	/* transition to IDLE from any other state */
			if (!IS_MOTOR_3PH)
			{
				if( !(event & (BFMASK(SFT_MOTOR_EVT_IDLE,1))) && (pSafetyStatus->status == SFT_MODULE_IDLE))
				{
					SAFETY_REGION_START();
					SAFETY_DATA_CHECK( PM_SFT_WM_Motor_Consistency );
					PM_SFT_WM_ConsistencyFSM_Resync();
					SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_Consistency );
					SAFETY_REGION_END();
				}
			}
#endif



	/* update Event */
	Events = event;

#ifdef PM_SFT_WM_DEBUG_EVT_MONITOR_SUPPORT
	{
		int i;
		SFT_MotorEventBmpType msk=1;
		for (i=0; i<SFT_MOTOR_EVT_NUM; i++)
		{
			if(event & msk)
			{
				MonitorCnt[i]++;
			}
			msk <<= 1;
		}
		msk=1;

		{
			if(((pos>0) && ((EventBuff[pos-1].eventbmp != (event & ~EventMsk)) || (EventBuff[pos-1].state != (MotorData.state & ~StateMsk)))) || (pos ==0))
			{
				if(pos < (int)(sizeof(EventBuff)/sizeof(EventBuff[0])))
				{
					OS_TIME	time = OS_GetTime();
					EventBuff[pos].eventbmp=event & ~EventMsk;
					EventBuff[pos].state = MotorData.state & ~StateMsk;
					EventBuff[pos].time = time -startTime;
					startTime = time;
				}
				pos++;
			}
		}

		for (i=0; i<PM_SFT_WM_STATES_NUM; i++)
		{
			if(MotorData.state & msk)
			{
				StatesMonitorCnt[i]++;
			}
			msk <<= 1;
		}
	}
#endif

	LIB_SFT_LeaveSafetyModule(pSafetyStatus);

}
#if defined(CONFIG_PM_MOTORGATE)
/**
\retval TRUE, protection kept active
\retval FALSE protection cancelled
 */
static bool_t PM_SFT_WM_ACMotorKeepProtection(bool8_t bKeep)
{
	bool_t res = TRUE;

	if(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1))
	{
		/* Keep pSafetyStatus in SFT_MODULE_PROTECTION if virtual stop required */
		if(Tools_IsTimerElapsed(&(MotorData.timerVirtualStop), 1000*CONFIG_PM_SAFETY_UNLOCK_DOOR_TIMEOUT) && (!bKeep))
		{
			MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1);
			MotorData.iDrumSpeed=0; /* let's assume stopped */
			res = FALSE;
		}
	}else
	{
		if((!MotorData.iDrumSpeed) && Tools_IsTimerElapsed(&(MotorData.speedFilter), 3000))
		{
			/* deactivate */
			MotorData.state &= ~BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1);
		}
	}
	return res;
}
#endif
/*
Returns Custom faults
\return whether data has been changed
*/
bool_t PM_SFT_WM_MotorGetFault (uint8_t *pfaultCode, uint8_t *pfaultSubcode)
{
	if(pfaultCode && pfaultSubcode)
	{
		*pfaultCode=MotorData.prot.uCode;
		*pfaultSubcode=MotorData.prot.uSubCode;
		return TRUE;
	}

	return FALSE;
}

/**
\n Retrieves active protections
\n

\param [out]	pfaultCode	not used
\param [out]	pfaultSubcode	not used


\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
void PM_SFT_WM_MotorGetActiveProtection(uint8_t *pfaultCode, uint8_t *pfaultSubcode)
{
	PM_SFT_WM_MotorGetFault (pfaultCode, pfaultSubcode);
}


/*@*/

/**
\n retrieves current drum speed value.
\n
\param		[out] pSpeedValue, reliable functions returns FALSE

\return		bool_t
\retvalue 	TRUE	error
\retvalue 	FALSE	no error

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
bool_t PM_SFT_WM_GetDrumSpeed(DrumSpeedType * pSpeedValue)
{
	bool_t res = TRUE;
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	PM_SFT_WM_StateType msk =   BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1)|BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1)|BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_HAPPENED,1)|BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,1);

	if (IS_MOTOR_3PH)
	{
		res= (((!((MotorData.commProtStep==PM_SFT_WM_MOTOR_COMMPROT_FAULT_FILTER)||(MotorData.commProtStep==PM_SFT_WM_MOTOR_COMMPROT_IDLE))) || (MotorData.state & ~(msk)))?TRUE:FALSE);
	}
#endif
#if defined(CONFIG_PM_MOTORGATE)
	if ( !IS_MOTOR_3PH )
	{
		res= (PM_SFT_WM_FSM_IsVirtualStopActive() || PM_SFT_WM_FSM_IsNoTachoAtSoftStart());
	}
#endif

	if(pSpeedValue)
	{
		*pSpeedValue=MotorData.iDrumSpeed;
	}
	return res;
}



/**
\n returns whether drum speed is zero
\n

\return		bool_t
\retvalue 	TRUE		speed is zero
\retvalue 	FALSE 		speed not zero or error (there is a issue on integrity or communication )

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
bool_t PM_SFT_WM_DrumSpeedIsZero(void)
{
	bool_t retVal = FALSE;

	/* Motor 3ph */
	bool_t state;
	DrumSpeedType vCurrent = 0;

	state = PM_SFT_WM_GetDrumSpeed(&vCurrent);

	if(state == FALSE)
	{
		if( (0 == vCurrent))
		{
			if (Tools_IsTimerElapsed(&(MotorData.speedFilter), 3000))
			{
				retVal = TRUE;
			}
		}
	}
	else
	{
		/* not reliable */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
		if (IS_MOTOR_3PH)
		{
			/*  */
			if(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_TIMEOUTED,1))
			{
				/* timeout expired */
				retVal = TRUE;
			}
		}
#endif /* CONFIG_EXTERN_MODULE_L_SUPPORT */
#if defined(CONFIG_PM_MOTORGATE)
		if (!IS_MOTOR_3PH)
		{
			/* not reliable means not sure motor is stopped */
			retVal = FALSE;
		}
#endif
	}

	return retVal;
}


#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
/**
@name Force communication safety action related functions
@{
*/

/**
\n forces communication
\n

\return		bool_t
\retvalue 	TRUE		accepted
\retvalue 	FALSE		ignored because another request is ongoing and communication will be forced in a few moments

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
void PM_SFT_WM_RestoreCommunicationWithExternalBoard(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( MotorData );
	MotorData.bRestoreCommReq=TRUE;
	SAFETY_DATA_UPDATE( MotorData );
	SAFETY_REGION_END();
}
/*@}*/

/** indicates whether External board Communication Manager is in idle state*/
bool_t PM_SFT_WM_Motor_CommManagerIsIdle(void)
{
	return(bool_t)(MotorData.commStep == PM_SFT_WM_MOTOR_COMM_IDLE);
}

/**
@name Suspend communication safety action related functions
@{
*/
/**
	Requests disabling communication with external board because
	a fault has been issued by external board
*/
bool_t PM_SFT_WM_Motor_DisableTmpComm(void)
{
	bool_t res = TRUE;	/* TRUE, meaning the request is accepted by default*/

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( MotorData );

	/* then issue the request */
	MotorData.bSuspendCommReq=TRUE;

	SAFETY_DATA_UPDATE( MotorData );
	SAFETY_REGION_END();

	return res;
}


/** Indicates whether communication has been restored
*/
bool_t PM_SFT_WM_Motor_RestoredComm(void)
{
	return ((MotorData.bCommActive>1) && (MotorData.bSuspendCommReq==FALSE));
}

/*@}*/
#endif



#if defined(CONFIG_PM_MOTORGATE)
/*
Safety interface for tach
@{
 */
bool_t PM_SFT_WM_FSM_GetIsTachOutOfRange(void)
{
	return (bool_t) (PM_SFT_WM_Motor_SlowDown.evt & BFMASK(SFT_MOTOR_EVT_TACH_OUT_OF_RANGE,1))?TRUE:FALSE;
}

bool_t PM_SFT_WM_FSM_GetIsSlowDownKO(void)
{
	return (bool_t) (PM_SFT_WM_Motor_SlowDown.evt & BFMASK(SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO,1))?TRUE:FALSE;
}

bool_t PM_SFT_WM_FSM_IsTachReady(void)
{
	return (bool_t) (MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_READY,1))?TRUE:FALSE;
}


bool_t PM_SFT_WM_FSM_IsVirtualStopActive(void)
{
	return (bool_t) ( (MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1)) )?TRUE:FALSE;
}

bool_t PM_SFT_WM_FSM_IsNoTachoAtSoftStart(void)
{
	return (bool_t) (MotorData.state & BFMASK(PM_SFT_WM_MOTOR_NOTACH_SSTART,1))?TRUE:FALSE;
}
/*@}*/
#endif


/* Local Function */


#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)

/**
\n Retrieves whether communication with external board is working fine or not
\n

\return		bool_t
\retvalue 	TRUE		communication with external board is good
\retvalue 	FALSE		communication with external board not working

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
static bool_t PM_SFT_WM_CommunicationWithExternalBoardIsGood(void)
{
	return (!(MotorData.commProtStep==PM_SFT_WM_MOTOR_COMMPROT_NO_COMM));
}


/**
\n Retrieves last error
\n

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
static PM_SFT_WM_StateType PM_SFT_WM_GetMotorError(void)
{
	return MotorData.state;
}

/**
\n Retrieves whether communication with external board is working fine or not
\n

\return		bool_t
\retvalue 	TRUE		communication with external board is good
\retvalue 	FALSE		communication with external board not working

\author		Roberto Fioravanti
\date		22/02/2013
\version

\note
*/
static bool_t PM_SFT_WM_ExternalBoardIsFault(void)
{
	return (bool_t)(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT,1));
}



#endif

/**
 *
 */
static bool_t PM_SFT_WM_SwitchToWarning(SFT_MotorEventBmpType event)
{
	bool_t bFault=FALSE; /* don't switch to warning by default */

	/* unexpected ZC missing : check if last motor speed is not zero */
	if ( event & BFMASK(SFT_MOTOR_EVT_NO_ZC,1) )
	{
		if( MotorData.iDrumSpeed)
		{
			MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING, 1);
		}
		else
		{
			MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING, 1);
		}
	}

	if( event & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1) )
	{
		MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING, 1);
	}

	if(MotorData.state & (BFMASK(PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING,1)))
	{
		MotorData.prot.uCode = 0x01;
		MotorData.prot.uSubCode=0x05;
		bFault = TRUE;
	}else
	{
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
		if(event & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1))
		{
			if(event & BFMASK(SFT_MOTOR_EVT_COMM_FAULT,1))
			{
				/* 2 cases :
				 * - data corrupted (bit PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID has been set )
				 * - else no commm
				 * */
				if(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID,1))
				{
					MotorData.prot.uCode=18;
					MotorData.prot.uSubCode=2;
					bFault = TRUE;
				}
				else
				{
					MotorData.prot.uCode=18;
					MotorData.prot.uSubCode=1;
					bFault = TRUE;
				}
			}
			else
			{
				/* nothing to do */
			}
		}
#endif
#if defined(CONFIG_PM_MOTORGATE)
		/* nothing to do */
#endif
	}
	return bFault;
}

/**
 *
 */
static bool_t PM_SFT_WM_SwitchToProtection(SFT_MotorEventBmpType event)
{
	bool_t bFault=FALSE; /* don't switch to protection by default */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	if((event & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1)) || MotorData.iDrumSpeed)
	{
		if(event & BFMASK(SFT_MOTOR_EVT_EXTERNAL_FAULT,1))
		{
			if( (!PM_SFT_WM_Motor_RestoredComm()) )
			{
				/* comm suspended, then go to protection */
				bFault = TRUE;
				Tools_StartTimer(&MotorData.monitorTimer);
			}
			else
			{
				/* Force no fault */
				MotorData.fault.uCode = 0;
				MotorData.fault.uSubCode= 0;
				MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT,1);
			}

			MotorData.prot.uCode=MotorData.fault.uCode;
			MotorData.prot.uSubCode=MotorData.fault.uSubCode;
		}
	}
#endif
#if defined(CONFIG_PM_MOTORGATE)
	if((event & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1)) || (MotorData.state & BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1)))
	{
		bFault = TRUE;
		MotorData.prot.uCode=1;
		if(event & BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1))
		{
			MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_FORCE_FBK_PROT,1);
		}
		else
		{
			MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_FORCE_FBK_PROT,1);
		}

		if( event & BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1) )
		{
			MotorData.prot.uSubCode=0x03;
		}
		else if( event & BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1) )
		{
			MotorData.prot.uSubCode=0x02;
		}
		if( event & BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1) )
		{
			MotorData.prot.uSubCode=0x01;
		}
		else
		{
			/* nothing to do, keep last sub code set */
		}
	}
	else if(!(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_FORCE_FBK_PROT,1)) &&
			((event & BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1)) || (MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1))))
	{
		bFault = TRUE;
		MotorData.prot.uCode=2;
		MotorData.prot.uSubCode=1;
	}
	else
	{
		/* nothing to do */
	}
#endif
	return bFault;
}

/*
Extern Motor protections state machine
\param [in] event
It processes the following state bit:
 - PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM
 - PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION
 - PM_SFT_WM_MOTOR_NO_ERROR
It generates:
- PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID
*/
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
static void PM_SFT_WM_Motor_CommProt(PM_SFT_WM_StateType	state)
{

	static bool8_t	bCorruption = FALSE;
	/* filter events */
	state &= (BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1)|BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1));

	switch(MotorData.commProtStep)
	{
		case PM_SFT_WM_MOTOR_COMMPROT_IDLE:

			bCorruption = FALSE;
			if(state)
			{
				MotorData.commProtStep = PM_SFT_WM_MOTOR_COMMPROT_FAULT_FILTER;
				if(state & (BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1)))
				{
					bCorruption = TRUE;
				}

				Tools_SetTimeout(&(MotorData.timer2), 3000);
			}
			else
			{
				/* No Error, clear  */
				MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID,1);
			}

			break;

		case PM_SFT_WM_MOTOR_COMMPROT_FAULT_FILTER:
			if(0 == state)
			{
				/* no event, means no error, so in the meanwhile communication has been restored*/
				MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_IDLE;
				MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID,1);
			}else
			{
				/* If communication go back active and data corruption occurs */
				if (state == BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1))
				{
					bCorruption = TRUE;
				}
				/* Communication lost! */
				else if (state == BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1))
				{
					bCorruption = FALSE;
				}
				else
				{
					/* Keep bCorruption as is now - nothing to do */
				}

				if(Tools_IsTimeoutExpired(&(MotorData.timer2)))
				{
					MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_NO_COMM;
				}

			}
			break;
		case PM_SFT_WM_MOTOR_COMMPROT_NO_COMM:
			if(0 == state)
			{
				/* no event, means no error, so in the meanwhile communication has been restored: thanks to attempts made by application*/
				MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_IDLE;
			}
			else
			{
				if(bCorruption == TRUE)
				{
					MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION_VALID,1);
				}
			}

			break;

		default:
			MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_IDLE;
	}

}
#endif

/*
External board Communication manager state machine for safety actions.
Manages requests of suspension and forcing of communication.
*/
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
static void PM_SFT_WM_Motor_CommManager(void)
{

	switch(MotorData.commStep)
	{
		case PM_SFT_WM_MOTOR_COMM_IDLE:

			MotorData.bCommActive=1;
			if(MotorData.bRestoreCommReq)
			{
				MotorData.commStep=PM_SFT_WM_MOTOR_COMMSUSPEND_COMM;
			}
			else if(MotorData.bSuspendCommReq)
			{
				MotorData.commStep=PM_SFT_WM_MOTOR_COMMSUSPEND_COMM;
				Tools_SetTimeout(&(MotorData.timer), 1000);
			}
			else
			{
				break;
			}
			/* -fallthrough */
		case PM_SFT_WM_MOTOR_COMMSUSPEND_COMM:
			if(MotorData.bSuspendCommReq)
			{
				if(Tools_IsTimeoutExpired(&(MotorData.timer)))
				{
					PM_SFT_WM_ResetCommManager();
					MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_IDLE;
				}
				else
				{
					MotorData.bCommActive=FALSE;
					CP_UART_DisableComm();
				}
			}
			else if (MotorData.bRestoreCommReq)
			{
				PM_SFT_WM_ResetCommManager();
			}

			break;
	}
}


static void PM_SFT_WM_ResetCommManager(void)
{
	MotorData.commStep=PM_SFT_WM_MOTOR_COMM_IDLE;
	MotorData.bCommActive+=2;
	MotorData.bSuspendCommReq=FALSE;
	MotorData.bRestoreCommReq=FALSE;
	CP_UART_EnableComm();
}
#endif

/**
Process info buffer
*/
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)

static void PM_SFT_WM_ProcessExternMotorInfo(void)
{
	PM_SFT_WM_InfoExternMotorType * pInfoBuffer= (PM_SFT_WM_InfoExternMotorType*)ExtMotor_InfoBuffer;
#if defined(SFT_MOTOR_PROTO_ARCADIA3)
	uint16_t Chk;
	uint16_t ebSignature;			/* external board checksum*/
#else
	LIB_ModifiedChkType Chk;
	LIB_ModifiedChkType ebSignature;			/* external board checksum*/
#endif
	InfoCntType ebSftCnt;						/* current external board safety counter value*/


	UseExternalInfo();

	/*  First of all process safety counter to catch no comm */
	ebSftCnt = PM_SFT_WM_GET_EXTMOTOR_SFTCNT(pInfoBuffer);	/* read safety counter*/

	/*check Safety counter: if not changed since last time return PM_SFT_WM_NO_NOT_UPDATED */
	if(PM_SFT_WM_MATCH_SFT_CNT(ebSftCnt == MotorData.nfoCntLastValue))
	{
		/* safety cnt is KO, no updates coming form external board */
		MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1);
	}
	else
	{
		/* safety cnt is OK, it is enough to suppose that new data from external board has been received  ... */
		MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_NO_COMM,1);

		/* ... then check integrity */
#if defined(SFT_MOTOR_PROTO_ARCADIA3)
		Chk = LIB_crc_calculate(6, (uint8_t*)pInfoBuffer);		/* modified chk computation */
#else
		Chk = LIB_ModifiedChk16_Computation(3, (uint16_t*)pInfoBuffer);		/* modified chk computation */
#endif
		ebSignature = PM_SFT_WM_GET_EXTMOTOR_CHKMOD(pInfoBuffer);			/*get signature form info buffer*/

		/* update safety counter value*/
		MotorData.nfoCntLastValue = ebSftCnt;

		/* check CHK value */
		if(!PM_SFT_WM_MATCH_SIGN(Chk == ebSignature))
		{
			/* integrity KO */
			MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1);
		}
		else
		{
			/* integrity OK...*/
			/* ... then remove data corruption signal and not ready*/
			MotorData.state &= ~(BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_DATACORRUPTION,1));
			/* ... then extract other info */

			/* get speed */
			MotorData.iDrumSpeed = PM_SFT_WM_GET_EXTMOTOR_SPEED(pInfoBuffer);
			/* get fault*/
			MotorData.fault.uCode = PM_SFT_WM_GET_EXTMOTOR_FAULT(pInfoBuffer);
			MotorData.fault.uSubCode = PM_SFT_WM_GET_EXTMOTOR_FAULTSUB(pInfoBuffer);

			/*MotorData.fault = WM_MotorDebug.fault;*/

			/* check external board fault */
			if((MotorData.fault.uCode != 0) || (MotorData.fault.uSubCode!=0) )
			{
				MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT,1) |
							BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_HAPPENED, 1);
			}
			else
			{
				MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT,1);

				if (Tools_IsTimerElapsed(&MotorData.monitorTimer, 3000))
				{
					MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_EXTERNAL_FAULT_HAPPENED,1);
				}
			}
		}
	}
	UnuseExternalInfo();	
}

#endif


#if defined(CONFIG_PM_MOTORGATE)
/**
Resets FSM relating monitor of tach (PM_SFT_WM_FSM_TachMonitor())
*/
static void PM_SFT_WM_FSM_TachMonitorResetFSM(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_SFT_WM_Motor_TachMonitor );

	PM_SFT_WM_Motor_TachMonitor.evt &= ~(BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1)|BFMASK(SFT_MOTOR_EVT_AUX2,1));

	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_TachMonitor );
	SAFETY_REGION_END();
}

/*
AC Motor monitor to detect missing tach while motor is running
\param evt	events
	\li in:SFT_MOTOR_EVT_DRUM_STOP,SFT_MOTOR_EVT_TACH_IN_RANGE
\return SFT_MotorEventBmpType output events
	\li SFT_MOTOR_EVT_MISSING_TACH

\attention global variable affected: PM_SFT_WM_Motor_TachMonitor

\par Architecture Requirements ID:
\n\li SARS_XCM_MN_SCM_5
\n\li SARS_XCM_MN_SCM_8
*/
static SFT_MotorEventBmpType PM_SFT_WM_FSM_TachMonitor(SFT_MotorEventBmpType evt)
{
	SFT_MotorEventBmpType outevt = 0;

	if(evt & BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1))
	{
		if(PM_SFT_WM_Motor_TachMonitor.evt & (BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1)|BFMASK(SFT_MOTOR_EVT_AUX2,1)))
		{
			outevt |= BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1)|BFMASK(SFT_MOTOR_EVT_AUX1,1);
		}
		else
		{
			/* nothing to do */
		}
	}
	else
	{
		if(evt & BFMASK(SFT_MOTOR_EVT_AUX2,1))
		{
			/* validating high speeds */
			if(Tools_IsTimeoutDisabled(&(PM_SFT_WM_Motor_TachMonitor.tachValid)))
			{
				if(! (PM_SFT_WM_Motor_TachMonitor.evt & BFMASK(SFT_MOTOR_EVT_AUX2,1)) )
				{
					Tools_SetTimeout(&(PM_SFT_WM_Motor_TachMonitor.tachValid), 150);
				}
			}
			else if(Tools_IsTimeoutExpired(&(PM_SFT_WM_Motor_TachMonitor.tachValid)))
			{
				PM_SFT_WM_Motor_TachMonitor.evt |= BFMASK(SFT_MOTOR_EVT_AUX2,1);
			}
		}
		else
		{
			/* do nothing */
		}
	}

	if(! (evt & BFMASK(SFT_MOTOR_EVT_AUX2,1)))
	{
		Tools_DisableTimeout(&(PM_SFT_WM_Motor_TachMonitor.tachValid));
		PM_SFT_WM_Motor_TachMonitor.evt &= ~BFMASK(SFT_MOTOR_EVT_AUX2,1);
	}

	if(evt & BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1))
	{
		PM_SFT_WM_Motor_TachMonitor.evt |= BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1);
	}
	else
	{
		/* nothing to do */
	}

	if( (evt & BFMASK(SFT_MOTOR_EVT_TACH_OUT_OF_RANGE,1)))
	{
		PM_SFT_WM_Motor_TachMonitor.evt &= ~BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1);
	}
	else
	{
		/* nothing to do */
	}

	return outevt;
}

/*
AC Motor monitor
\param evt	events
	\li in:
\return SFT_MotorEventBmpType output events
	\li out:SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART, SFT_MOTOR_EVT_MISSING_TACH

\attention global variable affected: MotorData

\par Architecture Requirements ID:
\n\li SARS_XCM_MN_SCM_1
\n\li SARS_XCM_MN_SCM_2
\n\li SARS_XCM_MN_SCM_3
\n\li SARS_XCM_MN_SCM_4
\n\li SARS_XCM_MN_SCM_5
\n\li SARS_XCM_MN_SCM_6
\n\li SARS_XCM_MN_SCM_7
\n\li SARS_XCM_MN_SCM_8
*/
static SFT_MotorEventBmpType PM_SFT_WM_FSM_ACMotorMonitor(SFT_MotorEventBmpType evt)
{
	int32_t	iTach = 0;
	uint8_t consistency = PM_CS_NOT_VALID_CODE;
	SFT_MotorEventBmpType outevt = 0;

	bool_t bDrumStop=FALSE;

	iTach = (int32_t)PM_UM_Tacho_GetValue();

	/* AC Motor stopped*/
	(void)PM_UM_getBasketStopped( &bDrumStop );

	if (MotorData.MotorPos != LOAD_NOT_FOUND )
	{
		if(PM_SFT_WM_MATCH_DRUM_STOP(bDrumStop))
		{
			outevt |= BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1);
		}
		else
		{
			/* process Slow down and tach in range */
			outevt |= PM_SFT_WM_FSM_SlowDownCheck(evt, iTach);	/* evt only required */

			if( iTach < 6000 )
			{
				/* high speed detection */
				outevt |= BFMASK(SFT_MOTOR_EVT_AUX2,1);
			}
			else
			{
				/* nothing to do */
			}
		}

		/* process missing tach */
		if( PM_SFT_WM_MATCH_MISSING_TACH_AT_SSTART(PM_UM_IsPhaseAngleMinAtSoftStart()) )
		{
			/* tach signal missing at soft start */
			outevt |= BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1);
		}else if( PM_SFT_WM_MATCH_MISSING_TACH (PM_UM_IsTimeoutTachometer()) )
		{
			/* tach signal missing after soft start */
			outevt |= BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1);
		}else
		{
			SAFETY_REGION_START();
			SAFETY_DATA_CHECK( PM_SFT_WM_Motor_TachMonitor );

			outevt |= PM_SFT_WM_FSM_TachMonitor(evt|outevt);

			SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_TachMonitor );
			SAFETY_REGION_END();
		}

		if(! (outevt & (BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1))) )
		{
			/* update drum speed only if no error */
			if(outevt & BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1))
			{
				MotorData.iDrumSpeed =0;
			}
			else
			{
				MotorData.iDrumSpeed = PM_UM_GetMotorSpeed();
			}
		}

		/* process soft start */
		outevt |= PM_SFT_WM_SoftStartFSM(evt|outevt); /* evt|outevt required */

		SAFETY_REGION_START();
		SAFETY_DATA_CHECK( PM_SFT_WM_Motor_Consistency );

		if(outevt & BFMASK(SFT_MOTOR_EVT_TACH_SLOW_DOWN_READY,1))
		{
			MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_TACH_READY,1);
		}
		else
		{
			MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_TACH_READY,1);
		}

		if((PM_CS_NO_ERROR == PM_Consistency_IsValueReady(MotorData.MotorPos, &consistency)) && (!PM_UM_IsSuspendedTriac()) )
		{
			outevt |= PM_SFT_WM_ConsistencyFSM(evt|outevt, consistency);
		}
		else
		{
			/* nothing to do */
		}

		SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_Consistency );
		SAFETY_REGION_END();
	}
	else
	{
		/* ERROR!!! MotorData.MotorPos = LOAD_NOT_FOUND */
	}
	return outevt;
}



/* SlowDownCheck */

/*
SlowDownCheck, drum speed slow down check fsm
\param [in] evt bitmap events ( SFT_MOTOR_EVT_ZC_GOOD)
\param [in] i32TachValue tacho value [us]

\return SFT_MotorEventBmpType bitmap events
\retval SFT_MOTOR_EVT_TACH_OUT_OF_RANGE  SFT_MOTOR_EVT_TACH_IN_RANGE  SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO

\attention global variable affected PM_SFT_WM_Motor_SlowDown

\par Architecture Requirements ID:
\n\li SARS_XCM_MN_SCM_6
\n\li SARS_XCM_MN_SCM_8
*/
static SFT_MotorEventBmpType PM_SFT_WM_FSM_SlowDownCheck(SFT_MotorEventBmpType evt, int32_t i32TachValue)
{

	SFT_MotorEventBmpType out = 0;
	Loader_Struct 	faultParam;


	faultParam.Data=0;
	faultParam.DataLenght = 0;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_SFT_WM_Motor_SlowDown );

	switch(PM_SFT_WM_Motor_SlowDown.fsm_state)
	{
		case PM_SFT_WM_FSM_SLOWDOWN_INIT:
		{
			if(SF_OK == BasicLoader((settingPntr_t)SF_PTR_SAFETY_PARAMETERS, (uint8_t)SF_DISPL_SFT_FAULT_ACTIONS, &faultParam))
			{

				PM_SFT_WM_Motor_SlowDown.TachTh = PM_SFT_WM_GET_TACH_MAX_VAL_TH((PM_SFT_WM_SFSafetyActionsParamType*)(faultParam.Data));
				PM_SFT_WM_Motor_SlowDown.DeltaTachTh = PM_SFT_WM_GET_TACH_DELTA((PM_SFT_WM_SFSafetyActionsParamType*)(faultParam.Data));
				PM_SFT_WM_Motor_SlowDown.Period = 100*PM_SFT_WM_GET_SPEED_TOUT((PM_SFT_WM_SFSafetyActionsParamType*)(faultParam.Data));

				/*transition */
				PM_SFT_WM_Motor_SlowDown.fsm_state = PM_SFT_WM_FSM_SLOWDOWN_WAIT_4_EVAL;
				Tools_SetTimeout(&(PM_SFT_WM_Motor_SlowDown.Tout), 0);
			}
			else
			{
				out |= BFMASK(SFT_MOTOR_EVT_INVALID_SETTING_PARAMS,1);
				break;
			}
		}
			/* -fallthrough */
		case PM_SFT_WM_FSM_SLOWDOWN_WAIT_4_EVAL:
			if (FALSE != PM_SFT_WM_FSM_IsTachOutOfRange(i32TachValue) )
			{
				Tools_SetTimeout(&(PM_SFT_WM_Motor_SlowDown.Tout), 0);
				out |= BFMASK(SFT_MOTOR_EVT_TACH_OUT_OF_RANGE,1)|BFMASK(SFT_MOTOR_EVT_TACH_SLOW_DOWN_READY,1);
			}
			else if( TRUE == Tools_IsTimeoutExpired(&(PM_SFT_WM_Motor_SlowDown.Tout)) )
			{
				/* it's time to evaluate delta tach */

				out |= BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1)|BFMASK(SFT_MOTOR_EVT_TACH_SLOW_DOWN_READY,1);
				if(PM_SFT_WM_FSM_IsDeltaTachOutOfRange(PM_SFT_WM_FSM_DeltaTachEval(i32TachValue))  )
				{
					/* here slow down KO!!! validated */
					out |= BFMASK(SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO,1);
				}
				else
				{
					/* here slow is down OK */
					/* nothing to do*/
				}

				PM_SFT_WM_Motor_SlowDown.TachValue=i32TachValue;
				Tools_SetTimeout(&(PM_SFT_WM_Motor_SlowDown.Tout), (OS_TIME)PM_SFT_WM_Motor_SlowDown.Period);
			}
			break;
		default :
			/* unexpected condition */
			PM_SFT_WM_Motor_SlowDown.fsm_state = PM_SFT_WM_FSM_SLOWDOWN_INIT;
	}


	PM_SFT_WM_Motor_SlowDown.evt=out;

	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SlowDown );
	SAFETY_REGION_END();

	return out;
}

static bool_t PM_SFT_WM_FSM_IsTachOutOfRange(int32_t i32TachValue)
{
	return (bool_t) ((i32TachValue>=PM_SFT_WM_Motor_SlowDown.TachTh) || (i32TachValue==0));
}

static bool_t PM_SFT_WM_FSM_IsDeltaTachOutOfRange(int32_t i32DeltaTachValue)
{
	return (bool_t) (i32DeltaTachValue<=PM_SFT_WM_Motor_SlowDown.DeltaTachTh)?TRUE:FALSE;
}



static int32_t PM_SFT_WM_FSM_DeltaTachEval(int32_t i32NewTachValue)
{
	return i32NewTachValue - PM_SFT_WM_Motor_SlowDown.TachValue;
}

/*
Restarts slowdown check
\attention global variable affected PM_SFT_WM_Motor_SlowDown
*/
void PM_SFT_WM_FSM_SlowDownCheckResync(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_SFT_WM_Motor_SlowDown );

	PM_SFT_WM_Motor_SlowDown.fsm_state = PM_SFT_WM_FSM_SLOWDOWN_INIT;

	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SlowDown );
	SAFETY_REGION_END();
}


/*
Soft Start Fault detection fsm
\attention global variable affected PM_SFT_WM_Motor_SoftStart
\param [in] evt bitmap events  SFT_MOTOR_EVT_ZC_GOOD, SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART, SFT_MOTOR_EVT_MISSING_TACH,
SFT_MOTOR_EVT_MOTOR_REQ_ON
\return SFT_MotorEventBmpType bitmap events
\retval SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ


\par Architecture Requirements ID:
\n\li SARS_XCM_MN_SCM_2
*/
static SFT_MotorEventBmpType PM_SFT_WM_SoftStartFSM(SFT_MotorEventBmpType evt)
{
	SFT_MotorEventBmpType out=0;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_SFT_WM_Motor_SoftStart );

	switch(PM_SFT_WM_Motor_SoftStart.fsm_state)
	{
		case PM_SFT_WM_FSM_SOFTSTART_INIT:
			/*transition */
			PM_SFT_WM_Motor_SoftStart.fsm_state = PM_SFT_WM_FSM_SOFTSTART_WAIT_4_NOTACH;
			/*break;*/
			/* -fallthrough */
		case PM_SFT_WM_FSM_SOFTSTART_WAIT_4_NOTACH:
			if(FALSE == (evt & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1)))
			{
				Tools_DisableTimeout(&(PM_SFT_WM_Motor_SoftStart.Tout));
			}
			else
			{
				if( (evt & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1)) ||
					( ((evt & (BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1))) == (BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1)|BFMASK(SFT_MOTOR_EVT_MISSING_TACH,1))))
					)
				{
						Tools_SetTimeout(&(PM_SFT_WM_Motor_SoftStart.Tout), (OS_TIME)1000*PM_SFT_WM_GET_PROTECTION_TIME((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data) );
						PM_SFT_WM_Motor_SoftStart.fsm_state = PM_SFT_WM_FSM_SOFTSTART_MOTOR_OFF_PAUSE;
						out |= BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1);
				}
			}

			break;
		case PM_SFT_WM_FSM_SOFTSTART_MOTOR_OFF_PAUSE:
			if(!(evt & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1)) || (Tools_IsTimeoutExpired(&(PM_SFT_WM_Motor_SoftStart.Tout))) )
			{
				/* NO_ZC_WHILE_RUNNING not taken into account */
				Tools_DisableTimeout(&(PM_SFT_WM_Motor_SoftStart.Tout));
				PM_SFT_WM_Motor_SoftStart.fsm_state = PM_SFT_WM_FSM_SOFTSTART_WAIT_4_NOTACH;
				if( evt & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1) )
				{
					PM_UM_ResetPhaseAngleMinAtSoftStart();
				}
			}
			else
			{
				out |= BFMASK(SFT_MOTOR_EVT_TACHKO_MOTOR_PROT_REQ,1);
			}
			break;
		default:
			/* End of FSM */
			;
	}

	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SoftStart );
	SAFETY_REGION_END();

	return out;
}

static void PM_SFT_WM_SoftStart_ResetFSM(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_SFT_WM_Motor_SoftStart );
	PM_SFT_WM_Motor_SoftStart.fsm_state = PM_SFT_WM_FSM_SOFTSTART_INIT;
	SAFETY_DATA_UPDATE( PM_SFT_WM_Motor_SoftStart );
	SAFETY_REGION_END();
}



/* Fault detection by Consistency check */
static bool_t PM_SFT_WM_ConsistencyEnable(void)
{
	uint16_t alpha = PM_UM_GetSecureAlpha();

	if(alpha < PM_SFT_WM_GET_PHASE_ANGLE_TH((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data) && (alpha!=0))
	{
		return FALSE;
	}
	return TRUE;
}


static void PM_SFT_WM_ConsistencyFSM_Resync(void)
{
	/* init state */
	PM_SFT_WM_Motor_Consistency.fsm_state = PM_SFT_WM_FSM_CONSISTENCY_INIT;
	/* reset event's bitmap */
	PM_SFT_WM_Motor_Consistency.evt=0;
}

/*
PM_SFT_WM_ConsistencyFSM, consistency fsm
\param [in] evt bitmap events ( SFT_MOTOR_EVT_ZC_GOOD)
\param [in] consistency consistency

\return SFT_MotorEventBmpType bitmap events
\retval SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ  SFT_MOTOR_EVT_REL_MOTOR_PREFAULT_REQ

\attention global variable affected PM_SFT_WM_Motor_SlowDown


\par Architecture Requirements ID:
\n\li SARS_XCM_MN_SCM_2
\n\li SARS_XCM_MN_SCM_3
\n\li SARS_XCM_MN_SCM_4
\n\li SARS_XCM_MN_SCM_7
*/
static SFT_MotorEventBmpType PM_SFT_WM_ConsistencyFSM(SFT_MotorEventBmpType evt, uint8_t u8Consistency)
{
	SFT_MotorEventBmpType tmp=0, preEvt=0 ;
	preEvt = PM_SFT_WM_Motor_Consistency.evt|evt;

#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
	if(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_THERMAL_PROT,1))
	{
		/* suspend triac @soft start */
		PM_UM_SoftStartSuspend(TRUE);
	}
#endif /* defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START) */

	switch(PM_SFT_WM_Motor_Consistency.fsm_state)
	{
		case PM_SFT_WM_FSM_CONSISTENCY_INIT:
			/*transition */
			PM_SFT_WM_Motor_Consistency.fsm_state = PM_SFT_WM_FSM_CONSISTENCY_WAIT_4_ZC_GOOD;
			PM_SFT_WM_Motor_Consistency.fsm_state_sc=PM_SFT_WM_FSM_SC_INIT;

			MotorData.state &=  ~BFMASK(PM_SFT_WM_MOTOR_FORCE_REL_SC_PROT,1);

			/*break;*/
			/* -fallthrough */
		case PM_SFT_WM_FSM_CONSISTENCY_WAIT_4_ZC_GOOD:

			if(evt & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1))
			{
				switch(u8Consistency & 0x07 /*CONSISTENCY_FAILURE_MASK*/)
				{
					case PM_CS_VALUE_OK:
						if( (u8Consistency & PM_CS_STATUS_ON /* CONSISTENCY_STATE_MASK*/ ) == (PM_CS_STATUS_OFF) )
						{
							/* OFF, OK */
							tmp |= BFMASK(SFT_MOTOR_EVT_FBK_MOTOR_OFF_OK,1);
							MotorData.state &= ~(BFMASK(PM_SFT_WM_MOTOR_FORCE_REL_SC_PROT,1));
						}
						else
						{
							/* ON, OK*/

							tmp |= BFMASK(SFT_MOTOR_EVT_FBK_MOTOR_ON_OK,1);
							/*
							 * cancel
							 * - thermal prot
							 * - */
							MotorData.state &= ~(BFMASK(PM_SFT_WM_MOTOR_TRIAC_SC_WARN,1)|BFMASK(PM_SFT_WM_MOTOR_THERMAL_PROT,1));
							Tools_DisableTimeout(&(PM_SFT_WM_Motor_ThermalProt.Tout));
#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
							/* restore triac @soft start */
							PM_UM_SoftStartSuspend(FALSE);
#endif /* defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START) */
						}
						break;

					case PM_CS_VALUE_ERR_GENERIC:
						tmp |= PM_SFT_WM_FbkGenericErrFSM(preEvt);
						break;

					case PM_CS_VALUE_ERR_SC:

						if((u8Consistency&PM_CS_STATUS_ON) == (PM_CS_STATUS_ON))
						{
							if((MotorData.state & BFMASK(PM_SFT_WM_MOTOR_THERMAL_PROT,1)) /* && (suspend active) */)
							{
								if( Tools_IsTimeoutDisabled(&(PM_SFT_WM_Motor_ThermalProt.Tout)) )
								{
									if(evt & BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1))
									{
										Tools_StartTimer(&(PM_SFT_WM_Motor_ThermalProt.Tout));
									}

								} else if(Tools_IsTimerElapsed(&(PM_SFT_WM_Motor_ThermalProt.Tout), 700))
								{
									Tools_DisableTimeout(&(PM_SFT_WM_Motor_ThermalProt.Tout));
									tmp |= BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1);
								}
							}

							/* triac motor is driven, triac in short circuit  */
							if(PM_SFT_WM_ConsistencyEnable() && (evt & BFMASK(SFT_MOTOR_EVT_MOTOR_REQ_ON,1)))
							{
								tmp |= PM_SFT_WM_FbkRelTriacSCErrFSM(preEvt);
							}

							tmp |= BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_DETECTED,1);

							if(!(evt & BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1)))
							{
								if(MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TRIAC_SC_WARN,1))
								{
									tmp |= BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1);
								}
							}

							MotorData.state &= ~BFMASK(PM_SFT_WM_MOTOR_TRIAC_SC_WARN,1);
						}
						else
						{
							/* triac motor is not driven, then relay getting stuck on 'close' */

							tmp |= (BFMASK(SFT_MOTOR_EVT_REL_FAULT,1));
							MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_FORCE_REL_SC_PROT,1);

							if((tmp & (BFMASK(SFT_MOTOR_EVT_REL_FAULT,1)) ) && !(preEvt & BFMASK(SFT_MOTOR_EVT_REL_FAULT,1)))
							{
								tmp |= BFMASK(SFT_MOTOR_EVT_REL_FAULT_INIT, 1);
							}
						}
						break;
					case PM_CS_VALUE_ERR_DM:
						if(PM_SFT_WM_ConsistencyEnable())
						{
							tmp |= BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1);

							if((preEvt & (BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1)) ) == 0 )
							{
								tmp |= BFMASK(SFT_MOTOR_EVT_FBK_ERR_INIT,1);
							}
							else
							{
								/* nothing to do */
							}
						}

						break;
					default:
						;
				}

				if( ((evt|tmp) & (BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1)|BFMASK(SFT_MOTOR_EVT_FBK_MOTOR_OFF_OK,1))) == (BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1)|BFMASK(SFT_MOTOR_EVT_FBK_MOTOR_OFF_OK,1)))
				{
					MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_TRIAC_SC_WARN,1);
				}

				if(! (MotorData.state & BFMASK(PM_SFT_WM_WAIT_FOR_MOTOR_STOP,1)))
				{
					if( (tmp & BFMASK(SFT_MOTOR_EVT_FBK_ERR_INIT,1)) )
					{
						Tools_StartTimer(&(PM_SFT_WM_Motor_Consistency.Tout));
					}

					if( tmp & (BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1)) )
					{
						/* feedback or triac in ShortCircuit/DM */
						PM_SFT_WM_Motor_Consistency.TimeOut = 100*PM_SFT_WM_GET_FBK_VALIDATION_TIME((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data);

						if( ( Tools_IsTimerElapsed(&(PM_SFT_WM_Motor_Consistency.Tout), PM_SFT_WM_Motor_Consistency.TimeOut)
								/* triac SC validated by tach not slowing down */
							) || (tmp & BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1)) )
						{
							tmp |= BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1);
						}
					}

					if( (tmp & BFMASK(SFT_MOTOR_EVT_REL_FAULT_INIT,1)) )
					{
						Tools_StartTimer(&(PM_SFT_WM_Motor_Consistency.Tout));
					}
					if( tmp & (BFMASK(SFT_MOTOR_EVT_REL_FAULT,1)) )
					{
						if( Tools_IsTimerElapsed(&(PM_SFT_WM_Motor_Consistency.Tout), PM_SFT_WM_Motor_Consistency.TimeOut)	)
						{
							tmp |= BFMASK(SFT_MOTOR_EVT_REL_MOTOR_PREFAULT_REQ,1);
						}
					}

					if( tmp & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1) )
					{
						PM_SFT_WM_Motor_Consistency.fsm_state++;
						Tools_StartTimer(&(PM_SFT_WM_Motor_Consistency.Tout));
					}
				}
				else
				{
					PM_SFT_WM_ConsistencyFSM_Resync();
				}
			}
			else
			{
				/* do nothing */
				PM_SFT_WM_ConsistencyFSM_Resync();
			}
			break;
		case PM_SFT_WM_FSM_CONSISTENCY_WAIT_4_PROT_COMPLETED:

			if((evt & BFMASK(SFT_MOTOR_EVT_ZC_GOOD,1)) && (preEvt & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1)))
			{
				/* this state ignores feedback evaluation */
				PM_SFT_WM_Motor_Consistency.TimeOut = 1000*PM_SFT_WM_GET_PROTECTION_TIME((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data);

				if( ! Tools_IsTimerElapsed(&(PM_SFT_WM_Motor_Consistency.Tout), PM_SFT_WM_Motor_Consistency.TimeOut) && (evt & BFMASK(SFT_MOTOR_EVT_LOCK_DOOR_REQ,1) ))
				{
					tmp |= BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1);
				}
				else
				{
					PM_SFT_WM_ConsistencyFSM_Resync();
				}
			}
			else
			{
				PM_SFT_WM_ConsistencyFSM_Resync();
			}

			break;
		default:
			;
	}

	/* update events */
	PM_SFT_WM_Motor_Consistency.evt=tmp;

	return PM_SFT_WM_Motor_Consistency.evt;
}


/**
 in:SFT_MOTOR_EVT_TACH_IN_RANGE
 SFT_MOTOR_EVT_REL_FAULT
 out:
 SFT_MOTOR_EVT_FBK_ERR_INIT
 SFT_MOTOR_EVT_FBK_GEN_ERR
 */
static SFT_MotorEventBmpType PM_SFT_WM_FbkGenericErrFSM(SFT_MotorEventBmpType evt)
{
	SFT_MotorEventBmpType out=0;

	if((evt & (BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1)) ) == 0 )
	{
		out |= BFMASK(SFT_MOTOR_EVT_FBK_ERR_INIT,1);
	}
	else
	{
		/* nothing to do */
	}

	out |= BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1);

	return out;
}

/**
 \param [in]	evt events (SFT_MotorEventBmpType)
 SFT_MOTOR_EVT_FBK_GEN_ERR, SFT_MOTOR_EVT_FBK_RT_SC_ERR, SFT_MOTOR_EVT_FBK_T_DM_ERR
 \retval SFT_MotorEventBmpType,  bitmap of
 SFT_MOTOR_EVT_FBK_RT_SC_ERR
 SFT_MOTOR_EVT_FBK_T_DM_ERR
 SFT_MOTOR_EVT_FBK_ERR_INIT
 */
static SFT_MotorEventBmpType PM_SFT_WM_FbkRelTriacSCErrFSM(SFT_MotorEventBmpType evt)
{
	SFT_MotorEventBmpType out=0;
	switch(PM_SFT_WM_Motor_Consistency.fsm_state_sc)
	{
		case PM_SFT_WM_FSM_SC_INIT:
			PM_SFT_WM_Motor_Consistency.fsm_state_sc = PM_SFT_WM_FSM_SC_IDLE;
			/*break;*/
		case PM_SFT_WM_FSM_SC_IDLE:
			if( evt & BFMASK(SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO,1) )
			{
				/* SFT_MOTOR_EVT_SLOW_DOWN_CHECK_KO means also SFT_MOTOR_EVT_TACH_IN_RANGE,
				 * then triac in SC is assumed*/
				out |= BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1);
			}
			else if(evt & BFMASK(SFT_MOTOR_EVT_TACH_IN_RANGE,1))
			{
				/* drum slows down, then thermal protection is active:
				 * next motor activation should fail leading to missing tach at soft start */

				MotorData.state |= BFMASK(PM_SFT_WM_MOTOR_THERMAL_PROT,1);
			}
			else if (evt & BFMASK(SFT_MOTOR_EVT_TACH_OUT_OF_RANGE,1))
			{
				/* tach not in range, motor is running (SFT_MOTOR_EVT_DRUM_STOP cannot be set) at low speed (not in range)
				 * */
				if((evt & BFMASK(SFT_MOTOR_EVT_FBK_MOTOR_ON_OK,1)) /*&& !(evt & BFMASK(SFT_MOTOR_EVT_DRUM_STOP,1))*/)
				{
					/* prev step was "fbk ok" , then assume fbk fault */
					out |= BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1);
				}
				else
				{
					/* "fbk ok" never detected since start , assume feedback is in SC */
					out |= BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1);
				}

			}
			else
			{
				/* */
			}


			break;
		default:
			/* unexpected condition */
			PM_SFT_WM_Motor_Consistency.fsm_state_sc = PM_SFT_WM_FSM_SC_INIT;
	}

	if(out & BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1))
	{
		if((evt & (BFMASK(SFT_MOTOR_EVT_FBK_GEN_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_RT_SC_ERR,1)|BFMASK(SFT_MOTOR_EVT_FBK_T_DM_ERR,1)) ) == 0 )
		{
			out |= BFMASK(SFT_MOTOR_EVT_FBK_ERR_INIT,1);
		}
		else
		{
			/* nothing to do */
		}
	}

	return out;
}

#endif

/*
Initializes state of the module
please call this method inside a block
prerequisites:
\code
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( MotorData );

	SAFETY_DATA_UPDATE( MotorData );
	SAFETY_REGION_END();
\endcode
as it changes content of the global variable MotorData
*/
static void PM_SFT_WM_Motor_InitState(void)
{
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	MotorData.nfoCntLastValue = 0;
	MotorData.commProtStep=PM_SFT_WM_MOTOR_COMMPROT_NO_COMM;
	MotorData.commStep=PM_SFT_WM_MOTOR_COMM_IDLE;

	MotorData.bCommActive=TRUE;
	MotorData.bSuspendCommReq=FALSE;
	MotorData.bRestoreCommReq=FALSE;
#endif

	MotorData.state=PM_SFT_WM_MOTOR_NO_ERROR;
	/* reset dsp fault and sub code*/
	MotorData.fault.uCode=0;
	MotorData.fault.uSubCode=0;
	/* reset safety module fault and sub code*/
	MotorData.prot.uCode=0;
	MotorData.prot.uSubCode=0;

	MotorData.iDrumSpeed=0;

#if defined(CONFIG_PM_MOTORGATE)
	MotorData.MotorPos=LIB_IOM_GetLoadPos((uint8_t) LD_ENGINE_POS);
	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_MOTOR, &(MotorData.faultParam));
#endif
}

uint16_t PM_SFT_WM_Motor_GetTime(sft_module_status_t *pStatus)
{
	OS_TIME tmpTimer = 0;
	OS_TIME sysTimer = OS_GetTime();
	int16_t retVal = 0;


	if ((MotorData.state & BFMASK(PM_SFT_WM_MOTOR_NOZC_WHILE_RUNNING, 1))
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
			|| ((MotorData.commProtStep == PM_SFT_WM_MOTOR_COMMPROT_NO_COMM) && (pStatus->status == SFT_MODULE_WARNING) )
#endif
		)
	{
		/* 3ph + ac */
		tmpTimer = MotorData.unlockDL + 1000*CONFIG_PM_SAFETY_UNLOCK_DOOR_TIMEOUT;
	}
#if defined(CONFIG_PM_MOTORGATE)
	else if(pStatus->status == SFT_MODULE_PREFAULT)
	{
		tmpTimer = MotorData.timer2 + 1000*PM_SFT_WM_GET_REL_FAULT_FILTER_TIME((PM_SFT_WM_SFMotorFaultType*)MotorData.faultParam.Data);
	}
	else if(pStatus->status == SFT_MODULE_FAULT)
	{
		tmpTimer = sysTimer;
	}
	else if((MotorData.state & BFMASK(PM_SFT_WM_MOTOR_TACH_VIRTUAL_STOP,1)))
	{
		/* missing tach not soft start*/
		tmpTimer = MotorData.timerVirtualStop + 1000*CONFIG_PM_SAFETY_UNLOCK_DOOR_TIMEOUT;
	}
	else if(Events & BFMASK(SFT_MOTOR_EVT_FBKKO_MOTOR_PROT_REQ,1))
	{
		/* fbk ko*/
		tmpTimer = PM_SFT_WM_Motor_Consistency.Tout + PM_SFT_WM_Motor_Consistency.TimeOut;
	}
	else if(Events & BFMASK(SFT_MOTOR_EVT_MISSING_TACH_AT_SOFTSTART,1))
	{
		/* missing tach @ soft start */
		tmpTimer = PM_SFT_WM_Motor_SoftStart.Tout;
	}
#endif
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	else if (MotorData.commStep == PM_SFT_WM_MOTOR_COMMSUSPEND_COMM)
	{
		tmpTimer = MotorData.timer;
	}
	else if ((MotorData.commProtStep == PM_SFT_WM_MOTOR_COMMPROT_FAULT_FILTER) || (MotorData.commProtStep == PM_SFT_WM_MOTOR_COMMPROT_IDLE))
	{
		tmpTimer = MotorData.timer2;
	}
#endif
	else if(pStatus->status == SFT_MODULE_MONITORING)
	{
		tmpTimer = MotorData.monitorTimer + 6000;
	}

	retVal = (int16_t) ((tmpTimer - sysTimer) / 100);
	if(retVal<0)
	{
		retVal=0;
	}
	return retVal;
}
