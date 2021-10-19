/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	[module_name_slave] 
@{
\Par		Trash.c
\n			[Module's detailed description]
@}
\ingroup   	[module_name_master] 
*/
/**
\n
\Par		Trash.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		Trash.c 
\ingroup	[module_name_slave]
\date		Jul 7, 2010 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n 
\n 		01.04.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "StandardDefines.h"
#include "LIB_UTilities.h"
#include "Trash.h"
#include "TrashExp.h"
#include "PlatformConfig.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

#define VLINE_CHK_LONG_START_VALUE 		128
#define VLINE_TH_CHK 	  				10


/* Valore di soglia per distinguere fra 220V e 110V */
#define VLINE_TH_110_220    (ADC_ValueType)(((uint32_t)ONE_AD_SAMPLE_MAX_VALUE*V_EFF_TH*SQUARE_ROOT_2*RES_2)/(VCC*(RES_1+RES_2)))
#define VLINE_AWG_FILTER_DECISION		100 /* 2seconds filter */
#define VLINE_MAX_V 	10
#define VLINE_MIN_V 	0
#define V_LINE_ATD_TH_220V       ((VRMS_BACKUP_220V*SQUARE_ROOT_2+RTN)*RES_2*ONE_AD_SAMPLE_MAX_VALUE)/(VCC*(RES_1+RES_2))  
#define V_LINE_ATD_TH_110V       ((VRMS_BACKUP_100V*SQUARE_ROOT_2+RTN)*RES_2*ONE_AD_SAMPLE_MAX_VALUE)/(VCC*(RES_1+RES_2))  


/*lint -esym(750,VLINE_REF_LOW_DEFAULT) VLINE_REF_LOW_DEFAULT
  used under some options of compilation */
#define VLINE_REF_LOW_DEFAULT			0
/*lint -esym(750,VLINE_REF_HIGH_DEFAULT) VLINE_REF_HIGH_DEFAULT
  used under some options of compilation */
#define VLINE_REF_HIGH_DEFAULT			1025
/*lint -esym(750,DELTA_VLINE_REF_VALUE_DEAFAULT) DELTA_VLINE_REF_VALUE_DEAFAULT
  used under some options of compilation */
#define DELTA_VLINE_REF_VALUE_DEAFAULT	10

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/* Var to chk the VLINE measure*/
static uint8_t VLINE_220 = 0;
static uint8_t VLINE_110 = 0;
static uint8_t VLINE_CHK = 0;

static uint8_t  VLINE_CHK_LONG   = 0;
static uint8_t  VLINE_awg_filter = 0;

#if (CHECK_VLINE_CROSSING == 1)

/*! Counter of the number of crossing under the refl*/
static uint16_t VLine_Number_Of_Crossing_refl = 0;
/*! Counter of the number of crossing over the refh*/
static uint16_t VLine_Number_Of_Crossing_refh = 0;
/*! Low Reference value  */
static uint16_t VLine_Ref_Value_L = VLINE_REF_LOW_DEFAULT;
/*! High Reference value  */
static uint16_t VLine_Ref_Value_H = VLINE_REF_HIGH_DEFAULT;
/*! Delta Reference value for the crossing filter  */
static uint8_t Delta_VLine_Ref_Value = DELTA_VLINE_REF_VALUE_DEAFAULT;
/*! Reference old value  */
static uint16_t old_value = 0;
/*! Reference old value  */
static uint8_t initial_filter = 0;

#endif /* #if (CHECK_VLINE_CROSSING == 1) */

/* Soglie per backup VLINE */
const ADC_ValueType VLineAtdTh[2]=
{
    V_LINE_ATD_TH_110V,     /* 110V */
    V_LINE_ATD_TH_220V      /* 220V */
};
#if defined(WBOX) && (CALIBRATION_ENABLE == 1)
static struct Loader CalibrationDataPtr_ElectSupp;
static struct Loader CalibrationDataPtr_Factory;
#endif /* #if defined(WBOX) && (CALIBRATION_ENABLE == 1) */


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
/**
Inizializzazioni per la funzione VlineMeasureUpdate().


\attention \b   VLINE_220   setup
\attention \b   VLINE_110   setup
\attention \b   VLINE_CHK   setup
\attention \b   VLINE_CHK_LONG  setup
\attention \b   VLINE_awg_filter    setup


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void ResetVlineMeasure(void)
{
    VLINE_220 			= 0;
    VLINE_110 			= 0;
    VLINE_CHK 			= 0;
    VLINE_CHK_LONG 		= VLINE_CHK_LONG_START_VALUE;
    VLINE_awg_filter 	= 0;
}
/**
La funzione ha il compito di determinare la tensione di rete 220 V o 110 V,
la ricerca viene eseguita dinamicamente. Chiamata in TASK(Task_MeasureLive).

\param  [in]		Vline misura della V di picco in A/D

\attention \b VLINE_CHK contatore usato come filtro
\attention \b VLINE_220 e \b VLINE_110 sono le variabili utilizzate per decidere se
              siamo in presenza di 220V o di 110V
\attention \b VLINE_TH_110_220  soglia in A/D per determinare la V line 


\return 	uint8_t

\retval		1 -> misura disponibile \n
            0 -> misura non disponibile
             	 

\warning	none		

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t VlineMeasureUpdate(ADC_ValueType Vline)
{

    /* After syncronization */
    if ( VLINE_CHK >= VLINE_TH_CHK )
    {
        if (Vline >= VLINE_TH_110_220)
        {
            /* VLINE 220V */
            if (VLINE_CHK_LONG<255)
            {
                VLINE_CHK_LONG++;
            }
        }
        else
        {
            /* VLINE 110V */
            if (VLINE_CHK_LONG>0)
            {
                VLINE_CHK_LONG--;
            }
        }


        if ((++VLINE_awg_filter) == VLINE_AWG_FILTER_DECISION)
        {
            /* Select the VLINE ref for VLINE backup */
            if (VLINE_CHK_LONG >= VLINE_CHK_LONG_START_VALUE)
            {
                /* VLINE 220V */
                VLINE_220 = VLINE_MAX_V;
                VLINE_110 = VLINE_MIN_V;
            }
            else
            {
                /* VLINE 110V */
                VLINE_110 = VLINE_MAX_V;
                VLINE_220 = VLINE_MIN_V;
            }
            /* Restart the long filter */
            VLINE_awg_filter = 0;
            VLINE_CHK_LONG   = VLINE_CHK_LONG_START_VALUE;
        }

        return 1;
    }


    /* First syncronization */

    VLINE_CHK++;


    if ( Vline >= ((ADC_ValueType)VLINE_TH_110_220) )
    {
        VLINE_220++;
    }
    else
    {
        VLINE_110++;
    }

    return 0;
}
/**
Ritorna il valore della tensione di alimentazione.


\attention \b VLINE_110 used as input \n
		   \b VLINE_220 used as input

\return 	uint8_t

\retval		
            VLINE_110V \n
 	 		VLINE_220V


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint8_t VlineMeasureType(void)
{

    if (VLINE_110 > VLINE_220)
    {
        return VLINE_110V; 
    }
    else
    {
        return VLINE_220V; 
    }

}
/**
Seleziona la soglia per il Backup da VLINE.

\param  [in]	line valori ammissibili VLINE_110V,VLINE_220V

\attention \b VLineAtdTh[] used as R

\return 	ADC_ValueType
 
\retval		Valore espresso in A/D per soglia di backup 	 
		

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

ADC_ValueType VlineGetTh(ADC_ValueType line)
{
    if (line == VLINE_220V)
    {        
        return VLineAtdTh[1];
    }
    else
    {        
        return VLineAtdTh[0];
    }
}

/**
Gestione sorgenti interrupt TZ1 \n 
gestione del triac motore universale e del modulo FreqMeter.

\author 	Roberto Fioravanti
\date		gg/mm/aaaa
\version	1.0.0

\todo       cambiare nome Isr_Tpm2Overflow    
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
#if defined(BUILD_RENESAS_36079)
ISR (Isr_Tpm2Overflow)
{
#ifdef __UNIVERSAL_MOTOR_H
	TriacMotordriver();
#endif
#if  defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
  PLP_TimerManager();
#endif
#ifdef __FREQMETER_H	
	FreqMeterTimerManager();
#endif
}
#endif /* BUILD_RENESAS_36079 is defined */

