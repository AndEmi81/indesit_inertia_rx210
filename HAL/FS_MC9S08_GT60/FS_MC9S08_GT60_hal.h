/**
\defgroup HAL_FS_MC9S08_GT60  Freescale MC9S08 GT60 HAL
\ingroup FS_MC9S08_GT60
*/

/**
\defgroup HAL_FS_MC9S08_GT60_CLS  Freescale MC9S08 GT60 supported drivers Classes


This module lists the driver classes supported by this HAL.



\date    22/09/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_MC9S08_GT60
*/

/**
\defgroup HAL_FS_MC9S08_GT60_HISTORY  Freescale MC9S08 GT60 History
\ingroup HAL_FS_MC9S08_GT60
This module contains the history

\defgroup HAL_FS_MC9S08_GT60_HISTORY_HAL  Freescale MC9S08 GT60 HAL History
\ingroup  HAL_FS_MC9S08_GT60_HISTORY
*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    FS_MC9S08_GT60_hal.h
\ingroup HAL
\date    22/09/2009
\author  Roberto Fioravanti
\version 1.0.0


\addtogroup HAL_FS_MC9S08_GT60_HISTORY_HAL
@{
\n
\n 		00.00.10 
\n		27/01/2010 
\n		Roberto Fioravanti
\n		changes
\n 		- addiction of I2C macro forr unblock routines and nested interrupts
\n 		00.00.10 
\n		22/09/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- creation
@}

*/

/*
Copyright (c) 2009-2010, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup HAL_FS_MC9S08_GT60_CLS
@{*/



#ifndef __HAL_FREESCALE_MC9S08_GT60_H
#define __HAL_FREESCALE_MC9S08_GT60_H





/* ******** */
/* Includes */
/* ******** */
#include "Config.h"                               /* For: Application configuration */
#include "FS_MC9S08_GT60_reg.h"

/* ****** */
/* Errors */
/* ****** */

/* Dependancy rule: it is not possible to include this file direcly but only trought a driver. */
#if !defined(__DRIVER_H)
#error "This is a private header file -- do not include directly."
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* I2C Class HAL Support */
/* ********************* */

/** \defgroup HAL_FS_MC9S08_GB60_I2C  I2C Class driver support    
*/


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY

/*  I2C related registers redefinition                           
This approach helps when generating new files for additional parts of the same family.
*/
/*HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION> */
#define _HAL_S08_I2C_SAR_REG                        (IIC1A)
#define _HAL_S08_I2C_CTRL_REG                       (IIC1C)
#define _HAL_S08_I2C_ENABLE_REG                     (IIC1C_IICEN)
#define _HAL_S08_I2C_TX_CONTROL_REG                 (IIC1C_TX)
#define _HAL_S08_I2C_STARTSTOP_REG                  (IIC1C_MST)
#define _HAL_S08_I2C_RESTART_REG                    (IIC1C_RSTA)
#define _HAL_S08_I2C_TXAK_EN_REG                    (IIC1C_TXAK)
#define _HAL_S08_I2C_STATUS_REG                     (IIC1S)
#define _HAL_S08_I2C_BUSBUSY_REG                    (IIC1S_BUSY)
#define _HAL_S08_I2C_AL_REG                         (IIC1S_ARBL)
#define _HAL_S08_I2C_ADDRMATCH_REG                  (IIC1S_IAAS)
#define _HAL_S08_I2C_SLAVE_RW_REG                   (IIC1S_SRW)
#define _HAL_S08_I2C_INT_REG                        (IIC1C_IICIE)
#define _HAL_S08_I2C_RXACKBITVAL_REG                (IIC1S_RXAK)
#define _HAL_S08_I2C_TXACKBITVAL_REG                (IIC1C_TXAK)
#define _HAL_S08_I2C_INT_REQ_FLAG_REG               (IIC1S_IICIF)
#define _HAL_S08_I2C_TX_DATA_REG                    (IIC1D)
#define _HAL_S08_I2C_RX_DATA_REG                    (IIC1D)
#define _HAL_S08_I2C_INT_REQ_REG                    (IIC1S_IICIF)
#define _HAL_S08_I2C_MULT_REG                       (IIC1F_MULT)
#define _HAL_S08_I2C_ICR_REG                        (IIC1F)
#define _HAL_S08_I2C_SDA_IO_REG                     (PTCD_PTCD2)
#define _HAL_S08_I2C_SCL_IO_REG                     (PTCD_PTCD3)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* Peripheral management */

