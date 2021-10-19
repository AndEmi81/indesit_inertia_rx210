/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_LinkedLoadsExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_LinkedLoadsExp.h
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

#ifndef __PM_LINKED_LOADS_EXP_H
#define __PM_LINKED_LOADS_EXP_H

/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
#define LINKED_LOADS_DUMMY_PARAM   0xFF
#define LINKED_LOADS_DUMMY_PIN     LINKED_LOADS_DUMMY_PARAM


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**
pin type
*/
typedef uint8_t         LinkedLoads_PinType;

/**
load type
*/
typedef uint8_t         LinkedLoads_LoadType;
/**
Timing type
*/
typedef uint8_t         LinkedLoads_TimingType;

/**
\hideinitializer
Linked load error type
*/
typedef enum eLinkedLoads
{
    LINKEDLOADS_NO_ERR=0,                           /**< no error*/
    LINKEDLOADS_NO_ROOM_ERR,                        /**< not enough space */
    LINKEDLOADS_LOAD_ALREADY_PRESENT_ERR,           /**< load already present */
    LINKEDLOADS_LOAD_NOT_FOUND_ERR,                 /**< primary load not found */
    LINKEDLOADS_LOAD_NOT_FOUND2_ERR,                /**< seondary load not found */
    LINKEDLOADS_LOAD_NOT_INIT_ERR,                  /**< load not init */
    LINKEDLOADS_LOAD_SET_LOAD_ERR                  /**< set load error */
    
}LinkedLoadsError_Type;

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_LinkedLoads_Init(void);
void PM_LinkedLoads_NewLoad(LinkedLoads_LoadType primaryload,
                            LinkedLoads_LoadType secondaryload,
                            uint8_t load_id,
                            LinkedLoads_TimingType firstdelay,
                            LinkedLoads_TimingType seconddelay,
                            uint8_t cfg);

void PM_LinkedLoads_SetLoad(uint8_t load_on, uint8_t load_id);

void PM_LinkedLoads_Manager(void);


uint8_t PM_LinkedLoads_GetMaxTiming(uint8_t timingid);
uint8_t PM_LinkedLoads_GetTiming(uint8_t loadPos, uint8_t status);

#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
void PM_LinkedLoads_Driver(void);
#endif /* !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

#endif /* __PM_LINKED_LOADS_EXP_H */

/* *********** */
/* End of file */
/* *********** */

