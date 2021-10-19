/*
Copyright (c) 2004-2012, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_GetParams.h
\brief   Il file contiene le funzioni per prelevare i parametri dal settings
 	 o da FW  vedi CONFIG_PARAM_FROM_FIRMWARE definito da configuratore.
\addtogroup PM_MotorGate
\author  Marco Sperandini
*/
/*@{*/
#ifndef _PM_UM_GETPARAMS_H_
#define _PM_UM_GETPARAMS_H_

#include "Config.h"
#include "PM_UM_UniversalMotor.h"
void        setCommonParamsPtr( void );
uint8_t     setHalfFieldParamPtr( void );
void        getOffsetSpeedRangeFromSetting( void );
void  		getOutOfRangeParamsFromSetting( outOfRangeParams_t *outOfRangeParams );
void 		getEngineControlParamFromSetting(inControlloreData_t *controlloreData);
int8_t 		Get_common_MCUOscCompDelta( void );
#endif /* _PM_UM_GETPARAMS_H_ */
