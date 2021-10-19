/**
Source File 

\file    PumpControl.c
\ingroup PumpControl
\date    4/03/2008
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author Roberto Fioravanti
*/


/** 
\note : 
*/ 


/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void PMP_uP_PumpControl_Init(void);
static uint8_t PMP_uP_PumpControl_StepMgmt(uint8_t highStatement, uint8_t lowStatement);

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Gestisce l'esecuzione del microprogramma PumpControl.

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/


uint8_t PMP_uP_PumpControl_Exe( uint8_t highStatement, uint8_t lowStatement)
{
uint8_t nRet=S_FALSE;


	if ( ( FALSE == Interpreter_Data[0].stepInitialized ) || ( FALSE == Interpreter_Data[0].optionalStepInitialized ) ) 
	{
		/* Init passo o restore from backup */

		PMP_uP_PumpControl_Init();    
		return nRet;
	} 
  
  /* TODO1: condizioni particolari in cui deve essere sospesa l'attività del microprogramma */
/*    
  if( !Alarm )
  {
*/


	if (uPDoorStatus == UP_DOOR_WAIT)
	{  /* porta aperta  -> il uP va sospeso */

		return S_FALSE;  				    
	}

/*		
	}
*/

	nRet= PMP_uP_PumpControl_StepMgmt(highStatement, lowStatement);

	return nRet;   
}

/* ************** */
/* Local Function */
/* ************** */

/**
Inizializza la macchina a stati del microprogramma PumpControl.

\param nesssuno
\return nessuno

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

*/
static void PMP_uP_PumpControl_Init(void)
{
	uP_PumpCtrl_Data.step = PUMP_CTRL_INIT;
}



/**
Gestisce l'avanzamento della macchina a stati del microprogramma PumpControl

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/
static uint8_t PMP_uP_PumpControl_StepMgmt(uint8_t highStatement, uint8_t lowStatement)
{
	uint8_t nRet=S_FALSE;

	switch(uP_PumpCtrl_Data.step) 
	{
		case PUMP_CTRL_INIT:

			SETBIT(uP_PumpCtrl_Data.flags ,PUMP_CTRL_UP_RUNNING_BIT);

			if (BasicLoader(PTR_UP_PUMPCONTROL, lowStatement, &uPPumpCtrl_setting_data) != S_OK ) 
			{		 			
				uP_PumpCtrl_Data.step = PUMP_CTRL_ERROR;
				return S_FALSE;
			}

			if(OFFS_B(uPPumpCtrl_setting_data.Data, PUMP_CTRL_SPEED_DISPL))
			{
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
				if( ( HWC_Flags.bit.bldc ) || ( HWC_Flags.bit.bldc1 ) )
				{  	      
					SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],OFFS_B(uPPumpCtrl_setting_data.Data, PUMP_CTRL_SPEED_DISPL)*PUMP_CTRL_BLDC_SPEED_MUL);
					SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP],OFFS_B(uPPumpCtrl_setting_data.Data, PUMP_CTRL_TIME_DISPL)*PUMP_CTRL_BLDC_RAMP_TIME_MUL);
				}
#endif
				SET_PUMP(LD_WASH_PUMP_IND, LOAD_ON)
			}

			nRet=S_OK;

			break;


		case PUMP_CTRL_ERROR:
			nRet=S_OK;
			CLRBIT(uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT);
			break;

		default:
			uP_PumpCtrl_Data.step=PUMP_CTRL_INIT;
			nRet=S_FALSE;
	}    

	return nRet;
}
/**/


/* *********** */
/* End of file */
/* *********** */
