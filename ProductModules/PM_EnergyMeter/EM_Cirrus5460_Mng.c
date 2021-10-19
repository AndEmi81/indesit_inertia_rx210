/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	EM Energy Meter
@{
\Par		Energy Meter
\n		This module implements the interface APIs to the Energy Meter.
\n    The Energy Meter is based on Cirrus5461 chip. The communication is made via \link SWSPI \endlink
\n    Available operations on this device are mainly Reset, Configure (default configuration not changeable), Start and Stop measure.
\n    Then you can read energy parameters like Power, V, I, Vrms, Irms, TotEnergy (Since start) both as raw data or in Watts, Volts, and Amperes.
\n    
\n    There are also APIs that allows to start and stop Energy consumption counters:
\n    With start you must provide also a time period and an energy thresholds.
\n    On these counters you can retrieve the value (energy in Wh consumed since start), the time elapsed (s since start) and the MAX energy consumed in a 10 seconds time slice.
\n    Moreover you have back some flags that tells if the energy counter reached the given threshold in the given time period.
\n    Other flags are the running state of the counter (started/stopped) and the running state of the timer (Time period elapsed or not).
@}
\ingroup PM
*/

/**
\n
\Par		Product Module Energy Meter
\n
\file		  ${EM_Cirrus5460_Mng.c}
\ingroup	EM
\date		  01.10.2009
\version	Platform2_00.00.12 (project baseline related to creation of this module)
\author		Andrea Mantovani
\n
\n
\Par\b	History:
\n 
\n 		00.00.12 
\n		01/10/2009 
\n		developer Andrea Mantovani
\n		changes First revision
\n 			
\n
\n
\n 		01.03.00 <baseline di progetto relativa alla versione precedente la corrente>
\n		gg/mm/aaaa <data in cui è stata realizzata >
\n		developer <progettista autore della modifica>
\n		changes <principali modifiche apportate>
\n 		- change 1
\n		- change 2
\n
\n
\n 
*/

/* ******** */
/* Includes */
/* ******** */


#include "StandardDefines.h"

#include "EM_Cirrus5460_Mng.h"
#include "Cirrus5460def.h"

#include "OCDIO_drv.h"
#include "SWSPI_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


#define	CIRRUS_CS_CLEAR       OCDIO_SetPinValueSync((uint8_t)SPISW_CS_IO_PIN, (uint8_t)LOW)
#define	CIRRUS_CS_SET         OCDIO_SetPinValueSync((uint8_t)SPISW_CS_IO_PIN, (uint8_t)HI)
#define	CIRRUS_RESET_CLEAR    OCDIO_SetPinValueSync((uint8_t)EM_RESET_IO_PIN, (uint8_t)LOW)
#define	CIRRUS_RESET_SET      OCDIO_SetPinValueSync((uint8_t)EM_RESET_IO_PIN, (uint8_t)HI)


/* 
  this define should be more general
  to do...
 */
#define nop()  _NOP_()


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */
static uint8_t CirrusTemp[4]; 

static uint32_t  aui32PavgSinceStart[2]={0,0}; /*64 bit accumulator: [0]=MSB [1]=LSB*/
static uint32_t  ui32TimeElapsedSinceStart=0; /*sec*/
static bool_t    bAccuEnabled=FALSE;
static uint16_t  ui16CirrusReadingCounter=0;

static stEnergyCounters_t astCounters[MAX_NUM_ENERGYCOUNTERS];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* local functions */
/* change to static?
  to do...
*/
void SwInit_SPIsw_CS5460(void);
void PowerMeter_HWRecover(void); 
void WriteReg_SPIsw_CS5460(uint8_t CirrusReg, const uint8_t *PValue);  
void ReadReg_SPIsw_CS5460(uint8_t CirrusReg, uint8_t *DataBuf); 
void SerialInit_SPIsw_CS5460(void);
void SwReset_SPIsw_CS5460(void); 
uint32_t Raw2WattHour(uint32_t ui32Input);

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported functions */

/*---------------------------------------------------

Cirrus Initialization Functions:

According to CIRRUS data sheet the serial port reset can be performed by any of the following actions.

1-  drive CS pin low (if it's already low perform low->high an then high->low transitions)
2-  Hw reset: drive reset low for at least 10 us.
3-  Issue serial port initialization sequence that is clocking 3 (or more) SYNCH1 command bytes followed by one SYNCH0 byte

if CS is driven low and high at each SPI access we can avoid implementing procedure 1-
---------------------------------------------------*/ 

