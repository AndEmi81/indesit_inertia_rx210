/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Description
\n		UART Driver.
\n
\file		OCUART_drv.h
\ingroup	OCUART
\date		06/07/2009
\version	00.00.03
\author	developper Roberto Fioravanti
\n
\n
\n
\n
\par		History (titolo)
\n 		00.00.59
\n		09/09/2010
\n		developper Roberto Fioravanti
\n		changes
\n		- addition prototype OCUART_TxByteSync
\n
\n 		00.00.45
\n		06/05/2010
\n		developper Roberto Fioravanti
\n		changes
\n		- changed prototype (OCUART_RxStreamMngr)
\n		00.00.32
\n		24/02/2010 
\n		developper Roberto Fioravanti
\n		changes 
\n 		- includes updated
\n		- addiction of OCUART_RxStreamMngr prototype
\n 		00.00.03
\n		06/07/2009 <data in cui � stata realizzata la versione corrente>
\n		developper Roberto Fioravanti
\n		changes 
\n 		- creation
\n
\n etc.
*/

/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/





#ifndef __OCUART_DRV_H
#define __OCUART_DRV_H


/* ******** */
/* Includes */
/* ******** */
#include  "Config.h"
#include  "Hal_Interface.h"

#include  HAL_CFG

#include "io_driver.h"


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
IO_ErrorType OCUART_InitSync(IO_PhyInitSyncType * initSync);
IO_ErrorType OCUART_InitSWSync(void);
IO_ErrorType OCUART_DeInitSync(void * deinitSync);
void OCUART_EnableSync(void);
void OCUART_DisableSync(void);
#if defined(OCUART_USE_SET_BAUDRATESYNC)
void OCUART_SetBaudRateSync(UART_BaudRateType);
#endif
IO_ErrorType OCUART_RxSync(CommDriverParamType * param);
IO_ErrorType OCUART_TxSync(CommDriverParamType * param);
void OCUART_RxDataMngr(void);
void OCUART_ErrMngr(void);
void OCUART_TxDataMngr(void);
void OCUART_TxByteSync(uint8_t data);
void OCUART_OutputStreamMngr(IO_ChannelType ch);
uint8_t OCUART_RxStreamMngr(IO_ChannelType ch);
void OCUART_ReadData(uint8_t *data);
IO_ErrorType OCUART_CallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);
uint8_t OCUART_GetError(void);
void OCUART_RxDataMngrEx(void);
void OCUART_TxDataMngrEx(void);
#endif /* __OCUART_DRV_H */



/* *********** */
/* End of file */
/* *********** */