/*! doc for this function? */
#if (PWM_LOAD_ENABLE == 1)
/*! Inizializza il timer 2 per il PWM */
void PWMinit (void)
{
#if defined(BUILD_FREESCALE_GB60) 
    TPM2SC = 0x08;   /* bus clock; ps=1 */
    TPM2MOD = PWM_PERIOD;
    TPM2C0SC = 0x00; /* no PWM */
    TPM2C1SC = 0x00; /* no PWM */
    TPM2C2SC = 0x00; /* no PWM */
    TPM2C3SC = 0x00; /* no PWM */
#endif
}
#endif /*(PWM_LOAD_ENABLE == 1) */

#if (CHECK_VLINE_CROSSING == 1)
/**
La funzione serve per settare il valore della variabile VLine_Number_Of_Crossing_refl (statistics).

\param  [in]		init_value valore che si vuole impostare.

\attention \b VLine_Number_Of_Crossing_refl numero di volte in cui la V di picco � andata sotto la \n
              soglia predefinita
              
\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void SetVLineRefCntInitValueRefL(uint16_t init_value)
{

    VLine_Number_Of_Crossing_refl = init_value;

}
/**
La funzione serve per settare il valore della variabile VLine_Number_Of_Crossing_refh (statistics).

\param  [in]		init_value valore che si vuole impostare.

\attention \b VLine_Number_Of_Crossing_refh numero di volte in cui la V di picco � andata sopra la \n
              soglia predefinita
              
\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void SetVLineRefCntInitValueRefH(uint16_t init_value)
{

    VLine_Number_Of_Crossing_refh = init_value;

}
/**
Setup delle soglie di ammissibilita della V di picco.

\param  [in]		refl limite inferiore di ammissibilit� della V di picco in valori A/D
\param  [in]		refh limite superiore di ammissibilit� della V di picco in valori A/D

\attention \b VLine_Ref_Value_L used as input/output 
\attention \b VLine_Ref_Value_H used as input/output 
\attention \b old_value used as input/output 
\attention \b initial_filter used as input/output 

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void SetVLineRef(uint16_t refl, uint16_t refh)
{
    VLine_Ref_Value_L = refl;
    VLine_Ref_Value_H = refh;
    /*	Delta_VLine_Ref_Value = da file di setting; */
    old_value = VLine_Ref_Value_L + 1;
    initial_filter = 0;
}


