/**
\defgroup HAL_NX_M0_LPC1229  NXP M0 LPC1229 HAL
\ingroup NX_M0_LPC1229
*/

/**
\defgroup HAL_NX_M0_LPC1227_CLS  NXP M0 LPC1227 supported drivers Classes

This module lists the driver classes supported by this HAL.
\ File NX_M0_LPC1227_hal.h
\ ingroup HAL_NX_M0_LPC1227
\ 03/09/2011
\ author Massimo Incerti
\ version 00.00.03




\ File NX_M0_LPC1227_hal.h
\ ingroup HAL_NX_M0_LPC1227
\ 03/08/2011
\ author Massimo Incerti
\ version 00.00.02



*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    NX_M0_LPC1229_hal.h
\ingroup HAL_NX_M0_LPC1229
\date    12/03/2009
\author  Roberto Fioravanti
\version 00.00.01


\addtogroup HAL_NX_M0_LPC1229_HISTORY_HAL
@{
\n        00.00.10
\n            10/06/2011
\n            Massimo Incerti
\n            Changes
\n             -- ADC support completed (missing Multichannel Mutlisamples support with SWDMA)
\n             -- RESET support completed, added SYS_HardFaultReset() routine.

\n      
\n        00.00.09
\n            xx/xx/2011
\n            Giancarlo Parodi
\n	      -- WDT support added and completed	
\n
\n
\n
\n
\n
\n         00.00.08
\n             05/23/2011
\n             Massimo Incerti
\n             Changes
\n             - CLK support added
\n             - IRQ support added
\n             - ADC support added (to be tested and completed) 
\n             - Reset support added
\n             - LVD support added 
\n             - DMA support added (to be tested and completed)
\n
\n
\n
\n
\n	    00.00.07	
\n 		04/11/2011
\n		Giancarlo Parodi
\n		Changes
\n		- TIMER support added (output compare, input capture, pwm)
\n 		- cleaned up unused Freescale specific sections at end of module, originally commented out with #if(0)
\n
\n	    00.00.06	
\n 		03/29/2011
\n		Massimo Incerti
\n		Changes
\n		- DIO support added
\n              - Changes in the HAL for DIO reflect the chenges in OCDIO_drv.c ver
\n                00.00.02   10 May 2011\n		
\n		
\n	    00.00.05	
\n 		03/29/2011
\n		Giancarlo Parodi
\n		Changes
\n		- FLASH support added
\n              - removed unneeded multiple inclusions of HAL, HAL_CFG
\n              - removed WDT_refresh (done by OCFLS driver)
\n              - DRV_TEST_MODE is used to include empty definitions for 
\n                critical sections
\n
\n 		00.00.04
\n		03/16/2011
\n		Massimo Incerti
\n		Changes
\n              - UART support added 
\n
\n
\n 		00.00.03
\n		03/09/2011
\n              Massimo Incerti
\n 		Changes
\n		- CRC Support added
\n		
\n		
\n 		00.00.02
\n              03/09/2011
\n 		Massimo Incerti
\n		Changes
\n		- I2C support added
\n	
\n    
\n
\n 		00.00.01 
\n		02/08/2011 
\n		Massimo Incerti
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

/**
\addtogroup HAL_NX_M0_LPC1227_CLS
@{*/



#ifndef __HAL_NX_LPC12xx_HAL_H
#define __HAL_NX_LPC12xx_HAL_H





/* ******** */
/* Includes */
/* ******** */
#include "Config.h"                               /* For: Application configuration */
#include "NX_M0_LPC1227_reg.h"

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

/** \defgroup HAL_NX_M0_LPC1229_I2C  I2C Class driver support    
 */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY



#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR                        (1)
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK                (0)
#define OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK                               (1)


/** Define I2C bit masks for Control registers (clear and Set)  */

/* I2C Control Set Register */
#define I2CONSET_I2EN       (0x1<<6)  
#define I2CONSET_AA         (0x1<<2)
#define I2CONSET_SI         (0x1<<3)
#define I2CONSET_STO        (0x1<<4)
#define I2CONSET_STA        (0x1<<5)

/* I2C Control clear Register */
#define I2CONCLR_AAC        (0x1<<2)  
#define I2CONCLR_SIC        (0x1<<3)
#define I2CONCLR_STAC       (0x1<<5)
#define I2CONCLR_I2ENC      (0x1<<6)


/** Define I2C bit masks for Status Register  */


/*    I2C Peripheral in Slave Mode  */

#define SLAVE_TX_GIVE_ACK              0xA8
#define SLAVE_TX_RECEIVE_ACK           0xB8
#define SLAVE_TX_RECEIVE_NACK          0xC0
#define SLAVE_RX_GIVE_FIRST_ACK        0x60
#define SLAVE_RX_GIVE_ACK              0x80


/*     I2C Peripheral in Master Mode*/

#define START_CONDITION                0x08
#define MASTER_GIVE_ACK                0x18
#define MASTER_GIVE_ACK_LAST_BYTE      0x28
#define MASTER_RECEIVE_ACK             0x40
#define MASTER_RECEIVE_NACK            0x48
#define MASTER_RX_GIVE_ACK             0x50
#define MASTER_GIVE_NACK               0x58
#define MASTER_ARBITRATION_LOST        0x38
#define MASTER_RESTART_CONDITION       0x10

/*     Miscellaneous States      */


#define I2C_UNKNOWN_STATE                  (0xF8)




/* 
I2C related registers redefinition                           
This approach helps when generating new files for additional parts of the same family.
*/
/*HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION> */


/* Address Registers*/
#define _HAL_M0_I2C_SAR0_REG                        (LPC_I2C->ADR0)    
#define _HAL_M0_I2C_SAR1_REG                        (LPC_I2C->ADR1)    
#define _HAL_M0_I2C_SAR2_REG                        (LPC_I2C->ADR2)    
#define _HAL_M0_I2C_SAR3_REG                        (LPC_I2C->ADR3)    
/* Control Registers Set and Reset*/
#define _HAL_M0_I2C_CTRL_REG                        (LPC_I2C->CONCLR)  
#define _HAL_M0_I2C_ENABLE_REG                      (LPC_I2C->CONSET)  

/* Flow Control REgisters */
#define _HAL_M0_I2C_DISABLE_REG                     (LPC_I2C->CONCLR)      // Clear Register
#define _HAL_M0_I2C_TX_CONTROL_REG                  ((LPC_I2C->DAT)&0x01)  // Always the R/W bit in the address independently form slave and master roles
#define _HAL_M0_I2C_START_REG                       (LPC_I2C->CONSET) 
#define _HAL_M0_I2C_STOP_REG                        (LPC_I2C->CONSET)
#define _HAL_M0_I2C_RESTART_REG                     (LPC_I2C->CONSET)
#define _HAL_M0_I2C_TXAK_EN_REG                     (LPC_I2C->STAT)        // Depending on the status register in every moment of the transitions


/* Status Registers*/
#define _HAL_M0_I2C_STATUS_REG                      (LPC_I2C->STAT)       // Status Register 
#define _HAL_M0_I2C_BUSBUSY_REG                     (LPC_I2C->STAT)       // Status of busy every status except oxF8 which is bus free condition
#define _HAL_M0_I2C_AL_REG                          (LPC_I2C->STAT)       


/* Address Mask and R/W Registers */
#define _HAL_M0_I2C_ADDRMATCH0_REG                  (LPC_I2C->MASK0)   
#define _HAL_M0_I2C_ADDRMATCH1_REG                  (LPC_I2C->MASK1)   
#define _HAL_M0_I2C_ADDRMATCH2_REG                  (LPC_I2C->MASK2)   
#define _HAL_M0_I2C_ADDRMATCH3_REG                  (LPC_I2C->MASK0)   
#define _HAL_M0_I2C_SLAVE_RW_REG                    ((LPC_I2C->DAT)&0x01)



/* I2C Enable Interrupt Register*/
#define _HAL_M0_I2C_INT_REG                         (NVIC->ISER[0]=(1 << ((uint32_t)(12) & 0x1F)));


/* Ack/Nack Flag Registers*/
#define _HAL_M0_I2C_RXACKBITVAL_REG                 (LPC_I2C->STAT)  // Check ACK and NAK depending on the status codes (see UM)
#define _HAL_M0_I2C_MASTER_REG                      (LPC_I2C->CONSET)
#define _HAL_M0_I2C_MASTERCLR_REG                   (LPC_I2C->CONCLR)
#define _HAL_M0_I2C_TXACKBITVAL_REG                 (LPC_I2C->CONSET)  
#define _HAL_M0_I2C_TXACKBITVALCLR_REG              (LPC_I2C->CONCLR)


/*Interrupt Flags Set and Clear*/

#define _HAL_M0_I2C_INT_REQ_FLAG_REG                (LPC_I2C->CONSET)  // To be rechecked interrupt flag
#define _HAL_M0_I2C_INT_REQ_FLAG_REG_CLR            (LPC_I2C->CONCLR)




/* TX and RX Data Registers*/
#define _HAL_M0_I2C_TX_DATA_REG                     (&(LPC_I2C->DAT))   // Data Buffer I2C TX         
#define _HAL_M0_I2C_RX_DATA_REG                     (&(LPC_I2C->DAT))   // Data Buffer I2C RX
#define _HAL_M0_I2C_DATA_REG                        (LPC_I2C->DAT)



/* Interrupt Flag set and reset Registers*/
#define _HAL_M0_I2C_INT_REQ_REG                     (LPC_I2C->CONSET)   // To be recheked Interrupt flag in I2C0CONSET register
#define _HAL_M0_I2C_INT_REQ_CLR_REG                 (LPC_I2C->CONCLR)  // Clear interrupt flag register



/* I2C Communication Speed Registers*/
#define _HAL_M0_I2C_SCLH_REG                        (LPC_I2C->SCLH)     //NEW     // Different implementation from Freescale recheck Clock generation
#define _HAL_M0_I2C_SCLL_REG                        (LPC_I2C->SCLL)     //NEW    // Different implementation from Freescale recheck Clock generation



/*  I2C Pins Logic levels*/
#define _HAL_M0_I2C_SDA_IO_REG                      ((LPC_GPIO0->PIN)&(0x01<<11))    // Data register for I2C pin SDA  
#define _HAL_M0_I2C_SCL_IO_REG                      ((LPC_GPIO0->PIN)&(0x01<<10))    // Data register for I2C pin SCL  



/*  Not used in this implementation */
#define _HAL_M0_I2C_STOP_CLEAR                      (LPC_I2C->CONCLR)



/* Driver configuration */
/*
*/
#define OCI2C_ON_SLAVE_TX_EVENT_IN_ISR  (1)
#define OCI2C_ON_SLAVE_ADDRESSED_TX_EVENT_IN_DMACALLBACK (0)
#define OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK (1)
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

/* */
#define I2C_ClkEnable()   {}
/* */
#define I2C_ClkDisable()   {}
/* */
#define I2C_Enable()      {_HAL_M0_I2C_ENABLE_REG = I2CONSET_I2EN; \
                          _HAL_M0_I2C_MASTER_REG=I2CONSET_AA; \
                          LPC_IOCON->PIO0_10 = 0x102;  /* I2C Functionality SCL*/ \
                          LPC_IOCON->PIO0_11 = 0x102;  /* I2C Functionality SDA*/ \
                          }
/* */
#define I2C_Disable()     { _HAL_M0_I2C_DISABLE_REG = I2CONCLR_I2ENC;  \
                          LPC_IOCON->PIO0_10 = 0x02;  /*  PIO0_10 Functionality*/ \
                          LPC_IOCON->PIO0_11 = 0x02;  /*  PIO0_11 Functionality*/ \
                          }
/* */
#define I2C_CtrlReset()   { _HAL_M0_I2C_CTRL_REG = (0x1F<<2); }     
/* */
#define I2C_IntRegReset() {} 
/* */
#define I2C_Reset()       I2C_OnIntAck();
/**/
#define I2C_ReadData()    {_HAL_M0_I2C_RX_DATA_REG; }
/**/
#define I2C_WriteData(b)  {_HAL_M0_I2C_DATA_REG = ((I2C_DataRegType)(b)); }
#define I2C_SendAddr(b)   I2C_WriteData(b)
/* */                    
#define I2C_pReceiveDataRegister() (_HAL_M0_I2C_RX_DATA_REG)
/* */
#define I2C_pTransmitDataRegister() (_HAL_M0_I2C_TX_DATA_REG)



/* Configuration */

/**/
#define     I2C_MasterEnable()      {_HAL_M0_I2C_ENABLE_REG=I2CONSET_I2EN; }         
/**/
#define     I2C_MasterDisable()     { }         
/**/
#define     I2C_SetClockRate(b)     {_HAL_M0_I2C_SCLH_REG=BUS_FREQUENCY_Hz  / (2*(b)*1000) ;_HAL_M0_I2C_SCLL_REG=(BUS_FREQUENCY_Hz  / (b*1000))-LPC_I2C->SCLH; }
/**/
#define     I2C_ResetClockRate(b)    {_HAL_M0_I2C_SCLH_REG=0;_HAL_M0_I2C_SCLL_REG=0; }
/**/
#define     I2C_SetSlaveAddress(b)  {_HAL_M0_I2C_SAR0_REG = (I2C_SlaveAddressType)(b); }     
/**/
#define     I2C_SetTxMode()         {}
/**/
#define     I2C_SetRxMode()         {}




/****************************************************************************/
/*Questa � valida per il solo slave*/ //#define     I2C_IsTxMode()     ((_HAL_M0_I2C_STATUS_REG ==SLAVE_TX_GIVE_ACK))
#define     I2C_IsTxMode()     ((_HAL_M0_I2C_STATUS_REG ==SLAVE_TX_RECEIVE_ACK))

/*Includendo anche il master */
//#define     I2C_IsTxMode()     ((_HAL_M0_I2C_STATUS_REG ==SLAVE_TX_GIVE_ACK)||(_HAL_M0_I2C_STATUS_REG ==START_CONDITION)||(_HAL_M0_I2C_STATUS_REG ==MASTER_GIVE_ACK)||(_HAL_M0_I2C_STATUS_REG ==MASTER_GIVE_ACK_LAST_BYTE))

/*****************************************************************************/


/***************************************************************************/
#define     I2C_ClearStart()    {_HAL_M0_I2C_STOP_CLEAR =(I2CONCLR_SIC)|(I2CONCLR_STAC);}

/**************************************************************************/


/*Start & Stop condition */

/**************************************************************************/


#define     I2C_IssueStartCondition() {  _HAL_M0_I2C_ENABLE_REG = I2CONSET_I2EN;  _HAL_M0_I2C_START_REG = I2CONSET_STA; }


/**************************************************************************/



/**************************************************************************/
#define     I2C_IssueReStartCondition() {   _HAL_M0_I2C_RESTART_REG = I2CONSET_STA; }
/**************************************************************************/



/**************************************************************************/
#define     I2C_IssueStopCondition()  {   _HAL_M0_I2C_STOP_REG = I2CONSET_STO; }  
/**************************************************************************/

/* Interrupt management */

/**************************************************************************/
/* Functions in core_cm0.h*/
#define     I2C_IntEnable()   { NVIC_EnableIRQ(I2C_IRQn); }               
/**/
#define     I2C_IntDisable()  { NVIC_DisableIRQ(I2C_IRQn); }              
/**************************************************************************/


/*Peripheral state management*/
/******************************************************************************/
#define     I2C_IsBusBusy() (_HAL_M0_I2C_BUSBUSY_REG != I2C_UNKNOWN_STATE ) 
/******************************************************************************/
/*******************************************************************************/
#define     I2C_IsSdaIoHigh() (_HAL_M0_I2C_SDA_IO_REG != 0x00)           
/********************************************************************************/
/********************************************************************************/
#define     I2C_IsSclIoHigh() ( _HAL_M0_I2C_SCL_IO_REG != 0x00 )         
/********************************************************************************/
/**/
#define     I2C_GetStatus() (_HAL_M0_I2C_STATUS_REG)           
/**/
#define     I2C_ResetStatus() 	{(void)I2C_GetStatus(); /*_HAL_300H_I2C_STATUS_REG=0;*/ }

/*********************************************************************************************/
#define     I2C_ResetIntReqFlag() {_HAL_M0_I2C_INT_REQ_FLAG_REG_CLR = I2CONCLR_SIC;} 
/*********************************************************************************************/
/**/


/* Implemented arbitration lost in case of master communication only */
/***********************************************************************************************/
#define     I2C_IsArbitrationLost()  (_HAL_M0_I2C_AL_REG == MASTER_ARBITRATION_LOST)
/***********************************************************************************************/





/***********************************************************************************************/
#define     I2C_IsSlaveAddressed()  ((_HAL_M0_I2C_STATUS_REG==SLAVE_RX_GIVE_FIRST_ACK)||(_HAL_M0_I2C_STATUS_REG==SLAVE_TX_GIVE_ACK) )
/***********************************************************************************************/






/* */
#define     I2C_IsGeneralCallAddress() FALSE
/* */
#define     I2C_ResetSlaveAddressedSig() {}
/* */
#define     I2C_ResetGeneralCallAddressSig() {}




/*********************************************************************************************/
#define     I2C_ResetArbitrationLost() {_HAL_M0_I2C_INT_REQ_CLR_REG = I2CONCLR_SIC;}    
/*********************************************************************************************/


/***********************************************************************************************/
#define     I2C_IsSlaveTrasmitRequest() ((_HAL_M0_I2C_STATUS_REG == SLAVE_TX_GIVE_ACK)||(_HAL_M0_I2C_STATUS_REG == SLAVE_TX_RECEIVE_ACK))  
/***********************************************************************************************/


/************************************************************************************************/
#define     I2C_IsAcknowledgeEnabled() (((_HAL_M0_I2C_TXACKBITVAL_REG)&(I2CONSET_AA)) == 0)     
/************************************************************************************************/

/************************************************************************************************/
/**/
#define     I2C_IsMaster() ((_HAL_M0_I2C_STATUS_REG == START_CONDITION)||(_HAL_M0_I2C_STATUS_REG == MASTER_GIVE_ACK)||(_HAL_M0_I2C_STATUS_REG == MASTER_GIVE_ACK_LAST_BYTE)||(_HAL_M0_I2C_STATUS_REG == MASTER_RECEIVE_ACK)||(_HAL_M0_I2C_STATUS_REG == MASTER_GIVE_NACK)||(_HAL_M0_I2C_STATUS_REG == MASTER_RX_GIVE_ACK)||(_HAL_M0_I2C_STATUS_REG == MASTER_RECEIVE_NACK)||(_HAL_M0_I2C_STATUS_REG == MASTER_RESTART_CONDITION))
/************************************************************************************************/




#define     I2C_ResetTxSig() {}




/*Acknowlegde bit  */

/**********************************************************************************************************/
#define     I2C_ReceivedAcknowledgeVal()   (((_HAL_M0_I2C_STATUS_REG)==(SLAVE_RX_GIVE_FIRST_ACK))||((_HAL_M0_I2C_STATUS_REG)==(SLAVE_RX_GIVE_ACK)))
/**********************************************************************************************************/




/**********************************************************************************************************/
#define     I2C_EnableAcknowledge()     {   _HAL_M0_I2C_TXACKBITVAL_REG = I2CONSET_AA; }
/**********************************************************************************************************/





/**********************************************************************************************************/
/*Questa funziona nel caso di Slave */
#define     I2C_DisableAcknowledge()    {  _HAL_M0_I2C_TXACKBITVAL_REG = I2CONSET_AA;  }

/*Questa nel caso di master*/
//#define     I2C_DisableAcknowledge()    {  _HAL_M0_I2C_CTRL_REG=I2CONCLR_AAC;_HAL_M0_I2C_CTRL_REG=I2CONCLR_SIC;  }

/*Al momento pare funzionante nel caso di Master E Slave, da riverificare eventualmente*/
//#define     I2C_DisableAcknowledge()    ( (I2C_IsMaster()==1)?(_HAL_M0_I2C_CTRL_REG=(I2CONCLR_AAC|I2CONCLR_SIC)):(_HAL_M0_I2C_TXACKBITVAL_REG=I2CONSET_AA))



/**********************************************************************************************************/




/**********************************************************************************************************/
#define     I2C_SlaveStartRx(){ _HAL_M0_I2C_MASTER_REG=I2CONSET_AA ;I2C_ResetIntReqFlag();}  	
/**********************************************************************************************************/



#define     I2C_SlaveStartTx()	\
          I2C_ResetIntReqFlag()\
        while((_HAL_M0_I2C_INT_REQ_FLAG_REG & I2CONCLR_SIC) == 0);  \
        I2C_IntEnable()  	

/* see: OCI2C_ON_SLAVE_TX_EVENT_IN_ISR */
#define     I2C_OnSlaveAddressMatchTransmit()\
            I2C_ResetSlaveAddressedSig();\
            I2C_DisableAcknowledge();\
            I2C_IntDisable()



/* This section contains HAL class extension */


#define I2C_NestedIsr_Begin()			/* empty */
#define I2C_NestedIsr_End()				/* empty */

#define I2C_OnEnterIsr()    			/* empty */
#define I2C_OnIntAck()      			I2C_ResetIntReqFlag()
#define I2C_DummyRead()     			I2C_ReadData()
/* Slave receives unexpected data */
#define I2C_OnSlaveRxDummy()                    I2C_DummyRead();
#define I2C_SlaveAddressedTx_Action()   /*empty*/

/*not applicable*/
#define I2C_Level3BlockMonitorSync()   0//(((_HAL_M0_I2C_STATUS_REG==START_CONDITION)|(_HAL_M0_I2C_STATUS_REG==MASTER_GIVE_ACK)|(_HAL_M0_I2C_STATUS_REG==MASTER_GIVE_ACK_LAST_BYTE))|((_HAL_M0_I2C_STATUS_REG==SLAVE_TX_GIVE_ACK)|(_HAL_M0_I2C_STATUS_REG==SLAVE_TX_RECEIVE_ACK)|(_HAL_M0_I2C_STATUS_REG==SLAVE_TX_RECEIVE_NACK))&(!((_HAL_M0_I2C_ENABLE_REG)&(I2CONSET_AA))))
/*not applicable*/
#define I2C_Level4BlockMonitorSync()   0//((_HAL_M0_I2C_STATUS_REG==SLAVE_RX_GIVE_FIRST_ACK)|(_HAL_M0_I2C_STATUS_REG==SLAVE_RX_GIVE_ACK))&(!((_HAL_M0_I2C_ENABLE_REG)&(I2CONSET_AA)))
/*not applicable*/
#define I2C_Level5BlockMonitorSync()   0//!(I2C_IsBusBusy()&((_HAL_M0_I2C_STATUS_REG==SLAVE_RX_GIVE_FIRST_ACK)|(_HAL_M0_I2C_STATUS_REG==SLAVE_RX_GIVE_ACK)))







/* ********************* */
/* I2C Class HAL Support */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */



/* ********************************************************************************************** */
/*                                  BEGIN DMA Class HAL Support                                   */
/* ********************************************************************************************** */




/*******   DMA   ************************************/

#define _HAL_M0_DMA_CHANNEL_COUNT                (21)
#define _HAL_M0_DMA_STATUS_REG                   (LPC_DMA->STATUS)
#define _HAL_M0_DMA_CFG_REG                      (LPC_DMA->CFG)
#define _HAL_M0_DMA_CTRL_BASE_PTR_REG            (LPC_DMA->CTRL_BASE_PTR)
#define _HAL_M0_DMA_ATL_CTRL_BASE_PTR_REG        (LPC_DMA->ATL_CTRL_BASE_PTR)
#define _HAL_M0_DMA_WAITONREQ_STATUS_REG         (LPC_DMA->WAITONREQ_STATUS)
#define _HAL_M0_DMA_CHNL_SW_REQUEST_REG          (LPC_DMA->CHNL_SW_REQUEST)
#define _HAL_M0_DMA_CHNL_USEBURST_SET_REG        (LPC_DMA->CHNL_USEBURST_SET)
#define _HAL_M0_DMA_CHNL_USEBURST_CLR_REG        (LPC_DMA->CHNL_USEBURST_CLR)
#define _HAL_M0_DMA_CHNL_REQ_MASK_SET_REG        (LPC_DMA->CHNL_REQ_MASK_SET)
#define _HAL_M0_DMA_CHNL_REQ_MASK_CLR_REG        (LPC_DMA->CHNL_REQ_MASK_CLR)
#define _HAL_M0_DMA_CHNL_ENABLE_SET_REG          (LPC_DMA->CHNL_ENABLE_SET)
#define _HAL_M0_DMA_CHNL_ENABLE_CLR_REG          (LPC_DMA->CHNL_ENABLE_CLR)
#define _HAL_M0_DMA_CHNL_PRI_ALT_SET_REG         (LPC_DMA->CHNL_PRI_ALT_SET)
#define _HAL_M0_DMA_CHNL_PRI_ALT_CLR_REG         (LPC_DMA->CHNL_PRI_ALT_CLR)
#define _HAL_M0_DMA_CHNL_PRIORITY_SET_REG        (LPC_DMA->CHNL_PRIORITY_SET)
#define _HAL_M0_DMA_CHNL_PRIORITY_CLR_REG        (LPC_DMA->CHNL_PRIORITY_CLR)
#define _HAL_MO_DMA_ERR_CLR_REG                  (LPC_DMA->ERR_CLR)
#define _HAL_MO_DMA_CHNL_IRQ_STATUS_REG          (LPC_DMA->CHNL_IRQ_STATUS)
#define _HAL_MO_DMA_IRQ_ERR_ENABLE_REG           (LPC_DMA->IRQ_ERR_ENABLE)
#define _HAL_MO_DMA_CHNL_IRQ_ENABLE_REG          (LPC_DMA->CHNL_IRQ_ENABLE)




typedef struct {
  uint8_t    cycle_ctrl:3;
  uint8_t    next_use_burst:1;
  uint32_t   nminus_1:9;
  uint8_t    r_power:4;
  uint8_t    src_prot_ctrl:3;
  uint8_t    dst_prot_ctrl:3;
  uint8_t    src_size:2;
  uint8_t    src_inc:2;
  uint8_t    dst_size:2;
  uint8_t    dst_inc:2;
 }chn_ctrl_data_Typedef;

typedef struct {

  uint32_t source_end_ptr;
  uint32_t dest_end_ptr;
  chn_ctrl_data_Typedef  chn_ctr_data;
}dma_chn_ctrl_data_Typedef;



#define DMA_CHANNEL_0			0  /*UART0 TX*/
#define DMA_CHANNEL_1			1  /*UART0 RX*/
#define DMA_CHANNEL_2			2  /*UART1 TX*/
#define DMA_CHANNEL_3			3  /*UART1 RX*/
#define DMA_CHANNEL_4			4  /*SSP TX*/
#define DMA_CHANNEL_5			5  /*SSP RX*/
#define DMA_CHANNEL_6			6  /*ADC*/
#define DMA_CHANNEL_7			7  /*RTC*/
#define DMA_CHANNEL_8			8  /*TMR32B0 MATCH0*/
#define DMA_CHANNEL_9			9  /*TMR32B0 MATCH1*/
#define DMA_CHANNEL_10			10 /*TMR32B1 MATCH0*/
#define DMA_CHANNEL_11			11 /*TMR32B1 MATCH1*/
#define DMA_CHANNEL_12			12 /*TMR16B0 MATCH0*/
#define DMA_CHANNEL_13			13 /*TMR16B1 MATCH0*/
#define DMA_CHANNEL_14			14 /*Comparator 0*/
#define DMA_CHANNEL_15			15 /*Comparator 1*/
#define DMA_CHANNEL_16			16 /*PIO0*/
#define DMA_CHANNEL_17			17 /*PIO1*/
#define DMA_CHANNEL_18			18 /*PIO2*/



/*Memory buffer for DMA location */ 

#define DMA_CTRL_BUFFER_LOC          0x10001000   /*To be moved according to the projects settings*/
#define SIZEOF_DMA_CTRL              sizeof(dma_chn_ctrl_data_Typedef)

#define LPC_DMA_CHN0                 ((dma_chn_ctrl_data_Typedef*)  DMA_CTRL_BUFFER_LOC)
#define LPC_DMA_CHN1                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN2                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +2*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN3                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +3*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN4                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +4*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN5                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +5*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN6                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +6*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN7                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +7*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN8                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +8*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN9                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +9*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN10                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +10*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN11                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +11*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN12                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +12*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN13                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +13*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN14                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +14*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN15                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +15*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN16                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +16*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN17                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +17*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN18                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +18*SIZEOF_DMA_CTRL))


/*Memory buffer for DMA location */ 




#define _HAL_M0_DMA_STATUS_SHIFT_REG     ((_HAL_M0_DMA_STATUS_REG>>4)&0xF)
#define DMA_BLOCK_STALLED           0x08
#define DMA_BLOCK_DONE              0x09
#define DMA_BLOCK_GATHER_SCATTER    0x0A
#define DMA_BLOCK_IDLE              0x00







/**
Device assignment
*/

/*The assignement of the devices to the channels is fixed and mapped*/
/*We could use this function to activate and enable the single DMA channel*/

/*Or is it something that at peripheral level must be activated?*/

  
#define     DMA_SetDevice(channel, device)          {_HAL_M0_DMA_CHNL_ENABLE_SET_REG |= (0x01 << channel) ; }
#define     DMA_DisableDevice(channel, device)      {_HAL_M0_DMA_CHNL_ENABLE_CLR_REG |= (0x01 << channel) ; }




#define     DMA_GetDevice(channel) (_HAL_M0_DMA_CHNL_ENABLE_SET_REG) 



/**
DMA Source address assignment
*/

