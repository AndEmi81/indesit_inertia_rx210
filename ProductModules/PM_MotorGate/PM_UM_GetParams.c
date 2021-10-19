/*
Copyright (c) 2004-2012, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_GetParams.c
\brief   Il file contiene le funzioni per prelevare i parametri dal settings
 	 o da FW  vedi CONFIG_PARAM_FROM_FIRMWARE definito da configuratore.
\ingroup PM_MotorGate
\author  Marco Sperandini
*/
/*@{*/
#include "Config.h"

/* ******** */
/* Includes */
/* ******** */


#ifdef CONFIG_PM_MOTORGATE
#include "LIB_Utilities.h"
#include "PM_UM_GetParams.h"
#include "PM_UM_Measure.h"
#include "PM_UM_UniversalMotor.h"
#include "PM_UniversalMotorMeasure.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* valori di ritorno della funzione ApplyFormulaParam() */
#define FORMULA_PARAM_ERR_OK    0
#define FORMULA_PARAM_ERR_m_d   1

/* #define utilizzate dalla funzione ApplyFormulaParam() */
#define VAC_OFFSET              4
#define SIZE_ENGINE_DATA_BLOCK  6
#define M_D_OFFSET              1
#define Q_OFFSET                (2/sizeof(uint16_t))

#define PM_ENGINE_PARAM_CMNDATA_MCU_OSC_COMP_VAL(_ptr)					MBGET(1, _ptr, McuOscComp)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**
*/
typedef struct InfoExtMotor
{
	unsigned char rpmmin0;					/**< rpm min, 1 bytes, unsigned				*/
	unsigned char alphaStepSoftStart0;		/**< alpha Step Soft Start, 1 bytes, unsigned				*/
	unsigned char alphaIni1;				/**< alpha , initial value - 2 bytes, unsigned , BE				*/
	unsigned char alphaIni0;				/*                 											*/
	unsigned char relaydelay0;				/**< rpm min, 1 bytes, unsigned				*/
	unsigned char RTx10_0;					/**< alpha Step Soft Start, 1 bytes, unsigned				*/
	unsigned char tachPoles0;				/**< rpm min, 1 bytes, unsigned				*/
	unsigned char triacMinFireFireTime0;	/**< alpha Step Soft Start, 1 bytes, unsigned				*/
	unsigned char alphaMax1;				/**< alpha , initial value - 2 bytes, unsigned , BE				*/
	unsigned char alphaMax0;				/*                 											*/
	unsigned char rebalancedispl0;			/**< rebalance displ, 1 bytes, unsigned				*/
	unsigned char unbalanceAvgSamplesNum0;	/**< unbalance Avg Samples Num, 1 bytes, unsigned				*/
	unsigned char unbalanceDiscardsNum0;	/**< unbalance Discards Num, 1 bytes, unsigned				*/
	unsigned char PhaseSamplesNum0;			/**< Phase Samples Num, 1 bytes, unsigned				*/
	unsigned char McuOscComp0;				/**< Mcu Osc Comp, 1 bytes, unsigned				*/
}PM_SFT_WM_InfoExternMotorType;







#if defined (CONFIG_PARAM_FROM_FIRMWARE)

static uint8_t  offset_tabSpeedRange;
extern const uint16_t tabSpeedRange[6];
#else


/* *************************** */
/* Local Data 				   */
/* *************************** */
static uint8_t  displOutOfRangeTab;
static uint8_t  displParamPD;


/* *************************** */
/* Extern Data 				   */
/* *************************** */

extern uint8_t  *pcommmonparam;
extern uint8_t	 u8CommonParamSize;
extern uint8_t  *pHalfFieldParam;

#endif  /* CONFIG_PARAM_FROM_FIRMWARE */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

static bool_t IsAvailable_common_MCUOscCompDelta(void);

/* ***************** */
/* Exported Functions*/
/* ***************** */
#if !defined(CONFIG_PARAM_FROM_FIRMWARE)

