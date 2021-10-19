/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	DoorLock
\n		Wax doorlock functions
\n
\n
\file		PM_DoorLock_Wax.h
\ingroup    DoorLockWax
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
#ifndef _PM_DOORLOCK_WAX_H
#define _PM_DOORLOCK_WAX_H

#if	defined(CONFIG_PM_DOORLOCK_WAX_ENABLE)




void PM_DoorLock_Wax_Install_drv( uint8_t pt );

void PM_DoorLock_Wax_Reset_PilotType( uint8_t type );

bool_t PM_DoorLock_Wax_SetPosition(uint8_t door_position);

bool_t PM_DoorLock_Wax_InitStatus( void );

void PM_DoorLock_Wax_EnterStableStatus( void );

void PM_DoorLock_Wax_OnGoingNotStable( void );

void PM_DoorLock_Wax_OnStable( void );

uint8_t PM_DoorLock_Wax_Consistency(void);
uint8_t PM_DoorLock_Wax_StatusReached(void);

#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE */
#endif /* _PM_DOORLOCK_WAX_H */
