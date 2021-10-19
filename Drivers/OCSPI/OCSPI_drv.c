/*
Copyright (c) 2010-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		    OCSPI - On Chip SPI Source file
\n			    Contains implementation of the OCSPI driver
\n
\file		    OCSPI_drv.c
\ingroup	  OCSPI
\date		    18/07/2011
\version	  00.00.01
\author		  A.Ricci, F.Tarabelloni
\n
\n
\Par\b	History:
\n 
\n 		        00.00.01
\n          	18/07/2011
\n          	A.Ricci, F.Tarabelloni
\n            - Creation on the base of existing files for MC9S08
\n
*/

/* *************************** */
/* Includes                    */
/* *************************** */
#include "OCSPI_drv.h"
#include "LIB_TaskMonitoring.h"

/* *************************** */
/* Defines and Macros          */
/* *************************** */
#define SPI1_ClkEnable()            SPI_ClkEnable(1) 
#define SPI1_ResetStatus()          SPI_ResetStatus(1)
#define SPI1_Enable()               SPI_Enable(1)
#define SPI1_IntEnable()            SPI_IntEnable(1)
#define SPI1_SetBaudRate(b)         SPI_SetBaudRate(1, b)
#define SPI1_MasterEnable()         SPI_MasterEnable(1)
#define SPI1_SetClockPolarity(pol)  SPI_SetClockPolarity(1, pol)
#define SPI1_SetClockPhase(pha)     SPI_SetClockPhase(1, pha)
#define SPI1_SetShiftDir(dir)       SPI_SetShiftDir(1, dir)
#define SPI1_GetStatus()            SPI_GetStatus(1)
#define SPI1_ReadData()             SPI_ReadData(1)
#define SPI1_WriteData(chr)         SPI_WriteData(1, chr)

#define SPI_RX_BUF_SIZE    SPI1_RX_BUF_SIZE_CFG
#define SPI_TX_BUF_SIZE    SPI1_TX_BUF_SIZE_CFG

/* SPI State value list */
#define OVERRUN_ERR      0x01          /* Overrun error flag bit   */
#define CHAR_IN_RX       0x08          /* Char is in RX buffer     */
#define FULL_TX          0x10          /* Full transmit buffer     */
#define RUNINT_FROM_TX   0x20          /* Interrupt is in progress */
#define FULL_RX          0x40          /* Full receive buffer      */

#define ON_ERROR         0x01
#define ON_FULL_RX       0x02
#define ON_RX_CHAR       0x04
#define ON_FREE_TX       0x08
#define ON_TX_CHAR       0x10

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* *************************** */
/* Variables                   */
/* *************************** */
typedef struct {
/** \brief Callback function configuration structure. */
gSPICfg_t Cfg;
/** \brief State of SPI operations. */
uint8_t State;
/** \brief Error during SPI operations. */
uint8_t Error;
/** \brief Decoded error used to export the occurred error. */
gSPIError_t StructError;

/** \brief Input buffer for SPI communication. */
gSPIData_t RxBuffer[SPI_RX_BUF_SIZE];
/** \brief Index for reading from input buffer. */
gSPIDataLen_t RxIndxR;
/** \brief Index for writing to input buffer. */
gSPIDataLen_t RxIndxW;
/** \brief Length of data in Rx buffer. */
gSPIDataLen_t RxBufLen;

/** \brief Output buffer for SPI communication. */
gSPIData_t TxBuffer[SPI_TX_BUF_SIZE];
/** \brief Index for reading from output buffer. */
gSPIDataLen_t TxIndxR;
/** \brief Index for writing to output buffer. */
gSPIDataLen_t TxIndxW;
/** \brief Length of data in Tx buffer. */
gSPIDataLen_t TxBufLen;
} SpiDrv_t;

static SpiDrv_t Spi0;

/* *************************** */
/* Function prototypes         */
/* *************************** */
void OCSPI_InitHwSync(void);
void SPI_OnError(void);
void SPI_OnFreeTxBuf(void);
void SPI_OnFullRxBuf(void);
void SPI_OnRxChar(void);
void SPI_OnTxChar(void);
void SPI_ISR1(void);

/* *************************** */
/* Function bodies             */
/* *************************** */


/*--------------------------------------------------+
 | SPI_SendChar                                     |
 | Send a char from SPI bus                         |
 +--------------------------------------------------*/
