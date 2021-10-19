/**
Header file del modulo microprogramma JumpIf

\file		APP_JumpIf.h
\ingroup	APP_JumpIf
\date		22/09/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_JUMPIF_H
#define __APP_JUMPIF_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t JIF_JumpIf_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment);
#else
uint8_t JIF_JumpIf_Exe(uint8_t highStatement, uint8_t lowStatement);
#endif /* (MULTI_COMPARTMENT_APPL != 1) */
uint8_t JIF_CheckCondition(Loader_Struct const* pDataCond);

#endif /* __APP_JUMPIF_H */

/* *********** */
/* End of file */
/* *********** */
