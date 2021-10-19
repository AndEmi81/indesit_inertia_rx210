/**
\defgroup HAL_FS_HCS08_GB60  Freescale HCS08 GB60 HAL
\ingroup FS_HCS08_GB60
*/

/**
\defgroup HAL_FS_HCS08_GB60_CLS  Freescale HCS08 GB60 supported drivers Classes

This module lists the driver classes supported by this HAL.


\date    07/05/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_HCS08_GB60
*/

/**
\defgroup HAL_FS_HCS08_GB60_HISTORY  Freescale HCS08 GB60 History
\ingroup HAL_FS_HCS08_GB60
This module contains the history

\defgroup HAL_FS_HCS08_GB60_HISTORY_HAL  Freescale HCS08 GB60 HAL History
\ingroup  HAL_FS_HCS08_GB60_HISTORY
*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    FS_HCS08_GB60_hal.h
\ingroup HAL_FS_HCS08_GB60
\date    12/03/2009
\author  Roberto Fioravanti
\version 00.00.01


\addtogroup HAL_FS_HCS08_GB60_HISTORY_HAL
@{
\n 		00.00.33
\n		11/03/2010 
\n		Stefano Ughi
\n		changes
\n 		- added Watchdog HAL
\n		- added Flash HAL
\n		- added ADC HAL
\n		- added Clock HAL
\n
\n 		00.00.27
\n		27/01/2010 
\n		Roberto Fioravanti
\n		changes
\n 		- added I2C_Level3BlockMonitorSync()	I2C_Level4BlockMonitorSync() I2C_Level5BlockMonitorSync()
\n		- added I2C_NestedIsr_Begin() I2C_NestedIsr_End() as empty because nested isr not supported
\n
\n 		00.00.14
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- Pin Port defines for 'ModuleIOMap' table (to be defined in config.c file) added.
\n
\n 		00.00.13
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n    -	Alignment with Cefriel version include lots of HAL functions for GPIO management.
\n
\n 		00.00.01 
\n		13/05/2009 
\n		Roberto Fioravanti
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
\addtogroup HAL_FS_HCS08_GB60_CLS
@{*/



#ifndef __HAL_FREESCALE_HCS08_GB60_H
#define __HAL_FREESCALE_HCS08_GB60_H





/* ******** */
/* Includes */
/* ******** */
#include "Config.h"                               /* For: Application configuration */
#include "FS_HCS08_GB60_reg.h"

/* ****** */
/* Errors */
/* ****** */

/* Dependancy rule: it is not possible to include this file direcly but only trought a driver. */
#if !defined(__DRIVER_H)
#error "This is a private header file -- do not include directly."
#endif

#pragma MESSAGE DISABLE C5703
#pragma MESSAGE DISABLE C5702
#pragma MESSAGE DISABLE C5917
#pragma MESSAGE DISABLE C5902
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* I2C Class HAL Support */
/* ********************* */

/** \defgroup HAL_FS_HCS08_GB60_I2C  I2C Class driver support    
 */


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR                        (0)
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK                (1)
#define OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK                               (1)


/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY

/* 
I2C related registers redefinition                           
This approach helps when generating new files for additional parts of the same family.
*/
/*HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION> */
#define _HAL_S08_I2C_SAR_REG                        (IIC1A)            /* */
#define _HAL_S08_I2C_CTRL_REG                       (IIC1C)            /* */
#define _HAL_S08_I2C_ENABLE_REG                     (IIC1C_IIC1EN)     /* */
#define _HAL_S08_I2C_TX_CONTROL_REG                 (IIC1C_TX)         /* */
#define _HAL_S08_I2C_STARTSTOP_REG                  (IIC1C_MST)        /* */
#define _HAL_S08_I2C_RESTART_REG                    (IIC1C_RSTA)       /* */
#define _HAL_S08_I2C_TXAK_EN_REG                    (IIC1C_TXAK)       /* */
#define _HAL_S08_I2C_STATUS_REG                     (IIC1S)            /* */
#define _HAL_S08_I2C_BUSBUSY_REG                    (IIC1S_BUSY)       /* */
#define _HAL_S08_I2C_AL_REG                         (IIC1S_ARBL)       /* */
#define _HAL_S08_I2C_ADDRMATCH_REG                  (IIC1S_IAAS)       /* */
#define _HAL_S08_I2C_SLAVE_RW_REG                   (IIC1S_SRW)        /* */
#define _HAL_S08_I2C_INT_REG                        (IIC1C_IIC1IE)     /* */
#define _HAL_S08_I2C_RXACKBITVAL_REG                (IIC1S_RXAK)       /* */
#define _HAL_S08_I2C_TXACKBITVAL_REG                (IIC1C_TXAK)       /* */
#define _HAL_S08_I2C_INT_REQ_FLAG_REG               (IIC1S_IIC1IF)     /* */
#define _HAL_S08_I2C_TX_DATA_REG                    (IIC1D)            /* */
#define _HAL_S08_I2C_RX_DATA_REG                    (IIC1D)            /* */
#define _HAL_S08_I2C_INT_REQ_REG                    (IIC1S_IIC1IF)     /* */
#define _HAL_S08_I2C_MULT_REG                       (IIC1F_MULT)       /* */
#define _HAL_S08_I2C_ICR_REG                        (IIC1F)            /* */
#define _HAL_S08_I2C_SDA_IO_REG                     (PTCD_PTCD2)       /* */
#define _HAL_S08_I2C_SCL_IO_REG                     (PTCD_PTCD3)       /* */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/** @name I2C
 *
 */
/*@{*/

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


/**
Reset lines SDA e SCL
*/
#define     I2C_Reset()   {   }
/*@}*/



/** @name Data registers
 *
 */
/*@{*/
#define     I2C_ReadData() _HAL_S08_I2C_RX_DATA_REG
/* */
#define     I2C_WriteData(b) {   _HAL_S08_I2C_TX_DATA_REG = (I2C_DataRegType)(b); }
#define     I2C_SendAddr(b)   I2C_WriteData(b)
/* */                    
#define     I2C_pReceiveDataRegister() (&_HAL_S08_I2C_RX_DATA_REG)
/* */
#define     I2C_pTransmitDataRegister() (&_HAL_S08_I2C_TX_DATA_REG)
/*@}*/


/** @name Configuration
 *
 */
/*@{*/
/* */
#define     I2C_SetClockRate(b)     {   _HAL_S08_I2C_ICR_REG = (I2C_ClockRateType)(b); } 
/**/
#define     I2C_ResetClockRate(b)	I2C_SetClockRate(b)
/* */
#define     I2C_SetSlaveAddress(b)  {   _HAL_S08_I2C_SAR_REG = (I2C_SlaveAddressType)(b); } 

#define     I2C_MasterEnable()  { }

/* */
#define     I2C_MasterDisable() { }



/* */
#define     I2C_SetTxMode()    {    _HAL_S08_I2C_TX_CONTROL_REG = 1; }

/* */
#define     I2C_SetRxMode()   {    _HAL_S08_I2C_TX_CONTROL_REG = 0; }

/* */
#define     I2C_IsTxMode()     (_HAL_S08_I2C_TX_CONTROL_REG!=0x00)
/*@}*/


/** @name Start & Stop condition
 *
 */
/*@{*/
#define     I2C_IssueStartCondition() {    _HAL_S08_I2C_STARTSTOP_REG = 1; }
/* */
#define     I2C_IssueReStartCondition() {   _HAL_S08_I2C_RESTART_REG = 1; }
/* */
#define     I2C_IssueStopCondition()  {   _HAL_S08_I2C_STARTSTOP_REG = 0; }

/*@}*/


/** @name Interrupt
 *
 */
/*@{*/
#define     I2C_IntEnable() { _HAL_S08_I2C_INT_REG = 1; }
/* */

#define     I2C_IntDisable()  { _HAL_S08_I2C_INT_REG = 0; }
/*@}*/


/** @name Status
 *
 */
/*@{*/
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
/*@}*/



/** @name Acknolegde bit
 *
 */
/*@{*/
#define     I2C_ReceivedAcknowledgeVal()   (_HAL_S08_I2C_RXACKBITVAL_REG)
/* */
#define     I2C_EnableAcknowledge() {   _HAL_S08_I2C_TXACKBITVAL_REG = 0; }
/* */
#define     I2C_DisableAcknowledge() {   _HAL_S08_I2C_TXACKBITVAL_REG = 1; }
/*@}*/
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


#define I2C_Level3BlockMonitorSync()				((IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK)) == IIC1C_TX_MASK)
#define I2C_Level4BlockMonitorSync()				((IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK|IIC1C_MST_MASK)) == IIC1C_TXAK_MASK)
#define I2C_Level5BlockMonitorSync()				(IIC1C & (IIC1C_TX_MASK|IIC1C_TXAK_MASK|IIC1C_MST_MASK))
/*@}*/
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

/** \defgroup HAL_FS_HCS08_GB60_SYS  System Class driver support     
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


/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_DIO_SUPPORT)

#define PIN_DUMMY\
{\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y)\
{\
{(DIO_PortAddressBasicType)&PT##x##D},\
(DIO_ModuleMapMaskType)PT##x##D_PT##x##D##y##_MASK\
}

/* sharing between dataInput and dataOutput */
#define dataOutput dataInput
/* dummy declaration */
#define driveStrength dataInput
typedef struct
{
    uint8_t dataInput;
    uint8_t pullup;
    uint8_t slewRate;
    uint8_t direction;
}portreg_struct_t;
#define portreg_ext_struct_t portreg_struct_t





#pragma INLINE
#pragma NO_LOOP_UNROLL
static void DeInitGPIO(void)
{
uint8_t *pByte = &PTADD;
uint8_t i;

    for(i=0;i<5;i++)
    {
        *pByte      = 0x00;     /* Clear Direction Reg. */
        *(pByte-1)  = 0x00;     /* Clear Slew Rate Reg. */
        *(pByte-2)  = 0x00;     /* Clear Pull Up Reg.   */
        pByte +=4;              /* Point to next Direction Reg. */    
    }
}



#define DIO_PULL_UP_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t PullUpIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}
#define DIO_PULL_DOWN_IS_AVAILABLE	FALSE
#pragma INLINE
static bool_t PullDownIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_INPUT_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t InputIsAvaibleOn(portreg_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}
#define DIO_SLEW_RATE_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t SlewRateIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}
#define DIO_DRIVE_STRENGTH	FALSE
#pragma INLINE		
static bool_t DriveStrengthIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_OUTPUT_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t OutputIsAvaibleOn(portreg_struct_t *port,DIO_ModuleMapMaskType bit)
{
	return 1;
}
#define DIO_PASSIVE_FILTER_IS_AVAILABLE  FALSE
#pragma INLINE
static bool_t PassiveFilterIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_INTERRUPT_IS_AVAILABLE    FALSE
#pragma INLINE
static bool_t DIO_InterruptIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#define DIO_DIGITAL_FILTER_IS_AVAILABLE   FALSE
#pragma INLINE
static bool_t DIO_DigitalFilterIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}
#pragma INLINE
static void DIO_SetDigitalFltClkFreq(portreg_ext_struct_t *port,uint16_t filterFrequency)
{
}

#pragma INLINE
static uint16_t DIO_GetDigitalFltFrequency(portreg_ext_struct_t *port)
{
    return 0;
}
#pragma INLINE
static void DIO_SetDigitalFilterNsamples(portreg_ext_struct_t *port,uint8_t nSamples)
{
}
#pragma INLINE
static uint8_t DIO_GetDigitalFilterNsamples(portreg_ext_struct_t *port)
{
    return (uint8_t)(0);    
}
#pragma INLINE
static void DIO_EnableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma INLINE
static void	DIO_DisableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma INLINE
static bool_t DIO_InterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#define DIO_DMA_IS_AVAILABLE  FALSE
#pragma INLINE
static bool_t DIO_DMA_IsAvaibleOn(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#pragma INLINE
static void DIO_EnableDMAOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma INLINE
static void DIO_DisableDMAOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
}
#pragma INLINE
static bool_t DIO_DMA_OnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;   
}
#pragma INLINE
static bool_t DIO_LevelDetectIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;
}
#pragma INLINE
static bool_t DIO_EdgeDetectIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    return 0;    
}
#pragma INLINE
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
/** Define TMR Capability */
#define _HAL_TMR_CAPABILITY

#define TPM1SC_CLKSx_MASK   (TPM1SC_CLKSA_MASK | TPM1SC_CLKSB_MASK)
#define TPM1C0SC_MS0x_MASK  (TPM1C0SC_MS0A_MASK | TPM1C0SC_MS0B_MASK) 
#define TPMxSC_CPWMS_MASK   (TPM1SC_CPWMS_MASK)

/* Hardware Counter Config. Register Addr. */        
static const TMR_CNTAddressType TmrCntMap[NUM_HW_CNTR]={
(TMR_CNTAddressType)&TPM1SC,
(TMR_CNTAddressType)&TPM2SC,
};
/* Timer x Channel n Status and Control Register Addr. */        
static const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH]={
{(TMR_CHAddressType)&TPM1C0SC, 0},
{(TMR_CHAddressType)&TPM1C1SC, 0},
{(TMR_CHAddressType)&TPM1C2SC, 0},
{(TMR_CHAddressType)&TPM2C0SC, 1},
{(TMR_CHAddressType)&TPM2C1SC, 1},
{(TMR_CHAddressType)&TPM2C2SC, 1},
{(TMR_CHAddressType)&TPM2C3SC, 1},
{(TMR_CHAddressType)&TPM2C4SC, 1} 
};


/* Timer counter structure */
typedef struct
{
    uint8_t sc;      /* status and control reg. */
    uint16_t cnt;    /* counter */
    uint16_t mod;    /* modulo */
} tmrcntreg_struct_t;

/* Timer channel structure */
typedef struct
{
    uint8_t sc;     /* Timer x Channel n Status and Control Register*/
    uint16_t v;     /* Timer x Channel Value Registers */
} tmrchreg_struct_t;

#pragma INLINE 
static bool_t CheckClearSourceIdFailed(uint8_t clearingSourceId)
{
    if(TMR_CLEAR_SOURCE_IS_INTERNAL==clearingSourceId)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/* Empty declaration, no clock gate available */
#define TMR_ClockGate(hwCntrlId,gate)

#define TMR_SetPrescaler(module, prescaler)\
{\
module -> sc = (uint8_t)((module -> sc & ~TPM1SC_PS_MASK) | (((prescaler) << TPM1SC_PS_BITNUM) & TPM1SC_PS_MASK));\
}
/* Prescaler to the M.C.U. Reset Value */
#define TMR_SetPrescalerDfltValue(module)\
{\
module -> sc &= (~TPM1SC_PS_MASK);\
}
#define TMR_SetCountDir(module, direction)\
{\
if (direction == 2){\
module -> sc |= TPM1SC_CPWMS_MASK;}\
else{\
module -> sc &= ~TPM1SC_CPWMS_MASK;}\
}
/* Counting Dir. to the M.C.U. Reset Value */
#define TMR_SetCountDirDfltValue(module)\
{\
module -> sc &= ~TPM1SC_CPWMS_MASK;\
}

#define TMR_SetModulo(module, period) {module -> mod = period;}
/* Set Timer Counter Modulo Reg. to the default value */
#define TMR_SetModuloDfltValue(module) {module -> mod = 0;} 

#define TMR_GetModulo(module) (module -> mod)

#define TMR_SetCounter(module, value) {module -> cnt = value;}
/* Set Timer Counter to the default value */
#define TMR_SetCounterDfltValue(module) {module -> cnt = 0;}

#define TMR_GetCounter(module) (module -> cnt)

#define TPM1SC_CLKSx_BITNUM TPM1SC_CLKSA_BITNUM

#define TMR_SetClockSource(module, source)\
{\
module -> sc = (uint8_t)((module -> sc & ~TPM1SC_CLKSx_MASK)\
| (((source + 1) << TPM1SC_CLKSx_BITNUM) & TPM1SC_CLKSx_MASK));\
}
/* Stop Timer's Counter */
#define TMR_StopClockSource(module) {module -> sc &= ~TPM1SC_CLKSx_MASK;}
/* Timer Overfow Interrupt Enable */
#define TMR_EnableTI(module)\
{\
module -> sc |= TPM1SC_TOIE_MASK;\
}
/* Timer Overfow Interrupt Disable */
#define TMR_DisableTI(module) {module -> sc &= ~TPM1SC_TOIE_MASK;}

#define TMR_ReadTOF(module) ((module -> sc & TPM1SC_TOF_MASK) != 0)
/* Clear Timer Overflow Flag */
#define TMR_ClearTOF(module)\
{\
if (module -> sc & TPM1SC_TOF_MASK)\
{\
module -> sc &= ~TPM1SC_TOF_MASK;\
}\
}

#pragma INLINE
static bool_t TMR_GetTOF(tmrcntreg_struct_t *pChannel)
{
    return ((pChannel->sc & TPM1SC_TOF_MASK)?1:0);    
}
/* Set a channel in Input Capture Mode */
#define TMR_SetChannelInputChapture(channel) {channel -> sc &= ~TPM1C0SC_MS0x_MASK;}

#define TMR_SetIniPinState(invertion)
/* Set a channel in Output Compare Mode (Software compare only) */
#define TMR_SetChannelOutputCompare(channel)\
{\
channel -> sc = (uint8_t)((channel -> sc & ~TPM1C0SC_MS0B_MASK) | TPM1C0SC_MS0A_MASK) ;\
}

#define TMR_SetChannelPWM(channel) {channel -> sc |= TPM1C0SC_MS0B_MASK;}
/*
Pin not used for TPM channel; use as an external clock for the TPM or
revert to general-purpose I/O
*/
#define TMR_SetChannelDefaultMode(channel) {channel -> sc &= ~TPM1C0SC_ELS0x_MASK;}

#pragma INLINE 
static void TMR_SetChannelEdgeMode(tmrchreg_struct_t *pChannel,uint8_t mode, bool_t inversion)
{
    pChannel -> sc &= (~TPM1C0SC_ELS0x_MASK);

    switch(mode)
    {
        /* Rising Edge */
        case 1:
            if (inversion)
            {
            /* Switch to falling edge */
                pChannel -> sc |= (TPM1C0SC_ELS0B_MASK);    
            }
            else
            {
                pChannel -> sc |= (TPM1C0SC_ELS0A_MASK);                    
            }
            break;
        /* Falling Edge */
        case 2:
            if (inversion)
            {
            /* Switch to rising edge */
                pChannel -> sc |= (TPM1C0SC_ELS0A_MASK);                                    
            }
            else
            {
                pChannel -> sc |= (TPM1C0SC_ELS0B_MASK);                    
            }        
            break;
        case 3:
            /* inversion is meaningless */
            pChannel -> sc |= (TPM1C0SC_ELS0x_MASK);
            break;
        default:
            break;
    }
}

#define TMR_SetValue(channel, value) {channel -> v = value;}

#define TMR_GetValue(channel) (channel -> v)
/* Channel n interrupt requests enabled */
#define TMR_EnableCHI(channel) {channel -> sc |= TPM1C0SC_CH0IE_MASK;}
/* Channel n interrupt requests disabled */
#define TMR_DisableCHI(channel) {channel -> sc &= ~TPM1C0SC_CH0IE_MASK;}

#define TMR_ReadCHF(channel) ((channel -> sc & TPM1C0SC_CH0F_MASK) != 0)

/* Clear CHnF pending flag */
#define TMR_ClearCHF(channel)\
{\
if (channel -> sc & TPM1C0SC_CH0F_MASK)\
{\
channel -> sc &= ~TPM1C0SC_CH0F_MASK;\
}\
}
/* Set Output Compare Value */
#define TMR_SetThreshold1(channel, value) {channel -> v = value;}

/* This M.C.U. doesn't support Threshold2 config. */
#pragma INLINE
static bool_t Threshold2SetUpUnsupported(uint8_t channel)
{
    return 1;    
}
#define TMR_SetThreshold2(channel, value) {}

/* Check if Output Compare Mode is supported */
#pragma INLINE
static bool_t OutputCompareModeUnsupported(uint8_t mode,bool_t inversion)
{
    if((6==mode)||(5==mode)&&(TRUE==inversion))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#pragma INLINE     
static void TMR_SetChannelOutputMode(tmrchreg_struct_t *pChannel,uint8_t mode, bool_t inversion)
{
    switch(mode)
    {        
        case 4:
            /* Software compare only */
            pChannel -> sc &= (uint8_t)~TPM1C0SC_ELS0x_MASK;
            break;
        case 5:
            /* Toggle output on compare */
            pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM1C0SC_ELS0B_MASK) | TPM1C0SC_ELS0A_MASK);
            break;
        case 7:
            /* Set output on compare */
            if (inversion)
            {
                pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM1C0SC_ELS0A_MASK) | TPM1C0SC_ELS0B_MASK);                
            }
            else
            {
                pChannel -> sc |= (uint8_t)TPM1C0SC_ELS0x_MASK;   
            }
            break;
        case 8:
            /* Clear output on compare */
            if (inversion)
            {
                pChannel -> sc |= (uint8_t)TPM1C0SC_ELS0x_MASK;    
            }
            else
            {
                pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM1C0SC_ELS0A_MASK) | TPM1C0SC_ELS0B_MASK);                   
            }

            break;
    }
}

#define TMR_DeInitChannel(channel)\
{\
TMR_DisableCHI(channel)\
TMR_SetChannelDefaultMode(channel)\
TMR_SetChannelOutputCompare(channel)\
TMR_SetValue(channel,0)\
TMR_SetChannelInputChapture(channel)\
TMR_ClearCHF(channel)\
}

#define TMR_SetTimeOn(channel, value) {channel -> v = value;}

#define TMR_SetChannelPulses(channel, inverted) { \
if (inverted) channel -> sc |= (uint8_t)TPM1C0SC_ELS0A_MASK; \
else channel -> sc = (uint8_t)((channel -> sc & ~TPM1C0SC_ELS0A_MASK) | TPM1C0SC_ELS0B_MASK); \
}

#endif /* #if defined(HAL_TIMER_SUPPORT) */

/* ********************************************************************************************** */
/*                                  END TMR Class HAL Support                                     */
/* ********************************************************************************************** */


/* ********************** */
/* UART Class HAL Support */
/* ********************** */
/* ********************** */
/*           END          */
/* ********************** */
/* Define UART1 Capability */
#define _HAL_UARTx_CAPABILITY
#ifdef HAL_OPTION_UART
#if     HAL_OPTION_UART ==  1 
#define HAL_UART_DEV   1
#elif   HAL_OPTION_UART ==  2
#define HAL_UART_DEV   2
#else
#error "uart not available"
#endif
#endif



/* Baud rates calculations */

#define _HALUART_CALC_BAUD(baud, divisor)   (uint16_t)((uint16_t)(((uint32_t)BUS_FREQUENCY_Hz / ((uint32_t)16.0 * (baud)))))

/** Baud Rates */
#define _HALUART_BAUD_115200               (_HALUART_CALC_BAUD(115200UL,	))
#define _HALUART_BAUD_76800                (_HALUART_CALC_BAUD(76800UL,		))
#define _HALUART_BAUD_57600                (_HALUART_CALC_BAUD(57600UL,		))
#define _HALUART_BAUD_38400                (_HALUART_CALC_BAUD(38400UL,		))
#define _HALUART_BAUD_28800                (_HALUART_CALC_BAUD(28800U,		))
#define _HALUART_BAUD_19200                (_HALUART_CALC_BAUD(19200U,		))
#define _HALUART_BAUD_14400                (_HALUART_CALC_BAUD(14400,			))
#define _HALUART_BAUD_9600                 (_HALUART_CALC_BAUD(9600,			))
#define _HALUART_BAUD_4800                 (_HALUART_CALC_BAUD(4800,			))
#define _HALUART_BAUD_2400                 (_HALUART_CALC_BAUD(2400,			))
#define _HALUART_BAUD_1200                 (_HALUART_CALC_BAUD(1200,			))




#define _HAL_S08_UART1_ERROR_MASK					(SCI1S1_FE_MASK|SCI1S1_PF_MASK|SCI1S1_OR_MASK)

/** \defgroup HAL_HAL_RN_300H_39079_UART  UART Class driver support    
\ingroup HAL_RN_300H_39079_CLS
 
*/

#define _HAL_S08_UART1_BAUDRATE_REG(n)											(SCI1BD)
#define _HAL_S08_UART1_TX_DATAREG(n)                  	    (SCI1D)
#define _HAL_S08_UART1_RX_DATAREG(n)              	        (SCI1D)
#define _HAL_S08_UART1_ENABLE_TX_REG(n)            	    	  (SCI1C2)
#define _HAL_S08_UART1_ENABLE_RX_REG(n)            	    	  (SCI1C2)
#define _HAL_S08_UART1_ENABLE_TX_IRQ_REG(n)            		  (SCI1C2)
#define _HAL_S08_UART1_ENABLE_RX_IRQ_REG(n)            		  (SCI1C2)

//#define _HAL_S08_UARTx_CLK_ENABLE_REG(n)										(SCI3##n.SCR3.BIT.CKE)

#define _HAL_S08_UART1_STATUS_RX_REG(n)            	    	  (SCI1S1)
#define _HAL_S08_UART1_STATUS_TX_REG(n)            	    	  (SCI1S1)
//#define _HAL_S08_UART1_STATUS_TXEND_REG(n)            	    (SCI3.SSR.BIT.TEND)
#define _HAL_S08_UART1_STATUS_ERR_PE_REG(n)            		  (SCI1S1)
#define _HAL_S08_UART1_STATUS_ERR_FER_REG(n)            		(SCI1S1)
#define _HAL_S08_UART1_STATUS_ERR_OER_REG(n)            		(SCI1S1)

#define _HAL_S08_UART1_STATUS_ERR_REG(n)            	    	(SCI1S1)//(_HAL_S08_UARTx_STATUS_ERR_PER_REG(n)|| _HAL_S08_UARTx_STATUS_ERR_FER_REG(n) || _HAL_S08_UARTx_STATUS_ERR_OER_REG(n))
#define _HAL_S08_UART1_FRAME_CFG_REG(n)            	    	(SCI3##n.SMR.BYTE)
//#define _HAL_S08_UART1_TX_PIN                              IO.PMR1.BIT.TXD

#define _HAL_S08_UART1_PARITY_ENABLE_REG(n)            		SCI1C1
#define _HAL_S08_UART1_PARITY_VAL_REG(n)            				SCI1C1
#define _HAL_S08_UART1_STOP_REG(n)            	      			(SCI3##n.SMR.BIT.STOP)
#define _HAL_S08_UART1_MODE_REG(n)            	      			(SCI3##n.SMR.BIT.COM)
#define _HAL_S08_UART1_DATALEN_REG(n)            	      	(SCI1C1)
#define _HAL_S08_UART1_CLOCK_DIVISOR_REG(n)            		(SCI1BD)



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

#define 	UART_ClkEnable(n)		        {}
#define 	UART_ClkDisable(n)		        {}

#define 	  UART_Enable(n)		            /* not used */
#define     UART_TxEnable(n)              { 	SCI1S1; _HAL_S08_UART1_ENABLE_TX_REG(n) |= SCI1C2_TE_MASK; }
#define     UART_RxEnable(n)              { _HAL_S08_UART1_ENABLE_RX_REG(n) |= SCI1C2_RE_MASK; }



#define     UART_TxDisable(n)  	          { /*_HAL_S08_UART1_ENABLE_TX_REG(n) &= ~SCI1C2_TE_MASK;*/ }
#define     UART_RxDisable(n)  	          { _HAL_S08_UART1_ENABLE_RX_REG(n) &= ~SCI1C2_RE_MASK; }
#define 	  UART_Disable(n)		            /* not used */


#define     UART_SetParityVal(n,val)		  { (val)? _HAL_S08_UART1_PARITY_VAL_REG(n) |= SCI1C1_PE_MASK; : _HAL_S08_UART1_PARITY_VAL_REG(n) &= ~SCI1C1_PE_MASK;}
#define     UART_SetParityMode(n,val)		  { if(val) \
                                            _HAL_S08_UART1_PARITY_ENABLE_REG(n) |= SCI1C1_PT_MASK; \
                                            else _HAL_S08_UART1_PARITY_ENABLE_REG(n) &= ~SCI1C1_PT_MASK;}
#define     UART_SetDataLen(n,val)			  { if (val) \
                                            _HAL_S08_UART1_DATALEN_REG(n) |= SCI1C1_M_MASK; \
                                            else _HAL_S08_UART1_DATALEN_REG(n) &= ~SCI1C1_M_MASK;}
#define     UART_SetStop(n,val)					    /* not used */
#define     UART_SetMode(n,val)					    /* not used */
#define     UART_SetClockDivisor(n,val)		{_HAL_S08_UART1_CLOCK_DIVISOR_REG(n)=(val);}

/*@}*/



/*  Data register
 *  
 */

#define     UART_ReadData(n) 		              _HAL_S08_UART1_RX_DATAREG(n);

