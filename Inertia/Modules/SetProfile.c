
/*
** **************************************************************************
** **************************************************************************
*/ 
/** 
Gestisce il microprogramma Set profile , consentendo l'esecuzione di un profilo di carichi.

Source File

\file    	SetProfile.c
 
\version 	1.0.0
\date    	21/10/2009
\authors	Lorenzo Morbidelli

\note : 
uP ripreso da Hottima e riadattato per Dea700. il 29/07/10 
Alcuni parametri presenti in Hottima qui non servono.
\authors 	Emanuela Antognoli
*/ 
/*
** **************************************************************************
** **************************************************************************
*/

/*
Copyright (c) 2004-2007, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version
\date
\author
*/


/* ******** */
/* Includes */
/* ******** */


#include "CstmHeaders.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void SPF_InitSeg(uP_SetProfile_Struct *uP_CmpSetProfile_Data_struct);
/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ******************* */
/* External variables  */
/* ******************* */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Gestisce l'esecuzione del microprogramma Set PRofile.
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
\param [in] compartment specifica la cavit? per il controllo della temperatura.

 
\return S_OK se la condizione di avanzamento passo per il microprogramma ? verificata (raggiungimento temperatura target),
\return S_FALSE se non ? ancora verificata(conteggio in corso),
\return codice di errore in altri casi

\note \version		1.0.0
\date 			20/10/2009
\author			L. Morbidelli

\note \version		1.2.0
\date 			23/08/2010
\author			Emanuela Antognoli
\note   tolte parti di codice non necessarie in dea 700 rispetto a Hottima.
- aggiunta calibrazione.

*/
uint8_t	SPF_SetProfile_Exe(uint8_t highStatement, uint8_t LowStatement)
{
Interpreter_Struct *pItpData = Interpreter_Data;
Loader_Struct uPSetProfile_setting_data_tmp;
uP_SetProfile_Struct *uP_CmpSetProfile_Data = &uP_SetProfile_Data;
uint16_t mask_load_sel;
uint8_t nr_load_profile,nr_profiles;
uint8_t activation_value;
uint16_t mask_seg_sel=0x0001;
int8_t delta;
#if defined(CONFIG_LIB_ADC_CALIBRATION)
/*extern struct Loader CalibrationDataPtr_ElectSupp;*/
struct Loader CalibrationDataPtr_Factory;	
int16_t tempMulResult;
#endif /*(#if defined(CONFIG_LIB_ADC_CALIBRATION))*/

	
	if (pItpData->stepInitialized == FALSE)
	{
		/* Prima iterazione del passo */
		uP_CmpSetProfile_Data->status = SETPROFILE_INIT;
		uP_CmpSetProfile_Data->end_profile=FALSE;
	}


	if ( uPDoorStatus == UP_DOOR_WAIT ) 
	{ /* porta aperta -> il uP va sospeso */

		(void)SWTC_Stop((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER);

		return S_FALSE;
	}


		/* Caricamento dati dalla flash */	
	if (BasicLoader( PTR_UP_SET_PROFILE, LowStatement, &uPSetProfile_setting_data_tmp ) != S_OK)
	{
#ifdef _SETPROFILE_CHECKERRORS_				
			DBG_CheckErr(HIO_ERR_BASIC_LOADER);
#endif				 
		return S_OK;
	}	
		
	if (uP_CmpSetProfile_Data->status == SETPROFILE_INIT) 
	{	
		uP_CmpSetProfile_Data->loadMap_profile = OFFS_W(uPSetProfile_setting_data_tmp.Data,0);		
		/*!Init timer per durata segmento*/	
		uP_CmpSetProfile_Data->nSeg_counter = 0;
			
#if defined(CONFIG_LIB_ADC_CALIBRATION)
		uP_CmpSetProfile_Data->corr_factor = ( ( OFFS_B(uPSetProfile_setting_data_tmp.Data,4) & PROFILE_CF_MASK ) != 0 );


#endif /* ((CONFIG_LIB_ADC_CALIBRATION)) */
		
		SPF_InitSeg(uP_CmpSetProfile_Data);
		
		/* di default il profilo � on per consentire l'uso del SPF anche senza cooking control o preheating
		Su dea non viene modificato da nessuno ma lo lasciamo per uguaglianza*/		
		uP_CmpSetProfile_Data->cntrl_profile=PROFILE_ON;
		
		uP_CmpSetProfile_Data->status = SETPROFILE_EXE;
	}/*PREHEAT_INIT*/
	
	if (uP_CmpSetProfile_Data->status == SETPROFILE_EXE) 
	{
		if (uP_CmpSetProfile_Data->cntrl_profile==PROFILE_OFF)
		{	/* profilo off */
			(void)SWTC_Stop((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER);
			/* per dea 700*/			
			LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b = ((OFFS_B(uPSetProfile_setting_data_tmp.Data,2))&VEL_FAN_OFF_MASK);
		}
		else
		{	
			/* management carichi */
			mask_load_sel=0x0001;
			mask_seg_sel=0x0001;
			nr_load_profile=0;
			nr_profiles=0;
			while (mask_load_sel)
			{
				if ((uP_CmpSetProfile_Data->loadMap_profile) & (mask_load_sel))
				{
					if ((OFFS_W(uPSetProfile_setting_data_tmp.Data,(3+(nr_profiles))))&(mask_seg_sel<<(((uint8_t)(uP_CmpSetProfile_Data->nSeg_counter)))))	
					{				
						activation_value = TRUE;
					}
					else
					{
						activation_value = FALSE;
					}
					SPF_SetProfileLoadMng (nr_load_profile,activation_value,uPSetProfile_setting_data_tmp);	
					nr_profiles++;
				}
				mask_load_sel<<=1;
				nr_load_profile++;
			}
			/* scorrimento profilo e set contatori segmenti */
			delta = 0;
#if defined(CONFIG_LIB_ADC_CALIBRATION)
			
			if ( ( uP_CmpSetProfile_Data->corr_factor != 0 ) &&
				 ( LIB_CAL_GetCalibData((uint8_t)ID_CAL_PNTR_FACTORY, &CalibrationDataPtr_Factory) == BSLDR_OK) &&
				 ( LoadsReq_New[Res_Pos[LD_DRY_RES_IND]].b ==LOAD_ON )
				)		
			{/* richiesta applicazione fattore correttivo RESISTENZA ASCIUGATURA*/
			
				if((uint8_t)CalibrationDataPtr_Factory.Data[DRY_RESISTOR_CAL_OFFSET] != 0xFF)
				{				
					/* Formula:  aggiornamento target periodo */
					tempMulResult = (int8_t)CalibrationDataPtr_Factory.Data[DRY_RESISTOR_CAL_OFFSET] * SEG_TIME_BASE;
/*lint -e734 (-32640 <= tempMulResult <= 32385) => (-25 <= delta <= 23) */
					delta = tempMulResult / MNT_CALIBRATION_UP_MANTAIN_CST;

				}
			}
#endif /*(CONFIG_LIB_ADC_CALIBRATION )*/
				

			if( SWTC_GetElapsedTime((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER) >= (SWTC_TimeType)(SEG_TIME_BASE+ delta) )
			{
				if (uP_CmpSetProfile_Data->c_counter == (OFFS_B(uPSetProfile_setting_data_tmp.Data,3)&PROFILE_C_MASK))
				{
					if (uP_CmpSetProfile_Data->nSeg_counter < ((OFFS_B(uPSetProfile_setting_data_tmp.Data,3)&PROFILE_NS_MASK)>>4))
					{	
						uP_CmpSetProfile_Data->nSeg_counter++;		
					}
					else
					{							
						uP_CmpSetProfile_Data->nSeg_counter = 0;
						uP_CmpSetProfile_Data->end_profile= TRUE;
					}		
					uP_CmpSetProfile_Data->c_counter = 0;				
				}	
				else
				{				
					uP_CmpSetProfile_Data->c_counter++;			
				}
				(void)Set_MediumCounter((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_MCounterValue)0x7FFF);	/* MQ introdurre define */
				
			}	
		 	(void)SWTC_Run((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER);
			
		}
	}/*SETPROFILE_EXE*/
	
	
	if (uP_CmpSetProfile_Data->end_profile==TRUE)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

static void SPF_InitSeg(uP_SetProfile_Struct *uP_CmpSetProfile_Data_struct)
{
		uP_CmpSetProfile_Data_struct->c_counter = 0;

		(void)Set_MediumCounter((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_MCounterValue)0x7FFF);	/* MQ introdurre define */

		(void)SWTC_Run((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER);
		
}

/**
Gestisce  attivazione carichi per Dea 700: resistenza asciugatura e ventola asciugatura

\param [in]		
\return 		
 
\version		1.1.0
\date 			23/08/2010
\author			Emanuela Antognoli

\note			
				  
*/


void SPF_SetProfileLoadMng (uint8_t nr_load_profile_temp,uint8_t activation_value_temp, Loader_Struct uPSetProfile_setting_data_tmp_loc)
{
	if ( nr_load_profile_temp == Fan_Pos[LD_DRY_FAN_IND])
	/*stato motoventilatore asciugatura */
	{	
		if (activation_value_temp)
		{
			LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b =((OFFS_B(uPSetProfile_setting_data_tmp_loc.Data,2)&VEL_FAN_ON_MASK)>>2);
		}
		else
		{
			LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b =((OFFS_B(uPSetProfile_setting_data_tmp_loc.Data,2)&VEL_FAN_OFF_MASK));
		}
		
	}
	else 
	{ /*  alla resistenza gli associo semplicemente il valore di attivazione ( ON-OFF)*/
		LoadsReq_New[nr_load_profile_temp].b = activation_value_temp;
	}	
}
