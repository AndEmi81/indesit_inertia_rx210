/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_PWM_LoadsControl  
@{
\n      Implementation of the Product Library to achieve the aim
\n
\par    Configuration:
\n
\n\li	A_CFG
\n
\par    Exported Functions:
\n
\n\li	 void PWMLoadInit ( uint8_t pin, uint8_t pilottype )
\n\li   void PWMResetLoad( void )
\n\li   void PWMSetLoad (uint8_t reg, uint8_t config, uint8_t pin, const uint8_t *const pStruct)
\n\li   void PWMControlManager ( void )
\n\li   static void SetDuty ( uint8_t channel_Id, uint16_t D )
\n  
@}
\ingroup   	ProductM 
*/
/**
\n
\par		PM_PWM_LoadsControl.c
\n			inside this module we manage to activate PWM loads in 2 different type: Dimmer and Constant PWM
\n      For the Dimmer loads there is the possibility to switch between control on/off and Dimmer considering 
\n      the LoadParam.s.config value, for the Constant PWM control there are 15 parameters in the setting file
\n      that they can be adressed using LoadParameter.s.reg.
\n
\file		PM_PWM_LoadsControl.c 
\ingroup	PM_PWM_LoadsControl
\date			31/05/2011 
\version	01.00.00 
\author		Giuseppe Boccolato 
\n
\n
\par\b	History:
\n 
\n 		01.00.00 
\n		31/05/2011 
\n		Giuseppe Boccolato 
\n		changes 
\n 			Creation
\n
\n
\todo  PWM_PROFILER must be tested when the Load will be included in the setting file.

*/

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "OutputManagerExp.h"
#include "PM_PWM_LoadsControl.h"
#include "OCTPM_drv.h"
#include "OCDIO_drv.h"

#if defined(CONFIG_PM_PWM_LOADSCONTROL)

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct PWM_CTRLStruct {

	uint8_t   pin;              /* Pin */
	uint8_t   Ch_Id;            /* Channel Identifier */
	uint8_t   PylotType;        /* Pylot Type */
	uint16_t  PWMReg;           /* Duty Cycle regulator */
	uint8_t   initDone;         /* Params Initialization Done ghost */
	uint8_t   indexDimmer;	
} PWM_CTRLType;


typedef struct PWM_DimmerStruct {
   /* LED Dimmer parameters */
  uint8_t   tr;               /* Rise time, 10 ms multiple */
	uint8_t   tf;               /* Fall time, 10 ms multiple */
	uint16_t  pt1;              /* V/L characteristic of LED... */
	uint16_t  pt2;
	uint16_t  D1;               /* ...best fitting parameters */
	uint16_t  D2;
	uint16_t  step;             /* Step of the Dimmer */
	uint8_t   OnOffStatus;      /* Status of the dimmer load */
} PWM_DimmerType;

 

/* ********* */
/* Variables */
/* ********* */

/* Global */
extern IO_PWM_CfgType ChTmrPWM_cfg[]; 

/* Static */
static PWM_CTRLType PWMVar[CONFIG_PM_PWM_LOADSC_PWM_NUM];
static PWM_DimmerType PWMDimmerVar[CONFIG_PM_PWM_LOADSC_DIMR_NUM];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void SetDuty ( uint8_t channel_Id, uint16_t D );

/* *************** */
/* Function Bodies */
/* *************** */



/**
<Initialization of PWM loads> 

\param  [in]		pin ofset vs ModuleIOmap
\param  [out]		
\param  [in,out]	<pin , pilottype> 	 

\attention \b 

\return 	void

\retval		none		 	 

\warning			

\author   Giuseppe Boccolato 
\date		  31/05/11 
\version	1.0.0

\bug	

\todo		
   
\note	
\par 
\b Note 1:
			documentation by Giuseppe Boccolato
			\par \b Note n:	

*/


void PWMLoadInit ( uint8_t pin, uint8_t pilottype )
{
IO_ChannelType tmrCh;
IO_ChannelType i;	
uint8_t indexdimmer=0;  
    if ( OCTPM_GetChannelSync(OCDIO_GetIOMapItem(pin),&tmrCh) == (IO_ErrorType)IO_E_OK )
    {
        /* There is a timer channel linked to pin, is this channel configured as a Pwm channel ?  */
        for ( i = 0; i < CONFIG_PM_PWM_LOADSC_PWM_NUM; i++ )
        {
        
            if ( tmrCh == ChTmrPWM_cfg[i].PWM_Id )
            {
                /* Set structure for the Manager function */
                PWMVar[i].pin = pin;
                PWMVar[i].Ch_Id = tmrCh;
                PWMVar[i].PylotType = pilottype;
                if(DIM_LED_L==pilottype){
                  PWMVar[i].indexDimmer=indexdimmer;
                  indexdimmer++;
                } 
                (void)OCPWM_InitSync(&ChTmrPWM_cfg[i]);
            }    
        }    
    }	
}


/**
<Reset of PWM loads> 

\param  [in]		pin ofset vs ModuleIOmap
\param  [out]		
\param  [in,out]	 	 

\attention \b 

\return 	void

\retval		none		 	 

\warning			

\author   Giuseppe Boccolato 
\date		  31/05/11 
\version	1.0.0

\bug	

\todo		
   
\note	
\par 
\b Note 1:
			documentation by Giuseppe Boccolato
			\par \b Note n:	

*/

void PWMResetLoad( void )
{
uint8_t i;
	
	for( i=0; i < (uint8_t)(CONFIG_PM_PWM_LOADSC_PWM_NUM); i++ )
	{
		memset(&PWMVar[i],0,sizeof(PWMVar[i]));
		
		/* Setting of default values */
		PWMVar[i].pin = (uint8_t)0xFF;
		PWMVar[i].PylotType = (uint8_t)0xFF;		
	}	
}


/**
<PWM set load function it's the activator of the PWM loads> 

\param  [in]		pin ofset vs ModuleIOmap
\param  [out]		
\param  [in,out]	<pin , pilottype> 	 

\attention \b 

\return 	void

\retval		none		 	 

\warning			

\author   Giuseppe Boccolato 
\date		  31/05/11 
\version	1.0.0

\bug	

\todo		
   
\note	
\par 
\b Note 1:
			documentation by Giuseppe Boccolato
			\par \b Note n:	

*/

void PWMSetLoad (uint8_t reg, uint8_t config, uint8_t pin, const uint8_t *const pStruct)
{
uint8_t i;
uint8_t j;
    /* find the desired pin to set */
    for( i=0; i < (uint8_t)(CONFIG_PM_PWM_LOADSC_PWM_NUM); i++ )
    {    	
    	if(PWMVar[i].pin == pin)
    	{    		
            /* Dimmer LED function management */
            if( PWMVar[i].PylotType == DIM_LED_L )
            {
                /* config=0 -> Dimmer disabled, ON-OFF command */
                if (0 == config)
                {
                	PWMVar[i].PWMReg = ( ((reg & LOAD_REG_MASK) != 0) ? 1000 : 0 );
                }
                j= PWMVar[i].indexDimmer;	
               /* Acquisition of setting values */
                PWMDimmerVar[j].tr  = *pStruct;
                PWMDimmerVar[j].tf  = *(pStruct+1);
                PWMDimmerVar[j].D1  = *(uint16_t*)(pStruct+2);
                PWMDimmerVar[j].pt1 = *(uint16_t*)(pStruct+4); 
                PWMDimmerVar[j].D2  = *(uint16_t*)(pStruct+6);
                PWMDimmerVar[j].pt2 = *(uint16_t*)(pStruct+8);   
                /* notify ON or OFF status */
                PWMDimmerVar[j].OnOffStatus = ( ((reg & LOAD_REG_MASK) != 0) ? 1 : 0 );
            }
    	      	  
            /* PWM Profiler function management */
            
            if( PWMVar[i].PylotType == PWM_PROF_L )
            {
                /* Acquisition of setting value */
                
               if( (reg <=(uint16_t) *(pStruct)) && (reg >0) )
               { /*BG control if the regulation is valid*/
                  PWMVar[i].PWMReg = (((uint16_t) *(pStruct+reg))*10);
               } 
               else
               {
               PWMVar[i].PWMReg=0;
               }
               
                        
            }
    	  
            PWMVar[i].initDone = TRUE;

            return;    	  
    	}    	
    }  
}


/**
<The PWM control manager that must be called every 10 milliseconds> 

\param  [in]		pin ofset vs ModuleIOmap
\param  [out]		
\param  [in,out]	<pin , pilottype> 	 

\attention \b 

\return 	void

\retval		none		 	 

\warning			

\author   Giuseppe Boccolato 
\date		  31/05/11 
\version	1.0.0

\bug	

\todo		
   
\note	
\par 
\b Note 1:
			documentation by Giuseppe Boccolato
			\par \b Note n:	

*/