#define     UART_WriteData(n,b) 	          	{   _HAL_S08_UART1_TX_DATAREG(n) = (UART_DataRegType)(b); }
                  
   
#define     UART_pReceiveDataRegister(n) 	  	(&_HAL_S08_UART1_RX_DATAREG(n))

#define     UART_pTransmitDataRegister(n)   	(&_HAL_S08_UART1_TX_DATAREG(n))

			
/* Configuration
 *  
 */
#define     UART_SetBaudRate(n,b) 	          {   _HAL_S08_UART1_BAUDRATE_REG(n) = (UART_BaudRateType)(b); }

//#define     UART_SetFrameConfig(n,b) 	{   _HAL_S08_UARTx_FRAME_CFG_REG(n) = (UART_FrameCfgType)(b); }



/* Interrupt
 *  
 */
#define     UART_TxIntEnable(n)               { _HAL_S08_UART1_ENABLE_TX_IRQ_REG(n) |= SCI1C2_TIE_MASK; }
#define     UART_RxIntEnable(n)               { _HAL_S08_UART1_ENABLE_RX_IRQ_REG(n) |= SCI1C2_RIE_MASK; }

#define     UART_TxIntDisable(n)              { _HAL_S08_UART1_ENABLE_TX_IRQ_REG(n) &= ~SCI1C2_TIE_MASK; }
#define     UART_RxIntDisable(n)              { _HAL_S08_UART1_ENABLE_RX_IRQ_REG(n) &= ~SCI1C2_RIE_MASK; }



/*  State
 *  
 */
#define     UART_IsPeError(n)        	(_HAL_S08_UART1_STATUS_ERR_PE_REG(n) & SCI1S1_PF_MASK)	/* \todo deve ritornare un valore fisso se non abilitato */
#define     UART_IsFerError(n)       	(_HAL_S08_UART1_STATUS_ERR_FER_REG(n) & SCI1S1_FE_MASK )
#define     UART_IsOerError(n)       	(_HAL_S08_UART1_STATUS_ERR_OER_REG(n) & SCI1S1_OR_MASK)
#define     UART_IsError(n)				    (_HAL_S08_UART1_STATUS_ERR_REG(n) & _HAL_S08_UART1_ERROR_MASK)


#define     UART_ClearError(n)       	{_HAL_S08_UART1_STATUS_ERR_REG(n); _HAL_S08_UART1_RX_DATAREG(n);}
#define 	  UART_ResetPeError(n)		  {_HAL_S08_UART1_STATUS_ERR_PE_REG(n); _HAL_S08_UART1_RX_DATAREG(n);}
#define     UART_ClearRxIrq(n)       	{} /*  */
#define     UART_ClearTxIrq(n)       	{/*_HAL_S08_UART1_STATUS_TXEND_REG(n); _HAL_S08_UARTx_STATUS_TXEND_REG(n)=0;_HAL_S08_UARTx_STATUS_TX_REG(n); _HAL_S08_UARTx_STATUS_TX_REG(n)=0;}//{_HAL_S08_UARTx_STATUS_TX_REG(n); _HAL_S08_UARTx_STATUS_TX_REG(n)=0;*/}

#define     UART_IsTxInt(n)           	(_HAL_S08_UART1_STATUS_TX_REG(n))
#define     UART_IsTxEndInt(n)			(_HAL_S08_UARTx_STATUS_TXEND_REG(n))
#define     UART_IsRxInt(n)           	(_HAL_S08_UART1_STATUS_RX_REG(n))

#define 	UART_IsTxDataRegFull(n)   	(_HAL_S08_UART1_STATUS_TX_REG(n) == 0)





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



/* Map mcu specific registers names to a 'standard' set of names 
              
              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>
              
    (this helps when generating new files for additional parts of the same family).
*/


/** This is the bit position of STOPE bit */
#define _HAL_S08_SYS_SOPT_STOPE_BITPOS		5
/** This is the bit mask of STOPE bit */
#define _HAL_S08_SYS_SOPT_STOPE_MASK   		(1<<_HAL_S08_SYS_SOPT_STOPE_BITPOS)
/** This is the bit position of BKGDPE bit */
#define _HAL_S08_SYS_SOPT_BKGDPE_BITPOS		1
/** This is the bit mask of BKGDPE bit */
#define _HAL_S08_SYS_SOPT_BKGDPE_MASK		(1<<_HAL_S08_SYS_SOPT_BKGDPE_BITPOS)


/*@}*/

/* ********************************************************************************************** */
/*                                  END System Class HAL Support                                  */
/* ********************************************************************************************** */
/**
Used to store CCR register before changing it   
*/
extern byte CCR_copy;
/** 
Disable Global Enable Interrupt 
*/
#define EnterCritical()\
{\
asm tpa;			/* Transfer CCR into Accumulator*/\
asm sei;			/* Disable G.E.I. */\
asm sta CCR_copy;	/* Save Old CCR in CCR_copy*/\
}
/*lint +e950 */

/**
Enable Global Enable Interrupt if enabled before EnterCritical()
*/
#define ExitCritical()\
{\
asm lda CCR_copy;	/* Load CCR_copy into Accumulator*/\
asm tap;			/* Transfer Accumulator into CCR*/\
}
/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined (HAL_CLK_SUPPORT)

/** Define Clock Capability */
#define _HAL_CLK_CAPABILITY

/* Max allow device frequency */
#define CLK_MAX_DEVICE_FREQ         40000000
/* Nominal Internal Clock Frequency [Hz] (FEI Mode) */
#define CLK_INT_FREQ_FEI            243000
/* Nominal Internal Clock Frequency [Hz] (SCM Mode) */
#define CLK_INT_FREQ_SCM            8000000
/* Used in order to obtain N/R integer values */
#define CLK_NORM_FACTOR             64

/* Map mcu specific registers names to a 'standard' set of names 
              
              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>
              
    (this helps when generating new files for additional parts of the same family).
*/
#define _HAL_S08_CLK_ICGC1_REG      (ICGC1)
#define _HAL_S08_CLK_ICGC2_REG      (ICGC2)
#define _HAL_S08_CLK_ICGS1_REG      (ICGS1)
#define _HAL_S08_CLK_ICGS2_REG      (ICGS2)
#define _HAL_S08_CLK_ICGFLT_REG     (ICGFLT)
#define _HAL_S08_CLK_ICGTRM_REG     (ICGTRM)
#define _HAL_S08_CLK_TRIMMING_VALUE (*(const far uint8_t *)0xFFBE)

/* High Gain Oscillator Select 
0 Oscillator configured for low power operation.
1 Oscillator configured for high amplitude operation.
*/
#define _HAL_S08_CLK_ICGC1_HGO_BITPOS   7
#define _HAL_S08_CLK_ICGC1_HGO_MASK     (1<<_HAL_S08_CLK_ICGC1_HGO_BITPOS)

/* Frequency Range Select
The RANGE bit controls the oscillator, reference divider, and FLL loop prescaler multiplication factor (P). 
It selects one of two reference frequency ranges for the ICG. The RANGE bit is
write-once after a reset. 
The RANGE bit only has an effect in FLL engaged external and FLL bypassed external modes.
0 Oscillator configured for low frequency range. FLL loop prescale factor P is 64.
1 Oscillator configured for high frequency range. FLL loop prescale factor P is 1.
*/
#define _HAL_S08_CLK_ICGC1_RANGE_BITPOS 6
#define _HAL_S08_CLK_ICGC1_RANGE_MASK   (1<<_HAL_S08_CLK_ICGC1_RANGE_BITPOS)

/* External Reference Select (Note:bit is write-once after a reset)
0 External clock requested.
1 Oscillator using crystal or resonator requested.
*/
#define _HAL_S08_CLK_ICGC1_REFS_BITPOS  5
#define _HAL_S08_CLK_ICGC1_REFS_MASK    (1<<_HAL_S08_CLK_ICGC1_REFS_BITPOS)

/*
Clock Mode Select 
The CLKS bits control the clock mode. If FLL bypassed external is requested, it will not
be selected until ERCS = 1. If the ICG enters off mode, the CLKS bits will remain unchanged.Writes to the CLKS
bits will not take effect if a previous write is not complete.
The CLKS bits are writable at any time, unless the first write after a reset was CLKS = 0X, the CLKS bits cannot
be written to 1X until after the next reset (because the EXTAL pin was not reserved).
00 Self-clocked
01 FLL engaged, internal reference
10 FLL bypassed, external reference
11 FLL engaged, external reference
*/
#define _HAL_S08_CLK_ICGC1_CLKS0_BITPOS  3
#define _HAL_S08_CLK_ICGC1_CLKS1_BITPOS  4

#define _HAL_S08_CLK_ICGC1_CLKS_MASK    ((1<<_HAL_S08_CLK_ICGC1_CLKS1_BITPOS)|(1<<_HAL_S08_CLK_ICGC1_CLKS0_BITPOS))

/*
Enable Oscillator in Off Mode
The OSCTEN bit controls whether or not the oscillator circuit remains enabled
when the ICG enters off mode.
0 Oscillator disabled when ICG is in off mode unless ENABLE is high, CLKS = 10, and REFST = 1.
1 Oscillator enabled when ICG is in off mode, CLKS = 1X and REFST = 1.
*/

#define _HAL_S08_CLK_ICGC1_OSCSTEN_BITPOS   2
#define _HAL_S08_CLK_ICGC1_OSCSTEN_MASK     (1<<_HAL_S08_CLK_ICGC1_OSCSTEN_BITPOS)

/*
Loss of Clock Disable
0 Loss of clock detection enabled.
1 Loss of clock detection disabled.
*/
#define _HAL_S08_CLK_ICGC1_LOCD_BITPOS   1
#define _HAL_S08_CLK_ICGC1_LOCD_MASK     (1<<_HAL_S08_CLK_ICGC1_LOCD_BITPOS)

/*
Loss of Lock Reset Enable 
The LOLRE bit determines what type of request is made by the ICG following a
loss of lock indication. The LOLRE bit only has an effect when LOLS is set.
0 Generate an interrupt request on loss of lock.
1 Generate a reset request on loss of lock.
*/
#define _HAL_S08_CLK_ICGC2_LOLRE_BITPOS  7
#define _HAL_S08_CLK_ICGC2_LOLRE_MASK    (1<<_HAL_S08_CLK_ICGC2_LOLRE_BITPOS)

/*
Multiplication Factor
The MFD bits control the programmable multiplication factor in the FLL loop. 
*/
#define _HAL_S08_CLK_ICGC2_MFD0_BITPOS  4
#define _HAL_S08_CLK_ICGC2_MFD_MASK     ((1<<6)|(1<<5)|(1<<4))

/*
Loss of Clock Reset Enable
The LOCRE bit determines how the system handles a loss of clock condition
0 Generate an interrupt request on loss of clock.
1 Generate a reset request on loss of clock.
*/
#define _HAL_S08_CLK_ICGC2_LOCRE_BITPOS  3
#define _HAL_S08_CLK_ICGC2_LOCRE_MASK    (1<<_HAL_S08_CLK_ICGC2_LOCRE_BITPOS)

/*
Reduced Frequency Divider
The RFD bits control the value of the divider following the clock select circuitry
*/
#define _HAL_S08_CLK_ICGC2_RFD0_BITPOS  0
#define _HAL_S08_CLK_ICGC2_RFD_MASK     ((1<<2)|(1<<1)|(1<<0))

/*
FLL Lock Status
The LOCK bit indicates whether the FLL has acquired lock. The LOCK bit is cleared in off,
self-clocked, and FLL bypassed modes.
0 FLL is currently unlocked.
1 FLL is currently locked.
*/
#define _HAL_S08_CLK_ICGS1_LOCK_BITPOS  3
/*
External Reference Clock Status
The ERCS bit is an indication of whether or not the external reference
clock (ICGERCLK) meets the minimum frequency requirement.
0 External reference clock is not stable, frequency requirement is not met.
1 External reference clock is stable, frequency requirement is met.
*/
#define _HAL_S08_CLK_ICGS1_ERCS_BITPOS  1

#define CLK_SCM_Mode_Cfg()  ((0<<_HAL_S08_CLK_ICGC1_CLKS1_BITPOS)|(0<<_HAL_S08_CLK_ICGC1_CLKS0_BITPOS))
#define CLK_FEI_Mode_Cfg()  ((0<<_HAL_S08_CLK_ICGC1_CLKS1_BITPOS)|(1<<_HAL_S08_CLK_ICGC1_CLKS0_BITPOS))
#define CLK_FBE_Mode_Cfg()  ((1<<_HAL_S08_CLK_ICGC1_CLKS1_BITPOS)|(0<<_HAL_S08_CLK_ICGC1_CLKS0_BITPOS))
#define CLK_FEE_Mode_Cfg()  ((1<<_HAL_S08_CLK_ICGC1_CLKS1_BITPOS)|(1<<_HAL_S08_CLK_ICGC1_CLKS0_BITPOS))

#define CLK_MFD_RFD_Cfg(MFD,RFD)    ((MFD<<_HAL_S08_CLK_ICGC2_MFD0_BITPOS)|(RFD<<_HAL_S08_CLK_ICGC2_RFD0_BITPOS))       

#define CLK_ResetForLossOfClock_Cfg()   _HAL_S08_CLK_ICGC2_LOCRE_MASK
#define CLK_ResetForLossOfLock_Cfg()    _HAL_S08_CLK_ICGC2_LOLRE_MASK


#define CLK_Gain_Cfg()          (CLK_HIGH_GAIN?_HAL_S08_CLK_ICGC1_HGO_MASK:0)
#define CLK_Range_Cfg()         (CLK_HIGH_FREQUENCY?_HAL_S08_CLK_ICGC1_RANGE_MASK:0)
#define CLK_Reference_Cfg()     (CLK_XTAL?_HAL_S08_CLK_ICGC1_REFS_MASK:0)
#define CLK_InStopMode_Cfg()    (CLK_OFF_IN_STOP?0:_HAL_S08_CLK_ICGC1_OSCSTEN_MASK)
/* Loss of clock ever enabled */
#define CLK_LossOfClock_Cfg()   (0<<_HAL_S08_CLK_ICGC1_LOCD_BITPOS)

#define Is_FLL_Lock()           (_HAL_S08_CLK_ICGS1_REG & (1<<_HAL_S08_CLK_ICGS1_LOCK_BITPOS)?TRUE:FALSE) 
#define CLK_IsOscillatorReady() ((_HAL_S08_CLK_ICGS1_REG & (1<<_HAL_S08_CLK_ICGS1_ERCS_BITPOS))?1:0)

/*
Fixed a frequency, this table is used to find correct values of MFD and RFD. 
*/      
static const uint16_t TabNdivR[8][8] = 
{
256,	384,	512,	640,	768,	896,	1024,	1152,
128,	192,	256,	320,	384,	448,	512,	576,
64, 	96, 	128,	160,	192,	224,	256,	288,
32, 	48, 	64,	    80, 	96, 	112,	128,	144,
16,	    24,	    32,	    40, 	48,	    56,	    64, 	72,
8,  	12,	    16, 	20,    	24, 	28,	    32,	    36,
4,  	6,  	8,  	10, 	12, 	14, 	16,	    18,
2,	    3,	    4,	    5,  	6,  	7,  	8,  	9,    
};


/*
If External clock reference is selected the function setup ICG in FBE mode 
in order to activate the oscillator block.  
*/
#pragma INLINE
static void CLK_EngageClkRefSync(void)
{
    #if !defined (CLK_INT_REF_CLOCK_ACTIVE)
    
    _HAL_S08_CLK_ICGC1_REG = (CLK_Gain_Cfg()|CLK_Range_Cfg()|CLK_Reference_Cfg()|CLK_InStopMode_Cfg()|CLK_LossOfClock_Cfg()|CLK_FBE_Mode_Cfg());
    _HAL_S08_CLK_ICGC2_REG = (CLK_ResetForLossOfClock_Cfg());
    
    #endif  /* #if !defined (CLK_INT_REF_CLOCK_ACTIVE) */
}
/*
-External clock source  
-FEE mode
-Loss of clock detection enabled.
-Reset for loss of clock
-Reset for loss of lock
-f_icgout = f_ext*P*N/R   (P=1 if CLK_HIGH_FREQUENCY is TRUE,P=64 if CLK_HIGH_FREQUENCY is FALSE )
-P = 64
-f_bus=f_icgout/2
Note: Ensure that f_icgdclock=f_icgout * R, does not exceed fICGDCLKmax = 40MHz
*/
#pragma INLINE    
static bool_t CLK_SetFLLExternalClockSync(uint32_t desiredClock)
{
uint16_t n_div_r_desired;
int16_t old_n_div_r =0x7FFF;
uint8_t mfd,rfd;
uint8_t mfd_r,rfd_r;

    #if !defined (CLK_INT_REF_CLOCK_ACTIVE) 
    
    #if (CLK_HIGH_FREQUENCY == FALSE)
    n_div_r_desired = (uint16_t)(desiredClock*CLK_NORM_FACTOR/(64*CLK_EXT_CLOCK_FREQ)); 
    #endif
    
    #if (CLK_HIGH_FREQUENCY == TRUE)
    n_div_r_desired = (uint16_t)(desiredClock*CLK_NORM_FACTOR/CLK_EXT_CLOCK_FREQ);     
    #endif
   
    
    for (rfd=0;rfd<=7;rfd++)
    {
        for (mfd=0;mfd<=7;mfd++)
        {
            if(abs(TabNdivR[rfd][mfd]-n_div_r_desired)<old_n_div_r)
            {
                old_n_div_r = abs(TabNdivR[rfd][mfd]-n_div_r_desired);
                mfd_r = mfd;
                rfd_r = rfd;    
            }            
        }
    }
    _HAL_S08_CLK_ICGC1_REG = (CLK_Gain_Cfg()|CLK_Range_Cfg()|CLK_Reference_Cfg()|CLK_InStopMode_Cfg()|CLK_FEE_Mode_Cfg());
    _HAL_S08_CLK_ICGC2_REG = (CLK_ResetForLossOfClock_Cfg()|CLK_MFD_RFD_Cfg(mfd_r,rfd_r)|CLK_ResetForLossOfLock_Cfg());
    
    /* Wait for FLL lock */
    while(Is_FLL_Lock()==FALSE)    
    {
        
    }
    return(1);
    #else
    return(0);
    #endif  /* #if !defined (CLK_INT_REF_CLOCK_ACTIVE) */
        
}
/*
-External clock source  
-FBE mode
-Loss of clock detection enabled.
-Reset for loss of clock
-Reset for loss of lock
-f_icgout = f_ext/R
-f_bus=f_icgout/2
*/
#pragma INLINE
static bool_t CLK_SetExternalClockSync(uint32_t desiredClock)
{
uint8_t rfd;
uint8_t rfd_r;
int32_t min_distance = 0x7FFFFFFF;

    #if !defined(CLK_INT_REF_CLOCK_ACTIVE)
    for (rfd=0;rfd<=7;rfd++)
    {
        if(labs(desiredClock-CLK_EXT_CLOCK_FREQ/(1<<rfd))<min_distance)
        {
            min_distance = labs(desiredClock-CLK_EXT_CLOCK_FREQ/(1<<rfd)); 
            rfd_r = rfd;   
        }
    }
    _HAL_S08_CLK_ICGC1_REG = (CLK_Gain_Cfg()|CLK_Range_Cfg()|CLK_Reference_Cfg()|CLK_InStopMode_Cfg()|CLK_FBE_Mode_Cfg());
    _HAL_S08_CLK_ICGC2_REG = (CLK_ResetForLossOfClock_Cfg()|CLK_MFD_RFD_Cfg(0,rfd_r)|CLK_ResetForLossOfLock_Cfg());

    return(1);
    #else
    return(0);
    #endif  /* #if !defined(CLK_INT_REF_CLOCK_ACTIVE) */
}
/*
-Internal clock source  (f_int=8 MHz)
-SCM mode
-Loss of clock detection enabled.
-Reset for loss of clock
-Reset for loss of lock
-f_icgout = f_irg/R
-f_bus=f_icgout/2
*/
#pragma INLINE
static bool_t CLK_SetInternalClockSync(uint32_t desiredClock)
{
uint8_t rfd;
uint8_t rfd_r;
int32_t min_distance = 0x7FFFFFFF;
    

    for (rfd=0;rfd<=7;rfd++)
    {
        if(labs(desiredClock-CLK_INT_FREQ_SCM/(1<<rfd))<min_distance)
        {
            min_distance = labs(desiredClock-CLK_INT_FREQ_SCM/(1<<rfd)); 
            rfd_r = rfd;   
        }
    }
    _HAL_S08_CLK_ICGC1_REG = CLK_SCM_Mode_Cfg();
    _HAL_S08_CLK_ICGC2_REG = (CLK_ResetForLossOfClock_Cfg()|CLK_MFD_RFD_Cfg(0,rfd_r));
    
    return (1);    
}

/*
-Internal clock source  (f_irg=243 KHz)
-FEI mode
-Loss of clock detection enabled.
-Reset for loss of clock
-Reset for loss of lock
-f_icgout = (f_irg/7)*64*N/R
-P = 64
-f_bus=f_icgout/2
Note: Ensure that f_icgdclock=f_icgout * R, does not exceed fICGDCLKmax = 40MHz
*/
#pragma INLINE
static bool_t CLK_SetFLLInternalClockSync(uint32_t desiredClock)
{
uint16_t n_div_r_desired;
uint16_t old_n_div_r =0x7FFF;
uint8_t mfd,rfd;
uint8_t mfd_r,rfd_r;

    #if defined (CLK_INT_REF_CLOCK_ACTIVE)    
    n_div_r_desired = (uint16_t)(desiredClock*CLK_NORM_FACTOR/((64*CLK_INT_FREQ_FEI)/7)); 
    
    for (rfd=0;rfd<=7;rfd++)
    {
        for (mfd=0;mfd<=7;mfd++)
        {
            if(abs(TabNdivR[rfd][mfd]-n_div_r_desired)<old_n_div_r)
            {
                old_n_div_r = abs(TabNdivR[rfd][mfd]-n_div_r_desired);
                mfd_r = mfd;
                rfd_r = rfd;    
            }            
        }
    }
    _HAL_S08_CLK_ICGC1_REG = CLK_FEI_Mode_Cfg();
    _HAL_S08_CLK_ICGC2_REG = (CLK_ResetForLossOfClock_Cfg()|CLK_MFD_RFD_Cfg(mfd_r,rfd_r)|CLK_ResetForLossOfLock_Cfg());
    /* Wait for FLL lock */
    while(Is_FLL_Lock()==FALSE)    
    {
        
    }
    /* Load Trimming Value from Flash */
    _HAL_S08_CLK_ICGTRM_REG = _HAL_S08_CLK_TRIMMING_VALUE;
    return(1);
    #else
    return(0);
    #endif  /* #if defined (CLK_INT_REF_CLOCK_ACTIVE) */

}
 
/*
Update Trimming Value 
*/
#pragma INLINE
static void CLK_SetTrimSync(CLK_TrimmingType desiredTrim)
{
    _HAL_S08_CLK_ICGTRM_REG = desiredTrim;    
}
/*
Return Trimming Value 
*/
#pragma INLINE 
static CLK_TrimmingType CLK_GetTrimSync(void)
{
    return(_HAL_S08_CLK_ICGTRM_REG);    
}

#endif /* #if defined (HAL_CLK_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                     */
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
/** This is the System Option Register */
#define _HAL_S08_WDT_SOPT_REG               (SOPT)
/** This is the bit position of COPT bit */
#define _HAL_S08_WDT_SOPT_COPT_BITPOS		6
/** This is the bit mask of COPT bit */
#define _HAL_S08_WDT_SOPT_COPT_MASK   		(1<<_HAL_S08_WDT_SOPT_COPT_BITPOS)
/** This is the bit position of COPE bit */
#define _HAL_S08_WDT_SOPT_COPE_BITPOS		7
/** This is the bit mask of COPE bit */
#define _HAL_S08_WDT_SOPT_COPE_MASK			(1<<_HAL_S08_WDT_SOPT_COPE_BITPOS)

/**
WatchDog Setup.\n
Attention because of _HAL_S08_WDT_SOPT_REG is a write-once register,\n
so only the first write after reset is honored, also BDM and STOP Mode are setup here.
*/

#define	WDT_InitSync()\
{\
_HAL_S08_WDT_SOPT_REG=\
(\
(WDT_ENABLED<<_HAL_S08_WDT_SOPT_COPE_BITPOS)|\
(WDT_TIMEOUT<<_HAL_S08_WDT_SOPT_COPT_BITPOS)|\
(STOP_ENABLE<<_HAL_S08_SYS_SOPT_STOPE_BITPOS)|\
(BDM_ENABLE<<_HAL_S08_SYS_SOPT_BKGDPE_BITPOS)\
);\
}

/**
Feed the dog
*/
/*lint -e522 */
#define WDT_Refresh() {asm sta SRS;}

#define WDT_Enabled()   WDT_ENABLED
#define WDT_AllowUpdate()   FALSE
/*lint -esym(715,timeout) */
/*lint -esym(715,window) */ 
/*lint -esym(715,testByte) */ 
/**
Unsupported 
*/
#define WDT_StartSync()  
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
#define WDT_Counter()   0
/**
Unsupported 
*/  
#define WDT_GetWindow() 0
/**
Unsupported 
*/  
#define WDT_RSTCounter()     0
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
#define FLASH_CMD_IN_RAM
/** All flash's Pages have the same size */
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
/** This is the size in bytes of a flash's page.*/
#define FLASH_PAGE_SIZE_IN_BYTES    512
/* 63 is the max. avaible value for flash module clock divisor */
#if ((BUS_FREQUENCY_Hz/FLS_MODULE_FREQUENCY_Hz)>63)
	#define FLS_FLG_PRDIV8	1
	#define FLS_DIV ((BUS_FREQUENCY_Hz/8/FLS_MODULE_FREQUENCY_Hz)-1)
	#if (FLS_DIV > 63)
		#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor too high!"
	#endif
	#if (FLS_DIV < 0)
		#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor is negative!"
	#endif	
#else
	#define FLS_FLG_PRDIV8	0
	#define FLS_DIV ((BUS_FREQUENCY_Hz/FLS_MODULE_FREQUENCY_Hz)-1)
#endif
/* Map mcu specific registers names to a 'standard' set of names 

				HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>

	(this helps when generating new files for additional parts of the same family).
*/
/** This is the  FLASH Clock Divider Register (FCDIV) */
#define _HAL_S08_FLS_FCDIV_REG					(FCDIV)
/** This is the bit position of PRDIV8 bit */
#define _HAL_S08_FLS_FCDIV_PRDIV8_BITPOS		6

/** This is the  FLASH Status Register (FSTAT) */
#define _HAL_S08_FLS_FSTAT_REG					(FSTAT)
/** This is the bit position of FCBEF bit */
#define _HAL_S08_FLS_FSTAT_FCBEF_BITPOS			7
/** This is the bit position of FCCF bit */
#define _HAL_S08_FLS_FSTAT_FCCF_BITPOS			6
/** This is the bit position of FPVIOL bit */
#define _HAL_S08_FLS_FSTAT_FPVIOL_BITPOS		5
/** This is the bit position of FACCERR bit */
#define _HAL_S08_FLS_FSTAT_FACCERR_BITPOS		4

#define _HAL_S08_FLS_FSTAT_ERR_MASK				((1<<_HAL_S08_FLS_FSTAT_FPVIOL_BITPOS)|(1<<_HAL_S08_FLS_FSTAT_FACCERR_BITPOS))
/** This is the  FLASH Command Register (FCMD) */
#define _HAL_S08_FLS_FCMD_REG					(FCMD)
/**
Initialization of the FLASH Module Clock.\n
Programming and erase operations are allowed. 
 */
#define FLS_Init()\
{\
	_HAL_S08_FLS_FCDIV_REG =\
	((FLS_FLG_PRDIV8<<_HAL_S08_FLS_FCDIV_PRDIV8_BITPOS)|FLS_DIV);\
}
/**
Abort any command and clear errors 
*/
#define FLS_InitRegsForErasing()\
{\
	_HAL_S08_FLS_FSTAT_REG =_HAL_S08_FLS_FSTAT_ERR_MASK;\
}
/**
Abort any command and clear errors 
*/
#define FLS_InitRegsForProgramming()\
{\
	_HAL_S08_FLS_FSTAT_REG =_HAL_S08_FLS_FSTAT_ERR_MASK;\
}
/**
Page Erase Routine
*/

#pragma TEST_CODE == SIZE_CMD_ERASE_IN_RAM			/* Check code lenght */
#pragma INLINE
static void FLS_ErasePageCmdIsInRam(FLS_paramType param)
{
	/* Write a dummy value at Address "param" */
	*(uint8_t*)param = 0x00;
	/* Write Erase Page Command */ 
	_HAL_S08_FLS_FCMD_REG = 0x40;   
	/* Launch the command */                  		
	_HAL_S08_FLS_FSTAT_REG = (1<<_HAL_S08_FLS_FSTAT_FCBEF_BITPOS);                         	
  	/* Warning we must wait min. 4 bus cycle before testing FCCF */
  	
	/* Wait for command completion */
  	do{
  		/* This statement is 4 machine cycles long,
  		   if watchdog is not enabled this is a dummy statement  */
  		WDT_Refresh();	
  	}while( 0 == ( _HAL_S08_FLS_FSTAT_REG&(1<<_HAL_S08_FLS_FSTAT_FCCF_BITPOS)));
}
/**
Write Routine, Burst Command.
Size must be != 0.
- address   Specify start point of the source data.
- position  Specify start point of destination. 
*/