IO_ErrorType SPI_SendChar( gSPIData_t chr )
{
  EnterCriticalSection();                             /* Save the PS register */

  if (Spi0.TxBufLen == SPI_TX_BUF_SIZE) {       /* Is number of chars in buffer the same as the size of transmit buffer? */
    ExitCriticalSection();                            /* Restore the PS register */
    return ERR_TXFULL; /** \todo IO_E_TXFULL is better (to add) */
  }

  Spi0.TxBufLen++;                              /* Increase number of bytes in the transmit buffer */
  Spi0.TxBuffer[Spi0.TxIndxW] = chr;             /* Store char to buffer */

  /*MS gSpiTxIndxW = (uint8_t)((gSpiTxIndxW + 1) & (SPI_TX_BUF_SIZE - 1)); */ /* Update index */
  Spi0.TxIndxW = (Spi0.TxIndxW + 1) % (SPI_TX_BUF_SIZE); /* Update index */

  if(!(Spi0.State & RUNINT_FROM_TX))
  {          /* Transmition running ? */
    SPI1_WriteData( Spi0.TxBuffer[Spi0.TxIndxR] ); /* Store char to transmitter register */
    Spi0.State |= RUNINT_FROM_TX;               /* Set RUNINT_FROM_TX flag */
  }
  ExitCriticalSection();                              /* Restore the PS register */
  return IO_E_OK;                              /* OK */
}


/*--------------------------------------------------+ 
 | SPI_RecvChar                                     |
 | Receive a char from SPI bus                      |
 +--------------------------------------------------*/
IO_ErrorType SPI_RecvChar( gSPIData_t *pChr )
{
  uint8_t tmpState;

  if (Spi0.RxBufLen > 0) {                     /* Is number of received chars greater than 0? */
    EnterCriticalSection();                          /* Save the PS register */
    Spi0.RxBufLen--;                           /* Decrease number of received chars */
    *pChr = Spi0.RxBuffer[Spi0.RxIndxR];        /* Read the char */
    Spi0.RxIndxR = (uint8_t)((Spi0.RxIndxR + 1) & (SPI_RX_BUF_SIZE - 1)); /* Update index */
    tmpState = Spi0.State;                     /* Safe the flags */
    Spi0.State &= ~(OVERRUN_ERR | FULL_RX);    /* Clear flag "char in RX buffer" */
    ExitCriticalSection();                           /* Restore the PS register */
  }
  else {
    return ERR_RXEMPTY; /** \todo IO_E_RXEMPTY is better (to add) */
  }
  if ((tmpState & (OVERRUN_ERR | FULL_RX)) != 0) { /* Has the overrun occurred? */
    return ERR_OVERRUN; /** \todo IO_E_OVERRUN is better (to add) */
  } 
  else {
    return IO_E_OK;
  }
}


/*--------------------------------------------------+ 
 | SPI_SendBlock                                    |
 | Send a block of data from SPI bus                |
 +--------------------------------------------------*/
IO_ErrorType SPI_SendBlock( gSPIData_t *pData, uint16_t dataLen, uint16_t *pSnd )
{
  IO_ErrorType result = IO_E_OK;
  uint16_t i;
  for (i=0; i<dataLen; i++) {
    result = SPI_SendChar(*(pData + i));  /* Send one byte */
    if (result != IO_E_OK) {
      break;
    }
  }
  *pSnd = i;                          /* Return number of sent bytes */
  return result;                      /* Return error code */
}


/**
 * SPI_RecvBlock                                    |
 * Receive a block of data from SPI bus             |
 \param  [in]	pData 	- pointer to buffer
 \param  [in]	dataLen - number of bytes to get from buffer
 \param  [in]	pRcv 	- number of bytes to get from buffer
 \return 	IO_E_OK, ERR_RXEMPTY, ERR_OVERRUN
 */
IO_ErrorType SPI_RecvBlock( gSPIData_t *pData, uint16_t dataLen, uint16_t *pRcv )
{
  IO_ErrorType mResult = IO_E_OK;    /* Most serious error */
  IO_ErrorType result;
  uint16_t i = 0;

  while (i < dataLen) {
    result = SPI_RecvChar(pData++);  /* Retrieve byte from Rx buffer */
    if (result > mResult) {          /* Is last error most serious than through error state? */
      mResult = result;              /* If yes then prepare error value to return */
    }
    if ((result != IO_E_OK) && (result != ERR_OVERRUN)) {
      *pRcv = i;                      /* Return number of received bytes */
      return mResult;                 /* Return error code */
    }
    i++;
  }
  *pRcv = i;                          /* Return number of received bytes */
  return mResult;                     /* Return error code */
}


