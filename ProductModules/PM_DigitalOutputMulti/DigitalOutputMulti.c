/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/



/**
Gestione di carichi collegati a 2 pin utilizzati in modalità On/Off.
\file    DigitalOutputMulti.c
\ingroup PM_DigitalOutputMulti
\version 1.0.0
\date    28/01/2008
\author  Stefano Ughi
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
#if defined(PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
#include "Config.h"
#include "DigitalOutputMulti.h"
#else
#include "SkeletonGlobal.h"
#include "IO_Manager.h"
#endif
#include "LIB_ClassBVar.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS)
#define DMD_NUM_OF_LOADS CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct DMDEL_Struct {

	uint8_t   pin1;           /* 1� Pin */
	uint8_t   pin2;           /* 2� Pin */
	uint8_t   act_delay;      /* Ritardo di attivazione */
	uint8_t   deact_delay;    /* Ritardo di disattivazione */
	uint8_t   delay_cnt;      /* Contatore per la gestione del ritardo di pilotaggio */
	uint8_t   loadState;      /* Stato del carico */
	
} DMDEL_Type;

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
static DMDEL_Type DMDVar[DMD_NUM_OF_LOADS];
static uint8_t    DMDLoads;

/* ***************** */
/* Exported function */
/* ***************** */
void PM_DigDelayedMulti_InitSync(void)
{	int i;
	SAFETY_REGION_START();
	DMDLoads = 0;
	for(i=0;i<DMD_NUM_OF_LOADS; i++){
		SAFETY_DATA_ELEM_UPDATE( DMDVar, i );
	}
	SAFETY_DATA_UPDATE( DMDLoads );
	SAFETY_REGION_END();
}

/**
Funzione per l'inizializzazione di un carico con PylotType DIGITALDELAYED_MULTI_L. 

\param  [in]	pModulePins puntatore al campo contenente i 2 pins
\param  [in]	delay_activation: ritardo, impostabile da setting, per l'attivazione del carico
\param  [in]	delay_deactivation: ritardo, impostabile da setting, per la disattivazione del carico

\author 	Leonardo Sparti
\date		  27/05/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void DigDelayedMulti_Init(pModulesPins_t pModulePins, uint8_t delay_activation, uint8_t delay_deactivation)
{
  DMDEL_Type *pDMDVar;
  
  PLT_ASSERT(DMDLoads < DMD_NUM_OF_LOADS)

  SAFETY_REGION_START();
  SAFETY_DATA_CHECK( DMDLoads );
  SAFETY_DATA_ELEM_CHECK( DMDVar, DMDLoads );

  pDMDVar = &DMDVar[DMDLoads];
  
  pDMDVar->pin1 = *pModulePins;
  pDMDVar->pin2 = *(pModulePins+1);
  pDMDVar->act_delay = delay_activation;
  pDMDVar->deact_delay = delay_deactivation;
  pDMDVar->delay_cnt = 0;
  
  DMDLoads++;

  SAFETY_DATA_ELEM_UPDATE( DMDVar, DMDLoads );
  SAFETY_DATA_UPDATE( DMDLoads );
  SAFETY_REGION_END();
}

/**
Funzione per l'attivazione/disattivazione di un carico con PylotType DIGITAL_OUTPUT_MULTI_L.
\n Se cfg = DIGOUT_MULTI_FIRST_PIN viene pilotato solo il primo pin mentre il secondo
\n mantiene lo stato corrente

\n Se cfg = DIGOUT_MULTI_SECOND_PIN viene pilotato solo il secondo pin mentre il primo
\n mantiene lo stato corrente

\n Se cfg è diverso dalle precedenti, allora i pin vengono pilotati insieme

\param  [in]	loadParam type L_PARAM stato del carico
\param  [in]	pModulePins puntatore al campo contenente i 2 pins che servono a \n
                pilotare il carico

\author 	Stefano Ughi
\date		10/03/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void DigOutMultiSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins )
{
	uint8_t loadState1 = (loadParam.s.reg?1:0);
	uint8_t loadState2 = loadState1;
	#if defined(PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
	uint8_t cfg;
	cfg = loadParam.s.config;
	if(cfg == DIGOUT_MULTI_FIRST_PIN )
	{
		loadState1 = 1 - loadState1;
	}
	else if (cfg == DIGOUT_MULTI_SECOND_PIN)
	{
		loadState2 = 1 - loadState2;
	}
#endif /* CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS */

	LIB_DIO_SET_LOAD(loadState1, *pModulePins);
	LIB_DIO_SET_LOAD(loadState2, *(pModulePins+1));

	#else
    SET_LOAD( loadState1, *pModulePins );
    SET_LOAD( loadState2, *(pModulePins+1) );
	#endif
}

