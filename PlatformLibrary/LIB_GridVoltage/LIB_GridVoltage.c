/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_GridVoltage.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_GridVoltage.c 
\ingroup	LIB_GridVoltage
\date		Jan 12, 2011 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\par\b	History:
\n 
\n 		01.00.00 
\n		Jan 12, 2011 
\n		stefano.ughi 
\n		changes 
\n 			Creation
\n
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "LIB_GridVoltage.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/*----------- Nominal Voltage Detection ---------------*/
/* A/D value in order to distinguish nominal voltage 230V e 110V */
#define VLINE_THRESHOLD     (ADC_ValueType)(((uint32_t)ONE_AD_SAMPLE_MAX_VALUE*V_EFF_TH*SQUARE_ROOT_2*RES_2)/(VCC*(RES_1+RES_2)))

#define VLINE_SHORT_FLT_MAX_VALUE       10
#define VLINE_LONG_FLT_START_VALUE 		128
#define VLINE_MAX_V 	                10
#define VLINE_MIN_V 	                0
#define VLINE_AWG_FILTER_DECISION		100 
/*----------- Nominal Voltage Detection ---------------*/

/*----------- Under and Over Voltage Detection ---------------*/
/** Initial Filter before starting acquisition */
#define VLINE_TH_FLT                    50
#define VLINE_HISTERESYS                10  /* [A/D] */
/*----------- Under and Over Voltage Detection ---------------*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/*----------- Nominal Voltage Detection ---------------*/
static uint8_t VlineChkShortFilter;
static uint8_t VlineChkLongFilter;
static uint8_t VlineCntr_230;
static uint8_t VlineCntr_110;
static uint8_t Vline_awg_filter;
/*----------- Nominal Voltage Detection ---------------*/

/*----------- Under and Over Voltage Detection ---------------*/
/** Low Reference value  */
static uint16_t VLine_Ref_Value_L;
/** High Reference value  */
static uint16_t VLine_Ref_Value_H;
/** Number of crossing under the refl*/
static uint16_t VLine_Number_Of_Crossing_refl;
/** Number of crossing over the refh*/
static uint16_t VLine_Number_Of_Crossing_refh;
/** Initial filter before starting acquisition */
static uint8_t VlineTH_InitalFlt;
/** If TRUE LIB_GV_SetVLineRef() called by Application */
static bool_t VlineRefSetupDone = FALSE;
/** If TRUE Over Voltage Detected, don't update again over voltage counter */
static bool_t OverVoltage;
/** If TRUE Under Voltage Detected, don't update again under voltage counter */
static bool_t UnderVoltage;
/*----------- Under and Over Voltage Detection ---------------*/

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n Set up of eligibility thresholds for V peak [A/D].
\n Called by Application, used for statistics.

\param  [in]	refl lower limit for the eligibility of V peak.
\param  [in]	refh upper limit for the eligibility of V peak.

\author 	Stefano Ughi
\date		Jan 13,2011
\version	1.0.1

\todo       sarebbe opportuno che i valori passati alla funzione fossero in volt e non in A/D
            sebbene questo comporti una modifica del file di setting.
                
\note		\par \b Nota 1:
			documentation by Stefano Ughi
            \par \b Nota 2:
            Ex SetVLineRef
*/
void LIB_GV_SetVLineRef(uint16_t refl, uint16_t refh)
{
    VLine_Ref_Value_L = refl;
    VLine_Ref_Value_H = refh;
    VLine_Number_Of_Crossing_refl = 0;
    VLine_Number_Of_Crossing_refh = 0;    
    OverVoltage = FALSE;
    UnderVoltage = FALSE;    
    VlineTH_InitalFlt = 0;    
    VlineRefSetupDone = TRUE;
}
/**
\n Determination of the number of times that V peak was found out a band of acceptance,
\n the function is used only for statistical purposes.


\author 	Stefano Ughi
\date		Jan 13,2011
\version	1.0.1

\todo       sarebbe opportuno lavorare in volt  
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void ManageVLineRefCrossing(ADC_ValueType vPeakADvalue)
{
    if ( FALSE == VlineRefSetupDone )
    {
        /* LIB_GV_SetVLineRef() not called by Application */
        return;
    }

    if ( VlineTH_InitalFlt < VLINE_TH_FLT )
    {
        VlineTH_InitalFlt++;
        return;
    }

    if ( vPeakADvalue > VLine_Ref_Value_H )
    {
        if ( FALSE == OverVoltage )
        {
            VLine_Number_Of_Crossing_refh++;
            OverVoltage = TRUE;           
        }
    }
    else if ( vPeakADvalue < VLine_Ref_Value_L )
    {
        if ( FALSE == UnderVoltage )
        {
            VLine_Number_Of_Crossing_refl++;
            UnderVoltage = TRUE;    
        }
    }
    else
    {
        /* VLine_Ref_Value_H >= vPeakADvalue >= VLine_Ref_Value_L */
        if ( vPeakADvalue <= (VLine_Ref_Value_H-VLINE_HISTERESYS) )
        {
            OverVoltage = FALSE;    
        }
        if ( vPeakADvalue >= (VLine_Ref_Value_L+VLINE_HISTERESYS) )
        {
            UnderVoltage = FALSE; 
        }
    }
}
/**
Returns the # of times that the V peak is gone below the fixed threshold.

\return 	uint16_t


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint16_t LIB_GV_ReadVLineCrossing_under_refl(void)
{
    return VLine_Number_Of_Crossing_refl;
}

/**
Return the # of times that the V peak is gone above the fixed threshold.

\return 	uint16_t


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint16_t LIB_GV_ReadVLineCrossing_over_refh(void)
{
    return VLine_Number_Of_Crossing_refh;
}
/**
The function is used to set the value of the variable VLine_Number_Of_Crossing_refl (statistics).

\param  [in]	init_value value to be set.

              
\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_GV_SetVLineRefCntInitValueRefL(uint16_t init_value)
{

    VLine_Number_Of_Crossing_refl = init_value;

}
/**
The function is used to set the value of the variable VLine_Number_Of_Crossing_refh (statistics).

\param  [in]	init_value value to be set.

              
\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_GV_SetVLineRefCntInitValueRefH(uint16_t init_value)
{

    VLine_Number_Of_Crossing_refh = init_value;

}

/**
Computes RMS of Voltage 

\param  [in]		measureADvLine V peak [A/D]

\return 	int16_t

\retval		RMS Voltage		 	 

\author 	stefano.ughi 
\date		Jan 13, 2011 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			Ex getRMS_Vac	

*/
int16_t LIB_GV_getRMS_Vac( ADC_ValueType measureADvLine )
{
uint8_t vacPeak_AD_Value;
int16_t vacRMS;
    
	/* vacPeak_AD_Value in 8 bits resolution */
	#if (ADC_RESOLUTION > 8)
	vacPeak_AD_Value = (uint8_t) (measureADvLine >> (ADC_RESOLUTION-8));
	#else
	vacPeak_AD_Value = (measureADvLine << (8-ADC_RESOLUTION));	
	#endif
   
    vacRMS = (int16_t) ((vacPeak_AD_Value*(uint16_t)((VCC*(RES_1+RES_2)*100)/(255*RES_2*SQUARE_ROOT_2))/100)-(uint16_t)((RTN*100)/(SQUARE_ROOT_2*100)));

    return( vacRMS );
}

/**
Setup for LIB_GV_CheckNominalVoltage().
Utilizzata per BackUp da Vline e da modulo PLP.


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			Ex ResetVlineMeasure

*/

void LIB_GV_InitNominalGridVoltageMeasure(void)
{
    VlineCntr_230 = 0;
    VlineCntr_110 = 0;
    VlineChkShortFilter = 0;
    VlineChkLongFilter = VLINE_LONG_FLT_START_VALUE;
    Vline_awg_filter = 0;
}

/**
La funzione ha il compito di determinare la tensione di rete nominale 230 V o 110 V,
la ricerca viene eseguita dinamicamente. Chiamata in TASK(Task_MeasureLive).
Utilizzata per BackUp da Vline e da modulo PLP.

\param  [in]		Vline misura della V di picco in A/D

\attention \b VlineChkShortFilter contatore usato come filtro
\attention \b VlineCntr_230 e \b VlineCntr_110 sono le variabili utilizzate per decidere se
              siamo in presenza di 230V o di 110V
\attention \b VLINE_THRESHOLD  soglia in A/D per determinare la V line 


\return 	bool_t

\retval		TRUE -> misura disponibile \n
            FALSE -> misura non disponibile
             	 

\warning	none		

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Note 2:
			Ex VlineMeasureUpdate

*/
bool_t LIB_GV_CheckNominalVoltage(ADC_ValueType Vline)
{
    /* After syncronization */
    if ( VlineChkShortFilter >= VLINE_SHORT_FLT_MAX_VALUE )
    {
        if (Vline >= VLINE_THRESHOLD)
        {
            /* V LINE is 230V */
            if (VlineChkLongFilter<255)
            {
                VlineChkLongFilter++;
            }
        }
        else
        {
            /* V LINE is 110V */
            if (VlineChkLongFilter>0)
            {
                VlineChkLongFilter--;
            }
        }


        if ((++Vline_awg_filter) == VLINE_AWG_FILTER_DECISION)
        {
            /* Select the VLINE ref for VLINE backup */
            if (VlineChkLongFilter >= VLINE_LONG_FLT_START_VALUE)
            {
                /* V LINE is 230V */
                VlineCntr_230 = VLINE_MAX_V;
                VlineCntr_110 = VLINE_MIN_V;
            }
            else
            {
                /* V LINE is 110V */
                VlineCntr_110 = VLINE_MAX_V;
                VlineCntr_230 = VLINE_MIN_V;
            }
            /* Restart the long filter */
            Vline_awg_filter = 0;
            VlineChkLongFilter = VLINE_LONG_FLT_START_VALUE;
        }

        return TRUE;
    }


    /* First syncronization */

    VlineChkShortFilter++;

    if ( Vline >= VLINE_THRESHOLD )
    {
        VlineCntr_230++;
    }
    else
    {
        VlineCntr_110++;
    }

    return FALSE;
}

/**
Returns the nominal Voltage value.
Utilizzata per BackUp da Vline e da modulo PLP.


\return 	uint8_t

\retval		NOMINAL_VLINE_IS_110V
\retval		NOMINAL_VLINE_IS_230V


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			Ex VlineMeasureType

*/
uint8_t LIB_GV_GetNominalGridVoltage(void)
{

    if ( VlineCntr_110 > VlineCntr_230 )
    {
        return NOMINAL_VLINE_IS_110V; 
    }
    else
    {
        return NOMINAL_VLINE_IS_230V; 
    }
}


/* Local Function */

/* Errors */
#if (VLINE_SHORT_FLT_MAX_VALUE > 255)
#error "\n Max. Value for VLINE_SHORT_FLT_MAX_VALUE is 255"
#endif
#if (VLINE_LONG_FLT_START_VALUE != 128)
#error "\n VLINE_LONG_FLT_START_VALUE must be 128"
#endif
#if (VLINE_TH_FLT > 255)
#error "\n Max. Value for VLINE_TH_FLT is 255"
#endif
/* *********** */
/* End of file */
/* *********** */

