/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/




/**
\n
\par        PM_PowerProfile.c
\n          it contains Power Profile 1.0 functionalities
\file       PM_PowerProfile.c
\ingroup    PM_PowerProfile
\date       Nov 8, 2010 
\version    00.00.76 
\author     Roberto Fioravanti, Andrea Ricci
\n
\n
\par\b      History:
\n
\n          00.00.03
\n          25/11/2010
\n          Andrea Ricci
\n          changes
\n          - hp added: number (power profiles) equal to number of target
\n          - reduced multiple PP storage down-to one PP
\n          - APP_GetStoredPowerProfile funtion added
\n          - APP_GetStoredPowerProfileID function added
\n          - APP_GetEnergyPhaseID function added
\n          - function bodies added
\n			
\n          Roberto Fioravanti
\n          changes
\n          - APP_PowerProfileType updated with: powerProfileSID, totalPowerProfilesNum
\n			- addition of APP_Init, APP_UpdateStatus, APP_BuildBmpFunctionsActingAsMofifiers, APP_RegulationsMonitor
\n			- updated methods APP_GetPowerProfileAnsw, APP_GetTotalPowerProfiles
\n 			- update of powerProfileSID in APP_GetPowerProfile
\n          - documentation
\n
\n
\n          00.00.76
\n          09/11/2010
\n          Roberto Fioravanti 
\n          changes 
\n          - file created
*/

/* ******** */
/* Includes */
/* ******** */

#include "PM_PowerProfile.h"
#include "LIB_Utilities.h"

#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT) && !defined(CONFIG_PM_POWER_PROFILE_V2_0)

/**********************/
/* Defines and Macros */
/**********************/
#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
#if !defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_ENPHASE_MAX_NUM)
#error "missing CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_ENPHASE_MAX_NUM definition"
#endif
#endif


/** assertion macro */
#define PM_POWER_PROFILE_ASSERT(Exp, ErrCode, ID)		if (!(Exp)) \
														{\
															if(PM_PowerProfile_Error(ID, ErrCode))\
															{\
																while(1){;}\
															}\
														}



/*PowerProfileStatus - flags*/
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

/**
macro to read bit-fields of SF_PowerProfileDataEnergyPhaseType
*/
#define SF_ENERGYPHASE_GET_PHASE_INDEX(_ptr)					MBGET(1, (SF_PowerProfileDataEnergyPhaseType*)(_ptr), index)
#define SF_ENERGYPHASE_GET_FUNCID(_ptr)							BF_8_GET(MBGET(1, (_ptr), initFuncPhase),0,7)
#define SF_ENERGYPHASE_GET_PHASE(_ptr)							((MBGET(2, _ptr, phase)))
#define SF_ENERGYPHASE_GET_MACROPHASE_ID(_ptr)					BF_16_GET(MBGET(2, _ptr, powerAndID), 13,3)
#define SF_ENERGYPHASE_GET_MEANPOWER(_ptr)						BF_16_GET((MBGET(2, _ptr, powerAndID)),0,13)
#define SF_ENERGYPHASE_GET_DURATION(_ptr)						BF_16_GET((MBGET(2, _ptr, duration)),0,11)


/**
macro to read bit-fields of SF_EnergyPhaseDataParamsType
 */
#define SF_ENERGYPHASEPARAMS_GET_MAXDELAY(_ptr)					BF_16_GET(MBGET(2, _ptr, maxDelay), 0,11)
#define SF_ENERGYPHASEPARAMS_GET_MAXPOWER(_ptr)					BF_16_GET(MBGET(2, _ptr, maxPower), 0,13)
#define SF_ENERGYPHASEPARAMS_GET_DURATIONMOD(_ptr)				MBGET(1, _ptr, durationModIdx)
#define SF_ENERGYPHASEPARAMS_GET_MAXPOWERMOD(_ptr)				MBGET(1, _ptr, maxPowerModIdx)
#define SF_ENERGYPHASEPARAMS_GET_ENERGYMOD(_ptr)				MBGET(1, _ptr, energyModIdx)


/*max energy value*/
#define PM_POWER_PROFILE_MAX_ENERGY_VALUE 						65535

/*max duration value*/
#define PM_POWER_PROFILE_MAX_DURATION_VALUE 					65535

/* main setting index */
#define MAIN_SF_INDEX											0
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/**
  Energy phase structure in Power Profile Data pointer, main parameters
  Structure definition does not reflect binary structure.
  @{
  */
typedef struct sfEnergyPhase_tag
{
	unsigned char index0;					/**< energy Phase index                 */
	unsigned char initFuncPhase0;			/**< initial functional phase           */
	unsigned char duration1;				/**< energy phase duration: lower 11 bits						*/
	unsigned char duration0;
	unsigned char powerAndID1;				/**< ID: 13-15*/
	unsigned char powerAndID0;				/**< power: 0-12*/
} SF_PowerProfileDataEnergyPhaseType;
/*@}*/


/**
  Energy phases, additional parameters (Energy Phase Data pointer)
  @{
  */
typedef struct sfEnergyPhaseAdd_tag
{
	unsigned char maxPower1;			/**< max power: 0-12*/
	unsigned char maxPower0;
	unsigned char maxDelay1;			/**< max delay: 0-10*/
	unsigned char maxDelay0;
	unsigned char durationModIdx0;		/**< duration modifier index */
	unsigned char maxPowerModIdx0;		/**< max power modifier index */
	unsigned char energyModIdx0;		/**< energy modifier index */
	unsigned char dummy;				/**< filler */
} SF_EnergyPhaseDataParamsType;
/*}@*/