#define     DMA_SetSAR(channel, address) {LPC_DMA_CHN##channel->source_end_ptr=address;} 

#define     DMA_GetSAR(channel) (LPC_DMA_CHN##channel->source_end_ptr)

 
/**
DMA Destination address assignment
*/
#define     DMA_SetDAR(channel, address) { LPC_DMA_CHN##channel->dest_end_ptr=address; } 
#define     DMA_GetDAR(channel) (LPC_DMA_CHN##channel->dest_end_ptr)

/**
DMA Byte count register
*/
/*Number of DMA cycles that must be performed from 1 to 1024*/

#define     DMA_SetBCR(channel, count) {LPC_DMA_CHN##channel->chn_ctr_data.nminus_1=count;}
#define     DMA_GetBCR(channel) (LPC_DMA_CHN##channel->chn_ctr_data.nminus_1)


/**
DMA status register check
*/

#define     DMA_IsTransDone(channel) (LPC_DMA->CHNL_IRQ_STATUS&(0x01<<channel))

#define     DMA_IsChannelBusy(channel) (_HAL_M0_DMA_STATUS_SHIFT_REG !=DMA_BLOCK_IDLE)

#define     DMA_IsPending(channel) ((_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_STALLED)&(_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_DONE)&(_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_GATHER_SCATTER))


/*To be verified*/ 

#define     DMA_IsDestinationError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)

#define     DMA_IsSourceError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)

#define     DMA_IsConfigError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)




/**
DMA status register clear flags
*/


#define     DMA_ClearTransDone(channel) { *((uint8_t*)&(*(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)))) = (uint8_t)DSR0_DONE_MASK;}

/**
DMA control register setting
*/

#define     DMA_EnableInt(channel)  {_HAL_MO_DMA_CHNL_IRQ_ENABLE_REG |=(0x01<<channel); }
#define     DMA_DisableInt(channel) { _HAL_MO_DMA_CHNL_IRQ_ENABLE_REG &= ~(0x01<<channel); }


/*Functionalities that are covered by other functions in the same HAL implementation*/
#define     DMA_PeripheralRequestOn(channel) {  }
#define     DMA_PeripheralRequestOff(channel) { }


/*Functionalities covered automatically by the processor or in other registers.*/

#define     DMA_ContinuousTransfer(channel) {  }
#define     DMA_SingleTransfer(channel) {  }


/*Auto Align functionality to be discussed and checked in the user manual*/

#define     DMA_EnableAutoAlign(channel) {  }
#define     DMA_DisableAutoAlign(channel) { }



/*Source data structure setup macros*/

#define     DMA_EnableSourceIncrement(channel)  {LPC_DMA_CHN##channel->chn_ctr_data.src_inc=LPC_DMA_CHN##channel->chn_ctr_data.src_size;}
#define     DMA_DisableSourceIncrement(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_inc=DMA_DATA_NO_INC ; }

#define     DMA_SetSourceSizeToLong(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_LONG;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_LONG; }
#define     DMA_SetSourceSizeToByte(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_BYTE;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_BYTE; }
#define     DMA_SetSourceSizeToWord(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_WORD;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_WORD; }


/*Destination data structure setup macros*/

#define     DMA_EnableDestinationIncrement(channel) { LPC_DMA_CHN##channel->chn_ctr_data.dest_inc=LPC_DMA_CHN##channel->chn_ctr_data.dest_size;}
#define     DMA_DisableDestinationIncrement(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_inc=DMA_DATA_NO_INC ; }


/*There is the constrain that source and destination data size must be the same*/
/*Thus this macro will set both data sizes at the same time*/

#define     DMA_DATA_SIZE_BYTE   0
#define     DMA_DATA_SIZE_LONG   1   /*16 bit data?*/
#define     DMA_DATA_SIZE_WORD   2

#define     DMA_DATA_NO_INC      3



#define     DMA_SetDestinationSizeToLong(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_LONG;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_LONG;}
#define     DMA_SetDestinationSizeToByte(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_BYTE;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_BYTE;}
#define     DMA_SetDestinationSizeToWord(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_WORD;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_WORD;}


/*Software request DMa transfers*/

#define     DMA_SetStartOn(channel) { _HAL_M0_DMA_CHNL_SW_REQUEST_REG |=(0x01<<channel); }
#define     DMA_SetStartOff(channel) {_HAL_M0_DMA_CHNL_SW_REQUEST_REG & =~(0x01<<channel) }



/*No similar functionalities for the LPC1227 uDMA controller*/
#define     DMA_SetSourceAddressModulo(channel, modulo) { }
#define     DMA_SetDestinationAddressModulo(channel, modulo) { }




/*Mechanism similar to the priority setting in LPC122x family, to be discussed */
/*and implemented together.*/
#define     DMA_SetDisableRequestOn(channel) { }
#define     DMA_SetDisableRequestOff(channel) { }





/*LPC1227 cannot link two consecutive DMA transfers through specific registers */
#define     DMA_LinkChannelControlMode(channel, mode) {  }
#define     DMA_LinkChannel1Mode(channel, mode) { }
#define     DMA_LinkChannel2Mode(channel, mode) { }


/*Sets the DMA registers to the deinit configuration*/
/*Channel dependent DeInit the single channel functionality*/

#define     DMA_ControlDefault(channel) {_HAL_M0_DMA_CHNL_ENABLE_CLR_REG |= (0x01 << channel);}








/* ********************************************************************************************** */
/*                                  END DMA Class HAL Support                                     */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN CRC Class HAL Support                                   */
/* ********************************************************************************************** */


/*CRC Peripheral Enable/Disable defines*/

#define CRC_CLOCK_ON     (0x01<<6)
#define CRC_CLOCK_OFF    (0xFFFFFFBF)

/*CRC Standards mode Defines*/

#define CRC_CCIT          0x00000000
#define CRC_16            0x00000015
#define CRC_32            0x00000036

/*SEEDs for various standards Defines*/

#define SEED_CCIT         0x0000FFFF
#define SEED_CRC_16       0x00000000
#define SEED_CRC_32       0xFFFFFFFF





#define CRC_HW_SUPPORTED
#define CRC_ClockGateOn()    (LPC_SYSCON->SYSAHBCLKCTRL |=CRC_CLOCK_ON)
#define CRC_ClockGateOff()   (LPC_SYSCON->SYSAHBCLKCTRL &=CRC_CLOCK_OFF)



/*Hardware Activation of the CRC Module functions*/
#define InitCRCmodule(){CRC_ClockGateOn();}
#define DeInitCRCmodule(){CRC_ClockGateOff();}


/*Configuration of the CRC needed*/
#define InitCRC(seed) {LPC_CRC->MODE=CRC_CCIT;LPC_CRC->SEED=seed;}


/*CRC Computation*/
#define ComputeCRC(data) {LPC_CRC->WR_DATA_BYTE=(uint8_t)data;}
#define GetFinalCRC() (LPC_CRC->SUM)




/* ********************************************************************************************** */
/*                                  END CRC Class HAL Support                                     */
/* ********************************************************************************************** */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* General               */
/* ********************* */

/** \defgroup HAL_NX_M0_LPC1229_SYS  System Class driver support     
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


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* IO                    */
/* ********************* */
/** \defgroup HAL_NX_M0_LPC1229_IO  IO Class driver support
*/
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_DIO_SUPPORT)

/** Define DIO Capability */
#define _HAL_DIO_CAPABILITY

/*Section related to offsets in the IOCON domain*/

/*PORT 0*/
#define GPIO_IOCON00_MASK             0x44
#define GPIO_IOCON01_MASK             0x48 
#define GPIO_IOCON02_MASK             0x4C
#define GPIO_IOCON03_MASK             0x54
#define GPIO_IOCON04_MASK             0x58
#define GPIO_IOCON05_MASK             0x5C
#define GPIO_IOCON06_MASK             0x60
#define GPIO_IOCON07_MASK             0x64 
#define GPIO_IOCON08_MASK             0x68
#define GPIO_IOCON09_MASK             0x6C
#define GPIO_IOCON010_MASK            0x90
#define GPIO_IOCON011_MASK            0x94
#define GPIO_IOCON012_MASK            0x98
#define GPIO_IOCON013_MASK            0x9C
#define GPIO_IOCON014_MASK            0xA0
#define GPIO_IOCON015_MASK            0xA4
#define GPIO_IOCON016_MASK            0xA8
#define GPIO_IOCON017_MASK            0xAC
#define GPIO_IOCON018_MASK            0xB0
#define GPIO_IOCON019_MASK            0x08
#define GPIO_IOCON020_MASK            0x0C 
#define GPIO_IOCON021_MASK            0x10
#define GPIO_IOCON022_MASK            0x14 
#define GPIO_IOCON023_MASK            0x18
#define GPIO_IOCON024_MASK            0x1C
#define GPIO_IOCON025_MASK            0x20
#define GPIO_IOCON026_MASK            0x24
#define GPIO_IOCON027_MASK            0x28
#define GPIO_IOCON028_MASK            0x3C
#define GPIO_IOCON029_MASK            0x40
#define GPIO_IOCON030_MASK            0xB4
#define GPIO_IOCON031_MASK            0xB8

/*PORT 1*/
#define GPIO_IOCON10_MASK             0xBC
#define GPIO_IOCON11_MASK             0xC0
#define GPIO_IOCON12_MASK             0xC4
#define GPIO_IOCON13_MASK             0xC8
#define GPIO_IOCON14_MASK             0xCC
#define GPIO_IOCON15_MASK             0xD0
#define GPIO_IOCON16_MASK             0xD4


/*PORT 2*/
#define GPIO_IOCON20_MASK             0x70
#define GPIO_IOCON21_MASK             0x74
#define GPIO_IOCON22_MASK             0x78
#define GPIO_IOCON23_MASK             0x7C
#define GPIO_IOCON24_MASK             0x80
#define GPIO_IOCON25_MASK             0x84
#define GPIO_IOCON26_MASK             0x88
#define GPIO_IOCON27_MASK             0x8C
#define GPIO_IOCON28_MASK             0xE0
#define GPIO_IOCON29_MASK             0xE4
#define GPIO_IOCON210_MASK            0xE8
#define GPIO_IOCON211_MASK            0xEC
#define GPIO_IOCON212_MASK            0x2C
#define GPIO_IOCON213_MASK            0x30 
#define GPIO_IOCON214_MASK            0x34
#define GPIO_IOCON215_MASK            0x38




/*LPCIO Mask definitions*/
/*PORT 0*/
#define LPC0_0_MASK                  (1<<0)
#define LPC0_1_MASK                  (1<<1)
#define LPC0_2_MASK                  (1<<2)
#define LPC0_3_MASK                  (1<<3)
#define LPC0_4_MASK                  (1<<4)
#define LPC0_5_MASK                  (1<<5)
#define LPC0_6_MASK                  (1<<6)
#define LPC0_7_MASK                  (1<<7)
#define LPC0_8_MASK                  (1<<8)
#define LPC0_9_MASK                  (1<<9)
#define LPC0_10_MASK                 (1<<10)
#define LPC0_11_MASK                 (1<<11)
#define LPC0_12_MASK                 (1<<12)
#define LPC0_13_MASK                 (1<<13)
#define LPC0_14_MASK                 (1<<14)
#define LPC0_15_MASK                 (1<<15)
#define LPC0_16_MASK                 (1<<16)
#define LPC0_17_MASK                 (1<<17)
#define LPC0_18_MASK                 (1<<18)
#define LPC0_19_MASK                 (1<<19)
#define LPC0_20_MASK                 (1<<20)
#define LPC0_21_MASK                 (1<<21)
#define LPC0_22_MASK                 (1<<22)
#define LPC0_23_MASK                 (1<<23)
#define LPC0_24_MASK                 (1<<24)
#define LPC0_25_MASK                 (1<<25)
#define LPC0_26_MASK                 (1<<26)
#define LPC0_27_MASK                 (1<<27)
#define LPC0_28_MASK                 (1<<28)
#define LPC0_29_MASK                 (1<<29)
#define LPC0_30_MASK                 (1<<30)
#define LPC0_31_MASK                 (1<<31)


/*PORT 1*/
#define LPC1_0_MASK                  (1<<0)
#define LPC1_1_MASK                  (1<<1)
#define LPC1_2_MASK                  (1<<2)
#define LPC1_3_MASK                  (1<<3)
#define LPC1_4_MASK                  (1<<4)
#define LPC1_5_MASK                  (1<<5)
#define LPC1_6_MASK                  (1<<6)


/*PORT 2*/
#define LPC2_0_MASK                  (1<<0)
#define LPC2_1_MASK                  (1<<1)
#define LPC2_2_MASK                  (1<<2)
#define LPC2_3_MASK                  (1<<3)
#define LPC2_4_MASK                  (1<<4)
#define LPC2_5_MASK                  (1<<5)
#define LPC2_6_MASK                  (1<<6)
#define LPC2_7_MASK                  (1<<7)
#define LPC2_8_MASK                  (1<<8)
#define LPC2_9_MASK                  (1<<9)
#define LPC2_10_MASK                 (1<<10)
#define LPC2_11_MASK                 (1<<11)
#define LPC2_12_MASK                 (1<<12)
#define LPC2_13_MASK                 (1<<13)
#define LPC2_14_MASK                 (1<<14)
#define LPC2_15_MASK                 (1<<15)



#define PIN_DUMMY {\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,(DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y) {\
{(DIO_PortAddressBasicType)LPC_GPIO##x##_BASE,(DIO_PortAddressExtendedType)(LPC_IOCON_BASE+GPIO_IOCON##x##y##_MASK)},\
(DIO_ModuleMapMaskType)LPC##x##_##y##_MASK\
}

typedef struct {
  uint32_t  function            : 3;
  uint32_t  reserved_1          : 1;
  uint32_t  pullup              : 1;
  uint32_t  reserved_2          : 1;
  uint32_t  driveStrength       : 1;
  uint32_t  ad_mode             : 1;
  uint32_t  reserved_3          : 2;  
  uint32_t  opendrain           : 1;
  uint32_t  reserved_4          : 1;
  uint32_t  inverted            : 1;
  uint32_t  digitalFilter       : 2;
  uint32_t  digitalFilterControl: 3;
  uint32_t  do_not_use          :14;
} portreg_ext_struct_t;

typedef struct
{
  uint32_t MASK;       /*!< Offset: 0x000 Pin value mask register (R/W) */
  uint32_t dataInput;      /*Attenzione alla ridefinizione di PIN in IOPIN per uniformit� con definizione di PIN in config.c*/
  uint32_t dataOutput;  /*OUT in GPIO typedef*/
  uint32_t SET;
  uint32_t CLR;        /*!< Offset: 0x010 Pin output value clear register (R/W) */
  uint32_t NOT;         
  uint32_t dont_use;
  uint32_t dont_use2;
  uint32_t direction;    /*DIR in GPIO Tyoedef*/    /*!< Offset: 0x020 Data direction register (R/W) */
  uint32_t IS;           /* Interrupt related register*/
  uint32_t IBE;          /* Interrupt related register*/
  uint32_t IEV;          /* Interrut related register*/
  uint32_t IE;           /* Interrupt related register*/
  uint32_t RIS;
  uint32_t MIS;
  uint32_t IC;
} portreg_struct_t;



/*LPC1227 specific functionality*/
/*Four pins (PIO0_27, PIO0_28, PIO0_29, PIO0_12) are designated
high-drive pins with drive levels of 16 mA or 12 mA.*/



/** Port Interrupt control register */
#define _HAL_M0_GPIOIE_REG(port)                    (LPC_GPIO##port##->IE)

#define DIO_PULL_UP_IS_AVAILABLE	TRUE


#define DIO_IsAvailablePullUp(ptr) (1)


#define DIO_PULL_DOWN_IS_AVAILABLE	FALSE
/*For LPC1227 pull down control is not available use the pullup clear instead?*/
/*Pulldown is not available on LPC122x devices*/

#define DIO_IsAvailablePullDown(ptr) (1)



#define DIO_INPUT_IS_AVAILABLE	TRUE


#define IsAvailableAsInput(ptr)            (1)



#define DIO_SLEW_RATE_IS_AVAILABLE	FALSE

/*Slew rate control of the port is not available on LPC1227*/
#define DIO_IsAvailableSlewRate(ptr)      (0)


#define DIO_DRIVE_STRENGTH	TRUE
/*DIO Drive strength is available on selected pins only*/
/*Four pins (PIO0_27, PIO0_28, PIO0_29, PIO0_12) are designated
high-drive pins with drive levels of 16 mA or 12 mA.*/
#define DIO_IsAvailableDriveStrength(ptr)      (1)


