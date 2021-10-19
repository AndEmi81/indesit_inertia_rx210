/*
Copyright (c) 2004-2011, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par    Software Energy Meter
\n
\n
\file     ${PM_EnergyMeterSw.h}
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



#ifndef __EM_SOFTWARE_H
#define __EM_SOFTWARE_H

/* ********** */
/* Includes   */
/* ********** */
#include "PM_EnergyMeterSwExp.h"

/* #include "SkeletonDefines.h" */

/* #include "EM_SoftwareTmp.h" */ /* PLOS - Deleted */

/* ************************ */
/* Functional parameters    */
/* ************************ */


#define DEBUG_WITH_AIDA           1   /* consente il debug con AIDA */

#define INTERRUPT_MANAGE          0   /* use enable/disable interrupt to safe operate on double
                                         buffer */

#define EMS_BUFFER_DOUBLE_USE     0   /* use double buffer management for safe operation on V
                                         under measure and V to use for power calculation */

/* Dimensione (word) del singolo item della Outputs Map */
#define IO_MANAGER_LOAD_TABLE_ITEM_SIZE   4
/* Offset del campo Load_ID nelle strutture della Outputs Map relative ai carichi */
#define LOAD_ID_OFFSET                    (0)

/* Energy Meter: Algorithm parameters displacement */
#define ALGO_PARAMS_DISPLACEMENT          (0x0)

/* #define DSP_POWER_OFFSET      14 */
/* #define DSP_ENERGY_OFFSET     16 */

#define LOAD_STS_ALWAYS_ON    0xFF
#define LOAD_ID_RES_TO_CALIB  0x20

#define DSPL_IO_OUTPUT_MAP_PM         0x03


/* ************************ */
/* Version parameters       */
/* ************************ */


/* ************************ */
/* Configuration parameters */
/* ************************ */



#define EMS_PW_UPDATE_PERIOD    (100) /* the period in msec for the PowerMeter_Accum call */
#if (EMS_PW_UPDATE_PERIOD > 1000) || ((1000 % EMS_PW_UPDATE_PERIOD) != 0)
 #error Invalid EMS_PW_UPDATE_PERIOD value.
#endif

#define EMS_V_AVERAGE_N_SAMPLES (100) /* number of samples for voltage average calculation */

#define EMS_ENERGY_COUNTERS_MAX   (0)

#define EMS_TABLE_PARAM_D_DEFAULT (0)


/* ************************ */
/* Features parameters      */
/* ************************ */
#define EMS_V_PEAK_HOLD           0   /* enable the V peak hold */

#define EMS_V_PEAK_AVERAGE        1   /* enable the V peak average calculation in order to get
                                         the V peak on N samples */

#define EMS_V_RMS_AVERAGE         1   /* enable the V rms average calculation in order to get
                                         the V rms on N samples */
#define EMS_V_RMS_AVERAGE_PHY     1

#define EMS_V_PEAK_THRESHOLD      0   /* enable the V peak threshold measurement */

#define EMS_USE_TABLE_PARAMD      0   /* enable use of parameter D for table loads */

#define EMS_USE_ELAPSED_TIMER     0   /* enable use of elapsed timer */


/* ********************************************* */
/* W-BOX: Indesit Calibration Plant displacement */
/* ********************************************* */
/* PTR_WBOX.INDESIT_PLANT_CALI_DISPLACEMENT */
#define INDESIT_PLANT_CALI_WLEN           1  	/* min size (word) */
/* #define CONFIG_RESISTOR_CALIB_OFFSET      1 	*/ /* Resistor calibration factor offset (byte) */
#define RESISTOR_CALIB_NO_CALIB        0xFF  	/* Resistor calibration factor: no calibration*/
#define RESISTOR_CALIB_DEF_VALUE        128  	/* Default value for Resistor calibration factor */


/* ************************ */
/* Setting File parameters  */
/* ************************ */

