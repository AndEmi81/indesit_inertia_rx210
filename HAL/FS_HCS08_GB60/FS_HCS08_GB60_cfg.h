/**
\defgroup HAL_FS_HCS08_GB60_cfg	Hardware Abstraction Layer Configuration for Freescale S08 GB60

HAL configuration for Freescale HCS08 GB60 microcontroller

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
 

\section HAL_SWDMA_SUPPORT_sec HAL_SWDMA_SUPPORT
Must be defined to use SW DMA driver.

\section SWDMA_CH_NUM_sec SWDMA_CH_NUM
Number of channel of SW DMA

\date    12/03/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_HCS08_GB60

\defgroup HAL_FS_HCS08_GB60_HISTORY_CFG  Freescale HCS08 GB60 HAL Configuration history
\ingroup  HAL_FS_HCS08_GB60_HISTORY
*/

/**
Configuration Header file for Freescale HCS08 GB60.

\file    FS_HCS08_GB60_cfg.h
\ingroup HAL_FS_HCS08_GB60_cfg
\date    25/02/2009
\author  Roberto Fioravanti
\version 1.0.0

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\addtogroup HAL_FS_HCS08_GB60_HISTORY_CFG
@{
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
\addtogroup HAL_FS_HCS08_GB60_cfg
@{*/



#ifndef __HAL_FREESCALE_HCS08_GB60_CFG_H
#define __HAL_FREESCALE_HCS08_GB60_CFG_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"


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

#if (BUS_FREQUENCY_Hz!=10000000)  && (BUS_FREQUENCY_Hz!=20000000)
#error "Value of BUS_FREQUENCY_Hz not valid in Config.h."
#endif /* BUS_FREQUENCY_Hz!=10000000 */


#if (I2C_CLOCKRATE_kbps!=25) && \
	(I2C_CLOCKRATE_kbps!=50) && \
	(I2C_CLOCKRATE_kbps!=100) && \
    (I2C_CLOCKRATE_kbps!=200)
#error "Value of I2C_CLOCKRATE_kbps not valid in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */


#if (BUS_FREQUENCY_Hz == 10000000)

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
#define I2C_CLOCKRATE_SETTING 22 

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

#if		HAL_OPTION_UART_DATALEN==9
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


#endif
#define PIN_OUT_DIR   1
#define PIN_IN_DIR    0
#define PP_END_OF_UNTOUCHABLE_RAM 0x80
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

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */

#define MCU_PIN_PACKAGE   64
#define DUMMY_PORT_DATA_ADDR	0xFF

/* Size of pointer to point Ports's Registers */
typedef uint8_t DIO_PortAddressBasicType;

#define extended basic     
typedef struct
{
    DIO_PortAddressBasicType basic;
}DIO_ModuleMapPortAddressType;

/* Size of Ports's Registers */
typedef uint8_t DIO_ModuleMapMaskType;

/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class												  */
/* ********************************************************************************************** */
#if defined(HAL_TIMER_SUPPORT)
/* Timers's Channels Map Type */
/* Number of Hardware Counters (Eg. TPM1 Counter,TPM2 Counter) */
#define NUM_HW_CNTR     2        
/* Number of Channels linked to Hardware Counters (Eg. TPM1CH0 ) */
#define NUM_HW_CNTR_CH  8


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t TMR_ChResourceType;    /* cfr. NUM_HW_CNTR_CH */
/* Period value for the HW counter [tick] */
typedef uint16_t    Period_ValueType;
typedef	Period_ValueType    IO_HwTimerType;
/* Hardware Counter Precaler */
typedef uint8_t     PRS_ValueType;

typedef uint16_t TMR_AddressType;
typedef TMR_AddressType TMR_CNTAddressType;
typedef TMR_AddressType TMR_CHAddressType;

typedef struct TMR_ChMapType_tag {
    TMR_CHAddressType Channel;
    uint8_t hwCntrlId;
}TMR_ChMapType;

