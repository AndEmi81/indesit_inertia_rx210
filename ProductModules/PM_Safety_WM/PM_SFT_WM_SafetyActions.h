/**
\n
\par		PM_Safety_WM_SafetyActions
\n
\n
\file		PM_SFT_WM_SafetyActions.h
\ingroup	PM_SFT_SafetyActions
\date		18/02/2013
\author		Francesco Barletta
\n
*/

#ifndef PM_PEC_WM_SAFETYACTIONS_H_
#define PM_PEC_WM_SAFETYACTIONS_H_



#include "Config.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafetyExp.h"
#include "Tools.h"


/* typedef */



uint8_t PM_SFT_WM_LoadsOff_Res(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_SwitchRelayOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_FanOn(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_LoadsOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_ForceDrainOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_Drain(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_ElectrovAllOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_UnlockDoor(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_LoadsOffTotal(iom_loads_struct_t *newLoads, uint8_t status);
/*motor*/
uint8_t PM_SFT_WM_RestoreCommWithExternalBoard(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_WM_DisableTmpCommWithExternalBoard(iom_loads_struct_t *pNewLoads, uint8_t init);
uint8_t PM_SFT_WM_SuspendDoorlock(iom_loads_struct_t *pNewLoads, uint8_t init);
/*washres*/
uint8_t PM_SFT_WM_SwitchRelayTestProcedure(iom_loads_struct_t *pNewLoads, uint8_t init);
#if defined(CONFIG_PM_MOTORGATE)
uint8_t PM_SFT_WM_AC_MotorProt(iom_loads_struct_t *pNewLoads, uint8_t init);
#endif /* defined(CONFIG_PM_MOTORGATE) */
#endif /* PM_PEC_WM_SAFETYACTIONS_H_ */
