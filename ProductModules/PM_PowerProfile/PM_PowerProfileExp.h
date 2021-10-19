/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par        PM_PowerProfileExp.h
\n          This file contains all defines,macro,typedef and functions's prototypes that Application
            can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file       PM_PowerProfileExp.h
\ingroup    PM_PowerProfile
\date       Jul 20, 2012
\version    00.00.1
\author     Roberto Fioravanti
\n
\n
\par\b      History:
*/

#ifndef __PM_POWER_PROFILE_EXP_H
#define __PM_POWER_PROFILE_EXP_H

/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"
#include "IO_driver.h"

#if defined(CONFIG_PM_POWER_PROFILE_V2_0)
#include "LIB_FixedExp.h"
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
 * \brief   Power profile status
 * \details
 * bit 	7 	Delay transition
 * 	bits 	6-4 power profile SID (0 means invalid)
 * 	bits 	3-0 energy phase ID
 *
 *	\ref PM_PowerProfile_PowerProfileStatusType
 */
/** @{ */
#define  PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK    0x80
#define  PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_OFFS   7
#define  PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_MSK         0x70
#define  PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_OFFS        4
#define  PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK           0x0F
/** @} */

#define		POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF  0
#define 	POWER_PROFILE_PHASE_DELAY_TRANSITION_ON		1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Modifier value */
typedef int16_t PM_PowerProfile_ModifierValueType;


#if defined(CONFIG_PM_POWER_PROFILE_V2_0)

/** Modifier Value index */
typedef uint8_t PM_PowerProfile_ModifierValueIndexType;

/** Alfa Value index */
typedef uint8_t PM_PowerProfile_AlfaValueIndexType;

/** Modifier value */
typedef int16_t PM_PowerProfile_ModifierValueType;

/** Alpha value */
typedef LIB_Fixed_ValueType PM_PowerProfile_AlfaValueType;

/** Callback that executes modifier value retrieve from application. */
typedef int (*CALLBACK_PowerProfile_ModifierValueType)(PM_PowerProfile_ModifierValueIndexType index, PM_PowerProfile_ModifierValueType *valuePtr);

/** Callback that executes alpha value retrieve from application. */
typedef int (*CALLBACK_PowerProfile_AlphaValueType)(PM_PowerProfile_AlfaValueIndexType index, PM_PowerProfile_AlfaValueType *valuePtr);

#else  /* defined(CONFIG_PM_POWER_PROFILE_V2_0) */

/** Modifier index */
typedef uint8_t PM_PowerProfile_ModifierIndexType;

/** Callback that executes modifier computation */
typedef int (*CALLBACK_PowerProfile_ModifierType)(PM_PowerProfile_ModifierIndexType idx, PM_PowerProfile_ModifierValueType* value);

#endif  /* defined(CONFIG_PM_POWER_PROFILE_V2_0) */


/** Energy Phase ID. */
typedef uint8_t PM_PowerProfile_EnergyPhaseIDType;

/** Energy phase duration. */
typedef uint16_t PM_PowerProfile_DurationType;

/** Energy phase peak power. */
typedef uint16_t PM_PowerProfile_PeakPowerType;

/** Energy phase energy. */
typedef uint16_t PM_PowerProfile_EnergyType;

/** Energy phase max activation delay. */
typedef uint16_t PM_PowerProfile_MaxActivationDelayType;

/** Energy phase macro phase id. */
typedef uint8_t PM_PowerProfile_MacroPhaseIDType;

/** Cycle type. */
typedef uint8_t PM_PowerProfile_CycleType;

/** Power Profile inde.x */
typedef uint8_t PM_PowerProfile_ProfileIndexType;

/** Power Profile ID. */
typedef uint8_t PM_PowerProfile_ProfileIDType;

/** Functional phase ID. */
typedef uint8_t PM_PowerProfile_FunctionalPhaseIDType;

/** Modifier index. */
typedef uint16_t PM_PowerProfile_ModIndex;

/** Regulation bitmap. */
typedef uint8_t PM_PowerProfile_RegBmpType;

/** Appliance phase. */
typedef uint8_t PM_PowerProfile_AppliancePhaseType;

/** Power Profile size [B]. */
typedef uint8_t PM_PowerProfile_PowerProfileSizeType;

/** Power Profile SID. */
typedef uint8_t PM_PowerProfile_PowerProfileSIDType;

/** Status flags. */
typedef uint32_t PM_PowerProfile_StatusFlagsType;

/** Energy Phase descriptor */
typedef struct PM_PowerProfile_EnergyPhaseType
{
  PM_PowerProfile_EnergyPhaseIDType       energyPhaseID;        /**< Energy phase ID. */
  PM_PowerProfile_MacroPhaseIDType        macroPhaseID;         /**< Macro phase ID. */
  PM_PowerProfile_DurationType            expectedDuration;     /**< Expected duration [min]. */
  PM_PowerProfile_PeakPowerType           peakPower;            /**< Peak power [W]. */
  PM_PowerProfile_EnergyType              energy;               /**< Energy consumption [Wh]. */
  PM_PowerProfile_MaxActivationDelayType  maxActivationDelay;   /**< Max activation delay [min]. */
} PM_PowerProfile_EnergyPhaseType;

/** Number of power profiles. */
typedef uint8_t PM_PowerProfile_PowerProfileNumType;

/** Phase delay transition. */
typedef uint8_t PM_PowerProfile_PhaseDelayTransitionType;

/** Power profile error type */  /* TODO: Replace in functinos */
typedef enum PM_PowerProfile_ErrorType
{
   POWER_PROFILE_NO_ERR = 0,          	/**< No error. */
   POWER_PROFILE_GENERIC_ERR,         	/**< Generic error. */
   POWER_PROFILE_ID_ERR,              	/**< Power profile ID not valid. */
   POWER_PROFILE_SF_READING_ERR,      	/**< Setting file reading error. */
   POWER_PROFILE_NOT_AVAILABLE,       	/**< Power profile storage error. */
   POWER_PROFILE_MODIFIERS_VAR_TABLE_NOT_EXISTENT,
   POWER_PROFILE_MISSING_ENERGY_PHASE,
   POWER_PROFILE_BUILD_BMP_FUNC_ERR,  	/**< Setting file reading error. */
   POWER_PROFILE_FUNTIONAL_NO_MATCH,  	/**< No match with funtional phase. */
   POWER_PROFILE_EXCEEDINGCOMPARTMENT,  /**< No match with compartment. */
} PM_PowerProfile_ErrorType;

/** Cycle label. */
typedef uint8_t PM_PowerProfile_LabelType;

/** Compartment. */
typedef uint8_t PM_PowerProfile_CompartmentType;

/** Phase index. */
typedef uint8_t PM_PowerProfile_PhaseType;

/** Regulation index. */
typedef uint8_t PM_PowerProfile_RegulationType;

/** Number of regulations. */
typedef uint8_t PM_PowerProfile_RegulationsNumberType;

/** Power Profile status. */
typedef uint8_t PM_PowerProfile_PowerProfileStatusType;


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

PM_PowerProfile_PowerProfileNumType PM_PowerProfile_GetPowerProfilesNum(void);

IO_ErrorType PM_PowerProfile_GetPowerProfile
(
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_CycleType cycle,
  uint8_t mSetting,
  PM_PowerProfile_EnergyPhaseType **ppFirstEnergyPhase,
  PM_PowerProfile_PowerProfileSizeType *pSize
);

IO_ErrorType PM_PowerProfile_SetDuration
(
  PM_PowerProfile_ProfileIDType 		powerProfileID,
  PM_PowerProfile_DurationType 		duration
);

PM_PowerProfile_PowerProfileStatusType PM_PowerProfile_UpdatePowerProfileStatus
(
  PM_PowerProfile_CompartmentType			_Compartment,
  PM_PowerProfile_ProfileIndexType		_Index,
  PM_PowerProfile_PhaseType				_Phase,
  uint8_t 								mSetting,
  bool8_t									_Clear,
  PM_PowerProfile_PhaseDelayTransitionType transition,
  PM_PowerProfile_PowerProfileStatusType 	status
);

void PM_PowerProfile_SetForceReload (PM_PowerProfile_ProfileIDType _PowerProfileID);
void PM_PowerProfile_ResetForceReload(PM_PowerProfile_ProfileIDType _PowerProfileID);

IO_ErrorType	PM_PowerProfile_GetEnergyPhaseMaxActivationDelay
(
  PM_PowerProfile_ProfileIDType 			powerProfileID,
  PM_PowerProfile_ProfileIndexType 		_Index,
  PM_PowerProfile_FunctionalPhaseIDType	functionalPhaseId,
  uint8_t 								mSetting,
  bool_t* 								pNewEnergyPhase,
  uint16_t* 								pMaxActivationDelay
);

#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
IO_ErrorType PM_PowerProfile_AddEnergyPhaseInfoSpecialCycle (PM_PowerProfile_ProfileIDType powerProfileID,
                                                  uint8_t NAntiXtoAdd,
                                                  PM_PowerProfile_ProfileIndexType SpecialCycleIndex,
                                                  PM_PowerProfile_ProfileIDType currentEnPhaseID );

void PM_PowerProfile_GetTotalProfileEnergy(PM_PowerProfile_ProfileIDType powerProfileID, uint16_t* EnergyPhaseSum);

#endif /* defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT) */

void PM_PowerProfile_TotalProfileDuration(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType *energyPhaseDurationPtr);
void PM_PowerProfile_TotalProfileEnergy(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType *energyPhaseEnergyTotalPtr);


#if defined(CONFIG_PM_POWER_PROFILE_V2_0)

void PM_PowerProfile_SetParamModifierFunc(CALLBACK_PowerProfile_ModifierValueType func);
void PM_PowerProfile_SetAlphaFunc(CALLBACK_PowerProfile_AlphaValueType func);

PM_PowerProfile_DurationType PM_PowerProfile_ComputeTimeToEnd
(
  uint8_t mSetting,
  PM_PowerProfile_CompartmentType compartmentIndex,
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
  PM_PowerProfile_FunctionalPhaseIDType runningFunctionalPhaseID,
  PM_PowerProfile_DurationType runningEnergyPhaseNoSpecialsElapsedTime
);

bool_t PM_PowerProfile_IsRecomputationReady(bool_t recomputationRequest);
void PM_PowerProfile_UpdateTemporalFilters
(
  PM_PowerProfile_CompartmentType compartmentIndex,
  bool_t powerProfileHasChanged
);

#else  /* defined(CONFIG_PM_POWER_PROFILE_V2_0) */

void PM_PowerProfile_SetModifierFunc(CALLBACK_PowerProfile_ModifierType func);

PM_PowerProfile_DurationType PM_PowerProfile_ComputeTimeToEnd
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_EnergyPhaseIDType runningEnergyPhaseID,
  PM_PowerProfile_DurationType runningEnergyPhaseNoSpecialsElapsedTime
);

#endif  /* defined(CONFIG_PM_POWER_PROFILE_V2_0) */

#endif /* __PM_POWER_PROFILE_EXP_H */

/* *********** */
/* End of file */
/* *********** */

