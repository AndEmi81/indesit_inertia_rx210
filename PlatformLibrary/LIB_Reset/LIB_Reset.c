/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_Reset.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_Reset.c
\ingroup	LIB_Reset
\date		Mar 7, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Mar 7, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n 		01.00.01
\n		Mar 28, 2011
\n		stefano.ughi
\n		changes
\n 			- added void LIB_Reset_ForceColdReset(void)
\n
\n 		01.00.02
\n		Mar 31, 2011
\n		stefano.ughi
\n		changes
\n 			- removed #include "LIB_ResetExp.h"
\n
\n 		01.00.03
\n		May 12, 2011
\n		stefano.ughi
\n		changes
\n 			- added sections and variables declarations for LPC122x
*/

#define MODULE_BODY_LIB_RESET
/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "Config.h"
#include "OCWDT_drv.h"
#include "OCCLK_drv.h"
#include "OCFLS_drv.h"

#if defined(CONFIG_PL_DRV_OCCRC)
#include "OCCRC_drv.h"
#else
#include "SWCRC_drv.h"
#endif

#include "LIB_Backup.h"
#include "LIB_Reset.h"
#include "LIB_Generic.h"

#include "LIB_Setting.h"
#if defined(CONFIG_PL_CLASS_B_CHECK)
#include "LIB_ClassBCheck.h"
#endif
#include "LIB_ClassBVar.h"

#include "LIB_Trace.h"
#if defined(CONFIG_OCLPM_DRV_ENABLE)
#include "LIB_LowPowerMode.h"
#endif /*defined(CONFIG_OCLPM_DRV_ENABLE)*/
/* ****************** */
/* Defines and Macros */
/* ****************** */

#define RST_BY_RAM_ERR_MASK     (1<<(sizeof(RST_Type)*BYTE_SIZE - 1))


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


typedef struct LIB_Reset_TimedRequest_s
{
	uint16_t reqChkKeyword;
	TOOLS_TIMER Timeout;
} LIB_Reset_TimedRequestType;

/** Struct for keyword's management */
typedef struct KeyWordsInfo_Tag
{
	/** R.O.M. pointer */
	void *pSource;
	/** R.A.M. pointer */
	void *pTarget;
	/** data's size in bytes */
	size_t size;
} KeyWordsInfo_t;



/* ********* */
/* Variables */
/* ********* */

/* Global */

/** BootVersionInfo data structure:

   uint8_t is_present;
   uint8_t ver_major;
   uint8_t ver_minor;
   uint8_t ver_debug;

*/
uint8_t BootVersionInfo[4];

/* Static */
#if defined CONFIG_LIB_RESET_TIMED_REQ
/** Timed request */
static LIB_Reset_TimedRequestType TimedRequest;
#endif

/** Keywords */
static const int8_t KeyRomInfo_1[] = "Solo due cose sono infinite: l'universo e la stupidita' umana e non sono sicuro della prima.";
/** Keyword */
static const int8_t KeyRomInfo_2[] = "Il dubbio non e' piacevole, ma la certezza e' ridicola. Solo gli imbecilli son sicuri di cio' che dicono.";
/*----------------------------------------------------------------*/
/*              TOOL CHAIN DEPENDENT DECLARATIONS                 */
/*----------------------------------------------------------------*/      
/* ***************** Codewarrior **********************************/
#if defined (__CWCC__)     
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */

/** Reset control status var - ResetSrc + System mode + Flags */
NO_INIT_VAR ResetStatus_t ResetCtrl;

/** Counters showing the number of reset after a "cold" reset */
NO_INIT_VAR uint16_t ResetCnt[NUM_RST_SOURCE];

