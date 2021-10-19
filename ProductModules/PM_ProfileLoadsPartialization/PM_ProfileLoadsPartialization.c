/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_ProfileLoadsPartialization  
@{
\n			Implementation of the Product Library to achieve the aim
\n			This module allows to pilot A.C. loads with partialized wave
\n      and, consequently, modulated power
\n
\par    Configuration:
\n
\n\li	  A_CFG
\n
\par    Exported Functions:
\n
\n\li
\n\li
\n  
@}
\ingroup   	ProductM
*/
/**
\n
\par      PM_ProfileLoadsPartialization.c
\n        Implementation of all functions necessary to achieve the aim.
\n
\file     PM_ProfileLoadsPartialization.c
\ingroup  ProfileLoadsPartialization
\date     Oct 25, 2011
\version  01.00.00
\author	  Leonardo Sparti
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Oct 25, 2011
\n		Leonardo Sparti
\n		changes
\n 			Creation
\n
\n
*/



/* ******** */
/* Includes */
/* ******** */

#include "MainInterface.h"
#include "OutputManagerExp.h"
#include "PM_ProfileLoadsPartialization.h"
#include "PM_ProfileLoadsPartializationExp.h"
#include "OCTPM_drv.h"
#include "OCIRQ_drv.h"
#include "DigitalOutputs.h"
#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_PM_PLP_DEBUG_ISR_TRACE_PIN) && defined(CONFIG_PM_PLP_DEBUG_SCH_TRACE_PIN)
#if CONFIG_PM_PLP_DEBUG_ISR_TRACE_PIN == CONFIG_PM_PLP_DEBUG_SCH_TRACE_PIN
#error "CONFIG_PM_PLP_DEBUG_ISR_TRACE_PIN has the same value of CONFIG_PM_PLP_DEBUG_SCH_TRACE_PIN"
#endif
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*partialization value [us]*/
typedef uint16_t	PLP_PartializationValType;


/* Loads Structure */
#if !defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
/* For the only Dimmer, a simpler structure is sufficient */
typedef struct PLP_LStruct {

	uint8_t   pin;				    /* Pin */
	IO_HwTimerType* ptractualoc[2];		
	
	uint8_t  activated;
	uint8_t  toactivate;
    
	IO_HwTimerType  OcValue[2];

	uint16_t* p_nextprof; 	  /* Pointer to the next profile of the table */
	
	struct PLP_LStruct *pnxtld[2];
	
} PLP_LType;

#else  /* CONFIG_PM_PLP_NORMAL_SUPPORT defined */

/* PLP load struct */
typedef struct PLP_LStruct {

	uint8_t   pin;					/* Pin */
	uint8_t   stato;				/* Load Status */
	uint8_t   repetition;			/* The profile has to be repeated or not */
	uint8_t   counter;        
	uint8_t   profilelen;			/* Length of the profile in halfwaves */
	PLP_PartializationValType* ptractualprof;		/* Pointer to the actual profile */
	PLP_PartializationValType* ptractualoc[2];		/* */
	uint8_t   pylottype;			/* PilotType */
	
    uint8_t  activated;
    uint8_t  toactivate;
    
    PLP_PartializationValType  OcValue[2];
    int16_t  Comp;

    uint8_t  sync;             		/* 1: with negative halfwave; */
    								/* 0: with negative halfwave; */
    uint8_t  ToCompensate;
	uint16_t* p_nextprof;			/* Pointer to the next profile of the table */
	
	struct PLP_LStruct *pnxtld[2];
	
} PLP_LType;

#endif  /* !defined(CONFIG_PM_PLP_NORMAL_SUPPORT) */


/* Scheduler Structure */
typedef struct PLP_SStruct
{
	uint8_t  state;
	struct PLP_LStruct *pfstld[2];
	struct PLP_LStruct *pcurrld;
	
} PLP_SType;


/* ********* */
/* Variables */
/* ********* */
#if defined(CONFIG_PM_PLP_EMULATE_PROFILE) && defined(CONFIG_PM_PLP_DEBUG)
PLP_PartializationValType Profilerus[4][4]=
{
	{(800), (100), (2000), (5000)},
	{(300), (1500), (3500), (7000)},
	{(800), (2000), (1000), (5000)},
	{(300), (1500), (3500), (7000)}
};
#endif

