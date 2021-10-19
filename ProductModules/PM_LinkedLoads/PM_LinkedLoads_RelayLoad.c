/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_LinkedLoads_RelayLoad.c
\n			Specfic methods for linked loads of type relay + load.
\n
\file		PM_LinkedLoads_RelayLoad.c
\ingroup	LINKEDLOADS_RL
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
#include <stddef.h>
#include "Config.h"
#include "PM_LinkedLoads.h"
#include "PM_LinkedLoads_RelayLoadExp.h"
#include "OutputManagerExp.h"
#include "LIB_Error.h"

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
extern struct Loader TriacRelay_OutputTablePtr;
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
Initialization of a relay + load load
\param  [in]	pin_relay pin of the relay
\param  [in]    linkedload_loadid id of the loads linked to the relay
\param  [in]	my_load_id load id or position
\param  [in]	delay_loadlinked delay of the (de)activation of the load in 10 ms
\param  [in]	delay_relay delay of the deactivation of the relay in 10 ms

\return 	none

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/	
void PM_LinkedLoads_RelayLoad_NewLoad(      LinkedLoads_LoadType pin_relay,
                                            LinkedLoads_LoadType linkedload_loadid, 
                                            uint8_t my_load_id,
                                            LinkedLoads_TimingType delay_loadlinked, 
                                            LinkedLoads_TimingType delay_relay)
{
    PM_LinkedLoads_NewLoad( pin_relay,                                      /* pin relay */
    						linkedload_loadid,                              /* load id of linked load */
    						my_load_id,                                     /* my load id */
                            delay_loadlinked,                               /* first delay */
                            delay_relay,								    /* second delay */
                            LINKEDLOADS_RELAYTRIAC|LINKEDLOADS_RELAYLOAD);                                             /* no cfg needed */
}



/**
Returns the status of the linked load of a relay + load identified by its load_id
\param  [in]	load_id load id of the RL.

\return 	uint8_t
\retval     TRUE        load is ON
\retval     FALSE       load is OFF. This value is returne also if the load_id is not found.

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
uint8_t PM_LinkedLoads_RelayLoad_GetStatusLinkedLoad(LinkedLoads_LoadType load_id)
{
	uint8_t PilotType,loadpos;
	uint16_t *pint;
	uint8_t *loadPtr;
	
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*)NULL;
	
	
	/* search for load id */
	pLinkedLoad = LinkedLoads_Find2(load_id);
		
	if(!pLinkedLoad)
	{
	    /*Load not found*/
	    PLT_ERROR_WARNING(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND)
		return FALSE;
	}
	
	/*Load found!*/
	loadpos = pLinkedLoad->secondaryLoad;	
	
	pint = ((uint16_t*)((void*)TriacRelay_OutputTablePtr.Data)) + OUTPUT_TABLE_ITEM_SIZE_IN_WORDS*loadpos;
	
	loadPtr = (uint8_t*)pint;
	
	PilotType = (uint8_t)(((*(loadPtr+2) & 0x7C) >> 2) | ((*(loadPtr+2) & 0x03) << 5));
	
	switch(PilotType)
	{
	    /*linked load to the relay can be one of the following */
		case RELAY_TRIAC_L:
		case RELAY_LOAD_L:
		case TRIACRELAY_L:
		case DIGITALDELAYED_L:
		
    		if(PM_LinkedLoads_GetLoadState(loadpos) != LL_ALL_OFF)
    		{
    			return TRUE;
    		}
    		
    		return FALSE;

		
		default:
		
    		if(LIB_IOM_GetLoadStatus(loadpos))
    		{
    			return TRUE;
    		}
	}	
	
	return FALSE;	
}


/**
Returns the status of the linked load of a relay + load identified by its load_id
\param  [in]	load_pos secondary load position
\param  [in]	status status

\return 	bool_t
\retval     TRUE        
\retval     FALSE       

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
bool_t PM_LinkedLoads_RelayLoad_NotDrive(uint8_t load_pos, uint8_t status)
{
	uint8_t load_status;
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*)NULL;
	
	pLinkedLoad = LinkedLoads_FindSecondaryLoad(load_pos);
	
    if(!pLinkedLoad)
	{
	    /*Load not found*/
	    PLT_ERROR_WARNING(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND);
		return FALSE;
	}
	
	/*Load found!*/
	
	if(pLinkedLoad->cfg & LINKEDLOADS_RELAYLOAD)
	{
	    load_status = (pLinkedLoad->Value & LINKED_LOADS_2ON)?1:0;

		if(status!=load_status)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}
/* Local Function */
/* *********** */
/* End of file */
/* *********** */

