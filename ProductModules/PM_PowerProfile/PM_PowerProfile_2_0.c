/*
Copyright (c) 2004-2014, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\n
\par        PM_PowerProfile_2_0.c
\n          it contains version 2.0 of Power Profile
\file       PM_PowerProfile_2_0.c
\ingroup    PM_PowerProfile
\date       May 27, 2014
\version    01.00
\author
\n
\n
\par\b      History:
\n
\n          01.00
\n          09/11/2010
\n          Alessandro Sabbatini
\n          - file created
*/

/* ******** */
/* Includes */
/* ******** */
/*lint -e537*/

#include "PM_PowerProfile.h"
#include "StandardDefines.h"
#include "LIB_UTilities.h"


/*lint +e537*/


#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT) && defined(CONFIG_PM_POWER_PROFILE_V2_0)

/**********************/
/* Defines and Macros */
/**********************/

/** assertion macro */
#define PM_POWER_PROFILE_ASSERT(Exp, ErrCode, ID)		if (!(Exp)) \
														{\
															if(PM_PowerProfile_Error(ID, ErrCode))\
															{\
																while(1){;}\
															}\
														}



/* PowerProfileStatus flags */
#define PM_POWER_PROFILE_STATUS_FLAGS__NO_MODIFIERS				1
#define PM_POWER_PROFILE_STATUS_FLAGS__NO_PTR_POWER_PROFILE		2
#define PM_POWER_PROFILE_STATUS_FLAGS__NO_ENERGY_PHASE			4
#define PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD				8

#if !defined(CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM)
#define CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM 				1
#endif

#if !defined(CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM)
#define CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM 			1
#endif

/* Macros to read bit-fields of SF_PowerProfileDataEnergyPhaseType */
#define SF_ENERGYPHASE_GET_PHASE_INDEX(_ptr)            MBGET(1, (SF_PowerProfileDataEnergyPhaseType*)(_ptr), index)
#define SF_ENERGYPHASE_GET_FUNCID(_ptr)                 BF_8_GET(MBGET(1, (_ptr), initFuncPhase),0,7)
#define SF_ENERGYPHASE_GET_DURATION_OFF(_ptr)           BF_16_GET((MBGET(2, _ptr, Doff_)),0,11)
#define SF_ENERGYPHASE_GET_DURATION_ON(_ptr)            BF_16_GET((MBGET(2, _ptr, Don_)),0,11)
#define SF_ENERGYPHASE_GET_MACROPHASE_ID(_ptr)          BF_16_GET(MBGET(2, _ptr, powerAndID_OFF), 13,3)
#define SF_ENERGYPHASE_GET_MEANPOWER_OFF(_ptr)          BF_16_GET((MBGET(2, _ptr, powerAndID_OFF)),0,13)
#define SF_ENERGYPHASE_GET_MEANPOWER_ON(_ptr)           BF_16_GET((MBGET(2, _ptr, power_ON)),0,13)


/* Macros to read bit-fields of SF_EnergyPhaseDataParamsType */
#define SF_ENERGYPHASEPARAMS_GET_MAXPOWER(_ptr)         BF_16_GET(MBGET(2, _ptr, maxPower), 0,13)
#define SF_ENERGYPHASEPARAMS_GET_MAXDELAY(_ptr)         BF_16_GET(MBGET(2, _ptr, maxDelay), 0,11)
#define SF_ENERGYPHASEPARAMS_GET_MAXDELAY_OVLD(_ptr)    BF_16_GET(MBGET(2, _ptr, maxDelayOvld), 0,11)
#define SF_ENERGYPHASEPARAMS_GET_MAXPOWERMOD(_ptr)      MBGET(2, _ptr, maxPowerModIdx)
#define SF_ENERGYPHASEPARAMS_GET_ENERGYMOD_ON(_ptr)     MBGET(2, _ptr, onMeanPowerModIdx)
#define SF_ENERGYPHASEPARAMS_GET_ENERGYMOD_OFF(_ptr)    MBGET(2, _ptr, offMeanPowerModIdx)
#define SF_ENERGYPHASEPARAMS_GET_DURATIONMOD_ON(_ptr)   MBGET(2, _ptr, DonModIdx)
#define SF_ENERGYPHASEPARAMS_GET_DURATIONMOD_OFF(_ptr)  MBGET(2, _ptr, DoffModIdx)
#define SF_ENERGYPHASEPARAMS_GET_MAXDELAYMOD(_ptr)      MBGET(2, _ptr, maxDelayModIdx)
#define SF_ENERGYPHASEPARAMS_GET_MAXDELAY_OVLDMOD(_ptr) MBGET(2, _ptr, maxDelayOvldModIdx)

/** Max energy value.*/
#define PM_POWER_PROFILE_MAX_ENERGY_VALUE     0xFFFF

/** Max duration value.*/
#define PM_POWER_PROFILE_MAX_DURATION_VALUE   0xFFFF

/** Main setting index.*/
#if !defined(MAIN_SF_INDEX)
#define MAIN_SF_INDEX                         0
#endif /* !defined(MAIN_SF_INDEX) */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Power profile status */
typedef struct PM_PowerProfile_StatusType
{
  PM_PowerProfile_ErrorType       error;
  PM_PowerProfile_StatusFlagsType flags;
} PM_PowerProfile_StatusType;