/*----------------------
   HW reset function
----------------------*/ 
void PowerMeter_HWReset(void) 
{
  uint8_t ui8Count;   

  CIRRUS_RESET_CLEAR;
  
  /* how to perform 10us delay ?*/
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }
     
  CIRRUS_RESET_SET; 
  
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }
  
}

/*----------------------
   HW recover function
----------------------*/ 
void PowerMeter_HWRecover(void) 
{ 

  uint8_t ui8Count;

  CIRRUS_CS_CLEAR;
  
  
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }
  
  CIRRUS_RESET_CLEAR;
  
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }   
  
  CIRRUS_RESET_SET; 
  
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }
  
  /*SerialInit_SPIsw_CS5460();*/   /*	 Init CS5460 serial port */
  
  /*SwReset_SPIsw_CS5460();*/
  
  SwInit_SPIsw_CS5460();
  
  SerialInit_SPIsw_CS5460();   	/* Init CS5460 serial port */
  
  CIRRUS_CS_SET;
  
}



/*-------------------------------- 
 Reset and config Cirrus registers
 with default values
 ---------------------------------*/
 
void PowerMeter_InitConfig(void)
{
  uint8_t ui8Count;
       
  CIRRUS_RESET_CLEAR;
  
  /* how to implement 10us delay?*/
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  } 
  
  CIRRUS_RESET_SET;
  
  /* guard time */
  for(ui8Count=0;ui8Count<8;ui8Count++) 
  {
    nop();
  }
  
  /* SerialInit_SPIsw_CS5460();*/
  
  SwInit_SPIsw_CS5460();
  
  SerialInit_SPIsw_CS5460();   
  
  CIRRUS_CS_SET;  
 }  
 
 
 /*------------------------------ 
 Sends Start calibration  command
 --------------------------------*/  
 void PowerMeter_StartCal(void) 
 {
    /* which sequence to implement? */
 }


 /*------------------------------ 
 Sends Start measure command according
 to parameter passed.
 --------------------------------*/
 void PowerMeter_StartMeasure(eConvertion_t C_Conv,bool_t bResetCounters)
 {
   /*
     Reset energy accumulator
     Activate flag to enable accumulation
   */
   
   if(bResetCounters==TRUE) 
   {
     aui32PavgSinceStart[0]=0;
     aui32PavgSinceStart[1]=0;
     ui32TimeElapsedSinceStart=0;
   }
   
   bAccuEnabled=TRUE;
    
 
   if(C_Conv==SINGLE_CONV) 
   {
     CirrusTemp[0]= STARTSINGLECONV;
   }
   else 
   {
     CirrusTemp[0]= STARTCONTCONV; 
   }
    
   CIRRUS_CS_CLEAR;
     
   SWSPI_WriteDataSync((uint8_t)1, CirrusTemp);  

   CIRRUS_CS_SET;
  
 }

/*--------------------------------- 
 Reset flag to disable accumulation
 ----------------------------------*/  
void PowerMeter_StopMeasure(void) 
{
  bAccuEnabled=FALSE;
}

/*----------------------------------------------- 
 Returns the value of the instant I reg of Cirrus
 the value is in range -1 +1 with 2^(-23) resolution
 ------------------------------------------------*/ 
void PowerMeter_GetInstant_I(uint8_t *Ibin) 
{ 
  ReadReg_SPIsw_CS5460 ((uint8_t)I_Register, CirrusTemp );
  
  *(Ibin)=CirrusTemp[0];
  *(Ibin+1)=CirrusTemp[1];
  *(Ibin+2)=CirrusTemp[2];
}

/*----------------------------------------------- 
 Returns the value of the instant V reg of Cirrus
 the value is in range -1 +1 with 2^(-23) resolution
 ------------------------------------------------*/ 
void PowerMeter_GetInstant_V(uint8_t *Vbin)
{ 
  ReadReg_SPIsw_CS5460 ((uint8_t)V_Register, CirrusTemp );
  
  * (Vbin)=CirrusTemp[0];
  * (Vbin+1)=CirrusTemp[1];
  * (Vbin+2)=CirrusTemp[2];
}   


