/*
Copyright (c) 2009-2010, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/



/**
\n
\par		Description
\n		On chip UART driver (\ref OCUART_Class) implementation. Peripheral configuration, data transmission and reception.
		Data transmission and reception is managed through DMA. SW DMA driver is used if not available
		On chip DMA. The DMA channel used CPU-dependent.
		\warning On chip DMA tested for Freescale AG128
\n
\file		OCUART_drv.c
\ingroup	OCUART
\date		06/07/2009
\version	00.00.01
\author		Roberto Fioravanti
\n
\n
\par		History
\n 		00.00.45
\n		06/05/2010
\n		developper Roberto Fioravanti
\n		changes
\n 		- faster STX syncro (OCUART_RxStreamMngr)
\n		- changed prototype (OCUART_RxStreamMngr)
\n 		00.00.41
\n		06/07/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- cast void form memcopy functions
\n 		00.00.25
\n		06/07/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- bug fix: param to OCUART_ReadData in OCUART_RxStreamMngr
\n
\n 		00.00.03
\n		06/07/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- Creazione
*/



#define __DRIVER_H
/* ******** */
/* Includes */
/* ******** */
#include <string.h>

#include "OCUART_drv.h"
#include HAL

#if !defined(_HAL_DMA_CAPABILITY)  || !defined(UART_OCDMA_SUPPORT)
#include "SWDMA_drv.h"
#else
#include "OCDMA_drv.h"
#endif /* !defined(_HAL_DMA_CAPABILITY) */

#include "LIB_TaskMonitoring.h"

#if defined(HAL_UART_SUPPORT)

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined(OCUART_BUFFER_SIZE)
typedef uint8_t OCUART_CircularBufferType;
#else
typedef uint32_t OCUART_CircularBufferType;
#endif

#define UART_DEVICE						HAL_UART_DEV		/*impostazione da configurazione*/
#if defined(OCUART_BUFFER_SIZE)
#define OCUART_CIRCULAR_BUFFER_SIZE     OCUART_BUFFER_SIZE
#else
#define OCUART_BUFFER_SIZE              (OCUART_BUFFER_LSIZE*4)
#define OCUART_CIRCULAR_BUFFER_SIZE     OCUART_BUFFER_LSIZE
#endif




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t OCUART_State;

typedef struct OCUART_tag {
    uint8_t *pHead; /* Puntatore alla testa dell'InpBuffer (scrittura) */
    uint8_t *pTail; /* Puntatore alla coda dell'InpBuffer  (lettura) */
    uint8_t *pRead;
    Comm_Notification     EOO_callback;         /* End Of Operation callback */

    IO_MemPtrType         TxBuffer;             /* pnt buffer trasmissione */
    IO_MemPtrType         RxBuffer;             /* pnt buffer ricezione */
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
    IO_MemPtrType         CircBuffer;           /* pnt buffer trasmissione */
#endif    
#if (!defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)) && defined(UART_CHECK_RXBUFFER_OVERFLOWS)    
    IO_MemPtrType         pMaxRxBuffer;           /* */
#endif    
    IO_PacketSizeType       TxLen;              /* numero byte da trasmettere */
    IO_PacketSizeType    	RxLen;              /* numero byte da ricevere */
    IO_ChannelType        DmaCh;                /* canale dma */
    I2C_SlaveAddressType  CurrentSlave;         /* indirizzo slave corrente comunicazione */

    IO_OptionType        options;
    IO_OptionType        toptions;
    OCUART_State           state;
    IO_ChannelType        id;                   /* Driver channel ID */

    /* slave */
    Comm_Notification      notifyCB;            /* notification callback */
    uint8_t InputLen;
    uint8_t MngrState;
    uint8_t ErrorFound;
}OCUART_Type;

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static uint8_t OCUART_Data2Read(void);
static uint8_t OCUART_GetDataFromTail(void);
static void RemoveTail(void);
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
static IO_ErrorType UartRxCallback(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
#endif /* #if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT) */
/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */


OCUART_CircularBufferType OCUART_Buffer[OCUART_CIRCULAR_BUFFER_SIZE];
OCUART_Type OCUART;


/* ***************** */
/* Exported function */
/* ***************** */
/**
UART driver variables initialization.

\return 		IO_ErrorType
\retvalue       IO_E_OK - successful
\warning

\version		1.0.0
\date 			08/07/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_InitSWSync(void)
{

	#if !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)
	OCUART.pHead = OCUART.pTail = OCUART.pRead = (uint8_t*)OCUART_Buffer;
	#endif
    
	OCUART.InputLen=0;
	OCUART.MngrState=0;
	OCUART.ErrorFound=0;
	OCUART.RxLen=0;
	(void)memset(OCUART_Buffer, 0, sizeof(OCUART_Buffer));
	return IO_E_OK;
}

/**
Driver UART initialization


\param [in] 	BCP_PhyInitSyncType * initSync

\return 		IO_ErrorType
\retvalue   IO_E_OK - successful
\warning

\version		1.0.0
\date 			08/07/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_InitSync(IO_PhyInitSyncType * initSync)
{
  
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
  OCDMA_TransferType  transfer = {0};

  OCUART.pHead =  OCUART.pTail = OCUART.pRead = (uint8_t*)OCUART_Buffer;

  OCUART.CircBuffer = (uint8_t*)OCUART_Buffer;
#endif

	OCUART.notifyCB=initSync->eventCB;
	OCUART.id=initSync->id;
#if (!defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)) && defined(UART_CHECK_RXBUFFER_OVERFLOWS)
	OCUART.pMaxRxBuffer = initSync->pMaxRxData;
#endif	

    UART_ClkDisable(UART_DEVICE);
	
	(void)memset(OCUART_Buffer, 0, sizeof(OCUART_Buffer));
    
    
    
	OCUART_DisableSync(); 		/* disabilita per impostare il clock */
	UART_ClkEnable(UART_DEVICE);

	UART_ResetPeError(UART_DEVICE)

	/*periferica*/
#if HAL_UART_DATALEN
	UART_SetParityVal(UART_DEVICE,	val)
#endif


	UART_SetParityMode	    (UART_DEVICE,		HAL_UART_PARITY)
	UART_SetDataLen			(UART_DEVICE,		HAL_UART_DATALEN)
	UART_SetStop			(UART_DEVICE,		HAL_UART_STOP)
	UART_SetMode			(UART_DEVICE,		HAL_UART_MODE)
	UART_SetClockDivisor    (UART_DEVICE,		HAL_UART_BAUDRATE_DIVISOR);

	UART_SetBaudRate (UART_DEVICE, _HALUART_CALC_BAUD(HAL_OPTION_UART_BAUDRATE,	HAL_UART_DIVISOR));



#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
#if (UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE)
	  transfer.bcr = 1024;
    transfer.sar = UART_pReceiveDataRegister(UART_DEVICE);
    transfer.dar = initSync -> rxbuffer;

    transfer.channel = OCDMA_CHANNEL_ID(UART_RX_DMA_CH) | OCDMA_CHANNEL_DEVICE(UART_RX_DMA_DEV);
    transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;
    transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;  
    transfer.modulo = OCDMA_MODULO_DMOD(3);
    
    OCDMA_SetChannelDeviceSync(&transfer);
    OCDMA_SetTransferParamSync(&transfer);    
    OCDMA_SetTransferSync(&transfer);
    OCDMA_StartTransferSync(0, &transfer, NULL);

    UART_RxDMAEnable(UART_DEVICE);
#else
    transfer.bcr = OCUART_BUFFER_SIZE;
    transfer.sar = UART_pReceiveDataRegister(UART_DEVICE);
    //transfer.dar = initSync -> rxbuffer;
    transfer.dar = (uint8_t*)OCUART_Buffer;

    transfer.channel = OCDMA_CHANNEL_ID(UART_RX_DMA_CH) | OCDMA_CHANNEL_DEVICE(UART_RX_DMA_DEV);
    transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;
    transfer.perlink = OCDMA_PERLINK_ERQ;  
    
    OCDMA_SetChannelDeviceSync(&transfer);
    OCDMA_SetTransferParamSync(&transfer);    
    OCDMA_SetTransferSync(&transfer);
    OCDMA_StartTransferSync(0, &transfer, UartRxCallback);

    UART_RxDMAEnable(UART_DEVICE);
#endif 	/*	(UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE) */
#endif	/* defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT) */

	OCUART_EnableSync();

	return IO_E_OK;
}

