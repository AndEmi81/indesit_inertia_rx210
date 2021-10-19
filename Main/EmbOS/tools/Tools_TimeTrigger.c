/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    ToolsPeriod Time triggering
@{
\par            Description
                EmbOS-based tool time measurement.
@}
\ingroup        MainDebug
*/
/**
\n
\par		    Time Trigger
\n
\file		    Tools_Period.c
\ingroup	    MainDebug
\date		    24/12/2010
\version	    00.00.00
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		        00.00.00
\n          	24/12/2010
\n          	Roberto Fioravanti
\n            - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */

#include "Main.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */


/* Static */

/* ******************** */
/* Functions Prototypes */
/* ******************** */
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

void Tools_CreateTimeTrigger ( TOOLS_TIMETRIGGER* pTimeTrigger, OS_TIME period, OS_BOOL start )
{
	OS_MEMSET ( pTimeTrigger, 0, sizeof ( TOOLS_TIMETRIGGER ) );
	pTimeTrigger->period = period;
	pTimeTrigger->enable = start;
}

void Tools_SetTimeTriggerStatus ( TOOLS_TIMETRIGGER* pTimeTrigger, OS_BOOL start )
{
	if (start)
	{
		pTimeTrigger->timeRef = OS_GetTime();
	}

	pTimeTrigger->enable = start;
}

OS_BOOL Tools_IsTimeTriggerActive( TOOLS_TIMETRIGGER* pTimeTrigger)
{
	return pTimeTrigger->enable;
}

OS_BOOL Tools_IsTriggered( TOOLS_TIMETRIGGER* pTimeTrigger)
{

	if(pTimeTrigger->enable)
	{
		OS_TIME t =OS_GetTime();
		if(t - pTimeTrigger->timeRef >= pTimeTrigger->period)
		{
			pTimeTrigger->timeRef = t;
			return (OS_BOOL)1;
		}
	}
	return (OS_BOOL)0;
}

void Tools_StartTimer ( TOOLS_TIMER* pTimer )
{
	/*OS_MEMSET ( pTimer, 0, sizeof ( TOOLS_TIMER ) );*/
	*pTimer = OS_GetTime();
}

OS_BOOL Tools_IsTimerElapsed ( TOOLS_TIMER* pTimer , OS_TIME time)
{
	OS_TIME t =OS_GetTime();
	if((t - *pTimer) >= time)
	{
		return (OS_BOOL)1;
	}
	return (OS_BOOL)0;
}

void Tools_SetTimeout ( TOOLS_TIMER* pTimer, OS_TIME timeout )
{
	OS_TIME newTimeout = OS_GetTime() + timeout;

	/* Workaround to avoid value '0' as '0' means disabled */
	if (newTimeout == 0)
	{
		newTimeout = 1;
	}

	*pTimer = newTimeout;
}

void Tools_DisableTimeout ( TOOLS_TIMER* pTimer)
{
	*pTimer = 0;
}

OS_BOOL	Tools_IsTimeoutDisabled( TOOLS_TIMER* pTimer)
{
	return ((OS_BOOL) (0 == *pTimer));
}

OS_BOOL Tools_IsTimeoutExpired ( TOOLS_TIMER* pTimer)
{
	OS_BOOL retValue = (OS_BOOL) FALSE;
	OS_TIME curTime = OS_GetTime();

	if (*pTimer != 0)
	{

			if (*pTimer <= curTime)
			{
				/* Check if highest nibble of timers are coherent - otherwise timeout set had overflow! */
				if ((curTime >> 20) >=  (*pTimer >> 20))
				{
					retValue = (OS_BOOL) TRUE;
				}
			}
	}

	return retValue;
}
/* Local Function */

/* *********** */
/* End of file */
/* *********** */
