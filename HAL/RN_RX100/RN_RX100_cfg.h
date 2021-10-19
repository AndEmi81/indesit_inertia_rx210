
#ifndef _RN_RX100_CFG_H_
#define _RN_RX100_CFG_H_

/* ******** */
/* Includes */
/* ******** */

#include "Config.h"



/** Defines the start program/erase address for the different flash areas **/
#define RAM_AREA_START_ADDRESS	0x00000000
#if defined(RN_RX100_R5F5111)
	#if defined(RN_RX100_R5F5111_5) || defined(RN_RX100_R5F5111_4)
	#define RAM_SIZE				0x00004000   /* 16 KB */
	#elif defined(RN_RX100_R5F5111_3) || defined(RN_RX100_R5F5111_1)
	#define RAM_SIZE				0x00002800	/* 10 KB */
	#else
	#define RAM_SIZE				0x00002000  /* 8 KB */
	#endif
#elif defined(RN_RX100_R5F5110)
	#if defined(RN_RX100_R5F5110_5) || defined(RN_RX100_R5F5110_4)
	#define RAM_SIZE				0x00004000
	#elif defined(RN_RX100_R5F5110_3) || defined(RN_RX100_R5F5110_1)
	#define RAM_SIZE				0x00002800
	#else
	#define RAM_SIZE				0x00002000
	#endif
#else
	#error "Microcontroller Type Not Defined"
#endif /* defined(RN_RX100_R5F511) */

/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */

typedef uint8_t CLK_TrimmingType;
#define IWDTCLK_FREQ_Hz		15000
/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                     */
/* ********************************************************************************************** */

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

#if (BUS_FREQUENCY_Hz != 32000000)
#error "Value of BUS_FREQUENCY_Hz not valid in Config.h."
#endif /* BUS_FREQUENCY_Hz!=10000000 */


#if (I2C_CLOCKRATE_kbps!=100) && \
		(I2C_CLOCKRATE_kbps!= 50) && \
		(I2C_CLOCKRATE_kbps!= 25) && \
		(I2C_CLOCKRATE_kbps!= 10)
#error "Value of I2C_CLOCKRATE_kbps not valid in Config.h."
#endif /* I2C_CLOCKRATE_kbps != xxx */

#define I2C_CLOCKRATE_SETTING		I2C_CLOCKRATE_kbps

#if (BUS_FREQUENCY_Hz == 16000000)

#if (I2C_CLOCKRATE_kbps == 100)
#define HAL_RX100_I2C_CKS_SETTING 			2
#define HAL_RX100_I2C_ICBRH_SETTING 		15
#define HAL_RX100_I2C_ICBRL_SETTING 		18
#define HAL_RX100_I2C_SDDL_SETTING 			7
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 50)
#define HAL_RX100_I2C_CKS_SETTING 			3
#define HAL_RX100_I2C_ICBRH_SETTING 		16
#define HAL_RX100_I2C_ICBRL_SETTING 		19
#define HAL_RX100_I2C_SDDL_SETTING 			5
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 25)
#define HAL_RX100_I2C_CKS_SETTING 			4
#define HAL_RX100_I2C_ICBRH_SETTING 		16
#define HAL_RX100_I2C_ICBRL_SETTING 		19
#define HAL_RX100_I2C_SDDL_SETTING 			5
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 10)
#define HAL_RX100_I2C_CKS_SETTING 			5
#define HAL_RX100_I2C_ICBRH_SETTING 		22
#define HAL_RX100_I2C_ICBRL_SETTING 		25
#define HAL_RX100_I2C_SDDL_SETTING 			1
#define HAL_RX100_I2C_DLCS_SETTING 			0

#endif

#endif /* BUS_FREQUENCY_Hz==16000000 */

#if (BUS_FREQUENCY_Hz == 20000000)

