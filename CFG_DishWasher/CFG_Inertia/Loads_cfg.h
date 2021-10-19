/*
 * Loads_Cfg.h
 *
 *  Created on: Jan 14, 2013
 *      Author: alessandro.sabbatini
 */

#ifndef _LOADS_CFG_H_
#define _LOADS_CFG_H_



#define N_EXTERNAL 2



#define CONFIG_NUM_EXTDLOAD_CFG		(N_EXTERNAL )
#define CONFIG_NUM_EXTDLOADS		1

#define ID_IND_MASK					0x0F

#define MAKE_IO_ID(type,index)		((uint8_t)(((type) << 4)|((index) & ID_IND_MASK)))



bool_t LIB_IOM_CFG_SetAllLoadsFilter(uint8_t loadPos);

#endif /* _LOADS_CFG_H_ */
