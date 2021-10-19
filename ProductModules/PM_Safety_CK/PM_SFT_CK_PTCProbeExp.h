/*
 * PM_SFT_CK_PTCProbeExp.h
 *
 *  Created on: 15/lug/2013
 *      Author: consul.fimiani
 */

#ifndef PM_SFT_CK_PTCPROBEEXP_H_
#define PM_SFT_CK_PTCPROBEEXP_H_





/* ****************** */
/* Defines and Macros */
/* ****************** */
#define PTC_TEST_OK					0	/* The test was successful 				*/
#define PTC_TEST_FAILED				1	/* The test failed 						*/
#define PTC_TEST_IN_PROGR			2	/* The test is still in progress		*/
#define PTC_TEST_STOPPED			3	/* The test was stopped					*/



#define PTC_CYCLE_RUN		0xAA	/* The cycle is in run  				*/
#define PTC_CYCLE_STOP		0x55	/* The cycle is stopped 				*/
#define PTC_CYCLE_FAULT		0xBB	/* The cycle is in fault				*/
#define PTC_CYCLE_NOT_VALID 0xFF
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
 * Type of PTC probe
 */
typedef enum
{
	prbTOP,
	prbBOTTOM

}ePTCProbeType;


/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t PM_SFT_CK_PTCProbe_GetProbeTestResult(ePTCProbeType prbType);
uint8_t PM_SFT_CK_PTCProbe_SetCycleStatus(uint8_t stMain, uint8_t stTop, uint8_t stXL);
uint16_t PM_SFT_CK_gettHighTempFaulCounter(void);

#endif /* PM_SFT_CK_PTCPROBEEXP_H_ */
