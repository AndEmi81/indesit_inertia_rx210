/*
Copyright (c) 2004-2013, Indesit Company
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n	This module contains Loads Request modules
\n
\file		LoadsRequest.c
\ingroup    LoadsRequest
\date	    2013-01-17
\version	01.00.00
\author		 Alessandro Sabbatini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		2013-01-17
\n		Alessandro Sabbatini
\n		changes
\n 			Creation
\n
*/
#define MODULE_BODY_LIB_IOMANAGER
/* ******** */
/* Includes */
/* ******** */

#define _LIB_IOM_LOADSREQUEST_
#include "string.h"
#include "Config.h"
#include "LoadsRequest.h"
#include "OutputManager.h"
#include "ExternalOutputManagerExp.h"
#include "LIB_ProductSafety.h"
#include "LIB_ClassBVar.h"
#include "LIB_Error.h"

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
#include "PM_LinkedLoads.h"
#endif


#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

static struct iom_loadsreq_status_s
{
	uint16_t	keyword;
	uint16_t	timeout;
	uint8_t idPos[LD_MAX_NUMBER];
} IOM_LoadsReq_Status;


/** todo: da descrivere */
uint8_t IOM_LoadsReq[N_MAXLOADS];

#if defined(CONFIG_NUM_EXTDLOADS)
static iom_extdload_pos_t IOM_extdLoads_Pos[CONFIG_NUM_EXTDLOADS];
static uint8_t IOM_extdLoadsReq[CONFIG_NUM_EXTDLOADS][CONFIG_EXT_LOAD_PARAMDATA_SZ];
#else
#define IOM_extdLoadsReq 	NULL
#endif

#if defined(CONFIG_NUM_SR_LOADS)

/** Parametri HI Level del carico Switch Relay */
typedef struct _SwitchRHLParams_Struct
{
	uint8_t LoadPosition_SwitchRelayON;		/**< Posizione del carico della coppia per il quale � necessario pilotare anche lo Switch Relay */
	uint8_t LoadPosition_SwitchRelayOFF;	/**< Posizione del carico della coppia per il quale � necessario pilotare anche lo Switch Relay */
} SwitchRHLParams_Struct;

/** Parametri HI Level del carico Switch Relay con dato indicante la lunghezza dell'area */
typedef struct _SwitchRelayHLParams_Struct
{
	SwitchRHLParams_Struct *pCoupleData;			/**< Puntatore all'area dati contenente le coppie di carichi connesse allo Switch Relay*/
	uint8_t CoupleDataLength;						/**< Lunghezza area dati contenente le coppie */
	uint8_t DelayCnt;
} SwitchRelayHLParams_Struct;

static uint8_t IOM_LoadsID_SRTypeLoads_Pos[CONFIG_NUM_SR_LOADS];

/** variabili utilizzate con carico "Switch Relay"*/
static SwitchRelayHLParams_Struct IOM_SwitchRelay_Params[CONFIG_NUM_SR_LOADS];

static void LIB_IOM_ManageSwitchRelay(uint8_t srItem, iom_loads_struct_t *newLoadStr);
static void LIB_IOM_InitSR_Params(uint8_t srItem);

#endif

#if defined(CONFIG_NUM_MR_LOADS)
/** HI Level params of Master Relay load */
typedef struct _MasterRHLParams_Struct
{
	uint8_t LdPos_MRelON;		/**< Position of load linked to Master Relay ON status */
	uint8_t LdPos_MRelOFF;		/**< Position of load linked to Master Relay OFF status */
} MasterRHLParams_Struct;

/** Parametri HI Level del carico Switch Relay con dato indicante la lunghezza dell'area */
typedef struct _MRelayHLParm_Struct
{
	MasterRHLParams_Struct *pCoupleData;			/**< Pointer of couples connected to Master Relay */
	uint8_t CoupleDataLen;							/**< Data length of couples configuration */
	uint8_t DelayCnt;
} MRelayHLParm_Struct;

static uint8_t IOM_LoadsID_MRTypeLoads_Pos[CONFIG_NUM_MR_LOADS];

/** Master Relay status param vars */
static MRelayHLParm_Struct IOM_MRelay_Params[CONFIG_NUM_MR_LOADS];

static void LIB_IOM_ManageMasterRelay(uint8_t mrItem, iom_loads_struct_t *newLoadStr);
static void LIB_IOM_InitMR_Params(uint8_t mrItem);

#endif

static void LIB_IOM_IntRefreshTimeout(void);

