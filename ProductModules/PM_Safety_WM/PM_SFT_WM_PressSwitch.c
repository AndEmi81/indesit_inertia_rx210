/**
\n
\par		PM_Safety_WM_PressSwitch
\n
\n
\file		PM_SFT_WM_PressSwitch.c
\ingroup	PM_SFT_PressSwitch
\date		19/03/2013
\author		Salvatore Fimiani
\n
*/
#include "Config.h"
#include "PM_SFT_WM_PressSwitch.h"
#include "LIB_Utilities.h"
#include "LoadsRequestExp.h"
#include "PM_SFT_WM_Motor.h"
#include "LIB_ClassBVar.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Macros related to the Normative Press Switch Fault Params Table */
#define PRSWH_FAULT_TIME_OPCODE_MASK			0xF8
#define DSPL_PRSWH_FLT_PARAM					0x0Cu	/* Normative Pressure Sensor Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_PRSWH_PREF_TIME				0
#define DSPL_OFFSET_PRSWH_FAULT_TIME_OPCODE		2
#define DSPL_OFFSET_PRSWH_FAULT_TIME			3

#define DSPL_OFFSET_PRSWH_FILL_DELTA_TICK		4
#define DSPL_OFFSET_PRSWH_FILL_TIMEOUT			5

#define DSPL_OFFSET_PRSWH_DIST_DELTA_TICK		6
#define DSPL_OFFSET_PRSWH_DIST_TIMEOUT			7
#define DSPL_OFFSET_PRSWH_DIST_SPEED			8

#define DSPL_OFFSET_PRSWH_SPIN_DELTA_TICK		10
#define DSPL_OFFSET_PRSWH_SPIN_TIMEOUT			11
#define DSPL_OFFSET_PRSWH_SPIN_SPEED			12

#define DSPL_OFFSET_PRSWH_LEDGE_LEVEL			14
#define DSPL_OFFSET_PRSWH_REDGE_LEVEL			16
#define DSPL_OFFSET_PRSWH_DELTA_TICKS_AUTOZ		18
#define DSPL_OFFSET_PRSWH_WATER_SAFE_LEVEL		19

/***************************************************/

#define BAD_PARAMETERS_SF				255

#define DRUM_DIST_MODE				0
#define DRUM_SPIN_MODE				1
#define DRUM_STOPPED_MODE 			2
#define DRUM_STATUS_NOT_DEFINED		3

/* Tests if the load has just been turned on */
#define EVENT_TURNING_ON(loadForTest)		((loadForTest.status == LOAD_ON)&&(loadForTest.statusOld == LOAD_OFF))
/* Tests if the load has just been turned off */
#define EVENT_TURNING_OFF(loadForTest)		((loadForTest.status == LOAD_OFF)&&(loadForTest.statusOld == LOAD_ON))
/* Tests if the status has changed from the previous */
#define EVENT_STATUS_CHANGED(loadForTest)		(loadForTest.newStatus != loadForTest.oldStatus)

#define SYNCTIME_VALUE		10


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct
{
	uint8_t  currVal;			/* Current alert source type 											*/
	uint8_t  lastVal;			/* Previous alert source type 											*/
}AlertType_t;


typedef struct
{

	uint8_t  fillDeltaTick;		/**< Delta Ticks in water filling 							 	*/
	uint8_t  distDeltaTick;		/**< Delta Ticks for drain water during the distribution cycle	*/
	uint8_t  spinDeltaTick;		/**< Delta Ticks for drain water during the spin cycle			*/
	uint8_t  deltaTicksAutoz;	/**< Ticks number to be added to the autozero value 				*/
	uint8_t  waterSafeLevel;	/**< Minimum safety level for water in drum 						*/
	uint16_t faultTime;			/**< Timeout to the fault for out of the plausibility range 		*/
	uint16_t fillTimeout;		/**< Timeout in water filling (100ms)								*/
	uint16_t distTimeout;		/**< Timeout for drain water during the distribution cycle (100ms)*/
	uint16_t spinTimeout;		/**< Timeout for drain water during the spin cycle (100ms)		*/
	uint16_t spinSpeed;			/**< Minimum speed during the spin cycle 							*/
	uint16_t distSpeed;			/**< Maximum speed during the distribution cycle 					*/
	uint16_t prefaultTime;		/**< Timeout to the fault for out of the plausibility range 		*/
	uint16_t leftEdgeEmptyLev;	/**< Min possible ticks number for the empty level 				*/
	uint16_t rightEdgeEmptyLev; /**< Max possible ticks number for the empty level 				*/



}PressSwitchSafety_t;

typedef struct
{
	LPS_ValueType currVal;			/* Current value acquired by the press switch 							*/
	LPS_ValueType oldVal;			/* Previous value acquired by the press switch 							*/
	LPS_ValueType maxVal;			/* Max value acquired till now 											*/
	LPS_ValueType minVal;			/* Min value acquired till now 											*/
}PressSwitchSample_t;



/* ********* */
/* Variables */
/* ********* */


/* Global */

/** Variables to be inserted into Backup
 @{ */
CalibStatusInfo_t prsCalibStatus;
prsZeroRefInfo_t prsZeroRefInfo = {0xFFFF, 0};
/* @}*/

uint16_t prsGeneralCnt = 0;  		/* General counter for measuring how long an anomaly on the press switch lasts	*/


uint8_t drainPumpStatus = PRSWH_PUMP_BLOCKED;

static bool_t isDrainPumpTestOK = FALSE;

/* Counter for synchronizing the operations every SYNCTIME_VALUE x 100 ms */
static uint8_t syncCnt = 0;

LoadInfos_t prsDoorInfos = {LOAD_NOT_FOUND,LOAD_REG_OFF,LOAD_REG_OFF};  		/* Info about the Door Lock load 			 */
LoadInfos_t prsEVWashInfos = {LOAD_NOT_FOUND,LOAD_REG_OFF,LOAD_REG_OFF};		/* Info about the Wash Electrovalve load	 */
LoadInfos_t prsEVPrewashInfos = {LOAD_NOT_FOUND,LOAD_REG_OFF,LOAD_REG_OFF};		/* Info about the Pre-wash Electrovalve load */
LoadInfos_t prsEVHotwashInfos = {LOAD_NOT_FOUND,LOAD_REG_OFF,LOAD_REG_OFF};		/* Info about the Hot-wash Electrovalve load */
LoadInfos_t prsDrainPumpInfos = {LOAD_NOT_FOUND,LOAD_REG_OFF,LOAD_REG_OFF};		/* Info about the Drain Pump load 			 */
uint8_t prsWashResPos =  LOAD_NOT_FOUND;





PressSwitchSafety_t prsFltParams; /**< Fault parameters */
PressSwitchSample_t prsSwitchSample = {0,0,0,0xFFFF};
AlertType_t prsAlertType;

struct
{
	uint8_t pos;
	uint8_t pilotStatus;
	uint8_t newStatus;
	uint8_t oldStatus;
	DrumSpeedType speed;
}prsDrumInfos = {LOAD_NOT_FOUND, LOAD_REG_OFF, DRUM_STATUS_NOT_DEFINED,DRUM_STATUS_NOT_DEFINED,0};




/* Local */
static bool_t prsWaterFillTestEN = FALSE;			/* Flag for enabling/disabling the press switch test during the water filling */
static bool_t wtrFillTestInPrgr = FALSE;
static int8_t fillTestRes = TEST_STOPPED;

static bool_t prsWaterDrainTestEN = FALSE;
static bool_t wtrDrainTestInPrgr = FALSE;

static uint8_t prsEVWashAppCmd = LOAD_OFF;
static uint8_t prsEVPrewashAppCmd = LOAD_OFF;
static uint8_t prsEVHotwashAppCmd = LOAD_OFF;

static uint8_t protFrom = ALERTSRC_PRSWH_NONE;
static uint16_t prefResetTimer = GEN_FLT_RESET_TIMER_VALUE;

/* ******************* */
/* Functions Prototype */
/* ******************* */

static bool_t PM_SFT_WM_PressSwitch_GetZeroRef(LPS_ValueType *zeroValue);
/* static bool_t PM_SFT_WM_PressSwitch_EvOff(iom_loads_struct_t *newLoads); */

/* Exported function */

/**
\n This function initializes the status of the pressure switch

\param
\return

\author		Salvatore Fimiani
\date
\version

\note

*/

