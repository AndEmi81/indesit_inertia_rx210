/**
\defgroup HAL_RN_300H_39079_cfg	Hardware Abstraction Layer Configuration for Renesas 300H 39079

HAL configuration for Renesas 300H 39079 microcontroller
\par Configuration

\section BUS_FREQUENCY_Hz_sec BUS_FREQUENCY_Hz
Declares the value in Hz of the bnus frequency.
\subsection BUS_FREQUENCY_Hz_subsec Supported values
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
n.a.

 

\section HAL_SWDMA_SUPPORT_sec HAL_SWDMA_SUPPORT
Must be defined to use SW DMA driver.

\section SWDMA_CH_NUM_sec SWDMA_CH_NUM
Number of channel of SW DMA

\date    16/06/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_RN_300H_39079
\defgroup HAL_RN_300H_39079_HISTORY_CFG  Renesas 300H 39079 HAL Configuration history
\ingroup  HAL_RN_300H_39079_HISTORY
*/

/**
Configuration Header file for Renesas 300H 39079.

\file    RN_300H_39079_cfg.h
\ingroup HAL_RN_300H_39079
\date    16/06/2009
\author  Roberto Fioravanti
\version 1.0.0



\addtogroup HAL_RN_300H_39079_HISTORY_CFG
@{

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
\addtogroup HAL_RN_300H_39079_cfg
@{*/



#ifndef __HAL_RENESAS_300H_36079_CFG_H
#define __HAL_RENESAS_300H_36079_CFG_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "StandardDefines.h"

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

#if (BUS_FREQUENCY_Hz!=16000000)
#error "Value of BUS_FREQUENCY_Hz not valid in Config.h."
#endif /* BUS_FREQUENCY_Hz!=10000000 */


#if (I2C_CLOCKRATE_kbps!=100) && \
    (I2C_CLOCKRATE_kbps!=200)
#error "Value of I2C_CLOCKRATE_kbps not valid in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */


#if (BUS_FREQUENCY_Hz == 16000000)

#if (I2C_CLOCKRATE_kbps == 100)
#define I2C_CLOCKRATE_SETTING 153 /* TODO: calcolare il vero valore */

#elif (I2C_CLOCKRATE_kbps == 200)
#define I2C_CLOCKRATE_SETTING 137 /* TODO: calcolare il vero valore */

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

/*UART*/
#if defined(HAL_UART_SUPPORT)
#if HAL_OPTION_UART_ASYN_MODE==TRUE
#define	HAL_UART_MODE				0
#else
#define	HAL_UART_MODE				1
#endif

#if HAL_OPTION_UART_STOP==1
#define HAL_UART_STOP 			0
#elif HAL_OPTION_UART_STOP==2
#define HAL_UART_STOP 			1
#else
#error "HAL_OPTION_UART_STOP: valore non supportato"
#endif

#if		HAL_OPTION_UART_DATALEN==7
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

#define PP_END_OF_UNTOUCHABLE_RAM 		0x00
#define REMAP_ADDRESS1								0xE000
#define REMAP_ADDRESS1_OFFSET					0xFF0000

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef uint8_t I2C_DataRegType;
typedef uint8_t I2C_SlaveAddressType;
typedef uint8_t I2C_ClockRateType;

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
#define MCU_PIN_PACKAGE         64
#define PORT_DIRECTION_MIRRORING
#define NUM_DIRECTION_REGS	    11	  /* 0xFFFFD4-0xFFFFDE (based on port data regs.) */
#define DFLT_PORT_DIR_VALUE	    0
#define DUMMY_PORT_DATA_ADDR	0x00000000


/* Size of pointer to point Ports's Registers */
typedef uint32_t DIO_PortAddressBasicType;
typedef uint32_t DIO_PortAddressExtendedType;
typedef struct
{
    DIO_PortAddressBasicType basic;
    DIO_PortAddressExtendedType extended;
}DIO_ModuleMapPortAddressType;
/* Size of Ports's Registers */
typedef uint8_t DIO_ModuleMapMaskType;

typedef struct IO_ModuleIOMap_tag {
  DIO_ModuleMapPortAddressType PortAddress; /**< Address of the Port Data Register relsated to the port pin */
  DIO_ModuleMapMaskType        PinMask;     /**< Port pin mask  */
}IO_ModuleIOMapType;

/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

typedef uint16_t	UART_BaudRateType;
typedef uint8_t 	UART_DataRegType;
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
#endif
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


typedef uint16_t FLS_positionType;
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

/* ********************************************************************************************** */
/*                                  BEGIN TIMER Class												  */
/* ********************************************************************************************** */
#define NUM_HW_CNTR 	4
#define NUM_HW_CNTR_CH 12

typedef uint8_t         	PRS_ValueType;
typedef uint16_t        	Period_ValueType;
typedef	Period_ValueType	IO_HwTimerType;


typedef uint32_t TMR_CNT_AddressType;
typedef uint32_t TMR_CH_AddressType;
typedef struct TMR_ChMapType_tag{
	TMR_CH_AddressType Channel;
    uint8_t hwCntrlId;
}TMR_ChMapType;
/* ********************************************************************************************** */
/*                                  END TIMER Class												  */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)

#if (ADC_USE_HW_DMA)
#error "Hardware DMA not supported by H839079 microcontroller"
#endif 

#if (BUS_FREQUENCY_Hz != 20000000) && (BUS_FREQUENCY_Hz != 16000000)
#error "ADC_CONV_FREQUENCY_Hz computed only for bus frequency of 20 MHz and 16 MHz\n
please set ADC_CONV_FREQUENCY_Hz according to the new frequency and Convertion Time States with formula\n
ADC_CONV_FREQUENCY_Hz = BUS_FREQUENCY_Hz/TimeStates"
#endif
#if (BUS_FREQUENCY_Hz == 16000000)
#if (ADC_CONV_FREQUENCY_Hz == 119403)
#define ADC_HIGH_FRQ_CONV FALSE
#elif (ADC_CONV_FREQUENCY_Hz == 228571)
#define ADC_HIGH_FRQ_CONV TRUE
#else
#error "ADC_CONV_FREQUENCY_Hz must be 119403 Hz or 228571 Hz with Bus Frequency of 16 MHz"
#endif
#endif /* #if (BUS_FREQUENCY_Hz == 16000000) */

#if (BUS_FREQUENCY_Hz == 20000000)
#if (ADC_CONV_FREQUENCY_Hz == 149254)
#define ADC_HIGH_FRQ_CONV FALSE
#elif (ADC_CONV_FREQUENCY_Hz == 285714)
#define ADC_HIGH_FRQ_CONV TRUE
#else
#error "ADC_CONV_FREQUENCY_Hz must be 149254 Hz or 285714 Hz with Bus Frequency of 20 MHz"
#endif
#endif /* #if (BUS_FREQUENCY_Hz == 20000000) */

#if (ADC_RESOLUTION > 16)
#error "ADC_RESOLUTION too high, max value is 16"
#endif
#if (ADC_RESOLUTION <= 0)
#error "Are you joking?"
#endif
/* ATD Continuous Conversion not supported (SW DMA Single and Multi Channel Convertions)*/
#define ADC_NO_CONTINUOS_MODE_SUPPORTED

#endif /* #if defined(HAL_ADC_SUPPORT) */

#endif /* __HAL_RENESAS_300H_36079_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */