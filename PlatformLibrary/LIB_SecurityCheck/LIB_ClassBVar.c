/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n	Il file contiene le funzioni per la gestione delle variabili di classe B
\n
\file		LIB_ClassBVar.c
\ingroup    grpDataCare
\date	    20/09/2011
\version	01.00.00
\author		 Marco Sperandini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		20/09/2011
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/
#define MODULE_BODY_CLASSBVAR
/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "Config.h"
#include "LIB_ClassBVar.h"
#include "LIB_Error.h"
#include "LIB_Trace.h"

#if defined(CONFIG_LIB_CLASS_B_USE_OCCRC)
#include "OCCRC_drv.h"
#define LIB_ClassBVar_CRC_calculate OCCRC_Calculate
#else
#include "SWCRC_drv.h"
#define LIB_ClassBVar_CRC_calculate SWCRC_Calculate
#endif


#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#define DEFAULT_SEED				0xFFFF

#define DISABLE_CLASSB_KEYWORD		0xF00DBEEF

static uint32_t disableClassBVar;

/**
 * Nuova interfaccia variabili di classe B
 * sono allocate nel codice normalmente
 * vengono pero enumerate per poterle identificare
 * viene tenuta una tabella con i crc per ognuna
 */
#if defined (__CWCC__)
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */
NO_INIT_VAR uint16_t Sec_CrcExtVarTable[SEC_NUM_EXTVAR];
#elif defined(__GNUC__)
uint16_t Sec_CrcExtVarTable[SEC_NUM_EXTVAR]  __attribute__ ((section (".noinit")));
#else
uint16_t Sec_CrcExtVarTable[SEC_NUM_EXTVAR];
#endif

#if defined( CONFIG_PL_DEBUG )
static uint8_t  Sec_CrcExtVarTableResult[SEC_NUM_EXTVAR]  __attribute__ ((section (".noinit")));/*DEBUG*/
void Sec_InitSafety(void)
{	int i;
	for(i=0;i<SEC_NUM_EXTVAR; i++){
		Sec_CrcExtVarTableResult[i]=0;
	}
}
#endif

void Sec_SafeCheckFailHandler( uint16_t id);

void Sec_SafeCheckFailHandler( uint16_t id){
#if defined( CONFIG_PL_DEBUG )
	if( Sec_CrcExtVarTableResult[id] < 0xFF)
	{
		Sec_CrcExtVarTableResult[id]++;
	}
#endif
	if (!SAFETY_CHECK_ISDISABLED())
	{
		PLT_ERROR_FATAL(PLT_ERROR_DATACORRUPTION_1);
	}
}


void Sec_StartSafety(void){
	TRACE_DATA_INTEGRITY_START();
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL)	&& CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL > 0
#if CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL == 1
	OS_EnterRegion();
#else
	/* no safety region protection */
#endif
#else
	EnterCriticalSection();
#endif

}
void Sec_EndSafety(void){
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL)	&& CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL > 0
#if CONFIG_CLASS_B_DATA_INTEGRITY_SAFETYREGION_LEVEL == 1
	OS_LeaveRegion();
#else
	/* no safety region protection */
#endif
#else
	ExitCriticalSection();
#endif
	TRACE_DATA_INTEGRITY_END();
}

/**
\n	Data Safety check variable
\n	This function checks if variable is coherent with its check value;
\n	if not, calls Sec_SafeCheckFailHandler() - usually resets the system
\n
\param [in] 	pos			Position of related check value
\param [in,out] dataPtr		Pointer to data variable
\param [in] 	dataLen		Lenght of data variable


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/
void Sec_CheckExtVar(SecExtVarID_t pos, void* dataPtr, uint16_t dataLen)
{
#if  defined( CONFIG_PL_DEBUG )
	uint16_t crc = DEFAULT_SEED;
#endif

	PLT_ASSERT(pos<SEC_NUM_EXTVAR)

	if( !Sec_CheckIfVal(Sec_CrcExtVarTable[pos], dataPtr, dataLen))
	{
		Sec_SafeCheckFailHandler(pos);
#if  defined( CONFIG_PL_DEBUG )
		Sec_CrcExtVarTable[pos] = crc;
#endif
	}
}

/**
\n	Data Safety check variable
\n	This function checks if variable is coherent with its check value
\n	returning result of this check
\n
\param [in] 	chkVal		Expected check value
\param [in,out] dataPtr		Pointer to data variable
\param [in] 	dataLen		Lenght of data variable


\return bool_t
\retval	TRUE	if check succeeded
\retval	FALSE	if check failed


\par Architecture Requirements ID:
\li
\li


*/
bool_t Sec_CheckIfVal(uint16_t chkVal, void* dataPtr, uint16_t dataLen)
{
	return (chkVal == LIB_ClassBVar_CRC_calculate( dataLen, (uint8_t *)dataPtr , DEFAULT_SEED ));
}

