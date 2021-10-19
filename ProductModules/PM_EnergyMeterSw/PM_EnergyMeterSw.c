/*
 Copyright (c) 2004-2011, Indesit Company, I.D.D..
 Tutti i diritti sono riservati. All rights reserved.
 */


/**
\Par    Software Energy Meter
\n
\n
\file     ${PM_EnergyMeterSw.c}
\ingroup  PM_EnergyMeterSw
\date     13-06-2011
\version  Platform2_00.00.12 (project baseline related to creation of this module)
\author   Intecs
\n
\n
\Par\b  History:
\n
\n    00.00.12
\n    13-06-2011
\n    developer Intecs
\n    changes First revision
\n
\n
\n    01.03.00 <baseline di progetto relativa alla versione precedente la corrente>
\n    gg/mm/aaaa <data in cui � stata realizzata >
\n    developer <progettista autore della modifica>
\n    changes <principali modifiche apportate>
\n    - change 1
\n    - change 2
\n
\n
\n
*/
/* ********** */
/* Includes   */
/* ********** */


#if defined(OS_EmbOS)
#include "RTOS.h"
#else
#include "OSEK.h" 
#include "SkeletonGlobal.h" 
#endif





/*lint -e537*/
#include "Config.h"
#include "callback.h"
#include "LIB_Utilities.h"
#include "PM_EnergyMeterSw.h"
#include "PM_EnergyMeterSwTable.h"
#include "OutputManagerExp.h"
/*lint +e537*/
#if defined(CONFIG_PM_ENERGY_METER_SW)



#if (EMS_V_PEAK_AVERAGE == 0)
#include <math.h>
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Indexes for loads */
#define m_ui8RLoadIndex           (m_sLoadIndex.resistor)
#define m_ui8MLoadIndex           (m_sLoadIndex.motor)
#define m_ui8TLoadIndex           (m_sLoadIndex.table)
/* Flags */
#define m_bAccuEnabled            (m_sFlags.accuEnabled)
#define m_bOneShot                (m_sFlags.oneShot)
#define m_ui8EnergyUpdateCnt      (m_sFlags.energyUpdateCnt)

#if (EMS_BUFFER_DOUBLE_USE == 0)
#define m_psVMeasAccCurr          (&m_sVMeasAcc)
#define m_psVMeasAccHold          (&m_sVMeasAcc)
#endif

/* Energy Meter Data: Algorithm Parameters */
#define ALGO_PARAMS_NOT_VALID()   ((m_sAlgoParams.Data == NULL) || (m_sAlgoParams.DataLenght < ALGO_PARAMS_WLEN))
#define g_ui16ALGO_PMAX           (*((uint16_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_PMAX])))
#define g_ui16ALGO_EMAX           (*((uint16_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_EMAX])))
#define g_ui16ALGO_KENERGY_MANT   (*((uint16_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_KENERGY_MANT])))
#define g_i8ALGO_KENERGY_EXP      (*((int8_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_KENERGY_EXP])))
#define g_ui8ALGO_TS              (*((uint8_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_TS])))
#define g_ui16ALGO_VMAX10         (*((uint16_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_VMAX10])))
#define g_ui16ALGO_NORM_RTN       (*((uint16_t*)&(m_sAlgoParams.Data[ALGO_PARAMS_OFFSET_NORM_RTN])))


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


typedef struct
{
 #if (EMS_V_PEAK_AVERAGE || EMS_V_RMS_AVERAGE)
  /* accumulator for normalized Vpeak values */
  uint32_t  ui32VPeakAcc;
 #endif

  /* accumulator for normalized Vpeak^2 values */
  uint32_t  ui32VPeak2Acc;

  uint8_t   ui8NSamples;
} sVMeasAcc_t;


typedef struct
{
  uint8_t   ui8Mask;      /* mask to perform control on active load */
  uint8_t*  pui8Status;   /* active load variable pointer: NULL = always active */
  uint16_t  ui16Gmant;    /* 1/R' (mantissa) */
  int8_t    i8Gexp;       /* 1/R' (exponent) */
} sRLoadParam_t;


typedef struct
{
  uint8_t   ui8Mask;      /* mask to perform control on active load */
  uint8_t*  pui8Status;   /* active load variable pointer: NULL = always active */
  uint16_t* pui16PVal;    /* pointer were retrieve the current value of power */
  uint32_t* pui32EVal;    /* pointer were retrieve the current value of energy */

} sMLoadParam_t;


typedef struct
{
  uint8_t   ui8Mask;      /* mask to perform control on active load */
  uint8_t*  pui8Status;   /* active load variable pointer: NULL = always active */
  stTable_t sTable;       /* load data table */
} sTLoadParam_t;


typedef struct
{
  uint8_t accuEnabled     : 1;  /* energy meter store and measurement activation */
  uint8_t oneShot         : 1;  /* one shot or continuous store and measurement */
  uint8_t energyUpdateCnt : 6;  /* counter for update energy period elapsed */
} sFlags_t;


typedef struct
{
  uint8_t resistor      : 2;  /* index for resistive load */
  uint8_t motor         : 2;  /* index for motor load */
  uint8_t table         : 4;  /* index for table load */
} sLoadIndex_t;


/* ****************** */
/* Exported variables */
/* ****************** */



/* Energy counter from start of life in [Wh] = [J]*3600 */
/*static*/ uint32_t b_ui32EnergyCounterWh;          /* SI Backup */

/* Accumulator for energy in a second = [Ws] = [J] */
/*static*/ uint16_t b_ui16EnergyCounterWhAcc;       /* SI Backup */

#if EMS_USE_ELAPSED_TIMER
/* Time in seconds elapsed from start of life */
static uint32_t b_ui32TimeElapsedSinceStart;    /* NO Backup */
#endif

/* ****************** */
/* Local variables    */
/* ****************** */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_EMS_MLOAD_ITEMS_MAX)
sEMS_ExtInfo_t EMSInfoBuffer[CONFIG_EMS_MLOAD_ITEMS_MAX];
#endif
/* ****************** */
/* Debug variables    */
/* ****************** */



/* PLOS */
/* Pointer to DSPInfoBuffer */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
STATIC uint8_t * pDSPInfoBuffer;
#endif

/* Normalization algorithm parameters */

STATIC Loader_Struct m_sAlgoParams = { /*lint -epp*/ NULL /*lint -epp*/, 0 };


#if CONFIG_EMS_RLOAD_ITEMS_MAX
/* Array for resistive loads parameters collection */
STATIC sRLoadParam_t m_asRLoadParam[CONFIG_EMS_RLOAD_ITEMS_MAX];
#endif

#if CONFIG_EMS_MLOAD_ITEMS_MAX
/* Array for motor loads parameters collection */
STATIC sMLoadParam_t m_asMLoadParam[CONFIG_EMS_MLOAD_ITEMS_MAX];
#endif

#if CONFIG_EMS_TLOAD_ITEMS_MAX
/* Array for Table loads parameters collection */
STATIC sTLoadParam_t m_asTLoadParam[CONFIG_EMS_TLOAD_ITEMS_MAX];
#endif

STATIC sLoadIndex_t m_sLoadIndex = { 0, 0, 0 };

/* flag to activate energy meter store and measurement */
STATIC sFlags_t m_sFlags = { 1, 0, 0 };

/* last normalized peak voltage measured */
#if EMS_V_PEAK_HOLD
 STATIC uint16_t m_ui16VPeak = 0;
#endif

#if EMS_V_PEAK_THRESHOLD
 /* last max normalized peak voltage measured */
 STATIC uint16_t m_ui16VPeakThreshold = 0;
#endif

#if EMS_V_RMS_AVERAGE
 /* Average physical/normalized Vrms on last N samples */
 STATIC uint16_t m_ui16VrmsAvg = 0;
#endif

/* double buffer management data structure */
#if EMS_BUFFER_DOUBLE_USE
 STATIC sVMeasAcc_t  m_asVMeasAcc[2];
 STATIC sVMeasAcc_t* m_psVMeasAccCurr = &m_asVMeasAcc[0];
 STATIC sVMeasAcc_t* m_psVMeasAccHold = &m_asVMeasAcc[1];
