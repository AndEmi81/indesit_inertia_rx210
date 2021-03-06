/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCTPM_drv.h
\n			Header file of OCTPM_drv.c
\n
\file		OCTPM_drv.h
\n			functions's prototypes, defines, typedef ... .
\n
\file		OCTPM_drv.h
\ingroup	OCTPM
\date		Oct 22, 2010
\version	01.00.00
\author		Jaroslav Musil,Stefano Ughi
\n
\n
\par	History:
\n
\n 		01.00.01
\n		Dec 15, 2010
\n		Stefano Ughi
\n      - functions review
\n 		01.00.00
\n		Oct 22, 2010
\n		Jaroslav Musil
\n      - creation on base of Stefano Ughi's specifications
\n
*/

#ifndef __OCTPM_DRV_H
#define __OCTPM_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL
#include "IO_driver.h"
#include "config.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Hardware Counters config. type */
typedef struct HwCntrOptionsTag
{
	/** if '1' do not set registers */
	bool8_t skipHWcntrConf:1;
	/** free */
	bool8_t free:7;
} HwCntrOptionsType;

/**
Hardware Counter
*/
typedef struct IO_HwCntrCfgTag
{
	/** Timer Id */
	IO_ChannelType hwCntrlId;
	/** Clearing Source in order to obtain the right period
	    0xFF Internal Resorce otherwise Output Compare Channel */
	IO_ChannelType clearingSourceId;
	/** Prescaler Value */
	PRS_ValueType prescaler;
	/** Timer Clock Source */
	uint8_t clockSource;
	/** up, down, up & down ....*/
	uint8_t countingMode;
	/** Timer Resolution [nSec] */
	IO_TickResType tickResolution;
	/** Period value for the HW counter [tick] */
	Period_ValueType period;
	/** Options */
	HwCntrOptionsType options;

} IO_HwCntrCfgType;
/**
Input Capture
*/
typedef struct IO_ICU_CfgTag
{
	/** Input Capture Channel Id */
	IO_ChannelType ICU_Id;
	/** invertion */
	bool8_t inverted;
} IO_ICU_CfgType;

/**
Input Capture Current Setup
*/
typedef enum
{
	ICU_EDGE_MODE_INVALID=0,
	ICU_EDGE_MODE_RISING,
	ICU_EDGE_MODE_FALLING,
	ICU_EDGE_MODE_BOTH
} ICU_EdgeModeType;

/**
Compare Match/Output Compare configuration structure
*/
typedef struct IO_OCU_CfgTag
{
	/** Compare Match/Output Compare function timer channel ID */
	IO_ChannelType OCU_Id;
	/** invertion */
	bool8_t inverted;
} IO_OCU_CfgType;
/**
Pulse Width Modulation
*/
typedef struct IO_PWM_CfgTag
{
	/** Pulse Width Modulation Channel Id */
	IO_ChannelType PWM_Id;
	/** default period for PWM */
	IO_TimingValueType dfltPeriod;
	/** invertion */
	bool8_t inverted;
} IO_PWM_CfgType;
/**
Pulse Width Demodulation
*/
typedef struct IO_PWD_CfgTag
{
	/** Pulse Width Demodulation Channel Id */
	IO_ChannelType	PWD_Id;
	/** invertion */
	bool8_t 		inverted: 1;
	bool8_t  		in_use: 1;
	bool8_t			noTon: 1;	/* if set Ton is not measured */
} IO_PWD_CfgType;
/**
Pulse Width Demodulation
*/
typedef struct IO_PWD_MeasureTag
{
	/** period in nSec */
	IO_TimingValueType period;
	/** tOn in nSec */
	IO_TimingValueType tOn;
} IO_PWD_MeasureType;
/**
Pulse Width Demodulation
*/
typedef struct IO_PWD_RawMeasureTag
{
	/** period in ticks */
	uint32_t period;
	/** tOn in ticks */
	uint32_t tOn;
} IO_PWD_RawMeasureType;

/* this defines a relationship between the channel address */
/* and the hardware timer. Note that the address type is the  */
/* match register address, not the control register address */
/* see comments in hal.h */
typedef struct TMR_ChMapType_tag
{
    TMR_CHAddressType  Channel; 
    TMR_CNTAddressType hwCntrlId;
} TMR_ChMapType;


/* TMR_CntTrigVal */
typedef enum{
	HW_CNTR_TRIG_ZC = 0,
	NUM_HW_CNTR_COLLECTIONS
}OCTPM_TriggerIndexType;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */

void OCTPM_InitSync(void);
/************** Timer counter - begin ************/
IO_ErrorType			OCHWCNTR_InitSync(const IO_HwCntrCfgType *pHwCntrCfg);
void					OCHWCNTR_DeInitSync(IO_ChannelType tmrId);
IO_TimingValueType		OCHWCNTR_GetSync(IO_ChannelType tmrId);
IO_HwTimerType			OCHWCNTR_GetTicksSync(IO_ChannelType tmrId);
IO_ErrorType			OCHWCNTR_SetSync(IO_ChannelType tmrId, IO_TimingValueType timerValue);
IO_ErrorType			OCHWCNTR_Enable(IO_ChannelType tmrId, bool_t enable);
void 					OCHWCNTR_EnableMulti(uint32_t uBmp);
IO_ErrorType			OCHWCNTR_EnableISR(IO_ChannelType tmrId, bool_t enableISR, Comm_Notification callback);
IO_TickResType			OCHWCNTR_GetTickResolution(IO_ChannelType tmrId);
IO_TickResType 			OCHWCNTR_GetResolutionByTimerChannel(IO_ChannelType ch);
Period_ValueType		OCHWCNTR_GetTimerModulus(IO_ChannelType tmrId);
Period_ValueType		OCHWCNTR_GetDelta(IO_ChannelType tmrId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR);
IO_TimingValueType		OCHWCNTR_GetPeriod(IO_ChannelType tmrId);
IO_TimingValueType		OCHWCNTR_GetMaxPeriod(IO_ChannelType tmrId);
void					OCHWCNTR_Trigger(OCTPM_TriggerIndexType triggID );


/* Call Inside Interrupts */
void                 HWCNTR_ISR(IO_ChannelType channel);
/************** Timer counter - end **************/

/************** Timer input capture - begin **************/
IO_ErrorType         OCICU_InitSync(const IO_ICU_CfgType *pICU_Cfg);
IO_ErrorType         OCICU_DeInitSync(IO_ChannelType channel);
IO_TimingValueType   OCICU_GetSync(IO_ChannelType channel);
IO_HwTimerType       OCICU_GetTickSync(IO_ChannelType channel); 

IO_ErrorType         OCICU_SetModeSync(IO_ChannelType channel, IO_ModeType mode);
IO_ErrorType         OCICU_GetModeSync(IO_ChannelType channel, ICU_EdgeModeType *pMode);
IO_ErrorType         OCICU_ToggleModeSync(IO_ChannelType channel);
IO_ErrorType         OCICU_EnableISR(IO_ChannelType channel, bool_t enableISR, Comm_Notification callback);
/************** Timer input capture - end ****************/

/************** Timer output compare - begin **************/
IO_ErrorType		OCOCU_InitSync(IO_OCU_CfgType *pOCU_Cfg);
IO_ErrorType		OCOCU_DeInitSync(IO_ChannelType channel);
IO_TimingValueType	OCOCU_GetSync(IO_ChannelType channel);
IO_HwTimerType		OCOCU_GetSyncTickResolution(IO_ChannelType channel);
void				OCOCU_SetThreshold1Sync(IO_ChannelType channel, IO_TimingValueType threshold1);
void				OCOCU_SetThreshold1TickResolutionSync(IO_ChannelType channel, IO_HwTimerType threshold1);
void				OCOCU_SetThreshold2Sync(IO_ChannelType channel, IO_TimingValueType threshold2);
void				OCOCU_SetThreshold2TickResolutionSync(IO_ChannelType channel, IO_HwTimerType threshold2);
IO_ErrorType		OCOCU_SetModeSync(IO_ChannelType channel, IO_ModeType mode);
IO_ErrorType		OCOCU_EnableISR(IO_ChannelType channel, bool_t enableISR, Comm_Notification callback);
bool8_t				OCOCU_IsTickElapsed(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, IO_TickResType tick);
void				OCOCU_SetEventSync(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, uint32_t nsDelay);
void				OCOCU_SetRelativeEventSync(IO_ChannelType channel, uint32_t nsDelay);
IO_HwTimerType 		OCOCU_GetTickValByTriggerIndex(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex);
void 				OCOCU_SetTickValByTriggerIndex(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, IO_HwTimerType val);
/**
\n The function enables or disables the ISR of the channel for output compare event.

\param  [in]	channel - the identificator of the channel
\param  [in]	enableISR - true enables the ISR, false disables the ISR
\param  [in]	callback - pointer to the callback function

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
#define OCOCU_EnableISR(channel, enableISR, callback) OCICU_EnableISR(channel, enableISR, callback)
/************** Timer output compare - end ****************/

/************** Timer PWM - begin **************/
IO_ErrorType       OCPWM_InitSync(IO_PWM_CfgType *pPWM_Cfg);
IO_ErrorType       OCPWM_DeInitSync(IO_ChannelType channel);
IO_ErrorType       OCPWM_SetSync(IO_ChannelType channel, IO_TimingValueType tOn);
IO_TimingValueType OCPWM_GetSync(IO_ChannelType channel);
IO_TimingValueType OCPWM_GetPeriodSync(IO_ChannelType channel);
IO_ErrorType       OCPWM_SetPeriodSync(IO_ChannelType channel, IO_TimingValueType period);
void               OCPWM_SetAllSync(IO_ChannelType channel, IO_TimingValueType tOn, IO_TimingValueType period);

/**
\n The function enables or disables the ISR of the channel for PWM at the on-time edge of the pulse period.

\param  [in]	channel - the identificator of the channel
\param  [in]	enableISR - true enables the ISR, false disables the ISR
\param  [in]	callback - pointer to the callback function

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

*/
#define OCPWM_EnableT0ISR(channel, enableISR, callback) OCICU_EnableISR(channel, enableISR, callback)


IO_ErrorType OCPWM_EnableT1ISR(IO_ChannelType channel, bool_t enableISR, Comm_Notification callback);
/************** Timer PWM - end **************/

/************** Timer PWD - begin ****************************************************************/
IO_ErrorType			OCPWD_InitSync(IO_PWD_CfgType *pPWD_Cfg);
IO_ErrorType			OCPWD_DeInitSync(IO_ChannelType channel);
IO_ErrorType			OCPWD_StartSampleSync(IO_ChannelType channel, Comm_Notification callback);
IO_ErrorType			OCPWD_SetFilterTime(IO_ChannelType channel, IO_TimingValueType newFilterTime);
IO_ErrorType			OCPWD_SetTonMeasure(IO_ChannelType channel, bool8_t bTon);
void					OCPWD_GetAllSync(IO_ChannelType channel, IO_PWD_MeasureType *measurement);
void					OCPWD_GetAllSyncInTicks(IO_ChannelType channel, IO_PWD_RawMeasureType *measurement);
IO_ErrorType			OCPWD_StopSampleSync(IO_ChannelType channel);
/************** Timer PWD - end ******************************************************************/

/* Channel interrupts ********************************************************************************/
void HWTMRCH_ISR(IO_ChannelType channel);
/* ***************************************************************************************************/
IO_ErrorType 	OCTPM_GetChannelSync(const IO_ModuleIOMapType *const pIOMap,IO_ChannelType *pChannel);
uint8_t			OCTPM_GetTimerIDFromChannelID_1(uint8_t chID );
IO_ErrorType    OCTPM_GetTimerIDFromChannelID(int chID, uint8_t* pTmrId);
uint8_t			OCTPM_GetPinIDFromChannelID(int chID );
bool_t          OCTMR_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pTMRch);
uint8_t 		OCTMR_GetChannelIDFromPinID(uint8_t pinID);

/* ***************************************************************************************************/
#endif /* __OCTPM_DRV_H */

/* *********** */
/* End of file */
/* *********** */