typedef struct TMR_ChVsPinMapType_tag {
    DIO_PortAddressBasicType Port;
    DIO_ModuleMapMaskType PinMask;
}TMR_ChVsPinMapType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
#endif
/* ********************************************************************************************** */
/*                                  END TMR Class												  */
/* ********************************************************************************************** */


/* UART Class type declarations */
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
#if defined (HAL_CLK_SUPPORT)

#if defined (CLK_INT_REF_CLOCK_ACTIVE) && defined (CLK_EXT_CLOCK_IN_STOP_MODE)
#error "Property CLK_EXT_CLOCK_IN_STOP_MODE is valid only for External Clock sources "
#endif

#if !defined(CLK_INT_REF_CLOCK_ACTIVE)

#if ((CLK_EXT_CLOCK_FREQ >= 32000) && (CLK_EXT_CLOCK_FREQ <= 100000 ))
#define CLK_HIGH_FREQUENCY  FALSE
#elif ((CLK_EXT_CLOCK_FREQ >= 1000000) && (CLK_EXT_CLOCK_FREQ <= 16000000))
#define CLK_HIGH_FREQUENCY  TRUE
#else
#error "Property CLK_EXT_CLOCK_FREQ is not valid"
#endif

#endif /* #if !defined(CLK_INT_REF_CLOCK_ACTIVE) */

#if defined (CLK_HIGH_GAIN_OSCILLATOR) && defined (CLK_INT_REF_CLOCK_ACTIVE)
#warning "Property CLK_HIGH_GAIN_OSCILLATOR has no effects if defined CLK_INT_REF_CLOCK_ACTIVE"
#endif

#if defined (CLK_INT_REF_CLOCK_ACTIVE) && defined(CLK_EXT_CLOCK_XTAL)
#error "Properties CLK_INT_REF_CLOCK_ACTIVE and CLK_EXT_CLOCK_XTAL can't be defined at the same time"
#endif 


#if !defined (CLK_INT_REF_CLOCK_ACTIVE)
#if defined (CLK_HIGH_GAIN_OSCILLATOR) 
#define CLK_HIGH_GAIN   TRUE 
#else
#define CLK_HIGH_GAIN   FALSE
#endif
#endif  /* !defined (CLK_INT_REF_CLOCK_ACTIVE) */

#if defined (CLK_EXT_CLOCK_XTAL)
#define CLK_XTAL    TRUE
#else
#define CLK_XTAL    FALSE
#endif /* #if defined (CLK_EXT_CLOCK_XTAL) */

#if defined (CLK_EXT_CLOCK_IN_STOP_MODE)
#define CLK_OFF_IN_STOP TRUE
#else
#define CLK_OFF_IN_STOP FALSE
#endif /* #if defined (CLK_EXT_CLOCK_IN_STOP_MODE) */

#endif /* #if defined (HAL_CLK_SUPPORT) */

typedef uint8_t CLK_TrimmingType;

/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN WDT Class												  */
/* ********************************************************************************************** */
#if defined (HAL_WDT_SUPPORT)

#ifdef WDT_UPDATE_AFTER_WRITE
#error "WDT_UPDATE_AFTER_WRITE not supported!"
#endif
#ifdef WDT_WINDOWED_MODE
#error "WDT_WINDOWED_MODE not supported!"
#endif
#ifdef WDT_INT_ENABLED 
#error "WDT_INT_ENABLED not supported!"
#endif
#ifdef WDT_ALTERNATE_CLOCK
#error "Property not supported! watchdog is clocked by bus frequency"
#endif
#ifdef WDT_ENABLED_IN_STOP
#error "For GB60 watchdog is ever disabled in Stop Mode"
#endif
#ifdef WDT_ENABLED_IN_DEBUG
#error "Property not supported for GB60"
#endif
#ifdef WDT_ENABLED_IN_WAIT
#error "For GB60 watchdog is ever enabled in Wait Mode"
#endif
#ifdef WDT_WINDOW
#error "Property not supported for GB60"
#endif

#ifndef WDT_TIMEOUT
#error "Please define WDT_TIMEOUT,possibles values 0 or 1"
#endif 

#if (WDT_TIMEOUT > 1)
#error "WDT_TIMEOUT must be 0 -> short timeout or 1 -> long timeout"
#endif

#endif /* #if defined (HAL_WDT_SUPPORT) */
/*
Meaningless typedefs just to compile
*/
typedef uint8_t WDT_windowType;
typedef uint8_t WDT_timeoutType;
typedef uint8_t WDT_counterType;
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
	#if (BUS_FREQUENCY_Hz < 150000)
		#warning "Bus Frequency too low for erase/write operations "
	#endif /* (BUS_FREQUENCY_Hz < 150000) */
	#if !defined(FLS_MODULE_FREQUENCY_Hz)
		#error "FLS_MODULE_FREQUENCY_Hz property not defined in FlashConfig.h."
	#endif /* !defined(FLS_MODULE_FREQUENCY_Hz) */
	
	#if (FLS_MODULE_FREQUENCY_Hz > 200000)
		#error "FLS_MODULE_FREQUENCY_Hz is higher than 200 kHz"
	#endif /* (FLS_MODULE_FREQUENCY_Hz > 200000) */
	
	#if (FLS_MODULE_FREQUENCY_Hz < 150000)
		#error "FLS_MODULE_FREQUENCY_Hz is lower than 150 kHz"
	#endif /* (FLS_MODULE_FREQUENCY_Hz < 150000) */

#endif /* defined(HAL_FLS_SUPPORT) */


#define SIZE_CMD_ERASE_IN_RAM   21
#define SIZE_BURST_CMD_IN_RAM	72


typedef uint16_t FLS_sizeType;
/** 
C.P.U. memory max. address,\n
E.g. if C.P.U. memory max. address is 0xFFFF then FLS_positionType will be uint16_t \n
     if C.P.U. memory max. address is 0xFFFFFFFF then FLS_positionType will be uint32_t.
*/
typedef uint16_t FLS_positionType;
typedef uint32_t FLS_sizePLUSpositionType;
typedef FLS_positionType FLS_paramType;
typedef uint8_t* FLS_SourcePtrType;
typedef uint8_t FLS_VerifyType;
typedef uint16_t FLS_BlanckCheckType;


typedef struct   
{
    uint32_t cmd[SIZE_CMD_ERASE_IN_RAM/4 + ((SIZE_CMD_ERASE_IN_RAM%4)?+1:0)];
}FLS_EraseCmdInRamType;


typedef struct   
{
    uint32_t cmd[SIZE_BURST_CMD_IN_RAM/4 + ((SIZE_BURST_CMD_IN_RAM%4)?+1:0)];
}FLS_BurstCmdInRamType;
#define FLS_BLANCK_VALUE			(FLS_BlanckCheckType)0xFFFF
#define SIZE_OF_FLS_BlanckCheckType	2
#define SIZE_OF_FLS_VerifyType		1
#define FLS_MAX_ADDR        		0xFFFF



/* ********************************************************************************************** */
/*                                  END FLS Class												  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)

#if (ADC_USE_HW_DMA)
#error "Hardware DMA not supported by GB60 microcontroller"
#endif 
#if (ADC_CONV_FREQUENCY_Hz>2000000)
#error "ADC_CONV_FREQUENCY_Hz too high! max value is 2000000 Hz"
#endif
#if (ADC_CONV_FREQUENCY_Hz<500000)
#error "ADC_CONV_FREQUENCY_Hz too low! min. value is 500000 Hz"
#endif
#if (ADC_RESOLUTION > 16)
#error "ADC_RESOLUTION too high, max value is 16"
#endif
#if (ADC_RESOLUTION <= 0)
#error "Are you joking?"
#endif

#endif /* #if defined(HAL_ADC_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */

#endif /* __HAL_FREESCALE_HCS08_GB60_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
