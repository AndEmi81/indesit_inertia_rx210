/**
\n	This module contains Loads Request modules
\n
\file		LoadsRequestExp.h
\ingroup    LoadsRequest
\date	    2013-01-14
\version	01.00.00
\author		 Alessandro Sabbatini
\n
\n*/

#ifndef _LIB_IOM_LOADSREQUESTEXP_H_
#define _LIB_IOM_LOADSREQUESTEXP_H_

#include "OutputManagerExp.h"


#define LOAD_SR_NOT_FOUND		0x00
#define LOAD_SR_FOUND			0x01
#define LOAD_SR_CFG_COUPLE		0x02
#define LOAD_SR_SAME_POS		0x04
#define LOAD_SR_POS_ON			0x08
#define LOAD_SR_ITSELF			0x10
#define LOAD_SR_TRANSITION		0x20

#define LOAD_MR_NOT_FOUND		0x00
#define LOAD_MR_FOUND			0x01
#define LOAD_MR_CFG_COUPLE		0x02
#define LOAD_MR_SAME_POS		0x04
#define LOAD_MR_POS_ON			0x08
#define LOAD_MR_ITSELF			0x10
#define LOAD_MR_TRANSITION		0x20

typedef struct
{
	uint8_t extLoadItm;
	uint8_t loadID;
	uint8_t pilotType;
	uint8_t szInfoBuffer;
	uint8_t *pInfoBuffer;
} ext_load_config;

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

#if !defined(_LIB_IOM_LOADSREQUEST_)
extern const uint8_t IOM_LoadsReq[N_MAXLOADS];
#endif

uint8_t LIB_IOM_GetLoadPos(uint8_t loadItem);
uint8_t LIB_IOM_GetLoadPosByExtdLoadItem(uint8_t extdLoadItem);
uint8_t LIB_IOM_UpdateLoadRequestByItem(uint8_t loadItem, uint8_t newValue, uint8_t *loadReqArray);
uint8_t LIB_IOM_GetExtdLoadPosByLoadPos(uint8_t loadPos);


uint8_t LIB_IOM_FindLoadOnSwitchRelay(uint8_t loadPos);
uint8_t LIB_IOM_GetSRLoadPosByRelLdPos(uint8_t loadPos);
uint8_t LIB_IOM_FindLoadOnMasterRelay(uint8_t loadPos);
uint8_t LIB_IOM_GetMRLoadPosByRelLdPos(uint8_t loadPos);
uint8_t LIB_IOM_GetExtBoardPos(uint8_t extLoadItem);
bool_t LIB_IOM_ExtdLoadIsExternal(uint8_t extdLoadItem);

void LIB_IOM_ProcessAllLoadsRequests(const uint8_t *pNewLoadsReq,  uint8_t  (*const pNewExtdLoadsReq)[CONFIG_EXT_LOAD_PARAMDATA_SZ]);

#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
#endif /* _LIB_IOM_LOADSREQUESTEXP_H_ */
