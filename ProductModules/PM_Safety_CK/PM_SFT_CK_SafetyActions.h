/*
 * PM_PEC_CK_SafetyActions.h
 *
 *  Created on: Feb 18, 2013
 *      Author: francesco.barletta
 */

#ifndef PM_PEC_CK_SAFETYACTIONS_H_
#define PM_PEC_CK_SAFETYACTIONS_H_



#include "Config.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafetyExp.h"
#include "Tools.h"



uint8_t PM_SFT_CK_UnlockDoor(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_LockDoor(iom_loads_struct_t *newLoads, uint8_t status);

uint8_t PM_SFT_CK_KeepHeaterMRelayON(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_HeatersOffByMrOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_HeatersOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_KeepFansMRelayON(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_FansOffByMrOff(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_KeepAllLoadsOFF(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_CK_CoolTumble(iom_loads_struct_t *newLoads, uint8_t status);


#endif /* PM_PEC_CK_SAFETYACTIONS_H_ */
