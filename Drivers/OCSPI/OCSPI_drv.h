/*
Copyright (c) 2010-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup	  OCSPI
@{
\Par		    Serial Peripheral Interface driver.
\n		        Contains all the necessary functions to control the ports of the controller
\n		        The list of SPI pin declaration must be created in Config.c.
@}
\ingroup    Drivers
*/

/**
\Par		    OCSPI - On Chip SPI Header file
\n			    Contains defines, typedef, prototypes of the SPI driver
\n
\file		    OCSPI_drv.h
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

/**
\addtogroup     OCSPI
@{*/

#ifndef __OCSPI_HAL_H
#define __OCSPI_HAL_H

#define __DRIVER_H

/* *************************** */
/* Includes                    */
/* *************************** */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL
#include "IO_driver.h"


/* *************************** */
/* Defines and Macros          */
/* *************************** */
#define ERR_OK           0             /* OK */
#define ERR_SPEED        1             /* This device does not work in the active speed mode. */
#define ERR_RANGE        2             /* Parameter out of range. */
#define ERR_VALUE        3             /* Parameter of incorrect value. */
#define ERR_OVERFLOW     4             /* Timer overflow. */
#define ERR_MATH         5             /* Overflow during evaluation. */
#define ERR_ENABLED      6             /* Device is enabled. */
#define ERR_DISABLED     7             /* Device is disabled. */
#define ERR_BUSY         8             /* Device is busy. */
#define ERR_NOTAVAIL     9             /* Requested value or method not available. */
#define ERR_RXEMPTY      10            /* No data in receiver. */
#define ERR_TXFULL       11            /* Transmitter is full. */
#define ERR_BUSOFF       12            /* Bus not available. */
#define ERR_OVERRUN      13            /* Overrun error is detected. */
#define ERR_FRAMING      14            /* Framing error is detected. */
#define ERR_PARITY       15            /* Parity error is detected. */
#define ERR_NOISE        16            /* Noise error is detected. */
#define ERR_IDLE         17            /* Idle error is detected. */
#define ERR_FAULT        18            /* Fault error is detected. */
#define ERR_BREAK        19            /* Break char is received during communication. */
#define ERR_CRC          20            /* CRC error is detected. */
#define ERR_ARBITR       21            /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define ERR_PROTECT      22            /* Protection error is detected. */
#define ERR_UNDERFLOW    23            /* Underflow error is detected. */
#define ERR_UNDERRUN     24            /* Underrun error is detected. */
#define ERR_COMMON       25            /* Common error of a device. */
#define ERR_LINSYNC      26            /* LIN synchronization error is detected. */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* SPI Class type definitions */
typedef uint8_t gSPIState_t;
typedef uint8_t gSPIData_t;
typedef uint8_t gSPIDataLen_t;
typedef union {
  uint8_t err;
  struct {
    bool8_t OverRun  : 1;            /* OverRun error flag - the data overflow on the input has been detected. Both hardware detection (if supported) and software detection (when the value of Input buffer size property is 0) is used. */
    bool8_t RxBufOvf : 1;            /* Rx buffer full error flag - the input circular buffer defined by the Input buffer size property has overrun. */
    bool8_t FaultErr : 1;            /* Fault mode error flag - only if supported by hardware */
  } errName;
} gSPIError_t;
typedef void (*gpSPICallback_t)(gSPIState_t state, gSPIError_t* error); 

/**
 * On chip SPI class.
 */
typedef struct gSPICfg_tag {
  gpSPICallback_t txCallback;
  gpSPICallback_t rxCallback;
  gpSPICallback_t errorCallback;  
} gSPICfg_t;
/* *************************** */
/* Exported variables          */
/* *************************** */


/* *************************** */
/* Function prototypes         */
/* *************************** */
#if defined(FREESCALE_MC9S08_QE128)
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
/* Place it in NON_BANKED memory */
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */ 
/* INTERRUPT_KEYWORD void OCSPI_ISR1(void);	*/
#pragma CODE_SEG DEFAULT
#endif
/**
\par    Description:
\n      This function is used to send a character to SPI bus.
\param  [in] chr - character send to SPI
\retval IO_E_OK - operation processed successful
\retval ERR_TXFULL - Tx buffer full (\note change the name of this return)
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_SendChar(gSPIData_t chr);

/**
\par    Description:
\n      This function is used to receive a character from SPI bus.
\param  [out] pChr - pointer of character received from SPI
\retval IO_E_OK - operation processed successful
\retval ERR_RXEMPTY - Rx buffer empty (\note change the name of this return)
\retval ERR_OVERRUN - Overrun (\note change the name of this return)
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_RecvChar(gSPIData_t *pChr);

/**
\par    Description:
\n      This function is used to send a block of data to SPI bus.
\param  [in]  pData - pointer of data block
\param  [in]  dataLen - length of data block
\param  [out] pSnd - pointer to number of character sent.
\retval IO_E_OK - operation processed successful
\retval ERR_TXFULL - Tx buffer full (\note change the name of this return)
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_SendBlock(gSPIData_t *pData, uint16_t dataLen, uint16_t *pSnd);

/**
\par    Description:
\n      This function is used to receive a block of data from SPI bus.
\param  [out] pData - pointer of data block
\param  [in]  dataLen - length of data block
\param  [out] pRcv - pointer to number of character received
\retval IO_E_OK - operation processed successful
\retval ERR_RXEMPTY - Rx buffer empty (\note change the name of this return)
\retval ERR_OVERRUN - Overrun (\note change the name of this return)
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_RecvBlock(gSPIData_t *pData, uint16_t dataLen, uint16_t *pRcv);

/**
\par    Description:
\n      This function is used to clear the SPI Rx buffer
\param  none
\retval IO_E_OK - operation processed successful
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_ClearRxBuf(void);

/**
\par    Description:
\n      This function is used to clear the SPI Tx buffer
\param  none
\retval IO_E_OK - operation processed successful
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_ClearTxBuf(void);

/**
\par    Description:
\n      This function retrieves error information about occurred error.
\param  [out] pError - pointer to error information
\retval IO_E_OK - operation processed successful
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_GetError(gSPIError_t *pError);

/**
\par    Description:
\n      This function performs a reset of SPI peripheral.
\param  none
\return none
\note   documentation by: A. Ricci, F. Tarabelloni
*/
void SPI_ResetSync(void);

/**
\par    Description:
\n      This function performs a Init of SPI peripheral.
\param  [in] pInitSync - pointer to SPI setting struct
\retval IO_E_OK - operation processed successful
\note   documentation by: A. Ricci, F. Tarabelloni
*/
IO_ErrorType SPI_InitSync(gSPICfg_t* pInitSync);


#endif /* __OCSPI_HAL_H */


/* *************************** */
/* End of file                 */
/* *************************** */
