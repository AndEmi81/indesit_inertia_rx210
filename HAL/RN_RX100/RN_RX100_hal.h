/**
 \par Description
 \n Microcontroller Renesas RX100 Family HAL parameters

 \n
 \file    RN_RX100_hal.h
 \version 0.0.1
 \date    02/11/2011
 \author  Andrea Emili, SPES scpa

 \n
 \n
 \par		History
 \n 		0.0.1
 \n		02/11/2011
 \n		Andrea Emili
 \n		"modify" (change)
 \n		-
 \n		-(12/15/2011): Timer HAL Partialy complete
 \n		-(30/11/2001): IO Complete
 \n		-(11/15/2011): Add Clock HAç configuration
 \n		-(11/02/2011): File Creation, and clock configuration
 */

#ifndef _RN_RX100_HAL_H_
#define _RN_RX100_HAL_H_

/* ******** */
/* Includes */
/* ******** */
#include "RN_RX100_reg.h"
#include "RN_RX100_cfg.h"
#include "IO_driver.h"

/* ***************** */
/* Extern Functions */
/* ***************** */
void HAL_ErrorHandler( uint8_t err );

/* ********************************************************************************************** */
/*                                  FILE DEFINE HAL Support                                   */
/* ********************************************************************************************** */
/* Output Compare behaviour */

#define _HAL_BIT0_MASK	(1<<0)
#define _HAL_BIT1_MASK	(1<<1)
#define _HAL_BIT2_MASK	(1<<2)
#define _HAL_BIT3_MASK	(1<<3)
#define _HAL_BIT4_MASK	(1<<4)
#define _HAL_BIT5_MASK	(1<<5)
#define _HAL_BIT6_MASK	(1<<6)
#define _HAL_BIT7_MASK	(1<<7)

#define MTU_TIOR_MODE_OUTPUT_PROIBITED 0x00
#define MTU_TIOR_MODE_PIN_0_0		0x01		/* Initial output is zero, output 0*/
#define MTU_TIOR_MODE_PIN_0_1		0x02		/* Initial output is zero, output 1*/
#define MTU_TIOR_MODE_PIN_HIGH		0x03 		/* Initial output is zero toggle output at compare match*/
#define MTU_TIOR_MODE_PIN_1_0		0x05		/* Initial output is 1, output 0*/
#define MTU_TIOR_MODE_PIN_1_1		0x06		/* Initial output is 1, output 1*/
#define MTU_TIOR_MODE_PIN_1_TOOGLE	0x07		/* Initial output is high toggle output at compare match*/

#define MTU_TIOR_MODE_RISING_EDGE	(0x8)
#define MTU_TIOR_MODE_FALING_EDGE	(0x9)
#define MTU_TIOR_MODE_BOTH_EDGE		(0xa)

#define MTUO_TIOR_MODE_OUTPUT_PROIBITED		(0x0)
#define MTUO_TIOR_MODE_OUTPUT_00			(0x1)	/* Initial Output 0 - 0 at compare*/
#define MTUO_TIOR_MODE_OUTPUT_01			(0x2)	/* Initial Output 0 - 1 at compare*/
#define MTUO_TIOR_MODE_OUTPUT_0T			(0x3)	/* Initial Output 0 - toogle at compare*/
#define MTUO_TIOR_MODE_OUTPUT_10			(0x5)	/* Initial Output 1 - 0 at compare*/
#define MTUO_TIOR_MODE_OUTPUT_11			(0x6)	/* Initial Output 1 - 1 at compare*/
#define MTUO_TIOR_MODE_OUTPUT_1T			(0x7)	/* Initial Output 1 - T at compare*/
#define MTUO_TIOR_MODE_PIN_IO				(0x8)	/* Initial output is set as standard I/O */

#define SET_L_NIBBLE(X,Y)	X = ( (X & 0xF0) | (0x0F & Y))
#define SET_H_NIBBLE(X,Y)	X = ( (X & 0x0F) | (0xF0 & Y<<4))

#define GET_L_NIBBLE(X)		(X & 0x0F)
#define GET_H_NIBBLE(X)		((X & 0xF0)>>4)

#define _SET_BIT( byte, n, value )  {if( value ) byte |= (1<<n); else byte &= ~(1<<n);}
#define SET_BIT( byte, n )    ( byte |= (1<<n))
#define CLEAR_BIT( byte, n )  ( byte &= ~(1<<n))

#define GET_BIT( value, n )    ( (value>>n) & (0x1))

#define LOW   0
#define HIGH  1

#define DISABLE_REGISTER_WRITE_PROTECTION			SYSTEM.PRCR.WORD =  0xA50F
#define ENABLE_REGISTER_WRITE_PROTECTION			SYSTEM.PRCR.WORD =  0xA500
/* Macro For Common Register */
/*Low Power Mode Register*/
#define _HAL_RX100_MODULE_STOP_CONTROL_1_REGISTER_B		SYSTEM.MSTPCRB.LONG /* Module Stop Contro l Register B*/
/*End Macro For Common Register*/

#define DISABLE_PORT_REGISTER_WRITE_PROTECTION(){\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE |= 0x40;\
}
#define ENABLE_PORT_REGISTER_WRITE_PROTECTION(){\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE =  0x80;\
}

/*
 * Macro: ENABLE_ELC_HW_MODULE
 * Description: Bit9 of MSTPCRB Register, Low Power Section
 * Note:
 * Status:
 */
#define ENABLE_ELC_HW_MODULE(){\
	DISABLE_REGISTER_WRITE_PROTECTION;\
	SYSTEM.MSTPCRB.LONG &= 0xFFFFFDFF;\
	ENABLE_REGISTER_WRITE_PROTECTION;\
}/* end ENABLE_ELC_HW_MODULE*/

#define DISABLE_ELC_HW_MODULE(){\
	DISABLE_REGISTER_WRITE_PROTECTION;\
	SYSTEM.MSTPCRB.LONG |= 0x00000200;\
	ENABLE_REGISTER_WRITE_PROTECTION;\
}/* end ENABLE_ELC_HW_MODULE*/

/* ICU General HAL Macros */
/* Interrupt priority register */
#define _HAL_RX100_ICU_IPR(vector)							ICU.IPR[vector].BYTE
/* Interrupt Request Enable Register */
#define _HAL_RX100_INTERRUPT_REQUEST_ENABLE_REGISTER(ier)	ICU.IER[ier].BYTE
/* Interrupt Request Register */
#define _HAL_RX100_INTERRUPT_REQUEST_REGISTER(vector)		ICU.IR[vector].BYTE

/* ********************************************************************************************** */
/*                                  BEGIN LVD Class HAL Support                                   */
/* ********************************************************************************************** */
/* LVD Module Registers */
/* Voltage Monitor Circuit/Comparator A Control Register */
#define _HAL_RX100_VOLTAGE_MONITOR_CONTROL_REGISTER		SYSTEM.LVCMPCR.BYTE
#define LVCMPCR_EXCREFINP1_MASK		(1<<0)
#define LVCMPCR_EXVCCINP1_MASK		(1<<1)
#define LVCMPCR_EXCREFINP2_MASK		(1<<2)	/* Comparator A2 Reference Voltage*/
#define LVCMPCR_EXVCCINP2_MASK		(1<<3)	/* Comparator A2 Comparison Voltage*/
#define LVCMPCR_LVD1E_MASK			(1<<5)	/* Voltage Detection 1/Comparator A1 Enable*/
#define LVCMPCR_LVD2E_MASK			(1<<6)	/* Voltage Detection 2/Comparator A2 Enable*/
/* Voltage Detection Level Select Register */
#define _HAL_RX100_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER		SYSTEM.LVDLVLR.BYTE
#define _LVD1_DETECTION_LEVEL_LOW_MASK	0x0B
#define _LVD1_DETECTION_LEVEL_HIGH_MASK	0x01
#define _LVD2_DETECTION_LEVEL_LOW_MASK	0x80	/*Voltage Detection 2 Level Select 2.95*/
#define _LVD2_DETECTION_LEVEL_HIGH_MASK	0x00	/*Voltage Detection 2 Level Select 4.15*/
/* Voltage Monitoring 1 Circuit/Comparator A1 Control Register 0 */
#define _HAL_RX100_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_0		SYSTEM.LVD1CR0.BYTE
#define LVD1CR0_LVD1CMPE_MASK		(1<<2)
#define LVD1CR0_LVD1RIE_MASK		(1<<0)	/*Interrupt enable for V1*/
#define LVD1CR0_LVD1RI_MASK			(1<<6)	/*1 Reset enable; 0 Interrupt enable*/
/* Voltage Monitoring 2 Circuit/Comparator A2 Control Register 0 */
#define _HAL_RX100_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_0		SYSTEM.LVD2CR0.BYTE
#define LVD2CR0_LVD2CMPE_MASK		(1<<2)
#define LVD2CR0_LVD2RIE_MASK		(1<<0)	/*Interrupt enable for V1*/
#define LVD2CR0_LVD2RI_MASK			(1<<6)	/*1 Reset enable; 0 Interrupt enable*/
/* Voltage Monitoring 1 Circuit/Comparator A1 Control Register 1 */
#define _HAL_RX100_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_1		SYSTEM.LVD1CR1.BYTE
/* Voltage Monitoring 1 Circuit/Comparator A1 Status Register */
#define _HAL_RX100_VOLTAGE_MONITORING_1_STATUS_REGISTER		SYSTEM.LVD1SR.BYTE
#define LVD1SR_LVD1DET_MASK		(1<<0)
/*Voltage Monitoring 2 Circuit/Comparator A2 Control Register 1*/
#define _HAL_RX100_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_1		SYSTEM.LVD2CR1.BYTE
/*Voltage Monitoring 2 Circuit/Comparator A2 Status register*/
#define _HAL_RX100_VOLTAGE_MONITORING_2_STATUS_REGISTER		SYSTEM.LVD2SR.BYTE
#define LVD2SR_LVD2DET_MASK		(1<<0)

/*LVD2 Interrupt*/
#define LVD_INTERRUPT_VECTOR 89
#define LVD_INTERRUPT_IER_REGISTER	0x0B
#define LVD2_IER_ENABLE_MASK	(1<<1)
#define LVD_INTERRUPT_PRIORITY 0x01

#define UC_VDD_X_10 		VCC*10 					/* There is not in NXP HAL and in Freescale is in Aplliance/Configuration/BoardConfigurations.h*/

/* LVD * 10 */
#define SYS_VLVDL0  25.6
#define SYS_VLVDL1  40.0

/* LVW * 10 */
#define SYS_VLVWH0  29.5
#define SYS_VLVWH1  41.5


/*[V] Microcontroller's operating voltage Volt*10 used by LVD */
#define LVD_AllowUpDate()   			FALSE	/* Like Freescale HAL*/
/* LVD reset disable*/
#define LVDR_Disable(){\
		DISABLE_REGISTER_WRITE_PROTECTION;\
		_HAL_RX100_VOLTAGE_MONITOR_CONTROL_REGISTER = 0x00;\
		_HAL_RX100_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_0 = 0x82;\
		ENABLE_REGISTER_WRITE_PROTECTION;\
}
#define LVDWarningIntOn(){\
		_HAL_RX100_ICU_IPR(LVD_INTERRUPT_VECTOR) = LVD_INTERRUPT_PRIORITY ;\
		_HAL_RX100_INTERRUPT_REQUEST_REGISTER(LVD_INTERRUPT_VECTOR) = 0x00;\
		_HAL_RX100_INTERRUPT_REQUEST_ENABLE_REGISTER(LVD_INTERRUPT_IER_REGISTER) |= LVD2_IER_ENABLE_MASK;\
}
#define LVDWarningIntOff(){\
		_HAL_RX100_INTERRUPT_REQUEST_REGISTER(LVD_INTERRUPT_VECTOR) = 0x00;\
		_HAL_RX100_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_0 &= ~LVD2CR0_LVD2RIE_MASK;\
		_HAL_RX100_ICU_IPR(LVD_INTERRUPT_VECTOR) = 0x00;\
}
#define LVDGetWarningFlag()	(_HAL_RX100_VOLTAGE_MONITOR_CONTROL_REGISTER & 0x40)
#define LVDWarningAcknoledge()	_HAL_RX100_INTERRUPT_REQUEST_REGISTER(LVD_INTERRUPT_VECTOR) = 0x00	/*It is automatically cancelled*/
#define LVDSetStopOn()
#define LVDSetStopOff()

/*
 * Function: LVDR_SetUp
 * Description: Sets the low voltage detection level to generate LV reset. Turns on the LV detection
 *				mode and turns on the reset on LV feature.
 * Note:
 * Status:
 */
void LVDR_SetUp(uint8_t tripPointVR);

/*
 * Function: LVDW_SetTrip
 * Description: Set voltage trip to generate LVD warning
 * Note:
 * Status:
 */
void LVDW_SetTrip(uint8_t tripPointVW);

/*
 * Function: LVDR_GetTrip
 * Description:  Reads the LVD trip
 * Status:
 * Note:
 */
uint8_t LVDR_GetTrip(void);

/*
 * Function: LVDW_getTrip
 * Description: Reads the LVD warning trip
 * Note:
 * Status:
 */
uint8_t LVDW_GetTrip(void);

/* ********************************************************************************************** */
/*                                  END LVD Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN RESET Class HAL Support                                 */
/* ********************************************************************************************** */
/* CUSTOM RESET MASK TO GET THE RESET VALUE */
#define POWER_ON_RESET_MASK		(1<<0)
#define LOW_VOLTAGE_RESET_VD1	(1<<2)			/* LVD1 is configure for Low Voltage detection Reset*/
#define IWDT_RESET_MASK			(1<<16)			/* IWDT Reset*/
#define USER_RESET_MASK 		(1<<18)			/* User Reset is Software Reset*/
/*Reset Module Registers*/
/* Reset Status Register 0 */
#define _RX100_RESET_STATUS_REGISTER_0					SYSTEM.RSTSR0.BYTE
#define _RX100_POWER_ON_RESET_FLAG_MASK					0x01
#define _RX100_VOLTAGE_MONITOR_0_RESET_FLAG_MASK		0x02
#define _RX100_VOLTAGE_MONITOR_1_RESET_FLAG_MASK		0x04
#define _RX100_VOLTAGE_MONITOR_2_RESET_FLAG_MASK		0x08
#define _RX100_DEEP_SOFTWARE_STANDBY_RESET_FLAG_MASK	0x80
/* Reset Status Register 1 */
#define _RX100_RESET_STATUS_REGISTER_1					SYSTEM.RSTSR1.BYTE
#define _RX100_COLD_WARM_START_FLAG_MASK				0x01
/* Reset Status Register 2 */
#define _RX100_RESET_STATUS_REGISTER_2					SYSTEM.RSTSR2.BYTE
#define _RX100_IWDT_RESET_FLAG_MASK						0x01
#define _RX100_WDT_TIMER_RESET_FLAG_MASK				0x02
#define _RX100_SOFTWARE_RESET_FLAG_MASK					0x04
/* Software Reset Status Register */
#define _RX100_SOFTWARE_RESET_STATUS_REGISTER			SYSTEM.SWRR		/* Writing A501h re sets the LSI. These bits are read as 0000h.*/
/**/
typedef uint32_t resetSource_t;

#define _RST_VALUE				 (*((unsigned int    *) &SYSTEM.RSTSR0.BYTE))

#define SYS_SetPowerOnReset(reg)
#define SYS_SetExternalPinReset(reg)
#define SYS_SetWatchdogReset(reg)
#define SYS_SetLowVoltageReset(reg)
#define SYS_SetLossOfClockReset(reg)
#define SYS_SetIllegalOpcodeReset(reg)
#define SYS_SetIllegalAddressReset(reg)
#define SYS_SetUserReset(reg)
#define SYS_SetDebuggerReset(reg)

#define UserReset()		{DISABLE_REGISTER_WRITE_PROTECTION;_RX100_SOFTWARE_RESET_STATUS_REGISTER = 0xA501;}
/* #define SYS_UserReset(reg)					((reg == USER_RESET_MASK))*/
#define MCU_RequiresResetAfterPOR()    		FALSE		/* AE This value must be check*/

/* Reset Functions prototype*/
resetSource_t SYS_GetResetSrcVal(void);
bool_t SYS_PowerOnReset(resetSource_t rstSrc);
bool_t SYS_WatchdogReset(resetSource_t rstSrc);
bool_t SYS_ExternalPinReset(resetSource_t rstSrc);
bool_t SYS_LossOfClockReset(resetSource_t rstSrc);
bool_t SYS_IllegalOpcodeReset(resetSource_t rstSrc);
bool_t SYS_LowVoltageReset(resetSource_t rstSrc);
bool_t SYS_DebuggerReset(resetSource_t rstSrc);
bool_t SYS_IllegalAddressReset(resetSource_t rstSrc);
bool_t SYS_UserReset(resetSource_t rstSrc);

/* ********************************************************************************************** */
/*                                  END RESET Class HAL Support                                 */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_CLK_SUPPORT)

void CLK_SetTrimSync(uint16_t);
void CLK_EngageClkRefSync(void);
bool_t CLK_SetInternalClockSync(uint32_t desiredClock);
bool_t CLK_SetExternalClockSync(uint32_t desiredClock);
bool_t CLK_SetFLLExternalClockSync(uint32_t desiredClock);
bool_t CLK_SetFLLInternalClockSync(uint32_t desiredClock);

/* extensions*/
uint32_t CLK_GetInternalClockSync( void );

/* ****************** */
/* Defines and Macros */
/* ****************** */
/************/
/* Max allow device frequency */
/************/
#define CLK_MAX_DEVICE_FREQ         32000000			/* RX110 Max System Frequency*/

/************/
/* INT CLK */
/************/
/* #define CLK_INT_CLOCK_FREQ   		32000000	definita nel file di configurazione*/		/* CLK value in Hz*/
/************/
/* EXT CLK */
/************/
#if 0 /*TODO: MS gestione clock esterno, non gestita */
#define CLK_EXT_CLOCK_FREQ   		20000000			/* EXT CLK value in MHz*/
#define CLK_MIN_MAIN_CLOCK_FREQ 	1000000				/* EXT CLK min frequency 1MHz*/
#define CLK_MAX_MAIN_CLOCK_FREQ     20000000			/* EXT CLK max frequency 20MHz*/
#if (CLK_EXT_CLOCK_FREQ > CLK_MAX_MAIN_CLOCK_FREQ)		/* Check error max EXT CLK*/
#error "EXT CLK must be < 20MHz check CLK_EXT_CLOCK_FREQ DEFINE"
#endif

#if (CLK_EXT_CLOCK_FREQ < CLK_MIN_MAIN_CLOCK_FREQ)		/* Check error min EXT CLK*/
#error "EXT CLK must be > 1MHz check CLK_EXT_CLOCK_FREQ DEFINE"
#endif
/* End External Clock value assignment*/

#endif
/** Define Clock Capability */
#define _HAL_CLK_CAPABILITY

/************/
/* CLOCK related registers */
/************/

/*** End Set Clock Source ***/
/* Internal or external reference clock for the PLL input*/
/* SYSPLLCLKSEL register must be unlocked before changing the value*/

#define IRC_SOURCE 0
#define EXT_SOURCE 1

#define RX210_WRITE_PROTEC_OFF 						{SYSTEM.PRCR.WORD =  0xA503;}		/* PRCR Write Protection OFF*/
#define RX210_WRITE_PROTEC_ON 						{SYSTEM.PRCR.WORD =  0xA500;}     /* PRCR Write Protection ON*/


#define CLK_DisableFLLInBypassMode()                {}
#define CLK_EnableFLLInBypassMode()                 {} /*PLL ON*/

#define CLK_SetExternalRefOscillator()              {}
#define CLK_SetExternalRefClockSource()             {}

#define CLK_EnableInternalRefClock()				{}
#define CLK_DisableInternalRefClock()				{}

#define CLK_EnableExternalRefClock()                {CLK_SetExternalClockSource();}
#define CLK_DisableExternalRefClock()               {}

#define CLK_EnableExternalRefClockInStopMode()      {}
#define CLK_DisableExternalRefClockInStopMode()     {}

#define CLK_SetTrim(trim)                           {}
#define CLK_GetTrim()                               ()

#define CLK_IsInternalFLLRefClock()                 ()
#define CLK_IsExternalFLLRefClock()                 ()

/*#define CLK_IsFLLClockSource()                      ()*/
/*#define CLK_IsInternalClockSource()                 ()*/
/*#define CLK_IsExternalClockSource()                 ()*/


#define CLK_IsOscillatorReady()                     /*lint -e506 */(1)

#define CLK_SetFTrim(ftrim)                         {}
/*#define CLK_GetFTrim()                              ()*/

#define CLK_GetTrimSync() 0


#endif
/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_DIO_SUPPORT)

bool_t DIO_IsNotDummyIO(const IO_ModuleIOMapType* ptr);

/* RX210 Port Direction BYTE define*/
#define RX210_PORT_NUMBER	13			/* Number of Rx210 ports*/
#define PORT0_PDR_BASE_ADDRES 	0x0008C000		/* PORT Register Base Address it's PORT0 Address*/
#define PORT_PIN_IS_OUTPUT 1
#define PORT_PIN_IS_INPUT  0
#define PORT0_PDR	PORT0.PDR.BYTE
#define PORT1_PDR	PORT1.PDR.BYTE
#define PORT2_PDR	PORT2.PDR.BYTE
#define PORT3_PDR	PORT3.PDR.BYTE
#define PORT4_PDR	PORT4.PDR.BYTE
#define PORT5_PDR	PORT5.PDR.BYTE
#define PORTA_PDR	PORTA.PDR.BYTE
#define PORTB_PDR	PORTB.PDR.BYTE
#define PORTC_PDR	PORTC.PDR.BYTE
#define PORTE_PDR	PORTE.PDR.BYTE
#define PORTH_PDR	PORTH.PDR.BYTE

/* RX210 Pull Up Control define*/
#define PORT0_PCR	PORT0.PCR.BYTE
#define PORT1_PCR	PORT1.PCR.BYTE
#define PORT2_PCR	PORT2.PCR.BYTE
#define PORT3_PCR	PORT3.PCR.BYTE
#define PORT4_PCR	PORT4.PCR.BYTE
#define PORT5_PCR	PORT5.PCR.BYTE
#define PORTA_PCR	PORTA.PCR.BYTE
#define PORTB_PCR	PORTB.PCR.BYTE
#define PORTC_PCR	PORTC.PCR.BYTE
#define PORTE_PCR	PORTE.PCR.BYTE
#define PORTH_PCR	PORTH.PCR.BYTE
/* PMR */
#define PIN_IS_FOR_PHERIPHERAL_FUNCTIONS 1
#define PIN_IS_FOR_GENERIC_IO			 0
#define PORT1_PMR	PORT1.PMR.BYTE
#define PORT2_PMR	PORT2.PMR.BYTE
#define PORT3_PMR	PORT3.PMR.BYTE
#define PORT4_PMR	PORT4.PMR.BYTE
#define PORT5_PMR	PORT5.PMR.BYTE
#define PORTA_PMR	PORTA.PMR.BYTE
#define PORTB_PMR	PORTB.PMR.BYTE
#define PORTC_PMR	PORTC.PMR.BYTE
#define PORTE_PMR	PORTE.PMR.BYTE
#define PORTH_PMR	PORTH.PMR.BYTE