void setCommonParamsPtr( void )
{
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	/* nothing to do */
#else
    struct Loader loader;
    (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, PM_UM_COMMON_PARAM_DISPL, &loader);
    pcommmonparam = loader.Data;
    u8CommonParamSize = loader.DataLenght;
#endif
}




int8_t Get_common_MCUOscCompDelta( void )
{
	int8_t delta=0;
	if(IsAvailable_common_MCUOscCompDelta())
	{
		delta = (int8_t)PM_ENGINE_PARAM_CMNDATA_MCU_OSC_COMP_VAL((PM_SFT_WM_InfoExternMotorType*)pcommmonparam);
	}
	return delta;
}

uint8_t setHalfFieldParamPtr( void )
{
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	return TRUE;
#else
     struct Loader loader;
    (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, PM_UM_MID_FIELD_DISPL, &loader);
    
    if( loader.DataLenght )
    {
        /* Mezzo Campo Presente */
        pHalfFieldParam = loader.Data;
        return TRUE;
    }
    return FALSE;
#endif /* CONFIG_PARAM_FROM_FIRMWARE */
}
#endif


#ifdef CONFIG_PARAM_FROM_FIRMWARE
/** In base alla velocit� target e alla tabella tabSpeedRange[] individua la
fascia di velocit� utile al controllo

\param [in]  vTarget
\param [in]  tabSpeedRange[]
\param [out] offset_tabSpeedRange

16/03/2006 Stefano Ughi
*/
void getOffsetSpeedRangeFromSetting( void )
{
    extern const uint16_t tabSpeedRange[] ;
    uint8_t loopSearch,i;
    
    loopSearch 	= FALSE;
    i= 0;
    while( loopSearch == FALSE )
    {
        if( PM_UM_CtrlVar.target_rpm <= tabSpeedRange[i] )
        {
            loopSearch = TRUE;
        }
        else
        {
            i ++;
        }
    }
    offset_tabSpeedRange = i;
}
#else /* CONFIG_PARAM_FROM_FIRMWARE */
/**
Determina gli spiazzamenti vs. CONFIG_PM_UM_PTR_ENGINE_CONST relativi ai parametri di
out of range e di cocontrollo P.D.
\param [in]  vTarget
\param [out] displOutOfRangeTab	 (static)
\param [out] displParamPD		 (static)


04/05/2006 Stefano Ughi
*/
void getOffsetSpeedRangeFromSetting( void )
{
    struct Loader tabSpeedR;
    uint8_t loopSearch;
   
    (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_PARAM, PM_UM_TAB_SPEED_RANGE_DISPL, &tabSpeedR);

    loopSearch 	= FALSE;
    while( loopSearch == FALSE )
    {
        if( PM_UM_CtrlVar.target_rpm <= (*(uint16_t *)tabSpeedR.Data) )
        {
            loopSearch = TRUE;
        }
        else
        {
            tabSpeedR.Data+=4;
        }
    }
    displOutOfRangeTab 	= *(tabSpeedR.Data+2);
    displParamPD	= *(tabSpeedR.Data+3);

}
#endif /* CONFIG_PARAM_FROM_FIRMWARE */

#ifdef CONFIG_PARAM_FROM_FIRMWARE
/**
restituisce la struttura dei parametri per la determinazione
delle fasce di out of range

\param [in]  engineControlFlags.bit.vFreq (static)
\param [in]  offset_tabSpeedRange (static)

\return una struttura tipo outOfRangeParams_t

16/03/2006 Stefano Ughi
*/
outOfRangeParams_t  getOutOfRangeParamsFromSetting( void )
{
    extern const uint8_t tabOutRange[2][ sizeof(tabSpeedRange)/sizeof(tabSpeedRange[0])][5];
    outOfRangeParams_t outOfRangeParams;

    outOfRangeParams = *( outOfRangeParams_t *)&tabOutRange[ PM_UM_CtrlVar.engineControlFlags.bit.vFreq ][offset_tabSpeedRange][0];

    return( outOfRangeParams );
}
#else  /* CONFIG_PARAM_FROM_FIRMWARE */
/**
restituisce la struttura dei parametri per la determinazione
delle fasce di out of range

\param [in]  displOutOfRangeTab (static)
\param [in]  CONFIG_PM_UM_PTR_ENGINE_CONST (static const)

\return una struttura tipo outOfRangeParams_t

04/05/2006 Stefano Ughi
*/