#define DIO_IsPullUpEnabled(ptr)\
    (((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup==1)





#define DIO_IsPullDownEnabled(ptr)\
    (((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup==0)


#define DIO_IsOuput(ptr)\
    (((portreg_struct_t *)ptr->PortAddress.basic)->direction & ptr->PinMask)


#define DIO_OUTPUT_IS_AVAILABLE	TRUE
/*
\param [in] ptr points to IO_ModuleIOMapType.
*/
#define IsAvailableAsOutput(ptr)            (1)

#define DIO_PASSIVE_FILTER_IS_AVAILABLE  FALSE
/*Passive filtering functionality is not available on LPC1227 devices*/

#define DIO_INTERRUPT_IS_AVAILABLE    TRUE

#define DIO_IsInterruptAvailable(ptr)       (1)



#define DIO_DIGITAL_FILTER_IS_AVAILABLE   TRUE
/*Digital filtering is available on every pin (up to 55 in 64 pins packages devices*/
#define DIO_IsDigitalFilterAvailable(ptr)       (1)

/*May 2011 Driver modification*/
/*GPIO Clock gating*/
#define DIO_Init()     {LPC_SYSCON->SYSAHBCLKCTRL |=((0x01<<29)|(0x01<<30)|(0x01<<31));}


/*Modification May 2011*/

/*Attenzione deve essere interpretata, per il momento �*/
/*identica all' interrupt enable*/


#define NVIC_PIO_2   43
#define NVIC_PIO_1   42
#define NVIC_PIO_0   41



#define DIO_IsInterruptFlag(pMap)\
  (((pMap->PortAddress.basic == LPC_GPIO0_BASE) && (NVIC_GetPendingIRQ((IRQn_Type)NVIC_PIO_0)))\
    ||((pMap->PortAddress.basic == LPC_GPIO1_BASE) && (NVIC_GetPendingIRQ((IRQn_Type)NVIC_PIO_1)))\
  ||((pMap->PortAddress.basic == LPC_GPIO2_BASE) && (NVIC_GetPendingIRQ((IRQn_Type)NVIC_PIO_2))))



#define DIO_PullUpEnable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup =1;

#define DIO_PullUpDisable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup =0;






#define DIO_SetDirAsOutput(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->direction |= ptr->PinMask;  



/*Modification May 2011*/

#define DIO_SetDirAsInput(ptr)\
    ((portreg_struct_t *)ptr->PortAddress.basic)->direction &= ~(ptr->PinMask);  

#define DIO_SetInterruptMode_FallingEdge(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IEV &= ~(ptr->PinMask) )

#define DIO_SetInterruptMode_RisingEdge(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IEV |= ptr->PinMask )



#define S_MODE3       3
#define S_MODE2       2
#define S_MODE1       1
#define NO_FILTERING  0

#define DIO_IsDigitalFilterEnabled(ptr)\
  (((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter != NO_FILTERING)?1:0




#define DIO_IsInterruptMode_RisingEdge(ptr)\
  (( ((portreg_struct_t *)ptr->PortAddress.basic)->IEV & ptr->PinMask ) == 1)



#define DIO_DigitalFilterEnable(ptr)\
 ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter=S_MODE3



#define DIO_DigitalFilterDisable(ptr)\
 ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter=NO_FILTERING


/* filterFrequency is in KHz */
#define DIO_DigitalFilterClkFrequencySet(ptr, filterFrequency)\
{\
  if ((BUS_FREQUENCY_KHz/(filterFrequency*(((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter))<=255)) \
  {\
      LPC_SYSCON->FILTERCLKCFG0=BUS_FREQUENCY_KHz/(filterFrequency*(((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter));\
      ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilterControl=0; /*Select divisor 0*/\
  }\
  else\
  {\
      LPC_SYSCON->FILTERCLKCFG0=255;\
      ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilterControl=0;  /*Select divisor 0*/\
  }  \
}

void DIO_SetDigitalFltClkFreq(portreg_ext_struct_t *port,uint16_t filterFrequency);



#define DIO_DigitalFilterClkFrequencyGet(ptr)\
  (BUS_FREQUENCY_Hz/(1000*LPC_SYSCON->FILTERCLKCFG0*(((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter)))\



#define DIO_DigitalFilterNumSamplesSet(ptr,nSamples)\
  if(nSamples<=S_MODE3)\
    ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter=nSamples;\
  else\
    ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter=S_MODE3;\


#define DIO_DigitalFilterNumSamplesGet(ptr)\
    ((portreg_ext_struct_t *)ptr->PortAddress.extended)->digitalFilter


#define DIO_InterruptEnable(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IE |= ptr->PinMask )
  


#define DIO_InterruptDisable(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IE &= ~(ptr->PinMask) )


#define DIO_IsInterruptEnabled(ptr)\
  (( ((portreg_struct_t *)ptr->PortAddress.basic)->IE & (ptr->PinMask) ) != 0)


#define DIO_ClearInterrupt(ptr)\
( ((portreg_struct_t *)ptr->PortAddress.basic)->IC |= (ptr->PinMask) )





#define DIO_DMA_IS_AVAILABLE  TRUE

#define DIO_IsDmaAvailable(ptr) (1)


#define DIO_DmaEnable(ptr)\
{\
    if(ptr->PortAddress.basic == LPC_GPIO0_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG |= (0x01U<<DMA_CHANNEL_16);\
    else if(ptr->PortAddress.basic == LPC_GPIO1_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG |= (0x01U<<DMA_CHANNEL_17);\
    else if(ptr->PortAddress.basic == LPC_GPIO2_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG |= (0x01U<<DMA_CHANNEL_18);\
}


#define DIO_DmaDisable(ptr)\
{\
    if(ptr->PortAddress.basic == LPC_GPIO0_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG &= ~(0x01U<<DMA_CHANNEL_16);\
    else if(ptr->PortAddress.basic == LPC_GPIO1_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG &= ~(0x01U<<DMA_CHANNEL_17);\
    else if(ptr->PortAddress.basic == LPC_GPIO2_BASE)     _HAL_M0_DMA_CHNL_ENABLE_SET_REG &= ~(0x01U<<DMA_CHANNEL_18);\
}


#define DIO_IsDmaEnabled(ptr)\
  ((\
     ((pMap->PortAddress.basic == LPC_GPIO0_BASE)&&(_HAL_M0_DMA_CHNL_ENABLE_SET_REG & (0x01<<DMA_CHANNEL_16)))\
  || ((pMap->PortAddress.basic == LPC_GPIO1_BASE)&&(_HAL_M0_DMA_CHNL_ENABLE_SET_REG & (0x01<<DMA_CHANNEL_17)))\
  || ((pMap->PortAddress.basic == LPC_GPIO2_BASE)&&(_HAL_M0_DMA_CHNL_ENABLE_SET_REG & (0x01<<DMA_CHANNEL_18)))\
  )?1:0)



#define DIO_LevelDetectInterruptEnable(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IS |= ptr->PinMask )




#define DIO_EdgeDetectInterruptEnable(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IS &= ~(ptr->PinMask) )


#define DIO_GetDetectInterruptMode(ptr)\
  ( ((portreg_struct_t *)ptr->PortAddress.basic)->IS & (ptr->PinMask)!=0?1:2 )



#define DIO_DriveStrengthEnable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength =1;

#define DIO_DriveStrengthDisable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength =0;

#define DIO_IsDriveStrengthEnabled(ptr)   (((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength == 1)



#define DIO_GetVal(ptr)\
(( ((portreg_struct_t *)ptr->PortAddress.basic)->dataInput & ptr->PinMask ) !=0 )


#define DIO_SetLow(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->CLR |= ptr->PinMask;


#define DIO_SetHigh(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->SET |= ptr->PinMask;


#define DeInitGPIO()  {LPC_SYSCON->SYSAHBCLKCTRL &=~((0x01<<29)|(0x01<<30)|(0x01<<31));}

#endif /* defined((HAL_DIO_SUPPORT) */

/* ********************* */
/* IO                    */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


/* ********************** */
/* UART Class HAL Support */
/* ********************** */
/* ********************** */
/*           END          */
/* ********************** */
/* Define UART1 Capability */
/* Define UART1 Capability */
#define _HAL_UARTx_CAPABILITY

#ifdef  HAL_OPTION_UART
  #if     HAL_OPTION_UART ==  1
    #define HAL_UART_DEV   0    //1
  #elif   HAL_OPTION_UART ==  2
    #define HAL_UART_DEV   1 //2
  #else
    #error "uart not available"
  #endif
#endif



/* Baud rates calculations */

//#define _HALUART_CALC_BAUD(baud, divisor)   (uint16_t)((uint16_t)(((uint32_t)BUS_FREQUENCY_Hz / ((uint32_t)16.0 * (baud)))))

/*MODIFICA MASSIMO INCERTI*/
#define _HALUART_CALC_BAUD(baud, divisor)   (uint16_t)((uint16_t)(((uint32_t)BUS_FREQUENCY_Hz / (5.5*(baud)))))

//#define _HALUART_CALC_BAUD(baud, divisor)   (uint16_t)(((uint32_t)BUS_FREQUENCY_Hz / (16*1.5*baud)))


/** Baud Rates */
#define _HALUART_BAUD_115200               (_HALUART_CALC_BAUD(115200UL,	))
#define _HALUART_BAUD_76800                (_HALUART_CALC_BAUD(76800UL,		))
#define _HALUART_BAUD_57600                (_HALUART_CALC_BAUD(57600UL,		))
#define _HALUART_BAUD_38400                (_HALUART_CALC_BAUD(38400UL,		))
#define _HALUART_BAUD_28800                (_HALUART_CALC_BAUD(28800U,		))
#define _HALUART_BAUD_19200                (_HALUART_CALC_BAUD(19200U,		))
#define _HALUART_BAUD_14400                (_HALUART_CALC_BAUD(14400,		))
#define _HALUART_BAUD_9600                 (_HALUART_CALC_BAUD(9600,		))
#define _HALUART_BAUD_4800                 (_HALUART_CALC_BAUD(4800,		))
#define _HALUART_BAUD_2400                 (_HALUART_CALC_BAUD(2400,		))
#define _HALUART_BAUD_1200                 (_HALUART_CALC_BAUD(1200,		))

/**/


#define _HAL_M0_UART0_BAUDRATE_REG               		  (LPC_UART0->DLL)  // Lower part of UART Baudrate register 
#define _HAL_M0_UART0_BAUDRATE_REG_H               		  (LPC_UART0->DLM)  // Higher part of UART Baudrate register
#define _HAL_M0_UART0_TX_DATAREG                  	          ((LPC_UART0->THR))
#define _HAL_M0_UART0_RX_DATAREG                 	          (/*&*/(LPC_UART0->RBR))
#define _HAL_M0_UART0_ENABLE_TX_REG             	    	  (LPC_IOCON->PIO0_2) /* UART0 in LPC1227 equivale a UART1 nel SW Indesit  configurazione come da modulo francobollo */
#define _HAL_M0_UART0_ENABLE_RX_REG             	    	  (LPC_IOCON->PIO0_1) /* UART0 in LPC1227 equivale a UART1 nel SW Indesit  configurazione come da modulo francobollo */
#define _HAL_M0_UART0_ENABLE_TX_IRQ_REG            		  (LPC_UART0->IER)
#define _HAL_M0_UART0_ENABLE_RX_IRQ_REG            		  (LPC_UART0->IER)


#define _HAL_M0_UART1_BAUDRATE_REG               		  (LPC_UART1->DLL)
#define _HAL_M0_UART1_BAUDRATE_REG_H               		  (LPC_UART1->DLM)  // Higher part of UART Baudrate register
#define _HAL_M0_UART1_TX_DATAREG                  	          (/*&*/(LPC_UART1->THR))
#define _HAL_M0_UART1_RX_DATAREG                	          (/*&*/(LPC_UART1->RBR))
#define _HAL_M0_UART1_ENABLE_TX_REG             	    	  (LPC_IOCON->PIO0_9) /* UART1 in LPC1227 equivale a UART2 nel SW Indesit  configurazione come da modulo francobollo */
#define _HAL_M0_UART1_ENABLE_RX_REG             	    	  (LPC_IOCON->PIO0_8) /* UART1 in LPC1227 equivale a UART2 nel SW Indesit  configurazione come da modulo francobollo */
#define _HAL_M0_UART1_ENABLE_TX_IRQ_REG            		  (LPC_UART1->IER)
#define _HAL_M0_UART1_ENABLE_RX_IRQ_REG            		  (LPC_UART1->IER)

/* Definizione registri tramite una macro che identifica il numero della UART */

#define _HAL_M0_UART_BAUDRATE_REG(n)           		  ((n==0)?(LPC_UART0->DLL):(LPC_UART1->DLL))  // Only the lower part of UART Baudrate register for the moment
#define _HAL_M0_UART_TX_DATAREG(n)                  	  ((n==0)?(LPC_UART0->THR):(LPC_UART1->THR))  /*      (LPC_UART0->THR)*/
#define _HAL_M0_UART_RX_DATAREG(n)              	  ((n==0)?(LPC_UART0->RBR):(LPC_UART1->RBR))  /*      (LPC_UART0->RBR) */
//#define _HAL_M0_UART_ENABLE_TX_REG(n)            	  ((n==0)?(LPC_IOCON->PIO2_2):(LPC_IOCON->PIO2_13))  /*	  LPC_IOCON domain */
//#define _HAL_M0_UART_ENABLE_RX_REG(n)            	  ((n==0)?(LPC_IOCON->PIO2_1):(LPC_IOCON->PIO2_12))  /*	  LPC_IOCON domain */
#define _HAL_M0_UART_ENABLE_TX_IRQ_REG(n)            	  ((n==0)?(LPC_UART0->IER):(LPC_UART1->IER))  /*	  (LPC_UART0->IER) */
#define _HAL_M0_UART_ENABLE_RX_IRQ_REG(n)            	  ((n==0)?(LPC_UART0->IER):(LPC_UART1->IER))  /*	  (LPC_UART0->IER) */
#define _HAL_Mo_UART_CLR_REG(n)                           ((n==0)?(LPC_UART0->LCR):(LPC_UART1->LCR))










#define _HAL_M0_UARTx_CLK_ENABLE_REG(n)	(LPC_SYSCON->SYSAHBCLKCTRL)



/* UART 0 Registers */

#define _HAL_M0_UART0_STATUS_RX_REG            	    	  (LPC_UART0->LCR)
#define _HAL_M0_UART0_STATUS_TX_REG            	    	  (LPC_UART0->LCR)
#define _HAL_M0_UART0_LINE_STATUS_REG                     (LPC_UART0->LSR)

#define _HAL_M0_UART0_STATUS_ERR_PE_REG   /* Parity Error*/	  (LPC_UART0->LSR)
#define _HAL_M0_UART0_STATUS_ERR_FER_REG  /* Framing Error */  (LPC_UART0->LSR)
#define _HAL_M0_UART0_STATUS_ERR_OER_REG  /* Overrun Error*/   (LPC_UART0->LSR)

#define _HAL_M0_UART0_STATUS_ERR_REG            	    	  (LPC_UART0->LSR)
#define _HAL_M0_UART0_FRAME_CFG_REG              	    	  (LPC_UART0->LCR)


#define _HAL_M0_UART0_PARITY_ENABLE_REG            		   (LPC_UART0->LCR)
#define _HAL_M0_UART0_PARITY_VAL_REG            		   (LPC_UART0->LCR)
#define _HAL_M0_UART0_STOP_REG            	      		   (LPC_UART0->LCR)  /* To be verified*/
#define _HAL_M0_UART0_MODE_REG            	      		   (LPC_UART0->FCR)  /* To e verified DMA mode assumption*/
#define _HAL_M0_UART0_DATALEN_REG                	      	   (LPC_UART0->LCR)
#define _HAL_M0_UART0_CLOCK_DIVISOR_REG_LSB              	   (LPC_UART0->DLL)

// Added,  typical of LPC1227 microcontroller

#define _HAL_M0_UART0_CLOCK_DIVISOR_REG_MSB             	   (LPC_UART0->DLM)
#define _HAL_M0_UART0_INT_ID_REG                                   (LPC_UART0->IIR)    /*Interrupt ID register UART0*/
#define _HAL_M0_UART0_FIFO_REG                                     (LPC_UART0->FIFOLVL)
#define _HAL_M0_UART0_CLR_REG                                      (LPC_UART0->LCR)
#define _HAL_M0_UART0_FDR_REG                                      (LPC_UART0->FDR)


/* UART 1 Registers */


#define _HAL_M0_UART1_STATUS_RX_REG              	    	  (LPC_UART1->LCR)
#define _HAL_M0_UART1_STATUS_TX_REG             	    	  (LPC_UART1->LCR)
#define _HAL_M0_UART1_LINE_STATUS_REG                             (LPC_UART1->LSR)



#define _HAL_M0_UART1_STATUS_ERR_PE_REG    /* Parity Error*/	  (LPC_UART1->LSR)
#define _HAL_M0_UART1_STATUS_ERR_FER_REG   /* Framing Error */    (LPC_UART1->LSR)
#define _HAL_M0_UART1_STATUS_ERR_OER_REG  /* Overrun Error*/      (LPC_UART1->LSR)

#define _HAL_M0_UART1_STATUS_ERR_REG            	    	  (LPC_UART1->LSR)
#define _HAL_M0_UART1_FRAME_CFG_REG              	    	  (LPC_UART1->LCR)


#define _HAL_M0_UART1_PARITY_ENABLE_REG            		  (LPC_UART1->LCR)
#define _HAL_M0_UART1_PARITY_VAL_REG            		  (LPC_UART1->LCR)
#define _HAL_M0_UART1_STOP_REG            	      		  (LPC_UART1->LCR)  /* To be verified*/
#define _HAL_M0_UART1_MODE_REG            	      		  (LPC_UART1->FCR)  /* To e verified DMA mode assumption*/
#define _HAL_M0_UART1_DATALEN_REG               	      	  (LPC_UART1->LCR)
#define _HAL_M0_UART1_CLOCK_DIVISOR_REG_LSB             	  (LPC_UART1->DLL)

// Added,  typical of LPC1227 microcontroller

#define _HAL_M0_UART1_CLOCK_DIVISOR_REG_MSB             	   (LPC_UART1->DLM)
#define _HAL_M0_UART1_INT_ID_REG                                   (LPC_UART1->IIR) /*Interrupt ID register UART1*/
#define _HAL_M0_UART1_FIFO_REG                                     (LPC_UART1->FIFOLVL)
#define _HAL_M0_UART1_CLR_REG                                      (LPC_UART1->LCR)
#define _HAL_M0_UART1_FDR_REG                                      (LPC_UART1->FDR)
/* Macros to address any generic UART in the microcontroller */


#define _HAL_M0_UART_STATUS_RX_REG(n)            	    	  (n==0 ?(LPC_UART0->LCR):(LPC_UART1->LCR))
#define _HAL_M0_UART_STATUS_TX_REG(n)            	    	  (n==0 ?(LPC_UART0->LCR):(LPC_UART1->LCR))

#define _HAL_M0_UART_STATUS_ERR_PE_REG(n)   /* Parity Error*/	  (n==0 ?(LPC_UART0->LSR):(LPC_UART1->LSR))
#define _HAL_M0_UART_STATUS_ERR_FER_REG(n)  /* Framing Error */  (n==0 ?(LPC_UART0->LSR):(LPC_UART1->LSR))
#define _HAL_M0_UART_STATUS_ERR_OER_REG(n)  /* Overrun Error*/   (n==0 ?(LPC_UART0->LSR):(LPC_UART1->LSR))

#define _HAL_M0_UART_STATUS_ERR_REG(n)            	    	  (n==0 ?(LPC_UART0->LSR):(LPC_UART1->LSR))
#define _HAL_M0_UART_FRAME_CFG_REG(n)            	    	  (n==0 ?(LPC_UART0->LCR):(LPC_UART1->LCR))


#define _HAL_M0_UART_PARITY_ENABLE_REG(n)            		   ((n==0) ? (LPC_UART0->LCR) : (LPC_UART1->LCR) )
#define _HAL_M0_UART_PARITY_VAL_REG(n)            		   ((n==0) ? (LPC_UART0->LCR):(LPC_UART1->LCR))
#define _HAL_M0_UART_STOP_REG(n)            	      		   ((n==0) ?(LPC_UART0->LCR):(LPC_UART1->LCR))  /* To be verified*/
#define _HAL_M0_UART_MODE_REG(n)            	      		   ((n==0) ?(LPC_UART0->FCR):(LPC_UART1->FCR))  /* To e verified DMA mode assumption*/
#define _HAL_M0_UART_DATALEN_REG(n)            	      	           ((n==0) ?(LPC_UART0->LCR):(LPC_UART1->LCR))
#define _HAL_M0_UART_CLOCK_DIVISOR_REG_LSB(n)            	   ((n==0) ?(LPC_UART0->DLL):(LPC_UART1->DLL))
#define _HAL_M0_UART_INT_ID_REG(n)                                 ((n==0) ?(LPC_UART0->IIR):(LPC_UART1->IIR))

// Added,  typical of LPC1227 microcontroller

#define _HAL_M0_UART_CLOCK_DIVISOR_REG_MSB(n)            	   (n==0 ?(LPC_UART0->DLM):(LPC_UART1->DLM))
#define _HAL_MO_UART_INT_ID_REG(n)                                 (n==0 ?(LPC_UART0->IIR):(LPC_UART1->IIR))
#define _HAL_MO_UART_FIFO_REG(n)                                   (n==0 ?(LPC_UART0->FIFOLVL):(LPC_UART1->FIFOLVL)) 

/* PInout Mask pins*/

/*  UART0   RXD0 PIO0_1  */
/*          TXD0 PIO0_2  */

/*  UART 1  RXD1 PIO0_8  */
/*          TXD1 PIO0_9  */


#define UART0_TE_MASK         0x02   /*Da verificare*/
#define UART0_RE_MASK         0x02   /*Da verificare*/
#define UART1_TE_MASK         0x02   /*da verificare*/
#define UART1_RE_MASK         0x02   /*Da verificare*/


/*  UART0   RXD0 PIO2_1  */
/*          TXD0 PIO2_2  */

/*  UART 1  RXD1 PIO2_12  */
/*          TXD1 PIO2_13  */


#define UART0_TE_ALT_MASK         0x08   /*Da verificare*/
#define UART0_RE_ALT_MASK         0x08   /*Da verificare*/
#define UART1_TE_ALT_MASK         0x03   /*da verificare*/
#define UART1_RE_ALT_MASK         0x03   /*Da verificare*/


/*Parity Mask values  */
/* */

#define     UART_PAR_ENABLE_MASK   (0x01<<3)
#define     UART_PAR_CHECK_MASK    (0x01<<3)

/*Lenght Mask values */

#define     UART_LEN_8_BIT_MASK     0x03 
#define     UART_LEN_9_BIT_MASK     0x01  /*Attenzione, bisogna intervenire su RS485CTRL register DA FARE!!*/

/*Interrupt Enable Mask values */

#define     UART_TXIE_MASK          0x02
#define     UART_RXIE_MASK          0x01   /*da 0x05*/


/*State Handling MAsk values*/

#define     UART_PARITY_ERROR_MASK   0x04
#define     UART_FRAMING_ERROR_MASK  0x08
#define     UART_OVERRUN_ERROR_MASK  0x02
#define     UART_GENERIC_ERROR_MASK  0x7E
#define     UART_LS_TX_EMPTY         (0x01<<5)

/*Interrupt ID Mask values*/
#define     UART_GENERIC_INT_MASK    0xFE   /*AND comparison*/
#define     UART_TX_INT_MASK         0xF2   /*AND comparison*/
#define     UART_RX_INT_MASK         0x04   /*AND comparison*/    


/*Enable Latches for Baud rate masks*/
#define     UART_DLAB_ENABLE_MASK    (0x01U<<7) /*0x80*/





/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


/*  Peripheral management and configuration
 *  
 */

#define     UART_ClkEnable(n)       (n==0)?(LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12)):(LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13));
#define     UART_ClkDisable(n)      (n==0)?(LPC_SYSCON->SYSAHBCLKCTRL &= (uint32_t)(~(1<<12))):(LPC_SYSCON->SYSAHBCLKCTRL &= (uint32_t)(~(1<<13)));

#define     UART_Enable(n)          (n==0)?(LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12)):(LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13));

#define     UART_Disable(n)		{}            /* not used */





#define     UART_TxEnable(n)      /* Enable Configurazione IOs per uso come UART TX in the IOCON domain*/        {if(n==0)_HAL_M0_UART0_ENABLE_TX_REG |= UART0_TE_MASK; else _HAL_M0_UART1_ENABLE_TX_REG |= UART1_TE_MASK;}



#define     UART_RxEnable(n)      /* Enable COnfigurazione IO per uso come UART RX in the IOCON domain*/         if(n==0) _HAL_M0_UART0_ENABLE_RX_REG |= UART0_RE_MASK; else _HAL_M0_UART1_ENABLE_RX_REG |= UART1_RE_MASK;



#define     UART_TxDisable(n)  	 if(n==0) _HAL_M0_UART0_ENABLE_TX_REG &= (uint32_t)(~UART0_TE_MASK); else _HAL_M0_UART1_ENABLE_TX_REG &= (uint32_t)(~UART1_TE_MASK); /* Back to the GPIO configuration*//* Disable configurazione IO per uso come GPIO TX, in the IOCON domain*/



#define     UART_RxDisable(n)  	 if(n==0) _HAL_M0_UART0_ENABLE_RX_REG &= (uint32_t)(~UART0_RE_MASK); else _HAL_M0_UART1_ENABLE_RX_REG &= (uint32_t)(~UART1_RE_MASK); /* Back to the GPIO configuration*//* Disable configurazione IO per uso come GPIO rX, in the IOCON domain*/






#define     UART_SetParityVal(n,val)    ( (val)? (_HAL_M0_UART_PARITY_VAL_REG(n) |= UART_PAR_ENABLE_MASK) : (HAL_M0_UART_PARITY_VAL_REG(n) &= (uint32_t)(~UART_PAR_ENABLE_MASK)))

        


#define  UART0_SetParityMode(val)       ( (val) ? (_HAL_M0_UART0_PARITY_ENABLE_REG |= UART_PAR_CHECK_MASK) : (_HAL_M0_UART0_PARITY_ENABLE_REG &= (uint32_t)(~UART_PAR_CHECK_MASK)))
#define  UART1_SetParityMode(val)       ( (val) ? (_HAL_M0_UART1_PARITY_ENABLE_REG |= UART_PAR_CHECK_MASK) : (_HAL_M0_UART1_PARITY_ENABLE_REG &= (uint32_t)(~UART_PAR_CHECK_MASK)))


#define  UART_SetParityMode(n,val)      if(n==0)  UART0_SetParityMode(val); else UART1_SetParityMode(val);





#define  _HAL_M0_UART0_RS485CTRLREG		(LPC_UART0->RS485CTRL)
#define  _HAL_M0_UART1_RS485CTRLREG		(LPC_UART1->RS485CTRL)


#define  UART0_SetDataLen(val)          ( (val==0)? (_HAL_M0_UART0_DATALEN_REG |= UART_LEN_8_BIT_MASK) :   (_HAL_M0_UART0_RS485CTRLREG |=0x2) )
#define  UART1_SetDataLen(val)          ( (val==0)? (_HAL_M0_UART1_DATALEN_REG |= UART_LEN_8_BIT_MASK) :   (_HAL_M0_UART1_RS485CTRLREG |=0x2) )

#define  UART_SetDataLen(n,val)         if(n==0) UART0_SetDataLen(val); else UART1_SetDataLen(val);  



#define     UART_SetStop(n,val)					    /* not used */
#define     UART_SetMode(n,val)					    /* not used */



#define     UART0_SetClockDivisor(val)          (_HAL_M0_UART0_FDR_REG=val)                                                            
#define     UART1_SetClockDivisor(val)          (_HAL_M0_UART1_FDR_REG=val)                                                            

#define     UART_SetClockDivisor(n,val)         ((n==0)?(UART0_SetClockDivisor(val)):(UART1_SetClockDivisor(val)))



/*@}*/



/*  Data register
 *  
 */

#define     UART_ReadData(n)                    _HAL_M0_UART_RX_DATAREG(n);

#define     UART_WriteData(n,b) 	          	((n==0)?(_HAL_M0_UART0_TX_DATAREG = (UART_DataRegType)(b)):(_HAL_M0_UART1_TX_DATAREG = (UART_DataRegType)(b)))
                  
   
#define     UART_pReceiveDataRegister(n) 	  	(&_HAL_M0_UART_RX_DATAREG(n))

//#define     UART_pTransmitDataRegister(n)   	        (&_HAL_M0_UART_TX_DATAREG(n))
#define     UART_pTransmitDataRegister(n)       ((n==0)?((uint8_t*)(&_HAL_M0_UART0_TX_DATAREG)):((uint8_t*)(&_HAL_M0_UART1_TX_DATAREG)))


			
/* Configuration
 *  
 */

#pragma inline
void UART_SetBaudRate(uint8_t n, uint16_t b) 
{
  ((n==0)?(_HAL_M0_UART0_CLR_REG |=UART_DLAB_ENABLE_MASK):(_HAL_M0_UART1_CLR_REG |=UART_DLAB_ENABLE_MASK));
  ((n==0)?(_HAL_M0_UART0_BAUDRATE_REG=/*(UART_BaudRateType)*/(b)&0xFF):(_HAL_M0_UART1_BAUDRATE_REG=(UART_BaudRateType)(b)&0xFF));
  ((n==0)?(_HAL_M0_UART0_BAUDRATE_REG_H=(UART_BaudRateType)(b)&0xFF00):(_HAL_M0_UART1_BAUDRATE_REG_H=(UART_BaudRateType)(b)&0xFF00));
  ((n==0)?(_HAL_M0_UART0_CLR_REG &=(~UART_DLAB_ENABLE_MASK)):(_HAL_M0_UART1_CLR_REG &=(~UART_DLAB_ENABLE_MASK)));
}



//#define     UART_SetFrameConfig(n,b) 	          {   _HAL_S08_UARTx_FRAME_CFG_REG(n) = (UART_FrameCfgType)(b); }



/* Interrupt
 *  
 */

#define     UART_TxIntEnable(n)                if(n==0) {_HAL_M0_UART0_ENABLE_TX_IRQ_REG |= UART_TXIE_MASK; NVIC_EnableIRQ(UART0_IRQn);} else {_HAL_M0_UART1_ENABLE_TX_IRQ_REG |= UART_TXIE_MASK; NVIC_EnableIRQ(UART1_IRQn);}/*NVIC Control Interrupt activation to do */

#define     UART_RxIntEnable(n)                if(n==0) {_HAL_M0_UART0_ENABLE_TX_IRQ_REG |= UART_RXIE_MASK; NVIC_EnableIRQ(UART0_IRQn);} else {_HAL_M0_UART1_ENABLE_TX_IRQ_REG |= UART_RXIE_MASK; NVIC_EnableIRQ(UART1_IRQn);}/*NVIC control interrupt activation to do */ 




#define     UART_TxIntDisable(n)              if(n==0) _HAL_M0_UART0_ENABLE_TX_IRQ_REG &= (uint32_t)(~UART_TXIE_MASK); else _HAL_M0_UART1_ENABLE_TX_IRQ_REG &= (uint32_t)(~UART_TXIE_MASK);/*NVIC Control Interrupt deactivation to do */ 

#define     UART_RxIntDisable(n)              if(n==0) _HAL_M0_UART0_ENABLE_TX_IRQ_REG &= (uint32_t)(~UART_RXIE_MASK); else _HAL_M0_UART1_ENABLE_TX_IRQ_REG &= (uint32_t)(~UART_RXIE_MASK);/*NVIC Control Interrupt deactivation to do */




/*  State
 *  
 */
#define     UART_IsPeError(n)        	(_HAL_M0_UART_STATUS_ERR_PE_REG(n) & UART_PARITY_ERROR_MASK)	/* \todo deve ritornare un valore fisso se non abilitato */
#define     UART_IsFerError(n)       	(_HAL_M0_UART_STATUS_ERR_FER_REG(n) & UART_FRAMING_ERROR_MASK )
#define     UART_IsOerError(n)       	(_HAL_M0_UART_STATUS_ERR_OER_REG(n) & UART_OVERRUN_ERROR_MASK)
#define     UART_IsError(n)		        (_HAL_M0_UART_STATUS_ERR_REG(n) & UART_GENERIC_ERROR_MASK)


#define     UART_ClearError(n)       	{_HAL_M0_UART_STATUS_ERR_OER_REG(n); _HAL_M0_UART_RX_DATAREG(n);} /*For most of the errors a read of the register is sufficient, see case by case*/
#define     UART_ResetPeError(n)	{_HAL_M0_UART_STATUS_ERR_PE_REG(n); _HAL_M0_UART_RX_DATAREG(n);}  /*Error Reset reading the register*/
#define     UART_ClearRxIrq(n)       	{} /* To clear the interrupts a reading of the INt register is sufficient */
#define     UART_ClearTxIrq(n)       	{} /* To clear the interrupts a reading of the INT register is sufficient*/


#define     UART_IsTxEndInt(n)		(_HAL_M0_UART_INT_ID_REG(n))  /*To be verified in the driver, dont understand the meaning*/

/* Transmitter Holding Register Empty interrupt */
#define     UART_IsTxInt(n)           	((n==0)?(((_HAL_M0_UART0_INT_ID_REG)==UART_TX_INT_MASK)):((_HAL_M0_UART1_INT_ID_REG)==UART_TX_INT_MASK))
/* Receive Data Available */
#define     UART_IsRxInt(n)           	((n==0)?(((_HAL_M0_UART0_INT_ID_REG)&UART_RX_INT_MASK)):((_HAL_M0_UART1_INT_ID_REG)&UART_RX_INT_MASK))


#define     UART_IsTxDataRegFull(n)   	((n==0)?(!(_HAL_M0_UART0_LINE_STATUS_REG & UART_LS_TX_EMPTY)):(!((_HAL_M0_UART1_LINE_STATUS_REG & UART_LS_TX_EMPTY))))




/* DMA Section*/


#define     UART_TxDMAEnable(n)             { }
#define     UART_RxDMAEnable(n)             { }

#define     UART_TxDMADisable(n)            { }
#define     UART_RxDMADisable(n)            { }


/* ********************** */
/* UART Class HAL Support */
/* ********************** */
/* ********************** */
/*           END          */
/* ********************** */

/* ********************************************************************************************** */
/*                                  BEGIN System Class HAL Support                                */
/* ********************************************************************************************** */

/** @name System Class
 *  
 */
/*@{*/

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define System Capability */
#define _HAL_SYS_CAPABILITY

extern unsigned long      CSTACK;
#define RAM_BASE          (0x10000000)
#define RAM_SIZE          (0x2000)
#define RAM_END           (RAM_BASE+RAM_SIZE)
#define INIT_STACK_BASE   (unsigned int )(CSTACK)
#define INIT_STACK_SIZE   (0x170)

//#define EnterCriticalSection()   ( __set_PRIMASK(0))
//#define ExitCriticalSection() ( __set_PRIMASK(1))

#define EnterCritical EnterCriticalSection
#define ExitCritical ExitCriticalSection

typedef  uint32_t resetSource_t;

/* Map mcu specific registers names to a 'standard' set of names 
              
              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>
              
    (this helps when generating new files for additional parts of the same family).
*/

/*#define LPO_FREQUENCY_Hz    1000*/
//
//extern uint32_t gCopyOfStatusRegister;
/** 
Disable Global Enable Interrupt 
*/

/*inline __declspec(register_abi) asm uint16_t __EnterCritical(void)
{
        move.w  SR,D0
        move.l  D0,D1
        ori.l #0x0700,D1
        move.w D1,SR
        andi.l #0x0700,D0
}*/

//#define EnterCritical() {gCopyOfStatusRegister = __EnterCritical();}

/**
Enable Global Enable Interrupt if enabled before EnterCritical()
*/
/*
inline __declspec(register_abi) asm void __ExitCritical(uint16_t storedStackPointer)
{
        move.w  SR,D1
        andi.l #0xF8FF,D1
        or.l D1,D0
        move.w D0,SR
}

#define ExitCritical()  {__ExitCritical(gCopyOfStatusRegister);}
  */  

/** Reset status register */


#define _HAL_M0_SRS_REG                 (LPC_SYSCON->SYSRESSTAT)
#define _RST_VALUE                       _HAL_M0_SRS_REG

/*Low voltage detect source mask*/
#define _HAL_M0_SRS_LVD_MASK            (0x01<<3)
/*Watchdog Reset source mask*/
#define _HAL_M0_SRS_WDOG_MASK           (0x01<<2)
/*External Reset source mask*/
#define _HAL_M0_SRS_EXTRST_MASK         (0x01<<1)


/*System Reset detected mask*/
#define _HAL_M0_SRS_PIN_MASK            (0x01<<4)

/*Power on Reset detected mask*/
#define _HAL_M0_SRS_POR_MASK            (0x01<<0)








#define _HAL_M0_BOD_REG                 LPC_SYSCON->BODCTRL;
#define _HAL_M0_BOD_RST_EN_MASK         (0x01<<4)   


inline resetSource_t SYS_GetResetSrcVal(void)
{
  resetSource_t val= LPC_SYSCON->SYSRESSTAT;
  LPC_SYSCON->SYSRESSTAT |= 1;
  return val;
}


#define SYS_LowVoltageReset(reg)        ((reg == _HAL_M0_SRS_LVD_MASK)) 
#define SYS_WatchdogReset(reg)          ((reg == _HAL_M0_SRS_WDOG_MASK)) 



#define SYS_ExternalPinReset(reg)       (((reg) == _HAL_M0_SRS_EXTRST_MASK)) 

#define SYS_DebuggerReset(reg)          (reg == 0)

/* This define MUST be set according to UserReset() */
#define SYS_UserReset(reg)              SYS_WatchdogReset(reg)

/*To clear reset sources the sw must write 0x01, POR event clears all the other reset sources*/
#define SYS_ClearResetSource(reg)       {reg = 0x01;}

/* Not supported*/
#define SYS_SetDebuggerReset(reg)       {reg = 0;}

/*Set low voltage reset event*/
#define SYS_SetLowVoltageReset(reg)     {reg |= _HAL_M0_SRS_LVD_MASK;} 

/*Set Watchdog Reset event*/
#define SYS_SetWatchdogReset(reg)       {reg |= _HAL_M0_SRS_WDOG_MASK;} 

#define SYS_SetIllegalAddressReset(reg) {  }
#define SYS_SetIllegalOpcodeReset(reg)  { } 
#define SYS_SetLossOfClockReset(reg)    { }


#define SYS_LossOfClockReset(dummy)   FALSE
#define SYS_IllegalOpcodeReset(dummy) FALSE
#define SYS_IllegalAddressReset(dummy) FALSE
 

#define SYSRES_ADDR       0x10001FDC

#define MCU_RequiresResetAfterPOR()  FALSE


typedef struct
{
uint32_t reset_vector;
}sysreset_type;

#define HARDFAULT_MASK     (0x1<<5)


#define SYS_COPYRESETVECTOR             ((sysreset_type*)SYSRES_ADDR)
#define SYS_HardFaultReset(reg)         {SYS_COPYRESETVECTOR->reset_vector |= HARDFAULT_MASK;}


        


#define SYS_SetExternalPinReset(reg)    {reg |= _HAL_M0_SRS_PIN_MASK;} 

#define SYS_SetPowerOnReset(reg)        {reg |= _HAL_M0_SRS_POR_MASK;}

/* This define MUST be set according to UserReset() */

#define SYS_SetUserReset(reg)           SYS_SetWatchdogReset(reg) 






inline bool_t SYS_PowerOnReset(resetSource_t rstSrc)
{
    if ( rstSrc == (_HAL_M0_SRS_POR_MASK|_HAL_M0_SRS_LVD_MASK) )
    {
        return 1;    
    }
    else
    {
        return 0;    
    }
}

/*LVD part, aka Brown Out detect Hardware abstraction layer*/


/* LVD * 10 */
#define SYS_VLVDL0  21.8
#define SYS_VLVDL1  24.7
#define SYS_VLVDL2  27.6

#define SYS_VLWL0   23.8
#define SYS_VLWL1   26.7
#define SYS_VLWL2   29.2


#define SYS_BODRSTLEV_MASK_0    0x01  /*The reset assertion threshold voltage is 2.038 V 
                                      the reset de-assertion threshold voltage is 2.180 V*/
#define SYS_BODRSTLEV_MASK_1    0x02  /*The reset assertion threshold voltage is 2.336 V;
                                      the reset de-assertion threshold voltage is 2.471 V.*/
#define SYS_BODRSTLEV_MASK_2    0x03  /*The reset assertion threshold voltage is 2.624 V;
                                      the reset de-assertion threshold voltage is 2.761 V.*/


#define SYS_BODRST_ENABLE       0x00



#define SYS_BODINTLEV_MASK_0    0x4

#define SYS_BODINTLEV_MASK_1    0x8

#define SYS_BODINTLEV_MASK_2    0xC


/* LVW */


/*
Low-Voltage Warning Voltage Select
*/

#define LVWLowTripPoint()   {/*SPMSC2_LVWV = 0;*/}

#define LVWHighTripPoint()  {/*SPMSC2_LVWV = 1;*/}


/* 
LVD warning interrupt Enable/Disable 
*/

#define LVDWarningIntOn()   {} 
#define LVDWarningIntOff()  {} 


#define LVDGetWarningFlag()     ()
#define LVDWarningAcknoledge()  {} 

/* 
LVD in stop mode Enable/Disable
*/

/*Not supported in LPC1227 microcontroller*/

#define LVDSetStopOn()  {}
#define LVDSetStopOff() {}


/*
LVD reset disable 
*/


#define LVDR_Disable()  { _HAL_M0_BOD_REG &= ~_HAL_M0_BOD_RST_EN_MASK;}
#define LVDR_Enable()   { _HAL_M0_BOD_REG |= _HAL_M0_BOD_RST_EN_MASK;}


/* 
Set voltage trip to generate LVD reset 
*/
#pragma inline

void LVDR_SetUp(uint8_t tripPointVR)
{

    int8_t dist_0,dist_1,dist_2, distance;
    
    dist_0 = abs((int16_t)tripPointVR - (int16_t)SYS_VLVDL0);
    
    dist_1 = abs((int16_t)tripPointVR - (int16_t)SYS_VLVDL1);
    
    dist_2 = abs((int16_t)tripPointVR - (int16_t)SYS_VLVDL2);
    
    
    if (dist_0<dist_1)
      distance=dist_0;
    else 
      distance=dist_1;
    
    if (distance>dist_2)  
      distance=dist_2;
    
   
    if(distance==dist_0)
        LPC_SYSCON->BODCTRL |= SYS_BODRSTLEV_MASK_0;
    if(distance==dist_1)
        LPC_SYSCON->BODCTRL |= SYS_BODRSTLEV_MASK_1;
    if(distance==dist_2)
        LPC_SYSCON->BODCTRL |= SYS_BODRSTLEV_MASK_2;
       
    
 
}

/* Set voltage trip to generate LVD warning */
#pragma inline
void  LVDW_SetTrip(uint8_t tripPointVW) 

{
 
  int8_t dist_0,dist_1,dist_2, distance;

    
    dist_0 = abs((int16_t)tripPointVW - (int16_t)SYS_VLWL0);
    
    dist_1 = abs((int16_t)tripPointVW - (int16_t)SYS_VLWL1);
    
    dist_2 = abs((int16_t)tripPointVW - (int16_t)SYS_VLWL2);
    
    
    if (dist_0<dist_1)
      distance=dist_0;
    else 
      distance=dist_1;
    
    if (distance>dist_2)  
      distance=dist_2;
    
   
    if(distance==dist_0)
        LPC_SYSCON->BODCTRL |= SYS_BODINTLEV_MASK_0;
    if(distance==dist_1)
        LPC_SYSCON->BODCTRL |= SYS_BODINTLEV_MASK_1;
    if(distance==dist_2)
        LPC_SYSCON->BODCTRL |= SYS_BODINTLEV_MASK_2;
       
    
  
  
  
  
  }



inline uint8_t LVDR_GetTrip(void)
{
    if ((LPC_SYSCON->BODCTRL & SYS_BODRSTLEV_MASK_0) != 0)
    {
        return (uint8_t)SYS_BODRSTLEV_MASK_0;
    }
    if ((LPC_SYSCON->BODCTRL & SYS_BODRSTLEV_MASK_1) != 0)
    {
        return (uint8_t)SYS_BODRSTLEV_MASK_1;
    }
    if  ((LPC_SYSCON->BODCTRL & SYS_BODRSTLEV_MASK_2) != 0)
    {
        return (uint8_t)SYS_BODRSTLEV_MASK_2;
    }
    return (uint8_t)0;
}

/* Reads the LVD warning trip */
#pragma inline 
uint8_t LVDW_GetTrip(void)  
{
  
  if (((LPC_SYSCON->BODCTRL) & SYS_BODINTLEV_MASK_0) != 0)
    {
        return (uint8_t)SYS_BODINTLEV_MASK_0;
    }
    if (((LPC_SYSCON->BODCTRL>>4) & SYS_BODINTLEV_MASK_1) == 0)
    {
        return (uint8_t)SYS_BODINTLEV_MASK_1;
    }
    if  ((LPC_SYSCON->BODCTRL & SYS_BODINTLEV_MASK_1) == 0)
    {
        return (uint8_t)SYS_BODINTLEV_MASK_1;
    }
  
    return (uint8_t)0;
 }








/*@}*/

/* ********************************************************************************************** */
/*                                  END System Class HAL Support                                  */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */

#if defined(HAL_ADC_SUPPORT)
/* ****************** */
/* Defines and Macros */
/* ****************** */




#define ADC_PIN(x,y,z)\
{\
  (DIO_PortAddressBasicType)LPC_GPIO##x##_BASE,\
  (DIO_PortAddressExtendedType)(LPC_IOCON_BASE+GPIO_IOCON##x##y##_MASK),\
  (DIO_ModuleMapMaskType)LPC##x##_##y##_MASK,\
  z\
}







/* Max clock in Hz */
#define ADC_MAX_CLOCK               9000000

/* Max clock in Hz when low power mode */
#define ADC_MAX_CLOCK_LOW_POWER     4000000

/* Max clock in Hz when low power mode */
#define ADC_MIN_CLOCK               400000

/* This defines if the Vrefh-Vrefl reference is used. Otherwise the Vdda-Vssa is used */
#define ADC_REFERENCE_VREFH_VREFL   0

/* Defines if the ADC runs in low power mode; otherwise it runs in fast mode */
#define ADC_LOW_POWER_MODE          0

/* Defines the long sample time; otherwise it is in the short sample time mode */
#define ADC_LONG_SAMPLE_TIME        1


/* 255 is the max. avaible value for flash module clock divisor */

#if ((BUS_FREQUENCY_Hz / ADC_CONV_FREQUENCY_Hz) > 8)

	#define ADC_DIV ((BUS_FREQUENCY_Hz / ADC_CONV_FREQUENCY_Hz) - 1)
        #if (ADC_DIV > 255)
		#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, divisor too high!"
	#endif
#endif


	#define ADC_DIV ((BUS_FREQUENCY_Hz / ADC_CONV_FREQUENCY_Hz) - 1)





typedef struct 
{
  uint8_t SEL:8;
  uint8_t CLK_DIV:8;
  uint8_t BURST:1;
  uint8_t unused:4;
  uint8_t PDN:1;
  uint8_t do_not_touch:2;
  uint8_t START:3;
  uint8_t EDGE:1;
  uint8_t unused_2:4;
}AD_ctrl_reg_t;


typedef struct 
{
  uint8_t  unused_1:6;
  uint16_t RESULT:10;
  uint8_t  unused_2:8;
  uint8_t  CHANNEL:3;
  uint8_t  unused_3:3;
  uint8_t  OVERRUN:1;
  uint8_t  DONE:1;
}AD_global_Data_reg_t;


#define AD_Ctrl ((AD_ctrl_reg_t*)LPC_ADC_BASE)
#define AD_Global_Data_reg  ((AD_global_Data_reg_t*)(LPC_ADC_BASE+4))

//static uint16_t copy_of_AD_result/*=AD_Global_Data_reg->RESULT*/;


#if (ADC_RESOLUTION > 10)
    #define ADC_RESOLUTION_MODE         12
#elif (ADC_RESOLUTION > 8)
    #define ADC_RESOLUTION_MODE         10
#else
    #define ADC_RESOLUTION_MODE         8
#endif    

#define ADC_RESOLUTION_RIGHT_SHIFT (3 + ADC_RESOLUTION_MODE - ADC_RESOLUTION)






#define ADC_ENABLE_CLOCK_MASK                    14
#define ADC_POWERUP_MASK                         4
#define ADC_START_MASK                           24


#define _HAL_M0_GLOBAL_INT_MASK                  8


#define _HAL_M0_ADC_ADINTEN_REG                         (LPC_ADC->INTEN)


#define ADC_SetBusClock()                               {}


#define ADC_GlobalIntEnable()                       {NVIC_EnableIRQ(ADC_IRQn);}
#define ADC_GlobalIntDisable()                      {NVIC_DisableIRQ(ADC_IRQn);}



#define ADC_ClockGateOn()                           (LPC_SYSCON->SYSAHBCLKCTRL |=(0x01U<<ADC_ENABLE_CLOCK_MASK))
#define ADC_ClockGateOff()                          (LPC_SYSCON->SYSAHBCLKCTRL &= ~(0x01U<<ADC_ENABLE_CLOCK_MASK))
#define ADC_ClearCtrl_Register()                    {LPC_ADC->CR=0x00;}

#define ADC_IsConversionCompleteA()		            ((LPC_ADC->GDR & (0x01U<<31))/*AD_Global_Data_reg->DONE==1*/)
#define ADC_IsConversionCompleteB()		            (AD_Global_Data_reg->DONE==1)

#define ADC_EnableInterruptA(ch)  		            {_HAL_M0_ADC_ADINTEN_REG |=(0x01U<<ch);NVIC_EnableIRQ(ADC_IRQn);}

/*The global interrupt pin must be always enabled to ensure even the polling operation of the ADC to be checked*/
#define ADC_DisableInterruptA()  		            {NVIC_DisableIRQ(ADC_IRQn);_HAL_M0_ADC_ADINTEN_REG |= (_HAL_M0_GLOBAL_INT_MASK);} 


#define ADC_EnableInterruptB(ch)  		            {_HAL_M0_ADC_ADINTEN_REG |=(0x01U<<ch);NVIC_EnableIRQ(ADC_IRQn);}
#define ADC_DisableInterruptB()  		            {NVIC_DisableIRQ(ADC_IRQn);_HAL_M0_ADC_ADINTEN_REG |= (_HAL_M0_GLOBAL_INT_MASK);} 

#define ADC_ContinuesConversionA()  		        {AD_Ctrl->BURST=1;}
#define ADC_SingleConversionA()  		            {AD_Ctrl->BURST=0;}

#define ADC_ContinuesConversionB()  		        {AD_Ctrl->BURST=1;}
#define ADC_SingleConversionB()  		            {AD_Ctrl->BURST=0;}

#define ADC_SelectChannelA(channel)  		        {LPC_ADC->CR &=(0xFFFFFF00);LPC_ADC->CR |= (0x01<<channel);}
#define ADC_SelectChannelB(channel)  		        {AD_Ctrl->SEL |=(0x01U<<channel);}

#define ADC_ReadChannelA()  		                (AD_Ctrl->SEL)
#define ADC_ReadChannelB()  		                (AD_Ctrl->SEL)
                                                     


/*Disable Channel achieved using the power down bit in the AD control register ??*/
/*to be verified in the driver's test*/

#define ADC_DisableChannelA()  		                {AD_Ctrl->PDN=0;}
#define ADC_DisableChannelB()         		        {AD_Ctrl->PDN=0;}

#define ADC_IsConversionActive()  		        ()

/*Hardware Triggers Masks*/

#define HAL_ADC_EDGE_TIMER16_0_CAP0_OCCURS    2
#define HAL_ADC_EDGE_TIMER32_0_CAP0_OCCURS    3
#define HAL_ADC_EDGE_TIMER32_0_MAT0_OCCURS    4
#define HAL_ADC_EDGE_TIMER32_0_MAT1_OCCURS    5
#define HAL_ADC_EDGE_TIMER16_0_MAT0_OCCURS    6
#define HAL_ADC_EDGE_TIMER16_0_MAT1_OCCURS    7

/*By default, not used*/
/** put on Edge of timer16 o capture0 event occurs*/

#define ADC_SetHWTrigger()        		        {AD_Ctrl=HAL_ADC_EDGE_TIMER16_0_CAP0_OCCURS;}
/*To be sure that no hardware trigger is activated put START @0*/
/*To software start the conversion this field must be set @ 1*/

#define ADC_SetSWTrigger()        		        {AD_Ctrl->PDN=1;AD_Ctrl->BURST=0;AD_Ctrl->START=0;}

#define ADC_SetBackToBackConversion()           {} /*Not implemented in NXP LPC1227 microcontroller*/
#define ADC_SetIndependentConversion()          {} /*Not implemented in NXP LPC1227 microcontroller*/

#define ADC_EnableDMA()                         {_HAL_M0_DMA_CHNL_ENABLE_SET_REG |=(0x01<<DMA_CHANNEL_6);}
#define ADC_DisableDMA()                        {_HAL_M0_DMA_CHNL_ENABLE_CLR_REG |=(0x01<<DMA_CHANNEL_6);}

#define ADC_SetVRefToVrefhVrefl()               {}   /*Not implemented in NXP LPC1227 microcontroller*/
#define ADC_SetVRefToVddaVssa()                 {}   /*Not implemented in NXP LPC1227 microcontroller*/
#define ADC_SetVRefToOnChipBandgap()            {}   /*Not implemented in NXP LPC1227 microcontroller*/

#define ADC_SetLowPowerConversion()             {}   /*Not implemented in NXP LPC1227 microcontroller*/
#define ADC_SetHighSpeedConversion()            {}   /*Not implemented in NXP LPC1227 microcontroller*/

#define ADC_SelectClockDiv(div)                 {AD_Ctrl->CLK_DIV=div;}

#define ADC_SetLongSampleTime()                 {}
#define ADC_SetShortSampleTime()                {}

#define ADC_SetConversion8bit()                 {}   /*Not supported in LPC122y7 microcontroller*/
#define ADC_SetConversion10bit()                {}   /*Default conversion in LPC1227 nothing to do*/
#define ADC_SetConversion12bit()                {}   /*Not supported in LPC1227 microcontroller*/


#define ADC_ReadResultA()		                ((LPC_ADC->GDR>>6)&0x03FF)
#define ADC_ReadResultB()		                ((AD_Global_Data_reg->RESULT)&0x03FF)



#define ADC_PowerUp()                           {LPC_SYSCON->PDRUNCFG &= ~(0x01U<<ADC_POWERUP_MASK);}








/* added channel parameter for compatibility with others uc S.U. 31/05/2010 */
#define ADC_pResultRegister(channel)            ((AD_Global_Data_reg)/*->RESULT LPC_ADC->GDR*/)   /*Global data register for every conversion*/
#define ADC_pChannelRegister(channel)           ((uint8_t *)AD_Ctrl->SEL)

#define ADC_INT_ENABLED  1
#define ADC_INT_DISABLED 0

#define AD_FUNC_A    0x3
#define AD_FUNC_B    0x2
#define AD_FUNC_C    0x1

#define DEFAULT_IO  0x0



/**
Disable the I/O port control of MCU pins that are used as analog inputs.
*/

#define ADC_DeConfigurePinAsChannel(channel)  {((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->function = DEFAULT_IO;\
((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->ad_mode = 1;}


/**
Enable the I/O port control of MCU pins that are not used as analog inputs.
*/

#define ADC_ConfigurePinAsChannel(channel){\
((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->pullup=0;\
((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->ad_mode=0;\
switch(channel){\
case 0:\
case 1:((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->function = AD_FUNC_A;break;\
case 2:\
case 3:\
case 4:((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->function = AD_FUNC_B;break;\
case 5:\
case 6:\
case 7:((portreg_ext_struct_t *)(ADCMap[channel].extAddress))->function = AD_FUNC_C;break;\
  default:break;\
};}



/**
'0' if channel is valid; otherwise '1'
*/
#define ADC_CheckChannel(channel)               (channel >7)




#define ADC_StartNow()   {LPC_ADC->CR &= ~(0x07U << ADC_START_MASK);LPC_ADC->CR |=(0x01U<<ADC_START_MASK);}



/**
\n Writes to ADC registers:
\n\li enables/disables interrupt from conversion on A
\n\li sets independent conversion
\n\li sets single converison
\n\li interrupts any running conversion 
\n\li disables hw trigger
\n\li disables dma
\n\li initiates a new conversion on the defined channel.

*/



inline void ADC_Init(void)
{
    
  uint8_t clkdiv;
  
  
  /*ADC Power up, powers up the ADC in the PDRUNCFG register*/
    /*Default disabled*/
  
    ADC_PowerUp();
  
     /* Bus clock to the ADC module is enabled  */
    
      
  
    ADC_ClockGateOn();
    
    /* Clear control register, put it in a known state*/
    ADC_ClearCtrl_Register();
    
    

    ADC_SetVRefToVddaVssa();


    /* Sets the ADC dividor */
    
    clkdiv=(uint8_t)(BUS_FREQUENCY_Hz/ADC_CONV_FREQUENCY_Hz);
  
  
     LPC_ADC->CR = (((clkdiv-1) << 8) & ((uint32_t) 0x0000FF00) ) | (1<<21);
    


    /* Sets the independent conversion of A & B results */
    ADC_SetIndependentConversion();
}


//#define ExitCriticalSection()             {__set_PRIMASK(0);}
//#define EnterCriticalSection()           {__set_PRIMASK(1);}




#pragma inline 
void ADC_StartConvertionOnChannel(uint8_t channel,uint8_t atdInterrupt)
{
    if(atdInterrupt==ADC_INT_ENABLED)
    {
    	ADC_EnableInterruptA(channel);
    }
    if(atdInterrupt==ADC_INT_DISABLED)
    {
    	ADC_DisableInterruptA();
    }
    
    /*Burst control, configuration for SW controlled*/
    
    ADC_SetSWTrigger(); 
    
    /*AD conversion without DMA support*/
    ADC_DisableDMA();
    
    ADC_SelectChannelA(channel);
    
    /*ADC Start now*/    
    ADC_StartNow();   
}





#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
    /**
    \n Writes to ADC registers:
    \n\li enables interrupt from conversion on A
    \n\li sets independent conversion
    \n\li sets continues converison
    \n\li interrupts any running conversion 
    \n\li disables hw trigger
    \n\li disables dma
    \n\li initiates a new conversion on the defined channel.

    */
    #define ADC_StartMultiSampleConvertionOnChannel(channel)    { ADC_SetIndependentConversion(); ADC_ContinuesConversionA(); ADC_SetSWTrigger(); ADC_SelectChannelA(channel);ADC_EnableInterruptA(channel);ADC_StartNow()} 

    /**
    \n Writes to ADC registers:
    \n\li enables interrupt from conversion on A
    \n\li sets independent conversion
    \n\li sets single converison
    \n\li interrupts any running conversion 
    \n\li disables hw trigger
    \n\li disables dma

    */

#define ADC_EnableGlobalInterrupt()           { ADC_EnableInterruptA(8);   }


    #define ADC_StartMultiSampleConvertionMultiChannel()        {NVIC_EnableIRQ(ADC_IRQn); ADC_SetIndependentConversion(); ADC_ContinuesConversionA(); ADC_SetSWTrigger();ADC_EnableGlobalInterrupt();} 
    
    /**
    \n Converts the channel number to register value:
    \n\li Ores the channel number with the IE mask in order the interrupt to be still enabled
    */


/*Massimo Incerti, da completare per il supporto del Multichannel+Multisamples*/
/*10 Giugno 2011*/




#define ADC_CorrectChannel(channel)     (0x01<<channel)


/*Implementazione come da Freescale AG128*/
// #define ADC_CorrectChannel(channel)     (channel | _HAL_MCF51_ADC_ADCSC1_AIEN_MASK)



#endif


typedef struct ADC_Map_tag {
  DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register relaated to the port pin */
  DIO_PortAddressExtendedType extAddress;
  DIO_ModuleMapMaskType        PinMask;     /**< Port pin mask  */
  uint8_t channel; /**< ADC Channel number  */
} ADC_MapType;

#define BEGIN_MODULE_ADC_MAP(n)    static const ADC_MapType ADCMap[n]={
  
#define END_MODULE_ADC_MAP()       };


#define ADC_MAPPED_CHANNELS_NUMBER 8

/* Table of mapped channels */



BEGIN_MODULE_ADC_MAP(ADC_MAPPED_CHANNELS_NUMBER)
ADC_PIN(1,0,2),
ADC_PIN(0,31,1),
ADC_PIN(0,30,0),
ADC_PIN(1,1,3),
ADC_PIN(1,2,4),
ADC_PIN(1,3,5),
ADC_PIN(1,4,6),
ADC_PIN(1,5,7)
END_MODULE_ADC_MAP()



/*
Starting from portAddress and pinMask (read pin) search for AD channel  
*/

#pragma inline /*inline*/
bool_t ADC_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pADch)
{
uint8_t j;
bool_t channelfound = FALSE;


for (j=0;j<ADC_MAPPED_CHANNELS_NUMBER;j++)
{
  if(ADCMap[j].PortAddress==portAddress.basic)
  
  {
    
    if((ADCMap[j].PinMask))
   {
   
     if((ADCMap[j].PinMask)==(pinMask))
     {   channelfound=TRUE;
     
     break;
     }
   }
}  
}

    if (channelfound)
    {
        *pADch = ADCMap[j].channel; 
    }

/*Decode the pin number according to the pinmask structure defined*/

    return channelfound;
}
/**
Channel selected for ATD convertion
*/
#define ADC_GetActiveChannel()	                                (ADC_ReadChannelA())

/**
\n Wait for Conversion Complete.
*/
#define ADC_WaitForConvertionOnChannel(channel)	                while(!(LPC_ADC->GDR & (0x01U<<31)))
    
/**
\n\li ADC Conversion Result depends on the desired resolution and the module can be programmed for 8, 10 or 12 bits accordingly
\n\li This function get the converted value and modify it to the desired resolution
 */			

#define ADC_GetConvertedValueOnChannel()		                (ADC_ReadResultA())
#define ADC_GetCorrectResolutionResult(result)                  ((result>>6)&0x3FF)





/**
\n Test Conversion Complete Flag.
\n The CCF is a read-only bit which is set each time a conversion is complete. 
*/
#define ADC_EndConvertion()             (ADC_IsConversionCompleteA()) 

/**
\n Stops any possible ADC conversion by applying the 0x1f channel.
*/
#define ADC_StopConversion()            ADC_DisableChannelA()


    
/**
Set all ADC Registers to the reset state value
*/
#define ADC_DeInit()\
{\
    ADC_ClockGateOff();\
}



#endif /* #if defined(HAL_ADC_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */





/* ********************************************************************************************** */
/*                                  BEGIN DMA Class HAL Support                                   */
/* ********************************************************************************************** */




/*******   DMA   ************************************/

#define _HAL_M0_DMA_CHANNEL_COUNT                (21)
#define _HAL_M0_DMA_STATUS_REG                   (LPC_DMA->STATUS)
#define _HAL_M0_DMA_CFG_REG                      (LPC_DMA->CFG)
#define _HAL_M0_DMA_CTRL_BASE_PTR_REG            (LPC_DMA->CTRL_BASE_PTR)
#define _HAL_M0_DMA_ATL_CTRL_BASE_PTR_REG        (LPC_DMA->ATL_CTRL_BASE_PTR)
#define _HAL_M0_DMA_WAITONREQ_STATUS_REG         (LPC_DMA->WAITONREQ_STATUS)
#define _HAL_M0_DMA_CHNL_SW_REQUEST_REG          (LPC_DMA->CHNL_SW_REQUEST)
#define _HAL_M0_DMA_CHNL_USEBURST_SET_REG        (LPC_DMA->CHNL_USEBURST_SET)
#define _HAL_M0_DMA_CHNL_USEBURST_CLR_REG        (LPC_DMA->CHNL_USEBURST_CLR)
#define _HAL_M0_DMA_CHNL_REQ_MASK_SET_REG        (LPC_DMA->CHNL_REQ_MASK_SET)
#define _HAL_M0_DMA_CHNL_REQ_MASK_CLR_REG        (LPC_DMA->CHNL_REQ_MASK_CLR)
#define _HAL_M0_DMA_CHNL_ENABLE_SET_REG          (LPC_DMA->CHNL_ENABLE_SET)
#define _HAL_M0_DMA_CHNL_ENABLE_CLR_REG          (LPC_DMA->CHNL_ENABLE_CLR)
#define _HAL_M0_DMA_CHNL_PRI_ALT_SET_REG         (LPC_DMA->CHNL_PRI_ALT_SET)
#define _HAL_M0_DMA_CHNL_PRI_ALT_CLR_REG         (LPC_DMA->CHNL_PRI_ALT_CLR)
#define _HAL_M0_DMA_CHNL_PRIORITY_SET_REG        (LPC_DMA->CHNL_PRIORITY_SET)
#define _HAL_M0_DMA_CHNL_PRIORITY_CLR_REG        (LPC_DMA->CHNL_PRIORITY_CLR)
#define _HAL_MO_DMA_ERR_CLR_REG                  (LPC_DMA->ERR_CLR)
#define _HAL_MO_DMA_CHNL_IRQ_STATUS_REG          (LPC_DMA->CHNL_IRQ_STATUS)
#define _HAL_MO_DMA_IRQ_ERR_ENABLE_REG           (LPC_DMA->IRQ_ERR_ENABLE)
#define _HAL_MO_DMA_CHNL_IRQ_ENABLE_REG          (LPC_DMA->CHNL_IRQ_ENABLE)





/*

typedef struct {
  uint8_t    cycle_ctrl:3;
  uint8_t    next_use_burst:1;
  uint32_t   nminus_1:9;
  uint8_t    r_power:4;
  uint8_t    src_prot_ctrl:3;
  uint8_t    dst_prot_ctrl:3;
  uint8_t    src_size:2;
  uint8_t    src_inc:2;
  uint8_t    dst_size:2;
  uint8_t    dst_inc:2
 }chn_ctrl_data_Typedef;

typedef struct {

  uint32_t source_end_ptr;
  uint32_t dest_end_ptr;
  chn_ctrl_data_Typedef  chn_ctr_data;
}dma_chn_ctrl_data_Typedef;

*/

#define DMA_CHANNEL_0			0  /*UART0 TX*/
#define DMA_CHANNEL_1			1  /*UART0 RX*/
#define DMA_CHANNEL_2			2  /*UART1 TX*/
#define DMA_CHANNEL_3			3  /*UART1 RX*/
#define DMA_CHANNEL_4			4  /*SSP TX*/
#define DMA_CHANNEL_5			5  /*SSP RX*/
#define DMA_CHANNEL_6			6  /*ADC*/
#define DMA_CHANNEL_7			7  /*RTC*/
#define DMA_CHANNEL_8			8  /*TMR32B0 MATCH0*/
#define DMA_CHANNEL_9			9  /*TMR32B0 MATCH1*/
#define DMA_CHANNEL_10			10 /*TMR32B1 MATCH0*/
#define DMA_CHANNEL_11			11 /*TMR32B1 MATCH1*/
#define DMA_CHANNEL_12			12 /*TMR16B0 MATCH0*/
#define DMA_CHANNEL_13			13 /*TMR16B1 MATCH0*/
#define DMA_CHANNEL_14			14 /*Comparator 0*/
#define DMA_CHANNEL_15			15 /*Comparator 1*/
#define DMA_CHANNEL_16			16 /*PIO0*/
#define DMA_CHANNEL_17			17 /*PIO1*/
#define DMA_CHANNEL_18			18 /*PIO2*/



/*Memory buffer for DMA location */ 

#define DMA_CTRL_BUFFER_LOC          0x10001000   /*To be moved according to the projects settings*/
#define SIZEOF_DMA_CTRL              sizeof(dma_chn_ctrl_data_Typedef)

#define LPC_DMA_CHN0                 ((dma_chn_ctrl_data_Typedef*)  DMA_CTRL_BUFFER_LOC)
#define LPC_DMA_CHN1                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN2                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +2*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN3                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +3*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN4                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +4*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN5                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +5*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN6                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +6*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN7                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +7*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN8                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +8*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN9                 ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +9*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN10                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +10*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN11                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +11*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN12                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +12*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN13                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +13*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN14                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +14*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN15                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +15*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN16                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +16*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN17                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +17*SIZEOF_DMA_CTRL))
#define LPC_DMA_CHN18                ((dma_chn_ctrl_data_Typedef*) (DMA_CTRL_BUFFER_LOC +18*SIZEOF_DMA_CTRL))


/*Memory buffer for DMA location */ 




#define _HAL_M0_DMA_STATUS_SHIFT_REG     ((_HAL_M0_DMA_STATUS_REG>>4)&0xF)
#define DMA_BLOCK_STALLED           0x08
#define DMA_BLOCK_DONE              0x09
#define DMA_BLOCK_GATHER_SCATTER    0x0A
#define DMA_BLOCK_IDLE              0x00







/**
Device assignment
*/

/*The assignement of the devices to the channels is fixed and mapped*/
/*We could use this function to activate and enable the single DMA channel*/

/*Or is it something that at peripheral level must be activated?*/

  
#define     DMA_SetDevice(channel, device)          {_HAL_M0_DMA_CHNL_ENABLE_SET_REG |= (0x01 << channel) ; }
#define     DMA_DisableDevice(channel, device)      {_HAL_M0_DMA_CHNL_ENABLE_CLR_REG |= (0x01 << channel) ; }




#define     DMA_GetDevice(channel) (_HAL_M0_DMA_CHNL_ENABLE_SET_REG) 



/**
DMA Source address assignment
*/

#define     DMA_SetSAR(channel, address) {LPC_DMA_CHN##channel->source_end_ptr=address;} 

#define     DMA_GetSAR(channel) (LPC_DMA_CHN##channel->source_end_ptr)

 
/**
DMA Destination address assignment
*/
#define     DMA_SetDAR(channel, address) { LPC_DMA_CHN##channel->dest_end_ptr=address; } 
#define     DMA_GetDAR(channel) (LPC_DMA_CHN##channel->dest_end_ptr)

/**
DMA Byte count register
*/
/*Number of DMA cycles that must be performed from 1 to 1024*/

#define     DMA_SetBCR(channel, count) {LPC_DMA_CHN##channel->chn_ctr_data.nminus_1=count;}
#define     DMA_GetBCR(channel) (LPC_DMA_CHN##channel->chn_ctr_data.nminus_1)


/**
DMA status register check
*/

#define     DMA_IsTransDone(channel) (LPC_DMA->CHNL_IRQ_STATUS&(0x01<<channel))

#define     DMA_IsChannelBusy(channel) (_HAL_M0_DMA_STATUS_SHIFT_REG !=DMA_BLOCK_IDLE)

#define     DMA_IsPending(channel) ((_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_STALLED)&(_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_DONE)&(_HAL_M0_DMA_STATUS_SHIFT_REG!=DMA_BLOCK_GATHER_SCATTER))


/*To be verified*/ 

#define     DMA_IsDestinationError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)

#define     DMA_IsSourceError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)

#define     DMA_IsConfigError(channel) (_HAL_MO_DMA_ERR_CLR_REG&0x01)




/**
DMA status register clear flags
*/


#define     DMA_ClearTransDone(channel) { *((uint8_t*)&(*(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)))) = (uint8_t)DSR0_DONE_MASK;}

/**
DMA control register setting
*/

#define     DMA_EnableInt(channel)  {_HAL_MO_DMA_CHNL_IRQ_ENABLE_REG |=(0x01<<channel); }
#define     DMA_DisableInt(channel) { _HAL_MO_DMA_CHNL_IRQ_ENABLE_REG &= ~(0x01<<channel); }


/*Functionalities that are covered by other functions in the same HAL implementation*/
#define     DMA_PeripheralRequestOn(channel) {  }
#define     DMA_PeripheralRequestOff(channel) { }


/*Functionalities covered automatically by the processor or in other registers.*/

#define     DMA_ContinuousTransfer(channel) {  }
#define     DMA_SingleTransfer(channel) {  }


/*Auto Align functionality to be discussed and checked in the user manual*/

#define     DMA_EnableAutoAlign(channel) {  }
#define     DMA_DisableAutoAlign(channel) { }



/*Source data structure setup macros*/

#define     DMA_EnableSourceIncrement(channel)  {LPC_DMA_CHN##channel->chn_ctr_data.src_inc=LPC_DMA_CHN##channel->chn_ctr_data.src_size;}
#define     DMA_DisableSourceIncrement(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_inc=DMA_DATA_NO_INC ; }

#define     DMA_SetSourceSizeToLong(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_LONG;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_LONG; }
#define     DMA_SetSourceSizeToByte(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_BYTE;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_BYTE; }
#define     DMA_SetSourceSizeToWord(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_WORD;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_WORD; }


/*Destination data structure setup macros*/

#define     DMA_EnableDestinationIncrement(channel) { LPC_DMA_CHN##channel->chn_ctr_data.dest_inc=LPC_DMA_CHN##channel->chn_ctr_data.dest_size;}
#define     DMA_DisableDestinationIncrement(channel) { LPC_DMA_CHN##channel->chn_ctr_data.src_inc=DMA_DATA_NO_INC ; }


/*There is the constrain that source and destination data size must be the same*/
/*Thus this macro will set both data sizes at the same time*/

#define     DMA_DATA_SIZE_BYTE   0
#define     DMA_DATA_SIZE_LONG   1   /*16 bit data?*/
#define     DMA_DATA_SIZE_WORD   2

#define     DMA_DATA_NO_INC      3



#define     DMA_SetDestinationSizeToLong(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_LONG;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_LONG;}
#define     DMA_SetDestinationSizeToByte(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_BYTE;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_BYTE;}
#define     DMA_SetDestinationSizeToWord(channel) {LPC_DMA_CHN##channel->chn_ctr_data.src_size=DMA_DATA_SIZE_WORD;LPC_DMA_CHN##channel->chn_ctr_data.dst_size=DMA_DATA_SIZE_WORD;}


/*Software request DMa transfers*/

#define     DMA_SetStartOn(channel) { _HAL_M0_DMA_CHNL_SW_REQUEST_REG |=(0x01<<channel); }
#define     DMA_SetStartOff(channel) {_HAL_M0_DMA_CHNL_SW_REQUEST_REG & =~(0x01<<channel) }



/*No similar functionalities for the LPC1227 uDMA controller*/
#define     DMA_SetSourceAddressModulo(channel, modulo) { }
#define     DMA_SetDestinationAddressModulo(channel, modulo) { }




/*Mechanism similar to the priority setting in LPC122x family, to be discussed */
/*and implemented together.*/
#define     DMA_SetDisableRequestOn(channel) { }
#define     DMA_SetDisableRequestOff(channel) { }





/*LPC1227 cannot link two consecutive DMA transfers through specific registers */
#define     DMA_LinkChannelControlMode(channel, mode) {  }
#define     DMA_LinkChannel1Mode(channel, mode) { }
#define     DMA_LinkChannel2Mode(channel, mode) { }


/*Sets the DMA registers to the deinit configuration*/
/*Channel dependent DeInit the single channel functionality*/

#define     DMA_ControlDefault(channel) {_HAL_M0_DMA_CHNL_ENABLE_CLR_REG |= (0x01 << channel);}








/* ********************************************************************************************** */
/*                                  END DMA Class HAL Support                                     */
/* ********************************************************************************************** */

























/* ********************************************************************************************** */
/*                                  BEGIN IRQ Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_IRQ_SUPPORT)
/*
- Bus clock to the IRQ module is enabled.
- Disable IRQ Interrupt
- IRQ pin function is enabled
- Pulling
- Edge or Level Plus Edge
- Init. Sync
- Acknowledges interrupt request events
- IRQ Interrupt Enable

*/
/*

According to Indesit indications IRQ positioned on PIO1_6 to retain the
compatibility with the Castellated module between AG128 and LPC1227
*/


#define _HAL_M0_IRQ_CLK_EN_REG    (LPC_SYSCON->SYSAHBCLKCTRL)
#define _HAL_M0_IRQ_GPIO_CTRL     (LPC_IOCON->PIO1_6) 
#define _HAL_M0_IRQ_DIR_REG       (LPC_GPIO1->DIR)  /*Direction register*/
#define _HAL_M0_IRQ_INT_REG       (LPC_GPIO1->IE)   /*Interrupt enable*/
#define _HAL_M0_IRQ_IS_REG        (LPC_GPIO1->IS)   /*interrupt sense*/
#define _HAL_M0_IRQ_EV_REG        (LPC_GPIO1->IEV)  /*Interrupt event*/ 
#define _HAL_M0_IRQ_BE_REG        (LPC_GPIO1->IBE)  /*Both edges register*/
#define _HAL_M0_IRQ_MIS_REG       (LPC_GPIO1->MIS)  /*RO reflects the interrupt event on the port*/
#define _HAL_M0_IRQ_VALUE_REG     (LPC_GPIO1->PIN)//(LPC_GPIO1->OUT)  /*Values of the entire port*/
#define _HAL_M0_IRQ_INTCL_REG     (LPC_GPIO1->IC)





#define IRQ_PIN_ON_PORT  (6)
#define DISABLE_PULL_UP  4
#define PIO1_6_RESET_VALUE 0x90

#define IRQEDG_POS  5
#define IRQMOD_POS  0

/*Two NOP cycles are reccomended after the clearing of the interrupt flag in */
/*order to take into account the sync mechanism between NVIC and GPIO domain*/

#define IRQ_Acknowledge()  {_HAL_M0_IRQ_INTCL_REG |= (0x01U<<IRQ_PIN_ON_PORT);__NOP();__NOP();}


#define IRQ_Init()\
{\
  _HAL_M0_IRQ_CLK_EN_REG |= 1<<30;\
    _HAL_M0_IRQ_DIR_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
      _HAL_M0_IRQ_GPIO_CTRL &= ~(0x01U<<DISABLE_PULL_UP);\
    _HAL_M0_IRQ_IS_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
    _HAL_M0_IRQ_BE_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
      _HAL_M0_IRQ_INT_REG |=(0x01U<<IRQ_PIN_ON_PORT);\
      NVIC_EnableIRQ(EINT1_IRQn);\
}
#define IRQ_SetEdge(edge)\
{\
    _HAL_M0_IRQ_IS_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
    _HAL_M0_IRQ_BE_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
    if (edge){_HAL_M0_IRQ_EV_REG |= (0x01U<<IRQ_PIN_ON_PORT);}\
    else{_HAL_M0_IRQ_EV_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);}\
}


inline bool_t IRQ_GetEdge(void)
{
    return(bool_t)((_HAL_M0_IRQ_EV_REG & (0x01<<IRQ_PIN_ON_PORT))?1:0);
}
/*
- Clear Interrupt Request Status and Control Register  
- Disable Bus clock to the IRQ module.
*/
#define IRQ_DeInit()\
{\
    _HAL_M0_IRQ_INT_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);\
    _HAL_M0_IRQ_GPIO_CTRL = PIO1_6_RESET_VALUE;\
}
#define IRQ_IntEnable(enable)\
{\
    if (enable){_HAL_M0_IRQ_INT_REG |=(0x01U<<IRQ_PIN_ON_PORT);NVIC_EnableIRQ(EINT1_IRQn);}\
    else{_HAL_M0_IRQ_INT_REG &= ~(0x01U<<IRQ_PIN_ON_PORT);NVIC_DisableIRQ(EINT1_IRQn);}\
}

inline bool_t IRQ_pinIsHigh(void)
{
    return(bool_t)((_HAL_M0_IRQ_VALUE_REG & (0x01U<<IRQ_PIN_ON_PORT))?1:0);    
}
#define IRQ_pinIsLow() (!IRQ_pinIsHigh())
#endif /* #if defined(HAL_IRQ_SUPPORT) */
/* ********************************************************************************************** */
/*                                   END IRQ Class HAL Support                                    */
/* ********************************************************************************************** */
















/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/*<belonging to the module, visible to the whole Platform project>*/
/* Max allow device frequency */
#define CLK_MAX_DEVICE_FREQ         33000000

/* Range of PLL reference frequency */
#define CLK_MIN_REF_RANGE           31250
#define CLK_MAX_REF_RANGE           39062





/** Define Clock Capability */
#define _HAL_CLK_CAPABILITY

/*PLL related registers*/
#define _HAL_M0_CLK_SYSPLLCTRL_REG                  (LPC_SYSCON->SYSPLLCTRL)
#define _HAL_M0_CLK_SYSPLLSTATUS_REG                (LPC_SYSCON->SYSPLLSTAT)
#define _HAL_M0_CLK_SYSPLLCLKSEL_REG                (LPC_SYSCON->SYSPLLCLKSEL)
#define _HAL_M0_CLK_SYSPLLCLKUEN_REG                (LPC_SYSCON->SYSPLLCLKUEN)




/*Clock generation related registers*/
#define _HAL_M0_CLK_SYSOSCCTRL_REG                  (LPC_SYSCON->SYSOSCCTRL)
#define _HAL_M0_CLK_IRCCTRL_REG                     (LPC_SYSCON->IRCCTRL)      /* IRC Trimming register*/
#define _HAL_M0_CLK_MAINCLKSEL_REG                  (LPC_SYSCON->MAINCLKSEL)
#define _HAL_M0_CLK_MAINCLKUEN_REG                  (LPC_SYSCON->MAINCLKUEN)
#define _HAL_M0_CLK_SYSAHBCLKDIV_REG                (LPC_SYSCON->SYSAHBCLKDIV)


/*Clockout related registers*/

#define _HAL_M0_CLK_CLKOUTCLKSEL_REG                (LPC_SYSCON->CLKOUTCLKSEL)
#define _HAL_M0_CLK_CLKOUTUEN_REG                   (LPC_SYSCON->CLKOUTUEN)
#define _HAL_M0_CLK_CLKOUTDIV_REG                   (LPC_SYSCON->CLKOUTDIV)


#define INTERNAL_CLK_SOURCE     0
#define SYSTEM_CLK_SOURCE     1
#define PLL_OUT__CLK_SOURCE     3

#define LOW   0
#define HIGH  1




#define CLK_SetPLLOutClockSource()                     {_HAL_M0_CLK_MAINCLKUEN_REG=LOW;_HAL_M0_CLK_MAINCLKSEL_REG=PLL_OUT__CLK_SOURCE;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH; }
#define CLK_SetInternalClockSource()                {_HAL_M0_CLK_MAINCLKUEN_REG=LOW;_HAL_M0_CLK_MAINCLKSEL_REG=INTERNAL_CLK_SOURCE;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH; }
#define CLK_SetExternalClockSource()                {_HAL_M0_CLK_MAINCLKUEN_REG=LOW;_HAL_M0_CLK_MAINCLKSEL_REG=SYSTEM_CLK_SOURCE;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH; }

/* There is no equivalent divider in LPC122x?? look at the driver */

#define CLK_SetRefDiv(dividor)                      { }


/* Internal or external reference clock for the PLL input*/
/* SYSPLLCLKSEL register must be unlocked before changing the value*/

#define IRC_SOURCE 0
#define EXT_SOURCE 1



#define CLK_SetInternalPLLRefClock()                {_HAL_M0_CLK_MAINCLKUEN_REG=LOW; _HAL_M0_CLK_SYSPLLCLKSEL_REG = IRC_SOURCE ;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH;}
#define CLK_SetExternalPLLRefClock()                {_HAL_M0_CLK_MAINCLKUEN_REG=LOW; _HAL_M0_CLK_SYSPLLCLKSEL_REG = EXT_SOURCE ;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH; }


/*Enable/Disable Internal Reference clock*/
/*    acting on the PCON domain  */

#define _HAL_M0_CLK_POWERDWN_REG                    (LPC_SYSCON->PDRUNCFG)
#define CLK_PWRDWN_IRC_MASK                         (0x01U<<0)                          
#define CLK_PWRDWN_EXT_OSC_MASK                     (0x01U<<5)

#define CLK_EnableInternalRefClock()                {_HAL_M0_CLK_POWERDWN_REG &= ~ CLK_PWRDWN_IRC_MASK;}
#define CLK_DisableInternalRefClock()               {_HAL_M0_CLK_POWERDWN_REG |=  CLK_PWRDWN_IRC_MASK;}


/*Not supported in LPC1227 microcontroller*/

#define CLK_EnableInternalRefClockInStopMode()      { }
#define CLK_DisableInternalRefClockInStopMode()     { }



/*System clock divider for LPC1227*/

#define _HAL_M0_CLK_DIV_REG                         (LPC_SYSCON->SYSAHBCLKDIV)
#define CLK_SetBusFreqDiv(divider)                  {_HAL_M0_CLK_DIV_REG = (uint8_t)divider;}



/* High and Low frequency range setup*/

#define HIGH_FREQUENCY   1
#define LOW_FREQUENCY    0


#define CLK_SetHighFreqRange()                      {_HAL_M0_CLK_SYSOSCCTRL_REG = HIGH_FREQUENCY;}
#define CLK_SetLowFreqRange()                       {_HAL_M0_CLK_SYSOSCCTRL_REG = LOW_FREQUENCY;}


/* Not implemented in LPC1227 microcontroller*/
#define CLK_SetHighGainOscillator()                 { }
#define CLK_SetLowPowerOscillator()                 { }

/*ByPass handling*/

#define BYPASS_ON              1
#define BYPASS_OFF             0

//#define CLK_DisableFLLInBypassMode()                {_HAL_M0_CLK_SYSOSCCTRL_REG = BYPASS_OFF; }
//#define CLK_EnableFLLInBypassMode()                 {_HAL_M0_CLK_SYSOSCCTRL_REG = BYPASS_ON; }

/*Disable PLL to amulate the Bypass mode turn PLL off*/



#define CLK_TurnsPLLOff()                           {_HAL_M0_CLK_POWERDWN_REG |= (0x01U<<7);}
#define CLK_TurnsPLLOn()                            {_HAL_M0_CLK_POWERDWN_REG &= ~(0x01U<<7);}  
#define CLK_DisableFLLInBypassMode()                {_HAL_M0_CLK_MAINCLKUEN_REG=LOW;_HAL_M0_CLK_MAINCLKSEL_REG =SYSTEM_CLK_SOURCE;_HAL_M0_CLK_MAINCLKUEN_REG=HIGH ;_HAL_M0_CLK_POWERDWN_REG |= (0x01<<7);} /*PLL OFF*/
#define CLK_EnableFLLInBypassMode()                 {_HAL_M0_CLK_POWERDWN_REG &= ~(0x01U<<7);} /*PLL ON*/



#define CLK_SetExternalRefOscillator()              { }
#define CLK_SetExternalRefClockSource()             { } 


#define CLK_EnableExternalRefClock()                {}
#define CLK_DisableExternalRefClock()               { }


#define CLK_EnableExternalRefClockInStopMode()      {}
#define CLK_DisableExternalRefClockInStopMode()     {}


#define CLK_SetTrim(trim)                           {_HAL_M0_CLK_IRCCTRL_REG = trim; }
#define CLK_GetTrim()                               (_HAL_M0_CLK_IRCCTRL_REG)




#define CLK_IsInternalFLLRefClock()                 (_HAL_M0_CLK_SYSPLLCLKSEL_REG == IRC_SOURCE ) 
#define CLK_IsExternalFLLRefClock()                 (_HAL_M0_CLK_SYSPLLCLKSEL_REG == EXT_SOURCE ) 





#define CLK_IsFLLClockSource()                      (_HAL_M0_CLK_MAINCLKSEL_REG == PLL_OUT__CLK_SOURCE) 
#define CLK_IsInternalClockSource()                 (_HAL_M0_CLK_MAINCLKSEL_REG == INTERNAL_CLK_SOURCE) 
#define CLK_IsExternalClockSource()                 (_HAL_M0_CLK_MAINCLKSEL_REG == EXTERNAL_CLK_SOURCE) 

#define CLK_IsOscillatorReady()                     (1)

#define CLK_SetFTrim(ftrim)                         { }
#define CLK_GetFTrim()                              ()




#define CLK_INT_CLOCK_FREQ            12000000
#define CLK_EXT_CLOCK_FREQ            12000000



//#define CLK_EnableExternalRefClock(var)  {var++;}

//#define CLK_DisableExternalRefClock(var) {var++;}


inline void CLK_EngageClkRefSync(void) 
{   
                            
//    int global;                     
                                                            
                                                            
#if defined(CLK_EXT_REF_CLOCK_ACTIVE)

  /* Enable external clock source for the processor */    
//    CLK_EnableExternalRefClock(global);                           

#else                                                       

    /* Disable external clock source for the processor */   
//    CLK_DisableExternalRefClock(global);                          

#endif    /*CLK_EXT_REF_CLOCK_ACTIVE */
    
                                                            
#if defined(CLK_INT_REF_CLOCK_ACTIVE)                              
    
    /* Enable internal clock source for the processor */    
    CLK_EnableInternalRefClock();                           

#else                                                       

    /* Disable internal clock source for the processor */   
    CLK_DisableInternalRefClock();                          

#endif /*CLK_INT_REF_CLOCK_ACTIVE*/   
}                                                                                                                       

/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the internal
reference clock in the FLL bypass mode.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline bool_t CLK_SetInternalClockSync(uint32_t desiredClock)
{
    uint32_t frequency, frequency0;
    int32_t difference, difference0;
    uint8_t i;
#if defined (CLK_INT_REF_CLOCK_ACTIVE)
    
    i = 0;
    
    frequency0 = (int32_t)CLK_INT_CLOCK_FREQ;
    
    do
    {
        frequency = frequency0 >> i;
        i++;
    }
    while (!(frequency <= desiredClock));

    /* Difference between the desired and possible frequency */    
    difference = desiredClock - frequency;
    
    /* Makes absolute value */
    if (difference < 0)
    {
        difference = -difference;
    }

    /* Difference between the desired and possible frequency from the previous step */
    difference0 = desiredClock - frequency0;
    
    /* Makes absolute value */
    if (difference0 < 0)
    {
        difference0 = -difference0;
    }
    
    /* If the previous step frequncy was closer to the desired, changes the i */
    if (difference0 < difference)
    {
       i--;
    }


    /* Selects the internal clock source */
    CLK_SetInternalClockSource();

    /* Sets the bus frequency dividor */
    CLK_SetBusFreqDiv(i);
    
    /*Turns the PLL off since it is not used*/
   CLK_TurnsPLLOff();
    
    
    

    return (bool_t)1; 
#else
    return (bool_t)0;
#endif
}


/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the external
reference clock in the FLL bypass mode.

\param  [in]	desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Massimo Incerti
\date		20/05/2011
\version	1.0.0

*/
inline bool_t CLK_SetExternalClockSync(uint32_t desiredClock)
{
#if defined (CLK_EXT_REF_CLOCK_ACTIVE) 
    int32_t frequency, frequency0;
    int32_t difference, difference0;
    uint8_t i;
    i = 0;
    
    frequency0 = (int32_t)CLK_EXT_CLOCK_FREQ;
    
    do
    {
        frequency = frequency0 >> i;
        i++;
    }
    while ((i < 255) && !(frequency <= desiredClock));

    /* Difference between the desired and possible frequency */    
    difference = desiredClock - frequency;
    
    /* Makes absolute value */
    if (difference < 0)
    {
        difference = -difference;
    }

    /* Difference between the desired and possible frequency from the previous step */
    difference0 = desiredClock - frequency0;
    
    /* Makes absolute value */
    if (difference0 < 0)
    {
        difference0 = -difference0;
    }
    
    /* If the previous step frequncy was closer to the disered, changes the i */
    if (difference0 < difference)
    {
        i--;
    }
    

    /* Sets the external reference clock for PLL */
//      CLK_SetExternalFLLRefClock();
    
    
    /* Selects the external clock source */
    CLK_SetExternalClockSource();


    /* Sets the bus frequency dividor */
    CLK_SetBusFreqDiv(i);
    
      /*Turns the PLL off since it is not used*/
   CLK_TurnsPLLOff();
    
    

    return (bool_t)1; 
#else
    (void) desiredClock;
    return (bool_t)0;
#endif
}

typedef  uint32_t  pllclk_t;

#define BEGIN_MODULE_SYS_EXT_MAP(n)    static const pllclk_t SYS_PLL_CLKout_EXT[n]={
  
#define END_MODULE_SYS_EXT_MAP()       };

#define SYS(i)                     ((pllclk_t)i)



BEGIN_MODULE_SYS_EXT_MAP(13)
SYS(CLK_EXT_CLOCK_FREQ),
SYS(2*CLK_EXT_CLOCK_FREQ),
SYS(3*CLK_EXT_CLOCK_FREQ),
SYS(4*CLK_EXT_CLOCK_FREQ),
SYS(5*CLK_EXT_CLOCK_FREQ),
SYS(6*CLK_EXT_CLOCK_FREQ),
SYS(7*CLK_EXT_CLOCK_FREQ),
SYS(8*CLK_EXT_CLOCK_FREQ),
SYS(9*CLK_EXT_CLOCK_FREQ),
SYS(10*CLK_EXT_CLOCK_FREQ),
SYS(11*CLK_EXT_CLOCK_FREQ),
SYS(12*CLK_EXT_CLOCK_FREQ),
SYS(13*CLK_EXT_CLOCK_FREQ)
END_MODULE_SYS_EXT_MAP()       


#define BEGIN_MODULE_SYS_INT_MAP(n)    static const pllclk_t SYS_PLL_CLKout_INT[n]={
  
#define END_MODULE_SYS_INT_MAP()       };

BEGIN_MODULE_SYS_INT_MAP(13)
SYS(CLK_INT_CLOCK_FREQ),
SYS(2*CLK_INT_CLOCK_FREQ),
SYS(3*CLK_INT_CLOCK_FREQ),
SYS(4*CLK_INT_CLOCK_FREQ),
SYS(5*CLK_INT_CLOCK_FREQ),
SYS(6*CLK_INT_CLOCK_FREQ),
SYS(7*CLK_INT_CLOCK_FREQ),
SYS(8*CLK_INT_CLOCK_FREQ),
SYS(9*CLK_INT_CLOCK_FREQ),
SYS(10*CLK_INT_CLOCK_FREQ),
SYS(11*CLK_INT_CLOCK_FREQ),
SYS(12*CLK_INT_CLOCK_FREQ),
SYS(13*CLK_INT_CLOCK_FREQ)
END_MODULE_SYS_INT_MAP()       





/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with FLL on the external
reference clock.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Massimo Incerti
\date		20/05/2011
\version	1.0.0

*/

#define PLL_MIN_FREQ    10000000
#define PLL_MAX_FREQ    25000000

static const uint8_t multipliers[4]={1,2,4,8};

#pragma diag_suppress=Pe177
static bool_t CLK_SetFLLExternalClockSync(uint32_t desiredClock)
{
uint32_t ahbdiv=0;
bool_t found;
uint8_t i,psel;

uint8_t M,P;
uint32_t risultato,temp;

char counter;


CLK_SetExternalPLLRefClock();



 for(counter=0;counter<=12;counter++)
 {
 
    if((SYS_PLL_CLKout_EXT[counter]%desiredClock)==0)   
    {
      M=counter;
      risultato=SYS_PLL_CLKout_EXT[counter];
      
      if(M==0)
        M=1;
           
      found=1;
      break;
      
    } 
  
 }
 
 if(found==0)
   return (bool_t)0;
 
 
 
 /*P calculation*/
 
 
 for (i=0;i<=3;i++)
 {

   temp=risultato*2*multipliers[i];
   if((temp>156000000)&(temp<320000000))
   {
     P=2*multipliers[i];
     break;
   }   
      
   
 }   
 
 
 
 ahbdiv=(risultato)/desiredClock;
 
 switch(P)
 {case 1: 
   psel=0x00;
   break;
 case 2:
   psel=0x01;
   break;
 case 4:
   psel=0x02;
   break;
 case 8:
   psel=0x03;
   break;
 }  
 #pragma diag_remark=Pe177
 
 
 _HAL_M0_CLK_SYSPLLCTRL_REG =(M)|
                              (psel<<5)| /*calculated psel previous step*/
                              (0<<7)| /*Direct*/
                              (0<<8); /*Bypass=0*/  
 
 
 /*Power Up PLL*/
 
  CLK_TurnsPLLOn();

  /*Set divider to generate the sustem clock */
  
  LPC_SYSCON->SYSAHBCLKDIV = ahbdiv;
  
  /*Wait for the PLL to be locked*/
  
  while(!(_HAL_M0_CLK_SYSPLLSTATUS_REG));
  CLK_SetPLLOutClockSource();
  
  return (bool_t)1;
  
  
}





/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with PLL on the internal
reference clock.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error



\author 	Massimo Incerti
\date		20/05/2010
\version	1.0.0
\Note:           
*/










#pragma diag_suppress=Pe177
static bool_t CLK_SetFLLInternalClockSync(uint32_t desiredClock)
{

    uint32_t ahbdiv=0;
    bool_t found;
    uint8_t i,psel;
    uint8_t M,P;
    uint32_t risultato,temp;

    char counter;

    CLK_SetInternalPLLRefClock();


    for(counter=0;counter<=12;counter++)
    {
    
        if((SYS_PLL_CLKout_INT[counter]%desiredClock)==0)   
        {
            M=counter;
            risultato=SYS_PLL_CLKout_INT[counter];
            
            if(M==0)
            {
                M=1;
            }
               
            found=1;
            break;
          
        } 
    
    }
 
 if(found==0)
   return (bool_t)0;
 
 
 
 /*P calculation*/
 
 
 for (i=0;i<=3;i++)
 {

   temp=risultato*2*multipliers[i];
   if((temp>156000000)&(temp<320000000))
   {
     P=2*multipliers[i];
     break;
   }   
      
   
 }   
 
 
 
 ahbdiv=(risultato)/desiredClock;
 
 switch(P)
 {case 1: 
   psel=0x00;
   break;
 case 2:
   psel=0x01;
   break;
 case 4:
   psel=0x02;
   break;
 case 8:
   psel=0x03;
   break;
 }  
 #pragma diag_remark=Pe177
 
 
 _HAL_M0_CLK_SYSPLLCTRL_REG =(M)|
                              (psel<<5)| /*calculated psel previous step*/
                              (0<<7)| /*Direct*/
                              (0<<8); /*Bypass=0*/  
 
 
 /*Power Up PLL*/
 
  CLK_TurnsPLLOn();

  /*Set divider to generate the sustem clock */
  
  LPC_SYSCON->SYSAHBCLKDIV = ahbdiv;
  
  /*Wait for the PLL to be locked*/
  
  while(!(_HAL_M0_CLK_SYSPLLSTATUS_REG));
  CLK_SetPLLOutClockSource();
  
  return (bool_t)1;
  
  
}

  
  
  
  



/**
Sets up the trimming value. The function feeds both
the trimming register and the fine trimming bit.

\param  [in]		desiredTrim 16-bit value where the trim value is the higher 8 bits
                                and the fine trim bit is the highest bit of the lower 8 bits.
                                I.e. trim bit 8 - 15, fine trim bit 7, unused bits 0 - 6.


\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline void CLK_SetTrimSync(uint16_t desiredTrim)
{
    CLK_SetTrim((uint8_t)(desiredTrim));
    
}

/**
Gets the trimming value. not implemented

\return 	uint16_t

\retval		0

\author 	Roberto Fioravanti
\date		5/02/2010
\version	1.0.0

*/
#define CLK_GetTrimSync()       0


                                                            
/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                   */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN WDT Class HAL Support                                   */
/* ********************************************************************************************** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define WatchDog Capability */
#define _HAL_WDT_CAPABILITY


/**
Feed the dog
*/
#define WDT_Refresh() {\
  LPC_WDT->FEED = 0xAA;\
  LPC_WDT->FEED = 0x55;\
}

/*
After writing 0xAA to WDFEED, access to any Watchdog register other than writing
0x55 to WDFEED causes an immediate reset/interrupt when the Watchdog is enabled,
and sets the WDTOF flag. The reset will be generated during the second PCLK following
an incorrect access to a Watchdog register during a feed sequence
*/
#define WDT_ResetMCU()\
{\
  LPC_WDT->FEED = 0xAA;\
  LPC_WDT->TC = 0;\
}

#define UserReset() WDT_ResetMCU()

/* internal functions for the WD HAL */

/* the programmed values are written in the wachdog block */
/* configuration details */
/* MODE reg: */
/*  - Watchdog event will reset the chip */
/*  - WDTOF is always cleared by writing zero */
/*  - WDINT is always cleared by writing one */
/*  - WDPROTECT bit is set depending on UPDATE_AFTER_WRITE */
/*  - WDLOCKCLOCK bit is left open */
/*  - WDLOCKDP bit is left open */
/*  - WDLOCKEN lock bit is set depending on UPDATE_AFTER_WRITE */
/* CLKSEL reg: */
/*  - WDLOCK bit is left open so that changing of clock source is possible */
#pragma inline


#define WDT_FREQ          (2.6)
#define WDT_CTRL_VAL      (0x0A<<5)
#define WDT_MILLIS_TO_TICK(millis) (((int)(((millis)*2.6*1000000)/(4*1000*2))) & 0xFFFFFF)

void WDT_InitSync(void) 
{ 

  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);	        /* provide clock*/
  LPC_SYSCON->PDRUNCFG &= ~(0x1U<<6);    	/* Let WDT clock run */
  
  LPC_SYSCON->WDTOSCCTRL = WDT_CTRL_VAL;        /*divider*/
  LPC_WDT->MOD = (1u << 1);  		        /*mode: reset*/
  
  LPC_WDT->WDCLKSEL &= ~0x80000003;
  LPC_WDT->WDCLKSEL = 1; /* WD oscillator */ //WDT_ALTERNATE_CLOCK;
  LPC_WDT->TC = WDT_MILLIS_TO_TICK(WDT_TIMEOUT);
    
  WDT_Refresh();

  LPC_WDT->WARNINT=0;					/* no warning */
  
  LPC_WDT->MOD |=  (WDT_ENABLED << 0);	/* enable */
  
  WDT_Refresh();
  
  if(WDT_WINDOWED_MODE == 1)
  { 
    LPC_WDT->WINDOW =WDT_MILLIS_TO_TICK(WDT_WINDOW);
  }
  else if(WDT_WINDOWED_MODE == 0) 
  { 
    LPC_WDT->WINDOW = 0xFFFFFF;
  };
  
  
  
}


#define WDT_SetSync(timeout) {\
  LPC_WDT->TC = (timeout & 0xFFFFFF);\
}


#define WDT_StartSync() {\
  LPC_WDT->MOD |= 0x1;\
  LPC_WDT->FEED = 0xAA;\
  LPC_WDT->FEED = 0x55;\
}

/* clear the enable bit */
/* keep reset as wd event */
/* clear wdtof and wdint */
#define WDT_StopSync() {\
  LPC_WDT->MOD = 0x0A;\
  LPC_WDT->FEED = 0xAA;\
  LPC_WDT->FEED = 0x55;\
}

#define WDT_SetWindowingSync(window) {\
  LPC_WDT->WINDOW = (window & 0xFFFFFF);\
}

#define WDT_AllowUpdate() WDT_UPDATE_AFTER_WRITE


/* refresh to apply values */ 
#define WDT_StopWindowingSync() {\
  LPC_WDT->WINDOW = 0xFFFFFF;\
}

#define WDT_GetWindow()       ((WDT_counterType) LPC_WDT->WINDOW)

#define WDT_Enabled()         ((LPC_WDT->MOD & 0x1)?1:0)

#define WDT_Counter()         ((WDT_counterType) LPC_WDT->TV)
#define WDT_IsWindowOpen()    (WDT_Counter()<WDT_GetWindow()? 1 : 0)

/**
Unsupported 
*/  
#define WDT_RSTCounter() 0   
#define WDT_ClearRSTCounter()
#define WDT_SetQuickTestSync(timeout)    \
    (void)timeout;
#define WDT_SetByteTestSync(timeout, testByte) \
    (void)timeout;  \
    (void)testByte;
#define WDT_IsTest() 0

/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_FLS_SUPPORT)

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define Flash Capability */
#define _HAL_FLS_CAPABILITY

/** Define Commands for managing flash resident in RAM */
/** Define Data for managing flash API result resident in RAM */
#define FLASH_CMD_IN_RAM

/** All flash's Pages have the same size */
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
/** This is the size in bytes of a flash's page.*/
#define FLASH_PAGE_SIZE_IN_BYTES    512

// used to compute page number from position (address)
// shift 9 bit right equals dividing by 512, page size
#define FLS_PAGE_SHIFTVAL 9

// used to compute page number from position (address)
// shift 12 bit right equals dividing by 4096, sector size
#define FLS_SECTOR_SHIFTVAL 12

#define BUS_FREQUENCY_KHz (BUS_FREQUENCY_Hz / 1000) 



/* Map mcu specific registers names to a 'standard' set of names 

				HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>

	(this helps when generating new files for additional parts of the same family).
*/


// used to add one wait state in case freq > 30 MHz
#define _HAL_M0_FLS_OVERRIDE (0x1u << 15) 
#define _HAL_M0_FLS_ONE_WAITSTATE (BUS_FREQUENCY_Hz > 30000000)

// define the address of the IAP routines
#define IAP_LOCATION (0x1FFF1FF1)


// definition of the IAP command types
#define NX_CMD_IDX 0

typedef enum IAP_CMD_tag
{
  PREPARE_FOR_WRITE = 50,
  COPY_RAM_TO_FLASH = 51,
  ERASE_SECTOR = 52,
  BLANK_CHECK_SECTOR = 53,
  READ_PART_ID = 54,
  READ_BCODE_VERS = 55,
  COMPARE = 56,
  REINVOKE = 57,
  READ_UID = 58,
  ERASE_PAGE = 59,
  ERASE_INFO_PAGE = 60
    
} IAP_CMD_TYPE;

// definition of the IAP return status types
#define NX_RETURN_CODE_IDX 0

typedef enum IAP_CMD_RSTAT_tag
{
  CMD_SUCCESS = 0,
  INVALID_CMD = 1,
  SRC_ADDR_ERR = 2,
  DST_ADDR_ERR = 3,
  SRC_ADDR_NOT_MAPPED = 4,
  DST_ADDR_NOT_MAPPED = 5,
  COUNT_ERR = 6,
  INVALID_SECTOR = 7,
  SECTOR_NOT_BLANK = 8,
  SECTOR_NOT_PREPARED = 9,
  COMPARE_ERR = 10,
  BUSY = 11
  
} IAP_RSTAT_TYPE;


/**
Initialization of the FLASH Module.\n
Programming and erase operations are allowed. 
 */
// add one waitstate for the flash by clearing PRESETCTRL bit 15
// bc.of speed < 40 MHz no need to change also FLASHCFG, is preset at reset for 1ws
#if(_HAL_M0_FLS_ONE_WAITSTATE)
  #define FLS_Init() (LPC_SYSCON->PRESETCTRL &= ~(_HAL_M0_FLS_OVERRIDE))
#else
  #define FLS_Init() 
#endif

/**
Abort any command and clear errors 
*/
#define FLS_InitRegsForErasing() {}
/* _HAL_S08_FLS_NXTAT_REG =_HAL_S08_FLS_NXTAT_ERR_MASK; */

///**
//Abort any command and clear errors 
//*/
#define FLS_InitRegsForProgramming() {}

/* position is divided by 512 to determine the start page number */
/* since driver erases page by page, end page number is equal to */
/* start page number */
#define FLS_Erase(position)\
{\
  RamCmd.iap_entry = (IAP) IAP_LOCATION;\
  RamCmd.iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
  RamCmd.iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
  RamCmd.iapCommand[2] = ((position) >> FLS_SECTOR_SHIFTVAL);\
  RamCmd.iap_entry((unsigned int*) RamCmd.iapCommand, (unsigned int*) RamCmd.iapResult);\
\
  if(CMD_SUCCESS == RamCmd.iapResult[NX_RETURN_CODE_IDX])\
  {\
      RamCmd.iapCommand[NX_CMD_IDX] = ERASE_PAGE;\
      RamCmd.iapCommand[1] = ((position) >> FLS_PAGE_SHIFTVAL);\
      RamCmd.iapCommand[2] = ((position) >> FLS_PAGE_SHIFTVAL);\
      RamCmd.iapCommand[3] = BUS_FREQUENCY_KHz;\
      RamCmd.iap_entry((unsigned int*) RamCmd.iapCommand, (unsigned int*) RamCmd.iapResult);\
  }\
}

      
/* first prepare sectors for write */
/* then perform the write */
//#define FLS_Programming(pSource,size,position)\
//{\
//\
//  iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
//  iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
//  iapCommand[2] = ((position) >> FLS_SECTOR_SHIFTVAL);\
//  iap_entry((unsigned int*) iapCommand, (unsigned int*) iapResult);\
//\
//  if(CMD_SUCCESS == iapResult[NX_RETURN_CODE_IDX])\
//  {\
//    iapCommand[NX_CMD_IDX] = COPY_RAM_TO_FLASH;\
//    iapCommand[1] = (position);\
//    iapCommand[2] = (uint32_t)(pSource);\
//    iapCommand[3] = (size);\
//    iapCommand[4] = BUS_FREQUENCY_KHz;\
//    iap_entry((unsigned int*) iapCommand, (unsigned int*) iapResult);\
//  }\
//}
#define FLS_Programming(pSource, size, position) {\
  FLS_positionType  pF;\
  uint8_t* pS; \
  int32_t tempSize;\
  uint32_t  i = 0, j = 0;\
  uint8_t unalignedBuffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};  \
  RamCmd.iap_entry = (IAP) IAP_LOCATION;\
  RamCmd.iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
  RamCmd.iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
  RamCmd.iapCommand[2] = ((position + size) >> FLS_SECTOR_SHIFTVAL);\
  RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
  if(CMD_SUCCESS != RamCmd.iapResult[NX_RETURN_CODE_IDX]) return (IO_ErrorType)IO_E_PROGRAMMING;\
\
  pF = position;\
  pS = (uint8_t*) pSource;\
\
  if(pF & 0x3)\
  {\
    pF = pF & 0xFFFFFFFC;\
    for(i=(position & 0x3), j=0; (i < 4) && (j < size); i++, j++)\
    {\
      unalignedBuffer[i] = (uint8_t) *((uint8_t*)pS++);\
    }\
\
    RamCmd.iapCommand[NX_CMD_IDX] = COPY_RAM_TO_FLASH;\
    RamCmd.iapCommand[1] = pF;\
    RamCmd.iapCommand[2] = (uint32_t)(unalignedBuffer);\
    RamCmd.iapCommand[3] = (4);\
    RamCmd.iapCommand[4] = BUS_FREQUENCY_KHz;\
    RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
    pF+=4;\
\
    if(CMD_SUCCESS != RamCmd.iapResult[NX_RETURN_CODE_IDX]) return (IO_ErrorType)IO_E_PROGRAMMING;\
  }\
\
  if (j != size)\
  {\
  tempSize = size - j;\
\
  if(tempSize >= 4)\
  {\
    if(((uint32_t)pS & 0x3))\
    {\
      while(tempSize >= 4 )\
      {\
        RamCmd.iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
        RamCmd.iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
        RamCmd.iapCommand[2] = ((position + size) >> FLS_SECTOR_SHIFTVAL);\
        RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
        unalignedBuffer[0] = *pS;\
        unalignedBuffer[1] = *(pS + 1);\
        unalignedBuffer[2] = *(pS + 2);\
        unalignedBuffer[3] = *(pS + 3);\
        RamCmd.iapCommand[NX_CMD_IDX] = COPY_RAM_TO_FLASH;\
        RamCmd.iapCommand[1] = pF;\
        RamCmd.iapCommand[2] = (uint32_t)(unalignedBuffer);\
        RamCmd.iapCommand[3] = (4);\
        RamCmd.iapCommand[4] = BUS_FREQUENCY_KHz;\
        RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
        pF += sizeof(FLS_VerifyType);\
    	pS += sizeof(FLS_VerifyType);\
        tempSize-=sizeof(FLS_VerifyType);\
      }\
    }\
    else\
    {\
      RamCmd.iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
      RamCmd.iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
      RamCmd.iapCommand[2] = ((position + size) >> FLS_SECTOR_SHIFTVAL);\
      RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
      RamCmd.iapCommand[NX_CMD_IDX] = COPY_RAM_TO_FLASH;\
      RamCmd.iapCommand[1] = (pF);\
      RamCmd.iapCommand[2] = (uint32_t)(pS);\
      RamCmd.iapCommand[3] = (tempSize & 0xFFFFFFFC);\
      RamCmd.iapCommand[4] = BUS_FREQUENCY_KHz;\
      RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
      pF +=  (tempSize & 0xFFFFFFFC);\
      pS +=  (tempSize & 0xFFFFFFFC);\
    }\
  }\
\
  if(tempSize & 0x3)\
  {\
\
    for(i=0;i<(tempSize & 0x3);i++)\
    {\
      unalignedBuffer[i] = *(pS++);\
    }\
\
    for(; i < 4; i++)\
    {\
      unalignedBuffer[i] = 0xFF;\
    }\
\
    RamCmd.iapCommand[NX_CMD_IDX] = PREPARE_FOR_WRITE;\
    RamCmd.iapCommand[1] = ((position) >> FLS_SECTOR_SHIFTVAL);\
    RamCmd.iapCommand[2] = ((position + size) >> FLS_SECTOR_SHIFTVAL);\
    RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
\
    RamCmd.iapCommand[NX_CMD_IDX] = COPY_RAM_TO_FLASH;\
    RamCmd.iapCommand[1] = (pF);\
    RamCmd.iapCommand[2] = (uint32_t)(unalignedBuffer);\
    RamCmd.iapCommand[3] = (4);\
    RamCmd.iapCommand[4] = BUS_FREQUENCY_KHz;\
    RamCmd.iap_entry((unsigned int*)RamCmd.iapCommand, (unsigned int*)RamCmd.iapResult);\
  }\
  }\
}


// returns zero if successful, non zero if error
#define FLS_CheckErrors() RamCmd.iapResult[NX_RETURN_CODE_IDX]

#endif /*defined(HAL_FLS_SUPPORT)*/
/* ********************************************************************************************** */
/*                                  END FLS Class HAL Support                                     */
/* ********************************************************************************************** */

///* ********************************************************************************************** */
///*                                  BEGIN ADC Class HAL Support                                   */
///* ********************************************************************************************** */
//#if defined(HAL_ADC_SUPPORT)
//
///* Calculate Prescaler Value 
//   factor:=(PRS+1)*2; ATD Conversion frequency = BusClockFrequency/factor
//*/
//#define ADC_PRS	(BUS_FREQUENCY_Hz/(2*ADC_CONV_FREQUENCY_Hz)-1)
//#if (ADC_PRS>15)
//#error "please check ADC_CONV_FREQUENCY_Hz value is not compatible with BUS_FREQUENCY_Hz "
//#endif
//#if (ADC_PRS<0)
//#error "please check ADC_CONV_FREQUENCY_Hz value is not compatible with BUS_FREQUENCY_Hz"
//#endif
//#if ((BUS_FREQUENCY_Hz/((ADC_PRS+1)*2))>2000000)
//#error "problem encounterd with ADC_PRS computation"
//#endif
//#if ((BUS_FREQUENCY_Hz/((ADC_PRS+1)*2))<500000)
//#error "problem encounterd with ADC_PRS computation"
//#endif
//
//
///** This is the  ATD Control Register (bit3-bit0 used for prescaler) */
//#define _HAL_S08_ADC_ATD1C_REG					(ATD1C)
///** This is the bit position of ATDPU bit */
//#define _HAL_S08_ADC_ATD1C_ATDPU_BITPOS			7
///** This is the bit position of DJM bit */
//#define _HAL_S08_ADC_ATD1C_DJM_BITPOS			6
///** This is the bit position of RES8 bit */
//#define _HAL_S08_ADC_ATD1C_RES8_BITPOS			5
///** This is the bit position of SGN bit */
//#define _HAL_S08_ADC_ATD1C_SGN_BITPOS			4
///** This is the  ATD Status and Control Register (ATD1SC) (bit5-bit0 Channel Selection)*/
//#define _HAL_S08_ADC_ATD1SC_REG					(ATD1SC)
///** This is the bit position of CCF bit (read only)*/
//#define _HAL_S08_ADC_ATD1SC_CCF_BITPOS			7
///** This is the bit position of ATDIE bit */
//#define _HAL_S08_ADC_ATD1SC_ATDIE_BITPOS		6
///** This is the bit position of ATDCO bit */
//#define _HAL_S08_ADC_ATD1SC_ATDCO_BITPOS		5
///** This is the mask for channel field inside ATD1SC  */
//#define _HAL_S08_ADC_ATD1SC_ATDCH_MASK          0x1F
//
///* Channel selection */
//#define ADC_CH0_SEL		0
//#define ADC_CH1_SEL		1
//#define ADC_CH2_SEL		2
//#define ADC_CH3_SEL		3
//#define ADC_CH4_SEL		4
//#define ADC_CH5_SEL		5
//#define ADC_CH6_SEL		6
//#define ADC_CH7_SEL		7
//#define ADC_VREFH_SEL	0x1E
//#define ADC_VREFL_SEL	0x1F
//
///** This is the  ATD Result Data Register Address (ATD1R) (word, read only) */
//#define _HAL_S08_ADC_ATD1R_ADDR					((uint16_t*)0x0052)
//#define _HAL_S08_ADC_ATD1R                      (ATD1R)
//
//
///** This is the  ATD Pin Enable Register (ATD1PE) (bitmap) */
//#define _HAL_S08_ADC_ATD1PE_REG					(ATD1PE)
///**
//\n\li ATD Power Up
//\n\li Result register data is right justified
//\n\li 10-bit resolution selected
//*/
//#define ADC_Init()\
//{\
//	_HAL_S08_ADC_ATD1C_REG =\
//	((1<<_HAL_S08_ADC_ATD1C_ATDPU_BITPOS)|(1<<_HAL_S08_ADC_ATD1C_DJM_BITPOS)|ADC_PRS);\
//}
///**
//Set all ADC Registers to the reset state value
//*/
//#define ADC_DeInit()\
//{\
//	_HAL_S08_ADC_ATD1C_REG = 0x00;\
//	_HAL_S08_ADC_ATD1SC_REG = 0x01;\
//	_HAL_S08_ADC_ATD1PE_REG = 0x00;\
//}
///**
//'0' if channel is allowable '1' otherwise 
//*/
//#define ADC_CheckChannel(channel)	(((channel<=7)||(channel==ADC_VREFH_SEL)||(channel==ADC_VREFL_SEL))?0:1)
///**
//\n\li if channel <= 7 set, according to the selected channel, the pin as dedicated to the ATD module.
//\n\li if channel > 8 clear ATD1PE register
//\n\li A write to this register will abort the current conversion but will not initiate a new conversion.
//*/
//#define ADC_ConfigurePinAsChannel(channel)	((channel<=7)?(_HAL_S08_ADC_ATD1PE_REG |= (1<<channel)):(_HAL_S08_ADC_ATD1PE_REG = 0x00))
///**
//\n\li if channel <= 7 set, according to the selected channel, the pin to default configuration.
//\n\li if channel > 8 clear ATD1PE register
//\n\li A write to this register will abort the current conversion but will not initiate a new conversion.
//*/
//#define ADC_DeConfigurePinAsChannel(channel)	((channel<=7)?(_HAL_S08_ADC_ATD1PE_REG &= (~(1<<channel))):(_HAL_S08_ADC_ATD1PE_REG = 0x00))
///**
//\n Writes to the ATD status and control register 
//\n\li clear the CCF flag 
//\n\li cancels any pending interrupts
//\n\li clear or set ATD Interrupt
//\n\li clear ATD Continuous Conversion
//\n\li initiates a new conversion.
//
//*/
//#define ADC_StartConvertionOnChannel(channel,atdInterrupt)	(_HAL_S08_ADC_ATD1SC_REG = (channel|(atdInterrupt?(1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS):0)))
//
///**
//Channel selected for ATD convertion
//*/
//#define ADC_GetActiveChannel()	(_HAL_S08_ADC_ATD1SC_REG & _HAL_S08_ADC_ATD1SC_ATDCH_MASK )
//
///**
//\n Wait for Conversion Complete.
//\n The CCF is a read-only bit which is set each time a conversion is complete. 
//*/
//#define ADC_WaitForConvertionOnChannel(channel)	while((_HAL_S08_ADC_ATD1SC_REG&(1<<_HAL_S08_ADC_ATD1SC_CCF_BITPOS)) == 0){}
//
///**
//\n Test Conversion Complete Flag.
//\n The CCF is a read-only bit which is set each time a conversion is complete. 
//*/
//#define ADC_EndConvertion()	(_HAL_S08_ADC_ATD1SC_REG&(1<<_HAL_S08_ADC_ATD1SC_CCF_BITPOS))
///**
//\n\li ADC Conversion Result is ever in 10 bit 
//\n\li This macro modify it to the desired resolution
//*/
//#define ADC_GetCorrectResolutionResult(result)   ((ADC_RESOLUTION > 10)?(result<<=(ADC_RESOLUTION-10)):(result>>=(10-ADC_RESOLUTION)))               
//
///**
//\n\li ADC Conversion Result is ever in 10 bit 
//\n\li This function get the converted value and modify it to the desired resolution
// */	
//#pragma INLINE
//static uint16_t ADC_GetConvertedValueOnChannel(void)
//{
//uint16_t adConvValue;
//	adConvValue = *_HAL_S08_ADC_ATD1R_ADDR;
//    ADC_GetCorrectResolutionResult(adConvValue);
//	return(adConvValue);
//}
//
///**
//Writes to the ATD status and control register clears the CCF flag, cancels any pending interrupts, and
//initiates a new conversion.
//A write to the ATD pin enable register will abort the current conversion but will not initiate a new conversion. 
//*/
//
//#define ADC_StopConversion()\
//{\
//_HAL_S08_ADC_ATD1SC_REG = 0x01;\
//_HAL_S08_ADC_ATD1PE_REG = 0x00;\
//}
//
//
//
//#define ADC_pResultRegister()   (&_HAL_S08_ADC_ATD1R)
//#define ADC_pChannelRegister()  (&_HAL_S08_ADC_ATD1SC_REG)
//
///**
//\n Writes to the ATD status and control register 
//\n\li clear the CCF flag 
//\n\li cancels any pending interrupts
//\n\li set ATD Interrupt
//\n\li set ATD Continuous Conversion
//\n\li initiates a new conversion on channel
//*/
//#define ADC_StartMultiSampleConvertionOnChannel(channel)\
//{\
//_HAL_S08_ADC_ATD1SC_REG = ((1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS)|(1<<_HAL_S08_ADC_ATD1SC_ATDCO_BITPOS)|channel);\
//}
//
//#define ADC_StartMultiSampleConvertionMultiChannel()
//
//
//#define ADC_CorrectChannel(channel)     (channel | (1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS))
//
//#endif /* #if defined(HAL_ADC_SUPPORT) */
///* ********************************************************************************************** */
///*                                  END ADC Class HAL Support                                     */
///* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_TIMER_SUPPORT)


/** Define TMR Capability */
#define _HAL_TMR_CAPABILITY

/* total number of on-chip hw counters */

#if !defined(LPC0_28_MASK)
#error "Ciao"
#endif

#define TMR_PIN(x,y)    \
{\
    (DIO_PortAddressBasicType)LPC_GPIO##x##_BASE,\
    (DIO_PortAddressExtendedType)(LPC_IOCON_BASE+GPIO_IOCON##x##y##_MASK),\
    (DIO_ModuleMapMaskType)LPC##x##_##y##_MASK\
}

#define TMR_PIN_DUMMY \
{\
    (DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,\
    (DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR,\
    (DIO_ModuleMapMaskType)0\
}

/* data register accesses are always 32-bit */
typedef uint32_t _HAL_RegType; 

/* link CMSIS definition type to address pointer type used within driver */
/* driver will use pointers to tmrcntreg_struct_t */
typedef LPC_TMR_TypeDef tmrcntreg_struct_t;

typedef struct TMR_Map_tag
{
	DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register relaated to the port pin */
        DIO_PortAddressExtendedType extAddress;
	DIO_ModuleMapMaskType PinMask;        /**< Port pin mask  */
} TMR_MapType;

#define BEGIN_MODULE_TMR_MAP(n)    static const TMR_MapType TmrChToPinMap[n]={

#define END_MODULE_TMR_MAP()       };

/* Table of mapped channels */

BEGIN_MODULE_TMR_MAP(NUM_HW_CNTR_CH)
		/*16B0*/
        TMR_PIN(2,0),
        TMR_PIN(2,1),
        TMR_PIN_DUMMY,	/*on 16-bit timer outputs 2&3 are not routed out of the chip*/
        TMR_PIN_DUMMY,	/*on 16-bit timer outputs 2&3 are not routed out of the chip*/
		/*16B1*/
        TMR_PIN(2,2),
        TMR_PIN(2,3),
        TMR_PIN_DUMMY,	/*on 16-bit timer outputs 2&3 are not routed out of the chip*/
        TMR_PIN_DUMMY,	/*on 16-bit timer outputs 2&3 are not routed out of the chip*/
		/*32B0*/
        TMR_PIN(2,4),
        TMR_PIN(2,5),
        TMR_PIN(2,6),
        TMR_PIN(2,7),
		/*32B1*/
        TMR_PIN(2,8),
        TMR_PIN(2,9),
        TMR_PIN(2,10),
        TMR_PIN(2,11)
END_MODULE_TMR_MAP()

/* addresses of the base pointers for the timer IP */
/* set the timer address, will use CMSIS where possible */
static const TMR_CNTAddressType TmrCntMap[NUM_HW_CNTR]=
{
    (TMR_CNTAddressType) LPC_CT16B0_BASE, 
    (TMR_CNTAddressType) LPC_CT16B1_BASE, 
    (TMR_CNTAddressType) LPC_CT32B0_BASE,
    (TMR_CNTAddressType) LPC_CT32B1_BASE
};

/* these masks are going to be used for the clock gating at the SYS bus level */
/* used in SYSAHBCLKCTRL register */
#define CT16B0_CLK_MASK (1u << 7)
#define CT16B1_CLK_MASK (1u << 8)
#define CT32B0_CLK_MASK (1u << 9)
#define CT32B1_CLK_MASK (1u << 10)

/* table of clock gating fields for the timers */
static const uint32_t TimerClockGate[NUM_HW_CNTR]=
{
  CT16B0_CLK_MASK,
  CT16B1_CLK_MASK,
  CT32B0_CLK_MASK,
  CT32B1_CLK_MASK
};

/* for the channels, there is one control register for all, not one for each */
/* in this case link CMSIS definition type to address pointer type used within driver */
/* driver will use pointers to tmrcntreg_struct_t */
typedef TMR_CHAddressType tmrchreg_struct_t; 

/* this defines the mapping for the channels for all timers */
/* in this implementation the match register addresses are stored in the table */
/* !!! not !!! the control register address */
/* why: on LPC there is just one control register for all match (channel) registers */
/* it does not make sense to repeat the same control register address in each 4 locations */
/* within the structure, so the match register address MRn is chosen instead */
/* within the HAL implementation, the address of the match register will be used to */
/* get back to the control register address, and to determine which of the channels will be referenced */
#define _MR_OFFSET_MASK (0x3F)
#define _MR0_OFFSET (0x18)
#define _MR1_OFFSET (0x1C)
#define _MR2_OFFSET (0x20)
#define _MR3_OFFSET (0x24)

#define LPC_TMR16B0_MR0 ((0x40010018UL))
#define LPC_TMR16B0_MR1 (LPC_CT16B0_BASE + _MR1_OFFSET)
#define LPC_TMR16B0_MR2 (LPC_CT16B0_BASE + _MR2_OFFSET)
#define LPC_TMR16B0_MR3 (LPC_CT16B0_BASE + _MR3_OFFSET)
#define LPC_TMR16B1_MR0 (LPC_CT16B1_BASE + _MR0_OFFSET)
#define LPC_TMR16B1_MR1 (LPC_CT16B1_BASE + _MR1_OFFSET)
#define LPC_TMR16B1_MR2 (LPC_CT16B1_BASE + _MR2_OFFSET)
#define LPC_TMR16B1_MR3 (LPC_CT16B1_BASE + _MR3_OFFSET)
#define LPC_TMR32B0_MR0 (LPC_CT32B0_BASE + _MR0_OFFSET)
#define LPC_TMR32B0_MR1 (LPC_CT32B0_BASE + _MR1_OFFSET)
#define LPC_TMR32B0_MR2 (LPC_CT32B0_BASE + _MR2_OFFSET)
#define LPC_TMR32B0_MR3 (LPC_CT32B0_BASE + _MR3_OFFSET)
#define LPC_TMR32B1_MR0 (LPC_CT32B1_BASE + _MR0_OFFSET)
#define LPC_TMR32B1_MR1 (LPC_CT32B1_BASE + _MR1_OFFSET)
#define LPC_TMR32B1_MR2 (LPC_CT32B1_BASE + _MR2_OFFSET)
#define LPC_TMR32B1_MR3 (LPC_CT32B1_BASE + _MR3_OFFSET)

#pragma diag_suppress=Pe177
static const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH]=
{
    /* this are the four channels for timer 0 CT16B0 */                          
    {(TMR_CHAddressType) LPC_TMR16B0_MR0, HW_TMR_ID_0},
    {(TMR_CHAddressType) LPC_TMR16B0_MR1, HW_TMR_ID_0},
    {(TMR_CHAddressType) LPC_TMR16B0_MR2, HW_TMR_ID_0},
    {(TMR_CHAddressType) LPC_TMR16B0_MR3, HW_TMR_ID_0},
                          
    /* this are the four channels for timer 1 CT16B1 */
    {(TMR_CHAddressType) LPC_TMR16B1_MR0, HW_TMR_ID_1},
    {(TMR_CHAddressType) LPC_TMR16B1_MR1, HW_TMR_ID_1},
    {(TMR_CHAddressType) LPC_TMR16B1_MR2, HW_TMR_ID_1},
    {(TMR_CHAddressType) LPC_TMR16B1_MR3, HW_TMR_ID_1},

    /* this are the four channels for timer 2 CT32B0 */
    {(TMR_CHAddressType) LPC_TMR32B0_MR0, HW_TMR_ID_2},
    {(TMR_CHAddressType) LPC_TMR32B0_MR1, HW_TMR_ID_2},
    {(TMR_CHAddressType) LPC_TMR32B0_MR2, HW_TMR_ID_2},
    {(TMR_CHAddressType) LPC_TMR32B0_MR3, HW_TMR_ID_2},
                          
    /* this are the four channels for timer 3 CT32B1 */
    {(TMR_CHAddressType) LPC_TMR32B1_MR0, HW_TMR_ID_3},
    {(TMR_CHAddressType) LPC_TMR32B1_MR1, HW_TMR_ID_3},
    {(TMR_CHAddressType) LPC_TMR32B1_MR2, HW_TMR_ID_3},
    {(TMR_CHAddressType) LPC_TMR32B1_MR3, HW_TMR_ID_3},                          
};
#pragma diag_remark=Pe177

/**********************************************************/                          
/* following some definitions of the register masks */  
/* private definitions of HAL */
/**********************************************************/
#define _CLEAR_MASK_DEFAULT (0xFFFFFFFF)
                          
/* interrupt register IR */
#define _IR_DEFAULT (0x0)
                          
/* definition of the interrupt masks */
#define _IR_MR0_INT  (1u << 0)
#define _IR_MR1_INT  (1u << 1)
#define _IR_MR2_INT  (1u << 2)
#define _IR_MR3_INT  (1u << 3)

/* definition of the capture masks */
#define _IR_CR0_INT  (1u << 4)
#define _IR_CR1_INT  (1u << 5)
#define _IR_CR2_INT  (1u << 6)
#define _IR_CR3_INT  (1u << 7)

/* this mask is used to determine which channel is linked to the driver channel id */
/* it works only if the number of channels is multiple of two and equal for all timers */
/* like on LPC1227 */
#ifdef NXP_LPC12xx_1227
  // use last two bits of channel ID to determine the LPC id by masking
  #define _LPC_CHID_MASK  (0x3)
  #define _LPC_TID_MASK  (0x3)                          
#elif
  #error "LPC Target Hal does not support usage of _LPC_CHID_MASK in _getModuloChMask, "
#endif


/* this function returns the interrupt mask related to a specific match channel */
uint32_t _getMatchIntChMask(uint32_t chId);

/* this function returns the interrupt mask related to a specific capture channel */
#pragma inline 
uint32_t _getCaptureIntChMask(uint32_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_IR_CR0_INT);
      
    case 1: 
      return(_IR_CR1_INT);
      
    case 2:
      return(_IR_CR2_INT);
      
    case 3:
      return(_IR_CR3_INT);
  
    default:
      return(0); // should never get here
  }
}

/* timer counter registr TC */
#define _TC_DEFAULT (0x0)
                          
/* timer control registr TCR */
#define _TCR_DEFAULT (0x0)
                          
#define _TCR_CEN    (1u << 0) // counter enable
#define _TCR_CRST   (1u << 1) // counter reset

/* this function just checks if the timer is running or not */
#pragma inline
bool_t _isTimerStopped(tmrcntreg_struct_t* pTmr_cnt)
{ 
  if( !((pTmr_cnt->TC) & _TCR_CEN) || ((pTmr_cnt->TC) & _TCR_CRST))
  { 
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }     
}
                          
/* match control register MCR */
/* definition of the interrupt masks */
#define _MR0_INT (1u << 0)  // timer interrupt on match0
#define _MR1_INT (1u << 3)  // timer interrupt on match1
#define _MR2_INT (1u << 6)  // timer interrupt on match2
#define _MR3_INT (1u << 9)  // timer interrupt on match3

/* this function returns the interrupt enable mask related to a specific match channel */
/* in the mask bits 31:4 are zero, the mask is made to be used in |= mode to enable the interrupt */
#pragma inline 
static uint32_t _createMcrIntEnChMask(uint32_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_MR0_INT);
      
    case 1: 
      return(_MR1_INT);
      
    case 2:
      return(_MR2_INT);
      
    case 3:
      return(_MR3_INT);
  
    default:
      return(0); // should never get here
  }
}

/* this function returns the interrupt disable mask related to a specific match channel */
/* in the mask bits 31:12 are set at 0, the mask is made to be used in &= mode */
#pragma inline
static uint32_t _createMcrIntDisChMask(uint8_t source) 
{
  _HAL_RegType mcrClearMask = (0xFFF);
  
  switch(source) {

    case 0:
      mcrClearMask &= (~_MR0_INT); // bit 0 zeroed
    break;
  
    case 1:
      mcrClearMask &= (~_MR1_INT); // bit 3 zeroed
    break;
  
    case 2:     
      mcrClearMask &= (~_MR2_INT); // bit 6 zeroed
  
    case 3:     
      mcrClearMask &= (~_MR3_INT); // bit 9 zeroed
    break;
  }
  
  return(mcrClearMask);
}


/* definition of the reset masks */
#define _MR0_RESET (1u << 1)  // reset timer on match0
#define _MR1_RESET (1u << 4)  // reset timer on match1
#define _MR2_RESET (1u << 7)  // reset timer on match2
#define _MR3_RESET (1u << 10) // reset timer on match3



/* this function computes the modulo channel mask for a specific channel */
#pragma inline 
static uint32_t _getModuloChMask(uint8_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_MR0_RESET);
      
    case 1: 
      return(_MR1_RESET);
      
    case 2:
      return(_MR2_RESET);
      
    case 3:
      return(_MR3_RESET);
  
    default:
      return(0); // should never get here
  }
}

/* this function creates the mask to disable the reset feature (modulo channel) */
/* the bits from 31:12 are zero, the mask is to be used in &= mode */
#pragma inline
static uint32_t _createModuloChClrMask(uint32_t chId)
{
  _HAL_RegType mcrResetClearMask = (0xFFF);
  
  switch(chId & _LPC_CHID_MASK) {

    case 0:
      mcrResetClearMask &= (~_MR0_RESET); // bit 1 zeroed
    break;
  
    case 1:
      mcrResetClearMask &= (~_MR1_RESET); // bit 4 zeroed
    break;
  
    case 2:     
      mcrResetClearMask &= (~_MR2_RESET); // bit 7 zeroed
  
    case 3:     
      mcrResetClearMask &= (~_MR3_RESET); // bit 10 zeroed
    break;
  }
  
  return(mcrResetClearMask);
}


/* definition of the stop masks */
#define _MR0_STOP (1u << 2)  // timer stop on match0
#define _MR1_STOP (1u << 5)  // timer stop on match1
#define _MR2_STOP (1u << 8)  // timer stop on match2
#define _MR3_STOP (1u << 11) // timer stop on match3

/* this function returns the stop enable mask related to a specific match channel */
#pragma inline 
static uint32_t _getMcrStopEnChMask(uint32_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_MR0_STOP);
      
    case 1: 
      return(_MR1_STOP);
      
    case 2:
      return(_MR2_STOP);
      
    case 3:
      return(_MR3_STOP);
  
    default:
      return(0); // should never get here
  }
}                          

/* this function creates the mask to disable the stop feature (modulo channel) */
/* bits 31:12 are zero, mask is to be used in &= mode */
#pragma inline 
static uint32_t _createMcrStopDisChMask(uint32_t chId)
{
  _HAL_RegType mcrStopDisableMask = (0xFFF);
  
  switch(chId & _LPC_CHID_MASK) {

    case 0:
      mcrStopDisableMask &= (~_MR0_STOP); // bit 2 zeroed
    break;
  
    case 1:
      mcrStopDisableMask &= (~_MR1_STOP); // bit 5 zeroed
    break;
  
    case 2:     
      mcrStopDisableMask &= (~_MR2_STOP); // bit 8 zeroed
  
    case 3:     
      mcrStopDisableMask &= (~_MR3_STOP); // bit 11 zeroed
    break;
  }
  
  return(mcrStopDisableMask);
}


/* match register MR */
#define _MR_DEFAULT (0x0)

/* capture control register CCR  */
#define _CCR_DEFAULT (0x0U)

/* definition of the capture masks */
#define _CAP0_RE   (1u << 0) // timer capture0 on rising edge
#define _CAP0_FE   (1u << 1) // timer capture0 on falling edge
#define _CAP0_INT  (1u << 2) // timer interrupt on capture0

#define _CAP1_RE   (1u << 3) // timer capture1 on rising edge
#define _CAP1_FE   (1u << 4) // timer capture1 on falling edge
#define _CAP1_INT  (1u << 5) // timer interrupt on capture1

#define _CAP2_RE   (1u << 6) // timer capture2 on rising edge
#define _CAP2_FE   (1u << 7) // timer capture2 on falling edge
#define _CAP2_INT  (1u << 8) // timer interrupt on capture2

#define _CAP3_RE   (1u << 9)  // timer capture3 on rising edge
#define _CAP3_FE   (1u << 10) // timer capture3 on falling edge
#define _CAP3_INT  (1u << 11) // timer interrupt on capture3


#define _CAP0_CLR_MASK (~((_CAP0_RE)|(_CAP0_FE)|(_CAP0_INT))) // bit 2:0 are zeroed
#define _CAP1_CLR_MASK (~((_CAP1_RE)|(_CAP1_FE)|(_CAP1_INT))) // bit 5:3 are zeroed
#define _CAP2_CLR_MASK (~((_CAP2_RE)|(_CAP2_FE)|(_CAP2_INT))) // bit 8:6 are zeroed
#define _CAP3_CLR_MASK (~((_CAP3_RE)|(_CAP3_FE)|(_CAP3_INT))) // bit 11:9 are zeroed

/* this function returns the interrupt enable mask related to a specific channel */
/* bits 31:12 are zero, mask is to be used in |= mode */
#pragma inline 
static uint32_t _createCcrIntEnChMask(uint32_t chId)
{
  switch(chId & _LPC_CHID_MASK) {
    
  case 0:
      return(_CAP0_INT);
      
    case 1: 
      return(_CAP1_INT);
      
    case 2:
      return(_CAP2_INT);
      
    case 3:
      return(_CAP3_INT);
  
    default:
      return(0); // should never get here
  }
} 

/* this function creates a disable mask mask for the capture control register related to a specific channel */
/* it returns a mask with the specific bit cleared, mask to be used in &= mode  */
#pragma inline 
static uint32_t _createCcrIntDisChMask(uint32_t chId)
{
  _HAL_RegType ccrIntDisMask = (0xFFF);

  switch(chId & _LPC_CHID_MASK) {
    
    case 0:
      ccrIntDisMask &= (~_CAP0_INT); // bit 2 is zeroed
      break;
      
    case 1: 
      ccrIntDisMask &= (~_CAP1_INT); // bit 5 is zeroed
      break;
      
    case 2:
      ccrIntDisMask &= (~_CAP2_INT); // bit 8 is zeroed
      break;
      
    case 3:
      ccrIntDisMask &= (~_CAP3_INT); // bit 11 is zeroed
      break;
    }

  return(ccrIntDisMask);
} 

/* this function is used to create a CCR mask */
/* used to clear CCR bits when picking an external timer source clock */
/* bits 31:12 are zero, mask is to be used in &= mode */
#pragma inline 
static _HAL_RegType _createCcrClearMaskForClkSource(uint8_t source) 
{
  _HAL_RegType ccrClearMask = (0xFFF);
  
  switch(source) {

    case CAP0_IN:
      ccrClearMask &= _CAP0_CLR_MASK; // bit 2:0 are zeroed
    break;
  
    case CAP1_IN:
      ccrClearMask &= _CAP1_CLR_MASK; // bit 5:3 are zeroed
    break;
  
    case CAP2_IN:      
      ccrClearMask &= _CAP2_CLR_MASK; // bit 8:6 are zeroed
    break;
  
    case CAP3_IN:  
      ccrClearMask &= _CAP3_CLR_MASK; // bit 11:9 are zeroed
    break;
  }
  
  return(ccrClearMask);
}

/* this function is used to create a ccr clear mask based on channel id  */
/* called by TMR_DeInitChannel > _SetChannelDefaultMode */              
/* cannot use _createCcrClearMaskForClkSource since switch/case definitions are different */
#pragma inline 
static _HAL_RegType _createCcrClearMask(uint8_t chId) 
{
  _HAL_RegType ccrClearMask = (0xFFF);
  
  switch(chId & _LPC_CHID_MASK) {

    case CAP0_IN:
      ccrClearMask &= _CAP0_CLR_MASK; // bit 2:0 are zeroed
    break;
  
    case CAP1_IN:
      ccrClearMask &= _CAP1_CLR_MASK; // bit 5:3 are zeroed
    break;
  
    case CAP2_IN:    
      ccrClearMask &= _CAP2_CLR_MASK; // bit 8:6 are zeroed
    break;
  
    case CAP3_IN:    
      ccrClearMask &= _CAP3_CLR_MASK; // bit 11:9 are zeroed
    break;
  }
  
  return(ccrClearMask);
}                          
                          
/* capture register CAP */
#define _CR_DEFAULT (0x0)


/* external match register EMR */
#define _EMR_DEFAULT (0x0)

/* definition of the external match pin actions */
#define _EXTMATCH_NOCHANGE (0u)
#define _EXTMATCH_SET      (1u)
#define _EXTMATCH_CLR      (2u)
#define _EXTMATCH_TOGGLE   (3u)

/* definition of the position bits for the EMC status */
#define _EMR_EM0 (1u << 0)
#define _EMR_EM1 (1u << 1)
#define _EMR_EM2 (1u << 2)
#define _EMR_EM3 (1u << 3)

/* definition of the position bits for the EMC control */
#define _EMR_EMC0_POS (4)
#define _EMR_EMC1_POS (6)
#define _EMR_EMC2_POS (8)
#define _EMR_EMC3_POS (10)

/* definitions of actions for external match */
#define _EMR_EMC_NOCHANGE(n)  ( _EXTMATCH_NOCHANGE << _EMR_EMC##n##_POS )
#define _EMR_EMC_SET(n)       ( _EXTMATCH_SET << _EMR_EMC##n##_POS )
#define _EMR_EMC_CLR(n)       ( _EXTMATCH_CLR << _EMR_EMC##n##_POS )
#define _EMR_EMC_TOGGLE(n)    ( _EXTMATCH_TOGGLE << _EMR_EMC##n##_POS )

/* use this mask for pre-set of the external match outputs */
#define _EMR_EM_HIGH(n) (_EMR_EM##n##)

/* the preset is to have all outputs high */
/* can be changed with this macro */
#define _EMR_PRESET  (_EMR_EM_HIGH(0) | _EMR_EM_HIGH(1) | _EMR_EM_HIGH(2) | _EMR_EM_HIGH(3))

/* this function is used to clear the EMR bitfields */
/* bits 31:12 are zero, mask is to be used in &= mode */
#pragma inline 
static uint32_t _createEmrClrMask(uint32_t chId)
{
  _HAL_RegType emrClrMask;
  emrClrMask = ((0xFFF) & _EMR_PRESET);
  
  switch(chId & _LPC_CHID_MASK) {

    case 0:
      emrClrMask &= _EMR_EMC_NOCHANGE(0); // bit 5:4 are zeroed
    break;
  
    case 1:
      emrClrMask &= _EMR_EMC_NOCHANGE(1); // bit 7:6 are zeroed
    break;
  
    case 2:    
      emrClrMask &= _EMR_EMC_NOCHANGE(2); // bit 9:8 are zeroed
    break;
  
    case 3:    
      emrClrMask &= _EMR_EMC_NOCHANGE(3); // bit 11:10 are zeroed
    break;
  }
  
  return(emrClrMask);
}

/* count control register CTCR */
#define _CTCR_DEFAULT (0x0)

/* definition of the timer / counter mode */
#define _CTM_TIMER_MODE   (0u) // work in timer mode
#define _CTM_RECAP_MODE   (1u) // counter mode on rising edge
#define _CTM_FECAP_MODE   (2u) // counter mode on falling edge
#define _CTM_ANYCAP_MODE  (3u) // counter mode on both edges

/* definition of the count input select */
#define _CIS_CAP0_IN        (0u << 2)       // capture 0 is counter input
#define _CIS_CAP1_IN        (1u << 2)       // capture 1 is counter input
#define _CIS_CAP2_IN        (2u << 2)       // capture 2 is counter input (16-bit counter only)
#define _CIS_CAP3_IN        (3u << 2)       // capture 3 is counter input (16-bit counter only)                         
#define _CIS_COMP_LEVEL     (_CIS_CAP2_IN)  // comparator level is sampled for clocking (32-bit counter only)
#define _CIS_COMP_EDGE      (_CIS_CAP3_IN)  // comparator edge is sampled for clocking (32-bit counter only)

/* definition of capture clear */
#define _ENCC_CAPCLR        (1u << 4)  

/* definition of clear source selection */
#define _SELCC_CAP0RE     (0u << 5)
#define _SELCC_CAP0FE     (1u << 5)

#define _SELCC_CAP1RE     (2u << 5)
#define _SELCC_CAP1FE     (3u << 5)
#define _SELCC_CAP2RE     (4u << 5)
#define _SELCC_CAP2FE     (5u << 5)
#define _SELCC_CAP3RE     (6u << 5)
#define _SELCC_CAP3FE     (7u << 5)

/* this function is private and used to define the CTCR mask for the timer source clock */
#pragma inline 
static _HAL_RegType _getCtcrMask(tmrcntreg_struct_t *tmr_ptr, uint32_t source)
{ 
  _HAL_RegType ctcrMask = _CTCR_DEFAULT;
  
  //#define TMR_CLK_SOURCE_IS_BUS_CLOCK             0x00
  //#define TMR_CLK_SOURCE_IS_EXTERNAL_PIN          0x02  /* both edge */
  //#define TMR_CLK_SOURCE_IS_EXTERNAL_PIN_RE       0x12  /* rising edge */
  //#define TMR_CLK_SOURCE_IS_EXTERNAL_PIN_FE       0x22  /* falling edge */  
  switch(source) {
    
    case TMR_CLK_SOURCE_IS_BUS_CLOCK:
      ctcrMask |= _CTM_TIMER_MODE; // work in timer mode
      return(ctcrMask); // no need to reconfigure the rest
  
    case TMR_CLK_SOURCE_IS_EXTERNAL_PIN_RE:
      ctcrMask |= _CTM_RECAP_MODE; // counter mode on rising edge
    break;
  
    case TMR_CLK_SOURCE_IS_EXTERNAL_PIN_FE:    
      ctcrMask |= _CTM_FECAP_MODE; // counter mode on falling edge
    break;
  
    case TMR_CLK_SOURCE_IS_EXTERNAL_PIN:
      ctcrMask |= _CTM_ANYCAP_MODE; // counter mode on both edges
    break;
  }

  //  #define TMR_ID0_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // choose cap0in for clocking
  //  #define TMR_ID1_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // choose cap0in for clocking
  //  #define TMR_ID2_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // choose cap0in for clocking
  //  #define TMR_ID3_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // choose cap0in for clocking
  // timerId = _getTimerIdFromAddress(tmr_ptr);
  // pick the correct choice depending on the timer address
  switch((TMR_AddressType)tmr_ptr) {

    // if the source clock is not bus clock, 
    // configure which count input select is desired for this clock
    case LPC_CT16B0_BASE:
      ctcrMask |= TMR_ID0_CLK_SOURCE_INPUT_SEL;
      // need to clear the corresponding capture control fields in CCR !
      tmr_ptr->CCR &= _createCcrClearMaskForClkSource(TMR_ID0_CLK_SOURCE_INPUT_SEL);
    break;
  
    case LPC_CT16B1_BASE:
      ctcrMask |= TMR_ID1_CLK_SOURCE_INPUT_SEL; 
      tmr_ptr->CCR &= _createCcrClearMaskForClkSource(TMR_ID1_CLK_SOURCE_INPUT_SEL);
    break;
  
    case LPC_CT32B0_BASE:
      ctcrMask |= TMR_ID2_CLK_SOURCE_INPUT_SEL; 
      tmr_ptr->CCR &= _createCcrClearMaskForClkSource(TMR_ID2_CLK_SOURCE_INPUT_SEL);
    break;
  
    case LPC_CT32B1_BASE:
      ctcrMask |= TMR_ID3_CLK_SOURCE_INPUT_SEL; 
      tmr_ptr->CCR &= _createCcrClearMaskForClkSource(TMR_ID3_CLK_SOURCE_INPUT_SEL);
    break;
  }
  

  return(ctcrMask);
}


                          
/* Pwm control register PWMC */
#define _PWMC_DEFAULT (0x0)

/* definition of bit masks for the pwm mode */
#define _PWMC_PWM0_EN (1u << 0)
#define _PWMC_PWM1_EN (1u << 1)
#define _PWMC_PWM2_EN (1u << 2)
#define _PWMC_PWM3_EN (1u << 3)

#define _PWMC_PWM0_DIS ((~_PWMC_PWM0_EN) & (0xF))
#define _PWMC_PWM1_DIS ((~_PWMC_PWM0_EN) & (0xF))
#define _PWMC_PWM2_DIS ((~_PWMC_PWM0_EN) & (0xF))
#define _PWMC_PWM3_DIS ((~_PWMC_PWM0_EN) & (0xF))

/* this function is used to enable the PWM bitfields */
/* returns a mask with bit set depending on the channel */
#pragma inline 
static _HAL_RegType _createPwmEnMask(uint32_t chId)
{  
  switch(chId & _LPC_CHID_MASK) {

    case 0:
      return(_PWMC_PWM0_EN);
  
    case 1:
      return(_PWMC_PWM1_EN);
  
    case 2:    
      return(_PWMC_PWM2_EN);
  
    case 3:    
      return(_PWMC_PWM3_EN);
  }
  return(0); // should never get here
}

/* this function is used to clear the PWM bitfields */
/* bit 31:4 are zero, mask is to be used in &= mode */
#pragma inline 
static _HAL_RegType _createPwmClrMask(uint32_t chId)
{
  _HAL_RegType pwmClrMask = (0xF);
  
  switch(chId & _LPC_CHID_MASK) {

    case 0:
      pwmClrMask &= (_PWMC_PWM0_DIS);
    break;
  
    case 1:
      pwmClrMask &= (_PWMC_PWM1_DIS);
    break;
  
    case 2:    
      pwmClrMask &= (_PWMC_PWM2_DIS);
    break;
  
    case 3:    
      pwmClrMask &= (_PWMC_PWM3_DIS);
    break;
  }
  
  return (pwmClrMask);
}

/*******************************************************/
/* module level utility functions */
/*******************************************************/
                          
/* this variable is used to track the mode of the channel (input capture / output compare) */
/* 0 is output compare, 1 is input capture */ 
/*  extern uint32_t _hal_channelMode;  */

/* input capture is set to 1 */
static void _trackChannelInputCapture(uint8_t channelNum) 
{
  DIO_PortAddressExtendedType chFunc = TmrChToPinMap[channelNum].extAddress;

  if (channelNum >= NUM_HW_CNTR_CH)
    return;
  
  switch(channelNum)
  {
  case 0:
    ((portreg_ext_struct_t *) chFunc)->function = (uint8_t) 3; 
    break;

  case 2:
  case 3:
  case 5:
  case 6:
    break;
    
  default:
   ((portreg_ext_struct_t *) chFunc)->function = (uint8_t) 2;     
    break;
    
  }

}

/* output compare is set to 0  */
static void _trackChannelOutputCompare(uint8_t channelNum)
{
  DIO_PortAddressExtendedType chFunc = TmrChToPinMap[channelNum].extAddress;

  if (channelNum >= NUM_HW_CNTR_CH)
  return;
  
  switch(channelNum)
  {
  case 0:
    ((portreg_ext_struct_t *) chFunc)->function = (uint8_t) 0x04; 
    break;
    
  case 2:
  case 3:
  case 5:
  case 6:
    break;
    
  default:
    ((portreg_ext_struct_t *) chFunc)->function = (uint8_t) 0x03; 
    break;
    
  }

}


#define _setChannelModeFlagDefault(channelNum) ( _trackChannelOutputCompare(channelNum) )

static bool_t isChannelOutputCompare(uint8_t channelNum) 
{
  DIO_PortAddressExtendedType chFunc = TmrChToPinMap[channelNum].extAddress;

  if (channelNum < NUM_HW_CNTR_CH)
  {  
    switch(channelNum)
    {
    case 0:
      if ( ((portreg_ext_struct_t *) chFunc)->function == 0x04)
      { 
        return TRUE;
      }
      break;
  
    case 2:
    case 3:
    case 5:
    case 6:
      break;
      
    default:
      if ( ((portreg_ext_struct_t *) chFunc)->function == 0x03)
      { 
        return TRUE;
      }
      break;
      
    }
  
  }
  return FALSE;
  
}                          

bool_t isChannelInputCapture(uint8_t channelNum);

                                    
/* this variable is used to track the PWM mode of the channel */
/* 1 is PWM mode */ 
extern uint32_t _hal_pwmEnabled;

/* PWM is set to 1 */
static void _trackChannelPwm(uint8_t channelNum) { _hal_pwmEnabled |= (1 << channelNum); }

#define _setChannelPwmFlagDefault(channelNum) { _hal_pwmEnabled &= (~(1u << channelNum)); }

static bool_t isChannelPwm(uint8_t channelNum) {
  if( _hal_pwmEnabled & (1 << channelNum))
  { 
    return (TRUE);
  }
  else
  {
    return (FALSE);
  } 
}                          
                                    
                                    
/* this variable is used to track the modulo channel of the counter */                                    
/* see NX_M0_LPC1227_hal.c for implementation details */                                    
extern uint8_t _hal_tmrModuloChannel;
                                    
static void _trackModuloChannel(uint8_t timerId, uint8_t channelId)
{ 
  // safety check to make sure channelId is in range
  channelId &= _LPC_CHID_MASK; 

  switch(timerId & _LPC_TID_MASK)
  {
  case 0:
      _hal_tmrModuloChannel |= channelId;
    break;
  case 1:
      _hal_tmrModuloChannel |= (channelId << 2);    
    break;
  case 2:
      _hal_tmrModuloChannel |= (channelId << 4);
    break;
  case 3:
      _hal_tmrModuloChannel |= (channelId << 6);    
    break;
  }
}

static uint8_t _getModuloChannel(uint8_t timerId)
{
  switch(timerId & _LPC_TID_MASK)
  {
  case 0:
      return(_hal_tmrModuloChannel & (0x3));
  case 1:
      return(_hal_tmrModuloChannel & (0x3 << 2));
  case 2:
      return(_hal_tmrModuloChannel & (0x3 << 4));
  case 3:
      return(_hal_tmrModuloChannel & (0x3 << 6));   
  }
  return(0); // should never get here  
}

static void _resetModuloChannelDefault(uint8_t timerId)
{
  switch(timerId & _LPC_TID_MASK)
  {
  case 0:
      _hal_tmrModuloChannel &= (0xFC);
    break;
  case 1:
      _hal_tmrModuloChannel &= (0xF3);    
    break;
  case 2:
      _hal_tmrModuloChannel &= (0xCF);
    break;
  case 3:
      _hal_tmrModuloChannel &= (0x3F);    
    break;
  }  
}

                                    
/* this function determines the ID of the timer from the base address */
/* CT16B0 id = 0, CT16B1 id = 1, CT32B0 id = 2, CT32B1 id = 3 */
/* address map: */
/* CT16B0 0x4001 0000 */
/* CT16B1 0x4001 4000 */
/* CT32B0 0x4001 8000 */
/* CT32B1 0x4001 C000 */
#define _TMR_ID_ADDRESS_SHIFT (14)
#pragma inline 
static uint8_t _getTimerIdFromAddress(tmrcntreg_struct_t* tmr_ptr)
{
  _HAL_RegType mask;
  
  // shift 14 bits right
  mask = ((_HAL_RegType)tmr_ptr >> _TMR_ID_ADDRESS_SHIFT); 
  
  // mask for the only significant bits A[15:14]
  mask &= (0x3);  

  return((uint8_t)mask);
}

/* this function is used to get the LPC channel number from the register address */
#pragma inline 
static uint8_t _getLpcChannelIdFromAddress(tmrchreg_struct_t* tmr_ptr)
{  
  switch((uint32_t)tmr_ptr & _MR_OFFSET_MASK) {
    case _MR0_OFFSET:
      return(0);

    case _MR1_OFFSET:
      return(1);

    case _MR2_OFFSET:
      return(2);

    case _MR3_OFFSET:
      return(3);
  };
  return(0); // should never get here
}

/* this function returns the timer base address depending on the number */
#pragma inline 
static TMR_CNTAddressType _getTmrBaseAddress(uint32_t lpcTimerId)
{
  switch(lpcTimerId) {
    
    case HW_TMR_ID_0:
      return(LPC_CT16B0_BASE);
      
    case HW_TMR_ID_1: 
      return(LPC_CT16B1_BASE);
      
    case HW_TMR_ID_2:
      return(LPC_CT32B0_BASE);
      
    case HW_TMR_ID_3:
      return(LPC_CT32B1_BASE);
  
    default:
      return(0); // should never get here
  };
}


/* this function configures default modes for the channel, by clearing: */
/* reset and stop on MCR */
/* capture edges on CCR */
/* external match control on EMR (if the timer is not running!!) */   
/* match mode in PWM register */
#pragma inline 
static void _SetChannelDefaultMode(tmrchreg_struct_t *pTmr_ch)
{
  uint32_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];

  // get the channel id, this is the LPC id not the driver id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);         
    
  if(isChannelOutputCompare(channelId + (timerId*NUM_HW_CNTR)))
  {
    // clear the bit in the MCR with appropriate mask
    // in this case the masking will work since LPC ids are 
    // assigned in the same fashion as the driver does
    timerAddress->MCR &= _createModuloChClrMask(channelId);
    timerAddress->MCR &= _createMcrStopDisChMask(channelId);
  
    // clear the bit in the EMR with appropriate mask
    if(_isTimerStopped(timerAddress)) timerAddress->EMR &= _createEmrClrMask(channelId);
    
  } else if(isChannelInputCapture(channelId + (timerId*NUM_HW_CNTR)))
  {
    // clear the capture mode for the CCR
    switch(channelId) {

    case 0: 
      timerAddress->CCR &= _createCcrClearMask(0);
      break;
    case 1: 
      timerAddress->CCR &= _createCcrClearMask(1);
      break;
    case 2: 
      timerAddress->CCR &= _createCcrClearMask(2);
      break;
    case 3: 
      timerAddress->CCR &= _createCcrClearMask(3);
      break;
    default:
      break;
    }     
  }

  // reset the PWM mode if used in the control register
  timerAddress->PWMC &= _createPwmClrMask(channelId);    
  
  // clear the track flags
  _setChannelPwmFlagDefault(channelId + (timerId*NUM_HW_CNTR)); 
  _setChannelModeFlagDefault(channelId + (timerId*NUM_HW_CNTR));
                            
}



/* ***************************************** */                          
/* HAL function implementations */                          
/* ***************************************** */                          
                          
/* this function fails if the clock source for modulo (clear source) is not specified */
/* by means of a channel ID. The LPC uses a generic channel as a reset source */
/* so if the freescale channel setup is passed (TMR_CLEAR_SOURCE_IS_INTERNAL)*/
/* the check needs to fail */
#pragma inline 
bool_t CheckClearSourceIdFailed(uint8_t clearingSourceId)
{
  if(TMR_CLEAR_SOURCE_IS_INTERNAL==clearingSourceId) { return 1; }
  else { return 0;}
}

/* GP clock gating deve essere fatto all'interno di una critical section */
/* invece exit critical viene chiamata prima di TMR_ClockGate */
/* true means the timer gets clocked */
#pragma inline 
void  TMR_ClockGate(uint8_t hwCntrlId, bool_t gate)
{
    uint32_t scg1Mask;    
    scg1Mask = TimerClockGate[hwCntrlId];
    if ( TRUE == gate )
    {
        LPC_SYSCON->SYSAHBCLKCTRL |= scg1Mask;    
    }
    else
    {
        LPC_SYSCON->SYSAHBCLKCTRL &= (~scg1Mask);            
    }
}


/* prescale register PR */ 
#define _PR_DEFAULT 0x0 /* Prescaler to the M.C.U. Reset Value */
#define TMR_SetPrescaler(tmr_ptr, prescaler) {  tmr_ptr -> PR = (uint32_t) prescaler; }
#define TMR_SetPrescalerDfltValue(tmr_ptr) { tmr_ptr -> PR = _PR_DEFAULT; }

/* the LPC1227 does support only up-counting so this is not applicable */
#define TMR_SetCountDir(module, direction) 
#define TMR_SetCountDirDfltValue(module)                           
                          
/* this function configures the modulo value for the specific channel */
/* it is being called by both OCHWCNTR_InitSync and by OCPWM_SetPeriodSync */
/* the modulo period must be consistent in the two configurations */
/* in case the configuration is being changed by the PWM setup, it will overwrite the period settings */
/* within the hardware timer configuration done by OCHWCNTR_InitSync */
#pragma inline 
void TMR_SetModulo(tmrcntreg_struct_t* tmr_ptr, Period_ValueType period, uint8_t clrSrcId)
{
  uint8_t timerId;
  
  /* when called from PWM routine OCPWM_SetPeriodSync clrSrcId is the PWM channel id */
  /* check if this channel is setup in PWM mode, if yes get the associated modulo channel, as we are being called */
  /* by OCPWM_SetPeriodSync. The check should return true since the channel is a PWM channel */
  if(isChannelPwm(clrSrcId))
  {
    clrSrcId = _getModuloChannel(_getTimerIdFromAddress(tmr_ptr));
  }
  /* otherwise we are within the OCHWCNTR_InitSync call so we use clrSrc Id coming from the timer cfg */
  /* at this point this channel for the timer is not yet configured as PWM mode so isChannelPwm will fail */
  /* clrSrcId is the channel being used for modulo operations, which is configurable but unique per timer */
 
  tmr_ptr->MCR |= _getModuloChMask(clrSrcId); 
  
  switch(clrSrcId & _LPC_CHID_MASK) {
    case 0:
      tmr_ptr->MR0 = period;
      break;
    case 1:
      tmr_ptr->MR1 = period;
      break;
    case 2:
      tmr_ptr->MR2 = period;
      break;
    case 3:
      tmr_ptr->MR3 = period;
      break;  
  };
  
  /* get base channel register type from LPCbase address for timer */
  timerId = _getTimerIdFromAddress(tmr_ptr);

  /* used as utility function in case PWM mode is needed */
  /* track the modulo in case we are configuring the HW timer, from OCHWCNTR_InitSync  */
  /* when called from OCPWM_SetPeriodSync this will have already been set as PWM */
  /* by the PWM channel setup, so the following should be skipped */  
  if(!isChannelPwm(clrSrcId)) _trackModuloChannel(timerId, clrSrcId);  
}

/* Set Timer Counter Modulo Reg. to the default value */
#pragma inline 
void TMR_SetModuloDfltValue(tmrcntreg_struct_t* tmr_ptr, uint8_t clrSrcId)
{ 
  uint8_t timerId;

  // set the match register back to default value
  switch(clrSrcId & _LPC_CHID_MASK) {
  case 0:
    tmr_ptr->MR0 = _MR_DEFAULT;
    break;
  case 1:
    tmr_ptr->MR1 = _MR_DEFAULT;
  break;
  case 2:
    tmr_ptr->MR2 = _MR_DEFAULT;
  break;
  case 3:
    tmr_ptr->MR3 = _MR_DEFAULT;
  break;
  }
  
  /* since the complete timer is reset to default, reset as well EMR to its preset value */
  tmr_ptr->EMR = _EMR_PRESET;

  /* get base channel register type from LPCbase address for timer */
  timerId = _getTimerIdFromAddress(tmr_ptr);

  /* clear the modulo channel flag for the timer */  
  _resetModuloChannelDefault((uint8_t)timerId); 
  
}

/* retrieve the modulo value from the used channel */
/* called only by PWM functions, so get the modulo channel straightforward */
/* parameter channel is the PWM channel, but modulo is another channel on the HW timer */
/* the modulo channel has been configured within OCHWCNTR_InitSync */
#pragma inline 
Period_ValueType TMR_GetModulo(tmrcntreg_struct_t* tmr_ptr, uint8_t channel)
{ 
  /* retrieve which channel is used for modulo on the timer */
  uint8_t moduloChannel = _getModuloChannel(_getTimerIdFromAddress(tmr_ptr));  
  (void) channel;
  
  switch(moduloChannel & _LPC_CHID_MASK) {
  case 0:
    return(tmr_ptr->MR0);
  case 1:
    return(tmr_ptr->MR1);
  case 2:
    return(tmr_ptr->MR2);
  case 3:
    return(tmr_ptr->MR3);
  }
  return(0); // should never get here
}

/* internal function for configuring the match value register */
#define _setMatchValue(pTmr_ch, value) *pTmr_ch = (_HAL_RegType) value


/* configure the counter register value */
#define TMR_SetCounter(module, value) {module -> TC = value;}

/* Set Timer Counter to the default value */
#define TMR_SetCounterDfltValue(module) { module -> TC = _TC_DEFAULT;}

/* return the counter value */
#define TMR_GetCounter(module) (module -> TC)

/* this function sets the clock source and starts also the timer */
/* now program the CTCR reg */\
/* finally start the timer, enable on & reset off */\
#define TMR_SetClockSource(tmr_ptr, source) {\
  tmr_ptr->CTCR |= _getCtcrMask(tmr_ptr, source);\
  tmr_ptr->TCR = (_TCR_CEN & (~(_TCR_CRST)));\
}
//#define TMR_SetClockSource(tmrcntreg_struct_t *tmr_ptr, uint8_t source) {\
//  pTmr_cnt->CTCR |= _getCtcrMask(tmr_ptr, source);\
//  pTmr_cnt->TCR = (_TCR_CEN & (~(_TCR_CRST)));\
//}

/* stops the timer via the enable bit in TCR */
/* this function sets the clock source to default for the counter */
/* stop the timer in TCR */
/* set CTM in CTCR back in timer mode */
/* set to default value 0 */
/* do not reset the counters in TCR, nobody asked */
/* tmrPtr->TCR = _TCR_CRST; */
#define TMR_StopClockSource(tmrPtr) {\
    tmrPtr->TCR = _TCR_DEFAULT; \
    tmrPtr->CTCR = (_CTCR_DEFAULT & _CTM_TIMER_MODE);\
}

/* this function enables the modulo channel interrupt */
/* there is no dedicated overflow register so one of the match channels is used */
#define TMR_EnableTI(pTmr_cnt, channel) (pTmr_cnt->MCR |= _createMcrIntEnChMask(channel))
//#pragma inline 
//void TMR_EnableTI(tmrcntreg_struct_t* pTmr_cnt, IO_ChannelType channel) { 
//  pTmr_cnt->MCR |= _getMcrIntEnChMask(channel);
//}
                          
/* this function is used to disable the overflow interrupt */
/* there is no dedicated overflow register so one of the match channels is used */
#define TMR_DisableTI(pTmr_cnt, channel) (pTmr_cnt->MCR &= _createMcrIntDisChMask(channel))
//#pragma inline 
//void TMR_DisableTI(tmrcntreg_struct_t* pTmr_cnt, IO_ChannelType channel) {
//  pTmr_cnt->MCR &= _createMcrIntClearChMask(channel);
//}

/* this function clears the interrupt flag for the modulo channel */  
/* there is no dedicated overflow register so one of the match channels is used */
/* clear the bit in the IR by writing with appropriate mask */
/* write a one into the appropriate bit (zero has no effect) */
#define TMR_ClearTOF(pTmr_cnt, channel) pTmr_cnt->IR = _getMatchIntChMask(channel)

/* #pragma inline 
void TMR_ClearTOF(tmrcntreg_struct_t* pTmr_cnt, uint8_t channel) {
  pTmr_cnt->IR = _createMcrIntEnChMask(channel);
}                          
*/                       

#pragma inline 
bool_t TMR_GetTOF(tmrcntreg_struct_t* pTmr_cnt, uint8_t channel)
{ 
  /* get the bit in the IR with appropriate mask */
  return (((pTmr_cnt->IR) & _getMatchIntChMask(channel)) ? TRUE : FALSE);    
}

/* Set a channel in Input Capture Mode */
/* On LPC there are no config bits to select output compare mode so do nothing */
#pragma inline 
void TMR_SetChannelInputChapture(tmrchreg_struct_t *pChannel)
{
  uint32_t channelId, timerId;

  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pChannel);
  channelId = _getLpcChannelIdFromAddress(pChannel); 
  _trackChannelInputCapture(channelId + (timerId*NUM_HW_CNTR));
}
                                 

#define TMR_SetIniPinState(invertion)

/* Set a channel in Output Compare Mode (Software compare only) */
/* On LPC there are no config bits to select output compare mode so do nothing */
#pragma inline 
void TMR_SetChannelOutputCompare(tmrchreg_struct_t *pChannel)
{
  uint32_t channelId, timerId;

  /* just track the channel type for internal usage */
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pChannel);
  channelId = _getLpcChannelIdFromAddress(pChannel); 
  _trackChannelOutputCompare(channelId + (timerId*NUM_HW_CNTR));
}
                          
#pragma inline 
void TMR_SetChannelPWM(tmrchreg_struct_t *pChannel)
{
  uint8_t channelId, timerId;
  LPC_TMR_TypeDef * timerAddress;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pChannel);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];

  // get the channel id, this is the LPC id not the driver id
  channelId = _getLpcChannelIdFromAddress(pChannel);

  // now configure the channel in PWM mode
  timerAddress->PWMC |= _createPwmEnMask(channelId);  

  /* need to configure also the modulo channel for the timer in PWM mode */
  /* modulo channel was selected during OCHWCNTR_InitSync > TMR_SetModulo */
  timerAddress->PWMC |= _createPwmEnMask(_getModuloChannel(timerId));    

  /* mark the channel as PWM type */
  _trackChannelPwm(channelId + (timerId*NUM_HW_CNTR));
}  


