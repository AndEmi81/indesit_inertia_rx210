/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Safety Common
\n
\n
\file		PM_SFT_CK_Common.c
\ingroup	PM_Safety_WM
\date		13/02/2013
\author		Marco Sperandini
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "Analog.h"
#include "LIB_Generic.h"
#include "LIB_ClassBVar.h"
#include "PM_SFT_CK_Common.h"
#include "PM_SFT_CK_PTCProbe.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

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

#define RESET_PREFAULT_OFFSET	1
uint16_t PM_SFT_CK_PrefaultResetTimeout; /** in 100ms */

/* TODO PM_SFT_CK_Init */
void PM_SFT_CK_Init(void)
{
	Loader_Struct data;
	/** Get the Reset Prefault Timeout */
	PM_SFT_CK_PrefaultResetTimeout = 0;
	if (BasicLoader(SF_PTR_HW_CONFIG, SF_DISPL_TIME_BASE, &data))
	{
		/* ERRORE */
		PLT_ASSERT(PM_SFT_CK_PrefaultResetTimeout);
	}
	else
	{
		PM_SFT_CK_PrefaultResetTimeout = (*(uint8_t*) ((uint8_t*) data.Data + RESET_PREFAULT_OFFSET)) * 10; /* restituito in base 100ms */
	}
}

uint16_t PM_SFT_CK_GetPrefaultResetTimeout(void)
{
	return PM_SFT_CK_PrefaultResetTimeout;
}

