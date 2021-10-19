/*
Copyright (c) 2009-2010, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n		Implementation of the DMA driver. 
\n
\file		OCDMA_drv.c
\ingroup	OCDMA
\date		14/07/2010
\version	00.00.25
\author		Jaroslav Musil

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
#define __DRIVER_H
/* ******** */
/* Includes */
/* ******** */

#include  "OCDMA_drv.h"
#include "LIB_TaskMonitoring.h"

#ifdef I2C_OCDMA_SUPPORT
#include HAL

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
Comm_Notification OCDMA_callback[OCDMA_CH_NUM];

/* **************** */
/* Local variables */
/* **************** */

/* Structure defining the the timer channel is busy */
static uint8_t DMA_ChResource = 0;

/* ***************** */
/* Exported function */
/* ***************** */

/**
Driver global state initialization initialization.
 
\notes:			Call before using any other method
\return 		IO_ErrorType
\retval			IO_E_OK

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/

IO_ErrorType OCDMA_InitSync(void)
{   
    DMA_ChResource = 0;
    return (IO_ErrorType)IO_E_OK;
}

/**
Deinitialization.
 
\param [in]   IO_ChannelType channel - the channel that should be deinitialized.
 
\return 	  IO_ErrorType

\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID

\notes:

\version		1.0.0
\date 			12/07/2010
\author			Roberto Fioravanti
*/
IO_ErrorType OCDMA_DeInitSync(IO_ChannelType channel)
{
    if (channel >= OCDMA_CH_NUM) 
    {
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    EnterCriticalSection();

    if (DMA_ChResource & (1 << channel))
    {
    	ExitCriticalSection();

        DMA_ControlDefault(channel);
        DMA_ClearTransDone(channel);    
        DMA_SetDevice(channel, 0);
        DMA_SetSAR(channel, 0);
        DMA_SetDAR(channel, 0);    
        
        DMA_ChResource &= ~(1 << channel);
    }
    else
    {
    	ExitCriticalSection();
    }
    return (IO_ErrorType)IO_E_OK;
}


/**
Returns the number of channels. 

\return 	  IO_ChannelType
\retval       number of channels.

\notes:       index range [0..[number of channels)]

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ChannelType OCDMA_GetChannelNumSync(void) 
{  
    return (IO_ChannelType)(OCDMA_CH_NUM);
}

/**
Returns the current destination address of the DMA channel. 

\param [in] 	IO_ChannelType channel

\return 	  OCDMA_DarType
\retval       pointer to the destination address.

\version		1.0.0
\date 			13/07/2010
\author			Jaroslav Musil
*/
OCDMA_DarType OCDMA_GetCurrentDestinationAddress(IO_ChannelType channel) 
{  
    return (OCDMA_DarType)(DMA_GetDAR(channel));
}

/**
Returns the current source address of the DMA channel. 

\param [in] 	IO_ChannelType channel

\return 	  OCDMA_SarType
\retval       pointer to the source address.

\version		1.0.0
\date 			13/07/2010
\author			Jaroslav Musil
*/
OCDMA_SarType OCDMA_GetCurrentSourceAddress(IO_ChannelType channel) 
{  
    return (OCDMA_SarType)(DMA_GetSAR(channel));
}

/**
Verifies the disponibility of the channel.
 
\param [in] 	IO_ChannelType channel - DMA channel
\param [in] 	uint32_t bcr - byte count

 
\return 	  IO_ErrorType
\retval       IO_E_OK       (Available)
\retval       IO_E_INVALID_CHANNEL_ID

\notes:

\version		1.0.0
\date 			14/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_UpdateBCR(IO_ChannelType channel, uint32_t bcr) 
{  
    if (DMA_ChResource & (1 << channel) == 0)
    {
        
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    DMA_SetBCR(OCDMA_GET_CHANNEL_ID(channel), bcr); 

    return (IO_ErrorType)IO_E_OK;
}



/**
Verifies the disponibility of the channel.
 
\param [in] 	IO_ChannelType channel
 
\return 	  IO_ErrorType
\retval       IO_E_BUSY     (Not available)
\retval       IO_E_OK       (Available)
\retval       IO_E_INVALID_CHANNEL_ID

\notes:

\version		1.0.0
\date 			13/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCDMA_CheckChannelAvailabilitySync(IO_ChannelType channel)
{
    if (channel >= OCDMA_CH_NUM) 
    {
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    if (DMA_ChResource & (1 << channel))
    {
        return (IO_ErrorType)IO_E_BUSY;
    }

    return (IO_ErrorType)IO_E_OK;
}


/**
The fuction verifies if the transaction is done.

\param [in]   IO_ChannelType channel - the channel that should be verified

\return 	  IO_ErrorType

\retval       IO_E_OK
\retval       IO_E_BUSY
\retval       IO_E_INVALID_CHANNEL_ID

\notes:

\version		1.0.1
\date 			12/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_CheckChannelDoneSync(IO_ChannelType channel)
{
    if (channel >= OCDMA_CH_NUM) 
    {
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    /* Checks if the transaction of the particular channel is done */
    if (DMA_IsTransDone(channel))
    {
        return (IO_ErrorType)IO_E_OK;
    }
      
    return (IO_ErrorType)IO_E_BUSY;
}