#pragma define_section key_1 ".key_block_1" far_absolute
#if _lint
#define KEY_1_BLOCK
#else   /* _lint */
#define KEY_1_BLOCK __declspec(key_1)
#endif  /* _lint */
/** Keywords */
KEY_1_BLOCK static uint8_t KeyRamInfo_1[sizeof(KeyRomInfo_1)];
#pragma define_section key_2 ".key_block_2" far_absolute
#if _lint
#define KEY_2_BLOCK
#else   /* _lint */
#define KEY_2_BLOCK __declspec(key_2)
#endif  /* _lint */
/** Keywords */
KEY_2_BLOCK static uint8_t KeyRamInfo_2[sizeof(KeyRomInfo_2)];

/* ***************** EWARM **********************************/
#elif defined(__ICCARM__)
#define __ram_initialization __iar_data_init3
#pragma location = ".noinit"
	/** Reset control status var - ResetSrc + System mode + Flags */
 __no_init ResetStatus_t ResetCtrl;
	/** Counters showing the number of reset after a "cold" reset */
 __no_init uint16_t ResetCnt[NUM_RST_SOURCE];
#pragma location = ".key_block_1"
static uint8_t KeyRamInfo_1[sizeof(KeyRomInfo_1)];
#pragma location = ".key_block_2"
static uint8_t KeyRamInfo_2[sizeof(KeyRomInfo_2)];
#elif defined(__GNUC__)

/** Reset control status var - ResetSrc + System mode + Flags */
	ResetStatus_t ResetCtrl __attribute__ ((section (".noinit")));

	/** Counters showing the number of reset after a "cold" reset */
	uint16_t ResetCnt[NUM_RST_SOURCE] __attribute__ ((section (".noinit")));
	
	/** Keywords */
	static uint8_t KeyRamInfo_1[sizeof(KeyRomInfo_1)] __attribute__ ((section (".key_block_1")));
	/** Keywords */
	static uint8_t KeyRamInfo_2[sizeof(KeyRomInfo_2)] __attribute__ ((section (".key_block_2")));


#else
#error "No IDE defined"

	/** Reset control status var - ResetSrc + System mode + Flags */
	ResetStatus_t ResetCtrl;
	/** Counters showing the number of reset after a "cold" reset */
	uint16_t ResetCnt[NUM_RST_SOURCE] ;

	static uint8_t KeyRamInfo_1[sizeof(KeyRomInfo_1)];
	static uint8_t KeyRamInfo_2[sizeof(KeyRomInfo_2)];

#endif
/*----------------------------------------------------------------*/
/*           END TOOL CHAIN DEPENDENT DECLARATIONS                */
/*----------------------------------------------------------------*/
/** KeyInfo is a null terminated array of KeyWordsInfo_t type */
static const KeyWordsInfo_t KeyInfo[]=
{
	{(void*)KeyRomInfo_1,(void*)KeyRamInfo_1,sizeof(KeyRomInfo_1)},
	{(void*)KeyRomInfo_2,(void*)KeyRamInfo_2,sizeof(KeyRomInfo_2)},
	{NULL, NULL,0}
};

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void Set_NewSystemMode ( RST_Type rstSrc );
static void LIB_ClearResetCounters(void);
static void LIB_UpdateResetCounters(void);

static bool_t LIB_Reset_SetProgrammingModeRequest ( SysMode_Type newMode, SysSubMode_Type newSubMode );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
\n This function is called inside _startup after an M.C.U. reset and manage reset's source
\n in order to define application behaviour, SystemMode setup and "cold" or "hot" reset
\n are performed.

\attention \b ResetSrc  R/W no init
\attention \b NewSystemMode  R/W no init
\attention \b SystemMode  R/W no init
\attention \b Reset  R/W no init