/*----------------------------------------------- 
 Returns the value of the instant P reg of Cirrus
 the value is in range -1 +1 with 2^(-23) resolution
 ------------------------------------------------*/ 
void PowerMeter_GetInstant_P(uint8_t *Pbin) 
{
  ReadReg_SPIsw_CS5460 ((uint8_t)P_Register, CirrusTemp );
  
  * (Pbin)=CirrusTemp[0];
  * (Pbin+1)=CirrusTemp[1];
  * (Pbin+2)=CirrusTemp[2];
}

/*----------------------------------------------
  Return the value of Pavg reg that is 
  the average power calculated on last N samples.
  ----------------------------------------------*/
void PowerMeter_GetAvgLastCycle_P(uint8_t *Pavgbin) 
{
  ReadReg_SPIsw_CS5460 ((uint8_t)E_Register, CirrusTemp );
  
  * (Pavgbin)=CirrusTemp[0];
  * (Pavgbin+1)=CirrusTemp[1];
  * (Pavgbin+2)=CirrusTemp[2];
  
}

/*------------------------------------------------------
  Return the value of Pavg reg converted in 100mW units.
  Pavg is the average power calculated on last N samples.   
  -----------------------------------------------------*/
void PowerMeter_GetAvgLastCycle_P_100mW(uint32_t *ui32Pavgbin) 
{
  uint32_t ui32Result=0;
  int16_t  i16Exp=0;

  /* read Pavg from Cirrus*/
  ReadReg_SPIsw_CS5460 ((uint8_t)E_Register, CirrusTemp );
  
  ui32Result  =	(uint32_t)CirrusTemp[2];
  ui32Result |=	(uint32_t)CirrusTemp[1] <<8;
  ui32Result |=	(uint32_t)CirrusTemp[0] <<16;
  
  /*
    must get absolute value in case of negative ?
    to be verified...
  */
  if((ui32Result & 0x00800000UL)!=0UL) 
  {
    ui32Result = ~ui32Result;
    /* the previous operation inverted also MSByte that must be 0 instead */
    ui32Result &= 0x00FFFFFFUL;    
    ui32Result +=1UL;    
  }
  
  /*
    Pavg in in range -1 +1
    to keep 16Msb you have to divide by 2^7
  */
  ui32Result >>= 7;
  
  /* ( 16 bit X 16 bit resolution -> result fits in 32 bits) */
  ui32Result *= (uint32_t)DefaultKPP_ma; 
  
  /* apply exponent */  
  /*
    to avoid overflow in shift left calculate net shift that is:
    DefaultKPP_exp + 16
    note: 16 because 2^(-16) is the value of LSB 
  */
  i16Exp=DefaultKPP_exp + 16;
  
  if(i16Exp & 0x80)
	{
	  ui32Result <<= (-i16Exp);
	} else 
	{
	  ui32Result >>= i16Exp;
	}
	
	*ui32Pavgbin= ui32Result;
}


/*----------------------------------------------- 
 Returns the value of the I rms reg of Cirrus
 the value is in range 0 +1 with 2^(-24) resolution and is 
 calculated on last N samples
 ------------------------------------------------*/ 
void PowerMeter_Get_I_RMS(uint8_t *IrmsBin) 
{
  ReadReg_SPIsw_CS5460 ((uint8_t)IRMS_Register, CirrusTemp);  
  
  /*
    Portable but not optimized version  
    *Irms= CirrusTemp[2]| (CirrusTemp[1]<<8) | (CirrusTemp[0]<<16);       
  */

  * (IrmsBin)=CirrusTemp[0];
  * (IrmsBin+1)=CirrusTemp[1];
  * (IrmsBin+2)=CirrusTemp[2]; 
}

/*----------------------------------------------- 
 Returns the value of the I rms reg of Cirrus converted in 100mA units.
 The value is in range 0 +1 with 2^(-24) resolution and is 
 calculated on last N samples
 ------------------------------------------------*/ 
void PowerMeter_Get_I_RMS_100mA(uint32_t *ui32Irms) 
{
  uint32_t ui32Result=0;
  ReadReg_SPIsw_CS5460 ((uint8_t)IRMS_Register, CirrusTemp);  
  
  /*
    Portable but not optimized version  
 	  *Vrms= CirrusTemp[2]| (CirrusTemp[1]<<8) | (CirrusTemp[0]<<16);
  */
  
  ui32Result  =	(uint32_t)CirrusTemp[2];
  ui32Result |=	(uint32_t)CirrusTemp[1] <<8;
  ui32Result |=	(uint32_t)CirrusTemp[0] <<16;  

  /* 
     Irms is in range 0.0--1.0
     to keep 16Msb you have to divide by 2^8
  */  
  ui32Result >>= 8;
  
  /* ( 16 bit X 16 bit resolution -> result fits in 32 bits) */
  ui32Result *= (uint32_t)DefaultKII_ma; 
  
  /* apply exponent */  
  if(DefaultKII_exp <0)
	{
	  ui32Result <<= (-DefaultKII_exp);
	} else 
	{
	  ui32Result >>= DefaultKII_exp;
	}
	
	/* 2^(-16) is the value of LSB */
	ui32Result >>= 16;
	
	*ui32Irms= ui32Result;

}


/*----------------------------------------------- 
 Returns the value of the V rms reg of Cirrus
 the value is in range 0 +1 with 2^(-24) resolution and is 
 calculated on last N samples
 ------------------------------------------------*/ 
void PowerMeter_Get_V_RMS(uint8_t *VrmsBin) 
{
   ReadReg_SPIsw_CS5460 ((uint8_t)VRMS_Register, CirrusTemp );
  
  /*
    Portable but not optimized version  
 	  *Vrms= CirrusTemp[2]| (CirrusTemp[1]<<8) | (CirrusTemp[0]<<16);
  */

   	* (VrmsBin)=CirrusTemp[0];
   	* (VrmsBin+1)=CirrusTemp[1];
  	* (VrmsBin+2)=CirrusTemp[2];

}

/*----------------------------------------------- 
 Returns the value of the I rms reg of Cirrus converted in 100mV units.
 The value is in range 0 +1 with 2^(-24) resolution and is 
 calculated on last N samples.
 ------------------------------------------------*/ 
void PowerMeter_Get_V_RMS_100mV(uint32_t *ui32Vrms) 
{
   uint32_t ui32Result=0;
   ReadReg_SPIsw_CS5460 ((uint8_t)VRMS_Register, CirrusTemp );
  
  /*
    Portable but not optimized version  
 	  *Vrms= CirrusTemp[2]| (CirrusTemp[1]<<8) | (CirrusTemp[0]<<16);
  */
  
  ui32Result  =	(uint32_t)CirrusTemp[2];
  ui32Result |=	(uint32_t)CirrusTemp[1] <<8;
  ui32Result |=	(uint32_t)CirrusTemp[0] <<16;  

  /* 
     Vrms is in range 0.0--1.0
     to keep 16Msb you have to divide by 2^8
  */  
  ui32Result >>= 8;
  
  /* ( 16 bit X 16 bit resolution) */
  ui32Result *= (uint32_t)DefaultKVV_ma; 
  
  /* apply exponent */  
  if(DefaultKVV_exp <0)
	{
	  ui32Result <<= (-DefaultKVV_exp);
	} else 
	{
	  ui32Result >>= DefaultKVV_exp;
	}
	
	/* 2^(-16) is the value of LSB */
	ui32Result >>= 16;
	
	*ui32Vrms= ui32Result;

}

