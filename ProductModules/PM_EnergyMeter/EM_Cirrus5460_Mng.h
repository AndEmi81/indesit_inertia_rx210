/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		Energy Meter
\n			Product Module Energy Manager based on CIRRUS5460 Header file
\n
\file		  ${EM_Cirrus5460_Mng.h}
\ingroup	EM
\date		  01/10/2009
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

#ifndef __EM_CIRRUS5460_MNG_H
#define __EM_CIRRUS5460_MNG_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define MAX_NUM_ENERGYCOUNTERS   2

#define EM_COUNT_STAT_MASK  0x1
#define EM_E_OVERRUN_MASK   0x2
#define EM_T_ELAPSED_MASK   0x8

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef enum 
{ 
  SINGLE_CONV,
  CONTINUOUS_CONV
}
eConvertion_t;

typedef struct 
{
 uint8_t   ui8Flags;
 uint16_t  ui16EnergyCons;
 uint16_t  ui16TimeElapsed;
 uint16_t  ui16MaxEnergy10sec;
 uint16_t  ui16EnergyTreshold;
 uint16_t  ui16TimePeriod;
 uint32_t  ui32RawAccum;
 uint32_t  ui32Raw10secAcc;
 uint32_t  ui32RawMaxEnergy10sec;
}
stEnergyCounters_t;

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/**
This function drives the HW reset pin of the power meter.
It performs a high->low->high transitions sequence.

\param  [in]		  void
\param  [out]		  void
\param  [in,out]	none

\attention \b 

\return 	void

\retval		[none]		 	 

\warning	[the duration of low state is made via "nop" and therefore is uP speed dependent]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_HWReset(void); 


/**

Performs Energy meter register configuration
Values to be written are defined in file Cirrus5460def.h

\param  [in]		  void
\param  [out]		  void
\param  [in,out]	none

\attention \b 

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_InitConfig(void); 


/**
  This function starts the automatic calibration procedure.
  This is a feature of Cirrus 5461 chip

\param  [in]		  void
\param  [out]		  void
\param  [in,out]	none

\attention \b 

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[This function is empty.To be verified if it's really necessary]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_StartCal(void);

 

/**
Gives to Energy Meter (EM) the command to start mesuring power.
It's possible to start either a one-shot or a continuous mesuring mode of operation.
In One-shot mode the EM collects N samples of I,V and P and calculates one value for Irms, Vrms and Pavg and then stops.
In continuous mode the EM simply keeps on performing one measure after one. The new measure ovverides the old values. I't up to the user not to lose measures.
For example, with a 4,096Mhz external clock and an internal divider K=1 and N=4000(default) we'll get a new measure each second.

This function reset the SW accumulator of energy and the timer counter of the time elaspeset sunce start measure (These are global vars) if
bResetCounters parameter is TRUE.

It sets the accumulator enable flag (global var).

\param  [in]		  <C_Conv>         [one shot or continuous conversion]
\param  [in]      <bResetCounters> [tells whether or not to reset energy accumulator and time elapsed counters]
\param  [out]		  void
\param  [in,out]	none

\attention \b aui32PavgSinceStart is the global var used to accumulate energy
           \b ui32TimeElapsedSinceStart is the global var used to count seconds elapsed since start measure.
           \b bAccuEnabled is the accumulation enable flag.
           

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_StartMeasure(eConvertion_t C_Conv,bool_t bResetCounters);



/**
Disables the accumulation enable flag (bAccuEnabled)

\param  [in]		  void
\param  [out]		  void
\param  [in,out]	none

\attention \b bAccuEnabled is the accumulation enable flag.

\return 	void

\retval		[void]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			

*/
void PowerMeter_StopMeasure(void) ;



/**
 Returns the value of the instant current (I)
 The value is in range -1 +1 (2 complement) with 2^(-23) resolution (24 bits)
 The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.

\param  [in]		  void
\param  [out]		  none
\param  [in,out]	Ibin [uint8_t pointer that is an array of 3 chars(24bits), Ibin[0] is the MSByte]

\attention \b 

\return 	void

\retval		[void]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			

*/
void PowerMeter_GetInstant_I(uint8_t *Ibin);



/**
 Returns the value of the instant voltage (V)
 the value is in range -1 +1 (2 complement) with 2^(-23) resolution (24 bits)
 The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.

\param  [in]		  void
\param  [out]		  none
\param  [in,out]	Vbin [uint8_t pointer that is an array of 3 chars(24bits), Vbin[0] is the MSByte]

\attention \b 

\return 	void

\retval		[void]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_GetInstant_V(uint8_t *Vbin);



/**
 Returns the value of the instant power (P)
 The value is in range -1 +1 (2 complement) with 2^(-23) resolution (24 bits)
 The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.

\param  [in]		  void
\param  [out]		  none
\param  [in,out]	Pbin [uint8_t pointer that is an array of 3 chars(24bits), Pbin[0] is the MSByte]

\attention \b 

\return 	void

\retval		[void]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_GetInstant_P(uint8_t *Pbin);



/**
Return the value of Pavg calculated on last N samples.
The value is in range -1 +1 (2 complement) with 2^(-23) resolution (24 bits).
The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.  


\param  [in]		void
\param  [out]		none
\param  [in,out]	Pavgbin [uint8_t pointer that is an array of 3 chars(24bits), Pavgbin[0] is the MSByte]	 

\attention \b 

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_GetAvgLastCycle_P(uint8_t *Pavgbin);




/**
Returns the value of Pavg converted in 100mW units.
Pavg is the average power calculated on last N samples.

\param  [in]		void
\param  [out]		none
\param  [in,out]	uint32_t *ui32Pavg [Pavg in 100mW units]	 

\attention \b Kpp is the coefficent to obtain the Power in 100mW units it is calculated from board schematic and is a #define

\return 	none

\retval		[void]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_GetAvgLastCycle_P_100mW(uint32_t *ui32Pavg);




/**
Returns the value of the I rms calculated on last N samples.
The value is in range 0 +1 with 2^(-24) resolution (24bits).
The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.

\param  [in]		void
\param  [out]		none
\param  [in,out]	uint8_t *Irmsbin [uint8_t pointer that is an array of 3 chars(24bits), Irmsbin[0] is the MSByte]	 

\attention \b

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			

*/
void PowerMeter_Get_I_RMS(uint8_t *Irmsbin);



