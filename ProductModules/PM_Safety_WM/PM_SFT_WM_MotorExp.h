/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_MT
\n
\n
\file		PM_SFT_WM_MotorExp.h
\ingroup	PM_SFT_Motor
\date		06/11/2013
\author		Roberto Fioravanti
\n
*/

#ifndef PM_SAFETY_WM_MOTOREXP_H_
#define PM_SAFETY_WM_MOTOREXP_H_

/* ********** */
/* Includes */
/* ********* */

#include "Config.h"


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
bool_t PM_SFT_WM_DrumSpeedIsZero(void);
bool_t PM_SFT_WM_FSM_IsNoTachoAtSoftStart(void);
#endif /* PM_SAFETY_WM_MOTOREXP_H_ */


/* *********** */
/* End of file */
/* *********** */