/**
\n Function that return setting file Load position of a load present in Loads configuration table

\param		[in]	loadItem	Position of load in Loads configuration table

\return		uint8_t

\retval		== LOAD_NOT_FOUND	if selected Load is not present in setting file
\retval		!= LOAD_NOT_FOUND	Setting file load position


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_GetLoadPos(uint8_t loadItem)
{
	uint8_t tmpLoadPos = LOAD_NOT_FOUND;

	if (loadItem < LD_MAX_NUMBER) {
		tmpLoadPos = IOM_LoadsReq_Status.idPos[loadItem];
	}

	return tmpLoadPos;
}

/**
\n Function that updates a Load request status of a load - if present - in Loads configuration table

\param		[in]	loadItem		Position of load in Loads configuration table
\param		[in]	newItem			New Load request status value
\param		[out]	loadReqArray	Load request array to update

\return		uint8_t

\retval		== LOAD_NOT_FOUND	if selected Load is not present in setting file
\retval		!= LOAD_NOT_FOUND	Setting file load position


\author 	Alessandro Sabbatini
\date		Apr 18, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_UpdateLoadRequestByItem(uint8_t loadItem, uint8_t newValue, uint8_t *loadReqArray)
{
	uint8_t tmpLoadPos = LOAD_NOT_FOUND;

	if (loadItem < LD_MAX_NUMBER) {
		tmpLoadPos = IOM_LoadsReq_Status.idPos[loadItem];

		if (tmpLoadPos < N_MAXLOADS)
		{
			loadReqArray[tmpLoadPos] = newValue;
		}

	}

	return tmpLoadPos;
}

/**
\n Function that return setting file Load position of an extended load present
\n in Extended Loads configuration table

\param		[in]	extloadItem	Position of Extended load in Extended Loads configuration table

\return		uint8_t

\retval		== LOAD_NOT_FOUND	if selected Load is not present in setting file
\retval		!= LOAD_NOT_FOUND	Setting file load position


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_GetLoadPosByExtdLoadItem(uint8_t extLoadItem)
{
	uint8_t tmpLoadPos = LOAD_NOT_FOUND;

#if defined(CONFIG_NUM_EXTDLOADS)

	if (extLoadItem < CONFIG_NUM_EXTDLOADS)
	{
		tmpLoadPos = IOM_extdLoads_Pos[extLoadItem].loadPos;
	}

#endif

	return tmpLoadPos;

}

/**
\n Function that return External Board position table related to an Extended load - if external

\param		[in]	extloadItem	Position of Extended load in Extended Loads configuration table

\return		uint8_t

\retval		== LOAD_NOT_FOUND	if extended load is not an external one
\retval		!= LOAD_NOT_FOUND	External Board position in IOM_External_LoadsTable[]


\author 	Alessandro Sabbatini
\date		Sep 02, 2014
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_GetExtBoardPos(uint8_t extLoadItem)
{
	uint8_t retVal = LOAD_NOT_FOUND;

#if defined(CONFIG_NUM_EXTDLOADS)

	if (extLoadItem < CONFIG_NUM_EXTDLOADS)
	{
		retVal = IOM_extdLoads_Pos[extLoadItem].extBoardPos;
	}

#endif

	return retVal;
}

/**
\n Function that return if an Extended load is external

\param		[in]	extloadItem	Position of Extended load in Extended Loads configuration table

\return		bool_t

\retval		TRUE 				if extended load is not an external one
\retval		FALSE				otherwise

\author 	Alessandro Sabbatini
\date		Sep 02, 2014
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

bool_t LIB_IOM_ExtdLoadIsExternal(uint8_t extLoadItem)
{
	return ( LIB_IOM_GetExtBoardPos(extLoadItem) != LOAD_NOT_FOUND);
}

/**
\n Function that return Extended Load item position in configuration file
\n (same position used for related buffer) searching from setting file load position

\param		[in]	loadPos	Setting file load position to search

\return		uint8_t

\retval		== LOAD_NOT_FOUND	if not present or not an Extended load
\retval		!= LOAD_NOT_FOUND	Extended Load item position


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_IOM_GetExtdLoadPosByLoadPos(uint8_t loadPos)
{
	uint8_t tmpExtdLoadPos = LOAD_NOT_FOUND;
	uint8_t i;

#if defined(CONFIG_NUM_EXTDLOADS)

	for (i = 0;((tmpExtdLoadPos == LOAD_NOT_FOUND) && (i < CONFIG_NUM_EXTDLOADS)); i++)
	{
		if (loadPos == IOM_extdLoads_Pos[i].loadPos)
		{
			tmpExtdLoadPos = i;
		}

	}
#else
	(void) i;
#endif

	return tmpExtdLoadPos;

}
/**
\n Function that initializes all load position defined; these are stored into
\n		IOM_LoadsReq_Status.idPos[]
\n		IOM_extLoads_Pos[] (if CONFIG_NUM_EXTDLOADS is defined)
\n		IOM_LoadsID_SRTypeLoads_Pos[] (if CONFIG_NUM_SR_LOADS is defined)
\n		IOM_LoadsID_MRTypeLoads_Pos[] (if CONFIG_NUM_MR_LOADS is defined)

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_InitLoadsPos(void)
{
	uint8_t i;

	SAFETY_REGION_START();

	for (i=0; i < LD_MAX_NUMBER; i++)
	{
		IOM_LoadsReq_Status.idPos[i] = LIB_IOM_FindLoadByID(IOM_LoadID_Table[i],(uint8_t **) NULL);
	}

	IOM_LoadsReq_Status.keyword = IOM_LDREQ_STARTUP_KEYWORD;
	IOM_LoadsReq_Status.timeout = CONFIG_LIB_LOADSREQ_TIMEOUT;
	SAFETY_DATA_UPDATE(IOM_LoadsReq_Status);

	/* Clear and initialize Loads Requests */
	memset(IOM_LoadsReq, 0, sizeof(IOM_LoadsReq));
	SAFETY_DATA_UPDATE(IOM_LoadsReq);

#if defined(CONFIG_NUM_EXTDLOADS)

	/* Clear and initialize Extended Loads Requests */
	memset((uint8_t *)IOM_extdLoadsReq, 0, sizeof(IOM_extdLoadsReq));
	SAFETY_DATA_UPDATE(IOM_extdLoadsReq);

	/* Fill extended loads position table */
	memset(&IOM_extdLoads_Pos[0], LOAD_NOT_FOUND, sizeof(IOM_extdLoads_Pos) );

	for (i=0; i < CONFIG_NUM_EXTDLOAD_CFG; i++)
	{
		uint8_t tmpPos = LIB_IOM_FindLoadByID(IOM_extdLoadID_Table[i].loadID, NULL);
		uint8_t tmpPilotType = LIB_IOM_GetLoadPilotType(tmpPos);

		if (tmpPilotType == IOM_extdLoadID_Table[i].pilotType)
		{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)

			if  (IOM_extdLoadID_Table[i].szInfoBuffer != 0)
			{
				if (LIB_IOM_External_New( IOM_extdLoadID_Table[i].pInfoBuffer, IOM_extdLoadID_Table[i].szInfoBuffer, tmpPos) != LIB_IOM_NO_ERROR)
				{
					/* If function fails, extended load is disabled */
					tmpPos = LOAD_NOT_FOUND;
				}
			}
#endif

			if (tmpPos != LOAD_NOT_FOUND)
			{
				iom_extdload_pos_t *pTmpExtItm = &IOM_extdLoads_Pos[IOM_extdLoadID_Table[i].extLoadItm];

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
				pTmpExtItm->extBoardPos = LIB_IOM_Ext_GetBoardCounter() - 1;
#endif
				pTmpExtItm->loadPos = tmpPos;
			}
		}

	}

	SAFETY_DATA_UPDATE(IOM_extdLoads_Pos);
#endif

	SAFETY_REGION_END();

	LIB_IOM_SRPos_Init();
	LIB_IOM_MRPos_Init();

}

/**
\n Function that initializes Switch relay loads and structures in
\n		IOM_LoadsID_SRTypeLoads_Pos[] (if CONFIG_NUM_SR_LOADS is defined)

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Oct 25, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_SRPos_Init(void)
{
	uint8_t i;

#if defined(CONFIG_NUM_SR_LOADS)

	SAFETY_REGION_START();
	for (i=0; i < CONFIG_NUM_SR_LOADS; i++)
	{
		IOM_LoadsID_SRTypeLoads_Pos[i] = LIB_IOM_FindLoadByID(IOM_LoadsID_SRTypeLoads_Table[i],(uint8_t **) NULL);

		if ( IOM_LoadsID_SRTypeLoads_Pos[i] != LOAD_NOT_FOUND )
		{
			LIB_IOM_InitSR_Params(i);
		}
	}

	SAFETY_DATA_UPDATE(IOM_LoadsID_SRTypeLoads_Pos);
	SAFETY_REGION_END();

#else
	(void) i;
#endif

}

/**
\n Function that initializes Master relay loads and structures in
\n		IOM_LoadsID_MRTypeLoads_Pos[] (if CONFIG_NUM_MR_LOADS is defined)

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Oct 25, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_MRPos_Init(void)
{
	uint8_t i;

#if defined(CONFIG_NUM_MR_LOADS)

	SAFETY_REGION_START();
	for (i=0; i < CONFIG_NUM_MR_LOADS; i++)
	{
		IOM_LoadsID_MRTypeLoads_Pos[i] = LIB_IOM_FindLoadByID(IOM_LoadsID_MRTypeLoads_Table[i],(uint8_t **) NULL);

		if ( IOM_LoadsID_MRTypeLoads_Pos[i] != LOAD_NOT_FOUND )
		{
			LIB_IOM_InitMR_Params(i);
		}
	}

	SAFETY_DATA_UPDATE(IOM_LoadsID_MRTypeLoads_Pos);
	SAFETY_REGION_END();

#else
	(void) i;
#endif

}
/**
\n Function that processes all Loads Request from application

\param		[in] newLoadsReq		Array of all Loads request (max index is N_MAXLOADS)
\param		[in] newExtdLoadsReq	Array of all Extended Loads request (max index is CONFIG_NUM_EXTDLOADS)

\return		none

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_ProcessAllLoadsRequests(const uint8_t *newLoadsReq, uint8_t (*const newExtdLoadsReq)[CONFIG_EXT_LOAD_PARAMDATA_SZ])
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IOM_LoadsReq);
	memcpy(IOM_LoadsReq, newLoadsReq, sizeof(IOM_LoadsReq));
	SAFETY_DATA_UPDATE(IOM_LoadsReq);
#if defined(CONFIG_NUM_EXTDLOADS)
	SAFETY_DATA_CHECK(IOM_extdLoadsReq);
	memcpy(IOM_extdLoadsReq, newExtdLoadsReq, sizeof(IOM_extdLoadsReq));
	SAFETY_DATA_UPDATE(IOM_extdLoadsReq);
#endif
	LIB_IOM_IntRefreshTimeout();
	SAFETY_REGION_END();
}

/**
\n Function that processes all Loads Request from application

\param		[in] newLoadsReq		Load request (in GIOM_LoadStruct format)

\return		none

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_ProcessSingleLoadRequest(const GIOM_LoadStruct *newLoadsReq)
{
#if defined(CONFIG_NUM_EXTDLOADS)
	uint8_t extLoadPos = LIB_IOM_GetExtdLoadPosByLoadPos(newLoadsReq->loadID);
#endif /* defined(CONFIG_NUM_EXTDLOADS) */
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IOM_LoadsReq);
#if defined(CONFIG_NUM_EXTDLOADS)
	SAFETY_DATA_CHECK(IOM_extdLoadsReq);
#endif

#if defined(CONFIG_NUM_EXTDLOADS)
	if (extLoadPos != LOAD_NOT_FOUND)
	{
		memcpy(IOM_extdLoadsReq[extLoadPos], newLoadsReq->data, sizeof(IOM_extdLoadsReq[0]));
	}
	else
#endif /* defined(CONFIG_NUM_EXTDLOADS) */
	{
		IOM_LoadsReq[newLoadsReq->loadID] = newLoadsReq->loadParam.b;
	}

	SAFETY_DATA_UPDATE(IOM_LoadsReq);
#if defined(CONFIG_NUM_EXTDLOADS)
	SAFETY_DATA_UPDATE(IOM_extdLoadsReq);
#endif
	LIB_IOM_IntRefreshTimeout();
	SAFETY_REGION_END();
}