/* PTR_ENERGY_METER_DATA.ALGO_PARAMS_DISPLACEMENT  */
#define ALGO_PARAMS_WLEN                  6  /* min size (word) */
#define ALGO_PARAMS_OFFSET_PMAX           0  /* Value [W] of the normalization of the power. For translate normalized to physical values??. */
#define ALGO_PARAMS_OFFSET_EMAX           2  /* Value [Ws] of the normalization of the energy. For translate normalized to physical values??. */
#define ALGO_PARAMS_OFFSET_KENERGY_MANT   4  /* parameter for the calculation of energy in the normalized format: Mantissa */
#define ALGO_PARAMS_OFFSET_KENERGY_EXP    6  /* parameter for the calculation of energy in the normalized format: Exponent */
#define ALGO_PARAMS_OFFSET_TS             7  /* update period of the calculation of energy (expressed in multiples of 100ms) */
#define ALGO_PARAMS_OFFSET_VMAX10         8  /* Maximum voltage measured by the circuit calculated from the partition ratio
                                              * and the supply voltage of the uC. It is multiply by 10. */
#define ALGO_PARAMS_OFFSET_NORM_RTN      10  /* Value of normalized RTN (to subtract from line in voltage) */

/* PTR_ENERGY_METER_DATA. all loads */
#define EM_DATA_HEADER_COMMON_WLEN        2 /* Dimensione (word) dell'header comune a tutti i tipi di tabelle energetiche (le prime 2 words) */
#define EM_DATA_ENERGY_LOAD_TYPE          0
#define EM_DATA_LOAD_TYPE_POSITION        1

/* PTR_ENERGY_METER_DATA.RESISTOR */
#define EM_DATA_RESISTIVE_WLEN            (EM_DATA_HEADER_COMMON_WLEN + 2) /* Dimensione (word) di una tabella energetica Resistive */
#define EM_DATA_RESISTIVE_MANT_IDX        4 /* Posizione (byte), nella tabella, della word contenente il valore della mantissa del carico resistivo */
#define EM_DATA_RESISTIVE_EXP_IDX         6 /* Posizione (byte), nella tabella, della word contenente il valore dell'esponente del carico resistivo */


/* ****************** */
/* Defines and Macros */
/* ****************** */

#define EM_COUNT_STAT_MASK    0x1
#define EM_E_OVERRUN_MASK     0x2
#define EM_T_ELAPSED_MASK     0x8

#if DEBUG_WITH_AIDA
#define STATIC
#else
#define STATIC static
#endif

#define S_OK_PLAT              0x00  /**< Ok */
#define S_FALSE_PLAT           0x01  /**< Busy */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */




/* Enumeration for "Energy Load Type" field in Energy Meter Data Structure */
typedef enum
{
  LOAD_FIRST_ITEM = 1,

  RESISTOR = LOAD_FIRST_ITEM,       /* Energy Load Type = RESISTOR */
  MOTOR,                            /* Energy Load Type = ENERGY (MOTOR) */
  TABLEP,                           /* Energy Load Type = TABLE */

  LOAD_LAST_ITEM = TABLEP
} eLoad_t;


/*lint -e18 Lint bug: suppress unjustified Error 18, fixed on version t of LINT*/
typedef struct
{
  uint8_t   ui8Mask;          /* */
  uint8_t*  pui8Status;       /* pointer to current load status */
  eLoad_t   eType;            /* type of load ie: RESISTOR, MOTOR or TABLE */

  union
  {
    struct
    {
      uint16_t* pui16GMant;   /* pointer to 1/R' (mantissa) into settings file */
      int8_t*   pi8Gexp;      /* pointer to 1/R' (exponent) into settings file */
      uint8_t*  pui8Calib;    /* pointer to Resistor calibration factor */
    } res; /* Resistor params */

    struct
    {
      uint16_t* pui16PVal;    /* pointer were retrieve the current value of Power (from power section) */
      uint32_t* pui32EVal;    /* pointer were retrieve the current value of Energy (from energy section) */
    } mot;  /* Motor params */

    struct
    {
      uint8_t*  pui8Table;    /* pointer to load data table into settings file */
      uint8_t   ui8TableLen;  /* size (word) of load data table */
    } tab;  /* Table params */
  } params;             /**< parameters */
} sLoadRegister_t;
/*lint +e18 */


typedef enum
{
  SINGLE_CONV,
  CONTINUOUS_CONV
} eConvertion_t;


typedef struct
{
  uint16_t  ui16EnergyCons;
  uint16_t  ui16TimeElapsed;
  uint16_t  ui16MaxEnergy10sec;
  uint16_t  ui16EnergyTreshold;
  uint16_t  ui16TimePeriod;
  uint32_t  ui32RawAccum;
  uint32_t  ui32Raw10secAcc;
  uint32_t  ui32RawMaxEnergy10sec;
  uint8_t   ui8Flags;
} stEnergyCounters_t;


