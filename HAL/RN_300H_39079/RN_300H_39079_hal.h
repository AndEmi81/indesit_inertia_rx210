/**
\defgroup HAL_RN_300H_39079 Hardware Abstraction Layer for Renesas 300H 39079
\ingroup RN_300H_39079
*/

/**
\defgroup HAL_RN_300H_39079_CLS Renesas 300H 39079 supported drivers Classes

This module lists the driver classes supported by this HAL.


\date    16/06/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_RN_300H_39079
*/

/**
\defgroup HAL_RN_300H_39079_HISTORY  Renesas 300H 39079 History
\ingroup HAL_RN_300H_39079
This module contains the history

\defgroup HAL_RN_300H_39079_HISTORY_HAL  Renesas 300H 39079 HAL History
\ingroup  HAL_RN_300H_39079_HISTORY
*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    RN_300H_39079_hal.h
\ingroup HAL_RN_300H_39079
\date    16/06/2009
\author  Roberto Fioravanti
\version 1.0.0

\n
\addtogroup HAL_RN_300H_39079_HISTORY_HAL
@{
\n 		00.00.25
\n		08/01/2010 
\n		developper Roberto Fioravanti
\n		changes
\n 		- added I2C_Level3BlockMonitorSync()	I2C_Level4BlockMonitorSync() I2C_Level5BlockMonitorSync()
\n		- added I2C_NestedIsr_Begin() I2C_NestedIsr_End() as empty because nested isr not supported
\n
\n 		00.00.22
\n		26/11/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- bug fix: I2C_ReceivedAcknowledgeVal, I2C_IsBusBusy: they return boolean.
\n
\n
\n 		00.00.21
\n		16/11/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- changed access to registers (from bit field to mask)
\n
\n 		00.00.12
\n		08/10/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- introduzione <machine.h> per uso Intrinsic Functions (nop)
\n    - introduzione SW_RESET_MCU definibile in config.h
\n    - _NOP_(); in I2C_Reset
\n
\n 		00.00.02
\n		16/06/2009 
\n		developper Roberto Fioravanti
\n		changes
\n 		- creazione
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




#ifndef __HAL_RENESAS_300H_39079_H
#define __HAL_RENESAS_300H_39079_H





/* ******** */
/* Includes */
/* ******** */
//#include "Config.h"                               /* For: Application configuration */
#include "RN_300H_39079_reg.h"
#include <machine.h> 																/* include <machine.h> for Intrinsic Functions */

/* ****** */
/* Errors */
/* ****** */

/* Dependancy rule: it is not possible to include this file direcly but only trought a driver. */
#if !defined(__DRIVER_H)
//#error "This is a private header file -- do not include directly."
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

/** \defgroup HAL_RN_300H_39079_I2C  I2C Class driver support    
    \ingroup HAL_RN_300H_39079_CLS
 */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Define I2C Capability */
#define _HAL_I2C_CAPABILITY

#define		OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR			(1)
#define		OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK	(1)
#define		OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK					(1)

#define _HAL_300H_I2C_SAR_REG                      		(IIC2.SAR.BYTE)             
#define _HAL_300H_I2C_CTRL1														(IIC2.ICCR1.BYTE)           
#define _HAL_300H_I2C_CTRL2														(IIC2.ICCR2.BYTE)           
#define _HAL_300H_I2C_ENABLE_REG                    	(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_TX_CONTROL_REG                	(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_MST_CONTROL_REG               	(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_STARTSTOP_REG1                	(IIC2.ICCR2.BYTE)       
#define _HAL_300H_I2C_STARTSTOP_REG2                	(IIC2.ICCR2.BYTE)        
#define _HAL_300H_I2C_MST_TEST_REG										(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_RESET_SDA_AND_SCL_REG						(IIC2.ICCR2.BYTE)     
#define _HAL_300H_I2C_INT_REG													(IIC2.ICIER.BYTE)           
#define _HAL_300H_I2C_TXAK_EN_REG                    	(IIC2.ICIER.BYTE)      
#define _HAL_300H_I2C_STATUS_REG                     	(IIC2.ICSR.BYTE)            
#define _HAL_300H_I2C_BUSBUSY_REG                    	(IIC2.ICCR2.BYTE)       
#define _HAL_300H_I2C_AL_REG                         	(IIC2.ICSR.BYTE)       
#define _HAL_300H_I2C_ADDRMATCH_REG                  	(IIC2.ICSR.BYTE)         
#define _HAL_300H_I2C_GCALL_ADDRMATCH_REG            	(IIC2.ICSR.BYTE)         
#define _HAL_300H_I2C_SLAVE_RW_REG                   	(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_TX_INT_REG                     	(IIC2.ICIER.BYTE)        
#define _HAL_300H_I2C_RX_INT_REG                     	(IIC2.ICIER.BYTE)        
#define _HAL_300H_I2C_RXACKBITVAL_REG                	(IIC2.ICIER.BYTE)      
#define _HAL_300H_I2C_TXACKBITVAL_REG                	(IIC2.ICIER.BYTE)      
#define _HAL_300H_I2C_TX_DATA_REG                    	(IIC2.ICDRT)                
#define _HAL_300H_I2C_RX_DATA_REG                    	(IIC2.ICDRR)                
#define _HAL_300H_I2C_CKS_REG                        	(IIC2.ICCR1.BYTE)        
#define _HAL_300H_I2C_SDA_IO_REG                     	(IO.PDR5.BYTE)            
#define _HAL_300H_I2C_SCL_IO_REG                     	(IO.PDR5.BYTE)            


#define _HAL_300H_I2C_ENABLE_MASK                     	BIT7_MASK
#define _HAL_300H_I2C_TX_CONTROL_MASK                 	BIT4_MASK
#define _HAL_300H_I2C_MST_CONTROL_MASK                	BIT5_MASK
#define _HAL_300H_I2C_CKS_MASK													(BIT3_MASK|BIT2_MASK|BIT1_MASK|BIT0_MASK)

#define _HAL_300H_I2C_BUSBUSY_MASK											BIT7_MASK
#define _HAL_300H_I2C_STARTSTOP_REG1_MASK             	_HAL_300H_I2C_BUSBUSY_MASK
#define _HAL_300H_I2C_STARTSTOP_REG2_MASK             	BIT6_MASK
#define _HAL_300H_I2C_RESET_SDA_AND_SCL_MASK          	BIT1_MASK

#define	_HAL_300H_I2C_TX_INT_MASK												BIT7_MASK
#define	_HAL_300H_I2C_TX_END_INT_MASK										BIT6_MASK
#define _HAL_300H_I2C_RX_INT_MASK												BIT5_MASK
#define _HAL_300H_I2C_RXACKBITVAL_MASK                	BIT1_MASK      
#define _HAL_300H_I2C_TXACKBITVAL_MASK									BIT0_MASK
#define _HAL_300H_I2C_TXAK_EN_MASK											_HAL_300H_I2C_TXACKBITVAL_MASK

