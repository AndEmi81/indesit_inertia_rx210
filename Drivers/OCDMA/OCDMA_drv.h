/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
Header file of the file OCDMA_drv.c

\file    OCDMA_drv.h
\ingroup OCDMA
\date    14/07/2010
\author  Jaroslav Musil
\version 0.0.25

\n
\n
\par		History

\n 		00.00.25
\n		14/07/2010 
\n		developper Jaroslav Musil
\n		changes
\n 		- added functions
\n      - transacation done flag clearening before BCR update and in the interrupt functions
\n      - the setting structure changed from the use of bitfields to OR/AND masks
\n
\n 		00.00.24
\n		01/12/2009 
\n		developper Jaroslav Musil
\n		changes
\n 		- implementaion of the hw DMA
\n
\n 		00.00.12
\n		08/10/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- aggiornamento delle strutture ai tipi standard
\n
\n 		00.00.01
\n		30/09/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- creazione
*/




#ifndef __OCDMA_DRV_H
#define __OCDMA_DRV_H


/* ******** */
/* Includes */
/* ******** */

#include "Config.h"                               /* For: Application configuration */

#include  "Hal_Interface.h"
#include  HAL_CFG

#include "io_driver.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define OCDMA_SIZE_8BITs			0x01
#define OCDMA_SIZE_16BITs			0x02
#define OCDMA_SIZE_32BITs			0x00

#define OCDMA_ADDR_CS					(1<<0)
#define OCDMA_ADDR_AA					(1<<1)
#define OCDMA_ADDR_DINC					(1<<2)
#define OCDMA_ADDR_DSIZE_BYTE   		(1<<3)
#define OCDMA_ADDR_DSIZE_WORD   		(2<<3)
#define OCDMA_ADDR_DSIZE_LONG   		(0)
#define OCDMA_ADDR_DSIZE           		(3<<3)
#define OCDMA_ADDR_SINC					(1<<5)
#define OCDMA_ADDR_SSIZE_BYTE   		(1<<6)
#define OCDMA_ADDR_SSIZE_WORD   		(2<<6)
#define OCDMA_ADDR_SSIZE_LONG   		(0)
#define OCDMA_ADDR_SSIZE           		(3<<6)

#define OCDMA_PERLINK_ERQ				        (1<<0)
#define OCDMA_PERLINK_D_ERQ				        (1<<1)
#define OCDMA_PERLINK_LINKCC_NO			        (0)
#define OCDMA_PERLINK_LINKCC_LCH1CS_LCH2BCR0    (1<<2)
#define OCDMA_PERLINK_LINKCC_LCH1CS             (2<<2)
#define OCDMA_PERLINK_LINKCC_LCH1BCR0           (3<<2)
#define OCDMA_PERLINK_LCH1(channel)             (channel<<4)
#define OCDMA_PERLINK_LCH2(channel)             (channel<<6)
#define OCDMA_GET_PERLINK_LINKCC(perlink)       ((perlink>>2) & 3)
#define OCDMA_GET_PERLINK_LCH1(perlink)         ((perlink>>4) & 3)
#define OCDMA_GET_PERLINK_LCH2(perlink)         (perlink>>6)

#define OCDMA_MODULO_SMOD(modoption)            (modoption<<0)
#define OCDMA_MODULO_DMOD(modoption)            (modoption<<4)
#define OCDMA_GET_MODULO_SMOD(modulo)           (modulo & 0xf)
#define OCDMA_GET_MODULO_DMOD(modulo)           (modulo >> 4)


#define OCDMA_CHANNEL_ID(channel)               (channel<<0)
#define OCDMA_GET_CHANNEL_ID(channel)           (channel & 0xf)
#define OCDMA_CHANNEL_DEVICE(device)            (device<<4)
#define OCDMA_GET_CHANNEL_DEVICE(channel)       (channel >> 4)



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef void (* OCDMA_Callback)(IO_ChannelType, IO_ErrorType);


typedef uint8_t * 	OCDMA_DarType; /* Pointer to an 8-bit data */
typedef uint8_t * 	OCDMA_SarType; /* Pointer to an 8-bit data */
typedef uint32_t  	OCDMA_BcrType; /*IO_ValueType*/

/* DMA channel transfer structure */
/* Needs to be in this order to be the largest data in the first place */
typedef struct OCDMA_Transfer_tag
{
  OCDMA_BcrType bcr;                /* 32-bit */
  OCDMA_SarType sar;                /* 32-bit, 16-bit or 8-bit depending on the core */
  OCDMA_DarType dar;                /* 32-bit, 16-bit or 8-bit depending on the core */
  uint8_t addr;                     /* 8-bit, address type options */       
  uint8_t perlink;                  /* 8-bit, peripheral and link options */         
  uint8_t modulo;                   /* 8-bit, address modulo options */         
  uint8_t channel;                  /* 8-bit, channel and peripheral assignment */
} OCDMA_TransferType;



/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
IO_ErrorType OCDMA_InitSync(void);
IO_ErrorType OCDMA_DeInitSync(IO_ChannelType channel);
IO_ChannelType OCDMA_GetChannelNumSync(void);
OCDMA_DarType OCDMA_GetCurrentDestinationAddress(IO_ChannelType channel);
OCDMA_SarType OCDMA_GetCurrentSourceAddress(IO_ChannelType channel);
IO_ErrorType OCDMA_UpdateBCR(IO_ChannelType channel, uint32_t bcr);
IO_ErrorType OCDMA_CheckChannelAvailabilitySync(IO_ChannelType channel);
IO_ErrorType OCDMA_SetChannelDeviceSync(const OCDMA_TransferType* transfer);
IO_ErrorType OCDMA_SetTransferParamSync(const OCDMA_TransferType* transfer);
IO_ErrorType OCDMA_SetTransferSync(const OCDMA_TransferType* transfer);
IO_ErrorType OCDMA_FollowTransferSync(const OCDMA_TransferType* transfer);
IO_ErrorType OCDMA_CheckChannelDoneSync(IO_ChannelType channel);
IO_ErrorType OCDMA_StartTransferSync(const uint8_t swStart, const OCDMA_TransferType* transfer, Comm_Notification callback);

void OCDMA_CH0_ISR(void);
void OCDMA_CH1_ISR(void);
void OCDMA_CH2_ISR(void);
void OCDMA_CH3_ISR(void);


#endif /* __OCDMA_DRV_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