\author 	stefano.ughi
\date		Mar 3, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
*/
/**
\n Called by _startup after S.P. initialization for hardware's initializations
\n that must be performed as soon as possible.

\author 	stefano.ughi
\date		Mar 08,2011
\version	1.0.0

\note		\par \b Nota 1:
		documentation by Stefano Ughi

*/
void __lib_reset_manager(void)
{
	bool_t ColdReset = TRUE;
	bool_t CheckKeyWords = FALSE;
	RST_Type resetSource;

#if defined(CONFIG_OCLPM_DRV_ENABLE)
	/* support for sleep mode */
	if( (ResetCtrl.NewSystemMode==SYSTEM_SLEEP_MODE) && LIB_CheckKeywordsRam() )
	{
		ResetCtrl.SystemMode = ResetCtrl.NewSystemMode;
		SAFETY_DATA_UPDATE(ResetCtrl);/* the OS in not started , no safety region*/
		LIB_LPM_Sleep_Manager();
		
		/* This point should never be reached because of previous sleep instruction */
		return;
	}
#endif /* SLEEP_MODE_ENABLE */

	resetSource = OCRST_GetResetSourceSync();

	switch (resetSource)
	{
	case RST_BY_POWER_ON:
		ResetCtrl.ResetSrc = resetSource;
		Set_NewSystemMode ( resetSource );
		ResetCtrl.Reset = LIB_RESET_FLAG_APPL_POR;
		if ( OCRST_MCU_RequiresResetAfterPOR() == TRUE )
		{
			/* Flag condition */
			ResetCtrl.Reset |= LIB_RESET_FLAG_AFTER_POR;
			SAFETY_DATA_UPDATE(ResetCtrl); /* the OS in not started , no safety region*/
			OCRST_GenerateUserResetSync();
		}
		else
		{
			ResetCtrl.Reset &= ~LIB_RESET_FLAG_AFTER_POR;
		}
		break;

	case RST_BY_LOW_VOLT_DETECT:
	case RST_BY_ILLEGAL_OPCODE:
	case RST_BY_PIN:
	case RST_BY_LOSS_OF_CLOCK:
	case RST_BY_WATCHDOG:
	case RST_BY_ILLEGAL_ADDR_ACCESS:
	case RST_BY_DEBUGGER:
		CheckKeyWords = TRUE;
		ResetCtrl.ResetSrc = resetSource;
		Set_NewSystemMode ( resetSource );
		break;
		
	case RST_BY_USER:

		/* Checks previous reset */
		if ( ( ResetCtrl.Reset & LIB_RESET_FLAG_AFTER_POR ) && ( RST_BY_POWER_ON == ResetCtrl.ResetSrc ) )
		{
			/* Note ResetSrc not changed ! */
			Set_NewSystemMode ( RST_BY_POWER_ON );
			ResetCtrl.Reset &= ~LIB_RESET_FLAG_AFTER_POR;
		}
		else
		{
			CheckKeyWords = TRUE;
			ResetCtrl.ResetSrc = resetSource;
			Set_NewSystemMode ( resetSource );
		}
		break;

	default:
		/* Unknown Reset Source (ResetSrc = 0) */
		ResetCtrl.ResetSrc = resetSource;
		Set_NewSystemMode ( resetSource );
		break;
	}

	if ( SYSTEM_NORMAL_MODE == ResetCtrl.NewSystemMode )
	{
		if ( LIB_SettingPOST_Sync() == FALSE )
		{
			ResetCtrl.NewSystemMode = SYSTEM_UNCONSISTENT_SETTING_FILE_MODE;
			ResetCtrl.NewSystemSubMode = SYSTEM_NO_SUBMODE;
		}
	}
	
	ResetCtrl.SystemMode = ResetCtrl.NewSystemMode;
	ResetCtrl.SystemSubMode = ResetCtrl.NewSystemSubMode;
	
	if ( SYSTEM_UNCONSISTENT_SETTING_FILE_MODE == ResetCtrl.SystemMode )
	{
		/* Se il file di setting risulta non buono non ha senso fare un reset caldo,
		   meglio reinizializzare tutta la RAM */
		CheckKeyWords = FALSE;
	}

	if ( TRUE == CheckKeyWords )
	{
		if ( LIB_CheckKeywordsRam() == TRUE )
		{
			ColdReset = FALSE;			/* cancel COLD reset request */
		}
		else
		{
			ColdReset = TRUE;			/* confirm COLD reset request */
			/* Tag this condition setting M.S.B. of ResetSrc */
			ResetCtrl.ResetSrc |= RST_BY_RAM_ERR_MASK;
		}
		CheckKeyWords = FALSE;
	}

	if ( TRUE == ColdReset )
	{
#if ( defined(CONFIG_LIB_CLASS_B_RAM) || defined(CONFIG_LIB_CLASS_B_FLASH))
		/* Memory check can be skipped if entering Programming Mode       *
		 * it cannot happen e.g. at Power On, it must pass to another one */
		if ( SYSTEM_PROGRAMMING_MODE != ResetCtrl.SystemMode )
		{
			InitMemoryCheck();
		}
#endif
		/* this function is implemented inside startup because target dependent */
		__ram_initialization();
		
		LIB_KeywordsRamCorruption();
		LIB_ClearResetCounters();
		SAFETY_CHECK_ENABLE();
#if defined CONFIG_LIB_RESET_TIMED_REQ
		Tools_DisableTimeout(&ResetCtrl.Reset_timeout);
#endif

		ResetCtrl.Reset |= LIB_RESET_FLAG_IS_COLD_RESET;
		ResetCtrl.Reset &= ~LIB_RESET_FLAG_IS_REMOTE_CTRL;
	}
	else
	{
		ResetCtrl.Reset &= ~LIB_RESET_FLAG_IS_COLD_RESET;
	}

	LIB_UpdateResetCounters();
	/** updare crc, the OS is not started */
	SAFETY_DATA_UPDATE(ResetCtrl);/* the OS in not started , no safety region*/

	/* Clean or update BootVersionInfo */
	if (BootShortInfo.signature == BOOT_SIGNATURE)
	{
		BootVersionInfo[0] = 1;
		memcpy(BootVersionInfo+1, BootShortInfo.shortInfo+1,3);
	}
	else
	{
		memset(BootVersionInfo, 0, 4);
	}

}
/**
\n Called by _startup after S.P. initialization for hardware's initializations
\n that must be performed as soon as possible.

\author 	stefano.ughi
\date		Mar 08,2011
\version	1.0.0

\note		\par \b Nota 1:
		documentation by Stefano Ughi

*/
void __initialize_hardware(void)
{

	(void)OCWDT_InitSync();
    (void)OCCLK_SetClockSync(SYS_FREQ_Hz);

#if defined(CONFIG_LIB_CLASS_B_CPU) && !defined(BOOTLOADER_IEC60335)
    RegCheck();
#endif /* CONFIG_LIB_CLASS_B_CPU*/

	(void)OCFLS_InitSync();
#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)
	OCDIO_InitSync();
#endif

	LIB_Backup_SetBackupControlMonitor(BACKUP_NO_SOURCE);

	if (BootShortInfo.signature != BOOT_SIGNATURE)
	{
	    OCRST_CopyFromRealResetSrc();
	}
#if defined(CONFIG_PL_CLASS_B_CHECK) && !defined(BOOTLOADER_IEC60335)
    InitCheck();
#endif
    /* CRC driver used by: Lib ClassB var (Pre-Operation Self Test), communitation,  */
#if defined(CONFIG_PL_DRV_OCCRC)
	( void ) OCCRC_InitSync();
#else
	SWCRC_InitSync();
#endif
}