IO_OCU_CfgType OcuPLP_cfg =
{
	CONFIG_PM_PLP_TMR_CH_ID,	/* timer ch ID*/
	TMR_CH_INV
};

PLP_SType PLPScheduler;

PLP_LType PLPVar[CONFIG_PM_PLP_MAX_ITEMS_NUMBER];

uint8_t PLPLoads=0;


static Main_SequencerActivityType Act_PLP_Scheduler;

#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)

/* Value of activation point of partialization for dimmer loads */
uint16_t dimReg[CONFIG_PM_PLP_MAX_ITEMS_NUMBER];
/* Step of the Dimmer */
uint8_t dimStep[CONFIG_PM_PLP_MAX_ITEMS_NUMBER];

/* Status of a dimmer load */
uint8_t OnOffStatus[CONFIG_PM_PLP_MAX_ITEMS_NUMBER];

uint16_t dimMax=DIM_MAX;    /* Maximum value of partialization */
uint16_t dimMin=DIM_OFF;    /* Minimum value of partialization */

#endif /* defined(CONFIG_PM_PLP_DIMMER_SUPPORT) */



/* ******************* */
/* Functions Prototype */
/* ******************* */
static IO_ErrorType PLP_OCU_Event(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType validate);
static uint8_t PLP_LoadsAtZC(LIB_ZcEdgeType edge);
static void PLP_SetOC(LIB_ZcEdgeType isredge);
static void PLP_DriverPins(LIB_ZcEdgeType edge);
static void PLP_turnOff(void);
static void PM_PLP_InitActivity(void);
static void Init_PLP_DrvResources(void);
/* *************** */
/* Function Bodies */
/* *************** */

/**
This function adds a load with a specific type in order to be
able to manage different types of loads
\n

\param  [in]	uint8_t pin      ->   pin module
\param  [in]	uint8_t type     ->   load id


\return 	

\retval
       

\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_PLP_InsertLoad(uint8_t pin, uint8_t type)
{
    PLP_LType *pPLPVar;


    if(PLPLoads >= CONFIG_PM_PLP_MAX_ITEMS_NUMBER)
    {
    	/* nothing to do */
        return ;
    }
    
    pPLPVar = &PLPVar[PLPLoads];
    
    if(pPLPVar->pin==0xFF)
    {
        pPLPVar->pin=pin;
        
#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
        pPLPVar->pylottype=type;
#else
        (void)type;
#endif
        
        PLPLoads++;  /* inc the index */
        return;
    }

}


#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
/**
Set Load Function
\n

\param  [in]	uint8_t on          ->   on/off status
\param  [in]	uint8_t pin         ->   pin module
\param  [in]	uint16_t* pProfile  ->   profile pnt
\param  [in]	uint8_t len         ->   profile length
\param  [in]	uint8_t repeat      ->   repetition (0 = infinite)
\param  [in]	uint8_t cfg         ->   configuration (sync and compensation)


\return 	

\retval   
          

\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_PLP_SetLoad(uint8_t on, uint8_t pin, PLP_PartializationValType* pProfile, uint8_t len, uint8_t repeat, uint8_t cfg)
{
    uint8_t i=0;
    bool_t found = FALSE;
    PLP_LType *pPLPVar;

	/* find the desired pin to set */
	while((!found) && (i<PLPLoads))
	{
		pPLPVar = &PLPVar[i];

		if(pPLPVar->pin == pin)
		{
			found=TRUE;

			switch(pPLPVar->pylottype)
			{
#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)
			case DIM_LAMP_L:
				/* do nothing */
				break;
#endif /* defined(CONFIG_PM_PLP_DIMMER_SUPPORT)*/

			default:
				/*
				 * TODO : check why this has been commented
				//if load is already activated generate an error
				if(PLPVar[i].activated)
					return;
				*/
				if(on)
				{
					/* if pnext profile is empty concatenate profile */
					if((pPLPVar->p_nextprof!=0))
					{
						return ;
					}

					/* insert: */

					pPLPVar->p_nextprof=pProfile;           /* ptr to next profile */
					pPLPVar->repetition=repeat;             /* repetition */
					pPLPVar->profilelen=len;                /* profile length */
					pPLPVar->sync=(uint8_t)(cfg&1);         /* configuration: sync & compensation */
					pPLPVar->ToCompensate=(uint8_t)(cfg&2);
					pPLPVar->toactivate=1;                  /* book activation! */

				}
				else
				{
					pPLPVar->activated=0;                   /* turn off load (without profile completion) */
				}
			}
		}
		i++;
	}
}
#endif /*ENABLE_SIMPLE_PLP is defined */