/** Power profile descriptor */
typedef struct PM_PowerProfile_DescriptorType
{
  CALLBACK_PowerProfile_ModifierValueType paramValueGet;
  CALLBACK_PowerProfile_AlphaValueType    alphaValueGet;
  PM_PowerProfile_PowerProfileNumType     totalPowerProfilesNum;
  PM_PowerProfile_PowerProfileStatusType  profileStatus[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
  PM_PowerProfile_EnergyPhaseType         energyPhase[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM][CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM];
  PM_PowerProfile_DurationType            energyPhaseCounter[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
  PM_PowerProfile_PowerProfileSizeType    powerProfileSize[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
  PM_PowerProfile_PowerProfileSIDType     powerProfileSID[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
  PM_PowerProfile_StatusType              status[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
} PM_PowerProfile_DescriptorType;


/** Modifier table 1st level header */
typedef struct PM_PowerProfile_ModTab_Level1HeaderType
{
  /* Byte 0 */
  uint8_t alfa0VarPos;        /**< Indice della variabile alpha 0. */

  /* Byte 1 */
#if defined(__BIT_ORDER_MSB_FIRST__)
  uint8_t unusedB1b37   : 5;
  uint8_t  ML           : 3;  /**< Logica di combinazione tra modificatori */
#else /* __BIT_ORDER_MSB_FIRST__ */
  uint8_t ML            : 3;  /**< Logica di combinazione tra modificatori */
  uint8_t unusedB1b37   : 5;
#endif /* !__BIT_ORDER_MSB_FIRST__ */
} PM_PowerProfile_ModTab_Level1HeaderType;


/** Alpha value index type */
typedef uint8_t PM_PowerProfile_ModTab_AlphaIndexType;

/** Modifier value index type */
typedef uint16_t PM_PowerProfile_ModTab_ModIndexType;


/** Modifier table 1st level entry */
typedef struct PM_PowerProfile_ModTab_Level1EntryType
{
  /* Byte 0 */
  uint8_t unusedB0;

  /* Byte 1 */
#if defined(__BIT_ORDER_MSB_FIRST__)
  uint8_t unusedB1b67   : 2;
  uint8_t alfaVarValPos : 6;  /**< Indice della variabile alpha. */
#else /* __BIT_ORDER_MSB_FIRST__ */
  uint8_t alfaVarValPos : 6;  /**< Indice della variabile alpha.*/
  uint8_t unusedB1b67   : 2;
#endif /* !__BIT_ORDER_MSB_FIRST__ */

  /* Bytes 2..3 */
  uint16_t TableIndex;        /**< Indice della tabella di livello 2. */

} PM_PowerProfile_ModTab_Level1EntryType;


/** Modifier table 2nd level header */
typedef struct PM_PowerProfile_ModTab_Level2HeaderType
{
#if defined(__BIT_ORDER_MSB_FIRST__)
  /* Byte 0 */
  uint8_t SearchMode    : 3;  /**< Metodo di ricerca. */
  uint8_t SVal          : 2;  /**< Formato dati valori modificati. */
  uint8_t SMod          : 2;  /**< Formato dati modificatori. */
  uint8_t unusedB0b0    : 1;
#else /* __BIT_ORDER_MSB_FIRST__ */
  /* Byte 0 */
  uint8_t reservedB0b0  : 1;
  uint8_t SMod          : 2;  /**< Formato dati modificatori. */
  uint8_t SVal          : 2;  /**< Formato dati valori modificati. */
  uint8_t SearchMode    : 3;  /**< Metodo di ricerca. */
#endif /* !__BIT_ORDER_MSB_FIRST__ */

  /* Byte 1 */
  uint8_t itemsNumber;        /**< Numero di elementi della tabella. */

  /* Byte 2 */
  uint8_t ImmediateVal;       /**< Valore immediato. */

  /* Byte 3 */
  uint8_t DT            : 2;  /**< Tipo di tabella. */
  uint8_t ModVarValue1  : 6;  /**< Indice variabile modificatore. */
} PM_PowerProfile_ModTab_Level2HeaderType;


/**
 * \brief   Extended modifier value type.
 * \details Used as common denominator type for all table in/out data types.
 * \pre     Must be at least twice the size of the biggest modifier type:
 *          \code{.c} sizeof(PM_PowerProfile_ModTab_ExtendedModifierValueType) >= (2 * sizeof(PM_PowerProfile_ModifierValueType)) \endcode
 */
typedef int32_t PM_PowerProfile_ModTab_ExtendedModifierValueType;

/** Modifier table input/output data type enumerator */
typedef enum PM_PowerProfile_ModTab_InOutType
{
  PM_PowerProfile_ModTab_InOutType_UINT8  = 0,  /**< \p uint8_t */
  PM_PowerProfile_ModTab_InOutType_UINT16 = 1,  /**< \p uint16_t */
  PM_PowerProfile_ModTab_InOutType_INT8   = 2,  /**< \p int8_t */
  PM_PowerProfile_ModTab_InOutType_INT16  = 3,  /**< \p int16_t */
  PM_PowerProfile_ModTab_InOutType__LENGTH
} PM_PowerProfile_ModTab_InOutType;


/** Modifier table search mode */
typedef enum PM_PowerProfile_ModTab_SearchModeType
{
  PM_PowerProfile_ModTab_SearchModeType_EQUALITY      = 0,  /**< operator == */
  PM_PowerProfile_ModTab_SearchModeType_EXCESS        = 1,  /**< operator <= */
  PM_PowerProfile_ModTab_SearchModeType_DEFICIENCY    = 2,  /**< operator >= */
  PM_PowerProfile_ModTab_SearchModeType_INTERPOLATION = 3,  /**< operator lerp() */
  PM_PowerProfile_ModTab_SearchModeType__LENGTH
} PM_PowerProfile_ModTab_SearchModeType;


/** Modifier table type */
typedef enum PM_PowerProfile_ModTab_TableType
{
  PM_PowerProfile_ModTab_TableType_MONODIMENSIONAL  = 0,  /**< Monodimensional table (only 2nd level) */
  PM_PowerProfile_ModTab_TableType_BIDIMENSIONAL    = 1,  /**< Bidimensional table (nested 1st-2nd levels) */
  PM_PowerProfile_ModTab_TableType_IMMEDIATE        = 2,  /**< Immediate value (in 1st level) */
  PM_PowerProfile_ModTab_TableType__LENGTH
} PM_PowerProfile_ModTab_TableType;


/** Modifier logic */
typedef enum PM_PowerProfile_ModTab_ModLogicType
{
  PM_PowerProfile_ModTab_ModLogicType_MAX = 0,  /**< Maximum among all modifiers */
  PM_PowerProfile_ModTab_ModLogicType_MIN = 1,  /**< Minimum among all modifiers */
  PM_PowerProfile_ModTab_ModLogicType_SUM = 2,  /**< Sum all modifiers */
  PM_PowerProfile_ModTab_ModLogicType__LENGTH
} PM_PowerProfile_ModTab_ModLogicType;


/** Size in bytes of table in/out data */
static const uint8_t PM_PowerProfile_ModTab_InOutSizes[PM_PowerProfile_ModTab_InOutType__LENGTH] =
{
  (uint8_t)sizeof(uint8_t),
  (uint8_t)sizeof(uint16_t),
  (uint8_t)sizeof(int8_t),
  (uint8_t)sizeof(int16_t)
};


static PM_PowerProfile_ModTab_ExtendedModifierValueType PM_PowerProfile_ModTab_ExtendValue
(
  const void *datumPtr,
  PM_PowerProfile_ModTab_InOutType type
);

static IO_ErrorType PM_PowerProfile_ModTab_QueryTable
(
  const PM_PowerProfile_ModTab_Level2HeaderType *tableHeaderPtr,
  const PM_PowerProfile_ModifierValueType *inputPtr,
  PM_PowerProfile_ModifierValueType *outputPtr,
  bool_t secondLevel
);

static IO_ErrorType PM_PowerProfile_ModTab_GetValue
(
  uint8_t mSetting,
  PM_PowerProfile_ModIndex modIndex,
  PM_PowerProfile_ModifierValueType *modValuePtr
);


/**
Energy phase structure in Power Profile Data pointer, main parameters
Structure definition does not reflect binary structure.
*/
typedef struct SF_PowerProfileDataEnergyPhaseType
{
  uint8_t index0;           /**< energy Phase index */
  uint8_t initFuncPhase0;   /**< initial functional phase */
  uint8_t Doff_1;           /**< default duration in minutes of the energy phase, with dominant load OFF: lower 11 bits */
  uint8_t Doff_0;
  uint8_t Don_1;            /**< default duration in minutes of the energy phase, with dominant load OFF: lower 11 bits */
  uint8_t Don_0;
  uint8_t powerAndID_OFF1;  /**< ID: 13-15 */
  uint8_t powerAndID_OFF0;  /**< power with dominant load OFF: 0-12 */
  uint8_t power_ON1;        /**< power with dominant load ON : 0-12 */
  uint8_t power_ON0;
} SF_PowerProfileDataEnergyPhaseType;


/**
Energy phases, additional parameters (Energy Phase Data pointer)
*/
typedef struct SF_EnergyPhaseDataParamsType
{
  uint8_t maxPower1;            /**< max power: 0-12*/
  uint8_t maxPower0;
  uint8_t maxDelay1;            /**< max delay: 0-10*/
  uint8_t maxDelay0;
  uint8_t maxDelayOvld1;        /**< max delay overload: 0-10*/
  uint8_t maxDelayOvld0;
  uint8_t maxPowerModIdx1;      /**< max power modifier index */
  uint8_t maxPowerModIdx0;
  uint8_t onMeanPowerModIdx1;   /**< energy modifier index, load ON */
  uint8_t onMeanPowerModIdx0;
  uint8_t offMeanPowerModIdx1;  /**< energy modifier index, load OFF */
  uint8_t offMeanPowerModIdx0;
  uint8_t DonModIdx1;           /**< duration modifier index, load ON */
  uint8_t DonModIdx0;
  uint8_t DoffModIdx1;          /**< duration modifier index, load OFF */
  uint8_t DoffModIdx0;
  uint8_t maxDelayModIdx1;      /**< max delay modifier index */
  uint8_t maxDelayModIdx0;
  uint8_t maxDelayOvldModIdx1;  /**< max delay overload modifier index */
  uint8_t maxDelayOvldModIdx0;
} SF_EnergyPhaseDataParamsType;


/** SID filter downcounter type */
typedef uint8_t PM_PowerProfile_SIDFilterDowncounterType;

/**
SID filter structure.
*/
typedef struct PM_PowerProfile_SIDFilterType
{
  /** Downcounter for APP change bursts filter.*/
  PM_PowerProfile_SIDFilterDowncounterType requestBurstDowncounter;

  /** Downcounter for external bus access filter.*/
  PM_PowerProfile_SIDFilterDowncounterType busAccessDowncounter;

  /** A recomputation was queued because of ongoing transmission on the external bus.*/
  bool_t queuedBusAccessRequest;
} PM_PowerProfile_TemporalFiltersType;


/**
\name SID temporal filter parameters
@{
*/
#define PP_SIDFILTER_CHANGEBURST_WINDOW (2)   /**< Window length (100ms unit) for regulation burst filter.*/
#define PP_SIDFILTER_BUSACCESS_WINDOW   (20)  /**< Window length (100ms unit) for external bus access filter.*/
/**@}*/

/*********************************/
/* Variables                     */
/*********************************/

/* Extern */

/* Global */

/* Static */

/**
 * \brief   Power Profile buffer.
 * \details Little Endian data format. Exchanged with external devices.
 */
static PM_PowerProfile_DescriptorType PM_PowerProfile_Data;

/**
 * \brief   Energy phase ID map
 * \details Maps the inserted Energy Phase index to the Energy Phase index
 *          of the Setting File.
 */
static PM_PowerProfile_EnergyPhaseIDType PM_PowerProfile_EnergyPhaseMap[CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM];

/**
 * \brief   Temporal filters.
 * \details Holds the status of temporal filters used to recompute and send
 *          the Power Profile.
 */
static PM_PowerProfile_TemporalFiltersType PM_PowerProfile_TemporalFilters = { 0, 0, FALSE };


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */

static void PM_PowerProfile_UpdateGlobalStatus
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_StatusFlagsType flags
);

static PM_PowerProfile_ErrorType PM_PowerProfile_Error
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_ErrorType _ErrorCode
);

static IO_ErrorType PM_PowerProfile_GetEnergyPhaseID
(
	uint8_t mSetting,
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
	PM_PowerProfile_FunctionalPhaseIDType functionalPhaseID,
	PM_PowerProfile_EnergyPhaseIDType* energyPhaseIDPtr
);

static void PM_PowerProfile_ReloadPowerProfileInfo
(
	PM_PowerProfile_ProfileIDType powerProfileID,
	PM_PowerProfile_EnergyPhaseIDType	currentEnergyPhaseID,
	PM_PowerProfile_EnergyPhaseType **ppCurrentEnergyPhase,
	PM_PowerProfile_PowerProfileSizeType *pSize
);

IO_ErrorType PM_PowerProfile_GetModValue
(
  uint8_t mSetting,
  uint16_t index,
  PM_PowerProfile_ModifierValueType modBase,
  LIB_Fixed_ValueType *modResultPtr
);

IO_ErrorType PM_PowerProfile_CalcDurationPartial
(
  LIB_Fixed_ValueType durationValue,
  PM_PowerProfile_DurationType *resultPtr
);

IO_ErrorType PM_PowerProfile_CalcDurationTotal
(
  PM_PowerProfile_DurationType duration1,
  PM_PowerProfile_DurationType duration2,
  PM_PowerProfile_DurationType *resultPtr
);

IO_ErrorType PM_PowerProfile_CalcPeakPower
(
  PM_PowerProfile_ModIndex peakPowerIndex,
  uint8_t mSetting,
  const SF_EnergyPhaseDataParamsType *sfEnergyPhaseAddPtr,
  PM_PowerProfile_PeakPowerType *peakPowerPtr
);

IO_ErrorType PM_PowerProfile_CalcEnergy
(
  PM_PowerProfile_DurationType durationOn,
  PM_PowerProfile_DurationType durationOff,
  const SF_PowerProfileDataEnergyPhaseType *energyPhaseParamsPtr,
  PM_PowerProfile_EnergyType *meanEnergyPtr
);


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
  Module initialization.

\author     Roberto Fioravanti
\date       20/07/2012
\version    1.0.0
*/
void PM_PowerProfile_Init (void)
{
	/*fill with 0s the internal variable*/
	memset(&PM_PowerProfile_Data, 0, sizeof(PM_PowerProfile_Data));
}


/**
Sets the function for getting of values of the modifiers
\param[in] func function pointer
\author     Pietro Loschiavo
\date       14/03/2014
\version    1.0.0
*/
void PM_PowerProfile_SetParamModifierFunc(CALLBACK_PowerProfile_ModifierValueType func)
{
	PM_PowerProfile_Data.paramValueGet = func;
}


/**
Sets the function for getting of alpha to application
\param[in] func function pointer
\author     Pietro Loschiavo
\date       14/03/2014
\version    1.0.0
*/
void PM_PowerProfile_SetAlphaFunc(CALLBACK_PowerProfile_AlphaValueType func)
{
  PM_PowerProfile_Data.alphaValueGet = func;
}


/**
	Returns the maximum number of managed power profile
\author     Roberto Fioravanti
\date       06/08/2012
\version    1.0.0
*/
PM_PowerProfile_PowerProfileNumType PM_PowerProfile_GetPowerProfilesNum(void)
{
	return CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM;
}


/**
Updates the status of the power profile
\param[in]	mSetting		setting to search in
\param[in]	_Compartment	compartment of the appliente
\param[in]	_Index			displacement in PTR_POWER_PROFILE_DATA
\param[in]  _Phase     		functional phase
\param[in]  _Clear			if at 1 clear the status
\param[in]	transition		new value for transition bit
\param[in]	status			current status

\return     PM_PowerProfile_PowerProfileStatusType
\ref PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK
\ref PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_OFFS
\ref PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_MSK
\ref PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_OFFS
\ref PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK

\author     Roberto Fioravanti
\date       24/11/2010
\version    1.0.0
*/
PM_PowerProfile_PowerProfileStatusType PM_PowerProfile_UpdatePowerProfileStatus
(
  PM_PowerProfile_CompartmentType _Compartment,
  PM_PowerProfile_ProfileIndexType _Index,
  PM_PowerProfile_PhaseType _Phase,
  uint8_t mSetting,
  bool8_t _Clear,
  PM_PowerProfile_PhaseDelayTransitionType transition,
  PM_PowerProfile_PowerProfileStatusType status
)
{
	PM_PowerProfile_PowerProfileStatusType appStatus = 0;
	PM_PowerProfile_EnergyPhaseIDType energyPhID = 0;

	if ( _Clear == TRUE ) {
		appStatus = status; 		/* copy current value	*/
		/* Clear:
		 - the PP counter bits (x000 xxxx)
		 - the En. Phase transition bit (0xxx xxxx)
		 * */
		appStatus &= ~(PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK|
						PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_MSK);
	}
	else
	{
		if ( PM_PowerProfile_GetEnergyPhaseID(mSetting, _Index, _Phase, &energyPhID) == IO_E_OK )
		{
			appStatus = energyPhID & PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK;
			appStatus |= ((PM_PowerProfile_Data.powerProfileSID[_Compartment] << PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_OFFS) & PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_MSK);
			/* update the transition bit */
			appStatus |= ((transition!=0)?PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK:0);
		}
		else
		{
			appStatus=0;
		}
	}

	PM_PowerProfile_Data.profileStatus[_Compartment]=appStatus;

	return appStatus;
}


/**
  Returns the pointer of energy phase maximum activation delay
  (pMaxActivationDelay) if only the requested functional phase (functionalPhaseId) of selected
  power profile (powerProfileID) is the first functional phase of a EnergyPhase.
  \param[in]	mSetting			Setting to search in
  \param[in]  _Index        		index in setting for selected power profile;
  \param[in]  functionalPhaseId     requested functional phase;
  \param[out] pNewEnergyPhase       is the functional phase selected the first of EnergyPhase ?
  \param[out] pMaxActivationDelay   max activation delay of selected energy phase:
                                    this contains valid value only if *pNewEnergyPhase = TRUE
  \return     status of operations.

  \author     Andrea Ricci
  \date       02/03/2011
  \version    1.0.0
*/
IO_ErrorType PM_PowerProfile_GetEnergyPhaseMaxActivationDelay
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_ProfileIndexType _Index,
  PM_PowerProfile_FunctionalPhaseIDType functionalPhaseId,
  uint8_t mSetting,
  bool_t *pNewEnergyPhase,
  uint16_t *pMaxActivationDelay
)
{
	uint8_t i = 0;
	uint8_t energyPhaseCount = 0;
	uint8_t* pPPData = 0;
	Loader_Struct sfData = {0};
	SF_PowerProfileDataEnergyPhaseType* psfEnergyPhase = 0;

	uint8_t bl = (!SF_OK);

	/* Default value */
	*pNewEnergyPhase = FALSE;
	*pMaxActivationDelay = 0x0000;

	/* retrieve power profile info */

	bl = MSettingBasicLoader(	mSetting, SF_PTR_POWER_PROFILE, _Index, &sfData);

	if ( bl != SF_OK )
	{
		return IO_E_FAIL;
	}
	else
	{
		/* is there an energy phase? */
		energyPhaseCount = sfData.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType) / sizeof(uint16_t));

		if( energyPhaseCount == 0 )
		{
			/* the current power profile does not include any energy phase !?! */
			PM_PowerProfile_UpdateGlobalStatus(powerProfileID, PM_POWER_PROFILE_STATUS_FLAGS__NO_ENERGY_PHASE);
			return IO_E_FAIL;
		}
		else
		{
			pPPData = sfData.Data;

			/* retrieve each energy phases from sf.. */
			for ( i = 0; i < energyPhaseCount; i++ )
			{
				psfEnergyPhase = (SF_PowerProfileDataEnergyPhaseType*)((void*)pPPData);
				/* hp:  functional phases in monotonic order*/
				if ( SF_ENERGYPHASE_GET_FUNCID(psfEnergyPhase) == functionalPhaseId )
				{
					*pNewEnergyPhase = TRUE;
					break;
				}
				/* move to the next energy phases inside power profile description */
				pPPData += sizeof(SF_PowerProfileDataEnergyPhaseType);
			}

			if ( *pNewEnergyPhase == TRUE )
			{

				bl = MSettingBasicLoader(mSetting, SF_PTR_ENERGY_PHASE, SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase), &sfData);

				if ( bl != SF_OK )
				{
					/* error loading power profile index data */
					return IO_E_FAIL;
				}
				else
				{
					if ( sfData.DataLenght == (sizeof(SF_EnergyPhaseDataParamsType) / sizeof(uint16_t)) )
					{
						*pMaxActivationDelay = SF_ENERGYPHASEPARAMS_GET_MAXDELAY((SF_EnergyPhaseDataParamsType*)((void*)(sfData.Data)));
					}
				}
			}
		}
	}
	return IO_E_OK;
}