/** */
typedef uint8_t PM_PowerProfile_RegBmpType;
/** */
typedef uint8_t PM_PowerProfile_AppliancePhaseType;
/** */
typedef uint8_t PM_PowerProfile_PowerProfileSizeType;
/** */
typedef uint8_t PM_PowerProfile_PowerProfileSIDType;

/** */
typedef uint32_t PM_PowerProfile_StatusFlagsType;



/** */
typedef struct PowerProfile_Error_s
{
	PM_PowerProfile_ErrorType				error;
	PM_PowerProfile_StatusFlagsType			flags;
}PowerProfile_StatusType;
/** */
typedef struct PowerProfile_s
{
	CALLBACK_PowerProfile_ModifierType		modifier;
	PM_PowerProfile_PowerProfileNumType		totalPowerProfilesNum;
	PM_PowerProfile_PowerProfileStatusType	profileStatus[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
	PM_PowerProfile_EnergyPhaseType			energyPhase[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM][CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM];
	PM_PowerProfile_DurationType			energyPhaseCounter[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
	PM_PowerProfile_PowerProfileSizeType	powerProfileSize[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
	PM_PowerProfile_PowerProfileSIDType		powerProfileSID[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
	PowerProfile_StatusType					status[CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM];
} PM_PowerProfile_DescriptorType;
/*********************************/
/* Variables                     */
/*********************************/
/* Extern */

/* Global */


/* Static */
static PM_PowerProfile_DescriptorType   PowerProfile_Data;

#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
static PM_PowerProfile_EnergyPhaseType PM_SpecialCycleEnergyData;
#endif /* defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT) */

static uint8_t PM_PPLinktoSF[CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
static void PM_PowerProfile_UpdateGlobalStatus(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_StatusFlagsType flags);
static PM_PowerProfile_ErrorType PM_PowerProfile_Error(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_ErrorType _ErrorCode);
static IO_ErrorType PM_PowerProfile_GetEnergyPhaseID
(
	uint8_t mSetting,
	PM_PowerProfile_FunctionalPhaseIDType functionalPhaseId,
	PM_PowerProfile_EnergyPhaseIDType* pEnergyPhaseID,
	PM_PowerProfile_ProfileIndexType _Index
);

static void PM_PowerProfile_ReloadPowerProfileInfo
(
	PM_PowerProfile_ProfileIDType 		powerProfileID,
	PM_PowerProfile_EnergyPhaseIDType	currentEnergyPhaseID,
	PM_PowerProfile_EnergyPhaseType		**ppCurrentEnergyPhase,
	uint16_t               				*pSize
);

#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
static IO_ErrorType PM_PowerProfile_GetPowerProfileSpecialCycle ( PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_ProfileIndexType SpecialCycleIndex,
  PM_PowerProfile_EnergyPhaseType * pEnergyPhase);

static IO_ErrorType PM_PowerProfile_UpdatePowerProfileWithSpecialCycle ( PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_EnergyPhaseType SpecialCycleEnergyPhase,
  uint8_t NAntiXtoAdd,
  PM_PowerProfile_ProfileIDType _EnPhaseID);
#endif /* defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT) */
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
	memset(&PowerProfile_Data, 0, sizeof(PowerProfile_Data));
}

/**
Sets the function for computation of values of the modifiers
\param[in] func function pointer
\author     Roberto Fioravanti
\date       06/08/2012
\version    1.0.0
*/
void PM_PowerProfile_SetModifierFunc(CALLBACK_PowerProfile_ModifierType func)
{
	PowerProfile_Data.modifier = func;
}

/**
	Returns the maximum number of managed power profile
\author     Roberto Fioravanti
\date       06/08/2012
\version    1.0.0
*/
PM_PowerProfile_PowerProfileNumType PM_PowerProfile_GetPowerProfilesNum(void)
{
	return (PM_PowerProfile_PowerProfileNumType)CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM;
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
PM_PowerProfile_PowerProfileStatusType PM_PowerProfile_UpdatePowerProfileStatus(
											PM_PowerProfile_CompartmentType			_Compartment,
											PM_PowerProfile_ProfileIndexType		_Index,
											PM_PowerProfile_PhaseType				_Phase,
											uint8_t 								mSetting,
											bool8_t 									_Clear,
											PM_PowerProfile_PhaseDelayTransitionType transition,
											PM_PowerProfile_PowerProfileStatusType 	status
											)
{
	PM_PowerProfile_PowerProfileStatusType appStatus = 0;
	PM_PowerProfile_EnergyPhaseIDType energyPhID = 0;

	if (_Clear == TRUE) {
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
		if(PM_PowerProfile_GetEnergyPhaseID(mSetting, _Phase, &energyPhID, _Index) == IO_E_OK)
		{
			appStatus = energyPhID & PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK;
			appStatus |= ((PowerProfile_Data.powerProfileSID[_Compartment] << PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_OFFS) & PM_POWER_PROFILE_STATUS__POWER_PROFILE_SID_MSK);
			/* update the transition bit */
			appStatus |= ((transition!=0)?PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK:0);
		}
		else
		{
			appStatus=0;
		}
	}

	PowerProfile_Data.profileStatus[_Compartment]=appStatus;

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
IO_ErrorType	PM_PowerProfile_GetEnergyPhaseMaxActivationDelay
							(
								PM_PowerProfile_ProfileIDType 			powerProfileID,
								PM_PowerProfile_ProfileIndexType 		_Index,
								PM_PowerProfile_FunctionalPhaseIDType	functionalPhaseId,
								uint8_t 								mSetting,
								bool_t* 								pNewEnergyPhase,
								uint16_t* 								pMaxActivationDelay
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

	bl = MSettingBasicLoader(	mSetting,
								(uint16_t)SF_PTR_POWER_PROFILE,
								_Index,
								&sfData);

#if 0 /* CYCLE_DOWNLOAD */
	bl = BasicLoader(
				(uint16_t)SF_PTR_POWER_PROFILE,
				_Index,
				&sfData);
#endif /* CYCLE_DOWNLOAD */
	if((bl != SF_OK))
	{
		return IO_E_FAIL;
	}
	else
	{
		/* is there an energy phase? */
		energyPhaseCount = sfData.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType)/sizeof(uint16_t));

		if(energyPhaseCount == 0)
		{
			/* the current power profile does not include any energy phase !?! */
			PM_PowerProfile_UpdateGlobalStatus(powerProfileID, PM_POWER_PROFILE_STATUS_FLAGS__NO_ENERGY_PHASE);
			return IO_E_FAIL;
		}
		else
		{
			pPPData = sfData.Data;

			/* retrieve each energy phases from sf.. */
			for (i=0; i<energyPhaseCount; i++)
			{
				psfEnergyPhase = (SF_PowerProfileDataEnergyPhaseType*)((void*)pPPData);
				/* hp:  functional phases in monotonic order*/
				if (SF_ENERGYPHASE_GET_FUNCID(psfEnergyPhase) == functionalPhaseId)
				{
					*pNewEnergyPhase = TRUE;
					break;
				}
				/* move to the next energy phases inside power profile description */
				pPPData += sizeof(SF_PowerProfileDataEnergyPhaseType);
			}

			if (*pNewEnergyPhase == TRUE)
			{

				bl = MSettingBasicLoader(mSetting,
										(uint16_t)SF_PTR_ENERGY_PHASE,
										SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase),
										&sfData);
#if 0 /* CYCLE_DOWNLOAD */
				bl = BasicLoader(
							(uint16_t)SF_PTR_ENERGY_PHASE,
							SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase),
							&sfData);
#endif /* CYCLE_DOWNLOAD */

				if(bl != SF_OK)
				{
					/* error loading power profile index data */
					return IO_E_FAIL;
				}
				else
				{
					if (sfData.DataLenght == (sizeof(SF_EnergyPhaseDataParamsType)/sizeof(uint16_t)))
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
		PM_PowerProfile_ProfileIDType 		powerProfileID,
		PM_PowerProfile_DurationType 		duration)
{
	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= powerProfileID) && (powerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, powerProfileID)
	PowerProfile_Data.energyPhaseCounter[powerProfileID-1] = duration;
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
							PM_PowerProfile_ProfileIndexType _Index,
							PM_PowerProfile_ProfileIDType 	powerProfileID,
							PM_PowerProfile_CycleType cycle,
							uint8_t mSetting,
							PM_PowerProfile_EnergyPhaseType** ppFirstEnergyPhase,
							PM_PowerProfile_PowerProfileSizeType* pSize
							)
{
	Loader_Struct sfData={0};
	/* pointer to energy phase data struct */
	SF_PowerProfileDataEnergyPhaseType* psfEnergyPhase = 0;
	uint8_t* pPPData = 0;
	/* pointer to energy phase params struct */
	SF_EnergyPhaseDataParamsType* psfEnergyPhaseAdd;

	uint8_t energyPhaseCount = 0;
	uint8_t actualEnergyPhaseCount = 0;
	uint8_t i;
	uint8_t result;
	/* pointer to energy phase descriptor*/
	PM_PowerProfile_EnergyPhaseType* pEnergyPhase;
	uint8_t swEnergyPhaseIns;

	PM_PowerProfile_ModifierValueType value = 0;

	/* */
	uint8_t powerProfileSID;

	PM_PowerProfile_MacroPhaseIDType 	macroID;
	/* phase energy data local variables for computation in native format */
	PM_PowerProfile_DurationType		duration;
	PM_PowerProfile_EnergyType			energy;
	PM_PowerProfile_PeakPowerType		peakpower;
    /* retrieve power profile info */


	if ( SF_OK != MSettingBasicLoader(
										mSetting,
										(uint16_t)SF_PTR_POWER_PROFILE,
										_Index,
										&sfData))
    {
    	/* error loading power profile index data */
    	return IO_E_FAIL;
    } else
    {
		energyPhaseCount = sfData.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType)/sizeof(uint16_t));
		/* is there an energy phase? */
		if (energyPhaseCount)
		{
			pPPData = sfData.Data;

			/* retrieve each energy phases from sf.. */
			UsePowerProfile();
			for (i=0; i<energyPhaseCount; i++)
			{
				/**/
				swEnergyPhaseIns = 1;
				/* point to current energy phase of the module descriptor*/
				pEnergyPhase = &(PowerProfile_Data.energyPhase[powerProfileID-1][actualEnergyPhaseCount]);
				/* point to setting data */
				psfEnergyPhase = (SF_PowerProfileDataEnergyPhaseType*)((void*)pPPData);
				/* data retrieve in native format for computation*/
				pEnergyPhase->energyPhaseID		= (PM_PowerProfile_EnergyPhaseIDType)(actualEnergyPhaseCount+1);
				macroID	=	SF_ENERGYPHASE_GET_MACROPHASE_ID(psfEnergyPhase);
				duration=	SF_ENERGYPHASE_GET_DURATION(psfEnergyPhase);
				energy	=	SF_ENERGYPHASE_GET_MEANPOWER(psfEnergyPhase);

				PM_PPLinktoSF[actualEnergyPhaseCount] = i + 1;

				if ( SF_OK != MSettingBasicLoader(	mSetting,
													(uint16_t)SF_PTR_ENERGY_PHASE,
													SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase),
													&sfData))
#if 0 /* CYCLE_DOWNLOAD */
				if ( SF_OK != BasicLoader(
										(uint16_t)SF_PTR_ENERGY_PHASE,
										SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase),
										&sfData))
#endif /* CYCLE_DOWNLOAD */
				{
					/* error loading power profile index data */
					UnusePowerProfile();
					return IO_E_FAIL;
				} else
				{
					if (sfData.DataLenght == (sizeof(SF_EnergyPhaseDataParamsType)/sizeof(uint16_t)))
					{
						/*init pointer to energy phase params */
						psfEnergyPhaseAdd = (SF_EnergyPhaseDataParamsType*)(((void*)sfData.Data));
						/*power*/
						peakpower = SF_ENERGYPHASEPARAMS_GET_MAXPOWER(psfEnergyPhaseAdd);

						/* modify energy phase parameters by means of modifiers */

						/* init modifier */
						value = 0;
						if(PowerProfile_Data.modifier != 0)
						{
							
						/* expected duration modifier*/

						result = (PowerProfile_Data.modifier)(SF_ENERGYPHASEPARAMS_GET_DURATIONMOD(psfEnergyPhaseAdd), &value);
						/*
						TODO : not needed if 'value' is initialized
						if (result == MOD_DONT_CARE || result == S_FALSE)
						{
							modifier = 0;
						}
						else
						{

						}
						*/

						if ( (int16_t)( duration + value ) > 0 )
						{
						  duration += value;
						}
						else
						{
						  duration = 0;
						}

						if (duration == 0)
						{
							swEnergyPhaseIns = FALSE;
						}
						else
						{
							/* energy modifier*/
							value = 0;
							result = (PowerProfile_Data.modifier)(SF_ENERGYPHASEPARAMS_GET_ENERGYMOD(psfEnergyPhaseAdd), &value);
							/*
							 * TODO : not needed if 'value' is initialized
							if (result == MOD_DONT_CARE || result == S_FALSE)
							{
								modifier = 0;
							}*/
							energy += (uint16_t)value;

							/* peak power modifier*/
							value = 0;
							result = (PowerProfile_Data.modifier)(SF_ENERGYPHASEPARAMS_GET_MAXPOWERMOD(psfEnergyPhaseAdd), &value);
							/*
							 * TODO
							result = GetModifierValue(psfEnergyPhaseAdd->maxPowerModIdx, &modifier);
							* TODO : not needed if 'value' is initialized
							if (result == MOD_DONT_CARE || result == S_FALSE)
							{
								modifier = 0;
							}
							*/
							peakpower += (uint16_t)value;
							}
						}
					} else
					{
						/* error loading energy phase data */
						UnusePowerProfile();
						return IO_E_FAIL;
					}
				}
				/*fill protocol struct with LE format */
				pEnergyPhase->macroPhaseID = macroID;
				energy = (energy * duration)/60; /*Wh*/
				pEnergyPhase->energy = EndianU16_NToL(energy);
				pEnergyPhase->expectedDuration = EndianU16_NToL(duration);
				pEnergyPhase->peakPower = EndianU16_NToL(peakpower);
				pEnergyPhase->maxActivationDelay = EndianU16_NToL(SF_ENERGYPHASEPARAMS_GET_MAXDELAY(psfEnergyPhaseAdd));

				/* move to the next energy phases inside power profile description */
				pPPData += sizeof(SF_PowerProfileDataEnergyPhaseType);
				if (swEnergyPhaseIns == 1)
				{
					actualEnergyPhaseCount++;

					if ( actualEnergyPhaseCount < ( CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM ))
					{
					  /* Do nothing */
					}
					else
					{
						i = energyPhaseCount; /* Exit loop */
					}
				}
			}
			UnusePowerProfile();
		}
		else
		{
			/* the current power profile does not include any energy phase !?! */
			return IO_E_FAIL;
		}
    }

    *ppFirstEnergyPhase = &(PowerProfile_Data.energyPhase[powerProfileID-1][0]);
    *pSize = actualEnergyPhaseCount * sizeof(PM_PowerProfile_EnergyPhaseType);
    PowerProfile_Data.powerProfileSize[powerProfileID-1] = actualEnergyPhaseCount * sizeof(PM_PowerProfile_EnergyPhaseType);

	powerProfileSID = (PowerProfile_Data.powerProfileSID[powerProfileID-1] + 1) % 8;
	if (powerProfileSID == 0)
	{
		powerProfileSID++;
	}
	PowerProfile_Data.powerProfileSID[powerProfileID-1] = powerProfileSID;

	return IO_E_OK;
}


/*
	Forces reloading of information because interpreter is running.
	E.g. it allows the update the remaining duration
	\param[in]	_PowerProfileID	power profile ID

	\author     Roberto Fioravanti
	\date       07/08/2012
	\version    1.0.0
*/
void PM_PowerProfile_SetForceReload ( PM_PowerProfile_ProfileIDType _PowerProfileID)
{
	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= _PowerProfileID) && (_PowerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, _PowerProfileID)
	PowerProfile_Data.status[_PowerProfileID-1].flags |= PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD;
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
	PowerProfile_Data.status[_PowerProfileID-1].flags &= ~(PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD);
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
									PM_PowerProfile_EnergyPhaseType** ppFirstEnergyPhase,
									PM_PowerProfile_PowerProfileSizeType* pSize
								)
{
	PM_PowerProfile_EnergyPhaseIDType currentEnPhaseID = 1;

	PM_POWER_PROFILE_ASSERT((CONFIG_PM_POWER_PROFILE_COMPARTMENTS_NUM >= _PowerProfileID) && (_PowerProfileID>=1), POWER_PROFILE_EXCEEDINGCOMPARTMENT, _PowerProfileID)

    /* if the PowerProfile is already running --> reload the the PowerProfile info */
    if (PowerProfile_Data.status[_PowerProfileID-1].flags & PM_POWER_PROFILE_STATUS_FLAGS__FORCERELOAD)
    {
		currentEnPhaseID = (PowerProfile_Data.profileStatus[_PowerProfileID-1] & PM_POWER_PROFILE_STATUS__ENERGY_PHASE_ID_MSK);

		/* Reload the PowerProfile info */
		PM_PowerProfile_ReloadPowerProfileInfo(_PowerProfileID, currentEnPhaseID, ppFirstEnergyPhase, pSize);
    }
    else
    {
		*ppFirstEnergyPhase = &(PowerProfile_Data.energyPhase[_PowerProfileID-1][0]);
		*pSize = PowerProfile_Data.powerProfileSize[_PowerProfileID-1];
    }
}