#if defined(OCUART_USE_SET_BAUDRATESYNC)
/**
Sets the baud rate.
*/
void OCUART_SetBaudRateSync(UART_BaudRateType baudRateRegType) 
{
	UART_SetBaudRate(UART_DEVICE, _HALUART_CALC_BAUD(baudRateRegType, HAL_UART_DIVISOR));
}
#endif

/**
Enables UART peripheral (both RX and TX) and initilizes sw variables.

\version		1.0.0
\date 			08/07/2009
\author			Roberto Fioravanti
*/
void OCUART_EnableSync(void)
{
	(void)OCUART_InitSWSync();
	
	UART_ClkEnable(UART_DEVICE);
	UART_Enable(UART_DEVICE) 	/**\todo deve dipendere da UART_DEVICE*/
	UART_TxEnable				(UART_DEVICE)
	UART_RxEnable				(UART_DEVICE)

	UART_RxIntEnable		(UART_DEVICE)
}

/**
Disables UART peripheral (RX and TX) and iitializaes variables.

\version		1.0.0
\date 			08/07/2009
\author			Roberto Fioravanti
*/
void OCUART_DisableSync(void)
{
    UART_ClkDisable(UART_DEVICE);  				/* if not implemented, UART_Disable() does it*/
	UART_Disable				(UART_DEVICE)
	UART_TxDisable				(UART_DEVICE)
	UART_RxDisable				(UART_DEVICE)

	UART_RxIntDisable			(UART_DEVICE)
	UART_TxIntDisable			(UART_DEVICE)
	
	(void)OCUART_InitSWSync();
	
}


/**
Deinitializes UART driver

\param [in] 	void * deinitSync (ignored)

\return 		IO_ErrorType
\retvalue   IO_E_OK
\warning

\version		1.0.0
\date 			07/07/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_DeInitSync(void * deinitSync)
{
    (void) deinitSync;
    
	UART_ClkDisable(UART_DEVICE);
	OCUART_DisableSync();

    return IO_E_OK;
}


/**
Callback (slave mode)
\param [in] 	IO_NotificationType notifVal notification code
\param [in]     IO_ChannelType channel communication channel
\param [in]     IO_ErrorType error

\version		00.00.32
\date 			24/02/2010
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_CallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{
    (void) error;
    (void) channel;
    (void) notifVal;
    
  if(OCUART.notifyCB)
  {
    if(OCUART.toptions & IO_CLOSE_TX)
    {
      /* notifica */
		(void)(OCUART.notifyCB)((IO_NotificationType)IO_N_COMPLETED, OCUART.id, (IO_ErrorType)IO_E_OK);
    } else
    {
    	(void)(OCUART.notifyCB)((IO_NotificationType)IO_N_TX_DONE, OCUART.id, (IO_ErrorType)IO_E_OK);
    }
  }
	return IO_E_OK;
}

/**
Transmits data

\param [in] 	CommDriverParamType * param transmit paramenters

\return 		IO_ErrorType
\retvalue   IO_E_INVALID_VALUE: buffer o len non validi
            IO_E_BUSY: bus occupato (solo master)
            IO_E_OK: trasmissione avviata regolarmente.

\warning

\notes:

\version		1.0.0
\date 			07/07/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_TxSync(CommDriverParamType * param)
{
#if !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)
	SWDMA_TransferType  transfer={0};
#else
    OCDMA_TransferType  transfer = {0};
#endif /* !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT) */
  
	if(!param->buffer) 
	{
		return IO_E_INVALID_VALUE;
	}

	if(!param->len)
	{
		return IO_E_INVALID_VALUE;
	}

	OCUART.toptions=param->option;

	UART_TxEnable(UART_DEVICE);

    transfer.bcr=param->len;
    transfer.sar=param->buffer;
    transfer.dar=UART_pTransmitDataRegister(UART_DEVICE);
