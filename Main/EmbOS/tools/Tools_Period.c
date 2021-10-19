/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    ToolsPeriod Measuring Period
@{
\par            Description
                EmbOS-based tool for period measurement.
@}
\ingroup        MainDebug
*/
/**
\n
\par		    Period Measurement
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

/**
  Measure Overhead for time measurement so we can take this into account by subtracting it
*/

void Tools_CreatePeriodMeasure ( TOOLS_PERIOD* pPeriodMeasure )
{
	volatile OS_U32 _TimeCopy;
	OS_MEMSET ( pPeriodMeasure, 0, sizeof ( TOOLS_PERIOD ) );
	OS_Timing_Start ( & ( pPeriodMeasure->Overhead ) );
	_TimeCopy = OS_Timing_GetCycles ( &pPeriodMeasure->In ); /* dummy operation */
	OS_Timing_End ( & ( pPeriodMeasure->Overhead ) );
}



void Tools_PeriodMeasure ( TOOLS_PERIOD* pPeriodMeasure )
{
	OS_TIMING _TimeCopy;
	OS_Timing_End ( & ( pPeriodMeasure->In ) );
	_TimeCopy = OS_Timing_GetCycles ( & ( pPeriodMeasure->In ) );
	OS_Timing_Start ( & ( pPeriodMeasure->In ) );
	
	if ( pPeriodMeasure->state < 2 )
	{
		pPeriodMeasure->state++;
	}
	
	pPeriodMeasure->Period_cyc = OS_Timing_GetCycles ( &_TimeCopy ) + OS_Timing_GetCycles ( & ( pPeriodMeasure->Overhead ) ); /* Calculate real context switching time (w/o measurement overhead)*/
}


OS_TIMING Tools_GetPeriodCycles ( TOOLS_PERIOD* pPeriodMeasure )
{
	if ( pPeriodMeasure->state < 2 )
	{
		return 0;
	}
	
	return pPeriodMeasure->Period_cyc;
}

/* Local Function */

/* *********** */
/* End of file */
/* *********** */