inline uint8_t TMR_GetChannelEdgeMode(tmrchreg_struct_t *pChannel)
{
	uint8_t res=0;      /* unknown */
    (void) pChannel;
/* TODO implement*/

	return res;
}

inline void TMR_ToggleChannelEdgeMode(tmrchreg_struct_t *pChannel)
{
  (void) pChannel;
/* TODO implement*/
}
                             
/* this function defines the match point for the PWM. When the counter reaches value */
/* the outputs are switched high. Since the tOn type is given, the match value needs to  */
/* be calculated by subtracting this period from the total modulo for the counter */                             
#pragma inline 
void TMR_SetTimeOn(tmrchreg_struct_t *pTmr_ch, uint32_t tOn)
{
//  uint32_t channelId; 
  uint32_t timerId;
  LPC_TMR_TypeDef* timerAddress;
//  _HAL_RegType ccrMask = _CCR_DEFAULT;
  uint32_t modulo;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];

  // get the channel id, this is the LPC id not the driver id
  //channelId = _getLpcChannelIdFromAddress(pTmr_ch);

  /* retrieve the modulo value for the counter */
  modulo = TMR_GetModulo(timerAddress, _getModuloChannel(timerId));
  
  /* set the match value for the PWM waveform to determine On time */
  _setMatchValue(pTmr_ch,(modulo - tOn));
}

