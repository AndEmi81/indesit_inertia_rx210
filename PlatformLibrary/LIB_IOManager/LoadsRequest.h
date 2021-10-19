/**
\n	This module contains Loads Request modules
\n
\file		LoadsRequest.h
\ingroup    LoadsRequest
\date	    2013-01-14
\version	01.00.00
\author		 Alessandro Sabbatini
\n
\n
*/

#ifndef _LIB_IOM_LOADSREQUEST_H_
#define _LIB_IOM_LOADSREQUEST_H_

#include "LoadsRequestExp.h"

#define		IOM_LDREQ_STARTUP_KEYWORD		0xBE50
#define		IOM_LDREQ_RUNNING_KEYWORD		0

typedef struct iom_loads_struct
{
	const uint8_t *loads_req;
	uint8_t *loads_ovr;
#if defined(CONFIG_NUM_EXTDLOADS)
	const uint8_t (*extdloads_req)[CONFIG_EXT_LOAD_PARAMDATA_SZ];
	uint8_t (*extdloads_ovr)[CONFIG_EXT_LOAD_PARAMDATA_SZ];
#endif
} iom_loads_struct_t;

typedef struct iom_extdload_pos
{
	uint8_t	loadPos;
	uint8_t extBoardPos;
} iom_extdload_pos_t;

#if !defined(_LOADS_CFG_)

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

extern const uint8_t IOM_LoadID_Table[LD_MAX_NUMBER];

#if defined(CONFIG_NUM_EXTDLOADS)
extern const ext_load_config IOM_extdLoadID_Table[CONFIG_NUM_EXTDLOADS];

#endif

#if defined(CONFIG_NUM_SR_LOADS)
extern const uint8_t IOM_LoadsID_SRTypeLoads_Table[CONFIG_NUM_SR_LOADS];

#endif

#if defined(CONFIG_NUM_MR_LOADS)
extern const uint8_t IOM_LoadsID_MRTypeLoads_Table[CONFIG_NUM_MR_LOADS];

#if defined(CONFIG_MR_CUSTOM_TABLE)

/** These must be defined in Load_cfg.c, if used !!! */

extern const uint8_t *IOM_Mrelay_CstmTable_LoadID[CONFIG_NUM_MR_LOADS];
extern uint8_t *IOM_Mrelay_CstmTable[CONFIG_NUM_MR_LOADS];
extern const uint8_t IOM_Mrelay_CstmTableLen[CONFIG_NUM_MR_LOADS];

#endif

#endif

#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */

#endif



#if defined(CONFIG_NUM_SR_LOADS)
uint8_t LIB_IOM_GetSRDelayTime(uint8_t srItem, uint8_t newLoadStatus);
#endif

#if defined(CONFIG_NUM_MR_LOADS)
uint8_t LIB_IOM_GetMRDelayTime(uint8_t mrItem, uint8_t newLoadStatus);
#endif


void LIB_IOM_InitLoadsPos(void);
void LIB_IOM_SRPos_Init(void);
void LIB_IOM_MRPos_Init(void);
void LIB_IOM_LoadsRequestManager(void);

void LIB_IOM_ProcessSingleLoadRequest(const GIOM_LoadStruct *newLoadsReq);
void LIB_IOM_ResetAllLoadRequest(void);

#endif /* _LIB_IOM_LOADSREQUEST_H_ */