void PM_SFT_WM_PressSwitch_Init(sft_module_status_t *pressSwitchMod)
{
	/* Pointer to the tab of the Press Switch Fault Parameters */
	struct Loader prsSwitchFltParTabPtr;
	uint8_t err = 0;
	bool_t bTemp = FALSE;


	prsDoorInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);
	prsEVWashInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_EV_POS);
	prsEVPrewashInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_PRE_WASH_EV_POS);
	prsEVHotwashInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_HOT_WASH_EV_POS);
	prsDrainPumpInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);
	prsDrumInfos.pos = LIB_IOM_GetLoadPos((uint8_t) LD_ENGINE_POS);
	prsWashResPos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_RES_POS);

	bTemp = (prsDoorInfos.pos == LOAD_NOT_FOUND) || (prsEVWashInfos.pos == LOAD_NOT_FOUND)||(prsEVPrewashInfos.pos == LOAD_NOT_FOUND )||
			(prsDrainPumpInfos.pos == LOAD_NOT_FOUND) || (prsDrumInfos.pos == LOAD_NOT_FOUND) || (prsWashResPos == LOAD_NOT_FOUND);
	if(bTemp == TRUE)
	{
		err = BAD_PARAMETERS_SF;
	}
	SAFETY_REGION_START();


	/* Get a pointer to the Probe Fault Parameters Tab */
	if ( 0 != BasicLoader(SF_PTR_FAULT, DSPL_PRSWH_FLT_PARAM, &prsSwitchFltParTabPtr) )
	{
		err = BAD_PARAMETERS_SF;
	}
	else
	{
		/* Initialization of the fault parameters */
		prsFltParams.prefaultTime = EndianU16_BToN(*((uint16_t *) (prsSwitchFltParTabPtr.Data)));
		if ((*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_FAULT_TIME_OPCODE)& PRSWH_FAULT_TIME_OPCODE_MASK) == 0x18)
		{
			prsFltParams.faultTime = (uint16_t)(*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_FAULT_TIME)) * 10;
		}
		else
		{
			prsFltParams.faultTime = (uint16_t)(*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_FAULT_TIME));
		}
		prsFltParams.faultTime	= (uint16_t)BASETIME_1S_TO_100MS(prsFltParams.faultTime);


		prsFltParams.fillDeltaTick = OFFS_B(prsSwitchFltParTabPtr.Data, DSPL_OFFSET_PRSWH_FILL_DELTA_TICK);
		prsFltParams.fillTimeout = (uint16_t)BASETIME_10S_TO_100MS(*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_FILL_TIMEOUT));

		prsFltParams.distDeltaTick = OFFS_B(prsSwitchFltParTabPtr.Data, DSPL_OFFSET_PRSWH_DIST_DELTA_TICK);
		prsFltParams.distTimeout = (uint16_t)BASETIME_10S_TO_100MS(*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_DIST_TIMEOUT));
		prsFltParams.distSpeed 	= EndianU16_BToN(*((uint16_t *) (prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_DIST_SPEED)));

		prsFltParams.spinDeltaTick = OFFS_B(prsSwitchFltParTabPtr.Data, DSPL_OFFSET_PRSWH_SPIN_DELTA_TICK);
		prsFltParams.spinTimeout = (uint16_t)BASETIME_10S_TO_100MS(*(prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_SPIN_TIMEOUT));
		prsFltParams.spinSpeed	= EndianU16_BToN(*((uint16_t *) (prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_SPIN_SPEED)));

		prsFltParams.leftEdgeEmptyLev = EndianU16_BToN(*((uint16_t *) (prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_LEDGE_LEVEL)));
		prsFltParams.rightEdgeEmptyLev = EndianU16_BToN(*((uint16_t *) (prsSwitchFltParTabPtr.Data+DSPL_OFFSET_PRSWH_REDGE_LEVEL)));

		prsFltParams.deltaTicksAutoz = OFFS_B(prsSwitchFltParTabPtr.Data,DSPL_OFFSET_PRSWH_DELTA_TICKS_AUTOZ);
		prsFltParams.waterSafeLevel = OFFS_B(prsSwitchFltParTabPtr.Data,DSPL_OFFSET_PRSWH_WATER_SAFE_LEVEL);

	}

	if (err == BAD_PARAMETERS_SF)
	{
		pressSwitchMod->status = SFT_MODULE_FAULT;
		pressSwitchMod->faultSrc = ALERTSRC_PRSWH_BAD_SETTING;
	}
	else
	{
		pressSwitchMod->status = SFT_MODULE_IDLE;
		pressSwitchMod->faultSrc = SFT_NO_FAULT_SRC;

		prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
	}

	prsCalibStatus.zeroRefLev = 0xFFFF;
	prsCalibStatus.cycleInProgr = FALSE;
	prsCalibStatus.isWashResEnabled = FALSE;
	prsCalibStatus.calibOK = FALSE;


/*	SAFETY_DATA_UPDATE(prsZeroRefInfo);*/
	SAFETY_DATA_UPDATE(prsCalibStatus);
	SAFETY_DATA_UPDATE(prsFltParams);
	SAFETY_REGION_END();

}



/**
\n This function implements the state machine which manages safety of the Pressure
\n Switch module.
\n
\n
\param[in,out] newLoads			Info related to loads status request
\param[in,out] prsSwitchMod		Info related to the safety status of this module
\param[in] 	   applReqFlags		Flags of requests by the application


\return void
\retval	N.A.

\par Architecture Requirements ID:
\li			SARS_WMMT_MN_SPRS_2
\n\li 		SARS_WMMT_MN_SPRS_3
\n\li		SARS_WMMT_MN_SPRS_4
\n\li		SARS_WMMT_MN_SPRS_5
\n\li		SARS_WMMT_MN_SPRS_6
\n\li		SARS_WMMT_MN_SPRS_7
\n\li		SARS_WMMT_MN_SPRS_8
\n\li		SARS_WMMT_MN_SPRS_9

*/
/*
\n\author	Salvatore Fimiani
\n\date		19/03/2013
\n\version	0.1
\n
\n
\n
*/
void PM_SFT_WM_PressSwitch_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *prsSwitchMod, uint8_t applReqFlags)
{

	LIB_SFT_EnterSafetyModule(prsSwitchMod);

	/* Checking for integrity of the fault parameters */
	SAFETY_REGION_START();

/*	SAFETY_DATA_CHECK(prsZeroRefInfo);*/
	SAFETY_DATA_CHECK(prsCalibStatus);
	SAFETY_DATA_CHECK(prsFltParams);

	/* Acquiring the loads' status and application requests */
	prsDoorInfos.status = LoadsStatus[prsDoorInfos.pos];
	prsEVWashInfos.status = LoadsStatus[prsEVWashInfos.pos];
	prsEVPrewashInfos.status = LoadsStatus[prsEVPrewashInfos.pos];
	if(prsEVHotwashInfos.pos != LOAD_NOT_FOUND)
	{
		prsEVHotwashInfos.status = LoadsStatus[prsEVHotwashInfos.pos];
		prsEVHotwashAppCmd = newLoads->loads_req[prsEVHotwashInfos.pos];
	}
	else
	{
		prsEVHotwashInfos.status = LOAD_OFF;
		prsEVHotwashAppCmd = LOAD_OFF;
	}
	prsDrainPumpInfos.status = LoadsStatus[prsDrainPumpInfos.pos];
	prsDrumInfos.pilotStatus = LoadsStatus[prsDrumInfos.pos];

	prsEVWashAppCmd = newLoads->loads_req[prsEVWashInfos.pos];
	prsEVPrewashAppCmd = newLoads->loads_req[prsEVPrewashInfos.pos];


	/* Checking the drum speed */
	if(PM_SFT_WM_DrumSpeedIsZero())
	{
		prsDrumInfos.newStatus = DRUM_STOPPED_MODE;
	}
	else if(PM_SFT_WM_GetDrumSpeed(&prsDrumInfos.speed))
	{
		prsDrumInfos.newStatus = DRUM_STATUS_NOT_DEFINED;
	}
	else if(prsDrumInfos.speed < prsFltParams.distSpeed)
	{
		prsDrumInfos.newStatus = DRUM_DIST_MODE;
	}
	else if(prsDrumInfos.speed > prsFltParams.spinSpeed)
	{
		prsDrumInfos.newStatus = DRUM_SPIN_MODE;
	}





	/* Only if the door is closed make checks */
	if(goodZCSignal() == TRUE)
	{
		/* Update the pressure level every SYNCTIME_VALUE ms (default 1000ms = 1sec) */
		if(syncCnt == 0)
		{
			/* Reading of LPS value */
			if(!PM_LPS_GetFaultFlag())
			{
				prsSwitchSample.currVal = PM_LPS_GetPSValue();
				/* If there is not an other alert source active */
				if(prsAlertType.currVal == ALERTSRC_PRSWH_OUT_OF_RANGE)
				{
					prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
				}
			}
			else
			{
				prsAlertType.currVal = ALERTSRC_PRSWH_OUT_OF_RANGE;
			}
		}



		/* Reads the empty value in order to calibrate the pressure switch */
		if(prsAlertType.currVal != ALERTSRC_PRSWH_OUT_OF_RANGE)
		{
			if(PM_SFT_WM_PressSwitch_GetZeroRef(&prsCalibStatus.zeroRefLev) == TRUE)
			{
				if((prsCalibStatus.zeroRefLev > prsFltParams.leftEdgeEmptyLev)&&
						(prsCalibStatus.zeroRefLev < prsFltParams.rightEdgeEmptyLev))
				{
					prsCalibStatus.cycleInProgr = TRUE;
					prsCalibStatus.calibOK = TRUE;
					prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
				}
				else
				{
					prsCalibStatus.calibOK = FALSE;
					prsAlertType.currVal = ALERTSRC_PRSWH_BAD_EMPTYLEV;
				}
			}
		}


		/* All checks are made only if the pressure switch is calibrated */
		if(prsCalibStatus.calibOK == TRUE)
		{
			/**************************************************************************
			 * Checks if there are all of conditions to enable the Pressure Switch test
			 * during the water filling
			 ***************************************************************************/
			/* If at least one of the electrovalves is ON */
			if((prsEVWashInfos.status == LOAD_ON)||(prsEVPrewashInfos.status == LOAD_ON)||(prsEVHotwashInfos.status == LOAD_ON))
			{
				/* Only if the motor is standstill */
				if(prsDrumInfos.pilotStatus == LOAD_REG_OFF)
				{
					if((prsAlertType.currVal != ALERTSRC_PRSWH_OUT_OF_RANGE) &&
							(prsAlertType.currVal != ALERTSRC_PRSWH_FILL_BLOCKED))
					{
						prsWaterFillTestEN = TRUE;
					}
					else
					{
						prsWaterFillTestEN = FALSE;
					}
				}
				else
				{
					prsWaterFillTestEN = FALSE;
				}
			}
			else /* All of the electrovalves are OFF */
			{
				prsWaterFillTestEN = FALSE;
			}


			/* Checks the conditions to enable the Pressure Switch test during the water drain */
			if(prsDrainPumpInfos.status == LOAD_ON)
			{
				if((prsAlertType.currVal != ALERTSRC_PRSWH_OUT_OF_RANGE)/*&&
						(prsDrumInfos.newStatus != DRUM_STATUS_NOT_DEFINED)*/)
				{
					prsWaterDrainTestEN = TRUE;
				}
				else
				{
					prsWaterDrainTestEN = FALSE;
				}
			}
			else
			{
				prsWaterDrainTestEN = FALSE;
			}


	/***************************************************************************
	 *  		Tests to see if the pressure switch is blocked during the water filling.
	 *  		This test is executed only if the conditions below exist:
	 *
	 *  			1)	At least one electrovalve turned on
	 *  			2)	Motor stopped
	 *  			3)	Pressure switch calibrated
	 *  			4)	Pressure switch values in range
	 *
	 ***************************************************************************/
			if(prsWaterFillTestEN == TRUE)
			{
				/* Test enabled but not yet in progress */
				if(!wtrFillTestInPrgr)
				{
					prsSwitchSample.minVal = prsSwitchSample.currVal;
					prsGeneralCnt = prsFltParams.fillTimeout;
					wtrFillTestInPrgr = TRUE;
					if(fillTestRes != TEST_OK)
					{
						fillTestRes = TEST_IN_PROGR;
					}
				}
				else /* If the test is in progress */
				{
					/* If the timeout is not expired */
					if(prsGeneralCnt > 0)
					{
						if((prsSwitchSample.currVal - prsSwitchSample.minVal) >= prsFltParams.fillDeltaTick)
						{
							wtrFillTestInPrgr = FALSE;  /* Stops the current test */
							fillTestRes = TEST_OK;
							prsSwitchSample.minVal = 0xFFFF;
							if((prsAlertType.currVal == ALERTSRC_PRSWH_FILL_BLOCKED)||
									(prsAlertType.currVal == ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF) ||
											(prsAlertType.currVal == ALERTSRC_PRSWH_DRAIN_BLOCKED))
							{
								prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
							}
						}
					}
					else
					{
						wtrFillTestInPrgr = FALSE;
						fillTestRes = TEST_FAILED;
						prsSwitchSample.minVal = 0xFFFF;
						prsAlertType.currVal = ALERTSRC_PRSWH_FILL_BLOCKED;
					}
				}
			}
			else /* Test not enabled */
			{
				/* Checks if there was a test in progress */
				if(wtrFillTestInPrgr == TRUE)
				{
					wtrFillTestInPrgr = FALSE;
					fillTestRes = TEST_STOPPED;
				}

			}

	/****************** End of the test ********************************************/



	/**************************************************************************************
	 *  		Test to see if the pressure switch is blocked during the water drain.
	 *  		This test is executed only if it exist the conditions below:
	 *
	 *  			1)	Pressure switch calibrated
	 *  			2)	Pressure switch values in range
	 *  			3)	Motor speed defined
	 *
	 **************************************************************************************/
			if(prsWaterDrainTestEN == TRUE)
			{
				uint8_t deltaTicksTh = 0 ;
				uint16_t timeoutTh = 0;

				if(prsDrumInfos.newStatus == DRUM_DIST_MODE || (prsDrumInfos.newStatus == DRUM_STOPPED_MODE) ||
											(prsDrumInfos.newStatus == DRUM_STATUS_NOT_DEFINED))
				{
					deltaTicksTh = prsFltParams.distDeltaTick;
					timeoutTh = prsFltParams.distTimeout;
				}
				else if (prsDrumInfos.newStatus == DRUM_SPIN_MODE)
				{
					deltaTicksTh = prsFltParams.spinDeltaTick;
					timeoutTh = prsFltParams.spinTimeout;
				}

				if(prsSwitchSample.currVal < (prsCalibStatus.zeroRefLev + prsFltParams.deltaTicksAutoz))
				{
					/* When all the drum has been drained the washing resistor is disabled till the
					 * next filling of water */
					prsCalibStatus.isWashResEnabled = FALSE;
				}
				else
				{
					/* The test is armed only when the pressure switch value is equal or greater than
					 * (prsCalibStatus.zeroRefLev + prsFltParams.deltaTicksAutoz) */
					if((!wtrDrainTestInPrgr) || EVENT_STATUS_CHANGED(prsDrumInfos))
					{
						prsSwitchSample.maxVal = prsSwitchSample.currVal;
						prsGeneralCnt = timeoutTh;
						wtrDrainTestInPrgr = TRUE;

						/* Information below all relates to the blocked pump algorithm */
						isDrainPumpTestOK = FALSE; /* At any timer re-trigger the pump rotor is supposed blocked */

					}
				}
				/* If the pump rotor was free, it becomes BLOCKED after 1 sec.
				 * Do not touch this! */
				if(syncCnt == 0)
				{
					drainPumpStatus = PRSWH_PUMP_BLOCKED;
				}


				/* ************************************************
				 * Below is the test to verify if the pump is draining
				 *
				 * ************************************************/
				if(wtrDrainTestInPrgr == TRUE)
				{
					if(prsGeneralCnt > 0)
					{/* If the timeout is not expired */
						/* Tracking the maximum has been added for intercepting  level changes of short time during the spin */
						prsSwitchSample.maxVal = MAX(prsSwitchSample.currVal, prsSwitchSample.maxVal);

						if((prsSwitchSample.maxVal-prsSwitchSample.currVal) >= deltaTicksTh)
						{
							wtrDrainTestInPrgr = FALSE;
							prsSwitchSample.maxVal = prsCalibStatus.zeroRefLev;
							if(prsAlertType.currVal == ALERTSRC_PRSWH_DRAIN_BLOCKED)
							{
								prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
							}

							isDrainPumpTestOK = TRUE;
							drainPumpStatus = PRSWH_PUMP_FREE;
						}
						else if(prsSwitchSample.maxVal == prsSwitchSample.currVal)
						{/* If max is different from the previous max value, reset every time the timer  */
							prsGeneralCnt = timeoutTh;
						}
					}
					else
					{
						wtrDrainTestInPrgr = FALSE;
						prsSwitchSample.maxVal = prsCalibStatus.zeroRefLev;
						/* The protection 5.05 has to be raised only if it is not yet a protection 5.09 raised */
						if(prsAlertType.currVal != ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF)
						{
							prsAlertType.currVal = ALERTSRC_PRSWH_DRAIN_BLOCKED;
						}

						isDrainPumpTestOK = FALSE;
					}
				}
				else
				{
					isDrainPumpTestOK = FALSE;
				}
			}
			else
			{
				wtrDrainTestInPrgr = FALSE;
				isDrainPumpTestOK = FALSE;	/* When the drain pump is switched off, the the test result flag is FALSE  */
			}
	/****************** End of the test **************************************************/

	/**************************************************************************************
	 * 				Monitoring algorithm for safety in normal operating mode
	 *************************************************************************************/

			/* This control is made only if there are not other alert sources */
			if((prsAlertType.currVal == ALERTSRC_PRSWH_NONE)||(prsAlertType.currVal == ALERTSRC_PRSWH_LITTLE_WATER)||
					(prsAlertType.currVal == ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF))
			{
				/* Checks if the water level in the drum is enough to enable the washing resistor */
				if(prsSwitchSample.currVal < (prsCalibStatus.zeroRefLev + prsFltParams.deltaTicksAutoz))
				{
					prsAlertType.currVal = ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF;
				}
				else if(prsSwitchSample.currVal < (prsCalibStatus.zeroRefLev + prsFltParams.waterSafeLevel))
				{
					prsAlertType.currVal = ALERTSRC_PRSWH_LITTLE_WATER;
				}
				else
				{
					/* If at least one of the electrovalves is ON */
					if((prsEVWashInfos.status == LOAD_ON)||(prsEVPrewashInfos.status == LOAD_ON)||
							(prsEVHotwashInfos.status == LOAD_ON))
					{
						prsCalibStatus.isWashResEnabled = TRUE;
					}
					/* The protection for LITTLE WATER is reset only if the Water Safe Level has
					 * been exceeded at least once with at least an EV open */
					if(prsCalibStatus.isWashResEnabled == TRUE)
					{
						prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
					}
					else
					{
						prsAlertType.currVal = ALERTSRC_PRSWH_LITTLE_WATER;
					}


				}
			}

		} /* If pressure switch is not calibrated */
		else if((prsAlertType.currVal != ALERTSRC_PRSWH_BAD_EMPTYLEV) && (prsAlertType.currVal != ALERTSRC_PRSWH_OUT_OF_RANGE ))
		{
			prsAlertType.currVal = ALERTSRC_PRSWH_TO_BE_CALIBRATED;
		}

		/******************************* End of the Algorithm **************************/

	}
	else /* If the door is open */
	{
		/* When the door is open, if the module status is not equals to FAULT, all alarms are reset */
		if(prsSwitchMod->status != SFT_MODULE_FAULT)
		{
			prsSwitchMod->status = SFT_MODULE_IDLE;
			prsSwitchMod->faultSrc = SFT_NO_FAULT_SRC;
			prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
		}


		/* The door could be open because the WM is in stand by. If it is not so, the calibration gets reset  */
		if(prsCalibStatus.cycleInProgr == FALSE)
		{
			prsCalibStatus.zeroRefLev = 0xFFFF;
			prsCalibStatus.calibOK = FALSE;
		}
	}



	/* State machine for the press switch's faults */
	switch (prsSwitchMod->status)
	{
		case SFT_MODULE_RESET:
			prsAlertType.currVal = ALERTSRC_PRSWH_NONE;
			protFrom = ALERTSRC_PRSWH_NONE;
			prsSwitchMod->faultSrc = SFT_NO_FAULT_SRC;
			prsSwitchMod->status = SFT_MODULE_IDLE;

			drainPumpStatus = PRSWH_PUMP_BLOCKED;
			isDrainPumpTestOK = FALSE;
			break;

		case SFT_MODULE_IDLE:
			if(prsAlertType.currVal != ALERTSRC_PRSWH_NONE)
			{
				prsSwitchMod->status = SFT_MODULE_MONITORING;
			}
			break;

		case SFT_MODULE_MONITORING:
			if(prsAlertType.currVal == ALERTSRC_PRSWH_OUT_OF_RANGE )
			{
				/* Set the timer with the timeout to prefault */
				if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
				{
					/* Only if in auto test mode */
					prsGeneralCnt = SFT_TEST_PREFAULT_TIMEOUT_100MS;
				}
				else
				{
					/* Only in normal mode */
					prsGeneralCnt = prsFltParams.prefaultTime;
				}

				prsSwitchMod->status = SFT_MODULE_WARNING;
			}
			else if(prsAlertType.currVal != ALERTSRC_PRSWH_NONE)
			{
				prsSwitchMod->status = SFT_MODULE_PROTECTION;
			}
			else
			{
				prsSwitchMod->status = SFT_MODULE_RESET;
			}
			protFrom = prsAlertType.currVal;
			prsSwitchMod->faultSrc = prsAlertType.currVal;

			break;

		case SFT_MODULE_WARNING:
			newLoads->loads_ovr[prsWashResPos] = LOAD_REG_OFF;
			if(prsAlertType.currVal == ALERTSRC_PRSWH_NONE)
			{
				prsSwitchMod->status = SFT_MODULE_RESET;
			}
			else if(prsAlertType.currVal != protFrom)
			{
				prsSwitchMod->status = SFT_MODULE_MONITORING;
			}
			else if(protFrom == ALERTSRC_PRSWH_OUT_OF_RANGE )
			{
				drainPumpStatus = PRSWH_PUMP_BLOCKED;
				if(prsGeneralCnt == 0)
				{
					if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
					{
						/* Only if in auto test mode */
						prsGeneralCnt = SFT_TEST_FAULT_TIMEOUT_100MS;
					}
					else
					{
						/* Only in normal mode */
						prsGeneralCnt = prsFltParams.faultTime;
					}
					prsSwitchMod->status = SFT_MODULE_PREFAULT;
				}
				prefResetTimer = GEN_FLT_RESET_TIMER_VALUE;
			}
		/*	prsSwitchMod->faultSrc = prsAlertType.currVal;*/
			break;

		case SFT_MODULE_PROTECTION:
			newLoads->loads_ovr[prsWashResPos] = LOAD_REG_OFF;

			if((protFrom == ALERTSRC_PRSWH_TO_BE_CALIBRATED)||(protFrom == ALERTSRC_PRSWH_BAD_EMPTYLEV)||
					(protFrom == ALERTSRC_PRSWH_DRAIN_BLOCKED)||(protFrom == ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF))
			{
				if(prsAlertType.currVal == protFrom)
				{
					/* The pump rotor status is updated every SYNCTIME_VALUE x 100ms */
					if(syncCnt == 0)
					{
						if(protFrom == ALERTSRC_PRSWH_DRAIN_BLKD_AT_ZEROREF)
						{
							if(isDrainPumpTestOK == TRUE)
							{
								drainPumpStatus = PRSWH_PUMP_FREE;
							}
							else
							{
								drainPumpStatus = PRSWH_PUMP_BLOCKED;
							}
						}
						else
						{
							drainPumpStatus = PRSWH_PUMP_BLOCKED;
						}
					} /*end syncCnt */
				}
			}

			if(protFrom == ALERTSRC_PRSWH_FILL_BLOCKED)
			{
				if((prsEVWashAppCmd == LOAD_OFF)&&(prsEVPrewashAppCmd == LOAD_OFF)&&(prsEVHotwashAppCmd == LOAD_OFF))
				{
					fillTestRes = TEST_STOPPED;
					prsSwitchMod->status = SFT_MODULE_RESET;
				}
			}
			else if(prsAlertType.currVal == ALERTSRC_PRSWH_NONE)
			{
				prsSwitchMod->status = SFT_MODULE_RESET;
			}
			else if(prsAlertType.currVal != protFrom)
			{
				prsSwitchMod->status = SFT_MODULE_MONITORING;
			}

			break;

		case SFT_MODULE_PREFAULT:
			newLoads->loads_ovr[prsWashResPos] = LOAD_REG_OFF;
			if(prsAlertType.currVal != protFrom)
			{
				if(prefResetTimer == 0)
				{
					if(prsAlertType.currVal == ALERTSRC_PRSWH_NONE)
					{
						prsSwitchMod->status = SFT_MODULE_RESET;
					}
					else
					{
						prsSwitchMod->status = SFT_MODULE_MONITORING;
					}
				}
				else
				{
					prefResetTimer--;
				}
			}
			else
			{
				drainPumpStatus = PRSWH_PUMP_BLOCKED;
				prefResetTimer = GEN_FLT_RESET_TIMER_VALUE;
				if(prsGeneralCnt == 0)
				{
					prsSwitchMod->status = SFT_MODULE_FAULT;
				}
			}
		/*	prsSwitchMod->faultSrc = prsAlertType.currVal;*/
			break;

		case SFT_MODULE_FAULT:
			/* Handle reset fault request */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				prsSwitchMod->status = SFT_MODULE_RESET;
			}
			drainPumpStatus = PRSWH_PUMP_BLOCKED;
			break;

		default:
			break;
	}


	/* Internal Timer */
	if(prsGeneralCnt > 0)
	{
		prsGeneralCnt--;
	}

	/* This counter is reset every SYNCTIME_VALUE x 100ms (default 1s) */
	if(syncCnt > 0)
	{
		syncCnt--;
	}
	else
	{
		syncCnt = SYNCTIME_VALUE;
	}


	prsDoorInfos.statusOld = prsDoorInfos.status;
	prsEVWashInfos.statusOld = prsEVWashInfos.status;
	prsEVPrewashInfos.statusOld = prsEVPrewashInfos.status;
	prsEVHotwashInfos.statusOld = prsEVHotwashInfos.status;
	prsDrumInfos.oldStatus = prsDrumInfos.newStatus;
	prsAlertType.lastVal = prsAlertType.currVal;