/**
Provides clock to the peripheral
*/
#define     I2C_ClkEnable()        {  }

/**
Do not provide clock to the peripheral
*/
#define     I2C_ClkDisable()       {  }
/**
Enable the peripheral
*/
#define     I2C_Enable()  {   _HAL_S08_I2C_ENABLE_REG = 1; }

/**
Disable the peripheral
*/
#define     I2C_Disable()       { _HAL_S08_I2C_ENABLE_REG = 0;  }

/**
Reset ctrl
*/
#define     I2C_CtrlReset() 	  { _HAL_S08_I2C_CTRL_REG = 0; }

/**
Reset interrupt
*/
#define     I2C_IntRegReset() 	{ 	}

/* */
#define     I2C_Reset()   {   }



/**/
#define     I2C_ReadData() _HAL_S08_I2C_RX_DATA_REG
/* */
#define     I2C_WriteData(b) {   _HAL_S08_I2C_TX_DATA_REG = (I2C_DataRegType)(b); }
#define     I2C_SendAddr(b)   I2C_WriteData(b)
/* */                    
#define     I2C_pReceiveDataRegister() (&_HAL_S08_I2C_RX_DATA_REG)
/* */
#define     I2C_pTransmitDataRegister() (&_HAL_S08_I2C_TX_DATA_REG)



/* Configuration */

/* */
#define     I2C_MasterEnable()  { }

/* */
#define     I2C_MasterDisable() { }

/* */
#define     I2C_SetClockRate(b)     {   _HAL_S08_I2C_ICR_REG = (I2C_ClockRateType)(b); } 
/**/
#define     I2C_ResetClockRate(b)    I2C_SetClockRate(b)
/* */
#define     I2C_SetSlaveAddress(b)  {   _HAL_S08_I2C_SAR_REG = (I2C_SlaveAddressType)(b); } 


/* */
#define     I2C_SetTxMode()    {    _HAL_S08_I2C_TX_CONTROL_REG = 1; }

/* */
#define     I2C_SetRxMode()   {    _HAL_S08_I2C_TX_CONTROL_REG = 0; }

/* */
#define     I2C_IsTxMode()     (_HAL_S08_I2C_TX_CONTROL_REG!=0x00)



/*Start & Stop condition */

/* */
#define     I2C_IssueStartCondition() {    _HAL_S08_I2C_STARTSTOP_REG = 1; }
/* */
#define     I2C_IssueReStartCondition() {   _HAL_S08_I2C_RESTART_REG = 1; }
/* */
#define     I2C_IssueStopCondition()  {   _HAL_S08_I2C_STARTSTOP_REG = 0; }




/* Interrupt management */

/* */
#define     I2C_IntEnable() { _HAL_S08_I2C_INT_REG = 1; }
/* */

#define     I2C_IntDisable()  { _HAL_S08_I2C_INT_REG = 0; }



