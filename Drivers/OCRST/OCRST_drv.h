/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCRST_drv.h
\n			Header file of OCRST_drv.c
\n
\file		OCRST_drv.h
\ingroup	OCRST
\date		May 4, 2010
\version	00.00.01
\author		Jaroslav Musil
\n
\par	History:
\n
\n 		00.00.01
\n		04/05/2010
\n		Jaroslav Musil
\n		First Insertion
\n
\n
\n etc.
*/

#ifndef __OCRST_DRV_H
#define __OCRST_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include  HAL_CFG
#include  HAL
#include "IO_driver.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/

RST_Type OCRST_GetResetSourceSync(void);
IO_ErrorType OCRST_SetSync(RST_Type rstValue);
void OCRST_GenerateUserResetSync(void);
bool_t OCRST_MCU_RequiresResetAfterPOR(void);

resetSource_t OCRST_SaveResetValue(void);
void OCRST_RestoreResetValue(resetSource_t resetCopyVal);

void OCRST_CopyFromRealResetSrc(void);
void OCRST_WriteBootKeywordArea(void);
void OCRST_WriteBootSettingKeywordArea(void);
#endif /* __OCRST_DRV_H */

/* *********** */
/* End of file */
/* *********** */