#if (I2C_CLOCKRATE_kbps == 100)
#define HAL_RX100_I2C_CKS_SETTING 			2
#define HAL_RX100_I2C_ICBRH_SETTING 		19
#define HAL_RX100_I2C_ICBRL_SETTING 		23
#define HAL_RX100_I2C_SDDL_SETTING 			7
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 50)
#define HAL_RX100_I2C_CKS_SETTING 			3
#define HAL_RX100_I2C_ICBRH_SETTING 		21
#define HAL_RX100_I2C_ICBRL_SETTING 		24
#define HAL_RX100_I2C_SDDL_SETTING 			5
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 25)
#define HAL_RX100_I2C_CKS_SETTING 			4
#define HAL_RX100_I2C_ICBRH_SETTING 		21
#define HAL_RX100_I2C_ICBRL_SETTING 		24
#define HAL_RX100_I2C_SDDL_SETTING 			5
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 10)
#define HAL_RX100_I2C_CKS_SETTING 			6
#define HAL_RX100_I2C_ICBRH_SETTING 		13
#define HAL_RX100_I2C_ICBRL_SETTING 		15
#define HAL_RX100_I2C_SDDL_SETTING 			1
#define HAL_RX100_I2C_DLCS_SETTING 			0

#endif

#endif /* BUS_FREQUENCY_Hz==20000000 */

#if (BUS_FREQUENCY_Hz == 32000000)

#if (I2C_CLOCKRATE_kbps == 100)
#define HAL_RX100_I2C_CKS_SETTING 			3
#define HAL_RX100_I2C_ICBRH_SETTING 		15
#define HAL_RX100_I2C_ICBRL_SETTING 		18
#define HAL_RX100_I2C_SDDL_SETTING 			7
#define HAL_RX100_I2C_DLCS_SETTING 			1

#elif (I2C_CLOCKRATE_kbps == 50)
#define HAL_RX100_I2C_CKS_SETTING 			4
#define HAL_RX100_I2C_ICBRH_SETTING 		16
#define HAL_RX100_I2C_ICBRL_SETTING 		19
#define HAL_RX100_I2C_SDDL_SETTING 			7
#define HAL_RX100_I2C_DLCS_SETTING 			0

#elif (I2C_CLOCKRATE_kbps == 25)
#define HAL_RX100_I2C_CKS_SETTING 			5
#define HAL_RX100_I2C_ICBRH_SETTING 		16
#define HAL_RX100_I2C_ICBRL_SETTING 		19
#define HAL_RX100_I2C_SDDL_SETTING 			7
#define HAL_RX100_I2C_DLCS_SETTING 			0

#elif (I2C_CLOCKRATE_kbps == 10)
#define HAL_RX100_I2C_CKS_SETTING 			6
#define HAL_RX100_I2C_ICBRH_SETTING 		22
#define HAL_RX100_I2C_ICBRL_SETTING 		25
#define HAL_RX100_I2C_SDDL_SETTING 			2
#define HAL_RX100_I2C_DLCS_SETTING 			0

#endif

#endif /* BUS_FREQUENCY_Hz==32000000 */


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

#define PP_END_OF_UNTOUCHABLE_RAM 				0x00
#define REMAP_ADDRESS1							0xE000
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
#define NUM_DIRECTION_REGS	    11	  /* 0xFFFFD4-0xFFFFDE (based on port data regs.) */
#define DFLT_PORT_DIR_VALUE	    0




/* Size of pointer to point Ports's Registers */
#define DUMMY_PORT_DATA_ADDR	0x00000000		/* Dummy address for dummy pin*/

typedef uint32_t DIO_PortAddressBasicType;
typedef uint32_t DIO_PortAddressExtendedType;
typedef uint8_t DIO_ModuleMapMaskType;

typedef struct
{
    DIO_PortAddressBasicType basic;
} DIO_ModuleMapPortAddressType;

