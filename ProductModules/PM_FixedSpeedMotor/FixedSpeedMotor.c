/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_FixedSpeedMotor 
@{
\n	It'is a platform 2.0 product module for dryer 2010 project.
\n  Aim is the driving of the Fixed Speed motor.

\par Hw Requirements
pins:
\li 2 pins for motor power supply (digital)
\li 1 pin for sense of rotation

\par Sw Requirements
\li  Dependancy
  - HAL and HAL_General
  - Drivers (OCDIO)
  - PM_Library      (LIB_DigitalIO and LIB_IOManager)
  - Platformlibrary
  
\par Management
\n	The module is based on a manager that is a function to be called periodically.
The priority of the method is the same of the task where it's called. It executes the state machine responsible
for the behavior of the motor.
\n  The inizialization method receive the configuration:
\li pins to be used
\li timings
\n  

\par State Machine
State machine ensures the motor is working well: it offers protection against the inversion of the sense of rotations, stopping 
the motor.

\par Configuration:
\code 
#define HAL_DIO_SUPPORT
#define IO_MODULE_MAP_SIZE    XX    <---  TBD
#define N_MAXLOADS            XX    <---  TBD
\endcode

\par Exported Functions:
\todo

@}
\ingroup   	ProductM
*/
/**
\n
\par		Fixed Speed Motor
\n			Implementation of fixed speed motor.
 
\n
\file		FixedSpeedMotor.c 
\ingroup	PM_FixedSpeedMotor 
\date		28-jul-2010 
\version	00.00.54 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:
\n 
\n 		00.00.72 
\n		20/10/2010 
\n		Roberto Fioravanti 
\n		changes 
\n      - new specification implementation: after T3 is elapsed, if speed is 0 then LS1 must be off
\n      -  state machine timings compensation: dropped timings between transition between states.
\n 		00.00.54 
\n		28/07/2010 
\n		Roberto Fioravanti 
\n		changes 
\n 		- creation
\n
\n
\n etc.
*/


/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "DigitalOutputs.h"
#include "FixedSpeedMotor.h"
#include "FixedSpeedMotorExp.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define FSM_GIOM_V_TARGET_INDX  0   /*Speed in GIOM_LoadStruct */

/* state flags */
#define FSM_STATE_INIT          (1<<0)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* state machine values */
typedef enum eFSM_SM_VALUE
{
  FSM_SM_IDLE=0,
  FSM_SM_INIT,
  FSM_SM_SET_POINT_MONITORING,
  FSM_SM_T3,
  FSM_SM_SET_LS1,
  FSM_SM_T2,
  FSM_SM_SET_LS3_ON
  
  
}FSM_StateMachineValType;

/* SpeedSign */
typedef int8_t FSM_SpeedSignType;

/* Target */
typedef int16_t FSM_VTargetType;
/* T2 */
typedef uint8_t FSM_T2Type;
/* T3 */
typedef uint16_t FSM_T3Type;

/* Set Point */
typedef struct 
{
  FSM_SpeedSignType   v_target;         /* new speed value (signed) */
}FSM_SetPointType;

/*current speed */
typedef FSM_SpeedSignType FSM_CurrentSpeedType;

/*manager */
typedef struct 
{
  FSM_CurrentSpeedType    VCurrent;		/* Current Speed value*/
  FSM_T3Type              T3Val;      /* T3 */
  FSM_T3Type              T3Cnt;      /* T3 counter */
  FSM_T2Type              T2Val;      /* T2 */
  FSM_T2Type              T2Cnt;      /* T2 counter */
  uint8_t                 u8SorPin;   /*Sense of rotation*/
  uint8_t									u8SuppHPin; /*Power supply, active high*/
  uint8_t                 u8SuppLPin; /*Power supply, active low*/
  uint8_t                 state;      /*state*/
  FSM_StateMachineValType u8SmVal;    /* state machine value */
    
}FSM_StateMachineType;


/* ********* */
/* Variables */
/* ********* */

/* Global */
/* Static */
static FSM_SetPointType      FSM_SetPoint;
static FSM_StateMachineType  FSM_StateMachine;

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**

Fixed Speed Motor initialization method.
\attention If not called, module is uninitialided and its behavior is unpredictable.

\return 	none
 
\author 	Roberto Fioravanti
\date		28-07-2010
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void FSM_Init(void)
{
	(void)memset(&FSM_SetPoint, 0, sizeof(FSM_SetPointType));
	(void)memset(&FSM_StateMachine, 0, sizeof(FSM_StateMachineType));
}

/**

*/

/**
Sets parameters, and allows the state machine to start.

\param [in] sor pin for sense of rotation
\param [in] suppH pin for power supply (active high)
\param [in] suppL pin for power supply (active low)
\param [in] T2 timing (x 10 ms)
\param [in] T3 timing (x 100 ms)

\attention It is assumed that FSM_Init has been called.
\attention If not called state machine remains in idle state.

\return 	none
 
\author 	Roberto Fioravanti
\date		28-07-2010
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void FSM_SetParam(uint8_t sor, 
                  uint8_t suppH,
                  uint8_t suppL,
                  uint8_t T2,
                  uint8_t T3
                  )
{
  FSM_StateMachine.state |= FSM_STATE_INIT; /* init done */
  FSM_StateMachine.u8SmVal = FSM_SM_INIT;				/*set state machine val*/
  FSM_StateMachine.u8SorPin = sor;									/*pin*/
  FSM_StateMachine.u8SuppHPin = suppH;									/*pin*/
  FSM_StateMachine.u8SuppLPin = suppL;											/*pin*/
  FSM_StateMachine.T2Val=T2;																		/*T2*/
  FSM_StateMachine.T3Val=T3*10;																			/*T3*/
}