#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)
/**
Set Load for a dimmer pilot type Function
\n

\param  [in]	uint8_t on          ->   on/off status
\param  [in]	uint8_t config      ->   configuration nibble
\param  [in]	uint8_t pin         ->   pin module
\param  [in]	uint8_t stepPart    ->   step of partialization

\param  [in]	uint16_t maxPart    ->   max partialization value
\param  [in]	uint16_t minPart    ->   min partialization value


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_DIM_SetLoad(uint8_t on, uint8_t config, uint8_t pin, const uint8_t* stepPart)
{
  uint8_t i;
  PLP_LType *pPLPVar;
  
  static uint8_t OLD_on = 0;
  
  
/* 
 // These settings could allow to calibrate dimMax and dimMin
 // value by editor but, at moment, they shall not be used
   dimMax = DIM_OFF-(*maxPart)+PLP_DRIVELBOUNDtck;
   dimMin = DIM_OFF-(*minPart);
*/

#if !defined(CONFIG_PM_PLP_SLIDER_SUPPORT)

  dimMax = DIM_MAX;
  dimMin = DIM_OFF;

#else  /* Slider case */

	for(i=0;i<PLPLoads;i++)
	{
		pPLPVar = &PLPVar[i];

		if(pPLPVar->pin==pin)
		{
			if(on > OLD_on)
			{
				dimMax = (uint16_t)( SLID_OFF - (SLID_STEP*on) );
				OLD_on = on;
			}
			else if(on < OLD_on)
			{
				dimMin = (uint16_t)( SLID_OFF - (SLID_STEP*on) );
				OLD_on = on;
				on=0;
			}
			else  /* on = OLD_on */
			{
				if( OnOffStatus[i] == 0 )
				{
					on=0;
				}
			}
		}
	}
  
#endif  /* !defined(SLIDER_OPTION) */
  
  
	/* find the desired pin to set */
	for(i=0;i<PLPLoads;i++)
	{
	  
		pPLPVar = &PLPVar[i];
	
	
#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
		if(pPLPVar->pylottype != DIM_LAMP_L)
		{
			continue;
		}
#endif
		if(dimReg[i] == 0xFFFF)                /* Initial condition */
		{
			dimReg[i] = dimMin;                  /* Variable initialization at application startup */
		}
	
		if(pPLPVar->pin==pin)
		{
			if (!config)
			{
				dimReg[i] = (uint16_t)( (on)?DIM_MAX:DIM_OFF );  /* config=0 -> Dimmer disabled, ON-OFF command */
			}


#if defined(CONFIG_PM_PLP_SLIDER_SUPPORT)
		  dimStep[i] = 10; /* Slider requires a fixed step */
		  (void)*stepPart;
#else
		  /* Acquisition of setting values */
		  dimStep[i] = *stepPart;
#endif  /* SLIDER_OPTION */


		  if ((dimReg[i] == DIM_OFF) && (!on))
		  {
			 pPLPVar->activated=0;              /* turn off load (without profile completion) */
		  }
		  else
		  {
			 pPLPVar->p_nextprof=&dimReg[i];    /* ptr to next profile */
			 pPLPVar->toactivate=1;             /* book activation */
		  }

		  OnOffStatus[i] = (uint8_t)( (on)?1:0 );    /* notify ON or OFF status */
		  return;
		}
	} /* end for loop */
}
#endif /* defined(CONFIG_PM_PLP_NORMAL_SUPPORT) */


/**
Check on warm reset if is necessary to enable the ISR of the channel
\n

\param  [in]	


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_PLP_Init_OnWarmReset(void)
{
	Init_PLP_DrvResources();
	
	if(PM_PLP_LoadsToDrive() == TRUE)
	{
		(void)OCOCU_EnableISR(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_ISR_ENABLE, PLP_OCU_Event);
	}
	PM_PLP_InitActivity();
}


/**
Inits Loads Structure and timer cfg
\n

\param  [in]	


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_PLP_Init(void)
{
    uint8_t i;

    Init_PLP_DrvResources();

    /*initialize global module structure*/
    for(i=0;i<CONFIG_PM_PLP_MAX_ITEMS_NUMBER;i++)
    {
		PLPVar[i].pin=0xFF;

		PLPVar[i].pnxtld[0]=(PLP_LType*)NULL;
		PLPVar[i].pnxtld[1]=(PLP_LType*)NULL;
    	
		/*Dimmer-specific initialization*/
#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)
			/* Dimmer variables initialization */
			dimReg[i]=0xFFFF;
			dimStep[i]=0;
			OnOffStatus[i]=0;
#endif
    }

    /* Sequencer activity configuration:
    Invoke PLP at first event
    */
    PM_PLP_InitActivity();