/**
\n Function that reset all Loads Request

\param		[in] newLoadsReq		Load request (in GIOM_LoadStruct format)

\return		none

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_ResetAllLoadRequest(void)
{
	SAFETY_REGION_START();

	memset(IOM_LoadsReq, 0, sizeof(IOM_LoadsReq));
	SAFETY_DATA_UPDATE(IOM_LoadsReq);
#if defined(CONFIG_NUM_EXTDLOADS)
	memset((uint8_t *)IOM_extdLoadsReq, 0, sizeof(IOM_extdLoadsReq));
	SAFETY_DATA_UPDATE(IOM_extdLoadsReq);
#endif
	LIB_IOM_IntRefreshTimeout();
	SAFETY_REGION_END();
}

/**
\n Loads Request Manager - this function calls Safety Supervisor and applies all load requests

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Feb 19, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IOM_LoadsRequestManager(void)
{

	iom_loads_struct_t tmpLoads;
	GIOM_LoadStruct load;
	static uint8_t IOM_LoadsOvr[N_MAXLOADS];
#if defined(CONFIG_NUM_EXTDLOADS)
	static uint8_t IOM_extdLoadsOvr[CONFIG_NUM_EXTDLOADS][CONFIG_EXT_LOAD_PARAMDATA_SZ];
#endif
	uint8_t i;
	uint8_t nLoads = LIB_IOM_NumberOfLoads();

	LIB_LOADSREQUEST__MONITOR_IN();

	if (IOM_LoadsReq_Status.keyword != IOM_LDREQ_STARTUP_KEYWORD)
	{
		/* Force Safety supervisor monitoring enabled */
		START_LIB_PRODUCTSAFETY_TASK_MONITORING();

		/* Reset all Override loads - reprocessed each iteration */
		memset(IOM_LoadsOvr, LOAD_NOT_FOUND, sizeof(IOM_LoadsOvr));

	#if defined(CONFIG_NUM_EXTDLOADS)
		memset((uint8_t *)IOM_extdLoadsOvr, 0, sizeof(IOM_extdLoadsOvr));
	#endif

		/* Process all Safety routines via Safety Supervisor */
		tmpLoads.loads_req = IOM_LoadsReq;
		tmpLoads.loads_ovr = IOM_LoadsOvr;
	#if defined(CONFIG_NUM_EXTDLOADS)
		tmpLoads.extdloads_req = (const uint8_t (*)[CONFIG_EXT_LOAD_PARAMDATA_SZ]) IOM_extdLoadsReq;
		tmpLoads.extdloads_ovr = IOM_extdLoadsOvr;
	#endif

	#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

		LIB_SFT_SafetySupervisor(&tmpLoads);

	#endif

	#if defined(CONFIG_NUM_MR_LOADS)
		/* Manages the Master Relay after the Safety is called */
		for (i = 0; i < CONFIG_NUM_MR_LOADS; i++)
		{
			if (IOM_LoadsID_MRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
			{
				LIB_IOM_ManageMasterRelay(i, &tmpLoads);
			}
		}
	#endif

	#if defined(CONFIG_NUM_SR_LOADS)
		/* Manages the Switch Relay after the Safety is called */
		for (i = 0; i < CONFIG_NUM_SR_LOADS; i++)
		{
			if (IOM_LoadsID_SRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
			{
				LIB_IOM_ManageSwitchRelay(i, &tmpLoads);
			}
		}
	#endif

		/* Loop to process all loads request coming from Application and Safety modules */
		for (i = 0; i < nLoads; i++)
		{
	#if defined(CONFIG_NUM_EXTDLOADS)
			uint8_t extLoadItem = LIB_IOM_GetExtdLoadPosByLoadPos(i);
	#endif
			if (IOM_LoadsOvr[i] != LOAD_DONT_EXE_SETLOAD)
			{
				load.loadID = i;

				if ( (IOM_LoadsOvr[i] != LOAD_NOT_FOUND) )
				{
					load.loadParam.b = IOM_LoadsOvr[i];
				}
				else
				{
					load.loadParam.b = IOM_LoadsReq[i];
				}

		#if defined(CONFIG_NUM_EXTDLOADS)
				if (extLoadItem != LOAD_NOT_FOUND)
				{
					if (IOM_LoadsOvr[i] != LOAD_NOT_FOUND)
					{
						memcpy(load.data, IOM_extdLoadsOvr[extLoadItem], sizeof(load.data));
					}
					else
					{
						memcpy(load.data, IOM_extdLoadsReq[extLoadItem], sizeof(load.data));
					}
				}
		#endif


				/* Error handling already done into this function */
				(void) LIB_IOM_GIOM_SetLoad(&load);
			}

		}
	}

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IOM_LoadsReq_Status);

#if defined(CONFIG_NUM_EXTDLOADS)
	SAFETY_DATA_CHECK(IOM_extdLoads_Pos);
#endif
	/* Remote control disables LoadsRequest timeout */
	if (!Main_IsRemoteEnabled())
	{
		/* When timeout reached, set all loads to OFF */
		if (IOM_LoadsReq_Status.timeout == 0)
		{
			LIB_IOM_ResetAllLoadRequest();
		}
		else
		{
			/* Timeout countdown */
			IOM_LoadsReq_Status.timeout--;
			SAFETY_DATA_UPDATE(IOM_LoadsReq_Status);
		}
	}
	SAFETY_REGION_END();

	LIB_LOADSREQUEST__MONITOR_OUT();

}

/**
\n Function that find if and how a load is configured on a Switch Relay

\param		[in] loadPos			Position

\return		Result is LOAD_SR_NOT_FOUND if load is not found, otherwise a bitmap with these values:

\retval		LOAD_SR_FOUND			Load is found on a Switch Realy configuration
\retval		LOAD_SR_CFG_COUPLE		Load is configured coupled with another one
\retval		LOAD_SR_SAME_POS		Switch Relay load status is on related Load position
\retval		LOAD_SR_POS_ON			Load is configured in Switch Relay ON position
\retval		LOAD_SR_ITSELF			Load requested is a Switch Realy itself
\retval		LOAD_SR_TRANSITION		Switch Relay is switching position

\author 	Alessandro Sabbatini
\date		Jan 31, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_IOM_FindLoadOnSwitchRelay(uint8_t loadPos)
{
	uint8_t tmpRes = LOAD_SR_NOT_FOUND;
	uint8_t i;

#if defined(CONFIG_NUM_SR_LOADS)

	for (i = 0; (tmpRes == LOAD_SR_NOT_FOUND) && (i < CONFIG_NUM_SR_LOADS); i++)
	{

		if (loadPos == IOM_LoadsID_SRTypeLoads_Pos[i])
		{
			tmpRes = LOAD_SR_FOUND | LOAD_SR_ITSELF;
		}
		else if (IOM_LoadsID_SRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
		{
			uint8_t j;
			uint8_t swRelayStatus;

			for (j=0; !(tmpRes & LOAD_SR_FOUND) && (j < IOM_SwitchRelay_Params[i].CoupleDataLength); j++)
			{
				uint8_t posON = IOM_SwitchRelay_Params[i].pCoupleData[j].LoadPosition_SwitchRelayON;
				uint8_t posOFF = IOM_SwitchRelay_Params[i].pCoupleData[j].LoadPosition_SwitchRelayOFF;

				swRelayStatus = LIB_IOM_GetLoadStatus(IOM_LoadsID_SRTypeLoads_Pos[i]);

				if (loadPos == posON)
				{
					tmpRes = LOAD_SR_FOUND | LOAD_SR_POS_ON;

					if (posOFF != LOAD_NOT_FOUND)
					{
						tmpRes |= LOAD_SR_CFG_COUPLE;
					}

					if (swRelayStatus == LOAD_REG_ON)
					{
						tmpRes |= LOAD_SR_SAME_POS;
					}
				}
				else if (loadPos == posOFF)
				{
					tmpRes = LOAD_SR_FOUND;

					if (posON != LOAD_NOT_FOUND)
					{
						tmpRes |= LOAD_SR_CFG_COUPLE;
					}

					if (swRelayStatus == LOAD_REG_OFF)
					{
						tmpRes |= LOAD_SR_SAME_POS;
					}
				}
				else
				{
					/* Nothing to do, go on search */
				}
			}
		}
		else
		{
			/* Nothing to do, go on search */
		}

	}

	if (tmpRes != LOAD_SR_NOT_FOUND)
	{
		if (IOM_SwitchRelay_Params[i-1].DelayCnt != 0)
		{
			tmpRes |= LOAD_SR_TRANSITION;
		}
	}

