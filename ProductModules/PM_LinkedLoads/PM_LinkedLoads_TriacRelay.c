/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_LinkedLoads_TriacRelay.c
\n			Specfic methods for linked loads of type triac + relay
\n
\file		PM_LinkedLoads_TriacRelay.c
\ingroup	LINKEDLOADS_TR
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
#include "PM_LinkedLoads_TriacRelayExp.h"


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
Initialization of a relay + triac load
\param  [in]	pin_relay pin of the relay
\param  [in]    pin_triac pin of the triac
\param  [in]	load_id load id or position
\param  [in]	delay_relay delay of the (de)activation of the relay respect to the triac activation in 10 ms
\param  [in]	delay_deact_triac delay of the deactivation of the triac in 10 ms

\return 	none

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
void PM_LinkedLoads_TriacRelay_NewLoad(     LinkedLoads_LoadType pin_triac,
                                            LinkedLoads_LoadType pin_relay, 
                                            uint8_t load_id, 
                                            LinkedLoads_TimingType delay_relay, 
                                            LinkedLoads_TimingType delay_deact_triac)
{
    PM_LinkedLoads_NewLoad( pin_triac,                                      /* pin triac */
                            pin_relay,                                      /* relay pin */
                            load_id,                                        /* load id */
                            delay_relay,                                    /* first delay */
                            delay_deact_triac,								/* second delay */
                            0);                                             /* no cfg needed */
}


/* *********** */
/* End of file */
/* *********** */

