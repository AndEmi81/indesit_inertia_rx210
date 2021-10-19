/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_CK
\n
\n
\file		PM_SFT_CK_Common.h
\ingroup	PM_Safety_CK
\date		13/02/2013
\author		Marco Sperandini
*/
#ifndef PM_SFT_CK_COMMON_H_
#define PM_SFT_CK_COMMON_H_

/*\addtogroup PM_Safety_CK
@{*/


#define BASETIME_1S_TO_100MS(counter)	(counter*10)  	/**/
#define BASETIME_10S_TO_100MS(counter)	(counter*100)	/**/
#define FLT_RESET_TIMEOUT_VALUE	50	/* Prefault Reset Timeout (Set to 5 sec) */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**
 * Info related to the status of a generic load
 */
typedef struct {
	uint8_t loadID;
	uint8_t pos;				/* Position of the load in the LoadStatus */
	uint8_t currStatus;
	uint8_t oldStatus;
	uint8_t probeID;
}LoadInfos_t;


void PM_SFT_CK_Init(void);

uint16_t PM_SFT_CK_GetTemperature(void);
uint16_t PM_SFT_CK_GetPrefaultResetTimeout(void);
/*@}*/

#endif /* PM_SFT_CK_COMMON_H_ */