#else
	(void) i;

#endif

	return tmpRes;
}

/** Function that looks position of Switch Relay (if any) where a given load
 	is configured.

\param		[in] loadPos			Position

\return		Result is LOAD_NOT_FOUND if load is not found, otherwise switch relay position

\author 	Alessandro Sabbatini
\date		Oct 16, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini

 */
uint8_t LIB_IOM_GetSRLoadPosByRelLdPos(uint8_t loadPos)
{
	uint8_t srPos = LOAD_NOT_FOUND;
	uint8_t i;

#if defined(CONFIG_NUM_SR_LOADS)

	for (i = 0; (srPos == LOAD_NOT_FOUND) && (i < CONFIG_NUM_SR_LOADS); i++)
	{
		uint8_t j;

		if (IOM_LoadsID_SRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
		{
			for (j=0; (srPos == LOAD_NOT_FOUND) && (j < IOM_SwitchRelay_Params[i].CoupleDataLength); j++)
			{
				if ( (loadPos == IOM_SwitchRelay_Params[i].pCoupleData[j].LoadPosition_SwitchRelayON) ||
					(loadPos == IOM_SwitchRelay_Params[i].pCoupleData[j].LoadPosition_SwitchRelayOFF) )
				{
					srPos = IOM_LoadsID_SRTypeLoads_Pos[i];
				}
			}
		}

	}

#else
	(void) i;

#endif

	return srPos;
}

/**
\n Function that find if and how a load is configured on a Master Relay

\param		[in] loadPos			Position

\return		Result is LOAD_MR_NOT_FOUND if load is not found, otherwise a bitmap with these values:

\retval		LOAD_MR_FOUND			Load is found on a Switch Realy configuration
\retval		LOAD_MR_CFG_COUPLE		Load is configured coupled with another one
\retval		LOAD_MR_SAME_POS		Switch Relay load status is on related Load position
\retval		LOAD_MR_POS_ON			Load is configured in Switch Relay ON position
\retval		LOAD_MR_ITSELF			Load requested is a Switch Realy itself
\retval		LOAD_MR_TRANSITION		Master Relay is switching position

\author 	Alessandro Sabbatini
\date		Jan 31, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_IOM_FindLoadOnMasterRelay(uint8_t loadPos)
{
	uint8_t tmpRes = LOAD_MR_NOT_FOUND;
	uint8_t i;

#if defined(CONFIG_NUM_MR_LOADS)

	for (i = 0; (tmpRes == LOAD_MR_NOT_FOUND) && (i < CONFIG_NUM_MR_LOADS); i++)
	{

		if (loadPos == IOM_LoadsID_MRTypeLoads_Pos[i])
		{
			tmpRes = LOAD_MR_FOUND | LOAD_MR_ITSELF;
		}
		else if (IOM_LoadsID_MRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
		{
			uint8_t j;
			uint8_t mRelayStatus;

			for (j=0; !(tmpRes & LOAD_MR_FOUND) && (j < IOM_MRelay_Params[i].CoupleDataLen); j++)
			{
				uint8_t posON = IOM_MRelay_Params[i].pCoupleData[j].LdPos_MRelON;
				uint8_t posOFF = IOM_MRelay_Params[i].pCoupleData[j].LdPos_MRelOFF;

				mRelayStatus = LIB_IOM_GetLoadStatus(IOM_LoadsID_MRTypeLoads_Pos[i]);

				if (loadPos == posON)
				{
					tmpRes = LOAD_MR_FOUND | LOAD_MR_POS_ON;

					if (posOFF != LOAD_NOT_FOUND)
					{
						tmpRes |= LOAD_MR_CFG_COUPLE;
					}

					if (mRelayStatus == LOAD_REG_ON)
					{
						tmpRes |= LOAD_MR_SAME_POS;
					}
				}
				else if (loadPos == posOFF)
				{
					tmpRes = LOAD_MR_FOUND;

					if (posON != LOAD_NOT_FOUND)
					{
						tmpRes |= LOAD_MR_CFG_COUPLE;
					}

					if (mRelayStatus == LOAD_REG_OFF)
					{
						tmpRes |= LOAD_MR_SAME_POS;
					}
				}
				else
				{
					/* Nothing to do, go on search */
				}
			}

		}
		else
		{
			/* Nothing to do, go on search */
		}

	}

	if (tmpRes != LOAD_MR_NOT_FOUND)
	{
		if (IOM_MRelay_Params[i-1].DelayCnt != 0)
		{
			tmpRes |= LOAD_MR_TRANSITION;
		}
	}

#else
	(void) i;

#endif

	return tmpRes;
}

/** Function that looks position of Master Relay (if any) where a given load
 	is configured.

\param		[in] loadPos			Position

\return		Result is LOAD_NOT_FOUND if load is not found, otherwise master relay position

\author 	Alessandro Sabbatini
\date		Oct 16, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini

 */
uint8_t LIB_IOM_GetMRLoadPosByRelLdPos(uint8_t loadPos)
{
	uint8_t mrPos = LOAD_NOT_FOUND;
	uint8_t i;

#if defined(CONFIG_NUM_MR_LOADS)

	for (i = 0; (mrPos == LOAD_NOT_FOUND) && (i < CONFIG_NUM_MR_LOADS); i++)
	{
		uint8_t j;

		if (IOM_LoadsID_MRTypeLoads_Pos[i] != LOAD_NOT_FOUND)
		{
			for (j=0; (mrPos == LOAD_NOT_FOUND) && (j < IOM_MRelay_Params[i].CoupleDataLen); j++)
			{
				if ( (loadPos == IOM_MRelay_Params[i].pCoupleData[j].LdPos_MRelON) ||
					(loadPos == IOM_MRelay_Params[i].pCoupleData[j].LdPos_MRelOFF) )
				{
					mrPos = IOM_LoadsID_MRTypeLoads_Pos[i];
				}
			}
		}
	}
#else
	(void) i;

#endif

	return mrPos;
}


