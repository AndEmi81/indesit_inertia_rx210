/**
\defgroup  PM_HoodMotor4Relay 
@{ 
\n Gestione di motori per Cappe a velocità variabile gestita da 4 Relay pilotati selettivamente
\n
\par Configuration:
\todo
\n
\par Exported Functions:
\todo
@}
\ingroup ProductM
*/



/**
\file    PM_HoodMotor4Relay.c
\ingroup PM_HoodMotor4Relay
\version 1.0.0
\date    30/09/2011
\author  Leonardo Sparti
*/
/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/* Old Versions
\version 1.4.0
\date    gg/mm/aaaa
\author  Progettista_1

\version 1.3.0
\date    10/04/2004
\author  Progettista_1
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_HoodMotor4Relay.h"
#include "StandardDefines.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct HM4_Struct {

	uint8_t   pin1;           /* 1° Pin */
	uint8_t   pin2;           /* 2° Pin */
	uint8_t   pin3;           /* 3° Pin */
	uint8_t   pin4;           /* 4° Pin */
	uint8_t   act_delay;      /* Ritardo di attivazione */
	uint8_t   delay_cnt;      /* Contatore per la gestione del ritardo di pilotaggio */
	uint8_t   loadState;      /* Stato del carico */
	uint8_t   initDone;       /* Params Initialization Done ghost */
	
} HM4_Type;

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* **************************** */
/* External functions           */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
static HM4_Type HM4Var;


/* ***************** */
/* Exported function */
/* ***************** */

/**
Funzione per l'inizializzazione di un carico con PylotType HOODMOTOR_4RELAY_L. 

\param  [in]	pModulePins puntatore al campo contenente i 4 pins
\param  [in]	delay_activation: ritardo, impostabile da setting, per l'attivazione dei singoli relay

\author 	Leonardo Sparti
\date		  30/09/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void HoodMotor4Rel_Init(pModulesPins_t pModulePins, uint8_t delay_activation)
{
  HM4_Type *pHM4Var;
  
  
  pHM4Var = &HM4Var;
  
  pHM4Var->pin1 = *pModulePins;
  pHM4Var->pin2 = *(pModulePins+1);
  pHM4Var->pin3 = *(pModulePins+2);
  pHM4Var->pin4 = *(pModulePins+3);
  pHM4Var->act_delay = delay_activation;
  pHM4Var->delay_cnt = 0;
  
}



/**
Funzione per il cambio di stato di un carico con PylotType HOODMOTOR_4RELAY_L. 

\param  [in]	loadParam type L_PARAM stato del carico
\param  [in]	pModulePins puntatore al campo contenente i 4 pins che servono a \n
                pilotare il carico

\author 	Leonardo Sparti
\date		  30/09/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void HoodMotor4RelSetLoad( L_PARAM loadParam, pModulesPins_t pModulePins )
{
  static uint8_t Old_State = 0xFF;
  HM4_Type *pHM4Var;
  
  
  pHM4Var = &HM4Var;
  
  if( (pHM4Var->pin1 == *pModulePins) && (pHM4Var->pin2 == *(pModulePins+1)) &&
      (pHM4Var->pin3 == *(pModulePins+2)) && (pHM4Var->pin4 == *(pModulePins+3)) )
  {
    pHM4Var->loadState = (uint8_t)( loadParam.s.reg>4 ? 4 : loadParam.s.reg );
    
    if(pHM4Var->loadState != Old_State)
    {
      pHM4Var->delay_cnt = 0;
    }
    
    Old_State = pHM4Var->loadState;
  }
  
  pHM4Var->initDone = TRUE;

}



/**
Funzione per la gestione delle velocità di un carico con PylotType HOODMOTOR_4RELAY_L. 

\param  [in]  none

\author 	Leonardo Sparti
\date		  30/11/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void HoodMotor4Rel_Manager( void )
{
  HM4_Type *pHM4Var;
  
  
  pHM4Var = &HM4Var;
  
  /* Manager function shall run only if init done */
  if( !pHM4Var->initDone )
  {
    return;
  }
  
  /* Changed status condition */
  if( pHM4Var->delay_cnt == 0)
  {
    /* To avoid contemporary relays activation, they are all switched OFF */
    LIB_DIO_SET_LOAD(OFF, pHM4Var->pin1);
    LIB_DIO_SET_LOAD(OFF, pHM4Var->pin2);
    LIB_DIO_SET_LOAD(OFF, pHM4Var->pin3);
    LIB_DIO_SET_LOAD(OFF, pHM4Var->pin4);
  }
  
  /* Motor speed setting and relay activation */
  if( (pHM4Var->loadState) && (pHM4Var->delay_cnt>=pHM4Var->act_delay) )
  {
  	switch(pHM4Var->loadState)
  	{
      case 1:
       LIB_DIO_SET_LOAD(ON, pHM4Var->pin1);
       break;
       
      case 2:
       LIB_DIO_SET_LOAD(ON, pHM4Var->pin2);
       break;
  			
      case 3:
       LIB_DIO_SET_LOAD(ON, pHM4Var->pin3);
       break;
  			
      case 4:
       LIB_DIO_SET_LOAD(ON, pHM4Var->pin4);
       break;
  			
      default:
       LIB_DIO_SET_LOAD(OFF, pHM4Var->pin1);
       LIB_DIO_SET_LOAD(OFF, pHM4Var->pin2);
       LIB_DIO_SET_LOAD(OFF, pHM4Var->pin3);
       LIB_DIO_SET_LOAD(OFF, pHM4Var->pin4);
       break;
    }
    
  }
  
  if( pHM4Var->delay_cnt < pHM4Var->act_delay )
  {
    pHM4Var->delay_cnt++;
  }
  
}

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */