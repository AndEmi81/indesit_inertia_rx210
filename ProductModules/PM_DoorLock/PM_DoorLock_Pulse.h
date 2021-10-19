/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	DoorLock
\n	Doorlock pulse functions
\n      
\n
\file		PM_DoorLock_Pulse.h
\ingroup        DoorLockPulse
\date	    	20/09/2011
\version	01.00.00
\author		marco.sperandini
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
#ifndef _PM_DOORLOCK_PULSE_H
#define _PM_DOORLOCK_PULSE_H

#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
/*lint -e537*/
#include "StandardTypes.h"
/*lint +e537*/

/*!	Timing Blocco Porta Impulsi
** ************************************************************************************************
*/

void PM_DoorLock_Pulse_Install_drv( uint8_t pt );

void PM_DoorLock_Pulse_Reset_PilotType( uint8_t type );

void PM_DoorLock_Pulse_Configure_Impulse_Properties(uint16_t impulse_properties);

bool_t PM_DoorLock_Pulse_SetPosition(uint8_t door_position);

bool_t PM_DoorLock_Pulse_InitStatus( void );

void PM_DoorLock_Pulse_EnterStableStatus( void );

void PM_DoorLock_Pulse_OngoingNotStable( void );

void PM_DoorLock_Pulse_OnStable( void );

uint8_t PM_DoorLock_Pulse_Consistency(void);
uint8_t PM_DoorLock_Pulse_StatusReached(void);

void PM_DoorLock_Pulse_DisableRetry(bool_t disable);
#endif /* CONFIG_PM_DOORLOCK_PULSE_ENABLE */
#endif /* _PM_DOORLOCK_PULSE_H */