/*
	Sets the duration EnPhElapsedTime_Counter
*/
IO_ErrorType PM_PowerProfile_SetDuration(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_DurationType duration
)
{
	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= powerProfileID) && (powerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, powerProfileID)
	PM_PowerProfile_Data.energyPhaseCounter[powerProfileID-1] = duration;
	return IO_E_OK;
}


/**
  Calculates and returns the energy phases of the requested power profile.
  Called by application supervisor.
  \param[in]	_Index					displacement in SF_PTR_POWER_PROFILE
  \param[in]	powerProfileID        requested power profile identifier;
  \param[in]	cycle                 current cycle
  \param[out]	ppFirstEnergyPhase    pointer to returned power profile buffer;
  \param[out]	pSize                 size of returned power profile
                                    ( i.e. #(EnergyPhases)*sizeof(EnergyPhase) );
  \return     status of operations.

  \author     Andrea Ricci
  \date       25/11/2010
  \version    1.0.0
*/
IO_ErrorType PM_PowerProfile_GetPowerProfile
(
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_CycleType cycle,
  uint8_t mSetting,
  PM_PowerProfile_EnergyPhaseType **firstEnergyPhasePtrPtr,
  PM_PowerProfile_PowerProfileSizeType *sizePtr
)
{
  Loader_Struct loader = { NULL, 0 };
  const SF_PowerProfileDataEnergyPhaseType *energyPhaseParamsPtr = NULL;  /* Pointer to energy phase data struct */
  const SF_EnergyPhaseDataParamsType *energyPhaseAddParamsPtr = NULL;  /* Pointer to energy phase params struct */

  PM_PowerProfile_EnergyPhaseIDType energyPhaseCount = 0;
  PM_PowerProfile_EnergyPhaseIDType insertedEnergyPhaseCount = 0;
  PM_PowerProfile_EnergyPhaseIDType i = 0;

  PM_PowerProfile_EnergyPhaseType *energyPhasePtr = NULL;  /* Pointer to energy phase descriptor inside the buffer */

  /* Duration */
  PM_PowerProfile_DurationType duration;
  PM_PowerProfile_DurationType durationOn;
  PM_PowerProfile_DurationType durationOnBase;
  PM_PowerProfile_DurationType durationOff;
  PM_PowerProfile_DurationType durationOffBase;
  PM_PowerProfile_ModIndex durationOnIndex;
  PM_PowerProfile_ModIndex durationOffIndex;
  LIB_Fixed_ValueType durationOnValue;
  LIB_Fixed_ValueType durationOffValue;

  /* Energy */
  PM_PowerProfile_EnergyType energy;

  /* Power*/
  PM_PowerProfile_PeakPowerType peakPower;
  PM_PowerProfile_ModIndex peakPowerIndex;

  IO_ErrorType errorIO = IO_E_OK;

  *firstEnergyPhasePtrPtr = NULL;
  *sizePtr = 0;

  /* Acquire Power Profile buffer */
  UsePowerProfile();

  /* Retrieve Power Profile info */
  if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_POWER_PROFILE, powerProfileIndex, &loader) )
  {
    energyPhaseCount = (loader.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType) / sizeof(uint16_t)));
    if ( energyPhaseCount > 0 )
    {
      energyPhaseParamsPtr = (const SF_PowerProfileDataEnergyPhaseType *)loader.Data;
      energyPhasePtr = &(PM_PowerProfile_Data.energyPhase[powerProfileID - 1][0]);

      /* Retrieve each energy phase from SF... */
      for ( i = 0; i < energyPhaseCount; ++i )
      {
        /* Load second table */
        if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_ENERGY_PHASE, SF_ENERGYPHASE_GET_PHASE_INDEX(energyPhaseParamsPtr), &loader) )
        {
          if ( (sizeof(SF_EnergyPhaseDataParamsType) / sizeof(uint16_t)) == loader.DataLenght )
          {
            energyPhaseAddParamsPtr = (const SF_EnergyPhaseDataParamsType *)loader.Data;

            /* Compute D_{Ron} value */
            durationOnIndex = SF_ENERGYPHASEPARAMS_GET_DURATIONMOD_ON(energyPhaseAddParamsPtr);
            durationOnBase = SF_ENERGYPHASE_GET_DURATION_ON(energyPhaseParamsPtr);
            PM_PowerProfile_GetModValue(mSetting, durationOnIndex, (PM_PowerProfile_ModifierValueType)durationOnBase, &durationOnValue);
            PM_PowerProfile_CalcDurationPartial(durationOnValue, &durationOn);

            /* Compute D_{Roff} value */
            durationOffIndex = SF_ENERGYPHASEPARAMS_GET_DURATIONMOD_OFF(energyPhaseAddParamsPtr);
            durationOffBase = SF_ENERGYPHASE_GET_DURATION_OFF(energyPhaseParamsPtr);
            PM_PowerProfile_GetModValue(mSetting, durationOffIndex, (PM_PowerProfile_ModifierValueType)durationOffBase, &durationOffValue);
            PM_PowerProfile_CalcDurationPartial(durationOffValue, &durationOff);

            /* Compute total duration */
            PM_PowerProfile_CalcDurationTotal(durationOn, durationOff, &duration);

            /* The energy phase is inserted only if its duration is not zero */
            if ( duration > 0 )
            {
              /* Compute power and energy */
              peakPowerIndex = SF_ENERGYPHASEPARAMS_GET_MAXPOWERMOD(energyPhaseAddParamsPtr);
              PM_PowerProfile_CalcPeakPower(peakPowerIndex, mSetting, energyPhaseAddParamsPtr, &peakPower);
              PM_PowerProfile_CalcEnergy(durationOn, durationOff, energyPhaseParamsPtr, &energy);

              /* Fill protocol struct with little endian format */
              energyPhasePtr->energyPhaseID = (insertedEnergyPhaseCount + 1);
              energyPhasePtr->macroPhaseID = SF_ENERGYPHASE_GET_MACROPHASE_ID(energyPhaseParamsPtr);
              energyPhasePtr->expectedDuration = EndianU16_NToL(duration);
              energyPhasePtr->peakPower = EndianU16_NToL(peakPower);
              energyPhasePtr->energy = EndianU16_NToL(energy);
              energyPhasePtr->maxActivationDelay = EndianU16_NToL(SF_ENERGYPHASEPARAMS_GET_MAXDELAY(energyPhaseAddParamsPtr));

              /* Linking phase retrieved from SF to one stored to APP Buffer */
              PM_PowerProfile_EnergyPhaseMap[insertedEnergyPhaseCount] = (i + 1);

              /* Chech whether the buffer is full (exceeding energy phases will be ignored) */
              ++insertedEnergyPhaseCount;
              if ( insertedEnergyPhaseCount >= CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM )
              {
                i = (energyPhaseCount - 1);  /* break */
              }

              /* Move to the next energy phase inside the buffer */
              ++energyPhasePtr;
            }

            /* Consider correct at this point */
            errorIO = IO_E_OK;
          }
          else
          {
            /* Wrong profile size */
            errorIO = IO_E_FAIL;
            i = (energyPhaseCount - 1);  /* break */
          }
        }
        else
        {
          /* Error loading power profile index data */
          errorIO = IO_E_FAIL;
          i = (energyPhaseCount - 1);  /* break */
        }

        /* Move to the next energy phase inside the power profile description */
        ++energyPhaseParamsPtr;
      }
    }
    else
    {
      /* The current power profile does not include any energy phases */
      errorIO = IO_E_FAIL;
    }
  }
  else
  {
    /* Error loading from setting file */
    errorIO = IO_E_FAIL;
  }

  if ( IO_E_OK == errorIO )
  {
    /* Update output values and sizes */
    *firstEnergyPhasePtrPtr = &(PM_PowerProfile_Data.energyPhase[powerProfileID - 1][0]);
    *sizePtr = (insertedEnergyPhaseCount * sizeof(PM_PowerProfile_EnergyPhaseType));
    PM_PowerProfile_Data.powerProfileSize[powerProfileID - 1] = *sizePtr;
  }

  /* Release Power Profile buffer */
  UnusePowerProfile();

  return (errorIO);
}


