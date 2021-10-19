/**
\n
\par		PM_SFT_washRes
\n
\n
\file		PM_SFT_WM_washRes.h
\ingroup	PM_SFT_washRes
\date		11/03/2013
\author		Francesco Barletta
\n
*/

#ifndef PM_SFT_WM_WASHRES_H_
#define PM_SFT_WM_WASHRES_H_


/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "LIB_ProductSafety.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define EVP_OFF		0	/* Elettrovalvola Prelavaggio OFF */
#define	PS_OFF		0	/* Pompa Scarico OFF */
#define SR_OFF		0	/* Switch Relay OFF(default): lato Loading/Washing(WR+EVP) */
#define	WR_OFF		0	/* Washing Res. OFF */
#define EVP_ON		1	/* Elettrovalvola Prelavaggio OFF */
#define	PS_ON		1	/* Pompa Scarico OFF */
#define SR_ON		1	/* Switch Relay OFF(default): lato Loading/Washing(WR+EVP) */
#define	WR_ON		1	/* Washing Res. OFF */

#define	N_SEC	10
#define	M_SEC	10
#define	P_SEC	10

#define	OK		1
#define	KO		0

#define NUM_TENT_MAX	3

#define NO_FAULTSRC_WR						0
#define FAULTSRC_WR_GEN_ERR					1
#define FAULTSRC_WR_OFF_ERR_RELAY_CC		2
#define FAULTSRC_WR_ON_ERR_RELAY_CA			3
#define FAULTSRC_WR_BAD_SETTING				4		/* Problems with parameters from the Setting File */

#define WR_CS_ON_OK						(PM_CS_STATUS_ON	| PM_CS_VALUE_OK)
#define WR_CS_OFF_OK					(PM_CS_STATUS_OFF	| PM_CS_VALUE_OK)
#define WR_CS_ON_ERROR_RELE_TRIAC_CA	(PM_CS_STATUS_ON	| PM_CS_VALUE_ERR_OC)
#define WR_CS_ON_GENERIC_ERROR			(PM_CS_STATUS_ON	| PM_CS_VALUE_ERR_GENERIC)
#define WR_CS_OFF_ERROR_RELE_TRIAC_CC	(PM_CS_STATUS_OFF	| PM_CS_VALUE_ERR_SC)
#define WR_CS_OFF_GENERIC_ERROR			(PM_CS_STATUS_OFF	| PM_CS_VALUE_ERR_GENERIC)

#define WR_OFF_PREFAULT_TIME	50		/* 50*100ms=	5s */
#define BEFORE_RESET_TIME		50		/* 50*100ms=	5s */
#define WR_OFF_FAULT_TIME		0		/*  			0s */
#define WR_ON_PREFAULT_TIME		600		/* 600*100ms=	60s */
#define WR_ON_FAULT_TIME		5400	/* 5400*100ms=	540s=9min */

#define WR_NOT_PILOTED	0
#define WR_PILOTED		1

#define WR_PINCHECK_START	 	3
#define WR_PINCHECK_DISABLED 	0xff

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
typedef uint16_t SFT_wrEventBmpType;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
uint16_t PM_SFT_WM_washRes_GetTime(sft_module_status_t *status);
void PM_SFT_WM_washResMon(iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags);
void PM_SFT_WM_washRes_Init(sft_module_status_t *safetyData );

uint16_t PM_SFT_WM_washRes_GetPrefaultTimeCA(uint8_t bAutoTest);
uint16_t PM_SFT_WM_washRes_GetFaultTimeCA(uint8_t bAutoTest);
uint16_t PM_SFT_WM_washRes_GetPrefaultTimeCC(uint8_t bAutoTest);
uint16_t PM_SFT_WM_washRes_GetFaultTimeCC(uint8_t bAutoTest);
#if 0
void PM_SFT_WM_WashRes_SwitchTest_Start(void);
void PM_SFT_WM_WashRes_SwitchTest_Halt(void);
#endif
uint16_t PM_SFT_WM_washRes_GetFltTimeCAOnHighTemp(uint8_t bAutoTest);
/* *********** */
/* End of file */
/* *********** */
#endif /* PM_SFT_WM_WASHRES_H_ */