/*	SAFETY_DATA_UPDATE(prsZeroRefInfo);*/
	SAFETY_DATA_UPDATE(prsCalibStatus);
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(prsSwitchMod);

}




void PM_SFT_WM_PrsSwitch_SetZeroRef(LPS_ValueType zeroValue)
{
	prsZeroRefInfo.val = zeroValue;
	prsZeroRefInfo.ready = TRUE;
}

/* Local Function */

uint8_t PM_SFT_WM_PrsSwitch_GetFillTestStatus(void)
{
	return fillTestRes;
}



static bool_t PM_SFT_WM_PressSwitch_GetZeroRef(LPS_ValueType *zeroValue)
{
	bool_t ret;

	ret = (prsZeroRefInfo.ready == 1)?TRUE:FALSE;
	if(ret==TRUE)
	{
		*zeroValue = prsZeroRefInfo.val;
		prsZeroRefInfo.ready = 0;
		prsZeroRefInfo.val = 0xFFFF;
	}

	return ret;
}


void PM_SFT_WM_PrsSwitch_ClearCycleInProgrFlag(void)
{
	if (LIB_SFT_GetFaultSourceStatus(SFT_FLT_SV_NO_SETTING) != SFT_MODULE_FAULT)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(prsCalibStatus);
		prsCalibStatus.cycleInProgr = FALSE;
		SAFETY_DATA_UPDATE(prsCalibStatus);
		SAFETY_REGION_END();
	}

}


#if 0
static bool_t PM_SFT_WM_PressSwitch_EvOff(iom_loads_struct_t *newLoads)
{

	newLoads->loads_ovr[prsEVWashInfos.pos] = LOAD_OFF;
	if(prsEVPrewashInfos.pos != LOAD_NOT_FOUND)
	{
		newLoads->loads_ovr[prsEVPrewashInfos.pos] = LOAD_OFF;

	}
	if(prsEVHotwashInfos.pos != LOAD_NOT_FOUND)
	{
		newLoads->loads_ovr[prsEVHotwashInfos.pos] = LOAD_OFF;

	}

	return TRUE;
}
#endif

uint16_t PM_SFT_WM_PressSwitch_GetTime(sft_module_status_t *status)
{
	return prsGeneralCnt;
}



