/**
\defgroup HAL_FS_MCF51_AG128_cfg	Hardware Abstraction Layer Configuration for Freescale MCF51 AG128

HAL configuration for the Freescale MCF51 AG128 microcontroller
\par Description
This level allows:
- the application to configure some properties and configuration of the micorcontroller, e.g. I2C register value based on 
the clock rate definition in kbps.
- define some target dependant types, defines etc.

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy name lastname

\par Parametri di configurazione
- \ref BUS_FREQUENCY_Hz_sec  Specifica la frequenza del bus del microcontrollore in Hz.
- \ref HAL_I2C_SUPPORT_sec Richiede il supporto del driver I2C.
- \ref I2C_CLOCKRATE_kbps_sec Definisce il clock rate dell'I2C. 
- \ref HAL_SWDMA_SUPPORT_sec Richiede il supporto del driver DMA sw.
- \ref SWDMA_CH_NUM_sec Definisce il numero di canali del driver DMA sw.

\date    1/12/2009
\author  Jaroslav Musil
\version 1.0.0
\ingroup HAL_FS_MCF51_AG128
*/

/**
Header file di configurazione per HAL di Freescale MCF51 AC128.

\file    FS_MCF51_AG128_cfg.h
\ingroup HAL_FS_MCF51_AG128_cfg
\date    1/12/2009
\author  Jaroslav Musil
\version 0.0.24


\section BUS_FREQUENCY_Hz_sec BUS_FREQUENCY_Hz
\subsection BUS_FREQUENCY_Hz_subsec_Values  Valori supportati
\todo indicare i valori supportati
\section HAL_I2C_SUPPORT_sec HAL_I2C_SUPPORT
Richiede la definizione di BUS_FREQUENCY_Hz.
\section I2C_CLOCKRATE_kbps_sec I2C_CLOCKRATE_kbps
Richiede la definizione di HAL_I2C_SUPPORT.
\subsection I2C_CLOCKRATE_kbps_subsec_Values  Valori supportati
\todo indicare i valori supportati
\section HAL_SWDMA_SUPPORT_sec HAL_SWDMA_SUPPORT
\section SWDMA_CH_NUM_sec SWDMA_CH_NUM


\par		History
\n
\n 		00.00.24
\n		1/12/2009 
\n		Jaroslav Musil
\n		changes
\n 		- creation


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
\addtogroup HAL_FS_MCF51_AG128_cfg
@{*/



#ifndef __HAL_FREESCALE_MCF51_AG128_CFG_H
#define __HAL_FREESCALE_MCF51_AG128_CFG_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"

/* ****** */
/* Errors */
/* ****** */
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


#endif /*  defined(HAL_UART_SUPPORT) */

#if defined(HAL_I2C_SUPPORT)


#if !defined(BUS_FREQUENCY_Hz)
#error "Propriety BUS_FREQUENCY_Hz not defined in Config.h."
#endif /* !defined(BUS_FREQUENCY_Hz)*/

/** The application is required to define the chip's operating speed */
#if !defined(I2C_CLOCKRATE_kbps)
#error "Propriety I2C_CLOCKRATE_kbps not defined in Config.h."
#endif /* !defined(I2C_CLOCKRATE_kbps)*/

#if (BUS_FREQUENCY_Hz > 24000000)  
//#error "BUS_FREQUENCY_Hz invalid value in Config.h."
#endif 


#if (I2C_CLOCKRATE_kbps!=100)
#error "I2C_CLOCKRATE_kbps invalid value in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */





#if (I2C_CLOCKRATE_kbps == 100)
#define I2C_CLOCKRATE_SETTING 0x8D /*preconditions: BUS_FREQUENCY_Hz = 19922944 MHz*/

#else
#error "Clock rate not supported.\n    \
Cambiare il valore di I2C_CLOCKRATE_kbps o BUS_FREQUENCY_Hz."

#endif /* I2C_CLOCKRATE_kbps == 100 */

//#endif /* BUS_FREQUENCY_Hz==10000000 */

/* [TODO: gestire altri valori ] */


#endif /* HAL_I2C_SUPPORT */


#if defined(HAL_SWDMA_SUPPORT)

#if !defined(SWDMA_CH_NUM)
#error "Propriety SWDMA_CH_NUM not defined in Config.h."
#endif /* !defined(I2C_CLOCKRATE_kbps)*/


#endif /* HAL_SWDMA_SUPPORT */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define OCDMA_CH_NUM  4

#define PIN_OUT_DIR   1
#define PIN_IN_DIR    0



/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */
typedef uint16_t CLK_TrimmingType;
/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                     */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN WDT Class												  */
/* ********************************************************************************************** */
typedef uint32_t WDT_windowType;
typedef uint32_t WDT_timeoutType;
typedef uint32_t WDT_counterType;
typedef uint16_t WDT_resetCntrRegType;
typedef uint8_t WDT_testByteType;
typedef uint16_t WDT_testType;
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

