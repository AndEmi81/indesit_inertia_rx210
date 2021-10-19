/*
 * PM_AlternatoExp.h
 *
 *  Created on: 06/nov/2013
 *      Author: consul.fimiani
 */

#ifndef PM_ALTERNATOEXP_H_
#define PM_ALTERNATOEXP_H_

#include "StandardTypes.h"


/* If TRUE, the positioning is ok */
bool_t PM_ALT_posReached(void);

/* Returns the current position of the servomechanism.
 * The possible values are: from 0 to (AlternatoVars.numAltPositions-1)
 * else the position is not defined */
uint8_t PM_ALT_getPos(void);

/* Returns the number of positions of the current servomechanism */
uint8_t PM_ALT_getNumPos(void);

/* Returns the value for Position Not Defined */
uint8_t PM_ALT_getPosNotDef(void);


#endif /* PM_ALTERNATOEXP_H_ */