#pragma TEST_CODE == SIZE_BURST_CMD_IN_RAM 	/* Check code lenght */ 
static void FLS_BurstProgCmdIsInRam(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{	
	do 
	{   /* FOR ALL BYTES THAT MUST BE WRITTEN: */
		/* 1) WAIT FOR FCBEF = 1 */
	  	do
	  	{
	 		/* if watchdog is not enabled this is a dummy statement  */
	  		WDT_Refresh();		  		
	  	}while( 0 == ( _HAL_S08_FLS_FSTAT_REG & (1<<_HAL_S08_FLS_FSTAT_FCBEF_BITPOS)));
		/* 2) WRITE TO FLASH TO BUFFER ADDRESS AND DATA */
		*(FLS_SourcePtrType)position = *pSource;
		/* 3) WRITE BURST COMMAND TO FCMD */ 
		_HAL_S08_FLS_FCMD_REG = 0x25;       	
		/* 4) WRITE 1 TO FCBEF TO LAUNCH COMMAND AND CLEAR FCBEF 
		   Note: Wait at least four bus cycles before checking FCBEF or FCCF.*/                  		
		_HAL_S08_FLS_FSTAT_REG = (1<<_HAL_S08_FLS_FSTAT_FCBEF_BITPOS);
		
		/* 5) CHECK ERRORS */
		if(_HAL_S08_FLS_FSTAT_REG & _HAL_S08_FLS_FSTAT_ERR_MASK)
		{
			return;
		}

		pSource++;                           	
		position++;								
		size--;
	}while(0 != size);
	/* COMMAND COMPLETE ? */ 
	do
	{
		
	}while( 0 == ( _HAL_S08_FLS_FSTAT_REG&(1<<_HAL_S08_FLS_FSTAT_FCCF_BITPOS)));
}
/**
Execute Erase Command in R.A.M.
- Copy FLS_ErasePageCmdIsInRam in RAM
- Execute in RAM Block Erase Cmd 
*/
/*lint -emacro(545,FLS_Erase) */
/*lint -emacro(546,FLS_Erase) */
/*lint -emacro(420,FLS_Erase) */ 
/*lint -emacro(643,FLS_Erase) */ 
/*lint -emacro(611,FLS_Erase) */
/*lint -emacro(826,FLS_Erase) */ 
/*lint -emacro(740,FLS_Erase) */        

#define FLS_Erase(position)\
{\
(void)memcpy(&(RamCmd.cmd), &FLS_ErasePageCmdIsInRam, SIZE_CMD_ERASE_IN_RAM);\
((pFLS_ErasePageCmdIsInRam)&(RamCmd.cmd))(position);\
}
       

/**
Execute Burst Command in R.A.M.
- Copy FLS_BurstProgCmdIsInRam in RAM
- Execute in RAM Burst Program Cmd 
*/
/*lint -emacro(545,FLS_Programming) */
/*lint -emacro(546,FLS_Programming) */
/*lint -emacro(420,FLS_Programming) */ 
/*lint -emacro(643,FLS_Programming) */ 
/*lint -emacro(611,FLS_Programming) */
/*lint -emacro(826,FLS_Programming) */ 
/*lint -emacro(740,FLS_Programming) */   
#define FLS_Programming(pSource,size,position)\
{\
(void)memcpy(&(RamCmd.cmd),&FLS_BurstProgCmdIsInRam,SIZE_BURST_CMD_IN_RAM);\
((pFLS_BurstProgCmdIsInRam)&(RamCmd.cmd))(pSource,size,position);\
}

#pragma INLINE
static uint8_t FLS_CheckErrors(void)
{  
	return(_HAL_S08_FLS_FSTAT_REG & _HAL_S08_FLS_FSTAT_ERR_MASK);
}

#endif /*defined(HAL_FLS_SUPPORT)*/
/* ********************************************************************************************** */
/*                                  END FLS Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)

/* Calculate Prescaler Value 
   factor:=(PRS+1)*2; ATD Conversion frequency = BusClockFrequency/factor
*/
#define ADC_PRS	(BUS_FREQUENCY_Hz/(2*ADC_CONV_FREQUENCY_Hz)-1)
#if (ADC_PRS>15)
#error "please check ADC_CONV_FREQUENCY_Hz value is not compatible with BUS_FREQUENCY_Hz "
#endif
#if (ADC_PRS<0)
#error "please check ADC_CONV_FREQUENCY_Hz value is not compatible with BUS_FREQUENCY_Hz"
#endif
#if ((BUS_FREQUENCY_Hz/((ADC_PRS+1)*2))>2000000)
#error "problem encounterd with ADC_PRS computation"
#endif
#if ((BUS_FREQUENCY_Hz/((ADC_PRS+1)*2))<500000)
#error "problem encounterd with ADC_PRS computation"
#endif


/** This is the  ATD Control Register (bit3-bit0 used for prescaler) */
#define _HAL_S08_ADC_ATD1C_REG					(ATD1C)
/** This is the bit position of ATDPU bit */
#define _HAL_S08_ADC_ATD1C_ATDPU_BITPOS			7
/** This is the bit position of DJM bit */
#define _HAL_S08_ADC_ATD1C_DJM_BITPOS			6
/** This is the bit position of RES8 bit */
#define _HAL_S08_ADC_ATD1C_RES8_BITPOS			5
/** This is the bit position of SGN bit */
#define _HAL_S08_ADC_ATD1C_SGN_BITPOS			4
/** This is the  ATD Status and Control Register (ATD1SC) (bit5-bit0 Channel Selection)*/
#define _HAL_S08_ADC_ATD1SC_REG					(ATD1SC)
/** This is the bit position of CCF bit (read only)*/
#define _HAL_S08_ADC_ATD1SC_CCF_BITPOS			7
/** This is the bit position of ATDIE bit */
#define _HAL_S08_ADC_ATD1SC_ATDIE_BITPOS		6
/** This is the bit position of ATDCO bit */
#define _HAL_S08_ADC_ATD1SC_ATDCO_BITPOS		5
/** This is the mask for channel field inside ATD1SC  */
#define _HAL_S08_ADC_ATD1SC_ATDCH_MASK          0x1F

/* Channel selection */
#define ADC_CH0_SEL		0
#define ADC_CH1_SEL		1
#define ADC_CH2_SEL		2
#define ADC_CH3_SEL		3
#define ADC_CH4_SEL		4
#define ADC_CH5_SEL		5
#define ADC_CH6_SEL		6
#define ADC_CH7_SEL		7
#define ADC_VREFH_SEL	0x1E
#define ADC_VREFL_SEL	0x1F

/** This is the  ATD Result Data Register Address (ATD1R) (word, read only) */
#define _HAL_S08_ADC_ATD1R_ADDR					((uint16_t*)0x0052)
#define _HAL_S08_ADC_ATD1R                      (ATD1R)
/* Port Data Register shared with ADC */
#define PORT_DATA_SHARED_WITH_ADC               (PTBD)

/** This is the  ATD Pin Enable Register (ATD1PE) (bitmap) */
#define _HAL_S08_ADC_ATD1PE_REG					(ATD1PE)
/**
\n\li ATD Power Up
\n\li Result register data is right justified
\n\li 10-bit resolution selected
*/
#define ADC_Init()\
{\
	_HAL_S08_ADC_ATD1C_REG =\
	((1<<_HAL_S08_ADC_ATD1C_ATDPU_BITPOS)|(1<<_HAL_S08_ADC_ATD1C_DJM_BITPOS)|ADC_PRS);\
}
/**
Set all ADC Registers to the reset state value
*/
#define ADC_DeInit()\
{\
	_HAL_S08_ADC_ATD1C_REG = 0x00;\
	_HAL_S08_ADC_ATD1SC_REG = 0x01;\
	_HAL_S08_ADC_ATD1PE_REG = 0x00;\
}
/**
'0' if channel is allowable '1' otherwise 
*/
#define ADC_CheckChannel(channel)	(((channel<=7)||(channel==ADC_VREFH_SEL)||(channel==ADC_VREFL_SEL))?0:1)
/**
\n\li if channel <= 7 set, according to the selected channel, the pin as dedicated to the ATD module.
\n\li if channel > 8 clear ATD1PE register
\n\li A write to this register will abort the current conversion but will not initiate a new conversion.
*/
#define ADC_ConfigurePinAsChannel(channel)	((channel<=7)?(_HAL_S08_ADC_ATD1PE_REG |= (1<<channel)):(_HAL_S08_ADC_ATD1PE_REG = 0x00))
/**
\n\li if channel <= 7 set, according to the selected channel, the pin to default configuration.
\n\li if channel > 8 clear ATD1PE register
\n\li A write to this register will abort the current conversion but will not initiate a new conversion.
*/
#define ADC_DeConfigurePinAsChannel(channel)	((channel<=7)?(_HAL_S08_ADC_ATD1PE_REG &= (~(1<<channel))):(_HAL_S08_ADC_ATD1PE_REG = 0x00))
/**
\n Writes to the ATD status and control register 
\n\li clear the CCF flag 
\n\li cancels any pending interrupts
\n\li clear or set ATD Interrupt
\n\li clear ATD Continuous Conversion
\n\li initiates a new conversion.

*/
#define ADC_StartConvertionOnChannel(channel,atdInterrupt)	(_HAL_S08_ADC_ATD1SC_REG = (channel|(atdInterrupt?(1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS):0)))

/**
Channel selected for ATD convertion
*/
#define ADC_GetActiveChannel()	(_HAL_S08_ADC_ATD1SC_REG & _HAL_S08_ADC_ATD1SC_ATDCH_MASK )

/**
\n Wait for Conversion Complete.
\n The CCF is a read-only bit which is set each time a conversion is complete. 
*/
#define ADC_WaitForConvertionOnChannel(channel)	while((_HAL_S08_ADC_ATD1SC_REG&(1<<_HAL_S08_ADC_ATD1SC_CCF_BITPOS)) == 0){}

/**
\n Test Conversion Complete Flag.
\n The CCF is a read-only bit which is set each time a conversion is complete. 
*/
#define ADC_EndConvertion()	(_HAL_S08_ADC_ATD1SC_REG&(1<<_HAL_S08_ADC_ATD1SC_CCF_BITPOS))
/**
\n\li ADC Conversion Result is ever in 10 bit 
\n\li This macro modify it to the desired resolution
*/
#define ADC_GetCorrectResolutionResult(result)   ((ADC_RESOLUTION > 10)?(result<<=(ADC_RESOLUTION-10)):(result>>=(10-ADC_RESOLUTION)))               

/**
\n\li ADC Conversion Result is ever in 10 bit 
\n\li This function get the converted value and modify it to the desired resolution
 */	
#pragma INLINE
static uint16_t ADC_GetConvertedValueOnChannel(void)
{
uint16_t adConvValue;
	adConvValue = *_HAL_S08_ADC_ATD1R_ADDR;
    ADC_GetCorrectResolutionResult(adConvValue);
	return(adConvValue);
}

/**
Writes to the ATD status and control register clears the CCF flag, cancels any pending interrupts, and
initiates a new conversion.
A write to the ATD pin enable register will abort the current conversion but will not initiate a new conversion. 
*/

#define ADC_StopConversion()\
{\
_HAL_S08_ADC_ATD1SC_REG = 0x01;\
_HAL_S08_ADC_ATD1PE_REG = 0x00;\
}


/* added channel parameter for compatibility with others uc S.U. 31/05/2010 */
#define ADC_pResultRegister(channel)   (&_HAL_S08_ADC_ATD1R)
#define ADC_pChannelRegister(channel)  (&_HAL_S08_ADC_ATD1SC_REG)

/*
\n Writes to the ATD status and control register 
\n\li clear the CCF flag 
\n\li cancels any pending interrupts
\n\li set ATD Interrupt
\n\li set ATD Continuous Conversion
\n\li initiates a new conversion on channel
*/
#define ADC_StartMultiSampleConvertionOnChannel(channel)\
{\
_HAL_S08_ADC_ATD1SC_REG = ((1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS)|(1<<_HAL_S08_ADC_ATD1SC_ATDCO_BITPOS)|channel);\
}

#define ADC_StartMultiSampleConvertionMultiChannel()


#define ADC_CorrectChannel(channel)     (channel | (1<<_HAL_S08_ADC_ATD1SC_ATDIE_BITPOS))
/*
Starting from portAddress and pinMask (read pin) search for AD channel  
*/
#pragma INLINE
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

#pragma MESSAGE INFORMATION C5703
#pragma MESSAGE INFORMATION C5702
#pragma MESSAGE WARNING     C5917
#pragma MESSAGE WARNING     C5902
/*============================================================= CEFRIEL ================================================================================*/
// temporary commented

#if 0

/* ******** */
/* Includes */
/* ******** */

#include "FS_HCS08_GB60_reg.h"
#include "string.h"
#include "StandardTypes.h"
#include "StandardDefines.h"
#include "debug.h"				/* Aggiunto solo per compilazione */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Numero di pin del modulo linkage = dimensione dell'array ModuleIOmap[] */
#define LINKAGE_MODULE_NUMOFPIN 44

/* Digital IO Ports */

#define DIO_PORTA                 0
#define DIO_PORTB                 1
#define DIO_PORTC                 2
#define DIO_PORTD                 3
#define DIO_PORTE                 4
#define DIO_PORTF                 5
#define DIO_PORTG                 6


#define IRQID_MCU                27 

struct portset_struct
{

uint8_t * pData_loc;
uint8_t  mask;

};


/* Timers */

struct timerset_struct 
{

uint8_t * pTPMySC;
uint8_t * pTPMyCxSC;
uint8_t McuPin;

};


/**
struttura dei registri di controllo e stato relativi ai timer 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Prescaler bit 0 */
    uint8_t PS0     :1;
    /** Prescaler bit 1 */
    uint8_t PS1     :1; 
    /** Prescaler bit 2 */
    uint8_t PS2		  :1; 
    /** Timer y Clock select A */                                      
    uint8_t CLKSA	  :1; 
    /** Timer y Clock select B */                                      
    uint8_t CLKSB	  :1;                                       
    /** Timer y PWM Enable */
    uint8_t CPWMS	  :1;                                       
    /** Timer y OF Interrupt Enable */
    uint8_t TOIE	  :1;
    /** Timer y OF Pending Flag */                                       
    uint8_t TOF		  :1;                                       
  } Bits;
} TPMySC_struct_t;