#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
IO_ErrorType PM_PowerProfile_AddEnergyPhaseInfoSpecialCycle (PM_PowerProfile_ProfileIDType powerProfileID,
                                                  uint8_t NAntiXtoAdd,
                                                  PM_PowerProfile_ProfileIndexType SpecialCycleIndex,
                                                  PM_PowerProfile_ProfileIDType currentEnPhaseID )
{

	IO_ErrorType funResult;
	uint8_t powerProfileSID;

	funResult = PM_PowerProfile_GetPowerProfileSpecialCycle(powerProfileID,SpecialCycleIndex,&PM_SpecialCycleEnergyData);
	if ( IO_E_OK != funResult )
	{
		return (IO_E_FAIL);
	}


	/* Update APP_Data with Special Cycle Energy Info */
	UsePowerProfile();
	funResult = PM_PowerProfile_UpdatePowerProfileWithSpecialCycle(powerProfileID, PM_SpecialCycleEnergyData, NAntiXtoAdd, currentEnPhaseID);
	UnusePowerProfile();
	if ( IO_E_OK != funResult )
	{
		return (IO_E_FAIL);
	}

	/* Update SID to communicate the change of APP_Data */
	powerProfileSID = (PowerProfile_Data.powerProfileSID[powerProfileID-1] + 1) % 8;
	if (powerProfileSID == 0)
	{
		powerProfileSID++;
	}
	PowerProfile_Data.powerProfileSID[powerProfileID-1] = powerProfileSID;

	return (IO_E_OK);
}