void  getOutOfRangeParamsFromSetting( outOfRangeParams_t *pOutOfRangeParams )
{
	struct Loader tabOutOfRange;

    (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_CONST, displOutOfRangeTab, &tabOutOfRange);

	if(pOutOfRangeParams)
	{
    	*pOutOfRangeParams = *( outOfRangeParams_t *)tabOutOfRange.Data;
	}
}

#endif /* CONFIG_PARAM_FROM_FIRMWARE */

#ifdef CONFIG_PARAM_FROM_FIRMWARE
/** In base alla regione di appartenenza della vTarget e
alla frequenza di rete ricava i parametri ST,CD,CPD del controllo.
CD e CPD vengono riscalati per l'opportuno rapporto di trasmissione
\param [in] offset_tabSpeedRange
\param [in] frequenza di rete
\param [in] trasmissionRatio
\param [in] tabPD_Param[]

\return inControlloreData_t

17/03/2006 Stefano Ughi
*/
inControlloreData_t getEngineControlParamFromSetting(void)
{
    extern const uint8_t tabRebalancePD_Param[10];
    extern const uint8_t tabPD_Param[2][sizeof(tabSpeedRange)/sizeof(tabSpeedRange[0])][10]; 
    inControlloreData_t controlloreData;
    uint8_t *p;

    static uint16_t param_CD=0;
    static uint16_t param_CPD=0;

    controlloreData.rFactor	= 32;

    if(PM_UM_CtrlVar.stateMachine.cmdflags & CMDFLAGS_REBALANCE_MASK)
    {
      controlloreData.pST 	= &tabRebalancePD_Param;
    }else{
      controlloreData.pST 	= &tabPD_Param[PM_UM_CtrlVar.engineControlFlags.bit.vFreq][offset_tabSpeedRange][0];
    }

    /* Riscalo per il vero rapporto di trasmissione */
    //mp16x16( (uint16_t*)((uint8_t *)controlloreData.pST+6), &trasmissionRatio, &mp16x16_Result );
    //p = (uint8_t *) &mp16x16_Result;
    //param_CD = ((*(p+1)<<8) + *(p+2));
    
    param_CD = (EndianU16_ToN(*(uint16_t*)((uint8_t *)controlloreData.pST+6)) * RTx10 )/118;
    param_CD = EndianU16_ToB(param_CD);
    controlloreData.pCD = &param_CD;

    //mp16x16( (uint16_t*)((uint8_t *)controlloreData.pST+8), &trasmissionRatio, &mp16x16_Result );
    //p =(uint8_t*) &mp16x16_Result;
    //param_CPD = ((*(p+1)<<8) + *(p+2));
    param_CPD = (EndianU16_ToN(*(uint16_t*)((uint8_t *)controlloreData.pST+8)) * RTx10)/118;
    param_CPD = EndianU16_ToB(param_CPD);   
    controlloreData.pCPD = &param_CPD;

    return controlloreData;
}
#else /* CONFIG_PARAM_FROM_FIRMWARE */
/**
In base alla velocit� target, tradotta in displParamPD e al puntatore
CONFIG_PM_UM_PTR_ENGINE_CONST ritorna i parametri del controllo P.D..

\param [in] CONFIG_PM_UM_PTR_ENGINE_CONST (static const)
\param [in] displParamPD  (static)

\return controlloreData (type inControlloreData_t)

04/05/2006 Stefano Ughi
*/
void getEngineControlParamFromSetting(inControlloreData_t *pControlloreData)
{
    struct Loader PDparam;

    if(PM_UM_CtrlVar.stateMachine.cmdflags & CMDFLAGS_REBALANCE_MASK)
    {
        (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_CONST, Get_common_reb_pd_offset(), &PDparam);
    }
    else
    {
        (void)BasicLoader(CONFIG_PM_UM_PTR_ENGINE_CONST, displParamPD, &PDparam);
    }

	if(pControlloreData)
	{
		pControlloreData->pST 	  = (PDparam.Data+0);
		pControlloreData->pCD 	  = (PDparam.Data+6);
		pControlloreData->pCPD	  = (PDparam.Data+8);
		pControlloreData->rFactor  = *(PDparam.Data+10);
	}
}

