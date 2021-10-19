/**
Source File 

\file    ActivateAlgorithm.c
\ingroup ActivateAlgorithm
\date    17/03/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
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
#include "PM_PulseCounterExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define AAL_GET_ALG_CODE(p)		OFFS_B((p), 0)
#define AAL_GET_ALG_DATA(p)		OFFS_B((p), 1)

#define TOUT_STAB_SENS_FILTER 	(OFFS_B(dirty_foam_setting_data.Data,3) & 0x7F)/*2sec*/

#define TOUT_STAB_STATO3_FILTER  (OFFS_B(Tab_HWCVS_data.Data, 0)& 0x7F)/* def 5 sec */
#define TOUT_END_ALYS3 			 ((dirty_foam_setting_data.DataLenght>2)?(OFFS_W(dirty_foam_setting_data.Data, 2)& 0x7FFF):1800 )/* def 30 min */
enum
{
	WASH_PUMP_ALG=0,
	DRAIN_PUMP_ALG
};

enum AAL_TURBIDITY_ID_INDX
{
    AAL_TURBIDITY_ONE_SHOT_R_ID=0,
    AAL_TURBIDITY_CONT_R_ID,
    AAL_TURBIDITY_VALIDATION_ID,
    AAL_TURBIDITY_ID_NUM
};


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
struct _Aal_TurbidityRead_s
{
    uint8_t idReading[AAL_TURBIDITY_ID_NUM];   /*  0: one shot;  
                                1: continuous   
                                2: validation */
};

typedef struct _Aal_TurbidityRead_s Aal_TurbidityRead_t;
/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* Algoritmi */

void AAL_Dirty_Foam_identify(uint8_t cmd);
#if (VIRTUAL_SENSOR_ENABLE == 1)
static void AAL_Load_Restore (void);
static void AAL_Load_Var_Init (void);
#endif

/* Generale */
uint8_t ReadTurbidityManager(uint8_t cmd, uint8_t alg);


/* ****************** */
/* Exported variables */
/* ****************** */

/* Algorithm Tables */
const ALGORITHM_PTR AAL_algList[] = {

	AAL_TurbiditySensorCalibration,     /* Alg 0 - Calibratione sensore di torbidità */
	AAL_LevToTimeRefUpdate,             /* Alg 1 - Calcolo tempo per pieno - portata */
	AAL_Dirty_Foam_identify,			/* Alg 2 - Riconoscimento sporco /schiuma */
	NullFunc,			                /* Alg 3 */
	AAL_ReadVirtualSensor,	            /* EA per provaAlg 4 - Lettura del sensore virtuale */
	AAL_ReadTurbidityContinuous,        /* Alg 5 - Lettura continua sensore di torbidità */
	AAL_ReadTurbidityOneShot,		    /* Alg 6 - Lettura one shot sensore di torbidità */
	AAL_ValidateTurbidityCalibration,	/* Alg 7 - Validazione sensore di torbidità */
   
	
	
};
/* **************** */
/* Global variables */
/* **************** */


/**/
Aal_TurbidityRead_t Aal_TurbidityRead;

/* ***************** */
/* Exported function */
/* ***************** */