/**
Ritorna il n� di volte in cui la V di picco � andata al di sotto della soglia prefissata.

\attention \b VLine_Number_Of_Crossing_refl counter used as input/output

\return 	uint16_t


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint16_t ReadVLineCrossing_under_refl(void)
{
    return VLine_Number_Of_Crossing_refl;
}

/**
Ritorna il n� di volte in cui la V di picco � andata al di sopra della soglia prefissata.

\attention \b VLine_Number_Of_Crossing_refh counter used as input/output

\return 	uint16_t


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint16_t ReadVLineCrossing_over_refh(void)
{
    return VLine_Number_Of_Crossing_refh;
}

/**
Determinazione del n� di volte in cui la V di picco � andata al di fuori di una fascia di accettazione,
la funzione serve solo a fini statistici.

\param  [in]		actual_vline_value valore di picco della Vline in unit� A/D

\attention \b VLine_Ref_Value_L global used as input limite inferiore, in unit� A/D, previsto per una Vline
              in range

\attention \b VLine_Ref_Value_H global used as input limite superiore, in unit� A/D, previsto per una Vline
              in range

\attention \b VLine_Number_Of_Crossing_refl used as output n� di volte in cui la tensione � andata sotto 
              VLine_Ref_Value_L a partire da una condizione di Vline in range 

\attention \b VLine_Number_Of_Crossing_refh used as output n� di volte in cui la tensione � andata sopra
              VLine_Ref_Value_H a partire da una condizione di Vline in range 

\attention \b Delta_VLine_Ref_Value used as input 
              
\return 	none

\retval		none 	 

\warning	none		

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void ManageVLineRefCrossing(ADC_ValueType actual_vline_value)
{
    if ((VLine_Ref_Value_L == VLINE_REF_LOW_DEFAULT) && (VLine_Ref_Value_H == VLINE_REF_HIGH_DEFAULT))
    {
        /* I valori di sopra soglia e sottosoglia non sono stati settati */
        return ;
    }

    if (!IsZCSynchronized())
    {
        /* La procedura di sincronizzazione con lo zero crossing voltage non � terminata */
        return ;
    }

    if (initial_filter < 50)
    {
        initial_filter++;
        return ;
    }


    if (( actual_vline_value > VLine_Ref_Value_L + Delta_VLine_Ref_Value ) && 
        ( old_value <= VLine_Ref_Value_L ) )
    {
        VLine_Number_Of_Crossing_refl++;
        old_value = actual_vline_value;
        return ;
    }
    else if ( (actual_vline_value < VLine_Ref_Value_H - Delta_VLine_Ref_Value) && 
              (old_value >= VLine_Ref_Value_H))
    {
        VLine_Number_Of_Crossing_refh++;
        old_value = actual_vline_value;
        return ;
    }
    else if ( !( ( ( actual_vline_value > VLine_Ref_Value_L ) && ( old_value <= VLine_Ref_Value_L ) ) ||
                 ( ( actual_vline_value < VLine_Ref_Value_H ) && ( old_value >= VLine_Ref_Value_H ) ) ) )
    {
        old_value = actual_vline_value;
        return ;
    }
    else
    {
        return ;
    }
}

