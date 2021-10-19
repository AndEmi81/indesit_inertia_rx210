/**
\defgroup HAL_FS_MC9S08_QE128  Freescale HCS08 QE128 HAL
\ingroup FS_MC9S08_QE128
*/

/**
\defgroup HAL_FS_MC9S08_QE128_CLS  Freescale HCS08 QE128 supported drivers Classes


This module lists the driver classes supported by this HAL.

\date    14/04/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_MC9S08_QE128
*/

/**
\defgroup HAL_FS_MC9S08_QE128_HISTORY  Freescale HCS08 QE128 History
\ingroup HAL_FS_MC9S08_QE128
This module contains the history

\defgroup HAL_FS_MC9S08_QE128_HISTORY_HAL  Freescale HCS08 QE128 HAL History
\ingroup  HAL_FS_MC9S08_QE128_HISTORY
*/

/**
HAL implementation file

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti

\file    FS_MC9S08_QE128_hal.h
\ingroup HAL_FS_MC9S08_QE128
\date    13/05/2009
\author  Roberto Fioravanti
\version 00.00.01


\addtogroup HAL_FS_MC9S08_QE128_HISTORY_HAL
@{
\n 		00.00.14
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- Pin Port defines for 'ModuleIOMap' table (to be defined in config.c file) added.
\n		- typedef portreg_struct_t for access to port data register  and direction register.
\n
\n 		00.00.12
\n		08/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- Introduzione defines generali:  _NOP_(), SW_RESET_MCU(). Eliminazione ASM_NOP
\n 		- I2C: _HAL_S08_I2C_CTRL_REG, I2C_CtrlReset(), I2C_IntRegReset(), I2C_ResetStatus(), I2C_ResetTxSig().
\n
\n 		00.00.08 
\n		13/05/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- Corrette le define _HAL_S08_I2C_SDA_IO_REG e _HAL_S08_I2C_SCL_IO_REG per testare correttamente le porte di SDA SCL
\n 		- 
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
\addtogroup HAL_FS_MC9S08_QE128_CLS
@{*/



#ifndef __HAL_FREESCALE_MC9S08_QE128_H
#define __HAL_FREESCALE_MC9S08_QE128_H





/* ******** */
/* Includes */
/* ******** */
#include "FS_MC9S08_QE128_reg.h"

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

/** \defgroup HAL_FS_MC9S08_QE128_I2C  I2C Class driver support    
*/


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR                        (0)
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK                (1)
#define OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK                               (1)


/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY

/** 
@name I2C related registers redefinition                           
This approach helps when generating new files for additional parts of the same family.
@{
*/
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
#define _HAL_S08_I2C_SDA_IO_REG                     (PTAD_PTAD2)
#define _HAL_S08_I2C_SCL_IO_REG                     (PTAD_PTAD3)


/*******     INSERITO SPISW   ************************************/

#define _HAL_S08_SPIHW2_EN_REG                      (SPI2C1_SPE)
#define _HAL_S08_SCI1_EN_TX_REG                     (SCI1C2_TE)
#define _HAL_S08_SCI1_EN_RX_REG                     (SCI1C2_RE)
#define _HAL_S08_KBI1_PE5_REG                       (KBI1PE_KBIPE5)
#define _HAL_S08_KBI1_PE4_REG                       (KBI1PE_KBIPE4)
#define _HAL_S08_KBI1_PE0_REG                       (KBI1PE_KBIPE0)
#define _HAL_S08_KBI2_PE2_REG                       (KBI2PE_KBIPE2)
#define _HAL_S08_ADP_PC5_REG                        (APCTL1_ADPC5)
#define _HAL_S08_ADP_PC4_REG                        (APCTL1_ADPC4)
#define _HAL_S08_ADP_PC10_REG                       (APCTL2_ADPC10)


#define _HAL_S08_PTB0_GPIO_DIR_REG                  (PTBDD_PTBDD0)
#define _HAL_S08_PTB1_GPIO_DIR_REG                  (PTBDD_PTBDD1)
#define _HAL_S08_PTD0_GPIO_DIR_REG                  (PTDDD_PTDDD0)
#define _HAL_S08_PTD2_GPIO_DIR_REG                  (PTDDD_PTDDD2)
#define _HAL_S08_PTF0_GPIO_DIR_REG                  (PTFDD_PTFDD0)

#define _HAL_S08_PTB0_GPIO_DATA_REG                 (PTBD_PTBD0)
#define _HAL_S08_PTB1_GPIO_DATA_REG                 (PTBD_PTBD1)
#define _HAL_S08_PTD0_GPIO_DATA_REG                 (PTDD_PTDD0)
#define _HAL_S08_PTD2_GPIO_DATA_REG                 (PTDD_PTDD2)
#define _HAL_S08_PTF0_GPIO_DATA_REG                 (PTFD_PTFD0)






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
#define     I2C_EnableAcknowledge()         {   _HAL_S08_I2C_TXACKBITVAL_REG = 0; }
/* */
#define     I2C_DisableAcknowledge()        {   _HAL_S08_I2C_TXACKBITVAL_REG = 1; }
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
    		
#define OCI2C_SlaveAddressedTx_Action()


/* This section contains HAL class extension */
#if (I2C_NESTING_ISR_ENABLE == TRUE)
#define I2C_NestedIsr_Begin() \
  if((I2C_GetStatus() & IIC1S_IAAS_MASK) == 0) { \
    I2C_IntDisable();     /* Disable Int I2c */\
    {__asm CLI;}          /* Enable global Int for nested interrupts */\
  }

#define I2C_NestedIsr_End() \
  { __asm SEI;}            /* Disable global Int for nested interrupts */\
  I2C_IntEnable()          /* Enable Int I2c */

#else

#define I2C_NestedIsr_Begin()

#define I2C_NestedIsr_End()

#endif /* (I2C_NESTING_ISR_ENABLE == TRUE) */

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

/** \defgroup HAL_FS_MC9S08_QE128_SYS  System Class driver support     
*/

/* */
#define _NOP_() asm("nop")

/* Reset Generation to be defined in config.h */
#ifndef SW_RESET_MCU
#define SW_RESET_MCU()
#endif

/** 
\def DELAY_MICROSEC 
us nop cycles conversion according to the bus frequency.

\todo rivedere il calcolo.
*/
#if (BUS_FREQUENCY_Hz == 10000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = �s di ritardo fisso dovuto alla chiamata della routine
  // *1000/2614 = moltiplicatore di 2,614�s che � la durata misurata per una singola iterazione del loop
  #define DELAY_MICROSEC(x)	((((long)x-3)*1000)/2614)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */

#if (BUS_FREQUENCY_Hz == 16000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = �s di ritardo fisso dovuto alla chiamata della routine
  // *1000/2614 = moltiplicatore di 2,614�s che � la durata misurata per una singola iterazione del loop
  #define DELAY_MICROSEC(x)	((((long)x-3)*1000)/1634)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */


#if (BUS_FREQUENCY_Hz == 20000000)
  /* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
  // 4 = �s di ritardo fisso dovuto alla chiamata della routine
  // *1000/1307 = moltiplicatore di 1,307�s che � la durata misurata per una singola iterazione del loop
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
/** \defgroup HAL_FS_MC9S08_QE128_IO  IO Class driver support
 @{
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
#define PIN_DUMMY\
{\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,(DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y)\
{\
{(DIO_PortAddressBasicType)&PT##x##D,(DIO_PortAddressExtendedType)&PT##x##PE},\
(DIO_ModuleMapMaskType)PT##x##D_PT##x##D##y##_MASK\
}
/* sharing between dataInput and dataOutput */
#define dataOutput dataInput
typedef struct
{
    uint8_t dataInput;
    uint8_t direction;
}portreg_struct_t;

typedef struct
{
    uint8_t pullup;     
    uint8_t slewRate;
    uint8_t driveStrength;    
}portreg_ext_struct_t;

#pragma INLINE
#pragma NO_LOOP_UNROLL
static void DeInitGPIO(void)
{
uint8_t *pByte;
uint8_t i;

    /* Clear Direction Reg. */
    PTADD = 0x00;
    PTBDD = 0x00;
    PTCDD = 0x00;
    PTDDD = 0x00;
    PTEDD = 0x00;
    PTFDD = 0x00;
    PTGDD = 0x00;
    PTHDD = 0x00;
    PTJDD = 0x00;
        
    pByte = &PTAPE;
    
    for(i=0;i<9;i++)
    {
        *pByte          = 0x00; /* Clear Pull Up Reg.   */ 
        *(pByte+1)      = 0x00; /* Clear Slew Rate Reg. */
        *(pByte+2)      = 0x00; /* Clear Drive Strength Reg. */
        pByte +=4;              /* Point to next Pull Up Reg. */    
    }    
}

#define DIO_PULL_UP_IS_AVAILABLE	TRUE
//#pragma INLINE
#define DIO_IsAvailablePullUp(ptr) (1)
#define DIO_IsAvailablePullDown(ptr) (0)
#define IsAvailableAsInput(ptr)            (1)
/*Slew rate control of the port is available*/
#define DIO_IsAvailableSlewRate(ptr)      (1)
#define DIO_IsAvailableDriveStrength(ptr)      (1)
#define DIO_IsPullUpEnabled(ptr)\
    ((((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup & ptr->PinMask) != 0)
#define DIO_IsOuput(ptr)\
    (((portreg_struct_t *)ptr->PortAddress.basic)->direction & ptr->PinMask)
/*
\param [in] ptr points to IO_ModuleIOMapType.
*/
#define IsAvailableAsOutput(ptr)            (1)
#define DIO_IsInterruptAvailable(ptr)       (0)
#define DIO_IsDigitalFilterAvailable(ptr)       (0)
#define DIO_IsInterruptFlag(pMap) /*empty*/
#define DIO_PullUpEnable(ptr)   ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->pullup |= (ptr)->PinMask;
#define DIO_PullUpDisable(ptr)   ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->pullup &= ~(ptr)->PinMask;


#define DIO_SetDirAsOutput(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->direction |= ptr->PinMask;  
#define DIO_SetDirAsInput(ptr)\
    ((portreg_struct_t *)ptr->PortAddress.basic)->direction &= ~(ptr->PinMask);  
#define DIO_SetInterruptMode_FallingEdge(ptr) /*empty*/
#define DIO_SetInterruptMode_RisingEdge(ptr) /*empty*/
#define DIO_IsInterruptMode_RisingEdge(ptr) /*empty*/
#define DIO_IsDigitalFilterEnabled(ptr) /*empty*/


#define DIO_DigitalFilterDisable(ptr) /*empty*/
#define DIO_DigitalFilterClkFrequencySet(ptr, filterFrequency) /*empty*/

#define DIO_PULL_DOWN_IS_AVAILABLE	FALSE
/*#pragma INLINE
static bool_t PullDownIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 0;
}*/
#define DIO_INPUT_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t InputIsAvaibleOn(portreg_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}

#define DIO_DigitalFilterClkFrequencyGet(ptr) (0)
#define DIO_DigitalFilterNumSamplesSet(ptr,nSamples) /*empty*/
#define DIO_DigitalFilterNumSamplesGet(ptr) (0)
#define DIO_InterruptEnable(ptr) /*empty*/
#define DIO_InterruptDisable(ptr) /*empty*/
#define DIO_IsInterruptEnabled(ptr) (0)
#define DIO_ClearInterrupt(ptr) /*empty*/




#define DIO_IsDmaAvailable(ptr) (0)


#define DIO_DmaEnable(ptr) /*empty*/
#define DIO_DmaDisable(ptr) /*empty*/
#define DIO_IsDmaEnabled(ptr) /*empty*/
#define DIO_SLEW_RATE_IS_AVAILABLE	TRUE
#pragma INLINE
static bool_t SlewRateIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}

#define DIO_DRIVE_STRENGTH	TRUE
#pragma INLINE
static bool_t DriveStrengthIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
	return 1;
}