void AAL_ReadVirtualSensor(uint8_t cmd)
{
		
	switch(cmd)
    {
        case AAL_ALG_INIT:
		/* qui parte il timer */
		  {
			Loader_Struct ReadVSDataLoader;
			if (HWC_Flags.bit.vs_wash_presence )
			{
				CLRBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* reset flag overflow ripristini */	
			}
			CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS); /* reset flag vuoto sensore in lavaggio */
			if(BasicLoader(PTR_ALGORITHM_READ_VS_DATA, AADisplParams[AAL_READVIRTUALSENSOR_ALG][0] , &ReadVSDataLoader) != S_OK)

			{
		        SETBIT(TerminatedAlgorithmBmp[0], AAL_READVIRTUALSENSOR_ALG);
		        CLRBIT(ActiveAlgorithmBmp[0],AAL_READVIRTUALSENSOR_ALG);
			}
			else
			{ /* OK */
#if (VIRTUAL_SENSOR_ENABLE == 1)
			 (void)SWTC_SetMValue((SWTC_CountersType)READ_VS_M_TIMER, (SWTC_MCounterValue)OFFS_W(ReadVSDataLoader.Data,0)&0x7FFF);
			  ReadVS_Data.pumpType=(OFFS_B(ReadVSDataLoader.Data, 0)&0x80)?DRAIN_PUMP_ALG:WASH_PUMP_ALG;
			  ReadVS_Data.targetSpeed =(uint16_t)OFFS_B(ReadVSDataLoader.Data,2)*PUMP_CTRL_BLDC_SPEED_MUL;
			  ReadVS_Data.targetRamp = OFFS_B(ReadVSDataLoader.Data,3)*PUMP_CTRL_BLDC_RAMP_TIME_MUL;
			  ReadVS_Data.ctrl_active =  (OFFS_B(ReadVSDataLoader.Data,4)& 0x80)>>7;
#endif
		    }
		  }
		  break;
#if (VIRTUAL_SENSOR_ENABLE == 1)
		  case AAL_ALG_MONITOR:
		  	
			if (uPDoorStatus == UP_DOOR_WAIT)
		  	{/* porta aperta o in attesa di timeout dopo la chiusura o delay MR-> il uP va sospeso */
   
				(void)SWTC_Stop((SWTC_CountersType)READ_VS_M_TIMER);
				return ;
			    
			}
		  (void)SWTC_Run((SWTC_CountersType)READ_VS_M_TIMER);
          {
			  uint8_t index = (ReadVS_Data.pumpType==DRAIN_PUMP_ALG)?DRAIN_BLDC_PUMP:WASH_BLDC_PUMP;
			  uint16_t speed, time;
			  
 			  if(SWTC_IsExpired((SWTC_CountersType)READ_VS_M_TIMER) && (ReadVS_Data.ctrl_active==0))
			  {
				  speed = 0;
				  time = 0;
				  (void)SWTC_Stop((SWTC_CountersType)READ_VS_M_TIMER);
		          SETBIT(TerminatedAlgorithmBmp[0], AAL_READVIRTUALSENSOR_ALG);
		          CLRBIT(ActiveAlgorithmBmp[0],AAL_READVIRTUALSENSOR_ALG);
			  }
			  else
			  {
				  speed = ReadVS_Data.targetSpeed;
				  time = ReadVS_Data.targetRamp;
			  }
			  
			  SET_BLDC_SPEED(BLDC_PumpReq_New[index],speed);
			  SET_BLDC_TIME(BLDC_PumpReq_New[index],time);
		  }		  
		  break;
#endif
		  default:	
          case AAL_ALG_DISABLE:
			  SETBIT(TerminatedAlgorithmBmp[0], AAL_READVIRTUALSENSOR_ALG);
	          CLRBIT(ActiveAlgorithmBmp[0],AAL_READVIRTUALSENSOR_ALG);
		  break;
	}	 
}


/**
Funzione di richiesta calibrazione del sensore di torbidità

\version		1.0.0
\date 			05/05/2008
\author			Roberto Fioravanti
*/
/*lint -e715
Elimina il warning sul parametro cmd che non viene utilizzato
*/
void AAL_TurbiditySensorCalibration(uint8_t cmd)
{
  
    if(!HWC_Flags.bit.turbsens)
    {
        /*esecuzione istantanea nell'ambito del passo */
        SETBIT(TerminatedAlgorithmBmp[0], AAL_TURBIDITY_SENSOR_CAL_ALG);
        CLRBIT(ActiveAlgorithmBmp[0],AAL_TURBIDITY_SENSOR_CAL_ALG);
        return;
    }
  
    if ( cmd == (uint8_t)AAL_ALG_INIT )
    {
      TurbidityCalibrationCmd();
    }
    else
    {
      if ( TurbiditySensorCal.state == (uint8_t)TURBSENS_VALID_CALIBR_VALUE_STATE )
      {
            /*esecuzione istantanea nell'ambito del passo */
            SETBIT(TerminatedAlgorithmBmp[0], AAL_TURBIDITY_SENSOR_CAL_ALG);
            CLRBIT(ActiveAlgorithmBmp[0],AAL_TURBIDITY_SENSOR_CAL_ALG);        
      }
      
    }
   
}

/**
Funzione di richiesta aggiornamento portata storica, ovvero tempo per 
raggiungere il pieno pressostato.
Il flag deve rimanere abilitato per tutta la durata del passo,
viene resettato nell'interpreter (init nuovo passo)

\version		1.0.0
\date 			17/03/2008
\author			Valeria Catani
*/
/*lint -e715
Elimina il warning sul parametro cmd che non viene utilizzato
*/
void AAL_LevToTimeRefUpdate(uint8_t cmd)
{
	if ( HWC_Flags.bit.vs_wash_presence==0)
	{/* EA con i sensori virtuali la portata storica non ha + senso,q uindi neanche l'agoritmo che non 
	dovrebbe mai essere impostato da setting*/
	
    	Flag_levTOtimeRef_update = TRUE;  
	}
    /* reset attiva funzione nell'init nuovo passo (interpreter) */
   
}

