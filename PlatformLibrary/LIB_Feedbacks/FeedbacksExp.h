/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		FeedbacksExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		FeedbacksExp.h
\ingroup	LIB_Feedbacks
\date		Nov 8, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Nov 8, 2010
\n		stefano.ughi
\n		changes
\n 		    Creation
\n
\n etc.
*/

#ifndef __FEEDBACKS_EXP_H
#define __FEEDBACKS_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "FeedbackConfig.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/**
Allowable values for Feedback[]
*/
/** feedback is low */
#define FDB_INPUT_LOW    			0
/** feedback is high */
#define FDB_INPUT_HI     			1
/** feedback is in opposition vs zero crossing voltage */
#define FDB_INPUT_FREQ_OPP_PHASE 	2
/** feedback is in phase with zero crossing voltage */
#define FDB_INPUT_FREQ_PHASE 		3

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
#if ( N_FEEDBACK <= 8 )
typedef uint8_t feedbackBitfield_t;
#elif ( ( N_FEEDBACK > 8 ) && ( N_FEEDBACK <= 16 ) )
typedef uint16_t feedbackBitfield_t;
#elif ( ( N_FEEDBACK > 16 ) && ( N_FEEDBACK <= 32 ) )
typedef uint32_t feedbackBitfield_t;
#else
#error "\n N_FEEDBACK property : N_FEEDBACK too high max value 32 "
#endif
#if (FEEDBACK_SAMPLES>127)
#error "\n FEEDBACK_SAMPLES property: FEEDBACK_SAMPLES value is too large, max value 127"
#endif

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

#if !defined(MODULE_BODY_LIB_FEEDBACKS)
/** The output of the module */
extern uint8_t Feedback[N_FEEDBACK];
/** Bitfield indicating if feedback's reading in position i is enabled "0" or not "1" */
extern feedbackBitfield_t FeedbackInputDisable;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
bool_t LIB_FDB_AreFeedbackReady(void);
bool_t LIB_FDB_GetFeedbackInput(uint8_t pos);
uint8_t LIB_FDB_GetFeedbackVal(uint8_t pos);
bool_t LIB_FDB_IsFeedbackReady(uint8_t pos);
bool_t LIB_FDB_IsFeedbackWritableReady(uint8_t pos);
feedbackBitfield_t LIB_FDB_GetFeedbackWritableBitMap(void);

#endif /* __FEEDBACKS_EXP_H */

/* *********** */
/* End of file */
/* *********** */

