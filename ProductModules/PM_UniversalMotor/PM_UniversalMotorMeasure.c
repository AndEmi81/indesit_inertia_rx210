/*
Copyright (c) 2012, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\par    Universal Motor measure
\n
\n
\file    PM_UniversalMotorMeasure.c
\brief   Measures
\ingroup PM_UniversalMotor
*/

#define MODULE_BODY_PM_UNIVMOTORMEASUER

/* ********** */
/* Includes   */
/* ********** */
#include "Config.h"

#if defined(CONFIG_PM_UM_MEASURE)
#if defined(CONFIG_PM_MOTORGATE)
#include "PM_UM_UniversalMotor.h"
#endif
#include "PM_UniversalMotorMeasure.h"
#include "LIB_UTilities.h"
#include "Main.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
Formula errors
*/

/* **************** */
/* Global variables */
/* **************** */

#if defined(CONFIG_PM_MOTORGATE)
extern uint8_t  *pHalfFieldParam;
#endif

/** measures*/
/*
 * Carica il puntatore ai dati della formula da applicare chiama la funzione che effettua
il calcolo.

\param  [in] formulaid, id della formula
\param  [in] pHalfFieldParam (static) puntatore al campo dati del mezzo campo
\param [out] engineData, type getEngineData_t

\retval  engineData->error = TRUE -> engineData->dato inconsistente !!
*/


/**
applica la formula:

   y = m_n/m_d*Vacref + q	  m_d != 0

\param [in]  pparamset, puntatore ai parametri.
\param [in]  VoltageRMS, Volt

\return error (FORMULA_PARAM_ERR_OK nessun errore)
\retval in result

\author	Roberto Fioravanti
\date		15/05/2006
\version	1.0.0

\note   - effettua il test se il denominatore m_d e'diverso da 0.

		12/06/2006 Stefano Ughi
		- se l'applicazione implementa sia la gestione del motore universale che
		  del motore trifase e se il target e' il motore trifase VoltageRMS
		  non ha senso al fine della misura in quanto la coppia  � indipendente
		  dalla Vline, in tal caso l'Editor impostera' m_n = 0 e Vacref a 0xFF
		  in modo che qualsiasi sia il valore di VoltageRMS pparamset punti sempre
		  alla prima quaterna di valori m_n,m_d,q e che il risultato della formula
		  sia sempre q.
		- se l'applicazione implementa solo la gestione del trifase pparamset punta
		  sempre alla prima quaterna di valori m_n,m_d,q e il risultato della formula,
		  con m_n = 0, e' sempre q per qualsiasi valore di VoltageRMS.


*/
void PM_UM_GetMotorData( PM_UM_Meas_t formulaid, getEngineData_t *engineData)
{
    uint16_t result;
    struct Loader loader;
	uint8_t tmpDispl = 0xff;
    loader.DataLenght = 0;

    engineData->error = TRUE;

    switch( formulaid )
    {
#if defined(CONFIG_PM_MOTORGATE)
    case PM_UM_MEASURE_APLHA_MIN:
        /* motore universale angolo minimo accensione triac soft start f(Vline) */
#if !defined (CONFIG_PARAM_FROM_FIRMWARE)
    	tmpDispl = PM_UM_ALPHA_MIN_SOFT_START_DISPL;
#endif
        break;

    case PM_UM_MEASURE_MID_FIELD:

#if !defined(CONFIG_PARAM_FROM_FIRMWARE)
    {
        /*  motore universale attivazione mezzo campo
        	questo ID viene chiamato sempre se presente mezzo campo su motore universale
           	per evitare caricamenti in continuo si sfrutta pHalfFieldParam
            inizializzato nella PM_UM_InitSync()
        */
    	if (pHalfFieldParam != NULL)
    	{
            loader.Data = (pHalfFieldParam + MID_FIELD_DATA_OFFSET);
            loader.DataLenght = !0;
    	}
    }
#endif

        break;

    case PM_UM_MEASURE_SPEED_LIM:
#if !defined(CONFIG_PARAM_FROM_FIRMWARE)

/* questa tabella può non esserci */
    if (!(BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, PM_UM_TORQUE_LIM_TAB_DISPL, &loader)))
	{
    	uint8_t *p;
    	uint8_t end;
    	uint8_t i=0;

		p = loader.Data;
		end = (*(p+(*p)))-1;
		p += *p + 1;

		while ( (!(PM_UM_GetTargetSync() <= (((uint16_t)(*p)*10))) ) && ( i <= end) )
		{
			p++;	i++;
    	}

		/* viene caricato l'indirizzo inziale del gruppo mx+q relativo al range corrente di velocità target
		i --> seleziona il gruppo mx+q da caricare
		dall'indirizzo iniziale della tabella si deve aggiungere il numero dati del campo fisso, il numero dati del campo
		variabile delle velocità +1 (per saltare il numero dati del campo variabile mx+q), e il prodotto i*SIZE_ENGINE_DATA_BLOCK
		in modo di raggiungere il gruppo mx+q corretto */

		p = loader.Data;
		loader.Data = p + (*p) + (*(p+(*p))+1) + i*SIZE_ENGINE_DATA_BLOCK;
		loader.DataLenght = end*SIZE_ENGINE_DATA_BLOCK;

		}
		else
		{
			loader.DataLenght = 0;
		}

#else
loader.DataLenght = 0;
#endif
    	break;
#endif /*defined(CONFIG_PM_MOTORGATE)*/

    case PM_UM_MEASURE_PHASE:
        /* 3ph , brush */
#if !defined(CONFIG_PARAM_FROM_FIRMWARE)
    	tmpDispl = PM_UM_ENGINE_PHASE_DISPL;
#endif
        break;

    case PM_UM_MEASURE_HL_PHASE:
        /* 3ph , brush */
#if !defined(CONFIG_PARAM_FROM_FIRMWARE)
        tmpDispl = PM_UM_ENGINE_HL_PHASE_DISPL;
#endif
        break;

    case PM_UM_MEASURE_ANTIFOAM:
        /* 3ph, brush */
#if !defined(CONFIG_PARAM_FROM_FIRMWARE)
        tmpDispl = PM_UM_ANTIFOAMING_DISPL;
#endif
        break;

    default:
        /* ID non previsto */
        break;
    }

    if (tmpDispl != 0xff)
    {
        (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, tmpDispl, &loader);
    }


    if(loader.DataLenght)
    {
        if( ApplyFormulaParam(loader.Data,&result) == FORMULA_PARAM_ERR_OK )
        {
            /* trattamento del risultato della formula  */

            engineData->dato = result;
            engineData->error = FALSE;
        }
    }

}


