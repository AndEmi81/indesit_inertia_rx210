/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file  PM_UM_MotorGateDriver.h
\brief Header file per il driver di pilotaggio motore
\ingroup PM_MotorGate
\author  Marco Sperandini
*/
/**@{*/

#ifndef __PM_UM_MOTOR_GATE_DRIVER_H
#define __PM_UM_MOTOR_GATE_DRIVER_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
/* ******************* */
/* Function prototypes */
/* ******************* */
void     	PM_UM_GateDrv_Init(uint8_t pin);
void    	PM_UM_GateDrv_SetAlpha(uint16_t alpha_n, bool8_t bHigh);
void 		PM_UM_GateDrv_SetCfg( bool8_t bHigh);
void     	PM_UM_GateDrv_SetImpulseLen( uint16_t len);
void     	PM_UM_GateDrv_check(void);

void     	PM_UM_GateDrv_Enable(void);
void		PM_UM_GateDrv_Disable(void);
uint8_t  	PM_UM_GateDrvIsEnabled ( void );

void 		PM_UM_OnZC(uint32_t cur_measure);
#endif /* __PM_UM_MOTOR_GATE_DRIVER_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
