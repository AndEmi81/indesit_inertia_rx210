/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par	On Chip Real Tiem Clock (RTC)
\n		This is the header file 
\n
\file		${OCRTC_drv.h}
\ingroup	OCRTC
\date		28-10-2011
\version	Platform2
\author		Marco Sperandini
\n
\n
\Par\b	History:
\n
\n
\n
*/

#ifndef __OCRTC_DRV_H
#define __OCRTC_DRV_H
#include "Config.h"
#ifndef AUTOCONF_INCLUDED
#include "autoconf.h"
#endif
#ifdef CONFIG_PL_DRV_OCRTC

/* ********** */
/* Includes   */
/* ********** */

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
 typedef void (*RTC_AlarmCallback_t)( uint32_t value );

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/**
Initializes the RTC

\param  [in]		  uint8_t freqSelection
\return 	IO_ErrorType
\retval   returns only IO_E_OK 
*/
void OCRTC_InitSync(void);
uint32_t OCRTC_getTimeSync(void);
void OCRTC_setTimeSync(uint32_t value);
int8_t OCRTC_setAlarmSync( uint32_t cmpAlarmValue, RTC_AlarmCallback_t cb );
void OCRTC_IsrHandler(void);


#else
#define OCRTC_InitSync()
#endif /* define(CONFIG_PL_DRV_OCRTC) */

#endif /* __OCRTC_DRV_H */

/* *********** */
/* End of file */
/* *********** */