/* RX210  BIT MASK */
#define PORTX_BIT0_MASK		(1<<0)
#define PORTX_BIT1_MASK		(1<<1)
#define PORTX_BIT2_MASK		(1<<2)
#define PORTX_BIT3_MASK		(1<<3)
#define PORTX_BIT4_MASK		(1<<4)
#define PORTX_BIT5_MASK		(1<<5)
#define PORTX_BIT6_MASK		(1<<6)
#define PORTX_BIT7_MASK		(1<<7)

/* Register Shift: Shift from the base Address PDR */
#define REG_PODR_MASK		0x20
#define REG_PIDR_MASK		0x40
#define REG_PMR_MASK		0x60
#define REG_ODR0_MASK		0x81
#define REG_ODR1_MASK		0x82
#define REG_DSCR_MASK		0xE0
#define REG_PCR_MASK		0xC0

#define PIN_DUMMY \
{\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y) \
{\
{(DIO_PortAddressBasicType)&PORT##x##_PDR},\
(DIO_ModuleMapMaskType)PORTX_BIT##y##_MASK\
}

/* sharing between dataInput and dataOutput */
/*#define dataOutput dataInput*/
/* dummy declaration */
#define slewRate 		pullup
/* dummy declaration */
#define driveStrength	pullup

typedef struct
{
    uint8_t direction;
} portreg_struct_t;

typedef struct
{
    uint8_t pullup;
    uint8_t digitalFilter;
    uint8_t digitalFilterControl;
} portreg_ext_struct_t;


void DIO_Init(void);
void DeInitGPIO(void);

#define DIO_OUTPUT_IS_AVAILABLE				TRUE
#define IsAvailableAsOutput(ptr)        	(1)		/* param [in] ptr points to IO_ModuleIOMapType*/

/* DIO_SetDirAsOutput */
#define DIO_SetDirAsOutput(ptr)   			((portreg_struct_t *)ptr->PortAddress.basic)->direction |= ptr->PinMask;
/* DIO_IsOutput(ptr) */
#define DIO_IsOuput(ptr)					(((portreg_struct_t *)ptr->PortAddress.basic)->direction & ptr->PinMask)
/* DIO_SetHigh */
#define DIO_SetHigh(ptr)					*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PODR_MASK) |= ptr->PinMask;

/* DIO_SetLow - Set output port low  */
#define DIO_SetLow(ptr)						*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PODR_MASK) &= ~ptr->PinMask;

/*!< INPUT Enable/Disable */
#define DIO_INPUT_IS_AVAILABLE				TRUE
#define IsAvailableAsInput(ptr)				(1)

/*!< DIO_SetDirAsOutput */
#define DIO_SetDirAsInput(ptr)				((portreg_struct_t *)ptr->PortAddress.basic)->direction &= ~(ptr->PinMask);
/* DIO_GetVal */
#define DIO_GetVal(ptr)						((*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PIDR_MASK) & ptr->PinMask) != 0)

/*!< PULL UP Enable/Disable */
#define DIO_PULL_UP_IS_AVAILABLE			TRUE
#define DIO_IsAvailablePullUp(ptr) 			(1)

/*!< DIO_IsPullUpEnabled  */
#define DIO_PullUpEnable(ptr)				*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PCR_MASK) |= ptr->PinMask;

/*!< PULL UP Disable */
#define DIO_PullUpDisable(ptr)				*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PCR_MASK) &= ~(ptr->PinMask);

/* PULL UP Checj */
#define DIO_IsPullUpEnabled(ptr)			((*(&(( (portreg_struct_t *)ptr->PortAddress.basic)->direction)+REG_PCR_MASK) & ptr->PinMask) != 0)

/*!< PULL Down Enable/Disable */
#define DIO_PULL_DOWN_IS_AVAILABLE			FALSE
#define DIO_IsAvailablePullDown(ptr) 		(1)		/* In the RX210 pull down is not available (to check)*/

/*!< IsPullDownEnabled to check*/
#define DIO_IsPullDownEnabled(ptr)			(((portreg_ext_struct_t *)ptr->PortAddress.extended)->pullup==0)

/*!< SLEW RATE Enable/Disable */
#define DIO_SLEW_RATE_IS_AVAILABLE			FALSE
#define DIO_IsAvailableSlewRate(ptr)		(0)		/* Slew rate on RX210 is not available (to check)*/

/*!< DIO Drive Strenght Enable/Disable*/
#define DIO_DRIVE_STRENGTH					FALSE

/* #define DIO_IsAvailableDriveStrength(ptr)      (0) */
/* #define DIO_DriveStrengthEnable(ptr)      ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength =1; */
/* #define DIO_DriveStrengthDisable(ptr)     ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength =0; */
/* #define DIO_IsDriveStrengthEnabled(ptr)   (((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength == 1) */

#define DIO_PASSIVE_FILTER_IS_AVAILABLE  	FALSE	/* Passive filtering functionality in not available in RX210 devices*/

/*May 2011 Driver modification*/
/*GPIO Clock gating*/

/*Modification May 2011*/

/*Attenzione deve essere interpretata, per il momento identica all' interrupt enable*/

#define NVIC_PIO_2   43
#define NVIC_PIO_1   42
#define NVIC_PIO_0   41

/*
 * Macro:
 * Description:
 * Note:
 * Status:
 */
#define DIO_IsInterruptFlag(pMap)\
		 ((pMap->PortAddress.basic == 1)\
			||(pMap->PortAddress.basic == 1)\
		  ||(pMap->PortAddress.basic == 1))



/*
 * Macro: DIO_SetInterruptMode_FallingEdge
 * Description:
 * Note:
 * Status:
 */
/* #define DIO_SetInterruptMode_FallingEdge(ptr)*/

/*
 * Macro:
 * Description:
 * Note:
 * Status:
 */
/*#define DIO_SetInterruptMode_RisingEdge(ptr)*/

#define S_MODE3       3
#define S_MODE2       2
#define S_MODE1       1
#define NO_FILTERING  0


/*NOTA:  L'interrupt sui digital IO Non  e' Disponibile */
#define DIO_INTERRUPT_IS_AVAILABLE    		FALSE

/* #define DIO_IsInterruptAvailable(ptr)       (0) */
/* #define DIO_InterruptEnable(ptr) */
/* #define DIO_InterruptDisable(ptr) */
/* #define DIO_IsInterruptEnabled(ptr) */
/* #define DIO_ClearInterrupt(ptr) */
/* #define DIO_LevelDetectInterruptEnable(ptr) */
/* #define DIO_EdgeDetectInterruptEnable(ptr) */
/* #define DIO_GetDetectInterruptMode(ptr) */
/* #define DIO_IsInterruptMode_RisingEdge(ptr) */

/* Il DMA sui digital IO non e' disponibile  */
#define DIO_DMA_IS_AVAILABLE  				FALSE
/* #define DIO_IsDmaAvailable(ptr) */
/* #define DIO_DmaEnable(ptr) */
/* #define DIO_DmaDisable(ptr) */
/* #define DIO_IsDmaEnabled(ptr) */


/*====================*/
/* IO DIGITAL FILTER SECTION*/
/*====================*/
#define DIO_DIGITAL_FILTER_IS_AVAILABLE   TRUE

#define DIGITAL_FILTER_MTU0 0
#define DIGITAL_FILTER_MTU1 1
#define DIGITAL_FILTER_MTU2 3
#define DIGITAL_FILTER_MTU3 4
#define DIGITAL_FILTER_MTU4 5
#define DIGITAL_FILTER_IRQ0 6
#define DIGITAL_FILTER_IRQ1 7
#define DIGITAL_FILTER_IRQ5 8
#define DIGITAL_FILTER_IRQ6 9


#define DIGITAL_FILTER_CLOCK_IRQ	IRQ_DIGITAL_FILTER_CLOCK_DIV_8
#define IRQ_DIGITAL_FILTER_CLOCK_DIV_1 		0
#define IRQ_DIGITAL_FILTER_CLOCK_DIV_8 		1
#define IRQ_DIGITAL_FILTER_CLOCK_DIV_32 	2
#define IRQ_DIGITAL_FILTER_CLOCK_DIV_64 	3

#define DIGITAL_FILTER_CLOCK_TIMER  TIMER_DIGITAL_FILTER_CLOCK_DIV_8
#define TIMER_DIGITAL_FILTER_CLOCK_DIV_1 	0x00
#define TIMER_DIGITAL_FILTER_CLOCK_DIV_8 	0x10
#define TIMER_DIGITAL_FILTER_CLOCK_DIV_32 0x20


typedef struct IO_DIGITAL_NOISE_tag
{
    DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register relaated to the port pin */
    DIO_ModuleMapMaskType PinMask; /**< Port pin mask  */
    uint8_t channel;
    uint8_t digital_filter_peripheral;
} IO_DIGITAL_NOISE_MapType;

#define IO_DIGITAL_FILTER_PIN(x,y,z,h) {\
(DIO_PortAddressBasicType)&PORT##x##_PDR,\
(DIO_ModuleMapMaskType)PORTX_BIT##y##_MASK,\
z,\
h\
}

#define MPC_PFS_REGISTER(x,y)	MPC.P##x##y##PFS.BYTE

/*
 * Macro: DIO_DigitalFilterNumSamplesGet
 * Description:
 * Note:
 * Status:
 */
#define DIO_DigitalFilterNumSamplesGet(ptr) 	3		/* In RX210 this parameter is not sizeable */
#define DIO_DigitalFilterNumSamplesSet(ptr,nSamples)	/* This function do nothing */
/*
 * Functions Declaration
 */
unsigned char DIO_IsDigitalFilterAvailable(const IO_ModuleIOMapType* pMap);
void          DIO_DigitalFilterEnable(const IO_ModuleIOMapType* pMap);
void          DIO_DigitalFilterClkFrequencySet(const IO_ModuleIOMapType* pMap, uint8_t filterFrequency);
unsigned char DIO_DigitalFilterClkFrequencyGet(const IO_ModuleIOMapType* pMap);
void          DIO_DigitalFilterDisable(const IO_ModuleIOMapType* pMap);
unsigned char DIO_IsDigitalFilterEnabled(const IO_ModuleIOMapType* pMap);
unsigned char DIO_CheckDigitalFilterPin(const IO_ModuleIOMapType* pMap);

/*
 * PortAddress1,PortAddress2 of type DIO_ModuleMapPortAddressType
 */
#define ModuleMapPortAddressMatch(PortAddress1, PortAddress2)		(PortAddress1.basic == PortAddress2.basic)
/*
 * PortAddress of type DIO_ModuleMapPortAddressType
 */
#define ModuleMapPortAddressIsValid(PortAddress)		(PortAddress.basic != DUMMY_PORT_DATA_ADDR)


#endif /* #if defined(HAL_DIO_SUPPORT) */
/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class HAL Support                                   */
/* ********************************************************************************************** */
/**
 * TIMER MAP RX210 CASTELLATED
 * - MTIOC0C Pin12	P32
 * - MTIOC2A Pin16	P26
 * - MTIOC3A Pin20	P14
 * - MTIOC0A Pin36	PB3
 * - MTIOC1A Pin47	PE4
 * - MTIOC3C Pin28	PC6
 * - MTIOC4C Pin37	PB1
 * - MTIOC4A Pin45	PA0
 */
#if defined(HAL_TIMER_SUPPORT)

/** Define TMR Capability */
#define _HAL_TMR_CAPABILITY
/* TIMER MACRO */
#define RX210_TMR_CH_OC_TOGGLE_TGRA   0x20   	/* Toggle output on compare Threshold TGRA */
#define RX210_TMR_CH_OC_TOGGLE_TGRB   0x40	/* Toggle output on compare Threshold TGRB */
#define RX210_TMR_CH_OC_TOGGLE_TGRC   0xA0	/* Toggle output on compare Threshold TGRC only MTU[0,1,3] */
#define RX210_TMR_CH_OC_PIN_HIGH      0x03   	/* Initial output is zero toggle output at compare match*/
#define RX210_CH_OC_PIN_LOW       	  0x07   	/* Initial output is high toggle output at compare match*/
/** ICU MACRO **/
#define RX210_TMR_CH_IC_POS_EDGE 		0x08	/* Input capture at rising edge*/
#define RX210_TMR_CH_IC_NEG_EDGE 		0x09	/* Input capture at falling edge*/
#define RX210_TMR_CH_IC_BOTH_EDGE 		0x0C	/* Input capture at both edge*/
/** Timer Enable Macro**/
#define MTU2A_TMU0_ENABLE		0x01		/* MTU2A peripheral TMU0 Enable*/
#define MTU2A_TMU1_ENABLE		0x02		/* MTU2A peripheral TMU1 Enable*/
#define MTU2A_TMU2_ENABLE		0x04		/* MTU2A peripheral TMU2 Enable*/
#define MTU2A_TMU3_ENABLE		0x40		/* MTU2A peripheral TMU3 Enable*/
#define MTU2A_TMU4_ENABLE		0x80		/* MTU2A peripheral TMU4 Enable*/
/* Register TIMER Define */
/* MUT Register*/
#define MTU2A_DISABLE_WRITE_PROTECTION	MTU.TRWER.BYTE = 0x00 	/* To use in MTU3 and MTU4*/
#define MTU2A_ENABLE_WRITE_PROTECTION	MTU.TRWER.BYTE = 0x01
/* MTU0 Register*/
#define MTU0_TCR	MTU0.TCR.BYTE
#define MTU0_TSR	MTU0.TSR.BYTE		/* b7 counter direction*/
#define MTU0_TGRA	MTU0.TGRA			/* MTUO Timer general register A*/
#define MTU0_TGRB 	MTU0.TGRB			/* MTUO Timer general register B*/
#define MTU0_TGRC 	MTU0.TGRC			/* MTUO Timer general register C*/
#define MTU0_TGRD 	MTU0.TGRD			/* MTUO Timer general register D*/
#define MTU0_TGRE 	MTU0.TGRE			/* MTUO Timer general register E*/
#define MUT0_TGRF 	MTU0.TGRF			/* MTUO Timer general register F*/
#define MTU0_TCNT	MTU0.TCNT			/* MTUO Timer Counter register*/
#define MTU0_TIER	MTU0.TIER.BYTE		/* MTU0 Interrupt enable/disable register*/
/* MTU1 Register */
#define MTU1_TCR	MTU1.TCR.BYTE
#define MTU1_TSR	MTU1.TSR.BYTE		/* b7 counter direction*/
#define MTU1_TGRA	MTU1.TGRA			/* MTU1  Timer general register A*/
#define MTU1_TGRB	MTU1.TGRB			/* MTU1  Timer general register B*/
#define MTU1_TCNT	MTU1.TCNT			/* MTU1  Timer Counter register*/
#define MTU1_TIER	MTU1.TIER.BYTE		/* MTU1 Interrupt enable/disable register*/
/*MTU2 Register*/
#define MTU2_TCR	MTU2.TCR.BYTE
#define MTU2_TSR	MTU2.TSR.BYTE		/* b7 counter direction*/
#define MTU2_TGRA	MTU2.TGRA			/* MTU2  Timer general register A*/
#define MTU2_TGRB	MTU2.TGRB			/* MTU2  Timer general register B*/
#define MTU2_TCNT	MTU2.TCNT			/* MTU2  Timer Counter register*/
#define MTU2_TIER	MTU2.TIER.BYTE		/* MTU2 Interrupt enable/disable register*/
/*MTU3 Register*/
#define MTU3_TCR	MTU3.TCR.BYTE
#define MTU3_TSR	MTU3.TSR.BYTE		/* b7 counter direction*/
#define MTU3_TGRA	MTU3.TGRA			/* MTU3  Timer general register A*/
#define MTU3_TGRB	MTU3.TGRB			/* MTU3  Timer general register B*/
#define MTU3_TGRC	MTU3.TGRC			/* MTU3  Timer general register B*/
#define MTU3_TGRD	MTU3.TGRD			/* MTU3  Timer general register D*/
#define MTU3_TCNT	MTU3.TCNT			/* MTU3  Timer Counter register*/
#define MTU3_TIER	MTU3.TIER.BYTE		/* MTU3 Interrupt enable/disable register*/
/*MTU4 Register*/
#define MTU4_TCR	MTU4.TCR.BYTE
#define MTU4_TSR	MTU4.TSR.BYTE		/* b7 counter direction*/
#define MTU4_TGRA	MTU4.TGRA			/* MTU4 Timer general register A*/
#define MTU4_TGRB	MTU4.TGRB			/* MTU4 Timer general register B*/
#define MTU4_TGRC	MTU4.TGRC			/* MTU4 Timer general register C*/
#define MTU4_TGRD	MTU4.TGRD			/* MTU4 Timer general register D*/
#define MTU4_TCNT	MTU4.TCNT			/* MTU4 Timer Counter register*/
#define MTU4_TIER	MTU4.TIER.BYTE		/* MTU4 Interrupt enable/disable register*/

/* MTU5 Register*/
#define MTU5_TCNTU	MTU5.TCNTU			/* MTU5 Timer Counter register*/
#define MTU5_TGRU	MTU5.TGRU			/* MTU5 Timer general register U*/
#define MTU5_TCNTV	MTU5.TCNTV			/* MTU5 Timer general register V*/
#define MTU5_TGRV	MTU5.TGRV			/* MTU5 Timer general register V*/
#define MTU5_TCNTW	MTU5.TCNTW			/* MTU5 Timer general register W*/
#define MTU5_TGRW	MTU5.TGRW			/* MTU5 Timer general register W*/

/*TIMER UNIT*/
#define TMR_MTU0	0
#define TMR_MTU1	1
#define TMR_MTU2	2
#define TMR_MTU3	3
#define TMR_MTU4	4
#define TMR_MTU5	5

/*TIMER CHANNEL*/
#define CH_TGRA		0
#define CH_TGRB		1
#define CH_TGRC		2
#define CH_TGRD		3
#define CH_TGRE		4
#define CH_TGRF		5


#define _PR_DEFAULT 0xF8		/* Prescaler reset value (TCR bo-b3 clear to 0)*/
#define ENABLE_MTU2A_MASK		0xFFFFFDFF		/* MTU2A 16 bit counter, 6 channels, must use in AND*/
#define DISABLE_MTU2A_MASK		0x00000200		/* MTU2A 16 bit counter, 6 channels, must use in OR*/




