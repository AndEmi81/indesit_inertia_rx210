/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Control.c
\brief   Implementazione di un controllo motore collettore per lavabiancheria
	algoritmo attualmente implementato sulle lavabiancheria Indesit
\ingroup PM_MotorGate 
\author  Stefano Ughi, Roberto Fioravanti, Marco Sperandini
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
#include <string.h>
#include <stdlib.h>
#include "RTOS.h"
#include "LIB_Utilities.h"
#include "LIB_ZeroCrossingExp.h"
#include "DigitalOutputs.h"
#include "PM_UM_Control.h"
#include "PM_UM_Measure.h"
#include "PM_UM_Tachometer.h"
#include "PM_UM_MotorGateDriver.h"
#include "PM_UM_UniversalMotor.h"
#include "PM_UM_GetParams.h"
#include "LIB_Trace.h"

#ifndef CONFIG_PM_MOTORGATE_PID_CONTROL

/* ***************** */
/* Exported function */
/* ***************** */

/** Calcola il nuovo angolo di parzializzazione di fase

\param inControlloData type inControlloreData_t   (cfr. Controllo_motore_collettore.h)
\param inControlloreVariables type inControlloreVariables_t   (cfr. Controllo_motore_collettore.h)
\param outControllore type outControllore_t (cfr. Controllo_motore_collettore.h)

Algoritmo: alpha_n = alpha_n_1 * ( ST + (CD * tau_n_1 * Vt) - (CPD * tau_n * Vt)) / 2el(rFactor)

     11/12/2006 Alberto Camerin
           Codice del controllore di Stefano U./Roberto F. portato a 32bits,
	         non viene usato il fattore (2el(rFactor) in quanto compreso neglik arrotondamenti dei calcoli
           Non viene pi� utilizzata la libreria matematica. Tutti i conti a 4byte con arrotondamenti
           NB: IPOTESI IMPORTANTE  -------------> r=32 !!!!!!!!

*/
void controllore(inControlloreData_t inControlloData, inControlloreVariables_t inControlloreVariables, outControllore_t *outControllore )
{
    unsigned long long int res1;
    unsigned long long int res2;
    unsigned long long int tot;
    static unsigned long long ST;
    static unsigned long long int CD;
    static unsigned long long int CPD;

    /*
    Inizializzazione default uscita controllore
    */
    outControllore->alpha = 0xFFFF;
    outControllore->error = CONTROLLORE_ALPHA_TOO_LARGE_ERROR;

    CD = (unsigned long int)(*(uint16_t *)inControlloData.pCD);

    /*
    CD*tau_n_1
    */
    res1 = CD * (unsigned long long int)inControlloreVariables.tau_n_1;
    /*
    CD*tau_n_1*Vt
    */
    res1 = res1 * inControlloreVariables.vTarget;

    
    ST= (*((unsigned long long int*)inControlloData.pST)) >> 16;

    /*
    ST+CD*tau_n_1*Vt
    */
    res1 = res1 + ST;


    CPD =  ((unsigned long int)(*((uint16_t *)inControlloData.pCPD)));

    /*
    CPD*tau_n
    */
    res2 = CPD * (unsigned long long int)inControlloreVariables.tau_n;
    
    /*
    CPD*tau_n*Vt
    */
    res2 = res2 * inControlloreVariables.vTarget;

    /*
    ST+CD*tau_n_1*Vt-CPD*tau_n*Vt
    */
    if(res1 < res2)
    {
        outControllore->error = CONTROLLORE_DIFFERENZA_ERROR;
    }
    else
    {
        /*
        alpha_n_1*(ST+CD*tau_n_1*Vt-CPD*tau_n*Vt))
        */
        tot = (res1 - res2);
        tot = tot * inControlloreVariables.alpha_n_1;
        tot = tot >> 32;

        if( tot <= 0xFFFF)
        {
            outControllore->alpha = (uint16_t)tot;
            outControllore->error = CONTROLLORE_NO_ERROR;
        }
        else
        {
        	outControllore->error = CONTROLLORE_ALPHA_TOO_LARGE_ERROR;
        }

    }

}
#endif  /* !CONFIG_PM_MOTORGATE_PID_CONTROL */