/*--------------------------------------------------+ 
 | SPI_ClearRxBuf                                   |
 | Clear the SPI Rx buffer                          |
 +--------------------------------------------------*/
IO_ErrorType SPI_ClearRxBuf( void )
{
  EnterCriticalSection();                        /* Save the PS register */
  Spi0.RxBufLen = 0;                       /* Set number of bytes in the transmit buffer to 0 */
  Spi0.RxIndxR = Spi0.RxIndxW = 0;          /* Reset indices */
  Spi0.State &= ~(OVERRUN_ERR | FULL_RX);  /* Clear flags */
  ExitCriticalSection();                         /* Restore the PS register */
  return IO_E_OK;                         /* OK */
}


/*--------------------------------------------------+ 
 | SPI_ClearTxBuf                                   |
 | Clear the SPI Tx buffer                          |
 +--------------------------------------------------*/
IO_ErrorType SPI_ClearTxBuf( void )
{
  EnterCriticalSection();                        /* Save the PS register */
  Spi0.TxBufLen = 0;                      /* Set number of bytes in the receive buffer to 0 */
  Spi0.TxIndxR = Spi0.TxIndxW = 0;          /* Reset indices */
  ExitCriticalSection();                         /* Restore the PS register */
  return IO_E_OK;                         /* OK */
}


/*--------------------------------------------------+ 
 | SPI_GetError                                     |
 | Get SPI Error                                    |
 +--------------------------------------------------*/
IO_ErrorType SPI_GetError( gSPIError_t *pError )
{
  EnterCriticalSection();                     /* Save the PS register */
  pError->err = 0;
  pError->errName.OverRun = ((Spi0.Error & OVERRUN_ERR) != 0); /* Overrun error */
  pError->errName.RxBufOvf = ((Spi0.Error & FULL_RX) != 0);    /* Buffer overflow */
  pError->errName.FaultErr = FALSE;    /* not used */  
  Spi0.Error = 0x00;                   /* Reset error flags */
  ExitCriticalSection();                      /* Restore the PS register */
  return IO_E_OK;                      /* OK */
}


/*--------------------------------------------------+ 
 | Test Functions                                   |
 |                                                  |
 +--------------------------------------------------*/
void SPI_OnError(void){
  __asm__("nop");
}

void SPI_OnRxChar(void){
  __asm__("nop");
}

void SPI_OnFullRxBuf(void){
  __asm__("nop");
}

void SPI_OnTxChar(void){
  __asm__("nop");
}

void SPI_OnFreeTxBuf(void){
  __asm__("nop");
}


/*--------------------------------------------------+ 
 | SPI_ISR1                                         |
 |                                                  |
 +--------------------------------------------------*/