/**
Checks Keywords's value.

\return 	bool_t

\retval		TRUE Test passed
            FALSE Test failed

\author 	stefano.ughi
\date		Mar 08,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_CheckKeywordsRam(void)
{
	const KeyWordsInfo_t  *pInfo;
	int cmpResult;

	for ( pInfo = KeyInfo; pInfo->size != 0; pInfo++ )
	{
		cmpResult = memcmp(pInfo->pSource, pInfo->pTarget, pInfo->size);
		if ( cmpResult )
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**
\n Sets Keywords to a prefixed value.
\n This service must be called after all initializations in case of cold reset event.

\author 	stefano.ughi
\date		Mar 08,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_SetKeywordsRam(void)
{
	const KeyWordsInfo_t  *pInfo;

	for ( pInfo = KeyInfo; pInfo->size != 0; pInfo++ )
	{
		(void)memcpy(pInfo->pTarget, pInfo->pSource, pInfo->size);
	}
}
/**
\n Corrupts the Keywords's value.

\author 	stefano.ughi
\date		Mar 10,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_KeywordsRamCorruption(void)
{
	const KeyWordsInfo_t  *pInfo;

	for ( pInfo = KeyInfo; pInfo->size != 0; pInfo++ )
	{
		(void)memset(pInfo->pTarget, 0xEE, pInfo->size);
	}
}
/**
\n Forces a "cold" reset corrupting Keyword and performing an user reset.

\author 	stefano.ughi
\date		Mar 28,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_Reset_ForceColdReset(void)
{
	/* A cold reset is forced; calling OS_Clean() would help to make it properly
	   even if OS is not yet started */
	OS_Clean();
	KeyRamInfo_1[0]=(uint8_t) (!KeyRamInfo_1[0]);
	OCRST_GenerateUserResetSync();
}