/* LPC1227 does not support custom waveforms in PWM mode */
/* the resulting waveform in each period starts at logic level low */
/* when the timer match occurs it switches to high until the end of the period */
/* at the end of a period (modulo) all outputs are reset to low again */
/* this function is left empty since the PWM mode is already configured in TMR_SetChannelPWM() */                             
#define TMR_SetChannelPulses(channel, inverted) 
                             
         
/* this function configures the event type which is responsible for the capture */          
#pragma inline
void TMR_SetChannelEdgeMode(tmrchreg_struct_t *pChannel, uint8_t mode, bool_t inversion)
{
  uint8_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  _HAL_RegType ccrMask = _CCR_DEFAULT;
  _HAL_RegType ccrClrMask = ~(_CCR_DEFAULT);  
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pChannel);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];

  // get the channel id, this is the LPC id not the driver id
  channelId = _getLpcChannelIdFromAddress(pChannel);         

  // configure which input line is going to be used for the channel in CCR
  switch(channelId) {

    case 0: 
      ccrClrMask &= ~(_CAP0_RE | _CAP0_FE);
      switch(mode)
      {
        /* Rising Edge */
        case TMR_CH_IC_POS_EDGE:
          if (inversion) { ccrMask |= _CAP0_FE; } /* Switch to falling edge */
          else { ccrMask |= _CAP0_RE; };
          break;
        /* Falling Edge */
        case TMR_CH_IC_NEG_EDGE:
          if (inversion) { ccrMask |= _CAP0_RE; } /* Switch to rising edge */                                  
          else { ccrMask |= _CAP0_FE; };
          break;       
        case TMR_CH_IC_BOTH_EDGE:
           /* inversion is meaningless */
           ccrMask |= (_CAP0_RE | _CAP0_FE);
            break;
        default:
            break;
      }    
    break;
  
    case 1: 
      ccrClrMask &= ~(_CAP1_RE | _CAP1_FE);
      switch(mode)
      {
        /* Rising Edge */
        case TMR_CH_IC_POS_EDGE:
          if (inversion) { ccrMask |= _CAP1_FE; } /* Switch to falling edge */
          else { ccrMask |= _CAP1_RE; };
          break;
        /* Falling Edge */
        case TMR_CH_IC_NEG_EDGE:
          if (inversion) { ccrMask |= _CAP1_RE; } /* Switch to rising edge */                                  
          else { ccrMask |= _CAP1_FE; };
          break;       
        case TMR_CH_IC_BOTH_EDGE:
           /* inversion is meaningless */
           ccrMask |= (_CAP1_RE | _CAP1_FE);
            break;
        default:
            break;
      }    
    break;

    case 2: 
      ccrClrMask &= ~(_CAP2_RE | _CAP2_FE);
      switch(mode)
      {
        /* Rising Edge */
        case TMR_CH_IC_POS_EDGE:
          if (inversion) { ccrMask |= _CAP2_FE; } /* Switch to falling edge */
          else { ccrMask |= _CAP2_RE; };
          break;
        /* Falling Edge */
        case TMR_CH_IC_NEG_EDGE:
          if (inversion) { ccrMask |= _CAP2_RE; } /* Switch to rising edge */                                  
          else { ccrMask |= _CAP2_FE; };
          break;       
        case TMR_CH_IC_BOTH_EDGE:
           /* inversion is meaningless */
           ccrMask |= (_CAP2_RE | _CAP2_FE);
            break;
        default:
            break;
      }    
    break;    
  
    case 3: 
      ccrClrMask &= ~(_CAP3_RE | _CAP3_FE);
      switch(mode)
      {
        /* Rising Edge */
        case TMR_CH_IC_POS_EDGE:
          if (inversion) { ccrMask |= _CAP3_FE; } /* Switch to falling edge */
          else { ccrMask |= _CAP3_RE; };
          break;
        /* Falling Edge */
        case TMR_CH_IC_NEG_EDGE:
          if (inversion) { ccrMask |= _CAP3_RE; } /* Switch to rising edge */                                  
          else { ccrMask |= _CAP3_FE; };
          break;       
        case TMR_CH_IC_BOTH_EDGE:
           /* inversion is meaningless */
           ccrMask |= (_CAP3_RE | _CAP3_FE);
            break;
        default:
            break;
      }    
    break;    
  }  
 
  timerAddress->CCR &= ccrClrMask;
  timerAddress->CCR |= ccrMask;
}


                          
//#pragma inline void _setMatchValue(tmrchreg_struct_t *pTmr_ch, value)
//{
//  /* the pointer points already to the match register */
//  *pTmrCh = (_HAL_RegType) value;                    
//}