void SPI_ISR1(void)
{
  gSPIData_t data;                          /* Temporary variable for data */
  uint8_t flags = 0;                        /* Temporary variable for flags */
  uint8_t status;                           /* Temporary variable for flags */

  IRQ_MONITOR_ENTER();

  status = SPI1_GetStatus();                /* Read the device error register */

  data = SPI1_ReadData();                   /* Read data from receiver */

  if(Spi0.RxBufLen < SPI_RX_BUF_SIZE) {     /* Is number of bytes in the receive buffer lower than size of buffer? */
	Spi0.RxBufLen++;                        /* Increase number of chars in the receive buffer */
    Spi0.RxBuffer[Spi0.RxIndxW] = data;       /* Save received char to the receive buffer */
    Spi0.RxIndxW = (uint8_t)((Spi0.RxIndxW + 1) & (SPI_RX_BUF_SIZE-1)); /* Update index */
    flags |= ON_RX_CHAR;                    /* If yes then set the OnRxChar flag */
    if(Spi0.RxBufLen == SPI_RX_BUF_SIZE) {   /* Is number of bytes in the receive buffer equal to the size of buffer? */
      flags |= ON_FULL_RX;                  /* Set flag "OnFullRxBuf" */
    }
  }
  else {
	  Spi0.State |= FULL_RX;                   /* Set flag "full RX buffer" */
    flags |= ON_ERROR;                      /* Set the OnError flag */
    Spi0.Error |= Spi0.State;                /* Update error flag mirror */
  }
  Spi0.TxBufLen--;                          /* Decrease number of chars in the transmit buffer */
  Spi0.TxIndxR = (uint8_t)((Spi0.TxIndxR + 1) & (SPI_TX_BUF_SIZE-1)); /* Update index */
  if (Spi0.TxBufLen) {                         /* Is number of bytes in the transmit buffer greather then 0? */
    SPI1_WriteData(Spi0.TxBuffer[Spi0.TxIndxR]); /* Store char to transmitter register */
  }
  else {
	  Spi0.State &= ~(RUNINT_FROM_TX|FULL_TX);  /* Clear "running int from TX" and "full TX buff" flags */
    flags |= ON_FREE_TX;                      /* Set the OnFreeTXBuf flag */
  }
  
  if(flags & ON_ERROR) {                          /* Is the error flag set? */
    SPI_OnError();                                /* for test only */
    if (Spi0.Cfg.errorCallback != NULL) {
    	Spi0.Cfg.errorCallback(Spi0.State, &Spi0.StructError); /* Invoke the error callback */
    }
  }
  else {
    if(flags & ON_RX_CHAR) {                      /* Is OnRxChar flag set? */
      SPI_OnRxChar();                             /* for test only */
      if (Spi0.Cfg.rxCallback != NULL) {
    	  Spi0.Cfg.rxCallback(Spi0.State, &Spi0.StructError);  /* Invoke the rx callback */
      }
    }
    if(flags & ON_FULL_RX) {                        /* Is OnTxChar flag set? */
      SPI_OnFullRxBuf();                            /* for test only */
      if (Spi0.Cfg.errorCallback != NULL) {
    	  Spi0.Cfg.errorCallback(Spi0.State, &Spi0.StructError); /* Invoke the error callback */
      }
    }
  }
  
  SPI_OnTxChar();                                 /* for test only */
  if (Spi0.Cfg.txCallback != NULL) {
	  Spi0.Cfg.txCallback(Spi0.State, &Spi0.StructError);      /* Invoke the tx callback */
  }
  
  if (flags & ON_FREE_TX) {                       /* Is flag "OnFreeTxBuf" set? */
    SPI_OnFreeTxBuf();                          
    if (Spi0.Cfg.errorCallback != NULL) {
    	Spi0.Cfg.errorCallback(Spi0.State, &Spi0.StructError); /* Invoke the error callback */
    }
  }

  IRQ_MONITOR_EXIT();

}

#if defined(FREESCALE_MC9S08_QE128)
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
/* Place it in NON_BANKED memory */
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/*INTERRUPT_KEYWORD void OCSPI_ISR1(void)
{
  SPI_ISR1();   
}*/
#pragma CODE_SEG DEFAULT
#endif

/*--------------------------------------------------+ 
 | OCSPI_InitHwSync                                 |
 | Function initializes HW SPI interface            |
 +--------------------------------------------------*/ 
void OCSPI_InitHwSync(void)
{
#if (SPI1_ENABLE == TRUE)

  SPI1_ClkEnable();
  SPI1_ResetStatus();
  
  SPI1_SetBaudRate(SPI1_BAUDRATE);
  
  SPI1_MasterEnable();
  SPI1_SetClockPolarity(SPI1_CLOCK_POLARITY);
  SPI1_SetClockPhase(SPI1_CLOCK_PHASE);
  SPI1_SetShiftDir(SPI1_SHIFT_DIRECTION);
  SPI1_Enable();
  SPI1_IntEnable();

#endif /*SPI1_ENABLE*/
  
} 

/*--------------------------------------------------+ 
 | SPI_ResetSync                                    |
 | Function resets SPI interface                    |
 +--------------------------------------------------*/ 
void SPI_ResetSync( void )
{
	Spi0.State = 0;
	Spi0.Error = 0;
  /* Clean SPI buffers */
  (void)SPI_ClearTxBuf();
  (void)SPI_ClearRxBuf();  

  SPI1_ResetStatus();
}

/*--------------------------------------------------+ 
 | SPI_InitSync                                     |
 | Function initializes SPI interface               |
 +--------------------------------------------------*/ 
IO_ErrorType SPI_InitSync( gSPICfg_t* pInitSync )
{
  /* Init the OCSPI data structure */
	Spi0.TxBufLen = 0;
	Spi0.RxBufLen = 0;
	Spi0.State = 0;
	Spi0.Error = 0;
  
  if (pInitSync != NULL) {
	  Spi0.Cfg.txCallback = pInitSync->txCallback;
	  Spi0.Cfg.rxCallback = pInitSync->rxCallback;
	  Spi0.Cfg.errorCallback = pInitSync->errorCallback;
  }

  /* Hw init */
  OCSPI_InitHwSync();

	return IO_E_OK;  
}
 
/* *************************** */
/* End of file                 */
/* *************************** */
