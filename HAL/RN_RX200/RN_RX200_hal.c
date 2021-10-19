/**
 HAL Functions
 Hal Functions to access at hardware Resources from Driver layer
 \file    RN_RX210_R5F2108_hal.c
 \version 0.0.1
 \date    02/11/2011
 \author  Andrea Emili
 */
/*
 "Diritti Riservati"
 */
/* Old Versions
 \version
 \date
 \author
 \version
 \date
 \author
 */
#define MODULE_BODY_HAL
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_ERROR_HANDLING)
#include "LIB_Error.h"
#endif
#include "RN_RX200_hal.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**

\par TMR alternate pin configuration

Some timers can be configured on more than one pin; standard PLT modules use
a default pin, anyway on custom modules (CUSTOM_MODULE defined) it's possible
to change default:

HAL_CH_TGRA2_ON_PB5

HAL_CH_TGRA3_ON_P17
HAL_CH_TGRA3_ON_PC7

HAL_CH_TGRA4_ON_PB3
HAL_CH_TGRA4_ON_PE2

HAL_CH_TGRB0_ON_PA1

HAL_CH_TGRB2_ON_PE5

HAL_CH_TGRB3_ON_PB7
HAL_CH_TGRB3_ON_P17

HAL_CH_TGRB4_ON_P30
HAL_CH_TGRB4_ON_P54
HAL_CH_TGRB4_ON_PE3

HAL_CH_TGRC0_ON_PB1

HAL_CH_TGRC3_ON_P16

HAL_CH_TGRC4_ON_PE1
HAL_CH_TGRC4_ON_PE5

HAL_CH_TGRD3_ON_P16
HAL_CH_TGRD3_ON_PC4

HAL_CH_TGRD4_ON_P55
HAL_CH_TGRD4_ON_PC3
HAL_CH_TGRD4_ON_PE4

 */

/* TMR coherence checks */

/* 1. Alternate pin on TMR channels can be used only on CUSTOM_MODULE! */
#if !defined(CUSTOM_MODULE)

#if defined(HAL_CH_TGRA2_ON_PB5)
#error "Alternate pin on TGRAx channels cannot be used in PLT standard module!"
#endif

#if defined(HAL_CH_TGRB0_ON_PA1) || defined(HAL_CH_TGRB2_ON_PE5) || defined(HAL_CH_TGRB3_ON_P17) \
	|| defined(HAL_CH_TGRB4_ON_P30) || defined(HAL_CH_TGRB4_ON_P54) || defined(HAL_CH_TGRB4_ON_PE3)
#error "Alternate pins on TGRBx channels cannot be used in PLT standard module!"
#endif

#if defined(HAL_CH_TGRD3_ON_P16) || defined(HAL_CH_TGRD3_ON_PC4) || defined(HAL_CH_TGRD4_ON_P55) \
	|| defined(HAL_CH_TGRD4_ON_PC3) || defined(HAL_CH_TGRD4_ON_PE4)
#error "Alternate pins on TGRDx channels cannot be used in PLT standard module!"
#endif

#else

/* 2. A timer can be defined only on one pin - no multiple definition allowed! */

#if (defined(HAL_CH_TGRA3_ON_P17) && defined(HAL_CH_TGRA3_ON_PC7))
#error "TGRA3 alternate pin conficts! Only one HAL_CH_TGRA3_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRA4_ON_PB3) && defined(HAL_CH_TGRA4_ON_PE2))
#error "TGRA4 alternate pin conficts! Only one HAL_CH_TGRA4_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRB3_ON_PB7) && defined(HAL_CH_TGRB3_ON_P17))
#error "TGRB3 alternate pin conficts! Only one HAL_CH_TGRB3_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRB4_ON_P30) && defined(HAL_CH_TGRB4_ON_P54)) \
	|| (defined(HAL_CH_TGRB4_ON_P30) && defined(HAL_CH_TGRB4_ON_PE3))
	|| (defined(HAL_CH_TGRB4_ON_P54) && defined(HAL_CH_TGRB4_ON_PE3))
#error "TGRB4 alternate pin conficts! Only one HAL_CH_TGRB4_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRC4_ON_PE1) && defined(HAL_CH_TGRC4_ON_PE1))
#error "TGRC4 alternate pin conficts! Only one HAL_CH_TGRC4_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRD3_ON_P16) && defined(HAL_CH_TGRD3_ON_PC4))
#error "TGRD3 alternate pin conficts! Only one HAL_CH_TGRD3_ON_xx can be used."
#endif

#if (defined(HAL_CH_TGRD4_ON_P55) && defined(HAL_CH_TGRD4_ON_PC3)) \
	|| (defined(HAL_CH_TGRD4_ON_P55) && defined(HAL_CH_TGRD4_ON_PE4))
	|| (defined(HAL_CH_TGRD4_ON_PC3) && defined(HAL_CH_TGRD4_ON_PE4))
#error "TGRD4 alternate pin conficts! Only one HAL_CH_TGRD4_ON_xx can be used."
#endif

/* 3. Timers who share same pin cannot be configured together! */

#if (defined(HAL_CH_TGRA3_ON_P17) && defined(HAL_CH_TGRB3_ON_P17))
#error "TGRA3 and TGRB3 cannot be configured on same pin (P17) !"
#endif

#if (defined(HAL_CH_TGRB2_ON_PE5) && defined(HAL_CH_TGRC4_ON_PE5))
#error "TGRB2 and TGRC4 cannot be configured on same pin (PE5) !"
#endif

#if (defined(HAL_CH_TGRC3_ON_P16) && defined(HAL_CH_TGRD3_ON_P16))
#error "TGRC3 and TGRD3 cannot be configured on same pin (P16) !"
#endif

#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/****** SYSTEM STACKS *******************************************************/
int	UserStack[256] 		__attribute__ ((section (".userStack")));
int	InterruptStack[256] __attribute__ ((section (".interruptStack")));

/* ***************** */
/* Exported function */
/* ***************** */

/* ************** */
/* Local Function */
/* ************** */
#if defined(CONFIG_ERROR_HANDLING)
#define HAL_ErrorHandler(err)	PLT_ERROR_FATAL(PLT_ERROR_HAL_1)
#else
#define HAL_ErrorHandler(err)	{}
#endif


/* ********************************************************************************************** */
/*                                  BEGIN LVD Class HAL Support                                   */
/* ********************************************************************************************** */
/*
- 05/07/2011:
	- AE: Clean old code.
*/

/*
 * Function: LVDR_SetUp
 * Description: Sets the low voltage detection level to generate LV reset. Turns on the LV detection
 *				mode and turns on the reset on LV feature.
 * Note:
 * Status:
 */
void LVDR_SetUp(uint8_t tripPointVR)
{
    /*int dist_0, dist_1;*/
    int i;
    /*uint8_t SPMSC2_Mirror = 0x00;*/

    for(i = 0; i < 16; i++)
    {
        if((tripPointVR*10) >= (415 - (i *15)))
        {
            break;
        }
    }
    if(i > 15)
    {
        i = 15;
    }
    DISABLE_REGISTER_WRITE_PROTECTION;
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= 0xF0;
    _HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER &=	0xF0;
    SET_L_NIBBLE(_HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER,i);
    /* Select Internal reference voltage*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= ~LVCMPCR_EXCREFINP1_MASK;
    /* Slecting Vcc as the voltage for comparison*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= ~LVCMPCR_EXVCCINP1_MASK;
    /* Enable the voltage detection circuit 1*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER |= LVCMPCR_LVD1E_MASK;
    /* Enabling output of the result of comparison by the voltage monitoring 1 circuit)*/
    _HAL_RX210_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_0 |= LVD1CR0_LVD1CMPE_MASK;
    /*Enable Reset*/
    _HAL_RX210_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_0 |= LVD1CR0_LVD1RI_MASK;	/* Reset Mode Enable*/
    for(i = 0; i<5000; i++)
    {
        __asm__("nop");
    }
    _HAL_RX210_VOLTAGE_MONITORING_1_STATUS_REGISTER &= ~LVD1SR_LVD1DET_MASK;
    _HAL_RX210_VOLTAGE_MONITORING_1_CIRCUIT_COMPARATOR_REGISTER_0 |= LVD1CR0_LVD1RIE_MASK;	/* Interrutp/Reset Enable*/
    /*Enable Interrupt*/
    ENABLE_REGISTER_WRITE_PROTECTION;
}/*end LVDR_SetUp*/

/*
 * Function: LVDW_SetTrip
 * Description: Set voltage trip to generate LVD warning
 * Note:
 * Status:
 */
void LVDW_SetTrip(uint8_t tripPointVW)
{
    uint16_t i;
    /*int8_t dist_0,dist_1,dist_2, distance;*/

    for(i = 0; i < 16; i++)
    {
        if((tripPointVW*10) >= (415 - (i *15)))
            break;
    }
    if(i > 15)
        i = 15;
    DISABLE_REGISTER_WRITE_PROTECTION;
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= 0x0F; 	/* Reset V1 level select*/
    _HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER &=	0x0F;
    SET_H_NIBBLE(_HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER,i);
    /* Select Internal reference voltage V2*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= ~LVCMPCR_EXCREFINP2_MASK;
    /* Slecting Vcc as the voltage for comparison V2*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER &= ~LVCMPCR_EXVCCINP2_MASK;
    /* Enable the voltage detection circuit 2*/
    _HAL_RX210_VOLTAGE_MONITOR_CONTROL_REGISTER |= LVCMPCR_LVD2E_MASK;
    /* Enabling output of the result of comparison by the voltage monitoring 1 circuit)*/
    _HAL_RX210_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_0 |= LVD2CR0_LVD2CMPE_MASK;
    /*Enable Reset*/
    _HAL_RX210_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_0 &= ~LVD2CR0_LVD2RI_MASK;	/* Interrupt Mode Enable*/
    /*Enable Maskable Interrupt*/
    _HAL_RX210_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_1  |= _HAL_BIT2_MASK;
    /* LVD2 DET Reset*/
    _HAL_RX210_VOLTAGE_MONITORING_2_STATUS_REGISTER &= ~_HAL_BIT0_MASK;
    for(i = 0; i<5000; i++)
    {
        __asm__("nop");
    }
    _HAL_RX210_VOLTAGE_MONITORING_2_STATUS_REGISTER &= ~LVD2SR_LVD2DET_MASK;
    _HAL_RX210_VOLTAGE_MONITORING_2_CIRCUIT_COMPARATOR_REGISTER_0 |= LVD2CR0_LVD2RIE_MASK;	/* Interrupt Enable*/

    ENABLE_REGISTER_WRITE_PROTECTION;
}/*end LVDW_SetTrip*/

/*
 * Function: LVDR_GetTrip
 * Description:  Reads the LVD trip
 * Status:
 * Note:
 */
uint8_t LVDR_GetTrip(void)
{
    return((415 - (GET_L_NIBBLE(_HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER)*15))/10);
}/*end LVDR_GetTrip*/

/*
 * Function: LVDW_getTrip
 * Description: Reads the LVD warning trip
 * Note:
 * Status:
 */
uint8_t LVDW_GetTrip(void)
{
    return((415 - (GET_H_NIBBLE(_HAL_RX210_VOLTAGE_DETECTION_LEVEL_SELECT_REGISTER)*15))/10);
}/*end LDW_GetTrip*/

/* ********************************************************************************************** */
/*                                  END LVD Class HAL Support                                     */
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

/* Watchdog Control */
#define _HAL_RX210_WDT_CONTROL_REGISTER 				IWDT.IWDTCR.WORD
/* Watchdog Refresh Register */
#define _HAL_RX210_WDT_REFRESH_REGISTER 				IWDT.IWDTRR
/* Watchdog Status Register */
#define _HAL_RX210_WDT_STATUS_REGISTER 					IWDT.IWDTSR.WORD
/* Watchdog Reset Control Register */
#define _HAL_RX210_WDT_RESET_CONTROL_REGISTER 			IWDT.IWDTRCR.BYTE
/* Watchdog Count Stop Register */
#define _HAL_RX210_WDT_COUNT_STOP_REGISTER				IWDT.IWDTCSTPR.BYTE
/* Watchdog Option function select Register */
#define _HAL_RX210_WDT_OPTION_FUNCTION_SELECT_REGISTER 	EXSYSTEM.OFS0.LONG

/*
 * Macro: WDT_Enable
 * Note: There isn't this feature in RX210
 * Status:
 * Note:
	- AE: This functions do nothing. In RX200 Option register can be written only at start up
 */
#define WDT_Enable(setting)

/*
 * Macro: WDT_Disable
 * Note: There isn't this feature in RX210
 * - It's impossible disable IWDT in run time
 * Status: Not implemented must be set at StartUp
 */
#define WDT_Disable(setting)

/*
* Function: WDT_Enabled
* Description:  check if WatchDog is enabled.
*/
bool_t WDT_Enabled(void)
{
    return WDT_ENABLED; /*TODO verifica se � abilitato leggendo i registri*/
}

/*
 *  Macro: WDT_Refresh
 *  Description: The required refresh within 20 bus clock cycles.
 *  You must take care not only to refresh the watchdog within the watchdog timer's actual time-out
 *  period, but also provide enough allowance for the time it takes for the refresh sequence to be
 *  detected by the watchdog timer, on the watchdog clock
 *  (2/3 watchdog cycles for window mode, 1 watchdog cycle for no window mode)
 *  Note:
 *  Status:
 */
void WDT_Refresh(void)
{
    _HAL_RX210_WDT_REFRESH_REGISTER = 0x00;
    _HAL_RX210_WDT_REFRESH_REGISTER = 0xFF;
}

/*
Returns if WDT timeout has been occurred
This can be used after a reset to see if the IWDT caused the reset.
*/
bool_t WDT_TimeoutExpired(void)
{
	return IWDT.IWDTSR.BIT.UNDFF?TRUE:FALSE;
}
/*
Returns if WDT untimely refresh caused reset
This can be used after a reset to see if the IWDT caused the reset.
*/
bool_t WDT_UntimelyResetOccurred(void)
{
	return IWDT.IWDTSR.BIT.REFEF?TRUE:FALSE;
}
/*
clears timeout signal
This can be used after a reset to see if the IWDT caused the reset.
*/
void WDT_ClearTimeoutExpired(void)
{
	IWDT.IWDTSR.BIT.UNDFF=0;
}
/*
clears untimely refresh signal
This can be used after a reset to see if the IWDT caused the reset.
*/
void WDT_ClearUntimelyResetOccurred(void)
{
	IWDT.IWDTSR.BIT.REFEF=0;
}

/*
 * Macro: WDT_EnableInWait
 * Description: This macro do no stop the down counter of IWDT at transition to low-power-consumption mode
 * Note: Enable/Disable IWDT in power save mode is done by one register for all power save modality
 * Status: To Test!
 */
#define WDT_EnableInWait	{_HAL_RX210_WDT_COUNT_STOP_REGISTER &= 0x00;}

/*
 * Macro: WDT_DisableInWait
 * Description: This macro stop of down counter of IWDT at transition to lower-power-consumption mode
 * Note: Enable/Disable IWDT in power save mode is done by one register for all power save modality
 * Status: To Test!
 */
#define WDT_DisableInWait	{_HAL_RX210_WDT_COUNT_STOP_REGISTER |= 0x80;}

/*
 * Macro: WDT_EnableInStop
 * Description: This macro stop of down counter of IWDT at transition to lower-power-consumption mode
 * Note: Enable/Disable IWDT in power save mode is done by one register for all power save modality
 * Status: To Test!
 */
#define WDT_EnableInStop 	{_HAL_RX210_WDT_COUNT_STOP_REGISTER &= 0x00;}

/*
 * Macro: WDT_DisableInStop
 * Description: This macro stop of down counter of IWDT at transition to lower-power-consumption mode
 * Note: Enable/Disable IWDT in power save mode is done by one register for all power save modality
 * Status: To Test!
 */
#define WDT_DisableInStop	{_HAL_RX210_WDT_COUNT_STOP_REGISTER |= 0x80:}

/*
 * Macro: WDT_EnableIRQ
 * Note:
 * Status: To Test!
 */
#define WDT_EnableIRQ(setting) 		_HAL_RX210_WDT_RESET_CONTROL_REGISTER &= 0x7F

/*
 * Macro: WDT_DisableIRQ
 * Note:
 * Status: To Test!
 */
#define WDT_DisableIRQ(setting)	 	_HAL_RX210_WDT_RESET_CONTROL_REGISTER |= 0x80

/*
 * Macro: WDT_SetLPOClock
 * Description: This function enable the IWDT-Dedicated Low-Speed Clock Oscillator Control Register (ILOCOCR)
 * Note:
 * Status: Tested
 */
#define WDT_SetLPOClock		{RX210_WRITE_PROTEC_OFF;SYSTEM.ILOCOCR.BYTE &= 0x00;RX210_WRITE_PROTEC_ON;}

/*
 * Macro: WDT_SetTimeOut
 * Description:
 * Note: Ask to Indesit about the implementation of this Macro.
 * Status: can't update Timeout of IWDT in this way!
 */
#define WDT_SetTimeOut(time) { _HAL_RX210_WDT_CONTROL_REGISTER |= time;}

/*
* Function: WDT_AllowUpdate
* Description: true if it is possible to ....
*/
bool_t WDT_AllowUpdate(void)
{
    return TRUE; /* TODO: quando deve dare false?*/
}

/*
 * Function: Force Reset by WDT
 * Description:
 * Note:
 * 	- The down conter is only readable
 */
void WDT_ResetMCU(void)
{
    /*TODO Verify a way to do this*/
}

#ifdef HAL_WDT_SUPPORT


#if WDT_TIMEOUT==RX210_WDT_TIMEOUT_1024
#define RX210_WDT_REFRESH_PERMITTED_WINDOW	0x01FF
#elif WDT_TIMEOUT==RX210_WDT_TIMEOUT_4096
#define RX210_WDT_REFRESH_PERMITTED_WINDOW	0x07FF
#elif WDT_TIMEOUT==RX210_WDT_TIMEOUT_8192
#define RX210_WDT_REFRESH_PERMITTED_WINDOW	0x0FFF
#elif WDT_TIMEOUT==RX210_WDT_TIMEOUT_16384
#define RX210_WDT_REFRESH_PERMITTED_WINDOW	0x1FFF
#else
#error "WDT_TIMEOUT not supported"
#endif
/*
 * Macro: WDT_IsWindowOpen
 * Description: returns TRUE if in refresh permitted window
 *
 * Note: check table 27.4 at Pag 867
 * Precndition: refresh-permitted window is at the end of the timeout period and window is 50%
 * Status: To do!
 */
uint8_t WDT_IsWindowOpen(void)
{

#if WDT_WINDOWED_MODE==TRUE
    return (WDT_ReadCounter()<RX210_WDT_REFRESH_PERMITTED_WINDOW);
#else
    return TRUE;
#endif

}/*end WDT_IsWindowOpen*/


/*
 * Macro: WDT_IsJustBeforePermittefWindow
 * Description: returns TRUE if counter is close to refresh permitted window

 * Precndition: refresh-permitted window is at the end of the timeout period and window is 50%
 */
bool_t WDT_IsJustBeforePermittedWindow(void)
{
	uint32_t counter=WDT_ReadCounter();
	bool_t bRes=FALSE;
	if (!( counter < RX210_WDT_REFRESH_PERMITTED_WINDOW ) && (counter > (uint32_t)((RX210_WDT_REFRESH_PERMITTED_WINDOW*95)/100)))
	{
		bRes=TRUE;
	}
	return bRes;
}


uint8_t WDT_IsRunning(void)
{
	return (WDT_ReadCounter()!=0) ? TRUE : FALSE;
}

/*
 * Function: WDT_InitSync
 * Description:
 * 1- Enable WDT Register mdoe
 * 2- Set all option (Clock division rater, windows start and position, time out period and reset or interrupt request output.
 * 3- Finally refresh the down-counter to start counting down from the value in the time-out period selection
 * Note:
 * 	1- Evaluate Windows end position
 *  2- The IWDT CRT register is only once writable.
 * Status: Tested!
 */
void WDT_InitSync(void)
{

#if  (WDT_ENABLED == TRUE) && !defined(WDT_ENABLED_AT_POWERON)
    /* Control register in memory because it can only write once to the register after unlock */
    uint16_t setting = 0;

    /* 1. Enable/Disable WDT Register mode
     * These are dummy operation                */
    /*lint -e506 */
    if(WDT_ENABLED)
    {
        WDT_Enable(setting);
    }
    else
    {
        WDT_Disable(setting);
    }
    /*lint +e506 */
    /* 2. Enable Clock 125KHz Low speed dedicated clock */
    WDT_SetLPOClock;

    /* 3. Set WDT Parameter
     * 3.1 Set Clock Divisor                    */
    setting |= WDT_PRESCALER;
    /* 3.2 Set Window Start position            */
    setting |= (WDT_WINDOW_START | WDT_WINDOW_END);
    /* 3.3 WDT Set time-out value               */
    setting |= WDT_TIMEOUT;
    /* 2.4 Write IWDT CRT Register with setting */
    _HAL_RX210_WDT_CONTROL_REGISTER = setting;
    /* 2.5 Enable/Disable Interrupt             */
#if (WDT_INT_ENABLED)
    WDT_EnableIRQ(setting);	/* Enable Interrupt / Disable Reset */
#else
    WDT_DisableIRQ(setting);    /* Enable Reset / Disable Reset */
#endif
#endif /* WDT_ENABLED */
}/*end WDT_InitSync(void)*/


/*
 * Function: WDT_StartSync
 * Description: This function start the IWDT
 * Note: IWDT start in WDT_InitSynch, this function is not supported
 * Status: Check
 */

void WDT_StartSync(void)
{
	/*lint -save -e506 */
    if(WDT_ENABLED)
        WDT_Refresh();
    /*lint -restore */
}/*end WDT_StartSync*/

/*
 * Macro: WDT_SetWindowingSync
 * Description: This function sets the desired window
 * Note: This function is not supported
 * Status:
 * Note: The Rx210 has two windows, Start Window and End Window. With this macro it's enable only the start window
 * at value give by the time variable. By default the end window is set to 0% while the start window to 100%.
 */

void WDT_SetWindowingSync(WDT_windowType window)
{
    /*lint -e530*/ /*Warning non consistente*/
    _HAL_RX210_WDT_CONTROL_REGISTER |= (WDT_WINDOW_START | WDT_WINDOW_END);
    /*lint +e530*/
}/*end WDT_SetWindowingSync*/

/*
 * Macro: WDT_StopWindowingSync
 * Description:This function deactivates the windowing feature of watchdog. The function
				influences the windowing enable bit of the control register.
 * Note: This function is not supported
 * Status:
 */
static void WDT_StopWindowingSync(void)
{

}/*end WDT_StopWindowingSync*/

/*
 * Macro: WDT_ReadCounter
 * Description: This function reads the watchdog counter register.
 * Note:
 * Status: Tested
 * Date: 02/14/2012
 */
uint32_t WDT_ReadCounter(void)
{
    return (_HAL_RX210_WDT_STATUS_REGISTER & 0x3FFF);
}/*end WDT_ReadCounter*/

/*
 * Macro: WDT_GetWindow
 * Description: This function returns the watchdog window register
 * Note:
 * Status: Tested
 * Date: 02/14/2012
 * Version: 1.0.0
 */

uint32_t WDT_GetWindow(void)
{
    return ((_HAL_RX210_WDT_CONTROL_REGISTER & 0xFF00)>>8);
}/*end WDT_GetWindow*/

/*
 * Macro: WDT_ReadRSTCounter
 * Description: This function reads the watchdog reset counter register
 * Note: This function is not supported
 * Status:
 */

uint16_t WDT_ReadRSTCounter(void)
{
    return WDT_RSTCounter();
}/*end WDT_ReadRSTCounter*/

/*
 * Macro: WDT_ResetRSTCounter
 * Description:
 * Note: This function is not supported
 * Status:
 */

void WDT_ResetRSTCounter(void)
{
    WDT_ClearRSTCounter();
}/*end WDT_ResetRSTCounter*/


#endif /* HAL_WDT_SUPPORT */
/*@}*/

/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */





/* ********************************************************************************************** */
/*                                  BEGIN RESET Class HAL Support                                 */
/* ********************************************************************************************** */
/*
 * Function: SYS_GetResetSrcVal
 * Description:
 * Note:
 * Status:
 */
resetSource_t SYS_GetResetSrcVal(void)
{
    resetSource_t val = 0;
    uint8_t val_0 = _RX210_RESET_STATUS_REGISTER_0;
    uint8_t val_1 = _RX210_RESET_STATUS_REGISTER_1;
    uint8_t val_2 = _RX210_RESET_STATUS_REGISTER_2;

    _RX210_RESET_STATUS_REGISTER_0 = 0x00;
    _RX210_RESET_STATUS_REGISTER_1 = 0x00;
    _RX210_RESET_STATUS_REGISTER_2 = 0x00;
    val |= ((resetSource_t) val_2 <<16);
    val |= ((resetSource_t) val_1 <<8);
    val |= (resetSource_t) val_0;
    return val;
}/*end resetSource_t SYS_GETResetSrcVal*/

/*
 * Function: SYS_PowerOnReset
 * Description:
 * Note:
 * Status:
 */
bool_t SYS_PowerOnReset(resetSource_t rstSrc)
{
    if((rstSrc & POWER_ON_RESET_MASK))
        return  1;
    else
        return 0;
}/*end SYS_PowerOnReset*/

/*
 * Function: SYS_PowerOnReset
 * Description:
 * Note:
 * Status:
*/
bool_t SYS_WatchdogReset(resetSource_t rstSrc)
{
    if((rstSrc & IWDT_RESET_MASK))
        return  1;
    else
        return 0;
}/*end SYS_WatchdogReset*/

/*
* Function: SYS_ExternalPinReset
*/
bool_t SYS_ExternalPinReset(resetSource_t rstSrc)
{
    if((rstSrc & 0xFFFFFFFF) == 0x00000000)
        return  1;
    else
        return 0;
}/*end SYS_ExternalPinReset*/

/*
* Function: SYS_LossOfClockReset
*/
bool_t SYS_LossOfClockReset(resetSource_t rstSrc)
{
    return 0;
}/*end SYS_ExternalPinReset*/

/*
* Function: SYS_IllegalOpcodeReset
*/
bool_t SYS_IllegalOpcodeReset(resetSource_t rstSrc)
{
    return 0;
}/*end SYS_IllegalOpcodeReset*/

/*
* Function: SYS_LowVoltageReset
*/
bool_t SYS_LowVoltageReset(resetSource_t rstSrc)
{
    if((rstSrc & LOW_VOLTAGE_RESET_VD1))
        return  1;
    else
        return 0;
}/*end SYS_ExternalPinReset*/

/*
* Function: SYS_SetDebuggerReset
*/
bool_t SYS_DebuggerReset(resetSource_t rstSrc)
{
    return 0;
}/*end SYS_SetDebuggerReset*/

/*
* Function: SYS_IllegalAddressReset
*/
bool_t SYS_IllegalAddressReset(resetSource_t rstSrc)
{
    return 0;
}/*end SYS_IllegalAddressReset*/

/*
* Function: SYS_UserReset
*/
bool_t SYS_UserReset(resetSource_t rstSrc)
{
	bool_t retVal = 0;
    if((rstSrc & USER_RESET_MASK))
    	retVal = 1;
    return retVal;
}/*end SYS_UserReset*/

/* ********************************************************************************************** */
/*                                  END RESET Class HAL Support                                 */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */

/*! \fn CLK_EngageClkRefSync
 \brief Test if the cristal is ready
 \param  void
 \return void
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: "Date"
 *	- File: "File name"
 *	- Comment: Do not apply in RX210
 */
void CLK_EngageClkRefSync(void)
{
#if defined(CLK_EXT_REF_CLOCK_ACTIVE)
    /* Enable external clock source for the processor */
    CLK_EnableExternalRefClock();
#else

    /* Disable external clock source for the processor */
    CLK_DisableExternalRefClock();

#endif    /*CLK_EXT_REF_CLOCK_ACTIVE */

#if defined(CLK_INT_REF_CLOCK_ACTIVE)				/* Not in RX210*/
    /* Enable internal clock source for the processor */
    CLK_EnableInternalRefClock();
#else
    /* Disable internal clock source for the processor */
    CLK_DisableInternalRefClock();
#endif /*CLK_INT_REF_CLOCK_ACTIVE*/

}/* end function CLK_EngageClkRefSync*/

/*! \fn CLK_SetInternalClockSync(uint32_t desiredClock)
 \brief Sets up the system clock frequency as close to the desiredClock
 value as possible. The system will run on the internal
 reference clock in the FLL bypass mode.
 \param  Desidered Clock
 \return Bool value 1 is set, o no set
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: "Date"
 *	- File: "File name"
 *	- Comment:
 *		- It's the HOCO clock source
 *		- Test function
 */
#if defined(CONFIG_HAL_CLK_FIXED_20MHZ)
/**
 *
 */
