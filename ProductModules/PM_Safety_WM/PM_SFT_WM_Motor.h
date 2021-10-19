/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_MT
\n
\n
\file		PM_SFT_WM_Motor.h
\ingroup	PM_SFT_Motor
\date		08/03/2013
\author		Roberto Fioravanti
\n
*/

#ifndef PM_SAFETY_WM_MOTOR_H_
#define PM_SAFETY_WM_MOTOR_H_

/* ********** */
/* Includes */
/* ********* */
#include "PM_SFT_WM_MotorExp.h"
#include "LIB_ProductSafety.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

#define FAULTSRC_WM_MOTOR_NOFAULT 		0		/* The Motor operates correctly */
#define FAULTSRC_WM_EXTERNAL_BOARD_KO 	1		/* see custom fault  */
#define FAULTSRC_WM_ACMOTOR_REL_KO 		2
#define FAULTSRC_WM_ACMOTOR_KO			3		/* see custom fault  */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef uint16_t	PM_SFT_WM_StateType;			/*bitmap \ref eSFT_MotorState*/
typedef int16_t		DrumSpeedType;
/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
uint16_t PM_SFT_WM_Motor_GetTime(sft_module_status_t *status);
void PM_SFT_WM_Motor_Init(sft_module_status_t *status);
void PM_SFT_WM_Motor_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);


bool_t PM_SFT_WM_MotorGetFault (uint8_t *faultCode, uint8_t *faultSubcode);

bool_t PM_SFT_WM_GetDrumSpeed(DrumSpeedType * pSpeedValue);


void PM_SFT_WM_RestoreCommunicationWithExternalBoard(void);


void PM_SFT_WM_MotorGetActiveProtection(uint8_t *pfaultCode, uint8_t *pfaultSubcode);

bool_t PM_SFT_WM_Motor_CommManagerIsIdle(void);

bool_t PM_SFT_WM_Motor_DisableTmpComm(void);
bool_t PM_SFT_WM_Motor_RestoredComm(void);


void PM_SFT_WM_FSM_SlowDownCheckResync(void);
bool_t PM_SFT_WM_FSM_GetIsTachOutOfRange(void);
bool_t PM_SFT_WM_FSM_GetIsSlowDownKO(void);
bool_t PM_SFT_WM_FSM_IsTachReady(void);
bool_t PM_SFT_WM_FSM_IsVirtualStopActive(void);
#endif /* PM_SAFETY_WM_MOTOR_H_ */


/* *********** */
/* End of file */
/* *********** */
