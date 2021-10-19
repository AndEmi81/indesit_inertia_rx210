/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n	Il file contiene le funzioni per la gestione delle variabili di classe B
\n
\file		LIB_ClassBVar.h
\ingroup    grpDataCare
\date		04/04/2012    
\version	01.00.00
\author		Marco Sperandini
\n
\n
\par	History:
\n
\n 		01.00.00
\n	    04/04/2012
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/



#ifndef __CLASSBVAR_H_
#define __CLASSBVAR_H_

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#include "DataSafety_cfg.h"

/* ******************* */
/* Function prototypes */
/* ******************* */
#if defined( CONFIG_PL_DEBUG )
void Sec_InitSafety(void);
#endif

#if defined (DATA_SAFETY_CFG_PROVA)
/** Cofigurazione di prova per ottimizzazione */
typedef struct{
	uint16_t id;
	uint16_t dataLen;
	uint8_t*  dataPtr;
} Sec_DataCheck;
void   Sec_CheckExtVar1(const Sec_DataCheck  *checkPtr );
void   Sec_UpdateExtVar1(const Sec_DataCheck *checkPtr );
void   Sec_CheckExtVar1_elem(const Sec_DataCheck *checkPtr, uint16_t pos );
void   Sec_UpdateExtVar1_elem(const Sec_DataCheck *checkPtr, uint16_t pos );

extern void disableInt( void );
#define SAFETY_REGION_START()	disableInt()
#define SAFETY_REGION_END()     disableInt()
#define SAFETY_DATA_CHECK( name )  Sec_CheckExtVar1( &SEC_CHECK_ ## name)
#define SAFETY_DATA_ELEM_CHECK( name, pos ) Sec_CheckExtVar1_elem( &SEC_CHECK_ ## name, pos)

#define SAFETY_DATA_UPDATE( name )      Sec_UpdateExtVar1( &SEC_CHECK_ ## name)
#define SAFETY_DATA_ELEM_UPDATE( name, pos ) Sec_UpdateExtVar1_elem( &SEC_CHECK_ ## name, pos)
#define SAFETY_DATA_CONFIG( name ) const Sec_DataCheck  SEC_CHECK_ ## name = { SEC_ID_ ## name, sizeof(name), (uint8_t*)&name }

#else
/**
 * Configurazione di default
 */

#if !defined(MODULE_BODY_CLASSBVAR)
extern const uint16_t Sec_CrcExtVarTable[SEC_NUM_EXTVAR];
#endif

void Sec_CheckExtVar(SecExtVarID_t pos, void* dataPtr, uint16_t dataLen);
bool_t Sec_CheckIfVal(uint16_t chkVal, void* dataPtr, uint16_t dataLen);
void Sec_UpdateExtVar( SecExtVarID_t pos, void* dataPtr, uint16_t dataLen);
void Sec_StartSafety(void);
void Sec_EndSafety(void);
void Sec_SafetyEnable(void);
void Sec_SafetyDisable(void);
bool_t Sec_SafetyIsDisable(void);

#define SAFETY_REGION_START()	Sec_StartSafety()
#define SAFETY_REGION_END()     Sec_EndSafety()

#define SAFETY_DATA_CHECK( name )  Sec_CheckExtVar( SEC_ID_ ## name, (void*)&name, (uint16_t)sizeof(name))
#define SAFETY_DATA_CHECKONLY( ptr, size, chk )  Sec_CheckIfVal( chk, (void*)ptr, (uint16_t)size)
#define SAFETY_DATA_ELEM_CHECK( name, pos ) Sec_CheckExtVar( (SecExtVarID_t)(SEC_ID_ ## name + pos), (void*)&name[pos], (uint16_t)sizeof(name[pos]))

#define SAFETY_DATA_UPDATE( name )      Sec_UpdateExtVar( SEC_ID_ ## name, (void*)&name, (uint16_t)sizeof(name))
#define SAFETY_DATA_ELEM_UPDATE( name, pos ) Sec_UpdateExtVar( (SecExtVarID_t)(SEC_ID_ ## name + pos), (void*)&name[pos], (uint16_t)sizeof(name[pos]))

#define	SAFETY_CHECK_ENABLE()			Sec_SafetyEnable()
#define	SAFETY_CHECK_DISABLE()			Sec_SafetyDisable()
#define	SAFETY_CHECK_ISDISABLED()		Sec_SafetyIsDisable()

#endif
#else  /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */

#define SAFETY_REGION_START()					{}
#define SAFETY_REGION_END()						{}

#define SAFETY_DATA_CHECK( name )				{}
#define SAFETY_DATA_CHECKONLY( name )			TRUE
#define SAFETY_DATA_ELEM_CHECK( name, pos )		{}

#define SAFETY_DATA_UPDATE( name )				{}
#define SAFETY_DATA_ELEM_UPDATE( name, pos )	{}

#define	SAFETY_CHECK_ENABLE()					{}
#define	SAFETY_CHECK_DISABLE()					{}
#define	SAFETY_CHECK_ISDISABLED()				TRUE

#define SAFETY_DATA_CONFIG( name )
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */


#endif /* __CLASSBVAR_H_ */

/* *********** */
/* End of file */
/* *********** */
