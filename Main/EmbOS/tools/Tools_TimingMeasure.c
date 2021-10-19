/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    ToolsMeasure Measure
@{
\par            Description
\n              EmbOS-based tool for high precision timing measurements.
@}
\ingroup        MainDebug
*/
/**
\n
\par		    Timing Measurement
\n
\file		    Tools_TimingMeasure.c
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
#include "Tools.h"


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

/*
Initializes a variable for timing measurement
Measures Overhead for time measurement so we can take this into account by subtracting it

\param [in]	pMeasure pointer to variable for timing measurement
*/
void Tools_CreateTimingMeasure ( TOOLS_TIMING* pMeasure )
{
	OS_MEMSET ( pMeasure, 0, sizeof ( TOOLS_TIMING ) );
	OS_Timing_Start ( & ( pMeasure->Overhead ) );
	OS_Timing_End ( & ( pMeasure->Overhead ) );
}

/**
Sets the time reference for a TOOLS_TIMING structure to system time value at the time of call.
\param [in]	pMeasure	pointer to variable for timing measurement
*/
void Tools_TimingSetRef ( TOOLS_TIMING* pMeasure )
{
	OS_Timing_Start ( & ( pMeasure->StartRef ) );
}

/**
Sets the time reference in a variable for timing measurement to a known value.
It allows to set the time reference at instant of time different from that in which the reference time has been obtained.
\param [in]	pMeasure pointer to variable for timing measurement
\param [in] sysTime system time previously stored
*/
void Tools_TimingSetRefBySysTime ( TOOLS_TIMING* pMeasure, OS_U32 sysTime)
{
	pMeasure->StartRef = sysTime;
}

/**
Sets the end time reference.
\param [in]	pMeasure pointer to variable for timing measurement
\param [in] sysTime system time previously stored

\return	measurement
*/
OS_TIMING Tools_GetTiming ( TOOLS_TIMING* pMeasure )
{
	OS_Timing_End ( & ( pMeasure->StartRef ) );
	/* take into account of overhead */
	pMeasure->Timing_cyc = OS_Timing_GetCycles ( & ( pMeasure->StartRef ) ) - OS_Timing_GetCycles ( & ( pMeasure->Overhead )  );

	return pMeasure->Timing_cyc;	
}




void Tools_CreateTimingStats ( TOOLS_TIMING_STATS* pStats )
{
	OS_MEMSET ( pStats, 0, sizeof ( TOOLS_TIMING_STATS ) );
	OS_MEMSET ( & ( pStats->Min ), 0xFF, sizeof ( pStats->Min ) );
}

void Tools_ProcessValue ( TOOLS_TIMING_STATS* pStats, OS_TIMING cycles )
{
	if ( cycles )
	{
		pStats->Current = cycles;
	}
	
	if ( cycles < pStats->Min )
	{
		pStats->Min = cycles;
	}
	
	if ( cycles > pStats->Max )
	{
		pStats->Max = cycles;
	}
}

/* Local Function */

/* *********** */
/* End of file */
/* *********** */
