/**
Source File 

\file    SetAlarm.c
\ingroup SetAlarm
\date    16/06/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author  
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
Gestisce l'esecuzione del microprogramma SetAlarm per la generazione diretta, da setting, di un fault
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma � verificata (istantaneo),
\return S_FALSE se non � ancora verificata(?),
\return codice di errore in altri casi
 
 
\version		1.0.0
\date 			16/06/2008
\author

*/
/*lint -e715
Elimina il warning su highStatement che non viene utilizzato
*/
uint8_t SAL_uP_SetAlarm_Exe (uint8_t HighStatement,uint8_t LowStatement)
{

Loader_Struct uPSetAlarm_setting_data; 

  if ( BasicLoader( PTR_UP_SETALARM, LowStatement, &uPSetAlarm_setting_data ) != S_OK )
  {
  	return S_OK;	/*non eseguo il uP, ma abilito avanzamento passo*/
  }  
 
  /* Dichiara il fault se non � presente un altro fault: corretto? */

  	FLT_manageFault(OFFS_B(uPSetAlarm_setting_data.Data,0),OFFS_B(uPSetAlarm_setting_data.Data,1));
	
	return S_OK;	
}


/* ************** */
/* Local Function */
/* ************** */



/* *********** */
/* End of file */
/* *********** */