/* match register and related capture register are offset by 5 words */
/* #define _HAL_MATCH_CAPTURE_OFFSET (5*sizeof(uint32_t))                 */
#define _HAL_MATCH_CAPTURE_OFFSET 5

/* this function clears a captured value */                          
#pragma inline
static void _clearCaptureValue(tmrchreg_struct_t *pTmr_ch, _HAL_RegType value)
{
  /* clear the associated capture register using the match reg offset */
  /* 'dirty trick' using the match register offset which is fixed at 5 words */
  *(pTmr_ch + _HAL_MATCH_CAPTURE_OFFSET) = (_HAL_RegType) value;                    
}
                          
/* this function will return the captured value from the register */
#pragma inline 
_HAL_RegType TMR_GetValue(tmrchreg_struct_t *pTmr_ch)
{
   uint8_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  uint32_t modulo, tOff;
    
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);       

  if(isChannelInputCapture(channelId + (timerId*NUM_HW_CNTR)))
  { 
    /* 'dirty trick' using the match register offset which is fixed at 5 words */
    return(*((_HAL_RegType *)pTmr_ch + _HAL_MATCH_CAPTURE_OFFSET));                          
  }
  else if(isChannelPwm(channelId + (timerId*NUM_HW_CNTR)))
  {
    /* get the modulo value for the timer */
    modulo = TMR_GetModulo(timerAddress, _getModuloChannel(timerId));

    /* the match value for the timer is read for the channel */
    tOff = TMR_GetModulo(timerAddress, channelId);
    
    /* return the value as period - match since on time is required */
    return(modulo - tOff);
  }
  
  return(0); /* should never get here */
}