/**
struttura dei registri di controllo e stato relativi ai canali dei timer 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Reserved */
    uint8_t         :1; 
    /** Reserved */
    uint8_t         :1; 
    /** Edge/Level Select Bit A */
    uint8_t ELSxA   :1; 
    /** Edge/Level Select Bit B */                                      
    uint8_t ELSxB   :1; 
    /** Mode Select A for TPMy Channel x */                                      
    uint8_t MSxA    :1;                                       
    /** Mode Select B for TPMy Channel x */
    uint8_t MSxB    :1;                                       
    /** Channel x Interrupt Enable */
    uint8_t CHxIE   :1;
    /** Channel x Event Pending Flag */                                       
    uint8_t CHxF    :1;                                       
  } Bits;
} TPMyCxSC_struct_t;


/** Numero dei canali agganciati al Timer 1 esclusi quelli utilizzati dall'Osek */
#define MCU_TPM1_NUMOFCHANNEL 2 
/** Numero dei canali agganciati al Timer 2 */
#define MCU_TPM2_NUMOFCHANNEL 5
/** Numero dei canali complessivi utilizzabili dal modulo */
#define MCU_TOTAL_NUMOFCHANNEL (MCU_TPM1_NUMOFCHANNEL + MCU_TPM2_NUMOFCHANNEL)

/** Timer e canali usati e numerati secondo l'odine nell'array timersMap[] */
/* Timer 1 */
/*#define TPM1CH0  used by Osek */
#define TPM1CH1  0
#define TPM1CH2  1
/* Timer 2 */
#define TPM2CH0  2
#define TPM2CH1  3
#define TPM2CH2	 4
#define TPM2CH3	 5	 
#define TPM2CH4	 6

/** Pin collegato al timer TPM2CH4 (ridefinito MCU_TIMERCHANNEL7) -> Port D pin 7 */
#define MCU_TIMERCHANNEL7_PIN PTDD_PTDD7

/** Canale disabilitato. Il pin corrispondente viene usato come General IO */
#define TIMER_CHANNEL_DISABLE   0x00

/** Configurazione del canale per modalità input capture on rising edge */
#define INPUT_CAPTURE_ON_RISING_EDGE	    0x04
/** Configurazione del canale per modalità input capture on falling edge */
#define INPUT_CAPTURE_ON_FALLING_EDGE     0x08
/** Configurazione del canale per modalità input capture on both edges */
#define INPUT_CAPTURE_ON_BOTH_EDGE		    0x0C
/** Configurazione del canale per modalità toggle output */
#define OUTPUT_COMPARE_TOGGLE_OUTPUT		  0x14
/** Configurazione del canale per modalità output high on compare */
#define OUTPUT_COMPARE_SET_OUTPUT			    0x18
/** Configurazione del canale per modalità output low on compare */
#define OUTPUT_COMPARE_RESET_OUTPUT			  0x1C

#if defined (WD_3PHM) \
    || defined (WD_UM) \
    || defined (WD_3PHM_UM) \
    || defined (WM_UM) \
    || defined (DIO_DEBUG)


/* Timer 2 Channel Configuration: 0 disable; 1 enable */

/** THIS CHANNEL CAN NOT USED BECAUSE IT'S USED BY OSEK */
	#define FREQMETER_TPM1_CH0_ENABLE   0 
/** This channel is not used in input capture */
	#define FREQMETER_TPM1_CH1_ENABLE	0
/** Pressostato lineare */	
	#define FREQMETER_TPM1_CH2_ENABLE	1
/** Feedback pompa scarico */		
	#define FREQMETER_TPM2_CH0_ENABLE 	1 
/** This channel is not used in input capture */
	#define FREQMETER_TPM2_CH1_ENABLE	0
/** This channel is not used in input capture (gate triac controllo motore) */
	#define FREQMETER_TPM2_CH2_ENABLE	0	
/** Feedback Triac Motore Universale */	
	#define FREQMETER_TPM2_CH3_ENABLE	1
/** Tachimetrica */		
	#define FREQMETER_TPM2_CH4_ENABLE	1   

#elif defined (DISHWASHER)
/** THIS CHANNEL CAN NOT BE USED BECAUSE OF OSEK */
	#define FREQMETER_TPM1_CH0_ENABLE   0 
/** This channel is not used in input capture */
	#define FREQMETER_TPM1_CH1_ENABLE	0
/** This channel is used for literCounter T022 */
	#define FREQMETER_TPM1_CH2_ENABLE	1
/** This channel is used for linearPressure T021 */
	#define FREQMETER_TPM2_CH0_ENABLE 	1 
/** This channel is not used in input capture */
	#define FREQMETER_TPM2_CH1_ENABLE	0
/** This channel is not used in input capture (PWM for turbidity sensor) T023 */
	#define FREQMETER_TPM2_CH2_ENABLE	0	
/** This channel is not used in input capture */
	#define FREQMETER_TPM2_CH3_ENABLE	0
/** This channel is not used in input capture */
	#define FREQMETER_TPM2_CH4_ENABLE	0   

#endif


#if defined (WD_3PHM) \
    || defined (WD_UM) \
    || defined (WD_3PHM_UM) \
    || defined (WM_UM) \
    || defined (DIO_DEBUG)

    /* 
    TPM1CxSC - TPM 1 Timer Channel x Status and Control Register 
    cfr. TPM2CxSC register for setup 
    */

    /** Timer 1 Channel 0 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH0   0x00
    /** Timer 1 Channel 1 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH1   0x00
    /** Timer 1 Channel 2 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH2   INPUT_CAPTURE_ON_RISING_EDGE

    /* 
    TPM2CxSC - TPM 2 Timer Channel x Status and Control Register 
    cfr. TPM2CxSC register for setup 
    */
    
    /** Timer 2 Channel 0 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH0   INPUT_CAPTURE_ON_RISING_EDGE
    /** Timer 2 Channel 1 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH1   INPUT_CAPTURE_ON_RISING_EDGE
    /** Timer 2 Channel 2 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH2   INPUT_CAPTURE_ON_RISING_EDGE
    /** Timer 2 Channel 3 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH3   INPUT_CAPTURE_ON_RISING_EDGE
    /** Timer 2 Channel 4 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH4   INPUT_CAPTURE_ON_RISING_EDGE

#elif defined (DISHWASHER)

    /* 
    TPM1CxSC - TPM 1 Timer Channel x Status and Control Register 
    cfr. TPM2CxSC register for setup 
    */

    /** Timer 1 Channel 0 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH0   0x00
    /** Timer 1 Channel 1 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH1   0x00
    /** Timer 1 Channel 2 setup */
    #define FREQMETER_SC_INIT_VAL_TPM1CH2   0x00

    /* 
    TPM2CxSC - TPM 2 Timer Channel x Status and Control Register 
    cfr. TPM2CxSC register for setup 
    */
    
    /** Timer 2 Channel 0 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH0   INPUT_CAPTURE_ON_RISING_EDGE
    /** Timer 2 Channel 1 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH1   0x00
    /** Timer 2 Channel 2 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH2   0x00
    /** Timer 2 Channel 3 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH3   0x00
    /** Timer 2 Channel 4 setup */
    #define FREQMETER_SC_INIT_VAL_TPM2CH4   0x00

#endif

#define PULSECOUNTER_LOCALFILTER_MCU        12
#define PULSECOUNTER_TICK_DURATION_MCU      12804UL	  /* nanosecondi */


/* Serial Communication Interface (UART) */

/** Serial Communication Interface (UART) usati e numerati secondo l'odine nell'array sciMap[] */
#define SCI1    0
#define SCI2		1

#define MCU_NUM_OF_UARTS  2

/**
Struttura del registro di controllo 1 relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Parity Type */                                       
    uint8_t PT      :1;
    /** Parity Enable */
    uint8_t PE      :1;
    /** Idle Line Type Select */
    uint8_t ILT     :1;               
    /** Receiver Wakeup Method Select */                                      
    uint8_t WAKE    :1;                                       
    /** 9-Bit or 8-Bit Mod  e Select */                                      
    uint8_t M       :1; 
    /** Receiver Source Select */
    uint8_t RSRC    :1; 
    /** SCI Stops in Wait Mode */
    uint8_t SCISWAI :1; 
    /** Loop Mode Select */
    uint8_t LOOPS   :1; 
  } Bits;
} SCIxC1_struct_t;

/**
Struttura del registro di controllo 2 relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Send Break */                                       
    uint8_t SBK     :1;                                       
    /** Receiver Wakeup Control */
    uint8_t RWU     :1;
    /** Receiver Enable */
    uint8_t RE      :1;                                       
     /** Transmitter Enable */                                      
    uint8_t TE      :1;                                       
    /** Idle Line Interrupt Enable (for IDLE) */                                      
    uint8_t ILIE    :1; 
    /** Receiver Interrupt Enable (for RDRF) */
    uint8_t RIE     :1; 
    /** Transmission Complete Interrupt Enable (for TC) */
    uint8_t TCIE    :1; 
    /** Transmit Interrupt Enable (for TDRE) */
    uint8_t TIE     :1; 
  } Bits;
} SCIxC2_struct_t;

/**
Struttura del registro di stato 1 relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Parity Error Flag */
    uint8_t PF      :1; 
    /** RFraming Error Flag */
    uint8_t FE      :1; 
    /** Noise Flag */
    uint8_t NF      :1; 
    /** Receiver Overrun Flag */                                      
    uint8_t OR      :1; 
    /** Idle Line Flag */                                      
    uint8_t IDLE    :1;                                       
    /** Receive Data Register Full Flag */
    uint8_t RDRF    :1;                                       
    /** Transmission Complete Flag */
    uint8_t TC      :1;
    /** Transmit Data Register Empty Flag */                                       
    uint8_t TDRE    :1;                                       
  } Bits;
} SCIxS1_struct_t;

/**
Struttura del registro di stato 2 relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Receiver Active Flag */
    uint8_t RAF     :1; 
    /** Reserved */
    uint8_t         :7; 
  } Bits;
} SCIxS2_struct_t;

/**
Struttura del registro di controllo 3 relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef union {
  uint8_t all_Data;
  struct {
    /** Parity Error Interrupt Enable */                                       
    uint8_t PEIE    :1;                                       
    /** Framing Error Interrupt Enable */
    uint8_t FEIE    :1;
    /** Noise Error Interrupt Enable */
    uint8_t NEIE    :1;                                       
    /** Overrun Interrupt Enable */                                      
    uint8_t ORIE    :1;                                       
    /** Reserved */                                      
    uint8_t         :1; 
    /** TxD1 Pin Direction in Single-Wire Mode */
    uint8_t TXDIR   :1; 
    /** Ninth Data Bit for Transmitter */
    uint8_t R8      :1; 
    /** Ninth Data Bit for Receiver */
    uint8_t T8      :1; 
  } Bits;
} SCIxC3_struct_t;


/**
Struttura dei registri di stato, controllo e dati relativo ai canali SCI 1 e 2 \n
per HCS08
*/
typedef struct
{

uint8_t SCIxBDH;
uint8_t SCIxBDL;
uint8_t SCIxC1;
uint8_t SCIxC2;
uint8_t SCIxS1;
uint8_t SCIxS2;
uint8_t SCIxC3;
uint8_t SCIxD;

}SCIreg_struct_t;


/* ****************** */
/* Exported variables */
/* ****************** */

/** Vettore di corrispondenza porta MCU - pin del modulo linkage-1 */
extern const struct portset_struct ModuleIOmap[LINKAGE_MODULE_NUMOFPIN];

/** Vettore di abilitazione dei canali all'interno del modulo */
extern const uint8_t MCUTPMChEnable[MCU_TOTAL_NUMOFCHANNEL]; 
/** Vettore di configurazione dei canali all'interno del modulo */
extern const uint8_t MCUTPMChConfig[MCU_TOTAL_NUMOFCHANNEL]; 
/** Vettore di configurazione dei registri dei timer e dei pin associati ai canali all'interno del modulo */
extern const struct timerset_struct timersMap[MCU_TOTAL_NUMOFCHANNEL];

/** Vettore di configurazione dei registri della Serial Communication Interface */
extern const uint8_t * sciMap[MCU_NUM_OF_UARTS];

/* ******************* */
/* Function prototypes */
/* ******************* */


/*======================================================================================*/

/* Microcontroller Digital IO Section */


/**
Inizializza le porte di IO

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_InitPort(void)
{ /* do nothing */ }


/**
Deinizializza le porte di IO

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			tutti i registri per la direzione di IO sono settati in ingresso
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_DeInitPort(void)
{

   PTADD = 0x00;	/* Port A - Input Direction */
   PTBDD = 0x00;	/* Port B - Input Direction */
   PTCDD = 0x00;	/* Port C - Input Direction */
   PTDDD = 0x00;	/* Port D - Input Direction */
   PTEDD = 0x00;	/* Port E - Input Direction */
   PTFDD = 0x00;	/* Port F - Input Direction */
   PTGDD = 0x00;	/* Port G - Input Direction */

}


/*--------------------------------------------------------------------------------------*/

/**
Funzioni HAL per la board del modulo linkage.
Permettono di settare le singole porte SOLO attraverso la matrice degli IO della porta ModuleIOMap[].
Il numero di riga della matrice corrisponde al pin del linkage - 1
*/


/**
Impone alto il valore dell'uscita per il pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_SetIOValPin(uint8_t boardpin)
{

  portreg_struct_t *port = (portreg_struct_t *)(ModuleIOmap[boardpin].pData_loc);
  
  uint8_t bit =  (ModuleIOmap[boardpin]).mask;  

  port->data |= bit;

}


/**
Impone basso il valore dell'uscita per il pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent
*/

#pragma INLINE
static void HAL_ResetIOValPin(uint8_t boardpin)
{


  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
   
  uint8_t bit =  ModuleIOmap[boardpin].mask;  
 
  port->data &= ~bit;

}