#endif /* (CHECK_VLINE_CROSSING == 1)*/
#if defined(WBOX) && (CALIBRATION_ENABLE == 1)
void InitCalibrationPointers_trash(void)
{

	if (WboxLoader(EEP_CAL_DATA_DISPL_ES,&CalibrationDataPtr_ElectSupp))
	{
	    /*Error loading data ---> avoid other error!!!*/
		CalibrationDataPtr_ElectSupp.Data		= NULL;
		CalibrationDataPtr_ElectSupp.DataLenght = 0;
	}

	if (WboxLoader(EEP_CAL_DATA_DISPL_FC,&CalibrationDataPtr_Factory))
	{
	    /*Error loading data ---> avoid other error!!!*/
		CalibrationDataPtr_Factory.Data			= NULL;
		CalibrationDataPtr_Factory.DataLenght = 0;
	}
    
}
struct Loader GetCalibrationPointers(uint8_t id_cal_pntrs)
{
struct Loader retValue = {NULL,0};

	if ( ID_CAL_PNTR_ELECT_SUPP == id_cal_pntrs )
	{
		retValue = CalibrationDataPtr_ElectSupp;
	}
	else if ( ID_CAL_PNTR_FACTORY == id_cal_pntrs )
	{
		retValue = CalibrationDataPtr_Factory;
	}
	else
	{
		/* do nothing */
	}
	return (retValue);
}

