/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
 * \page pageTimers
 *	\ingroup	PM_SwTimersCounters
 */

/**
\n
\par		SW Timers Counters
\n			Implementation of software timers and counters that can be used to execute a 
user-defined action after a delay or for time measurement.
 
\n
\file		SwTimersCounters.c 
\ingroup	PM_SwTimersCounters
\date		28-jun-2010 
\version	01.00.00 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:
\n 
\n 		00.00.70
\n		5/10/2010 
\n		Roberto Fioravanti 
\n		changes 
\n 		- fixed return of SWTC_IsStopped
\n 		00.00.50 
\n		28/06/2010 
\n		Roberto Fioravanti 
\n		changes 
\n 		- creation
\n
\n
\n etc.
*/


#define MODULE_BODY_SWTIMERS
/* ******** */
/* Includes */
/* ******** */
#include "SwTimersCounters.h"
#include "LIB_Error.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct SWTC_DataType_s{
	SWTC_InternalCounterType*	pInternalCountersTable;
	const SWTC_PrescalerTableType*	pPrescalersTable;
	uint8_t						PrescalersNum;
	SWTC_CounterConfigurationType* const* pCountersCfgTable;
	void* 						const* pCountersTable;
	uint8_t						CountersNum;
}
SWTC_DataType;
/* ********* */
/* Variables */
/* ********* */

/*
extern const SWTC_PrescalerTableType SWTC_PrescalerTable[SWTC_PRESCALERS_NUM];
extern SWTC_CounterConfigurationType* const SWTC_CounterConfigurationTable[SWTC_COUNTERS_NUM];
extern void* const SWTC_CountersTable[SWTC_COUNTERS_NUM];
*/
/* Global */

/* Static */
/*static SWTC_InternalCounterType SWTC_InternalCounter[SWTC_PRESCALERS_NUM];*/
static SWTC_DataType SWTC_Data;
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void SWTC_PrescalerManager ( SWTC_PrescalerType ps );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**

Timercounters manager initialization method.


\return 	none
 
\author 	Roberto Fioravanti
\date		28-06-2010
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void SWTC_InitManager(void)
{
	SWTC_Data.PrescalersNum=0;
	SWTC_Data.pPrescalersTable=NULL;
	SWTC_Data.pInternalCountersTable=NULL;

	SWTC_Data.CountersNum=0;
	SWTC_Data.pCountersCfgTable=NULL;
	SWTC_Data.pCountersTable=NULL;
}


void SWTC_InitTables(	const SWTC_PrescalerTableType* pPrescalers,
				SWTC_InternalCounterType* pIntCountersTable,
				uint8_t prescalersNum,
				SWTC_CounterConfigurationType* const* pCountersCfgTable,
				void* 						const* pCountersTable,
				uint8_t						CountersNum
				)
{
	uint8_t i = 0;
	SWTC_Data.PrescalersNum=prescalersNum;
	SWTC_Data.pPrescalersTable=pPrescalers;
	SWTC_Data.pInternalCountersTable=pIntCountersTable;

	for (i = 0; i < SWTC_Data.PrescalersNum; i++)
	{
		SWTC_Data.pInternalCountersTable[i]=0;
	}

	SWTC_Data.CountersNum=CountersNum;
	SWTC_Data.pCountersCfgTable=pCountersCfgTable;
	SWTC_Data.pCountersTable=pCountersTable;
}

/**
Timerscounters manager to be called periodically.
The period determines the foundamental time base.
\return 	none
 
\author 	Roberto Fioravanti
\date		28-06-2010
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void SWTC_Manager(void)
{
	SWTC_PrescalerType i;
	
	SWTC_Data.pInternalCountersTable[0]++;
	SWTC_PrescalerManager (0);
	
	for (i = 0; i < SWTC_Data.PrescalersNum; ++i)
	{
		if(SWTC_Data.pInternalCountersTable[i] >= SWTC_Data.pPrescalersTable[i])
		{
			SWTC_PrescalerManager ((SWTC_PrescalerType)(i+1));
			
			SWTC_Data.pInternalCountersTable[i]=0;
			if(i < (SWTC_Data.PrescalersNum-1))
			{
				SWTC_Data.pInternalCountersTable[i+1]++;
			}
		}
	}
}


/**

Sets the a new value for the 'small' (...SetSValue),
 'medium' (...SetMValue) or 'large' (...SetLValue) timercounter.
TimerCounter state (enable or not) remains unchanged, that means timercounter continues counting it is running.

\param  [in]		counterID timercounter identifier (zero-based index of the timercounter table)
\param  [in]		value new value applied to the timercounter.The range depends on size 
of the counter timercounter:
1<= value <= 2^(n)-1, where n is the number of the bit of the counter. See \ref SWTC_SCounterValue, \ref SWTC_MCounterValue, \ref SWTC_LCounterValue	


\return 	void

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

   
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void SWTC_SetSValue(SWTC_CountersType counterID, SWTC_SCounterValue value)
{
	uint8_t enable;
	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_SCounterVariableType* pCnt=0;
	
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	enable = (uint8_t) (*pcfg & SWTC_CFG_ENABLE_MSK);
	*pcfg &= (SWTC_CounterConfigurationType) (~SWTC_CFG_ENABLE_MSK);
	
	pCnt = (SWTC_SCounterVariableType*)(SWTC_Data.pCountersTable[counterID]);
	(pCnt)->counter = (SWTC_SCounterValue) (value - 1);
	
	*pcfg |= enable;

}


void SWTC_SetMValue(SWTC_CountersType counterID, SWTC_MCounterValue value)
{
	uint8_t enable;
	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_MCounterVariableType* pCnt=0;

	
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	enable = (uint8_t) (*pcfg & SWTC_CFG_ENABLE_MSK);
	*pcfg &= (SWTC_CounterConfigurationType) (~SWTC_CFG_ENABLE_MSK);
	
	pCnt = (SWTC_MCounterVariableType*)(SWTC_Data.pCountersTable[counterID]);
	(pCnt)->counter = (SWTC_MCounterValue) (value - 1);

	*pcfg |= enable;
}
#if defined(SWTC_L_ENABLED)
/**
\attention Available only if SWTC_L_ENABLED is defined in header configuration file
*/
void SWTC_SetLValue(SWTC_CountersType counterID, SWTC_LCounterValue value)
{
	uint8_t enable;
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_COUNTERS_NUM), PLT_ERROR_SWTC_INVALID_COUNTER);
	
	enable = (*SWTC_CounterConfigurationTable[counterID] & SWTC_CFG_ENABLE_MSK);
	*SWTC_CounterConfigurationTable[counterID] &= ~(SWTC_CFG_ENABLE_MSK);
	
	((SWTC_LCounterVariableType*)SWTC_CountersTable[counterID])->counter = value - 1;
	
	enable= *SWTC_CounterConfigurationTable[counterID] |= enable;

}
#endif



/**
Sets the a new value for the prescaler of the timercounter.
TimerCounter state (enable or not) remains unchanged.

\param  [in]		counterID timercounter identifier
\param  [in]		ps prescaler identifier.	

\return 	void

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

   
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void SWTC_SetPrescaler(SWTC_CountersType counterID, SWTC_PrescalerType ps)
{
	uint8_t enable;
	SWTC_CounterConfigurationType* pcfg=0;

	PLT_ERROR_FATAL_IF(!SWTC_CFG_PS_ASSERT(ps), PLT_ERROR_SWTC_INVALID_PS);

	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	enable = (uint8_t) (*pcfg & SWTC_CFG_ENABLE_MSK);
	*pcfg &= (SWTC_CounterConfigurationType) (~SWTC_CFG_ENABLE_MSK);

	*pcfg &= ~(SWTC_CFG_PS_MSK);
	*pcfg |= SWTC_CFG_PS(ps);

	*pcfg |= enable;
}


/**
Starts timercounter.

\param  [in]		counterID timercounter identifier
	
\return 	void

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
\par \b 1:
Use this method to restart a timer counter which was stopped by calling SWTC_Stop().
In this case timercounter will continue with the remaining time value preserved by stopping the timer.

\par \b 2:
this method has any effect on:
- 	running timercounters
-	not running timercounters because expired. To restart those timercounters use SWTC_SetXValue() method. 

    
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void  SWTC_Run(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	pcfg=SWTC_Data.pCountersCfgTable[counterID];

	*pcfg |= SWTC_CFG_ENABLE_MSK;
}

/**
Stops timercounter.

\param  [in]		counterID timercounter identifier
	
\return 	void


\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
\par \b 1:
The current value of the timercounter is kept until SWTC_RunTimerCounter() or SWTC_SetSValue(), 
SWTC_SetMValue() or SWTC_SetLValue() or SWTC_SetSValueAndRun(), SWTC_SetMValueAndRun(), SWTC_SetLValueAndRun() is called.
    
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void SWTC_Stop(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	*pcfg &= ~(SWTC_CFG_ENABLE_MSK);
}


/**
   Sets a new value and then starts timercounter.
   
   \param  [in]		counterID timercounter identifier
\param  [in]		value new value applied to the timercounter.The range depends on size 
of the counter timercounter:
1<= value <= 2^(n)-1, where n is the number of the bit of the counter. See \ref SWTC_SCounterValue, \ref SWTC_MCounterValue, \ref SWTC_LCounterValue

\return 	void

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
\par \b 1:
Use this method to restart a timer counter which was stopped by calling SWTC_Stop().
In this case timercounter will continue with the remaining time value preserved by stopping the timer.

\par \b 2:
this method has any effect on:
- 	running timercounters
-	not running timercounters because expired. To restart those timercounters use SWTC_SetXValue(). 

    
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
    

*/

/**
*/
void SWTC_SetSValueAndRun(SWTC_CountersType counterID, SWTC_SCounterValue value)
{
	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_SCounterVariableType* pCnt=0;
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	pcfg=SWTC_Data.pCountersCfgTable[counterID];

	*pcfg &= ~(SWTC_CFG_ENABLE_MSK);

	pCnt = (SWTC_SCounterVariableType*)(SWTC_Data.pCountersTable[counterID]);
	(pCnt)->counter = (SWTC_SCounterValue) (value - 1);

	*pcfg |= (SWTC_CFG_ENABLE_MSK);
}

/**

*/
void SWTC_SetMValueAndRun(SWTC_CountersType counterID, SWTC_MCounterValue value)
{
	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_MCounterVariableType* pCnt=0;

	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	
	pcfg=SWTC_Data.pCountersCfgTable[counterID];

	*pcfg &= ~(SWTC_CFG_ENABLE_MSK);

	pCnt = (SWTC_MCounterVariableType*)(SWTC_Data.pCountersTable[counterID]);
	(pCnt)->counter = (SWTC_MCounterValue) (value - 1);

	*pcfg |= (SWTC_CFG_ENABLE_MSK);
}

#if defined(SWTC_L_ENABLED)
/**
\attention Available only if SWTC_L_ENABLED is defined in header configuration file
*/
void SWTC_SetLValueAndRun(SWTC_CountersType counterID, SWTC_LCounterValue value)
{
	PL_ERROR_FATAL(counterID >= (SWTC_COUNTERS_NUM), PLT_ERROR_SWTC_INVALID_COUNTER);

	*SWTC_CounterConfigurationTable[counterID] &= ~(SWTC_CFG_ENABLE_MSK);
	
	((SWTC_LCounterVariableType*)SWTC_CountersTable[counterID])->counter = value - 1;
	
	*SWTC_CounterConfigurationTable[counterID] |= (SWTC_CFG_ENABLE_MSK);
}
#endif




/**
Returns if the timercounter is expired.

\param  [in]		counterID timercounter identifier
	
\return 	bool_t

\retval		TRUE (returned also for not invalid timercouner IDs)
\retval		FALSE	not expired.		 	 



\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
A timercounter not expired can be running or not.
      
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t SWTC_IsExpired(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	void* pCnt=0;

	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	pCnt = (void*)(SWTC_Data.pCountersTable[counterID]);
	
	switch(*pcfg & SWTC_CFG_SIZE_MSK)
	{
	#if defined(SWTC_L_ENABLED)	
		case SWTC_CFG_SIZE_L:
			return (bool_t)(((SWTC_LCounterVariableType*)pCnt)->counter < 0);
			break;
	#endif
		case SWTC_CFG_SIZE_S:
			return (bool_t)(((SWTC_SCounterVariableType*)pCnt)->counter < 0);
			/*lint -e{527} Warning 527:*/
			break;

		case SWTC_CFG_SIZE_M:
			return (bool_t)(((SWTC_MCounterVariableType*)pCnt)->counter < 0);
			/*lint -e{527} Warning 527:*/
			break;

	}
	
	return FALSE;	
}


/**
Returns if the timercounter is stopped or enabled.

\param  [in]		counterID timercounter identifier
	
\return 	bool_t

\retval		TRUE	stopped
\retval		FALSE	enabled (returned also for not invalid timercouner IDs).

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
A timercounter not expired can be running or not.
      
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t SWTC_IsStopped(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	
	return (bool_t)(!(*pcfg & SWTC_CFG_ENABLE_MSK));
}



/**
Retrigger an expired timercounter.
timercounter starts counting form it maximum value.

\param  [in]		counterID timercounter identifier
	
\return 	bool_t

\retval		TRUE (returned also for not invalid timercouner IDs)
\retval		FALSE	not expired.		 	 



\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

\attention
This method has no effect on not expired timercounters. 
      
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
			
			
*/
bool_t SWTC_Retrigger(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	bool_t pending=FALSE;
	void* pCnt=0;
	
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);

	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	pCnt = (void*)(SWTC_Data.pCountersTable[counterID]);

	switch(*pcfg & SWTC_CFG_SIZE_MSK)
	{
#if defined(SWTC_L_ENABLED)	
	case SWTC_CFG_SIZE_L:
		pending = (bool_t)(((SWTC_LCounterVariableType*)SWTC_CountersTable[counterID])->counter < 0);
		if(pending)
		((SWTC_LCounterVariableType*)pCnt)->counter += 0x7FFFFFFF;
		break;
#endif
	case SWTC_CFG_SIZE_S:		
		pending = (bool_t)(((SWTC_SCounterVariableType*)pCnt)->counter < 0);
		if(pending)
		{
			((SWTC_SCounterVariableType*)pCnt)->counter += 0x7F;
		}
		else
		{
		}
		break;
	case SWTC_CFG_SIZE_M:
		pending = (bool_t)(((SWTC_MCounterVariableType*)pCnt)->counter < 0);
		if(pending)
		{
			((SWTC_MCounterVariableType*)pCnt)->counter += 0x7FFF;
		}
		else
		{
		}
		break;	
	}

	return pending;
}

/**
Returns the remaining time before expire.
Value is in prescaler unit.

\param  [in]		counterID timercounter identifier
	
\return 	SWTC_TimeType

\retval		value ( 0 is returned if timercouner ID is invalid)

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

      
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
SWTC_TimeType SWTC_GetRemainingTime(SWTC_CountersType counterID)
{
	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_TimeType time=0;
	void* pCnt=0;
	
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	pCnt = (void*)(SWTC_Data.pCountersTable[counterID]);

	switch(*pcfg & SWTC_CFG_SIZE_MSK)
	{
#if defined(SWTC_L_ENABLED)	
	case SWTC_CFG_SIZE_L:
		time = ((SWTC_LCounterVariableType*)pCnt)->counter + 1;
		break;
#endif
	case SWTC_CFG_SIZE_S:
		time = ((SWTC_SCounterVariableType*)pCnt)->counter + 1;
		break;
		
	case SWTC_CFG_SIZE_M:
		time = ((SWTC_MCounterVariableType*)pCnt)->counter + 1;
		break;	
	}
	
	return time;
}

/**
Returns the elapsed time is in prescaler unit.
The returned value assumes that the TimerCounter was initialized at its maximum value.

General formula to get the elapsed time is:
InitVal - TimeCounterMaxVal + SWTC_GetElapsedTime()

where TimeCounterMaxVal depends on the size of the TimerCounter.


\param  [in]		counterID timercounter identifier
	
\return 	SWTC_TimeType

\retval		value ( 0 is returned if timercouner ID is invalid)

\author 	Roberto Fioravanti 
\date		Jun 26, 2010 
\version	1.0.0

      
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
SWTC_TimeType SWTC_GetElapsedTime(SWTC_CountersType counterID)
{

	SWTC_CounterConfigurationType* pcfg=0;
	SWTC_TimeType time=0;
	void* pCnt=0;
	
	PLT_ERROR_FATAL_IF(counterID >= (SWTC_Data.CountersNum), PLT_ERROR_SWTC_INVALID_COUNTER);
	
	pcfg=SWTC_Data.pCountersCfgTable[counterID];
	pCnt = (void*)(SWTC_Data.pCountersTable[counterID]);

	switch(*pcfg & SWTC_CFG_SIZE_MSK)
	{
#if defined(SWTC_L_ENABLED)	
	case SWTC_CFG_SIZE_L:
		time = 0x7FFFFFFF -1 - ((SWTC_LCounterVariableType*)pCnt)->counter;
		break;
#endif
	case SWTC_CFG_SIZE_S:
		time = (0x7F -1) - ((SWTC_SCounterVariableType*)pCnt)->counter;
		break;
		
	case SWTC_CFG_SIZE_M:
		time = (0x7FFF -1) - ((SWTC_MCounterVariableType*)pCnt)->counter;
		break;	
	}
	
	return time;
}


/* Local Function */
static void SWTC_PrescalerManager ( SWTC_PrescalerType ps )
{
	SWTC_PrescalerType i;
	SWTC_CounterConfigurationType* pcfg=0;
	void* pCnt=0;

	for ( i = 0; i < (SWTC_Data.CountersNum); i++ )
	{
		pcfg=SWTC_Data.pCountersCfgTable[i];
		pCnt = (void*)(SWTC_Data.pCountersTable[i]);

		if ( ( *pcfg & (SWTC_CFG_ENABLE_MSK) )
		&& ( (*pcfg & SWTC_CFG_PS_MSK) == SWTC_CFG_PS(ps) )
			)
		{
				/* Down Counter */
			switch(*pcfg & SWTC_CFG_SIZE_MSK)
			{
#if defined(SWTC_L_ENABLED)        	
			case SWTC_CFG_SIZE_L:
					if(((SWTC_LCounterVariableType*)pCnt)->counter >= 0)
					{
						((SWTC_LCounterVariableType*)pCnt)->counter --;
					}
					break;
#endif
			case SWTC_CFG_SIZE_S:
				if(((SWTC_SCounterVariableType*)pCnt)->counter >= 0)
				{
					((SWTC_SCounterVariableType*)pCnt)->counter --;
				}
				break;
			case SWTC_CFG_SIZE_M:
				if(((SWTC_MCounterVariableType*)pCnt)->counter >= 0)
					{
						((SWTC_MCounterVariableType*)pCnt)->counter --;
					}
				break;
	
			}        	
		}
	}
}

/* *********** */
/* End of file */
/* *********** */