#define TMR_PIN_DUMMY \
{\
(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,\
(DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR\
}

#define HAL_CH_TGRA0	0
#define HAL_CH_TGRB0	1
#define HAL_CH_TGRC0	2
#define HAL_CH_TGRD0	3
#define HAL_CH_TGRV0	4	/* overfow MTU0*/
#define HAL_CH_TGRE0	5
#define HAL_CH_TGRF0	6
/* MTU1 ------------------*/
#define HAL_CH_TGRA1	7
#define HAL_CH_TGRB1	8
#define HAL_CH_TGRV1	9	/*overfow MTU1*/
#define HAL_CH_TGRU1	10
/* MTU2 ------------------*/
#define HAL_CH_TGRA2	11
#define HAL_CH_TGRB2	12
#define HAL_CH_TGRV2	13	/* overfow MTU2*/
#define HAL_CH_TGRU2	14
#if defined(RN_RX100_R5F5111)
/* MTU3 ------------------*/
#define HAL_CH_TGRA3	15
#define HAL_CH_TGRB3	16
#define HAL_CH_TGRC3	17
#define HAL_CH_TGRD3	18
#define HAL_CH_TGRV3	19	/* overfow MTU3*/
/* MTU4 ------------------*/
#define HAL_CH_TGRA4	20
#define HAL_CH_TGRB4	21
#define HAL_CH_TGRC4	22
#define HAL_CH_TGRD4	23
#define HAL_CH_TGRV4	24	/* overfow MTU4*/

/* MTU5 non gestito*/
#define HAL_CH_TGRU5	25
#define HAL_CH_TGRV5	26
#define HAL_CH_TGRW5	27
#endif /* defined(RN_RX100_R5F5111) */
#define HAL_CH_MAX		25
/* Internal HAL Timer ID*/
#define HAL_TMR_MTU0	0
#define HAL_TMR_MTU1	1
#define HAL_TMR_MTU2	2
#if defined(RN_RX100_R5F5111)
#define HAL_TMR_MTU3	3
#define HAL_TMR_MTU4	4
#endif /* defined(RN_RX100_R5F5111) */
/*non gestire #define HAL_TMR_MTU5	5*/

/*TMR Functions prototypes*/
uint8_t MapClearResourceValue(uint8_t clrSrcId);
uint8_t MapClearResourceId(uint8_t clrValue);
uint8_t TMR_GetClearSourceId(          TMR_CNTAddressType tmrId);

/*TODO: verificare la gestine del supporto TMR_SetThreshold2*/
bool_t   Threshold2SetUpUnsupported(uint8_t channel);
#define  TMR_SetThreshold2(channel, value)

bool_t   TMR_CheckClearSourceIdFailed( TMR_CNTAddressType tmrId, uint8_t clearingSourceId);
TMR_CHAddressType  TMR_GetClearSourceCh(         TMR_CNTAddressType tmrId, uint8_t clearingSourceId);
#define  TMR_GetClearSourceCh TMR_GetClearSourceCh
void     TMR_ClockGate(                TMR_CNTAddressType tmrId, bool_t gate);
void     TMR_SetPrescaler(             TMR_CNTAddressType tmrId, uint8_t prescaler);
uint32_t TMR_GetPrescalerDiv(          TMR_CNTAddressType tmrID );
uint32_t TMR_GetTickResolution(        TMR_CNTAddressType tmrId );
#define TMR_GetTickResolution TMR_GetTickResolution /* needed to ?define a hal feature */

uint32_t TMR_SetSetPrescaleForTickResolution(TMR_CNTAddressType tmrId, uint32_t tickRes);
void     TMR_SetPrescalerDfltValue(    TMR_CNTAddressType tmrId);
void     TMR_SetCountDir( TMR_CNTAddressType pTmr_cnt, uint8_t direction );
void     TMR_SetCounterDfltValue(     TMR_CNTAddressType tmrID);
void     TMR_SetModulo(               TMR_CNTAddressType tmrId, Period_ValueType period, uint8_t clrSrcId);
void     TMR_StopClockSource(         TMR_CNTAddressType tmrId);

void   TMR_DisableTI( TMR_CNTAddressType tmrId, TMR_CHAddressType channel);
void   TMR_ClearTOF(  TMR_CNTAddressType tmrId, TMR_CHAddressType channel);
void   TMR_EnableTI(  TMR_CNTAddressType tmrId, TMR_CHAddressType channel);
bool_t TMR_GetTOF(    TMR_CNTAddressType tmrId, TMR_CHAddressType channel);


void     TMR_SetChannelInputChapture( TMR_CHAddressType chId);
void     TMR_SetChannelOutputCompare( TMR_CHAddressType chId);

void     TMR_SetChannelPWM(           TMR_CHAddressType chId);
void     TMR_ToggleChannelEdgeMode(   TMR_CHAddressType chId);
void	 TMR_SetTimerDefaultMode( 	  TMR_CNTAddressType tmdID);
void     TMR_SetTimeOn(               TMR_CHAddressType chId, Period_ValueType tOn);
void     TMR_GetTimeOn(               TMR_CHAddressType chID, Period_ValueType *tOn);
Period_ValueType     TMR_GetChValue(  TMR_CHAddressType chID);
void     TMR_SetChannelPulses(        TMR_CHAddressType chId, uint8_t inverted);
void     TMR_ClearChannelPulses(TMR_CHAddressType chId);
void     TMR_SetChannelEdgeMode(      TMR_CHAddressType chId, uint8_t mode, bool_t inversion);
Period_ValueType TMR_GetValue(TMR_CNTAddressType tmrID);
void     TMR_EnableCHI(		      TMR_CHAddressType chId);
void     TMR_DisableCHI(	      TMR_CHAddressType chId);
bool_t   TMR_ReadCHF(                 TMR_CHAddressType chId);
void     TMR_ClearCHF(                TMR_CHAddressType chId);
void     TMR_SetThreshold1(           TMR_CHAddressType chId, uint16_t value);
void     TMR_SetModuloDfltValue(      TMR_CNTAddressType tmrId, uint8_t clrSrcId);
Period_ValueType TMR_GetModulo(TMR_CNTAddressType tmdID, uint8_t channel);
uint8_t  TMR_GetChannelEdgeMode(      TMR_CHAddressType chId);
void     TMR_SetCountDirDfltValue(    TMR_CNTAddressType tmrId);
IO_HwTimerType TMR_GetCounter(        TMR_CNTAddressType tmrId);
void     TMR_SetCounter(              TMR_CNTAddressType tmrId, uint16_t value);
bool_t OutputCompareModeUnsupported(uint8_t mode, bool_t inversion);
void     TMR_SetClockSource(          TMR_CNTAddressType tmrID, uint8_t source);
void     TMR_SetChannelOutputMode(    TMR_CHAddressType chId, uint8_t mode, bool_t inversion);
void     TMR_DeInitChannel(           TMR_CHAddressType chId);
Period_ValueType TMR_GetDelta(        TMR_CNTAddressType tmrId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR);


void TMR_StopClockSourceMulti(uint32_t uBmp);
void TMR_SetClockSourceMulti(uint32_t uBmp);

#endif /*!< End TMR class HAL support*/
/* ********************************************************************************************** */
/*                                  END TMR Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN I2C Class HAL Support                                   */
/* ********************************************************************************************** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* I2C HAL characterization */
#define		OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR			(0)
#define		OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK	(1)
#define		OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK					(1)
#define		I2C_TXRX_SWITCH_BYDRV
/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY
/** I2C Module ON/OFF **/
#define I2C_MODULE_OFF	SYSTEM.MSTPCRB.LONG |= 0x00200000		/* Reg MSTPCRB b21, 1-> OFF*/
#define I2C_MODULE_ON	SYSTEM.MSTPCRB.LONG &= 0xFFDFFFFF		/* Reg MSTPCRB b21, 0-> ON*/
/**	RIICO RX210 Register define **/
/*
 I2C related registers redefinition
 This approach helps when generating new files for additional parts of the same family.
 */
/* HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION> */
#define _HAL_RX100_I2C_ICCR1_REG	RIIC0.ICCR1.BYTE					/* I2C Bus Control register 1*/
#define _HAL_RX100_I2C_ICCR2_REG	RIIC0.ICCR2.BYTE					/* I2C Bus Control register 2*/
#define _HAL_RX100_I2C_ICMR1_REG	RIIC0.ICMR1.BYTE					/* I2C Bus mode register 1*/
#define _HAL_RX100_I2C_ICMR2_REG	RIIC0.ICMR2.BYTE					/* I2C Bus mode register 2*/
#define _HAL_RX100_I2C_ICMR3_REG	RIIC0.ICMR3.BYTE					/* I2C Bus mode register 3*/
#define _HAL_RX100_I2C_ICFER_REG	RIIC0.ICFER.BYTE					/* I2C bus function enable register*/
#define _HAL_RX100_I2C_ICSER_REG	RIIC0.ICSER.BYTE					/* I2C Bus status Enable register*/
#define _HAL_RX100_I2C_ICIER_REG	RIIC0.ICIER.BYTE					/* I2C Bus Interrupt Control register*/
#define _HAL_RX100_I2C_ICSR1_REG	RIIC0.ICSR1.BYTE					/* I2C Bus status register 1*/
#define _HAL_RX100_I2C_ICSR2_REG	RIIC0.ICSR2.BYTE					/* I2C Bus status register 2*/
#define _HAL_RX100_I2C_SARL0_REG	RIIC0.SARL0.BYTE					/* I2C Slave address Register L0*/
#define _HAL_RX100_I2C_SARL1_REG	RIIC0.SARL1.BYTE					/* I2C Slave address Register L1*/
#define _HAL_RX100_I2C_SARL2_REG	RIIC0.SARL2.BYTE					/* I2C Slave address Register L2*/
#define _HAL_RX100_I2C_SARU0_REG	RIIC0.SARU0.BYTE					/* I2C Slave address Register U0*/
#define _HAL_RX100_I2C_SARU1_REG	RIIC0.SARU1.BYTE					/* I2C Slave address Register U1*/
#define _HAL_RX100_I2C_SARU2_REG	RIIC0.SARU2.BYTE					/* I2C Slave address Register U2*/
#define _HAL_RX100_I2C_ICBRL_REG	RIIC0.ICBRL.BYTE					/* I2C Bus Bit  rate Low-level register*/
#define _HAL_RX100_I2C_ICBRH_REG	RIIC0.ICBRH.BYTE					/* I2C Bus Bit rate High-level register*/
#define _HAL_RX100_I2C_ICDRT_REG	RIIC0.ICDRT							/* I2C Bust Trasmit Data Register*/
#define _HAL_RX100_I2C_ICDRR_REG	RIIC0.ICDRR							/* I2C Bus Receiver Data register*/
#define _HAL_RX100_I2C_ICDRS_REG	RIIC0.ICDRS.BYTE					/* I2C Bus shift register*/
/* Masks */
#define _HAL_RX100_I2C_ICCR2_ST_MASK					0x02
#define _HAL_RX100_I2C_ICCR2_RS_MASK					0x04
#define _HAL_RX100_I2C_ICCR2_SP_MASK					0x08
#define _HAL_RX100_I2C_ICCR2_RSPA_MASK					0x10
#define _HAL_RX100_I2C_ICCR2_TRS_MASK					0x20
#define _HAL_RX100_I2C_ICCR2_MST_MASK					0x40
#define _HAL_RX100_I2C_ICCR2_BBSY_MASK					0x80

#define _HAL_RX100_I2C_ICSR1_AAS0_MASK					0x01
#define _HAL_RX100_I2C_ICSR1_AAS1_MASK					0x02
#define _HAL_RX100_I2C_ICSR1_AAS2_MASK					0x04
#define _HAL_RX100_I2C_ICSR1_GCA_MASK					0x08
#define _HAL_RX100_I2C_ICSR1_DID_MASK					0x20
#define _HAL_RX100_I2C_ICSR1_HOA_MASK					0x80

#define _HAL_RX100_I2C_ICSR2_TMOF_MASK					0x01
#define _HAL_RX100_I2C_ICSR2_AL_MASK					0x02
#define _HAL_RX100_I2C_ICSR2_START_MASK					0x04
#define _HAL_RX100_I2C_ICSR2_STOP_MASK					0x08
#define _HAL_RX100_I2C_ICSR2_NACKF_MASK					0x10
#define _HAL_RX100_I2C_ICSR2_RDRF_MASK					0x20
#define _HAL_RX100_I2C_ICSR2_TEND_MASK					0x40
#define _HAL_RX100_I2C_ICSR2_TDRE_MASK					0x80

#define _HAL_RX100_I2C_ICMR3_ACKBR_MASK					0x04
#define _HAL_RX100_I2C_ICMR3_ACKBT_MASK					0x08
#define _HAL_RX100_I2C_ICMR3_ACKWP_MASK					0x10
#define _HAL_RX100_I2C_ICMR3_RDRFS_MASK					0x20
#define _HAL_RX100_I2C_ICMR3_WAIT_MASK					0x40
#define _HAL_RX100_I2C_ICMR3_SMBS_MASK					0x80

#define _HAL_RX100_I2C_ICIER_TMOIE_MASK					0x01
#define _HAL_RX100_I2C_ICIER_ALIE_MASK					0x02
#define _HAL_RX100_I2C_ICIER_STIE_MASK					0x04
#define _HAL_RX100_I2C_ICIER_SPIE_MASK					0x08
#define _HAL_RX100_I2C_ICIER_NACKIE_MASK				0x10
#define _HAL_RX100_I2C_ICIER_RIE_MASK					0x20
#define _HAL_RX100_I2C_ICIER_TEIE_MASK					0x40
#define _HAL_RX100_I2C_ICIER_TIE_MASK					0x80

/** Interrupt Definitions **/
/* I2C EEI INTERRUPT DEFINE*/

#define	I2C_EEI_INTERRUPT_ENABLE	_HAL_RX100_I2C_ICIER_REG |= _HAL_RX100_I2C_ICIER_NACKIE_MASK
#define I2C_EEI_INTERRUPT_DISABLE	_HAL_RX100_I2C_ICIER_REG &= ~_HAL_RX100_I2C_ICIER_NACKIE_MASK
#define I2C_EEI_INT_PRIORITY		0x01
#define I2C_EEI_INT_VECTOR			246
#define I2C_EEI_INT_IER_INDEX		0x1E
#define I2C_EEI_IER_BIT				(1<<6)

/*I2C RX INTERRUPT DEFINE*/
#define	I2C_RX_INTERRUPT_ENABLE		_HAL_RX100_I2C_ICIER_REG |= _HAL_RX100_I2C_ICIER_RIE_MASK
#define I2C_RX_INTERRUPT_DISABLE	_HAL_RX100_I2C_ICIER_REG &= ~_HAL_RX100_I2C_ICIER_RIE_MASK
#define I2C_RX_INT_PRIORITY			0x01
#define I2C_RX_INT_VECTOR			247
#define I2C_RX_INT_IER_INDEX		0x1E
#define I2C_RX_IER_BIT				(1<<7)

/*I2C TX INTERRUPT DEFINE*/
#define	I2C_TX_INTERRUPT_ENABLE		_HAL_RX100_I2C_ICIER_REG |= _HAL_RX100_I2C_ICIER_TIE_MASK
#define I2C_TX_INTERRUPT_DISABLE	_HAL_RX100_I2C_ICIER_REG &= ~_HAL_RX100_I2C_ICIER_TIE_MASK
#define	I2C_TX_INT_PRIORITY			0x01
#define I2C_TX_INT_VECTOR			248
#define I2C_TX_INT_IER_INDEX		0x1F
#define I2C_TX_IER_BIT				(1<<0)


/*I2C TX END INTERRUPT DEFINE*/
#define	I2C_TX_END_INTERRUPT_ENABLE		_HAL_RX100_I2C_ICIER_REG |= _HAL_RX100_I2C_ICIER_TEIE_MASK
#define I2C_TX_END_INTERRUPT_DISABLE	_HAL_RX100_I2C_ICIER_REG &= ~_HAL_RX100_I2C_ICIER_TEIE_MASK
#define	I2C_TX_END_INT_PRIORITY			0x01
#define I2C_TX_END_INT_VECTOR			249
#define I2C_TX_END_INT_IER_INDEX		0x1F
#define I2C_TX_END_IER_BIT				(1<<1)

/** Start, Stop, Restart Definitions **/
#define _HAL_I2C_START_CONDITION 	_HAL_RX100_I2C_ICCR2_REG = 0x02
#define _HAL_I2C_RESTART_CONDITION 	_HAL_RX100_I2C_ICCR2_REG = 0x04
#define _HAL_I2C_STOP_CONDITION 	_HAL_RX100_I2C_ICCR2_REG = 0x08


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
#define MASTER_ARBITRATION_LOST        0x02
#define MASTER_RESTART_CONDITION       0x10

/*     Miscellaneous States      */

#define I2C_UNKNOWN_STATE                  (0xF8)

/* Address Type Macro */
#define _HAL_RX_I2C_ADDR0_7BITS		_HAL_RX100_I2C_SARU0_REG =  0x00		/* The 7-bit address format is selected*/
#define _HAL_RX_I2C_ADDR0_10BITS	_HAL_RX100_I2C_SARU0_REG |= 0x01		/* The 10-bit address format is selected*/
#define _HAL_RX_I2C_ADDR1_7BITS		_HAL_RX100_I2C_SARU1_REG =  0x00		/* The 7-bit address format is selected*/
#define _HAL_RX_I2C_ADDR1_10BITS	_HAL_RX100_I2C_SARU1_REG |= 0x01		/* The 10-bit address format is selected*/
#define _HAL_RX_I2C_ADDR2_7BITS		_HAL_RX100_I2C_SARU2_REG =  0x00		/* The 7-bit address format is selected*/
#define _HAL_RX_I2C_ADDR2_10BITS	_HAL_RX100_I2C_SARU2_REG |= 0x01		/* The 10-bit address format is selected*/
/* Control Registers Set and Reset*/
#define _HAL_RX100_I2C_ENABLE_REG	_HAL_RX100_I2C_ICCR1_REG = 0x90

/* Flow Control Registers */
#define _HAL_RX100_I2C_DISABLE_REG	_HAL_RX100_I2C_ICCR1_REG = 0x10

/* Status Registers*/
#define _HAL_RX100_I2C_STATUS_REG	_HAL_RX100_I2C_ICSR1_REG		/* RIIC status register*/
#define _HAL_RX100_I2C_BUSBUSY_REG	(_HAL_RX100_I2C_ICCR2_REG & 0x80)

/****************************/
/** 		I2C MACRO	   **/
/****************************/
/*
 * Macro: I2C_ClkEnable
 * Description: Enable the I2C RX200 module
 * Status: Verified
 */
#define I2C_ClkEnable() {\
							DISABLE_REGISTER_WRITE_PROTECTION;\
							I2C_MODULE_ON;\
							ENABLE_REGISTER_WRITE_PROTECTION;\
						}

/*
 * Macro: I2C_ClkDisable
 * Description: Turn off I2C module
 * Status: Verified
 */
#define I2C_ClkDisable()   {\
								DISABLE_REGISTER_WRITE_PROTECTION;\
								I2C_MODULE_OFF;\
								ENABLE_REGISTER_WRITE_PROTECTION;\
							}

/*
 * Macro: I2C_Enable
 * Description:
 * 	- Enable I2C
 * 	- Initialize the PIN for I2C modality (P16 - SCL-DS) (P17 - SDA-DS)
 * Note:
 * Status: Verified
 */
#define I2C_Enable()\
{\
	_HAL_RX100_I2C_ENABLE_REG; \
	PORT1.PDR.BYTE &= 0x3F;\
	PORT1.PMR.BYTE |= 0xC0;\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE |= 0x40;\
	MPC.P17PFS.BYTE = 0x0F;\
	MPC.P16PFS.BYTE = 0x0F;\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE =  0x80;\
}/*end I2C Enable*/

/*
 * Macro: I2C_Disable
 * Description: Disable I2C Module and Release IO pins
 * Note:
 * Status: Verified
 */
#define I2C_Disable()\
{\
	_HAL_RX100_I2C_DISABLE_REG; \
	PORTB.PMR.BYTE &= 0x3F;\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE |= 0x40;\
	MPC.P17PFS.BYTE &= 0xF0;\
	MPC.P16PFS.BYTE &= 0xF0;\
	MPC.PWPR.BYTE =  0x00;\
	MPC.PWPR.BYTE =  0x80;\
}

/* Macro: I2C_CtrlReset
 * Description: The macro is used to reset the internal state of the RIIC
 * 	- ICCR1 b6
 * Status: Verified
 */
#define I2C_CtrlReset()	{ _HAL_RX100_I2C_ICCR1_REG = 0x10; _HAL_RX100_I2C_ICCR1_REG = 0x50; }

/*
 * Macro: I2C_IntRegReset
 * Description: I2C Interrupt Reset (disable all I2C interrupts)
 * Note: Also in Freescale AG128 is not done
 * Status:
 */
#define I2C_IntRegReset() {}
/*
 * Macro: I2C_Reset
 * Description:
 * Note:
 * Status: Verified
 */
#define I2C_Reset()       { _HAL_RX100_I2C_ICCR1_REG = 0xD0; _HAL_RX100_I2C_ICCR1_REG = 0x90; }

/*
 * Macro: I2C_ReadData
 * Description:
 * Status: Verified
 */
#define I2C_ReadData()    _HAL_RX100_I2C_ICDRR_REG

/*
 * Macro: I2C_WriteData
 * Description:
 * Note:
 * 	- Before send data must check the TDRE bit in ICSR2 register
 * Status: Verified
 */
#define I2C_WriteData(b)  { _HAL_RX100_I2C_ICDRT_REG = ((I2C_DataRegType)(b)); }

/*
 * Macro: I2C_SendAddr
 * Description:
 * Note:
 * Status: Verified
 */

#define I2C_SendAddr(b)  { \
		_HAL_RX100_I2C_ICDRT_REG = ((I2C_DataRegType)(b)); \
		if ((b) & 0x01) \
		{ \
			int addrDelay = 0;\
			while ( ((_HAL_RX100_I2C_ICSR2_REG & _HAL_RX100_I2C_ICSR2_RDRF_MASK) == 0) && (addrDelay<500)) \
			{ \
				addrDelay++; \
				if (!(addrDelay % 200)) \
				{ \
					_HAL_RX100_I2C_ICDRT_REG = ((I2C_DataRegType)(b)); \
				} \
			} \
		} \
	}

/*
 * Macro: I2C_pReceiveDataRegister
 * Description:
 * Status: Check meaning
 */
#define I2C_pReceiveDataRegister() ((uint8_t *)& _HAL_RX100_I2C_ICDRR_REG)

/*
 * Macro: I2C_pTrasmitDataRegister
 * Description:
 * Status: Check meaning
 */
#define I2C_pTransmitDataRegister() ((uint8_t *)& _HAL_RX100_I2C_ICDRT_REG)

/*
 * Macro: I2C_MasterEnable
 * Description: Set I2C in master mode
 * Note: Master/Slave mode is change in automatic
 * Status: Verified
 */
#define     I2C_MasterEnable()
/*
 * Macro: I2C_MasterDisable
 * Description: Disable I2C master mode, 0 meaning Slave mode.
 * Note: Master/Slave mode is change in automatic
 * Status: Verified
 */
#define     I2C_MasterDisable()
/*
 * Macro: I2C_SetClockRate(b)
 * Input: b(desired Transfer rate)
 * Status: Verified
 * Description:
 *  - PCLK is 20MHz, specific by Indesit
 *  - This Equation regulate the transer rate: Tranfer rate [kbps] = 1/(((ICBRH + 1) + (ICBRL +1))/(PCLK/PCLK_Prescaler))+500kbps(HW features see data sheet)
 * 	- ICMR1(b4-b6): Select the prescaler value that divide the PCLK
 * 	- ICMR1(b0-b2): Select 8-bit counter
 *  - ICBRH
 *  - ICBRL
 *  */
#define I2C_SetClockRate(b)\
		{\
			RIIC0.ICMR1.BYTE = ((HAL_RX100_I2C_CKS_SETTING << 4) | 0x08);\
			RIIC0.ICBRH.BYTE = 0xE0 | HAL_RX100_I2C_ICBRH_SETTING;\
			RIIC0.ICBRL.BYTE = 0xE0 | HAL_RX100_I2C_ICBRL_SETTING;\
			RIIC0.ICMR2.BIT.SDDL = HAL_RX100_I2C_SDDL_SETTING;\
			RIIC0.ICMR2.BIT.DLCS = HAL_RX100_I2C_DLCS_SETTING;\
		}/*end macro I2C_SetClockRate*/

/*
 * Macro: I2C_ResetClockRate(b)
 * Input: b(desired Transfer rate)
 * Status: Verified
 * Description:
 *  */
#define     I2C_ResetClockRate(b)    			I2C_SetClockRate(b)

/*
 * Macro: I2C_SetSlaveAddress
 * Input: b(slave address)
 * Status: Verified
 */
#define I2C_SetSlaveAddress(b)\
		{\
			_HAL_RX100_I2C_SARL0_REG = (I2C_SlaveAddressType)(b);\
			_HAL_RX_I2C_ADDR0_7BITS;\
		}/*end macro I2C_SetSlaveAddress*/
/*
 * Macro: I2C_SetTxMode
 * Description: Set the module in  Mode
 * Note: TX/RX mode is change in automatic way by peripheral
 * Status: Verified
 */
#define     I2C_SetTxMode()		{ }
/*
 * Macro: I2C_SetRxMode
 * Description: Set the modue in RX mode
 * Note: TX/RX mode is change in automatic way by peripheral
 * Status: Verified
 */
#define     I2C_SetRxMode()		{ }

/*
 * Macro: I2C_IsTxMode
 * Description:
 * Note: Includendo anche il master (Note old file)
 * Status: Verified
 */
#define     I2C_IsTxMode()	((_HAL_RX100_I2C_ICCR2_REG & _HAL_RX100_I2C_ICCR2_TRS_MASK) != 0x00)

/*
 * Macro: I2C_IssueStatCondition
 * Description:
 * Note:
 * 	- wait is to exit from loop in the case the bus is busy
 *  - must check if the bus is busy
 * Status: Verified
 */
#define     I2C_IssueStartCondition()	{ \
		int stDelay = 0;\
		_HAL_I2C_START_CONDITION; \
		while (((_HAL_RX100_I2C_ICSR2_REG & _HAL_RX100_I2C_ICSR2_TDRE_MASK) == 0) && (stDelay < 100)) {stDelay++;} \
	}

/*
 * Macro: I2C_IssueReStartCondition
 * Description:
 * Note:
 * Status: Verified
 */
#define     I2C_IssueReStartCondition() { \
		int rstDelay = 0;\
		_HAL_I2C_RESTART_CONDITION; \
		while (((_HAL_RX100_I2C_ICSR2_REG & _HAL_RX100_I2C_ICSR2_TDRE_MASK) == 0) && (rstDelay < 100)) {rstDelay++;} \
	}

/*
 * Macro: I2C_IssueStopCondition
 * Description
 * Note:
 * Status: Verified
 */
