/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_LinkedLoads.h
\n			This file contains all defines,macro,typedef and functions's prototypes used by the module
\n
\file		PM_LinkedLoads.h
\ingroup	PM_LinkedLoads
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

#ifndef __LINKED_LOADS_H
#define __LINKED_LOADS_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_LinkedLoadsExp.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
/** number of timings to drive the loads */
#define PM_LINKEDLOADS_NUM_OF_TIMINGS 			2 



#define LINKED_LOADS_DEFAULT_LOADID				0xFF

/**
@name linked load state bitmap 
@{
*/
#define TRIAC_RELAY_INIT_OK         			(1<<7)  /**< initialized */
#define RELAY_REQUEST							(1<<6)  /**< new request*/
/*@}*/


/**
@name linked load cfg bitmap 
@{
*/
#define LINKEDLOADS_RELAYTRIAC          		0x01        /**<\hideinitializer RT load type */
#define LINKEDLOADS_NOSECONDARYLOAD     		0x02        /**<\hideinitializer no secondary load  */
#define LINKEDLOADS_NOPRIMARYLOAD       		0x04        /**<\hideinitializer no primary load */
#define LINKEDLOADS_RELAYLOAD           		0x08        /**<\hideinitializer RL load  */
#define LINKEDLOADS_DONT_CARE           		0x10        /**<\hideinitializer dont care enabled */
/*@}*/


/**
@name load state bitmap
@{
*/
#define LL_1ON                          		1   /**<\hideinitializer primary load on */
#define LL_2ON                          		2   /**<\hideinitializer secondary load on */
/*@}*/

/**
@name load state values
@{
*/
#define LL_ALL_OFF                      		0   /**<\hideinitializer all off */
#define LL_ALL_ON                       		(LL_1ON|LL_2ON) /**<\hideinitializer all on */
/*@}*/


/* 
load value bitfield
*/
#define LINKED_LOADS_1ON 						0x10
#define LINKED_LOADS_1OFF 						~(LINKED_LOADS_1ON)

#define LINKED_LOADS_2ON    					0x20
#define LINKED_LOADS_2OFF 						~(LINKED_LOADS_2ON)

#define LINKED_LOADS_CFG_1ON_2ON    			(LINKED_LOADS_1ON|LINKED_LOADS_2ON)
#define LINKED_LOADS_CFG_1OFF_2OFF  			~(LINKED_LOADS_1ON|LINKED_LOADS_2ON)

#define PIN_ON	 								0x80
#define PIN_OFF  								0x7F


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* linked load state */
typedef union{
    uint8_t all;
    /*triacRelayModuleState_t bit;*/
}LinkedLoads_StateType;

/**
Linked load structure
*/
typedef struct{
    /** load id associate to the load */
    uint8_t loadid;
	/** current value */
    uint8_t Value;
    /** new value */
    uint8_t NewValue;
    /** Primary load 
        \li Triac+Relay:    triac pin
        \li Relay+Triac     relay pin
        \li DigitalDelayed: not used (dummy)
        \li Relay+load      relay pin
    */
    LinkedLoads_LoadType primaryLoad;
    
    /** Secondary load 
        \li Triac+Relay:    relay pin
        \li Relay+Triac     triac pin
        \li DigitalDelayed: load
        \li Relay+load      loadid of the load linked
    */
    LinkedLoads_LoadType secondaryLoad;
    /** counter */
    uint8_t cnt;
    /** timings */
    LinkedLoads_TimingType timing[PM_LINKEDLOADS_NUM_OF_TIMINGS];
    /** state 
    see  \ref TRIAC_RELAY_INIT_OK, \ref RELAY_REQUEST*/
    LinkedLoads_StateType state;
    /** cfg 
    see \ref LINKEDLOADS_RELAYTRIAC, \ref LINKEDLOADS_NOSECONDARYLOAD, \ref LINKEDLOADS_NOPRIMARYLOAD, \ref LINKEDLOADS_RELAYLOAD and \ref LINKEDLOADS_DONT_CARE 
 */
    uint8_t cfg;


}PM_LinkedLoadsType;


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */


PM_LinkedLoadsType* 	LinkedLoads_Find2(LinkedLoads_LoadType load_id);
PM_LinkedLoadsType* 	LinkedLoads_FindSecondaryLoad(LinkedLoads_LoadType load_id);
uint8_t 		PM_LinkedLoads_GetLoadState(LinkedLoads_LoadType load_id);

#endif /* __LINKED_LOADS_H */

/* *********** */
/* End of file */
/* *********** */