bool_t CLK_SetInternalClockSync(uint32_t desiredClock)
{
#if !defined(WDT_ENABLED_AT_POWERON)

    unsigned char clock_hococr2;
    volatile unsigned char i = 0; /* MS forse non serve il ciclo di ritardo */
#if defined(CONFIG_PL_DEBUG)
    if(desiredClock != 20000000){
    	HAL_ErrorHandler(1);
    }
#endif

    clock_hococr2 = 0x02;

    /*2. Remove Write Protection*/
    RX210_WRITE_PROTEC_OFF;
    /*3. Define a waiting time*/
    _HAL_RX210_CLK_MOSCWTCR_REG = 0x0D; /* 131072 state, wait over 10ms*/
    /*4. Turn Off PLL*/
    CLK_TurnsPLLOff();
    /*5. Set Internal Clock Source*/
    CLK_SetInternalClockSource();
    /*6. Set Clock Speed*/
    _HAL_RX210_CLK_HOCOCR2_REG = clock_hococr2;				/*!< Set internal clock frequency*/
    /*7. Turn On Clock Internal*/
    _HAL_RX210_CLK_HOCOCR_REG = 0x00; /*b0 = 0 ON, b0 = 1 OFF*/

    for (i = 0; i < 5; i++)
    {
    }
    /*8. Define Clock Value(Frequency Divider)*/

    SYSTEM.SCKCR.LONG = 0x11011111; /* System Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/

    RX210_WRITE_PROTEC_ON;
#endif

    return (bool_t) 1;
} /*end function CLK_SetInternalClockSync*/
#define CLK_GetInternalClockSync() 	(20000000)

#else
uint32_t internalClockFreq __attribute__ ((section (".noinit")));
bool_t CLK_SetInternalClockSync(uint32_t desiredClock)
{
    unsigned char clock_configure;
    unsigned char clock_hococr2;
    unsigned char i = 0;

    /*1. Evaluate Possible clock*/
    if (desiredClock == 20000000)
    {
        clock_hococr2 = 0x02;
        clock_configure = 0x00;
    }
    else if (desiredClock <= 32000000)
    {
        clock_hococr2   = 0x00;
        clock_configure = 0x01;
    }
    else if ((desiredClock > 32000000) && (desiredClock <= 36864000))
    {
        clock_hococr2 = 0x01;
        clock_configure = 0x02;
    }
    else if ((desiredClock > 36864000) && (desiredClock <= 40000000))
    {
        clock_hococr2 = 0x02;
        clock_configure = 0x03;
    }
    else if (desiredClock > 40000000)
    {
        clock_hococr2 = 0x03;
        clock_configure = 0x04;
    }
    else
    {
        /*TODO: gestire*/
    }
    /*2. Remove Write Protection*/
    RX210_WRITE_PROTEC_OFF;
    /*3. Define a waiting time*/
    _HAL_RX210_CLK_MOSCWTCR_REG = 0x0D; /* 131072 state, wait over 10ms*/
    /*4. Turn Off PLL*/
    CLK_TurnsPLLOff();
    /*5. Set Internal Clock Source*/
    CLK_SetInternalClockSource();
    /*6. Set Clock Speed*/
    _HAL_RX210_CLK_HOCOCR2_REG = clock_hococr2;				/*!< Set internal clock frequency*/
    /*7. Turn On Clock Internal*/
    _HAL_RX210_CLK_HOCOCR_REG = 0x00; /*b0 = 0 ON, b0 = 1 OFF*/

    for (i = 0; i < 5; i++)
    {
    }
    /*8. Define Clock Value(Frequency Divider)*/

    switch (clock_configure)
    {
        /* Internal Clock 20MHZ*/
    case 0:
        /*PCLKD(b0-b3)=20Mhz,Reserved(b4-b7),PCLKB(b8-b11)=20MHz,Reserved(b12-15),BCLK(b16-b19)=20MHz,Reserved(b20-b22),(b23)BCLK PIN = 0, ICLK(b24-b27) = 20MHz,FCLK(b28-b31)=20MHz*/
        SYSTEM.SCKCR.LONG = 0x11011111; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
        internalClockFreq = 20000000;
        break;
        /* Internal Clock 32MHz*/
    case 1:
        /*PCLKD(b0-b3)=32Mhz,Reserved(b4-b7),PCLKB(b8-b11)=32MHz,Reserved(b12-15),BCLK(b16-b19)=16MHz,Reserved(b20-b22),(b23)BCLK PIN = 0, ICLK(b24-b27) = 32MHz,FCLK(b28-b31)=32MHz*/
        SYSTEM.SCKCR.LONG = 0x00011010; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
        internalClockFreq = 32000000;
        break;
        /* Internal Clock 36864 kHz*/
    case 2:
        /*PCLKD(b0-b3)=16Mhz,Reserved(b4-b7),PCLKB(b8-b11)=16MHz,Reserved(b12-15),BCLK(b16-b19)=16MHz,Reserved(b20-b22),(b23)BCLK PIN = 0, ICLK(b24-b27) = 36864kHz,FCLK(b28-b31)=16MHz*/
        SYSTEM.SCKCR.LONG = 0x10011111; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
        internalClockFreq = 36864000;
        break;
        /* Internal Clock 40MHz*/
    case 3:
        /*PCLKD(b0-b3)=20Mhz,Reserved(b4-b7),PCLKB(b8-b11)=20MHz,Reserved(b12-15),BCLK(b16-b19)=20MHz,Reserved(b20-b22),(b23)BCLK PIN = 0, ICLK(b24-b27) = 40MHz,FCLK(b28-b31)=20MHz*/
        SYSTEM.SCKCR.LONG = 0x10011111; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
        internalClockFreq = 40000000;
        break;
        /* Internal Clock 50MHz*/
    case 4:
        /*PCLKD(b0-b3)=25Mhz,Reserved(b4-b7),PCLKB(b8-b11)=25MHz,Reserved(b12-15),BCLK(b16-b19)=25MHz,Reserved(b20-b22),(b23)BCLK PIN = 0, ICLK(b24-b27) = 50MHz,FCLK(b28-b31)=25MHz*/
        SYSTEM.SCKCR.LONG = 0x10011111; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
        internalClockFreq = 50000000;
        break;
    }

    RX210_WRITE_PROTEC_ON;

    return (bool_t) 1;
} /*end function CLK_SetInternalClockSync*/

uint32_t CLK_GetInternalClockSync(void)
{
    return internalClockFreq;
}
#endif
/*! \fn CLK_SetExternalClockSync(uint32_t desiredClock)
 \brief Sets up the system clock frequency as close to the desiredClock
 value as possible. The system will run on the external
 reference clock in the FLL bypass mode
 \param  Desidered Clock 32 bit value in Hz
 \return Bool value 1 is set, o no set
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: 04/11/2011
 *	- File: "File name"
 *	- Comment: This Function Set only the ICLK. The other clock (PCKD,PCLKB,BCLK,FCLK) are set at max frequency in function of desired frequency
 */
bool_t CLK_SetExternalClockSync(uint32_t desiredClock)
{
#if defined (CLK_EXT_REF_CLOCK_ACTIVE)
    int32_t frequency, frequency0;
    int32_t difference, difference0;
    int32_t divider_temp = 0x0000000000000000;
    uint8_t i;

    /* 1 Calculate the Main Clock divider Parameter */
    i = 0;
    frequency0 = (int32_t) CLK_EXT_CLOCK_FREQ; /* !< Assign the EXT Clock value defined in RN_RX210_R5F2108_hal.h */

    do
    {
        frequency = frequency0 >> i;
        i++;
    }
    while ((i < 255) && !(frequency <= desiredClock));

    /* Difference between the desired and possible frequency */
    difference = desiredClock - frequency;

    /* Makes absolute value */
    if (difference < 0)
    {
        difference = -difference;
    }

    /* Difference between the desired and possible frequency from the previous step */
    difference0 = desiredClock - frequency0;

    /* Makes absolute value */
    if (difference0 < 0)
    {
        difference0 = -difference0;
    }

    /* If the previous step frequncy was closer to the disered, changes the i */
    if (difference0 < difference)
    {
        i--;
    }/*end*/
    /*2. Remove Write Protection*/
    RX210_WRITE_PROTEC_OFF;
    /*3. Define a waiting time*/
    _HAL_RX210_CLK_MOSCWTCR_REG = 0x0D; /* 131072 state, wait over 10ms */
    /*4. Turn Off PLL*/
    CLK_TurnsPLLOff();
    /* 5. Set Clock Source to External */
    CLK_SetExternalClockSource();
    /* 6. Set Divider */
    divider_temp = (divider_temp | i);
    divider_temp = divider_temp << 24; /* Dal primo byte all'ultimo byte*/
    _HAL_RX210_CLK_SCKCR_REG = divider_temp;
    /*. Enable Write Protect*/
    RX210_WRITE_PROTEC_ON;

    return (bool_t) 1;
#else
    return (bool_t)0;
#endif
} /*end function CLK_SetExternalClockSync*/

/*! \fn CLK_SetTrimSync(uint16_t desiredTrim)
 \brief Sets up the trimming value. The function feeds both
 the trimming register and the fine trimming bit.
 \param
 \return
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: "Date"
 *	- File: "File name"
 *	- Comment:
 */
void CLK_SetTrimSync(uint16_t desiredTrim)
{
    /*CLK_SetTrim((uint8_t)(desiredTrim));*/

}/*end function CLK_SetTrimSync*/

/*! \fn CLK_SetFLLExternalClockSync(uint32_t desiredClock)
 \brief
 \param
 \return
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: "Date"
 *	- File: "File name"
 *	- Note: Considered a Cristall of 20MHz
 *	- Comment: This function Set Clock System with PLL and External Oscillator. With this function is set only the ICLK clock.
 *	The other clock((PCKD,PCLKB,BCLK,FCLK) are set at max admission frequency in function of PLL Setting.
 *		- PLL at Max Frequency 100MHz with 20MHz EXT CLK
 *		- PCLKD: PLL_OUT/4
 *		- PCLKB: PLL_OUT/4
 *		- BCLK: PLL_OUT/4
 *		- FCLK: PLL_OUT/4
 *		- FIXED EXT OSCILLATOR TO 20MHz (Take information about parametric version)
 */
bool_t CLK_SetFLLExternalClockSync(uint32_t desiredClock)
{
#if defined (CLK_EXT_REF_CLOCK_ACTIVE)
    int32_t reference;
    unsigned char difference = 0;
    int32_t i = 0;

    reference = (int32_t) CLK_EXT_CLOCK_FREQ;

    if (CLK_EXT_CLOCK_FREQ != 20000000)
    {
        return (bool_t) 0; /* !< The CKL Value is fixed with at 20MHz (Verificare) */
    }

    /*1. Remove Write Protection*/
    RX210_WRITE_PROTEC_OFF; /* Disable Protection*/
    /* Main Clock Oscillator Wait Control Register*/
    SYSTEM.MOSCWTCR.BYTE = 0x0D; /* 131072 state, wait over 10ms*/
    /* PLL Wait Control Register*/
    SYSTEM.PLLWTCR.BYTE = 0x0E; /* 2097152 state, wait over 12ms  @PLL=100MHz(20MHz/2*10) */
    /* Config PLL Control Register*/
    SYSTEM.PLLCR.WORD = 0x0901; /* ((ExternalClock x 10)/2) @PLL Frequency*/
    /* Main Clock Oscillator Control Register*/
    /* Main Clock Oscillator Control Register*/
    SYSTEM.MOSCCR.BYTE = 0x02; /* External oscillation input selection, Disable Internal Oscillator*/
    /* PLL Control Register 2 (if 1 stop, if 0 operating)*/
    SYSTEM.PLLCR2.BYTE = 0x00; /* Enable PLL*/
    for (i = 0; i < 0x168; i++)
        ; /* wait over 12ms*/
    /* System Clock Control Register*/
    SYSTEM.SCKCR.LONG = 0x21022222; /* Syestem Clock ICK=PLL/2,FCK and PCK and BCL=PLL/4*/
    /* PLL source clock*/
    SYSTEM.SCKCR3.WORD = 0x400; /* Select PLL like clock source*/
    /**/
    RX210_WRITE_PROTEC_ON; /* Enable Protection*/

    return (bool_t) 1;
#endif
    return (bool_t) 0;
}/* end function CLK_SetFLLExternalClockSync*/

/*! \fn CLK_SetFLLExternalClockSync(uint32_t desiredClock)
 \brief Sets up the system clock frequency as close to the desiredClock
 value as possible. The system will run on with PLL on the internal
 reference clock.
 \param
 \return
 * \section Informazioni
 *	- Company: SPES scpa
 *	- Author: Andrea Emili
 *	- Date: "Date"
 *	- File: "File name"
 *	- Comment:
 *		- On Rx210 PLL multiply only EXT oscillator Frequency
 */
bool_t CLK_SetFLLInternalClockSync(uint32_t desiredClock)
{
    return (bool_t) 0;
} /* end function CLK_SetFLLInternalClockSync*/

/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */

void DeInitGPIO(void)
{
    /*!< Reset all IO Port Init direction*/
    PORT1_PDR = 0x00;
    PORT2_PDR = 0x00;
    PORT3_PDR = 0x00;
    PORT4_PDR = 0x00;
    PORT5_PDR = 0x00;
    PORTA_PDR = 0x00;
    PORTB_PDR = 0x00;
    PORTC_PDR = 0x00;
    PORTE_PDR = 0x00;
    PORTH_PDR = 0x00;
    /*!< Reset Pull Up port to Init Value*/
    PORT1_PCR = 0x00;
    PORT2_PCR = 0x00;
    PORT3_PCR = 0x00;
    PORT4_PCR = 0x00;
    PORT5_PCR = 0x00;
    PORTA_PCR = 0x00;
    PORTB_PCR = 0x00;
    PORTC_PCR = 0x00;
    PORTE_PCR = 0x00;
    PORTH_PCR = 0x00;
}/*  De InitiGPIO */


bool_t DIO_IsNotDummyIO(const IO_ModuleIOMapType* ptr)
{
    return (bool_t) ( ((ptr->PortAddress.basic) != (DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR) && ((ptr->PinMask) != 0))?1:0;
}


#define DIGITAL_FILTER_MAP_SIZE 24
static const IO_DIGITAL_NOISE_MapType const NoiseFilterPinMap[DIGITAL_FILTER_MAP_SIZE]=
{
    /* MTUO */
    IO_DIGITAL_FILTER_PIN(B, 3, 0x01, CH_TGRA, DIGITAL_FILTER_MTU0),	/* 1 */
#if defined(HAL_CH_TGRB0_ON_PA1)
    IO_DIGITAL_FILTER_PIN(A, 1, 0x01, CH_TGRB, DIGITAL_FILTER_MTU0),	/* 2 */
#else
    IO_DIGITAL_FILTER_PIN(1, 5, 0x01, CH_TGRB, DIGITAL_FILTER_MTU0),	/* 2 */
#endif
#if defined(HAL_CH_TGRC0_ON_PB1)
    IO_DIGITAL_FILTER_PIN(B, 1, 0x01, CH_TGRC, DIGITAL_FILTER_MTU0),	/* 3 */
#else
    IO_DIGITAL_FILTER_PIN(3, 2, 0x01, CH_TGRC, DIGITAL_FILTER_MTU0),	/* 3 */
#endif
    IO_DIGITAL_FILTER_PIN(A, 3, 0x01, CH_TGRD, DIGITAL_FILTER_MTU0),	/* 4 */
    IO_DIGITAL_FILTER_PIN(E, 4, 0x02, CH_TGRA, DIGITAL_FILTER_MTU1),	/* 5 */
    IO_DIGITAL_FILTER_PIN(B, 5, 0x02, CH_TGRB, DIGITAL_FILTER_MTU1),	/* 6 */
#if defined(HAL_CH_TGRA2_ON_PB5)
    IO_DIGITAL_FILTER_PIN(B, 5, 0x01, CH_TGRA, DIGITAL_FILTER_MTU2),	/* 7 */
#else
    IO_DIGITAL_FILTER_PIN(2, 6, 0x01, CH_TGRA, DIGITAL_FILTER_MTU2),	/* 7 */
#endif
#if defined(HAL_CH_TGRB2_ON_PE5)
    IO_DIGITAL_FILTER_PIN(E, 5, 0x02, CH_TGRB, DIGITAL_FILTER_MTU2),	/* 8 */
#else
    IO_DIGITAL_FILTER_PIN(2, 7, 0x01, CH_TGRB, DIGITAL_FILTER_MTU2),	/* 8 */
#endif
#if defined(HAL_CH_TGRA3_ON_P17)
    IO_DIGITAL_FILTER_PIN(1, 7, 0x01, CH_TGRA, DIGITAL_FILTER_MTU3),	/* 9 */
#elif defined(HAL_CH_TGRA3_ON_PC7)
    IO_DIGITAL_FILTER_PIN(C, 7, 0x01, CH_TGRA, DIGITAL_FILTER_MTU3),	/* 9 */

#else
    IO_DIGITAL_FILTER_PIN(1, 4, 0x01, CH_TGRA, DIGITAL_FILTER_MTU3),	/* 9 */
#endif

#if defined(HAL_CH_TGRB3_ON_PB7)
    IO_DIGITAL_FILTER_PIN(B, 7, 1, CH_TGRB, DIGITAL_FILTER_MTU3),	/* 10 */
#elif defined(HAL_CH_TGRB3_ON_P17)
    IO_DIGITAL_FILTER_PIN(1, 7, 0x02, CH_TGRB, DIGITAL_FILTER_MTU3),	/* 10 */
#else
    IO_DIGITAL_FILTER_PIN(C, 5, 0x01, CH_TGRB, DIGITAL_FILTER_MTU3),	/* 10 */
#endif

#if defined(HAL_CH_TGRC3_ON_P16)
    IO_DIGITAL_FILTER_PIN(1, 6, 0x01, CH_TGRC, DIGITAL_FILTER_MTU3),	/* 11 */
#else
    IO_DIGITAL_FILTER_PIN(C, 6, 0x01, CH_TGRC, DIGITAL_FILTER_MTU3),	/* 11 */
#endif

#if defined(HAL_CH_TGRD3_ON_P16)
    IO_DIGITAL_FILTER_PIN(1, 6, 0x02, CH_TGRD, DIGITAL_FILTER_MTU3),	/* 12 */
#elif defined(HAL_CH_TGRD3_ON_PC4)
    IO_DIGITAL_FILTER_PIN(C, 4, 0x01, CH_TGRD, DIGITAL_FILTER_MTU3),	/* 12 */
#else
    IO_DIGITAL_FILTER_PIN(B, 6, 0x01, CH_TGRD, DIGITAL_FILTER_MTU3),	/* 12 */
#endif

#if defined(HAL_CH_TGRA4_ON_PB3)
    IO_DIGITAL_FILTER_PIN(B, 3, 0x02, CH_TGRA, DIGITAL_FILTER_MTU4),	/* 13 */
#elif defined(HAL_CH_TGRA4_ON_PE2)
    IO_DIGITAL_FILTER_PIN(E, 2, 0x02, CH_TGRA, DIGITAL_FILTER_MTU4),	/* 13 */
#else
    IO_DIGITAL_FILTER_PIN(A, 0, 0x01, CH_TGRA, DIGITAL_FILTER_MTU4),	/* 13 */
#endif

#if defined(HAL_CH_TGRB4_ON_P30)
    IO_DIGITAL_FILTER_PIN(3, 0, 0x01, CH_TGRB, DIGITAL_FILTER_MTU4),	/* 14 */
#elif defined(HAL_CH_TGRB4_ON_P54)
    IO_DIGITAL_FILTER_PIN(5, 4, 0x01, CH_TGRB, DIGITAL_FILTER_MTU4),	/* 14 */
#elif defined(HAL_CH_TGRB4_ON_PE3)
    IO_DIGITAL_FILTER_PIN(E, 3, 0x01, CH_TGRB, DIGITAL_FILTER_MTU4),	/* 14 */
#else
    IO_DIGITAL_FILTER_PIN(C, 2, 0x01, CH_TGRB, DIGITAL_FILTER_MTU4),	/* 14 */
#endif

#if defined(HAL_CH_TGRC4_ON_PE1)
    IO_DIGITAL_FILTER_PIN(E, 1, 0x01, CH_TGRC, DIGITAL_FILTER_MTU4),	/* 15 */
#elif defined(HAL_CH_TGRC4_ON_PE5)
    IO_DIGITAL_FILTER_PIN(E, 5, 0x01, CH_TGRC, DIGITAL_FILTER_MTU4),	/* 15 */
#else
    IO_DIGITAL_FILTER_PIN(B, 1, 0x02, CH_TGRC, DIGITAL_FILTER_MTU4),	/* 15 */
#endif

#if defined(HAL_CH_TGRD4_ON_P55)
    IO_DIGITAL_FILTER_PIN(5, 5, 0x01, CH_TGRD, DIGITAL_FILTER_MTU4),	/* 16 */
#elif defined(HAL_CH_TGRD4_ON_PC3)
    IO_DIGITAL_FILTER_PIN(C, 3, 0x01, CH_TGRD, DIGITAL_FILTER_MTU4),	/* 16 */
#elif defined(HAL_CH_TGRD4_ON_PE4)
    IO_DIGITAL_FILTER_PIN(E, 4, 0x01, CH_TGRD, DIGITAL_FILTER_MTU4),	/* 16 */
#else
    IO_DIGITAL_FILTER_PIN(3, 1, 0x01, CH_TGRD, DIGITAL_FILTER_MTU4),	/* 16 */
#endif

    IO_DIGITAL_FILTER_PIN(3, 0, 0x40, 0, DIGITAL_FILTER_IRQ0),				/* 17 */
    IO_DIGITAL_FILTER_PIN(H, 1, 0x40, 0, DIGITAL_FILTER_IRQ0),				/* 18 */
    IO_DIGITAL_FILTER_PIN(3, 1, 0x40, 0, DIGITAL_FILTER_IRQ1),				/* 19 */
    IO_DIGITAL_FILTER_PIN(H, 2, 0x40, 0, DIGITAL_FILTER_IRQ1),				/* 20 */
    IO_DIGITAL_FILTER_PIN(1, 5, 0x40, 0, DIGITAL_FILTER_IRQ5),				/* 21 */
    IO_DIGITAL_FILTER_PIN(E, 5, 0x40, 0, DIGITAL_FILTER_IRQ5),				/* 22 */
    IO_DIGITAL_FILTER_PIN(A, 4, 0x40, 0, DIGITAL_FILTER_IRQ5),				/* 23 */
    IO_DIGITAL_FILTER_PIN(A, 3, 0x40, 0, DIGITAL_FILTER_IRQ6),				/* 24 */
};

unsigned char DIO_CheckDigitalFilterPin(const IO_ModuleIOMapType* pMap)
{
    uint8_t i = 0;
    uint8_t pin_found = 0xFF;

    DIO_ModuleMapPortAddressType portAddress = pMap->PortAddress;
    DIO_ModuleMapMaskType pinMask = pMap->PinMask;

    for (i = 0; i < DIGITAL_FILTER_MAP_SIZE; i++)
    {
    	/* Find pin from digital pin list */
        if ((NoiseFilterPinMap[i].PortAddress == portAddress.basic) &&
        		(NoiseFilterPinMap[i].PinMask == pinMask))
        {
        	uint8_t tmpMPCMask = (NoiseFilterPinMap[i].MPCFuncVal <= 0x0f ? 0x0f : 0xf0);

        	/* Check if pin found is enabled to its function */
            if ( (*NoiseFilterPinMap[i].MPCAddress & tmpMPCMask) == NoiseFilterPinMap[i].MPCFuncVal)
            {
                pin_found = i;
                break;
            }
        }
    }
    return pin_found;
}/* end DIO_CheckDigitalFilterPin */
/*
 * Date: 06/18/2012
 * Function: DIO_IsDigitalFilterAvailable(IO_ModuleIOMapType* pMap)
 */
unsigned char DIO_IsDigitalFilterAvailable(const IO_ModuleIOMapType* pMap)
{
    uint8_t pin_found = FALSE;
    pin_found = DIO_CheckDigitalFilterPin(pMap);
    if(pin_found != 0xFF)
        pin_found = TRUE;
    else
        pin_found = FALSE;
    return pin_found;
}

/*
 * Function: DIO_DigitalFilterEnable
 * Note:
 * 	- To write MTU2a Noise filter register you must wake up the MTU2a periphera from low power mode
 * 	- IRQ Pin PFS register must be written before to use this function
 * 	- The check of which pin functionalities are enabled is done only in MTU2a shared pins.
 */
void DIO_DigitalFilterEnable(const IO_ModuleIOMapType* pMap)
{
    uint8_t j;
    uint8_t pin_mask;
    j = DIO_CheckDigitalFilterPin(pMap);

    if (j < DIGITAL_FILTER_MAP_SIZE)
    {
        pin_mask = NoiseFilterPinMap[j].channel;

        switch(NoiseFilterPinMap[j].digital_filter_peripheral)
        {
        /* Timers confugration */
        case DIGITAL_FILTER_MTU0:
            MTU0.NFCR.BYTE |= (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU1:
            MTU1.NFCR.BYTE |= (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU2:
            MTU2.NFCR.BYTE |= (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU3:
            MTU3.NFCR.BYTE |= (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU4:
            MTU4.NFCR.BYTE |= (1 << pin_mask);
            break;
		/* IRQ pin confugration */
        case DIGITAL_FILTER_IRQ0:
            ICU.IRQFLTE0.BIT.FLTEN0 = 0x01;
            break;
        case DIGITAL_FILTER_IRQ1:
            ICU.IRQFLTE0.BIT.FLTEN1 = 0x01;
            break;
        case DIGITAL_FILTER_IRQ5:
            ICU.IRQFLTE0.BIT.FLTEN5 = 0x01;
            break;
        case DIGITAL_FILTER_IRQ6:
            ICU.IRQFLTE0.BIT.FLTEN6 = 0x01;
            break;
        default:
        	break;
        }/* end switch */
    }
}/* end DIO_DigitalFilterEnable */

/*
 * Function: DIO_SetDigitalFltClkFreq
 */
void DIO_DigitalFilterClkFrequencySet(const IO_ModuleIOMapType* pMap, uint16_t filterFrequency)
{
    uint8_t j;
    uint8_t divisor = 0;
    uint16_t frequency_khz = (uint16_t) (CLK_GetInternalClockSync() / 1000);

    j = DIO_CheckDigitalFilterPin(pMap);

    if (j < DIGITAL_FILTER_MAP_SIZE)
    {
        if (filterFrequency <= (frequency_khz / 64))
        {
        	if (NoiseFilterPinMap[j].digital_filter_peripheral > DIGITAL_FILTER_MTU4)
        	{
            	divisor = 0x03;
        	}
        	else
        	{
        		/* Timers maximum divisor index is 2 */
            	divisor = 0x02;
        	}
        }
        else if (filterFrequency <= (frequency_khz / 32))
        {
        	divisor = 0x02;
        }
        else if (filterFrequency <= (frequency_khz / 8))
        {
        	divisor = 0x01;
        }
        else
        {
        	divisor = 0;
        }

        /* Setting of noise filter */
        switch(NoiseFilterPinMap[j].digital_filter_peripheral)
        {
        /* Timers confugration */
        case DIGITAL_FILTER_MTU0:
            MTU0.NFCR.BIT.NFCS = divisor;
            break;
        case DIGITAL_FILTER_MTU1:
            MTU1.NFCR.BIT.NFCS = divisor;
            break;
        case DIGITAL_FILTER_MTU2:
            MTU2.NFCR.BIT.NFCS = divisor;
            break;
        case DIGITAL_FILTER_MTU3:
            MTU3.NFCR.BIT.NFCS = divisor;
            break;
        case DIGITAL_FILTER_MTU4:
            MTU4.NFCR.BIT.NFCS = divisor;
            break;

		/* IRQ pin confugration */
		case DIGITAL_FILTER_IRQ0:
            ICU.IRQFLTC0.BIT.FCLKSEL0 = divisor;
			break;
		case DIGITAL_FILTER_IRQ1:
            ICU.IRQFLTC0.BIT.FCLKSEL1 = divisor;
			break;
		case DIGITAL_FILTER_IRQ5:
            ICU.IRQFLTC0.BIT.FCLKSEL5 = divisor;
			break;
		case DIGITAL_FILTER_IRQ6:
            ICU.IRQFLTC0.BIT.FCLKSEL6 = divisor;
			break;
		default:
			break;

        }/* end switch */
    }

}/* end DIO_SetDigitalFltClkFreq */

/*
 * Function: DIO_DigitalFilterClkFrequencyGet
 */
uint16_t DIO_DigitalFilterClkFrequencyGet(const IO_ModuleIOMapType* pMap)
{
    uint8_t j;
    uint8_t divisor=0xff;
    uint16_t frequency_khz = (uint16_t) CLK_GetInternalClockSync() / 1000;

    j = DIO_CheckDigitalFilterPin(pMap);

    if (j < DIGITAL_FILTER_MAP_SIZE)
    {
        switch(NoiseFilterPinMap[j].digital_filter_peripheral)
        {
        /* Timers confugration */
        case DIGITAL_FILTER_MTU0:
            divisor = MTU0.NFCR.BIT.NFCS;
            break;
        case DIGITAL_FILTER_MTU1:
            divisor = MTU1.NFCR.BIT.NFCS;
            break;
        case DIGITAL_FILTER_MTU2:
            divisor = MTU2.NFCR.BIT.NFCS;
            break;
        case DIGITAL_FILTER_MTU3:
            divisor = MTU3.NFCR.BIT.NFCS;
            break;
        case DIGITAL_FILTER_MTU4:
            divisor = MTU4.NFCR.BIT.NFCS;
            break;


		/* IRQ pin confugration */
		case DIGITAL_FILTER_IRQ0:
            divisor = ICU.IRQFLTC0.BIT.FCLKSEL0;
			break;
		case DIGITAL_FILTER_IRQ1:
            divisor = ICU.IRQFLTC0.BIT.FCLKSEL1;
			break;
		case DIGITAL_FILTER_IRQ5:
            divisor = ICU.IRQFLTC0.BIT.FCLKSEL5;
			break;
		case DIGITAL_FILTER_IRQ6:
            divisor = ICU.IRQFLTC0.BIT.FCLKSEL6;
			break;
		default:
			break;

        }/* end switch */

        switch(divisor)
        {
        case 0:
        	break;
        case 1:
        	frequency_khz = frequency_khz / 8;
        	break;
        case 2:
        	frequency_khz = frequency_khz / 32;
        	break;
        case 3:
        	frequency_khz = frequency_khz / 64;
        	break;
        default:
        	frequency_khz = 0;
        	break;
        }

    }
    return frequency_khz;

}/* end DIO_DigitalFilterClkFrequencyGet */

/*
 * Function: DIO_DigitalFilterDisable
 */
void DIO_DigitalFilterDisable(const IO_ModuleIOMapType* pMap)
{
    uint8_t j;
    /* uint8_t pin_mask; */
    j = DIO_CheckDigitalFilterPin(pMap);
    /* Check for IRQ functionality */

    if (j < DIGITAL_FILTER_MAP_SIZE)
    {
        switch(NoiseFilterPinMap[j].digital_filter_peripheral)
        {
        /* Timers confugration */
        case DIGITAL_FILTER_MTU0:
            MTU0.NFCR.BYTE = 0x00;
            break;
        case DIGITAL_FILTER_MTU1:
            MTU1.NFCR.BYTE = 0x00;
            break;
        case DIGITAL_FILTER_MTU2:
            MTU2.NFCR.BYTE = 0x00;
            break;
        case DIGITAL_FILTER_MTU3:
            MTU3.NFCR.BYTE = 0x00;
            break;
        case DIGITAL_FILTER_MTU4:
            MTU4.NFCR.BYTE = 0x00;
            break;

		/* IRQ pin confugration */
		case DIGITAL_FILTER_IRQ0:
            ICU.IRQFLTE0.BIT.FLTEN0 = 0x00;
			break;
		case DIGITAL_FILTER_IRQ1:
            ICU.IRQFLTE0.BIT.FLTEN1 = 0x00;
			break;
		case DIGITAL_FILTER_IRQ5:
            ICU.IRQFLTE0.BIT.FLTEN5 = 0x00;
			break;
		case DIGITAL_FILTER_IRQ6:
            ICU.IRQFLTE0.BIT.FLTEN6 = 0x00;
			break;
		default:
			break;

        }/* end switch */
    }
}/* end DIO_DigitalFilterDisable */

/*
 * Function: DIO_IsDigitalFilterEnabled
 */
unsigned char DIO_IsDigitalFilterEnabled(const IO_ModuleIOMapType* pMap)
{
    uint8_t j;
    uint8_t pin_mask;
    uint8_t is_digital_filter_enabled = FALSE;
    j = DIO_CheckDigitalFilterPin(pMap);
    /* Check for IRQ functionality */

    if (j < DIGITAL_FILTER_MAP_SIZE)
    {
        pin_mask = NoiseFilterPinMap[j].channel;
        /* Setting of noise filter */
        switch(NoiseFilterPinMap[j].digital_filter_peripheral)
        {
        /* Timers confugration */
        case DIGITAL_FILTER_MTU0:
            is_digital_filter_enabled = MTU0.NFCR.BYTE & (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU1:
            is_digital_filter_enabled = MTU1.NFCR.BYTE & (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU2:
            is_digital_filter_enabled = MTU2.NFCR.BYTE & (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU3:
            is_digital_filter_enabled = MTU3.NFCR.BYTE & (1 << pin_mask);
            break;
        case DIGITAL_FILTER_MTU4:
            is_digital_filter_enabled = MTU4.NFCR.BYTE & (1 << pin_mask);
            break;

		/* IRQ pin confugration */
		case DIGITAL_FILTER_IRQ0:
            is_digital_filter_enabled = (ICU.IRQFLTE0.BYTE & 0x01);
			break;
		case DIGITAL_FILTER_IRQ1:
            is_digital_filter_enabled = (ICU.IRQFLTE0.BYTE & 0x02);
			break;
		case DIGITAL_FILTER_IRQ5:
            is_digital_filter_enabled = (ICU.IRQFLTE0.BYTE & 0x20);
			break;
		case DIGITAL_FILTER_IRQ6:
            is_digital_filter_enabled = (ICU.IRQFLTE0.BYTE & 0x40);
			break;
		default:
			break;

        }/* end switch */
    }

    return is_digital_filter_enabled;

}/* end DIO_IsDigitalFilterEnabled */

/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class HAL Support                                   */
/* ********************************************************************************************** */

/*MTUO Interrupt vector define*/
#define MTU0_BASE_INT_VECTOR 		114
#define MTU0_INT_ENABLE_REGISTER 	0x0E
#define MTU0_INTERRUPT_PRIORITY 	0x01
#define TGIA0_INT_SHIF_VECTOR		0
#define TGIB0_INT_SHIF_VECTOR		1
#define TGIC0_INT_SHIF_VECTOR		2
#define TGID0_INT_SHIF_VECTOR		3
#define TGIV0_INT_SHIF_VECTOR		4
#define TGIE0_INT_SHIF_VECTOR		5
#define TGIF0_INT_SHIF_VECTOR		6
/*MTU1 Interrupt vector define*/
#define MTU1_BASE_INT_VECTOR 		121
#define MTU1_INT_ENABLE_REGISTER 	0x0F
#define MTU1_INTERRUPT_PRIORITY 	0x01
#define TGIA1_INT_SHIF_VECTOR		0
#define TGIB1_INT_SHIF_VECTOR		1
#define TGIV1_INT_SHIF_VECTOR		2
#define TGIU1_INT_SHIF_VECTOR		3
/*MTU2 Interrupt vector define*/
#define MTU2_BASE_INT_VECTOR 		125
#define MTU2_INT_ENABLE_REGISTER 	0x0F
#define MTU2_INTERRUPT_PRIORITY 	0x01
#define TGIA2_INT_SHIF_VECTOR		0
#define TGIB2_INT_SHIF_VECTOR		1
#define TGIV2_INT_SHIF_VECTOR		2
#define TGIU2_INT_SHIF_VECTOR		3
/*MTU3 Interrupt vector define*/
#define MTU3_BASE_INT_VECTOR 		129
#define MTU3_INT_ENABLE_REGISTER 	0x10
#define MTU3_INTERRUPT_PRIORITY 	0x01
#define TGIA3_INT_SHIF_VECTOR		0
#define TGIB3_INT_SHIF_VECTOR		1
#define TGIC3_INT_SHIF_VECTOR		2
#define TGID3_INT_SHIF_VECTOR		3
#define TGIV3_INT_SHIF_VECTOR		3
/*MTU4 Interrupt vector define*/
#define MTU4_BASE_INT_VECTOR 		134
#define MTU4_INT_ENABLE_REGISTER 	0x10
#define MTU4_INTERRUPT_PRIORITY 	0x01
#define TGIA4_INT_SHIF_VECTOR		0
#define TGIB4_INT_SHIF_VECTOR		1
#define TGIC4_INT_SHIF_VECTOR		2
#define TGID4_INT_SHIF_VECTOR		3
#define TGIV4_INT_SHIF_VECTOR		3


/** end Timer Interrupt define **/

/* Define Reset source in TRC and in TMR_CLEAR_SOURCE_IS_INTERNAL macro*/
#define TCNT_RESET_SOURCE_OVERFLOW	0x00
#define TCNT_RESET_SOURCE_TGRA  	0x01
#define TCNT_RESET_SOURCE_TGRB  	0x02
#define TCNT_RESET_SOURCE_TGRC  	0x05
#define TCNT_RESET_SOURCE_TGRD  	0x06



/*
 * RX200 MTUx Timer INTERRUPT DEFINE
 * TIER REGISTER: COUNTER INTERRUPT ENABLE/DISABLE
 */
#define TGIA_INTERRUPT_ENABLE		0x01		/* TIER Register b0*/
#define TGIB_INTERRUPT_ENABLE		0x02		/* TIER Register b1*/
#define TGIC_INTERRUPT_ENABLE		0x04		/* TIER Register b2*/
#define TGID_INTERRUPT_ENABLE		0x08		/* TIER Register b3*/
#define OVERFLOW_ISR_ENABLE			0x10		/* TIER Register b4*/
#define UNDERFLOW_ISR_ENABLE		0x20		/* TIER Register b5*/

/*
 * RX200 MTUxA Timer PWM DEFINE
 */
#define RX_PWM_MODE_1	0x02	/* PWM mode in TMDR register*/


/*
 * Variable: TimerClockGate
 * Description: Contains the counter mask for power enable register
 * Note:
 */
static const uint32_t const TimerClockGate[HAL_NUM_HW_CNTR] =
{
    ENABLE_MTU2A_MASK,
    ENABLE_MTU2A_MASK,
    ENABLE_MTU2A_MASK,
    ENABLE_MTU2A_MASK,
    ENABLE_MTU2A_MASK,
};/*end const*/

/*MS ottimizzazione
 */
uint8_t* const ChModeRegAddr[] =
{
    /* MTU0 ------------------*/
    (uint8_t*)&MTU0.TIORH.BYTE, /* HAL_CH_TGRA0 MTU0 IOA*/
    (uint8_t*)&MTU0.TIORH.BYTE, /* HAL_CH_TGRB0 MTU0 IOB*/
    (uint8_t*)&MTU0.TIORL.BYTE, /* HAL_CH_TGRC0 MTU0 IOC*/
    (uint8_t*)&MTU0.TIORL.BYTE, /* HAL_CH_TGRD0 MTU0 IOD*/
    (uint8_t*)0,					/* HAL_CH_TGRV0	 overfow MTU0*/
    (uint8_t*)0,					/* HAL_CH_TGRE0	*/
    (uint8_t*)0,					/* HAL_CH_TGRF0 */

    /* MTU1 ------------------*/
    (uint8_t*)&MTU1.TIOR.BYTE,   	/* HAL_CH_TGRA1 MTU1 IOA*/
    (uint8_t*)&MTU1.TIOR.BYTE,   	/* HAL_CH_TGRB1 MTU1 IOB*/
    (uint8_t*)0,					/* HAL_CH_TGRV1	overfow MTU1*/
    (uint8_t*)0,					/* HAL_CH_TGRU1 */
    /* MTU2 ------------------*/
    (uint8_t*)&MTU2.TIOR.BYTE,   /* HAL_CH_TGRA2 MTU2 IOA*/
    (uint8_t*)&MTU2.TIOR.BYTE,   /* HAL_CH_TGRB2 MTU2 IOB*/
    (uint8_t*)0,					/* HAL_CH_TGRV2 */
    (uint8_t*)0,					/* HAL_CH_TGRU2 */

    (uint8_t*)&MTU3.TIORH.BYTE,  	/* HAL_CH_TGRA3 MTU3 IOA*/
    (uint8_t*)&MTU3.TIORH.BYTE,	/* HAL_CH_TGRB3 MTU3 IOB*/
    (uint8_t*)&MTU3.TIORL.BYTE,  	/* HAL_CH_TGRC3 MTU3 IOC*/
    (uint8_t*)&MTU3.TIORL.BYTE,	/* HAL_CH_TGRD3 MTU3 IOD*/
    (uint8_t*)0, /* HAL_CH_TGRV3	 overfow MTU3*/

    (uint8_t*)&MTU4.TIORH.BYTE,  /* HAL_CH_TGRA4 MTU4 IOA*/
    (uint8_t*)&MTU4.TIORH.BYTE,/* HAL_CH_TGRB4 MTU4 IOB*/
    (uint8_t*)&MTU4.TIORL.BYTE,  /* HAL_CH_TGRC4 MTU4 IOC*/
    (uint8_t*)&MTU4.TIORL.BYTE,/* HAL_CH_TGRD4 MTU4 IOD*/
    (uint8_t*)0,				/* HAL_CH_TGRV4 overfow MTU4*/
};

const uint32_t ChModeRegAddr_Nibble = 0xA5110A; /* 101001010001000100001010b each bit means if the mode is in highNibble or register */

uint16_t* const ChTGRAAddr[] =
{
    /* MTU0 ------------------*/
    (uint16_t*)&MTU0_TGRA, /* HAL_CH_TGRA0 MTU0 TGRA*/
    (uint16_t*)&MTU0_TGRB, /* HAL_CH_TGRB0 MTU0 TRGB*/
    (uint16_t*)&MTU0_TGRC, /* HAL_CH_TGRC0 MTU0 TGRC*/
    (uint16_t*)&MTU0_TGRD, /* HAL_CH_TGRD0 MTU0 TGRD*/
    (uint16_t*)0,			/* HAL_CH_TGRV0	 overfow TGRV*/
    (uint16_t*)&MTU0_TGRE, /* HAL_CH_TGRE0	*/
    (uint16_t*)&MUT0_TGRF,	/* HAL_CH_TGRF0 */

    /* MTU1 ------------------*/
    (uint16_t*)&MTU1_TGRA, /* HAL_CH_TGRA1 MTU1 TRGA*/
    (uint16_t*)&MTU1_TGRB, /* HAL_CH_TGRB1 MTU1 TGRB*/
    (uint16_t*)0,			/* HAL_CH_TGRV1	overfow MTU1*/
    (uint16_t*)0,			/* HAL_CH_TGRU1 */
    /* MTU2 ------------------*/
    (uint16_t*)&MTU2_TGRA, /* HAL_CH_TGRA2 MTU2 TRGA*/
    (uint16_t*)&MTU2_TGRB, /* HAL_CH_TGRB2 MTU2 TGRB*/
    (uint16_t*)0,			/* HAL_CH_TGRV2 */
    (uint16_t*)0,			/* HAL_CH_TGRU2 */
    /* MTU3 ------------------*/
    (uint16_t*)&MTU3_TGRA,	/* HAL_CH_TGRA3 MTU3 TGRA*/
    (uint16_t*)&MTU3_TGRB,	/* HAL_CH_TGRB3 MTU3 TGRB*/
    (uint16_t*)&MTU3_TGRC,	/* HAL_CH_TGRC3 MTU3 TGRC*/
    (uint16_t*)&MTU3_TGRD,	/* HAL_CH_TGRD3 MTU3 TGRD*/
    (uint16_t*)0, 			/* HAL_CH_TGRV3	overfow MTU3*/
    /* MTU4 ------------------*/
    (uint16_t*)&MTU4_TGRA, /* HAL_CH_TGRA4 MTU4 IOA*/
    (uint16_t*)&MTU4_TGRB, /* HAL_CH_TGRB4 MTU4 IOB*/
    (uint16_t*)&MTU4_TGRC, /* HAL_CH_TGRC4 MTU4 IOC*/
    (uint16_t*)&MTU4_TGRD, /* HAL_CH_TGRD4 MTU4 IOD*/
    (uint16_t*)0,				/* HAL_CH_TGRV4 overfow MTU4*/
};



static uint32_t TMR_GetMultiBmp(uint32_t uBmp);

uint8_t MapClearResourceValue(uint8_t clrSrcId)
{
    uint8_t retValue=0;
    switch(clrSrcId)
    {
    case TCNT_CLEARING_TGRA:
        retValue = TCNT_RESET_SOURCE_TGRA;
        break;
    case TCNT_CLEARING_TGRB:
        retValue = TCNT_RESET_SOURCE_TGRB;
        break;
    case TCNT_CLEARING_TGRC:
        retValue = TCNT_RESET_SOURCE_TGRC;
        break;
    case TCNT_CLEARING_TGRD:
        retValue = TCNT_RESET_SOURCE_TGRD;
        break;
    case TCNT_CLEARING_OVERFLOW:
        retValue = TCNT_RESET_SOURCE_OVERFLOW;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return retValue;
}

uint8_t MapClearResourceId(uint8_t clrValue)
{
    uint8_t retValue=0;
    switch(clrValue)
    {
    case TCNT_RESET_SOURCE_TGRA:
        retValue = TCNT_CLEARING_TGRA;
        break;
    case TCNT_RESET_SOURCE_TGRB:
        retValue = TCNT_CLEARING_TGRB;
        break;
    case TCNT_RESET_SOURCE_TGRC:
        retValue = TCNT_CLEARING_TGRC;
        break;
    case TCNT_RESET_SOURCE_TGRD:
        retValue = TCNT_CLEARING_TGRD;
        break;
    case TCNT_RESET_SOURCE_OVERFLOW:
        retValue = TCNT_CLEARING_OVERFLOW;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return retValue;
}


static void SetInternalHWClearSourceId(uint8_t tmrId, uint8_t clrSrcId)
{
    uint8_t clrValue;
    clrValue = MapClearResourceValue(clrSrcId);

    switch(tmrId)
    {
    case HAL_TMR_MTU0:
        MTU0.TCR.BIT.CCLR = clrValue;
        break;
    case HAL_TMR_MTU1:
        MTU1.TCR.BIT.CCLR = clrValue;
        break;
    case HAL_TMR_MTU2:
        MTU2.TCR.BIT.CCLR = clrValue;
        break;
    case HAL_TMR_MTU3:
        MTU3.TCR.BIT.CCLR = clrValue;
        break;
    case HAL_TMR_MTU4:
        MTU4.TCR.BIT.CCLR = clrValue;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
}

static uint8_t GetInternalHWClearSourceValue(uint8_t tmrId)
{
    uint8_t clrValue = 0;

    switch(tmrId)
    {
    case HAL_TMR_MTU0:
        clrValue = MTU0.TCR.BIT.CCLR;
        break;
    case HAL_TMR_MTU1:
        clrValue = MTU1.TCR.BIT.CCLR;
        break;
    case HAL_TMR_MTU2:
        clrValue = MTU2.TCR.BIT.CCLR;
        break;
    case HAL_TMR_MTU3:
        clrValue = MTU3.TCR.BIT.CCLR;
        break;
    case HAL_TMR_MTU4:
        clrValue = MTU4.TCR.BIT.CCLR;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return clrValue;
}/*end*/

uint8_t TMR_GetClearSourceId(uint8_t tmrId)
{
    uint8_t clrValue;

    clrValue = GetInternalHWClearSourceValue(tmrId);

    return	MapClearResourceId(clrValue);
}

/*
 * Function: Threshold2SetUpUnsupported
 * Description:
 * Note:
 * Status: Check if this MCU support Threshold2 config
 */

bool_t Threshold2SetUpUnsupported(uint8_t channel)
{
    return (TRUE);
}/*end Thereshold2SetUpUnsupported*/


/*
 * Function: CheckClearSourceIdFailed
 * Description: this function fails if the clock source for modulo (clear source) is correct
 * Note:
 * Status:
 */

bool_t TMR_CheckClearSourceIdFailed(TMR_CNTAddressType tmrID, uint8_t clrSrcId)
{
    bool_t retVal=1; /* fail*/
    if(  clrSrcId == TCNT_CLEARING_OVERFLOW )
    {
        return 0; /* not fail*/
    }
    switch (tmrID)
    {
    case HAL_TMR_MTU0:
        if (   (clrSrcId == TCNT_CLEARING_TGRA)
                || (clrSrcId == TCNT_CLEARING_TGRB)
                || (clrSrcId == TCNT_CLEARING_TGRC)
                || (clrSrcId == TCNT_CLEARING_TGRD))
        {
            retVal = 0;
        }
        break;
    case HAL_TMR_MTU1:
        if((clrSrcId == TCNT_CLEARING_TGRA)
                || (clrSrcId == TCNT_CLEARING_TGRB))
        {
            retVal = 0;
        }
        break;
    case HAL_TMR_MTU2:
        if(		(clrSrcId == TCNT_CLEARING_TGRA)
                ||  (clrSrcId == TCNT_CLEARING_TGRB))
        {
            retVal = 0;
        }
        break;
    case HAL_TMR_MTU3:
        if(    (clrSrcId == TCNT_CLEARING_TGRA)
                || (clrSrcId == TCNT_CLEARING_TGRB)
                || (clrSrcId == TCNT_CLEARING_TGRC)
                || (clrSrcId == TCNT_CLEARING_TGRD))
        {
            retVal = 0;
        }
        break;
    case HAL_TMR_MTU4:
        if(    (clrSrcId == TCNT_CLEARING_TGRA)
                || (clrSrcId == TCNT_CLEARING_TGRB)
                || (clrSrcId == TCNT_CLEARING_TGRC)
                || (clrSrcId == TCNT_CLEARING_TGRD))
        {
            retVal = 0;
        }
        break;
    default:
        retVal = 1;
        break;
    }/*end switch*/

    return retVal;
}/*end TMR_CheckClearSourceIdFailed*/

/*
 * Function: TMR_GetClearSourceCh
 * Description: this function return the chid related to the clearsourceid of a timer, -1 if no channel
 * Note:
 * Status:
 */
TMR_CHAddressType TMR_GetClearSourceCh(TMR_CNTAddressType tmrID, uint8_t clrSrcId)
{
    TMR_CHAddressType chId=NO_CHANNEL_ID; /* no ch source*/
    if(  clrSrcId == TCNT_CLEARING_OVERFLOW )
    {
        return NO_CHANNEL_ID; /* no ch source*/
    }
    switch (tmrID)
    {
    case HAL_TMR_MTU0:
    {
        switch(clrSrcId)
        {
        case TCNT_CLEARING_TGRA:
            chId =	HAL_CH_TGRA0;
            break;
        case TCNT_CLEARING_TGRB:
            chId =	HAL_CH_TGRB0;
            break;
        case TCNT_CLEARING_TGRC:
            chId =	HAL_CH_TGRC0;
            break;
        case TCNT_CLEARING_TGRD:
            chId =	HAL_CH_TGRD0;
            break;
        }
    }
    break;
    case HAL_TMR_MTU1:
    {
        switch(clrSrcId)
        {
        case TCNT_CLEARING_TGRA:
            chId =	HAL_CH_TGRA1;
            break;
        case TCNT_CLEARING_TGRB:
            chId =	HAL_CH_TGRB1;
            break;
        }
    }
    break;
    case HAL_TMR_MTU2:
    {
        switch(clrSrcId)
        {
        case TCNT_CLEARING_TGRA:
            chId =	HAL_CH_TGRA2;
            break;
        case TCNT_CLEARING_TGRB:
            chId =	HAL_CH_TGRB2;
            break;
        }
    }
    break;
    case HAL_TMR_MTU3:
    {
        switch(clrSrcId)
        {
        case TCNT_CLEARING_TGRA:
            chId =	HAL_CH_TGRA3;
            break;
        case TCNT_CLEARING_TGRB:
            chId =	HAL_CH_TGRB3;
            break;
        case TCNT_CLEARING_TGRC:
            chId =	HAL_CH_TGRC3;
            break;
        case TCNT_CLEARING_TGRD:
            chId =	HAL_CH_TGRD3;
            break;
        }
    }
    break;
    case HAL_TMR_MTU4:
    {
        switch(clrSrcId)
        {
        case TCNT_CLEARING_TGRA:
            chId =	HAL_CH_TGRA4;
            break;
        case TCNT_CLEARING_TGRB:
            chId =	HAL_CH_TGRB4;
            break;
        case TCNT_CLEARING_TGRC:
            chId =	HAL_CH_TGRC4;
            break;
        case TCNT_CLEARING_TGRD:
            chId =	HAL_CH_TGRD4;
            break;
        }
    }
    break;
    default:

        break;
    }/*end switch*/
    return chId;
}/*end TMR_CheckClearSourceIdFailed*/



/*
 * Function: TMT_ClockGate
 * Author: Andrea Emili, SPES scpa
 * Description: Enable timer module Power Up and route clock to timer module enabled
 * TODO: Function, Test, inside critical section
 * Note: GP clock gating deve essere fatto all'interno di una critical section */
/* invece exit critical viene chiamata prima di TMR_ClockGate */
/* true means the timer gets clocked
 */

void TMR_ClockGate(TMR_CNTAddressType hwCntrlId, bool_t gate)
{
    /* uint32_t scg1Mask; */
    if( hwCntrlId >= NUM_HW_CNTR)
    {
        HAL_ErrorHandler(0);
    }
    /* scg1Mask = TimerClockGate[hwCntrlId];*/ /* Select the timer module to enable*/

    DISABLE_REGISTER_WRITE_PROTECTION;
    if (TRUE == gate)
    {
        SYSTEM.MSTPCRA.BIT.MSTPA9 = 0; /* Power up module*/
    }
    else
    {
        SYSTEM.MSTPCRA.BIT.MSTPA9 = 1; /* Power up module*/
    }
    ENABLE_REGISTER_WRITE_PROTECTION;

}/*end TMT_ClockGate function*/


const uint32_t prescalerDivMap[8]= { 1, 4,16, 64, 256, 1024, 1024, 1024};
/*
 * Function: TMR_SetPrescaler
 * Description:
 */
void TMR_SetPrescaler(TMR_CNTAddressType tmrID, uint8_t prescaler)
{
    switch(tmrID)
    {
    case HAL_TMR_MTU0:
        MTU0.TCR.BIT.TPSC = prescaler;
        break;
    case HAL_TMR_MTU1:
        MTU1.TCR.BIT.TPSC = prescaler;
        break;
    case HAL_TMR_MTU2:
        MTU2.TCR.BIT.TPSC = prescaler;
        break;
    case HAL_TMR_MTU3:
        MTU3.TCR.BIT.TPSC = prescaler;
        break;
    case HAL_TMR_MTU4:
        MTU4.TCR.BIT.TPSC = prescaler;
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }
}/*end TMR_SetPrescale*/

/* ritorna il divisore del prescaler del timer*/
uint32_t TMR_GetPrescalerDiv(TMR_CNTAddressType tmrID )
{
    uint32_t retVal=0;

    switch(tmrID)
    {
    case HAL_TMR_MTU0:
        retVal = prescalerDivMap[MTU0.TCR.BIT.TPSC];
        break;
    case HAL_TMR_MTU1:
        retVal = prescalerDivMap[MTU1.TCR.BIT.TPSC];
        break;
    case HAL_TMR_MTU2:
        retVal = prescalerDivMap[MTU2.TCR.BIT.TPSC];
        break;
    case HAL_TMR_MTU3:
        retVal = prescalerDivMap[MTU3.TCR.BIT.TPSC];
        break;
    case HAL_TMR_MTU4:
        retVal = prescalerDivMap[MTU4.TCR.BIT.TPSC];
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }
    return retVal;
}

/*  ritorna la tick resolution in base alla sorgente del timer ed al prescaler*/
uint32_t TMR_GetTickResolution( TMR_CNTAddressType tmrId )
{
    uint32_t retVal;
    retVal = 1000000000/(CLK_GetInternalClockSync()/TMR_GetPrescalerDiv(tmrId));
    return retVal;
}
/*MS TODO: imposta il prescaler in base alla sorgente del timer*/
uint32_t TMR_SetSetPrescaleForTickResolution(TMR_CNTAddressType tmrId, uint32_t tickRes)
{
    return 0;
}

/*
 * Function: TMR_SetPrescalerDfltValue
 * Description: Set Prescaler to the default value
 * Note: Reset value for prescaler is 0. So TCR bo-b3 clear to 0
 * Status: Test me!
 */
void TMR_SetPrescalerDfltValue(TMR_CNTAddressType tmrID)
{
    switch(tmrID)
    {
    case HAL_TMR_MTU0:
        MTU0.TCR.BYTE &= _PR_DEFAULT;
        break;
    case HAL_TMR_MTU1:
        MTU1.TCR.BYTE &= _PR_DEFAULT;
        break;
    case HAL_TMR_MTU2:
        MTU2.TCR.BYTE &= _PR_DEFAULT;
        break;
    case HAL_TMR_MTU3:
        MTU3.TCR.BYTE &= _PR_DEFAULT;
        break;
    case HAL_TMR_MTU4:
        MTU4.TCR.BYTE &= _PR_DEFAULT;
        break;
    default:
        HAL_ErrorHandler(1);
    }
}/* end TMR_SetPrescalerDfltValue*/



/*
 * Function: TMR_SetCountDir
 * Description:
 * Note: No selection of counter direction in RX200
 * Status: To check!
 * TODO: Check with Renesas the possibility to set this feature
 */
void TMR_SetCountDir( TMR_CNTAddressType pTmr_cnt, uint8_t direction )
{
    if( direction != 1) /* only TMR_COUNT_UP ia allowed*/
        HAL_ErrorHandler(1);
}

/*
 * Macro: TMR_SetCountDirDfltValue
 * Description:
 * Note: Only up counter direction
 */
void TMR_SetCountDirDfltValue(TMR_CNTAddressType tmrId)
{
    /* DO NOTHING: only TMR_COUNT_UP ia allowed*/
}
/*
 * Function: TMR_SetModulo
 * Description: this function configures the modulo value for the specific channel
 * 				it is being called by both OCHWCNTR_InitSync and by OCPWM_SetPeriodSync
 * 				the modulo period must be consistent in the two configurations
 * 				in case the configuration is being changed by the PWM setup, it will overwrite the period settings
 * within the hardware timer configuration done by OCHWCNTR_InitSync
 * Status: Test me!
 */
void TMR_SetModulo(TMR_CNTAddressType tmrID, Period_ValueType period, uint8_t clrSrcId)
{
    SetInternalHWClearSourceId( tmrID, clrSrcId);

    if (clrSrcId == TCNT_CLEARING_OVERFLOW && period != 0xFFFF )
    {
        HAL_ErrorHandler(1);
    }

    switch (tmrID)
    {
    case HAL_TMR_MTU0:
        if (clrSrcId == TCNT_CLEARING_TGRA) /* Set MTU0 compare value in TGRx*/
            MTU0.TGRA = period;
        else if (clrSrcId == TCNT_CLEARING_TGRB)
            MTU0.TGRB = period;
        else if (clrSrcId == TCNT_CLEARING_TGRC)
            MTU0.TGRC = period;
        else if (clrSrcId == TCNT_CLEARING_TGRD)
            MTU0.TGRD = period;
        else if (clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            /* nothing to do */
        }
        else
            HAL_ErrorHandler(1);
        break;
    case HAL_TMR_MTU1:
        if (clrSrcId == TCNT_CLEARING_TGRA) /* Set MTU0 compare value in TGRx*/
            MTU1.TGRA = period;
        else if (clrSrcId == TCNT_CLEARING_TGRB)
            MTU1.TGRB = period;
        else if (clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            /* nothing to do */
        }
        else
            HAL_ErrorHandler(1);
        break;
    case HAL_TMR_MTU2:
        if (clrSrcId == TCNT_CLEARING_TGRA) /* Set MTU0 compare value in TGRx*/
            MTU2.TGRA = period;
        else if (clrSrcId == TCNT_CLEARING_TGRB)
            MTU2.TGRB = period;
        else if (clrSrcId == TCNT_CLEARING_OVERFLOW)
		{
            /* nothing to do */
        }
        else
            HAL_ErrorHandler(1);
        break;
    case HAL_TMR_MTU3:
        if (clrSrcId == TCNT_CLEARING_TGRA) /* Set MTU0 compare value in TGRx*/
            MTU3.TGRA = period;
        else if (clrSrcId == TCNT_CLEARING_TGRB)
            MTU3.TGRB = period;
        else if (clrSrcId == TCNT_CLEARING_TGRC)
            MTU3.TGRC = period;
        else if (clrSrcId == TCNT_CLEARING_TGRD)
            MTU3.TGRD = period;
        else if (clrSrcId == TCNT_CLEARING_OVERFLOW)
		{
            /* nothing to do */
        }
        else
            HAL_ErrorHandler(1);
        break;
    case HAL_TMR_MTU4:
        if (clrSrcId == TCNT_CLEARING_TGRA) /* Set MTU0 compare value in TGRx*/
            MTU4.TGRA = period;
        else if (clrSrcId == TCNT_CLEARING_TGRB)
            MTU4.TGRB = period;
        else if (clrSrcId == TCNT_CLEARING_TGRC)
            MTU4.TGRC = period;
        else if (clrSrcId == TCNT_CLEARING_TGRD)
            MTU4.TGRD = period;
        else if (clrSrcId == TCNT_CLEARING_OVERFLOW)
		{
            /* nothing to do */
        }
        else
            HAL_ErrorHandler(1);
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/
}/*end TMR_SetModulo*/

/*
 * Function: TMR_StopClockSource
 * Description: Disable Clock source, TSTR Register Start/Stop timer counter
 * Note:
 * Status: Test me!
 */
void TMR_StopClockSource( TMR_CNTAddressType tmrID)
{
    if(tmrID >= HAL_NUM_HW_CNTR)	/* Check for errors*/
    {
        HAL_ErrorHandler(1);
    }
    if(tmrID > 2)				/* TSTR Reg*/
    {
        tmrID += 3;
    }

    CLEAR_BIT(MTU.TSTR.BYTE, tmrID);
}

/*
 * Function: TMR_EnableTI
 * Description: This function enables the modulo channel interrupt
 * there is no dedicated overflow register so one of the match channels is used
 */
void TMR_EnableTI( TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId)
{
    switch(tmrID)
    {
    case HAL_TMR_MTU0:/* MTU0*/
        if (clrSrcId == TCNT_CLEARING_OVERFLOW) /* Set Interrupt priority*/
        {
            ICU.IPR[MTU0_BASE_INT_VECTOR + clrSrcId].BYTE = MTU0_INTERRUPT_PRIORITY;
        }
        else
        {
            ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = MTU0_INTERRUPT_PRIORITY;
        }

        ICU.IR[MTU0_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00; /* Clear interrupt flag*/
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE |= (1 << (2 + clrSrcId)); /* Enable Interrupt for selected peripheral*/

        SET_BIT(MTU0.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU1:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW) /* Set Interrupt priority*/
        {
            ICU.IPR[MTU1_BASE_INT_VECTOR + 2].BYTE = MTU1_INTERRUPT_PRIORITY;
            ICU.IR[MTU1_BASE_INT_VECTOR + 2].BYTE = 0x00;
            ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE |= (1 << (clrSrcId - 1));
        }
        else
        {
            ICU.IPR[MTU1_BASE_INT_VECTOR].BYTE = MTU1_INTERRUPT_PRIORITY;
            ICU.IR[MTU1_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
            ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE |= (1 << (clrSrcId +1));
        }

        SET_BIT(MTU1.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU2:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)/* Set Interrupt priority*/
        {
            ICU.IPR[MTU2_BASE_INT_VECTOR + 2].BYTE = MTU2_INTERRUPT_PRIORITY;
            ICU.IR[MTU2_BASE_INT_VECTOR + 2].BYTE = 0x00;
            ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE |= (1 << (clrSrcId + 3));
        }
        else
        {
            ICU.IPR[MTU2_BASE_INT_VECTOR].BYTE = MTU2_INTERRUPT_PRIORITY;
            ICU.IR[MTU2_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
            ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE |= (1 << (clrSrcId + 5));
        }

        SET_BIT(MTU2.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU3:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)/* Set Interrupt priority*/
            ICU.IPR[MTU3_BASE_INT_VECTOR + clrSrcId].BYTE = MTU3_INTERRUPT_PRIORITY;
        else
            ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = MTU3_INTERRUPT_PRIORITY;

        ICU.IR[MTU3_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00; /* Clear interrupt flag*/
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= (1 << (1 + clrSrcId));

        SET_BIT(MTU3.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU4:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)/* Set Interrupt priority*/
            ICU.IPR[MTU4_BASE_INT_VECTOR + clrSrcId].BYTE = MTU4_INTERRUPT_PRIORITY;
        else
            ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = MTU4_INTERRUPT_PRIORITY;

        ICU.IR[MTU3_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00; /* Clear interrupt flag*/

        if (clrSrcId > TCNT_CLEARING_TGRC)
            ICU.IER[MTU3_INT_ENABLE_REGISTER + 1].BYTE |= (1 << (clrSrcId - 2));
        else
            /*TGRA and TGRB*/
            ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= (1 << (clrSrcId + 6));


        SET_BIT(MTU4.TIER.BYTE, clrSrcId);
        break;
    default:
        HAL_ErrorHandler(1);
    }
}/*end TMR_EnableTI*/

/*
 * Function: TMR_DisableTI
 * Description: This function is used to disable the overflow interrupt.
 * There is no dedicated overflow register so one the match channels is used.
 * Note:
 * Status: Test me!
 */
void TMR_DisableTI(TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId)
{
    switch(tmrID)
    {
    case HAL_TMR_MTU0:
        /* Disable Interrupt IER*/
        CLEAR_BIT( ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE ,(2+clrSrcId));
        CLEAR_BIT(MTU0.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU1:
        if(clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            CLEAR_BIT( ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE ,(clrSrcId-1));
        }
        else
        {
            CLEAR_BIT(ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE, (clrSrcId + 1));	/*AE(04122012)*/
        }
        CLEAR_BIT(MTU1.TIER.BYTE, clrSrcId);

        break;
    case HAL_TMR_MTU2:
        if(clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            CLEAR_BIT(ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE,  (clrSrcId+3));
        }
        else
        {
            CLEAR_BIT(ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE, (clrSrcId+5));
        }
        CLEAR_BIT(MTU2.TIER.BYTE, clrSrcId);

        break;
    case HAL_TMR_MTU3:
        CLEAR_BIT(ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE, (1+clrSrcId));
        CLEAR_BIT(MTU3.TIER.BYTE, clrSrcId);
        break;
    case HAL_TMR_MTU4:
        if(clrSrcId > TCNT_CLEARING_TGRC)
        {
            CLEAR_BIT(ICU.IER[MTU3_INT_ENABLE_REGISTER+1].BYTE, (clrSrcId-2));
        }
        else
        {
            CLEAR_BIT(ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE, (clrSrcId+6));
        }
            CLEAR_BIT(MTU4.TIER.BYTE, clrSrcId);
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }
}/* end TMR_DisableTI*/


/*
 * Function: TMR_ClearTOF
 * Description: This function clears the interrupt flag for the module channel.
 * There is no dedicated overflow register so one of the match channels is used.
 * Note:
 * 1.
 * 	 1- Disable TMR Interrupt
 *   2- Clear INT Flag
 *   3- Enable Interrupt
 * 2.
 *  Disable interrupt because in Test the IR flag isn't cleared otherwise.
 * Status: Test me
 */

void TMR_ClearTOF( TMR_CNTAddressType tmrId, TMR_CHAddressType clrSrcId)
{
    /*MS clrSrcId = TMR_GetClearSourceId(tmrId); non necessario */
    switch ( tmrId)
    {
    case HW_TMR_ID_0:
        ICU.IR[MTU0_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00; /* Clear interrupt flag*/
        break;
    case HW_TMR_ID_1:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)/* Set Interrupt priority*/
        {
            ICU.IR[MTU1_BASE_INT_VECTOR + 2].BYTE = 0x00;
        }
        else
        {
            ICU.IR[MTU1_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
        }
        break;
    case HW_TMR_ID_2:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)/* Set Interrupt priority*/
        {
            ICU.IR[MTU2_BASE_INT_VECTOR + 2].BYTE = 0x00;
        }
        else
        {
            ICU.IR[MTU2_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
        }
        break;
    case HW_TMR_ID_3:
        ICU.IR[MTU3_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
        break;
    case HW_TMR_ID_4:
        ICU.IR[MTU4_BASE_INT_VECTOR + clrSrcId].BYTE = 0x00;
        break;
    }/*end switch*/

}/* end TMR_ClearTOF*/

/*
 * Function: TMR_GetTOF
 * Description: Check if IR request is set and return with 1 is set or with 0 is not set
 * Note:
 * Status: Test me!
 */

bool_t TMR_GetTOF( TMR_CNTAddressType pTmr_cnt, TMR_CHAddressType channel)
{
    uint8_t clrSrcId;
    clrSrcId = TMR_GetClearSourceId( pTmr_cnt);

    switch (pTmr_cnt)
    {
    case HW_TMR_ID_0:
        if ((ICU.IR[MTU0_BASE_INT_VECTOR + clrSrcId].BYTE & 0x01) == 0x01)
            return 1;
        break;
    case HW_TMR_ID_1:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            if ((ICU.IR[MTU1_BASE_INT_VECTOR + 2].BYTE & 0x01) == 0x01)
                return 1;
        }/*end if*/
        else
        {
            if ((ICU.IR[MTU1_BASE_INT_VECTOR + clrSrcId].BYTE & 0x01) == 0x01)
                return 1;
        }/*end else*/
        break;
    case HW_TMR_ID_2:
        if (clrSrcId == TCNT_CLEARING_OVERFLOW)
        {
            if ((ICU.IR[MTU2_BASE_INT_VECTOR + 2].BYTE & 0x01) == 0x01)
                return 1;
        }
        else
        {
            if ((ICU.IR[MTU2_BASE_INT_VECTOR + clrSrcId].BYTE & 0x01) == 0x01)
                return 1;
        }
        break;
    case HW_TMR_ID_3:
        if ((ICU.IR[MTU3_BASE_INT_VECTOR + clrSrcId].BYTE & 0x01) == 0x01)
            return 1;
        break;
    case HW_TMR_ID_4:
        if ((ICU.IR[MTU4_BASE_INT_VECTOR + clrSrcId].BYTE & 0x01) == 0x01)
            return 1;
        break;
    }/*end switch*/
    /* get the bit in the IR with appropriate mask */
    return 0;
}/* end TMR_GETTOF*/

/*
 * Function: TMR_SetChannelInputChapture
 * Description: Set a channel in Input Capture Mode
 * Note: ICU can have one or two pin configurations.
 * Status: Test me!
 */
void TMR_SetChannelInputChapture( TMR_CHAddressType chId)
{
    volatile unsigned char *p1=0;
    volatile unsigned char *p2=0;
    volatile unsigned char *p3=0;
    unsigned char value=0;
    unsigned char bitNum=0;

    switch (chId)
    {
        /*MTU0*/
    case HAL_CH_TGRA0:/* (MTUO, MTI0C0A), (PB3,Pin36)*/
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 3;
        p3 = &	MPC.PB3PFS.BYTE;
        value = 0x01;
        break;

    case HAL_CH_TGRB0:/* (MTUO, MTI0C0B), (P15,Pin19), (PA1,Pin44)*/
#if defined(HAL_CH_TGRB0_ON_PA1)
        p1 = & PORTA.PDR.BYTE;
        p2 = & PORTA.PMR.BYTE;
        bitNum = 1;
        p3 = &	MPC.PA1PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.P15PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRC0:/*((MTUO, MTIOCOC),(P32,Pin12), (PB1,Pin37)*/
#if defined(HAL_CH_TGRC0_ON_PB1)
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 1;
        p3 = &	MPC.PB1PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORT3.PDR.BYTE;
        p2 = & PORT3.PMR.BYTE;
        bitNum = 2;
        p3 = &	MPC.P32PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRD0: /*((MTUO, MTIOCOD),(PA3,Pin43)*/
        p1 = & PORTA.PDR.BYTE;
        p2 = & PORTA.PMR.BYTE;
        bitNum = 3;
        p3 = &	MPC.PA3PFS.BYTE;
        value = 0x01;
        break;

        /*MTU1*/
    case HAL_CH_TGRA1:/* (MTU1, MTIOC1A), (PE4, Pin47)*/
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 4;
        p3 = &	MPC.PE4PFS.BYTE ;
        value = 0x02;
        break;

    case HAL_CH_TGRB1:/* (MTU1, MTIOC1B), (PB5, Pin35)*/
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.PB5PFS.BYTE;
        value = 0x02;
        break;

        /*MTU2*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), (P26,Pin16), (PB5,Pin35)*/
#if defined(HAL_CH_TGRA2_ON_PB5)
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.PB5PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORT2.PDR.BYTE;
        p2 = & PORT2.PMR.BYTE;
        bitNum = 6;
        p3 = &	MPC.P26PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRB2:/*(MTU2, MTIOC2B), (P27,Pin15), (PE5,Pin46)*/
#if defined(HAL_CH_TGRB2_ON_PE5)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.PE5PFS.BYTE;
        value = 0x02;
#else
        p1 = & PORT2.PDR.BYTE;
        p2 = & PORT2.PMR.BYTE;
        bitNum = 7;
        p3 = &	MPC.P27PFS.BYTE;
        value = 0x01;
#endif
        break;

        /*MTU3*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), (P14,Pin20),(PC7,Pin27),(P17,Pin17)*/
#if defined(HAL_CH_TGRA3_ON_PC7)
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 7;
        p3 = &	MPC.PC7PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRA3_ON_P17)
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 7;
        p3 = &	MPC.P17PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 4;
        p3 = &	MPC.P14PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRB3:/*(MTU3, MTIOC3B), (PC5,Pin29),(P17,Pin17),(PB7,Pin33)*/
#if defined(HAL_CH_TGRB3_ON_PB7)
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 7;
        p3 = &	MPC.PB7PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRB3_ON_P17)
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 7;
        p3 = &	MPC.P17PFS.BYTE;
        value = 0x02;
#else
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.PC5PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), (PC6,Pin 28), (P16, Pin18),*/
#if defined(HAL_CH_TGRC3_ON_P16)
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 6;
        p3 = &	MPC.P16PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 6;
        p3 = &	MPC.PC6PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRD3:/* (MTU3, MTIOC3D), (PB6, Pin34),P16,Pin 18), (PC4, Pin30)*/
#if defined(HAL_CH_TGRD3_ON_P16)
        p1 = & PORT1.PDR.BYTE;
        p2 = & PORT1.PMR.BYTE;
        bitNum = 6;
        p3 = &	MPC.P16PFS.BYTE;
        value = 0x02;
#elif defined(HAL_CH_TGRD3_ON_PC4)
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 4;
        p3 = &	MPC.PC4PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 6;
        p3 = &	MPC.PB6PFS.BYTE;
        value = 0x01;
#endif
        break;

        /* MTU4*/
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), (PA0,Pin45),(PB3,Pin36),(PE2,Pin49),*/
#if defined(HAL_CH_TGRA4_ON_PB3)
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 3;
        p3 = &	MPC.PB3PFS.BYTE;
        value = 0x02;
#elif defined(HAL_CH_TGRA4_ON_PE2)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 2;
        p3 = &	MPC.PE2PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORTA.PDR.BYTE;
        p2 = & PORTA.PMR.BYTE;
        bitNum = 0;
        p3 = &	MPC.PA0PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRB4:/* (MTU4, MTIOC4B), (PC2,Pin32),(P30,Pin14),(P54,Pin26),(PE3,Pin48)*/
#if defined(HAL_CH_TGRB4_ON_P30)
        p1 = & PORT3.PDR.BYTE;
        p2 = & PORT3.PMR.BYTE;
        bitNum = 0;
        p3 = &	MPC.P30PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRB4_ON_P54)
        p1 = & PORT5.PDR.BYTE;
        p2 = & PORT5.PMR.BYTE;
        bitNum = 4;
        p3 = &	MPC.P54PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRB4_ON_PE3)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 3;
        p3 = &	MPC.PE3PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 2;
        p3 = &	MPC.PC2PFS.BYTE;
        value = 0x01;
#endif
        break;

    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), (PB1, Pin37),(PE5,Pin46),(PE1,Pin50)*/
#if defined(HAL_CH_TGRC4_ON_PE5)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.PE5PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRC4_ON_PE1)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 1;
        p3 = &	MPC.PE1PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORTB.PDR.BYTE;
        p2 = & PORTB.PMR.BYTE;
        bitNum = 1;
        p3 = &	MPC.PB1PFS.BYTE;
        value = 0x02;
#endif
        break;

    case HAL_CH_TGRD4:/* (MTU4, MTIOC4D), (P31, Pin13),(P55,Pin25),(PC3,Pin31), (PE4,Pin47)*/
#if defined(HAL_CH_TGRD4_ON_P55)
        p1 = & PORT5.PDR.BYTE;
        p2 = & PORT5.PMR.BYTE;
        bitNum = 5;
        p3 = &	MPC.P55PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRD4_ON_PC3)
        p1 = & PORTC.PDR.BYTE;
        p2 = & PORTC.PMR.BYTE;
        bitNum = 3;
        p3 = &	MPC.PC3PFS.BYTE;
        value = 0x01;
#elif defined(HAL_CH_TGRD4_ON_PE4)
        p1 = & PORTE.PDR.BYTE;
        p2 = & PORTE.PMR.BYTE;
        bitNum = 4;
        p3 = &	MPC.PE4PFS.BYTE;
        value = 0x01;
#else
        p1 = & PORT3.PDR.BYTE;
        p2 = & PORT3.PMR.BYTE;
        bitNum = 1;
        p3 = &	MPC.P31PFS.BYTE;
        value = 0x02;
#endif
        break;

    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    if( p1 != 0 && p2 != 0 && p3 != 0)
    {
        CLEAR_BIT(*p1, bitNum);
        SET_BIT(*p2, bitNum);
        DISABLE_PORT_REGISTER_WRITE_PROTECTION();
        *p3 = value;
        ENABLE_PORT_REGISTER_WRITE_PROTECTION();
    }
}/*end TMR_SetChannelInputChapture*/


/*
 * Function: TMR_SetChannelOutputCompare
 * Description: Set a channel in Output Compare Mode (Software compare only, not deals with function on the pin)
 * Notes: TGRA is the threshold of HW timer
 * Status:
 * 	- Check the Indesit file maybe error!
 *  - To do!
 *
 */
void TMR_SetChannelOutputCompare( TMR_CHAddressType chId)
{
    volatile unsigned char *p1=0;
    unsigned char mode =MTUO_TIOR_MODE_OUTPUT_PROIBITED;

    PLT_ASSERT( chId >= HAL_CH_TGRA0 && chId < HAL_CH_MAX)

    p1 = ChModeRegAddr[chId];

    if(p1 != 0)
	{
		if( GET_BIT(ChModeRegAddr_Nibble, chId))
		{
			SET_H_NIBBLE(*p1, mode);
		}
		else
		{
			SET_L_NIBBLE(*p1, mode);
		}
	}
}/* end TMR_SetChannelOutputCompare*/

/*
 * Function: TMR_SetChannelPWM
 * Description: Setting of PWM Channel
 * Note:
 * Status:
 */
void TMR_SetChannelPWM( TMR_CHAddressType chId)
{

    switch (chId)
    {
        /* MTU0 ----------*/
    case HAL_CH_TGRA0:	/* (MTU0, MTIOC0A),PB3,Pin36*/
        MTU0.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
        PORTB.PDR.BYTE |= PORTX_BIT3_MASK;; 	/* PB3 is input*/
        PORTB.PMR.BYTE |= PORTX_BIT3_MASK;; 	/* PB3 is for peripheral function*/
        break;
    case HAL_CH_TGRB0:/* (MTUO, MTI0C0B), (P15,Pin19), (PA1,Pin44)*/
        MTU0.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM Mode*/
        SET_H_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRB0_ON_PA1)
        SET_BIT(PORTA.PDR.BYTE, 1);					/* is output */
        SET_BIT(PORTA.PMR.BYTE, 1);					/* is for peripheral function */
#else
        SET_BIT(PORT1.PDR.BYTE, 5);					/* is output */
        SET_BIT(PORT1.PMR.BYTE, 5);					/* is for peripheral function */
#endif
        break;
    case HAL_CH_TGRC0:/*((MTUO, MTIOCOC), P32, Pin12*/
        MTU0.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM Mode*/
        SET_L_NIBBLE( MTU0.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU0.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
        PORT3.PDR.BYTE |= PORTX_BIT2_MASK; 		/* P32 is Output*/
        PORT3.PMR.BYTE |= PORTX_BIT2_MASK; 		/* P32 is for peripheral function*/
        break;
    case HAL_CH_TGRD0: /*((MTUO, MTIOCOD),(PA3,Pin43)*/
        MTU0.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU0.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
        SET_H_NIBBLE( MTU0.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_BIT(PORTA.PDR.BYTE, 3);					/* is output*/
        SET_BIT(PORTA.PMR.BYTE, 3);
        break;
        /*MTU1 -----------*/
    case HAL_CH_TGRA1:/* (MTU1, MTIOC1A, PE4, Pin47)*/
        MTU1.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_H_NIBBLE( MTU1.TIOR.BYTE, MTU_TIOR_MODE_PIN_0_1);
        SET_L_NIBBLE( MTU1.TIOR.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_BIT(PORTE.PDR.BYTE, 4);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 4);
        break;
    case HAL_CH_TGRB1:/* (MTU1, MTIOC1B), (PB5, Pin35)*/
        MTU1.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU1.TIOR.BYTE, MTU_TIOR_MODE_PIN_0_1);
        SET_H_NIBBLE( MTU1.TIOR.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_BIT(PORTB.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORTB.PMR.BYTE, 5);
        break;
        /*MTU2 ----------*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), (P26,Pin16), (PB5,Pin35)*/
        MTU2.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU2.TIOR.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU2.TIOR.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRA2_ON_PB5)
        SET_BIT(PORTB.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORTB.PMR.BYTE, 5);
#else
        SET_BIT(PORT2.PDR.BYTE, 6);					/* is output*/
        SET_BIT(PORT2.PMR.BYTE, 6);
#endif
        break;
    case HAL_CH_TGRB2:/*(MTU2, MTIOC2B), (P27,Pin15), (PE5,Pin46)*/
        MTU2.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_H_NIBBLE( MTU2.TIOR.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU2.TIOR.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRB2_ON_PE5)
        SET_BIT(PORTE.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 5);
#else
        SET_BIT(PORT2.PDR.BYTE, 7);					/* is output*/
        SET_BIT(PORT2.PMR.BYTE, 7);
#endif
        break;
        /*MTU3 -----------*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), (P14,Pin20),(PC7,Pin27),(P17,Pin17)*/
        MTU3.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU3.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU3.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRA3_ON_PC7)
        SET_BIT(PORTC.PDR.BYTE, 7);					/* is output*/
        SET_BIT(PORTC.PMR.BYTE, 7);
#elif defined(HAL_CH_TGRA3_ON_P17)
        SET_BIT(PORT1.PDR.BYTE, 7);					/* is output*/
        SET_BIT(PORT1.PMR.BYTE, 7);
#else
        SET_BIT(PORT1.PDR.BYTE, 4);					/* is output*/
        SET_BIT(PORT1.PMR.BYTE, 4);
#endif
        break;
    case HAL_CH_TGRB3:/*(MTU3, MTIOC3B), (PC5,Pin29),(P17,Pin17),(PB7,Pin33)*/
        MTU3.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        MTU.TOER.BIT.OE3B = 1;
        SET_H_NIBBLE( MTU3.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU3.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRB3_ON_P17)
        SET_BIT(PORT1.PDR.BYTE, 7);					/* is output*/
        SET_BIT(PORT1.PMR.BYTE, 7);
#elif defined(HAL_CH_TGRB3_ON_PB7)
        SET_BIT(PORTB.PDR.BYTE, 7);					/* is output*/
        SET_BIT(PORTB.PMR.BYTE, 7);
#else
        SET_BIT(PORTC.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORTC.PMR.BYTE, 5);
#endif
        break;
    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), (PC6,Pin 28), (P16, Pin18),*/
        MTU3.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        SET_L_NIBBLE( MTU3.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU3.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRC3_ON_P16)
        SET_BIT(PORT1.PDR.BYTE, 6);					/* is output*/
        SET_BIT(PORT1.PMR.BYTE, 6);
#else
        SET_BIT(PORTC.PDR.BYTE, 6);					/* is output*/
        SET_BIT(PORTC.PMR.BYTE, 6);
#endif
        break;
    case HAL_CH_TGRD3:/* (MTU3, MTIOC3D), (PB6, Pin34),P16,Pin 18), (PC4, Pin30)*/
        MTU3.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        MTU.TOER.BIT.OE3D = 1;
        SET_H_NIBBLE( MTU3.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU3.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRD3_ON_P16)
        SET_BIT(PORT1.PDR.BYTE, 6);					/*  is output */
        SET_BIT(PORT1.PMR.BYTE, 6);
#elif defined(HAL_CH_TGRD3_ON_PC4)
        SET_BIT(PORTC.PDR.BYTE, 4);					/*  is output */
        SET_BIT(PORTC.PMR.BYTE, 4);
#else
        SET_BIT(PORTB.PDR.BYTE, 6);					/*  is output */
        SET_BIT(PORTB.PMR.BYTE, 6);
#endif
        break;
        /* MTU4 ----------- */
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), (PA0,Pin45),(PB3,Pin36),(PE2,Pin49), */
        MTU4.TMDR.BYTE |= (RX_PWM_MODE_1); /*  Select PWM Mode */
        MTU.TOER.BIT.OE4A = 1;
        SET_L_NIBBLE( MTU4.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU4.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRA4_ON_PB3)
        SET_BIT(PORTB.PDR.BYTE, 3);					/* is output*/
        SET_BIT(PORTB.PMR.BYTE, 3);
#elif defined(HAL_CH_TGRA4_ON_PE2)
        SET_BIT(PORTE.PDR.BYTE, 2);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 2);
#else
        SET_BIT(PORTA.PDR.BYTE, 0);					/* is output*/
        SET_BIT(PORTA.PMR.BYTE, 0);
#endif
        break;
    case HAL_CH_TGRB4:/* (MTU4, MTIOC4B), (PC2,Pin32),(P30,Pin14),(P54,Pin26),(PE3,Pin48)*/
        MTU4.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        MTU.TOER.BIT.OE4B = 1;
        SET_H_NIBBLE( MTU4.TIORH.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU4.TIORH.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRB4_ON_P30)
        SET_BIT(PORT3.PDR.BYTE, 0);					/* is output*/
        SET_BIT(PORT3.PMR.BYTE, 0);
#elif defined(HAL_CH_TGRB4_ON_P54)
        SET_BIT(PORT5.PDR.BYTE, 4);					/* is output*/
        SET_BIT(PORT5.PMR.BYTE, 4);
#elif defined(HAL_CH_TGRB4_ON_PE3)
        SET_BIT(PORTE.PDR.BYTE, 3);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 3);
#else
        SET_BIT(PORTC.PDR.BYTE, 2);					/* is output*/
        SET_BIT(PORTC.PMR.BYTE, 2);
#endif
        break;
    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), (PB1, Pin37),(PE5,Pin46),(PE1,Pin50)*/
        MTU4.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        MTU.TOER.BIT.OE4C = 1;
        SET_L_NIBBLE( MTU4.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_H_NIBBLE( MTU4.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRC4_ON_PE5)
        SET_BIT(PORTE.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 5);
#elif defined(HAL_CH_TGRC4_ON_PE1)
        SET_BIT(PORTE.PDR.BYTE, 1);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 1);
#else
        SET_BIT(PORTB.PDR.BYTE, 1);					/* is output*/
        SET_BIT(PORTB.PMR.BYTE, 1);
#endif
        break;
    case HAL_CH_TGRD4:/* (MTU4, MTIOC4D), (P31, Pin13),(P55,Pin25),(PC3,Pin31), (PE4,Pin47)*/
        MTU4.TMDR.BYTE |= (RX_PWM_MODE_1); /* Select PWM ModeÃ¬*/
        MTU.TOER.BIT.OE4D = 1;
        SET_H_NIBBLE( MTU4.TIORL.BYTE, MTU_TIOR_MODE_PIN_1_0);
        SET_L_NIBBLE( MTU4.TIORL.BYTE, MTU_TIOR_MODE_PIN_0_1);
#if defined(HAL_CH_TGRD4_ON_P55)
        SET_BIT(PORT5.PDR.BYTE, 5);					/* is output*/
        SET_BIT(PORT5.PMR.BYTE, 5);
#elif defined(HAL_CH_TGRD4_ON_PC3)
        SET_BIT(PORTC.PDR.BYTE, 3);					/* is output*/
        SET_BIT(PORTC.PMR.BYTE, 3);
#elif defined(HAL_CH_TGRD4_ON_PE4)
        SET_BIT(PORTE.PDR.BYTE, 4);					/* is output*/
        SET_BIT(PORTE.PMR.BYTE, 4);
#else
        SET_BIT(PORT3.PDR.BYTE, 1);					/* is output*/
        SET_BIT(PORT3.PMR.BYTE, 1);
#endif
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }/*end switch*/
}/* end TMR_SetChannelPWM*/

/*
 * Function: TMR_ToggleChannelEdgeMode
 * Description: Toogle edged type to falling or rising
 * Note:
 * Status: Test me!
 */
#if 0
void TMR_ToggleChannelEdgeMode( TMR_CHAddressType pChannel)
{
    int chId = (int)pChannel;
    uint8_t	mode;
    uint8_t highNibble=0;
    uint8_t *p1;

    switch( chId )
    {
        /*MTU0 ----------------------------------------------------------*/
    case HAL_CH_TGRA0:
        /* highNibble = 0; */
        p1 = & MTU0.TIORH.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU0.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRB0:
        highNibble = 1;
        p1 = & MTU0.TIORH.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU0.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU0.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU0.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRC0:
        /* highNibble = 0; */
        p1 = & MTU0.TIORL.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU0.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRD0:
        highNibble = 1;
        p1 = & MTU0.TIORL.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU0.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
        /*MTU1 ---------------------------------------------------------*/
    case HAL_CH_TGRA1:
        /* highNibble = 0; */
        p1 = & MTU1.TIOR.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU1.TIOR.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRB1:
        highNibble = 1;
        p1 = & MTU1.TIOR.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU1.TIOR.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
        /*MTU2 -------------------------------------------------------*/
    case HAL_CH_TGRA2:
        /* highNibble = 0; */
        p1 = & MTU2.TIOR.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU2.TIOR.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRB2:
        highNibble = 1;
        p1 = & MTU2.TIOR.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU2.TIOR.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
        /*MTU3 ---------------------------------------------------------*/
    case HAL_CH_TGRA3:
        /* highNibble = 0; */
        p1 = & MTU3.TIORH.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU3.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRB3:
        highNibble = 1;
        p1 = & MTU3.TIORH.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU3.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRC3:
        /* highNibble = 0; */
        p1 = & MTU3.TIORL.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU3.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRD3:
        highNibble = 1;
        p1 = & MTU3.TIORL.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU3.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
        /*MTU4 ----------------------------------------------------------*/
    case HAL_CH_TGRA4:
        /* highNibble = 0; */
        p1 = & MTU4.TIORH.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU4.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRB4:
        highNibble = 1;
        p1 = & MTU4.TIORH.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU4.TIORH.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    case HAL_CH_TGRC4:
    {
        /* highNibble = 0; */
        p1 = & MTU4.TIORL.BYTE;
#if 0
        mode = GET_L_NIBBLE(MTU4.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_L_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
    }
    break;
    case HAL_CH_TGRD4:
        highNibble = 1;
        p1 = & MTU4.TIORL.BYTE;
#if 0
        mode = GET_H_NIBBLE(MTU4.TIORL.BYTE);
        if ( mode == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_FALING_EDGE);
        else if(  mode == MTU_TIOR_MODE_FALING_EDGE )
            SET_H_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_RISING_EDGE);
#endif
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    if(highNibble)
    {
        if ( GET_H_NIBBLE(*p1) == MTU_TIOR_MODE_RISING_EDGE )
            SET_H_NIBBLE( *p1, MTU_TIOR_MODE_FALING_EDGE);
        else
            SET_H_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
    }
    else
    {
        if ( GET_L_NIBBLE(*p1) == MTU_TIOR_MODE_RISING_EDGE )
            SET_L_NIBBLE( *p1, MTU_TIOR_MODE_FALING_EDGE);
        else
            SET_L_NIBBLE( MTU0.TIORH.BYTE, MTU_TIOR_MODE_RISING_EDGE);
    }

}/*end TMR_ToggleChannelEdgeMode*/
#endif
void TMR_ToggleChannelEdgeMode( TMR_CHAddressType pChannel)
{
    int chId = (int)pChannel;
    volatile uint8_t *p1;

    p1 = ChModeRegAddr[chId];

    if( p1 !=0 )
    {
        if( GET_BIT(ChModeRegAddr_Nibble, chId))
        {
            if ( GET_H_NIBBLE(*p1) == MTU_TIOR_MODE_RISING_EDGE )
                SET_H_NIBBLE( *p1, MTU_TIOR_MODE_FALING_EDGE);
            else
                SET_H_NIBBLE( *p1, MTU_TIOR_MODE_RISING_EDGE);
        }
        else
        {
            if ( GET_L_NIBBLE(*p1) == MTU_TIOR_MODE_RISING_EDGE )
                SET_L_NIBBLE( *p1, MTU_TIOR_MODE_FALING_EDGE);
            else
                SET_L_NIBBLE( *p1, MTU_TIOR_MODE_RISING_EDGE);
        }
    }
    else
    {
        HAL_ErrorHandler(1);
    }
}/*end TMR_ToggleChannelEdgeMode*/

/*
 * Function: TMR_SetTimeOn
 * Description: This function defines the match point for the PWM.
 *              When the counter reaches value	the outputs are switched high.
 * Note:
 * Status:
 */
void TMR_SetTimeOn( TMR_CHAddressType chID, Period_ValueType tOn)
{
    volatile uint16_t* p1;
#if 0
    switch( chID )
    {
        /* MTU0 ---------*/
    case HAL_CH_TGRA0:
        MTU0_TGRA = tOn;
        break;
    case HAL_CH_TGRB0:
        MTU0_TGRB = tOn;
        break;
    case HAL_CH_TGRC0:
        MTU0_TGRC = tOn;
        break;
    case HAL_CH_TGRD0:
        MTU0_TGRD = tOn;
        break;
        /*MTU1 --------*/
    case HAL_CH_TGRA1:
        MTU1_TGRA = tOn;
        break;
    case HAL_CH_TGRB1:
        MTU1_TGRB = tOn;
        break;
        /* MTU2 -----------------*/
    case HAL_CH_TGRA2:
        MTU2_TGRA = tOn;
        break;
    case HAL_CH_TGRB2:
        MTU2_TGRB = tOn;
        break;
        /* MTU3 -----------------*/
    case HAL_CH_TGRA3:
        MTU3_TGRA = tOn;
        break;
    case HAL_CH_TGRB3:
        MTU3_TGRB = tOn;
        break;
    case HAL_CH_TGRC3:
        MTU3_TGRC = tOn;
        break;
    case HAL_CH_TGRD3:
        MTU3_TGRD = tOn;
        break;
        /* MTU4 -----------------*/
    case HAL_CH_TGRA4:
        MTU4_TGRA = tOn;
        break;
    case HAL_CH_TGRB4:
        MTU4_TGRB = tOn;
        break;
    case HAL_CH_TGRC4:
        MTU4_TGRC = tOn;
        break;
    case HAL_CH_TGRD4:
        MTU4_TGRD = tOn;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
#endif
    p1 = ChTGRAAddr[chID];
    if( p1 != 0)
    {
        *p1 = tOn;
    }
    else
    {
        HAL_ErrorHandler(1);
    }
}

void     TMR_GetTimeOn( TMR_CHAddressType chID, Period_ValueType *tOn)
{
    *tOn = TMR_GetChValue(chID);
}

/*
 * Function: TMR_GetChValue
 * Description: This function return the cannel registr register value.
 */
Period_ValueType TMR_GetChValue( TMR_CHAddressType chID )
{
    Period_ValueType value = 0;
    volatile uint16_t* p1;
#if 0
    switch( chID )
    {
        /* MTU0 ---------*/
    case HAL_CH_TGRA0:
        value = MTU0_TGRA;
        break;
    case HAL_CH_TGRB0:
        value = MTU0_TGRB;
        break;
    case HAL_CH_TGRC0:
        value = MTU0_TGRC;
        break;
    case HAL_CH_TGRD0:
        value = MTU0_TGRD;
        break;
        /*MTU1 --------*/
    case HAL_CH_TGRA1:
        value = MTU1_TGRA;
        break;
    case HAL_CH_TGRB1:
        value = MTU1_TGRB;
        break;
        /* MTU2 -----------------*/
    case HAL_CH_TGRA2:
        value = MTU2_TGRA;
        break;
    case HAL_CH_TGRB2:
        value = MTU2_TGRB;
        break;
        /* MTU3 -----------------*/
    case HAL_CH_TGRA3:
        value = MTU3_TGRA;
        break;
    case HAL_CH_TGRB3:
        value = MTU3_TGRB;
        break;

    case HAL_CH_TGRC3:
        value = MTU3_TGRC;
        break;
    case HAL_CH_TGRD3:
        value = MTU3_TGRD;
        break;
        /* MTU4 -----------------*/
    case HAL_CH_TGRA4:
        value = MTU4_TGRA;
        break;
    case HAL_CH_TGRB4:
        value = MTU4_TGRB;
        break;
    case HAL_CH_TGRC4:
        value = MTU4_TGRC;
        break;
    case HAL_CH_TGRD4:
        value = MTU4_TGRD;
        break;

    default:
        HAL_ErrorHandler(1);
        break;
    }
#endif
    p1 = ChTGRAAddr[chID];
    if( p1 != 0)
    {
        value = *p1;
    }
    else
    {
        HAL_ErrorHandler(1);
    }
    return value;
}


/*
 * Function: TMR_SetChannelPulses
 * Description: Turns on the PWM mode. Enable the output of PWM on PIN.
 * Note: Enable PWM Mode, inverted not used
 * Status:  Tested
 */
void TMR_SetChannelPulses(TMR_CHAddressType chId, uint8_t inverted)
{
    DISABLE_PORT_REGISTER_WRITE_PROTECTION();
    switch (chId)
    {
        /* MTU0 ----------*/
    case HAL_CH_TGRA0:								/* (MTU0, MTIOC0A),PB3,Pin36*/
        MPC.PB3PFS.BYTE = 0x01; 					/* PB3 is select as TMO0 function*/
        break;
    case HAL_CH_TGRB0:/* (MTUO, MTI0C0B), (P15,Pin19), (PA1,Pin44)*/
#if defined(HAL_CH_TGRB0_ON_PA1)
        MPC.PA1PFS.BYTE = 0x01; 					/* PA1 is select as MTI0C0B*/
#else
        MPC.P15PFS.BYTE = 0x01; 					/* P15 is select as MTI0C0B*/
#endif
        break;
    case HAL_CH_TGRC0:/*((MTUO, MTIOCOC), P32, Pin12*/
        MPC.P32PFS.BYTE = 0x01; 					/* P32 is select as TMO0 function*/
        break;
    case HAL_CH_TGRD0: /*((MTUO, MTIOCOD),(PA3,Pin43)*/
        MPC.PA3PFS.BYTE = 0x01;
        break;
        /*MTU1 -----------*/
    case HAL_CH_TGRA1:/* (MTU1, MTIOC1A, PE4, Pin47)*/
        MPC.PE4PFS.BYTE = 0x02; 					/* PE$ is select as TMO0 function*/
        break;
    case HAL_CH_TGRB1:/* (MTU1, MTIOC1B), (PB5, Pin35)*/
        MPC.PB5PFS.BYTE = 0x02;
        break;
        /*MTU2 ----------*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), (P26,Pin16), (PB5,Pin35)*/
#if defined(HAL_CH_TGRA2_ON_PB5)
        MPC.PB5PFS.BYTE = 0x01; 					/*PB5 is select as TMO0 function*/
#else
        MPC.P26PFS.BYTE = 0x01; 					/*P26 is select as TMO0 function*/
#endif
        break;
    case HAL_CH_TGRB2:/*(MTU2, MTIOC2B), (P27,Pin15), (PE5,Pin46)*/
#if defined(HAL_CH_TGRB2_ON_PE5)
        MPC.PE5PFS.BYTE = 0x02; 					/* PE5 is selected as MTIOC2B*/
#else
        MPC.P27PFS.BYTE = 0x01; 					/* P27 is selected as MTIOC2B*/
#endif
        break;
        /*MTU3 -----------*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), (P14,Pin20),(PC7,Pin27),(P17,Pin17)*/
#if defined(HAL_CH_TGRA3_ON_PC7)
        MPC.PC7PFS.BYTE = 0x01; 					/* PC7 is select as TMO0 function*/
#elif defined(HAL_CH_TGRA3_ON_P17)
        MPC.P17PFS.BYTE = 0x01; 					/* P17 is select as TMO0 function*/
#else
        MPC.P14PFS.BYTE = 0x01; 					/* P14 is select as TMO0 function*/
#endif
        break;
    case HAL_CH_TGRB3:/*(MTU3, MTIOC3B), (PC5,Pin29),(P17,Pin17),(PB7,Pin33)*/
#if defined(HAL_CH_TGRB3_ON_P17)
        MPC.P17PFS.BYTE = 0x02; 					/* P17 is selected as MTIOC3B*/
#elif defined(HAL_CH_TGRB3_ON_PB7)
        MPC.PB7PFS.BYTE = 0x01; 					/* PB7 is selected as MTIOC3B*/
#else
        MPC.PC5PFS.BYTE = 0x01; 					/* PC5 is selected as MTIOC3B*/
#endif
        break;
    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), (PC6,Pin 28), (P16, Pin18),*/
#if defined(HAL_CH_TGRC3_ON_P16)
        MPC.P16PFS.BYTE = 0x01; 					/* P16 is select as MTIOC3C function*/
#else
        MPC.PC6PFS.BYTE = 0x01; 					/* PC6 is select as MTIOC3C function*/
#endif
        break;
    case HAL_CH_TGRD3:/* (MTU3, MTIOC3D), (PB6, Pin34), (P16,Pin 18), (PC4, Pin30)*/
#if defined(HAL_CH_TGRD3_ON_P16)
        MPC.P16PFS.BYTE = 0x02; 					/* P16 is select as MTIOC3D function*/
#elif defined(HAL_CH_TGRD3_ON_PC4)
        MPC.PC4PFS.BYTE = 0x01; 					/* PC4 is select as MTIOC3D function*/
#else
        MPC.PB6PFS.BYTE = 0x01; 					/* PB6 is selected as MTIOC3D */
#endif
        break;
        /*MTU4 -----------*/
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), (PA0,Pin45),(PB3,Pin36),(PE2,Pin49),*/
#if defined(HAL_CH_TGRA4_ON_PB3)
        MPC.PB3PFS.BYTE = 0x02; 					/* PB3 is select as MTIOC4A function*/
#elif defined(HAL_CH_TGRA4_ON_PE2)
        MPC.PE2PFS.BYTE = 0x01; 					/* PE2 is select as MTIOC4A function*/
#else
        MPC.PA0PFS.BYTE = 0x01; 					/* PA0 is select as MTIOC4A function*/
#endif
        break;
    case HAL_CH_TGRB4:/* (MTU4, MTIOC4B), (PC2,Pin32),(P30,Pin14),(P54,Pin26),(PE3,Pin48)*/
#if defined(HAL_CH_TGRB4_ON_P30)
        MPC.P30PFS.BYTE = 0x01; 					/* P30 is select as MTIOC4B function*/
#elif defined(HAL_CH_TGRB4_ON_P54)
        MPC.P54PFS.BYTE = 0x01; 					/* P54 is select as MTIOC4B function*/
#elif defined(HAL_CH_TGRB4_ON_PE3)
        MPC.PE3PFS.BYTE = 0x01; 					/* PE3 is select as MTIOC4B function*/
#else
        MPC.PC2PFS.BYTE = 0x01; 					/* PC2 is select as MTIOC4B function*/
#endif
        break;
    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), (PB1, Pin37),(PE5,Pin46),(PE1,Pin50)*/
#if defined(HAL_CH_TGRC4_ON_PE5)
        MPC.PE5PFS.BYTE = 0x01; 					/* PE5 is select as MTIOC4C function*/
#elif defined(HAL_CH_TGRC4_ON_PE1)
        MPC.PE1PFS.BYTE = 0x01; 					/* PE1 is select as MTIOC4C function*/
#else
        MPC.PB1PFS.BYTE = 0x02; 				/* PB1 is select as MTIOC4C function*/
#endif
        break;
    case HAL_CH_TGRD4:/* (MTU4, MTIOC4D), (P31, Pin13),(P55,Pin25),(PC3,Pin31), (PE4,Pin47)*/
#if defined(HAL_CH_TGRD4_ON_P55)
        MPC.P55PFS.BYTE = 0x01; 					/* P30 is select as MTIOC4D function*/
#elif defined(HAL_CH_TGRD4_ON_PC3)
        MPC.PC3PFS.BYTE = 0x01; 					/* P54 is select as MTIOC4D function*/
#elif defined(HAL_CH_TGRD4_ON_PE4)
        MPC.PE4PFS.BYTE = 0x01; 					/* PE3 is select as MTIOC4D function*/
#else
        MPC.P31PFS.BYTE = 0x02; 					/* P31 is select as MTIOC4D*/
#endif
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }/*end switch*/
    ENABLE_PORT_REGISTER_WRITE_PROTECTION();
}/*end TMR_SetChannelPulses*/

/*
* Function: Clear Channel Pulse
* Description: Set the initial state of PWM Pin
*
*/
void TMR_ClearChannelPulses(TMR_CHAddressType chId)
{
    DISABLE_PORT_REGISTER_WRITE_PROTECTION();
    switch (chId)
    {
        /* MTU0 ----------*/
    case HAL_CH_TGRA0:	/* (MTU0, MTIOC0A),PB3,Pin36*/
        MPC.PB3PFS.BYTE = 0x00; /* PB3 is select as TMO0 function*/
        break;
    case HAL_CH_TGRB0:/* (MTUO, MTI0C0B), (P15,Pin19), (PA1,Pin44)*/
#if defined(HAL_CH_TGRB0_ON_PA1)
        MPC.PA1PFS.BYTE = 0x00; 					/* PA1 is select as MTI0C0B*/
#else
        MPC.P15PFS.BYTE = 0x00; 					/* P15 is select as MTI0C0B*/
#endif
        break;
    case HAL_CH_TGRC0:/*((MTUO, MTIOCOC), P32, Pin12*/
        MPC.P32PFS.BYTE = 0x00; 					/* P32 is select as MTIOCOC function*/
        break;
    case HAL_CH_TGRD0: /*((MTUO, MTIOCOD),(PA3,Pin43)*/
        MPC.PA3PFS.BYTE = 0x00;
        break;
        /*MTU1 -----------*/
    case HAL_CH_TGRA1:/* (MTU1, MTIOC1A, PE4, Pin47)*/
        MPC.PE4PFS.BYTE = 0x00; 					/* PE4 is select as MTIOC1A function*/
        break;
    case HAL_CH_TGRB1:/* (MTU1, MTIOC1B), (PB5, Pin35)*/
        MPC.PB5PFS.BYTE = 0x00;
        break;
        /*MTU2 ----------*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), (P26,Pin16), (PB5,Pin35)*/
#if defined(HAL_CH_TGRA2_ON_PB5)
        MPC.PB5PFS.BYTE = 0x00; 					/*PB5 is select as MTIOC2A function*/
#else
        MPC.P26PFS.BYTE = 0x00; 					/* P32 is select as MTIOC2A function*/
#endif
        break;
    case HAL_CH_TGRB2:/*(MTU2, MTIOC2B), (P27,Pin15), (PE5,Pin46)*/
#if defined(HAL_CH_TGRB2_ON_PE5)
        MPC.PE5PFS.BYTE = 0x00; 					/* PE5 is selected as MTIOC2B*/
#else
        MPC.P27PFS.BYTE = 0x00; 					/* P27 is selected as MTIOC2B*/
#endif
        break;
        /*MTU3 -----------*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), (P14,Pin20),(PC7,Pin27),(P17,Pin17)*/
#if defined(HAL_CH_TGRA3_ON_PC7)
        MPC.PC7PFS.BYTE = 0x00; 					/* PC7 is select as MTIOC3A function*/
#elif defined(HAL_CH_TGRA3_ON_P17)
        MPC.P17PFS.BYTE = 0x00; 					/* P17 is select as MTIOC3A function*/
#else
        MPC.P14PFS.BYTE = 0x00; 					/* P14 is select as MTIOC3A function*/
#endif
        break;
    case HAL_CH_TGRB3:/*(MTU3, MTIOC3B), (PC5,Pin29),(P17,Pin17),(PB7,Pin33)*/
#if defined(HAL_CH_TGRB3_ON_P17)
        MPC.P17PFS.BYTE = 0x00; 					/* P17 is selected as MTIOC3B*/
#elif defined(HAL_CH_TGRB3_ON_PB7)
        MPC.PB7PFS.BYTE = 0x00; 					/* PB7 is selected as MTIOC3B*/
#else
        MPC.PC5PFS.BYTE = 0x00; 					/* PC5 is selected as MTIOC3B*/
#endif
        break;
    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), (PC6,Pin 28), (P16, Pin18),*/
#if defined(HAL_CH_TGRC3_ON_P16)
        MPC.P16PFS.BYTE = 0x00; 					/* P16 is select as MTIOC3C function*/
#else
        MPC.PC6PFS.BYTE = 0x00; 					/* PC6 is select as MTIOC3C function*/
#endif
        break;
    case HAL_CH_TGRD3:/* (MTU3, MTIOC3D), (PB6, Pin34),P16,Pin 18), (PC4, Pin30)*/
#if defined(HAL_CH_TGRD3_ON_P16)
        MPC.P16PFS.BYTE = 0x00; 					/* P16 is select as MTIOC3D function*/
#elif defined(HAL_CH_TGRD3_ON_PC4)
        MPC.PC4PFS.BYTE = 0x00; 					/* PC4 is select as MTIOC3D function*/
#else
        MPC.PB6PFS.BYTE = 0x00; 					/* PC6 is selected as MTIOC3D*/
#endif
        break;
        /*MTU4 -----------*/
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), (PA0,Pin45),(PB3,Pin36),(PE2,Pin49),*/
#if defined(HAL_CH_TGRA4_ON_PB3)
        MPC.PB3PFS.BYTE = 0x00; 					/* PB3 is select as MTIOC4A function*/
#elif defined(HAL_CH_TGRA4_ON_PE2)
        MPC.PE2PFS.BYTE = 0x00; 					/* PE2 is select as MTIOC4A function*/
#else
        MPC.PA0PFS.BYTE = 0x00; 					/* PA0 is select as MTIOC4A function*/
#endif
        break;
    case HAL_CH_TGRB4:/* (MTU4, MTIOC4B), (PC2,Pin32),(P30,Pin14),(P54,Pin26),(PE3,Pin48)*/
#if defined(HAL_CH_TGRB4_ON_P30)
        MPC.P30PFS.BYTE = 0x00; 					/* P30 is select as MTIOC4B function*/
#elif defined(HAL_CH_TGRB4_ON_P54)
        MPC.P54PFS.BYTE = 0x00; 					/* P54 is select as MTIOC4B function*/
#elif defined(HAL_CH_TGRB4_ON_PE3)
        MPC.PE3PFS.BYTE = 0x00; 					/* PE3 is select as MTIOC4B function*/
#else
        MPC.PC2PFS.BYTE = 0x00; 				/* PA0 is select as MTIOC4B function*/
#endif
        break;
    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), (PB1, Pin37),(PE5,Pin46),(PE1,Pin50)*/
#if defined(HAL_CH_TGRC4_ON_PE5)
        MPC.PE5PFS.BYTE = 0x00; 					/* PE5 is select as MTIOC4C function*/
#elif defined(HAL_CH_TGRC4_ON_PE1)
        MPC.PE1PFS.BYTE = 0x00; 					/* PE1 is select as MTIOC4C function*/
#else
        MPC.PB1PFS.BYTE = 0x00; 				/* PB1 is select as MTIOC4C function*/
#endif
        break;
    case HAL_CH_TGRD4:/* (MTU4, MTIOC4D), (P31, Pin13),(P55,Pin25),(PC3,Pin31), (PE4,Pin47)*/
#if defined(HAL_CH_TGRD4_ON_P55)
        MPC.P55PFS.BYTE = 0x00; 					/* P30 is select as MTIOC4D function*/
#elif defined(HAL_CH_TGRD4_ON_PC3)
        MPC.PC3PFS.BYTE = 0x00; 					/* P54 is select as MTIOC4D function*/
#elif defined(HAL_CH_TGRD4_ON_PE4)
        MPC.PE4PFS.BYTE = 0x00; 					/* PE3 is select as MTIOC4D function*/
#else
        MPC.P31PFS.BYTE = 0x00; 				/* P31 is select as MTIOC4D*/
#endif
        break;
    default:
        HAL_ErrorHandler(0);
        break;
    }/*end switch*/
    ENABLE_PORT_REGISTER_WRITE_PROTECTION();
}/*end TMR_SetChannelPulses*/

/*
 * Function: TMR_SetChannelEdgeMode
 * Description: This function configures the channel as InputCapture
 * Note:
 * Macro:
 */
void TMR_SetChannelEdgeMode( TMR_CHAddressType chId, uint8_t mode, bool_t inversion)
{
    uint8_t *p1;

    /* mapping del mode*/
    switch( mode )
    {
    case TMR_CH_IC_POS_EDGE:
        if( inversion ) mode=MTU_TIOR_MODE_FALING_EDGE;
        else		mode=MTU_TIOR_MODE_RISING_EDGE;
        break;
    case TMR_CH_IC_NEG_EDGE:
        if( inversion ) mode=MTU_TIOR_MODE_RISING_EDGE;
        else 		mode=MTU_TIOR_MODE_FALING_EDGE;
        break;
    case TMR_CH_IC_BOTH_EDGE:
        mode=MTU_TIOR_MODE_BOTH_EDGE;
        break;
    }

#if 0
    switch (chId)
    {
        /*MTU0*/
    case HAL_CH_TGRA0:
        SET_L_NIBBLE(MTU0.TIORH.BYTE, mode);	/* IOA*/
        break;
    case HAL_CH_TGRB0:
        SET_H_NIBBLE(MTU0.TIORH.BYTE, mode);	/* IOB*/
        break;
    case HAL_CH_TGRC0:
        SET_L_NIBBLE(MTU0.TIORL.BYTE, mode); 	/* IOC*/
        break;
    case HAL_CH_TGRD0:
        SET_H_NIBBLE(MTU0.TIORL.BYTE, mode);	/* IOD*/
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        SET_L_NIBBLE(MTU1.TIOR.BYTE, mode); 	/* IOA*/
        break;
    case HAL_CH_TGRB1:
        SET_H_NIBBLE(MTU1.TIOR.BYTE, mode); 	/* IOB*/
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:
        SET_L_NIBBLE(MTU2.TIOR.BYTE, mode); 	/* IOA*/
        break;
    case HAL_CH_TGRB2:
        SET_H_NIBBLE(MTU2.TIOR.BYTE, mode); 	/* IOB*/
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:
        SET_L_NIBBLE(MTU3.TIORH.BYTE, mode); 	/* IOA*/
        break;
    case HAL_CH_TGRB3:
        SET_H_NIBBLE(MTU3.TIORH.BYTE, mode); 	/* IOB*/
        break;
    case HAL_CH_TGRC3:
        SET_L_NIBBLE(MTU3.TIORL.BYTE, mode); 	/* IOC*/
        break;
    case HAL_CH_TGRD3:
        SET_H_NIBBLE(MTU3.TIORL.BYTE, mode); 	/* IOD*/
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:
        SET_L_NIBBLE(MTU4.TIORH.BYTE, mode); 	/* IOA*/
        break;
    case HAL_CH_TGRB4:
        SET_H_NIBBLE(MTU4.TIORH.BYTE, mode); 	/* IOB*/
        break;
    case HAL_CH_TGRC4:
        SET_L_NIBBLE(MTU4.TIORL.BYTE, mode); 	/* IOC*/
        break;
    case HAL_CH_TGRD4:
        SET_H_NIBBLE(MTU4.TIORL.BYTE, mode); 	/* IOD*/
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
#endif
    p1 = ChModeRegAddr[chId];
    if( p1 !=0 )
    {
        if( GET_BIT(ChModeRegAddr_Nibble, chId))
        {
            SET_H_NIBBLE(*p1, mode);
        }
        else
        {
            SET_L_NIBBLE(*p1, mode);
        }
    }
    else
    {
        HAL_ErrorHandler(1);
    }
}/*end TMR_SetChannelEdgeMode*/


/*
 * Function: TMR_GetValue
 * Description: This function return the timer counter value
 * Note:
 * Status:
 */
Period_ValueType TMR_GetValue(TMR_CNTAddressType tmrID)
{
    _HAL_RegType tcnt_value = 0;

    switch( tmrID )
    {
    case HAL_TMR_MTU0:
        tcnt_value = MTU0.TCNT;
        break;
    case HAL_TMR_MTU1:
        tcnt_value = MTU1.TCNT;
        break;
    case HAL_TMR_MTU2:
        tcnt_value = MTU2.TCNT;
        break;
    case HAL_TMR_MTU3:
        tcnt_value = MTU3.TCNT;
        break;
    case HAL_TMR_MTU4:
        tcnt_value = MTU4.TCNT;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    return (tcnt_value);
}/*end TMR_GETValue*/

/*
 * Function: TMR_EnableCHI
 * Description: Enables the interrupt for the channel, output compare/input compare
 * Note: What interrupt enable? Check with indesit...
 * Status: To implement.
 */
void TMR_EnableCHI( TMR_CHAddressType chId)
{
    switch (chId)
    {
        /*MTUO*/
    case HAL_CH_TGRA0:
        ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU0.TIER.BYTE |= (TGIA_INTERRUPT_ENABLE);
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT2_MASK;
        break;
    case HAL_CH_TGRB0:
        ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU0.TIER.BYTE |= (TGIB_INTERRUPT_ENABLE);
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT3_MASK;
        break;
    case HAL_CH_TGRC0:
        ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU0.TIER.BYTE |= (TGIC_INTERRUPT_ENABLE);
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT4_MASK;
        break;
    case HAL_CH_TGRD0:
        ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU0.TIER.BYTE |= (TGID_INTERRUPT_ENABLE);
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT5_MASK;
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        ICU.IPR[MTU1_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU1.TIER.BYTE |= (TGIA_INTERRUPT_ENABLE);
        ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT1_MASK;
        break;
    case HAL_CH_TGRB1:
        ICU.IPR[MTU1_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU1.TIER.BYTE |= (TGIB_INTERRUPT_ENABLE);
        ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT2_MASK;
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:
        ICU.IPR[MTU2_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU2.TIER.BYTE |= (TGIA_INTERRUPT_ENABLE);
        ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT5_MASK;
        break;
    case HAL_CH_TGRB2:
        ICU.IPR[MTU2_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU2.TIER.BYTE |= (TGIB_INTERRUPT_ENABLE);
        ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT6_MASK;
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:
        ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU3.TIER.BYTE |= (TGIA_INTERRUPT_ENABLE);
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT1_MASK;
        break;
    case HAL_CH_TGRB3:
        ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU3.TIER.BYTE |= (TGIB_INTERRUPT_ENABLE);
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT2_MASK;
        break;
    case HAL_CH_TGRC3:
        ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU3.TIER.BYTE |= (TGIC_INTERRUPT_ENABLE);
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT3_MASK;
        break;
    case HAL_CH_TGRD3:
        ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU3.TIER.BYTE |= (TGID_INTERRUPT_ENABLE);
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT4_MASK;
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:
        ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU4.TIER.BYTE |= (TGIA_INTERRUPT_ENABLE);
        ICU.IER[MTU4_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT6_MASK;
        break;
    case HAL_CH_TGRB4:
        ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU4.TIER.BYTE |= (TGIB_INTERRUPT_ENABLE);
        ICU.IER[MTU4_INT_ENABLE_REGISTER].BYTE |= _HAL_BIT7_MASK;
        break;
    case HAL_CH_TGRC4:
        ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU4.TIER.BYTE |= (TGIC_INTERRUPT_ENABLE);
        ICU.IER[MTU4_INT_ENABLE_REGISTER +1].BYTE |= _HAL_BIT0_MASK;
        break;
    case HAL_CH_TGRD4:
        ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = HAL_CHANNEL_DEFAULT_INT_PRIO;
        MTU4.TIER.BYTE |= (TGID_INTERRUPT_ENABLE);
        ICU.IER[MTU4_INT_ENABLE_REGISTER +1].BYTE |= _HAL_BIT1_MASK;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch icu_id*/
}/* end TMR_EnableCHI*/

/*
 * Function: TMR_DisableCHI
 * Description: Disable the interrupt for the channel
 * Note:
 * Status:
 * 	- AE 25/05/2012: Completed with all CHI
 */
void TMR_DisableCHI( TMR_CHAddressType chId)
{
    switch (chId)
    {
        /*MTUO*/
    case HAL_CH_TGRA0:
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT2_MASK;
        MTU0.TIER.BYTE &= (~TGIA_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRB0:
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT3_MASK;
        MTU0.TIER.BYTE &= ~(TGIB_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRC0:
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT4_MASK;
        MTU0.TIER.BYTE &= (~TGIC_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRD0:
        ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT5_MASK;
        MTU0.TIER.BYTE &= ~(TGID_INTERRUPT_ENABLE);
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT1_MASK;
        MTU1.TIER.BYTE &= ~(TGIA_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRB1:
        ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT2_MASK;
        MTU1.TIER.BYTE &= ~(TGIB_INTERRUPT_ENABLE);
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), P26, Pin16*/
        ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT5_MASK;
        MTU2.TIER.BYTE &= (~TGIA_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRB2:
        ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT6_MASK;
        MTU2.TIER.BYTE &= (~TGIB_INTERRUPT_ENABLE);
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), P14, Pin20*/
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT1_MASK;
        MTU3.TIER.BYTE &= (~TGIA_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRB3:
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT2_MASK;
        MTU3.TIER.BYTE &= ~(TGIB_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), PC6, Pin 28*/
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT3_MASK;
        MTU3.TIER.BYTE &= (~TGIC_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRD3:
        ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT4_MASK;
        MTU3.TIER.BYTE &= ~(TGID_INTERRUPT_ENABLE);
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), PA0, Pin45*/
        ICU.IER[MTU4_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT6_MASK;
        MTU4.TIER.BYTE &= ~(TGIA_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRB4:
        ICU.IER[MTU4_INT_ENABLE_REGISTER].BYTE &= ~_HAL_BIT7_MASK;
        MTU4.TIER.BYTE &= ~(TGIB_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), PB1, Pin37*/
        ICU.IER[MTU4_INT_ENABLE_REGISTER +1].BYTE &= ~_HAL_BIT0_MASK;
        MTU4.TIER.BYTE &= (~TGIC_INTERRUPT_ENABLE);
        break;
    case HAL_CH_TGRD4:
        ICU.IER[MTU4_INT_ENABLE_REGISTER +1].BYTE &= ~_HAL_BIT1_MASK;
        MTU4.TIER.BYTE &= ~(TGID_INTERRUPT_ENABLE);
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    TMR_ClearCHF(chId);

    if ( (ICU.IER[MTU0_INT_ENABLE_REGISTER].BYTE & (_HAL_BIT2_MASK | _HAL_BIT3_MASK | _HAL_BIT4_MASK | _HAL_BIT5_MASK)) == 0)
    {
        ICU.IPR[MTU0_BASE_INT_VECTOR].BYTE = 0x00;
    }

    if ( (ICU.IER[MTU1_INT_ENABLE_REGISTER].BYTE & (_HAL_BIT1_MASK | _HAL_BIT2_MASK)) == 0)
    {
        ICU.IPR[MTU1_BASE_INT_VECTOR].BYTE = 0x00;
    }

    if ( (ICU.IER[MTU2_INT_ENABLE_REGISTER].BYTE & (_HAL_BIT5_MASK | _HAL_BIT6_MASK)) == 0)
    {
        ICU.IPR[MTU2_BASE_INT_VECTOR].BYTE = 0x00;
    }

    if ( (ICU.IER[MTU3_INT_ENABLE_REGISTER].BYTE & (_HAL_BIT1_MASK | _HAL_BIT2_MASK | _HAL_BIT3_MASK | _HAL_BIT4_MASK)) == 0)
    {
        ICU.IPR[MTU3_BASE_INT_VECTOR].BYTE = 0x00;
    }

    if ( ((ICU.IER[MTU4_INT_ENABLE_REGISTER].BYTE & (_HAL_BIT6_MASK | _HAL_BIT7_MASK)) == 0) &&
    		( (ICU.IER[MTU4_INT_ENABLE_REGISTER+1].BYTE & (_HAL_BIT0_MASK | _HAL_BIT1_MASK)) == 0) )
    {
        ICU.IPR[MTU4_BASE_INT_VECTOR].BYTE = 0x00;
    }

}/*end TMR_DisableCHI*/

/*
 * Function: TMR_ReadCHF
 * Description: Deve ritornate true se c'è una richiesta di interrupt pendente per il canale indicato
 * Note:
 * Status:
 */
bool_t TMR_ReadCHF( TMR_CHAddressType chID)
{
    bool_t retVal=FALSE;
    switch( chID )
    {
        /*MTUO*/
    case HAL_CH_TGRA0:
        retVal=(ICU.IR[MTU0_BASE_INT_VECTOR + TGIA0_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRB0:
        retVal=(ICU.IR[MTU0_BASE_INT_VECTOR + TGIB0_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRC0:
        retVal=(ICU.IR[MTU0_BASE_INT_VECTOR + TGIC0_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRD0:
        retVal=(ICU.IR[MTU0_BASE_INT_VECTOR + TGID0_INT_SHIF_VECTOR].BYTE);
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        retVal=(ICU.IR[MTU1_BASE_INT_VECTOR + TGIA1_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRB1:
        retVal=(ICU.IR[MTU1_BASE_INT_VECTOR + TGIB1_INT_SHIF_VECTOR].BYTE);
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), P26, Pin16*/
        retVal=(ICU.IR[MTU2_BASE_INT_VECTOR + TGIA2_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRB2:
        retVal=(ICU.IR[MTU2_BASE_INT_VECTOR + TGIB2_INT_SHIF_VECTOR].BYTE);
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), P14, Pin20*/
        retVal=(ICU.IR[MTU3_BASE_INT_VECTOR + TGIA3_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRB3:
        retVal=(ICU.IR[MTU3_BASE_INT_VECTOR + TGIB3_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), PC6, Pin 28*/
        retVal=(ICU.IR[MTU3_BASE_INT_VECTOR + TGIC3_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRD3:
        retVal=(ICU.IR[MTU3_BASE_INT_VECTOR + TGID3_INT_SHIF_VECTOR].BYTE);
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), PA0, Pin45*/
        retVal=(ICU.IR[MTU4_BASE_INT_VECTOR + TGIA4_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRB4:
        retVal=(ICU.IR[MTU4_BASE_INT_VECTOR + TGIB4_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), PB1, Pin37*/
        retVal=(ICU.IR[MTU4_BASE_INT_VECTOR + TGIC4_INT_SHIF_VECTOR].BYTE);
        break;
    case HAL_CH_TGRD4:
        retVal=(ICU.IR[MTU4_BASE_INT_VECTOR + TGID4_INT_SHIF_VECTOR].BYTE);
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return retVal;
}/* end TMR_ReadCHF*/

/*
 * Function: TMR_ClearCHF
 * Description: This function clears the channel interrupt flag both capture and match mode
 * Note: Used in ICU (input capture Unit)
 * Status: Only TGRA interrupt is enable check what interrupt enable
 */
void TMR_ClearCHF( TMR_CHAddressType channel)
{
    if( channel < HAL_CH_MAX )
    {
        ICU.IR[MTU0_BASE_INT_VECTOR + channel].BYTE = 0x00; /* Clear interrupt flag*/
    }
    else
    {
        HAL_ErrorHandler(1);
    }

}/*end TMR_ClearCHF*/

/*
 * Function: TMR_SetThreshold1
 * Description: This function programs the match value
 * Note: Threshold of Counter
 * Status: To Test!
 */
/*MS Ottimizzazione: la funzione è uguale a TMR_SetTimeOn */
void TMR_SetThreshold1( TMR_CHAddressType chId, uint16_t value)
{
    TMR_SetTimeOn(chId, value);
}
#if 0
void TMR_SetThreshold1( TMR_CHAddressType chId, uint16_t value)
{
    switch( chId )
    {
    case HAL_CH_TGRA0:
        MTU0_TGRA = value;
        break;
    case HAL_CH_TGRB0:
        MTU0_TGRB = value;
        break;
    case HAL_CH_TGRC0:
        MTU0_TGRC = value;
        break;
    case HAL_CH_TGRD0:
        MTU0_TGRD = value;
        break;
        /* MTU1 ------------------*/
    case HAL_CH_TGRA1:
        MTU1_TGRA = value;
        break;
    case HAL_CH_TGRB1:
        MTU1_TGRB = value;
        break;
        /* MTU2 ------------------*/
    case HAL_CH_TGRA2:
        MTU2_TGRA = value;
        break;
    case HAL_CH_TGRB2:
        MTU2_TGRB = value;
        break;
        /* MTU3 ------------------*/
    case HAL_CH_TGRA3:
        MTU3_TGRA = value;
        break;
    case HAL_CH_TGRB3:
        MTU3_TGRB = value;
        break;
    case HAL_CH_TGRC3:
        MTU3_TGRC = value;
        break;
    case HAL_CH_TGRD3:
        MTU3_TGRD = value;
        break;
        /* MTU4 ------------------*/
    case HAL_CH_TGRA4:
        MTU4_TGRA = value;
        break;
    case HAL_CH_TGRB4:
        MTU4_TGRB = value;
        break;
    case HAL_CH_TGRC4:
        MTU4_TGRC = value;
        break;
    case HAL_CH_TGRD4:
        MTU4_TGRD = value;
        break;

    default:
        HAL_ErrorHandler(1);
        break;
    }
}/* end TMR_SetThreshold1*/
#endif
/*
 * Function: TMR_SetModuloDfltValue
 * Description: Set Timer Counter Modulo Reg. to the default value
 * Note: Check TGRF if never used!
 * Status: Test me!
 */
void TMR_SetModuloDfltValue(TMR_CNTAddressType tmrId, uint8_t clrSrcId)
{
    TMR_SetModulo(tmrId, 0xFFFF, clrSrcId);
}/* end TMR_SetModuloDfltValue*/

/*
 * Function: Period_ValueType
 * Description:
 * Note: tmr_ptr non usato
 * Status:
 */
Period_ValueType TMR_GetModulo(TMR_CNTAddressType tmdID, uint8_t channel)
{
    uint16_t retValue = 0;
    uint8_t clrValue = GetInternalHWClearSourceValue(tmdID);

    switch( tmdID )
    {
    case HAL_TMR_MTU0:
        switch(clrValue)
        {
        case TCNT_RESET_SOURCE_TGRA:
            retValue = MTU0_TGRA;
            break;
        case TCNT_RESET_SOURCE_TGRB:
            retValue = MTU0_TGRB;
            break;
        case TCNT_RESET_SOURCE_TGRC:
            retValue = MTU0_TGRC;
            break;
        case TCNT_RESET_SOURCE_TGRD:
            retValue = MTU0_TGRD;
            break;
        case TCNT_RESET_SOURCE_OVERFLOW:
            retValue = 0xFFFF;
            break;
        default:
            HAL_ErrorHandler(1);
            break;
        }
        break;
    case HAL_TMR_MTU1:
        switch(clrValue)
        {
        case TCNT_RESET_SOURCE_TGRA:
            retValue = MTU1_TGRA;
            break;
        case TCNT_RESET_SOURCE_TGRB:
            retValue = MTU1_TGRB;
            break;
        case TCNT_RESET_SOURCE_OVERFLOW:
            retValue = 0xFFFF;
            break;
        default:
            HAL_ErrorHandler(1);
            break;
        }
        break;
    case HAL_TMR_MTU2:
        switch(clrValue)
        {
        case TCNT_RESET_SOURCE_TGRA:
            retValue = MTU2_TGRA;
            break;
        case TCNT_RESET_SOURCE_TGRB:
            retValue = MTU2_TGRB;
            break;
        case TCNT_RESET_SOURCE_OVERFLOW:
            retValue = 0xFFFF;
            break;
        default:
            HAL_ErrorHandler(1);
            break;
        }
        break;
    case HAL_TMR_MTU3:
        switch(clrValue)
        {
        case TCNT_RESET_SOURCE_TGRA:
            retValue = MTU3_TGRA;
            break;
        case TCNT_RESET_SOURCE_TGRB:
            retValue = MTU3_TGRB;
            break;
        case TCNT_RESET_SOURCE_TGRC:
            retValue = MTU3_TGRC;
            break;
        case TCNT_RESET_SOURCE_TGRD:
            retValue = MTU3_TGRD;
            break;
        case TCNT_RESET_SOURCE_OVERFLOW:
            retValue = 0xFFFF;
            break;
        default:
            HAL_ErrorHandler(1);
            break;
        }
        break;
    case HAL_TMR_MTU4:
        switch(clrValue)
        {
        case TCNT_RESET_SOURCE_TGRA:
            retValue = MTU4_TGRA;
            break;
        case TCNT_RESET_SOURCE_TGRB:
            retValue = MTU4_TGRB;
            break;
        case TCNT_RESET_SOURCE_TGRC:
            retValue = MTU4_TGRC;
            break;
        case TCNT_RESET_SOURCE_TGRD:
            retValue = MTU4_TGRD;
            break;
        case TCNT_RESET_SOURCE_OVERFLOW:
            retValue = 0xFFFF;
            break;
        default:
            HAL_ErrorHandler(1);
            break;
        }
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    return retValue;
}/*end Period_ValueType*/

/*
 * Function: TMR_GetChannelEdgeMode
 * Description:
 * Note: Check the res value
 * Status: Test me!
 */
uint8_t TMR_GetChannelEdgeMode( TMR_CHAddressType chId)
{
    uint8_t res = 0; /* unknown */
    uint8_t mode = 0;
    volatile uint8_t* p1;
#if 0
    switch (chId)
    {
        /*MTU0*/
    case HAL_CH_TGRA0:
        mode = GET_L_NIBBLE(MTU0.TIORH.BYTE); /* MTU0 IOA*/
        break;
    case HAL_CH_TGRB0:
        mode = GET_H_NIBBLE(MTU0.TIORH.BYTE); /* MTU0 IOB*/
        break;
    case HAL_CH_TGRC0:
        mode = GET_L_NIBBLE(MTU0.TIORL.BYTE); /* MTU0 IOC*/
        break;
    case HAL_CH_TGRD0:
        mode = GET_H_NIBBLE(MTU0.TIORL.BYTE); /* MTU0 IOC*/
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        mode = GET_L_NIBBLE(MTU1.TIOR.BYTE); /* MTU1 IOA*/
        break;
    case HAL_CH_TGRB1:
        mode = GET_H_NIBBLE(MTU1.TIOR.BYTE); /* MTU1 IOB*/
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:
        mode = GET_L_NIBBLE(MTU2.TIOR.BYTE); /* MTU2 IOA*/
        break;
    case HAL_CH_TGRB2:
        mode = GET_H_NIBBLE(MTU2.TIOR.BYTE); /* MTU2 IOB*/
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:
        mode = GET_L_NIBBLE(MTU3.TIORH.BYTE); /* MTU3 IOA*/
        break;
    case HAL_CH_TGRB3:
        mode = GET_H_NIBBLE(MTU3.TIORH.BYTE); /* MTU3 IOB*/
        break;
    case HAL_CH_TGRC3:
        mode = GET_L_NIBBLE(MTU3.TIORL.BYTE); /* MTU3 IOC*/
        break;
    case HAL_CH_TGRD3:
        mode = GET_H_NIBBLE(MTU3.TIORL.BYTE); /* MTU3 IOD*/
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:
        mode = GET_L_NIBBLE(MTU4.TIORH.BYTE); /* MTU4 IOA*/
        break;
    case HAL_CH_TGRB4:
        mode = GET_H_NIBBLE(MTU4.TIORH.BYTE); /* MTU4 IOB*/
        break;
    case HAL_CH_TGRC4:
        mode = GET_L_NIBBLE(MTU4.TIORL.BYTE); /* MTU4 C*/
        break;
    case HAL_CH_TGRD4:
        mode = GET_H_NIBBLE(MTU4.TIORL.BYTE); /* MTU4 IOD*/
        break;
    default:
        HAL_ErrorHandler(1);
    }/*end switch*/
#endif
    /*MS ottimizzazione codice */
    p1 = ChModeRegAddr[chId];
    if(p1 != 0)
    {
        if( GET_BIT(ChModeRegAddr_Nibble, chId))
        {
            mode = GET_H_NIBBLE(*p1);
        }
        else
        {
            mode = GET_L_NIBBLE(*p1);
        }
    }
    else
    {
        HAL_ErrorHandler(1);
    }


    switch( mode )
    {
    case MTU_TIOR_MODE_RISING_EDGE:
        res = 1;/*ICU_EDGE_MODE_RISING;*/
        break;
    case MTU_TIOR_MODE_FALING_EDGE:
        res = 2;/*ICU_EDGE_MODE_FALING;*/
        break;
    case MTU_TIOR_MODE_BOTH_EDGE:
        res = 3;/*ICU_EDGE_MODE_BOTH;*/
        break;
    }
    return res;
}/*end TMT_GetChannelEdgeMode*/

/*
 * Macro: TMR_SetCounterDfltValue
 * Description:
 * Note:
 * Status:
 */
void TMR_SetCounterDfltValue(TMR_CNTAddressType tmrID)
{
    switch( tmrID )
    {
    case HAL_TMR_MTU0:
        MTU0.TCNT = 0x00;
        break;
    case HAL_TMR_MTU1:
        MTU1.TCNT = 0x00;
        break;
    case HAL_TMR_MTU2:
        MTU2.TCNT = 0x00;
        break;
    case HAL_TMR_MTU3:
        MTU3.TCNT = 0x00;
        break;
    case HAL_TMR_MTU4:
        MTU4.TCNT = 0x00;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
}/*end TMR_SetCounterDfltValue*/

/*
 * Function: TMR_GetCounter
 * Description: return the counter value
 * Note:
 * Status: Test me!
 */

IO_HwTimerType TMR_GetCounter( TMR_CNTAddressType tmrId)
{
    IO_HwTimerType return_val = 0;

    switch( tmrId )
    {
    case HAL_TMR_MTU0:
        return_val = MTU0.TCNT;
        break;
    case HAL_TMR_MTU1:
        return_val = MTU1.TCNT;
        break;
    case HAL_TMR_MTU2:
        return_val = MTU2.TCNT;
        break;
    case HAL_TMR_MTU3:
        return_val = MTU3.TCNT;
        break;
    case HAL_TMR_MTU4:
        return_val = MTU4.TCNT;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return (return_val);
}/*end TMR_GetCounter*/

/*
 * Function: TMR_SetCounter
 * Description: Set the timer counter register value
 * Note:
 * Status: Test me!
 */
void TMR_SetCounter( TMR_CNTAddressType tmrId, uint16_t value)
{
    switch(tmrId)
    {
    case HAL_TMR_MTU0:
        MTU0.TCNT = value;
        break;
    case HAL_TMR_MTU1:
        MTU1.TCNT = value;
        break;
    case HAL_TMR_MTU2:
        MTU2.TCNT = value;
        break;
    case HAL_TMR_MTU3:
        MTU3.TCNT = value;
        break;
    case HAL_TMR_MTU4:
        MTU4.TCNT = value;
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
}/*end TMR_SetCounter*/

/*
 * Function: OutputCompareModeUnsupported
 * Description: This function checkes if Output Compare Mode is supported
 * Note:
 * Status:
 */
bool_t OutputCompareModeUnsupported(uint8_t mode, bool_t inversion)
{
    if( mode == TMR_CH_OC_PIN_HIGH ||
            mode == TMR_CH_OC_PIN_LOW  ||
            mode == TMR_CH_OC_TOGGLE_T1 ||
            mode == TMR_CH_OC_OFF )
        return 0;

    return 1;
}/* OutputCompareModeUnsupported*/

/*
 * Function: TMR_SetClockSource
 * Description:  Start the timer, enable on & reset off
 * Status: Complete me and Test me!
 */
void TMR_SetClockSource(TMR_CNTAddressType tmrID, uint8_t source)
{
    if( tmrID >= HAL_NUM_HW_CNTR )
    {
        HAL_ErrorHandler(1);
    }
    else
    {
        if( tmrID > 2)
        {
            MTU.TSTR.BYTE |= (1 << (tmrID+3));
        }
        else
        {
            MTU.TSTR.BYTE |= (1 << tmrID);
        }

    }
}/* TMR_SetClockSource*/


/*
 * Function: TMR_SetClockSource
 * Description:  Start the timer, enable on & reset off
 * Status: Complete me and Test me!
 */
static uint32_t TMR_GetMultiBmp(uint32_t uBmp)
{
	uint32_t i=0, msk =0, bmp=0, tmp=0;

	for(i=0, msk=1; i<HAL_NUM_HW_CNTR; i++, msk<<=1)
	{
		if( uBmp & msk)
		{
			if(i>2)
			{
				tmp = (1 << (i+3));
			}
			else
			{
				tmp = (1 << i);
			}
			bmp |= tmp;
		}
	}
	return bmp;
}

/*
 * Function: TMR_StopClockSourceMulti
 * Description:  stops the countes & reset its value
 * Status: Complete me and Test me!
 */
void TMR_StopClockSourceMulti(uint32_t uBmp)
{
	uint32_t i=0, msk =0, bmp=0;

	bmp = TMR_GetMultiBmp(uBmp);

	MTU.TSTR.BYTE &= ~bmp;

	for(i=0, msk=1; i<HAL_NUM_HW_CNTR; i++, msk<<=1)
	{
		if( uBmp & msk)
		{
			TMR_SetCounter(i, 0);
		}
	}

}/* TMR_SetClockSource*/

/*
 * Function: TMR_SetClockSourceMulti
 * Description:  Starts the timer &  enable it
 * Status: Complete me and Test me!
 */
void TMR_SetClockSourceMulti(uint32_t uBmp)
{
	uint32_t bmp=0;
	bmp = TMR_GetMultiBmp(uBmp);

	MTU.TSTR.BYTE |= bmp;
}/* TMR_SetClockSourceMulti*/

/*
 * Function: TMR_SetChannelOutputMode
 * Description: This function configures the output mode for the match event and associates the function to the pin if needed
 * Note:
 * Status: Test me!
 */
void TMR_SetChannelOutputMode( TMR_CHAddressType chId, uint8_t mode, bool_t inversion)
{
    volatile unsigned char *p1=0;
    volatile unsigned char *p2=0;
    volatile unsigned char *p3=0;
    unsigned char value=0;
    unsigned char bitNum=0;

    /**/
    switch( mode )
    {
    case TMR_CH_OC_OFF:	/*output off*/
        /*this should not be done because already done in TMR_SetChannelOutputCompare*/
    	mode = MTUO_TIOR_MODE_PIN_IO;
        break;
    case TMR_CH_OC_TOGGLE_T1:
        if( inversion )
        {
            mode = MTUO_TIOR_MODE_OUTPUT_0T;
        }
        else
        {
            mode=MTUO_TIOR_MODE_OUTPUT_1T;
        }
        break;
    case TMR_CH_OC_PIN_HIGH:
        mode = MTUO_TIOR_MODE_OUTPUT_01;
        break;
    case TMR_CH_OC_PIN_LOW:
        mode = MTUO_TIOR_MODE_OUTPUT_10;
        break;
    default:
        mode = MTUO_TIOR_MODE_OUTPUT_PROIBITED;
        break;
    }/*end switch*/


    if(mode != MTUO_TIOR_MODE_OUTPUT_PROIBITED)
    {
    	switch (chId)
		{
			/*MTU0*/
		case HAL_CH_TGRA0:	/* (MTU0, MTIOC0A),PB3,Pin36*/
			p1 = &PORTB.PDR.BYTE;
			p2 = &PORTB.PMR.BYTE;
			value = 0x01; /* PB3 is select as TMO0 function*/
			bitNum = 3;
			p3 = &MPC.PB3PFS.BYTE;

			break;
		case HAL_CH_TGRB0:/* (MTUO, MTI0C0B), (P15,Pin19), (PA1,Pin44)*/
#if defined(HAL_CH_TGRB0_ON_PA1)
			p1 = & PORTA.PDR.BYTE;
			p2 = & PORTA.PMR.BYTE;
			value = 0x01;
			bitNum = 1;
			p3 = &	MPC.PA1PFS.BYTE;
#else
			p1 = &PORT1.PDR.BYTE;
			p2 = &PORT1.PMR.BYTE;
			value = 0x01; /* P15 is select as MTI0C0B*/
			bitNum = 5;
			p3 = &MPC.P15PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRC0:/*((MTUO, MTIOCOC), P32, Pin12*/
#if defined(HAL_CH_TGRC0_ON_PB1)
			p1 = & PORTB.PDR.BYTE;
			p2 = & PORTB.PMR.BYTE;
			value = 0x01;
			bitNum = 1;
			p3 = &	MPC.PB1PFS.BYTE;
#else
			p1 = &PORT3.PDR.BYTE;
			p2 = &PORT3.PMR.BYTE;
			value = 0x01;
			bitNum = 2;
			p3 = &MPC.P32PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRD0: /*((MTUO, MTIOCOD),(PA3,Pin43)*/
			p1 = &PORTA.PDR.BYTE;
			p2 = &PORTA.PMR.BYTE;
			value = 0x01;
			bitNum = 3;
			p3 = &MPC.PA3PFS.BYTE;
			break;
			/*MTU1*/
		case HAL_CH_TGRA1:/* (MTU1, MTIOC1A, PE4, Pin47)*/
			p1 = &PORTE.PDR.BYTE;
			p2 = &PORTE.PMR.BYTE;
			value = 0x02;
			bitNum = 4;
			p3 = &MPC.PE4PFS.BYTE;
			break;
		case HAL_CH_TGRB1:/* (MTU1, MTIOC1B), (PB5, Pin35)*/
			p1 = &PORTB.PDR.BYTE;
			p2 = &PORTB.PMR.BYTE;
			value = 0x02;
			bitNum = 5;
			p3 = &MPC.PB5PFS.BYTE;
			break;
			/*MTU2*/
		case HAL_CH_TGRA2:/*(MTU2, MTIOC2A), (P26,Pin16), (PB5,Pin35)*/
#if defined(HAL_CH_TGRA2_ON_PB5)
			p1 = & PORTB.PDR.BYTE;
			p2 = & PORTB.PMR.BYTE;
			value = 0x01;
			bitNum = 5;
			p3 = &	MPC.PB5PFS.BYTE;
#else
			p1 = &PORT2.PDR.BYTE;
			p2 = &PORT2.PMR.BYTE;
			value = 0x01;
			bitNum = 6;
			p3 = &MPC.P26PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRB2:/*(MTU2, MTIOC2B), (P27,Pin15), (PE5,Pin46)*/
#if defined(HAL_CH_TGRB2_ON_PE5)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x02;
			bitNum = 5;
			p3 = &	MPC.PE5PFS.BYTE;
#else
			p1 = &PORT2.PDR.BYTE;
			p2 = &PORT2.PMR.BYTE;
			value = 0x01;
			bitNum = 7;
			p3 = &MPC.P27PFS.BYTE;
#endif
			break;
			/*MTU3*/
		case HAL_CH_TGRA3:/*(MTU3, MTIOC3A), (P14,Pin20),(PC7,Pin27),(P17,Pin17)*/
#if defined(HAL_CH_TGRA3_ON_PC7)
			p1 = & PORTC.PDR.BYTE;
			p2 = & PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 7;
			p3 = &	MPC.PC7PFS.BYTE;
#elif defined(HAL_CH_TGRA3_ON_P17)
			p1 = & PORT1.PDR.BYTE;
			p2 = & PORT1.PMR.BYTE;
			value = 0x01;
			bitNum = 7;
			p3 = &	MPC.P17PFS.BYTE;
#else
			p1 = &PORT1.PDR.BYTE;
			p2 = &PORT1.PMR.BYTE;
			value = 0x01;
			bitNum = 4;
			p3 = &MPC.P14PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRB3:/*(MTU3, MTIOC3B), (PC5,Pin29),(P17,Pin17),(PB7,Pin33)*/
#if defined(HAL_CH_TGRB3_ON_PB7)
			p1 = & PORTB.PDR.BYTE;
			p2 = & PORTB.PMR.BYTE;
			value = 0x01;
			bitNum = 7;
			p3 = &	MPC.PB7PFS.BYTE;
#elif defined(HAL_CH_TGRB3_ON_P17)
			p1 = & PORT1.PDR.BYTE;
			p2 = & PORT1.PMR.BYTE;
			value = 0x02;
			bitNum = 7;
			p3 = &	MPC.P17PFS.BYTE;
#else
			p1 = &PORTC.PDR.BYTE;
			p2 = &PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 5;
			p3 = &MPC.PC5PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRC3:/* (MTU3, MTIOC3C), (PC6,Pin 28), (P16, Pin18),*/
#if defined(HAL_CH_TGRC3_ON_P16)
			p1 = & PORT1.PDR.BYTE;
			p2 = & PORT1.PMR.BYTE;
			value = 0x01;
			bitNum = 6;
			p3 = &	MPC.P16PFS.BYTE;
#else
			p1 = &PORTC.PDR.BYTE;
			p2 = &PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 6;
			p3 = &MPC.PC6PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRD3:/* (MTU3, MTIOC3D), (PB6, Pin34),P16,Pin 18), (PC4, Pin30)*/
#if defined(HAL_CH_TGRD3_ON_P16)
			p1 = & PORT1.PDR.BYTE;
			p2 = & PORT1.PMR.BYTE;
			value = 0x02;
			bitNum = 6;
			p3 = &	MPC.P16PFS.BYTE;
#elif defined(HAL_CH_TGRD3_ON_PC4)
			p1 = & PORTC.PDR.BYTE;
			p2 = & PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 4;
			p3 = &	MPC.PC4PFS.BYTE;
#else
			p1 = &PORTB.PDR.BYTE;
			p2 = &PORTB.PMR.BYTE;
			value = 0x01;
			bitNum = 6;
			p3 = &MPC.PB6PFS.BYTE;
#endif
			MTU.TOER.BIT.OE3D = 1;
			break;
			/*MTU4*/
		case HAL_CH_TGRA4:/* (MTU4, MTIOC4A), (PA0,Pin45),(PB3,Pin36),(PE2,Pin49),*/
#if defined(HAL_CH_TGRA4_ON_PB3)
			p1 = & PORTB.PDR.BYTE;
			p2 = & PORTB.PMR.BYTE;
			value = 0x02;
			bitNum = 3;
			p3 = &	MPC.PB3PFS.BYTE;
#elif defined(HAL_CH_TGRA4_ON_PE2)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x01;
			bitNum = 2;
			p3 = &	MPC.PE2PFS.BYTE;
#else
			p1 = &PORTA.PDR.BYTE;
			p2 = &PORTA.PMR.BYTE;
			value = 0x01;
			bitNum = 0;
			p3 = &MPC.PA0PFS.BYTE;
#endif
			break;
		case HAL_CH_TGRB4:/* (MTU4, MTIOC4B), (PC2,Pin32),(P30,Pin14),(P54,Pin26),(PE3,Pin48)*/
#if defined(HAL_CH_TGRB4_ON_P30)
			p1 = & PORT3.PDR.BYTE;
			p2 = & PORT3.PMR.BYTE;
			value = 0x01;
			bitNum = 0;
			p3 = &	MPC.P30PFS.BYTE;
#elif defined(HAL_CH_TGRB4_ON_P54)
			p1 = & PORT5.PDR.BYTE;
			p2 = & PORT5.PMR.BYTE;
			value = 0x01;
			bitNum = 4;
			p3 = &	MPC.P54PFS.BYTE;
#elif defined(HAL_CH_TGRB4_ON_PE3)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x01;
			bitNum = 3;
			p3 = &	MPC.PE3PFS.BYTE;
#else
			p1 = &PORTC.PDR.BYTE;
			p2 = &PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 2;
			p3 = &MPC.PC2PFS.BYTE;
#endif
			MTU.TOER.BIT.OE4B = 1;
			break;
		case HAL_CH_TGRC4:/* (MTU4, MTIOC4C), (PB1, Pin37),(PE5,Pin46),(PE1,Pin50)*/
#if defined(HAL_CH_TGRC4_ON_PE5)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x01;
			bitNum = 5;
			p3 = &	MPC.PE5PFS.BYTE;
#elif defined(HAL_CH_TGRC4_ON_PE1)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x01;
			bitNum = 1;
			p3 = &	MPC.PE1PFS.BYTE;
#else
			p1 = &PORTB.PDR.BYTE;
			p2 = &PORTB.PMR.BYTE;
			value = 0x02;
			bitNum = 1;
			p3 = &MPC.PB1PFS.BYTE;
#endif
			MTU.TOER.BIT.OE4C = 1;
			break;
		case HAL_CH_TGRD4:/* (MTU4, MTIOC4D), (P31, Pin13),(P55,Pin25),(PC3,Pin31), (PE4,Pin47)*/
#if defined(HAL_CH_TGRD4_ON_P55)
			p1 = & PORT5.PDR.BYTE;
			p2 = & PORT5.PMR.BYTE;
			value = 0x01;
			bitNum = 5;
			p3 = &	MPC.P55PFS.BYTE;
#elif defined(HAL_CH_TGRD4_ON_PC3)
			p1 = & PORTC.PDR.BYTE;
			p2 = & PORTC.PMR.BYTE;
			value = 0x01;
			bitNum = 3;
			p3 = &	MPC.PC3PFS.BYTE;
#elif defined(HAL_CH_TGRD4_ON_PE4)
			p1 = & PORTE.PDR.BYTE;
			p2 = & PORTE.PMR.BYTE;
			value = 0x01;
			bitNum = 4;
			p3 = &	MPC.PE4PFS.BYTE;
#else
			p1 = &PORT3.PDR.BYTE;
			p2 = &PORT3.PMR.BYTE;
			value = 0x02;
			bitNum = 1;
			p3 = &MPC.P31PFS.BYTE;
#endif
			MTU.TOER.BIT.OE4D = 1;
			break;
		default:
			HAL_ErrorHandler(0);
			break;
		}/*end switch*/

		if( p1 != 0 && p2 != 0 && p3 != 0 )
		{
			if (mode == MTUO_TIOR_MODE_PIN_IO)
			{
				value = 0;
				CLEAR_BIT(*p1, bitNum);				/* as input*/
				CLEAR_BIT(*p2, bitNum); 			/* Use pin as general I/O port */
				mode = MTUO_TIOR_MODE_OUTPUT_PROIBITED;
			}
			else
			{
				SET_BIT(*p1, bitNum);				/* as output*/
				SET_BIT(*p2, bitNum); 				/* Use pin as I/O port for peripheral functions*/
			}
			DISABLE_PORT_REGISTER_WRITE_PROTECTION();
			*p3 = value;
			ENABLE_PORT_REGISTER_WRITE_PROTECTION();
		}
	}


    p1 = ChModeRegAddr[chId];
    if(p1 != 0)
    {
        if( GET_BIT(ChModeRegAddr_Nibble, chId))
        {
            SET_H_NIBBLE(*p1, mode);
        }
        else
        {
            SET_L_NIBBLE(*p1, mode);
        }
    }

}/* end TMR_SetChannelOutputMode*/

/*
 * Function: TMR_SetChannelDefaultMode
 * Description: This function configures default modes for the channel, by clearing:
 * 				- reset and stop on MCR
 * 				- capture edges on CCR
 * 				- external match control on EMR (if the timer is not running!!)
 * 				- match mode in PWM register
 * Note: This function set at defautl value MTUx register
 * Status:
 */
static void TMR_SetChannelDefaultMode( TMR_CHAddressType chId)
{
    volatile uint8_t* p1;


#if 0
    switch (chId)
    {
    case HAL_CH_TGRA0:
        SET_L_NIBBLE(MTU0.TIORH.BYTE,MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRB0:
        SET_H_NIBBLE(MTU0.TIORH.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRC0:
        SET_L_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRD0:
        SET_H_NIBBLE(MTU0.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
        /*MTU1*/
    case HAL_CH_TGRA1:
        SET_L_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRB1:
        SET_H_NIBBLE(MTU1.TIOR.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
        /*MTU2*/
    case HAL_CH_TGRA2:
        SET_L_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRB2:
        SET_H_NIBBLE(MTU2.TIOR.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
        /*MTU3*/
    case HAL_CH_TGRA3:
        SET_L_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRB3:
        MTU.TOER.BIT.OE3B = 0;
        SET_H_NIBBLE(MTU3.TIORH.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRC3:
        SET_L_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRD3:
        MTU.TOER.BIT.OE3D = 0;
        SET_H_NIBBLE(MTU3.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
        /*MTU4*/
    case HAL_CH_TGRA4:
        MTU.TOER.BIT.OE4A = 0;
        SET_L_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRB4:
        MTU.TOER.BIT.OE4B = 0;
        SET_L_NIBBLE(MTU4.TIORH.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRC4:
        MTU.TOER.BIT.OE4C = 0;
        SET_L_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    case HAL_CH_TGRD4:
        MTU.TOER.BIT.OE4D = 0;
        SET_H_NIBBLE(MTU4.TIORL.BYTE, MTU_TIOR_MODE_OUTPUT_PROIBITED);
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/
#endif
    p1 = ChModeRegAddr[chId];
    if( GET_BIT(ChModeRegAddr_Nibble, chId))
    {
        SET_H_NIBBLE(*p1, MTU_TIOR_MODE_OUTPUT_PROIBITED);
    }
    else
    {
        SET_L_NIBBLE(*p1, MTU_TIOR_MODE_OUTPUT_PROIBITED);
    }
}/*end TMR_SetChannelDefaultMode*/

/*
* Date: 05/07/2012
* Function: TMR_SetTimerDefaultMode
* Description: Set the TMR registers at default value.
* Notes:
	- AE: To verify the correct behaviour
*/
void TMR_SetTimerDefaultMode( TMR_CNTAddressType tmdID) /* TODO: cosa fa opera sul canale osul timer??????*/
{
    volatile uint8_t *p1=0;
    uint8_t i = 0;
    switch(tmdID)
    {
    case HAL_TMR_MTU0:
        p1 = &MTU0.TCR.BYTE;
#if 0
        for(i = 0; i<5 ; i++)
        {
            if(i != 5)
                *(&MTU0.TCR.BYTE + i) = 0x00;
        }
#endif
        break;
    case HAL_TMR_MTU1:
        p1 = & MTU1.TCR.BYTE;
#if 0
        for(i = 0; i<5 ; i++)
        {
            if(i != 5)
                *(&MTU1.TCR.BYTE + i) = 0x00;
        }
#endif
        break;
    case HAL_TMR_MTU2:
        p1 = & MTU2.TCR.BYTE;
#if 0
        for(i = 0; i<5 ; i++)
        {
            if(i != 5)
                *(&MTU2.TCR.BYTE + i) = 0x00;
        }
#endif
        break;
    case HAL_TMR_MTU3:
        p1 = & MTU3.TCR.BYTE;
#if 0
        for(i = 0; i<5 ; i++)
        {
            if(i != 5)
                *(&MTU3.TCR.BYTE + i) = 0x00;
        }
#endif
        break;
    case HAL_TMR_MTU4:
        p1 = & MTU4.TCR.BYTE;
#if 0
        for(i = 0; i<5 ; i++)
        {
            if(i != 5)
                *(&MTU4.TCR.BYTE + i) = 0x00;
        }
#endif
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }/*end switch*/

    if( p1 != 0)
    {
        for(i = 0; i<5 ; i++)
        {
            *(p1 + i) = 0x00;
        }
    }

}/*end TMR_SetChannelDefaultMode*/

/*
 * Function: TMR_DeInitChannel
 * Description: This function is used to bring the channel to its orginal state
 * Note:
 * Status: To do!
 */
void TMR_DeInitChannel( TMR_CHAddressType pChannel)
{
    TMR_DisableCHI(pChannel);
    TMR_SetChannelDefaultMode(pChannel); /*TODO: verificare*/
    TMR_ClearCHF(pChannel);
    TMR_ClearChannelPulses(pChannel);
}/* TMR_DeInitChannel*/


/*
 * Function: TMR_GetDelta
 * Description:
 */
Period_ValueType TMR_GetDelta(TMR_CNTAddressType trmId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR)
{
    Period_ValueType period;
    if( CntrVal_CURR > CntrVal_OLD)
    {
        return ((Period_ValueType) ((uint16_t) CntrVal_CURR - (uint16_t) CntrVal_OLD));
    }

    period = TMR_GetModulo(trmId, 0);
    return ((period - CntrVal_OLD)+CntrVal_CURR);

}/* end Period_ValueType*/


/* ********************************************************************************************** */
/*                                  END TMR Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN I2C Class HAL Support                                   */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN IRQ Class HAL Support                                   */
/* ********************************************************************************************** */
#if defined(HAL_IRQ_SUPPORT)

/*
* Function: IRQ_Init
* Description:
*
*/
void IRQ_Init(uint8_t irq_id)
{
    switch(irq_id)
    {
    case RX210_IRQ0_PH1:
        DISABLE_PORT_REGISTER_WRITE_PROTECTION();
        _HAL_RX210_PH1_PORT_PIN_AS_IRQ0;
        ENABLE_PORT_REGISTER_WRITE_PROTECTION();
        ICU.IPR[_HAL_RX210_ICU_IRQ0_VECTOR].BYTE = IRQ0_INTERRUPT_PRIORITY;
        break;
    case RX210_IRQ1_PH2:
        DISABLE_PORT_REGISTER_WRITE_PROTECTION();
        _HAL_RX210_PH2_PORT_PIN_AS_IRQ1;
        ENABLE_PORT_REGISTER_WRITE_PROTECTION();
        ICU.IPR[_HAL_RX210_ICU_IRQ1_VECTOR].BYTE = IRQ1_INTERRUPT_PRIORITY;
        break;
    case RX210_IRQ5_P15:
        DISABLE_PORT_REGISTER_WRITE_PROTECTION();
        _HAL_RX210_P15_PORT_PIN_AS_IRQ5;
        ENABLE_PORT_REGISTER_WRITE_PROTECTION();
        ICU.IPR[_HAL_RX210_ICU_IRQ5_VECTOR].BYTE = IRQ5_INTERRUPT_PRIORITY;
        break;
    case RX210_IRQ6_PA3:
        DISABLE_PORT_REGISTER_WRITE_PROTECTION();
        _HAL_RX210_PA3_PORT_PIN_AS_IRQ6;
        ENABLE_PORT_REGISTER_WRITE_PROTECTION();
        ICU.IPR[_HAL_RX210_ICU_IRQ6_VECTOR].BYTE = IRQ6_INTERRUPT_PRIORITY;
        break;
    case RX210_ELCIRQ0_PB0:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB0_MASK;
        break;
    case RX210_IRQ_PB1:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB1_MASK;
        break;
    case RX210_IRQ_PB3:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB3_MASK;
        break;
    case RX210_ELCIRQ5_PB5:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB5_MASK;
        break;
    case RX210_ELCIRQ6_PB6:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB6_MASK;
        break;
    case RX210_IRQ_PB7:
        ENABLE_ELC_HW_MODULE();
        ELC.ELSR[18].BYTE |= 0x63;
        ELC.PGR1.BYTE |= ELC_PB7_MASK;
        break;
    }/* end swith irq_id*/
    if(irq_id == RX210_ELCIRQ0_PB0 || irq_id == RX210_IRQ_PB1 || irq_id == RX210_IRQ_PB3 || irq_id == RX210_ELCIRQ5_PB5 || irq_id == RX210_ELCIRQ6_PB6 || irq_id == RX210_IRQ_PB7)
    {
        ELC.ELCR.BYTE |= 0x80;
        ICU.IPR[_HAL_RX210_ICU_ELC_ELSR18L_VECTOR].BYTE = ELC_ELSR18L_INTERRUPT_PRIORITY;
    }/*end if*/
}/*end IRQ_INIT*/

/*
 * Function: IRQ_SetEdge
 * Description:
 * Note:
 * Status:
 */

void IRQ_SetEdge(uint8_t edge, uint8_t irq_id)
{
    /*MS non ottimizzare peggiora la size */
    switch(irq_id)
    {
    case RX210_IRQ0_PH1:
        if(edge)
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ0) = _HAL_RX210_DETECTION_SENSE_RAISING_EDGE;
        else
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ0) = _HAL_RX210_DETECTION_SENSE_FALLING_EDGE;
        break;
    case RX210_IRQ1_PH2:
        if(edge)
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ1) = _HAL_RX210_DETECTION_SENSE_RAISING_EDGE;
        else
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ1) = _HAL_RX210_DETECTION_SENSE_FALLING_EDGE;
        break;
    case RX210_IRQ5_P15:
        if(edge)
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ5) = _HAL_RX210_DETECTION_SENSE_RAISING_EDGE;
        else
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ5) = _HAL_RX210_DETECTION_SENSE_FALLING_EDGE;
        break;
    case RX210_IRQ6_PA3:
        if(edge)
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ6) = _HAL_RX210_DETECTION_SENSE_RAISING_EDGE;
        else
            _HAL_RX210_IRQ_CONTROL_REGISTER(IRQ6) = _HAL_RX210_DETECTION_SENSE_FALLING_EDGE;
        break;
    }
    if(    irq_id == RX210_ELCIRQ0_PB0
            || irq_id == RX210_IRQ_PB1
            || irq_id == RX210_IRQ_PB3
            || irq_id == RX210_ELCIRQ5_PB5
            || irq_id == RX210_ELCIRQ6_PB6
            || irq_id == RX210_IRQ_PB7)
    {
        if(edge)
            ELC.PGC1.BYTE = _HAL_RX210_DETECTION_SENSE_ELC_RAISING_EDGE;
        else
            ELC.PGC1.BYTE = _HAL_RX210_DETECTION_SENSE_ELC_FALLING_EDGE;
    }
}/*end IRQ_SEtEdge*/

/*
 * Macro: IRQ_IntEnable(enable)
 * Decritpion:
 * Note:
 * 	- enable possible vale are 1/0 enable/disable
 * Status:
 * 	- IRQ1; IRQ5, IRQ6 inserted
 */

void IRQ_IntEnable(uint8_t enable, uint8_t irq_id)
{
    if(enable)
    {
        switch(irq_id)
        {
        case RX210_IRQ0_PH1:
            _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ0_VECTOR) = 0x00;
            _HAL_RX210_DIGITAL_FILTER_ENABLE_REGISTER |= IRQ0_DIGITAL_FILTER_MASK;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) |= _HAL_RX210_ICU_IRQ0_IER_MASK;
            break;
        case RX210_IRQ1_PH2:
            _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ1_VECTOR) = 0x00;
            _HAL_RX210_DIGITAL_FILTER_ENABLE_REGISTER |= IRQ1_DIGITAL_FILTER_MASK;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) |= _HAL_RX210_ICU_IRQ1_IER_MASK;
            break;
        case RX210_IRQ5_P15:
            _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ5_VECTOR) = 0x00;
            _HAL_RX210_DIGITAL_FILTER_ENABLE_REGISTER |= IRQ5_DIGITAL_FILTER_MASK;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) |= _HAL_RX210_ICU_IRQ5_IER_MASK;
            break;
        case RX210_IRQ6_PA3:
            _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ6_VECTOR) = 0x00;
            _HAL_RX210_DIGITAL_FILTER_ENABLE_REGISTER |= IRQ6_DIGITAL_FILTER_MASK;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) |= _HAL_RX210_ICU_IRQ6_IER_MASK;
            break;
        }/*end switch irq_id*/
        if(irq_id == RX210_ELCIRQ0_PB0 || irq_id == RX210_IRQ_PB1 || irq_id == RX210_IRQ_PB3 || irq_id == RX210_ELCIRQ5_PB5 || irq_id == RX210_ELCIRQ6_PB6 || irq_id == RX210_IRQ_PB7)
        {
            _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_ELC_ELSR18L_VECTOR) = 0x00;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_ELC_IER_REGISTER) |= _HAL_RX210_ICU_ELC_ELSR18L_IER_MASK;
        }
    }/*end if enable*/
}/*end IRQ_IntEnable*/

/*
 * Macro: IRQ_DeInit
 * Description: Clear Interrupt Request Status and Control Register & Disable Bus clock to the IRQ module.
 * Note: De Init all IRQ
 * Status:
 */
void IRQ_DeInit(uint8_t irq_id)
{
    switch (irq_id)
    {
    case RX210_IRQ0_PH1:
        _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) &= ~_HAL_RX210_ICU_IRQ0_IER_MASK;
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ0_VECTOR) = 0x00;
        ICU.IPR[_HAL_RX210_ICU_IRQ0_VECTOR].BYTE = 0x00;
        _HAL_RX210_PH1_PORT_PIN_AS_IO;
        break;
    case RX210_IRQ1_PH2:
        _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) &= ~_HAL_RX210_ICU_IRQ1_IER_MASK;
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ1_VECTOR) = 0x00;
        ICU.IPR[_HAL_RX210_ICU_IRQ1_VECTOR].BYTE = 0x00;
        _HAL_RX210_PH2_PORT_PIN_AS_IO;
        break;
    case RX210_IRQ5_P15:
        _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) &= ~_HAL_RX210_ICU_IRQ5_IER_MASK;
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ5_VECTOR) = 0x00;
        ICU.IPR[_HAL_RX210_ICU_IRQ5_VECTOR].BYTE = 0x00;
        _HAL_RX210_P15_PORT_PIN_AS_IO;
        break;
    case RX210_IRQ6_PA3:
        _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_IRQ_IER_REGISTER) &= ~_HAL_RX210_ICU_IRQ6_IER_MASK;
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ6_VECTOR) = 0x00;
        ICU.IPR[_HAL_RX210_ICU_IRQ6_VECTOR].BYTE = 0x00;
        _HAL_RX210_PA3_PORT_PIN_AS_IO;
        break;
    }/*end switch*/
    if(   irq_id == RX210_ELCIRQ0_PB0
            || irq_id == RX210_IRQ_PB1
            || irq_id == RX210_IRQ_PB3
            || irq_id == RX210_ELCIRQ5_PB5
            || irq_id == RX210_ELCIRQ6_PB6
            || irq_id == RX210_IRQ_PB7)
    {
        if(ELC.PGR1.BYTE != 0x00)
        {
            switch(irq_id)
            {
            case RX210_ELCIRQ0_PB0:
                ELC.PGR1.BYTE &= ~ELC_PB0_MASK;
                break;
            case RX210_IRQ_PB1:
                ELC.PGR1.BYTE &= ~ELC_PB1_MASK;
                break;
            case RX210_IRQ_PB3:
                ELC.PGR1.BYTE &= ~ELC_PB3_MASK;
                break;
            case RX210_ELCIRQ5_PB5:
                ELC.PGR1.BYTE &= ~ELC_PB5_MASK;
                break;
            case RX210_ELCIRQ6_PB6:
                ELC.PGR1.BYTE &= ~ELC_PB6_MASK;
                break;
            case RX210_IRQ_PB7:
                ELC.PGR1.BYTE &= ~ELC_PB7_MASK;
                break;
            }/*end switch*/
        }
        else
        {
            DISABLE_ELC_HW_MODULE();
            ICU.IPR[_HAL_RX210_ICU_ELC_ELSR18L_VECTOR].BYTE = 0x00;
            _HAL_RX210_INTERRUPT_REQUEST_ENABLE_REGISTER(_HAL_RX210_ICU_ELC_IER_REGISTER) &= ~_HAL_RX210_ICU_ELC_ELSR18L_IER_MASK;
        }
    }/*end if*/
}/*end IRQ_DeInit*/

/*
 * Function: IRQ_Acknowledge
 * Description:
 * Note: Two NOP cycles are reccomended after the clearing of the interrupt flag in order to take into account the sync mechanism between NVIC and GPIO domain
 * Status:
 */
void IRQ_Acknowledge(uint8_t irq_id)
{
    switch(irq_id)
    {
    case RX210_IRQ0_PH1:
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ0_VECTOR) = 0x00;
        break;
    case RX210_IRQ1_PH2:
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ1_VECTOR) = 0x00;
        break;
    case RX210_IRQ5_P15:
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ5_VECTOR) = 0x00;
        break;
    case RX210_IRQ6_PA3:
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_IRQ6_VECTOR) = 0x00;
        break;
    }/*end switch*/
    if(    irq_id == RX210_ELCIRQ0_PB0
            || irq_id == RX210_IRQ_PB1
            || irq_id == RX210_IRQ_PB3
            || irq_id == RX210_ELCIRQ5_PB5
            || irq_id == RX210_ELCIRQ6_PB6
            || irq_id == RX210_IRQ_PB7)
    {
        _HAL_RX210_IRQ_INTERRUPT_REQUEST_REGISTER(_HAL_RX210_ICU_ELC_ELSR18L_VECTOR) = 0x00;
    }
}/*end IRQ_Acknowledge*/

/*
 * Function: IRQ_GetEdge
 * Description: GET the interrupt setting edge
 * \return  1 if RAISING_EDGE  0 not rising
 * Note:
 * Status:
 */
bool_t IRQ_GetEdge(uint8_t irq_id)
{
	bool_t retVal = 0;

	PLT_ASSERT( irq_id >= RX210_IRQ0_PH1 && irq_id <= RX210_IRQ_PB7)

    switch(irq_id)
    {
    case RX210_IRQ0_PH1:
        if(_HAL_RX210_IRQ_CONTROL_REGISTER(IRQ0) == _HAL_RX210_DETECTION_SENSE_RAISING_EDGE)
            retVal = 1;
        break;
    case RX210_IRQ1_PH2:
        if(_HAL_RX210_IRQ_CONTROL_REGISTER(IRQ1) == _HAL_RX210_DETECTION_SENSE_RAISING_EDGE)
            retVal = 1;
        break;
    case RX210_IRQ5_P15:
        if(_HAL_RX210_IRQ_CONTROL_REGISTER(IRQ5) == _HAL_RX210_DETECTION_SENSE_RAISING_EDGE)
            retVal = 1;
        break;
    case RX210_IRQ6_PA3:
        if(_HAL_RX210_IRQ_CONTROL_REGISTER(IRQ6) == _HAL_RX210_DETECTION_SENSE_RAISING_EDGE)
            retVal = 1;
        break;
    }/*end switch*/
    if(irq_id == RX210_ELCIRQ0_PB0 || irq_id == RX210_IRQ_PB1 || irq_id == RX210_IRQ_PB3 || irq_id == RX210_ELCIRQ5_PB5 || irq_id == RX210_ELCIRQ6_PB6 || irq_id == RX210_IRQ_PB7)
    {
        if((ELC.PGC1.BYTE &0x03) == _HAL_RX210_DETECTION_SENSE_ELC_RAISING_EDGE)
            retVal = 1;
    }
    return(retVal);
}/*end IRQ_GetEdge*/

/*
 * Function: IRQ_pinIsHigh
 * Description: Check if the IRQ pin is High
 * Note:
 * Status
 */
bool_t IRQ_pinIsHigh(uint8_t irq_id)
{
	bool_t retVal = 0;
	PLT_ASSERT( irq_id >= RX210_IRQ0_PH1 && irq_id <= RX210_IRQ_PB7)

    switch(irq_id)
    {
    case RX210_IRQ0_PH1:
        if(IRQ0_PIN_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ1_PH2:
        if(IRQ1_PIN_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ5_P15:
        if(IRQ5_PIN_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ6_PA3:
        if(IRQ6_PIN_STATUS)
            retVal = 1;
        break;
    case RX210_ELCIRQ0_PB0:
        if(ELC_IRQ_PB0_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ_PB1:
        if(ELC_IRQ_PB1_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ_PB3:
        if(ELC_IRQ_PB3_STATUS)
            retVal = 1;
        break;
    case RX210_ELCIRQ5_PB5:
        if(ELC_IRQ_PB5_STATUS)
            retVal = 1;
        break;
    case RX210_ELCIRQ6_PB6:
        if(ELC_IRQ_PB6_STATUS)
            retVal = 1;
        break;
    case RX210_IRQ_PB7:
        if(ELC_IRQ_PB7_STATUS)
            retVal = 1;
        break;
    }/*end switch*/
    return(retVal);
}/*end IRQ_pinIsHigh*/
#endif /*HAL_IRQ SUPPORT*/
/* ********************************************************************************************** */
/*                                  END IRQ Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN UART Class HAL Support                                   */
/* ********************************************************************************************** */

#if 0
/* _HAL_RX200_UART_SSR_RDRF_MASK not available */
/*
 * Function: UART_RxByte
 * Desctription:
 */
int UART_RxByte(int dev, char * chPtr )
{
    unsigned int status;
    int retVal = 0;
    switch(dev)
    {
    case 0:
        status = _HAL_RX200_UART_SSR_REG(0);
        if( status & _HAL_RX200_UART_SSR_ORER_MASK )  /* clear the overrun error*/
        {
            _HAL_RX200_UART_SSR_REG(0) = 0;
        }
        if( status & _HAL_RX200_UART_SSR_RDRF_MASK )
        {
            *chPtr = UART_ReadData(0);
            retVal = 1;
        }
        break;
    case 1:
        status = _HAL_RX200_UART_SSR_REG(1);
        if( status & _HAL_RX200_UART_SSR_ORER_MASK )  /* clear the overrun error*/
        {
            _HAL_RX200_UART_SSR_REG(1) = 0;
        }
        if( status & _HAL_RX200_UART_SSR_RDRF_MASK )
        {
            *chPtr = UART_ReadData(1);
            retVal = 1;
        }
        break;
    case 5:
        status = _HAL_RX200_UART_SSR_REG(5);
        if( status & _HAL_RX200_UART_SSR_ORER_MASK )  /* clear the overrun error*/
        {
            _HAL_RX200_UART_SSR_REG(5) = 0;
        }
        if( status & _HAL_RX200_UART_SSR_RDRF_MASK )
        {
            *chPtr = UART_ReadData(5);
            retVal = 1;
        }
        break;
    case 12:
        status = _HAL_RX200_UART_SSR_REG(12);
        if( status & _HAL_RX200_UART_SSR_ORER_MASK )  /* clear the overrun error*/
        {
            _HAL_RX200_UART_SSR_REG(12) = 0;
        }
        if( status & _HAL_RX200_UART_SSR_RDRF_MASK )
        {
            *chPtr = UART_ReadData(12);
            retVal = 1;
        }
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return(retVal);
}/*end UART_RxByte*/
#endif

/*
 * Function: UART_TxByte
 * Desctription:
 */
int UART_TxByte(int dev, char * chPtr )
{
    unsigned int status;
    int retVal = 0;
    switch(dev)
    {
    case 0:
        status = _HAL_RX200_UART_SSR_REG(0);
        if(status & _HAL_RX200_UART_SSR_TEND_MASK )
        {
            UART_WriteData(0, *chPtr);
            retVal = 1;
        }
        break;
    case 1:
        status = _HAL_RX200_UART_SSR_REG(1);
        if(status & _HAL_RX200_UART_SSR_TEND_MASK )
        {
            UART_WriteData(1, *chPtr);
            retVal = 1;
        }
        break;
    case 5:
        status = _HAL_RX200_UART_SSR_REG(5);
        if(status & _HAL_RX200_UART_SSR_TEND_MASK )
        {
            UART_WriteData(5, *chPtr);
            retVal = 1;
        }
        break;
    case 12:
        status = _HAL_RX200_UART_SSR_REG(12);
        if(status & _HAL_RX200_UART_SSR_TEND_MASK )
        {
            UART_WriteData(12, *chPtr);
            retVal = 1;
        }
        break;
    default:
        HAL_ErrorHandler(1);
        break;
    }
    return(retVal);
}/*end UART_TxByte*/
/* ********************************************************************************************** */
/*                                  END UART  Class HAL Support                                   */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN CRC Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  END CRC Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
/* Table of mapped channels */
static const ADC_MapType ADCMap[ADC_MAPPED_CHANNELS_NUMBER]=
{
    ADC_PIN(4,0,0),
    ADC_PIN(4,1,1),
    ADC_PIN(4,2,2),
    ADC_PIN(4,3,3),
    ADC_PIN(4,4,4),
    ADC_PIN(4,6,6),
    ADC_PIN(E,0,8),
    ADC_PIN(E,5,13)
};

/*
 * Function: ADC_ConfigurePinAsChannel
 * Description:
 * Note:
 * Status:
 */
void ADC_ConfigurePinAsChannel(IO_ChannelType channel)
{
    DISABLE_PORT_REGISTER_WRITE_PROTECTION();
    switch((uint8_t)channel)
    {
    case 0:
        ADC_CHx_ENABLE_AS_PERIPH(0);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(0);
        break;
    case 1:
        ADC_CHx_ENABLE_AS_PERIPH(1);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(1);
        break;
    case 2:
        ADC_CHx_ENABLE_AS_PERIPH(2);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(2);
        break;
    case 3:
        ADC_CHx_ENABLE_AS_PERIPH(3);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(3);
        break;
    case 4:
        ADC_CHx_ENABLE_AS_PERIPH(4);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(4);
        break;
    case 6:
        ADC_CHx_ENABLE_AS_PERIPH(6);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(6);
        break;
    case 8:
        ADC_CHx_ENABLE_AS_PERIPH(8);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(8);
        break;
    case 13:
        ADC_CHx_ENABLE_AS_PERIPH(13);
        ADC_CHx_PIN_ENABLE_AS_ANALOG(13);
        break;
    }
    ENABLE_PORT_REGISTER_WRITE_PROTECTION();
}/*end ADC_ConfigurePinAsChannel*/

/*
 * function: ADC_DeConfigurePinAsChannel
 * Description: Configure Analog Input as Generic I/O
 * Note:
 * Status:
 */
void ADC_DeConfigurePinAsChannel(IO_ChannelType channel)
{
    DISABLE_PORT_REGISTER_WRITE_PROTECTION();
    switch(channel)
    {
    case 0:
        ADC_CHx_ENABLE_AS_IO(0);
        break;
    case 1:
        ADC_CHx_ENABLE_AS_IO(1);
        break;
    case 2:
        ADC_CHx_ENABLE_AS_IO(2);
        break;
    case 3:
        ADC_CHx_ENABLE_AS_IO(3);
        break;
    case 4:
        ADC_CHx_ENABLE_AS_IO(4);
        break;
    case 6:
        ADC_CHx_ENABLE_AS_IO(6);
        break;
    case 8:
        ADC_CHx_ENABLE_AS_IO(8);
        break;
    case 13:
        ADC_CHx_ENABLE_AS_IO(13);
        break;
    }
    ENABLE_PORT_REGISTER_WRITE_PROTECTION();
}/*end ADC_DeConfigurePinAsChannel*/


/*
 * Function: ADC_GetChannel
 * Description: Starting from portAddress and pinMask (read pin) search for AD channel
 * Note:
 * Status:
 */
bool_t ADC_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pADch)
{
    uint8_t j;
    bool_t channelfound = FALSE;

    for (j = 0; j < ADC_MAPPED_CHANNELS_NUMBER; j++)
    {
        if (ADCMap[j].PortAddress == portAddress.basic)
        {
            if (ADCMap[j].PinMask == pinMask)
            {
                channelfound = TRUE;
                break;
            }
        }
    }
    if (channelfound)
    {
        *pADch = ADCMap[j].channel;
    }
    return channelfound;
}/*end ADC_GetChannel*/

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
void ADC_StartConvertionOnChannel(IO_ChannelType channel, uint8_t atdInterrupt)
{
    if (atdInterrupt == ADC_INT_ENABLED)
    {
        ADC_EnableInterruptA();
    }
    else
    {
        ADC_DisableInterruptA();
    }
    ADC_SetIndependentConversion();
    ADC_SingleConversionA();
    ADC_SetSWTrigger();
    ADC_DisableDMA();
    ADC_SelectChannelA(channel);
    ADC_StartNow();
}/*end ADC_StartConvertionOnChannel*/

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
void ADC_StartMultiSampleConvertionOnChannel(uint8_t channel)
{
    ADC_EnableInterruptA();
    ADC_SetIndependentConversion();
    ADC_ContinuesConversionA();
    ADC_SetSWTrigger();
    ADC_SelectChannelA(channel);
}

/**
 \n Writes to ADC registers:
 \n\li enables interrupt from conversion on A
 \n\li sets independent conversion
 \n\li sets single converison
 \n\li interrupts any running conversion
 \n\li disables hw trigger
 \n\li disables dma

 */
void ADC_StartMultiSampleConvertionMultiChannel( void )
{
    ADC_EnableInterruptA();
    ADC_SetIndependentConversion();
    ADC_SingleConversionA();
    ADC_SetSWTrigger();
}



#endif /*  (ADC_USE_SW_DMA | ADC_USE_HW_DMA) */

/*
 * Function: ADC_Init
 * Description: Init of ADC Module
 * Note:
 * Status: Tested!
 */
void ADC_Init(void)
{
    /* Bus clock to the ADC module is enabled  */
    ADC_ClockGateOn();

#if (ADC_REFERENCE_VREFH_VREFL)
    /* Uses the Vrefh and Vrefl pins for the reference */
    ADC_SetVRefToVrefhVrefl();
#else
    /* Uses the Vdda and Vssa pins for the reference */
    ADC_SetVRefToVddaVssa();
#endif

#if (ADC_LOW_POWER_MODE)
    /* Low power mode */
    ADC_SetLowPowerConversion();
#else
    /* High speed mode */
    ADC_SetHighSpeedConversion();
#endif

#if (ADC_LONG_SAMPLE_TIME)
    /* Long sample time */
    ADC_SetLongSampleTime();
#else
    /* Short sample time */
    ADC_SetShortSampleTime();
#endif

    S12AD.ADCER.BIT.ADRFMT = 0; /* Right alignment format*/
#if (ADC_RESOLUTION_MODE == 12)
    /* It is the only resolution supported*/
    /* ADC will sample in 12-bit operation */
    /*ADC_SetConversion12bit();*/
#else
#error "only 12 bit ADC_RESOLUTION_MODE supported"
#endif

#if (ADC_BUS_CLOCK_DIV_2)
    /* The ADC source clock is bus / 2 */
    ADC_SetBusClockDiv2();
#else
    /* The ADC source clock is bus */
    ADC_SetBusClock();
#endif

    /* Sets the ADC divisor */
    ADC_SelectClockDiv(ADC_DIV);

    /* Sets the independent conversion of A & B results */
    ADC_SetIndependentConversion();

    /* Enables the Disconnection Detection Assist Function  */
    ADC_DisconnectingDetection(0x01,0x0F); /*Precharge with time 0x0F*/
}

/**
 Set all ADC Registers to the reset state value
 */
void ADC_DeInit(void)
{
    ADC_ClockGateOff();
}
/**
* Get the sample on channel already converted in the right resolution
*/
uint16_t ADC_GetConvertedValueOnChannel( uint8_t channel)
{
    uint16_t value=0;
    switch(channel)
    {
    case 0:
        value = _HAL_RX210_ADC_ADDR_REG(0);
        break;
    case 1:
        value = _HAL_RX210_ADC_ADDR_REG(1);
        break;
    case 2:
        value = _HAL_RX210_ADC_ADDR_REG(2);
        break;
    case 3:
        value = _HAL_RX210_ADC_ADDR_REG(3);
        break;
    case 4:
        value = _HAL_RX210_ADC_ADDR_REG(4);
        break;
    case 6:
        value = _HAL_RX210_ADC_ADDR_REG(6);
        break;
    case 8:
        value = _HAL_RX210_ADC_ADDR_REG(8);
        break;
    case 13:
        value = _HAL_RX210_ADC_ADDR_REG(13);
        break;
    }

    value = ADC_GetCorrectResolutionResult(value);
    return value;
}



/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                   */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  BEGIN DMA Class HAL Support                                   */
/* ********************************************************************************************** */


/*
 * Function: DMA_SetDevice
 * Description:
 */
void DMA_ClockGate(void)
{
    DISABLE_REGISTER_WRITE_PROTECTION;
    SYSTEM.MSTPCRA.BIT.MSTPA28 = 0;
    ENABLE_REGISTER_WRITE_PROTECTION;
}/* DMA_ClockGate */
/*
 * Function: DMA_SetDevice
 * Description:
 */
void DMA_SetDevice(uint8_t channel, uint8_t device)
{

    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(0) |= DMA_DTE_BIT_MASK;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) |= DMA_DTE_BIT_MASK;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(2) |= DMA_DTE_BIT_MASK;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(3) |= DMA_DTE_BIT_MASK;
    /* Enable DMAC Global */
    _HAL_RX210_DMA_MODULE_ACTIVATION_REGISTER_ENABLE;
}/* end DMA_SetDevice */

/*
 * Function: DMA_GetDevice
 */
uint8_t DMA_GetDevice(uint8_t channel)
{
    return 0;
}/* end DMA_GetDevice */

/*
 * Function: DMA_SetSAR
 * Description:
 */
void DMA_SetSAR(uint8_t channel , FLS_positionType address)
{
    if(channel == 0)
        _HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(0) = address;
    else if(channel == 1)
        _HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(1) = address;
    else if(channel == 2)
        _HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(2) = address;
    else if(channel == 3)
        _HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(3) = address;
}/* end DMA_SetSAR */

/*
 * Function: DMA_GetSAR
 * Description:
 */

uint8_t* DMA_GetSAR(uint8_t channel)
{
    if(channel == 0)
        return ((uint8_t*)_HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(0));
    else if(channel == 1)
        return ((uint8_t*)_HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(1));
    else if(channel == 2)
        return ((uint8_t*)_HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(2));
    else if(channel == 3)
        return ((uint8_t*)_HAL_RX210_DMA_SOURCE_ADDRESS_REGISTER(3));
    else
        HAL_ErrorHandler(1);

    return 0;
}/* end DMA_GetSAR */

/*
 * Function DMA_SetDAR
 * Description: Set DMA destionation address
 */
void DMA_SetDAR(uint8_t channel, FLS_positionType address)
{
    if(channel == 0)
        _HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(0) = address;
    else if(channel == 1)
        _HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(1) = address;
    else if(channel == 2)
        _HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(2) = address;
    else if(channel == 3)
        _HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(3) = address;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetDAR */
/*
 * Functon: DMA_GetDar
 * Description: Read DMA destionation address
 */
uint8_t* DMA_GetDAR(uint8_t channel)
{
    if(channel == 0)
        return ((uint8_t*)_HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(0));
    else if(channel == 1)
        return ((uint8_t*)_HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(1));
    else if(channel == 2)
        return ((uint8_t*)_HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(2));
    else if(channel == 3)
        return ((uint8_t*)_HAL_RX210_DMA_DESTINATION_ADDRESS_REGISTER(3));
    else
        HAL_ErrorHandler(1);
    return 0;
}/* end DMA_GetDAR */

/*
 * Function: DMA_SetBCR(channel, count)
 * Description: Set the DMA Transfer Count Register
 */
void DMA_SetBCR(uint8_t channel, uint32_t count)
{
    if(channel == 0)
    {
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(0) = count<<16;
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(0) |= count;
    }
    else if(channel == 1)
    {
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(1) = count<<16;
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(1) |= count;
    }
    else if(channel == 2)
    {
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(2) = count<<16;
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(2) = count;
    }
    else if(channel == 3)
    {
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(3) = count<<16;
        _HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(3) = count;
    }
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetBCR */

/*
 * Function: DMA_GetBCR
 * Description: Read DMA Transfer Count Register
 */
uint32_t DMA_GetBCR(uint8_t channel)
{
    if(channel == 0)
        return(_HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(0));
    else if(channel == 1)
        return(_HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(1));
    else if(channel == 2)
        return(_HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(2));
    else if(channel == 3)
        return(_HAL_RX210_DMA_TRANSFER_COUNT_REGISTER(3));
    else
        HAL_ErrorHandler(1);
    return 0;
}/* end DMA_GetBCR */

/*
 * Function:  DMA_EnableInt(channel)
 * Description:
 */
void DMA_EnableInt(uint8_t channel)
{
    if(channel == 0)
    {
        ICU.IPR[(DMAC_ICU_INTERRUPT_BASE_VECTOR + channel)].BYTE = DMAC_ICU_PRIORITY_INT_CH0;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(0) |= DMA_TRANSFER_END_INTERRUTP_MASK;
        ICU.IR[DMAC_ICU_INTERRUPT_BASE_VECTOR  + channel].BYTE = 0x00;
        ICU.IER[DMAC_CHANNEL0_ICU_IER].BYTE |= DMAC_CHANNEL0_IER_BIT;
    }
    else if(channel == 1)
    {
        ICU.IPR[DMAC_ICU_INTERRUPT_BASE_VECTOR + channel].BYTE = DMAC_ICU_PRIORITY_INT_CH1;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(1) |= DMA_TRANSFER_END_INTERRUTP_MASK;
        ICU.IR[DMAC_ICU_INTERRUPT_BASE_VECTOR  + channel].BYTE = 0x00;
        ICU.IER[DMAC_CHANNEL1_ICU_IER].BYTE |= DMAC_CHANNEL1_IER_BIT;
    }
    else if(channel == 2)
    {
        ICU.IPR[DMAC_ICU_INTERRUPT_BASE_VECTOR + channel].BYTE = DMAC_ICU_PRIORITY_INT_CH2;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(2) |= DMA_TRANSFER_END_INTERRUTP_MASK;
        ICU.IR[DMAC_ICU_INTERRUPT_BASE_VECTOR  + channel].BYTE = 0x00;
        ICU.IER[DMAC_CHANNEL2_ICU_IER].BYTE |= DMAC_CHANNEL2_IER_BIT;
    }
    else if(channel == 3)
    {
        ICU.IPR[DMAC_ICU_INTERRUPT_BASE_VECTOR + channel].BYTE = DMAC_ICU_PRIORITY_INT_CH3;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(3) |= DMA_TRANSFER_END_INTERRUTP_MASK;
        ICU.IR[DMAC_ICU_INTERRUPT_BASE_VECTOR  + channel].BYTE = 0x00;
        ICU.IER[DMAC_CHANNEL3_ICU_IER].BYTE |= DMAC_CHANNEL3_IER_BIT;
    }
    else
        HAL_ErrorHandler(1);
}/* end  DMA_EnableInt(channel) */

/*
 * Function:  DMA_EnableInt(channel)
 * Description:
 */
void DMA_DisableInt(uint8_t channel)
{
    if(channel == 0)
    {
        ICU.IER[DMAC_CHANNEL0_ICU_IER].BYTE &= ~DMAC_CHANNEL0_IER_BIT;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(0) &= ~DMA_TRANSFER_END_INTERRUTP_MASK;
    }
    else if(channel == 1)
    {
        ICU.IER[DMAC_CHANNEL1_ICU_IER].BYTE &= ~DMAC_CHANNEL1_IER_BIT;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(1) &= ~DMA_TRANSFER_END_INTERRUTP_MASK;
    }
    else if(channel == 2)
    {
        ICU.IER[DMAC_CHANNEL2_ICU_IER].BYTE &= ~DMAC_CHANNEL2_IER_BIT;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(2) &= ~DMA_TRANSFER_END_INTERRUTP_MASK;
    }
    else if(channel == 3)
    {
        ICU.IER[DMAC_CHANNEL3_ICU_IER].BYTE &= ~DMAC_CHANNEL3_IER_BIT;
        _HAL_RX210_DMA_INTERRUPT_SETTING_REGISTER(3) &= ~DMA_TRANSFER_END_INTERRUTP_MASK;
    }
    else
    {
        HAL_ErrorHandler(1);
    }

    ICU.IPR[DMAC_ICU_INTERRUPT_BASE_VECTOR + channel].BYTE = 0;
    ICU.IR[DMAC_ICU_INTERRUPT_BASE_VECTOR  + channel].BYTE = 0x00;

}/* end  DMA_EnableInt(channel) */

/*
 * Function: DMA_SetStartOn
 * Description:
 */
void DMA_SetStartOn(uint8_t channel)
{
    /* Enable software trigger for DMA */
    if(channel == 0)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(0)|= DMREQ_SWREQ_MASK;
    else if(channel == 1)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(1)|= DMREQ_SWREQ_MASK;
    else if(channel == 2)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(2)|= DMREQ_SWREQ_MASK;
    else if(channel == 3)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(3)|= DMREQ_SWREQ_MASK;

}/* end DMA_SetStartOn */

/*
 * Function: DMA_SetStartOff(channel)
 * Description:
 */
void DMA_SetStartOff(uint8_t channel)
{
    /*  Delete the software start FLAG */
    if(channel == 0)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(0)&= ~DMREQ_SWREQ_MASK;
    else if(channel == 1)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(1)&= ~DMREQ_SWREQ_MASK;
    else if(channel == 2)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(2)&= ~DMREQ_SWREQ_MASK;
    else if(channel == 3)
        _HAL_RX210_DMA_SOFTWARE_START_REGISTER(3)&= ~DMREQ_SWREQ_MASK;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetStartOff */

/*
 * Function: DMA_IsChannelBusy
 * Description:
 */
bool_t DMA_IsChannelBusy(uint8_t channel)
{
    if(channel == 0)
        return (_HAL_RX210_DMA_STATUS_REGISTER(0) & DMA_ACT_BIT_MASK);
    else if(channel == 1)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_ACT_BIT_MASK);
    else if(channel == 2)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_ACT_BIT_MASK);
    else if(channel == 3)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_ACT_BIT_MASK);
    else
        HAL_ErrorHandler(1);
    return 0;
}/* end DMA_IsChannelBusy */

/*
 * Function: DMA_IsTransDone
 * Description:
 */
bool_t DMA_IsTransDone(uint8_t channel)
{
    if(channel == 0)
        return (_HAL_RX210_DMA_STATUS_REGISTER(0) & DMA_DTIF_BIT_MASK);
    else if(channel == 1)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_DTIF_BIT_MASK);
    else if(channel == 2)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_DTIF_BIT_MASK);
    else if(channel == 3)
        return (_HAL_RX210_DMA_STATUS_REGISTER(1) & DMA_DTIF_BIT_MASK);
    else
        HAL_ErrorHandler(1);
    return 0;
}/* DMA_IsTransDone */

/*
 * Function: DMA_ClearTransDone(channel)
 * Description:
 */
void DMA_ClearTransDone(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_STATUS_REGISTER(0) = 0x00;
    else if(channel == 1)
        _HAL_RX210_DMA_STATUS_REGISTER(1) = 0x00;
    else if(channel == 2)
        _HAL_RX210_DMA_STATUS_REGISTER(2) = 0x00;
    else if(channel == 3)
        _HAL_RX210_DMA_STATUS_REGISTER(3) = 0x00;
    else
        HAL_ErrorHandler(1);
}/* DMA_ClearTransDone(channel) */

/*
 * Function: IsDMAChannelTrasferEnabled
 * Description: Check if DMA on channel is Enabled
 */
bool_t IsDMAChannelTrasferEnabled(uint8_t channel)
{
    if(channel == 0)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(0) & DMA_DTE_BIT_MASK);
    else if(channel == 1)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_DTE_BIT_MASK);
    else if(channel == 2)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_DTE_BIT_MASK);
    else if(channel == 3)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_DTE_BIT_MASK);
    else
        HAL_ErrorHandler(1);
    return 0;
}/* end IsDMAChannelTrasferEnabled */

/*
 * Function: DMA_IsPending
 * Description:
 * Note:
 * 	- 1 if DMA is operating
 * 	- 0 if DMA is suspended
 * 	Author: Andrea Emili, SPES scpa
 */
bool_t DMA_IsPending(uint8_t channel)
{
    if(channel == 0)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(0) & DMA_ACT_BIT_MASK);
    else if(channel == 1)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_ACT_BIT_MASK);
    else if(channel == 2)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_ACT_BIT_MASK);
    else if(channel == 3)
        return (_HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) & DMA_ACT_BIT_MASK);
    else
        HAL_ErrorHandler(1);
    return 0;
}/* DMA_IsPending */

/*
 * Function: DMA_IsDestinationError
 */
bool_t DMA_IsDestinationError(uint8_t channel)
{
    return 0;
}/* DMA_IsDestinationError */

/*
 * Function: DMA_IsSourceError
 */
bool_t DMA_IsSourceError(uint8_t channel)
{
    return 0;
}/* DMA_IsSourceError */

/*
 * Function: DMA_IsConfigError
 */
bool_t DMA_IsConfigError(uint8_t channel)
{
    return 0;
}/* DMA_IsConfigError */

/*
 * Function: DMA_PeripheralRequestOn
 */
void DMA_PeripheralRequestOn(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) |= INTERRUPT_REQUEST;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) |= INTERRUPT_REQUEST;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) |= INTERRUPT_REQUEST;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) |= INTERRUPT_REQUEST;
    else
        HAL_ErrorHandler(1);
}/* end DMA_PeripheralRequestOn */

/*
 * Function: DMA_PeripheralRequestOff
 * Description: Set DMA Request Source select as Software( Default modality)
 */
void DMA_PeripheralRequestOff(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) &= 0xFF00;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) &= 0xFF00;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) &= 0xFF00;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) &= 0xFF00;
    else
        HAL_ErrorHandler(1);
}/* end DMA_PeripheralRequestOn */

/*
 * Function: DMA_ContinuousTransfer
 * Description:
 */
void DMA_ContinuousTransfer(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) |= DMA_REPEAT_TRANSFER_MASK;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) |= DMA_REPEAT_TRANSFER_MASK;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) |= DMA_REPEAT_TRANSFER_MASK;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) |= DMA_REPEAT_TRANSFER_MASK;
    else
        HAL_ErrorHandler(1);
}/* end DMA_ContinuousTransfer */

/*
 * Function: DMA_SingleTransfer
 * Description: Set Normal Trasfer mode
 */
void DMA_SingleTransfer(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) &= 0x3FFF;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) &= 0x3FFF;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) &= 0x3FFF;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) &= 0x3FFF;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SingleTransfer */

/*
 * Function: DMA_SingleTransfer
 * Description: Set Normal Trasfer mode
 */
void DMA_BlockTransfer(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) |= DMA_BLOCK_TRANSFER_MASK;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) |= DMA_BLOCK_TRANSFER_MASK;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) |= DMA_BLOCK_TRANSFER_MASK;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) |= DMA_BLOCK_TRANSFER_MASK;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SingleTransfer */

/*
 * Function: DMA_SetDestinationSizeToByte
 */
void DMA_SetDestinationSizeToByte(uint8_t channel)
{
    /* TODO */
}/* DMA_SetDestinationSizeToByte */

/*
 * Function: DMA_SetDestinationSizeToWord
 */
void DMA_SetDestinationSizeToWord(uint8_t channel)
{
    /* TODO */
}/* DMA_SetDestinationSizeToWord */

/*
 * Function: DMA_SetDestinationSizeToLong
 */
void DMA_SetDestinationSizeToLong(uint8_t channel)
{
    /* TODO */
}/* DMA_SetDestinationSizeToLong */
/*
 * Function: void DMA_SetSourceSizeToLong(uint8_t channel)
 */
void DMA_SetSourceSizeToLong(uint8_t channel)
{
    /* TODO */
}/* end DMA_SetSourceSizeToLong */
/*
 * Function: void DMA_SetSourceSizeToByte(uint8_t channel)
 */
void DMA_SetSourceSizeToByte(uint8_t channel)
{
    /* TODO */
}/* end DMA_SetSourceSizeToByte */
/*
 * Function: DMA_SetSourceSizeToWord(uint8_t channel)
 */
void DMA_SetSourceSizeToWord(uint8_t channel)
{
    /* TODO */
}/* return DMA_SetSourceSizeToWord */

/*
 * Function: DMA_EnableSourceIncrement(uint8_t channel)
 */
void DMA_EnableSourceIncrement(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(0)|= DMA_SOURCE_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 1)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(1) |= DMA_SOURCE_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 2)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(2) |= DMA_SOURCE_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 3)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(3) |= DMA_SOURCE_ADDRESS_UPDATE_MODE_INCREMENT;
    else
        HAL_ErrorHandler(1);
}/* end DMA_EnableSourceIncrement */

/*
 * Function: DMA_EnableSourceIncrement(uint8_t channel)
 */
void DMA_DisableSourceIncrement(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(0) &= 0x3FFF;
    else if(channel == 1)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(1) &= 0x3FFF;
    else if(channel == 2)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(2) &= 0x3FFF;
    else if(channel == 3)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(3) &= 0x3FFF;
    else
        HAL_ErrorHandler(1);
}/* end DMA_DisableSourceIncrement */

/*
 * Function: DMA_EnableDestionationIncrement(uint8_t channel)
 */
void DMA_EnableDestinationIncrement(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(0)|= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 1)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(1) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 2)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(2) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 3)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(3) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else
        HAL_ErrorHandler(1);
}/* end DMA_EnableDestinationIncrement */


/*
 * Function: DMA_DisableDestionationIncrement(uint8_t channel)
 */
void DMA_DisableDestinationIncrement(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(0)|= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 1)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(1) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 2)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(2) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else if(channel == 3)
        _HAL_RX210_DMA_ADDRESS_MODE_REGISTER(3) |= DMA_DESTIONATION_ADDRESS_UPDATE_MODE_INCREMENT;
    else
        HAL_ErrorHandler(1);
}/* end DMA_EnableDestinationIncrement */

/*
 * Function: DMA_EnableAutoAlign
 * Description:
 * Note: There is not this feature in RX210 DMA
 */
void DMA_EnableAutoAlign(uint8_t channel)
{
}/* end DMA_EnableAutoAlign */

/*
 * Function: DMA_DisableAutoAlign
 * Note: There is not this featur in RX210 DMA
 */
void DMA_DisableAutoAlign(uint8_t channel)
{
}/* end DMA_DisableAutoAlign */

/*
 * DMA_SetSourceAddressModulo
 */
void DMA_SetSourceAddressModulo(uint8_t channel, uint8_t module)
{
}/* DMA_SetSourceAddressModulo */

/*
 * DMA_SetDestionationAddressModulo
 */
void DMA_SetDestinationAddressModulo(uint8_t channel, uint8_t module)
{
}/* DMA_SetDestionationAddressModulo */

/*
 * DMA_SetDisableRequestOn
 */
void DMA_SetDisableRequestOn(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) &= 0xFF00;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) &= 0xFF00;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) &= 0xFF00;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) &= 0xFF00;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetDisableRequestOn */

/*
 * DMA_SetEnableRequestOn
 * Description: Enable interrupt request from peripheral
 */
void DMA_SetEnableRequestOn(uint8_t channel)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) |= INTERRUPT_REQUEST;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) |= INTERRUPT_REQUEST;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) |= INTERRUPT_REQUEST;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) |= INTERRUPT_REQUEST;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetEnableRequestOn */

/*
 * Function: void DMA_LinkChannelControlMode
 * Description: Link the DMA to a hardware trigger form a peripheral
 * Note:
 *  - You must give the number of vector of peripheral interrupt
 */
void DMA_LinkChannelControlMode(uint8_t channel, uint8_t mode)
{
    if(channel == 0)
    {
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(0) &= ~DMA_DTE_BIT_MASK;
        _HAL_RX210_DMA_ICU_ACTIVATION_REGISTER(0) = mode;
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(0) |= DMA_DTE_BIT_MASK;
    }
    else if(channel == 1)
    {
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) &= ~DMA_DTE_BIT_MASK;
        _HAL_RX210_DMA_ICU_ACTIVATION_REGISTER(1) = mode;
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(1) |= DMA_DTE_BIT_MASK;
    }
    else if(channel == 2)
    {
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(2) &= ~DMA_DTE_BIT_MASK;
        _HAL_RX210_DMA_ICU_ACTIVATION_REGISTER(2) = mode;
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(2) |= DMA_DTE_BIT_MASK;
    }

    else if(channel == 3)
    {
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(3) &= ~DMA_DTE_BIT_MASK;
        _HAL_RX210_DMA_ICU_ACTIVATION_REGISTER(3) = mode;
        _HAL_RX210_DMA_TRANSFER_ENABLE_REGISTER(3) |= DMA_DTE_BIT_MASK;
    }
    else
        HAL_ErrorHandler(1);
}/* void DMA_LinkChannelControlMode */

/*
 * Function: DMA_LinkChannel1Mode
 */
void DMA_LinkChannel1Mode(uint8_t channel, uint8_t mode)
{
}/* DMA_LinkChannel1Mode */

/*
 * Function: DMA_LinkChannel2Mode
 */
void DMA_LinkChannel2Mode(uint8_t channel, uint8_t mode)
{
}/* DMA_LinkChannel2Mode */

/*
 * Function: DMA_ControlDefault
 */
void DMA_ControlDefault(uint8_t channel)
{
}/* DMA_ControlDefault */

/*
 * Function: DMA_SetDisableRequestOff
 */
void DMA_SetDisableRequestOff(uint8_t channel)
{
}/* DMA_SetDisableRequestOff */

/*
 * Function: DMA_GetTrasferMode
 */
uint8_t DMA_GetTrasferMode(uint8_t channel)
{
    uint8_t transfer_mode = 0xFF;
    if(channel == 0)
        transfer_mode = ((_HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) & 0xF000)>>14);
    else if(channel == 1)
        transfer_mode = ((_HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) & 0xF000)>>14);
    else if(channel == 2)
        transfer_mode = ((_HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) & 0xF000)>>14);
    else if(channel == 3)
        transfer_mode = ((_HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) & 0xF000)>>14);
    else
        HAL_ErrorHandler(1);

    return (transfer_mode);
}/* DMA_SetDisableRequestOff */

/*
 * Function: DMA_SetNumberOfRepetition
 * Description:
 * 	- In repetion mode set the number of repetion from 1 to 1024
 */
void DMA_SetNumberOfRepetion(uint8_t channel, uint16_t count)
{
    if(channel == 0)
        _HAL_RX210_DMA_BLOCK_TRANSFER_COUNT_REGISTER(0) = count;
    else if(channel == 1)
        _HAL_RX210_DMA_BLOCK_TRANSFER_COUNT_REGISTER(1) = count;
    else if(channel == 2)
        _HAL_RX210_DMA_BLOCK_TRANSFER_COUNT_REGISTER(2) = count;
    else if(channel == 3)
        _HAL_RX210_DMA_BLOCK_TRANSFER_COUNT_REGISTER(3) = count;
    else
        HAL_ErrorHandler(1);
}/* end DMA_SetNumberOfRepetition */

/*
 * Function: DMA_SetRepeatArea
 * Note: The possible modality are specificated in *hal.h, and they are:
 * 		REPEAT_AREA_IS_DESTIONATION
 *		REPEAT_AREA_IS_SOURCE
 *		REPEAT_AREA_IS_NOT_SPECIFIED
 *
 */
void DMA_SetRepeatArea(uint8_t channel, uint16_t mode)
{
    if(channel == 0)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) &= 0xCFFF;
    else if(channel == 1)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) &= 0xCFFF;
    else if(channel == 2)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) &= 0xCFFF;
    else if(channel == 3)
        _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) &= 0xCFFF;
    else
        HAL_ErrorHandler(1);

    if(mode != REPEAT_AREA_IS_DESTIONATION)
    {
        if(channel == 0)
            _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(0) |= mode;
        else if(channel == 1)
            _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(1) |= mode;
        else if(channel == 2)
            _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(2) |= mode;
        else if(channel == 3)
            _HAL_RX210_DMA_TRANSFER_MODE_REGISTER(3) |= mode;
    }
}/* nd DMA_SetRepeatArea */

/* ********************************************************************************************** */
/*                                  END DMA Class HAL Support                                   */
/* ********************************************************************************************** */
/* ********************************************************************************************** */
/*                                  START SPI Class HAL Support                                   */
/* ********************************************************************************************** */

/*
 * Function: SPI_ClkEnable
 */
void SPI_ClkEnable(unsigned char par)
{
    DISABLE_REGISTER_WRITE_PROTECTION;
    SYSTEM.MSTPCRB.BIT.MSTPB17 = 0;
    ENABLE_REGISTER_WRITE_PROTECTION;
}/* end SPI_ClkEnable */

/*
 * Function: SPI_Enable()
 */
void SPI_Enable(unsigned char par)
{
    RSPI0.SSLP.BIT.SSL0P = PDL_SPI_PIN_SSLO;	/* !< Slave Select Polarity Register */
    RSPI0.SPPCR.BYTE |= PDL_SPI_PIN_MOST_IDLE;	/* !< Mosi Idle control */
    RSPI0.SPDCR.BIT.SPLW = TRUE;				/* !< SPDR is access in longwords */
    RSPI0.SPCR.BIT.SPE |= TRUE;
}/* end SPI_Enable */

/*
 * Function: SPI_MasterEnable
 */
void SPI_MasterEnable(unsigned char par)
{
    /* SET SPI PIN */
    SPI_SET_SCLK_PIN;
    SPI_SET_SSL_PIN;
    SPI_SET_MOSI_PIN;
    SPI_SET_MISO_PIN;
    /* Set SPI AS MASTER */
    RSPI0.SPCR.BIT.MSTR = 1;	/* !< Set SPI as Master */
}/* end SPI_Enable */

/*
 * Function: SPI_SetClockPolarity
 */
void SPI_SetClockPolarity(unsigned char par, unsigned char par_1)
{
    RSPI0.SPCMD0.BIT.CPOL = par_1;
}/* end SPI_SetClockPolarity */

/*
 * Function: SPI_SetClockPhase
 */
void SPI_SetClockPhase(unsigned char par, unsigned char pha)
{
    RSPI0.SPCMD0.BIT.CPHA = pha;
}/* end SPI_SetClockPhase */

/*
 * Function: SPI_ResetStatus
 */
void SPI_ResetStatus(unsigned char par)
{
    unsigned char i = 0;

    RSPI0.SPCR.BIT.SPE = SPI_ENABLE;
    for(i = 0; i < 100; i++)
        __asm("nop");
    RSPI0.SPCR.BIT.SPE = SPI_DISABLE;	/* !< This command disable the SPI and performs reset of peripheral. */
}/* end SPI_ResetStatus */

/*
 * Function: SPI_SetBaudRate
 * Parameters:
 * 	- par_1
 * 	- baudrate: The value of BaudRate to set
 */
void SPI_SetBaudRate(unsigned char par_1, unsigned long int baudrate)
{
    RSPI0.SPCMD0.BIT.BRDV = BIT_RATE_DIVISION;
    RSPI0.SPBR.BYTE  = _HAL_SPI_BAUDRATE(baudrate);
}/* end SPI_SetBaudRate */

/*
 * Function: SPI1_SetShiftDir
 * Descrption: Set the as first tasmit bit the MSB or the LSB.shift_direction
 */
void SPI_SetShiftDir(unsigned char par ,unsigned char shift_direction)
{
    RSPI0.SPCMD0.BIT.LSBF = shift_direction;
}/* end SPI1_SetShiftDir */

/*
 * Function: SPI_WriteData
 */
void SPI_WriteData(unsigned char par_1, unsigned char data)
{
    while (RSPI0.SPSR.BIT.IDLNF) ;
    RSPI0.SPDR.LONG = (unsigned short)data;
} /* end SPI_WriteData */

/*
 * Function: SPI_ReadData
 */
unsigned char SPI_ReadData(unsigned char par_1)
{
    unsigned char data;
    data = RSPI0.SPDR.LONG;
    return data;
} /* end SPI_WriteData */

/*
 * Function: SPI_IntEnable
 */
void SPI_IntEnable(unsigned char par)
{
#if SPI_TX_INTERRUPT_ENABLE
    /* Clear IR */
    _HAL_RX210_SPI_TX_IR = 0;
    /* Enable peripheral TX interrupt request */
    RSPI0.SPCR.BIT.SPTIE = TRUE;
    /* Set Interrupt Priority */
    _HAL_RX210_SPI_IPR_REG = SPI_INT_PRIORITY;
    /* Enable ICU peripheral TX interrupt */
    _HAL_RX210_SPI_IER_REG |= SPI_TX_IER_BIT;
#endif
#if SPI_RX_INTERRUPT_ENABLE
    /* Clear IR */
    _HAL_RX210_SPI_RX_IR = 0;
    /* Enable peripheral RX interrupt request */
    RSPI0.SPCR.BIT.SPRIE = TRUE;
    /* Set Interrupt Priority */
    _HAL_RX210_SPI_IPR_REG = SPI_INT_PRIORITY;
    /* Enable ICU peripheral RX interrupt */
    _HAL_RX210_SPI_IER_REG |= SPI_RX_IER_BIT;
#endif
}/* end SPI_IntEnable */

/*
 * Function: SPI_GetStatus
 */
unsigned char SPI_GetStatus(unsigned char par)
{
    unsigned char status = 0;
    status = RSPI0.SPSR.BYTE;
    return status;
}/* end SPI_GetStatus */


/* ********************************************************************************************** */
/*                                  END SPI Class HAL Support                                   */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN LPM Class HAL Support                                   */
/* ********************************************************************************************** */
/* LOW Power Mode */
#if defined(HAL_LPM_SUPPORT)
void LPM_SleepModeProc(void)
{
	DISABLE_REGISTER_WRITE_PROTECTION;
	SYSTEM.OPCCR.BIT.OPCM=PDL_LPM_OPC_MIDDLE_SPEED_1A;

	/* RSTCKEN = 0 - osc. switch disable when return */
	SYSTEM.RSTCKCR.BIT.RSTCKEN  = 0;
	SYSTEM.RSTCKCR.BIT.RSTCKSEL = 1;
	/* Select sleep (or all-module clock stop) */
	SYSTEM.SBYCR.BIT.SSBY = 0;
	/* Prevent out-of-order execution */
	while (SYSTEM.SBYCR.BIT.SSBY != 0);
	__builtin_rx_wait();

	ENABLE_REGISTER_WRITE_PROTECTION;
}
#endif
/* ********************************************************************************************** */
/*                                  END LPM Class HAL Support                                   */
/* ********************************************************************************************** */

struct _ResetInfo ResetInfo __attribute__ ((section (".ResetInfo")));
const boot_short_info_t BootShortInfo __attribute__ ((section (".Bootloader")));

/* *********** */
/* End of file */
/* *********** */