/**
Funzione di richiesta lettura sensore di torbidità in modo continuo
\param [in]     uint8_t cmd

\par \b cmd comando
    valori supportati:
    vedi ReadTurbidityManager
    
\return         none

\version		1.0.0
\date 			21/05/2008
\author			Roberto Fioravanti
*/
void AAL_ReadTurbidityContinuous(uint8_t cmd)
{
    ReadTurbidityManager(cmd,(uint8_t)AAL_TURBIDITY_READ_CONTINUOUS);
}



/**
Funzione di richiesta lettura sensore di torbidità in modo one-shot
\param [in]     uint8_t cmd

\par \b cmd comando
    valori supportati:
    vedi ReadTurbidityManager
    
\return         none

\version		1.0.0
\date 			21/05/2008
\author			Roberto Fioravanti
*/
void AAL_ReadTurbidityOneShot(uint8_t cmd)
{
    if(TRUE==ReadTurbidityManager(cmd,AAL_TURBIDITY_READ_ONESHOT)) 
    {			
#ifdef _STATISTICS_ON_			    
        STC_Add_LastCycle_turbPerc_turbCalType();
#endif        
    }
}

/**
Funzione di validazione calibrazione del sensore di torbidità
\param [in]     uint8_t cmd

\par \b cmd comando
    valori supportati:
    vedi ReadTurbidityManager
    
\return         none

\version		1.0.0
\date 		    15/07/2008
\author			Roberto Fioravanti
*/
void AAL_ValidateTurbidityCalibration(uint8_t cmd)
{
    if(!HWC_Flags.bit.turbsens)
    {
        /*esecuzione istantanea nell'ambito del passo */
        SETBIT(TerminatedAlgorithmBmp[0], AAL_TURBIDITY_VALIDATE_CALIB);
        CLRBIT(ActiveAlgorithmBmp[0],AAL_TURBIDITY_VALIDATE_CALIB);
        return;
    }
    
    if(TurbiditySensorCal.val==TURBIDITY_PWM_MIN)
    {
        SETBIT(TerminatedAlgorithmBmp[0], AAL_TURBIDITY_VALIDATE_CALIB);
        CLRBIT(ActiveAlgorithmBmp[0],AAL_TURBIDITY_VALIDATE_CALIB);
        return;
    }
    if(TRUE == ReadTurbidityManager(cmd,AAL_TURBIDITY_VALIDATE_CALIB))
    {
        if((TurbidityReads.ADvalue<TURBIDITY_AIR_MIN_ADVAL))
        {
            TurbiditySensorCal.val=TURBIDITY_PWM_MIN;
            TurbiditySensorCal.adH2oRef=TURBIDITY_H2O_DEFAULT_ADVAL;
        }
        else if((TurbidityReads.ADvalue>TURBIDITY_AIR_MAX_ADVAL))
        {
            TurbiditySensorCal.val=TURBIDITY_PWM_MIN;
            TurbiditySensorCal.adH2oRef=TURBIDITY_H2O_DEFAULT_ADVAL;
        }
        else
        {
            /* lettura in range */
        }
		
		SETBIT(TerminatedAlgorithmBmp[0], AAL_TURBIDITY_VALIDATE_CALIB);
        CLRBIT(ActiveAlgorithmBmp[0],AAL_TURBIDITY_VALIDATE_CALIB);
    }
}