#if defined(CONFIG_NUM_SR_LOADS)

/**
\n Function that manages the Switch Relay position

\param		[in] srItem				Index of type srloads_table_t of the SR loads
\param		[in] newLoadStr			Struct of all Loads request (also Extended)

\return		none

\author 	Michele Tanzi
\date		Jan 29, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Michele Tanzi
*/
static void LIB_IOM_ManageSwitchRelay(uint8_t srItem, iom_loads_struct_t *newLoadStr)
{
	uint8_t i, posON, posOFF;
	bool_t keepStatus = FALSE;
	bool_t keepOverride = FALSE;
	bool_t srSetByLoad = FALSE;
	uint8_t curSrStatus = LIB_IOM_GetLoadStatus(IOM_LoadsID_SRTypeLoads_Pos[srItem]);
	uint8_t newSrStatus = LOAD_REG_OFF;
	uint8_t tmpNewLoadStatus;
	uint8_t tmpNewDelay;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IOM_LoadsID_SRTypeLoads_Pos);
	SAFETY_DATA_ELEM_CHECK(IOM_SwitchRelay_Params, srItem);

	/* If a safety action requires a specific position, set it */
	if (newLoadStr->loads_ovr[IOM_LoadsID_SRTypeLoads_Pos[srItem]] != LOAD_NOT_FOUND)
	{
		newSrStatus = newLoadStr->loads_ovr[IOM_LoadsID_SRTypeLoads_Pos[srItem]];
		keepOverride = TRUE;
	}
	else
	{
		for (i = 0; i < IOM_SwitchRelay_Params[srItem].CoupleDataLength; i++)
		{
			posON = IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayON;
			posOFF = IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayOFF;

			if (posOFF < N_MAXLOADS)
			{
				tmpNewLoadStatus = newLoadStr->loads_req[posOFF];

				if (newLoadStr->loads_ovr[posOFF] != LOAD_NOT_FOUND)
				{
					tmpNewLoadStatus = newLoadStr->loads_ovr[posOFF];
					keepOverride = TRUE;
				}

				if (tmpNewLoadStatus != LOAD_REG_OFF)
				{
					srSetByLoad = TRUE;
					if (curSrStatus == LOAD_REG_OFF)
					{
						keepStatus = TRUE;
					}
					else
					{
						newSrStatus = LOAD_REG_OFF;
					}
				}
			}

			if (posON < N_MAXLOADS)
			{
				tmpNewLoadStatus = newLoadStr->loads_req[posON];

				if (newLoadStr->loads_ovr[posON] != LOAD_NOT_FOUND)
				{
					tmpNewLoadStatus = newLoadStr->loads_ovr[posON];
					keepOverride = TRUE;
				}

				if (tmpNewLoadStatus != LOAD_REG_OFF)
				{
					srSetByLoad = TRUE;
					if (curSrStatus == LOAD_REG_ON)
					{
						keepStatus = TRUE;
					}
					else
					{
						newSrStatus = LOAD_REG_ON;
					}
				}
			}
		}
	}

	/* If no override is set by safety, check if application would force
	 * switch relay status regardless of loads status itself
	 */
	if (!keepOverride)
	{
		/* If Application would set Switch relay in a specific position, set it */
		if ((newLoadStr->loads_req[IOM_LoadsID_SRTypeLoads_Pos[srItem]] & LOAD_CFG_MASK) == LOAD_CFG_SR_FORCE_ON)
		{
			newSrStatus = LOAD_REG_ON;
			keepStatus = FALSE;
		}
		else if ((newLoadStr->loads_req[IOM_LoadsID_SRTypeLoads_Pos[srItem]] & LOAD_CFG_MASK) == LOAD_CFG_SR_FORCE_OFF)
		{
			newSrStatus = LOAD_REG_OFF;
			keepStatus = FALSE;
		}
		/* If no particular configuration is requested
		 * and no related loads has been set, consider switch relay request
		 */
		else if (!srSetByLoad)
		{
			newSrStatus = newLoadStr->loads_req[IOM_LoadsID_SRTypeLoads_Pos[srItem]];
		}
		else
		{
			/* Switch relay status is already set at this point.
			 * Nothing else to do */
		}
	}

	/* If keepStatus is set, keep Switch relay in current position */
	if (keepStatus)
	{
		newSrStatus = curSrStatus;
	}

	if (IOM_SwitchRelay_Params[srItem].DelayCnt == 0)
	{
		tmpNewDelay = LIB_IOM_GetSRDelayTime(srItem, newSrStatus);

		if (newSrStatus != curSrStatus)
		{
			tmpNewDelay += LIB_IOM_GetSRDelayTime(srItem, curSrStatus);

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
			/* Linked loads would require more time to complete their action
			 * Switch relay has to take care of it and perform delay AFTER its
			 * completion.
			 *
			 * Find maximum delay among all linked loads
			 * related to current switch relay
			 */
			uint8_t ldPos;
			uint16_t tmpLinkdLdDelay = 0;

			for (i = 0; i < IOM_SwitchRelay_Params[srItem].CoupleDataLength; i++)
			{
				/* Check OFF side load status */
				ldPos = IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayOFF;
				if (LIB_IOM_GetLoadStatus(ldPos) != LOAD_REG_OFF)
				{
					uint16_t tmpCurDelay = PM_LinkedLoads_GetTiming(ldPos, LOAD_REG_OFF);

					if (tmpCurDelay > tmpLinkdLdDelay)
					{
						tmpLinkdLdDelay = tmpCurDelay;
					}
				}

				/* Check ON side load status */
				ldPos = IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayON;
				if (LIB_IOM_GetLoadStatus(ldPos) != LOAD_REG_OFF)
				{
					uint16_t tmpCurDelay = PM_LinkedLoads_GetTiming(ldPos, LOAD_REG_OFF);

					if (tmpCurDelay > tmpLinkdLdDelay)
					{
						tmpLinkdLdDelay = tmpCurDelay;
					}
				}

			}

			/* Timing should be converted from 10ms to 100ms */
			tmpNewDelay += (tmpLinkdLdDelay/10) + ((tmpLinkdLdDelay % 10) ? 1 : 0);

#endif
			IOM_SwitchRelay_Params[srItem].DelayCnt = tmpNewDelay;
		}
	}
	else
	{
		IOM_SwitchRelay_Params[srItem].DelayCnt--;
	}

	/* Must be executed even when Delay is just set */
	if (IOM_SwitchRelay_Params[srItem].DelayCnt > 0 )
	{
		if (IOM_SwitchRelay_Params[srItem].DelayCnt > LIB_IOM_GetSRDelayTime(srItem, newSrStatus) )
		{
			newSrStatus = curSrStatus;
		}

		for (i = 0; i < IOM_SwitchRelay_Params[srItem].CoupleDataLength; i++)
		{
			if (IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayON != LOAD_NOT_FOUND)
			{
				newLoadStr->loads_ovr[IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayON] = LOAD_REG_OFF;
			}

			if (IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayOFF != LOAD_NOT_FOUND)
			{
				newLoadStr->loads_ovr[IOM_SwitchRelay_Params[srItem].pCoupleData[i].LoadPosition_SwitchRelayOFF] = LOAD_REG_OFF;
			}
		}
	}

	newLoadStr->loads_ovr[IOM_LoadsID_SRTypeLoads_Pos[srItem]] = newSrStatus;

	SAFETY_DATA_ELEM_UPDATE(IOM_SwitchRelay_Params, srItem);
	SAFETY_REGION_END();

}


