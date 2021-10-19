/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file     PM_UM_UniversalMotor.h
\brief   Application interface definition  for Univarsal Motor Module
*
*/

/**
Application interface definition  for Univarsal Motor Module
\defgroup PM_MotorGateExp Universal Motor Application Module Interface
\ingroup  ProductMExp
\par	History:
\n
\n 		01.00.00
\n		01/01/2012
\n		marco.sperandini
\n		changes
\n			Creation
\n
*/
/*@{*/

#ifndef __PM_UM_UNIVERSAL_MOTOR_EXP_H
#define __PM_UM_UNIVERSAL_MOTOR_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "StandardTypes.h"
#include "OutputManagerExp.h"
#include "PM_UniversalMotorExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef  uint16_t PM_UM_RpmType;
/** Parameter structure for module's initialization  */
typedef struct {
	uint8_t pinTriac;
	uint8_t pinClockwise;
	uint8_t pinAntiClockwise;
	uint8_t pinHalfField; 
} PM_UM_InitData;

typedef struct {
	uint8_t const * pPinTable;
	uint8_t	cfg;
} PM_UM_CfgDataType;

/** Universal motor position control bytes in the packet PC Contro */
#define GIOM_V_TARGET_INDX  0
#define GIOM_T_TARGET_INDX  (2+GIOM_V_TARGET_INDX)
#define GIOM_FLAGS_INDX	    (2+GIOM_T_TARGET_INDX)

/** Universal motor position control bytes in the packet PC Control */
enum MOTOR_FLAGS
{
    MOTOR_FLAGS_LAST_TARGET_UP=0,
    MOTOR_FLAGS_LAST_TARGET_REACHED,
    MOTOR_FLAGS_SPEED_SIGN,
    MOTOR_FLAGS_ZCREF_NOT_UPDATED
};
/** defines for cmdflags in PM_UM_MotorCmd */
#define RELAY_CFG_FOR_DOOR_LOCK_BIT     3
#define CMDFLAGS_REBALANCE_MASK         0x07
#define RELAY_CFG_FOR_DOOR_LOCK_MASK    (1<<RELAY_CFG_FOR_DOOR_LOCK_BIT)

#define MOTOR_FLAGS_LAST_TARGET_UP_BIT      (1<<MOTOR_FLAGS_LAST_TARGET_UP)
#define MOTOR_FLAGS_LAST_TARGET_REACHED_BIT (1<<MOTOR_FLAGS_LAST_TARGET_REACHED)
#define MOTOR_FLAGS_SPEED_SIGN_BIT          (1<<MOTOR_FLAGS_SPEED_SIGN)
#define MOTOR_FLAGS_ZCREF_NOT_UPDATED_BIT   (1<<MOTOR_FLAGS_ZCREF_NOT_UPDATED)

/** Structure to issue commands to the engine */
typedef struct {
	int16_t  v_tgt;                 /**<  target rpm speed*/
	uint16_t t_tgt;                 /**<  time to reach the target */
	uint16_t cmdflags;              /**<  command flags */
 }PM_UM_MotorCmd;

/* ***************************** */
/* Interface Function prototypes */
/* ***************************** */
/** Initializes the tachometer */


/** Initialize the pins of the loads used by the module */
void        PM_UM_InitParams(  uint8_t const * paramPtr);

/** Module initialization
*/
void      	PM_UM_InitSync( void );

/** Get the rpm drum speed */
extern PM_UM_RpmType PM_UM_GetCurrentRpmSync(void);

/** Controls the engine, by assigning the rpm speed of the drum.
0 Rpm stops the motor.
The sign of the speed indicates the direction of rotation of the drum, positive = clockwise, negative = counterclockwise.
Is handled the reversal of direction of rotation.
It is possibilie indicate at what time to arrive at the target operating a ramp. 
*/
extern void PM_UM_SetMotorCmd( PM_UM_MotorCmd *cmd );

uint16_t	PM_UM_GetTargetSync(void);

/** Like PM_UM_SetMotorCmd but use the GIOM_LoadStruct to issue the command to the motor */
extern void PM_UM_SetTargetSync(const GIOM_LoadStruct *cmd);

/** Gets the rpm motor speed , the direction is indicate by the sign, positive = clockwise, negative = counterclockwise.*/
extern int16_t PM_UM_GetMotorSpeed(void);

/** return TRUE if TACHO is ok, the value is  TRUE if the basket is stopped */
bool_t PM_UM_getBasketStopped(bool_t *value);

/** Returns TRUE if the motor is not active FALSE otherwise  */
uint8_t PM_UM_TriacMotorNotDriven(void);

/** Rerurns TRUE if the tacho has not been  available for tool long time during the status ON */
bool_t PM_UM_IsTimeoutTachometer( void );

/** Returns TRUE if the minimum angle allowed is applied in the SoftStart */
bool_t  PM_UM_IsPhaseAngleMinAtSoftStart( void );

/** reset del flag */
void PM_UM_ResetPhaseAngleMinAtSoftStart( void );

/** Gets the measure of the unbalance */
uint16_t PM_UM_GetUnbalance(void);

/** returns secure alpha*/
uint16_t PM_UM_GetSecureAlpha(void);

/** Returns TRUE if only one of clock/counterclockwise relay is ON */
bool_t PM_UM_RelaysInRunningState(void);

#ifdef CONFIG_PM_UM_MOTOR_MEASURE


/** Gets the phase average */
uint8_t PM_UM_GetPhaseAverage( PM_UM_Meas_t whichMeasure, uint16_t * value );


void PM_UM_OnZC_OK (void);
void PM_UM_OnZC_KO (void);
bool_t PM_UM_IsSuspendedTriac(void);
bool_t PM_UM_IsSuspendedTriacBySafety(void);

#endif /* CONFIG_PM_UM_MOTOR_MEASURE */

#endif /* __PM_UM_UNIVERSAL_MOTOR_EXP_H */
/** @} */

