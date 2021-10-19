/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/*
 * PM_Safety_CK_SafetyActions.c
 *
 *  Created on: Feb 18, 2013
 *      Author: francesco.barletta
 */


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SFT_CK_SafetyActions.h"
#include "PM_SFT_CK_Common.h"
#include "PM_DoorLockExp.h"
#include "LoadsRequestExp.h"
#include "LIB_UTilities.h"
#include "LIB_ClassBVar.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */
/**
 * Indice dei profili utilizzati per convenzione dalle ventole
 */
#define ID_TANG_POFILE_MAX_VEL  (3)
#define ID_TANG_POFILE_AUTO_OFF (4)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */
/**
\n 			Unlocks Door
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_UnlockDoor(iom_loads_struct_t *newLoads, uint8_t status)
{

	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	LIB_IOM_UpdateLoadRequestByItem(LD_DOOR_LOCK_POS, LOAD_REG_OFF, newLoads->loads_ovr);

#endif

	return TRUE;

}


/**
\n 			Unlocks Door
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_LockDoor(iom_loads_struct_t *newLoads, uint8_t status)
{

	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	LIB_IOM_UpdateLoadRequestByItem(LD_DOOR_LOCK_POS, LOAD_REG_ON, newLoads->loads_ovr);

#endif

	return TRUE;

}


/**
\n 			Force Heater Master Relay always ON only - it prevents to activate related loads
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_KeepHeaterMRelayON(iom_loads_struct_t *newLoads, uint8_t status)
{

	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_RES_POS, LOAD_CFG_MR_FORCE_DELAY | LOAD_REG_ON, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_COMMON_RELAY_POS, LOAD_REG_ON, newLoads->loads_ovr);

#endif

	return TRUE;

}


/**
\n 			Force Heaters OFF
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_HeatersOffByMrOff(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	/* COMMENT_251:
	 * La gestione della fase di transizione per MR heaters,dalla posizione ON(relay chiuso) ad OFF(relay aperto);
	 * della disattivazione anticipata dei carichi resistivi associati al MR rispetto a quella dello stesso MR sono
	 * svolte all'interno di LoadRequest.c\LIB_IOM_ManageMasterRelay() */
	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);

#endif

	return TRUE;

}

/**
\n 			Force Fans Master Relay always ON only - it prevents to activate related loads
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_KeepFansMRelayON(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_FAN_POS, LOAD_CFG_MR_FORCE_DELAY | LOAD_REG_ON, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_COMMON_RELAY_POS, LOAD_REG_ON, newLoads->loads_ovr);

#endif

	return TRUE;

}


/**
\n 			Force Fans OFF
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_FansOffByMrOff(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	/* COMMENT_251:
	 * La gestione della fase di transizione per MR ventole,dalla posizione ON(relay chiuso) ad OFF(relay aperto);
	 * della disattivazione anticipata dei carichi associati al MR rispetto a quella dello stesso MR sono svolte
	 * all'interno di LoadRequest.c\LIB_IOM_ManageMasterRelay() */
	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_FAN_POS, LOAD_REG_OFF, newLoads->loads_ovr);

#endif

	return TRUE;

}


/**
\n 			Force Fans Master Relay always ON only - it prevents to activate related loads
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_KeepAllLoadsOFF(iom_loads_struct_t *newLoads, uint8_t status)
{

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	(void) status;

	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_FAN_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_COMMON_RELAY_POS, LOAD_REG_OFF, newLoads->loads_ovr);

#endif

	return TRUE;

}

/**
\n 			Cool Tumble - keeps fan moving at MAX speed until (and even in) fault is restored
\param		...
\return 	TRUE	always

\author 	Alessandro Sabbatini
\date		Jul 18, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_CoolTumble(iom_loads_struct_t *newLoads, uint8_t status)
{
uint8_t retValue = TRUE;

	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	/* COMMENT_251: forces MRM ON - if present and even if not set by action */
	LIB_IOM_UpdateLoadRequestByItem(LD_MASTER_RELAY_FAN_POS, LOAD_REG_ON, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_COMMON_RELAY_POS, LOAD_REG_ON, newLoads->loads_ovr);

	/* COMMENT_251: si imposta il valore di massima velocità. */
	/* COMMENT_251: forzare lo stato di MV tangenziale su ON.
	 * MV tangenziale è un carico parzializzato con 4 (vedi Setting File) profili=livelli di
	 * velocità impostabili */
	LIB_IOM_UpdateLoadRequestByItem(LD_TANGENZIAL_FAN_POS, ID_TANG_POFILE_MAX_VEL, newLoads->loads_ovr);

	/* TODO 251: inserire un controllo per vedere se il fault viene ripristinato oppure se dovesse
	 * succedere,si eseguono delle azioni tra cui si disattiva MV tangenziale per riportarlo in
	 * uno stato di normale funzionamento? */

#endif

	return retValue;
}

/**
\n 			Force Heaters OFF by relays
\param		...
\return 	TRUE	always

\author
\date		Jun 27, 2013
\version	1.0.0
\note		\par \b Nota ...:
			documentation by ....
*/
uint8_t PM_SFT_CK_HeatersOff(iom_loads_struct_t *newLoads, uint8_t status)
{
	(void) status;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	/* COMMENT_251:
	 * La disattivazione dei carichi resistivi eseguita agendo sui singoli relays è un'azione richiesta
	 * in caso di gestione "tempo virtuale" */
	LIB_IOM_UpdateLoadRequestByItem(LD_GRILL1_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_GRILL2_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_CIRCULAR_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_SOLE_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);
	LIB_IOM_UpdateLoadRequestByItem(LD_STEAM_RES_POS, LOAD_REG_OFF, newLoads->loads_ovr);

	/* TODO 251:
	 * Si deve appurare la presenza delle resistenze prima di spegnerle?
	 */

#endif

	return TRUE;

}







