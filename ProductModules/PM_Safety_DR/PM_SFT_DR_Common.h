/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_DR
\n
\n
\file		PM_SFT_DR_Common.h
\ingroup	PM_Safety_DR
\date		05/06/2013
\author		Alessandro Sabbatini
*/
#ifndef PM_SFT_DR_COMMON_H_
#define PM_SFT_DR_COMMON_H_

/*\addtogroup PM_Safety_WM
@{*/

#define GEN_FLT_RESET_TIMER_VALUE	50	/* Set to 5 sec */
#define BASETIME_1S_TO_100MS(counter)	(counter*10)  	/**/
#define BASETIME_10S_TO_100MS(counter)	(counter*100)	/**/
#define BASETIME_1MIN_TO_100MS(counter)	(counter*600)	/**/

#define PREF_TIME_MASK		0x7FFF
#define FLT_TIME_MASK 		0x00FF
#define FLT_OPCODE_MASK 	0xF800

#define FLT_OPCODE_1SEC		0x1000
#define FLT_OPCODE_10SEC	0x1800
#define FLT_OPCODE_1MIN		0x2000


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

void PM_SFT_DR_Init(void);
uint16_t PM_SFT_DR_GetTimeByCntrValue(uint16_t fltCounterVal);
/*@}*/


#endif	/* PM_SFT_DR_COMMON_H_ */