/* this function enables the interrupt for the channel */
#pragma inline 
void TMR_EnableCHI(tmrchreg_struct_t *pTmr_ch)
{
   uint32_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;

  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);       

  if(isChannelOutputCompare(channelId + (timerId*NUM_HW_CNTR)))
  {  
    // set the bit in the MCR with appropriate mask
    timerAddress->MCR |= _createMcrIntEnChMask(channelId);
  } 
  else
  {
    // set the bit in the CCR with appropriate mask
    timerAddress->CCR |= _createCcrIntEnChMask(channelId);  
  }

  /* Enable nested interrupt  */
  NVIC_EnableIRQ((IRQn_Type)(TIMER_16_0_IRQn + timerId)); 
    
}

/* this function disables the channel interrupt in both capture and match mode */
#pragma inline 
void TMR_DisableCHI(tmrchreg_struct_t *pTmr_ch)
{
  uint32_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);         
    
  if(isChannelOutputCompare(channelId + (timerId*NUM_HW_CNTR)))
  {
    // clear the bit in the MCR with appropriate mask
    timerAddress->MCR &= _createMcrIntDisChMask(channelId);
  }
  else 
  {
    // clear the bit in the CCR with appropriate mask
    timerAddress->CCR &= _createCcrIntDisChMask(channelId);  
  }  

  /* Disable nested interrupt  */
  NVIC_DisableIRQ((IRQn_Type)(TIMER_16_0_IRQn + timerId)); 
}