/*--------------------------------------------------------------------------
Returns the value of energy in Wh since the last start measure.
----------------------------------------------------------------------------*/
void PowerMeter_GetAvgSinceStart_E_Wh(uint8_t *EWh,uint32_t *pui32TimeElapsed)
{
  uint32_t ui32PavgAccLo;
  uint32_t ui32PavgAccHi;
  uint32_t aui32PavgAcc[2]={0,0};
  int16_t  i16Exp=0;
  
  *pui32TimeElapsed= ui32TimeElapsedSinceStart;
  
  /* 
     bits [0-15] discarded
     Multiply bits [16-31] by KEE 
  */
  ui32PavgAccLo = Raw2WattHour(aui32PavgSinceStart[1]);  
  /*
  ui32PavgAccLo  =((aui32PavgSinceStart[1]>>16)&0x0000FFFFUL);

  ui32PavgAccLo *= (uint32_t)DefaultKEE_ma; 
  
  // apply exponent 
  if(DefaultKEE_exp <0)
	{
	  ui32PavgAccLo <<= (-DefaultKEE_exp);
	} else 
	{
	  ui32PavgAccLo >>= DefaultKEE_exp;
	} 
	
  // 2^(-7) is the value of LSB 
	ui32PavgAccLo >>= 7;
	*/
	
 /*
  You could also keep bits [32-47]
 */	
 // ui32PavgAccHi = Raw2WattHour(aui32PavgSinceStart[0]);
 
   
  ui32PavgAccHi  =(aui32PavgSinceStart[0]&0x0000FFFFUL);
  
  ui32PavgAccHi *= (uint32_t)DefaultKEE_ma; 
  
  // apply exponent
  /*
    to avoid overflow in shift left calculate net shift that is:
    DefaultKPP_exp + 1
    note: +1 because 2^(+1) is the value of LSB 
  */
  i16Exp=DefaultKEE_exp - 1;
  
  if(i16Exp & 0x80)
	{
	  ui32PavgAccHi <<= (-i16Exp);
	} else 
	{
	  ui32PavgAccHi >>= i16Exp;
	}
   
  
  /*
    Sum 32+32 to get 64bits
  */
  aui32PavgAcc[1]= ui32PavgAccHi<<16;
  if((0xFFFFFFFFUL-aui32PavgAcc[1])<ui32PavgAccLo)
  {
    /* overflow condition add carry */
    aui32PavgAcc[0]++;
  }
  aui32PavgAcc[1] += ui32PavgAccLo;
      
  
  *(EWh)  =(uint8_t) (aui32PavgAcc[0]&0x000000FFUL);       /*MSB*/  
	*(EWh+1)=(uint8_t)((aui32PavgAcc[1]&0xFF000000UL)>>24);
 	*(EWh+2)=(uint8_t)((aui32PavgAcc[1]&0x00FF0000UL)>>16);
	*(EWh+3)=(uint8_t)((aui32PavgAcc[1]&0x0000FF00UL)>>8);
	*(EWh+4)=(uint8_t) (aui32PavgAcc[1]&0x000000FFUL);       /*LSB*/
}


/*----------------------------------------------------------------------
  If the accumulation flag is enabled poll if a new Pavg is available.
  In that case accumulate the new val.
  If this function is called fast enough, a new value is read each second
  ---------------------------------------------------------------------*/                                                                   