/**
\n Forces an User reset .

\author         Alessandro Sabbatini
\date           Aug 03,2012
\version        1.0.0

\note           \par \b Nota 1:
                        documentation by Alessandro Sabbatini

*/
void LIB_Reset_ForceReset(void)
{
    OCRST_GenerateUserResetSync();
}

/**
This function is called by Application and returns if a POR happened .

\attention \b Reset.com_appl_POR noinit R

\return 	bool_t

\retval     TRUE POR happened
\retval     FALSE POR not happened

\author 	stefano.ughi
\date		Mar 11,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_Reset_GetPOR_flag(void)
{
	bool_t retVal = FALSE;
	if ( ResetCtrl.Reset & LIB_RESET_FLAG_APPL_POR )
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(ResetCtrl);
	
		ResetCtrl.Reset &= ~LIB_RESET_FLAG_APPL_POR;

		SAFETY_DATA_UPDATE(ResetCtrl);
		SAFETY_REGION_END();
		
		retVal = TRUE;
	}

	return retVal;
}
/**
This function returns IsColdReset value.

\return 	bool_t

\retval     TRUE reset "cold"
\retval     FALSE reset "hot"

\author 	stefano.ughi
\date		Mar 11,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_Reset_GetIsColdReset(void)
{
	return ((ResetCtrl.Reset & LIB_RESET_FLAG_IS_COLD_RESET) ? TRUE : FALSE );
}
/**
This function returns SystemMode value.

\return 	SysMode_Type

\author 	stefano.ughi
\date		Mar 14,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
SysMode_Type LIB_Reset_GetSystemMode(void)
{
	return(ResetCtrl.SystemMode);
}

/**
This function returns SystemMode value.

\return 	SysMode_Type

\author 	stefano.ughi
\date		Mar 14,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
SysSubMode_Type LIB_Reset_GetSystemSubMode(void)
{
	return(ResetCtrl.SystemSubMode);
}

/**
This function returns NewSystemMode value.

\return 	SysMode_Type

\author 	stefano.ughi
\date		Mar 14,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
SysMode_Type LIB_Reset_GetNewSystemMode(void)
{
	return(ResetCtrl.NewSystemMode);
}

/**
This function sets NewSystemMode.

\param  [in]    value new value for NewSystemMode

\author 	stefano.ughi
\date		Mar 14,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_Reset_SetNewSystemMode(SysMode_Type value)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	
	ResetCtrl.NewSystemMode = value;

	SAFETY_DATA_UPDATE(ResetCtrl);
	SAFETY_REGION_END();
}


/**
This function sets NewSystemSubMode.

\param  [in]    value new value for NewSubSystemMode

\author 	Roberto Fioravanti
\date		Mar 28,2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Roberto Fioravanti

*/
void LIB_Reset_SetNewSubSystemMode(SysSubMode_Type value)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	
	ResetCtrl.NewSystemSubMode = value;

	SAFETY_DATA_UPDATE(ResetCtrl);
	SAFETY_REGION_END();
}


