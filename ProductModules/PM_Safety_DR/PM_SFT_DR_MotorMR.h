/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_DR_MasterRelays
\n
\n
\file		PM_SFT_DR_MasterRelays.c
\ingroup	PM_Safety_DR
\date		05/06/2013
\author		Alessandro Sabbatini
\n
*/

#ifndef _PM_SAFETY_DR_MASTERRELAYS_H_
#define _PM_SAFETY_DR_MASTERRELAYS_H_

/**
\addtogroup PM_Safety_DR
@{*/
#define FAULTSRC_MOTORMR_PR_DOOR		1
#define FAULTSRC_MOTORMR_STUCK_OFF		2
#define FAULTSRC_MOTORMR_STUCK_ON		3
#define FAULTSRC_MOTORMR_BAD_SETTING	4

#define SFT_MR_FLT_PREFAULT_TIME		0
#define SFT_MR_FLT_FAULT_TIME_OP		2
#define SFT_MR_FLT_FAULT_TIME_VAL		3
#define SFT_MR_FLT_RESET_TIME			4

#define SFT_MR_PARM_DOOR_POS			2
#define SFT_MR_PARM_DOOR_N_ITEMS		3

#define SFT_MR_FLTR_TIMER_VALUE			50	/* Set to 5 sec */
#define SFT_MR_RESET_TIMER_VALUE		20	/* Set to 2 sec */
#define SFT_MR_FLTR_DOOR_TIMER			2

#define MR_CS_OFF_OK				(PM_CS_STATUS_OFF | PM_CS_VALUE_OK)
#define MR_CS_OFF_ERR_GENERIC		(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_GENERIC)
#define MR_CS_OFF_ERR_SC	 		(PM_CS_STATUS_OFF | PM_CS_VALUE_ERR_SC)

#define MR_CS_ON_OK					(PM_CS_STATUS_ON | PM_CS_VALUE_OK)
#define MR_CS_ON_ERR_GENERIC		(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_GENERIC)
#define MR_CS_ON_ERR_OC				(PM_CS_STATUS_ON | PM_CS_VALUE_ERR_OC)


uint16_t PM_SFT_DR_MotorMR_GetTime(sft_module_status_t *status);
void PM_SFT_DR_MotorMR_ForceDoorOpen(void);
bool_t PM_SFT_DR_MotorMR_IsDoorClosed(void);
void PM_SFT_DR_MotorMR_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
void PM_SFT_DR_MotorMR_Init( sft_module_status_t *safetyData );
/*@}*/
#endif /* _PM_SAFETY_DR_MASTERRELAYS_H_ */
