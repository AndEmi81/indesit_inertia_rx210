/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		        DigitalOutputMulti.h
\n			        Contiene le definizioni per l'attivazione/disattivazione di un carico
\n
\file           DigitalOutputMulti.h
\ingroup        PM_DigitalOutputMulti
\version        1.0.0
\date           28/01/2008
\author         Stefano Ughi
\n
\n
\Par\b	History:
\n 
\n 		          1.0.0
\n          		28/01/2008
\n          		Stefano Ughi
\n          		Terza versione
\n
\n
\n 		          1.4.0
\n          		10/06/2004
\n          		Progettista_1
\n          		Seconda versione
\n
\n
\n 		          1.3.0
\n          		10/04/2004
\n          		Progettista_1
\n          		Prima versione
\n
*/




#ifndef __DIGITAL_OUTPUT_MULTI_H
#define __DIGITAL_OUTPUT_MULTI_H

/* ******** */
/* Includes */
/* ******** */
#if defined(PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
#include "OutputManagerExp.h"
#include "DigitalOutputs.h"
#else
#include "SkeletonTypes.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
/* Digital Output Multi configuration bitmap */
#define DIGOUT_MULTI_FIRST_PIN	0x01	/* With this configuration only the first pin of two is piloted */
#define DIGOUT_MULTI_SECOND_PIN	0x02	/* With this configuration only the second pin of two is piloted */
#endif /* CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** typedef pointer in input a DigOutMultiSetLoad() */
typedef const uint8_t *const pModulesPins_t;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_DigDelayedMulti_InitSync(void);
void DigOutMultiSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins );
void DigOutMul3PSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins );
void DigDelayedMulti_Init(pModulesPins_t pModulePins, uint8_t delay_activation, uint8_t delay_deactivation);
void DigDelayedMultiSetLoad(L_PARAM loadParam, pModulesPins_t pModulePins );
void DigDelayedMulti_Manager( void );



#endif /* __DIGITAL_OUTPUT_MULTI_H */

/* *********** */
/* End of file */
/* *********** */