#else
 STATIC sVMeasAcc_t  m_sVMeasAcc;
#endif

/* Power in the last period */
STATIC uint16_t m_ui16PowerPeriod = 0;

/* Normalized Energy accumulator for Resistors and Tables */
STATIC uint32_t m_ui32EnergyRTAcc = 0;

/*(Phisical) Energy */
STATIC uint32_t m_ui32EnergyOffset = 0;

/* #define DONT_MANAGE_OVERFLOW  1 */
/* #define DONT_MANAGE_OVERFLOW_MAINK  1 */
/* #define DONT_MANAGE_OVERFLOW_DSP  1 */
#ifndef DONT_MANAGE_OVERFLOW
#ifndef DONT_MANAGE_OVERFLOW_MAINK
STATIC uint32_t m_ui32EnergyMainK_1 = 0;
#endif
STATIC uint32_t m_ui32EnergyMainOffset = 0;
#ifndef DONT_MANAGE_OVERFLOW_DSP
STATIC uint32_t m_ui32EnergyDspK_1 = 0;
STATIC uint32_t m_ui32EnergyDspOffset = 0;
#endif
#endif

static uint32_t m_ui32EnergyMotOld = 0;

#if EMS_ENERGY_COUNTERS_MAX
 STATIC stEnergyCounters_t m_asCounters[EMS_ENERGY_COUNTERS_MAX];
#endif


#if (CONFIG_EMS_TLOAD_ITEMS_MAX && EMS_USE_TABLE_PARAMD)
/* Pointer to current D param required to calculate power consumption with Tables */
STATIC uint16_t* m_pui16TableParamD = NULL;
#endif


/* ******************* */
/* Function prototypes */
/* ******************* */
static uint16_t VoltAvgMeasuresUpdate (sVMeasAcc_t* psVMeasAcc);
#if EMS_BUFFER_DOUBLE_USE
static void VMeasBufferHold       (void);
#endif
static void VMeasBufferReset      (void);
static void EnergyMeterUpdate     (uint32_t ui32EnergyPeriod);
static void EnergyCountersUpdate  (void);


STATIC eEMS_Err EM_LoadRegister (const Loader_Struct* psEMDataStruct, const Loader_Struct* psResCalibDataStruct,
                                 const Loader_Struct* psOutputsMap);

STATIC uint8_t* EM_LoadResCalib (uint8_t ui8LoadTablePositionRes, const Loader_Struct* psResCalibDataStruct,
                                 const Loader_Struct* psOutputsMap, uint8_t numLoad,const sEMS_ResCalWBoxOff_t pResCalTab[]);



/* ****************** */
/* Exported functions */
/* ****************** */

/**
 * \n This function is called by the application to get the pointer
 * \n to the motor's energy buffer in platform
 * \n
 * \param [in] posMotor: position of the motor according to the order in Setting File
 * \n		(i.e. the first motor has posMotor = 0, the second posMotor = 1 and so on)
 * \n
 * \return  pointer to sEMS_ExtInfo_t*
 * \n
 * \author 	Salvatore Fimiani
 * \date		08/11/2012
 * \version
 */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && CONFIG_EMS_MLOAD_ITEMS_MAX
sEMS_ExtInfo_t* EMS_GetPtrToDSPInfoBuffer(uint8_t posMotor)
{
	if(posMotor < m_ui8MLoadIndex)
	{
		return (&EMSInfoBuffer[posMotor]);
	}
	else
	{
		return NULL;
	}
}
#endif

/*--------------------------------
 Perform module initialization for
 a cold reset
 ---------------------------------*/
eEMS_Err PowerMeter_HWReset (void)
{
  return EMS_E_OK;
}



/*------------------------------------
 Set set the parameters used for
 power consumption calculation.

 09/11/2012: Added initialization for EMSInfoBuffer[CONFIG_EMS_MLOAD_ITEMS_MAX]
 ---------------------------------*/
eEMS_Err PowerMeter_InitConfig(Loader_Struct* psEnergyAlgoDataStruct
#if EMS_USE_TABLE_PARAMD
                               , uint16_t* pui16TableParamD
#endif
                              )
{


#ifdef CHECK_PARAM
  if (psEnergyAlgoDataStruct == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
  if (   (psEnergyAlgoDataStruct->Data == NULL)
      || (psEnergyAlgoDataStruct->DataLenght < ALGO_PARAMS_WLEN))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* Set normalization algorithm parameters */
  m_sAlgoParams = *psEnergyAlgoDataStruct;

 #if (CONFIG_EMS_TLOAD_ITEMS_MAX && EMS_USE_TABLE_PARAMD)
  /* Set the D param address required to calculate power consumption with Tables. */
  m_pui16TableParamD = pui16TableParamD;
 #endif

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && CONFIG_EMS_MLOAD_ITEMS_MAX
  uint8_t i;
  for(i = 0; i<CONFIG_EMS_MLOAD_ITEMS_MAX;i++)
  {
	  EMSInfoBuffer[i].ui16PVal = 0;
	  EMSInfoBuffer[i].ui32EVal = 0;

  }
#endif

  return EMS_E_OK;
}



/*--------------------------------
 Perform module initialization for
 a warm reset
 ---------------------------------*/
eEMS_Err PowerMeter_InitSw ()
{
 #ifdef CHECK_PARAM
 #endif

 #if EMS_V_PEAK_HOLD
  m_ui16VPeak = 0;
 #endif

 #if EMS_V_PEAK_THRESHOLD
  m_ui16VPeakThreshold = 0;
 #endif

 #if EMS_V_RMS_AVERAGE
  m_ui16VrmsAvg = 0;
 #endif

  m_bAccuEnabled = TRUE;
  m_bOneShot = FALSE;

  m_ui16PowerPeriod      = 0;

  m_ui8EnergyUpdateCnt   = 0;

  m_ui32EnergyRTAcc      = 0;

  m_ui32EnergyOffset     = 0;

 #ifndef DONT_MANAGE_OVERFLOW
 #ifndef DONT_MANAGE_OVERFLOW_MAINK
  m_ui32EnergyMainK_1 = 0;
 #endif

  m_ui32EnergyMainOffset = 0;

 #ifndef DONT_MANAGE_OVERFLOW_DSP
  m_ui32EnergyDspK_1     = 0;

  m_ui32EnergyDspOffset  = 0;
 #endif
 #endif

  /* Setting done successfully */
  return EMS_E_OK;
}



/*--------------------------------
 Not implemented
 ---------------------------------*/
eEMS_Err PowerMeter_StartCal (void)
{
  return EMS_E_UNKNOWN_MODE;
}



/*-------------------------------------
 Store information to get load status
 anytime is required to calculate power
 consumption
 --------------------------------------*/

eEMS_Err PowerMeter_LoadRegister (sLoadRegister_t* psLoadRegister)
{
  /* Parameter control */
 #ifdef CHECK_PARAM
  if (psLoadRegister == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }

  if ((psLoadRegister->eType < LOAD_FIRST_ITEM) && (psLoadRegister->eType > LOAD_LAST_ITEM))
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif
  int16_t compValue;
  int16_t diffToOvflw;
  int32_t tempVal1;
  /*
   * Setting new load's parameters
   */
  switch (psLoadRegister->eType)
  {
    case RESISTOR:
     #if CONFIG_EMS_RLOAD_ITEMS_MAX
      if (   (psLoadRegister->params.res.pui16GMant == NULL)
          || (psLoadRegister->params.res.pi8Gexp == NULL))
      {
        return EMS_E_INVALID_VALUE;
      }
      if (m_ui8RLoadIndex >= CONFIG_EMS_RLOAD_ITEMS_MAX)
      {
        return EMS_E_PARAM_IGNORED;
      }

      /* Setting load parameters for resistor */
      m_asRLoadParam[m_ui8RLoadIndex].ui8Mask       = psLoadRegister->ui8Mask;
      m_asRLoadParam[m_ui8RLoadIndex].pui8Status    = psLoadRegister->pui8Status;
      m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant     = *(psLoadRegister->params.res.pui16GMant);
      m_asRLoadParam[m_ui8RLoadIndex].i8Gexp        = *(psLoadRegister->params.res.pi8Gexp);


      /* Check for RESISTOR calibration */
      if (psLoadRegister->params.res.pui8Calib != NULL)
      {
        if (   (*(psLoadRegister->params.res.pui8Calib) != RESISTOR_CALIB_NO_CALIB)
            && (*(psLoadRegister->params.res.pui8Calib) != RESISTOR_CALIB_DEF_VALUE))
        {
        	/* Calibrate RESISTOR */
			diffToOvflw = 0xFFFF - m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant;
			tempVal1 = (((((int32_t)(RESISTOR_CALIB_DEF_VALUE - *(psLoadRegister->params.res.pui8Calib))) << 8) * 6554) >> 16);
			compValue = (uint16_t)((  m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant * tempVal1)>>15);
			if(compValue <= diffToOvflw)
			{
				m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant += compValue;
			}
			else
			{

				if(tempVal1 >= 0)
				{
					m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant = (m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant >> 1) +
						(compValue >> 1);

					m_asRLoadParam[m_ui8RLoadIndex].i8Gexp += 1;
				}
				else
				{
					m_asRLoadParam[m_ui8RLoadIndex].ui16Gmant += compValue;
				}

			}

        }
      }

      ++m_ui8RLoadIndex;
     #endif  /* CONFIG_EMS_RLOAD_ITEMS_MAX */
      break;

    case MOTOR:
     #if CONFIG_EMS_MLOAD_ITEMS_MAX
      if (   (psLoadRegister->params.mot.pui16PVal == NULL)
          || (psLoadRegister->params.mot.pui32EVal == NULL))
      {
        return EMS_E_INVALID_VALUE;
      }
      if (m_ui8MLoadIndex >= CONFIG_EMS_MLOAD_ITEMS_MAX)
      {
        return EMS_E_PARAM_IGNORED;
      }



      /* Setting energy load parameters for motor energy */
      m_asMLoadParam[m_ui8MLoadIndex].ui8Mask       = psLoadRegister->ui8Mask;
      m_asMLoadParam[m_ui8MLoadIndex].pui8Status    = psLoadRegister->pui8Status;
      m_asMLoadParam[m_ui8MLoadIndex].pui16PVal     = psLoadRegister->params.mot.pui16PVal;
      m_asMLoadParam[m_ui8MLoadIndex].pui32EVal     = psLoadRegister->params.mot.pui32EVal;


      m_ui8MLoadIndex++;
     #endif /* CONFIG_EMS_MLOAD_ITEMS_MAX */
      break;

    case TABLEP:
     #if CONFIG_EMS_TLOAD_ITEMS_MAX
      if (m_ui8TLoadIndex >= CONFIG_EMS_TLOAD_ITEMS_MAX)
      {
        return EMS_E_PARAM_IGNORED;
      }



      /* Setting load parameters for table */
      if (EMS_E_OK == TableInit(psLoadRegister->params.tab.pui8Table, psLoadRegister->params.tab.ui8TableLen,
                                &m_asTLoadParam[m_ui8TLoadIndex].sTable))
      {
        m_asTLoadParam[m_ui8TLoadIndex].ui8Mask     = psLoadRegister->ui8Mask;
        m_asTLoadParam[m_ui8TLoadIndex].pui8Status  = psLoadRegister->pui8Status;

        ++m_ui8TLoadIndex;
      }
     #endif  /* CONFIG_EMS_TLOAD_ITEMS_MAX */
      break;

    default:
      return EMS_E_INVALID_VALUE;
  }

  /* Setting done successfully */
  return EMS_E_OK;
}



/*------------------------------------
 Sends Start measure command according
 to parameter passed.
 -------------------------------------*/
eEMS_Err PowerMeter_StartMeasure (eConvertion_t C_Conv, bool_t bResetCounters)
{
  if (bResetCounters == TRUE)
  {


    b_ui32EnergyCounterWh = 0;
    b_ui16EnergyCounterWhAcc = 0;
   #if EMS_USE_ELAPSED_TIMER
    b_ui32TimeElapsedSinceStart = 0;
   #endif
  }

  m_bAccuEnabled = TRUE;

  if (C_Conv == SINGLE_CONV)
  {
    m_bOneShot = TRUE;
  }
  else
  {
    m_bOneShot = FALSE;
  }

  return EMS_E_OK;
}



/*---------------------------------
 Reset flag to disable accumulation
 ----------------------------------*/
eEMS_Err PowerMeter_StopMeasure (void)
{
  m_bAccuEnabled = FALSE;

  return EMS_E_OK;
}



/*--------------------------------
 Not implemented
 ---------------------------------*/
eEMS_Err PowerMeter_GetInstant_I (uint8_t* pui8I)
{
  return EMS_E_UNKNOWN_MODE;
}



/*-----------------------------------------------
 Returns the value of the last peak Voltage measured
 ------------------------------------------------*/
eEMS_Err PowerMeter_GetInstant_V (uint16_t* pui16V)
{
 #if EMS_V_PEAK_HOLD
 #ifdef CHECK_PARAM
  if (   (pui16V == NULL)
      || VLINE_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  /* Normalized to physical conversion */
  *pui16V = (uint16_t)((((uint32_t)m_ui16VPeak * g_ui16ALGO_VMAX10) / 10) >> 16);

  return EMS_E_OK;
 #else /* EMS_V_PEAK_HOLD */
  return EMS_E_UNKNOWN_MODE;
 #endif /* EMS_V_PEAK_HOLD */
}


/*-----------------------------------------------
 Set the value of the current absolute V peak measured
 ------------------------------------------------*/
eEMS_Err PowerMeter_SetAbsInstant_V (uint16_t ui16VADabs)
{
	uint32_t tmpVAbs;

	/* Convert to normalized value */
	tmpVAbs = (((uint32_t) ui16VADabs * 10) << 16) / (g_ui16ALGO_VMAX10);

  return PowerMeter_SetNormInst_V(tmpVAbs);
}


/*-----------------------------------------------
 Set the value of the current normalized V peak measured
 ------------------------------------------------*/
eEMS_Err PowerMeter_SetInstant_V (uint32_t ui32VADcalib)
{
 #ifdef CHECK_PARAM
  if (   (m_psVMeasAccCurr == NULL)
      || VLINE_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  /* ui32VADcalib is on at most 11 bits,
   * extend (multiply by 2^6) on at most 17 bits ... */
  ui32VADcalib <<= 6;
  /* ... if necessary, saturate on 16 bits ... */
  if (ui32VADcalib > 65535)
  {
    ui32VADcalib = 65535;
  }
  /* ... and, subtract normalized RTN */
  ui32VADcalib -= (uint32_t)g_ui16ALGO_NORM_RTN;

  return PowerMeter_SetNormInst_V(ui32VADcalib);
}

/*-----------------------------------------------
 Set the already normalized value of the current normalized V peak measured
 ------------------------------------------------*/
  eEMS_Err PowerMeter_SetNormInst_V (uint32_t ui32VADcalib)
  {

  /* here ui32VADcalib contains the normalized V peak (at most 16 bits) */

 #if EMS_V_PEAK_HOLD
  m_ui16VPeak = (uint16_t)ui32VADcalib;
 #endif

 #if EMS_V_PEAK_THRESHOLD
  if (ui32VADcalib > m_ui16VPeakThreshold)
  {
    m_ui16VPeakThreshold = (uint16_t)ui32VADcalib;
  }
 #endif

 #if (EMS_V_PEAK_AVERAGE || EMS_V_RMS_AVERAGE)
  /* Add to V peak accumulator */
  m_psVMeasAccCurr->ui32VPeakAcc += ui32VADcalib;
 #endif

  /* The voltage samples counter is increased */
  m_psVMeasAccCurr->ui8NSamples++;

  /* check if energy meter store and measurement is active */
  if (m_bAccuEnabled == TRUE)
  {
    /* energy meter store and measurement is active */

    /* Perform the squared values ... */
    ui32VADcalib *= ui32VADcalib;
    /* ... and the division by 2^16, in order to distribuite the computation of the normalized power. */
    ui32VADcalib >>= 16;

    /* The new value (at most 16 bits) is added to the accumulator in order to perform the average value */
    m_psVMeasAccCurr->ui32VPeak2Acc += ui32VADcalib;
  }
  else if (m_psVMeasAccCurr->ui8NSamples > EMS_V_AVERAGE_N_SAMPLES)
  {
    /* energy meter store and measurement is not active */

    /* updates avg Vpeak and Vrms each second */
    VoltAvgMeasuresUpdate(m_psVMeasAccCurr);
    VMeasBufferReset();
  }

  return EMS_E_OK;
}



/*-----------------------------------------------
 Returns the value of last measured instant power
 ------------------------------------------------*/
eEMS_Err PowerMeter_GetInstant_P (uint16_t* pui16P)
{
 #ifdef CHECK_PARAM
  if (pui16P == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  *pui16P = m_ui16PowerPeriod;

  return EMS_E_OK;
}



/*-------------------------------------------
 Return the value of average power calculated
 on last N samples (tip. 1 sec).
 --------------------------------------------*/
eEMS_Err PowerMeter_GetAvgLastCycle_P (uint16_t* pui16P)
{
 #ifdef CHECK_PARAM
  if (   (pui16P == NULL)
      || ALGO_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  *pui16P = m_ui16PowerPeriod;

  return EMS_E_OK;
}



/*-----------------------------------------------------
 Return the value of average power calculated
 on last N samples (tip. 1 sec) in hundredths of a watt.
 ------------------------------------------------------*/
eEMS_Err PowerMeter_GetAvgLastCycle_P_100mW (uint16_t* pui16P)
{
 #ifdef CHECK_PARAM
  if (   (pui16P == NULL)
      || ALGO_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  *pui16P = m_ui16PowerPeriod * 10;

  return EMS_E_OK;
}



/*--------------------------------
 Not implemented
 ---------------------------------*/
eEMS_Err PowerMeter_Get_I_RMS (uint8_t* pui8I)
{
  return EMS_E_UNKNOWN_MODE;
}



/*--------------------------------
 Not implemented
 ---------------------------------*/
eEMS_Err PowerMeter_Get_I_RMS_100mA (uint32_t* pui32I)
{
  return EMS_E_UNKNOWN_MODE;
}



/*--------------------------------
 Return the last rms volt measured
 ---------------------------------*/
eEMS_Err PowerMeter_Get_V_RMS (uint16_t* pui16V)
{
 #ifdef CHECK_PARAM
  if (pui16V == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

 #if EMS_V_RMS_AVERAGE
 #ifdef CHECK_PARAM
  if (VLINE_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

 #if EMS_V_RMS_AVERAGE_PHY
  *pui16V = m_ui16VrmsAvg;
 #else
  /* Normalized to physical conversion */
  *pui16V = (uint16_t)((((uint32_t)m_ui16VrmsAvg * g_ui16ALGO_VMAX10) / 10) >> 16);
 #endif

  return EMS_E_OK;
 #else /* EMS_V_RMS_AVERAGE */
  return EMS_E_UNKNOWN_MODE;
 #endif /* EMS_V_RMS_AVERAGE */
}



/*----------------------------------------------------------
 Return the last rms volt measured in hundredths of a volts.
 -----------------------------------------------------------*/
eEMS_Err PowerMeter_Get_V_RMS_100mV (uint32_t* pui32V)
{
 #ifdef CHECK_PARAM
  if (pui32V == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

 #if EMS_V_RMS_AVERAGE
 #ifdef CHECK_PARAM
  if (VLINE_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

 #if EMS_V_RMS_AVERAGE_PHY
  *pui32V = (uint32_t)m_ui16VrmsAvg * 10;
 #else
  /* Normalized to physical conversion */
  *pui32V = ((uint32_t)m_ui16VrmsAvg * g_ui16ALGO_VMAX10) >> 16;
 #endif

  return EMS_E_OK;
 #else /* EMS_V_RMS_AVERAGE */
  return EMS_E_UNKNOWN_MODE;
 #endif /* EMS_V_RMS_AVERAGE */
}



/*---------------------------------------------------------------
 Returns the value of energy in Wh since the last start measure.
 ----------------------------------------------------------------*/
eEMS_Err PowerMeter_GetAvgSinceStart_E_Wh (uint32_t* pui32EWh, uint32_t* pui32TimeElapsed)
{
 #ifdef CHECK_PARAM
  if (pui32EWh == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }

 #if EMS_USE_ELAPSED_TIMER
  if (pui32TimeElapsed == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif
 #endif

 #if EMS_USE_ELAPSED_TIMER
  /* Returning current timer value */
  *pui32TimeElapsed = b_ui32TimeElapsedSinceStart;
 #endif

  *pui32EWh = b_ui32EnergyCounterWh;

  return EMS_E_OK;
}



/*---------------------------------------------
 Called every second to perform counters update
 ----------------------------------------------*/
eEMS_Err PowerMeter_Accum (void)
{
  uint8_t   ui8Index;                   /* index used for accessing load data structure */
  uint16_t  ui16Vpeak2Avg_Norm;         /* average of normalized Vpeak squared: used to calculate power with RESISTOR load */
 #if CONFIG_EMS_TLOAD_ITEMS_MAX
  uint16_t  ui16VinTab_Norm;            /* average normalized Vpeak: used to calculate power with TABLE load */
 #endif
 #if (CONFIG_EMS_TLOAD_ITEMS_MAX | CONFIG_EMS_RLOAD_ITEMS_MAX)
  uint16_t  ui16PowerInstant_Norm;      /* normalized power instant for a load */
 #endif
  uint32_t  ui32PowerInstantRT_Norm;    /* normalized power instant for RESISTOR and TABLE load */
  uint16_t  ui16PowerInstantMot;        /* (phisical) power instant for power section of MOTOR load */
  uint32_t  ui32Energy;                 /* (phisical) energy in the update period */
  uint32_t  ui32EnergyMot;              /* calculated difference between the current and the previous (phisical) energy values in a
                                         * time period (tip. 1 sec) with energy section of MOTOR load */
 #ifndef DONT_MANAGE_OVERFLOW
  uint32_t  ui32DeltaEnergyRTAcc;       /* Energia normalizzata del ciclo corrente per RESISTOR, TABLE*/
 #ifdef DONT_MANAGE_OVERFLOW_MAINK
  uint32_t  ui32MaxDeltaEnergyRTAcc;
 #endif
 #endif
  bool_t    bLoadIsActive;              /* TRUE if load is active */


 #ifdef CHECK_PARAM
  if (ALGO_PARAMS_NOT_VALID())
  {
    return EMS_E_INVALID_VALUE;
  }
 #endif

  /* initialize automatic variables */
  ui32PowerInstantRT_Norm = 0;
  ui16PowerInstantMot     = 0;
  ui32EnergyMot           = 0;

  /* initialize global variables */

  /* function body */

  /* check if energy meter store and measurement is active */
  if (m_bAccuEnabled == TRUE)
  {
    /* energy meter store and measurement is active */

    /* check if energy update period is elapsed */
    m_ui8EnergyUpdateCnt++;
    /* if (m_ui8EnergyUpdateCnt >= (1000/EMS_PW_UPDATE_PERIOD)) */
    if (m_ui8EnergyUpdateCnt >= g_ui8ALGO_TS)
    {
      /* energy update period is elapsed */

     #if EMS_BUFFER_DOUBLE_USE
      /* hold the data that are going to use */
      VMeasBufferHold();
     #endif

      if (m_psVMeasAccHold->ui8NSamples > 0)
      {
        /*  calculate average of collected Vpeak squared */
        ui16Vpeak2Avg_Norm = (uint16_t)(m_psVMeasAccHold->ui32VPeak2Acc / m_psVMeasAccHold->ui8NSamples);
      }
      else
      {
        ui16Vpeak2Avg_Norm = 0;
      }

      {
        /* set tension value for table load */
      #if CONFIG_EMS_TLOAD_ITEMS_MAX
       #if (EMS_V_PEAK_AVERAGE || EMS_V_RMS_AVERAGE)
        ui16VinTab_Norm =                                           /* average Vpeak */
       #else
        ui16VinTab_Norm = ((uint16_t)sqrt(ui16Vpeak2Avg_Norm));    /* square root of average Vpeak squared */
       #endif
      #endif

        /* updates avg Vpeak and Vrms */
        VoltAvgMeasuresUpdate(m_psVMeasAccHold);

       #if (EMS_BUFFER_DOUBLE_USE == 0)
        /* reset data just used */
        VMeasBufferReset();
       #endif

        /* --------- Calculating power ---------- */

       #if CONFIG_EMS_RLOAD_ITEMS_MAX
        /* Check on active RESISTOR loads */



        for (ui8Index = 0; ui8Index < m_ui8RLoadIndex; ui8Index++)
        {



          bLoadIsActive = FALSE;
          if (m_asRLoadParam[ui8Index].pui8Status == NULL)
          {
            bLoadIsActive = TRUE;
          }
         /* FIXME: ?
          else if (   (*m_asRLoadParam[ui8Index].pui8Status & m_asRLoadParam[ui8Index].ui8Mask)
                   == m_asRLoadParam[ui8Index].ui8Mask)
          */
          else if ((*m_asRLoadParam[ui8Index].pui8Status & m_asRLoadParam[ui8Index].ui8Mask) != 0)
          {
            bLoadIsActive = TRUE;
          }

          if (bLoadIsActive)
          {
            /* calculate normalized power ... */
            ui16PowerInstant_Norm = (uint16_t)(((uint32_t)ui16Vpeak2Avg_Norm * m_asRLoadParam[ui8Index].ui16Gmant)
                                               >> (16 - m_asRLoadParam[ui8Index].i8Gexp));



            /* ... and add to the accumulator */
            ui32PowerInstantRT_Norm += ui16PowerInstant_Norm;
          }
        }
       #endif /* CONFIG_EMS_RLOAD_ITEMS_MAX */

       #if CONFIG_EMS_MLOAD_ITEMS_MAX
        /* Check on active MOTOR loads */



        for (ui8Index = 0; ui8Index < m_ui8MLoadIndex; ui8Index++)
        {



          bLoadIsActive = FALSE;
          if (m_asMLoadParam[ui8Index].pui8Status == NULL)
          {
            bLoadIsActive = TRUE;
          }
          /* FIXME: ?
           else if (   (*m_asMLoadParam[ui8Index].pui8Status & m_asMLoadParam[ui8Index].ui8Mask)
                    == m_asMLoadParam[ui8Index].ui8Mask)
           */
          else if ((*m_asMLoadParam[ui8Index].pui8Status & m_asMLoadParam[ui8Index].ui8Mask) != 0)
          {
            bLoadIsActive = TRUE;
          }

          if (bLoadIsActive)
          {



            /* add the (physical) power to a separate accumulator */
            ui16PowerInstantMot += *m_asMLoadParam[ui8Index].pui16PVal;
          }

          /* get energy from energy loads: energy loads use cumulative energy so we have to take in account only the
           * difference between the current and the previous energy values. */

          ui32EnergyMot += *m_asMLoadParam[ui8Index].pui32EVal;
        }
       #endif /* CONFIG_EMS_MLOAD_ITEMS_MAX */

       #if CONFIG_EMS_TLOAD_ITEMS_MAX
        /* Check on active loads (table)*/



        for (ui8Index = 0; ui8Index < m_ui8TLoadIndex; ui8Index++)
        {



          bLoadIsActive = FALSE;
          if (m_asTLoadParam[ui8Index].pui8Status == NULL)
          {
            bLoadIsActive = TRUE;
          }
          /* FIXME: ?
           else if (   (*m_asTLoadParam[ui8Index].pui8Status & m_asTLoadParam[ui8Index].ui8Mask)
                    == m_asTLoadParam[ui8Index].ui8Mask)
           */
          else if ((*m_asTLoadParam[ui8Index].pui8Status & m_asTLoadParam[ui8Index].ui8Mask) != 0)
          {
            bLoadIsActive = TRUE;
          }

          if (bLoadIsActive)
          {
            /* calculate normalized power for Table load ... */
            ui16PowerInstant_Norm = 0;
            if (EMS_E_OK == TablePower(&m_asTLoadParam[ui8Index].sTable, (
           #if (CONFIG_EMS_TLOAD_ITEMS_MAX && EMS_USE_TABLE_PARAMD)
                                       m_pui16TableParamD ? *m_pui16TableParamD :
           #endif
                                       EMS_TABLE_PARAM_D_DEFAULT),
                                       ui16VinTab_Norm,
                                       &ui16PowerInstant_Norm))
            {



              /* ... and add to accumulator */
              ui32PowerInstantRT_Norm += (uint32_t)ui16PowerInstant_Norm;
            }
          }
        }
       #endif /* CONFIG_EMS_TLOAD_ITEMS_MAX */

        /* update global variable for user query */
        m_ui16PowerPeriod =   (uint16_t)((ui32PowerInstantRT_Norm * g_ui16ALGO_PMAX) >> 16)   /* physical power for RESISTOR, TABLE ...*/
                            + ui16PowerInstantMot;                                            /* ... and MOTOR */

        /* --------- Calculating energy ---------- */

       #ifndef DONT_MANAGE_OVERFLOW
        /* Begin Overflow Energy Acc Management */

        /* Energia normalizzata del ciclo corrente per RESISTOR, TABLE*/
        ui32DeltaEnergyRTAcc = (((ui32PowerInstantRT_Norm * g_ui16ALGO_KENERGY_MANT) / 10) >> (16 - g_i8ALGO_KENERGY_EXP));

       #ifndef DONT_MANAGE_OVERFLOW_MAINK

        m_ui32EnergyRTAcc += ui32DeltaEnergyRTAcc;

        /* ui32Energy = (m_ui32EnergyRTAcc * g_ui16ALGO_EMAX) >> 16; */
        ui32Energy =   (((m_ui32EnergyRTAcc & 0xFFFF0000) >> 16) * g_ui16ALGO_EMAX)   /* 32 bit */
                     + (((m_ui32EnergyRTAcc & 0x0000FFFF) * g_ui16ALGO_EMAX) >> 16);  /* 32 bit */

        /* update energy with m_ui32EnergyPeriod */

        if (ui32Energy < (m_ui32EnergyMainK_1 - m_ui32EnergyMainOffset)) /* Reset Overflow Energy Main */
        {
          m_ui32EnergyMainOffset = m_ui32EnergyMainK_1;
        }

        ui32Energy += m_ui32EnergyMainOffset;
        m_ui32EnergyMainK_1 = ui32Energy;

       #else

        /* Controllo se c'� l'overflow del contatore globale normalizzato */
        ui32MaxDeltaEnergyRTAcc = 0xFFFFFFFF - m_ui32EnergyRTAcc;
        if ( ui32MaxDeltaEnergyRTAcc < ui32DeltaEnergyRTAcc)
        {
          /* C'� l'overflow del contatore normalizzato: aggiungo all'offset il valore fisico corrispondente a 2^32 */
          m_ui32EnergyMainOffset +=  g_ui16ALGO_EMAX << 16;
        }

        /* Sommp al contatore globale normalizzato l'energia normalizzata del ciclo corrente:
         * eventualmente ci sar� l'overflow */
        m_ui32EnergyRTAcc += ui32DeltaEnergyRTAcc;

        /* calcolo l'energia fisica associata al contatore globale normalizzato */
        //ui32Energy = (m_ui32EnergyRTAcc * g_ui16ALGO_EMAX) >> 16;
        ui32Energy =   (((m_ui32EnergyRTAcc & 0xFFFF0000) >> 16) * g_ui16ALGO_EMAX)   // 32 bit
                     + (((m_ui32EnergyRTAcc & 0x0000FFFF) * g_ui16ALGO_EMAX) >> 16);  // 32 bit

        /* Aggiungo l'offset che tiene conto degli overflow */
        ui32Energy += m_ui32EnergyMainOffset;

       #endif

        /* PATCH: Chiusura porta */
        if ( ui32EnergyMot == 0 )
        {
          ui32EnergyMot = m_ui32EnergyMotOld;
        }
        else
        {
          m_ui32EnergyMotOld = ui32EnergyMot;
        }
        /* END PATCH */

       #ifndef DONT_MANAGE_OVERFLOW_DSP
        if (ui32EnergyMot < (m_ui32EnergyDspK_1 - m_ui32EnergyDspOffset)) /* Reset Overflow Energy DSP */
        {
          m_ui32EnergyDspOffset = m_ui32EnergyDspK_1;
        }

        ui32EnergyMot += m_ui32EnergyDspOffset;
        m_ui32EnergyDspK_1 = ui32EnergyMot;
       #endif
        /* End Overflow Energy Acc management */
       #else
        m_ui32EnergyRTAcc += (((ui32PowerInstantRT_Norm * g_ui16ALGO_KENERGY_MANT) / 10) >> (16 - g_i8ALGO_KENERGY_EXP));

        /* physical energy for RESISTOR, TABLE */
        //ui32Energy = (m_ui32EnergyRTAcc * g_ui16ALGO_EMAX) >> 16;
        ui32Energy =   (((m_ui32EnergyRTAcc & 0xFFFF0000) >> 16) * g_ui16ALGO_EMAX)   // 32 bit
                     + (((m_ui32EnergyRTAcc & 0x0000FFFF) * g_ui16ALGO_EMAX) >> 16);  // 32 bit
       #endif

        /* update energy with m_ui32EnergyPeriod */
        EnergyMeterUpdate(ui32Energy + ui32EnergyMot);

        /* update partial counters */
        EnergyCountersUpdate();

       #if EMS_USE_ELAPSED_TIMER
        /* Incrementing local vitality counter */
        b_ui32TimeElapsedSinceStart++;
       #endif
      } /* (m_psVMeasAccHold->ui8NSamples > 0) */

      if (m_bOneShot == TRUE)
      {
        m_bAccuEnabled = FALSE;
      }

      m_ui8EnergyUpdateCnt = 0;
    } /* (m_ui8EnergyUpdateCnt >= g_ui8ALGO_TS) */
  } /* (m_bAccuEnabled == TRUE) */

  return EMS_E_OK;
}


/*--------------------------------
  Starts the given Energy counter
  --------------------------------*/
eEMS_Err PowerMeter_StartEnergyCount (uint8_t ui8CounterId, stEnergyCounters_t* pstCounterData)
{
 #if EMS_ENERGY_COUNTERS_MAX
  if (  (ui8CounterId < EMS_ENERGY_COUNTERS_MAX)
      &&(pstCounterData != NULL))
  {
    /* copy pstCounterData content to m_asCounters[ui8CounterId] */
    m_asCounters[ui8CounterId]=(*pstCounterData);
    /* reset some m_asCounters[ui8CounterId] field (all except ui16EnergyTreshold and ui16TimePeriod) */
    m_asCounters[ui8CounterId].ui8Flags               = EM_COUNT_STAT_MASK;
    m_asCounters[ui8CounterId].ui16EnergyCons         = 0;
    m_asCounters[ui8CounterId].ui16TimeElapsed        = 0;
    m_asCounters[ui8CounterId].ui16MaxEnergy10sec     = 0;
    m_asCounters[ui8CounterId].ui32RawAccum           = 0;
    m_asCounters[ui8CounterId].ui32Raw10secAcc        = 0;
    m_asCounters[ui8CounterId].ui32RawMaxEnergy10sec  = 0;

    return EMS_E_OK;
  }
  else
  {
    return EMS_E_INVALID_VALUE;
  }
 #else
  return EMS_E_UNKNOWN_MODE;
 #endif
}



/*-----------------------------------------------------------------
  Returns energy consumption data about the given Energy counter id
  -----------------------------------------------------------------*/
eEMS_Err PowerMeter_GetEnergyCount (uint8_t ui8CounterId, stEnergyCounters_t* pstCounterData)
{
 #if EMS_ENERGY_COUNTERS_MAX
  uint32_t ui32Tmp;

  if (   (ui8CounterId < EMS_ENERGY_COUNTERS_MAX)
      && (pstCounterData != NULL))
  {
    /* calculate Energy in Wh */

    ui32Tmp = m_asCounters[ui8CounterId].ui32RawAccum / 3600;
    m_asCounters[ui8CounterId].ui16EnergyCons = ui32Tmp & 0x0000FFFF;

    ui32Tmp = m_asCounters[ui8CounterId].ui32RawMaxEnergy10sec / 3600;
    m_asCounters[ui8CounterId].ui16MaxEnergy10sec = ui32Tmp & 0x0000FFFF;

    pstCounterData->ui8Flags            = m_asCounters[ui8CounterId].ui8Flags;
    pstCounterData->ui16EnergyCons      = m_asCounters[ui8CounterId].ui16EnergyCons;
    pstCounterData->ui16TimeElapsed     = m_asCounters[ui8CounterId].ui16TimeElapsed;
    pstCounterData->ui16MaxEnergy10sec  = m_asCounters[ui8CounterId].ui16MaxEnergy10sec;

    return EMS_E_OK;
  }
  else
  {
    return EMS_E_INVALID_VALUE;
  }
 #else
  return EMS_E_UNKNOWN_MODE;
 #endif
}



/*--------------------------------
  Stops the given Energy counter
  --------------------------------*/
eEMS_Err PowerMeter_StopEnergyCount (uint8_t ui8CounterId)
{
 #if EMS_ENERGY_COUNTERS_MAX
  if (ui8CounterId < EMS_ENERGY_COUNTERS_MAX)
  {
    m_asCounters[ui8CounterId].ui8Flags &= ~EM_COUNT_STAT_MASK;

    return EMS_E_OK;
  }
  else
  {
    return EMS_E_INVALID_VALUE;
  }
 #else
  return EMS_E_UNKNOWN_MODE;
 #endif
}



/* ******************* */
/* Local function      */
/* ******************* */


#if EMS_BUFFER_DOUBLE_USE

/**
Swap the double data structures used to decuple the Vrms accumulator measured and
Vrms used to calculate power

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   void

\retval   [none]

\warning  [none]

\author   Intecs
\date     30-05-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
static void VMeasBufferHold (void)
{
  sVMeasAcc_t*  psVMeasAccTmp;

 #if INTERRUPT_MANAGE
  /*  Disabling Interrupt */
  SUSPEND_ALL_INTERRUPTS
 #endif

  /* Switching accumulator */
  psVMeasAccTmp  = m_psVMeasAccCurr;
  m_psVMeasAccCurr = m_psVMeasAccHold;
  m_psVMeasAccHold = psVMeasAccTmp;

  /* reset the data structure that will be used */
  VMeasBufferReset();

 #if INTERRUPT_MANAGE
  /* Enabling Interrupt */
  RESUME_ALL_INTERRUPTS
 #endif
}

#endif /*EMS_BUFFER_DOUBLE_USE*/


/**
Reset the current structure used to manage Voltage: peack, rms, rms2

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   void

\retval   [none]

\warning  [none]

\author   Intecs
\date     30-05-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
static void VMeasBufferReset (void)
{
  /* Reset structure for next measure session */
 #if (EMS_V_PEAK_AVERAGE || EMS_V_RMS_AVERAGE)
  m_psVMeasAccCurr->ui32VPeakAcc  = 0;
 #endif

  m_psVMeasAccCurr->ui32VPeak2Acc  = 0;
  m_psVMeasAccCurr->ui8NSamples   = 0;
}


/**
Perform the average calculation of Vpeak accumulator (if enabled) or Vrms2 accumulator
(if enabled) than reset accumulator/s

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   Average normalized VPeak on last N samples

\retval   [none]

\warning  [none]

\author   Intecs
\date     30-05-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
static uint16_t VoltAvgMeasuresUpdate (sVMeasAcc_t* psVMeasAcc)
{
  uint16_t  ui16VPeakAvg = 0;

  if ( psVMeasAcc == NULL )
  {
    return ui16VPeakAvg;
  }

  /* the psVMeasAcc->ui8NSamples value cannot be 0 */

 #if (EMS_V_PEAK_AVERAGE || EMS_V_RMS_AVERAGE)
  if (psVMeasAcc->ui8NSamples > 0)
  {
    psVMeasAcc->ui32VPeakAcc /= psVMeasAcc->ui8NSamples;
  }
  else
  {
    psVMeasAcc->ui32VPeakAcc = 0;
  }

  {
    ui16VPeakAvg = (uint16_t)psVMeasAcc->ui32VPeakAcc;

   #if EMS_V_RMS_AVERAGE
    m_ui16VrmsAvg = (uint16_t)((psVMeasAcc->ui32VPeakAcc * 1000) / (SQUARE_ROOT_2 * 1000));

   #if EMS_V_RMS_AVERAGE_PHY
    /* Normalized to physical conversion */
    m_ui16VrmsAvg = (uint16_t)((((uint32_t)m_ui16VrmsAvg * g_ui16ALGO_VMAX10) / 10) >> 16);
   #endif /* EMS_V_RMS_AVERAGE_PHY */

   #endif /* EMS_V_RMS_AVERAGE */
  }
 #endif

  return ui16VPeakAvg;
}



/**
Perform update of the energy meter counter from start of life

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   void

\retval   [none]

\warning  [none]

\author   Intecs
\date     30-05-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
static void EnergyMeterUpdate (uint32_t ui32EnergyPeriod)
{
  uint32_t ui32Energy = ((b_ui32EnergyCounterWh * 3600) + b_ui16EnergyCounterWhAcc);

  /* update the energy meter counter */
  if (ui32EnergyPeriod < (ui32Energy - m_ui32EnergyOffset))
  {
    m_ui32EnergyOffset = ui32Energy;
  }

  ui32Energy  = ui32EnergyPeriod + m_ui32EnergyOffset;

  b_ui32EnergyCounterWh     = ui32Energy / 3600;
  b_ui16EnergyCounterWhAcc  = (uint16_t)(ui32Energy % 3600);
}



/**
Perform update of the energy partial counters

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   void

\retval   [none]

\warning  [none]

\author   Intecs
\date     30-05-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
static void EnergyCountersUpdate (void)
{
 #if EMS_ENERGY_COUNTERS_MAX
  uint8_t ui8Index;

  for (ui8Index = 0; ui8Index < EMS_ENERGY_COUNTERS_MAX; ui8Index++)
  {
    /* if counter enabled*/
    if ((m_asCounters[ui8Index].ui8Flags & EM_COUNT_STAT_MASK) !=0)
    {
      /* accumulate energy and time */
      m_asCounters[ui8Index].ui32RawAccum     += m_ui16PowerPeriod;
      m_asCounters[ui8Index].ui32Raw10secAcc  += m_ui16PowerPeriod;
      m_asCounters[ui8Index].ui16TimeElapsed++;

      if (((m_asCounters[ui8Index].ui16TimeElapsed) % 10) == 0)
      {
        /*
          save the max in 10 secs as raw
        */
        if(m_asCounters[ui8Index].ui32Raw10secAcc > m_asCounters[ui8Index].ui32RawMaxEnergy10sec)
        {
          m_asCounters[ui8Index].ui32RawMaxEnergy10sec = m_asCounters[ui8Index].ui32Raw10secAcc;
        }
        m_asCounters[ui8Index].ui32Raw10secAcc = 0;
      }

      /* if time period not yet elapsed but current time > time period ... */
      if (  (m_asCounters[ui8Index].ui16TimeElapsed >= m_asCounters[ui8Index].ui16TimePeriod)
          &&((m_asCounters[ui8Index].ui8Flags & EM_T_ELAPSED_MASK) == 0))
      {
        /* detect transition */
        m_asCounters[ui8Index].ui8Flags |= EM_T_ELAPSED_MASK;

        /* calculate energy consumed so far in Wh...*/
        m_asCounters[ui8Index].ui16EnergyCons = (uint16_t) (m_asCounters[ui8Index].ui32RawAccum / 3600);
        /*... if it is grater then energy threshold */
        if(m_asCounters[ui8Index].ui16EnergyCons > m_asCounters[ui8Index].ui16EnergyTreshold)
        {
          /* set the overrun flag*/
          m_asCounters[ui8Index].ui8Flags |= EM_E_OVERRUN_MASK;
        }
      }
    }
  }
 #endif
}

/* #ifdef POWER_METER_SW */
#if (CONFIG_RES_CALIBRATION_ENABLE == 1)
/**
  This function get a pointer to Resistor calibration factor

\param  [in]      uint8_t         ui8LoadTablePositionRes   ["Load Table Position" field for Resistor in current Energy Meter Data]
\param  [in]      Loader_Struct*  psResCalibDataStruct      [pointer to struct containing the Indesit Plant Cali-Box data]
\param  [in]      Loader_Struct*  psOutputsMap              [pointer to struct containing the Outputs Map]
\param  [out]     none
\param  [in,out]  none

\attention \b

\return           uint8_t* - pointer to Resistor calibration factor

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
STATIC uint8_t* EM_LoadResCalib (uint8_t ui8LoadTablePositionRes, const Loader_Struct* psResCalibDataStruct,
                                 const Loader_Struct* psOutputsMap, uint8_t numLoad, const sEMS_ResCalWBoxOff_t pResCalTab[])
{

	uint8_t i;
  uint16_t ui16LoadIdOffset = (ui8LoadTablePositionRes * IO_MANAGER_LOAD_TABLE_ITEM_SIZE * 2) + LOAD_ID_OFFSET;


 #if CHECK_TABLE
  if (   (psResCalibDataStruct == NULL)
      || (psOutputsMap == NULL))
  {
    return NULL;
  }
  if (   (psResCalibDataStruct->Data == NULL)
      || (psResCalibDataStruct->DataLenght < INDESIT_PLANT_CALI_WLEN))
  {
    return NULL;
  }
  if (   (psOutputsMap->Data == NULL)
      || (psOutputsMap->DataLenght == 0))
  {
    return NULL;
  }

  if (ui16LoadIdOffset >= (2 * psOutputsMap->DataLenght))
  {
    return NULL;
  }
 #endif


  for(i = 0; i<numLoad; i++ )
  {
	  if(pResCalTab[i].resLoadID == psOutputsMap->Data[ui16LoadIdOffset])
	  {
		  return &(psResCalibDataStruct->Data[pResCalTab[i].resCalOffset]);
	  }


  }
  return NULL;

}
#endif /*(CONFIG_RES_CALIBRATION_ENABLE == 1) */

/**
  This function register a load

\param  [in]      Loader_Struct*  psEMDataStruct            [pointer to struct containing the current energy table]
\param  [in]      Loader_Struct*  psResCalibDataStruct      [pointer to struct containing the Indesit Plant Cali-Box data]
\param  [in]      Loader_Struct*  psOutputsMap              [pointer to struct containing the Outputs Map]
\param  [out]     none
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
STATIC eEMS_Err EM_LoadRegister (const Loader_Struct* psEMDataStruct, const Loader_Struct* psResCalibDataStruct,
                                 const Loader_Struct* psOutputsMap) 
{
  uint8_t ui8EnergyLoadType;    /* Valore del campo "Energy Load Type" nella tabella energetica corrente */
  uint8_t ui8LoadTablePosition; /* Valore del campo "Load Table Position" nella tabella energetica corrente */
  uint8_t ui8TableWLen;         /* Dimensione (word) della tabella energetica corrente */
  uint8_t ui8Result;
  sLoadRegister_t sLoadRegister;

  static uint8_t ui8IdxInfoBuffer = 0; /*Indice identificativo del carico esterno (Motore) */

 #if CHECK_TABLE
  if (psEMDataStruct == NULL)
  {
	/*lint -e64*/
    return S_FALSE_PLAT;
    /*lint +e64*/
  }
 #endif

  /* Recupera la dimensione della tabella energetica corrente */
  ui8TableWLen = psEMDataStruct->DataLenght;

 #if CHECK_TABLE
  /* Verifica la dimensione minima della tabella */
  if (ui8TableWLen < EM_DATA_HEADER_COMMON_WLEN)
  {
    /* Dimensione non corretta della tabella corrente */
    /*lint -e64*/
    return S_FALSE_PLAT;
    /*lint +e64*/
  }
 #endif

  /*
   * Usa la tabella per registrare i carichi
   */

  /* Recupera il campo "Load Table Position" nella tabella energetica corrente */
  ui8LoadTablePosition = psEMDataStruct->Data[EM_DATA_LOAD_TYPE_POSITION];
  if (ui8LoadTablePosition != LOAD_STS_ALWAYS_ON)
  {
    sLoadRegister.pui8Status                  = &LoadsStatus[ui8LoadTablePosition];
  }
  else
  {
	/*lint -epp*/
    sLoadRegister.pui8Status                  = NULL;
    /*lint +epp*/
  }

  /* Recupera il campo "Energy Load Type" nella tabella energetica corrente */
  ui8EnergyLoadType = psEMDataStruct->Data[EM_DATA_ENERGY_LOAD_TYPE];
  /*lint -e64*/
  /*Questo messaggio può essere disabilitato in quanto in questo caso non è violata la regola MISRA 29
   * La regola 29 (5.4 in MISRA-C:2004)recita:
   * "Where the type definition is made in a header file and that header file is included in multiple
   * source files, this rule is not violated."
   */
  sLoadRegister.eType = ui8EnergyLoadType;
  /*lint +e64*/

  /* Registra il carico */
  switch (ui8EnergyLoadType)
  {
    case RESISTOR:
     #if CHECK_TABLE
      /* Verifica la dimensione minima di una tabella Resistive */
      if (ui8TableWLen >= EM_DATA_RESISTIVE_WLEN)
     #endif
      {
        /* Registra un carico Resistive */
       #if (CONFIG_RES_CALIBRATION_ENABLE == 1)
        sLoadRegister.params.res.pui8Calib    = EM_LoadResCalib(ui8LoadTablePosition, psResCalibDataStruct, psOutputsMap, CONFIG_NUM_RES_TO_CALIB, resCalTab);
       #else
        sLoadRegister.params.res.pui8Calib    = NULL;
       #endif
        sLoadRegister.ui8Mask                 = 0x0F;                                                       /* FIXME: Va bene questa maschera ? */
        sLoadRegister.params.res.pui16GMant   = (uint16_t*)&(psEMDataStruct->Data[EM_DATA_RESISTIVE_MANT_IDX]); /* puntatore nel file di setting */
        sLoadRegister.params.res.pi8Gexp      = (int8_t*)&(psEMDataStruct->Data[EM_DATA_RESISTIVE_EXP_IDX]);   /* puntatore nel file di setting */
      }
      break;
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
    case MOTOR:
      /* Registra i carichi Motor associati a EMSInfoBuffer[ui8IdxInfoBuffer]  */

      sLoadRegister.ui8Mask                   = 0x0F;                                                       /* FIXME: Va bene questa maschera ? */
      sLoadRegister.params.mot.pui16PVal      = &EMSInfoBuffer[ui8IdxInfoBuffer].ui16PVal;
      sLoadRegister.params.mot.pui32EVal      = &EMSInfoBuffer[ui8IdxInfoBuffer].ui32EVal;
      if (ui8IdxInfoBuffer < CONFIG_EMS_MLOAD_ITEMS_MAX )
      {
    	  ui8IdxInfoBuffer++;
      }
      else
      {
    	  /*Do nothing*/
      }

      break;
#endif /*CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/
    case TABLEP:
     #if CHECK_TABLE
      /* Verifica la dimensione minima di una tabella TABLE */
      if (ui8TableWLen >= EM_DATA_HEADER_COMMON_WLEN)
     #endif
      {
        /* Registra un carico Table */
        sLoadRegister.ui8Mask                 = 0x0F;                                                    /* FIXME: Va bene questa maschera ? */
        sLoadRegister.params.tab.pui8Table    = &(psEMDataStruct->Data[2 * EM_DATA_HEADER_COMMON_WLEN]);     /* puntatore nel file di setting */
        sLoadRegister.params.tab.ui8TableLen  = (ui8TableWLen - EM_DATA_HEADER_COMMON_WLEN);
      }
      break;

    default:
    /*lint -e64*/
      return S_FALSE_PLAT;
    /*lint +e64*/
  /*    break;*/
  }
  ui8Result = PowerMeter_LoadRegister(&sLoadRegister);

 #if CHECK_TABLE
  if (ui8Result != EMS_E_OK)
  {
    /* PowerMeter_LoadRegister: errore */
	/*lint -e64*/
    return S_FALSE_PLAT;
    /*lint -e64*/
  }
 #endif

  return EMS_E_OK;
}

void EM_LoadConfigRead(void)
{
  Loader_Struct sResCalibDataStruct = {0, 0};     /* Struct containing the Indesit Plant Cali-Box data */
  Loader_Struct sOutputsMap = {0, 0};             /* Struct containing the Outputs Map */
  Loader_Struct sEnergyDataStruct;                /* Struct containing the Energy Meter data */
  uint8_t       ui8IdxDisplacement;               /* Displacement (that is an index) to access the field �Energy Load i�:
                                                   *    0 -> Energy Load 0
                                                   *    1 -> Energy Load 1
                                                   *    �
                                                   */
#if EMS_USE_TABLE_PARAMD
  uint16_t TableParamD = 50; /* PLOS */
#endif
/* uint16_t SValveAddPower = 50; */




  /* Inizializza il modulo Power Meter */
  PowerMeter_InitSw();

  /* Recupera gli estremi degli Algorithm Parameters */
  if (S_OK_PLAT != BasicLoader(SF_PTR_ENERGY_METER, ALGO_PARAMS_DISPLACEMENT, &sEnergyDataStruct))
  {
    return;
  }

  /* Inizializza i parametri di configurazione del modulo Power Meter */
  PowerMeter_InitConfig(&sEnergyDataStruct
#if EMS_USE_TABLE_PARAMD
                        , &TableParamD                       
#endif
                       );


#if defined(CONFIG_LIB_ADC_CALIBRATION) /* NEW_CALIBRATION_LOADER */

  if (S_OK_PLAT != LIB_CAL_GetCalibData(ID_CAL_PNTR_FACTORY, &sResCalibDataStruct))
  {

  }
#endif

  /* Recupera gli estremi dell'Outputs Map */
    if (S_OK_PLAT != BasicLoader(SF_PTR_IO_CONFIG, DSPL_IO_OUTPUT_MAP_PM, &sOutputsMap))
  {
    /* return; */
  }

  /* Ciclo di accesso a tutte le tabelle energetiche per la registrazione dei carichi */
  for (ui8IdxDisplacement = (ALGO_PARAMS_DISPLACEMENT + 1); ; ++ui8IdxDisplacement)
  {
    /* Recupera gli estremi della tabella energetica al displacement 'ui8IdxDisplacement' */
    if (S_OK_PLAT != BasicLoader(SF_PTR_ENERGY_METER, ui8IdxDisplacement, &sEnergyDataStruct))
    {
      /* BasicLoader: non ci sono pi� carichi oppure errore */
      break;
    }



    /* Registra il carico associato alla tabella energetica */
    if (S_OK_PLAT != EM_LoadRegister(&sEnergyDataStruct, &sResCalibDataStruct, &sOutputsMap))
    {  
      /* EM_LoadRegister: errore */
      break;
    }


  }

}





/* #endif */ /* POWER_METER_SW */

#endif /* CONFIG_PM_ENERGY_METER_SW */
/* *********** */
/* End of file */
/* *********** */
