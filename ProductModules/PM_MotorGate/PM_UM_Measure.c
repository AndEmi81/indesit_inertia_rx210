/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Measure.c
\brief   Il file contiene le funzioni per le misure implementate.
         Le misure possono essere escluse in compilazione, vedi CONFIG_PM_UM_MOTOR_MEASURE nel configuratore
\ingroup PM_MotorGate
*/

/**
\addtogroup PM_MotorGate
*@{*/


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

#ifdef CONFIG_PM_MOTORGATE

#include <string.h>
#include <stdlib.h>
#include "PM_UniversalMotorExp.h"
#include "PM_UM_UniversalMotor.h"
#include "PM_UM_Measure.h"
#include "PM_UM_GetParams.h"
#include "LIB_Utilities.h"
#include "LIB_ZeroCrossingExp.h"

#ifdef CONFIG_PM_UM_MOTOR_MEASURE


/* *************************** */
/* Local Data 				   */
/* *************************** */
/* Numero campionamenti dell'angolo di parzializzazione di fase per:
  -algoritmo taglio movimentazione
  -algoritmo antischiuma
*/

static const uint8_t numSamplesAlpha[PM_UM_NUM_MEASURES] = {NUM_SAMPLES_ALPHA_PHASE,NUM_SAMPLES_ALPHA_ANTI_FOAM,NUM_SAMPLES_ALPHA_PHASE,NUM_SAMPLES_SPEED_LIM};


static uint8_t 	getPhaseSamples (void);
static uint8_t 	getPhaseSamplesForSpeedLim (void);
static int8_t	period_deltacompensation=0;
static uint32_t	period_compensation;

/* *************************** */
/* Extern Data 				   */
/* *************************** */

extern uint8_t  *pcommmonparam;

/* ***************** */
/* Extern function */
/* ***************** */


/* ***************** */
/* Exported function */
/* ***************** */

/** Inizializza la misura enginePhase e antiFoam per il motore universale.
\param  [in]    	whichMeasure, 0 -> enginePhase 1 -> antiFoam
\param  [in.out]    engine_measures[]

\return 	none
\retval 	none
*/
void engineMeasureInit( PM_UM_Meas_t whichMeasure )
{
    (void)memset( &engine_measures[whichMeasure], 0, sizeof(engine_measures[whichMeasure]) );
}

/** Interrompe la misura phaseEngine o antiFoam.

\param  [in]    whichMeasure, 0 -> phaseEngine 1 -> antiFoam

\param  [out]   engine_measures[whichMeasure].on_off
				engine_measures[whichMeasure].result

\retval 	none
*/
void switchOffEngineMeasure( PM_UM_Meas_t whichMeasure )
{
    engine_measures[whichMeasure].on_off = ENGINE_MEASURE_IS_OFF;
    engine_measures[whichMeasure].result = PM_UM_MEASURE_IS_NOT_READY;
}

/** Procedura di calcolo per l'innesco degli algoritmi
a) riduzione del tempo di ON del motore
b) antischiuma
SOLO MOTORE UNIVERSALE !!!!
Se la procedura e' attiva, viene effettuata la media dell'angolo di fase su numSamplesAlpha[whichMeasure]
campioni, e il risultato viene confrontato con un valore di riferimento determinato in
base alla V line.

\param	[in,out] engine_measures[whichMeasure] typedef engine_measures_t

\return none
*/

void engineMeasures( PM_UM_Meas_t whichMeasure )
{
    getEngineData_t engineData;
    uint8_t phase_samples=0;

	if (PM_UM_MEASURE_SPEED_LIM == whichMeasure)
	{
		phase_samples = getPhaseSamplesForSpeedLim();
	}
	else
	{
		phase_samples = getPhaseSamples();
	}

	if(phase_samples == 0)
	{
		phase_samples = numSamplesAlpha[whichMeasure];
	}


	period_deltacompensation = Get_common_MCUOscCompDelta();

    if( engine_measures[whichMeasure].on_off == ENGINE_MEASURE_IS_ON )
    {
        if( engine_measures[whichMeasure].num_samples_alpha < phase_samples )
        {
            engine_measures[whichMeasure].sum_alpha += getAlpha_n();

            engine_measures[whichMeasure].num_samples_alpha ++;
        }
        if( engine_measures[whichMeasure].num_samples_alpha >= phase_samples )
        {
            PM_UM_GetMotorData(whichMeasure, &engineData);
            if( engineData.error == FALSE )
            {
            	uint32_t average;
            	uint32_t periodcomp;

            	average = (engine_measures[whichMeasure].sum_alpha/phase_samples);

                period_compensation = (uint32_t)((int32_t)1000 - (int32_t)period_deltacompensation);
                periodcomp = (LIB_ZC_GetPeriodoMedio()*period_compensation)/1000;

                if((LIB_ZC_GetPeriodoMedio() > (uint32_t)0) && ((periodcomp) != LIB_ZC_GetPeriodoReteNominale()))
                {
                	average = average+NOMINAL_TO_STD((LIB_ZC_GetPeriodoReteNominale()));
                    average=((average * LIB_ZC_GetPeriodoReteNominale())/(periodcomp)) - NOMINAL_TO_STD(LIB_ZC_GetPeriodoReteNominale());
                }
                engine_measures[whichMeasure].average=average;

                engine_measures[whichMeasure].alphalimit = engineData.dato;

                if( engine_measures[whichMeasure].average > engineData.dato )
                {
                    engine_measures[whichMeasure].result = PM_UM_MEASURE_IS_UP;
                }
                else
                {
                    engine_measures[whichMeasure].result = PM_UM_MEASURE_IS_DOWN;
                }
            }
            else
            {
                engine_measures[whichMeasure].result = PM_UM_MEASURE_IS_NOT_READY;
            }
            engine_measures[whichMeasure].sum_alpha = 0;
            engine_measures[whichMeasure].num_samples_alpha = 0;

        }
    }
    else
    {
        engine_measures[whichMeasure].result = PM_UM_MEASURE_IS_NOT_READY;
    }
}

/** Ritorna l'angolo medio di parzializzazione. */
uint8_t PM_UM_GetPhaseAverage(PM_UM_Meas_t whichMeasure, uint16_t * value )
{
	uint8_t retVal = PM_UM_MEASURE_IS_NOT_READY;

	if (whichMeasure < PM_UM_NUM_MEASURES)
	{
	    if( engine_measures[whichMeasure].result != PM_UM_MEASURE_IS_NOT_READY )
	    {
	    	*value = engine_measures[whichMeasure].average;
	    	retVal = engine_measures[whichMeasure].result;
	    }
	}
    return retVal;
}




static uint8_t getPhaseSamples (void)
{
	return Get_common_phase_samples(pcommmonparam);
}


static uint8_t getPhaseSamplesForSpeedLim (void)
{
struct Loader loader;
uint8_t val=0;

  		if (BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, PM_UM_TORQUE_LIM_TAB_DISPL, &loader) )
		{
			/*speedLimTab = loader.Data;*/
			val= *(loader.Data+2);
		}
		else
		{
		}
  	return val;
}

#endif /* CONFIG_PM_UM_MOTOR_MEASURE */
#endif /* CONFIG_PM_MOTORGATE */

/* *********** */
/* End of file */
/* *********** */
/**@}*/
