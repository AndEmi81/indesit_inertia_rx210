/*
 * PM_SFT_WM_DryHeater.h
 *
 *  Created on: Apr 18, 2013
 *      Author: francesco.barletta
 */

#ifndef PM_SFT_WM_DRYHEAT_H_
#define PM_SFT_WM_DRYHEAT_H_


/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "LIB_ProductSafety.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define BF_8_GET(val, p, len) \
	(((uint8_t)(val) >> (p)) & ((1U<<(len))-1))
#define BF_16_GET(val, p, len) \
	(((uint16_t)(val) >> (p)) & ((1U<<(len))-1))
#define BF_32_GET(val, p, len) \
	(((uint32_t)(val) >> (p)) & ((1U<<(len))-1))

#define PM_SFT_WM_GET_PREFAULT_TIME_CA(_ptr)			MBGET(2, _ptr, relayCAprefault)
#define PM_SFT_WM_GET_FAULT_TIME_CA(_ptr)				MBGET(1, _ptr, relayCAfault)
#define PM_SFT_WM_GET_FAULT_TIME_CA_OPCODE(_ptr)		MBGET(1, _ptr, relayCAfaultOpcode)

#define PM_SFT_WM_GET_PREFAULT_TIME_CC(_ptr)			MBGET(2, _ptr, relayCCprefault)
#define PM_SFT_WM_GET_FAULT_TIME_CC(_ptr)				MBGET(1, _ptr, relayCCfault)
#define PM_SFT_WM_GET_FAULT_TIME_CC_OPCODE(_ptr)		MBGET(1, _ptr, relayCCfaultOpcode)

/* Define per la estroplazione valore CA Fault Time Opcode dallo spazio SF dedicato ai parametri fault si DR:
 * l'opcpde ha dimensione pari a 5bits, dal bit 11 a 15 del dato relayCAfaultOpcode0: */
#define SF_DRFAULTPARAMS_GET_CA_TIMEOP(_ptr)			BF_8_GET(PM_SFT_WM_GET_FAULT_TIME_CA_OPCODE(_ptr),11,5)
/* Define per la estroplazione valore CC Fault Time Opcode dallo spazio SF dedicato ai parametri fault si DR:
 * l'opcpde ha dimensione pari a 5bits, dal bit 11 a 15 del dato relayCCfaultOpcode0: */
#define SF_DRFAULTPARAMS_GET_CC_TIMEOP(_ptr)			BF_8_GET(PM_SFT_WM_GET_FAULT_TIME_CC_OPCODE(_ptr),11,5)

#define PM_SFT_CA_PREFAULT_AUTOTEST_TIME				10	/* AUTOTEST prefault time(base tempi=100ms) */
#define PM_SFT_PREFAULT_RESET_TIME						40	/* prefault reset time(base tempi=100ms) 	*/

#define	OK		1
#define	KO		0

#define NO_FAULTSRC_DR						0
#define FAULTSRC_DR_GEN_ERR					1
#define FAULTSRC_DR_OFF_RELAY_SC			2
#define FAULTSRC_DR_ON_RELAY_OC				3
#define FAULTSRC_DR_OFF_DISP				4
#define FAULTSRC_DR_BAD_SETTING				5	/* Problems with parameters from the Setting File */

#define DR_CS_ON_OK						(PM_CS_STATUS_ON	| PM_CS_VALUE_OK)
#define DR_CS_OFF_OK					(PM_CS_STATUS_OFF	| PM_CS_VALUE_OK)
#define DR_CS_ON_RELE_TRIAC_OC			(PM_CS_STATUS_ON	| PM_CS_VALUE_ERR_OC)
#define DR_CS_ON_GEN_ERR				(PM_CS_STATUS_ON	| PM_CS_VALUE_ERR_GENERIC)
#define DR_CS_OFF_RELE_TRIAC_SC			(PM_CS_STATUS_OFF	| PM_CS_VALUE_ERR_SC)
#define DR_CS_OFF_GEN_ERR				(PM_CS_STATUS_OFF	| PM_CS_VALUE_ERR_GENERIC)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/
typedef uint16_t SFT_drEventBmpType;

/* ******************* */
/* Function prototypes */
/* ******************* */
uint16_t PM_SFT_WM_DryRes_GetTime(sft_module_status_t *status);
void PM_SFT_WM_DryRes_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags);
void PM_SFT_WM_DryRes_Init(sft_module_status_t *safetyData );

uint16_t PM_SFT_WM_DryRes_GetPrefaultTimeCA(bool_t bAutoTest);
uint16_t PM_SFT_WM_DryRes_GetFaultTimeCA(bool_t bAutoTest);
uint16_t PM_SFT_WM_DryRes_GetPrefaultTimeCC(bool_t bAutoTest);
uint16_t PM_SFT_WM_DryRes_GetFaultTimeCC(bool_t bAutoTest);

/* *********** */
/* End of file */
/* *********** */
#endif /* PM_SFT_WM_WASHHEAT_H_ */