#if defined(CONFIG_PM_PLP_EMULATE_PROFILE) && defined(CONFIG_PM_PLP_DEBUG)
    /*
    Insert a dummy load
    */
    PM_PLP_InsertLoad(0x12, DIGITAL_L);
    /* dummy pin */
    PM_PLP_SetLoad(TRUE, 0x12, &Profilerus[0][2], 2, 0, 0);
#endif /* defined(CONFIG_PM_PLP_EMULATE_PROFILE) && defined(CONFIG_PM_PLP_DEBUG)*/
}


/**

 */
static void PM_PLP_InitActivity(void)
{
#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
    Main_AddSequencerActivity ( &Act_PLP_Scheduler, &PM_PLP_Scheduler );
#else
    Main_AddSequencerActivity ( &Act_PLP_Scheduler, &DIM_Scheduler );
#endif
    Act_PLP_Scheduler.profile=1;
}


/**
Detect Loads to be driven
\n

\param  [in]	


\return

\retval      TRUE        there are loads to drive
             FALSE       no loads to drive


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
uint8_t PM_PLP_LoadsToDrive(void)
{
	uint8_t i;
	
	
	for(i=0; i<PLPLoads; i++)
	{
	  if(PLPVar[i].toactivate || PLPVar[i].activated)
	  {
	      return TRUE;
	  }
	}
	return FALSE;
}








#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
/**
PLP Manager
\n

\param  [in]	int16_t  avgvoltagecomp    ->   vline compensation


\return

\retval   


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void PM_PLP_Scheduler(void)
{
    uint8_t i;
    PLP_LType * pzcl_;
    PLP_LType** ppzcl_;

    TRACE_PLP_START();
    LIB_ZcEdgeType edge = (LIB_ZC_GetCurrentEdge()? ZC_EDGE_RISING : ZC_EDGE_FALLING); /* edge of synchronization */
    LIB_ZcEdgeType edgeNxt = (LIB_ZcEdgeType)( (edge==ZC_EDGE_RISING)? ZC_EDGE_FALLING : ZC_EDGE_RISING);
    /* as soon as possible like in the ZC */

    /*PM_PLP_SetLoad(TRUE, 0x12, &Profilerus[0][2], 2, 0, 0);*/

    /*default: nothing to do*/
	PLPScheduler.state=2;
	if(PLPScheduler.pfstld[edge]!=0)
	{
		/*there are loads to drive in this halfwave */
		PLPScheduler.state=0;
		PLPScheduler.pcurrld=PLPScheduler.pfstld[(edge)];
	}
	/* forcing loads to turn off */
  	PLP_turnOff();
  	/* check for loads to drive at ZC event*/
	if(PLP_LoadsAtZC(edge)!=0)
	{
		PLP_DriverPins(edge);
	}
	/*plan next OCU event*/
	PLP_SetOC(edge);

    
    
    /* init scheduler list */
    PLPScheduler.pfstld[edgeNxt]=0;
    
    
    if(PLPLoads!=0)
    {
    	pzcl_=PLPVar;

		/* scan all loads */
    	for(i=0; i<PLPLoads; i++,pzcl_++)
		{
#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)
			if(pzcl_->pylottype == DIM_LAMP_L)
			{
				if(dimReg[i] == 0xFFFF)    /* Avoid dimmering if initialization is not yet present */
				{
					return;
				}

				if(OnOffStatus[i])         /* Increasing Dimmer */
				{
					if( dimReg[i] >= (dimStep[i]+dimMax) )
					{
						dimReg[i] -= dimStep[i];
					}
					else
					{
						/* Saturation value */
						dimReg[i] = dimMax;
					}
				}
				else                       /* Decreasing Dimmer */
				{
					dimReg[i] += dimStep[i];
					if( dimReg[i] > dimMin )
					{
						/* Saturation value */
						dimReg[i] = dimMin;

						if(dimReg[i] == DIM_OFF)
						{
						  pzcl_->activated=0;     /* turn off load */
						}
					}
				}

			}
#endif   /* defined(CONFIG_PM_PLP_DIMMER_SUPPORT) */

			/* if load is to activate */
			if((pzcl_->toactivate) && (pzcl_->p_nextprof!=0))
			{
				/* check sync */
				if(pzcl_->sync==edge)
				{
					pzcl_->activated=1;
					pzcl_->toactivate=0;
					pzcl_->ptractualprof=pzcl_->p_nextprof;     /* set next profile as actual */
					pzcl_->p_nextprof=0;                        /* set next profile as empty */
					pzcl_->ptractualoc[edge]=pzcl_->ptractualprof+pzcl_->profilelen;
					pzcl_->counter=0;                           /* reset cnt */
				}
			}

			if(pzcl_->activated)
			{
				/* if the loc pnt is at the end */
				if(pzcl_->ptractualoc[edge]>=pzcl_->ptractualprof+pzcl_->profilelen-1)
				{
					/* set the loc pnt at the begin of the profile */
					pzcl_->ptractualoc[edgeNxt]=pzcl_->ptractualprof;



					/* if the profile has repetition */
					if(pzcl_->repetition)
					{
						if(pzcl_->counter<pzcl_->repetition)
						{
							/* inc cnt */
							pzcl_->counter++;
						}
						else
						{
							/* driving has been completed!!! */
							pzcl_->activated=0;
						}
					}
				}
				else
				{
					/* inc the loc pnt */
					pzcl_->ptractualoc[edgeNxt]=pzcl_->ptractualoc[edge]+1;
				}


				/* Check if the load is to compensate */
				if(pzcl_->ToCompensate)
				{
					/* multiply the avg vline value */
					/*pzcl_->Comp=(int16_t)(avgvoltagecomp);*/ /* TODO Compensation at moment is unused */
					/* the old statement was: (avgvoltagecomp<<PLP_COMPENSATION_MULTIPLIER); */
				}
				else
				{
					pzcl_->Comp=0;
				}

				/* update the OC value */
				pzcl_->OcValue[edgeNxt]= (IO_HwTimerType)((pzcl_->Comp+*(pzcl_->ptractualoc[edgeNxt]))/* *(TIMER_SCALING)*/);

				if(pzcl_->OcValue[edgeNxt] < CONFIG_PM_PLP_FORCE_DELAY_AT_ZERO_TH)
				{
					pzcl_->OcValue[edgeNxt]=0;
				}
			}

			/* insert the load in the list if the load is activated */
			if(pzcl_->activated)
			{
				ppzcl_=&PLPScheduler.pfstld[edgeNxt];

				do{
					if((!*ppzcl_) || (pzcl_->OcValue[edgeNxt] < ((*ppzcl_)->OcValue[edgeNxt])))
					{
						pzcl_->pnxtld[edgeNxt]=*ppzcl_;
						*ppzcl_=pzcl_;
						break;
					}
					else
					{
						ppzcl_=&((*ppzcl_)->pnxtld[edgeNxt]);
					}
				}
				while(1);
			}
			else
			{
				/* reset the pointer */
				pzcl_->pnxtld[edgeNxt]=(PLP_LType*)0;
			}

			
		}
    }
    TRACE_PLP_END();
}
#endif /* ENABLE_SIMPLE_PLP defined */