void PowerMeter_Accum(void) 
{
  uint8_t  statusRegTmp[3];
  uint32_t ui32Result=0;
  uint8_t  ui8Index;
   
  if(TRUE==bAccuEnabled) 
  {
    /*  read Status_Register*/
    ReadReg_SPIsw_CS5460 ((uint8_t)Status_Register, statusRegTmp );
    if(   (statusRegTmp[0] & DRDY)!=0) 
    {
      ui16CirrusReadingCounter=0;
  
      
  #ifdef AIM_BOARD
  #ifdef AIM_debug
     /* Only for debug 
       led orange OFF
      */
      PTAD_PTAD1=0;
        
      /* Only for debug 
         led yellow ON
      */
      PTAD_PTAD2=1;
  #endif
  #endif
      
      
      ReadReg_SPIsw_CS5460 ((uint8_t)E_Register, CirrusTemp );

      /*one second has elapsed after last read*/
      ui32TimeElapsedSinceStart++;
            
      ui32Result  =	(uint32_t)CirrusTemp[2];
      ui32Result |=	(uint32_t)CirrusTemp[1] <<8;
      ui32Result |=	(uint32_t)CirrusTemp[0] <<16;
      
      /*
        must get absolute value in case of negative ?
        to be verified...
      */
      if((ui32Result & 0x00800000UL)!=0UL) 
      {
        ui32Result = ~ui32Result;
        /* the previous operation inverted also MSByte that must be 0 instead */
        ui32Result &= 0x00FFFFFFUL;    
        ui32Result +=1UL;    
      }  
      
      /*
       execute accumulation over 64 bit
      */
      if((0xFFFFFFFFUL-aui32PavgSinceStart[1])<ui32Result)
      {
        /* overflow condition add carry */
        aui32PavgSinceStart[0]++;
      }
      aui32PavgSinceStart[1] += ui32Result;
      
      
      /* Accumulate partial energy counters 
         Energy Monitor
      */
      
      for(ui8Index=0;ui8Index<MAX_NUM_ENERGYCOUNTERS;ui8Index++) 
      {
        /* if counter enabled*/
        if((astCounters[ui8Index].ui8Flags & EM_COUNT_STAT_MASK) !=0) 
        {
          /* accumulate energy and time */
          astCounters[ui8Index].ui32RawAccum += ui32Result;
          astCounters[ui8Index].ui32Raw10secAcc += ui32Result;
          astCounters[ui8Index].ui16TimeElapsed++;
          
          if(((astCounters[ui8Index].ui16TimeElapsed) % 10)==0) 
          {
             /*
               save the max in 10 secs as raw
             */
             if(astCounters[ui8Index].ui32Raw10secAcc > astCounters[ui8Index].ui32RawMaxEnergy10sec)
             {
               astCounters[ui8Index].ui32RawMaxEnergy10sec=astCounters[ui8Index].ui32Raw10secAcc;
             }
             astCounters[ui8Index].ui32Raw10secAcc=0;
          }
          
          /* if time period not yet elapsed but current time > time period ... */
          if ( (astCounters[ui8Index].ui16TimeElapsed>=astCounters[ui8Index].ui16TimePeriod) &&
                ((astCounters[ui8Index].ui8Flags & EM_T_ELAPSED_MASK) ==0) )
          {
            /* detect transition */ 
            astCounters[ui8Index].ui8Flags |= EM_T_ELAPSED_MASK;
            
            /* calculate energy consumed so far in Wh...*/
            astCounters[ui8Index].ui16EnergyCons =(uint16_t) Raw2WattHour(astCounters[ui8Index].ui32RawAccum);
            /*... if it is grater then energy threshold */
            if(astCounters[ui8Index].ui16EnergyCons > astCounters[ui8Index].ui16EnergyTreshold) 
            {
              /* set the overrun flag*/
              astCounters[ui8Index].ui8Flags |= EM_E_OVERRUN_MASK;
            }
          }
        }
      }
      
      /*
       Clear DRDY bit
       Writing 1 it clears that bit!
      */
      statusRegTmp[0]=0;
      statusRegTmp[1]=0;
      statusRegTmp[2]=0;
      statusRegTmp[0] |= DRDY;
      WriteReg_SPIsw_CS5460((uint8_t)Status_Register, statusRegTmp);
      
      #ifdef AIM_BOARD
      #ifdef AIM_debug
      
      /* Only for debug 
         led yellow OFF
      */
      PTAD_PTAD2=0;
      
      #endif
      #endif
    }
    else 
    {
      ReadReg_SPIsw_CS5460 ((uint8_t)Cycle_Count_Register, statusRegTmp );
      if (
           (statusRegTmp[0] != (uint8_t)((DefaultCirrusCycle&0x00FF0000UL)>>16UL))  ||        
           (statusRegTmp[1] != (uint8_t)((DefaultCirrusCycle&0x0000FF00UL)>>8UL))  ||
           (statusRegTmp[2] != (uint8_t)((DefaultCirrusCycle&0x000000FFUL)))
      ) 
      {
            
        /* check if Cirrus is stucked */
  #ifdef AIM_BOARD     
  #ifdef AIM_debug
       
        /* Only for debug 
         led orange ON
        */
        PTAD_PTAD1=1;
        
  #endif
  #endif
        
        PowerMeter_HWRecover();
        PowerMeter_StartMeasure(CONTINUOUS_CONV,FALSE);
   
      } else 
      {
        /* update counter for timeout*/
        ui16CirrusReadingCounter++;
        if(ui16CirrusReadingCounter>1500) 
        {
                  /* check if Cirrus is stucked */
  #ifdef AIM_BOARD     
  #ifdef AIM_debug
       
        /* Only for debug 
         led orange ON
        */
        PTAD_PTAD1=1;
        
  #endif
  #endif
        
        PowerMeter_HWRecover();
        PowerMeter_StartMeasure(CONTINUOUS_CONV,FALSE);
        ui16CirrusReadingCounter=0;
        }
      }
    }
  }
}
    

