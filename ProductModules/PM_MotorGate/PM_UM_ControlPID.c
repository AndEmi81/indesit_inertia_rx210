/*
Copyright (c) 2004-2012, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**

*/
/**
\file    PM_UM_ControlPID.c
\brief   Implementazione di un controllo motore collettore per lavabiancheria
	 algoritmo PID in coso di valutazione
\ingroup PM_MotorGate
\author  Marco Sperandini
*/
/**
\addtogroup PM_MotorGate
*@{*/
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

#if defined(CONFIG_PM_MOTORGATE) && defined(CONFIG_PM_MOTORGATE_PID_CONTROL)
#include <string.h>
#include <stdlib.h>
#include "RTOS.h"
#include "LIB_Utilities.h"
#include "DigitalOutputs.h"
#include "PM_UM_Control.h"
#include "PM_UM_UniversalMotor.h"

#include "LIB_Trace.h"


/* ***************** */
/* Exported function */
/* ***************** */

/** Calcola il nuovo angolo di parzializzazione di fase

\param inControlloData type inControlloreData_t   (cfr. Controllo_motore_collettore.h)
\param inControlloreVariables type inControlloreVariables_t   (cfr. Controllo_motore_collettore.h)
\return outControllore type outControllore_t (cfr. Controllo_motore_collettore.h)

*/
typedef struct {
  uint16_t rpm;
  int32_t kp;
  int32_t kd;
  int32_t ki;  
} PIDParams_t;

#define PID_SPEED_BAND (6)
PIDParams_t PidParams[PID_SPEED_BAND] = { 
  { 70, 100, 40, 0  },
  { 100, 100, 40, 0  },
  { 200, 100, 40, 0  },
  { 400, 100, 40, 0  },
  { 1000, 100, 40, 0  },  
  { 10000, 100, 40, 0  },  
};



typedef struct {
  PIDParams_t *paramPtr;
  int32_t integra;
   
   uint16_t vTarget; 

   uint16_t tau_target;
   uint16_t tau_n;
   uint16_t tau_n_1;
   
   uint16_t alpha_n_1;
   
   int32_t  errore;
   int32_t  correzione;
    
   outControllore_t outControllore;
} PIDCtrlVar_t;

volatile static PIDCtrlVar_t PIDCtrlVar;


void setSpeeFromTarget( uint16_t rpm ){
  int i;
  for(i=0; i< PID_SPEED_BAND && PidParams[i].rpm < rpm; i++)
  {
  }
  PIDCtrlVar.paramPtr =  &PidParams[i];
}


outControllore_t controllore(inControlloreData_t inControlloData, inControlloreVariables_t inControlloreVariables )
{
    
    PIDCtrlVar.tau_n     = NOMINAL_TO_STD(inControlloreVariables.tau_n);
    PIDCtrlVar.tau_n_1   = NOMINAL_TO_STD(inControlloreVariables.tau_n_1);
    PIDCtrlVar.alpha_n_1 = NOMINAL_TO_STD(inControlloreVariables.alpha_n_1);
    
    if(  inControlloreVariables.vTarget !=  PIDCtrlVar.vTarget ){
      setSpeeFromTarget( inControlloreVariables.vTarget );
    }
    PIDCtrlVar.vTarget =  inControlloreVariables.vTarget; 
    
    if(  PIDCtrlVar.vTarget == 0 )
      PIDCtrlVar.vTarget = 1;
    
    PIDCtrlVar.tau_target = (1200 * 1000000)/( PIDCtrlVar.vTarget * 118 * 16 );
    
    /*
    Inizializzazione default uscita controllore
    */
    PIDCtrlVar.outControllore.alpha = 0xFFFF;
    PIDCtrlVar.outControllore.error = CONTROLLORE_ALPHA_TOO_LARGE_ERROR;

    PIDCtrlVar.errore = PIDCtrlVar.tau_target - PIDCtrlVar.tau_n ;     /* contronto tra frequenze se positiva va troppo veloce */
    if( PIDCtrlVar.errore < 0){
      /* incementa la velocita*/
      TRACE_LED(0, LED_RED);
    }else{
    /* decrementa la velocita  */
      TRACE_LED(1, LED_RED);
    }
    
    PIDCtrlVar.correzione = PIDCtrlVar.errore;
    
    if( PIDCtrlVar.paramPtr->kd != 0 ){
    	PIDCtrlVar.correzione += ((PIDCtrlVar.tau_n - PIDCtrlVar.tau_n_1)/PIDCtrlVar.paramPtr->kd);
    }
    
    PIDCtrlVar.integra += PIDCtrlVar.errore;
    
    if( PIDCtrlVar.paramPtr->ki != 0 ){
    	PIDCtrlVar.correzione += ((PIDCtrlVar.integra)/PIDCtrlVar.paramPtr->ki);
    }
    
    PIDCtrlVar.correzione = PIDCtrlVar.correzione/PIDCtrlVar.paramPtr->kp;
    
    PIDCtrlVar.outControllore.alpha = PIDCtrlVar.alpha_n_1 + PIDCtrlVar.correzione;   
    
    PIDCtrlVar.outControllore.alpha = STD_TO_NOMINAL(PIDCtrlVar.outControllore.alpha);
    
    PIDCtrlVar.outControllore.error = CONTROLLORE_NO_ERROR;
    return (PIDCtrlVar.outControllore);
}


#endif /* CONFIG_PM_MOTORGATE */

/* *********** */
/* End of file */
/* *********** */
/**@}*/