/**
Funzione per l'attivazione/disattivazione di un carico con PylotType DIGITAL_OUTPUT_MULTI_3P_L. 

\param  [in]	loadParam type L_PARAM stato del carico
\param  [in]	pModulePins puntatore al campo contenente i 3 pins che servono a \n
                pilotare il carico

\author 	Leonardo Sparti
\date		  22/09/2010
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void DigOutMul3PSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins )
{
	uint8_t loadState;

    loadState = (loadParam.s.reg?1:0); 
	#if defined(PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
    LIB_DIO_SET_LOAD(loadState, *pModulePins);
    LIB_DIO_SET_LOAD(loadState, *(pModulePins+1));
    LIB_DIO_SET_LOAD(loadState, *(pModulePins+2));
	#else
	/* Non usato perch� si � gi� passati alla piattaforma 2 */
    SET_LOAD( loadState, *pModulePins );
    SET_LOAD( loadState, *(pModulePins+1) );
    SET_LOAD( loadState, *(pModulePins+2) );
	#endif
}

/**
Funzione per l'attivazione/disattivazione di un carico con PylotType DIGITALDELAYED_MULTI_L. 

\param  [in]	loadParam type L_PARAM stato del carico
\param  [in]	pModulePins puntatore al campo contenente i 2 pins che servono a \n
                pilotare il carico

\author 	Leonardo Sparti
\date		  27/05/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void DigDelayedMultiSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins )
{
  uint8_t i;
  DMDEL_Type *pDMDVar;
  
  SAFETY_REGION_START();
  SAFETY_DATA_CHECK( DMDLoads );

  PLT_ASSERT(DMDLoads < DMD_NUM_OF_LOADS)

  for(i=0;i<DMDLoads;i++)
  {
	SAFETY_DATA_ELEM_CHECK( DMDVar, i );

    pDMDVar = &DMDVar[i];
    
    if( (pDMDVar->pin1 == *pModulePins) && (pDMDVar->pin2 == *(pModulePins+1)) )
    {
      pDMDVar->loadState = (loadParam.s.reg?ON:OFF);
      pDMDVar->delay_cnt = 0;
    }

    SAFETY_DATA_ELEM_UPDATE( DMDVar, i );
  }

  SAFETY_REGION_END();
}


/**
Funzione per la gestione delle temporizzazioni di un carico con PylotType DIGITALDELAYED_MULTI_L. 

\param  [in]  none

\author 	Leonardo Sparti
\date		  27/05/2011
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void DigDelayedMulti_Manager( void )
{
  uint8_t i;
  DMDEL_Type *pDMDVar;
  
  SAFETY_REGION_START();
  SAFETY_DATA_CHECK( DMDLoads );

  for(i=0;i<DMDLoads;i++)
  {
	SAFETY_DATA_ELEM_CHECK( DMDVar, i );

    pDMDVar = &DMDVar[i];
    
    if(  ( (pDMDVar->loadState == ON)  && (pDMDVar->delay_cnt>=pDMDVar->act_delay)   )  ||
         ( (pDMDVar->loadState == OFF) && (pDMDVar->delay_cnt>=pDMDVar->deact_delay) )   )
    {
      #if defined(PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
        LIB_DIO_SET_LOAD(pDMDVar->loadState, pDMDVar->pin1);
        LIB_DIO_SET_LOAD(pDMDVar->loadState, pDMDVar->pin2);
      #else
        SET_LOAD( pDMDVar->loadState, pDMDVar->pin1 );
        SET_LOAD( pDMDVar->loadState, pDMDVar->pin2 );
      #endif
    }
    
    if( pDMDVar->delay_cnt < (pDMDVar->act_delay > pDMDVar->deact_delay ? pDMDVar->act_delay : pDMDVar->deact_delay) )
    {
      pDMDVar->delay_cnt++;
    }
    
    SAFETY_DATA_ELEM_UPDATE( DMDVar, i );
  }
  
  SAFETY_REGION_END();
}

/* ************** */
/* Local Function */
/* ************** */
#endif
/* *********** */
/* End of file */
/* *********** */
