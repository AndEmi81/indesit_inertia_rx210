/**
\defgroup HAL_FS_MC9S08_GT60_cfg	Hardware Abstraction Layer Configuration for Freescale S08 GT60

HAL configuration for Freescale MC9S08 GB60 microcontroller

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

\date    22/09/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_MC9S08_GT60
\defgroup HAL_FS_MC9S08_GT60_HISTORY_CFG  Freescale MC9S08 GT60 HAL Configuration history
\ingroup  HAL_FS_MC9S08_GT60_HISTORY
*/

/**
Configuration Header file for Freescale HCS08 GT60.

\file    FS_MC9S08_GT60_cfg.h
\ingroup HAL_FS_MC9S08_GT60_cfg
\date    22/09/2009
\author  Roberto Fioravanti
\version 1.0.0

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\addtogroup HAL_FS_MC9S08_GT60_HISTORY_CFG
@{
\n 		00.00.19
\n		xx/10/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- typedef DIO_ModuleMapPortAddressType, * DIO_ModuleMapPortType and DIO_ModuleMapMaskType for 'ModuleIOMap' table added 
\n    - #defines PIN_OUT_DIR and PIN_IN_DIR added
\n
\n 		00.00.11 
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
\addtogroup HAL_FS_MC9S08_GT60_cfg
@{*/



#ifndef __HAL_FREESCALE_MC9S08_GT60_CFG_H
#define __HAL_FREESCALE_MC9S08_GT60_CFG_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"


/* ****** */
/* Errors */
/* ****** */
#if defined(HAL_I2C_SUPPORT)

#if !defined(BUS_FREQUENCY_Hz)
#error "Proprietà BUS_FREQUENCY_Hz non definita in Config.h."
#endif /* !defined(BUS_FREQUENCY_Hz)*/

/** The application is required to define the chip's operating speed */
#if !defined(I2C_CLOCKRATE_kbps)
#error "Proprietà I2C_CLOCKRATE_kbps non definita in Config.h."
#endif /* !defined(I2C_CLOCKRATE_kbps)*/

#if (BUS_FREQUENCY_Hz!=10000000)
#error "Valore BUS_FREQUENCY_Hz non valido in Config.h."
#endif /* BUS_FREQUENCY_Hz!=10000000 */


#if (I2C_CLOCKRATE_kbps!=100) && \
    (I2C_CLOCKRATE_kbps!=200)
#error "Valore I2C_CLOCKRATE_kbps non valido in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */


#if (BUS_FREQUENCY_Hz == 10000000)

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
#error "Proprietà SWDMA_CH_NUM non definita in Config.h."
#endif /* !defined(I2C_CLOCKRATE_kbps)*/


#endif /* HAL_SWDMA_SUPPORT */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define PIN_OUT_DIR   1
#define PIN_IN_DIR    0
#define PP_END_OF_UNTOUCHABLE_RAM 0x80


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef uint8_t I2C_DataRegType;
typedef uint8_t I2C_SlaveAddressType;
typedef uint8_t I2C_ClockRateType;

/* DIO Class type definitions */
typedef uint8_t DIO_PortAddressType;
typedef struct
{
    DIO_PortAddressType basic;
    DIO_PortAddressType extended;
}DIO_ModuleMapPortAddressType;

typedef uint8_t *DIO_ModuleMapPortType;
typedef uint8_t DIO_ModuleMapMaskType;


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


#endif /* __HAL_FREESCALE_MC9S08_GT60_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */