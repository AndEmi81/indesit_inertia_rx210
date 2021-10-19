/*
Copyright (c) 2004-2013, Indesit Company.
Tutti i diritti sono riservati. All rights reserved.
*/

/**


*
*/
/**
\par    PM_DrumLock API for applications
\n      This file contains all defines,macro,typedef and functions's prototypes that Application can use.
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
\n		05/03/2013
\n		michele.tanzi
\n		changes
\n 			Creation
\n
*/

#ifndef _PM_DRUMLOCKEXP_H_
#define _PM_DRUMLOCKEXP_H_

#include "StandardTypes.h"


/* Return values for GetLockStatusFiltered */
#define DMLGEN_FEED_UNDEFINED	    	0x2 /*Unrecognized drum status*/
#define DMLGEN_FEED_UNLOCK          	0x1 /*Drum position UNLOCK (FDB_INPUT_HI)*/
#define DMLGEN_FEED_LOCK            	0x0 /*Drum position LOCK (FDB_INPUT_LOW)*/

/* Return values for DriverStatus */
#define DMLGEN_DEVICE_RUNNING     		0 /* Device running*/
#define DMLGEN_DEVICE_STOPPED_OK  		1 /* Device stable stopped*/
#define DMLGEN_DEVICE_STOPPED_FOR_ERROR	2 /* Device stable stopped for errors*/

/**
Check if the device is busy.
\return DMLGEN_DEVICE_RUNNING: busy,
        DMLGEN_DEVICE_STOPPED_OK: stop OK,
        DMLGEN_DEVICE_STOPPED_FOR_ERROR: stop with error
*/
uint8_t PM_DrumLock_DriverStatus( void );

/*
* Get the the lock status filtered with consistency check, and stability observation
* \return 	DMLGEN_FEED_LOCK, DMLGEN_FEED_UNLOCK, DMLGEN_FEED_UNDEFINED
*/
uint8_t PM_DrumLock_GetLockStatusFiltered( void );


#endif /* _PM_DRUMLOCKEXP_H_ */