/**
Returns the value of I rms converted in 100mA units.
Irms is the average power calculated on last N samples.

\param  [in]		void
\param  [out]		none
\param  [in,out]	uint32_t *ui32Irms [I rms in 100mA units]	 

\attention \b Kii is the coefficent to obtain the current in 100mA units it is calculated from board schematic and is a #define
           
           
\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}


*/
void PowerMeter_Get_I_RMS_100mA(uint32_t *ui32Irms);



/**
Returns the value of the V rms calculated on last N samples.
The value is in range 0 +1 with 2^(-24) resolution (24bits)
The value is not absolute but is a percentage of full scale tha can be measure by energy meter chip.


\param  [in]		  void
\param  [out]		  none
\param  [in,out]	uint8_t *Vrmsbin [uint8_t pointer that is an array of 3 chars(24bits), Vrmsbin[0] is the MSByte]	 

\attention \b 

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
void PowerMeter_Get_V_RMS(uint8_t *Vrmsbin);




/**
Returns the value of V rms converted in 100mV units.
Vrms is the average power calculated on last N samples.

\param  [in]		 void
\param  [out]		 none
\param  [in,out] uint32_t *ui32Vrms [uint8_t pointer that is an array of 3 chars(24bits), ui32Vrms[0] is the MSByte]	 

\attention \b Kvv is the coefficent to obtain the voltage in 100mV units it is calculated from board schematic and is a #define

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_Get_V_RMS_100mV(uint32_t *ui32Vrms);



/**
Returns the value of energy in Wh (40 bits) and 
the time elapsed in secs (32 bits) since the last start measure.

\param  [in]		  void
\param  [out]		  none
\param  [in,out]	uint8_t  *EWh              [uint8_t pointer that is an array of 5 chars(40bits) that is the energy used in Wh Ewh[0]=MSByte]
                  uint32_t *pui32TimeElapsed [Time elapsed in secs since start measure]	 

\attention \b Kee is the coefficent used to calculate energy in Wh it is calculated from Kpp and is put as a #define in an header file.

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date	  	19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_GetAvgSinceStart_E_Wh(uint8_t *EWh, uint32_t *pui32TimeElapsed);



/**
This function must be called in scheduler at a frequency higher than energy update.
It reads the Pavg and accumulates it in 64bits.

\param  [in]		  void
\param  [out]		  none
\param  [in,out]	none

\attention \b  It accumulates energy in a global variable if the accumulation flasg (global) is enabled

\return 	void

\retval		[none]		 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
void PowerMeter_Accum(void);


/**

This function starts couting Energy consumption on the given counter index.

\param  [in]		  ui8CounterId - which counter to be started
\param  [in]      pstCounterData - pointer to the data structure that contains all counter informations. You provide
                                   the Time and Energy thresholds.
\param  [out]		  none
\param  [in,out]	none

\attention \par \b  The input struct is copied in a static one inside the module.

\return 	uint8_t - error code

\retval		[none]	0 = OK
                  1 = Invalid parameters : CounterId out of range , Null pointer	 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  05-11-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
uint8_t PowerMeter_StartEnergyCount(uint8_t ui8CounterId,stEnergyCounters_t* pstCounterData);   


/**

This function return information on the given counter

\param  [in]		  ui8CounterId - which counter to be started
\param  [out]		  pstCounterData - pointer to the data structure that contains all counter informations. 
                                   It gives back the energy conter value (Wh), the time elapsed (s) since start and the MAX energy consumed in a 10 seconds time slice.
                                   It checks if the energy counter reaches the given threshold in a given time period and the check result is givewn back through the flag field.
                                   Other provided flags are the running state of the counter (started/stopped) and the running state of the timer (Time period elapsed or not).
\param  [in,out]	none

\attention 

\return 	uint8_t - error code

\retval		[none]	0 = OK
                  1 = Invalid parameters: CounterId out of range , Null pointer	 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  05-11-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
uint8_t PowerMeter_GetEnergyCount(uint8_t ui8CounterId,stEnergyCounters_t* pstCounterData);   


/**

This function stops couting Energy consumption on the given counter index.

\param  [in]		  ui8CounterId - which counter to be stopped
\param  [out]		  none
\param  [in,out]	none

\attention \b  

\return 	uint8_t - error code

\retval		[none]	0 = OK
                  1 = Invalid parameters : CounterId out of range 	 

\warning	[none]		

\author 	Andrea Mantovani
\date		  05-11-2009
\version	1.0.0

\bug		[none]

\todo		[none]
   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}
			
*/
uint8_t PowerMeter_StopEnergyCount(uint8_t ui8CounterId);






#endif /* __EM_CIRRUS5460_MNG_H */

/* *********** */
/* End of file */
/* *********** */
