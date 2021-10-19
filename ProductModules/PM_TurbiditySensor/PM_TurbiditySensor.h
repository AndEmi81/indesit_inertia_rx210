/**
\n
\par		PM_TurbiditySensor
\n
\n
\file		PM_TurbiditySensor.h
\ingroup	PM_TurbiditySensor
\date		14/10/2013
\author		Roberto Fioravanti
\n
*/

#ifndef PM_TURBIDITYSENSOR_H_
#define PM_TURBIDITYSENSOR_H_

/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#if defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
#include "IO_driver.h"
#include "AnalogExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* setting */
typedef struct sPM_TurbSensLLISettings
{
	unsigned char unused[2];			/**< analog channel data not used by this module			*/
	unsigned char delay0;				/**< delay between PWM and AD conversion to start	[20ms]
										This fields plays part in the definition of PM_TurbSensDelayType;
	 	 	 	 	 	 	 	 	 	*/
	unsigned char flags0;				/**< flags:
													bit7-to-1: unused
													bit0:	Polarity:
															1-Low-High
															0-High-Low								*/
}PM_TurbSensLLISettingsType;

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/* Exported Functions */

void PM_TurbiditySensor_SetParam(IO_ChannelType chADC, IO_ChannelType chPWM, PM_TurbSensLLISettingsType* pParam );
void PM_TurbiditySensor_Manager(void);
void PM_TurbiditySensor_StartCalib(ADC_ValueType val);
void PM_TurbiditySensor_StartAcq(IO_TimingValueType val);
bool8_t PM_TurbiditySensor_AcqIsReady(void);
bool8_t PM_TurbiditySensor_GetCalibration(IO_TimingValueType* pVal);
IO_TimingValueType PM_TurbiditySensor_GetPWMPeriod(void);
#else
#define PM_TurbiditySensor_SetParam(x, y, z)			;
#define PM_TurbiditySensor_Manager()
#define PM_TurbiditySensor_StartCalib()
#define PM_TurbiditySensor_StartAcq()

#define PM_TurbiditySensor_AcqIsReady()					FALSE
#define PM_TurbiditySensor_GetCalibration(x)			FALSE
#define PM_TurbiditySensor_GetPeriodPWM()
#endif /*defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)*/

#endif /* PM_TURBIDITYSENSOR_H_ */

/* *********** */
/* End of file */
/* *********** */