#define _HAL_300H_I2C_AL_MASK														BIT2_MASK
#define _HAL_300H_I2C_ADDRMATCH_MASK										BIT1_MASK
#define _HAL_300H_I2C_GCALL_ADDRMATCH_MASK							BIT0_MASK

#define _HAL_300H_I2C_SDA_IO_MASK												BIT6_MASK
#define _HAL_300H_I2C_SCL_IO_MASK												BIT7_MASK

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
#define     I2C_Enable()       	_HAL_300H_I2C_ENABLE_REG |= _HAL_300H_I2C_ENABLE_MASK;

/**
Disable the peripheral
*/
#define     I2C_Disable() 	  	{	_HAL_300H_I2C_ENABLE_REG &= ~_HAL_300H_I2C_ENABLE_MASK;  }

/**
Reset ctrl
*/
#define     I2C_CtrlReset() 		{ 	_HAL_300H_I2C_CTRL1 = 0; _HAL_300H_I2C_CTRL2=0; }

/**
Reset interrupt
*/
#define     I2C_IntRegReset() 	{ 	_HAL_300H_I2C_INT_REG = 0; }

/* */
#define		I2C_Reset()			{\
									_HAL_300H_I2C_RESET_SDA_AND_SCL_REG |= _HAL_300H_I2C_RESET_SDA_AND_SCL_MASK;\
									_NOP_(); \
									_HAL_300H_I2C_RESET_SDA_AND_SCL_REG &= ~_HAL_300H_I2C_RESET_SDA_AND_SCL_MASK;\
								}



/**/
#define     I2C_ReadData() 							_HAL_300H_I2C_RX_DATA_REG
/* */
#define     I2C_WriteData(b) 						{   _HAL_300H_I2C_TX_DATA_REG = (I2C_DataRegType)(b); }
#define     I2C_SendAddr(b) 						I2C_WriteData(b)
/* */                    
#define     I2C_pReceiveDataRegister() 	(&_HAL_300H_I2C_RX_DATA_REG)
/* */
#define     I2C_pTransmitDataRegister() (&_HAL_300H_I2C_TX_DATA_REG)



/* Configuration */

/* */

#define     I2C_MasterEnable()  				{ _HAL_300H_I2C_MST_CONTROL_REG |=  _HAL_300H_I2C_MST_CONTROL_MASK; }
/* */
#define     I2C_MasterDisable() 				{ _HAL_300H_I2C_MST_CONTROL_REG &=  ~_HAL_300H_I2C_MST_CONTROL_MASK; }

/* */
#define     I2C_SetClockRate(b) 				{   _HAL_300H_I2C_CKS_REG &= ~_HAL_300H_I2C_CKS_MASK; _HAL_300H_I2C_CKS_REG |= (I2C_ClockRateType)((b) & _HAL_300H_I2C_CKS_MASK); }
/**/
#define     I2C_ResetClockRate(b)    			I2C_SetClockRate(b)
/* */
#define     I2C_SetSlaveAddress(b) 				{   _HAL_300H_I2C_SAR_REG = (I2C_SlaveAddressType)(b); }


/* */
#define     I2C_SetTxMode()    					{    _HAL_300H_I2C_TX_CONTROL_REG |= _HAL_300H_I2C_TX_CONTROL_MASK; }

/* */
#define     I2C_SetRxMode()   					{    _HAL_300H_I2C_TX_CONTROL_REG  &= ~_HAL_300H_I2C_TX_CONTROL_MASK; }

/* */
#define     I2C_IsTxMode()     					(_HAL_300H_I2C_TX_CONTROL_REG & _HAL_300H_I2C_TX_CONTROL_MASK)



/*Start & Stop condition */

/* */
#define     I2C_IssueStartCondition() 		{  	_HAL_300H_I2C_STARTSTOP_REG1 |= _HAL_300H_I2C_STARTSTOP_REG1_MASK; _HAL_300H_I2C_STARTSTOP_REG2 &= ~_HAL_300H_I2C_STARTSTOP_REG2_MASK; }
/* */
#define     I2C_IssueReStartCondition() 	I2C_IssueStartCondition()
/* */
#define     I2C_IssueStopCondition()  		{ 	_HAL_300H_I2C_STARTSTOP_REG1 &= ~_HAL_300H_I2C_STARTSTOP_REG1_MASK; _HAL_300H_I2C_STARTSTOP_REG2 &= ~_HAL_300H_I2C_STARTSTOP_REG2_MASK; }




/* Interrupt management */
#define     I2C_IntEnable() 							{ _HAL_300H_I2C_TX_INT_REG |= (_HAL_300H_I2C_TX_INT_MASK|_HAL_300H_I2C_RX_INT_MASK); }
#define     I2C_IntDisable() 							{ _HAL_300H_I2C_TX_INT_REG &= ~(_HAL_300H_I2C_TX_INT_MASK|_HAL_300H_I2C_RX_INT_MASK);}

/* */
#define     I2C_TxIntDisable()  					{ _HAL_300H_I2C_TX_INT_REG &= ~_HAL_300H_I2C_TX_INT_MASK; }
/* */
#define     I2C_RxIntDisable()  					{ _HAL_300H_I2C_RX_INT_REG &= ~_HAL_300H_I2C_RX_INT_MASK; }

#define     I2C_TxIntEnable() 						{ _HAL_300H_I2C_TX_INT_REG |= _HAL_300H_I2C_TX_INT_MASK;}
#define     I2C_RxIntEnable() 						{ _HAL_300H_I2C_RX_INT_REG |= _HAL_300H_I2C_RX_INT_MASK; }
/* */

/* */




/*Peripheral state management*/
/* */
#define     I2C_IsBusBusy() 							((_HAL_300H_I2C_BUSBUSY_REG & _HAL_300H_I2C_BUSBUSY_MASK) != 0x00)
/* */
#define     I2C_IsSdaIoHigh() 						((_HAL_300H_I2C_SDA_IO_REG & _HAL_300H_I2C_SDA_IO_MASK) != 0x00)
/* */
#define     I2C_IsSclIoHigh() 						( (_HAL_300H_I2C_SCL_IO_REG& _HAL_300H_I2C_SCL_IO_MASK) != 0x00 )
/* */
#define     I2C_GetStatus() 							_HAL_300H_I2C_STATUS_REG
/* */
#define     I2C_ResetStatus() 						{(void)_HAL_300H_I2C_STATUS_REG; _HAL_300H_I2C_STATUS_REG=0;}

#define     I2C_ResetIntReqFlag() 					{   } 

#define     I2C_IsArbitrationLost()					(	_HAL_300H_I2C_AL_REG &  0x04)
/* */
#define     I2C_IsSlaveAddressed()					(_HAL_300H_I2C_ADDRMATCH_REG & 0x02)
/* */
#define     I2C_IsGeneralCallAddress()				((	_HAL_300H_I2C_GCALL_ADDRMATCH_REG & _HAL_300H_I2C_GCALL_ADDRMATCH_MASK) != 0x00)
/* */
#define     I2C_ResetSlaveAddressedSig()			{   _HAL_300H_I2C_ADDRMATCH_REG &= ~0x02; }
/* */
#define     I2C_ResetGeneralCallAddressSig()		{   _HAL_300H_I2C_GCALL_ADDRMATCH_REG = 0; }

