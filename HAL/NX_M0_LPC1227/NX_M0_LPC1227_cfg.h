/**
\defgroup HAL_NX_M0_LPC1227_cfg	Hardware Abstraction Layer Configuration for NXP LPC1227

HAL configuration for NXP LPC1227 microcontroller

\par Configuration

\section BUS_FREQUENCY_Hz_sec BUS_FREQUENCY_Hz
Declares the value in Hz of the bnus frequency.

\subsection BUS_FREQUENCY_Hz_subsec Supported values
\li 10000000

\section HAL_I2C_SUPPORT_sec HAL_I2C_SUPPORT
Must be defined to use I2C driver.

\subsection HAL_I2C_SUPPORT_subsec Requirements
BUS_FREQUENCY_Hz must be defined

\section I2C_CLOCKRATE_kbps_sec I2C_CLOCKRATE_kbps
Specifies the clock rate to be used for I2C driver

\subsection HAL_I2C_SUPPORT_subsec Requirements
HAL_I2C_SUPPORT must be defined

\section I2C_CLOCKRATE_kbps_subsec_Values  Supported values
\li BUS_FREQUENCY_Hz = 10000000
100, 200
 
\section HAL_UART_SUPPORT_sec HAL_UART_SUPPORT
Must be defined to use UART driver.

\subsection HAL_UART_SUPPORT_subsec Requirements
BUS_FREQUENCY_Hz must be defined

\section HAL_SWDMA_SUPPORT_sec HAL_SWDMA_SUPPORT
Must be defined to use SW DMA driver.

\section SWDMA_CH_NUM_sec SWDMA_CH_NUM
Number of channel of SW DMA

\section HAL_FLASH_SUPPORT_sec HAL_FLASH_SUPPORT
Must be defined to use FLASH driver.

\section  HAL_FLS_SUPPORT



\date    12/03/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_NX_M0_LPC1227

\defgroup HAL_NX_M0_LPC1227_HISTORY_CFG  NXP M0 LPC1227 HAL Configuration history
\ingroup  HAL_NX_M0_LPC1227_HISTORY
*/

/**
Configuration Header file for Freescale HCS08 GB60.

\file    NX_M0_LPC1227_cfg.h
\ingroup HAL_NX_M0_LPC1227_cfg
\date    30/03/2011
\author  Giancarlo Parodi
\version 1.0.0

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\addtogroup HAL_NX_M0_LPC1227_HISTORY_CFG
@{
\n      00.00.29
\n      30/03/2011
\n      Giancarlo Parodi
\n      changes
\n      - modified doxygen references to use LPC terms
\n
\n 		00.00.28
\n		11/03/2010 
\n		Stefano Ughi
\n		changes
\n 		- added Config. Watchdog HAL
\n		- added Config. Flash HAL
\n		- added Config. ADC HAL
\n		- added Config. Clock HAL
\n
\n 		00.00.32
\n		24/02/2010
\n		Roberto Fioravanti
\n		changes
\n      -   addiction of UART_BaudRateRegType
\n 		00.00.22 
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n      -   REMAP_ADDRESS1 and REMAP_ADDRESS1_OFFSET properties added
\n 		00.00.14 
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- typedef DIO_ModuleMapPortAddressType, * DIO_ModuleMapPortType and DIO_ModuleMapMaskType for 'ModuleIOMap' table added 
\n      - #defines PIN_OUT_DIR and PIN_IN_DIR added
\n
\n 		00.00.01 
\n		22/09/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- creation
@}
*/

/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup HAL_NX_M0_LPC1227_cfg
@{*/



#ifndef __HAL_NX_LPC12xx_CFG_H
#define __HAL_NX_LPC12xx_CFG_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "Config.h"
#include  "StandardDefines.h"


/* ****** */
/* Errors */
/* ****** */
#if defined(HAL_I2C_SUPPORT)

  #if !defined(BUS_FREQUENCY_Hz)
    #error "BUS_FREQUENCY_Hz not defined in Config.h."
  #endif /* !defined(BUS_FREQUENCY_Hz)*/

  /** The application is required to define the chip's operating speed */
  #if !defined(I2C_CLOCKRATE_kbps)
    #error "I2C_CLOCKRATE_kbps not defined in Config.h."
  #endif /* !defined(I2C_CLOCKRATE_kbps)*/

  #if (BUS_FREQUENCY_Hz!=33000000)
    #error "Value of BUS_FREQUENCY_Hz not valid in Config.h."
  #endif /* BUS_FREQUENCY_Hz!=10000000 */


  #if (I2C_CLOCKRATE_kbps!=25) && \
      (I2C_CLOCKRATE_kbps!=50) && \
      (I2C_CLOCKRATE_kbps!=100) && \
      (I2C_CLOCKRATE_kbps!=200)
    #error "Value of I2C_CLOCKRATE_kbps not valid in Config.h."
  #endif /* I2C_CLOCKRATE_kbps != xxx */


  #if (BUS_FREQUENCY_Hz == 33000000)
    #if (I2C_CLOCKRATE_kbps == 100)
      #define I2C_CLOCKRATE_SETTING 22 
    #elif (I2C_CLOCKRATE_kbps == 200)
      #define I2C_CLOCKRATE_SETTING 131 
    #endif /* I2C_CLOCKRATE_kbps == 100 */

  #endif /* BUS_FREQUENCY_Hz==10000000 */

  #if (BUS_FREQUENCY_Hz == 20000000)

    #if (I2C_CLOCKRATE_kbps == 25)
    #define I2C_CLOCKRATE_SETTING 158

    #elif (I2C_CLOCKRATE_kbps == 50)
    #define I2C_CLOCKRATE_SETTING 150

    #elif (I2C_CLOCKRATE_kbps == 100)
    #define I2C_CLOCKRATE_SETTING 86 /* TODO: calcolare il vero valore */

    #elif (I2C_CLOCKRATE_kbps == 200)
      //#define I2C_CLOCKRATE_SETTING 22 
      #define I2C_CLOCKRATE_SETTING 200 
    #endif /* I2C_CLOCKRATE_kbps == 100 */

  #endif /* BUS_FREQUENCY_Hz==20000000 */

/* [TODO: gestire altri valori ] */
#endif /* HAL_I2C_SUPPORT */


#if defined(HAL_SWDMA_SUPPORT)

  #if !defined(SWDMA_CH_NUM)
    #error "SWDMA_CH_NUM not defined in Config.h."
  #endif /* !defined(I2C_CLOCKRATE_kbps)*/

#endif /* HAL_SWDMA_SUPPORT */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/*UART*/
#if defined(HAL_UART_SUPPORT)
  /*#if HAL_OPTION_UART_ASYN_MODE==TRUE
  #define	HAL_UART_MODE				0
  #else
  #define	HAL_UART_MODE				1
  #endif*/

  #if HAL_OPTION_UART_STOP==1
    #define HAL_UART_STOP 			0
  #else
    #error "HAL_OPTION_UART_STOP: valore non supportato"
  #endif

  #if	HAL_OPTION_UART_DATALEN==9
    #define HAL_UART_DATALEN						1
  #elif	HAL_OPTION_UART_DATALEN==8
    #define HAL_UART_DATALEN						0
  #else
    #error "HAL_OPTION_UART_DATALEN: valore non supportato"
  #endif

  #if HAL_OPTION_UART_USE_PARITY==FALSE
    #define HAL_UART_PARITY						0
  #else
    #error "HAL_OPTION_UART_USE_PARITY: valore non supportato"
  #endif

  #define HAL_UART_BAUDRATE_DIVISOR 	HAL_OPTION_UART_BAUDRATE_DIVISOR
  #define HAL_UART_DIVISOR 	        1

#endif /* #if defined(HAL_UART_SUPPORT) */

#define PIN_OUT_DIR   1
#define PIN_IN_DIR    0
//#define PP_END_OF_UNTOUCHABLE_RAM 0x80
#define REMAP_ADDRESS1								0
#define REMAP_ADDRESS1_OFFSET					0

#define ___INLINE___   inline

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* I2C Class type definitions */
typedef uint8_t I2C_DataRegType;
typedef uint8_t I2C_SlaveAddressType;
typedef uint8_t I2C_ClockRateType;

/* DIO Class type definitions */
//typedef uint8_t DIO_ModuleMapPortAddressType, * DIO_ModuleMapPortType;
//typedef uint8_t DIO_ModuleMapMaskType;

typedef uint32_t	UART_BaudRateType;
typedef uint16_t	UART_BaudRateRegType;
typedef uint8_t 	UART_DataRegType;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */

typedef uint8_t CLK_TrimmingType;

/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN WDT Class												  */
/* ********************************************************************************************** */
#if defined (HAL_WDT_SUPPORT)


#if ((WDT_ENABLED != 0) && (WDT_ENABLED != 1))
  #error "WDT_ENABLED can be only 0 or 1"
#endif


#ifdef WDT_ENABLED_IN_STOP
  #error "Property not supported for LPC1227"
#endif

#ifdef WDT_ENABLED_IN_WAIT
  #error "Property not supported in LPC1227"
#endif


#ifndef WDT_TIMEOUT
  #error "Please define WDT_TIMEOUT"
#endif 

#if (WDT_TIMEOUT > 0xFFFFFF)
  #error  "desired WDT_TIMEOUT higher than maximum allowed (0xFFFFFF)"
#endif

#if (WDT_TIMEOUT < 0xFF)
//#error  "desired WDT_TIMEOUT lower than minimum allowed (0xFF)"
#endif

#if (WDT_INT_ENABLED > 0x3FF)
  #error "WDT_INT_ENABLED max value is 10-bit wide (0x3FF)"
#endif

#if ((WDT_ALTERNATE_CLOCK != 0) && ((WDT_ALTERNATE_CLOCK != 1)))
  #error "Allowed alternate clock values are 0 or 1"
#endif

#if (WDT_WINDOWED_MODE == 1)

  #ifndef WDT_TIMEOUT
    #error "Please define WDT_WINDOW"
  #endif 

  #if (WDT_WINDOW < 0x100)
    //#error  "desired WDT_WINDOW value makes impossible to feed the watchdog successfully"
  #endif

  #if (WDT_TIMEOUT >= WDT_WINDOW)
//    #error  "WDT_WINDOW must be greater than WDT_TIMEOUT"
  #endif
#endif

#if (WDT_ENABLED != WDT_ENABLED_IN_DEBUG)
  #error "WDT_ENABLED must be equal to WDT_ENABLED_IN_DEBUG"
#endif


#endif /* #if defined (HAL_WDT_SUPPORT) */


typedef uint32_t WDT_windowType;
typedef uint32_t WDT_timeoutType;
typedef uint32_t WDT_counterType;


/*
Meaningless typedefs just to compile
*/
typedef uint8_t WDT_resetCntrRegType;
typedef uint8_t WDT_testByteType;
typedef uint8_t WDT_testType;

/* ********************************************************************************************** */
/*                                  END WDT Class												  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class												  */
/* ********************************************************************************************** */
#if defined(HAL_FLS_SUPPORT)

	#if !defined(BUS_FREQUENCY_Hz)
		#error "BUS_FREQUENCY_Hz property not defined in Config.h."
	#endif /* !defined(BUS_FREQUENCY_Hz)*/

        #if (BUS_FREQUENCY_Hz > 33000000) // GP added check
		#error "Bus Frequency not supported, too high "
	#endif /* (BUS_FREQUENCY_Hz > 33000000) */

        // GP paranoid check - freq not supported!
        // would require reprogramming the flashcfg reg
        #if (BUS_FREQUENCY_Hz > 45000000)  
          #error "FLASHCFG = 0x0 @ 0x50060028 not valid in NX_M0_LPC1227_hal.h."
        #endif /* (BUS_FREQUENCY_Hz > 45000000) */


#define SIZE_IAP_CMD 5  /* 5 words worst case */
#define SIZE_IAP_STAT 4 /* 4 words worst case */

// definition of the function pointer to the IAP routines
typedef void (*IAP)(unsigned int[], unsigned int[]);

typedef uint32_t FLS_sizeType;
/** 
C.P.U. memory max. address,\n
E.g. if C.P.U. memory max. address is 0xFFFF then FLS_positionType will be uint16_t \n
     if C.P.U. memory max. address is 0xFFFFFFFF then FLS_positionType will be uint32_t.
*/
typedef uint32_t FLS_positionType;
typedef uint32_t FLS_sizePLUSpositionType;
typedef FLS_positionType FLS_paramType;

typedef uint32_t* FLS_SourcePtrType;
typedef uint32_t FLS_VerifyType;

typedef uint32_t FLS_BlanckCheckType;  

typedef struct   
{
    uint32_t iapCommand[SIZE_IAP_CMD];
    uint32_t iapResult[SIZE_IAP_STAT];
    IAP iap_entry;
}FLS_EraseCmdInRamType;


typedef struct   
{
    uint32_t iapCommand[SIZE_IAP_CMD];
    uint32_t iapResult[SIZE_IAP_STAT];
    IAP iap_entry/* = (IAP) IAP_LOCATION*/;
}FLS_BurstCmdInRamType;


// GP changed for 32-bit flash
#define FLS_BLANCK_VALUE  (FLS_BlanckCheckType)0xFFFFFFFF 
#define SIZE_OF_FLS_BlanckCheckType	4
#define SIZE_OF_FLS_VerifyType		4

// GP max size 128 K on LPC1227
#define FLS_MAX_ADDR        		0x1FFFF 

#endif /* defined(HAL_FLS_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END FLS Class												  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)

//#if (ADC_USE_HW_DMA)
//#error "Hardware DMA not supported by GB60 microcontroller"
//#endif 
//#if (ADC_CONV_FREQUENCY_Hz>2000000)
//#error "ADC_CONV_FREQUENCY_Hz too high! max value is 2000000 Hz"
//#endif
//#if (ADC_CONV_FREQUENCY_Hz<500000)
//#error "ADC_CONV_FREQUENCY_Hz too low! min. value is 500000 Hz"
//#endif
//#if (ADC_RESOLUTION > 16)
//#error "ADC_RESOLUTION too high, max value is 16"
//#endif
//#if (ADC_RESOLUTION <= 0)
//#error "Are you joking?"
//#endif

#endif /* #if defined(HAL_ADC_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class												  */
/* ********************************************************************************************** */
#if defined(HAL_TIMER_SUPPORT)

/* ****** */
/* Errors */
/* ****** */
  #if !defined(BUS_FREQUENCY_Hz)
    #error "BUS_FREQUENCY_Hz not defined in Config.h."
  #endif /* !defined(BUS_FREQUENCY_Hz)*/

/* Timers's Channels Map Type */
/* Number of Hardware Counters CT16B0, CT16B1, CT32B0, CT32B1 */
#define NUM_HW_CNTR     4 

/* Number of Channels linked to Hardware Counters (Eg. CT16B0CH0 ) */
/* Total number for all timers */
/* 4 channels per each timer */
#define NUM_HW_CNTR_CH  16


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint16_t TMR_ChResourceType;    /* cfr. NUM_HW_CNTR_CH */

/* Period value for the HW counter [tick] */
/* defined for max resolution possible */
typedef uint32_t          Period_ValueType; 
typedef	Period_ValueType  IO_HwTimerType;

/* Hardware Counter Prescaler */
/* define for max resolution possible */
typedef uint32_t     PRS_ValueType;

/* addresses for the control registers are always 32-bit */
typedef uint32_t TMR_AddressType;
typedef TMR_AddressType TMR_CNTAddressType;
typedef TMR_AddressType TMR_CHAddressType;

/* this defines a relationship between the channel address */
/* and the hardware timer. Note that the address type is the  */
/* match register address, not the control register address */
/* see comments in hal.h */
typedef struct TMR_ChMapType_tag {
    TMR_CHAddressType Channel;
    uint8_t hwCntrlId;
}TMR_ChMapType;

/* this defines the channel used for modulo (resets the timer) */
/* includes the channei id  */
typedef struct TMR_ChModuloType_tag {
    uint32_t tmrModuloChNum;
}TMR_ChModuloType;


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
#endif  /* #if defined(HAL_TIMER_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END TMR Class						  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class												  */
/* ********************************************************************************************** */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


// #define MCU_PIN_PACKAGE   64
#define DUMMY_PORT_DATA_ADDR	  0


/* Size of pointer to point Ports's Registers */
typedef uint32_t DIO_PortAddressBasicType;
typedef uint32_t DIO_PortAddressExtendedType;

typedef struct
{
    DIO_PortAddressBasicType basic;
    DIO_PortAddressExtendedType extended;           
} DIO_ModuleMapPortAddressType;

/* Size of Ports's Registers */
typedef uint32_t DIO_ModuleMapMaskType;

/* ********************************************************************************************** */
/*                                  END DIO Class												  */
/* ********************************************************************************************** */



#endif /* __HAL_NX_LPC12xx_CFG_H */




/*@}*/

/* *********** */
/* End of file */
/* *********** */
