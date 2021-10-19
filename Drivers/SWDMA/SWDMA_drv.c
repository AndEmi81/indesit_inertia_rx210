/*
Copyright (c) 2009-2010, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\file		SWDMA_drv.c
\ingroup	SWDMA
\date		05/2009
\version	00.00.01
\author		Roberto Fioravanti

\n
\n
\par		History
\n 		00.00.30
\n		20/02/2010
\n		developper Roberto Fioravanti
\n		changes
\n 		- addiction of word and long transfer capability (SWDMA_SIZE_8BITs_TRANSFER, SWDMA_SIZE_16BITs_TRANSFER, SWDMA_SIZE_32BITs_TRANSFER)
\n    - cycle steal functionality addiction (SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT)
\n    - peripheral initiated transfer support
\n
\n
\n 		00.00.12
\n		08/10/2009
\n		developer Roberto Fioravanti
\n		changes
\n 		- structures updating to standard types
\n
\n 		00.00.01
\n		30/09/2009
\n		developer Roberto Fioravanti
\n		changes
\n 		- creation
*/

/* ******** */
/* Includes */
/* ******** */
#include "string.h"

#include  "SWDMA_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define SWDMA_OPTION_SSIZE	(3<<3)
#define SWDMA_OPTION_DSIZE	(3<<6)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** struttura DMA */
typedef struct TD_tag{
  Comm_Notification callback;

  uint8_t *          SAR;     /* registro indirizzo di sorgente */
  uint8_t *          DAR;     /* registro indirizzo di destinazione */
  SWDMA_BcrType      BCR;     /* registro conteggio byte */
    struct status_tag{
    uint8_t done:1;                /* 1: operazione completata, non completata altrimenti */
    }status;

  SWDMA_AddressesOptsTypeU options;
  struct ctl_tag{
  uint16_t dev   :3;           /* device (hw only) */
  uint16_t en    :1;           /* 1: DMA abilitato, non abilitato altrimenti */
  }ctl;
}SWDMA_TransferDescriptorType;





/* **************************** */
/* Prototype of local functions */
/* **************************** */
IO_ErrorType SWDMA_SetChannelDeviceSync(IO_ChannelType channel);
IO_ErrorType SWDMA_SingleTransfer(IO_ChannelType channel);
void SWDMA_StatusManager(IO_ChannelType channel);



/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
SWDMA_TransferDescriptorType  SWDMA_Data[SWDMA_CH_NUM];

/* ***************** */
/* Exported function */
/* ***************** */

/**
Initialization.

\param [in] 	void* address (ignored)

\return 		  IO_ErrorType
\retval       IO_E_OK

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_InitSync(void* address)
{
    (void) address;
    return (IO_ErrorType)IO_E_OK;
}

/**
de-inizializzazione.

\param [in] 	void* address (ignored)

\return 		  IO_ErrorType
\retval       IO_E_OK

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_DeInitSync(void* address)
{
  (void) address;
  (void)memset(SWDMA_Data,0,sizeof(SWDMA_Data));

  return (IO_ErrorType)IO_E_OK;
}



/**
Ritorna il numero di canali.

\return 		  IO_ChannelType
\retval       number of channels.

\notes:       index range [0..[number of channels-1)]

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ChannelType SWDMA_GetChannelNumSync(void)
{
  return (IO_ChannelType)(SWDMA_CH_NUM);
}

/**
Verifica la disponibilità del canale.

\param [in] 	IO_ChannelType channel
canale

\return 		  IO_ErrorType
\retval       IO_E_BUSY     (Not available)
\retval       IO_E_OK       (Available)
\retval       IO_E_INVALID_CHANNEL_ID

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_CheckChannelAvailabilitySync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(SWDMA_Data[channel].ctl.dev)
  {
    return (IO_ErrorType)IO_E_BUSY;
  }

  return (IO_ErrorType)IO_E_OK;
}

/**
Enables the channel
\param [in]   channel, channel to be enabled

\return 		  IO_ErrorType
\retval       IO_E_OK       (Available)
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_ChannelEnableSync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  SWDMA_Data[channel].ctl.en=1;

  return (IO_ErrorType)IO_E_OK;
}

/**
Disable the channel
\param [in]   channel to be enabled

\return 		  IO_ErrorType
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_ChannelDisableSync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  SWDMA_Data[channel].ctl.en=0;

  return (IO_ErrorType)IO_E_OK;
}


/**
Checks if the channel is enabled

\param [in]   IO_ChannelType channel.
canale per cui chiedere la verifica.
\return 		  IO_DeviceStateType
\retval       IO_STATE_IDLE
\retval       IO_STATE_ACTIVE
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_DeviceStateType SWDMA_CheckChannelEnableSync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(SWDMA_Data[channel].ctl.en)
  {
    return (IO_ErrorType)IO_STATE_ACTIVE;
  }

  return (IO_ErrorType)IO_STATE_IDLE;
}


/**
Checks if done

\param [in]   IO_ChannelType channel.
canale per cui chiedere la verifica.
\return 		  IO_DeviceStateType
\retval       IO_E_OK: done
\retval       IO_E_BUSY: still working
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_CheckChannelDoneSync(IO_ChannelType channel)
{
  if(channel > SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(SWDMA_Data[channel].status.done)
  {
    return (IO_ErrorType)IO_E_OK;
  }

  return (IO_ErrorType)IO_E_BUSY;
}


/**
Set the device .

\param [in]   channel

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_BUSY
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_SetChannelDeviceSync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(SWDMA_Data[channel].ctl.dev)
  {
    return (IO_ErrorType)IO_E_BUSY;
  }

  SWDMA_Data[channel].ctl.dev=1;

  return (IO_ErrorType)IO_E_OK;
}

/**
Set Transfer parameters.

\param [in]   SWDMA_SetDarSync

\return       IO_ErrorType
\retval       IO_E_OK: params accepted
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM
\retval       IO_E_INVALID_VALUE:  dar and/or sar in transfer are null.

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_SetTransferSync(SWDMA_TransferType* transfer)
{
  if(transfer->channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if((!transfer->dar) && (!transfer->sar))
  {
    return (IO_ErrorType)IO_E_INVALID_VALUE;
  }

  SWDMA_Data[transfer->channel].DAR=transfer->dar;
  SWDMA_Data[transfer->channel].SAR=transfer->sar;
  SWDMA_Data[transfer->channel].BCR=transfer->bcr;
  SWDMA_Data[transfer->channel].options.optionbyte = transfer->options.optionbyte;

  return (IO_ErrorType)IO_E_OK;
}

/**
Starts the trasfer

\param [in]   SWDMA_StartTransferType

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID: channel value beyond SWDMA_CH_NUM
\retval       IO_E_INVALID_VALUE: BCR is zero;

\par Example: software initiated trasfer with I2C
\code
    SWDMA_TransferType  transfer = {0};

    ...

    transfer.bcr = OCI2C.TxLen;
    transfer.sar = OCI2C.TxBuffer;
    transfer.dar = I2C_pTransmitDataRegister();
    transfer.channel = OCI2C.DmaCh;

    transfer.options.optionbyte =	SWDMA_OPTION_SSIZE_BYTE|
									SWDMA_OPTION_DSIZE_BYTE|
									SWDMA_OPTION_SINC|
									SWDMA_OPTION_CS|
									SWDMA_OPTION_START;

    (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
    (void)SWDMA_SetTransferSync(&transfer);

\endcode

\par Example: peripheral initiated with I2C
\code

    SWDMA_TransferType  transfer = {0};

    ...

    transfer.bcr = OCI2C.TxLen;
    transfer.sar = OCI2C.TxBuffer;
    transfer.dar = I2C_pTransmitDataRegister();
    transfer.channel = OCI2C.DmaCh;

    transfer.options.optionbyte =	SWDMA_OPTION_SSIZE_BYTE|
									SWDMA_OPTION_DSIZE_BYTE|
									SWDMA_OPTION_DINC|
									SWDMA_OPTION_CS;

    (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
    (void)SWDMA_SetTransferSync(&transfer);

\endcode

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_StartTransferSync(IO_ChannelType channel, Comm_Notification callback)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(SWDMA_Data[channel].BCR == 0)
  {
    return (IO_ErrorType)IO_E_INVALID_VALUE;
  }

  SWDMA_Data[channel].status.done=0;

  SWDMA_Data[channel].callback=callback;



  if(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_START)
  {
		SWDMA_StatusManager(channel);
#if defined(SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT) && (SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT==TRUE)
	if(	! (SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_CS) )
    {
      SWDMA_ExeTransferMngrSync(channel);
    }
#endif /*	(SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT)  */

    return SWDMA_SingleTransfer(channel);
  }
  return IO_E_OK;
}

/**
Executes the transfer.

\param [in]   SWDMA_StartTransferType

\return       IO_ErrorType
\retval       IO_E_OK        ()
\retval       IO_E_INVALID_CHANNEL_ID
\retval       IO_E_INVALID_VALUE

\par Example: Interrupt management of SWDMA
\code

#if !defined(_HAL_DMA_CAPABILITY)
  if(SWDMA_CheckChannelEnableSync(\c channel) == (IO_ErrorType)IO_STATE_ACTIVE)
  {
    (void)SWDMA_ExeTransferMngrSync(\c channel);
  } else
#endif // !defined(_HAL_DMA_CAPABILITY)
  {

    ...
  }
\endcode
\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_ExeTransferMngrSync(IO_ChannelType channel)
{
  if(channel>SWDMA_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

#if defined(SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT) && (SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT==TRUE)
	  if(	! (SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_CS) )
  {
    memcpy(SWDMA_Data[channel].DAR, SWDMA_Data[channel].SAR, SWDMA_Data[channel].BCR);
    SWDMA_Data[channel].BCR = 0;
    SWDMA_Data[channel].status.done=1;
  }
#endif /*	(SWDMA_CONTINUOUS_MODE_TRANSFER_SUPPORT)  */

    if(SWDMA_Data[channel].status.done == 1)
  {
    SWDMA_Data[channel].ctl.en=0;

    if(SWDMA_Data[channel].callback)
    {
    	(void)(SWDMA_Data[channel].callback)((IO_NotificationType)IO_N_TRANSFER_DONE, channel, (IO_ErrorType)IO_E_OK);
    }

    return (IO_ErrorType)IO_E_OK;
  }

  SWDMA_StatusManager(channel);

  return SWDMA_SingleTransfer(channel);
}

/* ************** */
/* Local Function */
/* ************** */