#define     I2C_IssueStopCondition()  { \
		int spDelay = 0;\
		_HAL_RX100_I2C_ICSR2_REG &= ~_HAL_RX100_I2C_ICSR2_STOP_MASK; \
		_HAL_I2C_STOP_CONDITION; \
		I2C_DummyRead(); \
		while (((_HAL_RX100_I2C_ICSR2_REG & _HAL_RX100_I2C_ICSR2_STOP_MASK) == 0) && (spDelay < 100))  {spDelay++;} \
		I2C_ResetStatus(); \
	}

/*****************************/
/* Interrupt management 	 */
/*****************************/
/*
 * Macro: I2C_IntEnable()
 * Description: Interrupt Enable
 * Status: Verified
 * Note:
 * 	- Interrupt used:
 * 		- RX: ICIER.b5,
 * 		- TX (non ricordo però se  sia dato trasmesso TEND, che dato trasferito TCF)
 * 		- match address(? Seems there is not in RX200)
 */

#define I2C_IntRxEnable()		\
ICU.IPR[I2C_RX_INT_VECTOR].BYTE = I2C_RX_INT_PRIORITY;\
ICU.IR[I2C_RX_INT_VECTOR].BYTE = 0x00;\
I2C_RX_INTERRUPT_ENABLE;\
ICU.IER[I2C_RX_INT_IER_INDEX].BYTE |= I2C_RX_IER_BIT;

#define I2C_IntTxEnable()		\
ICU.IPR[I2C_TX_INT_VECTOR].BYTE = I2C_TX_INT_PRIORITY;\
ICU.IR[I2C_TX_INT_VECTOR].BYTE = 0x00;\
I2C_TX_INTERRUPT_ENABLE;\
ICU.IER[I2C_TX_INT_IER_INDEX].BYTE |= I2C_TX_IER_BIT;


#define I2C_IntTxEndEnable()		\
ICU.IPR[I2C_TX_END_INT_VECTOR].BYTE = I2C_TX_END_INT_PRIORITY;\
ICU.IR[I2C_TX_END_INT_VECTOR].BYTE = 0x00;\
I2C_TX_END_INTERRUPT_ENABLE;\
ICU.IER[I2C_TX_END_INT_IER_INDEX].BYTE |= I2C_TX_END_IER_BIT;


#define	I2C_IntEnable()	{		\
		I2C_IntRxEnable()		\
		/*I2C_IntTxEnable()*/	\
		I2C_IntTxEndEnable()	\
		}/* I2C_IntEnable*/



/*
 * Macro: I2C_IntDisable
 * Description: Disable all I2C module interrupt
 * Note:
 * Status: Verified
 */
#define I2C_IntRxDisable()\
ICU.IER[I2C_RX_INT_IER_INDEX].BYTE &= ~I2C_RX_IER_BIT;\
I2C_RX_INTERRUPT_DISABLE;\
ICU.IPR[I2C_RX_INT_VECTOR].BYTE = 0x00;\
ICU.IR[I2C_RX_INT_VECTOR].BYTE = 0x00;

#define I2C_IntTxDisable()\
ICU.IER[I2C_TX_INT_IER_INDEX].BYTE &= ~I2C_TX_IER_BIT;\
I2C_TX_INTERRUPT_DISABLE;\
ICU.IPR[I2C_TX_INT_VECTOR].BYTE = 0x00;\
ICU.IR[I2C_TX_INT_VECTOR].BYTE = 0x00;

#define I2C_IntTxEndDisable()\
ICU.IER[I2C_TX_END_INT_IER_INDEX].BYTE &= ~I2C_TX_END_IER_BIT;\
I2C_TX_END_INTERRUPT_DISABLE;\
ICU.IPR[I2C_TX_END_INT_VECTOR].BYTE = 0x00;\
ICU.IR[I2C_TX_END_INT_VECTOR].BYTE = 0x00;

#define	I2C_IntDisable()	{	\
		I2C_IntRxDisable()		\
		I2C_IntTxDisable()		\
		I2C_IntTxEndDisable()	\
		}/* I2C_IntDisable*/


/*****************************/
/*Peripheral state management*/
/*****************************/

/*
 * Macro: I2C_IsBusBusy()
 * Status: Verified
 */
/* ICCR2 b7 if 1 I2C busy*/
#define     I2C_IsBusBusy()  ((_HAL_RX100_I2C_ICCR2_REG & _HAL_RX100_I2C_ICCR2_BBSY_MASK) != 0)
/*
 * Macro: I2C_IsSdaIoHigh
 * Description:
 * Note:
 * Status:
 */
#define     I2C_IsSdaIoHigh() ((_HAL_RX100_I2C_ICCR1_REG & 0x01) != 0)

/*
 * Macro: I2C_IsSclIoHigh
 * Description:
 * Note:
 * Status:
 */
#define     I2C_IsSclIoHigh() ((_HAL_RX100_I2C_ICCR1_REG & 0x02) != 0)

/*
 * Macro: I2C_GetStatus
 * Description:
 * Note:
 * Status:
 */
#define     I2C_GetStatus() (_HAL_RX100_I2C_STATUS_REG)

/*
 * Macro: I2CResetStatus
 * Description:
 * Note:
 * Status:
 */
#define     I2C_ResetStatus() 	{	(void) _HAL_RX100_I2C_ICSR1_REG; \
									_HAL_RX100_I2C_ICSR1_REG = 0; \
									(void) _HAL_RX100_I2C_ICSR2_REG; \
									_HAL_RX100_I2C_ICSR2_REG = 0; }

/*
 * Macro: I2C_ResetIntReqFlag
 * Description: Reset I2C interrupt flag
 * Note: RX and TX ( Check the Macro I2C_IntEnable to see what are INT enable)
 * Status: Verified
 */
#define     I2C_ResetIntReqFlag()\
			{\
				ICU.IR[I2C_RX_INT_VECTOR].BYTE = 0x00;\
				ICU.IR[I2C_TX_INT_VECTOR].BYTE = 0x00;\
				ICU.IR[I2C_TX_END_INT_VECTOR].BYTE = 0x00;\
			}/* I2C_ResetIntreqFlag*/


/*
Description: specific Wake up statements
Note: none
 */
#define		I2C_EnableWU()	/* nothing to do */

/*********************************************************************/
/* Implemented arbitration lost in case of master communication only */
/*********************************************************************/

/*
 * Macro: I2C_IsArbitrationLost
 * Description: If one the flag show that bus mastership has been lost
 * Note:
 * Status: Test me!
 */
#define	I2C_IsArbitrationLost()  ((_HAL_RX100_I2C_ICSR2_REG & _HAL_RX100_I2C_ICSR2_AL_MASK) == _HAL_RX100_I2C_ICSR2_AL_MASK)

/*
 * Macro: I2C_IsSlaveAddressed
 * Description:
 * Note: Check the status of ICSR1 b0-b2 if the slave address is detected
 * Status Test for S0!
 */
#define     I2C_IsSlaveAddressed()  (( (_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS0_MASK) == _HAL_RX100_I2C_ICSR1_AAS0_MASK) ||\
										((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS1_MASK) == _HAL_RX100_I2C_ICSR1_AAS1_MASK) ||\
										((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS2_MASK) == _HAL_RX100_I2C_ICSR1_AAS2_MASK))

/*
 * Macro: I2C_IsGeneralCallAddress
 * Description:
 * Note:
 * Status:
 */
#define     I2C_IsGeneralCallAddress() ((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_GCA_MASK) == _HAL_RX100_I2C_ICSR1_GCA_MASK)	/* ICSR1 General Call Address*/
/*
 * Macro: I2C_ResetSlaveAddressedSig
 * Description:
 * Note:
 * Status: Test for S0!
 */
#define     I2C_ResetSlaveAddressedSig() {\
				if((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS0_MASK) == _HAL_RX100_I2C_ICSR1_AAS0_MASK)\
					_HAL_RX100_I2C_ICSR1_REG &= ~_HAL_RX100_I2C_ICSR1_AAS0_MASK;\
				if((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS1_MASK) == _HAL_RX100_I2C_ICSR1_AAS1_MASK)\
					_HAL_RX100_I2C_ICSR1_REG &= ~_HAL_RX100_I2C_ICSR1_AAS1_MASK;\
				if((_HAL_RX100_I2C_ICSR1_REG & _HAL_RX100_I2C_ICSR1_AAS2_MASK) == _HAL_RX100_I2C_ICSR1_AAS2_MASK)\
					_HAL_RX100_I2C_ICSR1_REG &= ~_HAL_RX100_I2C_ICSR1_AAS2_MASK;\
			}/* I2C_ResetSlaveAddressedSig*/
/*
 * Macro: I2C_ResetGeneralCallAffressSig
 * Description:
 * Note:
 * Status:
 */
#define     I2C_ResetGeneralCallAddressSig() {_HAL_RX100_I2C_ICSR1_REG &= ~_HAL_RX100_I2C_ICSR1_GCA_MASK;}

/*
 * Macro: I2C_ResetArbitrationLost
 * Description: Clear the AL Bit after reading AL = 1
 * Note:
 * Status:
 */
#define     I2C_ResetArbitrationLost() {_HAL_RX100_I2C_ICSR2_REG &= ~_HAL_RX100_I2C_ICSR2_AL_MASK;}

/*
 * Macro: I2C_IsSlaveTrasmitRequest
 * Description: Check the Save Mode R/W* ( last bit of address)
 * Note:
 * Status: Test me!
 */
#define     I2C_IsSlaveTrasmitRequest()  ((_HAL_RX100_I2C_ICCR2_REG & (_HAL_RX100_I2C_ICCR2_MST_MASK | _HAL_RX100_I2C_ICCR2_TRS_MASK)) == _HAL_RX100_I2C_ICCR2_TRS_MASK)

/*
 * Macro: I2C_IsAcknowledgeEnabled
 * Description:
 * Note: Understand the meaning of function
 * Status:
 */
#define     I2C_IsAcknowledgeEnabled() ((_HAL_RX100_I2C_ICMR3_REG & _HAL_RX100_I2C_ICMR3_ACKBT_MASK) == 0)

/*
 * Macro: I2C_IsMaster
 * Description:
 * Note:
 * Status: Verified
 */
#define     I2C_IsMaster() ((_HAL_RX100_I2C_ICCR2_REG & _HAL_RX100_I2C_ICCR2_MST_MASK) == _HAL_RX100_I2C_ICCR2_MST_MASK)

/*
 * Macro: I2C_ResetTxSig
 * Description:
 * Note: Also in freescale is not done
 * Status:
 */
#define     I2C_ResetTxSig() {_HAL_RX100_I2C_ICSR2_REG &= ~_HAL_RX100_I2C_ICSR2_TEND_MASK; }
/*
 * Macro: I2C_ReceivedAcknowlwdgeVal
 * Description: Receive Acknowledge
 * Note:
 * Status: Verified
 */
#define     I2C_ReceivedAcknowledgeVal() ((_HAL_RX100_I2C_ICMR3_REG & _HAL_RX100_I2C_ICMR3_ACKBR_MASK) != 0)


/*
 * Macro: I2C_EnableAcknowledge
 * Description: Write 0 to ICMR3 b3. This is write protection by ICMR b4
 * Note:
 * Status: Verified
 */
#define     I2C_EnableAcknowledge()    {	_HAL_RX100_I2C_ICMR3_REG |= _HAL_RX100_I2C_ICMR3_ACKWP_MASK; \
											_HAL_RX100_I2C_ICMR3_REG &= ~_HAL_RX100_I2C_ICMR3_ACKBT_MASK; \
											_HAL_RX100_I2C_ICMR3_REG &= ~_HAL_RX100_I2C_ICMR3_ACKWP_MASK; }

/*
 * Macro: I2C_DisableAcknowledge
 * Description: Ask for information!!
 * Note: Questa funziona nel caso di Slave (old file)
 * Status: Verified
 */
#define     I2C_DisableAcknowledge()	{	_HAL_RX100_I2C_ICMR3_REG |= _HAL_RX100_I2C_ICMR3_ACKWP_MASK; \
											_HAL_RX100_I2C_ICMR3_REG |= _HAL_RX100_I2C_ICMR3_ACKBT_MASK; \
											_HAL_RX100_I2C_ICMR3_REG &= ~_HAL_RX100_I2C_ICMR3_ACKWP_MASK; }

/*
 * Macro: I2C_SlaveStartRX
 * Description:
 * Note: Also in frescale AG128 is not implemented
 * Status:
 */
#define     I2C_SlaveStartRx()	I2C_IntTxEndDisable()\
								I2C_IntTxEnable()

/*
 * Macro: I2C_SlaveStartTx
 * Description:
 * Note:
 * Status:
 */
#define     I2C_SlaveStartTx()	I2C_IntTxEndEnable()\
								I2C_IntTxDisable()
/*
 * Macro: I2C_SlaveStartTx
 * Description: unlock bus
 * Note:
 * Status:
 */
#define I2C_OnSlaveRxDummy()                 /*lint -save -e522 */   I2C_WriteData(0x55);\
                                                I2C_DummyRead();\
												I2C_SetRxMode();\
												I2C_EnableAcknowledge(); /*lint -restore */
/*
 * Macro:
 * Description:
 * Status: To check!
 */
#define     I2C_OnSlaveAddressMatchTransmit()\
			I2C_ResetSlaveAddressedSig();\
			I2C_SetTxMode();\
			I2C_DisableAcknowledge()

/*
 * Macro: I2C_onEnterIsr
 * Description:
 * Note:
 * Status: Verified
 */
#define I2C_OnEnterIsr()	I2C_ResetIntReqFlag()
/*
 * Macro: I2C_OnIntAck
 * Description:
 * Note: Also in freescale is empty
 */
#define I2C_OnIntAck()	/* empty */

/*
 * Macro: I2C_DummyRead()
 * Description: Perform a dummy Read of I2C RX Register
 * Status: Verified
 */
#define I2C_DummyRead()     		/*lint -save -e522 */	I2C_ReadData() /*lint -restore */

/*
 * Macro: I2C_SlaveAddressedTx_Action
 * Description: Also in Freescale AG128 is empty
 * Status:
 */
#define I2C_SlaveAddressedTx_Action() 	/*empty*/

/* This section contains HAL class extension */

/*
 * Macro: I2C_NestedIsr_Begin
 * Description:
 * Status:
 * Note: Also in freescale is empty
 */
#define I2C_NestedIsr_Begin()	/* empty */

/*
 * Macro: I2C_NestedIsr_End
 * Description:
 * Note: Also in frescale is empty
 */
#define I2C_NestedIsr_End()		/* empty */

/*
 * Macro: I2C_Level3BlockMonitorSync
 * Description:
 * Note:
 * Status:
 */
#define I2C_Level3BlockMonitorSync()   ( (_HAL_RX100_I2C_ICCR2_REG & _HAL_RX100_I2C_ICCR2_TRS_MASK) & ((_HAL_RX100_I2C_ICMR3_REG & 0x08) == 0x80) )

/*
 * Macro: I2C_Level4BlockMonitorSync
 * Description:
 * Note:
 * Status:
 */
#define I2C_Level4BlockMonitorSync() ( (_HAL_RX100_I2C_ICCR2_REG & 0x60) & ((_HAL_RX100_I2C_ICMR3_REG & 0x08) == 0x80) )

/*
 * Macro: I2C_Level5BlockMonitorSync
 * Description:
 * Note:
 * Status:
 */
#define I2C_Level5BlockMonitorSync()	((_HAL_RX100_I2C_ICCR2_REG & 0x60) & (_HAL_RX100_I2C_ICMR3_REG & 0x08))

/* ********************************************************************************************** */
/*                                  END I2C Class HAL Support	                                  */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN UART Class HAL Support                                   */
/* ********************************************************************************************** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Define UART1 Capability */
#define _HAL_UARTx_CAPABILITY

#ifdef	HAL_OPTION_UART
#if     HAL_OPTION_UART ==  0
#define HAL_UART_DEV   0
#elif	HAL_OPTION_UART ==  1
#define HAL_UART_DEV   1
#elif	HAL_OPTION_UART ==  5
#define HAL_UART_DEV   5
#elif   HAL_OPTION_UART ==  12
#define HAL_UART_DEV   12
#else
#error "uart not available"
#endif
#endif

/* Baud rates calculations */

#define HAL_UART_DIVISOR					HAL_OPTION_UART_BAUDRATE_DIVISOR
#define _HALUART_CALC_BAUD(baud,divisor)   (uint16_t)((uint16_t)(((uint32_t)(BUS_FREQUENCY_Hz) / ((uint32_t)32.0 * (1 << (2 * divisor)) * (baud))))) -1


#define SCI0_MODULE_ENABLE		SYSTEM.MSTPCRB.LONG &= 0x7FFFFFFF
#define SCI0_MODULE_DISABLE		SYSTEM.MSTPCRB.LONG |= 0x80000000

#define SCI1_MODULE_ENABLE		SYSTEM.MSTPCRB.LONG &= 0xBFFFFFFF
#define SCI1_MODULE_DISABLE		SYSTEM.MSTPCRB.LONG |= 0x40000000

#define SCI5_MODULE_ENABLE		SYSTEM.MSTPCRB.LONG &= 0xFBFFFFFF
#define SCI5_MODULE_DISABLE		SYSTEM.MSTPCRB.LONG |= 0x04000000

#define SCI12_MODULE_ENABLE		SYSTEM.MSTPCRB.LONG &= 0xFFFFFFEF
#define SCI12_MODULE_DISABLE		SYSTEM.MSTPCRB.LONG |= 0x00000010

/** Baud Rates */
#define _HALUART_BAUD_115200               (_HALUART_CALC_BAUD(115200UL,	))
#define _HALUART_BAUD_76800                (_HALUART_CALC_BAUD(76800UL,		))
#define _HALUART_BAUD_57600                (_HALUART_CALC_BAUD(57600UL,		))
#define _HALUART_BAUD_38400                (_HALUART_CALC_BAUD(38400UL,		))
#define _HALUART_BAUD_28800                (_HALUART_CALC_BAUD(28800U,		))
#define _HALUART_BAUD_19200                (_HALUART_CALC_BAUD(19200U,		))
#define _HALUART_BAUD_14400                (_HALUART_CALC_BAUD(14400,			))
#define _HALUART_BAUD_9600                 (_HALUART_CALC_BAUD(9600, 0))
#define _HALUART_BAUD_4800                 (_HALUART_CALC_BAUD(4800, 0))
#define _HALUART_BAUD_2400                 (_HALUART_CALC_BAUD(2400, 1))
#define _HALUART_BAUD_1200                 (_HALUART_CALC_BAUD(1200, 1))

#define _HAL_RX100_UART_ERROR_MASK		    (_HAL_RX100_UART_SSR_PER_MASK | _HAL_RX100_UART_SSR_FER_MASK | _HAL_RX100_UART_SSR_ORER_MASK)

/* RX200 Register definitions*/
/** RDR:Receive data register	**/
#define SCI0_RDR	SCI0.RDR
#define SCI1_RDR	SCI1.RDR
#define SCI5_RDR	SCI5.RDR
#define SCI12_RDR	SCI12.RDR

/** TDR:Trasmit data register	**/
#define SCI0_TDR	SCI0.TDR
#define SCI1_TDR	SCI1.TDR
#define SCI5_TDR	SCI5.TDR
#define SCI12_TDR	SCI12.TDR

/** SMR:Serial mode register	**/
#define SCI0_SMR	SCI0.SMR.BYTE
#define SCI1_SMR	SCI1.SMR.BYTE
#define SCI5_SMR	SCI5.SMR.BYTE
#define SCI12_SMR	SCI12.SMR.BYTE

/** SCR:Serial control register	**/
#define SCI0_SCR	SCI0.SCR.BYTE
#define SCI1_SCR	SCI1.SCR.BYTE
#define SCI5_SCR	SCI5.SCR.BYTE
#define SCI12_SCR	SCI12.SCR.BYTE

/** SSR:Serial status register	**/
#define SCI0_SSR	SCI0.SSR.BYTE
#define SCI1_SSR	SCI1.SSR.BYTE
#define SCI5_SSR	SCI5.SSR.BYTE
#define SCI12_SSR	SCI12.SSR.BYTE

/** BBR:Bit rate register	**/
#define SCI0_BRR	SCI0.BRR
#define SCI1_BRR	SCI1.BRR
#define SCI5_BRR	SCI5.BRR
#define SCI12_BRR	SCI12.BRR

/** SCMR:Smart Card mode register	**/
#define SCI0_SCMR	SCI0.SCMR.BYTE
#define SCI1_SCMR	SCI1.SCMR.BYTE
#define SCI5_SCMR	SCI5.SCMR.BYTE
#define SCI12_SCMR	SCI12.SCMR.BYTE

#define _HAL_RX100_UART_RDR_REG(channel) 			    (SCI##channel##_RDR)
#define _HAL_RX100_UART_TDR_REG(channel) 			    (SCI##channel##_TDR)
#define _HAL_RX100_UART_SMR_REG(channel) 			    (SCI##channel##_SMR)
#define _HAL_RX100_UART_SCR_REG(channel) 			    (SCI##channel##_SCR)
#define _HAL_RX100_UART_SSR_REG(channel) 			    (SCI##channel##_SSR)
#define _HAL_RX100_UART_BRR_REG(channel) 			    (SCI##channel##_BRR)
#define _HAL_RX100_UART_SCMR_REG(channel)               (SCI##channel##_SCMR)

/* Macro per l'attivazione/disattivazione dei pin relativi alla UART */
#define SCI0_PIN_TX_PERIPH_MASK							0x01
#define SCI0_PIN_RX_PERIPH_MASK							0x02
#define SCI0_PIN_TX_FUNCTION_MASK						0x0A
#define SCI0_PIN_RX_FUNCTION_MASK						0x0A

#define SCI1_PIN_TX_PERIPH_MASK							0x40
#define SCI1_PIN_RX_PERIPH_MASK							0x01
#define SCI1_PIN_TX_FUNCTION_MASK						0x0A
#define SCI1_PIN_RX_FUNCTION_MASK						0x0A

#define SCI5_PIN_TX_PERIPH_MASK							(1<<3)
#define SCI5_PIN_RX_PERIPH_MASK							(1<<2)
#define SCI5_PIN_TX_FUNCTION_MASK						0x0A
#define SCI5_PIN_RX_FUNCTION_MASK						0x0A

#define SCI12_PIN_TX_PERIPH_MASK						(1<<1)
#define SCI12_PIN_RX_PERIPH_MASK						(1<<2)
#define SCI12_PIN_TX_FUNCTION_MASK						0x0C
#define SCI12_PIN_RX_FUNCTION_MASK						0x0C

#define _HAL_RX100_UART_SCR_CKE_MASK					0x03
#define _HAL_RX100_UART_SCR_TEIE_MASK					0x04
#define _HAL_RX100_UART_SCR_MPIE_MASK					0x08
#define _HAL_RX100_UART_SCR_RE_MASK						0x10
#define _HAL_RX100_UART_SCR_TE_MASK						0x20
#define _HAL_RX100_UART_SCR_RIE_MASK					0x40
#define _HAL_RX100_UART_SCR_TIE_MASK					0x80