/*
	Forces reloading of information because interpreter is running.
	E.g. it allows the update the remaining duration
	\param[in]	_PowerProfileID	power profile ID

	\author     Roberto Fioravanti
	\date       07/08/2012
	\version    1.0.0
*/
void PM_PowerProfile_SetForceReload(PM_PowerProfile_ProfileIDType _PowerProfileID)
{
	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= _PowerProfileID) && (_PowerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, _PowerProfileID)
	PM_PowerProfile_Data.status[_PowerProfileID-1].flags |= PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD;
}


/*
	Reloading of information because interpreter is running is stopped.
	\param[in]	_PowerProfileID	power profile ID
	\author     Roberto Fioravanti
	\date       07/08/2012
	\version    1.0.0
*/
void PM_PowerProfile_ResetForceReload(PM_PowerProfile_ProfileIDType _PowerProfileID)
{
	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= _PowerProfileID) && (_PowerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, _PowerProfileID)
	PM_PowerProfile_Data.status[_PowerProfileID-1].flags &= ~(PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD);
}


/**
  	  Retrieves the pointer to the currently stored power profile (ppFirstEnergyPhase), its size (pSize) by
  the power profile identifier (pPowerProfileID).
\param[in]  PowerProfileID        requested power profile identifier;
\param[out] ppFirstEnergyPhase    pointer to returned power profile buffer;
\param[out] pSize                 size of returned power profile
                                ( i.e. #(EnergyPhases)*sizeof(EnergyPhase) );
\return     none.

\author     Roberto Fioravanti, Andrea Ricci
\date       24/11/2010-25/11/2010
\version    1.0.0
*/
void PM_PowerProfile_GetPowerProfileByProfileID
(
  PM_PowerProfile_ProfileIDType _PowerProfileID,
  PM_PowerProfile_EnergyPhaseType **ppFirstEnergyPhase,
  PM_PowerProfile_PowerProfileSizeType *pSize
)
{
  PM_PowerProfile_EnergyPhaseIDType currentEnPhaseID = 1;

  PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= _PowerProfileID) && (_PowerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, _PowerProfileID)

  /* if the PowerProfile is already running --> reload the the PowerProfile info */
  if ( PM_PowerProfile_Data.status[_PowerProfileID-1].flags & PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD )
  {
    currentEnPhaseID = (PM_PowerProfile_Data.profileStatus[_PowerProfileID-1] & PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK);

    /* Reload the PowerProfile info */
    PM_PowerProfile_ReloadPowerProfileInfo(_PowerProfileID, currentEnPhaseID, ppFirstEnergyPhase, pSize);
  }
  else
  {
    *ppFirstEnergyPhase = &(PM_PowerProfile_Data.energyPhase[_PowerProfileID-1][0]);
    *pSize = PM_PowerProfile_Data.powerProfileSize[_PowerProfileID-1];
  }
}


