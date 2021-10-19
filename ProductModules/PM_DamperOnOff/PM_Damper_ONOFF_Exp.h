/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_Damper_ONOFF_Exp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_Damper_ONOFF_Exp.h
\ingroup	PM_DamperOnOff
\date		Nov 16, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 16, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n 		01.00.01
\n		Mar 31, 2011
\n		stefano.ughi
\n		changes
\n 			- added damperONOFFPosition_t PM_Damper_ONOFF_GetReedVal(void);
*/

#ifndef __DAMPER_ONOFF_EXP_H
#define __DAMPER_ONOFF_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */
/** define on damperONOFFPosition_t type variables: stop pilot */
#define DAMPER_ONOFF_STOP_PYLOT     0
/** define on damperONOFFPosition_t type variables: damper position closed */
#define DAMPER_ONOFF_POS_CLOSED     1
/** define on damperONOFFPosition_t type variables: damper position open */
#define DAMPER_ONOFF_POS_OPEN       2
/** define on damperONOFFPosition_t type variables: undefined position */
#define DAMPER_ONOFF_POS_UNDEF      3

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
typedef uint8_t damperONOFFPosition_t;

/** Bits used */
typedef struct
{
	/** if '1' module initialized */
	uint8_t init            :1;
	/** if '1' refresh request */
	uint8_t refresh         :1;
	/** if '1' suspend Damper Manager activity */
	uint8_t stopManager     :1;
	/** if '1' the module is piloting Damper */
	uint8_t pylotStateDamper:1;
} damperONOFFstate_t;

/** Union in order to manage vars. of type damperONOFFstate_t as byte or as bit */
typedef union
{
	/** byte access */
	uint8_t all;
	/** bit access */
	damperONOFFstate_t bit;
} damperONOFFstate_u;

/** Variables for device management */
typedef struct
{
	/** pointer to the damper configuration'area */
	const uint8_t *pDataDamperONOFF;
	/** this is the pin to drive the damper, offset vs. IO module map */
	uint8_t pin;
	/** After Damper has reached position I continue to drive the device for this time.*/
	uint8_t timeAfterPosReached;
	/** Current position of the device */
	damperONOFFPosition_t currPos;
	/** Requested position for the device */
	damperONOFFPosition_t reqPos;
	/** Refresh: return position */
	damperONOFFPosition_t returnPos;
	/** flags */
	damperONOFFstate_u state;

} damperONOFFStruct_t;


/* ****************** */
/* Exported variables */
/* ****************** */
extern damperONOFFStruct_t DamperONOFFVars;

/* ******************* */
/* Function prototypes */
/* ******************* */
damperONOFFPosition_t PM_Damper_ONOFF_GetPosition(void);
void PM_Damper_ONOFF_Refresh(void);
damperONOFFPosition_t PM_Damper_ONOFF_GetReedVal(void);
#endif /* __DAMPER_ONOFF_EXP_H */

/* *********** */
/* End of file */
/* *********** */

