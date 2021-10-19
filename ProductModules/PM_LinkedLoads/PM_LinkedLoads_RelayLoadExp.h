/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_LinkedLoads_RelayLoadExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_LinkedLoads_RelayLoadExp.h
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

#ifndef __PM_LINKEDLOADS_RELAYLOAD_EXP_H
#define __PM_LINKEDLOADS_RELAYLOAD_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "PM_LinkedLoadsExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_LinkedLoads_RelayLoad_NewLoad(      LinkedLoads_LoadType pin_relay,
                                            LinkedLoads_LoadType linkedload_loadid, 
                                            uint8_t my_load_id,
                                            LinkedLoads_TimingType delay_loadlinked, 
                                            LinkedLoads_TimingType delay_relay);
                                            
uint8_t PM_LinkedLoads_RelayLoad_GetStatusLinkedLoad(LinkedLoads_LoadType load_id);                                            
bool_t PM_LinkedLoads_RelayLoad_NotDrive(uint8_t load_pos, uint8_t status);
#endif /* __PM_LINKEDLOADS_RELAYLOAD_EXP_H */

/* *********** */
/* End of file */
/* *********** */