#define DIO_LevelDetectInterruptEnable(ptr) /*empty*/
#define DIO_EdgeDetectInterruptEnable(ptr) /*empty*/


#define DIO_GetDetectInterruptMode(ptr) 0
#define DIO_DriveStrengthEnable(ptr)   ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->driveStrength |= (ptr)->PinMask;
#define DIO_DriveStrengthDisable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength &= ~(ptr)->PinMask ;

#define DIO_IsDriveStrengthEnabled(ptr)   (((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength & (ptr)->PinMask)
#define DIO_GetVal(ptr)\
(( ((portreg_struct_t *)ptr->PortAddress.basic)->dataInput & ptr->PinMask ) !=0 )
#define DIO_SetLow(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput &= ~ptr->PinMask;
#define DIO_SetHigh(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput |= ptr->PinMask;
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
/* filterFrequency is in KHz */
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
#define _HAL_S08_SYS_SOPT1_STOPE_BITPOS		5
/** This is the bit mask of STOPE bit */
#define _HAL_S08_SYS_SOPT1_STOPE_MASK   	(1<<_HAL_S08_SYS_SOPT1_STOPE_BITPOS)
/** This is the bit position of BKGDPE bit */
#define _HAL_S08_SYS_SOPT1_BKGDPE_BITPOS	1
/** This is the bit mask of BKGDPE bit */
#define _HAL_S08_SYS_SOPT1_BKGDPE_MASK		(1<<_HAL_S08_SYS_SOPT1_BKGDPE_BITPOS)
/** This is the bit position of RSTPE bit */
#define _HAL_S08_SYS_SOPT1_RSTPE_BITPOS		0
/** This is the bit mask of STOPE bit */
#define _HAL_S08_SYS_SOPT1_RSTPE_MASK   	(1<<_HAL_S08_SYS_SOPT1_RSTPE_BITPOS)
/** This is the bit position of RSTPE bit */
#define _HAL_S08_SYS_SOPT1_RSTOPE_BITPOS	2
/** This is the bit mask of STOPE bit */
#define _HAL_S08_SYS_SOPT1_RSTOPE_MASK   	(1<<_HAL_S08_SYS_SOPT1_RSTOPE_BITPOS)


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

/**
Enable Global Enable Interrupt if enabled before EnterCritical()
*/
#define ExitCritical()\
{\
asm lda CCR_copy;	/* Load CCR_copy into Accumulator*/\
asm tap;			/* Transfer Accumulator into CCR*/\
}
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
/** This is the System Option Register 1 */
#define _HAL_S08_WDT_SOPT1_REG              (SOPT1)
/** This is the System Option Register 2 */
#define _HAL_S08_WDT_SOPT2_REG              (SOPT2)
/** This is the bit position of COPT bit */
#define _HAL_S08_WDT_SOPT1_COPT_BITPOS		6
/** This is the bit mask of COPT bit */
#define _HAL_S08_WDT_SOPT1_COPT_MASK   		(1<<_HAL_S08_WDT_SOPT_COPT_BITPOS)
/** This is the bit position of COPE bit */
#define _HAL_S08_WDT_SOPT1_COPE_BITPOS		7
/** This is the bit mask of COPE bit */
#define _HAL_S08_WDT_SOPT1_COPE_MASK		(1<<_HAL_S08_WDT_SOPT_COPE_BITPOS)
/** This is the bit position of COPE bit */
#define _HAL_S08_WDT_SOPT2_COPCLKS_BITPOS	7
/** This is the bit mask of COPE bit */
#define _HAL_S08_WDT_SOPT2_COPCLKS_MASK		(1<<_HAL_S08_WDT_SOPT2_COPCLKS_BITPOS)

/**
WatchDog Setup.\n
Attention because of _HAL_S08_WDT_SOPT1_REG and bit COPCLKS of _HAL_S08_WDT_SOPT2_REG are write-once,\n
so only the first write after reset is honored, also BDM and STOP Mode are setup here.
*/

#define	WDT_InitSync()\
{\
_HAL_S08_WDT_SOPT2_REG|=(WDT_BUS_CLOCK_SOURCE<<_HAL_S08_WDT_SOPT2_COPCLKS_BITPOS);\
_HAL_S08_WDT_SOPT1_REG=\
(\
(WDT_ENABLED<<_HAL_S08_WDT_SOPT1_COPE_BITPOS)|\
(WDT_TIMEOUT<<_HAL_S08_WDT_SOPT1_COPT_BITPOS)|\
(STOP_ENABLE<<_HAL_S08_SYS_SOPT1_STOPE_BITPOS)|\
(BDM_ENABLE<<_HAL_S08_SYS_SOPT1_BKGDPE_BITPOS)|\
(RSTO_ENABLE<<_HAL_S08_SYS_SOPT1_RSTOPE_BITPOS)|\
(RESET_ENABLE<<_HAL_S08_SYS_SOPT1_RSTPE_BITPOS)\
);\
}

/**
Feed the dog
*/
#define WDT_Refresh()\
{asm sta SRS;}  

#define WDT_Enabled()   WDT_ENABLED  
#define WDT_AllowUpdate()   FALSE
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
#define WDT_IsWindowOpen()    (1)
/**
Unsupported 
*/  
#define WDT_RSTCounter()    0
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
#define WDT_IsTest() FALSE
/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */
#ifdef HAL_FLS_SUPPORT
/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define Flash Capability */
#define _HAL_FLS_CAPABILITY
/** Define Commands for managing flash resident in RAM */
#define FLASH_CMD_IN_RAM
/** All flash's Pages have the same size */
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
/** This is the size in bytes of a flash's page. */
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
/** This is the  FLASH Clock Divider Register (FCDIV) Verified */
#define _HAL_S08_FLS_FCDIV_REG					(FCDIV)
/** This is the bit position of FDIVLD bit Verified */
#define _HAL_S08_FLS_FCDIV_FDIVLD_BITPOS        7
/** This is the bit position of PRDIV8 bit Verified */
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
Initialization of the FLASH Module Clock.
\n Programming and erase operations are allowed. 
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
#define SIZE_CMD_ERASE_IN_RAM 21
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
#define SIZE_BURST_CMD_IN_RAM	72
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
(void)memcpy(&flashCmdInRAM,&FLS_ErasePageCmdIsInRam,SIZE_CMD_ERASE_IN_RAM);\
((pFLS_ErasePageCmdIsInRam)&flashCmdInRAM)(position);\
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
(void)memcpy(&flashCmdInRAM,&FLS_BurstProgCmdIsInRam,SIZE_BURST_CMD_IN_RAM);\
((pFLS_BurstProgCmdIsInRam)&flashCmdInRAM)(pSource,size,position);\
}

#pragma INLINE
static uint8_t FLS_CheckErrors(void)
{  
	return(_HAL_S08_FLS_FSTAT_REG & _HAL_S08_FLS_FSTAT_ERR_MASK);
}
#endif /* #ifdef HAL_FLS_SUPPORT */
/* ********************************************************************************************** */
/*                                  END FLS Class HAL Support                                     */
/* ********************************************************************************************** */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ********************* */
/* IO                    */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


#endif /* __HAL_FREESCALE_MC9S08_QE128_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */