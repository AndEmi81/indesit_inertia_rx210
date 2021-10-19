/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		OCWDT_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCWDT_drv.c
\ingroup	OCWDT
\date		Nov 3, 2009
\version	00.00.17
\author		Jaroslav Musil, Stefano Ughi
\n
\n
\par	History:
\n
\n      00.00.21
\n		Feb 28, 2011
\n		stefano.ughi
\n		changes
\n 		- deep modifications Driver and HAL AG128 cfr. V.C.S.
\n      00.00.20
\n		Dec 20, 2010
\n		stefano.ughi
\n		changes
\n 		- fixed bug on watchdog refresh target AG128
\n 		00.00.19
\n		Feb 15, 2010
\n		Jaroslav Musil
\n		changes
\n 		- The init function lines moved to hal.
\n 		00.00.18
\n		Feb 16, 2010
\n		Jaroslav Musil
\n		changes
\n 		- The inline functions moved to hal.
\n 		00.00.17
\n		Nov 3, 2009
\n		stefano.ughi
\n		changes
\n 		- first insertion
\n
\n
\n
*/
#define MODULE_BODY_OCWDT
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OCWDT_drv.h"
#include "LIB_Error.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Commands and status */
typedef struct WatchDogTag
{
	/** if '1' dog can be fed */
	bool_t feedTheDog:1;
	/** free for future use */
	bool_t free:7;
} WatchDog_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

#if (WDT_WINDOWED_MODE==1)

#if defined (__CWCC__)
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */

/** Commands/Status for the dog */
NO_INIT_VAR static WatchDog_t WatchDog;

#elif defined(__ICCARM__)
#define __ram_initialization __iar_data_init3
#pragma location = ".noinit"

/** Commands/Status for the dog */
 __no_init static WatchDog_t WatchDog;

#elif defined(__GNUC__)

 /** Commands/Status for the dog */
 static WatchDog_t __attribute__((section(".noinit"))) WatchDog;

#else
#error "No IDE defined"
static WatchDog_t WatchDog;
#endif