/*--------------------------------
  Starts the given Energy counter
  --------------------------------*/                                                                   

uint8_t PowerMeter_StartEnergyCount(uint8_t ui8CounterId,stEnergyCounters_t* pstCounterData) 
{
  if((ui8CounterId<MAX_NUM_ENERGYCOUNTERS)&& (pstCounterData != NULL))
  {
    astCounters[ui8CounterId]=(*pstCounterData);
    astCounters[ui8CounterId].ui8Flags=0;
    astCounters[ui8CounterId].ui8Flags |= EM_COUNT_STAT_MASK;
    astCounters[ui8CounterId].ui16EnergyCons=0;
    astCounters[ui8CounterId].ui16TimeElapsed=0;
    astCounters[ui8CounterId].ui16MaxEnergy10sec=0;
    astCounters[ui8CounterId].ui32RawAccum=0;
    astCounters[ui8CounterId].ui32Raw10secAcc=0;
    astCounters[ui8CounterId].ui32RawMaxEnergy10sec=0;
    
    return 0;
  } 
  else
  {
    return 1;
  }
}


/*-----------------------------------------------------------------
  Returns energy consumption data about the given Energy counter id
  -----------------------------------------------------------------*/
uint8_t PowerMeter_GetEnergyCount(uint8_t ui8CounterId,stEnergyCounters_t* pstCounterData) 
{
  uint32_t ui32Tmp;
  
  if((ui8CounterId<MAX_NUM_ENERGYCOUNTERS)&& (pstCounterData != NULL))
  {
    /* calculate Energy in Wh */
    
    ui32Tmp = Raw2WattHour(astCounters[ui8CounterId].ui32RawAccum);     
    astCounters[ui8CounterId].ui16EnergyCons=ui32Tmp & 0x0000FFFF;
    
    ui32Tmp = Raw2WattHour(astCounters[ui8CounterId].ui32RawMaxEnergy10sec);
    astCounters[ui8CounterId].ui16MaxEnergy10sec=ui32Tmp & 0x0000FFFF;;    
    
    pstCounterData->ui8Flags=astCounters[ui8CounterId].ui8Flags;
    pstCounterData->ui16EnergyCons=astCounters[ui8CounterId].ui16EnergyCons;
    pstCounterData->ui16TimeElapsed=astCounters[ui8CounterId].ui16TimeElapsed;
    pstCounterData->ui16MaxEnergy10sec=astCounters[ui8CounterId].ui16MaxEnergy10sec;
    return 0;
  }
  else
  {
    return 1;
  }
}


/*--------------------------------
  Stops the given Energy counter
  --------------------------------*/
uint8_t PowerMeter_StopEnergyCount(uint8_t ui8CounterId) 
{
  if(ui8CounterId<MAX_NUM_ENERGYCOUNTERS)
  {
    astCounters[ui8CounterId].ui8Flags &= ~EM_COUNT_STAT_MASK;
    return 0;
  }
  else 
  {
    return 1;
  }
  
}


/*----------------- Local Functions ----------------*/

uint32_t Raw2WattHour(uint32_t ui32Input)
{
  uint32_t ui32Tmp;
   /* 
       bits [0-15] discarded
       Multiply bits [16-31] by KEE 
    */  
    ui32Tmp  =((ui32Input>>16)&0x0000FFFFUL);

    ui32Tmp *= (uint32_t)DefaultKEE_ma; 
    
    /* apply exponent */  
    if(DefaultKEE_exp <0)
  	{
  	  ui32Tmp <<= (-DefaultKEE_exp);
  	} else 
  	{
  	  ui32Tmp >>= DefaultKEE_exp;
  	} 
  	
    /* 2^(-7) is the value of LSB */
  	ui32Tmp >>= 7;
  	
  	return ui32Tmp;
}

/*----------------------
   Serial init sequence
  -----------------------*/
void SerialInit_SPIsw_CS5460(void)
{     
        
  CirrusTemp[0]=SYNC1;
  CirrusTemp[1]=SYNC1;
  CirrusTemp[2]=SYNC1;
  CirrusTemp[3]=SYNC0;  
                           
  CIRRUS_CS_CLEAR;
           
  SWSPI_WriteDataSync((uint8_t)4, CirrusTemp); 
  
  CIRRUS_CS_SET;      
}                               