/**
Manager lettura sensore di torbidità da algoritmo eseguito nell'algoritmo.

\param [in]     uint8_t cmd
\param [in]     uint8_t mode
\return         none

\par \b cmd comando
    valori supportati:
    \b AAL_ALG_INIT
    Avvia la lettura
    \b AAL_ALG_MONITOR
    Attende la disponibilità della lettura
    \b AAL_ALG_DISABLE
    Disabilita l'algoritmo
    
\par alg\b algoritmo
    valori supportati:
    \b AAL_TURBIDITY_READ_ONESHOT
    \b AAL_TURBIDITY_VALIDATE_CALIB
    Attiva la lettura del sensore di torbidità in due modi possibili:
    lettura continua (CONTINUOUS) e one-shot (ONE_SHOT_MODE).

    \b AAL_TURBIDITY_READ_CONTINUOUS
    Avvia la letttura nella modalità CONTINUOUS.
    L'algoritmo può essere terminato disattivando l'algoritmo.

\return     uint8_t
\retvalue   TRUE        Lettura completata
\retvalue   FALSE       Lettura in corso

\version		1.0.0
\date 			21/05/2008
\author			Roberto Fioravanti
*/
uint8_t ReadTurbidityManager(uint8_t cmd, uint8_t alg)
{
    uint8_t ret=FALSE;
    uint8_t idReading=TurbidityReads.id;
    
    /* indice letture */
    uint8_t indexId;
    switch(alg)
    {
        case AAL_TURBIDITY_VALIDATE_CALIB:
            indexId=(uint8_t)AAL_TURBIDITY_VALIDATION_ID;
           break;
        case AAL_TURBIDITY_READ_ONESHOT:
            indexId=(uint8_t)AAL_TURBIDITY_ONE_SHOT_R_ID;
           break;
        case AAL_TURBIDITY_READ_CONTINUOUS:
            indexId=(uint8_t)AAL_TURBIDITY_CONT_R_ID;
           break;
        default:
            return TRUE;
    }
    
    if(!HWC_Flags.bit.turbsens)
    {
        /*esecuzione istantanea nell'ambito del passo */
        SETBIT(TerminatedAlgorithmBmp[0], alg);
        CLRBIT(ActiveAlgorithmBmp[0],alg);
        return TRUE;
    }
    
    switch(cmd)
    {
        case AAL_ALG_INIT:
            Aal_TurbidityRead.idReading[indexId]=idReading;
            HIO_trbReadEnable((uint8_t)TURBSENS_READ_al_TRB_OS+indexId);
              
        break;
            
        case AAL_ALG_MONITOR:
            switch(alg)
            {
                /* singola lettura */
                case AAL_TURBIDITY_VALIDATE_CALIB:
                case AAL_TURBIDITY_READ_ONESHOT:
                    
                    if((idReading != Aal_TurbidityRead.idReading[indexId]) && (GIValid & GI_TURB_SENSOR_MSK))
                    {
                        /* l'algoritmo può terminare */                        
                        Aal_TurbidityRead.idReading[indexId]=idReading;
                        
                        SETBIT(TerminatedAlgorithmBmp[0], alg);
                        CLRBIT(ActiveAlgorithmBmp[0],alg);

                        HIO_trbReadDisable((uint8_t)TURBSENS_READ_al_TRB_OS+indexId);


                        ret=TRUE;
                    }
                    break;
                
                /* lettura continua */
                case AAL_TURBIDITY_READ_CONTINUOUS:
                    Aal_TurbidityRead.idReading[indexId]=idReading;
                    break;
            }
			
		    break;
		
		default:	
        case AAL_ALG_DISABLE:
            /* l'algoritmo può terminare */            
            HIO_trbReadDisable((uint8_t)TURBSENS_READ_al_TRB_OS+indexId);
            ret=TRUE;        
    }
    
    return ret;
}

