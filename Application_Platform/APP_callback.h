/**
Header file del modulo Application Platform Callback

\file		APP_callback.h
\ingroup	APP_callback
\date		17/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_CALLBACK_H
#define __APP_CALLBACK_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void ITP_preStepInit(void);
void ITP_newStepInit(void);

#if (ITP_LOADS_MGMT_ENABLE == 1)

#if (MULTI_COMPARTMENT_APPL == 1)	
    void ITP_loadsMgmt(uint8_t compartment);
#else
    void ITP_loadsMgmt(void);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

#endif  /* (ITP_LOADS_MGMT_ENABLE == 1) */

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
	void ITP_phaseTrackerUI(void);
#endif	/*#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)*/

#if	(SBY_STANDBY_ENABLE == 1)
	uint8_t STB_getStatusAppliance(void);
#endif


uint8_t SelectOperands(const Condition_Struct* pCondition, int32_t* pData,  int32_t* pOpnd);
uint8_t SelectOperandsAPP2_0(const Condition_Struct* pCondition, LIB_Fixed_ValueType* pData);


#endif /* __APP_CALLBACK_H */

/* *********** */
/* End of file */
/* *********** */