/**
Assigns the device the DMA channel.

\param [in]   OCDMA_TransferType* transfer

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID - channel out of range
\retval       IO_E_BUSY - channel is already assigned

\notes:

\version		1.0.1
\date 			13/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_SetChannelDeviceSync(const OCDMA_TransferType* transfer)
{
    if (OCDMA_GET_CHANNEL_ID(transfer -> channel) >= OCDMA_CH_NUM) 
    {
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    EnterCriticalSection();

    if (DMA_ChResource & (1 << OCDMA_GET_CHANNEL_ID(transfer -> channel)))
    {                          
    	ExitCriticalSection();
        
        return (IO_ErrorType)IO_E_BUSY;
    }

    /* Assing the DMA channel */
    DMA_ChResource |= (1 << OCDMA_GET_CHANNEL_ID(transfer -> channel));
    
    ExitCriticalSection();

    DMA_SetDevice((uint8_t) OCDMA_GET_CHANNEL_ID(transfer -> channel), (uint8_t) OCDMA_GET_CHANNEL_DEVICE(transfer -> channel));

    return (IO_ErrorType)IO_E_OK;
}

/**
Configures the transfer parameters of the particular DMA channel:
Destination and source incrementation, destination and source data type, peripheral reqeust,
cycle steal, disable peripheral request, 

\param [in]   OCDMA_TransferType* transfer

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID - channel not assigned
\retval       IO_E_INVALID_VALUE - wrong configuration

\notes:

\version		1.0.1
\date 			13/07/2010
\author			Jaroslav Musil
*/

