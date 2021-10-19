/*
Copyright (c) 2004-2013, Indesit Company.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par        PM_DrumLock
\n          Drumlock platform interface
\n
\file		PM_DrumLock.h
\ingroup	PM_DrumLock
\date		05/03/2013
\version	01.00.00
\author		michele.tanzi
\n
\n
\par	History:
\n
\n
*/
/**
\addtogroup PM_DrumLock
@{*/

#ifndef PM_DRUMLOCK_H_
#define PM_DRUMLOCK_H_

/*lint -e537*/
#include "Config.h"
/*lint +e537*/
#ifdef CONFIG_PM_DRUMLOCK
#include "PM_DrumLockExp.h"

/* Device status not initialized..NOT A COMMAND!!! */
#define DMLGEN_STATUS_NOT_INIT        	0x11

/* Commands for SetPosition */
#define DMLGEN_LOCK			0x0 /*Cmd to lock the drum*/
#define DMLGEN_UNLOCK		0x1 /*Cmd to unlock the drum*/


/*!	Functions
** ************************************************************************************************
*/
/* Init the device */
void PM_DrumLock_InitModule( void );

/* To control loop of device */
void  PM_DrumLock_Manager( int evt );

/** Configure the device timing to declare fault */
void PM_DrumLock_Configure_MotorOffTiming( uint8_t t_off_motor_lock, uint8_t t_off_motor_unlock );

/** Configures the used pin to control the Drum Lock*/
void  PM_DrumLock_Configure_LockPin( uint8_t pin );

/** Configures the feedback */
void PM_DrumLock_ConfigFeedback( uint8_t feedbackPos );

/** Issue a command to DoorLock,
DMLGEN_LOCK   Cmd to lock the drum
DMLGEN_UNLOCK Cmd to unlock the drum
*/
bool_t PM_DrumLock_SetPosition( uint8_t drum_position );

#endif	/* CONFIG_PM_DRUMLOCK */

#endif	/* PM_DRUMLOCK_H_ */
/*@}*/
