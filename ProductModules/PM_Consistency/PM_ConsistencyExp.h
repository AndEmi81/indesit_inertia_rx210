/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_ConsistencyExp.h
\ingroup	PM_Consistency
\date		Dec 14, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Dec 14, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __CONSISTENCY_EXP_H
#define __CONSISTENCY_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/** Consistency Value and Status bit mask */
#define PM_CS_VALUE_MASK			0x07
#define PM_CS_STATUS_MASK			0x18
#define PM_CS_FULL_MASK				(PM_CS_VALUE_MASK | PM_CS_STATUS_MASK)

/** Consistency Status values */
#define PM_CS_STATUS_OFF			0x00	/* Load status OFF: relay/sw.relay OFF + load OFF */
#define PM_CS_STATUS_ON				0x08	/* Load status ON: relay/sw.relay ON + load ON */
#define PM_CS_STATUS_TRANS			0x10	/* Transition state: relay/sw.relay ON + load OFF */

/** Consistency Values */
#define PM_CS_VALUE_OK						0x00
#define PM_CS_VALUE_ERR_GENERIC				0x01	/* Generic feedback error */
#define PM_CS_VALUE_ERR_FEEDBACK_KO			0x02	/* Feedback failure */
#define PM_CS_VALUE_ERR_SC 					0x03	/* Short circuit Relay/Triac */
#define PM_CS_VALUE_ERR_OC					0x04	/* Open circuit Relay/Triac */
#define PM_CS_VALUE_ERR_INTERR				0x05	/* Interrupted / disconnected load */
#define PM_CS_VALUE_ERR_DM					0x06	/* Diode mode Triac */
#define PM_CS_VALUE_WARN_DOOR				0x07	/* codice speciale per indicare il WARNING DOOR */

/** Consistency's value not valid  */
#define PM_CS_NOT_VALID_CODE 	0xFF
/** Caso particolare per blocco porta motore (case triac in c.c. motore che gira in continuo) */
#define PM_CS_INCONSISTENT_FEEDBACK 0xFE

/** Variable CS_Status_Var.Status: Status of the module if bit 7 = 1 -> è avvenuta la chiamata d'inizializzazione\n
    del modulo da parte dell' applicazione */
#define PM_CS_STATUS_INIT           (1<<7)
/** Variable CS_Status_Var.Status: Status of the module if bit 6 = 1 -> è avvenuta la chiamata d'inizializzazione\n
    del modulo al Reset */
#define PM_CS_STATUS_CONFIGURED     (1<<6)
/** Variable CS_Status_Var.Status: Status of the module if bit 5 = 1 -> il modulo è stato abilitato */
#define PM_CS_STATUS_ENABLE         (1<<5)


/** Ret value of PM_Consistency_IsValueReady() */
#define PM_CS_NO_ERROR  		  				0
/** Ret value of PM_Consistency_IsValueReady() */
#define PM_CS_ERROR 	 		  				1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */

/** Function to configure the output array for this module */
void PM_Consistency_ConfigureOutputArray(uint8_t *pLLConsistency);

/** Function to enable/disable the status of the module */
void PM_Consistency_SetModuleStatus(uint8_t status);

/** Function to  chk the status of the module */
uint8_t PM_Consistency_GetModuleStatus(void);

/** Chk if the CS value for the specific load is ready*/
uint8_t PM_Consistency_IsValueReady( uint8_t loadPos, uint8_t *pCS_Value);

/** Function returning all consistency status in array argument */
void PM_Consistency_GetAllCsValues(uint8_t *pConsistency);

#endif	/* __CONSISTENCY_EXP_H */

/* *********** */
/* End of file */
/* *********** */