#pragma inline 
bool_t TMR_ReadCHF(tmrchreg_struct_t *pTmr_ch)
{
  uint32_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);         

  if(isChannelInputCapture(channelId + (timerId*NUM_HW_CNTR)))
  {
    return((timerAddress->IR & _getCaptureIntChMask(channelId))? TRUE: FALSE);
  }
  else
  {
    return((timerAddress->IR & _getMatchIntChMask(channelId))? TRUE: FALSE);
  }
  
}
 

/* this function clears the channel interrupt flag in both capture and match mode */
#pragma inline 
void TMR_ClearCHF(tmrchreg_struct_t *pTmr_ch)
{
  uint32_t channelId, timerId;
  LPC_TMR_TypeDef* timerAddress;
  
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_ch);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pTmr_ch);         

  // clear the flag within the interrupt register 
  // writing a one, zeros have no effect  
  if(isChannelInputCapture(channelId + (timerId*NUM_HW_CNTR)))
  {
    // clear the flag also for the capture channel
    timerAddress->IR = _getCaptureIntChMask(channelId);   
  }
  else
  {
    timerAddress->IR = _getMatchIntChMask(channelId);
  }
}

/* this fucntion programs the match value */                          
#define TMR_SetThreshold1(channel, value) _setMatchValue(channel, value)
                          
/* This M.C.U. doesn't support Threshold2 config. */
#pragma inline 
bool_t Threshold2SetUpUnsupported(uint8_t channel) 
{ 
    (void) channel;
    return(TRUE); 
}
#define TMR_SetThreshold2(pTmr_ch, value) \
  (void) pTmr_ch;


/* this function checkes if Output Compare Mode is supported */
/* from driver: mode - 4 no edge event, 5 toggle on T1, 6 toggle on T2, 7 set high, 8 set low */
/* 'no edge' means no change */
#pragma inline 
bool_t OutputCompareModeUnsupported(uint8_t mode, bool_t inversion)
{
    /* T2 is not supported */
    /* mode 5 (toggle) and inversion does not make sense */  
    if ((6 == mode) || (5 == mode) && (1 == inversion))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* this function configures the output mode for the match event */
/* this is configured in the EMR register */
/* warning: the EMR should be written only during initialization !!!! */
/* changing configuration dynamically should be avoided */
/* there are side effects on EM0:EM3 at runtime doing a read / modify / write access */
/* _EMR_PRESET defines the default values for the pins before starting the timer */ 
#pragma inline
void TMR_SetChannelOutputMode(tmrchreg_struct_t *pChannel, uint8_t mode, bool_t inversion)
{
    
  uint32_t channelId, timerId, emrSetMask;
  LPC_TMR_TypeDef* timerAddress;
  
  emrSetMask = _EMR_PRESET;
    
  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pChannel);
  timerAddress = (LPC_TMR_TypeDef *) TmrCntMap[timerId];
  
  // get the channel id
  channelId = _getLpcChannelIdFromAddress(pChannel);  

  switch(mode) {        
        case 4:
            /* Software compare only */
            switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_NOCHANGE(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_NOCHANGE(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_NOCHANGE(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_NOCHANGE(3); 
              break; 
            }
        break;
        case 5:
            /* toggle pin */
            switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_TOGGLE(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_TOGGLE(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_TOGGLE(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_TOGGLE(3); 
              break; 
            }
        break;
        case 7:
            /* Set output on compare */
            if (inversion)
            {
              switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_CLR(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_CLR(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_CLR(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_CLR(3); 
              break;
              };
            }
            else
            {
              switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_SET(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_SET(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_SET(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_SET(3); 
              break; 
              }
            };
        break;
        case 8:
            /* Clear output on compare */
            if (inversion)
            {
              switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_SET(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_SET(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_SET(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_SET(3); 
              break;
              };
            }
            else
            {
              switch(channelId & _LPC_CHID_MASK) {
              case 0:
                emrSetMask |= _EMR_EMC_CLR(0); 
              break;
              case 1:
                emrSetMask |= _EMR_EMC_CLR(1); 
              break;
              case 2:
                emrSetMask |= _EMR_EMC_CLR(2); 
              break;
              case 3:
                emrSetMask |= _EMR_EMC_CLR(3); 
              break;
              };
            }

        break;
    }

    timerAddress -> EMR |= emrSetMask;
    
}

/* this function is used to bring the channel to its original state */
#pragma inline
void TMR_DeInitChannel(tmrchreg_struct_t *pChannel)
{
    // disable channel interrupt
    TMR_DisableCHI(pChannel);
    
    // set default mode 
    _SetChannelDefaultMode(pChannel);
    
    // reset value for output compare, clear capture value
    _setMatchValue(pChannel, 0);
    _clearCaptureValue(pChannel, (_HAL_RegType) 0);
    
    // clear channel flag
    TMR_ClearCHF(pChannel);         
}
         

#pragma inline 
static Period_ValueType TMR_GetDelta(tmrcntreg_struct_t *pTmr_cnt, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR)
{
  uint32_t timerId;

  // get base channel register type from LPCbase address for timer x
  timerId = _getTimerIdFromAddress((tmrcntreg_struct_t*) pTmr_cnt);

  if((timerId == 0) || (timerId == 1)) 
  { 
    return ((Period_ValueType) ((uint16_t) CntrVal_CURR - (uint16_t) CntrVal_OLD ));  
  }
  else 
  { 
    return ((Period_ValueType) (CntrVal_CURR - CntrVal_OLD));  
  };
}

/*
Starting from portAddress and pinMask (read pin) search for TMR channel
*/
inline bool_t TMR_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pTMRch)
{
	uint8_t j;
	bool_t channelfound = FALSE;


	for (j = 0; j < NUM_HW_CNTR_CH; j++)
	{
		if (TmrChToPinMap[j].PortAddress == portAddress.basic)
		{
			if (TmrChToPinMap[j].PinMask == pinMask)
			{
				channelfound = TRUE;
				break;
			}
		}
	}

	if (channelfound)
	{
		*pTMRch = j;
	}

	return channelfound;
}

#endif /* #if defined(HAL_TIMER_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END TMR Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN RTC Class HAL Support                                   */
/* ********************************************************************************************** */
//MS PROVA con sorgente interna, 
#define CLK_SRC_INT
#define RTC_SCALE_OUT(x) (x/51624)
#define RTC_SCALE_IN(x)  (x*51624)

inline void HAL_RTC_Init( void )
{
#ifdef  CLK_SRC_INT    
    LPC_PMU->SYSCFG |= 0x4 << 11 ; // seleziona la sorgente interna
    LPC_SYSCON->RTCCLKDIV = 0xFF; // non dovrebbe servire con sorgente esterna
#else    
    LPC_PMU->SYSCFG &= ~(0x0F << 11) ; // seleziona la sorgente delayed 1 Hz default
    //LPC_PMU->SYSCFG |= 0xA << 11 ; // seleziona la sorgente delayed 1 kHz
    //LPC_SYSCON->RTCCLKDIV  = 0x0; // non dovrebbe servire con sorgente esterna
#endif    

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 19); // Enables clock for RTC.
    LPC_RTC->CR = 0x1;  // Enable RTC
}

inline void HAL_RTC_SetTime( uint32_t time ){
  
#ifdef  CLK_SRC_INT  
  LPC_RTC->LR = RTC_SCALE_IN(time);   
#else
  LPC_RTC->LR = time;  
#endif
}

inline uint32_t HAL_RTC_GetTime( void ){
#ifdef  CLK_SRC_INT  
  return RTC_SCALE_OUT(LPC_RTC->DR); 
#else
  return LPC_RTC->DR;  
#endif
}

inline void HAL_RTC_SetAlarm( uint32_t time ){
#ifdef  CLK_SRC_INT    
  LPC_RTC->MR = RTC_SCALE_IN(time);
#else
  LPC_RTC->MR = time;
#endif
}
inline uint32_t HAL_RTC_GetAlarm( void ){
#ifdef  CLK_SRC_INT  
  return RTC_SCALE_OUT(LPC_RTC->MR);
#else  
  return LPC_RTC->MR;
#endif
}
inline void HAL_RTC_EnableInterrupt( void ){
  NVIC_EnableIRQ( RTC_IRQn ); 
  LPC_RTC->IMSC = 0x1;  
}
inline void HAL_RTC_DisableInterrupt( void ){
  NVIC_DisableIRQ( RTC_IRQn );
  LPC_RTC->IMSC = 0x0;  
}

inline void HAL_RTC_ClearInterrupt( void ){
  LPC_RTC->ICR = 0x1; // clear interrupt
}

inline bool_t HAL_RTC_IsAlarmExpired( void ){
  if( LPC_RTC->DR >= LPC_RTC->MR )
    return TRUE;
  return FALSE;
}

inline void HAL_RTC_isr(void){
  HAL_RTC_ClearInterrupt();
}

/* ********************************************************************************************** */
/*                                  END RTC Class HAL Support                                     */
/* ********************************************************************************************** */



/* END OF FILE */
#endif /* __HAL_NXP_M0_LPC1229_H */

/* ********************************************************************************************** */
/* END OF LPC1227 HAL																			  */
/* ********************************************************************************************** */


/*@}*/

/* *********** */
/* End of file */
/* *********** */
