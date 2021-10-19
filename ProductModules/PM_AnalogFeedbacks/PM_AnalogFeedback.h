/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_AnalogFeedbac.h
\n			header file of PM_AnalogFeedback.c
\n
\file		PM_AnalogFeedback.h
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

#ifndef __ANALOG_FEEDBACK_H
#define __ANALOG_FEEDBACK_H

/* ********** */
/* Includes */
/* ********* */

#include "PM_AnalogFeedbackExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
enum AnalogFeedbackErr_Tag
{
	AN_FEEDBACK_E_OK = 0,
	AN_FEEDBACK_CH_ADC_NOT_COMPLIANT,
	AN_FEEDBACK_POS_NOT_COMPLIANT

};
/** AnalogFeedbackValue (valore inconsistente) */
#define ANALOG_FEEDBACK_INC             0xFFFFFFFFu
#define ANALOG_FEEDBACK_NO_ADC_DEFINED  0xFFu
/** AnalogFeedbackReady */
#define ANALOG_FEEDBACK_NOT_READY       0x00000000u
/** define on AnalogFeedbackDisable */
#define ANALOG_FEEDBACK_ENABLED         0x00000000u
/** AnalogFeedbackInitialized */
#define ALL_ANALOG_FEEDBACK_NOT_INIT    0x00000000u

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/** valore in input a PM_AnalogFeedback_Setup() */
typedef void *pAnalogFeedbackData_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void PM_AnalogFeedback_Init( void );
enum AnalogFeedbackErr_Tag PM_AnalogFeedback_Setup( uint8_t chADC_AF, pAnalogFeedbackData_t pAnalogFeedbackData );
void PM_AnalogFeedbackManager( void );
bool_t PM_AnalogFeedbackIsReady( uint8_t posAnalogFeedback );
void PM_AnalogFeedbackResetMeasure( uint8_t posAnalogFeedback );


#endif /* __ANALOG_FEEDBACK_H */

/* *********** */
/* End of file */
/* *********** */
