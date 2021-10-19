/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCWDT_drv.h
\n			Header file of OCWDT_drv.c
\n
\file		OCWDT_drv.h
\ingroup	OCWDT
\date		Feb 16, 2010
\version	00.00.02
\author		Jaroslav Musil
\n
\n
\par	History:
\n
\n 		00.00.18
\n		Feb 16, 2010
\n		Jaroslav Musil
\n		changes
\n 		- The inline functions moved to hal.
\n
\n 		00.00.17
\n		Nov 3, 2009
\n		stefano.ughi
\n		changes
\n 		- first insertion
\n
*/

#ifndef __OCWDT_DRV_H
#define __OCWDT_DRV_H

#define __DRIVER_H

/* ********** */
/* Includes   */
/* ********** */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL
#include "IO_driver.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define WDT_IO_DEVICETYPE	CHANNEL
#define WDT_IO_ERRORHOOK	FALSE
#define WDT_IO_DRIVERMODUS	SYNCRON
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

IO_ErrorType OCWDT_InitSync(void);
void 		 OCWDT_RefreshSync(void);
IO_ErrorType OCWDT_SetSync(WDT_timeoutType timeout);
IO_ErrorType OCWDT_StopSync(void);
void 		 OCWDT_StartSync(void);
IO_ErrorType OCWDT_SetWindowingSync(WDT_windowType window);
WDT_counterType OCWDT_ReadCounter(void);
WDT_resetCntrRegType OCWDT_ReadRSTCounter(void);
IO_ErrorType OCWDT_ResetRSTCounter(void);
IO_ErrorType OCWDT_SetQuickTestSync(WDT_timeoutType timeout);
IO_ErrorType OCWDT_SetByteTestSync(WDT_timeoutType timeout, WDT_testByteType testByte);
WDT_testType OCWDT_IsTest(void);
WDT_windowType OCWDT_ReadWindow(void);


#endif /* __OCWDT_DRV_H */

/* *********** */
/* End of file */
/* *********** */
