/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Frequency Feedbacks
\n			TODO
\n
\file		PM_FrequencyFeedbacksExp.h
\ingroup	PM_FrequencyFeedbacks
\date		12/06/2012
\author		Roberto Fioravanti
\n
\n
*/

#ifndef __FREQUENCY_FEEDBACKS_EXP_H
#define __FREQUENCY_FEEDBACKS_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**
 * Feedback position
 */
typedef uint8_t		PM_FrequencyFeedback_PositionType;
/**
 *	Frequency Feedback value
 */
typedef enum	eFrequencyFeedbackValues
{
	/** digital low level */
	PM_FREQUENCY_FEEDBACK_LOW=0,
	/** digital high level */
	PM_FREQUENCY_FEEDBACK_HIGH,
	/** low frequency */
	PM_FREQUENCY_FEEDBACK_LOW_FREQ,
	/** high frequency */
	PM_FREQUENCY_FEEDBACK_HIGH_FREQ,
	/** unreliable value, meaning frequency feedback not yet acquired */
	PM_FREQUENCY_FEEDBACKS_UNRELIABLE_VALUE
}PM_FrequencyFeedbacks_eValueType;

typedef enum {
  FREQFB_OK = 0,
  FREQFB_E_INVALID_POSITION,
  FREQFB_E_UNEXPECTED_CONDITION,
  FREQFB_E_FAIL
} PM_FrequencyFeedback_Result;


typedef unsigned char PM_FrequencyFeedbacks_ValueType;

#if (CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER <= 8)
/**
 *	Bitmap
 */
typedef uint8_t	PM_FrequencyFeedbacks_BitmapType;
#else

#error "Frequency Feedbacks too many items: CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER must be <= 8"
#endif


/**
Frequency feedback global structure.
*/
typedef struct PM_FrequencyFeedbacks_Interface_s
{
	/** Frequency feedbacks value.
	Values are ordered by position and the content is reliable if the corresponding bit is set
	It can be used as interface (eg. lookup table)
	*/
	PM_FrequencyFeedbacks_ValueType		feedback[CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER];
	/** Bitmap indicating frequency feedbacks disabled by sw
	It can be used as interface (eg. lookup table)
	*/
	PM_FrequencyFeedbacks_BitmapType	disabled;
}PM_FrequencyFeedbacks_InterfaceType;

/* ****************** */
/* Exported variables */
/* ****************** */
#if !defined(MODULE_BODY_FREQFB)
extern const PM_FrequencyFeedbacks_InterfaceType        PM_FrequencyFeedbacks_Interface;
#endif

#if defined(CONFIG_FREQUENCY_FEEDBACKS_NOFREQ_MONITOR) && !defined(MODULE_BODY_FREQFB)
extern uint16_t	PM_FreqFeeback_NoFreqMonitor[CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER];
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
bool_t PM_FrequencyFeedbacks_Reliable(void);
PM_FrequencyFeedbacks_BitmapType PM_FrequencyFeedbacks_GetReadyRaw(void);
bool_t PM_FrequencyFeedbacks_GetReadyByPosition(PM_FrequencyFeedback_PositionType _Position);
void PM_FrequencyFeedbacks_ResetAllFeedbacks(void);
PM_FrequencyFeedback_Result PM_FrequencyFeedbacks_GetValueByPosition(PM_FrequencyFeedback_PositionType _Position, PM_FrequencyFeedbacks_ValueType* _pValue);
#endif /* __FREQUENCY_FEEDBACKS_EXP_H */

/* *********** */
/* End of file */
/* *********** */