#if !defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
/**
Dimmer Manager
\n

\param  [in]	


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void DIM_Scheduler(void)
{
    uint8_t i;
    uint8_t isredge;
    PLP_LType * pzcl_;
    PLP_LType** ppzcl_;
    
    
    pzcl_=PLPVar;

    /* edge of synchronization */
    isredge = (uint8_t)OCIRQ_GetEdgeSync();

    /* init scheduler list */
    PLPScheduler.pfstld[1^isredge]=0;
    
    if(PLPLoads==0)
    {
        return;
    }
    
    /* scan all loads */
    for(i=0; i<PLPLoads; i++,pzcl_++)
    {
#if defined(CONFIG_PM_PLP_DIMMER_SUPPORT)
        if(dimReg[i] == 0xFFFF)    /* Avoid dimmering if initialization is not yet present */
        {
          return;
        }
    
        if(OnOffStatus[i])         /* Increasing Dimmer */
        {
          if( dimReg[i] >= (dimStep[i]+dimMax) )
          {
            dimReg[i] -= dimStep[i];
          }
          else
          {
            /* Saturation value */
            dimReg[i] = dimMax;
          }
        }
        else                       /* Decreasing Dimmer */
        {
          dimReg[i] += dimStep[i];
          if( dimReg[i] > dimMin )
          {
            /* Saturation value */
            dimReg[i] = dimMin;
			
            if(dimReg[i] == DIM_OFF)
            {
              pzcl_->activated=0;     /* turn off load */
            }
          }
        }
#endif /* defined(CONFIG_PM_PLP_DIMMER_SUPPORT) */
        /* if load is to activate */
        if(pzcl_->toactivate)
        {
            pzcl_->activated=1;
            pzcl_->toactivate=0;
            pzcl_->ptractualoc[isredge]=pzcl_->p_nextprof;
        }
        
        if(pzcl_->activated)
        {
            /* set the loc pnt at the begin of the profile */
            pzcl_->ptractualoc[1^isredge]=pzcl_->p_nextprof;
            
            /* update the OC value */
            pzcl_->OcValue[1^isredge]= (IO_HwTimerType)(PLP_TIMER_SCALING((*pzcl_->ptractualoc[1^isredge])));

            if(pzcl_->OcValue[1^isredge]<PLP_DRIVELBOUNDtck)
            {
                pzcl_->OcValue[1^isredge]=0;
            }
        
            /* insert the load in the list if the load is acivated */
            ppzcl_=&PLPScheduler.pfstld[1^isredge];
        
            do{                        
                if((!*ppzcl_) || (pzcl_->OcValue[1^isredge] < ((*ppzcl_)->OcValue[1^isredge])))
                {
                    pzcl_->pnxtld[1^isredge]=*ppzcl_;
                    *ppzcl_=pzcl_;
                    break;
                }
                else
                {                   
                    ppzcl_=&((*ppzcl_)->pnxtld[1^isredge]);
                }
            }
            while(1);
        }
        else
        {
            /* reset the pointer */
            pzcl_->pnxtld[1^isredge]=(PLP_LType*)0;
        }
        
    }
}
#endif /* !defined(ENABLE_SIMPLE_PLP) */


/**
PLP ISR Callback
\n

\param  [in]	IO_NotificationType notifVal
\param  [in]	IO_ChannelType chid
\param  [in]	IO_ErrorType validate


\return 	IO_ErrorType

\retval		IO_E_OK


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
static IO_ErrorType PLP_OCU_Event(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType validate)
{
	LIB_ZcEdgeType edge = (OCIRQ_GetEdgeSync()? ZC_EDGE_RISING : ZC_EDGE_FALLING); /* edge of synchronization */

	(void) notifVal;
	(void) chid;
	(void) validate;

	TRACE_PLP_ISR_START();

	/* Drive the pins */
	PLP_DriverPins(edge);
	/* Set next OC */
	PLP_SetOC(edge);
	
	TRACE_PLP_ISR_END();

	return (IO_ErrorType)IO_E_OK;
}



/**
Interrupt service routine of PLP. The function calls the assigned callback function.
 
\param [in] 	void

\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/

void PLP_ISR(void)
{
	/* Calls the user callback */
	(void)(PLP_OCU_Event)((IO_NotificationType)0, (IO_ChannelType)0, (IO_ErrorType)0);
}


/**
Detect Loads to be driven on ZC event
\n

\param  [in]	edge   edge to be considered (as each edge could have its own loads to drive)


\return

\retval      TRUE        there are loads to drive
             FALSE       no loads to drive


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
static uint8_t PLP_LoadsAtZC(LIB_ZcEdgeType edge)
{

    if((PLPScheduler.pfstld[(edge)]) && (!(PLPScheduler.pfstld[(edge)]->OcValue[(edge)])))
    {
        return TRUE;
    }
    return FALSE;

}

/**
Set OC Value into OC register
\n

\param  [in]	LIB_ZcEdgeType  isredge   ->   edge


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
static void PLP_SetOC(LIB_ZcEdgeType isredge)
{
	PLP_LType * zcl;
	uint16_t 	ticks=0;

	zcl=PLPScheduler.pcurrld;

	switch(PLPScheduler.state)
	{
	    case 0:
	    	/* set event */
	    	OCOCU_SetEventSync(CONFIG_PM_PLP_TMR_CH_ID, HW_CNTR_TRIG_ZC, zcl->OcValue[(isredge)]*1000UL);
	    	/* enable ISR */
	    	(void)OCOCU_EnableISR(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_ISR_ENABLE, PLP_OCU_Event);
	      break;

	    case 1:
	    	/* Last phase angle OC value to turn off all loads */
	    	ticks =  (isredge)?( LIB_ZC_GetHalfWaveLengthNeg() - CONFIG_PM_PLP_TURNOFF_MARGIN ):( LIB_ZC_GetHalfWaveLengthPos() - CONFIG_PM_PLP_TURNOFF_MARGIN );
	    	/* set event */
	    	OCOCU_SetEventSync(CONFIG_PM_PLP_TMR_CH_ID, HW_CNTR_TRIG_ZC, ticks*1000UL);
			/* enable ISR */
			(void)OCOCU_EnableISR(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_ISR_ENABLE, PLP_OCU_Event);
	      break;

	    default:
   	        break;
	}
}

