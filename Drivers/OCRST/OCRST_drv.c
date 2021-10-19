/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		OCRST_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCRST_drv.c
\ingroup	OCRST
\date		May 4, 2010
\version	00.00.01
\author		Jaroslav Musil, Alessandro Sabbatini
\n
\n
\Par\b	History:
\n
\n 		00.00.01
\n		04/05/2010
\n		Jaroslav Musil
\n		First Insertion
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */
#include "OCRST_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if !defined (NO_BOOT_LOADER)
#define NO_BOOT_LOADER FALSE
#endif
/** Must be other than 0 */
#define USER_RST_KEYWORD    0xAABB

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/** This is a keyword used to detect an user reset */
#if defined (__CWCC__)     
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */
/** This variable contains ID reset's source */
NO_INIT_VAR uint16_t UserResetKeyWord;

#elif defined(__ICCARM__)
#define __ram_initialization __iar_data_init3
#pragma location = ".noinit"
/** This variable contains ID reset's source */
 __no_init uint16_t UserResetKeyWord;
#elif defined(__GNUC__)
uint16_t UserResetKeyWord __attribute__ ((section (".noinit")));
#else
#error "No IDE defined"
uint16_t UserResetKeyWord;
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
Determines the reset source comparing the reset source register and its copy
\n provided by BootLoader.
\n If Boot Loader is not present in your application, please insert #define NO_BOOT_LOADER TRUE in preprocessor.

\return 	RST_Type
\retval		reset source identification

\par Architecture Requirements ID: SARS_WMMT_MN_DrvRST_2
*/
RST_Type OCRST_GetResetSourceSync(void)
{

	resetSource_t rstSrc = _RST_COPY_VALUE;
	RST_Type rstID;

	if ( (SYS_UserReset(rstSrc)) && ( USER_RST_KEYWORD == UserResetKeyWord ) )
	{
		rstID = RST_BY_USER;
	}
	else if (SYS_PowerOnReset(rstSrc))
	{
		rstID = RST_BY_POWER_ON;
	}
	else if (SYS_ExternalPinReset(rstSrc))
	{
		rstID = RST_BY_PIN;
	}
	else if (SYS_WatchdogReset(rstSrc))
	{
		rstID = RST_BY_WATCHDOG;
	}
	else if (SYS_LowVoltageReset(rstSrc))
	{
		rstID = RST_BY_LOW_VOLT_DETECT;
	}
	else if (SYS_LossOfClockReset(rstSrc))
	{
		rstID = RST_BY_LOSS_OF_CLOCK;
	}
	else if (SYS_IllegalOpcodeReset(rstSrc))
	{
		rstID = RST_BY_ILLEGAL_OPCODE;
	}
	else if (SYS_IllegalAddressReset(rstSrc))
	{
		rstID = RST_BY_ILLEGAL_ADDR_ACCESS;
	}
	else if (SYS_DebuggerReset(rstSrc))
	{
		rstID = RST_BY_DEBUGGER;
	}
	else
	{
		rstID = RST_UNKNOWN;
	}
	UserResetKeyWord = 0;
	return (rstID);

}

/**
Sets the reset source, according to rstValue, either by the reset source register or by its copy.

\param  [in]	rstValue id of reset

\return 	IO_ErrorType
\retval		IO_E_OK

\note		Writing the reset source register might not be available on every core

*/
IO_ErrorType OCRST_SetSync(RST_Type rstValue)
{
	volatile resetSource_t *pRstSrc = (&_RST_COPY_VALUE);
	IO_ErrorType err = (IO_ErrorType)IO_E_OK;

	switch(rstValue)
	{
	case RST_BY_POWER_ON:
		SYS_SetPowerOnReset(*pRstSrc);
		break;
	case RST_BY_PIN:
		SYS_SetExternalPinReset(*pRstSrc);
		break;
	case RST_BY_WATCHDOG:
		SYS_SetWatchdogReset(*pRstSrc);
		break;
	case RST_BY_LOW_VOLT_DETECT:
		SYS_SetLowVoltageReset(*pRstSrc);
		break;
	case RST_BY_LOSS_OF_CLOCK:
		SYS_SetLossOfClockReset(*pRstSrc);
		break;
	case RST_BY_ILLEGAL_OPCODE:
		SYS_SetIllegalOpcodeReset(*pRstSrc);
		break;
	case RST_BY_ILLEGAL_ADDR_ACCESS:
		SYS_SetIllegalAddressReset(*pRstSrc);
		break;
	case RST_BY_USER:
		UserResetKeyWord = USER_RST_KEYWORD;
		SYS_SetUserReset(*pRstSrc);
		break;
	case RST_BY_DEBUGGER:
		SYS_SetDebuggerReset(*pRstSrc);
		break;
	default:
		err = (IO_ErrorType)IO_E_UNKNOWN_MODE;
		break;

	}

	return (err);
}

/**
This function generates an user reset.
*/
void OCRST_GenerateUserResetSync(void)
{
	EnterCriticalSection();

	UserResetKeyWord = USER_RST_KEYWORD;
	UserReset();

	/* Fallback to watchdog reset if User Reset fails */
	while(1) {
		volatile int foo=0;
		foo++;
	}
}
/**
Checks if MCU requires a reset under some conditions.

\return 	bool_t

\retval		TRUE MCU requires reset
\retval		FALSE MCU doesn't requires reset

*/
bool_t OCRST_MCU_RequiresResetAfterPOR(void)
{
	if ((BootShortInfo.signature != BOOT_SIGNATURE) && (MCU_RequiresResetAfterPOR()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/**
Provides resetSource value to save.
Used to preserve value during required reset (e.g. ClassB WD Check)

\return 	resetSource_t

\retval		ResetSrc value (registers map)

*/
resetSource_t OCRST_SaveResetValue(void)
{
	return _RST_COPY_VALUE;
}

/**
Set resetSource value saved previously.
Used to preserve value during required reset (e.g. ClassB WD Check);
it restores UserResetKeyWord if an user reset occurred.

\param  [in]	resetSource_t ResetSrc value (registers map)
\return	none
*/
void OCRST_RestoreResetValue(resetSource_t resetCopyVal)
{
	_RST_COPY_VALUE = resetCopyVal;

	if (SYS_UserReset(resetCopyVal))
	{
		UserResetKeyWord = USER_RST_KEYWORD;
	}
}

/**
Copy resetSource from real registers.
Used to simulate Bootloader ResetSource copy when bootloader is missing

\param  none
\return	none
*/
void OCRST_CopyFromRealResetSrc(void)
{
	OCRST_RestoreResetValue(SYS_GetResetSrcVal());
}

/**
Set BootStartKey

\param  none
\return	none
*/
void OCRST_WriteBootKeywordArea(void)
{
	ResetInfo.BootStartKey= BOOT_START_KEY;
}
void OCRST_WriteBootSettingKeywordArea(void)
{
	ResetInfo.BootStartKey= BOOT_SETTING_START_KEY;
}
/* *********** */
/* End of file */
/* *********** */

