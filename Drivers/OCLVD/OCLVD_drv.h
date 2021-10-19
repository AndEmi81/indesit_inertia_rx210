/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		OCLVD_drv.h
\n			Header file of OCLVD_drv.c
\n
\file		OCLVD_drv.h
\ingroup	OCLVD
\date		May 6, 2010
\version	00.00.01
\author		Jaroslav Musil
\n
\n
\Par\b	History:
\n
\n 		00.00.01
\n		06/05/2010
\n		Jaroslav Musil
\n		First Insertion
\n
\n
\n etc.
*/

#ifndef __OCLVD_DRV_H
#define __OCLVD_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
/*lint -e537*/
#include "Hal_Interface.h"
#include  HAL_CFG
#include  HAL
#include "IO_driver.h"
/*lint +e537*/
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
IO_ErrorType OCLVDR_SetSync(LVDR_TripPointType tripPointVR);
IO_ErrorType OCLVDNR_SetSync(void);

IO_ErrorType OCLVDW_SetSync(LVDW_TripPointType tripPointVW, Comm_Notification lowVoltageWarning);
void OCLVDW_ISR(void);
IO_ErrorType OCLVD_SetStop(bool_t enable);
LVDR_TripPointType OCLVDR_GetSync(void);
LVDW_TripPointType OCLVDW_GetSync(void);
bool_t OCLVDW_IsWarningOn(void);

#endif /* __OCLVD_DRV_H */

/* *********** */
/* End of file */
/* *********** */
