/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		PM_PWM_LoadsControl.h
\n			header file of PM_PWM_LoadsControl.c
\n
\file		PM_PWM_LoadsControl.h 
\ingroup	PM_PWM_LoadsControl 
\date		31/05/2011 
\version	01.00.00 
\author		Giuseppe Boccolato 
\n
\n
\Par\b	History:
\n
\n 		01.00.00 
\n		31/05/2011 
\n	  Giuseppe Boccolato 
\n		changes 
\n 			Creation
\n
*/

#ifndef __PWM_CTRL_H
#define __PWM_CTRL_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "PM_PWM_LoadsControlExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void PWMLoadInit ( uint8_t pin, uint8_t pilottype );
void PWMResetLoad( void );
void PWMSetLoad ( uint8_t on, uint8_t config, uint8_t pin, const uint8_t *const pStruct );
void PWMControlManager ( void );

#endif /* __PWM_CTRL_H */

/* *********** */
/* End of file */
/* *********** */