/**
\n Function that initializes the Switch Relay parameters

\param		[in] srItem				Index of type srloads_table_t of the SR loads
\param		[in] pOutMapItem		ptr to Outputs Map of the load (read from SF, I/O configuration Data struct)

\return		none

\author 	Michele Tanzi
\date		Jan 29, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Michele Tanzi
*/
static void LIB_IOM_InitSR_Params(uint8_t srItem)
{
	Loader_Struct ldParData;

	SAFETY_REGION_START();

	/* inizializzo la struttura di output */
	IOM_SwitchRelay_Params[srItem].pCoupleData = (SwitchRHLParams_Struct *)NULL;
	IOM_SwitchRelay_Params[srItem].CoupleDataLength = 0;

	/* vengono caricati i parametri del carico relay deviatore */
	if ( LOAD_NOT_FOUND != LIB_IOM_GetLoadPilotParamTable(IOM_LoadsID_SRTypeLoads_Pos[srItem], &ldParData) )
	{
		if (ldParData.DataLenght > 2)
		{
			uint8_t *tmpData = (ldParData.Data+4);

			IOM_SwitchRelay_Params[srItem].pCoupleData = (SwitchRHLParams_Struct *)tmpData;
			IOM_SwitchRelay_Params[srItem].CoupleDataLength = (ldParData.DataLenght)-2;
			IOM_SwitchRelay_Params[srItem].DelayCnt = LIB_IOM_GetSRDelayTime(srItem, LIB_IOM_GetLoadStatus(IOM_LoadsID_SRTypeLoads_Pos[srItem]));

		}
	}

	SAFETY_DATA_ELEM_UPDATE(IOM_SwitchRelay_Params, srItem);
	SAFETY_REGION_END();

}

#endif


#if defined(CONFIG_NUM_MR_LOADS)

/**
\n Function that manages the Master Relay position

\param		[in] mrItem				Index of type srloads_table_t of the MR loads
\param		[in] newLoadStr			Struct of all Loads request (also Extended)

\return		none

\author 	Alessandro Sabbatini
\date		Jun 6, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
static void LIB_IOM_ManageMasterRelay(uint8_t mrItem, iom_loads_struct_t *newLoadStr)
{
	uint8_t i;
	uint8_t curPos = IOM_LoadsID_MRTypeLoads_Pos[mrItem];
	uint8_t curMrStatus = LIB_IOM_GetLoadStatus(curPos);
	uint8_t newMrStatus = newLoadStr->loads_req[curPos];
	uint8_t tmpNewDelay = 0;
	MRelayHLParm_Struct *pCurMrelayParms = &IOM_MRelay_Params[mrItem];
	bool_t forceMRDelay = FALSE;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IOM_LoadsID_MRTypeLoads_Pos);
	SAFETY_DATA_ELEM_CHECK(IOM_MRelay_Params, mrItem);

	/* If a safety action requires a specific position, set it */
	if (newLoadStr->loads_ovr[curPos] != LOAD_NOT_FOUND)
	{
		newMrStatus = newLoadStr->loads_ovr[curPos];

		/* LOAD_CFG_MR_FORCE_DELAY flag can be used to force MR ON/OFF
		 * and disable all related loads
		 *
		 * It can be used only on safety modules/actions!
		 */
		if (newMrStatus & LOAD_CFG_MR_FORCE_DELAY)
		{
			/* Trick to force disable all related loads */
			/* Filter LOAD_CFG_MR_FORCE_DELAY bit */
			newMrStatus &= ~LOAD_CFG_MR_FORCE_DELAY;
			forceMRDelay = TRUE;
		}

	}


	if (pCurMrelayParms->DelayCnt == 0)
	{
		if (newMrStatus != curMrStatus)
		{
			tmpNewDelay = LIB_IOM_GetMRDelayTime(mrItem, newMrStatus);
			bool_t tmpWaitOff = FALSE;

			for (i = 0; i < pCurMrelayParms->CoupleDataLen; i++)
			{
				uint8_t tmpPos;

				if ((curMrStatus & LOAD_REG_MASK) == LOAD_REG_OFF)
				{
					tmpPos = pCurMrelayParms->pCoupleData[i].LdPos_MRelOFF;
				}
				else
				{
					tmpPos = pCurMrelayParms->pCoupleData[i].LdPos_MRelON;
				}

				if ( (LIB_IOM_GetLoadStatus(tmpPos) & LOAD_REG_MASK) != LOAD_REG_OFF)
				{
					tmpWaitOff = TRUE;
				}
			}

			if (tmpWaitOff)
			{
				tmpNewDelay += LIB_IOM_GetMRDelayTime(mrItem, curMrStatus);
			}

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
			/* Linked loads would require more time to complete their action
			 * Switch relay has to take care of it and perform delay AFTER its
			 * completion.
			 *
			 * Find maximum delay among all linked loads
			 * related to current switch relay
			 */
			uint8_t ldPos;
			uint16_t tmpLinkdLdDelay = 0;

			for (i = 0; i < pCurMrelayParms->CoupleDataLen; i++)
			{
				if (curMrStatus == LOAD_REG_OFF)
				{
					ldPos = pCurMrelayParms->pCoupleData[i].LdPos_MRelOFF;
				}
				else
				{
					ldPos = pCurMrelayParms->pCoupleData[i].LdPos_MRelON;
				}

				if (LIB_IOM_GetLoadStatus(ldPos) != LOAD_REG_OFF)
				{
					uint16_t tmpCurDelay = PM_LinkedLoads_GetTiming(ldPos, LOAD_REG_OFF);

					if (tmpCurDelay > tmpLinkdLdDelay)
					{
						tmpLinkdLdDelay = tmpCurDelay;
					}
				}

			}

			/* Timing should be converted from 10ms to 100ms */
			tmpNewDelay += (tmpLinkdLdDelay/10) + ((tmpNewDelay % 10) ? 1 : 0);
#endif

			pCurMrelayParms->DelayCnt = tmpNewDelay;

		}
	}
	else
	{
		pCurMrelayParms->DelayCnt--;
	}

	for (i = 0; i < pCurMrelayParms->CoupleDataLen; i++)
	{
		uint8_t tmpMrStatus = (newMrStatus & LOAD_REG_MASK);

		if ((pCurMrelayParms->DelayCnt > 0) || forceMRDelay || (tmpMrStatus == LOAD_REG_OFF))
		{
			if (pCurMrelayParms->pCoupleData[i].LdPos_MRelON != LOAD_NOT_FOUND)
			{
				newLoadStr->loads_ovr[pCurMrelayParms->pCoupleData[i].LdPos_MRelON] = LOAD_REG_OFF;
			}
		}

		if ((pCurMrelayParms->DelayCnt > 0) || forceMRDelay || (tmpMrStatus == LOAD_REG_ON))
		{
			if (pCurMrelayParms->pCoupleData[i].LdPos_MRelOFF != LOAD_NOT_FOUND)
			{
				newLoadStr->loads_ovr[pCurMrelayParms->pCoupleData[i].LdPos_MRelOFF] = LOAD_REG_OFF;
			}
		}
	}

	if (pCurMrelayParms->DelayCnt > LIB_IOM_GetMRDelayTime(mrItem, newMrStatus))
	{
		newMrStatus = curMrStatus;
	}

	newLoadStr->loads_ovr[curPos] = newMrStatus;

	SAFETY_DATA_ELEM_UPDATE(IOM_MRelay_Params, mrItem);
	SAFETY_REGION_END();

}


