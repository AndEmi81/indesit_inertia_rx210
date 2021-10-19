/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		FrequencyFeedbacks Frequency Feedbacks
\n			TODO
\n
\file		PM_FrequencyFeedbacks.h
\ingroup	PM_FrequencyFeedbacks
\date		12/06/2012
\author		Roberto Fioravanti
\n
\n
*/

#ifndef __FREQUENCY_FEEDBACKS_H
#define __FREQUENCY_FEEDBACKS_H

/* ********** */
/* Includes */
/* ********* */

#include "InputManagerExp.h"
#include "PM_FrequencyFeedbacksExp.h"

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
void PM_FrequencyFeedbacks_Init(void);
void PM_FrequencyFeedbacks_Manager(void);
IO_ErrorType PM_FrequencyFeedbacks_Configure(uint8_t _Pin, const freqFeedbackLLIparams_t* _CfgParams, PM_FrequencyFeedback_PositionType _Position);
IO_ErrorType PM_FrequencyFeedbacks_RestartFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position);
IO_ErrorType PM_FrequencyFeedbacks_DisableFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position);
IO_ErrorType PM_FrequencyFeedbacks_EnableFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position);

#endif /* __FREQUENCY_FEEDBACKS_H */

/* *********** */
/* End of file */
/* *********** */