#endif
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
This function initialises the WDT driver according to the user's inputs which are:
\n
\n\b    WDT_ENABLED: FALSE disables watchdog, TRUE enables watchdog
\n
\n\b    WDT_UPDATE_AFTER_WRITE: FALSE disables possible reconfiguration, TRUE enables reconfiguration after write
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported
\n
\n\b    WDT_WINDOWED_MODE: FALSE disables the windowing, TRUE enables the windowing
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported
\n
\n\b    WDT_INT_ENABLED: FALSE disables interrupt before reset, TRUE enables interrupt before reset
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported
\n
\n\b    WDT_ALTERNATE_CLOCK dedicated source or alternate source:
\n\li MCF51_AG128 FALSE LPO clock (dedicated), TRUE alternate clock (bus frequency)
\n\li GB/GT unsupported
\n\li QE128 supported
\n\li RN_300H_39079 To Check
\n
\n\b    WDT_ENABLED_IN_STOP: FALSE disables watchdog in stop mode, TRUE enables watchdog in stop mode
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li QE128 To Check
\n\li RN_300H_39079 To Check
\n
\n\b    WDT_ENABLED_IN_DEBUG: 0 disables watchdog in debug mode, 1 enables watchdog in debug mode
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported
\n
\n\b    WDT_ENABLED_IN_WAIT: 0 disables watchdog in wait mode, 1 enables watchdog in wait mode
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 To Check
\n
\n\b    WDT_TIMEOUT: sets the timeout value
\n\li MCF51_AG128 it should be at least 4 watchdog clocks and less than 2 ^ 32 - 1
\n\li GB/GT 0->Short timeout period selected (2el(13) cycles of BUSCLK).
\n\ 		1->Long timeout period selected (2el(18) cycles of BUSCLK)
\n\li QE128 To Check
\n\li RN_300H_39079 possible values [0-0xFE] 1 = 1,5625 msec typ, min. 0,78125 msec.
\n          Typical WatchDog Timeout = (0xFF-WDT_RELOAD_TIME)*1,5625 msec. To Check Internal Source
\n
\n\b    WDT_WINDOW: sets the window value, it should be lower than WDT_TIMEOUT
\n\li MCF51_AG128 supported.
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported
\n
\attention
\n\li               <b> GB/GT M.C.U. </b>
\n                  Because of _HAL_S08_WDT_SOPT_REG is a write-once register,
\n					so only the first write after reset is honored,
\n					also BDM and STOP Mode must be setup in SystemOptionConfig.h that must
\n					be included in Config.h.
\n					\b BDM_ENABLE=TRUE -> Enable BDM
\n					\b BDM_ENABLE=FALSE -> Disable BDM
\n					\b STOP_ENABLE=TRUE -> Enable Stop Mode
\n					\b STOP_ENABLE=FALSE -> Disable Stop Mode
\n\li   			<b> QE M.C.U. </b>
\n                  Because of _HAL_S08_WDT_SOPT1_REG is a write-once register,
\n					so only the first write after reset is honored,
\n					also BDM, STOP Mode, RSTO Mode, RESET Mode, must be setup in SystemOptionConfig.h that must
\n					be included in Config.h.
\n					\b BDM_ENABLE=TRUE -> Enable BDM
\n					\b BDM_ENABLE=FALSE -> Disable BDM
\n					\b STOP_ENABLE=TRUE -> Enable Stop Mode
\n					\b STOP_ENABLE=FALSE -> Disable Stop Mode
\n					\b RSTO_ENABLE=TRUE -> Enable RSTO, the PTC4/TPM3CH4/RSTO pin reflects the internal reset state
\n                  of the M.C.U.
\n					\b RSTO_ENABLE=FALSE -> Disable RSTO, the PTC4/TPM3CH4/RSTO pin is a a general-purpose port pin.
\n					\b RESET_ENABLE=TRUE -> Enable Reset Mode PTA5/IRQ/TCLK/RESET pin can be used to reset
\n                  the MCU from an external source when the pin is driven low.
\n					\b RESET_ENABLE=FALSE -> Disable Reset Mode PTA5/IRQ/TCLK/RESET pin is a general-purpose input port.
\n


\return 	IO_ErrorType

\retval		IO_E_OK

\note		\par \b Note 1:
			It's mandatory to perform this call before calling other services. \n
			\par \b Note 2:
			Mar 04,2011 Target AG128 call this routine before WCT elapsed !!

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
IO_ErrorType OCWDT_InitSync(void)
{
	WDT_InitSync();
#if (WDT_WINDOWED_MODE)
	WatchDog.feedTheDog = 0;
	WatchDog.free = 0;
#endif
	return (IO_ErrorType)IO_E_OK;
}

/**
\n This function refreshes the watchdog.
\n If it is not called before the watchdog counter reaches the timeout period,
\n a reset is generated. If the windowing is enabled, the refresh function must
\n be call after the windowing time is counted and before the watchdog time out is elapsed.

\return 	IO_ErrorType

\retval		IO_E_OK	refresh performed
\retval		IO_E_UNEXPECTED_CONDITION no refresh needed if watchdog disabled
\retval     IO_E_FAIL watchdog's refresh not allowable

\note		- added define WDT_WINDOWED_MODE and code.
            - added macro WDT_Enabled()

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
void OCWDT_RefreshSync(void)
{
	if ( WDT_Enabled() )
	{
#if (WDT_WINDOWED_MODE)
        if(WDT_IsWindowOpen())
        {
            if ( WatchDog.feedTheDog )
			{
				/*TODO MS EnterCriticalSection();*/
				WDT_Refresh();
				WatchDog.feedTheDog = 0;
				/* TODO MS ExitCriticalSection();*/
			}
			return; /* (IO_ErrorType)IO_E_OK; */
		}
		else
		{
			WatchDog.feedTheDog = 1;
			return; /*(IO_ErrorType)IO_E_FAIL;*/
		}