/**
Manager algoritmo riconoscimento sporco schiuma.

\param [in]     uint8_t cmd
\param [in]     uint8_t 
\return         none

\par \b cmd comando
    valori supportati:
    \b AAL_ALG_INIT
    Avvia la lettura
    \b AAL_ALG_DISABLE
    Disabilita l'algoritmo
    
\par alg\b algoritmo
    valori supportati:
    \b 

    \b AAL_TURBIDITY_READ_CONTINUOUS
    Avvia la letttura nella modalità CONTINUOUS.
    L'algoritmo può essere terminato disattivando l'algoritmo.

\return     uint8_t
\retvalue   TRUE        Lettura completata
\retvalue   FALSE       Lettura in corso

\version		1.0.0
\date 			11/01/2012
\author			Emanuela Antognoli
*/
void AAL_Dirty_Foam_identify(uint8_t cmd)
{
		
	switch(cmd)
    {
        case AAL_ALG_INIT:
		/* qui parta il timer */
		
			if((HWC_Flags.bit.vs_wash_presence == 0) ||(BasicLoader(PTR_ALGORITHM_DATA, DIRTY_FOAM_DISPL , &dirty_foam_setting_data) != S_OK))
			{
				 
		        SETBIT(TerminatedAlgorithmBmp[0], AAL_DIRTY_FOAM_IDENTIFY_ALG);
		        CLRBIT(ActiveAlgorithmBmp[0],AAL_DIRTY_FOAM_IDENTIFY_ALG);
#if (VIRTUAL_SENSOR_ENABLE == 1)
				dirty_foam_Data.reduc_speed=0;
#endif
			}else
		
			{ /* OK */
#if (VIRTUAL_SENSOR_ENABLE == 1)
				 dirty_foam_Data.fullPulseIn = 0;                  /* init var per conteggio impulsi entranti in pieno */
				/* algoritmo di riconoscimento sporco / schiuma */
				dirty_foam_Data.status = ALGO_SV_WAIT;
#endif
		    }
		 	CLRBIT(Flags_2, FLAG2_CLOGGED_FILTER);
#ifdef _STATISTICS_ON_	
			CLRBIT(Statistics_update_flag, STATISTIC_FLAG_DIRTY_UPDATE);
#endif
#if (VIRTUAL_SENSOR_ENABLE == 1)
			CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS); /* reset flag vuoto sensore in lavaggio */
		 	(void)SWTC_SetSValue((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER, (SWTC_SCounterValue)TOUT_STAB_STATO3_FILTER); 
			(void)SWTC_Stop((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER);
#endif
			break;
#if (VIRTUAL_SENSOR_ENABLE == 1)
		case AAL_ALG_MONITOR:
		
		
			if (uPDoorStatus == UP_DOOR_WAIT)
		  	{/* porta aperta o in attesa di timeout dopo la chiusura o delay MR-> il uP va sospeso */
   
			    (void)SWTC_Stop((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);
			    (void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
			    (void)SWTC_Stop((SWTC_CountersType)AAL_STAB_SENS_S_TIMER);
				(void)SWTC_Stop((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER);
				(void)SWTC_Stop((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER);
				return ;
			    
			}
			 		  
		  	if ( dirty_foam_Data.status==ALGO_LOAD_RESTORE)
			{/* deve essere fatto sia se il sensore ha un valore valido che invalido*/
				AAL_Load_Restore();
				break;
			}
		  	if ((TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH)))&& (!uP_AltCtrl_Data.SV_invalid_4alt))
	   		{
			
				if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3)
				{
					(void)SWTC_Run((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER);	
					if ( dirty_foam_Data.status == ALGO_SV_WAIT_END)
					{
						(void)SWTC_Run((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER);	
					}
				}else
				{
					(void)SWTC_SetSValue((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER, (SWTC_SCounterValue)TOUT_STAB_STATO3_FILTER); 
					(void)SWTC_Stop((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER);
					(void)SWTC_Stop((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER);
				}
				
				
				if (!SWTC_IsStopped((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER) && !SWTC_IsExpired((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER) )
				{ /* stato 3 del sensore stabile per tempo da setting  */
					return ; 
				}
			
			
				if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0)
	   			{/* vedo il vuoto esco */				
           			SETBIT(TerminatedAlgorithmBmp[0], AAL_DIRTY_FOAM_IDENTIFY_ALG);
		        	CLRBIT(ActiveAlgorithmBmp[0],AAL_DIRTY_FOAM_IDENTIFY_ALG);
					dirty_foam_Data.reduc_speed=0;
					dirty_foam_Data.status = ALGO_SV_END;
					dirty_foam_Data.filter_foam_on=FALSE;
					SETBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS);
					return;
				}
				switch (dirty_foam_Data.status)
				{
					case ALGO_SV_WAIT :
						dirty_foam_Data.reduc_speed=0;
						dirty_foam_Data.filter_foam_on=FALSE;
						if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3) 
						{
							dirty_foam_Data.status= ALGO_1LIMIT_POWER;	
							dirty_foam_Data.reduc_speed=1;
							dirty_foam_Data.filter_foam_on=TRUE;
						}
						
#ifdef _STATISTICS_ON_	
						CLRBIT(Statistics_update_flag, STATISTIC_FLAG_DIRTY_UPDATE);
#endif	
					break;
					case ALGO_1LIMIT_POWER:	
						
						dirty_foam_Data.reduc_speed=1;
						if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3) 
						{/* ho ridotto la velocità ma continuo a vedere filtro otturato */
							AAL_Load_Var_Init ();					
							dirty_foam_Data.status= ALGO_LOAD_RESTORE;
							dirty_foam_Data.load_exe_time=0;
							dirty_foam_Data.filter_foam_on=TRUE;
							
		
							/*torno alla velocità di prima */	
							dirty_foam_Data.reduc_speed=0;
						}else
							if ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==2) || (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==1))
							/* ho già ridotto la velocità una volta, e quindi si mantiene lo stato di riduzione della velocità*/
							{	
									dirty_foam_Data.status= ALGO_WAIT_1L;
									dirty_foam_Data.filter_foam_on=FALSE;
#ifdef _STATISTICS_ON_
									if ( TESTBIT(Flags_2, STATISTIC_FLAG_DIRTY_UPDATE)==0)
									{ 
										IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dirty_foam_redux_speed);
										SETBIT(Flags_2, STATISTIC_FLAG_DIRTY_UPDATE);
									}

#endif
							}
					break;
					case ALGO_WAIT_1L:
						/* dovrebbe essere già attiva ma ribadisce lo stato */
						dirty_foam_Data.reduc_speed=1;
						dirty_foam_Data.filter_foam_on=FALSE;
						if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3)
						{ 
						/* Se sono in questa condizione ho visto il pieno con la prima riduzione, quindi riduco di una seconda percentuale */
							dirty_foam_Data.status= ALGO_2LIMIT_POWER;
							dirty_foam_Data.reduc_speed=2;	
							dirty_foam_Data.filter_foam_on=TRUE;	
						}
					break;
					case ALGO_2LIMIT_POWER:

						dirty_foam_Data.reduc_speed=2;
						dirty_foam_Data.filter_foam_on=FALSE;
#ifdef _STATISTICS_ON_
						if ( TESTBIT(Flags_2, STATISTIC_FLAG_DIRTY_UPDATE))
						{ 
							IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dirty_foam_redux_speed);
							CLRBIT(Flags_2, STATISTIC_FLAG_DIRTY_UPDATE);
						}	
#endif
						if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3)
						{/*dopo la seconda riduzione , se non vedo mail il pieno il uP termina*/
							(void)SWTC_SetSValue((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER, (SWTC_MCounterValue)TOUT_END_ALYS3); 
							dirty_foam_Data.status = ALGO_SV_WAIT_END;
						}
					break;		
					case ALGO_LOAD_RESTORE:
						/* sto rabboccando ma ancora il sensore vale 3...*/
						/* se raggiungo il limite di caricamento allora il uP termina*/
						dirty_foam_Data.filter_foam_on=TRUE;
							
					break;				
					case ALGO_LOAD_WAIT:
						dirty_foam_Data.filter_foam_on=FALSE;
						if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==3) 
						{/* ho ridotto la velocità ma continuo a vedere filtro otturato */
							dirty_foam_Data.reduc_speed=0;
							AAL_Load_Var_Init();
						 
							dirty_foam_Data.load_exe_time=0;
												
							dirty_foam_Data.status= ALGO_LOAD_RESTORE;

							dirty_foam_Data.filter_foam_on=TRUE;
						}
						
					break;
					case ALGO_SV_WAIT_END:
						dirty_foam_Data.filter_foam_on=FALSE;
						if (SWTC_IsExpired((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER) )
						{	 /* vengo da una seconda riduzione di velocità e ho continuato a vedere 3  */
							dirty_foam_Data.status=ALGO_SV_END; 
							SETBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS);
							
					 	}
					break;
					case ALGO_SV_END:
						dirty_foam_Data.filter_foam_on=FALSE;
					default:
					break;
				}
			}

			/* algoritmo sospeso: non si fa nulla. rimane lo stato dove era.*/
			(void)SWTC_SetSValue((SWTC_CountersType)AAL_STAB_STATO3_S_TIMER, (SWTC_SCounterValue)TOUT_STAB_STATO3_FILTER);

		
			
		break;
#endif
		default:	
		case AAL_ALG_DISABLE:
		  SETBIT(TerminatedAlgorithmBmp[0], AAL_DIRTY_FOAM_IDENTIFY_ALG);
		  CLRBIT(ActiveAlgorithmBmp[0],AAL_DIRTY_FOAM_IDENTIFY_ALG);
#if (VIRTUAL_SENSOR_ENABLE == 1)
		  dirty_foam_Data.reduc_speed=0;
		  dirty_foam_Data.status= ALGO_SV_END;
		  dirty_foam_Data.filter_foam_on=FALSE;
		  (void)SWTC_Stop((SWTC_CountersType)AAL_END_ALWAYS3_M_TIMER);
#endif
		break;
	}
}