/* SS - nuove */
#define _HAL_RX100_UART_SSR_TEND_MASK                   0x04
#define _HAL_RX100_UART_SSR_PER_MASK					0x08
#define _HAL_RX100_UART_SSR_FER_MASK					0x10
#define _HAL_RX100_UART_SSR_ORER_MASK					0x20
/*#define _HAL_RX100_UART_SSR_RDRF_MASK					0x40
#define _HAL_RX100_UART_SSR_TDRE_MASK					0x80*/

#define SCI0_PIN_TX_ENABLE_AS_PERIPH					PORT2.PMR.BYTE |= SCI0_PIN_TX_PERIPH_MASK
#define SCI0_PIN_RX_ENABLE_AS_PERIPH					PORT2.PMR.BYTE |= SCI0_PIN_RX_PERIPH_MASK
#define SCI0_PIN_TX_ENABLE_AS_FUNCTION					MPC.P20PFS.BYTE |= SCI0_PIN_TX_FUNCTION_MASK
#define SCI0_PIN_RX_ENABLE_AS_FUNCTION					MPC.P21PFS.BYTE |= SCI0_PIN_RX_FUNCTION_MASK

#define SCI1_PIN_TX_ENABLE_AS_PERIPH					PORT2.PMR.BYTE |= SCI1_PIN_TX_PERIPH_MASK
#define SCI1_PIN_RX_ENABLE_AS_PERIPH					PORT3.PMR.BYTE |= SCI1_PIN_RX_PERIPH_MASK
#define SCI1_PIN_TX_ENABLE_AS_FUNCTION					MPC.P26PFS.BYTE |= SCI1_PIN_TX_FUNCTION_MASK
#define SCI1_PIN_RX_ENABLE_AS_FUNCTION					MPC.P30PFS.BYTE |= SCI1_PIN_RX_FUNCTION_MASK

#define SCI5_PIN_TX_ENABLE_AS_PERIPH					PORTC.PMR.BYTE |= SCI5_PIN_TX_PERIPH_MASK
#define SCI5_PIN_RX_ENABLE_AS_PERIPH					PORTC.PMR.BYTE |= SCI5_PIN_RX_PERIPH_MASK
#define SCI5_PIN_TX_ENABLE_AS_FUNCTION					MPC.PC3PFS.BYTE |= SCI5_PIN_TX_FUNCTION_MASK
#define SCI5_PIN_RX_ENABLE_AS_FUNCTION					MPC.PC2PFS.BYTE |= SCI5_PIN_RX_FUNCTION_MASK

#define SCI12_PIN_TX_ENABLE_AS_PERIPH					PORTE.PMR.BYTE |= SCI12_PIN_TX_PERIPH_MASK
#define SCI12_PIN_RX_ENABLE_AS_PERIPH					PORTE.PMR.BYTE |= SCI12_PIN_RX_PERIPH_MASK
#define SCI12_PIN_TX_ENABLE_AS_FUNCTION					MPC.PE1PFS.BYTE |= SCI12_PIN_TX_FUNCTION_MASK
#define SCI12_PIN_RX_ENABLE_AS_FUNCTION					MPC.PE2PFS.BYTE |= SCI12_PIN_RX_FUNCTION_MASK

#define SCI0_PIN_TX_DISABLE_AS_PERIPH					PORT2.PMR.BYTE &= ~SCI0_PIN_TX_PERIPH_MASK
#define SCI0_PIN_RX_DISABLE_AS_PERIPH					PORT2.PMR.BYTE &= ~SCI0_PIN_RX_PERIPH_MASK
#define SCI0_PIN_TX_DISABLE_AS_FUNCTION					MPC.P20PFS.BYTE &= ~SCI0_PIN_TX_FUNCTION_MASK
#define SCI0_PIN_RX_DISABLE_AS_FUNCTION					MPC.P21PFS.BYTE &= ~SCI0_PIN_RX_FUNCTION_MASK

#define SCI1_PIN_TX_DISABLE_AS_PERIPH					PORT2.PMR.BYTE &= ~SCI1_PIN_TX_PERIPH_MASK
#define SCI1_PIN_RX_DISABLE_AS_PERIPH					PORT3.PMR.BYTE &= ~SCI1_PIN_RX_PERIPH_MASK
#define SCI1_PIN_TX_DISABLE_AS_FUNCTION					MPC.P26PFS.BYTE &= ~SCI1_PIN_TX_FUNCTION_MASK
#define SCI1_PIN_RX_DISABLE_AS_FUNCTION					MPC.P30PFS.BYTE &= ~SCI1_PIN_RX_FUNCTION_MASK

#define SCI5_PIN_TX_DISABLE_AS_PERIPH					PORTC.PMR.BYTE &= ~SCI5_PIN_TX_PERIPH_MASK
#define SCI5_PIN_RX_DISABLE_AS_PERIPH					PORTC.PMR.BYTE &= ~SCI5_PIN_RX_PERIPH_MASK
#define SCI5_PIN_TX_DISABLE_AS_FUNCTION					MPC.PC3PFS.BYTE &= ~SCI5_PIN_TX_FUNCTION_MASK
#define SCI5_PIN_RX_DISABLE_AS_FUNCTION					MPC.PC2PFS.BYTE &= ~SCI5_PIN_RX_FUNCTION_MASK

#define SCI12_PIN_TX_DISABLE_AS_PERIPH					PORTE.PMR.BYTE &= ~SCI12_PIN_TX_PERIPH_MASK
#define SCI12_PIN_RX_DISABLE_AS_PERIPH					PORTE.PMR.BYTE &= ~SCI12_PIN_RX_PERIPH_MASK
#define SCI12_PIN_TX_DISABLE_AS_FUNCTION				MPC.PE1PFS.BYTE &= ~SCI12_PIN_TX_FUNCTION_MASK
#define SCI12_PIN_RX_DISABLE_AS_FUNCTION				MPC.PE2PFS.BYTE &= ~SCI12_PIN_RX_FUNCTION_MASK
/**  Peripheral management and configuration **/

/*
 * Macro: UART_ClkEnable
 * Description: Resume SCI modules from low power state
 * Note: None
 * Status:
 */
#define SCIx_MODULE_ENABLE(n)	SCI##n##_MODULE_ENABLE
#define 	UART_ClkEnable(n){\
				DISABLE_REGISTER_WRITE_PROTECTION;\
				SCIx_MODULE_ENABLE(n);\
				ENABLE_REGISTER_WRITE_PROTECTION;\
			}

/*
 * Macro: UART_ClkDisable
 * Description: Put SCI modules in low power state
 * Note: None
 * Status: Verified on SCI0
 */
#define SCIx_MODULE_DISABLE(n)	SCI##n##_MODULE_DISABLE
#define 	UART_ClkDisable(n){\
				DISABLE_REGISTER_WRITE_PROTECTION;\
				SCIx_MODULE_DISABLE(n);\
				ENABLE_REGISTER_WRITE_PROTECTION;\
			}

/*
 * Macro: UART_Enable
 * Description: set MCU pins related to a specified SCI as connected to the peripheral,
 instead of as general I/O
 * Note: None
 * Status: Verified on SCI0
 */
#define SCIx_PIN_TX_ENABLE_AS_PERIPH(n)			SCI##n##_PIN_TX_ENABLE_AS_PERIPH
#define SCIx_PIN_RX_ENABLE_AS_PERIPH(n)			SCI##n##_PIN_RX_ENABLE_AS_PERIPH
#define SCIx_PIN_TX_ENABLE_AS_FUNCTION(n)		SCI##n##_PIN_TX_ENABLE_AS_FUNCTION
#define SCIx_PIN_RX_ENABLE_AS_FUNCTION(n)		SCI##n##_PIN_RX_ENABLE_AS_FUNCTION
#define 	UART_Enable(n){\
		SCIx_PIN_TX_ENABLE_AS_PERIPH(n);\
		SCIx_PIN_RX_ENABLE_AS_PERIPH(n);\
		DISABLE_PORT_REGISTER_WRITE_PROTECTION();\
		SCIx_PIN_TX_ENABLE_AS_FUNCTION(n);\
		SCIx_PIN_RX_ENABLE_AS_FUNCTION(n);\
		ENABLE_PORT_REGISTER_WRITE_PROTECTION();\
}

/*
 * Macro: UART_Disable
 * Description: set MCU pins related to a specified SCI as as general I/O,
 instead of being connected to the peripheral.
 * Note:
 * Status:
 */
#define SCIx_PIN_TX_DISABLE_AS_PERIPH(n)			SCI##n##_PIN_TX_DISABLE_AS_PERIPH
#define SCIx_PIN_RX_DISABLE_AS_PERIPH(n)			SCI##n##_PIN_RX_DISABLE_AS_PERIPH
#define SCIx_PIN_TX_DISABLE_AS_FUNCTION(n)			SCI##n##_PIN_TX_DISABLE_AS_FUNCTION
#define SCIx_PIN_RX_DISABLE_AS_FUNCTION(n)			SCI##n##_PIN_RX_DISABLE_AS_FUNCTION

 #define 	UART_Disable(n){\
		SCIx_PIN_TX_DISABLE_AS_PERIPH(n);\
		SCIx_PIN_RX_DISABLE_AS_PERIPH(n);\
		DISABLE_PORT_REGISTER_WRITE_PROTECTION();\
		SCIx_PIN_TX_DISABLE_AS_FUNCTION(n);\
		SCIx_PIN_RX_DISABLE_AS_FUNCTION(n);\
		ENABLE_PORT_REGISTER_WRITE_PROTECTION();\
}

/*
 * Macro: UART_TxEnable
 * Description: UART Transmit enable
 * Note: None
 * Status:
 */
#define     UART_TxEnable(n){\
				_HAL_RX100_UART_SCR_REG(n) |= _HAL_RX100_UART_SCR_TE_MASK;\
}

/*
 * Macro: UART_RxEnable
 * Description: UART Receive Enable
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_RxEnable(n)			_HAL_RX100_UART_SCR_REG(n) |= _HAL_RX100_UART_SCR_RE_MASK;

/*
* Macro: UART_TXDisable
* Description: UART Transmit disable
* Note: None
* Status: Verified on SCI0
*/
#define     UART_TxDisable(n)			_HAL_RX100_UART_SCR_REG(n) &= ~_HAL_RX100_UART_SCR_TE_MASK;

/*
 * Macro: UART_RXDisable
 * Description: UART Receive disable
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_RxDisable(n)			_HAL_RX100_UART_SCR_REG(n) &= ~_HAL_RX100_UART_SCR_RE_MASK;

/*
 * Macro UART_SetParityVal
 * Description: Set Parity Value [val = 0 -> Select even parity; val = 1 -> select odd parity]
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_SetParityVal(n, val)		_HAL_RX100_UART_SMR_REG(n) |= (val<<5);

/*
 * Macro: UART_SetParityMode
 * Description: Set Parity Mode [val = 0 -> parity disabled; val = 1 -> parity enabled]
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_SetParityMode(n, val)		_HAL_RX100_UART_SMR_REG(n) |= (val<<4);

/*
 * Macro: UART_SetDataLen
 * Description: Macro set the UART data lenght 8 bits or 7 bits
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_SetDataLen(n, val)			_HAL_RX100_UART_SMR_REG(n) |= (val<<6);

/*
 * Macro: UART_SetStop
 * Description:
 * Note: Not used
 * Status:
 */
#define     UART_SetStop(n ,val)

/*
 * Macro: UART_SetMode
 * Description:
 * Note: Not used!
 * Status:
 */
#define     UART_SetMode(n, val)

/*
 * Macro: UART_SetClockDivisor
 * Description:
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_SetClockDivisor(n, val)		_HAL_RX100_UART_SMR_REG(n) |= (val & 0x03);

/*
 * Macro: UART_ReadData
 * Description:
 * Note: None
 * Status: Verified on SCI0
 */
#define UART_ReadData(n)						_HAL_RX100_UART_RDR_REG(n);

/*
 * Macro: UART_WriteData
 * Description:
 * Note: None
 * Status: Verified on SCI0
 */
#define     UART_WriteData(n, b)				_HAL_RX100_UART_TDR_REG(n) = b;

#define     UART_pReceiveDataRegister(n) 		((UART_DataRegType*)&_HAL_RX100_UART_RDR_REG(n))

#define     UART_pTransmitDataRegister(n)   	((UART_DataRegType*)&_HAL_RX100_UART_TDR_REG(n))


#define		UART_ErrorProcessBegin(n)			UART_RxIntDisable(n)\
												UART_RxDisable(n)

#define		UART_ErrorProcessEnd(n)				UART_RxEnable(n)\
												UART_RxIntEnable(n)

#define		UART_OnOerError(n)					{volatile uint8_t data;\
												data = UART_ReadData(n)}


/*
 * Macro: UART_SetBaudRate
 * Description:
 * Note: None
 * Status: Verified on SCI0
 */
#define UART_SetBaudRate(n, b)				_HAL_RX100_UART_BRR_REG(n) = b;

/* SCI0 Interrupt */
/** SCI0 TE1 Interrupt define  **/
#define SCI0_IPR_VECTOR				214
#define SCI0_TE1_INT_VECTOR 		217
#define SCI0_TE1_INT_IER_INDEX 		0x1B
#define SCI0_TE1_INT_PRIORITY 		1
#define SCI0_TE1_IER_BIT 			(1<<1)
/** SCI0 TX1 Interrupt define **/
#define SCI0_TX1_INT_VECTOR 		216
#define SCI0_TX1_INT_IER_INDEX 		0x1B
#define SCI0_TX1_INT_PRIORITY 		1
#define SCI0_TX1_IER_BIT 			(1<<0)
/** SCI0 RX1 Interrupt define **/
#define SCI0_RX1_INT_VECTOR 		215
#define SCI0_RX1_INT_IER_INDEX 		0x1A
#define SCI0_RX1_INT_PRIORITY 		1
#define SCI0_RX1_IER_BIT 			(1<<7)

/* SCI1 Interrupt */
/** SCI1 TE1 Interrupt define  **/
#define SCI1_IPR_VECTOR				218
#define SCI1_TE1_INT_VECTOR 		221
#define SCI1_TE1_INT_IER_INDEX 		0x1B
#define SCI1_TE1_INT_PRIORITY 		1
#define SCI1_TE1_IER_BIT 			(1<<5)
/** SCI1 TX1 Interrupt define **/
#define SCI1_TX1_INT_VECTOR 		220
#define SCI1_TX1_INT_IER_INDEX 		0x1B
#define SCI1_TX1_INT_PRIORITY 		1
#define SCI1_TX1_IER_BIT 			(1<<4)
/** SCI1 RX1 Interrupt define **/
#define SCI1_RX1_INT_VECTOR 		219
#define SCI1_RX1_INT_IER_INDEX 		0x1B
#define SCI1_RX1_INT_PRIORITY 		1
#define SCI1_RX1_IER_BIT 			(1<<3)

/* SCI5 Interrupt */
/** SCI5 TE1 Interrupt define  **/
#define SCI5_IPR_VECTOR				222
#define SCI5_TE1_INT_VECTOR 		225
#define SCI5_TE1_INT_IER_INDEX 		0x1C
#define SCI5_TE1_INT_PRIORITY 		1
#define SCI5_TE1_IER_BIT 			(1<<1)
/** SCI5 TX1 Interrupt define **/
#define SCI5_TX1_INT_VECTOR 		224
#define SCI5_TX1_INT_IER_INDEX 		0x1C
#define SCI5_TX1_INT_PRIORITY 		1
#define SCI5_TX1_IER_BIT 			(1<<0)
/** SCI5 RX1 Interrupt define **/
#define SCI5_RX1_INT_VECTOR 		223
#define SCI5_RX1_INT_IER_INDEX 		0x1B
#define SCI5_RX1_INT_PRIORITY 		1
#define SCI5_RX1_IER_BIT 			(1<<7)

/* SCI12 Interrupt */
/** SCI12 TE1 Interrupt define  **/
#define SCI12_IPR_VECTOR			238
#define SCI12_TE1_INT_VECTOR 		241
#define SCI12_TE1_INT_IER_INDEX 	0x1E
#define SCI12_TE1_INT_PRIORITY 		1
#define SCI12_TE1_IER_BIT 			(1<<1)
/** SCI12 TX1 Interrupt define **/
#define SCI12_TX1_INT_VECTOR 		240
#define SCI12_TX1_INT_IER_INDEX 	0x1E
#define SCI12_TX1_INT_PRIORITY 		1
#define SCI12_TX1_IER_BIT 			(1<<0)
/** SCI12 RX1 Interrupt define **/
#define SCI12_RX1_INT_VECTOR 		239
#define SCI12_RX1_INT_IER_INDEX 	0x1D
#define SCI12_RX1_INT_PRIORITY 		1
#define SCI12_RX1_IER_BIT 			(1<<7)

#define SCIx_IPR_VECTOR(n)			SCI##n##_IPR_VECTOR
#define SCIx_TE1_INT_VECTOR(n) 		SCI##n##_TE1_INT_VECTOR
#define SCIx_TE1_INT_IER_INDEX(n) 	SCI##n##_TE1_INT_IER_INDEX
#define SCIx_TE1_INT_PRIORITY(n) 	SCI##n##_TE1_INT_PRIORITY
#define SCIx_TE1_IER_BIT(n) 		SCI##n##_TE1_IER_BIT
/** SCI12 TX1 Interrupt define **/
#define SCIx_TX1_INT_VECTOR(n) 		SCI##n##_TX1_INT_VECTOR
#define SCIx_TX1_INT_IER_INDEX(n) 	SCI##n##_TX1_INT_IER_INDEX
#define SCIx_TX1_INT_PRIORITY(n) 	SCI##n##_TX1_INT_PRIORITY
#define SCIx_TX1_IER_BIT(n)			SCI##n##_TX1_IER_BIT
/** SCI12 RX1 Interrupt define **/
#define SCIx_RX1_INT_VECTOR(n) 		SCI##n##_RX1_INT_VECTOR
#define SCIx_RX1_INT_IER_INDEX(n) 	SCI##n##_RX1_INT_IER_INDEX
#define SCIx_RX1_INT_PRIORITY(n) 	SCI##n##_RX1_INT_PRIORITY
#define SCIx_RX1_IER_BIT(n) 		SCI##n##_RX1_IER_BIT

/** Interrupt & DMA	**/
/*
 * Macro: UART_TxIntEnable
 * Desription:
 * Note:
 * Status: Test me!
 */
/* SS - aggiunta e pulizia codice */
#define     UART_TxIntEnable(n)\
			{\
				ICU.IPR[SCIx_IPR_VECTOR(n)].BYTE = SCIx_TE1_INT_PRIORITY(n);\
				ICU.IR[SCIx_TE1_INT_VECTOR(n)].BYTE = 0x00;\
				_HAL_RX100_UART_SCR_REG(n) |= _HAL_RX100_UART_SCR_TEIE_MASK;\
				ICU.IER[SCIx_TE1_INT_IER_INDEX(n)].BYTE |= SCIx_TE1_IER_BIT(n);\
			}

/*
 * Macro: UART_RXIntEnable
 * Description:
 * Note:
 * Status:
 */
#define     UART_RxIntEnable(n)\
			{\
				ICU.IPR[SCIx_IPR_VECTOR(n)].BYTE = SCIx_RX1_INT_PRIORITY(n);\
				ICU.IR[SCIx_RX1_INT_VECTOR(n)].BYTE = 0x00;\
				_HAL_RX100_UART_SCR_REG(n) |= _HAL_RX100_UART_SCR_RIE_MASK;\
				ICU.IER[SCIx_RX1_INT_IER_INDEX(n)].BYTE |= SCIx_RX1_IER_BIT(n);\
			}

/*
 * Macro: UART_TxIntDisable
 * Description:
 * Note:
 * Status:
 */
#define     UART_TxIntDisable(n){\
				ICU.IER[SCIx_TE1_INT_IER_INDEX(n)].BYTE &= ~SCIx_TE1_IER_BIT(n);\
				_HAL_RX100_UART_SCR_REG(n) &= ~_HAL_RX100_UART_SCR_TEIE_MASK;\
				ICU.IR[SCIx_TE1_INT_VECTOR(n)].BYTE = 0x00;\
			}

/*
 * Macro: UART_RXIntDisable
 * Description:
 * Note:
 * Status:
 */
#define     UART_RxIntDisable(n){\
				ICU.IER[SCIx_RX1_INT_IER_INDEX(n)].BYTE &= ~SCIx_RX1_IER_BIT(n);\
				_HAL_RX100_UART_SCR_REG(n) &= ~_HAL_RX100_UART_SCR_RIE_MASK;\
				ICU.IR[SCIx_RX1_INT_VECTOR(n)].BYTE = 0x00;\
			}

/*
 * Macro: UART_TxDMAEnable
 * Description:
 * Note:
 * Status:
 *
 */
#define     UART_TxDMAEnable(n)             /*{ _HAL_MCF51_UART_C5_REG(n) |= _HAL_MCF51_UART_C5_TDMAS_MASK; }*/
/*
 * Macro: UART_RxDMAEnable
 * Description:
 * Note:
 * Status:
 */
#define     UART_RxDMAEnable(n)             /*{ _HAL_MCF51_UART_C5_REG(n) |= _HAL_MCF51_UART_C5_RDMAS_MASK; }*/
/*
 * Macro: UART_TxDMADisable
 * Description:
 * Note:
 * Status:
 */
#define     UART_TxDMADisable(n)            /*{ _HAL_MCF51_UART_C5_REG(n) &= ~_HAL_MCF51_UART_C5_TDMAS_MASK; }*/
/*
 * Macro: UART_RxDMADisable
 * Description:
 * Note:
 * Status:
 */
#define     UART_RxDMADisable(n)            /*{ _HAL_MCF51_UART_C5_REG(n) &= ~_HAL_MCF51_UART_C5_RDMAS_MASK; }*/

/**  State	**/

#define     UART_IsPeError(n)        	    (_HAL_RX100_UART_SSR_REG(n) & _HAL_RX100_UART_SSR_PER_MASK)
#define     UART_IsFerError(n)       	    (_HAL_RX100_UART_SSR_REG(n) & _HAL_RX100_UART_SSR_FER_MASK)
#define     UART_IsOerError(n)       	    (_HAL_RX100_UART_SSR_REG(n) & _HAL_RX100_UART_SSR_ORER_MASK)
#define     UART_IsError(n)				    (_HAL_RX100_UART_SSR_REG(n) & _HAL_RX100_UART_ERROR_MASK)

#define     UART_ClearError(n)       	    {\
												UART_ErrorProcessBegin(n)\
												if(UART_IsOerError(n))\
												{\
													UART_OnOerError(n)\
												}\
												_HAL_RX100_UART_SSR_REG(n) &= ~_HAL_RX100_UART_ERROR_MASK;\
												UART_ErrorProcessEnd(n)\
											}

#define 	UART_ResetPeError(n)		    {_HAL_RX100_UART_SSR_REG(n) &= ~_HAL_RX100_UART_SSR_PER_MASK;}
#define     UART_ClearRxIrq(n)       	    /*!< not needed : register reading does it*/
#define     UART_ClearTxIrq(n)       	    /*!< not needed : regitser reading does it*/
#define     UART_IsTxInt(n)           	    (ICU.IR[SCIx_TE1_INT_VECTOR(n)].BYTE != 0x00)
#define     UART_IsTxEndInt(n)			    ((_HAL_RX100_UART_SSR_REG(n) & _HAL_RX100_UART_SSR_TEND_MASK) != 0)
#define     UART_IsRxInt(n)           	    (1) /* it should test ( ICU.IR[SCIx_RX1_INT_VECTOR(n)].BYTE != 0x00) */

#define 	UART_IsTxDataRegFull(n)   	    ((_HAL_RX100_UART_SSR_REG(n) & (_HAL_RX100_UART_SSR_TEND_MASK)) == 0)


int UART_TxByte(int dev, char * chPtr );
int UART_RxByte(int dev, char * chPtr );

/* ********************************************************************************************** */
/*                                  END UART Class HAL Support                                   */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN WDT Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Watchdog
 * Note: In RX200 the peripheral's name is Indipendent Watchdog module
 */
/*@{*/

/* Define WatchDog Capability */
#define _HAL_WDT_CAPABILITY
/* Preprocessor Errors */
/*#if (WDT_TIMEOUT > 4294967295)
 #error "WDT_TIMEOUT must be less than 2^32"
 #endif
 #if ((WDT_WINDOWED_MODE == 1) & (WDT_TIMEOUT <= (WDT_WINDOW+1)))
 #error "WDT_WINDOW must be less than than (WDT_TIMEOUT-1)!"
 #endif
 #if (WDT_TIMEOUT < 4)
 #error "The time-out value of the watchdog must be set to a minimum of four watchdog clock cycles."
 #endif*/

/* Start Window values*/
#define START_WINDOW_25		0x0000
#define START_WINDOW_50		0x1000
#define START_WINDOW_75		0x2000
#define START_WINDOW_100	0x3000
/* End Window values*/
#define END_WINDOW_75		0x0000
#define END_WINDOW_50		0x0100
#define END_WINDOW_25		0x0200
#define END_WINDOW_0		0x0300
/*
 * Whatcdog timeout is express in ms.
 * Refers to the table 27.3 of User Manual to set the TIMEOUT
 * The follow parameters should be used:
 * 	- Divider: 4,64,128,512,2048,8192,
 * 	- Timeout: 0x00 (1024 cycles), 0x01 (4096 cycles), 0x20 (8192 cycles), 0x03 (16384 cycles)
 */
#define RX110_WDT_TIMEOUT_1024 		0x00
#define RX110_WDT_TIMEOUT_4096 		0x01
#define RX110_WDT_TIMEOUT_8192 		0x02
#define RX110_WDT_TIMEOUT_16384 	0x03

/** WDT PRESCALER **/
/*
 * The prescaler divided the PCKL
 */
#define RX110_WDT_DIVIDER_1 		0x00
#define RX110_WDT_DIVIDER_16 		0x20
#define RX110_WDT_DIVIDER_32 		0x30
#define RX110_WDT_DIVIDER_64 		0x40
#define RX110_WDT_DIVIDER_128 		0xF0
#define RX110_WDT_DIVIDER_256		0x50

#define IWDT_REG_MODIFY_ENABLE	1
#define IWDT_REG_MODIFY_DISABLE	0

void   WDT_InitSync(void);
bool_t WDT_Enabled(void);
void   WDT_Refresh(void);
bool_t WDT_TimeoutExpired(void);
bool_t WDT_UntimelyResetOccurred(void);
void WDT_ClearTimeoutExpired(void);
void WDT_ClearUntimelyResetOccurred(void);

bool_t WDT_AllowUpdate(void);
void   WDT_ResetMCU(void);

void   WDT_StartSync(void);
void   WDT_SetWindowingSync(WDT_windowType window);
uint32_t WDT_GetWindow(void);
uint8_t WDT_IsWindowOpen(void);
uint8_t WDT_IsRunning(void);
uint32_t WDT_ReadCounter(void);
uint16_t WDT_ReadRSTCounter(void);
bool_t WDT_IsJustBeforePermittedWindow(void);
void WDT_ResetRSTCounter(void);

/* NOT Implemented ------------------------------*/
#define WDT_SetSync(x) 		/* not allowed */
#define WDT_StopSync()		/* not allowed */
#define WDT_RSTCounter()        0	/* not allowed */
#define WDT_ClearRSTCounter()	/* not allowed */
/*-----------------------------------------------*/

/*@}*/

/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. A/D Converter
 *
 */
/*@{*/

#if defined(HAL_ADC_SUPPORT)
/* ****************** */
/* Defines and Macros */
/* ****************** */

#define ADC_PIN(x,y,z) {\
(DIO_PortAddressBasicType)&PORT##x##_PDR,\
(DIO_ModuleMapMaskType)PORTX_BIT##y##_MASK,\
z\
}

#define ADC_INT_ENABLED 			1
#define ADC_INT_DISABLED 			0

/* Max clock in Hz */
#define ADC_MAX_CLOCK               8000000

/* Max clock in Hz when low power mode */
#define ADC_MAX_CLOCK_LOW_POWER     4000000

/* Max clock in Hz when low power mode */
#define ADC_MIN_CLOCK               400000

/* This defines if the Vrefh-Vrefl reference is used. Otherwise the Vdda-Vssa is used */
#define ADC_REFERENCE_VREFH_VREFL   1


#define ADC_BUS_CLOCK_DIV_2			1
#define ADC_DIV 					((BUS_FREQUENCY_Hz / 2 / ADC_CONV_FREQUENCY_Hz) - 1)


#define ADC_RESOLUTION_MODE         12

#define ADC_SINGLE_SCAN_MODE 	0x0000
#define ADC_GROUP_SCAN_MODE 	0x2000
#define ADC_CONTINUE_SCAN_MODE	0x4000

/* ADC RX110 PIN CONFIGURATION AS ANALOG */

#if defined(RX110_NOMODULE_CUSTOM1) || defined(RX110_NOMODULE_CUSTOM2)
/* Pin P40 as Analog Input AN000*/
#define ADC_CH0_PIN_AS_PERIPH	 PORT4.PMR.BIT.B0 = 0
#define ADC_CH0_PIN_AS_IO		{PORT4.PMR.BIT.B0 = 0; MPC.P40PFS.BIT.ASEL = 0;}
#define ADC_CH0_PIN_AS_ANALOG	MPC.P40PFS.BYTE	|= 0x80

/* Pin P41 as Analog Input AN001*/
#define ADC_CH1_PIN_AS_PERIPH	 PORT4.PMR.BIT.B1 = 1
#define ADC_CH1_PIN_AS_IO		{PORT4.PMR.BIT.B1 = 0; MPC.P41PFS.BIT.ASEL = 0;}
#define ADC_CH1_PIN_AS_ANALOG	MPC.P41PFS.BIT.ASEL = 1;

/* Pin P42 as Analog Input AN002*/
#define ADC_CH2_PIN_AS_PERIPH	 PORT4.PMR.BIT.B2 = 1
#define ADC_CH2_PIN_AS_IO		{PORT4.PMR.BIT.B2 = 0; MPC.P42PFS.BIT.ASEL = 0;}
#define ADC_CH2_PIN_AS_ANALOG	MPC.P42PFS.BIT.ASEL = 1;


/* Pin P43 as Analog Input AN003*/
#define ADC_CH3_PIN_AS_PERIPH	 PORT4.PMR.BIT.B3 = 1
#define ADC_CH3_PIN_AS_IO		{PORT4.PMR.BIT.B3 = 0; MPC.P43PFS.BIT.ASEL = 0;}
#define ADC_CH3_PIN_AS_ANALOG	MPC.P43PFS.BIT.ASEL = 1;

/* Pin P44 as Analog Input AN004*/
#define ADC_CH4_PIN_AS_PERIPH	 PORT4.PMR.BIT.B4 = 1
#define ADC_CH4_PIN_AS_IO		{PORT4.PMR.BIT.B4 = 0; MPC.P44PFS.BIT.ASEL = 0;}
#define ADC_CH4_PIN_AS_ANALOG	MPC.P44PFS.BIT.ASEL = 1;



/* Pin P46 as Analog Input AN006*/
#define ADC_CH6_PIN_AS_PERIPH	 PORT4.PMR.BIT.B6 = 1
#define ADC_CH6_PIN_AS_IO		{PORT4.PMR.BIT.B6 = 0; 	MPC.P46PFS.BIT.ASEL = 0;}
#define ADC_CH6_PIN_AS_ANALOG	MPC.P46PFS.BIT.ASEL = 1;

/* Pin PE7 as Analog Input AN015*/
#define ADC_CH15_PIN_AS_PERIPH	 PORTE.PMR.BIT.B7 = 1
#define ADC_CH15_PIN_AS_IO		{PORTE.PMR.BIT.B7 = 0; 	MPC.PE7PFS.BIT.ASEL = 0;}
#define ADC_CH15_PIN_AS_ANALOG	MPC.PE7PFS.BIT.ASEL = 1;

/* Pin PE0 as Analog Input AN008*/
#define ADC_CH8_PIN_AS_PERIPH	 PORTE.PMR.BIT.B0 = 1
#define ADC_CH8_PIN_AS_IO		{PORTE.PMR.BIT.B0 = 0; 	MPC.PE0PFS.BIT.ASEL = 0;}
#define ADC_CH8_PIN_AS_ANALOG	MPC.PE0PFS.BIT.ASEL = 1;

/* Pin PE1 as Analog Input AN009*/
#define ADC_CH9_PIN_AS_PERIPH	 PORTE.PMR.BIT.B1 = 1
#define ADC_CH9_PIN_AS_IO		{PORTE.PMR.BIT.B1 = 0; 	MPC.PE1PFS.BIT.ASEL = 0;}
#define ADC_CH9_PIN_AS_ANALOG	MPC.PE1PFS.BIT.ASEL = 1;

/* Pin PE2 as Analog Input AN010*/
#define ADC_CH10_PIN_AS_PERIPH	 PORTE.PMR.BIT.B2 = 1
#define ADC_CH10_PIN_AS_IO		{PORTE.PMR.BIT.B2 = 0; 	MPC.PE2PFS.BIT.ASEL = 0;}
#define ADC_CH10_PIN_AS_ANALOG	MPC.PE2PFS.BIT.ASEL = 1;

/* Pin PE3 as Analog Input AN011*/
#define ADC_CH11_PIN_AS_PERIPH	 PORTE.PMR.BIT.B3 = 1
#define ADC_CH11_PIN_AS_IO		{PORTE.PMR.BIT.B3 = 0; 	MPC.PE3PFS.BIT.ASEL = 0;}
#define ADC_CH11_PIN_AS_ANALOG	MPC.PE3PFS.BIT.ASEL = 1;

/* Pin PE4 as Analog Input AN012*/
#define ADC_CH12_PIN_AS_PERIPH	 PORTE.PMR.BIT.B4 = 1
#define ADC_CH12_PIN_AS_IO		{PORTE.PMR.BIT.B4 = 0; 	MPC.PE4PFS.BIT.ASEL = 0;}
#define ADC_CH12_PIN_AS_ANALOG	MPC.PE4PFS.BIT.ASEL = 1;

/* Pin PE5 as Analog Input AN013*/
#define ADC_CH13_PIN_AS_PERIPH	 PORTE.PMR.BIT.B5 = 1
#define ADC_CH13_PIN_AS_IO		{PORTE.PMR.BIT.B5 = 0; 	MPC.PE5PFS.BIT.ASEL = 0;}
#define ADC_CH13_PIN_AS_ANALOG	MPC.PE5PFS.BIT.ASEL = 1;

/* Pin PE6 as Analog Input AN014*/
#define ADC_CH14_PIN_AS_PERIPH	 PORTE.PMR.BIT.B6 = 1
#define ADC_CH14_PIN_AS_IO		{PORTE.PMR.BIT.B6 = 0; 	MPC.PE6PFS.BIT.ASEL = 0;}
#define ADC_CH14_PIN_AS_ANALOG	MPC.PE6PFS.BIT.ASEL = 1;

#endif
#define ADC_CHx_ENABLE_AS_IO(n)				ADC_CH##n##_PIN_AS_IO
#define ADC_CHx_ENABLE_AS_PERIPH(n)			ADC_CH##n##_PIN_AS_PERIPH
#define ADC_CHx_PIN_ENABLE_AS_ANALOG(n)		ADC_CH##n##_PIN_AS_ANALOG
/* RX210 Registers */
/* A/D Data Register*/
#define RX_100_ADC_ADDR0_REG	S12AD.ADDR0
#define RX_100_ADC_ADDR1_REG	S12AD.ADDR1
#define RX_100_ADC_ADDR2_REG	S12AD.ADDR2
#define RX_100_ADC_ADDR3_REG	S12AD.ADDR3
#define RX_100_ADC_ADDR4_REG	S12AD.ADDR4
#define RX_100_ADC_ADDR5_REG	S12AD.ADDR5
#define RX_100_ADC_ADDR6_REG	S12AD.ADDR6
#define RX_100_ADC_ADDR7_REG	S12AD.ADDR7
#define RX_100_ADC_ADDR8_REG	S12AD.ADDR8
#define RX_100_ADC_ADDR9_REG	S12AD.ADDR9
#define RX_100_ADC_ADDR10_REG	S12AD.ADDR10
#define RX_100_ADC_ADDR11_REG	S12AD.ADDR11
#define RX_100_ADC_ADDR12_REG	S12AD.ADDR12
#define RX_100_ADC_ADDR13_REG	S12AD.ADDR13
#define RX_100_ADC_ADDR14_REG	S12AD.ADDR14
#define RX_100_ADC_ADDR15_REG	S12AD.ADDR15
#define _HAL_RX100_ADC_ADDR_REG(channel)	(RX_100_ADC_ADDR##channel##_REG)
/* A/D Data Duplication Register */
#define _HAL_RX100_ADC_ADDBLDR_REG	S12AD.ADDBLDR
/* A/D Temperature Sensor Data register */
#define _HAL_RX100_ADC_ADTSDR_REG	S12AD.ADTSDR
/** A/D Internal Reference Voltage Data Register */
#define _HAL_RX100_ADC_ADOCDR_REG	S12AD.ADOCDR
/** A/D Self Diagnosis Data Register */
#define  _HAL_RX100_ADC_ADRD_REG	S12AD.ADRD.WORD
/** A/D Control Register */
#define _HAL_RX100_ADC_ADCSR_REG	S12AD.ADCSR.WORD
/** A/D Channel Select Register */
#define _HAL_RX100_ADC_ADANSA_REG	S12AD.ADANSA.WORD
/** A/D Channel Select Register B */
#define _HAL_RX100_ADC_ADANSB_REG	S12AD.ADANSB.WORD
/** A/D-Converted Value Addition Mode Select Register*/
#define _HAL_RX100_ADC_ADADS_REG	S12AD.ADADS.WORD
/** A/D-Converted Value Addition Count Select Register */
#define _HAL_RX100_ADC_ADADC_REG	S12AD.ADADC.BYTE
/** A/D Control Extended Register */
#define _HAL_RX100_ADC_ADCER_REG	S12AD.ADCER.WORD
/** A/D Start Trigger Select Register */
#define _HAL_RX100_ADC_ADSTRGR_REG	S12AD.ADSTRGR.WORD
/** A/D Conversion Extended Input Control Register */
#define _HAL_RX100_ADC_ADEXICR_REG	S12AD.ADEXICR.WORD
/** A/D Sampling State Register n */
#define RX_100_ADC_ADDSSTR0_REG	S12AD.ADSSTRO
#define RX_100_ADC_ADDSSTR1_REG	S12AD.ADSSTR1
#define RX_100_ADC_ADDSSTR2_REG	S12AD.ADSSTR2
#define RX_100_ADC_ADDSSTR3_REG	S12AD.ADSSTR3
#define RX_100_ADC_ADDSSTR4_REG	S12AD.ADSSTR4
#define RX_100_ADC_ADDSSTR5_REG	S12AD.ADSSTR5
#define RX_100_ADC_ADDSSTR6_REG	S12AD.ADSSTR6
#define RX_100_ADC_ADDSSTR7_REG	S12AD.ADSSTR7
#define _HAL_RX100_ADC_ADDSSTR_REG(channel)	(RX_100_ADC_ADDSSTR##channel##_REG)	/* n=1-7*/
#define _HAL_RX100_ADC_ADDSSTRL_REG	S12AD.ADSSTRL.
#define _HAL_RX100_ADC_ADDSSTRT_REG	S12AD.ADSSTRT
#define _HAL_RX100_ADC_ADDSSTRO_REG	S12AD.ADSSTRO
/** A/D Sample and Hold Circuit Control Register */
#define _HAL_RX100_ADC_ADSHCR_REG	S12AD.ADSHCR.WORD
/** A/D Disconnecting Detection Control Register */
#define _HAL_RX100_ADC_ADDISCR_REG	S12AD.ADDISCR.BYTE

/* Support functions */
/*
 * Macro: ADC_ClockGateOn
 * Description: Power On the ADC Module
 * Status:
 * Note:
 */
#define ADC_ClockGateOn(){\
		SYSTEM.PRCR.WORD =  0xA50B;\
		SYSTEM.MSTPCRA.LONG &= 0xFFFDFFFF;\
		SYSTEM.PRCR.WORD =  0xA500;\
}/* end ADC_ClockGateOn*/

/*
 * Macro: ADC_DisconnectingDetection
 * Description: Enable\Disable the Disconnection Detection Assist Function
 * Status:
 * Note: mode (0: discharge; 1: precharge) - time (from 0x00 to 0x0F)
 */
/*RX200 #define ADC_DisconnectingDetection(mode,time) {S12AD.ADDISCR.BYTE = (((uint8_t)mode << 4) | (uint8_t)time);} */
#define ADC_DisconnectingDetection(mode,time) {}



/*
 * Macro: ADC_ClockGateOff
 * Description:Put in Power Save mode the ADC Module
 * Status:
 * Note:
 */
#define ADC_ClockGateOff(){\
		SYSTEM.PRCR.WORD =  0xA50B;\
		SYSTEM.MSTPCRA.LONG |= 0x00020000;\
		SYSTEM.PRCR.WORD =  0xA500;\
}/*end ADC_ClockGateOff*/

/*
 * Macro: ADC_IsConversionCompleteA
 * Description:
 * Status:
 * Note:
 */
#define ADC_IsConversionCompleteA()		       ((_HAL_RX100_ADC_ADCSR_REG & 0x8000))
/*
 * Macro: ADC_IsConversionCompleteB
 * Description:
 * Status:
 * Note:
 */
#define ADC_IsConversionCompleteB()		        /*((_HAL_MCF51_ADC_ADCSC1B_REG & _HAL_MCF51_ADC_ADCSC1_COCO_MASK) != 0)*/

/*
 * Macro: ADC_EnableInterruptA
 * Description:
 * Status:
 * Note: Tested!
 */
#define S12AD_INT_VECTOR		102
#define S12AD_ENABLE_VECTOR		0x0C
#define S12AD_INT_PRIORITY 		0x01

#define ADC_EnableInterruptA(){\
	ICU.IPR[S12AD_INT_VECTOR].BYTE = S12AD_INT_PRIORITY;\
	ICU.IR[S12AD_INT_VECTOR].BYTE = 0x00;\
	_HAL_RX100_ADC_ADCSR_REG |= 0x1000;\
	ICU.IER[S12AD_ENABLE_VECTOR].BYTE |= (1<<6);\
}

/*
 * Macro: ADC_DisableInterruptA
 * Description:
 * Status:
 * Note:
 */
#define ADC_DisableInterruptA()  		        {\
	ICU.IER[S12AD_ENABLE_VECTOR].BYTE &= ~(1<<6);\
	_HAL_RX100_ADC_ADCSR_REG &= 0xEFFF;\
	ICU.IPR[S12AD_INT_VECTOR].BYTE = 0x00;\
	ICU.IR[S12AD_INT_VECTOR].BYTE = 0x00;\
}


/*
 * Macro: ADC_EnableIntrruptB
 * Description:
 * Status:
 * Note:
 */
#define ADC_EnableInterruptB()  		        {_HAL_RX100_ADC_ADCSR_REG |= 0x0040;}

/*
 * Macro: ADC_DisableInterruptB
 * Description:
 * Status:
 * Note:
 */
#define ADC_DisableInterruptB()  		        {_HAL_RX100_ADC_ADCSR_REG &= 0xFFBF;}

/*
 * Macro: ADC_ContinuesConversionA
 * Description:
 * Status:
 * Note
 */
#define ADC_ContinuesConversionA()				{_HAL_RX100_ADC_ADCSR_REG |= 0x4000; _HAL_RX100_ADC_ADCSR_REG &= 0xDFFF;}

/*
 * Macro: ADC_SingleConversionA
 * Description:
 * Status:
 * Note:
 */
#define ADC_SingleConversionA()  		        {_HAL_RX100_ADC_ADCSR_REG &= 0x9FFF;}

/*
 * Macro: ADC_ContinuesConversionB()
 * Description:
 * Status:
 * Note: RX210 only Single Conversion in Group-mode
 */
#define ADC_ContinuesConversionB()

/*
 * Macro: ADC_SingleConversionB()
 * Description:
 * Status:
 * Note: RX210 only Single Conversion in Group-mode
 */
#define ADC_SingleConversionB()

/*
 * Macro: ADC_SelectChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_SelectChannelA(channel)  		    {(_HAL_RX100_ADC_ADANSA_REG = (1<<channel));}

/*
 * Macro: ADC_AddChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_AddChannelA(channel)  		    	{(_HAL_RX100_ADC_ADANSA_REG |= (1<<channel));}

/*
 * Macro: ADC_RemoveChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_RemoveChannelA(channel)  			{(_HAL_RX100_ADC_ADANSA_REG &= ~(1<<channel));}


/*
 * Macro: ADC_SelectChannelB
 * Description:
 * Status:
 * Note:
 */
#define ADC_SelectChannelB(channel)  		    {(_HAL_RX100_ADC_ADANSB_REG = (1<<channel));}

/*
 * Macro: ADC_AddChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_AddChannelB(channel)  		    	{(_HAL_RX100_ADC_ADANSB_REG |= (1<<channel));}

/*
 * Macro: ADC_RemoveChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_RemoveChannelB(channel)  			{(_HAL_RX100_ADC_ADANSB_REG &= ~(1<<channel));}

/*
 * Macro: ADC_ReadChannelA
 * Description:
 * Status:
 * Note:
 */
#define ADC_ReadChannelA()  		            _HAL_RX100_ADC_ADANSA_REG