/**
Retrieves total energy .
\param [in]		PowerProfileID        requested power profile identifier;
\param [out]	pEnergyPhaseSum       energy [wh]
\author 	Andrea Emili
\date		19/03/2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Andrea Emili
*/
void PM_PowerProfile_GetTotalProfileEnergy(PM_PowerProfile_ProfileIDType powerProfileID, uint16_t* pEnergyPhaseSum)
{
	uint16_t size = 0x00;
	uint8_t i = 0;
	if(pEnergyPhaseSum)
	{
		*pEnergyPhaseSum = 0;
		size = (PowerProfile_Data.powerProfileSize[powerProfileID-1]/CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM);

		for(i = 0; i < size; i++)
		{
			*pEnergyPhaseSum += EndianU16_LToN(PowerProfile_Data.energyPhase[powerProfileID-1][i].energy);
		}
	}
}

#endif /* defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT) */

/* Local Function bodies */
/**
  Returns the energy phase ID (*pEnergyPhaseID) which matches the
  requested functional phase (functionalPhaseId) of selected power profile (powerProfileID).
  \param[in]  mSetting				setting to search in;
  \param[in]  _Index        		power profile index in setting file
  \param[in]  functionalPhaseId     requested functional phase;
  \param[out] pEnergyPhaseID        pointer to returned energy phase id;

  \return     status of operations.

  \author     Andrea Ricci
  \date       25/11/2010
  \version    1.0.0
  \todo       the routine can be faster in some cases (actually frequently..?) if functional
              phases are locally stored during PP computation.
*/
static IO_ErrorType PM_PowerProfile_GetEnergyPhaseID
(
	uint8_t 								mSetting,
	PM_PowerProfile_FunctionalPhaseIDType	functionalPhaseId,
	PM_PowerProfile_EnergyPhaseIDType*		pEnergyPhaseID,
	PM_PowerProfile_ProfileIndexType		_Index
)
{
  Loader_Struct sfData = {0};
  SF_PowerProfileDataEnergyPhaseType* psfEnergyPhase;
  uint8_t* pPPData;
  uint8_t energyPhaseCount = 0;
  uint8_t i;
  uint8_t j;
  uint8_t swFpFound = 0;
  PM_PowerProfile_EnergyPhaseIDType lastFpFound = 0;

	/* retrieve power profile info */


    if ( SF_OK != MSettingBasicLoader(mSetting,
    								(uint16_t)SF_PTR_POWER_PROFILE,
    								_Index,
    								&sfData))
    {
    	/* error loading power profile index data */
    	return IO_E_FAIL;
    } else
    {
    	energyPhaseCount = sfData.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType)/sizeof(uint16_t));
    	/* is there at least an energy phase? */
    	if (energyPhaseCount)
    	{
    		pPPData = sfData.Data;

    		/* retrieve each energy phases from sf.. */
    		for (i=0; i<energyPhaseCount; i++)
    		{
    			psfEnergyPhase = (SF_PowerProfileDataEnergyPhaseType*)((void*)pPPData);
    			/* hp: monotone functional phases */
    			if (SF_ENERGYPHASE_GET_FUNCID(psfEnergyPhase) <= functionalPhaseId)
    			{
    			  /* returns the energy phase ID */
    			  /*****/
            /* Check if the current PP is an active PP phase (E.g. Is present
             * in APP buffer?
             */
            for ( j=0; j < CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM; j++ )
            {
              if ( PM_PPLinktoSF[j] == (i + 1))
              {
                lastFpFound = (PM_PowerProfile_EnergyPhaseIDType)(j+1);
                swFpFound = 1;
                break;
              }
            }
            /*****/

            /* The code above replace this one */
    				/* lastFpFound = (PM_PowerProfile_EnergyPhaseIDType)(i+1); */
    			} else
    			{
    				break;
    			}
    			/* move to the next energy phases inside power profile description */
    			pPPData += sizeof(SF_PowerProfileDataEnergyPhaseType);
    		}
    		if (swFpFound == 1)
    		{
    			*pEnergyPhaseID = lastFpFound;
    			return IO_E_OK;
    		}
    		else
    		{
    		  /* No matchin Power Phases */
    		  return IO_E_FAIL;
    		}
    	} else
    	{
    		/* the current power profile does not include any energy phase !?! */
    		return IO_E_FAIL;
    	}
	}

	/* reaching this point (see for cycle/if check above) means no functionalPhaseId match */
	return IO_E_FAIL;
}

/**

 */
static void PM_PowerProfile_ReloadPowerProfileInfo
					(
						PM_PowerProfile_ProfileIDType 		powerProfileID,
						PM_PowerProfile_EnergyPhaseIDType	currentEnergyPhaseID,
						PM_PowerProfile_EnergyPhaseType		**ppCurrentEnergyPhase,
						uint16_t               *pSize
					)
{
	uint8_t  i;
	PM_PowerProfile_DurationType expectedDuration; /*[min]*/
	PM_PowerProfile_PeakPowerType peakPower;
	PM_PowerProfile_EnergyType energy;

	for (i=0; i<CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM; i++)
	{
		if (PowerProfile_Data.energyPhase[powerProfileID-1][i].energyPhaseID == currentEnergyPhaseID)
		{
			/* get current expected duration LE to native */
			expectedDuration = EndianU16_LToN(PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration);

			/* Check if the enPhase has infinite duration */
			if (expectedDuration != 0x07FF)
			{
				PM_PowerProfile_DurationType duration=PowerProfile_Data.energyPhaseCounter[powerProfileID-1];
				if (expectedDuration <= duration)
				{
					expectedDuration = 1;
				}
				else
				{
					expectedDuration = (expectedDuration - duration);
				}

				peakPower = EndianU16_LToN(PowerProfile_Data.energyPhase[powerProfileID-1][i].peakPower);
				energy = (expectedDuration * peakPower) / 60; /*[Wh]*/
				/* Updates the fields */
				PowerProfile_Data.energyPhase[powerProfileID-1][i].energy = EndianU16_NToL(energy);
				PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration = EndianU16_NToL(expectedDuration);
			}

			*ppCurrentEnergyPhase = &(PowerProfile_Data.energyPhase[powerProfileID-1][i]);
			*pSize = PowerProfile_Data.powerProfileSize[powerProfileID-1] - (i * sizeof(PM_PowerProfile_EnergyPhaseType));
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
	if(PowerProfile_Data.status[powerProfileID-1].error == POWER_PROFILE_NO_ERR)
	{
		PowerProfile_Data.status[powerProfileID-1].error = _ErrorCode;
	}
	return _ErrorCode;
}

/*
	Internal error management
*/
static void PM_PowerProfile_UpdateGlobalStatus(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_StatusFlagsType flags)
{
	PowerProfile_Data.status[powerProfileID-1].flags |= flags;
}
/**
Calculate total duration on cycle stored in APP buffer.

\param[in]  PowerProfileID        requested power profile identifier;
\param[out] EnergyPhaseDuration calculated duration

\return     none

\author   Pietro Loschiavo
\date   14/04/2013
\version  1.0.0

\note

*/
void PM_PowerProfile_TotalProfileDuration(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType * EnergyPhaseDuration )
{
  uint16_t size = 0x00;
  uint8_t i = 0;


  *EnergyPhaseDuration = 0;

  size = (PowerProfile_Data.powerProfileSize[powerProfileID-1]/sizeof(PM_PowerProfile_EnergyPhaseType));

  if ( size > 0 )
  {
    for(i = 0; i < size; i++)
    {
      *EnergyPhaseDuration += EndianU16_LToN(PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration);
    }
  }
  else
  {
    *EnergyPhaseDuration = 0;
  }

}
/**
Calculate total energy on cycle stored in APP buffer.

\param[in]  PowerProfileID        requested power profile identifier;
\param[out] EnergyPhaseEnergyTotal calculated duration

\return     none

\author   Pietro Loschiavo
\date   14/04/2013
\version  1.0.0

\note

*/
void PM_PowerProfile_TotalProfileEnergy(PM_PowerProfile_ProfileIDType powerProfileID, PM_PowerProfile_EnergyType * EnergyPhaseEnergyTotal )
{
  uint16_t size = 0x00;
  uint8_t i = 0;


  *EnergyPhaseEnergyTotal = 0;

  size = (PowerProfile_Data.powerProfileSize[powerProfileID-1]/sizeof(PM_PowerProfile_EnergyPhaseType));

  if ( size > 0 )
  {
    for(i = 0; i < size; i++)
    {
      *EnergyPhaseEnergyTotal += EndianU16_LToN(PowerProfile_Data.energyPhase[powerProfileID-1][i].energy);
    }
  }
  else
  {
    *EnergyPhaseEnergyTotal = 0;
  }

}
#if defined(CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT)
static IO_ErrorType PM_PowerProfile_GetPowerProfileSpecialCycle ( PM_PowerProfile_ProfileIDType powerProfileID,
    PM_PowerProfile_ProfileIndexType SpecialCycleIndex,
    PM_PowerProfile_EnergyPhaseType * pEnergyPhase)
{

  Loader_Struct sfData;

  uint8_t energyPhaseCount = 0;

  SF_PowerProfileDataEnergyPhaseType* psfEnergyPhase;
  uint8_t* pPPData;

  SF_EnergyPhaseDataParamsType* psfEnergyPhaseAdd;

  if ( SF_OK != MSettingBasicLoader(
      MAIN_SF_INDEX,
      (uint16_t)SF_PTR_POWER_PROFILE,
      SpecialCycleIndex,
      &sfData))
  {
    /* error loading power profile index data */
    return IO_E_FAIL;
  }
  else
  {
    energyPhaseCount = sfData.DataLenght / (sizeof(SF_PowerProfileDataEnergyPhaseType)/sizeof(uint16_t));
    /* is there only PM_MAX_NUM_ENPHASE_SPCYCLE energy phase? */
    if ( CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_ENPHASE_MAX_NUM == energyPhaseCount)
    {
      pPPData = sfData.Data;

      /* retrieve energy phases from sf.. */
      psfEnergyPhase = (SF_PowerProfileDataEnergyPhaseType*)pPPData;

      pEnergyPhase->energyPhaseID = (CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_ENPHASE_MAX_NUM);
      pEnergyPhase->macroPhaseID = SF_ENERGYPHASE_GET_MACROPHASE_ID(psfEnergyPhase);
      pEnergyPhase->expectedDuration = SF_ENERGYPHASE_GET_DURATION(psfEnergyPhase);
      pEnergyPhase->energy = SF_ENERGYPHASE_GET_MEANPOWER(psfEnergyPhase);

      if ( SF_OK != MSettingBasicLoader(
          MAIN_SF_INDEX,
          (uint16_t)SF_PTR_ENERGY_PHASE,
          SF_ENERGYPHASE_GET_PHASE_INDEX(psfEnergyPhase),
          &sfData))
      {
        /* error loading power profile index data */
        return (IO_E_FAIL);
      }
      else
      {
        if (sfData.DataLenght == (sizeof(SF_EnergyPhaseDataParamsType)/sizeof(uint16_t)))
        {
          /*init pointer to energy phase params */
          psfEnergyPhaseAdd = (SF_EnergyPhaseDataParamsType*)(((void*)sfData.Data));
          pEnergyPhase->peakPower = SF_ENERGYPHASEPARAMS_GET_MAXPOWER(psfEnergyPhaseAdd);
        }
        else
        {
          /* error loading energy phase data */
          return (IO_E_FAIL);
        }
      }

      /* Calculate energy */
      pEnergyPhase->energy = (pEnergyPhase->expectedDuration * pEnergyPhase->energy) / 60;

      pEnergyPhase->expectedDuration = EndianU16_NToL(pEnergyPhase->expectedDuration);
      pEnergyPhase->peakPower = EndianU16_NToL(pEnergyPhase->peakPower);
      pEnergyPhase->energy = EndianU16_NToL(pEnergyPhase->energy);
      pEnergyPhase->maxActivationDelay = EndianU16_NToL(SF_ENERGYPHASEPARAMS_GET_MAXDELAY(psfEnergyPhaseAdd));

    }
    else
    {
      /* the current power profile does not include any energy phase !?! */
      return (IO_E_FAIL);
    }
  }

  return (IO_E_OK);
}

static IO_ErrorType PM_PowerProfile_UpdatePowerProfileWithSpecialCycle ( PM_PowerProfile_ProfileIDType powerProfileID,
    PM_PowerProfile_EnergyPhaseType SpecialCycleEnergyPhase,
    uint8_t NAntiXtoAdd,
    PM_PowerProfile_ProfileIDType _EnPhaseID)
{
  uint8_t  i;

  for (i=0; i<CONFIG_PM_POWER_PROFILE_ENERGY_PHASES_MAX_NUM; i++)
  {
      if (PowerProfile_Data.energyPhase[powerProfileID-1][i].energyPhaseID == _EnPhaseID)
      {
    	  PM_PowerProfile_EnergyType				energy;
    	  PM_PowerProfile_DurationType				expectedDuration;

    	  /* Update Energy */
    	  energy	=  (uint32_t)EndianU16_LToN( PowerProfile_Data.energyPhase[powerProfileID-1][i].energy) +
    			  ( (uint32_t)EndianU16_LToN( SpecialCycleEnergyPhase.energy ) * NAntiXtoAdd );

		  if (energy <= PM_POWER_PROFILE_MAX_ENERGY_VALUE )
		  {
			  PowerProfile_Data.energyPhase[powerProfileID-1][i].energy = EndianU16_NToL(energy);
		  }
		  else
		  {
			  PowerProfile_Data.energyPhase[powerProfileID-1][i].energy = EndianU16_NToL(PM_POWER_PROFILE_MAX_ENERGY_VALUE);
		  }


		  /* Update Expected Duration */
		  expectedDuration =  (uint32_t)EndianU16_LToN( PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration ) +
		          ( (uint32_t)EndianU16_LToN( SpecialCycleEnergyPhase.expectedDuration ) * NAntiXtoAdd );

		  if ((expectedDuration) <= PM_POWER_PROFILE_MAX_DURATION_VALUE )
		  {
			  PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration = EndianU16_NToL(expectedDuration);
		  }
		  else
		  {
			  PowerProfile_Data.energyPhase[powerProfileID-1][i].expectedDuration = EndianU16_NToL(PM_POWER_PROFILE_MAX_DURATION_VALUE);
		  }

		  /* Check and update (if needed) peakPower */
		  if ( EndianU16_LToN( PowerProfile_Data.energyPhase[powerProfileID-1][i].peakPower ) < EndianU16_LToN( SpecialCycleEnergyPhase.peakPower ))
		  {
			  PowerProfile_Data.energyPhase[powerProfileID-1][i].peakPower = SpecialCycleEnergyPhase.peakPower;
		  }

		  return (IO_E_OK);
      }
  }

  /* No matching PP in current AAP buffer */
  return (IO_E_FAIL);
}
#endif /*CONFIG_PM_POWER_PROFILE_SPECIAL_CYCLE_SUPPORT*/


/**
\brief      Compute the estimated Time-To-End.

\param[in]  powerProfileID:
            Requested power profile identifier.
\param[in]  runningEnergyPhaseID:
            ID of the current energy phase.
\param[in]  runningEnergyPhaseNoSpecialsElapsedTime:
            Time elapsed since the current energy phase has started,
            ignoring time contibution by special cycles.

\return     Estimated Time-To-End.
\retval 1   Dummy remaining time, if spending more than the expected time.
\retval 0   If not running a known energy phase.

\author     Andrea Zoppi
\date       25/03/2014
\version    1.0.1

\note
<pre>

  | run              | now                         | end
  v----------+-------v------------------+----------v
  | en.ph. 1 |   en.ph. 2    RUNNING    | en.ph. 3 |
  |          |<=== expectedDuration ===>|          |
  +----------^-------^------------------+----------^
             |       |<=========== tte ===========>|
             |<=====>| runningEnergyPhaseNoSpecialsElapsedTime

</pre>
*/
PM_PowerProfile_DurationType PM_PowerProfile_ComputeTimeToEnd
(
  PM_PowerProfile_ProfileIDType powerProfileID,
  PM_PowerProfile_EnergyPhaseIDType runningEnergyPhaseID,
  PM_PowerProfile_DurationType runningEnergyPhaseNoSpecialsElapsedTime
)
{
  const PM_PowerProfile_EnergyPhaseType *const energyPhases = PowerProfile_Data.energyPhase[powerProfileID-1];
  const uint8_t energyPhasesCount = (uint8_t)(PowerProfile_Data.powerProfileSize[powerProfileID-1] / sizeof(PM_PowerProfile_EnergyPhaseType));
  uint8_t runningPhase = energyPhasesCount, i = 0;
  PM_PowerProfile_DurationType tte = 0;

  /* Get the running phase index */
  for ( i = 0; i < energyPhasesCount; i++ )
  {
    if ( energyPhases[i].energyPhaseID == runningEnergyPhaseID )
    {
      runningPhase = i;
      i = (energyPhasesCount - 1); /* break */
    }
  }

  /* Compute only if the phase was found, otherwise return 0 */
  if ( runningPhase < energyPhasesCount )
  {
    /* Recompute the expected remaining time */
    for ( i = runningPhase; i < energyPhasesCount; i++ )
    {
      tte += energyPhases[i].expectedDuration;
    }

    /* Saturate the elapsed time of the running energy phase with the expected time */
    /* i.e.: considered_time = MAX(1, MIN(actual_time, nominal_time)) */
    if ( runningEnergyPhaseNoSpecialsElapsedTime <= energyPhases[runningPhase].expectedDuration )
    {
      if ( tte > runningEnergyPhaseNoSpecialsElapsedTime )
      {
        tte -= runningEnergyPhaseNoSpecialsElapsedTime;
      }
      else
      {
        tte = 1; /* Actually no more time left, but return 1 as dummy remaining time */
      }
    }
    else
    {
      if ( tte > energyPhases[runningPhase].expectedDuration )
      {
        tte -= energyPhases[runningPhase].expectedDuration;
      }
      else
      {
        tte = 1; /* Actually no more time left, but return 1 as dummy remaining time */
      }
    }
  }
  return (tte);
}


#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

/* *********** */
/* End of file */
/* *********** */