#define     I2C_ResetRxSig() 						{   IIC2.ICSR.BYTE; IIC2.ICSR.BYTE &= ~0x20; }

#define     I2C_ResetTxSig() 						{   IIC2.ICSR.BYTE; IIC2.ICSR.BYTE &= ~0x40; IIC2.ICSR.BYTE &= ~0x80; }

#define     I2C_ResetArbitrationLost()				{   _HAL_300H_I2C_AL_REG = 0; }

#define     I2C_IsSlaveTrasmitRequest()				(_HAL_300H_I2C_SLAVE_RW_REG & _HAL_300H_I2C_TX_CONTROL_MASK)

#define     I2C_IsAcknowledgeEnabled()				((IIC2.ICIER.BYTE & 0x01)== 0)


#define     I2C_IsMaster()							(_HAL_300H_I2C_MST_TEST_REG & _HAL_300H_I2C_MST_CONTROL_MASK)




/*Acknolegde bit  */

/* */
#define			I2C_ReceivedAcknowledgeVal()   		((_HAL_300H_I2C_RXACKBITVAL_REG & _HAL_300H_I2C_RXACKBITVAL_MASK) != 0)
/* */
#define     	I2C_EnableAcknowledge() 			{   _HAL_300H_I2C_TXACKBITVAL_REG &= ~_HAL_300H_I2C_TXACKBITVAL_MASK; }
/* */
#define			I2C_DisableAcknowledge() 			{   _HAL_300H_I2C_TXACKBITVAL_REG |= _HAL_300H_I2C_TXACKBITVAL_MASK; }


#define 		I2C_SlaveStartRx() 					{_HAL_300H_I2C_TX_INT_REG |= _HAL_300H_I2C_TX_INT_MASK;		_HAL_300H_I2C_TX_INT_REG &= ~_HAL_300H_I2C_TX_END_INT_MASK; }
#define 		I2C_SlaveStartTx() 					{_HAL_300H_I2C_TX_INT_REG &= ~_HAL_300H_I2C_TX_INT_MASK;		_HAL_300H_I2C_TX_INT_REG |= _HAL_300H_I2C_TX_END_INT_MASK;}
			

/* This section contains HAL class extension */
#define I2C_NestedIsr_Begin()

#define I2C_NestedIsr_End()

#define I2C_OnEnterIsr() I2C_ResetIntReqFlag()
#define I2C_OnIntAck()	/* empty */
#define I2C_DummyRead()							(void)I2C_ReadData()
/* Slave receives unexpected data */
#define I2C_OnSlaveRxDummy()                    I2C_WriteData(0x55);\
                                                I2C_DummyRead();\
												I2C_SetRxMode();\
												I2C_EnableAcknowledge();

#define I2C_ProcessRxRequest(data)	\
{	\
	if((OCI2C.state & OCI2C_UNEXPECTED_DATA_RX) && (data&1))\
	{\
		OCI2C.state &= ~OCI2C_UNEXPECTED_DATA_RX;\
		\
		I2C_WriteData(0x55);\
		OCI2C_SlaveStopCommSync();\
	}\
}												
#define I2C_OnSlaveAddressMatchTransmit() \
			I2C_ResetSlaveAddressedSig();\
            I2C_SetTxMode();\
    		I2C_DisableAcknowledge()\
			I2C_SlaveStartTx();

#define I2C_SlaveAddressedTx_Action()

/* Unblock routine */

#define I2C_Level3BlockMonitorSync()				(I2C_IsTxMode() && I2C_IsAcknowledgeEnabled())
#define I2C_Level4BlockMonitorSync()				( ((_HAL_300H_I2C_MST_TEST_REG & (_HAL_300H_I2C_MST_CONTROL_MASK | _HAL_300H_I2C_TX_CONTROL_MASK)) == 0)  && !I2C_IsAcknowledgeEnabled() )
#define I2C_Level5BlockMonitorSync()				((_HAL_300H_I2C_MST_TEST_REG & (_HAL_300H_I2C_MST_CONTROL_MASK | _HAL_300H_I2C_TX_CONTROL_MASK) || !I2C_IsAcknowledgeEnabled()))

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
/** nop */
#define _NOP_()  nop()

/** \defgroup HAL_FS_HCS08_GB60_SYS  System Class driver support     
*/
#ifndef SW_RESET_MCU
#define SW_RESET_MCU()
#endif 

/** 
\def DELAY_MICROSEC 
converte us in cicli nop in base alla frequenza di bus.

\todo rivedere il calcolo.
*/
#if (BUS_FREQUENCY_Hz == 16000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi LIB_Delay_us */
  /* x>2 (5/4)*/
  #define DELAY_MICROSEC(x)	(((uint16_t)(x*16)-20)/8)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */

#if (BUS_FREQUENCY_Hz == 20000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = �s di ritardo fisso dovuto alla chiamata della routine
  // *1000/1307 = moltiplicatore di 1,307�s che � la durata misurata per una singola iterazione del loop
  //#define DELAY_MICROSEC(x)	((((long)x-2)*1000)/1307)
#endif /* (BUS_FREQUENCY_Hz == 20000000) */

/**
Used to store CCR register before changing it   
*/
extern unsigned char CCR_copy;
/** 
Disable Global Enable Interrupt 
*/
#pragma inline (EnterCritical)
static void EnterCritical(void)
{
unsigned char tmp;
	
	tmp = get_ccr();
	set_imask_ccr(1);	
	CCR_copy = tmp;
}

/*lint +e950 */

/**
Enable Global Enable Interrupt if enabled before EnterCritical()
*/
#pragma inline (ExitCritical)
static void ExitCritical(void)
{
	set_ccr(CCR_copy);
}

/* ********************* */
/* General               */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */

/* ********************** */
/*         BEGIN          */
/* ********************** */
/* ********************** */
/* UART Class HAL Support */
/* ********************** */




/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Define UART1 Capability */
#define _HAL_UARTx_CAPABILITY

#ifdef HAL_OPTION_UART
#if HAL_OPTION_UART==1
#define HAL_UART_DEV
#elif HAL_OPTION_UART==2
#define HAL_UART_DEV	_2
#else
#error "uart non selezionata correttamente"
#endif
#endif



/* Baud rates calculations */

/*
OPTION__DEVICE_UART_BAUDRATE_DIVISOR=[0,1,2,3] 
APPL_HALUART_CALC_n_DIVISOR calcola 2^(2n-1)*/
#define HAL_UART_CALC_n_DIVISOR             		(HAL_OPTION_UART_BAUDRATE_DIVISOR==0 ? 0.5: \
																								(HAL_OPTION_UART_BAUDRATE_DIVISOR==1 ? 2.0: \
																								(HAL_OPTION_UART_BAUDRATE_DIVISOR==2 ? 8.0: 32.0)))
