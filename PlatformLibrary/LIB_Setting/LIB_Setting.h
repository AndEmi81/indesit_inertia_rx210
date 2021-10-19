/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_Setting
\n			header file of LIB_Setting.h
\n
\file		LIB_Setting.h
\ingroup	LIB_Setting
\date		Apr 18, 2013
\version	01.00.00
\author		Roberto Fioravanti

*/

#ifndef __LIB_SETTING_H
#define __LIB_SETTING_H

/* ********** */
/* Includes */
/* ********* */
/*lint -e537 TODO description*/
#include "StandardTypes.h"
/*lint +e537 TODO description*/

#include "LIB_SettingExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Setting signature*/
typedef uint16_t	LIB_SettingSignatureType;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
bool_t LIB_SettingPOST_Sync(void);

bool_t	LIB_SettingBIST_StartSync(uint16_t* pLen, uint8_t** ppStart, LIB_SettingSignatureType* pInitSeed);
LIB_SettingSignatureType	LIB_SettingBIST_StepSync(uint16_t len, uint8_t* ppStart, LIB_SettingSignatureType initSeed);
bool_t	LIB_SettingBIST_EndSync(LIB_SettingSignatureType sign);


#endif /* __LIB_SETTING_H */

/* *********** */
/* End of file */
/* *********** */
