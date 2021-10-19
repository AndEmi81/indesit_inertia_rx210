/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
Interface (macros, prototypes and type definitions) for software timers and
counters that can be used to execute a user-defined action after a delay or for time measurement.
\n
\file		SwTimersCounters.h
\ingroup	PM_SwTimersCounters
\date		28-jun-2010
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.71
\n		18/10/2010 
\n		Roberto Fioravanti 
\n		changes
\n 		- SWTC_Retrigger  prototype made available

\n 		00.00.50 
\n		28/06/2010 
\n		Roberto Fioravanti 
\n		changes
\n 		- creation
\n
\n
\n etc.
*/

#ifndef SWTIMERSCOUNTERS_H_
#define SWTIMERSCOUNTERS_H_


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"


#if (SWTC_PRESCALERS_NUM>256)
#error "SWTC_PRESCALERS_NUM: too many prescalers."
#endif

#if (SWTC_COUNTERS_NUM>256)
#error "SWTC_COUNTERS_NUM: too many timercounters."
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Configuration */
#define SWTC_CFG_ENABLE_MSK			(0x80)
#define SWTC_CFG_APPL_RES_MSK		(0x40)  /**< reserved by application level */
#define SWTC_CFG_SIZE_MSK			(0x03)
#define SWTC_CFG_PS_MSK				(0x0F<<2)
#define SWTC_CFG_PS_MSK_LEFT_AL		(0x0F)

#define SWTC_CFG_ENABLE				(1<<7)
#define SWTC_CFG_UP 				(1<<6)
#define SWTC_CFG_SIZE_S				(1<<0)	/**< \hideinitializer Small timercounter   */
#define SWTC_CFG_SIZE_M				(1<<1)	/**< \hideinitializer Medium timercounter   */
#define SWTC_CFG_SIZE_L				(0)		/**< \hideinitializer Large timercounter   */
#define SWTC_CFG_PS(ps)				(((SWTC_CFG_PS_MSK_LEFT_AL)&(ps))<<2)
#define SWTC_CFG_PS_ASSERT(ps)		(!((ps) ^ ((ps) & SWTC_CFG_PS_MSK_LEFT_AL)))

/** 
\hideinitializer
Initializes the timercounter configuration variable.
Extended version (_EX) allows another parameter (ex) used only by aplication level.
\param [in] enable tiemrcounter state at initialization. Values: 
-	TRUE enabled
-	FALSE disabled.

\param [in] ex info reserved by application (only available on _EX-version).
Values:
- 0
- 1
\param [in] prescaler prescaler ID.
\param [in] size specifies the size of timercouter. 
Values:
-	\ref SWTC_CFG_SIZE_S
-	\ref SWTC_CFG_SIZE_M
-	\ref SWTC_CFG_SIZE_L

\par Example 
\code
SWTC_CounterConfigurationType CfgCounter_S1 = {SWTC_CFG_INIT(TRUE,1,SWTC_CFG_SIZE_S)};
\endcode
@{
*/
#define SWTC_CFG_INIT(enable, prescaler, size)	\
	((enable?SWTC_CFG_ENABLE:0)|(size)|SWTC_CFG_PS(prescaler))
#define SWTC_CFG_INIT_EX(enable, up, prescaler, size)	\
	((enable?SWTC_CFG_ENABLE:0)| (up?SWTC_CFG_UP:0) |(size)|SWTC_CFG_PS(prescaler))	
	
/* @} */
	
/* Table configuration */
#define DECLARE__SWTC_PRESCALERS_TABLE()	\
	const SWTC_PrescalerTableType SWTC_PrescalerTable[]

/**
\hideinitializer
Begin of prescaler table
*/
#define BEGIN__SWTC_PRESCALERS_TABLE(prescalers_num)	\
	const SWTC_PrescalerTableType SWTC_PrescalerTable[prescalers_num-1]	\
	={

/**
\hideinitializer
End of prescaler table
*/
#define END__SWTC_PRESCALERS_TABLE()	\
};

/* Configuration */
#define DECLARE__SWTC_CONFIG_TABLE()	\
	SWTC_CounterConfigurationType* const SWTC_CounterConfigurationTable[]

/**
\hideinitializer
Begin of config table
*/
#define BEGIN__SWTC_CONFIG_TABLE(counters_num)	\
	SWTC_CounterConfigurationType* const SWTC_CounterConfigurationTable[counters_num]	\
	={

/**
\hideinitializer
End of config table
*/
#define END__SWTC_CONFIG_TABLE()	\
};

/*Counters*/
#define DECLARE__SWTC_COUNTERS_TABLE()	\
	void* const SWTC_CountersTable[]

/**
\hideinitializer
Begin of counters table
*/
#define BEGIN__SWTC_COUNTERS_TABLE(counters_num)	\
	void* const SWTC_CountersTable[counters_num]	\
	={
/**
\hideinitializer
End of counters table
*/
#define END__SWTC_COUNTERS_TABLE()	\
};

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef  uint8_t SWTC_InternalCounterType ;
typedef	uint8_t SWTC_PrescalerTableType;
typedef	uint8_t SWTC_PrescalerType;
/**
 Timercounter identifier: it's the zero-based index in the timercounter table.
 \see BEGIN__SWTC_COUNTERS_TABLE. 
*/
typedef	uint8_t SWTC_CountersType;

typedef	int32_t SWTC_TimeType;


/*
 @{ 
*/
typedef int8_t SWTC_SCounterValue;	/**< Type for 'small' timercounter: it is based on 7-bit counter */
typedef int16_t SWTC_MCounterValue; /**< Type for 'medium' timercounter: it is based on 15-bit counter */
#if defined(SWTC_L_ENABLED)
typedef int32_t SWTC_LCounterValue;	/**< Type for 'large' timercounter: it is based on 31-bit counter */
#endif
/*
 @} 
*/

typedef union 
{
	SWTC_SCounterValue	counter;
	
}SWTC_SCounterVariableType;

typedef union 
{
	SWTC_MCounterValue	counter;
	
}SWTC_MCounterVariableType;

#if defined(SWTC_L_ENABLED)
typedef union 
{
	SWTC_LCounterValue	counter;
	
}SWTC_LCounterVariableType;
#endif /* defined(SWTC_L_ENABLED) */

typedef  uint8_t SWTC_CounterConfigurationType;
/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
void SWTC_InitManager(void);
void SWTC_InitTables(	const SWTC_PrescalerTableType* pPrescalers,
				SWTC_InternalCounterType* pIntCountersTable,
				uint8_t prescalersNum,
				SWTC_CounterConfigurationType* const* pCountersCfgTable,
				void* 						const* pCountersTable,
				uint8_t						CountersNum
				);
void SWTC_Manager(void);

void SWTC_Run(SWTC_CountersType counterID);
void SWTC_Stop(SWTC_CountersType counterID);

void SWTC_SetPrescaler(SWTC_CountersType counterID, SWTC_PrescalerType ps);

bool_t SWTC_IsExpired(SWTC_CountersType counterID);
bool_t SWTC_IsStopped(SWTC_CountersType counterID);
bool_t SWTC_Retrigger(SWTC_CountersType counterID);

void SWTC_SetSValue(SWTC_CountersType counterID, SWTC_SCounterValue value);
void SWTC_SetMValue(SWTC_CountersType counterID, SWTC_MCounterValue value);

#if defined(SWTC_L_ENABLED)
void SWTC_SetLValue(SWTC_CountersType counterID, SWTC_LCounterValue value);
#endif

void SWTC_SetSValueAndRun(SWTC_CountersType counterID, SWTC_SCounterValue value);
void SWTC_SetMValueAndRun(SWTC_CountersType counterID, SWTC_MCounterValue value);

#if defined(SWTC_L_ENABLED)
void SWTC_SetLValueAndRun(SWTC_CountersType counterID, SWTC_LCounterValue value);
#endif

SWTC_TimeType SWTC_GetRemainingTime(SWTC_CountersType counterID);
SWTC_TimeType SWTC_GetElapsedTime(SWTC_CountersType counterID);
#endif	/* SWTIMERSCOUNTERS_H_ */

/* *********** */
/* End of file */
/* *********** */