#if !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)
    transfer.channel=1;        /** \todo search for the channel */
    transfer.options.optionbyte = 	SWDMA_OPTION_SSIZE_BYTE|
    								 SWDMA_OPTION_DSIZE_BYTE|
    								 SWDMA_OPTION_SINC|
    								 SWDMA_OPTION_START|
    								 SWDMA_OPTION_CS;
    (void)SWDMA_ChannelEnableSync(1);
    (void)SWDMA_SetTransferSync(&transfer);

    (void)SWDMA_StartTransferSync(1, OCUART_CallBack);
#else
    transfer.channel = OCDMA_CHANNEL_ID(UART_TX_DMA_CH) | OCDMA_CHANNEL_DEVICE(UART_TX_DMA_DEV);
    transfer.addr = OCDMA_ADDR_SINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;
    transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;    

    UART_TxDMAEnable(UART_DEVICE);

    OCDMA_SetChannelDeviceSync(&transfer);
    OCDMA_SetTransferParamSync(&transfer);    
    OCDMA_SetTransferSync(&transfer);
    OCDMA_StartTransferSync(0, &transfer, param -> callback);


#endif	/* !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT) */
    UART_TxIntEnable			(UART_DEVICE)

	return IO_E_OK;
}

/**
Transmits a single byte and waits for transmission completed.

\param [in] 	data uint8_t byte to be sent

\version		1.0.0
\date 			13/10/2009
\author			Roberto Fioravanti
*/
void OCUART_TxByteSync(uint8_t data)
{
	while(UART_IsTxDataRegFull(UART_DEVICE)){
	}
	UART_WriteData(UART_DEVICE, data);
}

/**
Starts reception.


\param [in] 	CommDriverParamType * param

\return 		IO_ErrorType
\retvalue   IO_E_INVALID_VALUE: buffer o len invalid (I2C e DMA)
            IO_E_INVALID_CHANNEL_ID:  DMA channel invalid
            IO_E_OK: success
\warning

\notes:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCUART_RxSync(CommDriverParamType * param)
{
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
    OCDMA_TransferType  transfer = {0};
#endif
	if((!param->buffer) || (!param->len))
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	OCUART.RxBuffer=param->buffer;
	OCUART.RxLen=param->len;
	OCUART.options=param->option;

	return (IO_ErrorType)IO_E_OK;
}


uint8_t OCUART_GetError(void)
{                       
    uint8_t err=(uint8_t)(OCUART.ErrorFound & 6);
    
    return err;
}

void OCUART_ErrMngr(void)
{
	if(UART_IsError(UART_DEVICE))
	{
		/** \todo gestione errori OCUART */
		if(UART_IsOerError(UART_DEVICE))
		{
		    OCUART.ErrorFound |=2;
		} else if(UART_IsFerError(UART_DEVICE))
		{
		    OCUART.ErrorFound |=4;
		    
		}else
		{
		    OCUART.ErrorFound |=1;
		}
		
		UART_ClearError(UART_DEVICE)
	}
}



void OCUART_TxDataMngrEx(void)
{
	/*if(UART_IsTxEvent(event))*/
	{
		UART_ClearTxIrq(UART_DEVICE);

#if !defined(_HAL_DMA_CAPABILITY)
		if(SWDMA_CheckChannelEnableSync(1) == IO_STATE_ACTIVE)
		{
			(void)SWDMA_ExeTransferMngrSync(1);
		} else
#endif /* !defined(_HAL_DMA_CAPABILITY) */
		{
			UART_TxIntDisable			(UART_DEVICE);
			UART_TxDisable(UART_DEVICE);
		}
	}
}


