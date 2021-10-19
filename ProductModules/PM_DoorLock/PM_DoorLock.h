/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	PM_DoorLock
\n	Doorlock platform interface
\n
\file		PM_DoorLock.h
\ingroup	PM_DoorLock
\date		7/10/2011
\version	01.00.00
\author		marco.sperandini Alberto.Camerin
\n
\n
\par	History:
\n
\n
*/


#ifndef PM_DOORLOCK_H_
#define PM_DOORLOCK_H_

/*lint -e537*/
#include "Config.h"
/*lint +e537*/
#ifdef CONFIG_PM_DOORLOCK
#include "PM_DoorLockExp.h"
#include "LIB_ZeroCrossingExp.h"
#include "notify.h"

/* Value to init the feedback field of the control structure */
#define BPGEN_FEEDBACK_NOT_INIT    		0xFF

/* Device status not initialized..NOT A COMMAND!!! */
#define BPGEN_STATUS_NOT_INIT        	0x11

/*!	Functions
** ************************************************************************************************
*/
void doorLockErrorHandler( int err_code );
/*Init the device*/
void PM_DoorLock_InitModule( uint8_t resetType );


void  PM_DoorLock_Manager(int evt);

/* Chk if the device is busy or not */
uint8_t PM_DoorLock_Busy_Check(void);
uint8_t PM_DoorLock_GetFeedbackVal(uint8_t);
uint8_t PM_DoorLock_StatusReached(void);
uint8_t PM_DoorLock_SetInitStatus(void);
uint8_t PM_DoorLock_GetLockStatus(void);

void PM_DoorLock_PulseReq(void);
void PM_DoorLock_Manager_Time(void);

void PM_DoorLock_Manager_OnZc(NotifyDataEvent* pEvent);
void PM_DoorLock_ResetFeedback(void);

/*!	This struct contains the variables to control the device
***************************************************************************************************
*/

#if defined( CONFIG_PM_DOORLOCK_MR_ENABLE )
typedef enum { DL_ERROR_NONE, DL_ERROR_MR_1 } DL_Error_t;
typedef enum { CMD_NONE, CMD_ON, CMD_OFF} CmdPin_t;
typedef enum { DL_INIT_ST,

	DL_UNLOCKED_ST,

	DL_LOCKING_CHECK_START_ST,
	DL_LOCKING_CHECK_2_ST,
	DL_LOCKING_CHECK_3_ST,
	DL_LOCKING_CHECK_4_ST,
	DL_LOCKING_CHECK_5_ST,
	DL_LOCKING_CHECK_END_ST,

	DL_LOCKING_START_ST,
	DL_LOCKING_ST,

	DL_LOCKED_ST,

	DL_UNLOCKING_START_ST,
	DL_UNLOCKING_ST,

	DL_LOCK_ERROR_ST,
	DL_UNLOCK_ERROR_ST
} DoorLockStatus_t;

typedef enum { DL_NONE_EVT, DL_LOCK_REQ_EVT, DL_UNLOCK_REQ_EVT, DL_INIT_REQ_EVT, DL_ERROR_EVT } DoorLockEvent_t;

#define MAX_NUM_FB			3
#define IDX_FB_LOCK			0
#define IDX_FB_MR			1
#define IDX_FB_CLOSED_DOOR	2

typedef struct {
	uint8_t pilot_type;
	uint8_t dl_pin;
	uint8_t mr_pin_1;
	uint8_t mr_pin_2;
    uint16_t Delta_Timing;
    uint16_t Max_Num_Impulsi;
    uint8_t dl_fb_lock_idx;
    uint8_t mr_fb_idx;
    uint8_t dl_fb_opendor_idx;
    uint8_t isSensing;

}DoorLockConfigParams_t;

struct PM_DoorLock_Cfg
{
	/* configuration */
    uint8_t  pilot_type;
    uint8_t  dl_pin;
    uint16_t Delta_Timing;
    uint16_t Max_Num_Impulsi;

    uint8_t	 mr_pin_1;
    uint8_t	 mr_pin_2;

    uint8_t dl_fb_lock_idx;
    uint8_t mr_fb_idx;
    uint8_t dl_fb_opendor_idx;
    uint8_t  isSensing;
};

struct PM_DoorLock_Ctrl
{
    uint8_t bpmanagercnt;	        /* divisor cycle*/

    /* status */
	DoorLockStatus_t status;
	DoorLockEvent_t  event;

    uint8_t 	cmdPin;
    uint16_t 	on_counter;
    uint16_t 	pilotCnt;
    uint16_t 	Delta_Timing;
    uint16_t 	Max_Num_Impulsi;
    uint8_t  	impulseCnt;

	uint8_t 	consistencyOld;
    uint8_t 	consistencyFiltered;
    uint8_t 	consistencyFilteredCnt;
    uint8_t 	statusFiltered;

    uint8_t  	retryDisabled;

    uint8_t 	disablePilotRelay;
    DL_Error_t 	error;
    uint16_t 	timer;

};
void  PM_DoorLock_Configure(DoorLockConfigParams_t *params);
#else

typedef struct{
	/*methods*/
	void   (*reset)( uint8_t type ); 		/* initializer*/
	bool_t (*setPosition)(uint8_t door_position); 	/* command*/
	bool_t (*setInitStatus)(void);					/* init status*/
	uint8_t (*statusReached)( void );
	void   (*enterStable)( void );
	void   (*onGoingNotStable)( void );
	void   (*onStable)( void );
	uint8_t (*getConsistency)( void );	/* manage the ongoing status */
} DoorLockFuncTab;
typedef enum { CMD_NONE, CMD_ON, CMD_OFF, CMD_ON_CERA, CMD_IMPULSE_ON_POS, CMD_IMPULSE_ON_POS_OFF, CMD_IMPULSE_ON_NEG, CMD_IMPULSE_ON_NEG_OFF } CmdPin;
#define MAX_NUM_FB	2
#define IDX_ZC	0
#define IDX_FB	1
struct PM_DoorLock_Cfg
{
	/* configuration */
    uint8_t  pilot_type;	        /*driving type*/
    uint8_t  control_pin;	        /*Pin to control the engine*/
    uint8_t  feedback_index[MAX_NUM_FB];	/*Feedback index for the _GLOBAL_ unsigned char Feedback[N_FEEDBACK] var in SkeletonGLobal.h*/
    uint8_t  feedback_type[MAX_NUM_FB];

    uint8_t  isSensing:		1;
    uint8_t  isConfigured:	1;

    DoorLockFuncTab *func;
};
struct PM_DoorLock_Ctrl
{
    /* status */

    uint8_t actual_position;      /*Actual status*/
    uint8_t desired_position;	    /*Desired status*/
    uint8_t last_desired_position;	/*Desired status*/
    uint8_t stop_status;	        /*To control the stop status*/
    uint8_t bpmanagercnt;	        /* divisor cycle*/
    uint8_t cmdPin;
    uint8_t pilot_pin_flag;	    	/*Flag to enable or disable the pilot of the PIN*/
    uint16_t on_counter;
    uint8_t consistencyFiltered;
    uint8_t consistencyFilteredCnt;
    uint8_t statusFiltered;
	uint8_t retValOld;
	uint8_t activated;
    uint8_t errCnt;					/* internal error counter */
    uint8_t zcFail;
    uint8_t  isReady:		1;

    union
    {
#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
        struct
        {
        	uint16_t hist_counter;
        	uint16_t Delta_Timing;
        	uint16_t Max_Num_Impulsi;
        	uint16_t Charge_Timing;
            uint8_t  impulseCnt;
            uint8_t retryDisabled;
        } bpi;
#endif /*CONFIG_PM_DOORLOCK_PULSE_ENABLE*/

#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
        struct
        {
        	uint16_t max_fault_wait_time_open;
        	uint16_t max_fault_wait_time_close;
        	uint16_t fault_wait_time;
        	uint16_t zc_lost_time;/* contatore filtro buco di rete in apertura */
        	uint16_t cntCycle;
        	uint16_t periodPartialized;
        	uint16_t tOn;
        	uint16_t filterTime;
            uint8_t  lock_pin;
            uint8_t  lastConsistency;
            uint8_t  zcFail;
            uint8_t  onFail;
            uint8_t  offFail;

        } bpc;
#endif /*CONFIG_PM_DOORLOCK_WAX_ENABLE*/
#if !( defined( CONFIG_PM_DOORLOCK_WAX_ENABLE) || defined( CONFIG_PM_DOORLOCK_WAX_ENABLE ) )
        struct
        {
            uint16_t    dummy;
        } dummy;
#endif
    } bp;


};
#endif

extern struct PM_DoorLock_Cfg  PM_DoorLock_Config_Var;
extern struct PM_DoorLock_Ctrl PM_DoorLock_Control_Var;

#endif
extern int doorLockStatusSaved;
#endif	/* PM_DOORLOCK_H_ */