/**
Legge il valore del pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\return 	bool_t

\retval		0, 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetIOValPin(uint8_t boardpin)
{											 

  portreg_struct_t *port = (portreg_struct_t *)(ModuleIOmap[boardpin].pData_loc);
  
  uint8_t bit =  ModuleIOmap[boardpin].mask;  
  

  if(port->data & bit)
  {
     return 1;
  }
  else
  {
  	 return 0;
  }

}


/**
Impone un pin come uscita

\param  [in]		boardpin	pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_SetIODirPin( uint8_t boardpin)
{   
	
  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
  uint8_t bit = ModuleIOmap[boardpin].mask;	
     
  port->direction |= bit;    

} 


/**
Impone un pin come ingresso

\param  [in]		boardpin	pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetIODirPin( uint8_t boardpin)
{   
	
  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
  uint8_t bit = ModuleIOmap[boardpin].mask;	
          
  port->direction &= ~bit;	

}


/**
Legge il valore del registro di direzione del pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\return 	bool_t

\retval		0, 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetIODirPin(uint8_t boardpin)
{											 

  portreg_struct_t *port = (portreg_struct_t *)(ModuleIOmap[boardpin].pData_loc);
  uint8_t bit =  ModuleIOmap[boardpin].mask;  
  
  if(port->direction & bit)
  {
     return 1;
  }
  else
  {
  	 return 0;
  }
  
}


/**
Setta il pullup per il pin selezionato.

\param  [in]		boardpin  pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_SetIOPullupPin(uint8_t boardpin)
{

  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
  uint8_t bit =  ModuleIOmap[boardpin].mask;  

  port->pull_up |= bit;    

}


/**
Resetta il pullup per il pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetIOPullupPin(uint8_t boardpin)
{

  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
  uint8_t bit =  ModuleIOmap[boardpin].mask;  

  port->pull_up &= ~bit;

}


/**
Legge il valore inserito nel registro di pull up per il pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\return 	bool_t

\retval		0, 1
\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetIOPullupPin(uint8_t boardpin)
{											 

  portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[boardpin].pData_loc;
  uint8_t bit =  ModuleIOmap[boardpin].mask;  
  

  if(port->pull_up & bit)
  {
     return 1;
  }
  else
  {
  	 return 0;
  }
  
}


/**
Restituisce il valore della maschera per il pin selezionato.

\param  [in]		boardpin	pin del linkage - 1

\return 	uint8_t

\retval		valore della maschera (da 0x00 a 0xFF)

\attention \b ModuleIOmap[] (global) in mappatura pin micro modulo linkage

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint8_t HAL_GetIOMask(uint8_t boardpin)
{											 

  return ModuleIOmap[boardpin].mask;  

}


/*--------------------------------------------------------------------------------------*/

/**
Funzioni HAL per la Mcu: scritte per permettere il debug con la maggior flessibilità possibile,
permettendo di settare le singole porte indipendentemente dalla matrice ModuleIOMap della porta.
Particolarmente utile quando, durante il debug, si fa ricorso a pin non disponibili sulla
linkage board.
*/

#ifdef DIO_DEBUG

/**
Setta i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_SetMcuIOPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTAD |= mcupin;
                  break;
    case DIO_PORTB : PTBD |= mcupin;
                  break;                                                   
    case DIO_PORTC : PTCD |= mcupin;
                  break;
    case DIO_PORTD : PTDD |= mcupin;
                  break;
    case DIO_PORTE : PTED |= mcupin;
                  break;
    case DIO_PORTF : PTFD |= mcupin;
                  break;
    case DIO_PORTG : PTGD |= mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Resetta i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin		maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetMcuIOPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTAD &= ~mcupin;
                  break;
    case DIO_PORTB : PTBD &= ~mcupin;
                  break;                                                   
    case DIO_PORTC : PTCD &= ~mcupin;
                  break;
    case DIO_PORTD : PTDD &= ~mcupin;
                  break;
    case DIO_PORTE : PTED &= ~mcupin;
                  break;
    case DIO_PORTF : PTFD &= ~mcupin;
                  break;
    case DIO_PORTG : PTGD &= ~mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Legge il valore dei pin di un'intera porta

\param  [in]		port		identificatore della porta

\return 	uint8_t

\retval		valore dei bit della porta (da 0x00 a 0xFF)

\attention \b Accesso diretto al microcontrollore

\return 	Le dimensioni del parametro ritornato sono pari alle dimensioni di una intera porta (GB60: 8 bit)

\retval		none

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static uint8_t HAL_GetMcuIOVal(uint8_t port)
{											 

uint8_t portValue;
switch (port) {
    case DIO_PORTA : portValue = PTAD;
                  break;
    case DIO_PORTB : portValue = PTBD;
                  break;                                                   
    case DIO_PORTC : portValue = PTCD;
                  break;
    case DIO_PORTD : portValue = PTDD;
                  break;
    case DIO_PORTE : portValue = PTED;
                  break;
    case DIO_PORTF : portValue = PTFD;
                  break;
    case DIO_PORTG : portValue = PTGD;
                  break;                                                      
    default:  break; /*gestione errore*/
  }

 return portValue;
}


/**
Legge il valore del pin selezionato di una porta

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin  

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetMcuIOValPin(uint8_t port, uint8_t mcupin)
{											 

bool_t portBitValue;
switch (port) {
    case DIO_PORTA : portBitValue = PTAD & mcupin;
                  break;
    case DIO_PORTB : portBitValue = PTBD & mcupin;
                  break;                                                   
    case DIO_PORTC : portBitValue = PTCD & mcupin;
                  break;
    case DIO_PORTD : portBitValue = PTDD & mcupin;
                  break;
    case DIO_PORTE : portBitValue = PTED & mcupin;
                  break;
    case DIO_PORTF : portBitValue = PTFD & mcupin;
                  break;
    case DIO_PORTG : portBitValue = PTGD & mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }

 return portBitValue;
}


/**
Impone i pin di un'intera porta come uscite usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_SetMcuIODirPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTADD |= mcupin;
                  break;
    case DIO_PORTB : PTBDD |= mcupin;
                  break;                                                   
    case DIO_PORTC : PTCDD |= mcupin;
                  break;
    case DIO_PORTD : PTDDD |= mcupin;
                  break;
    case DIO_PORTE : PTEDD |= mcupin;
                  break;
    case DIO_PORTF : PTFDD |= mcupin;
                  break;
    case DIO_PORTG : PTGDD |= mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Impone i pin di un'intera porta come ingressi usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetMcuIODirPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTADD &= ~mcupin;
                  break;
    case DIO_PORTB : PTBDD &= ~mcupin;
                  break;                                                   
    case DIO_PORTC : PTCDD &= ~mcupin;
                  break;
    case DIO_PORTD : PTDDD &= ~mcupin;
                  break;
    case DIO_PORTE : PTEDD &= ~mcupin;
                  break;
    case DIO_PORTF : PTFDD &= ~mcupin;
                  break;
    case DIO_PORTG : PTGDD &= ~mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
}


/**
Legge il valore della direzione della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin  

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetMcuIODirPin(uint8_t port, uint8_t mcupin)
{
  
  bool_t portPinDirection;

  switch (port) {
    case DIO_PORTA :  portPinDirection = PTADD & mcupin;
                  break;
    case DIO_PORTB :  portPinDirection = PTBDD & mcupin;
                  break;
    case DIO_PORTC :  portPinDirection = PTCDD & mcupin;
                  break;                                          
    case DIO_PORTD :  portPinDirection = PTDDD & mcupin;
                  break;
    case DIO_PORTE :  portPinDirection = PTEDD & mcupin;
                  break;
    case DIO_PORTF :  portPinDirection = PTFDD & mcupin;
                  break;
    case DIO_PORTG :  portPinDirection = PTGDD & mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
  return portPinDirection;
}


/**
Setta il pullup per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_SetMcuIOPullupPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTAPE |= mcupin;
                  break;
    case DIO_PORTB : PTBPE |= mcupin;
                  break;                                                   
    case DIO_PORTC : PTCPE |= mcupin;
                  break;
    case DIO_PORTD : PTDPE |= mcupin;
                  break;
    case DIO_PORTE : PTEPE |= mcupin;
                  break;
    case DIO_PORTF : PTFPE |= mcupin;
                  break;
    case DIO_PORTG : PTGPE |= mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Resetta il pullup per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetMcuIOPullupPin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTAPE &= ~mcupin;
                  break;
    case DIO_PORTB : PTBPE &= ~mcupin;
                  break;                                                   
    case DIO_PORTC : PTCPE &= ~mcupin;
                  break;
    case DIO_PORTD : PTDPE &= ~mcupin;
                  break;
    case DIO_PORTE : PTEPE &= ~mcupin;
                  break;
    case DIO_PORTF : PTFPE &= ~mcupin;
                  break;
    case DIO_PORTG : PTGPE &= ~mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Legge il valore di pullup della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin  

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetMcuIOPullupPin(uint8_t port, uint8_t mcupin)
{
  
  bool_t PortPinPullup;

  switch (port) {
    case DIO_PORTA :  PortPinPullup = PTAPE & mcupin;
                  break;
    case DIO_PORTB :  PortPinPullup = PTBPE & mcupin;
                  break;
    case DIO_PORTC :  PortPinPullup = PTCPE & mcupin;
                  break;                                          
    case DIO_PORTD :  PortPinPullup = PTDPE & mcupin;
                  break;
    case DIO_PORTE :  PortPinPullup = PTEPE & mcupin;
                  break;
    case DIO_PORTF :  PortPinPullup = PTFPE & mcupin;
                  break;
    case DIO_PORTG :  PortPinPullup = PTGPE & mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
  return PortPinPullup;
}


/**
Setta lo slew-rate per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_SetMcuIOSlewratePin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTASE |= mcupin;
                  break;
    case DIO_PORTB : PTBSE |= mcupin;
                  break;                                                   
    case DIO_PORTC : PTCSE |= mcupin;
                  break;
    case DIO_PORTD : PTDSE |= mcupin;
                  break;
    case DIO_PORTE : PTESE |= mcupin;
                  break;
    case DIO_PORTF : PTFSE |= mcupin;
                  break;
    case DIO_PORTG : PTGSE |= mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Resetta lo slew-rate per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void HAL_ResetMcuIOSlewratePin(uint8_t port, uint8_t mcupin)
{   
	
  switch (port) {
    case DIO_PORTA : PTASE &= ~mcupin;
                  break;
    case DIO_PORTB : PTBSE &= ~mcupin;
                  break;                                                   
    case DIO_PORTC : PTCSE &= ~mcupin;
                  break;
    case DIO_PORTD : PTDSE &= ~mcupin;
                  break;
    case DIO_PORTE : PTESE &= ~mcupin;
                  break;
    case DIO_PORTF : PTFPE &= ~mcupin;
                  break;
    case DIO_PORTG : PTGPE &= ~mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
} 


/**
Legge il valore di slew-rate della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin  

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static bool_t HAL_GetMcuIOSlewratePin(uint8_t port, uint8_t mcupin)
{
  
  bool_t PortPinSlewrate;

  switch (port) {
    case DIO_PORTA :  PortPinSlewrate = PTASE & mcupin;
                  break;
    case DIO_PORTB :  PortPinSlewrate = PTBSE & mcupin;
                  break;
    case DIO_PORTC :  PortPinSlewrate = PTCSE & mcupin;
                  break;                                          
    case DIO_PORTD :  PortPinSlewrate = PTDSE & mcupin;
                  break;
    case DIO_PORTE :  PortPinSlewrate = PTESE & mcupin;
                  break;
    case DIO_PORTF :  PortPinSlewrate = PTFSE & mcupin;
                  break;
    case DIO_PORTG :  PortPinSlewrate = PTGSE & mcupin;
                  break;                                                      
    default:  break; /*gestione errore*/
  }
  return PortPinSlewrate;
} 

#endif /* DIO_DEBUG */


/**
Restituisce il valore del pin prefissato per i segnali di interrupt

\return 	uint8_t

\retval		valore del pin per i segnali di interrupt

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static uint8_t HAL_GetIOIRQPin(void)
{
  
  return IRQID_MCU;

}


/*======================================================================================*/

/* Microcontroller Timers Section */

/** Timer enabling */

/* Come gestire il settaggio del clock? */

/* Timer - Disable */

/**
Disabilita il timer

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/


#pragma INLINE
static void TimerDisable(uint8_t timerId)
{ 

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.CLKSA=0;
	(*pTPMySCreg).Bits.CLKSB=0;

}


/* Timer - Enable */

/* Timer enabled */
#pragma INLINE
static void	TimerEnable(uint8_t timer_num)
{ /* do nothing */ }


/* Timer enabled using bus clock */

/**
Abilita il timer con frequenza pari a quella del bus clock

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerEnableBusClock(uint8_t timerId)
{ 

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.CLKSA=1;
	(*pTPMySCreg).Bits.CLKSB=0;
	
}


/* Timer enabled using fixed system clock */

/**
Abilita il timer con una frequenza prefissata (XCLK: fixed system clock)

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerEnableSystemClock(uint8_t timerId)
{ 

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.CLKSA=0;
	(*pTPMySCreg).Bits.CLKSB=1;

}


/* Timer enabled using external clock source */

/**
Abilita il timer usando un generatore di frequenza esterno

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerEnableExtClock(uint8_t timerId)
{ 

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.CLKSA=1;
	(*pTPMySCreg).Bits.CLKSB=1;

}


/** Configuration */

/**
Setta il prescaler del timer

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerSetPrescaler(uint8_t timerId, uint8_t prescaler)
{ 

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
//  	bool_t bit0, bit1, bit2, found;
  	
  	bool_t found;
  	uint8_t i,power;
  	
  	/*switch (prescaler) {
    	case 1 :  	bit0 = 0;
    				bit1 = 0;
					bit2 = 0;
					  break;
	    case 2 :  	bit0 = 1;
    				bit1 = 0;
					bit2 = 0;
					  break;
	    case 4 :  	bit0 = 0;
    				bit1 = 1;
					bit2 = 0;
					  break;
	    case 8 :  	bit0 = 1;
    				bit1 = 1;
					bit2 = 0;
					  break;                                       
    default:  break; 
    }								 */
    
    i=0;
  	power=1;
  	found = FALSE;
    
    while((i<8)&&(!found))
    {
      if(power == prescaler)
      {
        found = TRUE;
        break;     
      }
      
      power *= 2;
      i++;
    
    }
    
    if(found == TRUE)
    {
           
        (*pTPMySCreg).Bits.PS0 = (i & 1);
	      (*pTPMySCreg).Bits.PS1 = (i & 2)>>1;
	      (*pTPMySCreg).Bits.PS2 = (i & 4)>>2;
    
    }

}