/*Peripheral state management*/
/* */
#define     I2C_IsBusBusy() (_HAL_S08_I2C_BUSBUSY_REG != 0x00)
/* */
#define     I2C_IsSdaIoHigh() (_HAL_S08_I2C_SDA_IO_REG != 0x00)
/* */
#define     I2C_IsSclIoHigh() ( _HAL_S08_I2C_SCL_IO_REG != 0x00 )
/* */
#define     I2C_GetStatus() _HAL_S08_I2C_STATUS_REG
/* */
#define     I2C_ResetStatus() 	{(void)I2C_GetStatus(); /*_HAL_300H_I2C_STATUS_REG=0;*/ }
/* */
#define     I2C_ResetIntReqFlag() {   _HAL_S08_I2C_INT_REQ_FLAG_REG=1;  }
/* */
#define     I2C_IsArbitrationLost() (_HAL_S08_I2C_AL_REG != 0x00)
/* */
#define     I2C_IsSlaveAddressed()  (_HAL_S08_I2C_ADDRMATCH_REG != 0x00)
/* */
#define     I2C_IsGeneralCallAddress() FALSE
/* */
#define     I2C_ResetSlaveAddressedSig() {   _HAL_S08_I2C_ADDRMATCH_REG = 1; }
/* */
#define     I2C_ResetGeneralCallAddressSig() {   /* do nothing: n.a. */ }
/* */
#define     I2C_ResetArbitrationLost() {   _HAL_S08_I2C_AL_REG = 1; }
/* */
#define     I2C_IsSlaveTrasmitRequest() (_HAL_S08_I2C_SLAVE_RW_REG != 0)
/* */
#define     I2C_IsAcknowledgeEnabled() (_HAL_S08_I2C_TXAK_EN_REG == 0)
/* */
#define     I2C_IsMaster() (_HAL_S08_I2C_STARTSTOP_REG != 0)
/* */
#define     I2C_ResetTxSig() {}




/*Acknolegde bit  */

/* */
#define     I2C_ReceivedAcknowledgeVal()   (_HAL_S08_I2C_RXACKBITVAL_REG)
/* */
#define     I2C_EnableAcknowledge() {   _HAL_S08_I2C_TXACKBITVAL_REG = 0; }
/* */
#define     I2C_DisableAcknowledge() {   _HAL_S08_I2C_TXACKBITVAL_REG = 1; }

#define 		I2C_SlaveStartRx() 									
#define 		I2C_SlaveStartTx() 									I2C_IntEnable()





#define I2C_OnEnterIsr()                        I2C_ResetIntReqFlag()
#define I2C_OnIntAck()
#define I2C_DummyRead()                         (void)I2C_ReadData();

/* Slave receives unexpected data */
#define I2C_OnSlaveRxDummy()                    I2C_DummyRead();

#define I2C_OnSlaveAddressMatchTransmit()\
			        I2C_ResetSlaveAddressedSig();\
             		I2C_SetTxMode();\
    		        I2C_DisableAcknowledge();
    		
#define I2C_SlaveAddressedTx_Action()
/* This section contains HAL class extension */


#define I2C_NestedIsr_Begin()
#define I2C_NestedIsr_End()


#define I2C_Level3BlockMonitorSync()				(IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK) == IIC1C_TX_MASK)
#define I2C_Level4BlockMonitorSync()				(IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK|IIC1C_MST_MASK) == IIC1C_TXAK_MASK)
#define I2C_Level5BlockMonitorSync()				(IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK|IIC1C_MST_MASK))


/* This section contains HAL class extension */



/* ********************* */
/* I2C Class HAL Support */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* General               */
/* ********************* */

/** \defgroup HAL_FS_MC9S08_GB60_SYS  System Class driver support     
*/

/* */
#define _NOP_() asm("nop")

/* Reset Generation to be defined in config.h */
#ifndef SW_RESET_MCU
#define SW_RESET_MCU()
#endif

/* 
us nop cycles conversion according to the bus frequency.

\todo rivedere il calcolo.
*/
#if (BUS_FREQUENCY_Hz == 10000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = µs di ritardo fisso dovuto alla chiamata della routine
  // *1000/2614 = moltiplicatore di 2,614µs che è la durata misurata per una singola iterazione del loop
  #define DELAY_MICROSEC(x)	((((long)x-3)*1000)/2614)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */

#if (BUS_FREQUENCY_Hz == 20000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = µs di ritardo fisso dovuto alla chiamata della routine
  // *1000/1307 = moltiplicatore di 1,307µs che è la durata misurata per una singola iterazione del loop
  #define DELAY_MICROSEC(x)	((((long)x-2)*1000)/1307)
#endif /* (BUS_FREQUENCY_Hz == 20000000) */


/* ********************* */
/* General               */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


#endif /* __HAL_FREESCALE_MC9S08_GT60_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */