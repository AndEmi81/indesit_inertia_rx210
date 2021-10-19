/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par        PM_PowerProfile.h
\n          This file contains all defines, macro, typedef and functions's 
            prototypes used by the Power Profile module.
\n 
\file       PM_PowerProfile.h
\ingroup    PM_PowerProfile
\date       Nov 8, 2010 
\version    00.00.76 
\author     Roberto Fioravanti, Andrea Ricci
\n
\n
\par\b      History:
\n
\n          00.00.xx
\n          24-25/11/2010
\n          Andrea Ricci, Fabio Tarabelloni
\n          changes
\n          - added APP_ENERGY_PHASES_MAX_NUM setting (coming from .c)
\n          
\n          00.00.76
\n          XX/11/2010
\n          Roberto Fioravanti 
\n          changes 
\n          - file created
*/

#ifndef __POWER_PROFILE_H
#define __POWER_PROFILE_H

/* ******** */
/* Includes */
/* ******** */
#include "PM_PowerProfileExp.h"

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
void PM_PowerProfile_Init (void);
void PM_PowerProfile_GetPowerProfileByProfileID
(
  PM_PowerProfile_ProfileIDType _PowerProfileID,
  PM_PowerProfile_EnergyPhaseType **ppFirstEnergyPhase,
  PM_PowerProfile_PowerProfileSizeType* pSize
);

#endif /* __POWER_PROFILE_H */

/* *********** */
/* End of file */
/* *********** */