typedef struct  {
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
typedef uint32_t FLS_VerifyType;
typedef FLS_VerifyType* FLS_SourcePtrType;
typedef uint32_t FLS_BlankCheckType;


typedef struct
{
    uint8_t cmd[SIZE_CMD_ERASE_IN_RAM];
} FLS_EraseCmdInRamType;


typedef struct
{
    uint8_t cmd[SIZE_BURST_CMD_IN_RAM];
} FLS_BurstCmdInRamType;


#define FLS_BLANK_VALUE		(FLS_BlankCheckType)0xFFFFFFFF
#define SIZE_OF_FLS_BlankCheckType	4
#define SIZE_OF_FLS_VerifyType		4
#define FLS_MAX_ADDR        		0xFFFFFFFF	/* TODO: proteggere la flas dove risiede il bootloader e altro */

/*#define CONFIG_HAL_OVERLAY	1 */
/* #define DATA_CLONE_RECOVERY	1 */	/* enable data clone recovery */

/* ********************************************************************************************** */
/*                                  END FLS Class												  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class												  */
/* ********************************************************************************************** */


/* ****** */
/* Errors */
/* ****** */
#if !defined(BUS_FREQUENCY_Hz)
#error "BUS_FREQUENCY_Hz not defined in Config.h."
#endif /* !defined(BUS_FREQUENCY_Hz)*/

/* number of hw counters */
#define HAL_NUM_HW_CNTR		5

/* channel default interrupt priority, 1 LOW priority*/
#define HAL_CHANNEL_DEFAULT_INT_PRIO	1

/** Counter match register TCR- >CCLR **/
#define TCNT_CLEARING_TGRA		0		/* Set the TGRA compare match register on TRC*/
#define TCNT_CLEARING_TGRB		1		/* Set the TGRB compare match register on TRC*/
#define TCNT_CLEARING_TGRC		2		/* Set the TGRC compare match register on TRC*/
#define TCNT_CLEARING_TGRD		3		/* Set the TGRD compare match register on TRC*/
#define TCNT_CLEARING_OVERFLOW	4		/* Set clearing in overflow*/
/* Hardware Counter Clear Source ID */
#define TMR_CLEAR_SOURCE_IS_INTERNAL  TCNT_CLEARING_OVERFLOW	/* clear settings that not use channels*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t HAL_TimerId_t;
typedef uint8_t HAL_ChannelId_t;

typedef uint32_t TMR_ChResourceType;    /* cfr. NUM_HW_CNTR_CH */
typedef uint32_t _HAL_RegType; 			/* Data register access  are always 32-bit*/

/* Period value for the HW counter [tick] */
/* defined for max resolution possible */
typedef uint16_t          Period_ValueType;
typedef	Period_ValueType  IO_HwTimerType;

/* Hardware Counter Prescaler */
/* define for max resolution possible */
typedef uint32_t     PRS_ValueType;

/* addresses for the control registers (always 32-bit) replaced by ID */
typedef HAL_TimerId_t   	TMR_CNTAddressType;
typedef HAL_ChannelId_t 	TMR_CHAddressType;
#define NO_CHANNEL_ID 		0xFF


/* this defines the channel used for modulo (resets the timer) */
/* includes the channei id  */
typedef struct TMR_ChModuloType_tag
{
    uint32_t tmrModuloChNum;
} TMR_ChModuloType;

typedef IO_ModuleIOMapType TMR_MapType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ********************************************************************************************** */
/*                                  END TMR Class						  */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_ADC_SUPPORT)

#if (ADC_USE_HW_DMA)
#error "Hardware DMA not supported"
#endif

#if (BUS_FREQUENCY_Hz != 32000000)
	#error "ADC_CONV_FREQUENCY_Hz computed only for bus frequency of 20 MHz and 16 MHz\n please set ADC_CONV_FREQUENCY_Hz according to the new frequency and Convertion Time States with formula\n ADC_CONV_FREQUENCY_Hz = BUS_FREQUENCY_Hz/TimeStates"
#endif


#if (ADC_RESOLUTION > 16)
#error "ADC_RESOLUTION too high, max value is 16"
#endif
#if (ADC_RESOLUTION <= 0)
#error "Are you joking?"
#endif
/* ATD Continuous Conversion not supported (SW DMA Single and Multi Channel Convertions)*/
#define ADC_NO_CONTINUOS_MODE_SUPPORTED

#endif /* #if defined(HAL_ADC_SUPPORT) */

/* ********************************************************************************************** */
/*                                  BEGIN RST Class HAL Support                                   */
/* ********************************************************************************************** */

struct _ResetInfo {
	uint32_t 		BootStartKey;
	uint32_t		ResetSource;
};

extern struct _ResetInfo ResetInfo;
extern const boot_short_info_t BootShortInfo;

#endif /* _RN_RX100_CFG_H_ */



/*@}*/

/* *********** */
/* End of file */
/* *********** */
