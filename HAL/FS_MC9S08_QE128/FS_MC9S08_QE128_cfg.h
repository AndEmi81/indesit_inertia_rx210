/**
\defgroup HAL_FS_MC9S08_QE128_cfg	Hardware Abstraction Layer Configuration for Freescale S08 QE128

HAL configuration for Freescale HCS08 QE128 microcontroller

\par Configuration

\section BUS_FREQUENCY_Hz_sec BUS_FREQUENCY_Hz
Declares the value in Hz of the bnus frequency.
\subsection BUS_FREQUENCY_Hz_subsec Supported values
\li 10000000
\li 16000000

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
\li BUS_FREQUENCY_Hz = 16000000
50, 100
100, 200
 

\section HAL_SWDMA_SUPPORT_sec HAL_SWDMA_SUPPORT
Must be defined to use SW DMA driver.

\section SWDMA_CH_NUM_sec SWDMA_CH_NUM
Number of channel of SW DMA

\date    13/05/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_MC9S08_QE128

\defgroup HAL_FS_MC9S08_QE128_HISTORY_CFG  Freescale HCS08 QE128 HAL Configuration history
\ingroup  HAL_FS_MC9S08_QE128_HISTORY
*/

/**
Header file di configurazione per HAL di Freescale HCS08 QE128.

\file    FS_MC9S08_QE128_cfg.h
\ingroup HAL_FS_MC9S08_QE128
\date    13/05/2009
\author  Roberto Fioravanti
\version 1.0.0

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\addtogroup HAL_FS_MC9S08_QE128_HISTORY_CFG
@{
\n 		00.00.22 
\n		26/11/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- REMAP_ADDRESS1 and REMAP_ADDRESS1_OFFSET added
\n
\n 		00.00.14 
\n		28/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- typedef DIO_ModuleMapPortAddressType, * DIO_ModuleMapPortType and DIO_ModuleMapMaskType for 'ModuleIOMap' table added 
\n    - #defines PIN_OUT_DIR and PIN_IN_DIR added
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
\addtogroup HAL_FS_MC9S08_QE128_cfg
@{*/



#ifndef __HAL_FREESCALE_HCS08_QE128_CFG_H
#define __HAL_FREESCALE_HCS08_QE128_CFG_H

/* ******** */
/* Includes */
/* ******** */


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

#if (BUS_FREQUENCY_Hz!=10000000)  && \
    (BUS_FREQUENCY_Hz!=16000000)
#error "Value of BUS_FREQUENCY_Hz not valid in Config.h."
#endif /* BUS_FREQUENCY_Hz!=10000000 */


#if (I2C_CLOCKRATE_kbps!=50) && \
    (I2C_CLOCKRATE_kbps!=100) && \
    (I2C_CLOCKRATE_kbps!=200)
#error "Value of I2C_CLOCKRATE_kbps not valid in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */


#if (BUS_FREQUENCY_Hz == 10000000)

#if (I2C_CLOCKRATE_kbps == 100)
#define I2C_CLOCKRATE_SETTING 153 /* TODO: calcolare il vero valore */

#elif (I2C_CLOCKRATE_kbps == 200)
#define I2C_CLOCKRATE_SETTING 137 /* TODO: calcolare il vero valore */

#error "Clock rate non supportato alla frequenza corrente.\n    \
Cambiare il valore di I2C_CLOCKRATE_kbps o BUS_FREQUENCY_Hz."

#endif /* I2C_CLOCKRATE_kbps == 100 */

#endif /* BUS_FREQUENCY_Hz==10000000 */


#if (BUS_FREQUENCY_Hz == 16000000)

#if (I2C_CLOCKRATE_kbps == 50)
#define I2C_CLOCKRATE_SETTING 148 /*  */

#elif (I2C_CLOCKRATE_kbps == 100)
#define I2C_CLOCKRATE_SETTING 137 /* TODO: calcolare il vero valore */

#else
#error "Clock rate non supportato alla frequenza corrente.\n    \
Cambiare il valore di I2C_CLOCKRATE_kbps o BUS_FREQUENCY_Hz."
#endif /* I2C_CLOCKRATE_kbps == 100 */

#endif /* BUS_FREQUENCY_Hz==10000000 */

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

#define PIN_OUT_DIR   1
#define PIN_IN_DIR    0

#define PP_END_OF_UNTOUCHABLE_RAM 0x80
#define REMAP_ADDRESS1								0
#define REMAP_ADDRESS1_OFFSET					0

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
typedef uint16_t DIO_PortAddressExtendedType;

typedef struct
{
    DIO_PortAddressBasicType basic;
    DIO_PortAddressExtendedType extended;
}DIO_ModuleMapPortAddressType;
/* Size of Ports's Registers */
typedef uint8_t DIO_ModuleMapMaskType;

/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */



/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
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
	#if (WDT_ALTERNATE_CLOCK == TRUE)
	#define WDT_BUS_CLOCK_SOURCE	1
	#else
	#define WDT_BUS_CLOCK_SOURCE	0
	#endif
#else /*WDT_ALTERNATE_CLOCK nt defined*/
	#error "Property WDT_ALTERNATE_CLOCK not defined!"
#endif /*WDT_ALTERNATE_CLOCK*/
#ifdef WDT_ENABLED_IN_STOP
	#ifdef WDT_ALTERNATE_CLOCK
	#error "For QE128 watchdog is ever disabled in Stop Mode if WDT_ALTERNATE_CLOCK is TRUE"
	#endif
#endif
#ifdef WDT_ENABLED_IN_DEBUG
#error "Property not supported for QE128"
#endif
#ifdef WDT_ENABLED_IN_WAIT
#error "For QE128 watchdog is ever enabled in Wait Mode"
#endif
#ifdef WDT_WINDOW
#error "Property not supported for QE128"
#endif
#ifndef WDT_TIMEOUT
#error "Please define WDT_TIMEOUT,possibles values 0 or 1"
#endif 

#if (WDT_TIMEOUT > 1)
#error "WDT_TIMEOUT must be 0 -> short timeout or 1 -> long timeout"
#endif
#endif /*#if defined (HAL_WDT_SUPPORT)*/
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

typedef uint16_t FLS_sizeType;
/** 
C.P.U. memory max. address,\n
E.g. if C.P.U. memory max. address is 0xFFFF then FLS_positionType will be uint16_t \n
     if C.P.U. memory max. address is 0xFFFFFFFF then FLS_positionType will be uint32_t.
*/
typedef uint16_t FLS_positionType;  /* Verified */
typedef uint32_t FLS_sizePLUSpositionType;
typedef FLS_positionType FLS_paramType;
typedef uint8_t* FLS_SourcePtrType;
typedef uint8_t FLS_VerifyType;
typedef uint16_t FLS_BlanckCheckType;


#define FLS_BLANCK_VALUE			(FLS_BlanckCheckType)0xFFFF
#define SIZE_OF_FLS_BlanckCheckType	2
#define SIZE_OF_FLS_VerifyType		1
#define FLS_MAX_ADDR        		0xFFFF



/* ********************************************************************************************** */
/*                                  END FLS Class												  */
/* ********************************************************************************************** */
#endif /* __HAL_FREESCALE_HCS08_QE128_CFG_H */
/*@}*/

/* *********** */
/* End of file */
/* *********** */