/**
Sets request for new system mode and submode according to reset's source and previous NewSystemMode's value.

\param  [in]    newMode mode
\param  [in]    newSubMode submode

\attention \b   NewSystemMode new mode for Application R/W


\author 	Roberto Fioravanti
\date		Mar 28, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
bool_t LIB_Reset_SetSystemModeRequest ( SysMode_Type newMode, SysSubMode_Type newSubMode )
{
	bool_t supported=FALSE;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	
	switch ( newMode )
	{
		case SYSTEM_NORMAL_MODE:
			ResetCtrl.NewSystemMode = newMode;
			ResetCtrl.NewSystemSubMode = newSubMode;
			supported = TRUE;
			break;

		case SYSTEM_PROGRAMMING_MODE:
			supported = LIB_Reset_SetProgrammingModeRequest ( newMode, newSubMode );
			break;

		case SYSTEM_UNCONSISTENT_SETTING_FILE_MODE:
		default:
			;
			break;
	}
	SAFETY_DATA_UPDATE(ResetCtrl);
	SAFETY_REGION_END();

	return supported;
}

/* Local Function */
/**
Sets NewSystemMode according to reset's source and previous NewSystemMode's value.

\param  [in]    rstSrc reset source

\attention \b   NewSystemMode new mode for Application R/W


\author 	stefano.ughi
\date		Mar 7, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
static void Set_NewSystemMode ( RST_Type rstSrc )
{
	if ( RST_BY_POWER_ON == rstSrc )
	{
		ResetCtrl.NewSystemMode = SYSTEM_NORMAL_MODE;
		ResetCtrl.NewSystemSubMode = SYSTEM_NO_SUBMODE;
	}
	else
	{
		/* For all other reset's sources now we assume the same behaviour */
		switch ( ResetCtrl.NewSystemMode )
		{
			case SYSTEM_NORMAL_MODE:
			case SYSTEM_PROGRAMMING_MODE:
			case SYSTEM_UNCONSISTENT_SETTING_FILE_MODE:
				break;
			default:
			{
				ResetCtrl.NewSystemMode = SYSTEM_NORMAL_MODE;
				ResetCtrl.NewSystemSubMode = SYSTEM_NO_SUBMODE;
			}
		}
	}
}