/**
Module maneger: processes the command (set point) and executes the state machine to drive the motor.

\attention It is assumed that it's called evfery 10 ms.
\attention Before calling this method, FSM_Init() must be executed.
\attention So that the state machine is properly executed, FSM_SetParam() method must 
have been called befor, othewise state machine is in idle state.

\return 	none


 
\author 	Roberto Fioravanti
\date		28-06-2010
\version	0.0.1

\note		
\par \b Note 1:
state machine implements a protection against the inversion of the sense of rotation

\par \b Note 2:
			documentation by Roberto Fioravanti
*/
void FSM_Manager(void)
{
  FSM_SpeedSignType sign;
  
  if((FSM_StateMachine.state & FSM_STATE_INIT)==0)
  {
    /* param missing */
    return;
  }
  
  switch(FSM_StateMachine.u8SmVal)
  {
    case FSM_SM_IDLE:
      break;
      
    case FSM_SM_INIT:
      FSM_StateMachine.u8SmVal = FSM_SM_SET_POINT_MONITORING;
      /*LS3 OFF */
      LIB_DIO_SET_LOAD(0, FSM_StateMachine.u8SuppLPin);
      LIB_DIO_SET_LOAD(0,  FSM_StateMachine.u8SuppHPin);
      break;
      
    case FSM_SM_SET_POINT_MONITORING:
      sign = 0;
      
      if( FSM_SetPoint.v_target > 0 )
      {
        sign = 1;
      }
      
      if( FSM_SetPoint.v_target < 0 )
      {
        sign = -1;
      }
      
      if(FSM_StateMachine.VCurrent != sign)
      {
        FSM_StateMachine.VCurrent = sign;
                
        FSM_StateMachine.u8SmVal = FSM_SM_T3;
         
        /*timing variable intialization */
        FSM_StateMachine.T3Cnt = FSM_StateMachine.T3Val;
        FSM_StateMachine.T2Cnt = FSM_StateMachine.T2Val;

        /*LS3 (power supply) OFF*/
        LIB_DIO_SET_LOAD(0, FSM_StateMachine.u8SuppLPin);
        LIB_DIO_SET_LOAD(0, FSM_StateMachine.u8SuppHPin);
      }
      break;
      
    case FSM_SM_T3:
      if(!FSM_StateMachine.T3Cnt)
      {
        FSM_StateMachine.u8SmVal = FSM_SM_SET_LS1;
#ifdef FSM_DEBUG        
        LIB_DIO_SET_LOAD(1, 3);
#endif        
      } else
      {
        FSM_StateMachine.T3Cnt--;
        break;
      }
      
      
      
    case FSM_SM_SET_LS1:
            
  	  if(FSM_StateMachine.VCurrent>0)
  	  {
  	    LIB_DIO_SET_LOAD(1, FSM_StateMachine.u8SorPin);
  	  } else
  	  {
  	    LIB_DIO_SET_LOAD(0, FSM_StateMachine.u8SorPin);
  	  }
  	  
  	  if(FSM_StateMachine.VCurrent)
  	  {
  	    FSM_StateMachine.u8SmVal = FSM_SM_T2;
  	  } else
  	  {
  	    FSM_StateMachine.u8SmVal = FSM_SM_SET_POINT_MONITORING;
  	    break;
  	  }
  	        
    case FSM_SM_T2:
#ifdef FSM_DEBUG
      LIB_DIO_SET_LOAD(0, 3);
#endif      
      if(!FSM_StateMachine.T2Cnt)
      {
        FSM_StateMachine.u8SmVal = FSM_SM_SET_LS3_ON;
      } else
      {
        FSM_StateMachine.T2Cnt--;
        break;
      }
      
      
    case FSM_SM_SET_LS3_ON:
    
  	  /*LS3 ON*/
      LIB_DIO_SET_LOAD(1, FSM_StateMachine.u8SuppLPin);
      LIB_DIO_SET_LOAD(1, FSM_StateMachine.u8SuppHPin);
          
      FSM_StateMachine.u8SmVal = FSM_SM_SET_POINT_MONITORING;
      break;
  }
}

/**
Updates the set point with new value passed by pointer to GIOM_LoadStruct.

\param [in] cmd it's a pointer to GIOM_LoadStruct structure, with the following fields:
\li byte 0: set point hi
\li byte 0: set point low

set point = [set point hi]#[set point hi] as 16 bit signed (int16_t)

\return 	none
 
\author 	Roberto Fioravanti
\date		28-07-2010
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void FSM_SetPointUpdate(const GIOM_LoadStruct  * const cmd)
{
  
  FSM_VTargetType speed;
  
	speed = *((uint16_t *)((uint8_t *)&cmd->data[FSM_GIOM_V_TARGET_INDX]));
	
	if( speed > 0 )
  {
    FSM_SetPoint.v_target = 1;
  } else
  if( speed < 0 )
  {
    FSM_SetPoint.v_target = -1;
  } else
  {
    FSM_SetPoint.v_target=0;
  }
      
}


/*
\author Leonardo Sparti
*/
uint8_t FSM_GetCntStatus(void)
{
  if(FSM_StateMachine.u8SmVal != FSM_SM_SET_POINT_MONITORING)
  {
    return 1;
  }
  
  return 0;
  
}



/* *********** */
/* End of file */
/* *********** */