/* Local Function bodies */
/**
  Returns the energy phase ID (*pEnergyPhaseID) which matches the
  requested functional phase (functionalPhaseId) of selected power profile (powerProfileID).
  \param[in]  mSetting				setting to search in;
  \param[in]  powerProfileIndex        		power profile index in setting file
  \param[in]  functionalPhaseId     requested functional phase;
  \param[out] pEnergyPhaseID        pointer to returned energy phase id;

  \return     status of operations.

  \author     Andrea Ricci
  \date       25/11/2010
  \version    1.0.0
  \todo       the routine can be faster in some cases (actually frequently..?) if functional
              phases are locally stored during PP computation.
*/
IO_ErrorType PM_PowerProfile_GetEnergyPhaseID
(
	uint8_t mSetting,
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
	PM_PowerProfile_FunctionalPhaseIDType functionalPhaseID,
	PM_PowerProfile_EnergyPhaseIDType *energyPhaseIDPtr
)
{
  Loader_Struct loader = { NULL, 0 };
  const SF_PowerProfileDataEnergyPhaseType *energyPhaseParamsPtr = NULL;
  PM_PowerProfile_FunctionalPhaseIDType count = 0, i = 0, j = 0;
  IO_ErrorType errorIO = IO_E_FAIL;

	/* Retrieve power profile info */
  if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_POWER_PROFILE, powerProfileIndex, &loader) )
  {
    count = (loader.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType) / sizeof(uint16_t)));
    if ( count > 0 )
    {
      energyPhaseParamsPtr = (const SF_PowerProfileDataEnergyPhaseType *)loader.Data;

      /* Retrieve each energy phases from setting file */
      for ( i = 0; i < count; ++i )
      {
        if ( SF_ENERGYPHASE_GET_FUNCID(energyPhaseParamsPtr) <= functionalPhaseID )
        {
          /* Check if the current PP is an active PP phase (e.g. Is present in APP buffer?) */
          for ( j = 0; j < CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM; ++j )
          {
            if ( (i + 1) == PM_PowerProfile_EnergyPhaseMap[j] )
            {
              /* Energy phase found */
              *energyPhaseIDPtr = (PM_PowerProfile_EnergyPhaseIDType)(j + 1);
              errorIO = IO_E_OK;
              j = (CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM - 1);  /* break */
            }
          }
        }
        else
        {
          i = (count - 1);  /* break */
        }

        /* Move to the next energy phases inside power profile description */
        ++energyPhaseParamsPtr;
      }
    }
  }

  return (errorIO);
}

/**

 */
static void PM_PowerProfile_ReloadPowerProfileInfo
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_EnergyPhaseIDType currentEnergyPhaseID,
  PM_PowerProfile_EnergyPhaseType **ppCurrentEnergyPhase,
  PM_PowerProfile_PowerProfileSizeType *pSize
)
{
	uint8_t  i;
	PM_PowerProfile_DurationType expectedDuration; /*[min]*/
	PM_PowerProfile_PeakPowerType peakPower;
	PM_PowerProfile_EnergyType energy;

	for (i=0; i<CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM; i++)
	{
		if (PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].energyPhaseID == currentEnergyPhaseID)
		{
			/* get current expected duration LE to native */
			expectedDuration = EndianU16_LToN(PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration);

			/* Check if the enPhase has infinite duration */
			if (expectedDuration != 0x07FF)
			{
				PM_PowerProfile_DurationType duration=PM_PowerProfile_Data.energyPhaseCounter[powerProfileID-1];
				if (expectedDuration <= duration)
				{
					expectedDuration = 1;
				}
				else
				{
					expectedDuration = (expectedDuration - duration);
				}

				peakPower = EndianU16_LToN(PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].peakPower);
				energy = (expectedDuration * peakPower) / 60; /*[Wh]*/
				/* Updates the fields */
				PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].energy = EndianU16_NToL(energy);
				PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration = EndianU16_NToL(expectedDuration);
			}

			*ppCurrentEnergyPhase = &(PM_PowerProfile_Data.energyPhase[powerProfileID-1][i]);
			*pSize = PM_PowerProfile_Data.powerProfileSize[powerProfileID-1] - (i * sizeof(PM_PowerProfile_EnergyPhaseType));
      break;
    } /*endIf*/
  } /*endFor*/
}


/**
Error management.
Only the first error is stored.
\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static PM_PowerProfile_ErrorType PM_PowerProfile_Error(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_ErrorType _ErrorCode)
{
	if(PM_PowerProfile_Data.status[powerProfileID-1].error == POWER_PROFILE_NO_ERR)
	{
		PM_PowerProfile_Data.status[powerProfileID-1].error = _ErrorCode;
	}
	return _ErrorCode;
}


/*
	Internal error management
*/
static void PM_PowerProfile_UpdateGlobalStatus(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_StatusFlagsType flags)
{
	PM_PowerProfile_Data.status[powerProfileID-1].flags |= flags;
}


/**
Computes the total duration of the cycle stored into the APP buffer.

\param[in]  PowerProfileID        requested power profile identifier;
\param[out] EnergyPhaseDuration calculated duration

\author   Pietro Loschiavo
\author   Andrea Zoppi
\date     05/06/2014
\version  1.1.0
*/
void PM_PowerProfile_TotalProfileDuration(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType *energyPhaseDurationPtr)
{
  const PM_PowerProfile_PowerProfileSizeType energyPhasesCount = (PM_PowerProfile_PowerProfileSizeType)(PM_PowerProfile_Data.powerProfileSize[powerProfileID-1] / sizeof(PM_PowerProfile_EnergyPhaseType));
  PM_PowerProfile_PowerProfileSizeType i = 0;

  *energyPhaseDurationPtr = 0;

  if ( energyPhasesCount <= CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM )
  {
    for ( i = 0; i < energyPhasesCount; i++ )
    {
      *energyPhaseDurationPtr += EndianU16_LToN(PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration);
    }
  }
}


/**
Computes the total energy consumption of the cycle stored into the APP buffer.

\param[in]  PowerProfileID        requested power profile identifier;
\param[out] EnergyPhaseEnergyTotal calculated duration

\author   Pietro Loschiavo
\author   Andrea Zoppi
\date     05/06/2014
\version  1.1.0
*/
void PM_PowerProfile_TotalProfileEnergy(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType *energyPhaseEnergyTotalPtr)
{
  const PM_PowerProfile_PowerProfileSizeType energyPhasesCount = (PM_PowerProfile_PowerProfileSizeType)(PM_PowerProfile_Data.powerProfileSize[powerProfileID-1] / sizeof(PM_PowerProfile_EnergyPhaseType));
  PM_PowerProfile_PowerProfileSizeType i = 0;

  *energyPhaseEnergyTotalPtr = 0;

  if ( energyPhasesCount <= CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM )
  {
    for ( i = 0; i < energyPhasesCount; ++i )
    {
      *energyPhaseEnergyTotalPtr += EndianU16_LToN(PM_PowerProfile_Data.energyPhase[powerProfileID-1][i].energy);
    }
  }
}


IO_ErrorType PM_PowerProfile_GetModValue
(
  uint8_t mSetting,
  uint16_t index,
  PM_PowerProfile_ModifierValueType modBase,
  LIB_Fixed_ValueType *modResultPtr
)
{
  Loader_Struct loader = { NULL, 0 };
  const uint8_t *rawPtr = NULL;
  PM_PowerProfile_ModTab_AlphaIndexType alphaIndex = 0;
  PM_PowerProfile_ModTab_ModIndexType modIndex = 0;
  LIB_Fixed_ValueType modValue = LIB_FIXED_VALUESTRUCT_INIT(0, 0);
  LIB_Fixed_ValueType baseValue = LIB_FIXED_VALUESTRUCT_INIT(0, 0);
  LIB_Fixed_ValueType alphaValue = LIB_FIXED_VALUESTRUCT_INIT(0, 0);
  LIB_Fixed_ValueType currentValue = LIB_FIXED_VALUESTRUCT_INIT(0, 0);
  LIB_Fixed_ValueType finalValue = LIB_FIXED_VALUESTRUCT_INIT(0, 0);
  uint8_t modLogic = 0;
  uint8_t count = 0, i = 0;
  IO_ErrorType errorIO = IO_E_OK;

  *modResultPtr = LIB_Fixed_Const_ZERO;

  if ( 0xFFFF == index )
  {
    /* Missing modifiers table, just keep the base value */
    LIB_Fixed_FromInt(modBase, 0, modResultPtr);
  }
  else if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_APP_MODIFIERS,  index, &loader) )
  {
    rawPtr = loader.Data;

    /* Apply alpha_0 to the base value */
    LIB_Fixed_FromInt(modBase, 0, &baseValue);
    alphaIndex = ((const PM_PowerProfile_ModTab_Level1HeaderType *)rawPtr)->alfa0VarPos;
    if ( (alphaIndex & 0x3F) < 0x3F )  /* 6 bits long */
    {
      PM_PowerProfile_Data.alphaValueGet(alphaIndex, &alphaValue);
      LIB_Fixed_Mul(baseValue, alphaValue, &currentValue);
      baseValue = currentValue;
    }

    /* Calculate number of elements present in table Level 1 table */
    /* Note: all sizes must be aligned to words */
    count = ((loader.DataLenght - (sizeof(PM_PowerProfile_ModTab_Level1HeaderType) / sizeof(uint16_t))) / (sizeof(PM_PowerProfile_ModTab_Level1EntryType) / sizeof(uint16_t)));

    /* Initialize calculation variables */
    modLogic = ((const PM_PowerProfile_ModTab_Level1HeaderType *)rawPtr)->ML;
    switch ( (PM_PowerProfile_ModTab_ModLogicType)modLogic )
    {
    case PM_PowerProfile_ModTab_ModLogicType_MAX:
    {
      finalValue = LIB_Fixed_Const_MIN;  /* Start from the very minimum */
      break;
    }
    case PM_PowerProfile_ModTab_ModLogicType_MIN:
    {
      finalValue = LIB_Fixed_Const_MAX;  /* Start from the very maximum */
      break;
    }
    case PM_PowerProfile_ModTab_ModLogicType_SUM:
    {
      finalValue = LIB_Fixed_Const_ZERO; /* Accumulator is zero */
      break;
    }
    default:
    {
      /* Unsupported modifier logic, just skip modifiers */
      count = 0;
      errorIO = IO_E_FAIL;
      break;
    }
    }

    /* Iterate through table entries */
    rawPtr += sizeof(PM_PowerProfile_ModTab_Level1HeaderType);
    for ( i = 0; i < count; ++i )
    {
      /* Get Alpha Parameter from Application Layer */
      alphaValue = LIB_Fixed_Const_ONE;
      alphaIndex = ((const PM_PowerProfile_ModTab_Level1EntryType *)rawPtr)->alfaVarValPos;
      if ( (alphaIndex & 0x3F) < 0x3F )  /* 6 bits long */
      {
        PM_PowerProfile_Data.alphaValueGet(alphaIndex , &alphaValue);
      }

      /* Apply alpha_i to the modifier value */
      modValue = LIB_Fixed_Const_ONE;
      modIndex = ((const PM_PowerProfile_ModTab_Level1EntryType *)rawPtr)->TableIndex;
      if ( modIndex < 0xFFFF )
      {
        PM_PowerProfile_ModTab_GetValue(mSetting, modIndex, (PM_PowerProfile_ModifierValueType *)&modValue.fractional);
        LIB_Fixed_FromInt(modValue.fractional, 0, &modValue);
      }
      LIB_Fixed_Mul(modValue, alphaValue, &currentValue);

      /* Apply the modifier logic */
      switch ( (PM_PowerProfile_ModTab_ModLogicType)modLogic )
      {
      case PM_PowerProfile_ModTab_ModLogicType_MAX:
      {
        if ( LIB_Fixed_Cmp(currentValue, finalValue) > 0 )
        {
          finalValue = currentValue;
        }
        break;
      }
      case PM_PowerProfile_ModTab_ModLogicType_MIN:
      {
        if ( LIB_Fixed_Cmp(currentValue, finalValue) < 0 )
        {
          finalValue = currentValue;
        }
        break;
      }
      case PM_PowerProfile_ModTab_ModLogicType_SUM:
      {
        LIB_Fixed_Add(finalValue, currentValue, &finalValue);
        break;
      }
      default:
      {
        /* Unsupported modifier logic */
        i = (count - 1);  /* break */
        break;
      }
      }

      /* Move to the next item */
      rawPtr += sizeof(PM_PowerProfile_ModTab_Level1EntryType);
    }

    /* Apply accumulator to the base value */
    LIB_Fixed_Add(baseValue, finalValue, modResultPtr);
  }
  else
  {
    /* Error loading data */
    errorIO = IO_E_FAIL;
  }
  return (errorIO);
}


IO_ErrorType PM_PowerProfile_CalcDurationPartial
(
  LIB_Fixed_ValueType durationValue,
  PM_PowerProfile_DurationType *resultPtr
)
{
  LIB_Fixed_FractionalType integral = LIB_Fixed_Round(durationValue);

  *resultPtr = ((integral >= 0) ? (PM_PowerProfile_DurationType)integral : 0);

  return (IO_E_OK);
}


IO_ErrorType PM_PowerProfile_CalcDurationTotal
(
  PM_PowerProfile_DurationType duration1,
  PM_PowerProfile_DurationType duration2,
  PM_PowerProfile_DurationType *resultPtr
)
{
  *resultPtr = (duration1 + duration2);

  return (IO_E_OK);
}


IO_ErrorType PM_PowerProfile_CalcPeakPower
(
  PM_PowerProfile_ModIndex peakPowerIndex,
  uint8_t mSetting,
  const SF_EnergyPhaseDataParamsType *sfEnergyPhaseAddPtr,
  PM_PowerProfile_PeakPowerType *peakPowerPtr
)
{
  PM_PowerProfile_PeakPowerType peakPowerBase;
  LIB_Fixed_ValueType fxPeakPowerTotal;
  IO_ErrorType errorIO;

  peakPowerBase = SF_ENERGYPHASEPARAMS_GET_MAXPOWER(sfEnergyPhaseAddPtr);
  errorIO = PM_PowerProfile_GetModValue(mSetting, peakPowerIndex, (PM_PowerProfile_ModifierValueType)peakPowerBase, &fxPeakPowerTotal);
  if ( IO_E_OK == errorIO )
  {
    *peakPowerPtr = LIB_Fixed_Round(fxPeakPowerTotal);
  }
  else
  {
    /* Modifier missing from SF, just return the base value */
    *peakPowerPtr = peakPowerBase;
  }

  errorIO = IO_E_OK;
  return (errorIO);
}


IO_ErrorType PM_PowerProfile_CalcEnergy
(
  PM_PowerProfile_DurationType durationOn,
  PM_PowerProfile_DurationType durationOff,
  const SF_PowerProfileDataEnergyPhaseType *energyPhaseParamsPtr,
  PM_PowerProfile_EnergyType *meanEnergyPtr
)
{
  IO_ErrorType error = IO_E_FAIL;
  PM_PowerProfile_EnergyType powerMeanRon = SF_ENERGYPHASE_GET_MEANPOWER_ON(energyPhaseParamsPtr);
  PM_PowerProfile_EnergyType powerMeanRoff = SF_ENERGYPHASE_GET_MEANPOWER_OFF(energyPhaseParamsPtr);
  uint32_t accumulator = ((((uint32_t)durationOn * powerMeanRon) + ((uint32_t)durationOff * powerMeanRoff)) / 60U);  /* [Wh] */

  if ( accumulator <= 0xFFFF )
  {
    *meanEnergyPtr = (PM_PowerProfile_EnergyType)accumulator;
    error = IO_E_OK;
  }
  else
  {
    *meanEnergyPtr = 0;
  }
  return (error);
}


PM_PowerProfile_ModTab_ExtendedModifierValueType PM_PowerProfile_ModTab_ExtendValue
(
  const void *datumPtr,
  PM_PowerProfile_ModTab_InOutType type
)
{
  PM_PowerProfile_ModTab_ExtendedModifierValueType extended = 0;
  switch ( type )
  {
  case PM_PowerProfile_ModTab_InOutType_UINT8:
  {
    extended = (PM_PowerProfile_ModTab_ExtendedModifierValueType)(*(const uint8_t *)datumPtr);
    break;
  }
  case PM_PowerProfile_ModTab_InOutType_UINT16:
  {
    extended = (PM_PowerProfile_ModTab_ExtendedModifierValueType)(*(const uint16_t *)datumPtr);
    break;
  }
  case PM_PowerProfile_ModTab_InOutType_INT8:
  {
    extended = (PM_PowerProfile_ModTab_ExtendedModifierValueType)(*(const int8_t *)datumPtr);
    break;
  }
  case PM_PowerProfile_ModTab_InOutType_INT16:
  {
    extended = (PM_PowerProfile_ModTab_ExtendedModifierValueType)(*(const int16_t *)datumPtr);
    break;
  }
  default:
  {
    extended = 0;
    break;
  }
  }
  return (extended);
}


IO_ErrorType PM_PowerProfile_ModTab_QueryTable
(
  const PM_PowerProfile_ModTab_Level2HeaderType *tableHeaderPtr,
  const PM_PowerProfile_ModifierValueType *inputPtr,
  PM_PowerProfile_ModifierValueType *outputPtr,
  bool_t secondLevel
)
{
  const uint8_t *inputRawPtr = NULL;
  const uint8_t *outputRawPtr = NULL;
  PM_PowerProfile_ModTab_ExtendedModifierValueType inputCurr = 0;
  PM_PowerProfile_ModTab_ExtendedModifierValueType inputPrev = 0;
  PM_PowerProfile_ModTab_ExtendedModifierValueType outputCurr = 0;
  PM_PowerProfile_ModTab_ExtendedModifierValueType outputPrev = 0;
  PM_PowerProfile_ModTab_ExtendedModifierValueType inputExt = PM_PowerProfile_ModTab_ExtendValue(inputPtr, PM_PowerProfile_ModTab_InOutType_INT16);
  uint8_t count = tableHeaderPtr->itemsNumber;
  uint8_t sval = (secondLevel ? tableHeaderPtr->SVal : PM_PowerProfile_ModTab_InOutType_UINT16);
  IO_ErrorType errorIO = IO_E_FAIL;

  if ( count > 0 )
  {
    /* Calculate the beginning of the input and output values arrays */
    inputRawPtr = ((const uint8_t *)tableHeaderPtr + sizeof(PM_PowerProfile_ModTab_Level2HeaderType));
    inputRawPtr += ((sizeof(uint16_t) - ((size_t)inputRawPtr & (sizeof(uint16_t) - 1))) & (sizeof(uint16_t) - 1));  /* Align to the nearest next word */
    outputRawPtr = ((const uint8_t *)inputRawPtr + ((size_t)PM_PowerProfile_ModTab_InOutSizes[tableHeaderPtr->SMod] * count));
    outputRawPtr += ((sizeof(uint16_t) - ((size_t)outputRawPtr & (sizeof(uint16_t) - 1))) & (sizeof(uint16_t) - 1));  /* Align to the nearest next word */

    /* Initialize variables in special cases */
    inputPrev = PM_PowerProfile_ModTab_ExtendValue(inputRawPtr, tableHeaderPtr->SMod);
    switch ( (PM_PowerProfile_ModTab_SearchModeType)tableHeaderPtr->SearchMode )
    {
    case PM_PowerProfile_ModTab_SearchModeType_DEFICIENCY:
    {
      outputPrev = 0;  /* Will be used as output if outside range */
      break;
    }
    case PM_PowerProfile_ModTab_SearchModeType_INTERPOLATION:
    {
      outputCurr = PM_PowerProfile_ModTab_ExtendValue(outputRawPtr, sval);
      if ( (0 == count) || (inputExt <= inputPrev) )
      {
        /* Single value (constant output) or below input range, continue to lower bound */
        count = 0;
      }
      else
      {
        /* Move cursor to the second point (the algorithm interpolates backwards) */
        inputPrev = PM_PowerProfile_ModTab_ExtendValue(inputRawPtr, tableHeaderPtr->SMod);
        outputPrev = outputCurr;
        inputRawPtr += PM_PowerProfile_ModTab_InOutSizes[tableHeaderPtr->SMod];
        outputRawPtr += PM_PowerProfile_ModTab_InOutSizes[sval];
        --count;
      }
      errorIO = IO_E_OK;  /* Interpolation always returns a value */
      break;
    }
    default:
    {
      break;
    }
    }

    /* Iterate through points */
    for ( /* count = count */; count > 0; --count )
    {
      inputCurr = PM_PowerProfile_ModTab_ExtendValue(inputRawPtr, tableHeaderPtr->SMod);
      outputCurr = PM_PowerProfile_ModTab_ExtendValue(outputRawPtr, sval);

      /* Compare with the reference and process */
      switch ( (PM_PowerProfile_ModTab_SearchModeType)tableHeaderPtr->SearchMode )
      {
      case PM_PowerProfile_ModTab_SearchModeType_EQUALITY:
      {
        if ( inputCurr == inputExt )
        {
          errorIO = IO_E_OK;
          count = 1;  /* break */
        }
        break;
      }
      case PM_PowerProfile_ModTab_SearchModeType_EXCESS:
      {
        if ( inputExt <= inputCurr )
        {
          errorIO = IO_E_OK;
          count = 1;  /* break */
        }
        break;
      }
      case PM_PowerProfile_ModTab_SearchModeType_DEFICIENCY:
      {
        if ( inputCurr <= inputExt )
        {
          errorIO = IO_E_OK;
        }
        else
        {
          outputCurr = outputPrev;
          count = 1;  /* break */
        }
        break;
      }
      case PM_PowerProfile_ModTab_SearchModeType_INTERPOLATION:
      {
        if ( inputExt <= inputCurr )
        {
          /* Interpolate between intermediate points */
          outputCurr = LIB_Fixed_Round(LIB_Fixed_LerpY(
            LIB_Fixed_FromClampedAccumulator(inputPrev, 0),
            LIB_Fixed_FromClampedAccumulator(outputPrev, 0),
            LIB_Fixed_FromClampedAccumulator(inputCurr, 0),
            LIB_Fixed_FromClampedAccumulator(outputCurr, 0),
            LIB_Fixed_FromClampedAccumulator((inputExt - inputPrev), 0)
          ));
          count = 1;  /* break */
        }
        break;
      }
      default:
      {
        count = 1;  /* break */
        break;
      }
      }

      /* Next input and output values */
      inputPrev = inputCurr;
      outputPrev = outputCurr;
      inputRawPtr += PM_PowerProfile_ModTab_InOutSizes[tableHeaderPtr->SMod];
      outputRawPtr += PM_PowerProfile_ModTab_InOutSizes[sval];
    }
  }

  *outputPtr = ((IO_E_OK == errorIO) ? (PM_PowerProfile_ModifierValueType)outputCurr : 0);
  return (errorIO);
}


IO_ErrorType PM_PowerProfile_ModTab_GetValue
(
  uint8_t mSetting,
  PM_PowerProfile_ModIndex modIndex,
  PM_PowerProfile_ModifierValueType *modValuePtr
)
{
  Loader_Struct loader = { NULL, 0 };
  PM_PowerProfile_ModifierValueType value = 0;
  IO_ErrorType errorIO = IO_E_FAIL;

  const PM_PowerProfile_ModTab_Level2HeaderType *tableHeaderPtr = NULL;

  if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_APP_MODIFIERS, modIndex, &loader) )
  {
    tableHeaderPtr = (const PM_PowerProfile_ModTab_Level2HeaderType *)loader.Data;
    PM_PowerProfile_Data.paramValueGet(tableHeaderPtr->ModVarValue1, &value);

    switch ( (PM_PowerProfile_ModTab_TableType)tableHeaderPtr->DT )
    {
    case PM_PowerProfile_ModTab_TableType_MONODIMENSIONAL:
    {
      /* Monodimensional table, query 2nd level data */
      errorIO = PM_PowerProfile_ModTab_QueryTable(tableHeaderPtr, &value, modValuePtr, TRUE);
      break;
    }
    case PM_PowerProfile_ModTab_TableType_BIDIMENSIONAL:
    {
      /* Bidimensional table, query 1st level then 2nd level data */
      errorIO = PM_PowerProfile_ModTab_QueryTable(tableHeaderPtr, &value, modValuePtr, FALSE);
      if ( IO_E_OK == errorIO )
      {
        modIndex = (uint16_t)*modValuePtr;
        if ( SF_OK == MSettingBasicLoader(mSetting, SF_PTR_APP_MODIFIERS, modIndex, &loader) )
        {
          tableHeaderPtr = (const PM_PowerProfile_ModTab_Level2HeaderType *)loader.Data;
          PM_PowerProfile_Data.paramValueGet(tableHeaderPtr->ModVarValue1 , &value);
          errorIO = PM_PowerProfile_ModTab_QueryTable(tableHeaderPtr, &value, modValuePtr, TRUE);
        }
      }
      break;
    }
    case PM_PowerProfile_ModTab_TableType_IMMEDIATE:
    {
      /* Immediate value */
      *modValuePtr = (PM_PowerProfile_ModifierValueType)tableHeaderPtr->ImmediateVal;
      errorIO = IO_E_OK;
      break;
    }
    default:
    {
      /* Unsupported data type */
      break;
    }
    }
  }

  if ( IO_E_OK != errorIO )
  {
    *modValuePtr = 0;
  }
  return (errorIO);
}


/**
\brief      Compute the estimated Time-To-End.

\param[in]  mSetting:
            Index of target mSetting.
\param[in]  compartmentIndex:
            Compartment index.
\param[in]  powerProfileIndex:
            Requested power profile index.
\param[in]  runningFunctionalPhaseID:
            ID of the current functional phase.
\param[in]  runningEnergyPhaseNoSpecialsElapsedTime:
            Time elapsed since the current energy phase has started,
            ignoring time contibution by special cycles.

\return     Estimated Time-To-End.
\retval 1   Dummy remaining time, if spending more than the expected time.
\retval 0   If not running a known energy phase.

\author     Andrea Zoppi
\date       07/04/2014
\version    1.1.1

\note
<pre>

  | run              | now                         | end
  v----------+-------v------------------+----------v
  | en.ph. 1 |   en.ph. 2    RUNNING    | en.ph. 3 |
  |          |<=== expectedDuration ===>|          |
  +----------^-------^------------------+----------^
             |       |<======== timeToEnd ========>|
             |<=====>| runningEnergyPhaseNoSpecialsElapsedTime

</pre>
*/
PM_PowerProfile_DurationType PM_PowerProfile_ComputeTimeToEnd
(
  uint8_t mSetting,
  PM_PowerProfile_CompartmentType compartmentIndex,
  PM_PowerProfile_ProfileIndexType powerProfileIndex,
  PM_PowerProfile_FunctionalPhaseIDType runningFunctionalPhaseID,
  PM_PowerProfile_DurationType runningEnergyPhaseNoSpecialsElapsedTime
)
{
  PM_PowerProfile_DurationType elapsedTime = 0, timeToEnd = 0;
  PM_PowerProfile_EnergyPhaseIDType runningEnergyPhaseID = 0;
  uint8_t energyPhasesCount = 0, i = 0;

  /* Find the running phase index */
  if ( (compartmentIndex < CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM) &&
       (IO_E_OK == PM_PowerProfile_GetEnergyPhaseID(mSetting, powerProfileIndex, runningFunctionalPhaseID, &runningEnergyPhaseID)) )
  {
    energyPhasesCount = (uint8_t)(PM_PowerProfile_Data.powerProfileSize[compartmentIndex] / (uint8_t)sizeof(PM_PowerProfile_EnergyPhaseType));
    if ( energyPhasesCount <= CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM )
    {
      for ( i = 0; i < energyPhasesCount; i++ )
      {
        if ( PM_PowerProfile_Data.energyPhase[compartmentIndex][i].energyPhaseID == runningEnergyPhaseID )
        {
          /* Get the time reduction with respect to the expected energy phase duration */
          /* elapsedTime = MIN(runningEnergyPhaseNoSpecialsElapsedTime, expectedDuration) */
          elapsedTime = EndianU16_LToN(PM_PowerProfile_Data.energyPhase[compartmentIndex][i].expectedDuration); /* ceiling */
          if ( elapsedTime > runningEnergyPhaseNoSpecialsElapsedTime )
          {
            elapsedTime = runningEnergyPhaseNoSpecialsElapsedTime;
          }

          /* Compute the expected time-to-end from the beginning of the running energy phase */
          for ( /* i = i */; i < energyPhasesCount; i++ )
          {
            timeToEnd += EndianU16_LToN(PM_PowerProfile_Data.energyPhase[compartmentIndex][i].expectedDuration);
          }

          /* Remove the elapsed time of the running energy phase from the time-to-end */
          if ( timeToEnd > elapsedTime )
          {
            timeToEnd -= elapsedTime;
          }
          else
          {
            timeToEnd = 1;  /* floor */
          }

          i = (energyPhasesCount - 1);  /* break */
        }
      }
    }
  }
  return (timeToEnd);
}


/**
\brief      Check if the power profile is ready for recomputation.
\deatil     Should be polled by the supervisor.

\param[in] recomputationRequest
            Asking for recomputation of the power profile.
\return
            The temporal filter is ready.

\author     Andrea Zoppi
\date       11/07/2014
\version    1.0.0
*/
bool_t PM_PowerProfile_IsRecomputationReady(bool_t recomputationRequest)
{
  return (recomputationRequest);
}


/**
\brief      Updates the Power Profile SID.
\deatil     Applies temporal filters.

\param[in]  compartmentIndex:
            Compartment index.
\param[in]  powerProfileHasChanged:
            The power profile changed since last call.

\author     Andrea Zoppi
\date       13/05/2014
\version    1.0.0
*/
void PM_PowerProfile_UpdateTemporalFilters
(
  PM_PowerProfile_CompartmentType compartmentIndex,
  bool_t powerProfileHasChanged
)
{
  PM_PowerProfile_SIDFilterDowncounterType oldRequestBurstDowncounter = PM_PowerProfile_TemporalFilters.requestBurstDowncounter;
  PM_PowerProfile_SIDFilterDowncounterType oldBusAccessDowncounter = PM_PowerProfile_TemporalFilters.busAccessDowncounter;
  PM_PowerProfile_PowerProfileSIDType powerProfileSID = 0;

  if ( compartmentIndex < CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM )
  {
    /* Decrement downcounters */
    if ( PM_PowerProfile_TemporalFilters.requestBurstDowncounter > 0 )
    {
      --PM_PowerProfile_TemporalFilters.requestBurstDowncounter;
    }
    if ( PM_PowerProfile_TemporalFilters.busAccessDowncounter > 0 )
    {
      --PM_PowerProfile_TemporalFilters.busAccessDowncounter;
    }

    /* Rearm the burst downcounter if regulations changed */
    if ( TRUE == powerProfileHasChanged )
    {
      PM_PowerProfile_TemporalFilters.requestBurstDowncounter = PP_SIDFILTER_CHANGEBURST_WINDOW;
      PM_PowerProfile_TemporalFilters.queuedBusAccessRequest = TRUE;
    }

    /* Increment SID when there are pending requests, both counters finished, and at least one has just triggered */
    if ( PM_PowerProfile_TemporalFilters.queuedBusAccessRequest &&
         ((0 == PM_PowerProfile_TemporalFilters.requestBurstDowncounter) && (0 == PM_PowerProfile_TemporalFilters.busAccessDowncounter) &&
          ((0 != oldRequestBurstDowncounter) || (0 != oldBusAccessDowncounter))) )
    {
      /* Start a new external bus window and clear the queued regulation flag */
      PM_PowerProfile_TemporalFilters.busAccessDowncounter = PP_SIDFILTER_BUSACCESS_WINDOW;
      PM_PowerProfile_TemporalFilters.queuedBusAccessRequest = FALSE;

      /* Increment the SID */
      UsePowerProfile();
      powerProfileSID = ((PM_PowerProfile_Data.powerProfileSID[compartmentIndex] + 1) & 7);
      if ( 0 == powerProfileSID )
      {
        powerProfileSID = 1;  /* 0 is reserved */
      }
      PM_PowerProfile_Data.powerProfileSID[compartmentIndex] = powerProfileSID;
      UnusePowerProfile();
    }
  }
}

#endif /* defined(CONFIG_PM_POWER_PROFILE_SUPPORT) && defined(CONFIG_PM_POWER_PROFILE_V2_0) */


/* *********** */
/* End of file */
/* *********** */

