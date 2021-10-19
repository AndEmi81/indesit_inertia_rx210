/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_AnalogFeedbackExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_AnalogFeedbackExp.h
\ingroup	PM_AnalogFeedbacks
\date		Jul 19, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Jul 19, 2010
\n		stefano.ughi
\n		changes
\n 		- creation
\n
\n
*/

#ifndef __ANALOG_FEEDBACK_EXP_H
#define __ANALOG_FEEDBACK_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "autoconf.h"
#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

#if defined(CONFIG_PM_ANALOG_FEEDBACKS_NUM)
#if (CONFIG_PM_ANALOG_FEEDBACKS_NUM <= 8)
typedef uint8_t analogFeedbackInfoCmd_t;
#else
typedef uint16_t analogFeedbackInfoCmd_t;
#endif
typedef uint8_t analogFeedbackValue_t;
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
#if !defined(MODULE_BODY_PM_ANALOGFEEDBACK)
extern analogFeedbackValue_t AnalogFeedbackValue[CONFIG_PM_ANALOG_FEEDBACKS_NUM];
extern analogFeedbackInfoCmd_t AnalogFeedbackReady;
extern analogFeedbackInfoCmd_t AnalogFeedbackDisable;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
bool_t PM_AnalogFeedbackValueReady( uint8_t posAnalogFeedback );
bool_t PM_AnalogFeedbackAllValuesReady ( void );
analogFeedbackValue_t PM_AnalogFeedbackGetValue( uint8_t posAnalogFeedback );
analogFeedbackInfoCmd_t AnalogFeedbackGetMaskAnalogF_Enabled( void );

#endif /* CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT */

#endif /* __ANALOG_FEEDBACK_EXP_H */

/* *********** */
/* End of file */
/* *********** */

