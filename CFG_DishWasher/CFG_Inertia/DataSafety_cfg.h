
#ifndef _DATASAFETY_CFG_H_
#define _DATASAFETY_CFG_H_
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#include "FeedbackConfig.h"
#include "Main.h"
#include "AdcConfig.h"

typedef enum { 
	SEC_ID_Feedback,
	SEC_ID_FeedbackStatus = SEC_ID_Feedback +N_FEEDBACK,
	SEC_ID_FeedbackDescriptor = SEC_ID_FeedbackStatus +N_FEEDBACK,
	SEC_ID_FeedbackInputDisable =SEC_ID_FeedbackDescriptor+N_FEEDBACK,
	SEC_ID_PM_DoorLock_Config_Var,
	SEC_ID_PM_DoorLock_Control_Var,
	SEC_ID_IsZC_Good,
	SEC_ID_LIB_ZC_Control_Var,
	SEC_ID_LIB_ZC_Good_Control_Var,
	SEC_ID_TMR_IcuDefinition, 													/* is arry of NUM_HW_CNTR_CH*/
	SEC_ID_AnalogInputDisable         = SEC_ID_TMR_IcuDefinition + NUM_HW_CNTR_CH,

	SEC_ID_pHwCntrCfgTable,    /* is array of NUM_HW_CNTR */
	SEC_ID_TMR_PwdDefinition = SEC_ID_pHwCntrCfgTable   + NUM_HW_CNTR, /* is array of   NUM_HW_CNTR_CH */
	SEC_ID_PWD_CtrlVar 		 = SEC_ID_TMR_PwdDefinition + NUM_HW_CNTR_CH ,    /* is array of  NUM_HW_CNTR_CH */
	SEC_ID_RefInput          = SEC_ID_PWD_CtrlVar       + NUM_HW_CNTR_CH,
	SEC_ID_ADC_Convertions,  	    											/* is array of ADC_NUM_CHANNELLS_USED */
	SEC_ID_AnalogInputParam             = SEC_ID_ADC_Convertions        + ADC_NUM_CHANNELLS_USED, 	/* is array of ADC_NUM_CHANNELLS_USED */
	SEC_ID_AnalogInput                  = SEC_ID_AnalogInputParam       + ADC_NUM_CHANNELLS_USED,		/* is array of ADC_NUM_CHANNELLS_USED */
	SEC_ID_MainFeedbacksDelayOnZC       = SEC_ID_AnalogInput            + ADC_NUM_CHANNELLS_USED ,/* is array of MAIN_FEEDBACK_BOTH */
	SEC_ID_MainFeedbacksDelayOnZC_Score = SEC_ID_MainFeedbacksDelayOnZC + MAIN_FEEDBACK_BOTH, /* is array of MAIN_FEEDBACK_BOTH */
	SEC_ID_MainFeedbacksPeriod          = SEC_ID_MainFeedbacksDelayOnZC_Score    + MAIN_FEEDBACK_BOTH, /* is array of MAIN_FEEDBACK_BOTH */
	SEC_ID_MainFeedbacksPeriod_Score    = SEC_ID_MainFeedbacksPeriod       + MAIN_FEEDBACK_BOTH, /* is array of MAIN_FEEDBACK_BOTH */
	SEC_ID_DigInputs                    = SEC_ID_MainFeedbacksPeriod_Score + MAIN_FEEDBACK_BOTH,
	SEC_ID_DigInputsDisable,
	SEC_ID_LiveDelayOnZC,
	SEC_ID_MainLiveDelayOnZC_Score,
	SEC_ID_flashTestStatus,
	SEC_ID_ClassB_CheckRamTestState,
	SEC_ID_TachCtrlVar,
	SEC_ID_PM_SFT_WM_Cfg,
	SEC_ID_PM_SFT_WM_Data,
	SEC_ID_DL_faultStatus,
	SEC_ID_MotorData,
	SEC_ID_LinkedLoads_NextIndex,
	SEC_ID_LinkedLoads,				/* is array of CONFIG_LINKED_LOADS_NUM */
	SEC_ID_IOM_LoadsReq_Status = SEC_ID_LinkedLoads + CONFIG_LINKED_LOADS_NUM,
	SEC_ID_IOM_LoadsReq,
#if defined(CONFIG_NUM_EXTDLOADS)
	SEC_ID_IOM_extdLoads_Pos,
	SEC_ID_IOM_extdLoadsReq,
#endif
#if defined(CONFIG_NUM_SR_LOADS)
	SEC_ID_IOM_SwitchRelay_Params,				/* is array of CONFIG_LINKED_LOADS_NUM */
	SEC_ID_IOM_LoadsID_SRTypeLoads_Pos = SEC_ID_IOM_SwitchRelay_Params + CONFIG_NUM_SR_LOADS,
#endif
#if defined(CONFIG_NUM_MR_LOADS)
	SEC_ID_IOM_MRelay_Params,				/* is array of CONFIG_LINKED_LOADS_NUM */
	SEC_ID_IOM_LoadsID_MRTypeLoads_Pos = SEC_ID_IOM_MRelay_Params + CONFIG_NUM_MR_LOADS,
#endif
	SEC_ID_ResetCtrl,
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
	SEC_ID_PM_FrequencyFeedbacks_Interface,
#endif
	SEC_ID_CheckTasks,															/* is array of NUM_TASKS_TO_MONITOR */
	SEC_ID_LIB_TaskMonitorError = SEC_ID_CheckTasks + NUM_TASKS_TO_MONITOR,
	SEC_ID_DMDLoads,
	SEC_ID_DMDVar,			    /* is array of CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS */
	SEC_ID_PLT_Consistency = SEC_ID_DMDVar + CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS,	/* is array of N_MAXLOADS */
	SEC_ID_LoadsStatus     = SEC_ID_PLT_Consistency + N_MAXLOADS,	 					/* is array of N_MAXLOADS */
	SEC_ID_ExtdLoadsStatus  = SEC_ID_LoadsStatus + N_MAXLOADS,
	SEC_ID_SafetySupervisorStatus = SEC_ID_ExtdLoadsStatus + CONFIG_NUM_EXTDLOADS,
	SEC_ID_washNTCProbe,
	SEC_ID_persWashNTCProbe,
	SEC_ID_sftDrainPumpStatus,
	SEC_ID_sftDrainPumpVirtualTemp,
	SEC_ID_sftWRstatus,
	SEC_ID_DF_faultStatus,
	SEC_ID_tmrDrainPump,
#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK)
	SEC_ID_IRQMonitorStatus,
#endif
	/* Pressure Switch Module's Variables */
/*	SEC_ID_prsDoorInfos,
	SEC_ID_prsEVWashInfos,
	SEC_ID_prsEVPrewashInfos,
	SEC_ID_prsEVHotwashInfos,
	SEC_ID_prsDrainPumpInfos,*/
#if defined (CONFIG_APPLIANCE_WD)
	SEC_ID_dryNTCProbe,
	SEC_ID_persDryNTCProbe,
	SEC_ID_sftDRstatus,
#endif /* CONFIG_APPLIANCE_WD */
#if defined (CONFIG_PM_MOTORGATE)
	SEC_ID_PM_SFT_WM_Motor_SlowDown,
	SEC_ID_PM_SFT_WM_Motor_SoftStart,
	SEC_ID_PM_SFT_WM_Motor_Consistency,
	SEC_ID_PhaseAngle,
	SEC_ID_PM_UM_Sft,
#endif

	SEC_NUM_EXTVAR

} SecExtVarID_t;
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */
#endif