void PWMControlManager ( void )
{  
uint8_t i;
  
    for( i=0; i < (uint8_t)(CONFIG_PM_PWM_LOADSC_PWM_NUM); i++ )
    {
       uint8_t j = PWMVar[i].indexDimmer;
        /* Dimmer LED function management */
        if( (TRUE ==  PWMVar[i].initDone) && (PWMVar[i].PylotType == DIM_LED_L))
        {
            /*
            Step variation is achieved depending on V/L characteristic
            with the following 2 formulas, differentiating increasing
            from decreasing dimmer:
            increasing step = ( ((D2-D1)*1000) / ((pt2-pt1)*tr) )
            decreasing step = ( ((D2-D1)*1000) / ((pt2-pt1)*tf) )
            */
            
            
            if(PWMVar[i].PWMReg <= PWMDimmerVar[j].D1 )
            {
                PWMDimmerVar[j].step =(uint16_t)  (((uint32_t) PWMDimmerVar[j].D1 * 1000) / ((uint32_t) (PWMDimmerVar[j].pt1) * (PWMDimmerVar[j].tr)));
            }
            else if( PWMVar[i].PWMReg <= PWMDimmerVar[j].D2 )
            {
                PWMDimmerVar[j].step =(uint16_t) (((uint32_t)(PWMDimmerVar[j].D2- PWMDimmerVar[j].D1)* 1000) / ((uint32_t)(PWMDimmerVar[j].pt2-PWMDimmerVar[j].pt1) * PWMDimmerVar[j].tr));    
            }
            else
            {   
                PWMDimmerVar[j].step = (uint16_t) (((uint32_t)(1000 - PWMDimmerVar[j].D2)* 1000) / ( (uint32_t)(1000 - PWMDimmerVar[j].pt2) * PWMDimmerVar[j].tr));
            }

            /* Change step for decreasing dimmer */
            if( 0 == PWMDimmerVar[j].OnOffStatus)
            {
                PWMDimmerVar[j].step =(uint16_t) ( (uint32_t)PWMDimmerVar[j].step * (PWMDimmerVar[j].tr) / (uint32_t)(PWMDimmerVar[j].tf) );   
            }


            /* Dimmer regulation: dynamic calculation of Duty Cycle */
            if(PWMDimmerVar[j].OnOffStatus)    /* Increasing Dimmer */
            {     
              PWMVar[i].PWMReg += PWMDimmerVar[j].step;
              if( PWMVar[i].PWMReg > 1000)
              {
                 /* Saturation value */
                 PWMVar[i].PWMReg = 1000;
              }
            }
            else                         /* Decreasing Dimmer */
            {
                if(PWMVar[i].PWMReg >= PWMDimmerVar[j].step)
                {
                    PWMVar[i].PWMReg -= PWMDimmerVar[j].step;
                } 
                else
                {
                    /* Saturation value */
                    PWMVar[i].PWMReg = 0;
                }
            }

            /* Drive PWM pin with previous setting */
            SetDuty( PWMVar[i].Ch_Id, PWMVar[i].PWMReg );		
        }



        /* PWM Profiler function management */
        if( PWMVar[i].PylotType == PWM_PROF_L )
        {
            /* Drive PWM pin with setting file parameters */
            SetDuty( PWMVar[i].Ch_Id, PWMVar[i].PWMReg );		
        }
    }  
}


/**
<Sets duty cycle of the square wave fo the selected channel> 

\param  [in]		<channel_Id ,D>
\param  [out]		
\param  [in,out]	<pin , pilottype> 	 

\attention \b 

\return 	void

\retval		none		 	 

\warning			

\author   Giuseppe Boccolato 
\date		  31/05/11 
\version	1.0.0

\bug	

\todo		
   
\note	
\par 
\b Note 1:
			documentation by Giuseppe Boccolato
			\par \b Note n:	

*/

static void SetDuty ( uint8_t channel_Id, uint16_t D )
{
IO_TimingValueType tOn;
IO_TimingValueType period = 0;
uint8_t i;
bool_t ch_found;
  
    ch_found = FALSE;
    for ( i = 0; i < CONFIG_PM_PWM_LOADSC_PWM_NUM; i++ )
    {
        if ( channel_Id == ChTmrPWM_cfg[i].PWM_Id )
        {
            period = ChTmrPWM_cfg[i].dfltPeriod;
            ch_found = TRUE; 
        }    
    }    
  
    if ( TRUE == ch_found )
    {
        tOn = ((period * D)/1000);
        OCPWM_SetSync ( channel_Id , tOn );
    }
}

#endif /* defined(CONFIG_PM_PWM_LOADSCONTROL) */

/* *********** */
/* End of file */
/* *********** */