/** Filtra l'angolo calcolato dalla funzione outControllore_t controllore(inControllore_t inControlloParam)

\param  outControllore_t
\param  alpha_n_1	in tick (x 1.6 us)
\param  filterParamsPtr,
\return outFiltersAlpha_t

Nota 1: si assume che filterParams.maxAlpha>=alpha_n_1>=filterParams.minAlpha
attenzione quindi all'inizializzazione di alpha_n_1

Nota 2: se il nuovo angolo non e' in range viene applicata la massima variazione
consentita
19/10/2005 Stefano Ughi
*/
void filterNewAlpha( outControllore_t outControllore, const filterParams_t *filterParamsPtr,  uint16_t alpha_n_1_, outFiltersAlpha_t* pOut)
{
    uint16_t          newAlphaOutOfRange;
    
    uint16_t alpha = NOMINAL_TO_STD(outControllore.alpha);
    uint16_t alpha1 = NOMINAL_TO_STD(alpha_n_1_);
    
    if(pOut)
	{
		/*
		Valori di default dell'uscita
		*/
		pOut->alpha = alpha1;
		pOut->boundary = FILTER_NEW_ALPHA_ALPHA_SAME_PREVIOUS;

		if( outControllore.error == CONTROLLORE_NO_ERROR )
		{
			if( alpha < alpha1 )
			{
				/*
				Assumendo alpha_n_1 >= filterParams.minAlpha
				e filterParams.maxDev <= filterParams.minAlpha
				(alpha_n_1-filterParams.maxDev) >= 0

				N.B.: outControllore.alpha potrebbe risultare < filterParams.minAlpha
				ma agira' il filtro sotto
				*/
				newAlphaOutOfRange = (alpha1 - filterParamsPtr->maxDev);

				if( alpha > newAlphaOutOfRange )
				{
					pOut->alpha    = alpha;
					pOut->boundary = FILTER_NEW_ALPHA_ALPHA_IN_RANGE;
				}
				else
				{
					/*  eccede la variazione consentita
					Applica ad alpha la massima variazione negativa consentita
					*/
					pOut->alpha    = newAlphaOutOfRange;
					pOut->boundary = FILTER_NEW_ALPHA_ALPHA_OUT_DEV;
				}
			}
			else
			{
				/*
				case: outControllore.alpha >= alpha_n_1

				Assumendo alpha_n_1 <= filterParams.maxAlpha
				e filterParams.maxDev + filterParams.maxAlpha <= 0xFFFF
				(alpha_n_1+filterParams.maxDev) � in un uint16

				N.B.: outControllore.alpha potrebbe risultare > filterParams.maxAlpha
				ma agira' il filtro sotto
				*/

				newAlphaOutOfRange = (alpha1 + filterParamsPtr->maxDev);

				if( alpha < newAlphaOutOfRange )
				{
					pOut->alpha = alpha;
					pOut->boundary = FILTER_NEW_ALPHA_ALPHA_IN_RANGE;
				}
				else
				{
					/* eccede la variazione consentita
					Applica ad alpha la massima variazione positiva consentita
					*/
					pOut->alpha    = newAlphaOutOfRange;
					pOut->boundary = FILTER_NEW_ALPHA_ALPHA_OUT_DEV;
				}
			}
		}
		/*
		Controlla se nuovo angolo e' compreso tra il valore massimo e il valore minimo
		*/
		if( pOut->alpha > filterParamsPtr->maxAlpha )
		{
			pOut->alpha    = filterParamsPtr->maxAlpha;
			pOut->boundary = FILTER_NEW_ALPHA_ALPHA_MAX_VALUE;    /* eccede il valore massimo */
		}
		else
		{
			if( pOut->alpha < filterParamsPtr->minAlpha )
			{
				pOut->alpha    = filterParamsPtr->minAlpha;
				pOut->boundary = FILTER_NEW_ALPHA_ALPHA_MIN_VALUE; /* eccede il valore minimo */
			}
		}
	}
}
#endif /* CONFIG_PM_MOTORGATE */
/**@}*/
/* *********** */
/* End of file */
/* *********** */
