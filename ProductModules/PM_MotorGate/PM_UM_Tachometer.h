/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Tachometer.h
\brief   Interfaccia del drive della tachimetrica
\ingroup PM_MotorGate
*/

/**@{*/

#ifndef __PM_UM_TACHOMETER_H
#define __PM_UM_TACHOMETER_H

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define TACH_VALUE_KO  				0  	/* Data not ready */
#define TACH_VALUE_OK  				1	/* Data ready */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Tacho acquisition mode */
typedef enum {
	PM_UM_TACHO_CONTINUOUS=0,
	PM_UM_TACHO_ONE_SHOT
}PM_UM_Tachometer_SetAcqMode;

/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_UM_Tacho_Init(uint8_t pin);
void PM_UM_Tacho_Reset(void);
uint32_t PM_UM_Tacho_GetValue(void);

uint8_t  PM_UM_Tacho_GetNewValue(uint32_t *tacho, bool_t update);
void PM_UM_Tacho_SetNewValue(uint32_t val);

void      PM_UM_Tacho_Enable(void);
void      PM_UM_Tacho_Disable(void);

void      PM_UM_Tacho_SetMode( PM_UM_Tachometer_SetAcqMode howMeasure );
PM_UM_Tachometer_SetAcqMode   PM_UM_Tacho_GetMode(void);

IO_ErrorType PM_UM_Tacho_InputCaptureManager(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);

#endif /* __PM_UM_TACHOMETER_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