#define HAL_UART_DIVISOR												HAL_UART_CALC_n_DIVISOR
#define _HALUART_CALC_BAUD(baud,divisor)   (uint8_t)((uint8_t)((BUS_FREQUENCY_Hz / (64.0 * (divisor) * (baud))) + 0.5) - 1)

/** Baud Rates */
#define _HALUART_BAUD_115200               (_HALUART_CALC_BAUD(115200UL,	HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_76800                (_HALUART_CALC_BAUD(76800UL,		HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_57600                (_HALUART_CALC_BAUD(57600UL,		HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_38400                (_HALUART_CALC_BAUD(38400UL,		HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_28800                (_HALUART_CALC_BAUD(28800U,		HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_19200                (_HALUART_CALC_BAUD(19200U,		HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_14400                (_HALUART_CALC_BAUD(14400,			HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_9600                 (_HALUART_CALC_BAUD(9600,			HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_4800                 (_HALUART_CALC_BAUD(4800,			HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_2400                 (_HALUART_CALC_BAUD(2400,			HAL_UART_CALC_n_DIVISOR))
#define _HALUART_BAUD_1200                 (_HALUART_CALC_BAUD(1200,			HAL_UART_CALC_n_DIVISOR))


#define _HAL_300H_UARTx_PE_ERROR_MASK				(1<<3)
#define _HAL_300H_UARTx_FER_ERROR_MASK			(1<<4)
#define _HAL_300H_UARTx_OER_ERROR_MASK			(1<<5)

#define _HAL_300H_UARTx_ERROR_MASK					(_HAL_300H_UARTx_PE_ERROR_MASK|_HAL_300H_UARTx_FER_ERROR_MASK|_HAL_300H_UARTx_OER_ERROR_MASK)

/** \defgroup HAL_HAL_RN_300H_39079_UART  UART Class driver support    
\ingroup HAL_RN_300H_39079_CLS
 
*/

#define _HAL_300H_UARTx_BAUDRATE_REG(n)						(SCI3##n.BRR)
#define _HAL_300H_UARTx_TX_DATAREG(n)                  	    (SCI3##n.TDR)
#define _HAL_300H_UARTx_RX_DATAREG(n)              	        (SCI3##n.RDR)
#define _HAL_300H_UARTx_ENABLE_TX_REG(n)            	    (SCI3##n.SCR3.BIT.TE)
#define _HAL_300H_UARTx_ENABLE_RX_REG(n)            	    (SCI3##n.SCR3.BIT.RE)
#define _HAL_300H_UARTx_ENABLE_TX_IRQ_REG(n)            	(SCI3##n.SCR3.BIT.TEIE)
#define _HAL_300H_UARTx_ENABLE_RX_IRQ_REG(n)            	(SCI3##n.SCR3.BIT.RIE)

#define _HAL_300H_UARTx_CLK_ENABLE_REG(n)					(SCI3##n.SCR3.BIT.CKE)

#define _HAL_300H_UARTx_STATUS_RX_REG(n)            	    (SCI3##n.SSR.BIT.RDRF)
#define _HAL_300H_UARTx_STATUS_TX_REG(n)            	    (SCI3##n.SSR.BIT.TDRE)
#define _HAL_300H_UARTx_STATUS_TXEND_REG(n)            	    (SCI3##n.SSR.BIT.TEND)
#define _HAL_300H_UARTx_STATUS_ERR_PE_REG(n)            	(SCI3##n.SSR.BIT.PER)
#define _HAL_300H_UARTx_STATUS_ERR_FER_REG(n)            	(SCI3##n.SSR.BIT.FER)
#define _HAL_300H_UARTx_STATUS_ERR_OER_REG(n)            	(SCI3##n.SSR.BIT.OER)

#define _HAL_300H_UARTx_STATUS_ERR_REG(n)            	    (SCI3##n.SSR.BYTE)//(_HAL_300H_UARTx_STATUS_ERR_PER_REG(n)|| _HAL_300H_UARTx_STATUS_ERR_FER_REG(n) || _HAL_300H_UARTx_STATUS_ERR_OER_REG(n))
#define _HAL_300H_UARTx_FRAME_CFG_REG(n)            	    (SCI3##n.SMR.BYTE)
#if HAL_OPTION_UART==1
#define _HAL_300H_UARTx_TX_PIN                              IO.PMR1.BIT.TXD
#elif HAL_OPTION_UART==2
#define _HAL_300H_UARTx_TX_PIN                              IO.PMR1.BIT.TXD2
#endif

#define _HAL_300H_UARTx_PARITY_ENABLE_REG(n)            	(SCI3##n.SMR.BIT.PE)
#define _HAL_300H_UARTx_PARITY_VAL_REG(n)            		(SCI3##n.SMR.BIT.PM)
#define _HAL_300H_UARTx_STOP_REG(n)            	      		(SCI3##n.SMR.BIT.STOP)
#define _HAL_300H_UARTx_MODE_REG(n)            	      		(SCI3##n.SMR.BIT.COM)
#define _HAL_300H_UARTx_DATALEN_REG(n)            	      	(SCI3##n.SMR.BIT.CHR)
#define _HAL_300H_UARTx_CLOCK_DIVISOR_REG(n)            	(SCI3##n.SMR.BIT.CKS)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ******************* */
/* Function prototypes */
/* ******************* */


/*  Peripheral management and configuration
 *  
 */

#define 	UART_ClkEnable(n)		        {}
#define 	UART_ClkDisable(n)		        {}


#define 	UART_Enable(n)		{ _HAL_300H_UARTx_TX_PIN=1; _HAL_300H_UARTx_CLK_ENABLE_REG(n)=0;}
#define     UART_TxEnable(n)    { _HAL_300H_UARTx_ENABLE_TX_REG(n) = 1; }
#define     UART_RxEnable(n)    { _HAL_300H_UARTx_ENABLE_RX_REG(n) = 1; }



#define     UART_TxDisable(n)  	{ _HAL_300H_UARTx_ENABLE_TX_REG(n) = 0; }
#define     UART_RxDisable(n)  	{ _HAL_300H_UARTx_ENABLE_RX_REG(n) = 0; }
#define 	UART_Disable(n)		{ _HAL_300H_UARTx_TX_PIN=0;}


#define     UART_SetParityVal(n,val)		{_HAL_300H_UARTx_PARITY_VAL_REG(n)=(val);}
#define     UART_SetParityMode(n,val)		{_HAL_300H_UARTx_PARITY_ENABLE_REG(n)=(val);}
#define     UART_SetDataLen(n,val)			{_HAL_300H_UARTx_DATALEN_REG(n)=(val);}
#define     UART_SetStop(n,val)				{_HAL_300H_UARTx_STOP_REG(n)=(val);}
#define     UART_SetMode(n,val)				{_HAL_300H_UARTx_MODE_REG(n)=(val);}
#define     UART_SetClockDivisor(n,val)		{_HAL_300H_UARTx_CLOCK_DIVISOR_REG(n)=(val);}