PM_UM_FormulaError_Type ApplyFormulaParam(const uint8_t * pparamset, uint16_t * result)
{
	PM_UM_FormulaError_Type  error;
    uint8_t  m_n;
    uint8_t  m_d;
    uint8_t  Vacref;
    uint8_t  found;
    uint16_t q;
    uint16_t VoltageRMS;

    /*
    	N.B. la consistenza della variabile VoltageRMS � assicurata dal fatto che la
       	chiamata ApplyFormulaParam() (per il motore universale) avviene all'interno del
       	TASK Task_UniversalMotorSupervisor NON INTERROMPIBILE
    */

    VoltageRMS = (uint16_t) Main_GetLiveValue_Rms();

    if( VoltageRMS >= 0x0100 )
    {
        VoltageRMS = 255;
    }


    found = FALSE;
    while( found == FALSE )
    {
        Vacref = *(pparamset + VAC_OFFSET);

        if( (uint8_t)VoltageRMS > Vacref )
        {
            pparamset+=SIZE_ENGINE_DATA_BLOCK;
        }
        else
        {
            found=TRUE;
        }
    }


    /* lettura parametri */
    m_n = *pparamset;
    m_d = *(pparamset + M_D_OFFSET);
    q   = *( (uint16_t *)pparamset + Q_OFFSET);

    /* calcolo formula */
    error = FORMULA_PARAM_ERR_m_d;
    if(m_d)
    {
        *result = (( VoltageRMS * m_n)/m_d) + q;
        error = FORMULA_PARAM_ERR_OK;
    }
    return error;
}


engine_measures_t engine_measures[PM_UM_NUM_MEASURES] =
{
    {0,0,0, PM_UM_MEASURE_IS_OFF, PM_UM_MEASURE_IS_NOT_READY, 0},
    {0,0,0, PM_UM_MEASURE_IS_OFF, PM_UM_MEASURE_IS_NOT_READY, 0},
    {0,0,0, PM_UM_MEASURE_IS_OFF, PM_UM_MEASURE_IS_NOT_READY, 0},
#if defined(CONFIG_PM_MOTORGATE)
    {0,0,0, PM_UM_MEASURE_IS_OFF, PM_UM_MEASURE_IS_NOT_READY, 0}
#endif
};

/* ****************** */
/* Local variables    */
/* ****************** */

/* ***************** */
/* Extern function   */
/* ***************** */


/* **************************** */
/* Prototype of local functions */
/* **************************** */
/* ****************** */
/* Exported functions */
/* ****************** */

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT

/** this method provides torque to the module (only for external board)

\param  [in]    whichMeasure,
				0 -> torque per il motore trifase
				1 -> antiFoam per il trifase
\param  [in]   torqueMeasure torque
\return 	none
\retval 	none
\author 	Stefano Ughi
\date		12/06/2006
\version	1.0.0
*/
void PM_UM_SetTorque( uint16_t torqueMeasure, uint8_t whichMeasure )
{
    getEngineData_t engineData;

	if(whichMeasure < PM_UM_NUM_MEASURES)
	{
		uint8_t tmpResult = PM_UM_MEASURE_IS_NOT_READY;

		PM_UM_GetMotorData(whichMeasure, &engineData);
		if ( engineData.error == FALSE )
		{
			if ( torqueMeasure <= engineData.dato )
			{
				/* control effort  O.K. */
				tmpResult = PM_UM_MEASURE_IS_UP;
			}
			else
			{
				/* control effort  K.O. */
				tmpResult = PM_UM_MEASURE_IS_DOWN;
			}
		}

		engine_measures[whichMeasure].result = tmpResult;
	}
}
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT defined */

/** returns the measure
\param  [in]    whichMeasure
\return 	uint8_t
\retval 	ENGINE_MEASURE_IS_NOT_READY -> misura non disponibile,
		ENGINE_MEASURE_IS_UP -> misura sopra la spezzata,
		ENGINE_MEASURE_IS_DOWN -> misura sotto la spezzata.

\author 	Stefano Ughi
\date		06/06/2006
\version	1.0.0
*/
PM_UM_MeasureResult_Type PM_UM_GetMotorMeasures( PM_UM_Meas_t whichMeasure )
{
    return (PM_UM_MeasureResult_Type)(engine_measures[whichMeasure].result);
}

/**
Loads the pointer to data and computes the control effort

\param [in] formulaid, id della formula
\param [in] pHalfFieldParam (static) puntatore al campo dati del mezzo campo

\return  engineData, type getEngineData_t

\retval  engineData->error = TRUE -> engineData->dato inconsistente !!

\author	Roberto Fioravanti

\date 	16/05/2006

\bug

\note   check done

*/


#endif /* CONFIG_PM_UM_MEASURE */


/* *********** */
/* End of file */
/* *********** */
