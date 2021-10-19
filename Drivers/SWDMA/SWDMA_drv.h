/**
\defgroup SWDMA

Contiene l'implementazione del driver DMA sw

\par \b PCLint: x Errors, x Warnings: indentation, x Info 24/09/2008 nome cognome


\date    12/03/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup IODriver
*/

/**
Header file del file OCI2C_drv.c

\file    SWDMA_drv.h
\ingroup SWDMA
\date    12/03/2009
\version	00.00.30
\author  Roberto Fioravanti
\n
\n
\n
\n
\par		History
\n
\n 		00.00.01
\n		12/03/2009
\n		Roberto Fioravanti
\n		changes
\n 		- creation
\n
\n
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





#ifndef __SWDMA_DRV_H
#define __SWDMA_DRV_H


/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "Hal_Interface.h"
#include  HAL_CFG
#include "IO_driver.h"                               /* For: Application configuration */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define SWDMA_OPTION_START				(1<<0)
#define SWDMA_OPTION_CS					(1<<1)
#define SWDMA_OPTION_SINC				(1<<2)
#define SWDMA_OPTION_SSIZE_BYTE			(1<<3)
#define SWDMA_OPTION_SSIZE_WORD			(2<<3)
#define SWDMA_OPTION_SSIZE_LONG			(0)
#define SWDMA_OPTION_DINC				(1<<5)
#define SWDMA_OPTION_DSIZE_BYTE			(1<<6)
#define SWDMA_OPTION_DSIZE_WORD			(2<<6)
#define SWDMA_OPTION_DSIZE_LONG			(0)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef void (* SWDMA_Callback)(IO_ChannelType, IO_ErrorType);


typedef uint8_t * 	SWDMA_DarType;
typedef uint8_t * 	SWDMA_SarType;
typedef uint16_t  	SWDMA_BcrType;



typedef union SWDMA_AddressesOpts_tag_u{
                uint8_t optionbyte;
} SWDMA_AddressesOptsTypeU;

typedef struct SWDMA_Transfer_tag{
  IO_ChannelType channel;
  SWDMA_SarType sar;                  /**< source address */
  SWDMA_DarType dar;                  /**< destination address */
  SWDMA_BcrType bcr;                  /**< byte count register  */
  SWDMA_AddressesOptsTypeU options;
}SWDMA_TransferType;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
IO_ErrorType SWDMA_InitSync(void* unused);
IO_ErrorType SWDMA_DeInitSync(void* address);
IO_ChannelType SWDMA_GetChannelNumSync(void);
IO_ErrorType SWDMA_CheckChannelAvailabilitySync(IO_ChannelType channel);
IO_ErrorType SWDMA_ChannelDeviceSync(IO_ChannelType channel);
IO_ErrorType SWDMA_ChannelEnableSync(IO_ChannelType channel);
IO_ErrorType SWDMA_SetChannelDisableSync(IO_ChannelType channel);
IO_ErrorType SWDMA_ExeTransferMngrSync(IO_ChannelType channel);
IO_ErrorType SWDMA_SetTransferSync(SWDMA_TransferType*);
IO_ErrorType SWDMA_CheckChannelDoneSync(IO_ChannelType channel);
IO_DeviceStateType SWDMA_CheckChannelEnableSync(IO_ChannelType channel);
IO_ErrorType SWDMA_StartTransferSync(IO_ChannelType channel, Comm_Notification callback);
IO_ErrorType SWDMA_ChannelDisableSync(IO_ChannelType channel);
#endif /* __SWDMA_DRV_H */



/* *********** */
/* End of file */
/* *********** */
