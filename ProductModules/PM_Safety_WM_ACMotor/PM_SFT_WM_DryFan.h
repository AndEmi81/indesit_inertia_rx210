/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_DryFan
\n
\n
\file		PM_SFT_WM_DryFan.c
\ingroup	PM_SFT_DryFan
\date		02/04/2013
\author		Marco Sperandini
\n
*/

#ifndef PM_Safety_DRYINGFAN_H_
#define PM_Safety_DRYINGFAN_H_

#include "Config.h"
#include "LIB_ProductSafetyExp.h"
#include "LoadsRequest.h"
/**
\addtogroup PM_SFT_DryFan
@{*/


#define FAULTSRC_DRYFAN_ERR_FEEDBACK_KO			1	/* Feedback failure */
#define FAULTSRC_DRYFAN_ERR_SC 					3	/* Short circuit Relay/Triac ,F19.01	pin feedback motoventilatore in corto con Vcc*/
#define FAULTSRC_DRYFAN_ERR_INTERR				4	/* Interrupted / disconnected load */
#define FAULTSRC_DRYFAN_ERR_DM					6	/* Diode mode Triac */
#define FAULTSRC_DRYFAN_BAD_SETTING				7

/**
 * Valori rilevati dal consistency
 */
/*		|	L	|	H|	50Hz|	100Hz|
 * OFF	|	1	|	3|	6	|	0	 |
 * ON	|	1	|	0|	6	|	4	 |
 */
#define DRYFAN_OK			0	/* PM_CS_VALUE_OK */
#define DRYFAN_ERR_SC		3

uint16_t PM_SFT_WM_DryFan_GetTime(sft_module_status_t *status);
void PM_SFT_WM_DryFan_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_WM_DryFan_Init( sft_module_status_t *safetyData );
/*@}*/
#endif /* PM_Safety_DRYINGFAN_H_ */