/**
Programming mode request management

\param  [in]    newMode mode
\param  [in]    newSubMode submode

\attention \b   NewSystemMode new mode for Application R/W


\author 	Roberto Fioravanti
\date		Mar 28, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
static bool_t LIB_Reset_SetProgrammingModeRequest ( SysMode_Type newMode, SysSubMode_Type newSubMode )
{
	bool_t valid=FALSE;

	switch ( newSubMode )
	{
		case	SYSTEM_NO_SUBMODE:
		case 	SYSTEM_PROGRAMMING_SAVEREG_MODE:
			valid=TRUE;
			break;
		default:
		{
			ResetCtrl.NewSystemMode=SYSTEM_NORMAL_MODE;
			ResetCtrl.NewSystemSubMode=SYSTEM_NO_SUBMODE;
		}
	}
	if(valid)
	{
		ResetCtrl.NewSystemMode=SYSTEM_PROGRAMMING_MODE;
		ResetCtrl.NewSystemSubMode=newSubMode;
	}
	return valid;
}

/**
\n Clear ResetCnt[].

\attention\b ResetCnt[] used as W

\author 	stefano.ughi
\date		Mar 10,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
static void LIB_ClearResetCounters(void)
{
	(void)memset(ResetCnt, 0x00, sizeof(ResetCnt));
}
/**
\n Updates ResetCnt[] starting from ResetSrc's value.

\attention\b ResetSrc used as R
\attention\b ResetCnt[] used as W

\author 	stefano.ughi
\date		Mar 10,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
static void LIB_UpdateResetCounters(void)
{
	RST_Type mirrorResetSrc;

	mirrorResetSrc = ResetCtrl.ResetSrc;
	if ( mirrorResetSrc & RST_BY_RAM_ERR_MASK )
	{
		ResetCnt[RST_BY_RAM_ERR]++;
		mirrorResetSrc &= (RST_Type) (~RST_BY_RAM_ERR_MASK);
	}
	if ( mirrorResetSrc < NUM_RST_SOURCE )
	{
		ResetCnt[mirrorResetSrc]++;
	}
}

/** Detect a cold reset		*/
bool_t LIB_Reset_IsColdReset(void)
{
	RST_Type resetSource;
	bool_t retVal = FALSE;

	resetSource = OCRST_GetResetSourceSync();
	if( resetSource == RST_BY_POWER_ON ){
		retVal = TRUE;
	}else if( LIB_CheckKeywordsRam() == FALSE ){
		retVal = TRUE;
	}
	return retVal;
}
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
void LIB_Reset_SafetyCheck(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	SAFETY_REGION_END();
}
#endif

#if defined(CONFIG_LIB_RESET_TIMED_REQ)

void LIB_Reset_ResetTimedRequest(void)
{
	TimedRequest.reqChkKeyword = 0;
	TimedRequest.Timeout = 0;
}

void LIB_Reset_SetTimedRequest(uint16_t chkKeyword, uint32_t timeout)
{
	TimedRequest.reqChkKeyword = chkKeyword;
	Tools_SetTimeout(&(TimedRequest.Timeout), timeout);
}

bool_t LIB_Reset_ChkTimedRequest(uint16_t chkKeyword)
{
    bool_t completed = FALSE;

    if (TimedRequest.reqChkKeyword == chkKeyword)
    {
    	completed = !Tools_IsTimeoutExpired(&(TimedRequest.Timeout));
    }

	return completed;
}

void LIB_Reset_SetResetTimeout(uint32_t timeout)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	Tools_SetTimeout(&(ResetCtrl.Reset_timeout), timeout);
	SAFETY_DATA_UPDATE(ResetCtrl);
	SAFETY_REGION_END();
}

bool_t LIB_Reset_IsTimeToReset(void)
{
	return ( Tools_IsTimeoutExpired(&(ResetCtrl.Reset_timeout)) );
}

bool_t LIB_Reset_IsTimeoutSet(void)
{
	return ( !Tools_IsTimeoutDisabled(&(ResetCtrl.Reset_timeout)) );
}

void LIB_Reset_EnterRemoteControl(uint32_t timeout)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ResetCtrl);
	ResetCtrl.Reset |= LIB_RESET_FLAG_IS_REMOTE_CTRL;
	if (timeout)
	{
		Tools_SetTimeout(&(ResetCtrl.Reset_timeout), timeout);
	}
	SAFETY_DATA_UPDATE(ResetCtrl);
	SAFETY_REGION_END();
}

bool_t LIB_Reset_IsRemoteControl(void)
{
	return ( (ResetCtrl.Reset & LIB_RESET_FLAG_IS_REMOTE_CTRL) ? TRUE : FALSE );
}

uint32_t LIB_Reset_GetResetTimeout(void)
{
	return (uint32_t) (ResetCtrl.Reset_timeout - OS_GetTime());
}

#endif

/* *********** */
/* End of file */
/* *********** */