/* gestisce il caricamento con la logica del vecchio uP Load restore 
Il principio di funzionamento per i rabbocchi è quello del load restore.
la truttura dirty_foam_Data infatti ne ricalca la struttura e i parametri di ingresso.
Se è presente l'algoritmo dirty_foam_Data non deve essere presente il uP load restore.

\param [in]     none
\return         none
    
    
\par alg\b funzione per il rabbocco usato nell'algortitmo sporco / schiuma
\version		1.0.0
\date 			13/01/2012
\author			Emanuela Antognoli
*/
#if (VIRTUAL_SENSOR_ENABLE == 1)
static void AAL_Load_Restore (void)
{
	/* EA_COMMENT */

	SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_EXE);

	if ( TESTBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS) )
	{

		/* in analogia con la dichiarazione del KO Restore per WaterInDW > maxWater
		non si prosegue nel caricamento e viene abilitato avanzalomento passo 
		Il Jump su KO Restore verrà processato nel passo successivo 
		se Load Restore è l'unico uP del passo*/
		WaterInDW = OFFS_B(dirty_foam_setting_data.Data,0);
		dirty_foam_Data.reduc_speed=0;
		SETBIT(Flags_2, FLAG2_CLOGGED_FILTER);
		dirty_foam_Data.status = ALGO_SV_END;
		dirty_foam_Data.filter_foam_on=FALSE;
		return;
	}
	

	if ( ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==1)||(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==2))&& ( TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))) )
    {/* vuoto -> posso ancora caricare acqua, se ho tempo residuo */
					
		(void)SWTC_Run((SWTC_CountersType)AAL_STAB_SENS_S_TIMER);
	}else
	{
		/* else tempo di ripristino non esaurito */
		(void)SWTC_SetSValue((SWTC_CountersType)AAL_STAB_SENS_S_TIMER, (SWTC_SCounterValue)TOUT_STAB_SENS_FILTER);	
		(void)SWTC_Stop((SWTC_CountersType)AAL_STAB_SENS_S_TIMER);
	}
	
	if ( SWTC_IsExpired((SWTC_CountersType)AAL_STAB_SENS_S_TIMER) )
	{ /* caricamento a P.P. concluso (end conteggio per stabilità pressostato in pieno) */
		(void)SWTC_Stop((SWTC_CountersType)AAL_STAB_SENS_S_TIMER);
		dirty_foam_Data.status = ALGO_SV_WAIT;
		
		return ; 
	}

	if ( dirty_foam_Data.load_exe_time==0)
	{/* caricamento a turbina */
	
		if ( (PM_PulseCounter_GetWarning() == NO_PULSE_COUNTER_WARNING) && (TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) == 0) )
	    {/* Pulse counter OK && non bloccata */
		
    		if(  WaterInDW >= OFFS_B(dirty_foam_setting_data.Data,0) )			
			{
				/* non importa se il criterio non è valido, comunque abbiamo superato il valore massimo consentito per il caricamento , quindi 
				se non vedo il pieno è ko restore.*/
				SETBIT(Flags_2, FLAG2_CLOGGED_FILTER);
			  	if ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=1)&&(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=2) )
				{
					(void)SWTC_SetSValue((SWTC_CountersType)AAL_STAB_SENS_S_TIMER, (SWTC_SCounterValue)TOUT_STAB_SENS_FILTER);  
					SETBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* set flag overflow ripristini */	

					/* incremento contatore KO Restore per statistiche
					posso farlo qui perchè eseguo istruzione una volta soltanto dopo aver messo ad 1 il flag KO RESTORE
					e poi cambio stato in LOAD_RESTORE_TRUE */
#ifdef _STATISTICS_ON_                        
					MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.koRestore++;
#endif      
					
				}
				WaterInDW = OFFS_B(dirty_foam_setting_data.Data,0);
				dirty_foam_Data.status= ALGO_SV_END;
			}else
			{
				
				if ( TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))&&
			  	(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=5) )
				{
					/* ancora possibile caricare acqua */						
					if (dirty_foam_Data.init == TRUE)
					{/* questa init potrebbe avvenire anche dopo un passaggio per il pieno per cui fullPulseIn != 0*/

						GIWaterLoaded = 0;
						PulseLoaded = 0;
						dirty_foam_Data.emptyPulseIn = 0;

						dirty_foam_Data.waterInDWInit = WaterInDW; 
						PM_PulseCounter_SetCount(dirty_foam_Data.fullPulseIn);/* init pulse counter per nuovo conteggio */                                       /* init per caricamento con fullPulseIn = 0*/

						/* QI 28.10.09 init finestra di buio su controllo turbina. Messo qui per reinizializzare contatore al POR */
						(void)SWTC_SetSValue((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER, (SWTC_SCounterValue)TOUT_PULSE_COUNT_CHECK_DELAY);


						dirty_foam_Data.init = FALSE;

					}

					WaterInDW = (dirty_foam_Data.waterInDWInit + GIWaterLoaded);  /* aggiornamento acqua in vasca con quella caricata */
					/* QI 27.10.09 gestione finestra di buio su check turbina */
					TempCycle_Stats.loaded_water += GIWaterLoaded;
					cycle_loaded_water += GIWaterLoaded;
        
	       			FLT_PulseCounterCheckDelay_Mgmt();

	#ifdef _STATISTICS_ON_
					if((GIWaterLoaded>5) && (TempCycle_Stats.flags.bit.restoreCntEn==1)) 
					{                     
						MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.restore++;
						TempCycle_Stats.flags.bit.restoreCntEn =0 ;
					}
	#endif

					dirty_foam_Data.emptyPulseIn = (GIWaterLoaded * FlowMeterDivider);	 /* aggiornamento acqua in vasca in impulsi,
		                                                                                      rappresenta l'acqua in dl convertita in impulsi  */ 
	  				
					LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/
						      /* controllo turbina bloccata */
					if ( FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_EXE) == TRUE )
					{

						/* entro qui una sola volta,poi cambio modalità di ripristino */ 
						/* salva nella pila dei last prefault*/
						FLT_warningSave(FLT_CODE_PULSE_COUNTER, FLT_SUBCODE_PULSE_COUNTER, WRN_PREFAULT);
						dirty_foam_Data.load_exe_time=TRUE;

						SETBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS); 

					}
					
				}else
		        {/* stato 5  o criterio non valido*/
         
					/* non piloto EV ma tengo conto di eventuali impulsi conteggiati dalla turbina */
    
					/*impulsi contati durante la permanenza in pieno */
					if ( PulseLoaded >= dirty_foam_Data.emptyPulseIn )
					{
						dirty_foam_Data.fullPulseIn = ( PulseLoaded - dirty_foam_Data.emptyPulseIn );
					}

					dirty_foam_Data.init = TRUE;

					(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);	
		 			
					/* Rimango in questo stato anche se consento avanzamento passo */
	
					return ;
	
		        }
			}
		}

	    else
	    {/* Warning Pulse Counter: il conteggio dell'acqua in ingresso non è affidabile ->
	      non eseguo uP ma abilito avanzamento passo 	 */
       	  
	      dirty_foam_Data.load_exe_time = TRUE;
		  dirty_foam_Data.init = TRUE;  /* uso la stessa var perchè sono in uno stato diverso */
		  /* EA brevetto: se la turbina è KO si carica sempre a sensore , tanto al ciclo successivo o al caricamento successivo verrebbe ridato il fault.*/
	     /* turbina out of range*/
		  
		        
	    }
	}else

	{/*if ( dirty_foam_Data.load_exe_time )*/
		
	/* ripristino a tempo */
   		
		if ( TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))&&
	  	(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=5) )
		{ 
			
			/* carico pilotato  */
			LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/  

		}	
	}      

}


static void AAL_Load_Var_Init (void)
{
	TempCycle_Stats.flags.bit.restoreCntEn=1;

	/* Init variabili per caricamento a turbina */

	dirty_foam_Data.init = TRUE;

	PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);                        /* Reset di eventuali warning presenti */
	PulseLoaded = 0;
	PM_PulseCounter_SetCount(dirty_foam_Data.fullPulseIn);/* init pulse counter per nuovo conteggio */                                       /* init per caricamento con fullPulseIn = 0*/
	(void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT);     /* init funzione controllo turbina bloccata */
	dirty_foam_Data.fullPulseIn = 0;                  /* init var per conteggio impulsi entranti in pieno */
    GIWaterLoaded = 0;

	dirty_foam_Data.waterInDWInit = WaterInDW; 

	CLRBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);
	dirty_foam_Data.load_exe_time=0;

	(void)Set_SmallCounter((SWTC_CountersType)AAL_STAB_SENS_S_TIMER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)TOUT_STAB_SENS_FILTER);	
	
	
}
#endif
/* *********** */
/* End of file */
/* *********** */