#else

        /* MS EnterCriticalSection(); */
		WDT_Refresh();
		/* MS ExitCriticalSection(); */
#endif  /* #if (WDT_WINDOWED_MODE) */
	}
}

/**
\n This function sets the desired timeout.
\n\li MCF51_AG128 influences the time-out register.
\n\li HCS08 unsupported
\n\li RN_300H_39079 to check

\param  [in]	timeout  specifies the timeout value.
\return 	    IO_ErrorType

\retval		    IO_E_OK
\retval         IO_E_UNEXPECTED_CONDITION  action denied

\note		    Watchdog is not refreshed by this fuction.
                - added macro WDT_AllowUpdate()

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
IO_ErrorType OCWDT_SetSync(WDT_timeoutType timeout)
{
	if (WDT_AllowUpdate())
	{
		WDT_SetSync(timeout);
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}

/**
\n This function deactivates the watchdog.
\n\li MCF51_AG128 The function influences the enable bit of the control register.
\n\li HCS08 unsupported
\n\li RN_300H_39079 supported

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNEXPECTED_CONDITION action denied

\note		added macro WDT_AllowUpdate()

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
IO_ErrorType OCWDT_StopSync(void)
{
	if (WDT_AllowUpdate())
	{
		WDT_StopSync();
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}
/**
\n This function activates the watchdog.
\n\li MCF51_AG128 The function influences the enable bit of the control register.
\n\li HCS08 unsupported
\n\li RN_300H_39079 supported

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNEXPECTED_CONDITION action denied

\note		added macro WDT_AllowUpdate()

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
void OCWDT_StartSync(void)
{
	if (WDT_AllowUpdate())
	{
		WDT_StartSync();
		/* return (IO_ErrorType)IO_E_OK; */
	}
	else
	{
		PLT_ERROR_FATAL(PLT_ERROR_OCWDT_NOSTART);
		/* return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; */
	}
}

/**
\n This function sets the desired window.
\n\li MCF51_AG128 the function influences the window register.
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported

\param  [in]	window  specifies the window value.
\return 	    IO_ErrorType

\retval		    IO_E_OK	window register changed
\retval         IO_E_UNEXPECTED_CONDITION

\note		    Watchdog is not refreshed by this fuction.
                - added macro WDT_AllowUpdate()
                - added conditional define WDT_WINDOWED_MODE

\par Architecture Requirements ID: SARS_WMMT_MN_DrvWTD_2
*/
IO_ErrorType OCWDT_SetWindowingSync(WDT_windowType window)
{
#if (WDT_WINDOWED_MODE)
	if (WDT_AllowUpdate())
	{
		WDT_SetWindowingSync(window);
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
#else
	return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
#endif /* #if (WDT_WINDOWED_MODE) */
}


/**
\n This function returns the watchdog counter register.
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 to check

\return 	    Watchdog counter
\retval		    WDT_counterType

*/
WDT_counterType OCWDT_ReadCounter(void)
{
	return WDT_ReadCounter();
}

/**
\n This function returns the watchdog window register.
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported

\return 	    Watchdog window register
\retval		    WDT_windowType
*/
WDT_windowType OCWDT_ReadWindow(void)
{
	return WDT_GetWindow();
}
/**
This function reads the watchdog reset counter register.
\n\li MCF51_AG128 supported
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported

\return 	    Watchdog reset counter

\retval		    WDT_resetCntrRegType
*/
WDT_resetCntrRegType OCWDT_ReadRSTCounter(void)
{
	return WDT_RSTCounter();
}

/**
\n This function resets the watchdog reset counter register.
\n\li MCF51_AG128 The function influences the RST counter register.
\n\li HCS08 unsupported
\n\li RN_300H_39079 unsupported

\return 	    IO_ErrorType

\retval		    IO_E_OK
*/
IO_ErrorType OCWDT_ResetRSTCounter(void)
{

	WDT_ClearRSTCounter();

	return (IO_ErrorType)IO_E_OK;
}

/* *********** */
/* End of file */
/* *********** */