/*
 * Macro: ADC_ReadChannelB
 * Description:
 * Status:
 * Note:
 */
#define ADC_ReadChannelB()  		            _HAL_RX100_ADC_ADANSB_REG

/*
 *  Macro: ADC_DisableChannelA
 *  Description:
 *  Status:
 *  Note:
 */
#define ADC_DisableChannelA()					{(_HAL_RX100_ADC_ADANSA_REG = 0x0000);}

/*
 *  Macro: ADC_DisableChannelB
 *  Description:
 *  Status:
 *  Note:
 */
#define ADC_DisableChannelB()					{(_HAL_RX100_ADC_ADANSB_REG = 0x0000);}

/*
 * ADC_IsConversionActive
 * Description:
 * Status:
 * Note:
 */
#define ADC_IsConversionActive()  		       (_HAL_RX100_ADC_ADCSR_REG & 0x8000)

/*
 * Macro: ADC_StartNow
 * Description: Software start for ADC Channle Conversion
 * Note:
 * Status:
 */
#define ADC_StartNow()							{_HAL_RX100_ADC_ADCSR_REG |= 0x8000;}

/*
 * Macro: ADC_SetHWTrigger
 * Description:
 * Note:
 * Status: Test me
 */
#define ADC_SetHWTrigger()        		        {_HAL_RX100_ADC_ADCSR_REG |= 0x0300;_HAL_RX100_ADC_ADSTRGR_REG &=F0FF}
/*
 * Macro: ADC_SetSWTrigger
 * Description:
 * Note:
 * Status:
 */
#define ADC_SetSWTrigger()        		        {_HAL_RX100_ADC_ADCSR_REG &= 0xFCFF;} /* TODO: prima era così: _HAL_RX100_ADC_ADCSR_REG |= 0x0200;*/

/*
 * Macro: ADC_SetBackToBackConversion
 * Description:
 * Note:
 * Status: Not implemented in RX210
 */
/*#define ADC_SetBackToBackConversion()*/           /*!< Not implemented in RX210*/

/*
 * Macro: ADC_SetIndependentConversion()
 * Description:
 * Note:
 * Status: TODO
 */
/* #define ADC_SetIndependentConversion() */

/*
 * Macro: ADC_EnableDMA
 * Description:
 * Note:
 * Status:TODO
 */
#define ADC_EnableDMA()

/*
 * Macro: ADC_DisableDMA
 * Description:
 * Note:
 * Status: TODO
 */
#define ADC_DisableDMA()

/*
 * Macro: ADC_SetVRefToVrefhVrefl
 * Description:
 * Note:
 * Status: Not Implemented in RX210
 */
#define ADC_SetVRefToVrefhVrefl()               /* Not implemented in RX210*/

/*
 * Macro: ADC_SetVRefToVddaVssa
 * Description:
 * Note:
 * Status: Not Implemented in RX210
 */
/* #define ADC_SetVRefToVddaVssa()                 Not implemented in RX210 */

/*
 * Macro: ADC_SetVRefToOnChipBandgap
 * Description:
 * Note:
 * Status: Not Implemented in RX210
 */
/*#define ADC_SetVRefToOnChipBandgap()             Not implemented in RX210*/

/*
 * Macro: ADC_SetLowPowerConversion
 * Description:
 * Note:
 * Status: Not Implemented in RX210
 */
/* #define ADC_SetLowPowerConversion()            Not implemented in RX210 */

/*
 * Macro: ADC_SetHighSpeedConversion
 * Description:
 * Note:
 * Status: Not Implemented in RX210
 */
/* #define ADC_SetHighSpeedConversion()        */

/*
 * Macro: ADC_SelectClockDiv
 * Description:
 * Note: The clock DIV is select by the PCKD bits in SCKCR Register in phase of clock configuration
 * Status: Not implemented in RX210
 */
/* #define ADC_SelectClockDiv(div) NA */

/*
 * Macro: ADC_SetLongSampleTime
 * Description:
 * Note:
 * Status: Not implemented in RX210
 */
/* #define ADC_SetLongSampleTime()   */

/*
 * Macro: ADC_SetShortSampleTime
 * Description:
 * Note:
 * Status: Not implemented in RX210
 */
/* #define ADC_SetShortSampleTime()                {} */

/*
 * Macro: ADC_SetConversion8b
 * Description:
 * Note:
 * Status: Not implemented in RX210
 */
/* #define ADC_SetConversion8bit()			*/		/* !< Not implemented in RX210, the minimum resolution is 12 bit */

/*
 * Macro: ADC_SetConversion10b
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetConversion10bit()          */      /* < Not implemented in RX210, the minimum resolution is 12 bit TODO: remove? */

/*
 * Macro: ADC_SetConversion12b
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetConversion12bit()       */         /* !< Not implemented in RX210, it isn't selectable TODO: remove? */

/*
 * Macro: ADC_SetBusClock
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetBusClock() */

/*
 * Macro: ADC_SetBusClockDiv2
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetBusClockDiv2()  */

/*
 * Macro: ADC_SetAlternateClock
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetAlternateClock() */

/*
 * Macro: ADC_AsynchronousClock
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_SetAsynchronousClock() */

/*
 * Macro: ADC_ReadResultA
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_ReadResultA() */

/*
 * Macro: ADC_ReadResultB
 * Description:
 * Note:
 * Status: Not supported in RX210
 */
/* #define ADC_ReadResultB() */

/* added channel parameter for compatibility with others uc S.U. 31/05/2010 */
#define ADC_pResultRegister(channel) 	1	/* TODO: */
#define ADC_pChannelRegister(channel)	1	/* TODO: */
#define ADC_EmulateContinuousConversion()	/* TODO*/

/*
 * Function: ADC_ConfigurePinAsChannel
 * Description:
 * Note:
 * Status:
 */
void ADC_ConfigurePinAsChannel(IO_ChannelType channel);

/*
 * Function: ADC_DeConfigurePinAsChannel
 * Description: Configure Analog Input as Generic I/O
 * Note:
 * Status:
 */
void ADC_DeConfigurePinAsChannel(IO_ChannelType channel);

/*
 * Macro: ADC_CheckChannel
 * Description: '0' if chennel is valide; otherwise '1'
 * Note:
 * Status:
 */
#define ADC_CheckChannel(channel)               (channel >= 0x1f)

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
/*lint -emacro(774,ADC_StartConvertionOnChannel) */
void ADC_StartConvertionOnChannel(IO_ChannelType channel, uint8_t atdInterrupt);

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
void ADC_StartMultiSampleConvertionOnChannel(uint8_t channel);

/**
 \n Writes to ADC registers:
 \n\li enables interrupt from conversion on A
 \n\li sets independent conversion
 \n\li sets single converison
 \n\li interrupts any running conversion
 \n\li disables hw trigger
 \n\li disables dma

 */
void ADC_StartMultiSampleConvertionMultiChannel( void );

/**
 \n Converts the channel number to register value:
 \n\li Ores the channel number with the IE mask in order the interrupt to be still enabled
 */
#define ADC_CorrectChannel(channel)     (0x01<<channel)
#endif /* (ADC_USE_SW_DMA | ADC_USE_HW_DMA) */

typedef struct ADC_Map_tag
{
    DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register related to the port pin */
    DIO_ModuleMapMaskType PinMask; /**< Port pin mask  */
    uint8_t channel; /**< ADC Channel number  */
} ADC_MapType;

#if defined(RX110_NOMODULE_CUSTOM1)
#define ADC_MAPPED_CHANNELS_NUMBER 8
#elif defined(RX110_NOMODULE_CUSTOM2)
#define ADC_MAPPED_CHANNELS_NUMBER 14
#else
#error "Must define RX110_NOMODULE_CUSTOMXX"
#endif


/*
 * Function: ADC_GetChannel
 * Description: Starting from portAddress and pinMask (read pin) search for AD channel
 * Note:
 * Status:
 */
bool_t ADC_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pADch);

/*
 * Macro: ADC_GetActiveChannel
 * Description: Channel selected for ATD convertion
 * Note:
 * Status:
 */
#define ADC_GetActiveChannel()	(ADC_ReadChannelA())

/*
 * Macro: ADC_WaitForConvertionOnChannel
 * Description: Wait for conversion Complete
 * Note:
 * Status: Tested!
 */
#define ADC_WaitForConvertionOnChannel(channel){\
		while(ADC_IsConversionCompleteA())\
		{}\
}
/**
 \n\li ADC Conversion Result depends on the desired resolution and the module can be programmed for 8, 10 or 12 bits accordingly
 \n\li This function get the converted value and modify it to the desired resolution
 */
#if   (ADC_RESOLUTION_MODE > ADC_RESOLUTION)
#define ADC_GetCorrectResolutionResult(result)                  (result >>(ADC_RESOLUTION_MODE - ADC_RESOLUTION))
#elif (ADC_RESOLUTION_MODE < ADC_RESOLUTION)
#define ADC_GetCorrectResolutionResult(result)                  (result <<(ADC_RESOLUTION - ADC_RESOLUTION_MODE))
#else
#define ADC_GetCorrectResolutionResult(result)                  (result)
#endif

uint16_t ADC_GetConvertedValueOnChannel( uint8_t channel);




/*
 * Macro: ADC_EndConvertion
 * Description: Test if conversion is complete
 * Note:
 * Status:
 */
#define ADC_EndConvertion()          (ADC_IsConversionCompleteA())

/*
 * Macro: ADC_StopConversion
 * Description: Stops any possible ADC conversion
 * Note:
 * Status:
 */
#define ADC_StopConversion()            ADC_DisableChannelA()

/*
 * Function: ADC_Init
 * Description: Init of ADC Module
 * Note:
 * Status: Tested!
 */
void ADC_Init(void);

/**
 Set all ADC Registers to the reset state value
 */
void ADC_DeInit(void);

#endif /* #if defined(HAL_ADC_SUPPORT) */
/*@}*/
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN IRQ Class HAL Support                                   */
/** IRQ MAP **/
/*- IRQ0	Pin23	PH1 */
/*- IRQ1	Pin22	PH2 */
/*- IRQ5	Pin19	P15 */
/*- IRQ6	Pin43	PA3 */
/*- ELCIRQ0	Pin39	PB0 */
/*- ELCIRQ5	Pin35	PB5 */
/*- ELCIRQ6	Pin34	PB6 */
/* ********************************************************************************************* */
#if defined(HAL_IRQ_SUPPORT)


#define RX110_IRQ0_PH1		0	/* IRQ0 	Pin23	PH1*/
#define RX110_IRQ1_PH2		1	/* IRQ1 	Pin22	PH2*/
#define RX110_IRQ5_P15		2	/* IRQ5	Pin19	P15*/
#define RX110_IRQ6_PA3		3	/* IRQ6 	Pin43	PA3*/
#define RX110_ELCIRQ0_PB0	4	/* ELCIRQ0 Pin39	PB0*/
#define RX110_ELCIRQ5_PB5	5	/* ELCIRQ5 Pin35	PB5*/
#define RX110_ELCIRQ6_PB6	6	/* ELCIRQ6 Pin34	PB6*/
#define RX110_IRQ_PB1		7	/* TO configure*/
#define RX110_IRQ_PB3		8	/* TO configure*/
#define RX110_IRQ_PB7		9	/* TO configure*/

#define _HAL_RX100_PH1_PORT_PIN_AS_IRQ0		MPC.PH1PFS.BYTE |= 0x40
#define _HAL_RX100_PH2_PORT_PIN_AS_IRQ1		MPC.PH2PFS.BYTE |= 0x40
#define _HAL_RX100_P15_PORT_PIN_AS_IRQ5		MPC.P15PFS.BYTE |= 0x40
#define _HAL_RX100_PA3_PORT_PIN_AS_IRQ6		MPC.PA3PFS.BYTE |= 0x40
#define _HAL_RX100_PH1_PORT_PIN_AS_IO		{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PH1PFS.BYTE = 0x00;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define _HAL_RX100_PH2_PORT_PIN_AS_IO		{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PH2PFS.BYTE = 0x00;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define _HAL_RX100_P15_PORT_PIN_AS_IO		{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.P15PFS.BYTE = 0x00;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define _HAL_RX100_PA3_PORT_PIN_AS_IO		{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PA3PFS.BYTE = 0x00;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}

#define _HAL_RX100_DETECTION_SENSE_LOW_LEVEL 		0x00
#define _HAL_RX100_DETECTION_SENSE_FALLING_EDGE 	0x04
#define _HAL_RX100_DETECTION_SENSE_RAISING_EDGE 	0x08
#define _HAL_RX100_DETECTION_SENSE_BOTH_EDGE 		0x0C	/*Rising and falling edges*/
#define _HAL_RX100_DETECTION_SENSE_ELC_FALLING_EDGE 0x01	/*IRQ detect for ELC module*/
#define _HAL_RX100_DETECTION_SENSE_ELC_RAISING_EDGE 0x00	/*IRQ detect for ELC module*/


/* Interrupt Request Register */
#define _HAL_RX100_IRQ_INTERRUPT_REQUEST_REGISTER(vector)		ICU.IR[vector].BYTE
/* IRQ Control Register */
#define _HAL_RX100_IRQ_CONTROL_REGISTER(irq)					ICU.IRQCR[irq].BYTE

/* Interrupt Request Enable Register */
#define _HAL_RX100_INTERRUPT_REQUEST_ENABLE_REGISTER(ier)	ICU.IER[ier].BYTE
/* Digital Filter Enable Register*/
#define _HAL_RX100_DIGITAL_FILTER_ENABLE_REGISTER	ICU.IRQFLTE0.BYTE

#define _HAL_RX100_ICU_IPR(vector)			ICU.IPR[vector].BYTE
#define _HAL_RX100_ICU_IRQ_IER_REGISTER		8

/* IRQ0*/
#define IRQ0								0
#define _HAL_RX100_ICU_IRQ0_VECTOR			64
#define _HAL_RX100_ICU_IRQ0_IER_MASK		(1<<0)
#define IRQ0_INTERRUPT_PRIORITY				0x01
#define IRQ0_DIGITAL_FILTER_MASK			(1<<0)
/* IRQ1*/
#define IRQ1								1
#define _HAL_RX100_ICU_IRQ1_VECTOR			65
#define _HAL_RX100_ICU_IRQ1_IER_MASK		(1<<1)
#define IRQ1_INTERRUPT_PRIORITY				0x01
#define IRQ1_DIGITAL_FILTER_MASK			(1<<1)
/* IRQ5*/
#define IRQ5								5
#define _HAL_RX100_ICU_IRQ5_VECTOR			69
#define _HAL_RX100_ICU_IRQ5_IER_MASK		(1<<5)
#define IRQ5_INTERRUPT_PRIORITY				0x01
#define IRQ5_DIGITAL_FILTER_MASK			(1<<5)
/* IRQ6*/
#define IRQ6								6
#define _HAL_RX100_ICU_IRQ6_VECTOR			70
#define _HAL_RX100_ICU_IRQ6_IER_MASK		(1<<6)
#define IRQ6_INTERRUPT_PRIORITY				0x01
#define IRQ6_DIGITAL_FILTER_MASK			(1<<6)

/* Define for IRQ_PB0, IRQ_PB1, IRQ_PB3, IRQ_PB5, IRQ_PB6, IRQ_PB7 */
#define _HAL_RX100_ICU_ELC_ELSR18L_VECTOR	106
#define _HAL_RX100_ICU_ELC_ELSR19L_VECTOR	107
#define _HAL_RX100_ICU_ELC_IER_REGISTER		13
#define _HAL_RX100_ICU_ELC_ELSR18L_IER_MASK	(1<<2)
#define _HAL_RX100_ICU_ELC_ELSR19L_IER_MASK	(1<<3)
#define ELC_ELSR18L_INTERRUPT_PRIORITY		0x01
#define ELC_PB0_MASK 						(1<<0)
#define ELC_PB1_MASK 						(1<<1)
#define ELC_PB3_MASK 						(1<<3)
#define ELC_PB5_MASK 						(1<<5)
#define ELC_PB6_MASK 						(1<<6)
#define ELC_PB7_MASK 						(1<<7)

/*
 * Function: IRQ_Init
 * Description:
 * Note:
 * Status:
 */
void IRQ_Init(uint8_t irq_id);

/*
 * Function: IRQ_SetEdge
 * Description:
 * Note:
 * Status:
 */
void IRQ_SetEdge(uint8_t edge, uint8_t irq_id);

/*
 * Macro: IRQ_IntEnable(enable)
 * Decritpion:
 * Note:
 * 	- enable possible vale are 1/0 enable/disable
 * Status:
 * 	- IRQ1; IRQ5, IRQ6 inserted
 */
void IRQ_IntEnable(uint8_t enable, uint8_t irq_id);

/*
 * Macro: IRQ_DeInit
 * Description: Clear Interrupt Request Status and Control Register & Disable Bus clock to the IRQ module.
 * Note: De Init all IRQ
 * Status:
 */
void IRQ_DeInit(uint8_t irq_id);


/*
 * Function: IRQ_Acknowledge
 * Description:
 * Note: Two NOP cycles are reccomended after the clearing of the interrupt flag in order to take into account the sync mechanism between NVIC and GPIO domain
 * Status:
 */
void IRQ_Acknowledge(uint8_t irq_id);

/*
 * Function: IRQ_GetEdge
 * Description:
 * Note:
 * Status:
 */
bool_t IRQ_GetEdge(uint8_t irq_id);

#define IRQ0_PIN_STATUS		(PORTH.PIDR.BYTE & 0x02)
#define IRQ1_PIN_STATUS 	(PORTH.PIDR.BYTE & 0x04)
#define IRQ5_PIN_STATUS 	(PORT1.PIDR.BYTE & 0x20)
#define IRQ6_PIN_STATUS 	(PORTA.PIDR.BYTE & 0x08)
#define ELC_IRQ_PB0_STATUS 	(PORTB.PIDR.BYTE & 0x01)
#define ELC_IRQ_PB1_STATUS  (PORTB.PIDR.BYTE & 0x02)
#define ELC_IRQ_PB3_STATUS 	(PORTB.PIDR.BYTE & 0x08)
#define ELC_IRQ_PB5_STATUS 	(PORTB.PIDR.BYTE & 0x20)
#define ELC_IRQ_PB6_STATUS 	(PORTB.PIDR.BYTE & 0x40)
#define ELC_IRQ_PB7_STATUS 	(PORTB.PIDR.BYTE & 0x80)

/*
 * Function: IRQ_pinIsHigh
 * Description: Check if the IRQ pin is High
 * Note:
 * Status
 */
bool_t IRQ_pinIsHigh(uint8_t irq_id);

#define IRQ_pinIsLow(irq_id) (!IRQ_pinIsHigh(irq_id))

#endif /*HAL_IRQ SUPPORT*/
/* ********************************************************************************************** */
/*                                  END IRQ Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN CRC Class HAL Support                                   */
/* ********************************************************************************************** */
#define CRC_HW_SUPPORTED				/* RX210 support HW CRC*/

/* CRC Register Definitions */
/* CRC Control Register */
#define _HAL_RX100_CRC_CONTROL_REGISTER			CRC.CRCCR.BYTE
#define	BIT_CRC_MASK_8		0x01		/* 8bit CRC  X8+ X2+ X + 1*/
#define BIT_CRC_MASK_16		0x03		/* 16 bit CRC X16+ X15+ X5+ 1*/
#define BIT_CRC_LMS_MASK	(1<<2)		/* 0 Perform Operation LSB first; 1 Perform Operation MSB first*/
#define DORCLR_CRC_BIT_MASK		(1<<7)
/* CRC Data Input Register */
#define _HAL_RX100_CRC_DATA_INPUT_REGISTER		CRC.CRCDIR
/* CRC Data Output Register */
#define _HAL_RX100_CRC_DATA_OUTPUT_REGISTER		CRC.CRCDOR
/* CRC MASK Definitions */
#define CRC_LOW_POWER_MASK	(1<<23)	/* 1: CRC Low Power Mode Off, 0: CRC Active*/

/* CRC MACRO */
#define CRC_ClockGateOn(){\
		DISABLE_REGISTER_WRITE_PROTECTION;\
		_HAL_RX100_MODULE_STOP_CONTROL_1_REGISTER_B &= ~CRC_LOW_POWER_MASK;\
		ENABLE_REGISTER_WRITE_PROTECTION;\
}/*end CRC_ClockGateOn*/
#define CRC_ClockGateOff(){\
	DISABLE_REGISTER_WRITE_PROTECTION;\
	_HAL_RX100_MODULE_STOP_CONTROL_1_REGISTER_B |= CRC_LOW_POWER_MASK;\
	ENABLE_REGISTER_WRITE_PROTECTION;\
}/* end ClockGateOff*/

#define InitCRCmodule(){\
	CRC_ClockGateOn();\
	_HAL_RX100_CRC_CONTROL_REGISTER |= BIT_CRC_MASK_16;\
	_HAL_RX100_CRC_CONTROL_REGISTER |= BIT_CRC_LMS_MASK;\
	_HAL_RX100_CRC_CONTROL_REGISTER |= DORCLR_CRC_BIT_MASK;\
	_HAL_RX100_CRC_DATA_OUTPUT_REGISTER = 0;\
}/*end InitCRCmoudule*/
#define DeInitCRCmodule(){\
	_HAL_RX100_CRC_CONTROL_REGISTER &= 0x00;\
	_HAL_RX100_CRC_CONTROL_REGISTER &= 0x00;\
}
/*Seed is the initial value for CRC*/
#define InitCRC(seed)		{\
	CRC_ClockGateOn();\
	_HAL_RX100_CRC_CONTROL_REGISTER |= BIT_CRC_MASK_16;\
	_HAL_RX100_CRC_CONTROL_REGISTER |= BIT_CRC_LMS_MASK;\
	_HAL_RX100_CRC_CONTROL_REGISTER |= DORCLR_CRC_BIT_MASK;\
	_HAL_RX100_CRC_DATA_OUTPUT_REGISTER = seed;\
}

#define ComputeCRC(data){\
		_HAL_RX100_CRC_DATA_INPUT_REGISTER = data;\
}/*end ComputeCRC*/
#define GetFinalCRC()	_HAL_RX100_CRC_DATA_OUTPUT_REGISTER

/* ********************************************************************************************** */
/*                                  END CRC Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DMA Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name DMA
 * Features:
 * 	- 4 channels
 * 	- 512Mbytes transfer space
 */
/*@{*/

/**
DMAC Register definitions RX200
*/
/* DMA Source Address Register (DMSAR)*/
#define DMAC0_SOURCE_ADDRESS_REGISTER	DMAC0.DMSAR
#define DMAC1_SOURCE_ADDRESS_REGISTER	DMAC1.DMSAR
#define DMAC2_SOURCE_ADDRESS_REGISTER	DMAC2.DMSAR
#define DMAC3_SOURCE_ADDRESS_REGISTER	DMAC3.DMSAR
#define _HAL_RX100_DMA_SOURCE_ADDRESS_REGISTER(n)	DMAC##n##_SOURCE_ADDRESS_REGISTER
/* DMA Destination Address Register*/
#define DMAC0_DESTINATION_ADDRESS_REGISTER	DMAC0.DMDAR
#define DMAC1_DESTINATION_ADDRESS_REGISTER	DMAC1.DMDAR
#define DMAC2_DESTINATION_ADDRESS_REGISTER	DMAC2.DMDAR
#define DMAC3_DESTINATION_ADDRESS_REGISTER	DMAC3.DMDAR
#define _HAL_RX100_DMA_DESTINATION_ADDRESS_REGISTER(n)	DMAC##n##_DESTINATION_ADDRESS_REGISTER
/* DMA Transfer Count Register*/
#define DMAC0_TRANSFER_COUNT_REGISTER	DMAC0.DMCRA
#define DMAC1_TRANSFER_COUNT_REGISTER	DMAC1.DMCRA
#define DMAC2_TRANSFER_COUNT_REGISTER	DMAC2.DMCRA
#define DMAC3_TRANSFER_COUNT_REGISTER	DMAC3.DMCRA
#define _HAL_RX100_DMA_TRANSFER_COUNT_REGISTER(n)	DMAC##n##_TRANSFER_COUNT_REGISTER
/* DMA Block Transfer Count Register*/
#define DMAC0_BLOCK_TRANSFER_COUNT_REGISTER	DMAC0.DMCRB
#define DMAC1_BLOCK_TRANSFER_COUNT_REGISTER	DMAC1.DMCRB
#define DMAC2_BLOCK_TRANSFER_COUNT_REGISTER	DMAC2.DMCRB
#define DMAC3_BLOCK_TRANSFER_COUNT_REGISTER	DMAC3.DMCRB
#define _HAL_RX100_DMA_BLOCK_TRANSFER_COUNT_REGISTER(n)	DMAC##n##_BLOCK_TRANSFER_COUNT_REGISTER
/* DMA Transfer Mode Register*/
/* DMA Request Source select */
#define SOFTWARE_REQUEST	0x0000
#define INTERRUPT_REQUEST	0x0001
/* DMA Transfer data size */
#define DMA_TRANSFER_DATA_SIZE_8
#define DMA_TRANSFER_DATA_SIZE_16 	(0x1u<<8)
#define DMA_TRANSFER_DATA_SIZE_32 	(0x2u<<8)
/* DMA Repeat area select */
#define REPEAT_AREA_IS_DESTIONATION		0x0000
#define REPEAT_AREA_IS_SOURCE			0x1000
#define REPEAT_AREA_IS_NOT_SPECIFIED	0x2000
/* DMA Transfer MODE Select */
#define DMA_NORMAL_MODE 0
#define DMA_REPEAT_MODE 1
#define DMA_BLOCK_MODE  2
#define DMA_NORMAL_TRANSFER_MASK
#define DMA_REPEAT_TRANSFER_MASK 	(0x1u<<14)
#define DMA_BLOCK_TRANSFER_MASK 	(0x2u<<14)

#define DMAC0_TRANSFER_MODE_REGISTER	DMAC0.DMTMD.WORD
#define DMAC1_TRANSFER_MODE_REGISTER	DMAC1.DMTMD.WORD
#define DMAC2_TRANSFER_MODE_REGISTER	DMAC2.DMTMD.WORD
#define DMAC3_TRANSFER_MODE_REGISTER	DMAC3.DMTMD.WORD
#define _HAL_RX100_DMA_TRANSFER_MODE_REGISTER(n)	DMAC##n##_TRANSFER_MODE_REGISTER
/* DMA Interrupt Setting Register*/
/* ICU */
#define DMAC_ICU_INTERRUPT_BASE_VECTOR	198
/* channel 0 */
#define DMAC_ICU_PRIORITY_INT_CH0		2
#define DMAC_CHANNEL0_ICU_IER			0x18
#define DMAC_CHANNEL0_IER_BIT			(1<<6)
/* channel 1 */
#define DMAC_ICU_PRIORITY_INT_CH1		1
#define DMAC_CHANNEL1_ICU_IER			0x18
#define DMAC_CHANNEL1_IER_BIT			(1<<7)
/* channel 2 */
#define DMAC_ICU_PRIORITY_INT_CH2		1
#define DMAC_CHANNEL2_ICU_IER			0x19
#define DMAC_CHANNEL2_IER_BIT			(1<<0)
/* channel 3 */
#define DMAC_ICU_PRIORITY_INT_CH3		1
#define DMAC_CHANNEL3_ICU_IER			0x19
#define DMAC_CHANNEL3_IER_BIT			(1<<1)

/* DMAC Activation Request Select Register */
#define DMAC0_ICU_ACTIVATION_REGISTER		ICU.DMRSR0
#define DMAC1_ICU_ACTIVATION_REGISTER		ICU.DMRSR1
#define DMAC2_ICU_ACTIVATION_REGISTER		ICU.DMRSR2
#define DMAC3_ICU_ACTIVATION_REGISTER		ICU.DMRSR3
#define _HAL_RX100_DMA_ICU_ACTIVATION_REGISTER(n)	DMAC##n##_ICU_ACTIVATION_REGISTER

/* DMAC */
#define DMA_TRANSFER_END_INTERRUTP_MASK		(1<<4)
#define DMAC0_INTERRUPT_SETTING_REGISTER	DMAC0.DMINT.BYTE
#define DMAC1_INTERRUPT_SETTING_REGISTER	DMAC1.DMINT.BYTE
#define DMAC2_INTERRUPT_SETTING_REGISTER	DMAC2.DMINT.BYTE
#define DMAC3_INTERRUPT_SETTING_REGISTER	DMAC3.DMINT.BYTE
#define _HAL_RX100_DMA_INTERRUPT_SETTING_REGISTER(n)	DMAC##n##_INTERRUPT_SETTING_REGISTER
/* DMA Address Mode Register*/
#define DMA_DESTIONATION_ADDRESS_UPDATE_MODE_FIXED
#define DMA_DESTIONATION_ADDRESS_UPDATE_MODE_OFFSET			(0x1u<<6)
#define DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT		(0x2u<<6)
#define DMA_DESTIONATION_ADDRESS_UPDATE_MODE_DECREMENT		(0x3u<<6)

#define DMA_SOURCE_ADDRESS_UPDATE_MODE_FIXED
#define DMA_SOURCE_ADDRESS_UPDATE_MODE_OFFSET			(0x1u<<14)
#define DMA_SOURCE_ADDRESS_UPDATE_MODE_INCREMENT		(0x2u<<14)
#define DMA_SOURCE_ADDRESS_UPDATE_MODE_DECREMENT		(0x3u<<14)

#define DMAC0_ADDRESS_MODE_REGISTER		DMAC0.DMAMD.WORD
#define DMAC1_ADDRESS_MODE_REGISTER		DMAC1.DMAMD.WORD
#define DMAC2_ADDRESS_MODE_REGISTER		DMAC2.DMAMD.WORD
#define DMAC3_ADDRESS_MODE_REGISTER		DMAC3.DMAMD.WORD
#define _HAL_RX100_DMA_ADDRESS_MODE_REGISTER(n)	DMAC##n##_ADDRESS_MODE_REGISTER
/* DMA Offset Register*/
#define DMAC0_OFFSET_REGISTER		DMAC0.DMOFR
#define DMAC1_OFFSET_REGISTER		DMAC1.DMOFR
#define DMAC2_OFFSET_REGISTER		DMAC2.DMOFR
#define DMAC3_OFFSET_REGISTER		DMAC3.DMOFR
#define _HAL_RX100_DMA_OFFSET_REGISTER(n)	DMAC##n##_OFFSET_REGISTER
/* DMA Transfer Enable Register*/
#define DMA_DTE_BIT_MASK (1<<0)
#define DMAC0_TRANSFER_ENABLE_REGISTER		DMAC0.DMCNT.BYTE
#define DMAC1_TRANSFER_ENABLE_REGISTER		DMAC1.DMCNT.BYTE
#define DMAC2_TRANSFER_ENABLE_REGISTER		DMAC2.DMCNT.BYTE
#define DMAC3_TRANSFER_ENABLE_REGISTER		DMAC3.DMCNT.BYTE
#define _HAL_RX110_DMA_TRANSFER_ENABLE_REGISTER(n)	DMAC##n##_TRANSFER_ENABLE_REGISTER
/* DMA Software Start Register*/
#define DMREQ_SWREQ_MASK					(1<<0)
#define DMAC0_SOFTWARE_START_REGISTER		DMAC0.DMREQ.BYTE
#define DMAC1_SOFTWARE_START_REGISTER		DMAC1.DMREQ.BYTE
#define DMAC2_SOFTWARE_START_REGISTER		DMAC2.DMREQ.BYTE
#define DMAC3_SOFTWARE_START_REGISTER		DMAC3.DMREQ.BYTE
#define _HAL_RX110_DMA_SOFTWARE_START_REGISTER(n)	DMAC##n##_SOFTWARE_START_REGISTER
/* DMA Status Register*/
#define DMA_ESIF_BIT_MASK			(1<<0)	/* Transfer Escape end Interrupt 	*/
#define DMA_DTIF_BIT_MASK			(1<<4)	/* Transfer End Interrupt Flag 		*/
#define DMA_ACT_BIT_MASK			(1<<7)	/* Transfer Active flah 			*/
#define DMAC0_STATUS_REGISTER		DMAC0.DMSTS.BYTE
#define DMAC1_STATUS_REGISTER		DMAC1.DMSTS.BYTE
#define DMAC2_STATUS_REGISTER		DMAC2.DMSTS.BYTE
#define DMAC3_STATUS_REGISTER		DMAC3.DMSTS.BYTE
#define _HAL_RX110_DMA_STATUS_REGISTER(n)	DMAC##n##_STATUS_REGISTER
/* DMA Activation Source Flag Control Register*/
#define DMAC0_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER		DMAC0.DMCSL.BYTE
#define DMAC1_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER		DMAC1.DMCSL.BYTE
#define DMAC2_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER		DMAC2.DMCSL.BYTE
#define DMAC3_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER		DMAC3.DMCSL.BYTE
#define _HAL_RX110_DMA_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER(n)	DMAC##n##_ACTIVATION_SOURCE_FLAG_CONTROL_REGISTER
/* DMA Module Activation Register*/
#define _HAL_RX110_DMA_MODULE_ACTIVATION_REGISTER_ENABLE	DMAC.DMAST.BIT.DMST = 1
#define _HAL_RX110_DMA_MODULE_ACTIVATION_REGISTER_DISABLE	DMAC.DMAST.BIT.DMST = 0
/* (ICU)DMAC Activation Request Select Register m */
#define DMAC_ACTIVATION_REQUEST_SELECT_REGISTER_0_		ICU.DMRSR0
#define DMAC_ACTIVATION_REQUEST_SELECT_REGISTER_1_		ICU.DMRSR1
#define DMAC_ACTIVATION_REQUEST_SELECT_REGISTER_2_		ICU.DMRSR2
#define DMAC_ACTIVATION_REQUEST_SELECT_REGISTER_3_		ICU.DMRSR3
#define _HAL_RX110_DMAC_ACTIVATION_REQUEST_SELECT_REGISTER(n)	DMAC_ACTIVATION_REQUEST_SELECT_REGISTER_##n##_

#define OCDMA_CH_NUM 4		/* !< RX210 has 4 four DMA channels */

/**
DMA Functions prototypes
*/
void DMA_ClockGate(void);
void DMA_SetDevice(uint8_t channel, uint8_t device);
uint8_t DMA_GetDevice(uint8_t channel);
void DMA_SetSAR(uint8_t channel , FLS_positionType address);
uint8_t* DMA_GetSAR(uint8_t channel);
void DMA_SetDAR(uint8_t channel, FLS_positionType address);
uint8_t* DMA_GetDAR(uint8_t channel);
void DMA_SetBCR(uint8_t channel, uint32_t count);
uint32_t DMA_GetBCR(uint8_t channel);
void DMA_EnableInt(uint8_t channel);
void DMA_DisableInt(uint8_t channel);
void DMA_SetStartOn(uint8_t channel);
void DMA_SetStartOff(uint8_t channel);
bool_t DMA_IsChannelBusy(uint8_t channel);
uint8_t DMA_GetTrasferMode(uint8_t channel);
bool_t DMA_IsTransDone(uint8_t channel);
void DMA_ClearTransDone(uint8_t channel);
bool_t IsDMAChannelTrasferEnabled(uint8_t channel);
bool_t DMA_IsPending(uint8_t channel);
bool_t DMA_IsDestinationError(uint8_t channel);
bool_t DMA_IsSourceError(uint8_t channel);
bool_t DMA_IsConfigError(uint8_t channel);
void DMA_PeripheralRequestOn(uint8_t channel);
void DMA_PeripheralRequestOff(uint8_t channel);
void DMA_ContinuousTransfer(uint8_t channel);
void DMA_SingleTransfer(uint8_t channel);
void DMA_BlockTransfer(uint8_t channel);
void DMA_SetDestinationSizeToByte(uint8_t channel);
void DMA_SetDestinationSizeToWord(uint8_t channel);
void DMA_SetDestinationSizeToLong(uint8_t channel);
void DMA_SetSourceSizeToLong(uint8_t channel);
void DMA_SetSourceSizeToByte(uint8_t channel);
void DMA_SetSourceSizeToWord(uint8_t channel);
void DMA_EnableSourceIncrement(uint8_t channel);
void DMA_DisableSourceIncrement(uint8_t channel);
void DMA_EnableDestinationIncrement(uint8_t channel);
void DMA_DisableDestinationIncrement(uint8_t channel);
void DMA_EnableAutoAlign(uint8_t channel);
void DMA_DisableAutoAlign(uint8_t channel);
void DMA_SetSourceAddressModulo(uint8_t channel, uint8_t module);
void DMA_SetDestinationAddressModulo(uint8_t channel, uint8_t module);
void DMA_SetDisableRequestOn(uint8_t channel);
void DMA_SetEnableRequestOn(uint8_t channel);
void DMA_LinkChannelControlMode(uint8_t channel, uint8_t mode);
void DMA_LinkChannel1Mode(uint8_t channel, uint8_t mode);
void DMA_LinkChannel2Mode(uint8_t channel, uint8_t mode);
void DMA_ControlDefault(uint8_t channel);
void DMA_SetNumberOfRepetion(uint8_t channel,uint16_t count);
void DMA_SetRepeatArea(uint8_t channel, uint16_t mode);
void DMA_SetDisableRequestOff(uint8_t channel);
/**
DMA control register setting
*/
/* #define     DMA_ControlDefault(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = 0;} */



/* This section contains HAL class extension */


/*@}*/
/* ********************* */
/* DMA Class HAL Support */
/* ********************* */
/* ********************************************************************************************** */
/*                                  END DMA Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN SPI Class HAL Support                                   */
/* ********************************************************************************************** */
/** SPI CFG	**/
/* #define HAL_SPI_SUPPORT */
#if defined(HAL_SPI_SUPPORT)
#define SPI_ENABLE	TRUE
#define SPI_DISABLE	FALSE

/*  The are defined as SPI1 for driver compatibility */
#define SPI1_RX_BUF_SIZE_CFG 	32
#define SPI1_TX_BUF_SIZE_CFG 	32

#define SPI1_ENABLE	TRUE

/* Set SPI Pins */
/* SET SSL Port A4 */
#define SPI_SET_SSL_MPC_PIN		{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PA4PFS.BYTE = 0x0D;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define SPI_SET_SSL_PIN_DIR		{PORTA.PDR.BIT.B4 = PORT_PIN_IS_OUTPUT; PORTA.PMR.BIT.B4 = PIN_IS_FOR_PHERIPHERAL_FUNCTIONS;}
#define SPI_SET_SSL_PIN			{SPI_SET_SSL_PIN_DIR;SPI_SET_SSL_MPC_PIN;}
/* SET MOSI - Port A6 */
#define SPI_SET_MOSI_MPC_PIN	{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PA6PFS.BYTE = 0x0D;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define SPI_SET_MOSI_PIN_DIR	{PORTA.PDR.BIT.B6 = PORT_PIN_IS_OUTPUT; PORTA.PMR.BIT.B6 = PIN_IS_FOR_PHERIPHERAL_FUNCTIONS;}
#define SPI_SET_MOSI_PIN		{SPI_SET_MOSI_PIN_DIR;SPI_SET_MOSI_MPC_PIN;}
/* SET MISO - Port C7 */
#define SPI_SET_MISO_MPC_PIN	{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PC7PFS.BYTE = 0x0D;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define SPI_SET_MISO_PIN_DIR	{PORTC.PDR.BIT.B7 = PORT_PIN_IS_OUTPUT; PORTC.PMR.BIT.B7 = PIN_IS_FOR_PHERIPHERAL_FUNCTIONS;}
#define SPI_SET_MISO_PIN		{SPI_SET_MISO_PIN_DIR;SPI_SET_MISO_MPC_PIN;}
/* SCLK - Port C5 */
#define SPI_SET_SCLK_MPC_PIN	{DISABLE_PORT_REGISTER_WRITE_PROTECTION();MPC.PC5PFS.BYTE = 0x0D;ENABLE_PORT_REGISTER_WRITE_PROTECTION();}
#define SPI_SET_SCLK_PIN_DIR	{PORTC.PDR.BIT.B5 = PORT_PIN_IS_OUTPUT; PORTC.PMR.BIT.B5 = PIN_IS_FOR_PHERIPHERAL_FUNCTIONS;}
#define SPI_SET_SCLK_PIN		{SPI_SET_SCLK_PIN_DIR;SPI_SET_SCLK_MPC_PIN;}
/**/
/* BaudRate Setting ( SBPR = (PCLK/(2^BIT_RATE_DIVISION*2*SPI1_BAUDRATE))-1) */
#define SPI1_BAUDRATE	4000000
/* BIT Rate Division */
#define BIT_RATE_DIVISION	BASE_BIT_RATE_DIV_8
#define BASE_BIT_RATE_DIV_1	0
#define BASE_BIT_RATE_DIV_2	1
#define BASE_BIT_RATE_DIV_4 2
#define BASE_BIT_RATE_DIV_8 3
/**/
#define _HAL_SPI_BAUDRATE(baud_rate)	(((uint32_t)BUS_FREQUENCY_Hz/((1 << BIT_RATE_DIVISION)*2*baud_rate))-1)
/**/
/* SPI Clock Polarity */
#define SPI_CLOCK_POLARITY_CPOL		CPOL_HIGH
#define CPOL_LOW	0
#define CPOL_HIGH	1
/**/
/*SPI Clock Phase*/
#define SPI1_CLOCK_PHASE  					SPI_DATA_SAMPLING_ON_ODD_EDGE
#define SPI_DATA_SAMPLING_ON_ODD_EDGE 		0
#define SPI_DATA_SAMPLING_ON_EVEN_EDGE 		1
/**/
/* SPI LSB or MSB First */
#define SPI1_SHIFT_DIRECTION				SPI_MSB_FIRST
#define SPI_MSB_FIRST						0
#define SPI_LSB_FIRST						1
/**/
/* SPI Interrupt definitions */
#define SPI_INT_PRIORITY_VECT	44		/* !< SPI interrupt priority is the same for all interrupt sources */
#define SPI_INT_IER				0x05
#define SPI_INT_PRIORITY		0x01
#define _HAL_RX100_SPI_IPR_REG 	ICU.IPR[SPI_INT_PRIORITY_VECT].BYTE
#define _HAL_RX100_SPI_IER_REG  ICU.IER[SPI_INT_IER].BYTE
/*RX*/
#define SPI_RX_INTERRUPT_ENABLE		TRUE
#define SPI_RX_INT_VECT				45
#define _HAL_RX100_SPI_RX_IR       	ICU.IR[SPI_RX_INT_VECT].BYTE
#define SPI_RX_IER_BIT				(1<<5)
/**/
/*TX*/
#define SPI_TX_INTERRUPT_ENABLE		TRUE
#define SPI_TX_INT_VECT				46
#define SPI_TX_IER_BIT				(1<<6)
#define _HAL_RX100_SPI_TX_IR       	ICU.IR[SPI_TX_INT_VECT].BYTE
/**/
/**/
/* SS Polarity */
#define PDL_SPI_PIN_SSLO 			PDL_SPI_PIN_SSL0_LOW
#define PDL_SPI_PIN_SSL0_LOW 		0
#define PDL_SPI_PIN_SSL0_HIGH 		1
/* MOSI idle control */
#define PDL_SPI_PIN_MOST_IDLE		PDL_SPI_PIN_MOSI_IDLE_LAST
#define PDL_SPI_PIN_MOSI_IDLE_LAST	0x00
#define PDL_SPI_PIN_MOSI_IDLE_LOW	0x20
#define PDL_SPI_PIN_MOSI_IDLE_HIGH	0x30
/* Clock Polarity */
#define SPI1_CLOCK_POLARITY		CLK_POL_ACTIVE_LOW
#define CLK_POL_ACTIVE_LOW  0
#define CLK_POL_ACTIVE_HIGH 1


/* Functions prototype */
void SPI_IntEnable(unsigned char par);
void SPI_ClkEnable(unsigned char par);
void SPI_MasterEnable(unsigned char par);
void SPI_Enable(unsigned char par);
void SPI_SetClockPolarity(unsigned char par, unsigned char par_1);
void SPI_SetClockPhase(unsigned char par, unsigned char pha);
void SPI_ResetStatus(unsigned char par);
void SPI_SetBaudRate(unsigned char par_1, unsigned long int baudrate);
void SPI_SetShiftDir(unsigned char par ,unsigned char shift_direction);
void SPI_WriteData(unsigned char par_1, unsigned char data);
unsigned char SPI_ReadData(unsigned char par_1);
unsigned char SPI_GetStatus(unsigned char par);
#endif /* defined(HAL_SPI_SUPPORT)  */
/**/
/* ********************************************************************************************** */
/*                                  END SPI Class HAL Support                                   */
/* ********************************************************************************************** */

/* ***********************************************************************************************/
/*                                  START FLS Class HAL Support                                  */

/* ***********************************************************************************************/
#include "RN_RX100_hal_Flash.h"
/* ***********************************************************************************************/
/*                                  END FLS Class HAL Support                                   */
/* ***********************************************************************************************/

/* ***********************************************************************************************/
/*                                  START LPM Class HAL Support  (Low Power Mode )               */
/* ***********************************************************************************************/
void LPM_AllClockOff(void);
void LPM_SleepModeInit(void);
void LPM_SleepModeProc(void);
void LPM_SleepModeProcExit(void);
void LPM_WakupIoEnable(int pin, int enable);

/* ***********************************************************************************************/
/*                                  END LPM Class HAL Support  					                 */
/* ***********************************************************************************************/


#endif	/*  end file _RN_RX100_HAL_H_ */
/* ********************************************************************************************** */
/*                                  END OF FILE 												  */
/* ********************************************************************************************** */