/*@}*/



/*  Data register
 *  
 */

#define     UART_ReadData(n) 		            _HAL_300H_UARTx_RX_DATAREG(n);

#define     UART_WriteData(n,b) 	          	{   _HAL_300H_UARTx_TX_DATAREG(n) = (UART_DataRegType)(b); }
                  
   
#define     UART_pReceiveDataRegister(n) 	  	(&_HAL_300H_UARTx_RX_DATAREG(n))

#define     UART_pTransmitDataRegister(n)   	(&_HAL_300H_UARTx_TX_DATAREG(n))

			
/* Configuration
 *  
 */
#define     UART_SetBaudRate(n,b) 	{   _HAL_300H_UARTx_BAUDRATE_REG(n) = (UART_BaudRateType)(b); }

//#define     UART_SetFrameConfig(n,b) 	{   _HAL_300H_UARTx_FRAME_CFG_REG(n) = (UART_FrameCfgType)(b); }



/* Interrupt
 *  
 */
#define     UART_TxIntEnable(n)    { _HAL_300H_UARTx_ENABLE_TX_IRQ_REG(n) = 1; }
#define     UART_RxIntEnable(n)    { _HAL_300H_UARTx_ENABLE_RX_IRQ_REG(n) = 1; }

#define     UART_TxIntDisable(n)  { _HAL_300H_UARTx_ENABLE_TX_IRQ_REG(n) = 0; }
#define     UART_RxIntDisable(n)  { _HAL_300H_UARTx_ENABLE_RX_IRQ_REG(n) = 0; }



/*  State
 *  
 */
#define     UART_IsPeError(n)        	(_HAL_300H_UARTx_STATUS_ERR_PE_REG(n))	/* \todo deve ritornare un valore fisso se non abilitato */
#define     UART_IsFerError(n)       	(_HAL_300H_UARTx_STATUS_ERR_FER_REG(n))
#define     UART_IsOerError(n)       	(_HAL_300H_UARTx_STATUS_ERR_OER_REG(n))
#define     UART_IsError(n)				(_HAL_300H_UARTx_STATUS_ERR_REG(n) & _HAL_300H_UARTx_ERROR_MASK)


#define     UART_ClearError(n)       	_HAL_300H_UARTx_STATUS_ERR_REG(n) &= ~_HAL_300H_UARTx_ERROR_MASK;
#define 	UART_ResetPeError(n)		if(UART_IsPeError(n)) {_HAL_300H_UARTx_STATUS_ERR_PE_REG(n)=0;}
#define     UART_ClearRxIrq(n)       	{} /* non serve: � sufficiente leggere il registro */
#define     UART_ClearTxIrq(n)       	{}

#define     UART_IsTxInt(n)           	(_HAL_300H_UARTx_STATUS_TX_REG(n))
#define     UART_IsTxEndInt(n)			(_HAL_300H_UARTx_STATUS_TXEND_REG(n))
#define     UART_IsRxInt(n)           	(_HAL_300H_UARTx_STATUS_RX_REG(n))

#define 	UART_IsTxDataRegFull(n)   	(_HAL_300H_UARTx_STATUS_TX_REG(n) == 0)





/* ********************** */
/* UART Class HAL Support */
/* ********************** */
/* ********************** */
/*           END          */
/* ********************** */
/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_DIO_SUPPORT)
#include"IO_driver.h"
#define PIN_DUMMY\
{\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,(DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y)\
{\
{(DIO_PortAddressBasicType)&IO.PDR##x##.BYTE,(DIO_PortAddressExtendedType)&IO.PUCR##x##.BYTE},\
(DIO_ModuleMapMaskType)PDR##x##_PIN##y##_MASK\
}
/* sharing between dataInput and dataOutput */
#define dataOutput dataInput
/* dummy declaration */
#define slewRate 		pullup
/* dummy declaration */
#define driveStrength	pullup
typedef struct
{
    uint8_t dataInput;
    uint8_t hole[0x0F];
    uint8_t direction;
}portreg_struct_t;
typedef struct
{
    uint8_t pullup;         
}portreg_ext_struct_t;

#pragma inline (DIO_IsNotDummyIO)
static bool_t DIO_IsNotDummyIO(const IO_ModuleIOMapType* ptr)
{
	return (bool_t) ( ((ptr->PortAddress.basic) != (DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR) && ((ptr->PinMask) != 0))?1:0;
}

#pragma inline (DeInitGPIO)
static void DeInitGPIO(void)
{
	IO.PCR1 = 0x00;
	IO.PUCR1.BYTE = 0x00; 
	IO.PCR2 = 0x00; 
	IO.PCR3 = 0x00; 	
	IO.PCR5 = 0x00; 
	IO.PUCR5.BYTE = 0x00;
	IO.PCR6 = 0x00; 
	IO.PCR7 = 0x00; 
	IO.PCR8 = 0x00; 
	IO.PCRC = 0x00;	
}

#define DIO_PULL_UP_IS_AVAILABLE	TRUE
#pragma inline (DIO_IsAvailablePullUp)

static bool_t DIO_IsAvailablePullUp(IO_ModuleIOMapType * pMap)
{
	portreg_struct_t *port = (portreg_struct_t *)pMap->PortAddress.basic;
	if(((void*)port == (void*)&IO.PUCR1.BYTE)||((void*)port == (void*)&IO.PUCR5.BYTE))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#define DIO_PullUpEnable(ptr) 		(((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup) |= ptr->PinMask;
#define DIO_IsPullUpEnabled(ptr) 	(((((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup) & ptr->PinMask) != 0)
#define DIO_PullUpDisable(ptr)		(((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup) &= ~ptr->PinMask;


#define DIO_PULL_DOWN_IS_AVAILABLE	FALSE
#pragma inline (PullDownIsAvaibleOn)
static bool_t PullDownIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_INPUT_IS_AVAILABLE	TRUE
#pragma inline (IsAvailableAsInput)
static bool_t IsAvailableAsInput(IO_ModuleIOMapType * pMap)
{
	return 1;
}
#define DIO_SLEW_RATE_IS_AVAILABLE	FALSE
#pragma inline (SlewRateIsAvaibleOn)
static bool_t SlewRateIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_DRIVE_STRENGTH	FALSE
#pragma inline (DriveStrengthIsAvaibleOn)		
static bool_t DriveStrengthIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}

#define DIO_OUTPUT_IS_AVAILABLE	TRUE
#pragma inline(IsAvailableAsOutput)
static bool_t IsAvailableAsOutput(IO_ModuleIOMapType * pMap)
{
	portreg_struct_t *port = (portreg_struct_t *)pMap->PortAddress.basic;
	if((void*)port == (void*)&IO.PDRB.BYTE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

#define DIO_SetDirAsOutput(ptr)	 ((portreg_struct_t *)ptr->PortAddress.basic->direction) |= ptr->PinMask;
#define DIO_SetDirAsInput(ptr)	 ((portreg_struct_t *)ptr->PortAddress.basic->direction) &= ~ptr->PinMask;
#define DIO_IsOuput(ptr) 		((((portreg_struct_t *)ptr->PortAddress.basic->direction) & ptr->PinMask) != 0)
#define DIO_SetHigh(ptr)		((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput |= ptr->PinMask;
#define DIO_SetLow(ptr)			((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput &= ~ptr->PinMask;
#define DIO_GetVal(ptr)			((((portreg_struct_t *)ptr->PortAddress.basic)->dataInput & ptr->PinMask) != 0)


#pragma inline(GetMirrorPortDirectionRegister)
static uint8_t GetMirrorPortDirectionRegister(portreg_struct_t *port)
{
uint8_t i;
	i=(uint8_t)((uint8_t*)port-(uint8_t*)&IO.PDR1.BYTE);
	return(i);
}
#define DIO_PASSIVE_FILTER_IS_AVAILABLE  FALSE
#pragma inline (PassiveFilterIsAvaibleOn)		
static bool_t PassiveFilterIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_INTERRUPT_IS_AVAILABLE    FALSE
#pragma inline (DIO_InterruptIsAvaibleOn)		
static bool_t DIO_InterruptIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_DIGITAL_FILTER_IS_AVAILABLE   FALSE
#pragma inline (DIO_DigitalFilterIsAvaibleOn)		
static bool_t DIO_DigitalFilterIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#pragma inline (DIO_SetDigitalFltClkFreq)
static void DIO_SetDigitalFltClkFreq(portreg_ext_struct_t *port,uint16_t filterFrequency)
{
}
#pragma inline(DIO_GetDigitalFltFrequency) 
static uint16_t DIO_GetDigitalFltFrequency(portreg_ext_struct_t *port)
{
    return 0;
}
#pragma inline(DIO_SetDigitalFilterNsamples)
static void DIO_SetDigitalFilterNsamples(portreg_ext_struct_t *port,uint8_t nSamples)
{
}
#pragma inline(DIO_GetDigitalFilterNsamples)
static uint8_t DIO_GetDigitalFilterNsamples(portreg_ext_struct_t *port)
{
    return (uint8_t)(0);    
}
#pragma inline(DIO_EnableInterruptOnPort)
static void DIO_EnableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma inline(DIO_DisableInterruptOnPort)
static void	DIO_DisableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma inline(DIO_InterruptOnPort)
static bool_t DIO_InterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#define DIO_DMA_IS_AVAILABLE  FALSE
#pragma inline(DIO_DMA_IsAvaibleOn)
static bool_t DIO_DMA_IsAvaibleOn(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#pragma inline(DIO_EnableDMAOnPort)
static void DIO_EnableDMAOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma inline(DIO_DisableDMAOnPort)
static void DIO_DisableDMAOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma inline(DIO_DMA_OnPort)
static bool_t DIO_DMA_OnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;   
}
#pragma inline(DIO_LevelDetectIntrpt)
static bool_t DIO_LevelDetectIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#pragma inline(DIO_EdgeDetectIntrpt)
static bool_t DIO_EdgeDetectIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;    
}
#pragma inline(DIO_GetDetectModeIntrpt)
static uint8_t DIO_GetDetectModeIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#endif /* #if defined(HAL_DIO_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_TIMER_SUPPORT)
/* Timer counter structure */
typedef struct
{
    uint8_t timerCntrl;
    uint16_t cnt;
    uint16_t mod;    
}tmrcntreg_struct_t;

/* Timer channel structure */
typedef struct
{
    uint8_t sc;
    uint16_t v;
} tmrchreg_struct_t;

static TMR_CNT_AddressType TmrCntMap[NUM_HW_CNTR]={	
(TMR_CNT_AddressType)&TZ0.TCR.BYTE,		/*Timer Z0 Control Register Addr.*/
(TMR_CNT_AddressType)&TZ1.TCR.BYTE, 	/*Timer Z1 Control Register Addr.*/
(TMR_CNT_AddressType)&TB1.TMB1.BYTE,	/*Timer B1 Mode Register Addr.*/
(TMR_CNT_AddressType)&TV.TCRV0.BYTE		/*Timer V Control Register V0 */  
};

static TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH]={
{(TMR_CH_AddressType)&TZ0.TCR.BYTE, 0},
{(TMR_CH_AddressType)&TZ0.TCR.BYTE, 0},
{(TMR_CH_AddressType)&TZ0.TCR.BYTE, 0},
{(TMR_CH_AddressType)&TZ0.TCR.BYTE, 0},
{(TMR_CH_AddressType)&TZ0.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 1},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 2},
{(TMR_CH_AddressType)&TZ1.TCR.BYTE, 2},  
};

/* Empty declaration, no clock gate available */
#define TMR_ClockGate(hwCntrlId,gate)

/* Set Timers's Prescaler */
#pragma inline (TMR_SetPrescaler)
static void TMR_SetPrescaler(tmrcntreg_struct_t *pTmr_cnt, PRS_ValueType prescaler)
{

	if((TMR_CNT_AddressType)pTmr_cnt == (TMR_CNT_AddressType)&TV.TCRV0.BYTE)
	{
		if( (prescaler != 0x00) && (prescaler <= 7))
		{
			pTmr_cnt->timerCntrl&=(0x07^0xFF);
			pTmr_cnt->timerCntrl|=(prescaler>>1);
			if(prescaler&0x01)
			{
				TV.TCRV1.BYTE |= 0x01;
			}
			else
			{
				TV.TCRV1.BYTE &= (0x01^0xFF);
			}
		}	
	}
	else if((TMR_CNT_AddressType)pTmr_cnt == (TMR_CNT_AddressType)&TB1.TMB1.BYTE)
	{
		if( prescaler != 0x07)
		{
			pTmr_cnt->timerCntrl&=(0x07^0xFF);
			pTmr_cnt->timerCntrl|=(prescaler&0x07);
		}
	}
	else if(((TMR_CNT_AddressType)pTmr_cnt == (TMR_CNT_AddressType)&TZ0.TCR.BYTE)||
	        ((TMR_CNT_AddressType)pTmr_cnt == (TMR_CNT_AddressType)&TZ1.TCR.BYTE))
	{
		pTmr_cnt->timerCntrl&=(0x03^0xFF);
		pTmr_cnt->timerCntrl|=(prescaler&0x03);	
	}
	else
	{
		/*do nothing*/
	}
}
/*	TimerB1 up counter 
	TimerV up counter 
	TimerZ0 and TimerZ1 increment/decrement only in complementary PWM mode
	otherwise up counter */
#pragma inline (TMR_SetCountDir)
static void TMR_SetCountDir(tmrcntreg_struct_t *pTmr_cnt, uint8_t countingMode)
{
	
}
   

#endif /* #if defined(HAL_TIMER_SUPPORT) */

/* ********************************************************************************************** */
/*                                   END TMR Class HAL Support                                    */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN WDT Class HAL Support                                   */
/* ********************************************************************************************** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define WatchDog Capability */
#define _HAL_WDT_CAPABILITY

/* Map mcu specific registers names to a 'standard' set of names 
              
              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>
              
    (this helps when generating new files for additional parts of the same family).
*/
/** This is the WatchDog Timer control/status register */
#define _HAL_300H_WDT_TCSRWD_REG			(WDT.TCSRWD.BYTE)
/** This is the WatchDog Timer counter */
#define _HAL_300H_WDT_TCWD_REG				(WDT.TCWD)
/** This is the WatchDog Timer mode register */
#define _HAL_300H_WDT_TMWD_REG				(WDT.TMWD.BYTE)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/**
WatchDog Setup.
*/
#define WDT_InitSync()	/* just to compile */
#pragma inline (WDT_Init)
static void WDT_Init(bool_t enable)
{
	if(enable)
	{	
		/* set dedicated WD internal oscillator,
		   overflow time: min. 200 msec, typ. 400 msec 
		   at Reset WDTimer is On ! */ 
		_HAL_300H_WDT_TMWD_REG = 0xF7;

	}
	else
	{
		_HAL_300H_WDT_TCSRWD_REG = 0x8E;		/* enable to write in TCSRWE */
		_HAL_300H_WDT_TCSRWD_REG = 0x9E;		/* write in TCSRWE */
		_HAL_300H_WDT_TCSRWD_REG = 0x96;		/* enable to write in WDON */
		_HAL_300H_WDT_TCSRWD_REG = 0x92;		/* write WDON -> disable watchdog */
		_HAL_300H_WDT_TCSRWD_REG = 0x9E;		/* lock write on WDON and write '0' on TCSRWE */
		_HAL_300H_WDT_TCSRWD_REG = 0xAE;		/* lock write on TCSRWE */
	} 	
}
/* Just To Compile */
#define WDT_Refresh()
/**
Feed the dog\n
1 tick = 1,5625 msec typ., min. 0,78125 msec. WD window  = (0xFF-watchdog_tout)*tick msec.
*/
#define WDT_FeedTheDog(watchdog_tout)\
{\
	_HAL_300H_WDT_TCSRWD_REG = 0x2E;\
	_HAL_300H_WDT_TCSRWD_REG = 0x6E;\
	_HAL_300H_WDT_TCWD_REG = watchdog_tout;\
	_HAL_300H_WDT_TCSRWD_REG = 0x2E;\
	_HAL_300H_WDT_TCSRWD_REG = 0xAE;\
}    
/**
Unsupported 
*/
#define WDT_SetSync(timeout)
/**
Unsupported 
*/
#define WDT_StopSync()
/**
Unsupported 
*/  
#define WDT_SetWindowingSync(window)
/**
Unsupported 
*/  
#define WDT_StopWindowingSync()
/**
Unsupported 
*/  
#define WDT_Counter()
/**
Unsupported 
*/  
#define WDT_RSTCounter()
/**
Unsupported 
*/  
#define WDT_ClearRSTCounter()
/**
Unsupported 
*/  
#define WDT_SetQuickTestSync(timeout)
/**
Unsupported 
*/  
#define WDT_SetByteTestSync(timeout, testByte)
/**
Unsupported 
*/  
#define WDT_IsTest()

/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)
/* A/D Control/Status Register (ADCSR) */
#define _HAL_300H_ADC_ADCSR_REG		AD.ADCSR.BYTE
/* This is the bit position of A/D End Flag  bit */
#define _HAL_S08_ADC_ADCSR_ADF_BITPOS			7
/* This is the bit position of A/D Interrupt Enable bit */
#define _HAL_S08_ADC_ADCSR_ADIE_BITPOS			6
/* This is the bit position of A/D Start bit */
#define _HAL_S08_ADC_ADCSR_ADST_BITPOS			5
/* This is the bit position of Scan Mode bit */
#define _HAL_S08_ADC_ADCSR_SCAN_BITPOS			4
/* This is the bit position of Clock Select  bit */
#define _HAL_S08_ADC_ADCSR_CKS_BITPOS			3
/* This is the mask of channel selection */
#define _HAL_S08_ADC_ADCSR_CHANNEL_MASK			7


/* A/D Control Register (ADCR) */
#define _HAL_300H_ADC_ADCR_REG		AD.ADCR.BYTE
/* This is the bit position of Trigger Enable  bit */
#define _HAL_S08_ADC_ADCR_TRGE_BITPOS			7

/* A/D Data Registers A to D (ADDRA to ADDRD) */
#define _HAL_300H_ADC_ADDRA_REG		AD.ADDRA
#define _HAL_300H_ADC_ADDRB_REG		AD.ADDRB
#define _HAL_300H_ADC_ADDRC_REG		AD.ADDRC
#define _HAL_300H_ADC_ADDRD_REG		AD.ADDRD

/* Port Data Register shared with ADC */
#define PORT_DATA_SHARED_WITH_ADC	(IO.PDRB.BYTE)

/* Clear the End Conversion flag, a test on ADF bit is required before clear the bit 
cfr. rej09b0216_h836077hm[1].pdf pag. 371 */
#pragma inline (ADC_ClearEndConvPendingFlag)
static void ADC_ClearEndConvPendingFlag(void)
{
	if (_HAL_300H_ADC_ADCSR_REG & (1<<_HAL_S08_ADC_ADCSR_ADF_BITPOS))
	{
		_HAL_300H_ADC_ADCSR_REG &= ((1<<_HAL_S08_ADC_ADCSR_ADF_BITPOS)^0xFF);	
	} 
}
/* Starting from selected channel return the correspondent A/D result register */
#pragma inline (getADCresultReg)
static void *getADCresultReg(uint8_t channel)
{
void *pADCresultReg = NULL;
	switch (channel)
	{
		case 0:
		case 4:
			pADCresultReg = &_HAL_300H_ADC_ADDRA_REG; 
			break;

		case 1:
		case 5:
			pADCresultReg = &_HAL_300H_ADC_ADDRB_REG; 
			break;

		case 2:
		case 6:
			pADCresultReg = &_HAL_300H_ADC_ADDRC_REG; 
			break;

		case 3:
		case 7:
			pADCresultReg = &_HAL_300H_ADC_ADDRD_REG; 
			break;
	} 
	return(pADCresultReg);
}

/*
\n\li Select Convertion Time
\n\li Select Single Mode Convertion
\n\li No End Convertion Interrupt
\n\li Note 1: On this target result register data is ever left justified
\n\li Note 2: On this target resolution is ever 10-bit
*/
#define ADC_Init()\
{\
	_HAL_300H_ADC_ADCSR_REG =(ADC_HIGH_FRQ_CONV<<_HAL_S08_ADC_ADCSR_CKS_BITPOS);\
}
/*
Set all ADC Registers to the reset state value
*/
#define ADC_DeInit()\
{\
	_HAL_300H_ADC_ADCSR_REG = 0x00;\
	_HAL_300H_ADC_ADCR_REG &= ((1<<_HAL_S08_ADC_ADCR_TRGE_BITPOS)^0xFF);\
}
/*
'0' if channel is allowable '1' otherwise 
*/
#define ADC_CheckChannel(channel)	((channel<=7)?0:1)

/*
No Channel Configuration needed
*/
#define ADC_ConfigurePinAsChannel(channel)	

/*
\n Writes to the ATD status and control register 
\n\li clear the A/D End Flag. 
\n\li select channel
\n\li clear or set ATD Interrupt
\n\li start a new conversion.
*/
#pragma inline(ADC_StartConvertionOnChannel)
static void ADC_StartConvertionOnChannel(uint8_t channel,bool_t atdInterrupt)
{
	
	ADC_ClearEndConvPendingFlag();
	
	_HAL_300H_ADC_ADCSR_REG &= (_HAL_S08_ADC_ADCSR_CHANNEL_MASK^0xFF);
	_HAL_300H_ADC_ADCSR_REG |= channel;
	
	if ( atdInterrupt )
	{
		_HAL_300H_ADC_ADCSR_REG |= (1<<_HAL_S08_ADC_ADCSR_ADIE_BITPOS);
	}
	else
	{
		_HAL_300H_ADC_ADCSR_REG &= ((1<<_HAL_S08_ADC_ADCSR_ADIE_BITPOS)^0xFF);
	}
	_HAL_300H_ADC_ADCSR_REG |= (1<<_HAL_S08_ADC_ADCSR_ADST_BITPOS);
	
}

/**
\n Test End Convertion Flag.
\n The ADF is a read/write bit which is set each time a conversion is complete. 
*/
#define ADC_EndConvertion()	(_HAL_300H_ADC_ADCSR_REG & (1<<_HAL_S08_ADC_ADCSR_ADF_BITPOS))
/**
\n\li ADC Conversion Result is ever in 10 bit left justified mode
\n\li This macro modify it to the desired resolution ever right justified
*/
#define ADC_GetCorrectResolutionResult(result)\
result >>= 6;\
((ADC_RESOLUTION > 10)?(result<<=(ADC_RESOLUTION-10)):(result>>=(10-ADC_RESOLUTION)))

/*
\n\li This function get the converted value, shift right of 6 position the value and 
\n\li modify it to the desired resolution right justified.
\n\li clear the end convertion pending flag.
 */	
#pragma inline (ADC_GetConvertedValueOnChannel)
static uint16_t ADC_GetConvertedValueOnChannel(uint8_t chan)/* (void) */
{
void *pADCresultReg;
uint16_t adConvValue;
uint8_t channel;
	/* check the active channel */
	channel = (_HAL_300H_ADC_ADCSR_REG & _HAL_S08_ADC_ADCSR_CHANNEL_MASK);
	pADCresultReg = getADCresultReg(channel);
	adConvValue = *(uint16_t *)pADCresultReg; 
    ADC_GetCorrectResolutionResult(adConvValue);
	ADC_ClearEndConvPendingFlag();	
	return(adConvValue);
}


/*
\n Wait for Conversion Complete. 
*/
#define ADC_WaitForConvertionOnChannel(channel)	while((_HAL_300H_ADC_ADCSR_REG & (1<<_HAL_S08_ADC_ADCSR_ADF_BITPOS))== 0){}
/* Stop the convertion in Single Mode */
#define ADC_StopConversion()\
{\
_HAL_300H_ADC_ADCSR_REG &= (((1<<_HAL_S08_ADC_ADCSR_ADST_BITPOS)|(1<<_HAL_S08_ADC_ADCSR_ADIE_BITPOS))^0xFF);\
ADC_ClearEndConvPendingFlag();\
}

#define ADC_pResultRegister(channel)	getADCresultReg(channel)
	
#define ADC_CorrectChannel(channel)\
(channel | (1<<_HAL_S08_ADC_ADCSR_ADIE_BITPOS) | (1<<_HAL_S08_ADC_ADCSR_ADST_BITPOS) | (ADC_HIGH_FRQ_CONV<<_HAL_S08_ADC_ADCSR_CKS_BITPOS))

#define ADC_StartMultiSampleConvertionMultiChannel()

/*
\n Writes to the ATD status and control register 
\n\li cancels any pending interrupts
\n\li select channel
\n\li set ATD Interrupt
\n\li initiates a new conversion on channel
*/
#pragma inline (ADC_StartMultiSampleConvertionOnChannel)
static void ADC_StartMultiSampleConvertionOnChannel(uint8_t channel)
{
uint8_t mirrorADCSR;

	ADC_ClearEndConvPendingFlag();

	mirrorADCSR = _HAL_300H_ADC_ADCSR_REG;
	mirrorADCSR &= (_HAL_S08_ADC_ADCSR_CHANNEL_MASK^0xFF);
	mirrorADCSR |= channel;
	mirrorADCSR |= (1<<_HAL_S08_ADC_ADCSR_ADIE_BITPOS);
	mirrorADCSR |= (1<<_HAL_S08_ADC_ADCSR_ADST_BITPOS);
	_HAL_300H_ADC_ADCSR_REG = mirrorADCSR; 	
}
#define ADC_pChannelRegister(channel)  (&_HAL_300H_ADC_ADCSR_REG)
/*
Channel selected for ATD convertion
*/
#define ADC_GetActiveChannel()	(_HAL_300H_ADC_ADCSR_REG & _HAL_S08_ADC_ADCSR_CHANNEL_MASK )
/*
-Clear End Conv. Pending Flag
-Start a new conversion on the selected channel
*/
#define ADC_EmulateContinuousConversion()\
ADC_ClearEndConvPendingFlag();\
_HAL_300H_ADC_ADCSR_REG |= (1<<_HAL_S08_ADC_ADCSR_ADST_BITPOS);

/*
Starting from portAddress and pinMask (read pin) search for AD channel  
*/
#pragma inline (ADC_GetChannel)
static bool_t ADC_GetChannel(DIO_ModuleMapPortAddressType portAddress,DIO_ModuleMapMaskType pinMask,uint8_t *pADch)
{
uint8_t i;
    if ( portAddress.basic == (DIO_PortAddressBasicType)&PORT_DATA_SHARED_WITH_ADC )
    {
        for (i=0;i<8;i++)
        {
            if (pinMask == (1u<<i))
            {
                *pADch = i;
                return 1;                    
            }            
        }
    }
    return 0;
}

#endif /* #if defined(HAL_ADC_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */

#endif /* __HAL_RENESAS_300H_39079_H */

/* *********** */
/* End of file */
/* *********** */