/**
\n
\n
\n
\param [in] 	pos
\param [in,out] dataPtr
\param [in] 	dataLen


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/

void Sec_UpdateExtVar(SecExtVarID_t pos, void* dataPtr, uint16_t dataLen)
{
	uint16_t crc;
	PLT_ASSERT(pos<SEC_NUM_EXTVAR)

	crc = LIB_ClassBVar_CRC_calculate( dataLen, (uint8_t *)dataPtr , DEFAULT_SEED );
	Sec_CrcExtVarTable[pos] = crc;
}

void Sec_SafetyEnable(void)
{
	disableClassBVar = ~DISABLE_CLASSB_KEYWORD;
}

void Sec_SafetyDisable(void)
{
	disableClassBVar = DISABLE_CLASSB_KEYWORD;
}

bool_t Sec_SafetyIsDisable(void)
{
	return (disableClassBVar == DISABLE_CLASSB_KEYWORD);
}


#if defined(DATA_SAFETY_CFG_PROVA)
/**
 * Volendo ridure il numero di parametri da passare alle funzioni di check
 * si inizializza una struttura Sec_DataCheck in flash, con i dati da utilizzare per il check/update
 */
void Sec_CheckExtVar1(const Sec_DataCheck *checkPtr )
{
	uint16_t crc;
	/* bool_t retVal; */
	crc = LIB_ClassBVar_CRC_calculate( checkPtr->dataLen, (uint8_t *)checkPtr->dataPtr , DEFAULT_SEED );

	if( crc != Sec_CrcExtVarTable[checkPtr->id]){
		retVal=FALSE;
		Sec_CrcExtVarTableResult[checkPtr->id] = 1; /*DEBUG*/
	}else{
		/* retVal=TRUE; */
		Sec_CrcExtVarTableResult[checkPtr->id] = 0;
	}
	/* return retVal; */
}
bool_t Sec_CheckExtVar1_elem(const Sec_DataCheck *checkPtr, uint16_t pos )
{
	uint16_t crc;
	bool_t retVal;
	crc = LIB_ClassBVar_CRC_calculate( checkPtr->dataLen, checkPtr->dataPtr+(pos*checkPtr->dataLen) , DEFAULT_SEED );

	if( crc != Sec_CrcExtVarTable[checkPtr->id]){
		retVal=FALSE;
		Sec_CrcExtVarTableResult[checkPtr->id] = 1; /*DEBUG*/
	}else{
		retVal=TRUE;
		Sec_CrcExtVarTableResult[checkPtr->id] = 0;
	}
	return retVal;
}


void Sec_UpdateExtVar1(const Sec_DataCheck *checkPtr )
{
	uint16_t crc;
	crc = LIB_ClassBVar_CRC_calculate( checkPtr->dataLen, checkPtr->dataPtr , DEFAULT_SEED );
	Sec_CrcExtVarTable[checkPtr->id] = crc;
}

void Sec_UpdateExtVar1_elem(const Sec_DataCheck *checkPtr, uint16_t pos )
{
	uint16_t crc;
	crc = LIB_ClassBVar_CRC_calculate( checkPtr->dataLen, checkPtr->dataPtr+(pos*checkPtr->dataLen) , DEFAULT_SEED );
	Sec_CrcExtVarTable[checkPtr->id] = crc;
}
#endif /* DATA_SAFETY_CFG_PROVA */
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */

/* *********** */
/* End of file */
/* *********** */