/**
SWDMA_SingleTransfer.

\param [in] 	IO_ChannelType channel

\return 		  IO_ErrorType
\retval       IO_E_OK

\notes:

\version		1.0.0
\date 			09/07/2008
\author			Roberto Fioravanti
*/
IO_ErrorType SWDMA_SingleTransfer(IO_ChannelType channel)
{
  SWDMA_SarType sar=SWDMA_Data[channel].SAR;
  SWDMA_DarType dar=SWDMA_Data[channel].DAR;

  if(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SINC)
  {
#if		(SWDMA_FIXED_SIZE == TRUE)
		SWDMA_Data[channel].SAR++;
#else
    switch(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE)
    {
#if	(SWDMA_SIZE_8BITs_TRANSFER)
      case SWDMA_OPTION_SSIZE_BYTE:
        SWDMA_Data[channel].SAR++;
      break;
#endif	/*	(SWDMA_SIZE_8BITs_TRANSFER) */

#if		(SWDMA_SIZE_16BITs_TRANSFER)
      case SWDMA_OPTION_SSIZE_WORD:
        SWDMA_Data[channel].SAR+=2;
      break;
#endif	/*	(SWDMA_SIZE_16BITs_TRANSFER)			*/

#if		(SWDMA_SIZE_32BITs_TRANSFER)
      case SWDMA_OPTION_SSIZE_LONG:
        SWDMA_Data[channel].SAR+=4;
      break;
#endif	/*	(SWDMA_SIZE_16BITs_TRANSFER)			*/
    }
#endif		/*(SWDMA_FIXED_SIZE == TRUE)*/
  }

  if(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_DINC)
  {
#if	(SWDMA_FIXED_SIZE)
		SWDMA_Data[channel].DAR++;
#else
    switch(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_DSIZE)
    {
#if	(SWDMA_SIZE_8BITs_TRANSFER)
      case SWDMA_OPTION_DSIZE_BYTE:
        SWDMA_Data[channel].DAR++;
      break;
#endif	/*	(SWDMA_SIZE_8BITs_TRANSFER) */
#if	(SWDMA_SIZE_16BITs_TRANSFER)
      case SWDMA_OPTION_DSIZE_WORD:
        SWDMA_Data[channel].DAR+=2;
      break;
#endif	/*	(SWDMA_SIZE_16BITs_TRANSFER)*/
#if	(SWDMA_SIZE_32BITs_TRANSFER)
      case SWDMA_OPTION_DSIZE_LONG:
        SWDMA_Data[channel].DAR+=4;
      break;
#endif	/*	(SWDMA_SIZE_32BITs_TRANSFER) */

		}
#endif /*(SWDMA_FIXED_SIZE == TRUE)*/
  }

#if (SWDMA_FIXED_SIZE)
	*dar=*sar;
#else
  switch(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE)
  {
#if	(SWDMA_SIZE_8BITs_TRANSFER)
    case SWDMA_OPTION_SSIZE_BYTE:
    *dar=*sar;
    break;
#endif	/*	(SWDMA_SIZE_8BITs_TRANSFER)*/
#if		(SWDMA_SIZE_16BITs_TRANSFER)
    case SWDMA_OPTION_SSIZE_WORD:
    *((uint16_t*)dar)=*((uint16_t*)sar);
    break;
#endif	/*	(SWDMA_SIZE_16BITs_TRANSFER)*/
#if		(SWDMA_SIZE_32BITs_TRANSFER)
    case SWDMA_OPTION_SSIZE_LONG:
    *((uint32_t*)dar)=*((uint32_t*)sar);
    break;
#endif	/*	(SWDMA_SIZE_32BITs_TRANSFER)	*/
  }
#endif /*(SWDMA_FIXED_SIZE == TRUE)*/
  return (IO_ErrorType)IO_E_OK;
}

/**
SWDMA_StatusManager.

\param [in] 	IO_ChannelType channel

\notes:

\version		1.0.0
\date 			09/07/2008
\author			Roberto Fioravanti
*/
void SWDMA_StatusManager(IO_ChannelType channel)
{
#if		defined(SWDMA_SIZE_32BITs_TRANSFER)
  if(SWDMA_Data[channel].BCR <= ((SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE) == 0x00?
								  0x04:
								  (SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE)>>3))
#else
	if(SWDMA_Data[channel].BCR <= ((SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE)>>3))
#endif	/*		(SWDMA_SIZE_32BITs_TRANSFER)	*/
  {
    SWDMA_Data[channel].status.done=1;
  }

  if (SWDMA_Data[channel].status.done == 0)
  {
#if (SWDMA_FIXED_SIZE)
	SWDMA_Data[channel].BCR--;
#else
	switch(SWDMA_Data[channel].options.optionbyte & SWDMA_OPTION_SSIZE)
	{
#if	(SWDMA_SIZE_8BITs_TRANSFER)
		case SWDMA_OPTION_SSIZE_BYTE:
			SWDMA_Data[channel].BCR--;
		break;
#endif	/*	(SWDMA_SIZE_8BITs_TRANSFER)		*/
#if	(SWDMA_SIZE_16BITs_TRANSFER)
		case SWDMA_OPTION_SSIZE_WORD:
			SWDMA_Data[channel].BCR -= 2;
		break;
#endif	/*	(SWDMA_SIZE_16BITs_TRANSFER)	*/
#if	(SWDMA_SIZE_32BITs_TRANSFER)
		case SWDMA_OPTION_SSIZE_LONG:
			SWDMA_Data[channel].BCR -= 4;
		break;
#endif	/*	(SWDMA_SIZE_32BITs_TRANSFER)		*/
	}

#endif /* (SWDMA_FIXED_SIZE)	*/
	}
}

/* *********** */
/* End of file */
/* *********** */