/**
Pin driver. Multiple activations are managed too.
\n

\param  [in]	LIB_ZcEdgeType  edge    ->   edge


\return

\retval


\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
static void PLP_DriverPins(LIB_ZcEdgeType edge)
{
	PLP_LType * zcl;
	uint8_t i,multiple;


	zcl=PLPScheduler.pcurrld;   /* Get current pnt */

    i=	PLPScheduler.state;   /* Get Sch state */
	switch(i)
	{
	    case 0:
	        do
        	{
	        	/* activate all pin with the same or 'similar' OC */
	        	multiple=FALSE;
#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
        		if(zcl->pylottype == RELEZC_L)
        		{
        			LIB_DIO_SET_LOAD( (uint8_t)((zcl->stato)?(OFF):(ON)),zcl->pin );
        		}
        		else
#endif /* defined(CONFIG_PM_PLP_NORMAL_SUPPORT) */
        		{
        			LIB_DIO_SET_LOAD(ON,zcl->pin);
        		}

        		if((zcl->pnxtld[(edge)]) && ((CONFIG_PM_PLP_GROUP_ACTIVATION_TH + (((zcl)->OcValue[(edge)]))) >= ((zcl->pnxtld[(edge)])->OcValue[(edge)])))
        		{
        		    multiple=TRUE;
        		}

        		if(!zcl->pnxtld[(edge)])
        		{
        		    /* change Scheduler state if this is the last load of the list */
        		    PLPScheduler.state=1;
        		    PLPScheduler.pcurrld=0;/*RF*/
        		}
        		else
        		{
        			/* get the next load*/
        		    zcl=zcl->pnxtld[(edge)?1:0];
        		    PLPScheduler.pcurrld=zcl;
        	    }
        	}
        	while(multiple);
	        break;

	    case 1:
	        /* it's time to turn off the loads */
	        PLP_turnOff();

	        /* job is done */
	        PLPScheduler.state=2;
	        PLPScheduler.pfstld[(edge)?1:0]=0;
	        PLPScheduler.pcurrld =0;/*RF*/
	        break;
	    default:
	        /* do nothing!!! */
   	        break;
	}

}

/**
Turn off all loads except RELEZC_L

\author   Leonardo Sparti
\date     25/10/2011
\version  1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti
*/
static void PLP_turnOff(void)
{
    PLP_LType * pzcl_;
   	uint8_t i;

   	/* disable all loads .. */
    for(i=0,pzcl_=PLPVar; i<PLPLoads; i++,pzcl_++)
    {
#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)
    	/* ... except ones with PilotType RELEZC_L*/
    	if(pzcl_->pylottype != RELEZC_L)
#endif
        {
          LIB_DIO_SET_LOAD(OFF,pzcl_->pin);
        }
    }

    /* disable interrupt OC  */
    (void)OCOCU_EnableISR(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_ISR_DISABLE, PLP_OCU_Event);

}


/**
 * initzializes drivers resources
 */
static void Init_PLP_DrvResources(void)
{
	/* Compare Match function Init. */
	(void)OCOCU_DeInitSync(CONFIG_PM_PLP_TMR_CH_ID);
	(void)OCOCU_InitSync(&OcuPLP_cfg);
	(void)OCOCU_SetModeSync(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_OC_OFF);
	(void)OCOCU_EnableISR(CONFIG_PM_PLP_TMR_CH_ID, TMR_CH_ISR_DISABLE, NULL);
}
