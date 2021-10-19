/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_Damper_ONOFF.h
\n			header file of PM_Damper_ONOFF.c
\n
\file		PM_Damper_ONOFF.h
\ingroup	PM_DamperOnOff
\date		Nov 18, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 18, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n 		01.00.01
\n		Mar 31, 2011
\n		stefano.ughi
\n		changes
\n 			- removed damperONOFFPosition_t PM_Damper_ONOFF_GetReedVal(void);
*/

#ifndef __DAMPER_ONOFF_H
#define __DAMPER_ONOFF_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "PM_Damper_ONOFF_Exp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
/** define on damperONOFFError_t type variables: No Errors */
#define DAMPER_ONOFF_E_OK   0
/** define on damperONOFFError_t type variables: Errors */
#define DAMPER_ONOFF_E_KO   1

#define PLATFORM2_DAMPER_ONOFF


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
typedef uint8_t damperONOFFError_t;
/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
damperONOFFError_t PM_Damper_ONOFF_Init(void);
damperONOFFError_t PM_Damper_ONOFF_InitParams(uint8_t pin,const uint8_t *const pDataComp);
damperONOFFError_t PM_Damper_ONOFF_SetPosition(uint8_t damperPosReq);
damperONOFFError_t PM_Damper_ONOFF_Manager(void);
uint8_t PM_Damper_ONOFF_GetTmaxReachPos(void);
bool_t PM_Damper_ONOFF_IsDriven(void);


#endif /* __DAMPER_ONOFF_H */