/**
\n Function that initializes the Switch Relay parameters

\param		[in] mrItem				Index of type srloads_table_t of the MR loads
\param		[in] pOutMapItem		ptr to Outputs Map of the load (read from SF, I/O configuration Data struct)

\return		none

\author 	Michele Tanzi
\date		Jun 6, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
static void LIB_IOM_InitMR_Params(uint8_t mrItem)
{
	MRelayHLParm_Struct *pCurMrelayParms = &IOM_MRelay_Params[mrItem];
	Loader_Struct ldParData;

	SAFETY_REGION_START();

	/* inizializzo la struttura di output */
	pCurMrelayParms->pCoupleData = (MasterRHLParams_Struct *)NULL;
	pCurMrelayParms->CoupleDataLen = 0;

	/* vengono caricati i parametri del carico relay deviatore */
	if ( LOAD_NOT_FOUND != LIB_IOM_GetLoadPilotParamTable(IOM_LoadsID_MRTypeLoads_Pos[mrItem], &ldParData) )
	{
		if (ldParData.DataLenght > 2)
		{
			uint8_t *tmpData = (ldParData.Data+4);

			pCurMrelayParms->pCoupleData = (MasterRHLParams_Struct *)tmpData;
			pCurMrelayParms->CoupleDataLen = (ldParData.DataLenght)-2;
			pCurMrelayParms->DelayCnt = LIB_IOM_GetMRDelayTime(mrItem, LIB_IOM_GetLoadStatus(IOM_LoadsID_MRTypeLoads_Pos[mrItem]));

		}
		/* A partire dalla vers.1.2.7.0 EDT DEBUG HOTtima è possibile configurare un carico di tipo Dynamic: relay con
		 * carichi presenti su uno o entrambi i rami di uscita. Se ad esempio un Master Relay non venisse configurato
		 * come tale, funzionerebbe come un normale relay.
		 * Il warning presente di seguito ha lo scopo di segnalare il caso in cui questa opzione non venga usata in
		 * fase Editor per la compilazione del Setting File: ciò darebbe come conseguenza un campo dati di dimensione
		 * nulla. Il controllo direttamente superiore copre invece il caso in cui la struttura dati in oggetto fosse
		 * sì presente, ma con campi nulli. */
		else
		{
			LIB_Error_Warning_Handler(PLT_ERROR_MR_PARAMS_EMPTY);
		}
	}
#if defined(CONFIG_MR_CUSTOM_TABLE)
	else
	{
		uint8_t i;
		uint8_t tmpTableLen = IOM_Mrelay_CstmTableLen[mrItem];

		for (i = 0; i < (2 * tmpTableLen); i++)
		{
			IOM_Mrelay_CstmTable[mrItem][i] = LIB_IOM_FindLoadByID(IOM_Mrelay_CstmTable_LoadID[mrItem][i], NULL);
		}

		pCurMrelayParms->pCoupleData = (MasterRHLParams_Struct *)IOM_Mrelay_CstmTable[mrItem];
		pCurMrelayParms->CoupleDataLen = tmpTableLen;
		pCurMrelayParms->DelayCnt = LIB_IOM_GetMRDelayTime(mrItem, LIB_IOM_GetLoadStatus(IOM_LoadsID_MRTypeLoads_Pos[mrItem]));

	}
#endif

	SAFETY_DATA_ELEM_UPDATE(IOM_MRelay_Params, mrItem);
	SAFETY_REGION_END();

}


#endif

static void LIB_IOM_IntRefreshTimeout(void)
{
	SAFETY_DATA_CHECK(IOM_LoadsReq_Status);
	IOM_LoadsReq_Status.keyword = IOM_LDREQ_RUNNING_KEYWORD;
	IOM_LoadsReq_Status.timeout = CONFIG_LIB_LOADSREQ_TIMEOUT;
	SAFETY_DATA_UPDATE(IOM_LoadsReq_Status);

}

#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