IO_ErrorType OCDMA_SetTransferParamSync(const OCDMA_TransferType* transfer)
{
    uint8_t channel;

    channel = (uint8_t) OCDMA_GET_CHANNEL_ID(transfer -> channel);

    if (DMA_ChResource & (1 << channel) == 0)
    {
        
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

    /* Source data type */
    if ((transfer -> addr & OCDMA_ADDR_SSIZE) == OCDMA_ADDR_SSIZE_LONG)
    {
        DMA_SetSourceSizeToLong(channel);
    }
    else if ((transfer -> addr & OCDMA_ADDR_SSIZE) == OCDMA_ADDR_SSIZE_WORD)
    {
        DMA_SetSourceSizeToWord(channel); 
    }
    else
    {
        DMA_SetSourceSizeToByte(channel); 
    }

    /* Source address increment */
    if (transfer -> addr & OCDMA_ADDR_SINC)
    {
        DMA_EnableSourceIncrement(channel);
    }
    else
    {
        DMA_DisableSourceIncrement(channel);
    }

    /* Destination data type */
    if ((transfer -> addr & OCDMA_ADDR_DSIZE) == OCDMA_ADDR_DSIZE_LONG)
    {
        DMA_SetDestinationSizeToLong(channel);
    }
    else if ((transfer -> addr & OCDMA_ADDR_DSIZE) == OCDMA_ADDR_DSIZE_WORD)
    {
        DMA_SetDestinationSizeToWord(channel); 
    }
    else
    {
        DMA_SetDestinationSizeToByte(channel); 
    }
    
    /* Destination address increment */
    if (transfer -> addr & OCDMA_ADDR_DINC)
    {
        DMA_EnableDestinationIncrement(channel);
    }
    else
    {
        DMA_DisableDestinationIncrement(channel);
    }

    /* Data auto-align */
    if (transfer -> addr & OCDMA_ADDR_AA)
    {
        DMA_EnableAutoAlign(channel);
    }
    else
    {
        DMA_DisableAutoAlign(channel); 
    }

    /* Single/Continuous data transfer */
    if (transfer -> addr & OCDMA_ADDR_CS)
    {
        /* One transfer per request */
        DMA_SingleTransfer(channel); 
    }
    else
    {
        /* Continuous data transfer */
        DMA_ContinuousTransfer(channel);
    }


    /* Link channel 2 mode */
    DMA_LinkChannel2Mode(channel, OCDMA_GET_PERLINK_LCH2(transfer -> perlink));

    /* Link channel 1 mode */
    DMA_LinkChannel1Mode(channel, OCDMA_GET_PERLINK_LCH1(transfer -> perlink));

    /* Link channel control mode */
    DMA_LinkChannelControlMode(channel, OCDMA_GET_PERLINK_LINKCC(transfer -> perlink));

    /* Disable request */
    if (transfer -> perlink & OCDMA_PERLINK_D_ERQ)
    {
        /* The peripheral request is ignored as soon as the BCR gets zero */
        /* i.e. the erq is turned off automatically */
        DMA_SetDisableRequestOn(channel);
    }
    else
    {
        /* The peripheral request is not touched (erq) */
        DMA_SetDisableRequestOff(channel);
    }

    /* Destination address modulo */
    DMA_SetDestinationAddressModulo(channel, OCDMA_GET_MODULO_DMOD(transfer -> modulo))

    /* Source address modulo */
    DMA_SetSourceAddressModulo(channel, OCDMA_GET_MODULO_SMOD(transfer -> modulo))



    /* Configuration error check */
    if (DMA_IsConfigError(channel) == 0)
    {
        return (IO_ErrorType)IO_E_OK;
    }
    else
    {
        return  (IO_ErrorType)IO_E_INVALID_VALUE;
    }
}

/**
Configures the address transfer parameters of the particular DMA channel:
Destination and source address, byte counter and clears the transaction done flag. 

\param [in]   OCDMA_TransferType* transfer

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID - channel not assigned
\retval       IO_E_INVALID_VALUE - wrong configuration

\notes:

\version		1.0.1
\date 			13/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_SetTransferSync(const OCDMA_TransferType* transfer)
{
    uint8_t channel;

    channel = (uint8_t) OCDMA_GET_CHANNEL_ID(transfer -> channel);


    if (DMA_ChResource & (1 << channel) == 0)
    {
        
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }

 
    if((!transfer -> dar) && (!transfer -> sar) && (!transfer -> bcr))
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }

    /* Clears the transaction done flag */
    DMA_ClearTransDone(channel);

    /* Set destination address */
    DMA_SetDAR(channel, (uint32_t)transfer -> dar); 
  
    /* Set source address */
    DMA_SetSAR(channel, (uint32_t)transfer -> sar);
  
    /* Set data size */
    DMA_SetBCR(channel, transfer -> bcr); 

    return (IO_ErrorType)IO_E_OK;
}

/**
Configures the byte count register only and clears the transaction done flag.

\param [in]   OCDMA_TransferType* transfer

\return       IO_ErrorType
\retval       IO_E_OK
\retval       IO_E_INVALID_CHANNEL_ID - channel not assigned
\retval       IO_E_INVALID_VALUE - wrong configuration

\notes:

\version		1.0.0
\date 			13/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_FollowTransferSync(const OCDMA_TransferType* transfer)
{
    if (DMA_ChResource & (1 << OCDMA_GET_CHANNEL_ID(transfer -> channel)) == 0)
    {
        return (IO_ErrorType)IO_E_UNKNOWN_MODE;
    }

    if ((!transfer -> bcr))
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }

    /* Clears the transaction done flag */
    DMA_ClearTransDone(OCDMA_GET_CHANNEL_ID(transfer -> channel));

    /* Set data size */
    DMA_SetBCR(OCDMA_GET_CHANNEL_ID(transfer -> channel), transfer -> bcr); 

    return (IO_ErrorType)IO_E_OK;
}