/* ****************** */
/* Exported variables */
/* ****************** */

#if defined(CONFIG_RES_CALIBRATION_ENABLE) && CONFIG_NUM_RES_TO_CALIB
extern const sEMS_ResCalWBoxOff_t resCalTab[CONFIG_NUM_RES_TO_CALIB];
#endif

#if EMS_USE_ELAPSED_TIMER
/* Time in seconds elapsed from start of life */
extern  uint32_t b_ui32TimeElapsedSinceStart;    /* NO Backup */
#endif

/* ****************** */
/* Imported functions */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */







/**
  This function set the parameters used for power consumption calculation

\param  [in]      Loader_Struct*  psEnergyAlgoDataStruct    [pointer to struct containing the Energy Meter data]
\param  [in]      uint16_t*       pui16TableParamD          [pointer were retrieve the value of the D param for TABLE load]
\param  [out]     void
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
      Attenzione: deve essere invocata durante l'inizializzazione del sistema.
*/
eEMS_Err PowerMeter_InitConfig(Loader_Struct* psEnergyAlgoDataStruct
#if EMS_USE_TABLE_PARAMD
                               , uint16_t* pui16TableParamD
#endif
                              );



/**

Perform module initialization for a warm reset

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_InitSw (void);



/**
  Not used

\param  [in]      void
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\todo   [This function is empty.To be verified if it's really necessary]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_StartCal (void);



/**
  This function registers a new load used for power consumption calculation

\param  [in]      sLoadRegister_t* psLoadRegister       [ pointer to a struct containing the new load parameters ]
\param  [out]     void
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}
      Attenzione: il modulo Energy Meter, in caso di carichi ADDITIVI non gestisce la compensazione per
      adattare la misura alle variazioni di tensione. Quindi, il tipo ADDITIVO ed ENERGY deve essere usato
      per quei carichi la cui potenza viene misurata in altre parti del sistema come ad es un DSP.
      Per questi carichi si calcoli la resistenza e si inseriscano come carichi RESISTIVI.
*/
eEMS_Err PowerMeter_LoadRegister (sLoadRegister_t* psLoadRegister);



/**
Gives to Energy Meter (EM) the command to start mesuring power.
It's possible to start either a one-shot or a continuous mesuring mode of operation.
In One-shot mode the EM collects N samples of V and P and calculates one value for Vrms and Pavg
and then stops.
In continuous mode the EM simply keeps on performing one measure after one. The new measure ovverides
the old values. I't up to the user not to lose measures.

This function reset the SW accumulator of energy and the timer counter of the time elaspeset since
start measure (These are global vars) if bResetCounters parameter is TRUE.

It sets the accumulator enable flag (global var).

\param  [in]      <C_Conv>         [one shot or continuous conversion]
\param  [in]      <bResetCounters> [tells whether or not to reset energy accumulator and time elapsed counters]
\param  [out]     void
\param  [in,out]  none

\attention \b ui32EnergyCounterWh is the global var used to accumulate energy
           \b ui32TimeElapsedSinceStart is the global var used to count seconds elapsed since start measure.
           \b bAccuEnabled is the accumulation enable flag.


\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_StartMeasure (eConvertion_t C_Conv, bool_t bResetCounters);



/**
Disables the accumulation power meter

\param  [in]      void
\param  [out]     none
\param  [in,out]  none

\attention \b bAccuEnabled is the accumulation enable flag.

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}


*/
eEMS_Err PowerMeter_StopMeasure (void);



/**
 Not implemented

\param  [in]      none
\param  [out]     uint8_t* pui8I [not used]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}


*/
eEMS_Err PowerMeter_GetInstant_I (uint8_t* pui8I);



/**
 Returns the value of the last peak measured Voltage

\param  [in]      none
\param  [out]     uint16_t* pui16V [pointer were retrieve the volt value]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_GetInstant_V (uint16_t* pui16V);




/**
 Returns the value of last measured instant power

\param  [in]      none
\param  [out]     uint16_t* pui16P [the pointer were retrieve the power value]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_GetInstant_P (uint16_t* pui16P);



/**
 Return the value of average power calculated on last N samples (tip. 1 sec).

\param  [in]    void
\param  [out]   uint16_t* pui16P [the pointer were retrieve the power value]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_GetAvgLastCycle_P (uint16_t* pui16P);



/**
Return the value of average power calculated on last N samples (tip. 1 sec) in
hundredths of a watt.

\param  [in]      none
\param  [out]     uint16_t* pui16P [the pointer were retrieve the power value]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning    [none]

\author   Intecs
\date     13-06-2011
\version    1.0.0

\bug        [none]

\todo     [none]

\note     \par \b Nota 1:
 semplice fattore di conversione

*/
eEMS_Err PowerMeter_GetAvgLastCycle_P_100mW (uint16_t* pui16P);



/**
 Not implemented


\param  [in]    none
\param  [out]   uint8_t* pui8I [not used]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]



\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_Get_I_RMS (uint8_t* pui8I);



/**
 Not implemented


\param  [in]    none
\param  [out]   uint32_t* pui32I [not used]
\param  [in,out]  none

\attention \b


\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}


*/
eEMS_Err PowerMeter_Get_I_RMS_100mA (uint32_t* pui32I);



/**
 Return the last rms volt measured.


\param  [in]      none
\param  [out]     uint16_t* pui16V [the pointer were retrieve the volt value]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_Get_V_RMS (uint16_t* pui16V);



/**
 Return the last rms volt measured in hundredths of a volts.


\param  [in]     none
\param  [out]    uint32_t* pui32V [the pointer were retrieve the volt value]
\param  [in,out] none

\attention \b

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_Get_V_RMS_100mV (uint32_t* pui32V);



/**
  Returns the value of energy in Wh since the last start measure.


\param  [in]      none
\param  [out]     uint32_t* pui32EWh [pointer were retrieve the value of the energy]
\param  [out]     uint32_t* pui32TimeElapsed [pointer were retrieve time elapsed fron start]
\param  [in,out]  none

\attention \b Kee is the coefficent used to calculate energy in Wh it is calculated from Kpp and is put as a #define in an header file.

\return   eEMS_Err - error code

\retval   [eEMS_Err]

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_GetAvgSinceStart_E_Wh (uint32_t* pui32EWh, uint32_t* pui32TimeElapsed);







/**
 This function starts couting Energy consumption on the given counter index.

\param  [in]      ui8CounterId - which counter to be started
\param  [in]      pstCounterData - pointer to the data structure that contains all counter informations.
                                   You provide the Time period and Energy thresholds.
\param  [out]     none
\param  [in,out]  none

\attention \par \b  The input struct is copied in a static one inside the module.

\return   eEMS_Err - error code

\retval   [eEMS_Err]  0 = OK
                  1 = Invalid parameters : CounterId out of range , Null pointer

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0


\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_StartEnergyCount (uint8_t ui8CounterId, stEnergyCounters_t* pstCounterData);



/**
 This function return information on the given counter

\param  [in]      ui8CounterId - which counter to be started
\param  [out]     pstCounterData - pointer to the data structure that contains all counter informations.
                                   It gives back the energy conter value (Wh), the time elapsed (s) since start and the MAX energy consumed in a 10 seconds time slice.
                                   It checks if the energy counter reaches the given threshold in a given time period and the check result is givewn back through the flag field.
                                   Other provided flags are the running state of the counter (started/stopped) and the running state of the timer (Time period elapsed or not).
\param  [in,out]  none

\attention

\return   eEMS_Err - error code

\retval   [none]  0 = OK
                  1 = Invalid parameters: CounterId out of range , Null pointer

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_GetEnergyCount (uint8_t ui8CounterId, stEnergyCounters_t* pstCounterData);



/**
 This function stops couting Energy consumption on the given counter index.

\param  [in]      ui8CounterId - which counter to be stopped
\param  [out]     none
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]  0 = OK
                  1 = Invalid parameters : CounterId out of range

\warning  [none]

\author   Intecs
\date     13-06-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}

*/
eEMS_Err PowerMeter_StopEnergyCount (uint8_t ui8CounterId);

/*
#if (IO_MANAGER_BASICLOADER_CSTM == TRUE)
extern uint8_t BasicLoader( uint16_t EepromPointer, uint8_t Displacement, struct Loader *p );
#else
uint8_t BasicLoader( uint16_t EepromPointer, uint8_t Displacement, struct Loader *p );
#endif
*/



#endif /* __EM_SOFTWARE_H */


/* *********** */
/* End of file */
/* *********** */
