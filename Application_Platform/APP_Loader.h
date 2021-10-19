/**
Header file del modulo Loader

\file		APP_Loader.h
\ingroup	APP_Loader
\date		23/07/2008
\version	01.00.00
\author		Valeria Catani, Simone Costantini, Stefano Ughi, Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_LOADER_H 
#define __APP_LOADER_H

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
uint8_t LDR_phaseLoader(uint8_t compartment, uint8_t cycle, uint8_t phase, uint8_t mSetting, PhaseLoader_Struct *pDataOut);
#else
uint8_t LDR_phaseLoader(uint8_t cycle, uint8_t phase, uint8_t mSetting, PhaseLoader_Struct *pDataOut);
#endif /* (MULTI_COMPARTMENT_APPL != 1) */
bool_t MissedOnlyCheck(const Loader_Struct* pointer, uint8_t position);
uint8_t MissedLoadCheck(Loader_Struct* pDataOut, uint8_t position);
uint16_t MissedLoadShortCheck(Loader_Struct* pointer, uint8_t position);
bool_t SettingDataCompare (uint16_t check, uint16_t reference);
bool_t SettingDataReverseCompare (uint16_t check, uint16_t reference);

#endif /* __APP_LOADER_H */

/* *********** */
/* End of file */
/* *********** */
