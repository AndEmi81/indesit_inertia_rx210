/*
FCopyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_CK_Fan
\n
\n
\file		PM_SFT_CK_Fan.c
\ingroup	PM_Safety_CK
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
\addtogroup PM_Safety_CK
@{*/

/*
 * Reset for periodic self-test and monitoring
 */

#define FAULTSRC_FAN_ERR_DM				1	/* Diode mode Triac 			F24.71/81 */
#define FAULTSRC_FAN_ERR_SC 			2	/* Short circuit Relay/Triac  	F24.72/82 */
#define FAULTSRC_FAN_ERR_OC				3	/* Open circuit Relay/Triac 	F24.73/83 */
#define FAULTSRC_FAN_ERR_FEEDBACK_KO	4	/* Feedback failure 			F24.74/84 */
#define FAULTSRC_FAN_MR					5	/* Master Rele Fan 				F24.75/85 */
#define FAULTSRC_FAN_MR_DM				6	/* Master Rele Fan 				F24.76/86 */

#define FAULTSRC_FAN_BAD_SETTING				7

/**
 * Valori rilevati dal consistency
 */
/*		|	L	|	H|	50Hz|	100Hz|
 * OFF	|	2	|	3|	6	|	0	 |
 * ON	|	2	|	0|	6	|	4	 |
 */
#define FAN_OK			0	/* PM_CS_VALUE_OK */

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
/* Reset for periodic self-test and monitoring */
/* Inizio defines per PM_SFT_CK_Reset_Monitor() */
#define MR_FAN_CLOSED	(LIB_IOM_GetLoadStatus(pFanStatus->fan.mrLoadPos))	/* =1 =ON  =OPEN   */
#define MR_FAN_OPEN		!(MR_FAN_CLOSED)									/* =0 =OFF =CLOSED */

#define MR_RES_CLOSED	(LIB_IOM_GetLoadStatus(pFanStatus->mrResLoadPos))	/* =1 =ON  =OPEN   */
#define MR_RES_OPEN		!(MR_RES_CLOSED)									/* =0 =OFF =CLOSED */

#define DELAY_RESET_3s	30
#define DELAY_RESET_1s	10
/* Fine defines per PM_SFT_CK_Reset_Monitor() */

#endif

void PM_SFT_CK_FanTang_Init( sft_module_status_t *safetyData );
void PM_SFT_CK_FanTang_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
uint16_t PM_SFT_CK_FanTang_GetTime (sft_module_status_t *status);

void PM_SFT_CK_FanCirc_Init( sft_module_status_t *safetyData );
void PM_SFT_CK_FanCirc_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
uint16_t PM_SFT_CK_FanCirc_GetTime (sft_module_status_t *status);

/*@}*/
#endif /* PM_Safety_DRYINGFAN_H_ */