void OCUART_TxDataMngr(void)
{
	IRQ_MONITOR_ENTER();

	if(UART_IsTxInt(UART_DEVICE) )
	{
		UART_ClearTxIrq(UART_DEVICE);

#if !defined(_HAL_DMA_CAPABILITY)
		if(SWDMA_CheckChannelEnableSync(1) == IO_STATE_ACTIVE)
		{
			(void)SWDMA_ExeTransferMngrSync(1);
		} else
#endif /* !defined(_HAL_DMA_CAPABILITY) */
		{
			UART_TxIntDisable			(UART_DEVICE);
			UART_TxDisable(UART_DEVICE);
		}
	}

	IRQ_MONITOR_EXIT();
}

/**
Manages the circular buffer.
*/
void OCUART_RxDataMngr(void)
{
	IRQ_MONITOR_ENTER();

	if(UART_IsRxInt(UART_DEVICE))
	{
		*OCUART.pHead = UART_ReadData(UART_DEVICE);
		OCUART.pHead++;

		if ( OCUART.pHead >= ( (uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE) )
		{
			OCUART.pHead = (uint8_t*)OCUART_Buffer;
		}
		if ( OCUART.pHead == OCUART.pTail )
		{
			OCUART.pTail++;
			if ( OCUART.pTail >= ( (uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE) )
			{
		    	OCUART.pTail = (uint8_t*)OCUART_Buffer;
		  }
		}
		if( OCUART.InputLen < OCUART_BUFFER_SIZE )
		{
			OCUART.InputLen++;
		}
	}

	IRQ_MONITOR_EXIT();

}




/**
Manages the circular buffer.
*/
void OCUART_RxDataMngrEx(void)
{
	IRQ_MONITOR_ENTER();

	/*if(UART_IsRxEvent(event))*/
	{
		*OCUART.pHead = UART_ReadData(UART_DEVICE);
		OCUART.pHead++;

		if ( OCUART.pHead >= ( (uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE) )
		{
			OCUART.pHead = (uint8_t*)OCUART_Buffer;
        }
		if ( OCUART.pHead == OCUART.pTail )
		{
			OCUART.pTail++;
			if ( OCUART.pTail >= ( (uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE) )
			{
		    	OCUART.pTail = (uint8_t*)OCUART_Buffer;
		  }
		}
		if( OCUART.InputLen < OCUART_BUFFER_SIZE )
		{
			OCUART.InputLen++;
		}
	}

	IRQ_MONITOR_EXIT();

}


/**
(not yet implemented)
*/
void OCUART_OutputStreamMngr(IO_ChannelType ch)
{
    (void) ch;
	while(OCUART.TxLen--)
	{
		/*\todo send data */

	}
	/*\todo specific application code: example NewSysteMode */
	/*\todo enable rx and rx Int*/
}

/**
If any data received, this method passes by pointer the first character \n
extracted from the queue,
otherwise it returns error code (it does not wait for data)\n

\param  [out]  \b Chr  pointer to char

\return \c uint8_t
\author 	AA.VV.
\date		molto tempo fa
\version	1.0.0
\attention \b during the char extraction uart rx interrupt is disabled

\note		\par \b Nota 1:
			documented by Roberto Fioravanti
			\par \b Nota 2:
			Target Dependent
*/
void OCUART_ReadData(uint8_t *data)
{
	UART_RxIntDisable(UART_DEVICE);

   	*data = *OCUART.pTail;               			/* Received char */
   /* INCREMENTO TAIL */
	if ( OCUART.InputLen )
	{
		OCUART.pTail++;
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
        if (OCUART.pTail >= (OCUART.CircBuffer + OCUART_BUFFER_SIZE))
		{
			OCUART.pTail = OCUART.CircBuffer;
		}
#else	
		OCUART.InputLen--;
		if ( OCUART.pTail >= ( (uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE) )
		{
			OCUART.pTail = (uint8_t*)OCUART_Buffer;
		}		
#endif /* defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)*/
	}

   	OCUART.pRead=OCUART.pTail;

	UART_RxIntEnable(UART_DEVICE);
}


/**
*/
uint8_t OCUART_RxStreamMngr(IO_ChannelType ch)
{
uint8_t data2read=0;
uint8_t scan=1;
(void) ch;
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
    
#if (UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE)
    OCUART.pHead = OCDMA_GetCurrentDestinationAddress(UART_RX_DMA_CH);
#else
    if (OCDMA_GetCurrentDestinationAddress(UART_RX_DMA_CH) < OCUART.CircBuffer + OCUART_BUFFER_SIZE)
    {
        OCUART.pHead = OCDMA_GetCurrentDestinationAddress(UART_RX_DMA_CH);
    }
#endif /* (UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE)*/
    
    /*
    OCUART.InputLen=(uint8_t) (OCUART.pHead-OCUART.pTail);
    
    if(OCUART.pHead<OCUART.pTail)
    {
        OCUART.InputLen+=OCUART_BUFFER_SIZE;
    }
    */  
    
#if (UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE)    
    OCDMA_UpdateBCR(UART_RX_DMA_CH, 1024);
#endif /* (UART_AUTOMATIC_CIRCULAR_BUFFER == TRUE) */

#endif
    
    while(scan)
    {	
    
    
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
    OCUART.InputLen=(uint8_t) (OCUART.pRead-OCUART.pHead);
    
    if(OCUART.pRead<OCUART.pHead)
    {
        OCUART.InputLen+=OCUART_BUFFER_SIZE;
    }
#endif




    	switch(OCUART.MngrState)
    	{
    		case 0:
    			/*idle*/
    			OCUART.RxLen=0;

    			if(OCUART_Data2Read())
    			{
    				OCUART.MngrState+=1;
    				data2read=TRUE;
    				
    				/* Init data reception */
    				if(OCUART.notifyCB)
    				{
    				  /* notify about Read data init */
    					(void)(OCUART.notifyCB)(IO_N_READ_DATA_INIT, OCUART.id, IO_E_OK);
    				}

    			}
    			else
    			{
    				scan=0;
    			}
    			break;

    		case 1:
    		{
    				/*macchina a stati di ricezione*/
    		    if(!OCUART.RxLen && OCUART.notifyCB)
    		    {
    				IO_ErrorType err=(OCUART.notifyCB)(IO_N_TRANSFER_DONE, OCUART.id, IO_E_OK);
    			      /* notifica */
                    switch(err)
                    {
                    case IO_E_OK:
                    case IO_E_CHECK_PP:						
                    	OCUART.MngrState++;
                    	RemoveTail();
                    	scan=0;
                    	break;

                    case IO_E_INVALID_VALUE:					
                    	{
                    		/* leggi rimuovendo il dato dalla coda */
                    		uint8_t pdata;
                    		OCUART_ReadData(&pdata);
                    		OCUART.MngrState=0;
                    	}
                      	break;
                    default: /*IO_E_WORKING*/
                     
                    	if(!OCUART_Data2Read())
                    	{
                    		scan=0;
                    	}
                    	
                    }
    		    }
    		 
    		}
    			break;
    		case 2:
    			scan=0;
    			/* risposta */
    			if(!OCUART.RxLen && OCUART.notifyCB)
    			{
    			  /* notify about Read data init */
    			  (void)(OCUART.notifyCB)(IO_N_WRITE_DATA_INIT, OCUART.id, IO_E_OK);
    				OCUART.MngrState=0;
    			}
    			break;
    			
    			
    		default:
    			
    			OCUART.MngrState=0;
    			break;
    	}

    	if(OCUART.ErrorFound)
    	{
    		(void)OCUART_InitSWSync();
    	}
    			

    	if(OCUART_Data2Read())
    	{
    		if(OCUART.RxLen)
    		{
    				
    				uint8_t data = OCUART_GetDataFromTail();
    				
    				*OCUART.RxBuffer++=data;
    				data2read=TRUE;
    				
    			    OCUART.RxLen--;
        			
#if !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)    				
#if defined(UART_CHECK_RXBUFFER_OVERFLOWS)
    				if(OCUART.pMaxRxBuffer && OCUART.RxBuffer>OCUART.pMaxRxBuffer)
    				{
    				    (void)OCUART_InitSWSync();
    				} 
#endif    				
#endif    		
    		}
    	}
    	else
    	{
    	    scan=0;
    	}
    }
	if(!OCUART.RxLen)
	{
		data2read=TRUE;
	}
		
	return data2read;
}

/* ************** */
/* Local Function */
/* ************** */
/**
tests if there are byte to read in the queue
\return		FALSE if there are no bytes
			TRUE if there is at least one byte to read
\attention \b

\author 	AA.VV.
\date		molto tempo fa
\version	1.0.0
\todo aggiornare il commento
\note
*/
static uint8_t OCUART_Data2Read(void)
{
#if !defined(_HAL_DMA_CAPABILITY) || !defined(UART_OCDMA_SUPPORT)
	return (OCUART.pRead != OCUART.pHead);
#else	
	return (OCUART.InputLen);
#endif	
}


/**
reads the queue and updates the reading pointer
\return		\b uint8_t queue reading pointer before the update
\attention \b

\author 	AA.VV.
\date		molto tempo fa
\version	1.0.0

\todo aggiornare il commento
*/
static uint8_t OCUART_GetDataFromTail(void)
{
	uint8_t data;

	data=*OCUART.pRead;

	if( OCUART.pRead == OCUART.pHead )
	{
		return(data);
	}
	OCUART.pRead++;
		
#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)

	if (OCUART.pRead >= (OCUART.CircBuffer + OCUART_BUFFER_SIZE))
	{
    	OCUART.pRead = OCUART.CircBuffer;
    }
#else    
	if (OCUART.pRead >= ((uint8_t*)OCUART_Buffer + OCUART_BUFFER_SIZE))
	{
    	OCUART.pRead = (uint8_t*) OCUART_Buffer;
    }
#endif	
	return(data);
}

/**
clears the queue.

\attention \b rx interrupts are disabled during the operation

\author 	AA.VV.
\date		molto tempo fa
\version	1.0.0

\note		\par \b Nota 1:
			documented by Roberto Fioravanti
			\par \b Nota 2:
			Target Dependent
*/
static void RemoveTail(void)
{

	UART_RxIntDisable(UART_DEVICE)

	if ( OCUART.pRead < OCUART.pTail )
	{
		OCUART.InputLen -= (uint8_t)OCUART_BUFFER_SIZE;
	}

	OCUART.InputLen -= (uint8_t)(OCUART.pRead-OCUART.pTail);

	OCUART.pTail=OCUART.pRead;

	UART_RxIntEnable(UART_DEVICE)
}

#if defined(_HAL_DMA_CAPABILITY) && defined(UART_OCDMA_SUPPORT)
static IO_ErrorType UartRxCallback(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
#if (UART_AUTOMATIC_CIRCULAR_BUFFER == FALSE)
    OCDMA_TransferType  transfer = {0};

	UART_RxIntDisable(UART_DEVICE);
	
	transfer.bcr = OCUART_BUFFER_SIZE;
    transfer.sar = UART_pReceiveDataRegister(UART_DEVICE);
    transfer.dar = OCUART.CircBuffer;
    transfer.channel = OCDMA_CHANNEL_ID(UART_RX_DMA_CH);

    OCDMA_SetTransferSync(&transfer);    


    UART_RxIntEnable(UART_DEVICE);
#endif


    (void) notifVal;
    (void) chid;
    (void) error;

    return IO_E_OK;
}
#endif

#endif /* defined(HAL_UART_SUPPORT) */
/* *********** */
/* End of file */
/* *********** */
