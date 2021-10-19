/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCIRQ_drv.h
\n			header file of OCIRQ_drv.c
\n
\file		OCIRQ_drv.h
\ingroup	OCIRQ
\date		Nov 30, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 30, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __OCIRQ_DRV_H
#define __OCIRQ_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
/*lint -e537*/
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL
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
#if defined(CONFIG_OCIRQ_STATIC_CFG)
/*CONFIG_OCIRQ_STATIC_CFG use static OCIRQ configuration */
extern IO_ErrorType callbackIRQ_Event (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType validate);
#define OCIRQ_CALLBACK callbackIRQ_Event
#endif /* #if defined(CONFIG_OCIRQ_STATIC_CFG) */

IO_ErrorType OCIRQ_InitSync(Comm_Notification callback);
IO_ErrorType OCIRQ_DeInitSync(void);
IO_ErrorType OCIRQ_SetEdgeSync(bool_t edge);
bool_t OCIRQ_GetEdgeSync(void);
IO_ErrorType OCIRQ_IntrptEnableSync(bool_t enable);
void OCIRQ_ChangeEdgeSync(bool_t irqEdgeSync);

void IRQ_ISR(void);

#endif /* __OCIRQ_DRV_H */

/* *********** */
/* End of file */
/* *********** */