uint8_t WboxLoader(uint8_t displacement, Loader_Struct* pDataOut)
{
uint16_t nPage;
uint8_t* pData;
uint8_t nExtraPointerWords;
uint8_t i;
uint8_t nWriting;

#ifdef _BSLDR_ENABLE_ERROR_MNG_

	if (0xFF == displacement)
	{
		/**
		0xFF è l'ultimo indice consentito per un displacement tuttavia, essendo dispari,
		è anche l'ultimo displacement in assoluto quindi non è uno spiazzamento valido
		in quanto non è possibile specificare la sua lunghezza.
		*/
		return BSLDR_ERR_INVALID_DISPL;
	}

#endif

	pData = (uint8_t*)(FCT_START_ADDR);

	nExtraPointerWords = *pData;

#ifdef _BSLDR_ENABLE_ERROR_MNG_

	if (nExtraPointerWords > N_MAX_EXTRAPTR_WORDS)
	{
		return BSLDR_ERR_OVFL_EXTRAPTR;
	}
#endif

	/* Controlla gli ExtraPointer e imposta la pagina di conseguenza */
	nPage = 0;

	for (i = 1; i < (2 * nExtraPointerWords); i++)
	{
		/* displacement < 0xFF */
		if (displacement >= OFFS_B(pData, i))
		{
			nPage += 0x0100U;
		}

	}

	pData += (2 * nExtraPointerWords);		/* pData punta all'inizio campo displacement */

#ifdef _BSLDR_ENABLE_ERROR_MNG_

	if (displacement >= ((2 * (*pData)) - 1))
	{
		return BSLDR_ERR_INVALID_DISPL;
	}

#endif

  	nWriting = 8;

  	/* Dalla bitmap ricaviamo il numero di scritture delle calibrazioni */
  	for (i = 0; i < 8; i++)
  	{
  		if( ( ( *( pData + (2 * (*(pData + displacement) + nPage)) ) >> i ) & 0x01 ) == 1 )
  		{
  			nWriting = i;
  			break;
  		}
  	}

  	/* Valore di saturazione */
  	if( nWriting > *( pData + (2 * (*(pData + displacement) + nPage)) + 1 ) )
  	{
  		nWriting = *( pData + (2 * (*(pData + displacement) + nPage)) + 1 );
  	}

    if(nWriting != 0)
    {
        pDataOut->DataLenght = *( pData + (2 * (*(pData + displacement) + nPage)) + 2 );
        pDataOut->Data = pData + (2 * (*(pData + displacement) + nPage)) + 4 + ((nWriting-1) * pDataOut->DataLenght);
    }
    else
    {
        pDataOut->DataLenght = 0;
/*lint -epp*/
        pDataOut->Data       = NULL;
/*lint +epp*/
    }

	if (0 == pDataOut->DataLenght)
    {
		return BSLDR_WRN_LENGHT;
    }

	return BSLDR_OK;
}

#endif /*#if defined(WBOX) && (CALIBRATION_ENABLE == 1)*/

#if defined(BUILD_RENESAS_36079)
/**
Disabilita Interrupts, e i timer. Configura le porte in ingresso.

\return 	none
			
\warning	none		

\author 	
\date		gg/mm/aaaa
\version	1.0.0

\todo       Chiamare le varie DeInit dei Drivers
   
\note		\par \b Nota 1:
			microcontrollori supportati:			
			-Renesas  H8 36079
			\par \b Nota 2:
			documentazione: Roberto Fioravanti
			

*/
void Set_IO_to_Default(void)
{
	/*port direction*/
#if !defined(PLATFORM2_SUPPORT)	
	PORT.PCR1.BYTE = 0;
	PORT.PCR2.BYTE = 0;
	PORT.PCR3.BYTE = 0;
	PORT.PCR5.BYTE = 0;
	PORT.PCR6.BYTE = 0;
	PORT.PCR7.BYTE = 0;
	PORT.PCR8.BYTE = 0;
	PORT.PCRC.BYTE = 0;
#else
	IO.PCR1 = 0; 
	IO.PCR2 = 0; 
	IO.PCR3 = 0; 	
	IO.PCR5 = 0; 
	IO.PCR6 = 0; 
	IO.PCR7 = 0; 
	IO.PCR8 = 0; 
	IO.PCRC = 0;
#endif
	
	/*port data*/
	PORT.PDR1.BYTE = 0;
	PORT.PDR2.BYTE = 0;
	PORT.PDR3.BYTE = 0;
	PORT.PDR5.BYTE = 0;
	PORT.PDR6.BYTE = 0;
	PORT.PDR7.BYTE = 0;
	PORT.PDR8.BYTE = 0;
	PORT.PDRC.BYTE = 0;
	
	/*Disable Timer Output*/
	TMRZ.TOER.BYTE = 0xFF;
	TMRV.TCSRV.BYTE = 0;
	
	/*Disable PWM Output*/
	PORT_PMR1_BIT_PWM = 0;
	
	/*Interrupts*/
	INT_IENR1_BIT_IEN0=0;    /* IRQ0 interrupt disabled	  */
	IIC2.ICIER.BYTE=0;
	TMRZ0.TIER0.BYTE=0;
	TMRZ1.TIER1.BYTE=0;
} 
#endif  /* BUILD_RENESAS_36079 is defined */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

