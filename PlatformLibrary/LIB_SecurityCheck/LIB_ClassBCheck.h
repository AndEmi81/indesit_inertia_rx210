/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n		ClassB check for CPU, RAM, FLASH, CLOCK
\n		use the HAL  IEC60335 lib interface
\n
\file		LIB_ClassBCheck.h
\ingroup      LIB_ClassB
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



#ifndef __CLASSBCHECK_H_
#define __CLASSBCHECK_H_

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define CHECK_NOT_RUNNING   0
#define CHECK_RUNNING       1
#define CHECK_FAILED  	    2
#define CHECK_PASSED  	    3

#define FLASH_BLOCK_SETTING_FILE        255

#if defined(__ICCARM__)     /* EWARM */
#pragma section="CLASS_B_vars"
#pragma section="CLASS_B_check"

#define CLASS_B_VAR      _Pragma("location=\"CLASS_B_vars\"")
#define CLASS_B_VAR_COPY _Pragma("location=\"CLASS_B_check\"")
#endif
#if defined(__CWCC__)       /* Codewarrior */
#define CLASS_B_VAR     
#define CLASS_B_VAR_COPY 
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct {
  uint8_t* startAddr;
  uint8_t* endAddr;
} MemSegment;


typedef bool_t(*CLASSB_TEST_FUNC)(const uint32_t, const uint32_t, bool_t);

typedef struct {
	uint32_t 			startAddr;
	uint32_t			endAddr;
	CLASSB_TEST_FUNC	fpTest_Func;
}ClassB_RamTestDescriptorType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void InitCheck(void);
void InitMemoryCheck(void);

#ifdef CONFIG_LIB_CLASS_B_CPU
void RegCheck(void);
#endif /* CONFIG_LIB_CLASS_B_CPU*/

#ifdef CONFIG_LIB_CLASS_B_RAM
extern const ClassB_RamTestDescriptorType	RamAreaTestTable[NUM_RAM_CBOARD_TEST_BLOCKS];




void LIB_ClassB_RAM_BIST(void);

void LIB_ClassB_Keywords_BIST(void);

#endif /* CONFIG_LIB_CLASS_B_RAM*/

#ifdef CONFIG_LIB_CLASS_B_FLASH
extern const uint8_t Eeprom[];
extern const MemSegment AreaFlashCheckTable[NUM_FLASH_CBOARD_TEST_BLOCKS];
extern const uint16_t FlashCheckCode;

void CheckFlash_Init(bool_t isBIST);
uint8_t CheckFlash_Manager(uint16_t byte_to_check, bool_t skipSettingFile);
#endif /* CONFIG_LIB_CLASS_B_FLASH*/

#ifdef CONFIG_LIB_CLASS_B_CLOCK
void CheckClock_Init(void);
void CheckClock_Manager(uint16_t cmd);
#endif /* CONFIG_LIB_CLASS_B_CLOCK*/

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
void LIB_ClassB_WDTCheckInit(void);

#endif

#if defined(CONFIG_LIB_CLASS_B_ADC)
void ADC_CheckInit(void);
void ADC_Check(void);
#endif



#endif /* __CLASSBCHECK_H_ */

/* *********** */
/* End of file */
/* *********** */
