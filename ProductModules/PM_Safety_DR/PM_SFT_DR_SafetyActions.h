/*
 * PM_PEC_WM_SafetyActions.h
 *
 *  Created on: Feb 18, 2013
 *      Author: francesco.barletta
 */

#ifndef PM_SFT_DR_SAFETYACTIONS_H_
#define PM_SFT_DR_SAFETYACTIONS_H_



#include "Config.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafetyExp.h"
#include "Tools.h"


/* typedef */

uint8_t PM_SFT_DR_OffHeatersMRelay(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_DR_KeepHeaterMRelayON(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_DR_KeepMotorMRelayON(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_DR_CoolTumble(iom_loads_struct_t *newLoads, uint8_t status);
uint8_t PM_SFT_DR_OffMotorMRelay(iom_loads_struct_t *newLoads, uint8_t status);


#endif /* PM_SFT_DR_SAFETYACTIONS_H_ */
