/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_PulseCounter
\n
\file		PM_PulseCounterExp.h
\ingroup	PM_PulseCounter
\date		24/10/2013
\author		Salvatore Fimiani
\n
*/

#if  defined (CONFIG_PM_PULSECOUNTER)

#ifndef PM_PULSECOUNTEREXP_H_
#define PM_PULSECOUNTEREXP_H_



/**
 * \n	Input value for the PM_PulseCounter_Reset_Warning () function.
 * \n   With it you can reset the Low Frequency Warning
*/
#define PULSECOUNTER_RESET_LOW_WARNING  0

/**
 * \n	Input value for the PM_PulseCounter_Reset_Warning () function.
 * \n   With it you can reset the High Frequency Warning
*/
#define PULSECOUNTER_RESET_HIGH_WARNING 1

/**
 * \n	Input value for the PM_PulseCounter_Reset_Warning () function.
 * \n   With it you can reset both Low & High Frequency Warnings
*/
#define PULSECOUNTER_RESET_BOTH_WARNING 2





extern uint16_t PulseCount;





void PM_PulseCounter_SetCount( uint16_t newPulseCount );


uint16_t PM_PulseCounter_GetCount( void );

void PM_PulseCounter_Reset_Warning( uint8_t whichWarning);


uint8_t PM_PulseCounter_GetWarning( void );



#endif /* PM_PULSECOUNTEREXP_H_ */

#endif  /*defined (CONFIG_PM_PULSECOUNTER)*/
