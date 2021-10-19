/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_LinkedLoads_DigitalDelayed.c
\n			Specfic methods for digital delayed loads.
\n
\file		PM_LinkedLoads_DigitalDelayed.c
\ingroup	LINKEDLOADS_DD
\date		Oct 4, 2010 
\version	00.00.67 
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.67
\n		04/10/2010
\n		Roberto Fioravanti 
\n		changes 
\n 		- creation
*/

/* ******** */
/* Includes */
/* ******** */
#include "PM_LinkedLoads.h"
#include "PM_LinkedLoads_DigitalDelayedExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Initialization of a digital delayed load
\param  [in]	pin pin associated to the load
\param  [in]	load_id load id or position
\param  [in]	onDelay delay after the turn on cmd in 10 ms
\param  [in]	offDelay delay after the turn off cmd in 10 ms

\return 	none

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/		    


void PM_LinkedLoads_DigitalDelayed_NewLoad( LinkedLoads_LoadType pin, 
                                            uint8_t load_id, 
                                            LinkedLoads_TimingType onDelay, 
                                            LinkedLoads_TimingType offDelay)
{
    PM_LinkedLoads_NewLoad((LinkedLoads_LoadType)LINKED_LOADS_DUMMY_PIN,    /* dummy pin for triac */
                            pin,                                            /* relay pin */
                            load_id,                                        /* load id */
                            onDelay,                                /* first delay */
                            offDelay,								/* second delay */
                            0);                                             /* no cfg needed */
}

/* *********** */
/* End of file */
/* *********** */

