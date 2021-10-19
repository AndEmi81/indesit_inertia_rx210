/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_Setting
\n\li		General Purpose
			GetDefaultSettingStartAddress()
\n\li		Test of setting file
\n\lo		POST - LIB_SettingPOST_Sync() tests the whole setting
\n\lo		BIST - test made by chunks, no internal state is kept or managed by this library.
			methods: LIB_SettingBIST_StartSync(), LIB_SettingBIST_StepSync(), LIB_SettingBIST_EndSync()
\n
\par		Methods
\n			Depending on the CONFIG_LIB_SETTING_CHECK_TYPE_CRC two methods are available:
\n\li		modified CHK
\n\li		CRC16 CCITT(FFFF)
\n			the method must be choosen accoding to the application
\n
\file		LIB_Setting.c
\ingroup	LIB_Setting
\date		Apr 18, 2013
\version	01.00.00
\author		Roberto Fioravanti
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "LIB_Setting.h"
#include "LIB_Utilities.h"
#include "LIB_Generic.h"
#include "LIB_crcExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */



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

/* ********* */
/* Functions */
/* ********* */

/**
Synchrounous implementation of POST (Pre-Operation Self Test) to verify the integrity
of the whole setting file checking if the signature matches.

\return 	bool_t

\retval		TRUE 	test passed
\retval		FALSE	test not passed

\author 	Roberto Fioravanti
\date		Apr 18,2013
\version	1.0.0
*/
bool_t LIB_SettingPOST_Sync(void)
{
	bool_t res=FALSE;
	LIB_SettingSignatureType sign=0;
	uint8_t* pStart=0;
	uint16_t len=0;

	if (LIB_SettingBIST_StartSync(&len, &pStart, &sign))
	{
		if (len)
		{
			sign=LIB_SettingBIST_StepSync(len, pStart, sign);
			res=LIB_SettingBIST_EndSync(sign);
		}
	}
	return res;
}


/*
Allows setting BIST initialization.
This method provides start address, number of bytes, and the initial seed
\param [out]	pLen	pointer to len in bytes
\param [out]	ppStart	pointer to start address pointer
\param [out]	pInitSeed	initial seed to be used to test setting. It is affected by CONFIG_LIB_SETTING_CHECK_TYPE_CRC:
if defined CONFIG_LIB_SETTING_CHECK_TYPE_CRC checking is CRC else CHK

\return 	bool_t

\retval		TRUE 	test inizialized
\retval		FALSE	test not inizialized

\author 	Roberto Fioravanti
\date		Apr 18,2013
\version	1.0.0
*/
bool_t	LIB_SettingBIST_StartSync(uint16_t* pLen, uint8_t** ppStart, LIB_SettingSignatureType* pInitSeed)
{
	bool_t res = FALSE;
	uint16_t chkPtrVal = EndianU16_BToN(*(GetDefaultSettingStartAddress() + SF_PTR_CHECKSUM));

	if(pLen && ppStart && pInitSeed)
	{
		*pLen=0;

		if(chkPtrVal < APPL_SETTING_SIZE_W)
		{
			*pLen=chkPtrVal*2;

			*ppStart=(uint8_t*)GetDefaultSettingStartAddress();

#if defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)
			*pInitSeed=0xFFFF;
#else
			*pInitSeed=0;
#endif /*defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)*/
			res = TRUE;
		}
	}
	return res;
}


/**
Setting BIST single step.

\param  [in]		len number of bytes to test
\param  [in]		ppStart pointer to the start address
\param  [in]		initSeed is the initial seed

\n The applied method is affected by CONFIG_LIB_SETTING_CHECK_TYPE_CRC
if defined CONFIG_LIB_SETTING_CHECK_TYPE_CRC checking is CRC else CHK

\return 	LIB_SettingSignatureType

\retval		partial result taking into account the result of the earlie call to this method

\author 	Roberto Fioravanti
\date		Apr 18, 2013
\version	1.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
 */
LIB_SettingSignatureType	LIB_SettingBIST_StepSync(uint16_t len, uint8_t* ppStart, LIB_SettingSignatureType initSeed)
{
	LIB_SettingSignatureType	sign=0;
	if(ppStart)
	{
#if defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)
		sign=LIB_crc_partial_calculate(len, ppStart, initSeed);
#else
		sign=LIB_ModifiedChk_Computation(len, ppStart, initSeed);
#endif /*defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)*/
	}
	return sign;
}




/*
final step
if returns if the partial resutll matches the signature of the setting
\param [in]	sign	computed signature
\n The applied method is affected by CONFIG_LIB_SETTING_CHECK_TYPE_CRC
if defined CONFIG_LIB_SETTING_CHECK_TYPE_CRC checking is CRC else CHK

\return 	bool_t

\retval		TRUE 	test passed
\retval		FALSE	test not passed

\author 	Roberto Fioravanti
\date		Apr 18,2013
\version	1.0.0
*/
bool_t	LIB_SettingBIST_EndSync(LIB_SettingSignatureType sign)
{
	LIB_SettingSignatureType	settingSign=0;
	uint16_t chkPtrVal=0;

	chkPtrVal = EndianU16_BToN(*(GetDefaultSettingStartAddress() + SF_PTR_CHECKSUM));

#if defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)
	settingSign=STD_SF_PTR_CHECKSUM_CRC_VAL((STD_SettingSignatureType*)(GetDefaultSettingStartAddress() + chkPtrVal));
#else
	settingSign=STD_SF_PTR_CHECKSUM_CHK_VAL((STD_SettingSignatureType*)(GetDefaultSettingStartAddress() + chkPtrVal));
#endif /*defined(CONFIG_LIB_SETTING_CHECK_TYPE_CRC)*/
	return (sign==settingSign);
}

/* *********** */
/* End of file */
/* *********** */