#endif /* CONFIG_PARAM_FROM_FIRMWARE */

uint8_t Get_common_tacho_poles( void )
{
	uint8_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value = TACH_POLES_NUM;
#else
	value = *(pcommmonparam+CMN_PAR_TACHO_POLES);     
#endif
	return value;           
}

uint16_t Get_common_alpha_max(void) 
{
	uint16_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value = ( PM_UM_CtrlVar.engineControlFlags.bit.vFreq == HZ_50 )? ALPHA_MAX_50_HZ: ALPHA_MAX_60_HZ ;
#else
	value = *((uint16_t*)pcommmonparam+CMN_PAR_ALPHA_MAX/( sizeof(uint16_t)));
#endif  
	/* conversione */
	value = NOMINAL_TO_STD(value);
	return  value;
}		     

/*
returns alpha min in us
*/
uint16_t Get_common_alpha_min(void )
{
	uint16_t value;        
	value = ( PM_UM_CtrlVar.engineControlFlags.bit.vFreq == HZ_50 )? ALPHA_MIN_50_HZ: ALPHA_MIN_60_HZ ;
	
#ifdef CONFIG_PARAM_FROM_FIRMWARE

#else 
  {
  getEngineData_t          engineData;
	/* prende alpha_min dai parametri del motore */
        PM_UM_GetMotorData(PM_UM_MEASURE_APLHA_MIN, &engineData);
        if( engineData.error == FALSE ){
             value = engineData.dato;
        }                
   }
#endif
	/* conversione */
	return 	value;
}

/*
Returns the raw phase angle to apply at soft start  (x 1.6 us)
*/
uint16_t Get_common_alpha_ini_soft_start(void)
{
	uint16_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value  = ALPHA_INI_SOFT_START_50_HZ;
#else 
	value = (*((uint16_t*)pcommmonparam+(CMN_PAR_ALPHA_INI_SOFT_START/ sizeof(uint16_t))));
#endif	
	return  value; 
}
/*
Returns the step in us to apply to phase angle at soft start
*/
uint8_t Get_common_alpha_step_soft_start(void)
{
	uint8_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value = (ALPHA_STEP_SOFT_START_50_HZ/2);
#else
	value = *(pcommmonparam+CMN_PAR_ALPHA_STEP_SOFT_START);
#endif	  
	/* conversione */
	value = NOMINAL_TO_STD(value);
	return value;    
}


/*

*/
uint16_t Get_common_min_fire_triac(void)
{
	uint16_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value = 200;	
#else
        value = *(pcommmonparam+CMN_PAR_TIME_MIN_FIRE_TRIAC);
#endif
	/* conversione */
	value = NOMINAL_TO_STD(value);
	return value;		 
}		 

uint8_t Get_common_RTx10(void)
{
	uint8_t value;
#ifdef CONFIG_PARAM_FROM_FIRMWARE
	value = RTx10;
#else
	value = (*(pcommmonparam+CMN_PAR_RTx10));
#endif	
	return value;                 
}


/* ************** */
/* Local Function */
/* ************** */


static bool_t IsAvailable_common_MCUOscCompDelta(void)
{
	bool_t bRetVal=FALSE;
	uint32_t p;
	uint32_t maxDispl;

	p=(uint32_t)MBGET_MAX_DISPL(1, PM_SFT_WM_InfoExternMotorType, McuOscComp);
	maxDispl=u8CommonParamSize*2-1;
	if(p <= maxDispl)
	{
		bRetVal=TRUE;
	}

	return bRetVal;
}

#endif /*  CONFIG_PM_MOTORGATE */
 /*@}*/