/*-------------------------- 
  SW reset 
  --------------------------*/
void SwReset_SPIsw_CS5460(void)
{		

  CirrusTemp[0] = SWRESET;
  
  CIRRUS_CS_CLEAR;
     
  SWSPI_WriteDataSync((uint8_t)1, CirrusTemp);
  
  CIRRUS_CS_SET;  
}
   
 

/*-------------------------- 
  Write to a Cirrus register 
  --------------------------*/    

/* PValue is a pointer to a 3 bytes char array */

void WriteReg_SPIsw_CS5460(uint8_t CirrusReg, const uint8_t *PValue)
{

	uint8_t PacketTemp[4]; 
                 		
  PacketTemp[0]= (uint8_t)((CirrusReg << 1) | WRITE_REGISTER);    
  PacketTemp[1]=  *PValue; 
	PacketTemp[2]=  *(PValue+1); 
	PacketTemp[3]=  *(PValue+2);
	 	
	 
  CIRRUS_CS_CLEAR;
            
  SWSPI_WriteDataSync((uint8_t)4, PacketTemp);      
  
  CIRRUS_CS_SET;
 }                 
 
 
/*-------------------------- 
  Read a Cirrus register 
  --------------------------*/
void ReadReg_SPIsw_CS5460(uint8_t CirrusReg, uint8_t *DataBuf)
{
  uint8_t DataOut[3]={SYNC0,SYNC0,SYNC0};
  
  CirrusTemp[0] = ((CirrusReg << 1) & READ_REGISTER);
  
  CIRRUS_CS_CLEAR;
     
  SWSPI_WriteDataSync((uint8_t)1, CirrusTemp);
  
  SWSPI_ReadDataSync((uint8_t)3, DataBuf,DataOut);
  
  CIRRUS_CS_SET;
   
 }
        
/*---------------------------------
  Cirrus Software reset 
  and write a constant configuration 
  ---------------------------------*/

void SwInit_SPIsw_CS5460(void)
{
   
  SwReset_SPIsw_CS5460();

  CirrusTemp[0]= (DefaultConfigRegister&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultConfigRegister&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultConfigRegister&0x0000FFUL);    
  
  WriteReg_SPIsw_CS5460((uint8_t)Config_Register, CirrusTemp);

  
  CirrusTemp[0]= (DefaultIoffset&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultIoffset&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultIoffset&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460(Offset_Register_I, CirrusTemp); 


  CirrusTemp[0]= (DefaultIgain&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultIgain&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultIgain&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460(Gain_Register_I, CirrusTemp);
  
 
  CirrusTemp[0]= (DefaultVoffset&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultVoffset&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultVoffset&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460(Offset_Register_V, CirrusTemp);

  
  CirrusTemp[0]= (DefaultVgain&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultVgain&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultVgain&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460(Gain_Register_V, CirrusTemp); 
  
 
  CirrusTemp[0]= (DefaultCirrusCycle&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultCirrusCycle&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultCirrusCycle&0x0000FFUL); 
  
  WriteReg_SPIsw_CS5460(Cycle_Count_Register, CirrusTemp);
    
  
  CirrusTemp[0]= (DefaultPrate&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultPrate&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultPrate&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460(Prate_Register, CirrusTemp);
  
  
  CirrusTemp[0]= (DefaultTBC&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (DefaultTBC&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (DefaultTBC&0x0000FFUL); 
  
  WriteReg_SPIsw_CS5460(TBC_Register, CirrusTemp);
  
  
  CirrusTemp[0]= (Cirrus_Status&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (Cirrus_Status&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (Cirrus_Status&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460((uint8_t)Status_Register, CirrusTemp);
  
  CirrusTemp[0]= (Cirrus_Mask&0xFF0000UL)>>16UL;
  CirrusTemp[1]= (Cirrus_Mask&0x00FF00UL)>>8UL;
  CirrusTemp[2]= (Cirrus_Mask&0x0000FFUL);
  
  WriteReg_SPIsw_CS5460((uint8_t)Mask_Register, CirrusTemp);              
  
 } 
        

/* *********** */
/* End of file */
/* *********** */


																	  