#define SIZE_CMD_ERASE_IN_RAM   0x1C
#define SIZE_BURST_CMD_IN_RAM	0x3E


typedef uint32_t FLS_sizeType;
typedef uint32_t FLS_positionType;
typedef uint32_t FLS_sizePLUSpositionType;
typedef FLS_positionType FLS_paramType;
typedef uint32_t* FLS_SourcePtrType;
typedef uint32_t FLS_VerifyType;
typedef uint32_t FLS_BlankCheckType;


typedef struct   
{
    uint32_t cmd[SIZE_CMD_ERASE_IN_RAM/4 + ((SIZE_CMD_ERASE_IN_RAM%4)?+1:0)];
}FLS_EraseCmdInRamType;


typedef struct   
{
    uint32_t cmd[SIZE_BURST_CMD_IN_RAM/4 + ((SIZE_BURST_CMD_IN_RAM%4)?+1:0)];
}FLS_BurstCmdInRamType;




#define FLS_BLANK_VALUE			(FLS_BlankCheckType)0xFFFFFFFF
#define SIZE_OF_FLS_PositionType    4
#define SIZE_OF_FLS_BlanckCheckType	4
#define SIZE_OF_FLS_VerifyType		4
#define FLS_MAX_ADDR        		0x1FFFF




#endif /* defined(HAL_FLS_SUPPORT) */


/* ********************************************************************************************** */
/*                                  END FLS Class												  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN System Class HAL Support                                */
/* ********************************************************************************************** */

typedef uint8_t resetSource_t;
#define RstSrcUsedByBootLoader()    3   /* please adjust this value according to the defines 
                                           RST_BY_XYZ in IO_driver.h and with the real source of
                                           reset used by Boot Loader */
/* ********************************************************************************************** */
/*                                   END System Class HAL Support                                 */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class												  */
/* ********************************************************************************************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* ********************************************************************************************** */
/*                                  BEGIN DIO Class												  */
/* ********************************************************************************************** */

#define MCU_PIN_PACKAGE   64
#define DUMMY_PORT_DATA_ADDR	0
#define PORT_DATA_PULLING_ENABLE    TRUE

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

/* ModuleIoMap */
typedef struct IO_ModuleIOMap_tag {
  DIO_ModuleMapPortAddressType PortAddress; /**< Address of the Port Data Register relsated to the port pin */
  DIO_ModuleMapMaskType        PinMask;     /**< Port pin mask  */
}IO_ModuleIOMapType;
/* ********************************************************************************************** */
/*                                  END DIO Class												  */
/* ********************************************************************************************** */

/* I2C Class type definitions */
typedef uint8_t I2C_DataRegType;
typedef uint8_t I2C_SlaveAddressType;
typedef uint8_t I2C_ClockRateType;

/* UART Class type definitions */
typedef uint16_t	UART_BaudRateType;
typedef uint8_t 	UART_DataRegType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class												  */
/* ********************************************************************************************** */
#if defined(HAL_TIMER_SUPPORT)
/* Timers's Channels Map Type */
/* Number of Hardware Counters (Eg. TPM1 Counter,TPM2 Counter) */
#define NUM_HW_CNTR     3        
/* Number of Channels linked to Hardware Counters (Eg. TPM1CH0 ) */
#define NUM_HW_CNTR_CH  12


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint32_t HAL_TimerId_t;
typedef uint32_t HAL_ChannelId_t;
typedef uint16_t TMR_ChResourceType;    /* cfr. NUM_HW_CNTR_CH */
/* Period value for the HW counter [tick] */
typedef uint16_t    Period_ValueType;
typedef	Period_ValueType    IO_HwTimerType;
/* Hardware Counter Precaler */
typedef uint8_t     PRS_ValueType;

typedef uint32_t TMR_AddressType;
typedef TMR_AddressType TMR_CNTAddressType;
typedef TMR_AddressType TMR_CHAddressType;

#define NO_CHANNEL_ID 		0xFF

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
#endif  /* #if defined(HAL_TIMER_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END TMR Class												  */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN IRQ Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_IRQ_SUPPORT)
#if (IRQ_FILTER != 0)
#if (IRQ_EDGE != IRQ_ON_EDGE_AND_LEVEL)
#error "Filtering on IRQ interrupt is possible only with "IRQ_EDGE = IRQ_ON_EDGE_AND_LEVEL" check IRQ_cfg.h"
#endif
#endif

#define             IRQ_IS_UNMASKABLE_INT

#endif /* #if defined(HAL_IRQ_SUPPORT) */

/* ********************************************************************************************** */
/*                                  BEGIN RST Class HAL Support                                   */
/* ********************************************************************************************** */

struct _ResetInfo {
	uint32_t 	BootStartKey;
	uint8_t   dummy[3];
	uint8_t		ResetSource;
};

extern struct _ResetInfo ResetInfo;

/* ********************************************************************************************** */
/*                                  END RST Class HAL Support                                   */
/* ********************************************************************************************** */


#endif /* __HAL_FREESCALE_MCF51_AG128_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */