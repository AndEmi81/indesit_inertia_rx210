/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**


*
*/
/**
\par	PM_DoorLock API for applications
\n	This file contains all defines,macro,typedef and functions's prototypes that Application can use.
\n      Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\n
\file     PM_DoorLockExp.h
\ingroup  PM_DoorLock
\date
\version
\author
\n
\n
\par	History:
\n
\n 		01.00.00
\n		20/09/2011
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/

#ifndef _PM_DOORLOCKEXP_H_
#define _PM_DOORLOCKEXP_H_

#include "Config.h"

/*Cmd to open the door*/
#define BPGEN_OPEN			0x0
/*Cmd to close the door*/
#define BPGEN_CLOSE			0x1


/*Unrecognized door status*/
#define BPGEN_FEED_UNDEFINED	    0x2
/*Door position OPEN (FDB_INPUT_LOW)*/
#define BPGEN_FEED_OPEN           0x0
/*Door position CLOSE ( FDB_INPUT_HI )*/
#define BPGEN_FEED_CLOSE          0x1
#define BP_LOCKED 				BPGEN_FEED_CLOSE
#define BP_UNLOCKED 			BPGEN_FEED_OPEN

/* Device running*/
#define BPGEN_DEVICE_RUNNING     	0
/* Device stable stopped*/
#define BPGEN_DEVICE_STOPPED_OK  	1
/* Device stable stopped for errors*/
#define BPGEN_DEVICE_STOPPED_FOR_ERROR  2

/** *Configure the Pilot Type
type allower are: 
BLOCCO_PORTA_IMPULSI, 
BLOCCO_PORTA_MONO_IMPULSO,  // not supported
BLOCCO_PORTA_OMNIUM, ( not defined )
BLOCCO_PORTA_CERA, 
BLOCCO_PORTA_MOTORE // not supported
*/
void  PM_DoorLock_Configure_PilotType(uint8_t pt);

/** Configure the used pin to control the DoorLock*/
void  PM_DoorLock_Configure_ControlPin(uint8_t m_pin_engine);

/** Configure the feedback */
uint8_t PM_DoorLock_ConfigFeedback(uint8_t numFb, uint8_t readType, uint8_t feedbackPos);

/** Configure sensing */
void PM_DoorLock_Configure_Sensing( bool_t isSensing);

/** Enable/Disable the the module output pins.
if OFF the module do not control the DoorLock*/
void PM_DoorLock_SetBPPilot(uint8_t onoff);

/** Get the enable status of the output pins*/
uint8_t PM_DoorLock_GetPilotEnable(void);

/** Issue a command to DoorLock,
BPGEN_OPEN  Cmd to open the door
BPGEN_CLOSE Cmd to close the door
*/
bool_t PM_DoorLock_SetPosition(uint8_t door_position);
/**
 * usata internamente e non effettua il check di DataCare
 */
bool_t PM_DoorLock_SetPosition_Internal(uint8_t door_position);
/**
Check if the device is busy.
\return 	BPGEN_DEVICE_RUNNING: busy,	
		BPGEN_DEVICE_STOPPED_OK: stop OK, 
		BPGEN_DEVICE_STOPPED_FOR_ERROR: stop with error
*/
uint8_t PM_DoorLock_GetStatus(void);
/*
* Get the the lock status filtered with consistency check, and stability observation
* \return 	BPGEN_FEED_CLOSE, BPGEN_FEED_OPEN, BPGEN_FEED_UNDEFINED
*/
uint8_t PM_DoorLock_GetLockStatusFiltered(void);

/** Get the DoorLock  type as  configured by BloccoPorta_Configure_PilotType */
uint8_t PM_DoorLock_GetDoorLockType( void );

void PM_DoorLock_Configure_Ended( void);
uint8_t PM_DoorLock_CheckConfig(void);


#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
/** Configure the aux pin for Wax2Pin (only for CONFIG_BP_CERA_ENABLE)*/
void PM_DoorLock_Wax_Configure_AuxPin(uint8_t auxPin);
/** Configure the device timing to declare fault (only for CONFIG_BP_CERA_ENABLE)*/
void PM_DoorLock_Wax_Configure_FaultTiming(uint16_t mwtforfault, uint16_t mwtforfault_close) ;
void PM_DoorLock_Wax_Configure_Parzilize( uint16_t period, uint16_t tOn, uint16_t filterTime);

uint16_t PM_DoorLock_Wax_GetPrefaultTimeout(void);
#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE */

/** check the internal consitency using the door sensing feedback */
uint8_t PM_DoorLock_InternalConsistency( void );
#define DOORLOCK_LOCKED_OK 				0
#define DOORLOCK_UNLOCKED_OK 			1
#define DOORLOCK_OPENDOOR 				2
#define DOORLOCK_FAULT_FB 				3
#define DOORLOCK_GENERIC_FAIL  			4
#define DOORLOCK_LOCKED_FAIL_ZC  		5
#define DOORLOCK_LOCKED_GENERIC_FAIL	6
#define DOORLOCK_WARNING_DOOR	        7

#define DOORLOCK_NOTAVAILABLE  		0xFF
bool_t PM_DoorLock_isInstableCondition(void);
extern void PM_DoorLock_ForceUnlockDoor(void);
extern void PM_DoorLock_ResetForceUnlockDoor(void);

#define DL_FLT_PRAM_PrefaultTimeOffset 	(0)
#define DL_FLT_PRAM_FaultTimeOffset 	(3)

#define DL_PRAM_DoorLockOnTimeOffset 	(4)  /* x100ms */
#define DL_PRAM_DoorLockOffTimeOffset 	(5) /* x100ms */
#define DL_PRAM_FilterTimeOffset 		(6)
#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
uint8_t PM_DoorLock_Pulse_GetCmdPulse(void);
#endif

#endif /* _PM_DOORLOCKEXP_H_ */