/**
Prepares the particular DMA channel for transmission start. If the callback parameter is passed
into the function, it is set up as an callback when the interrupt of the DMA channel arrives;
in this case the DMA interrupt is enabled, otherwise the interrupt is disabled. If the sw_start is set then
the transfer is initiated by software start, otherwise the peripheral request initiates
the transfer.

\param [in]   OCDMA_TransferType* transfer
\param [in]   Comm_Notification callback

\return       IO_ErrorType
\retval       IO_E_OK        ()
\retval       IO_E_INVALID_CHANNEL_ID
\retval       IO_E_INVALID_VALUE

\notes:

\version		1.0.1
\date 			14/07/2010
\author			Jaroslav Musil
*/
IO_ErrorType OCDMA_StartTransferSync(const uint8_t swStart, const OCDMA_TransferType* transfer, Comm_Notification callback)
{
    uint16_t errorConfig;
    uint8_t channel;

    channel = (uint8_t) OCDMA_GET_CHANNEL_ID(transfer -> channel);

    if (DMA_ChResource & (1 << channel) == 0)
    {
        
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }


    /* Checks the configuration error of the channel */
    errorConfig = (uint16_t) DMA_IsConfigError(channel);

    /* Configuration error check */
    if (!errorConfig == 0)
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }

    /* Assignes the channel's ISR */
    OCDMA_callback[channel] = callback;

    /* if the channel's callback exists */
    if (OCDMA_callback[channel])
    {
        /* its interrupt is enabled */
        DMA_EnableInt(channel);  
    } 
    else
    {
        /* otherwise its interrupt is disabled */
        DMA_DisableInt(channel);    
    }

    /* Peripheral request */
    if (transfer -> perlink & OCDMA_PERLINK_ERQ)
    {
        /* The peripheral request is accepted */
        DMA_PeripheralRequestOn(channel);
    }
    else
    {
        /* Peripheral request is ignored */
        DMA_PeripheralRequestOff(channel);
    }


    if (swStart != 0)
    {
        /* Initiates the DMA transfer, cleared on the next clock cycle */
        DMA_SetStartOn(channel);
    }


    return (IO_ErrorType)IO_E_OK;  
}

/**
Interrupt service routine of DMA channel 0. The function clears the transaction done flag
and calls the assigned callback function.
 
\param [in] 	void

\version		1.0.1
\date 			14/07/2010
\author			Jaroslav Musil

\note
\par AG128
interrupt 68
\todo check if the keyword interrupt cuold be place in cofig file
*/

void OCDMA_CH0_ISR(void)
{
	IRQ_MONITOR_ENTER();

    DMA_ClearTransDone(0);    
    (OCDMA_callback[0])((IO_NotificationType)IO_N_TRANSFER_DONE, 0, (IO_ErrorType)IO_E_OK);

	IRQ_MONITOR_EXIT();
}

/**
Interrupt service routine of DMA channel 1.  The function clears the transaction done flag
and calls the assigned callback function.
 
\param [in] 	void

\version		1.0.1
\date 			14/07/2010
\author			Jaroslav Musil

\note
\par AG128
interrupt 69
\todo check if the keyword interrupt cuold be place in cofig file
*/
void OCDMA_CH1_ISR(void)
{
	IRQ_MONITOR_ENTER();

    DMA_ClearTransDone(1);
    (OCDMA_callback[1])((IO_NotificationType)IO_N_TRANSFER_DONE, 1, (IO_ErrorType)IO_E_OK);

	IRQ_MONITOR_EXIT();
}

/**
Interrupt service routine of DMA channel 2.  The function clears the transaction done flag
and calls the assigned callback function.
 
\param [in] 	void

\version		1.0.1
\date 			14/07/2010
\author			Jaroslav Musil

\note
\par AG128
interrupt 70
\todo check if the keyword interrupt cuold be place in cofig file
*/
void OCDMA_CH2_ISR(void)
{
	IRQ_MONITOR_ENTER();

    DMA_ClearTransDone(2);
    (OCDMA_callback[2])((IO_NotificationType)IO_N_TRANSFER_DONE, 2, (IO_ErrorType)IO_E_OK);

	IRQ_MONITOR_EXIT();
}

/**
Interrupt service routine of DMA channel 3.  The function clears the transaction done flag
and calls the assigned callback function.
 
\param [in] 	void
 
\notes:

\version		1.0.1
\date 			14/07/2010
\author			Jaroslav Musil
\note
\par AG128
interrupt 71
\todo check if the keyword interrupt cuold be place in cofig file
*/
void OCDMA_CH3_ISR(void)
{
	IRQ_MONITOR_ENTER();

    DMA_ClearTransDone(3);
    (OCDMA_callback[3])((IO_NotificationType)IO_N_TRANSFER_DONE, 3, (IO_ErrorType)IO_E_OK);

	IRQ_MONITOR_EXIT();
}

#endif /* I2C_OCDMA_SUPPORT */
/* *********** */
/* End of file */
/* *********** */