/**
Abilita l'OverFlow interrupt del timer

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerEnableOFInterrupt(uint8_t timerId)
{
	
	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.TOIE = 1;

}

/**
Disabilita l'OverFlow interrupt del timer

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerDisableOFInterrupt(uint8_t timerId)
{
	
	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.TOIE = 0;

}

/**
Resetta il flag di OverFlow del timer

\param  [in]		timerId		identificatore del timer

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerResetOFFlag(uint8_t timerId)
{
	
	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	(*pTPMySCreg).Bits.TOF = 0;

}


/**
Legge il valore del flag di OverFlow del timer

\param  [in]		timerId		identificatore della porta

\return 	bool_t

\retval		0, 1

\attention \b

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t TimerGetOFFlag(uint8_t timerId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[timerId].pTPMySC;
	
	return (*pTPMySCreg).Bits.TOF;

}


/**
Disabilita il canale di un timer (il pin associato può essere usato come GPIO o come input per un clock esterno)

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChDisable(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	/* Basterebbero questi due bit a 0 per disabilitare il canale e usare in pin come I/O generico */
	//(*pTPMyCxSCreg).Bits.ELSxA = 0;
	//(*pTPMyCxSCreg).Bits.ELSxB = 0;
	
  /* Azzero tutto il registro TPMyCxSC per disabilitare il canale come fatto in passato in MotorGateDriver.c */
  (*pTPMyCxSCreg).all_Data = TIMER_CHANNEL_DISABLE;

}


/**
Setta il canale di un timer in Input Capture Rising Edge

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChICRisingEdge(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	
	(*pTPMySCreg).Bits.CPWMS = 0;

	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	(*pTPMyCxSCreg).Bits.ELSxB = 0;
	(*pTPMyCxSCreg).Bits.MSxA = 0;
	(*pTPMyCxSCreg).Bits.MSxB = 0;
	
}


/**
Setta il canale di un timer in Input Capture Falling Edge

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChICFallingEdge(uint8_t channelId)
{
	
	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	(*pTPMySCreg).Bits.CPWMS = 0;

	(*pTPMyCxSCreg).Bits.ELSxA = 0;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;
	(*pTPMyCxSCreg).Bits.MSxA = 0;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in Input Capture Both Edge

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChICBothEdge(uint8_t channelId)
{
	
	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;


	(*pTPMySCreg).Bits.CPWMS = 0;
	
	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;
	(*pTPMyCxSCreg).Bits.MSxA = 0;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in Output - Compare solo software senza muovere alcun pin

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChOCSWOnly(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	

	(*pTPMySCreg).Bits.CPWMS = 0;

	(*pTPMyCxSCreg).Bits.ELSxA = 0;
	(*pTPMyCxSCreg).Bits.ELSxB = 0;
	(*pTPMyCxSCreg).Bits.MSxA = 1;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in Output Compare Toggle Output

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChOCToggleOutput(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 0;
	
	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	(*pTPMyCxSCreg).Bits.ELSxB = 0;
	(*pTPMyCxSCreg).Bits.MSxA = 1;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in Output Compare Reset Output

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChOCResetOutput(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 0;
	
	(*pTPMyCxSCreg).Bits.ELSxA = 0;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;
	(*pTPMyCxSCreg).Bits.MSxA = 1;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in Output Compare Set Output

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChOCSetOutput(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 0;
		
	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;
	(*pTPMyCxSCreg).Bits.MSxA = 1;
	(*pTPMyCxSCreg).Bits.MSxB = 0;

}


/**
Setta il canale di un timer in PWM Edge Aligned High True

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChSetEdgeAlignedPWMHighTrue(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;


	(*pTPMySCreg).Bits.CPWMS = 0;
	
	(*pTPMyCxSCreg).Bits.MSxB = 1;
		
	(*pTPMyCxSCreg).Bits.ELSxA = 0;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;
	//(*pTPMyCxSCreg).Bits.MSxA = 0;
	

}


/**
Setta il canale di un timer in PWM Edge Aligned Low True

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChSetEdgeAlignedPWMLowTrue(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 0;	

	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	//(*pTPMyCxSCreg).Bits.ELSxB = 0;
	//(*pTPMyCxSCreg).Bits.MSxA = 0;
	(*pTPMyCxSCreg).Bits.MSxB = 1;

}


/**
Setta il canale di un timer in PWM Center Aligned High True

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChSetCenterAlignedPWMHighTrue(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 1;	

	(*pTPMyCxSCreg).Bits.ELSxA = 0;
	(*pTPMyCxSCreg).Bits.ELSxB = 1;

}


/**
Setta il canale di un timer in PWM Center Aligned Low True

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChSetCenterAlignedPWMLowTrue(uint8_t channelId)
{

	TPMySC_struct_t *pTPMySCreg = (TPMySC_struct_t *)timersMap[channelId].pTPMySC;
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMySCreg).Bits.CPWMS = 1;
		
	(*pTPMyCxSCreg).Bits.ELSxA = 1;
	//(*pTPMyCxSCreg).Bits.ELSxB = 0;

}


/**
Inverte la sensibilità sul fronte

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChToggleEdge(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;

	(*pTPMyCxSCreg).all_Data ^= 0x0C;

}

/**
Legge il valore dei bit di configurazione del fronte per un canale del timer

\param  [in]		channelId	identificatore del canale

\return 	uint8_t

\retval		0x00, 0x04, 0x08, 0x0C

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint8_t TimerChGetEdge(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	return (*pTPMyCxSCreg).all_Data & 0x0C;

}


/**
Resetta il flag di evento per un canale del timer

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

/* Timer Channel Event Flag */
#pragma INLINE
static void TimerChResetEventFlag(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	(*pTPMyCxSCreg).Bits.CHxF = 0;

}


/**
Legge il valore del flag di configurazione del fronte per un canale del timer

\param  [in]		channelId	identificatore del canale
		
\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t TimerChGetEventFlag(uint8_t channelId)
{
	
	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	return (*pTPMyCxSCreg).Bits.CHxF;
}


/* Timer Channel interrupt management */

/**
Abilita l'interrupt a seguito di un evento per un canale del timer

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChEnableInterrupt(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	(*pTPMyCxSCreg).Bits.CHxIE = 1;
	
}


/**
Disabilita l'interrupt a seguito di un evento per un canale del timer

\param  [in]		channelId		identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerChDisableInterrupt(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	(*pTPMyCxSCreg).Bits.CHxIE = 0;
}


/**
Legge il valore del bit di configurazione dell'interrupt a seguito di un evento per un canale del timer

\param  [in]		channelId	identificatore del canale
\param  [out]		bool_t		  valore dei bit di configurazione dell'interrupt

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t TimerChGetInterrupt(uint8_t channelId)
{

	TPMyCxSC_struct_t *pTPMyCxSCreg = (TPMyCxSC_struct_t *)timersMap[channelId].pTPMyCxSC;
	
	return (*pTPMyCxSCreg).Bits.CHxIE;
	
}


/**
Setta il valore massimo del contatore
\param  [in]		channelId	identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerSetCounterModuloValue(uint8_t timer_num, uint16_t counterModulo)
{
	
	if(timersMap[timer_num].pTPMySC == &TPM1SC)
	{
		TPM1MOD = counterModulo;
	}
	else
	{
		TPM2MOD = counterModulo;	
	}

}


/**
Legge il valore del contatore per un timer
\param  [in]		channelId	identificatore del canale

\return  uint16_t

\retval  0x0000 - 0xFFFF

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint16_t TimerGetCounterValue(uint8_t channelId)
{
	
	uint16_t channelValue;
	//uint8_t *reg_addr = (uint8_t *)timersMap[channelId].pTPMySC;
	
	switch(channelId)
	{

		case(0): channelValue = TPM1CNT ;  break;
		
		case(1): channelValue = TPM1CNT ;  break;
		
		case(2): channelValue = TPM2CNT ;  break;
																		
		case(3): channelValue = TPM2CNT ;  break;
		
		case(4): channelValue = TPM2CNT ;  break;
		
		case(5): channelValue = TPM2CNT ;  break;
		
		case(6): channelValue = TPM2CNT ;  break;

		default: break;
		
	}

  return channelValue;

}


/**
Setta il valore di confronto del contatore per un canale
\param  [in]		channelId	identificatore del canale

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void TimerSetChannelValue(uint8_t channelId, uint16_t channelValue )
{
	
	//uint8_t *reg_addr = (uint8_t *)timersMap[channelId].pTPMySC;
	
	switch(channelId)
	{

		case(0): TPM1C1V = channelValue;  break;
		
		case(1): TPM1C2V = channelValue;  break;
		
		case(2): TPM2C0V = channelValue;  break;
		
		case(3): TPM2C1V = channelValue;  break;
		
		case(4): TPM2C2V = channelValue;  break;
		
		case(5): TPM2C3V = channelValue;  break;
		
		case(6): TPM2C4V = channelValue;  break;

		default: break;
		
	}

}


/**
Legge il valore di confronto del contatore per un canale
\param  [in]		channelId	identificatore del canale

\return  uint16_t

\retval  0x0000 - 0xFFFF

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint16_t TimerGetChannelValue(uint8_t channelId)
{
	
	uint16_t channelValue;
	//uint8_t *reg_addr = (uint8_t *)timersMap[channelId].pTPMySC;
	
	switch(channelId)
	{

		case(0): channelValue = TPM1C1V ;  break;
		
		case(1): channelValue = TPM1C2V ;  break;
		
		case(2): channelValue = TPM2C0V ;  break;
																		
		case(3): channelValue = TPM2C1V ;  break;
		
		case(4): channelValue = TPM2C2V ;  break;
		
		case(5): channelValue = TPM2C3V ;  break;
		
		case(6): channelValue = TPM2C4V ;  break;

		default: break;
		
	}

  return channelValue;

}


/**
Verifica se un canale è configurato
 
\param  [in]		factor	fattore di correzione

\param [in]	  *pTPMParam puntatore alla struttura che contiene i parametri di setting

\return     bool_t

\retval - TRUE  canale configurato
\retval - FALSE canale non configurato

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t TimerIsChannelConfigured (uint8_t channelId )
{
	
//	bool_t verof;
  switch(channelId)
	{

		case(0): return (((TPM1C1SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
		
		case(1): return (((TPM1C2SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
		
		case(2): return (((TPM2C0SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
																		
		case(3): return (((TPM2C1SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
		
		case(4): return (((TPM2C2SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
		
		case(5): return (((TPM2C3SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;
		
		case(6): return (((TPM2C4SC & (~0x80)) == TIMER_CHANNEL_DISABLE)?(FALSE):(TRUE));  break;

		default: break;
		
	}

//  return verof;
}


/**
Verifica la configurazione di un canale di un timer

\param  [in] 	*pTPMParam puntatore alla struttura che contiene i parametri di setting

\return     bool_t

\retval - TRUE  canale configurato in maniera diversa da quella voluta
\retval - FALSE canale configurato im maniera uguale a quella voluta

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			
*/

#pragma INLINE
static bool_t TimerChConfigChanged(uint8_t channelId, uint8_t config)
{
	
	switch(channelId)
	{

		case(0): return (((TPM1C1SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
		
		case(1): return (((TPM1C2SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
		
		case(2): return (((TPM2C0SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
																		
		case(3): return (((TPM1C2SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
		
		case(4): return (((TPM2C2SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
		
		case(5): return (((TPM2C3SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;
		
		case(6): return (((TPM2C4SC & (~0x80) ) != config)?(TRUE):(FALSE));  break;

		default: break;
		
	}
  
}


/**
Calcola e ritorna il valore di correzione del timer

\param  [in]		factor	fattore di correzione

\return  uint8_t

\retval  1

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint8_t TimerCorrectionFactor(uint8_t factor)
{
  return 1;
}


/*======================================================================================*/

/* Microcontroller Serial Communication Interface (SCI or UART) Section */


/**
Imposta il valore del divisore del Baud Rate

\param [in]		pIOdp puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting
\attention  BRdivisor vaore a 16 bit; utilizzati gli 8 superiori o inferiori nelle due istruzioni, senza, in realtà, perdita di informazioni.
\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			
*/

#pragma INLINE
static void SCISetBRDivisor(uint8_t port, uint16_t BRdivisor)
{

 	SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];

  (*pSCIreg).SCIxBDH = (BRdivisor>>8);
  (*pSCIreg).SCIxBDL = BRdivisor; 
	
}


/**
Ritorna il valore di della porta UART usata

\param  [in]		boardPin	pin del modulo linkage -1

\return  uint8_t

\retval  0  Per GB60 il buzzer è pilotato tramite i timer e non tramite la UART -> UART non usata

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static uint8_t SCIGetPort(uint8_t boardPin)
{
    
  /* Per GB60 il buzzer è pilotato tramite i timer e non tramite la UART -> UART non usata */
  return 0;

}


/**
Resetta lo stato della porta UART

\param  [in]		port	identificativo dell'interfaccia UART

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Per GB60 si ottiene effettuando una lettura del registro SCIxS1
			\par \b Nota 3:
			Target dependent

*/

#pragma INLINE
static void SCIResetStatus(uint8_t port)
{

  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];
	
  (*((SCIxS1_struct_t *) &((*pSCIreg).SCIxS1))).all_Data;

}


/**
Carica i nuovi dati da trasmettere
 
\param [in]		port	identificativo dell'interfaccia UART
\param [in]		data  dati da trasmettere
 
\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void SCILoadDataToTx(uint8_t port, uint8_t data)
{
  
  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];
  
  (*pSCIreg).SCIxD = data;
  
}


/**
Abilita l'interrupt in trasmissione

\param  [in]		port	identificativo dell'interfaccia UART

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void SCIEnableInterruptOnTx(uint8_t port)
{
  
  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];

  (*((SCIxC2_struct_t *) &((*pSCIreg).SCIxC2))).Bits.TIE = 1;

}


/**
Disabilita l'interrupt in trasmissione

\param  [in]		port	identificativo dell'interfaccia UART

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void SCIDisableInterruptOnTx(uint8_t port)
{
  
  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];

  (*((SCIxC2_struct_t *) &((*pSCIreg).SCIxC2))).Bits.TIE = 0;

}


/**
Abilita il trasmettitore

\param  [in]		port	identificativo dell'interfaccia UART

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void SCIEnableTransmitter(uint8_t port)
{
  
  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];

  (*((SCIxC2_struct_t *) &((*pSCIreg).SCIxC2))).Bits.TE = 1;

}


/**
Disabilita il trasmettitore

\param  [in]		port	identificativo dell'interfaccia UART

\author 	Pierluigi Ianni
\date		27/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static void SCIDisableTransmitter(uint8_t port)
{
  
  SCIreg_struct_t *pSCIreg = (SCIreg_struct_t *) sciMap[port];

  (*((SCIxC2_struct_t *) &((*pSCIreg).SCIxC2))).Bits.TE = 0;

}



/*======================================================================================*/

/* Microcontroller Interrupt Register Section */


/**
Legge il valore del bit di Edge nel registro degli interrupt

\param  [out]		bool_t  valore dei bit Edge nel registro degli interrupt

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			Target dependent

*/

#pragma INLINE
static bool_t IRQGetEdge()
{

	return IRQSC_IRQEDG;
	
}

#endif /* #if 0*/

#endif /* __HAL_FREESCALE_HCS08_GB60_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */