/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_LinkedLoads_RelayTriacExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_LinkedLoads_RelayTriacExp.h
\ingroup	LINKEDLOADS_RT
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

#ifndef __PM_LINKED_LOADS_RELAYTRIAC_EXP_H
#define __PM_LINKED_LOADS_RELAYTRIAC_EXP_H

/* ******** */
/* Includes */
/* ******** */


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
void PM_LinkedLoads_RelayTriac_NewLoad(     LinkedLoads_LoadType pin_relay,
                                            LinkedLoads_LoadType pin_triac,
                                            uint8_t load_id, 
                                            LinkedLoads_TimingType delay_triac, 
                                            LinkedLoads_TimingType delay_relay);


#endif /* __PM_LINKED_LOADS_RELAYTRIAC_EXP_H */

/* *********** */
/* End of file */
/* *********** */

