/**
\n
\par		PM_Safety_WM_PressSwitch
\n
\n
\file		PM_SFT_WM_PressSwitchExp.h
\ingroup	PM_SFT_PressSwitch
\date		04/04/2013
\author		Salvatore Fimiani
\n
*/

#ifndef PM_SFT_WM_PRESSSWITCHEXP_H_
#define PM_SFT_WM_PRESSSWITCHEXP_H_

#include "PM_LinearPressureSensor.h"

#define TEST_IN_PROGR	0
#define TEST_OK			1
#define TEST_FAILED		2
#define TEST_STOPPED	3





/* */

/* To be called by the application to get the status for the water filling test */
uint8_t PM_SFT_WM_PrsSwitch_GetFillTestStatus(void);

/* To be called by the application to set the empty calibration value for the pressure switch
 * at cycle start */
void PM_SFT_WM_PrsSwitch_SetZeroRef(LPS_ValueType zeroValue);

/* To be called by the application to indicate it is not a cycle start */
void PM_SFT_WM_PrsSwitch_ClearCycleInProgrFlag(void);

#endif /* PM_SFT_WM_PRESSSWITCHEXP_H_ */
