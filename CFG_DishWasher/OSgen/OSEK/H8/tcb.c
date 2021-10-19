/* file: tcb.c, automatically generated by GENH8.EXE, Version: 3.05 */
/* from: C:\PROGETTI\RTC_DEA700_2_STEP_DEV\CFG_DISHWASHER\OSGEN\OSEK\H8\DW_DEA700_H8.oil */
/* at  : Thu Sep 06 16:46:04 2012 */
/* Implementation: HitachiH8_3687Group */
/* Version of general code: 3.33a */

#include <vrm.h>
#define osdVrmGenMajRelNum 3
#define osdVrmGenMinRelNum 5
#include <vrm.h>

/* Start of invariant part */
#include <osek.h>
#include <osekext.h>

   /* structure for standard counter (derived from system clock) */
   osqROM0 osqROM1 const osqROM2 AlarmBaseType osqROM3 oskAlarmDefinitions=
   {
      OSMAXALLOWEDVALUE,
      OSTICKSPERBASE,
      OSMINCYCLE
   };

   osqTcbRAM1 osqTcbRAM2 TickType osqTcbRAM3 osAlarmTime[osdNumberOfAlarms];
   #ifndef osdUseGeneratedFastAlarm
      osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osAlarmActivated[osdNumberOfAlarms];
      osqTcbRAM1 osqTcbRAM2 osHeapType osqTcbRAM3 osAlarmHeap[osdNumberOfAlarms+1];    /* Minimum Heap */
   #endif

   #if (osdNumberOfVarAlarms > 0)
   osqTcbRAM1 osqTcbRAM2 TickType osqTcbRAM3 osAlarmCycleTime[osdNumberOfVarAlarms];
   #endif

#if (CC==BCC2) || (CC==ECC2)
   #if (osdNumberOfPriorities > 0)
      osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[osdNumberOfPriorities];
      osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskTail[osdNumberOfPriorities];
   #else
      osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[1];
      osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskTail[1];
   #endif
#else
   #if (osdNumberOfPriorities > 0)
      osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQReadyTask[osdNumberOfPriorities];
   #else
      osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQReadyTask[1];
   #endif
#endif

#if ((CC == BCC2) || (CC == ECC2))
   #if (osdNumberOfAllTasks > 0)
      osqTcbRAM1 osqTcbRAM2 osActivationCountType osqTcbRAM3 osTcbActivationCount[osdNumberOfAllTasks];
   #else
      osqTcbRAM1 osqTcbRAM2 osActivationCountType osqTcbRAM3 osTcbActivationCount[1];
   #endif
#endif

#if (osdRTSize > 1)
   osqRAM1 osqRAM2 osPrioFlagType osqRAM3 osQReadyPrios[osdRTSize];
#else
   osqRAM1 osqRAM2 osPrioFlagType osqRAM3 osQReadyPrios;
#endif

#if (osdNumberOfAllTasks > 0)
   osqTcbRAM1 osqTcbRAM2 osTaskStateType osqTcbRAM3 osTcbTaskState[osdNumberOfAllTasks];
   osStackPtrType osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 osTcbSP[osdNumberOfAllTasks];
   #if osdORTIDebug
   osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osTcbActualPrio[osdNumberOfAllTasks];
   osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbORTIServiceId[osdNumberOfAllTasks];
   #endif
   #if STATUS_LEVEL == EXTENDED_STATUS
    #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
     osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[osdNumberOfAllTasks];
    #else
     #if osdLib
      osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[osdNumberOfAllTasks];
     #endif
    #endif
   #endif
   #if osdEnableContextOverlay
   osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbReturnAddressState[osdNumberOfAllTasks];
   #endif
#else
   osqTcbRAM1 osqTcbRAM2 osTaskStateType osqTcbRAM3 osTcbTaskState[1];
   osStackPtrType osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 osTcbSP[1];
   #if osdORTIDebug
   osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osTcbActualPrio[1];
   osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbORTIServiceId[1];
   #endif
   #if STATUS_LEVEL == EXTENDED_STATUS
   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[1];
    #else
     #if osdLib
      osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[1];
     #endif
   #endif
   #endif
   #if osdEnableContextOverlay
   osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbReturnAddressState[1];
   #endif
#endif


   osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osResSavedPrio[osdNumberOfAllResources];
   #if STATUS_LEVEL == EXTENDED_STATUS
      osqTcbRAM1 osqTcbRAM2 osResCounterType osqTcbRAM3 osResOccupationCounter[osdNumberOfAllResources];
   #endif
   #if (STATUS_LEVEL == EXTENDED_STATUS && osdEnableAssertions) || osdORTIDebug
      osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osResActualTask[osdNumberOfAllResources];
   #endif


   osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbWaitEventMask[osdNumberOfExtendedTasks];

#if (osdNumberOfExtendedTasks > 0) || osdLib
   #if (osdNumberOfExtendedTasks == 0) && osdLib
   osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbSetEventMask[1];
   #else
   osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbSetEventMask[osdNumberOfExtendedTasks];
   #endif
#endif

#if osdLib
   osqROM0 osqROM1 const osqROM2 osRTSizeType    osqROM3 oskRTSize= osdRTSize;
   osqROM0 osqROM1 const osqROM2 osTaskIndexType osqROM3 oskNumberOfAllTasks=      osdNumberOfAllTasks;
   osqROM0 osqROM1 const osqROM2 osTaskIndexType osqROM3 oskNumberOfExtendedTasks= osdNumberOfExtendedTasks;
   osqROM0 osqROM1 const osqROM2 osPrioType      osqROM3 oskNumberOfPriorities=    osdNumberOfPriorities;

   osqROM0 osqROM1 const osqROM2 osResourceIndexType osqROM3 oskNumberOfAllResources= osdNumberOfAllResources;
   osqROM0 osqROM1 const osqROM2 osResourceIndexType osqROM3 oskNumberOfTaskResources= osdNumberOfTaskResources;
   osqROM0 osqROM1 const osqROM2 osResourceIndexType osqROM3 oskNumberOfInterruptResources= osdNumberOfInterruptResources;
   osqROM0 osqROM1 const osqROM2 osResourceIndexType osqROM3 oskNumberOfInternalResources= osdNumberOfInternalResources;

   osqROM0 osqROM1 const osqROM2 osAlarmIndexType osqROM3 oskNumberOfAlarms=         osdNumberOfAlarms;
   osqROM0 osqROM1 const osqROM2 osAlarmIndexType osqROM3 oskNumberOfVarAlarms=      osdNumberOfVarAlarms;
   osqROM0 osqROM1 const osqROM2 osAlarmIndexType osqROM3 oskNumberOfVarHeapAlarms=  osdNumberOfVarHeapAlarms;
   osqROM0 osqROM1 const osqROM2 osAlarmIndexType osqROM3 oskNumberOfStatHeapAlarms= osdNumberOfStatHeapAlarms;
   
   osqROM0 osqROM1 const osqROM2 TickType         osqROM3 oskMaxAllowedValue=        OSMAXALLOWEDVALUE;
   osqROM0 osqROM1 const osqROM2 TickType         osqROM3 oskMinCycle=               OSMINCYCLE;
   osqROM0 osqROM1 const osqROM2 TickType         osqROM3 oskTicksPerBase=           OSTICKSPERBASE;

#endif
/* End of invariant part */
/* taskstacks */


#pragma noregsave ProgrammingMode_InitTaskfunc
#pragma noregsave Task_Backupfunc
#pragma noregsave NormalMode_InitTaskfunc
#pragma noregsave Task_Flashfunc
#pragma noregsave Task_MeasureLivefunc
#pragma noregsave Task_Partializationfunc
#pragma noregsave Task_10_msfunc
#pragma noregsave Task_Feedbackfunc
#pragma noregsave Task_Feedback_Pfunc
#pragma noregsave Task_AnalogChannelsfunc
#pragma noregsave Task_Mainfunc
#pragma noregsave Task_Protocolfunc
#pragma noregsave Task_UartParserfunc
#pragma noregsave Task_SendBytesfunc

#pragma section Stack
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack6 [95];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack0 [100];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack1 [100];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack2 [85];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack3 [85];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack4 [85];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack5 [165];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack7 [75];
osqStack1 osqStack2 osStackDataType osqStack3 osTaskStack8 [50];
#pragma section

osqROM0 osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackTop[osdNumberOfAllTasks]=
{
   osAssignStack(osTaskStack6),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack1),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack2),
   osAssignStack(osTaskStack3),
   osAssignStack(osTaskStack4),
   osAssignStack(osTaskStack0),
   osAssignStack(osTaskStack5),
   osAssignStack(osTaskStack7),
   osAssignStack(osTaskStack8),
};

#if osdStackCheck
osqROM0 osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackBottom[osdNumberOfAllTasks]=
{
   osBottomOfStack(osTaskStack6),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack1),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack2),
   osBottomOfStack(osTaskStack3),
   osBottomOfStack(osTaskStack4),
   osBottomOfStack(osTaskStack0),
   osBottomOfStack(osTaskStack5),
   osBottomOfStack(osTaskStack7),
   osBottomOfStack(osTaskStack8),
};

#endif

#pragma section Stack
osqStack1 osqStack2 osStackDataType osqStack3 osSystemStack[osdSystemStackSize];
#pragma section
osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskSystemStackTop = osAssignStack(osSystemStack);
osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskSystemStackBottom = osBottomOfStack(osSystemStack);

#pragma section Stack
osqStack1 osqStack2 osStackDataType osqStack3 osIdleStack[osdIdleStackSize];
#pragma section
osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskIdleStackTop = osAssignStack(osIdleStack);
osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskIdleStackBottom = osBottomOfStack(osIdleStack);

osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskStartupStackTop = osAssignStack(osTaskStack5);
osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskStartupStackBottom = osBottomOfStack(osTaskStack5);


#if (CC == BCC2) || (CC == ECC2)
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_0[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_1[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_2[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_3[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_4[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_5[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_6[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_7[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_8[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_9[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_10[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_11[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_12[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_13[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_14[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_15[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_16[2];
osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQTaskActivation_17[2];

osqROM0 osqPTcbRAM1 osqPTcbRAM2 TaskType osqPTcbRAM3 * osqROM1 const osqROM2 osqROM3 oskQActivationQueues[osdNumberOfPriorities]=
{
   osQTaskActivation_0,
   osQTaskActivation_1,
   osQTaskActivation_2,
   osQTaskActivation_3,
   osQTaskActivation_4,
   osQTaskActivation_5,
   osQTaskActivation_6,
   osQTaskActivation_7,
   osQTaskActivation_8,
   osQTaskActivation_9,
   osQTaskActivation_10,
   osQTaskActivation_11,
   osQTaskActivation_12,
   osQTaskActivation_13,
   osQTaskActivation_14,
   osQTaskActivation_15,
   osQTaskActivation_16,
   osQTaskActivation_17,
};

osqROM0 osqROM1 const osqROM2 osQEntryCountType osqROM3 oskQMaxActivations[osdNumberOfPriorities]=
{
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
};

#endif


/* task start functions (only for call tree) */
#if defined osdCPlusPlus
   extern "C"{
#endif
extern void ProgrammingMode_InitTaskfunc(void);
extern void Task_Backupfunc(void);
extern void NormalMode_InitTaskfunc(void);
extern void Task_Flashfunc(void);
extern void Task_MeasureLivefunc(void);
extern void Task_Partializationfunc(void);
extern void Task_10_msfunc(void);
extern void Task_Feedbackfunc(void);
extern void Task_Feedback_Pfunc(void);
extern void Task_AnalogChannelsfunc(void);
extern void Task_Mainfunc(void);
extern void Task_Protocolfunc(void);
extern void Task_UartParserfunc(void);
extern void Task_SendBytesfunc(void);
#if defined osdCPlusPlus
   }
#endif

osqROM0 osTaskFctType osqROM1 const osqROM2 osqROM3 oskTcbTaskStartAddress[osdNumberOfAllTasks] = {
   Task_Protocolfunc,
   ProgrammingMode_InitTaskfunc,
   Task_Backupfunc,
   NormalMode_InitTaskfunc,
   Task_Flashfunc,
   Task_MeasureLivefunc,
   Task_Partializationfunc,
   Task_10_msfunc,
   Task_Feedbackfunc,
   Task_Feedback_Pfunc,
   Task_AnalogChannelsfunc,
   Task_Mainfunc,
   Task_UartParserfunc,
   Task_SendBytesfunc,
};

#if osdMixedPreempt
osqROM0 osqROM1 const osqROM2 osTaskPreemptionType osqROM3 oskTcbTaskPreemption[osdNumberOfAllTasks] = {
   osdTaskTypePreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypePreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypePreempt,
   osdTaskTypePreempt,
   osdTaskTypePreempt,
   osdTaskTypeNonPreempt,
   osdTaskTypePreempt,
   osdTaskTypePreempt,
   osdTaskTypePreempt,
};
#endif /* osdMixedPreempt */

osqROM0 osqROM1 const osqROM2 osTaskFlagType osqROM3 oskTcbTaskFlags[osdNumberOfAllTasks] = {
 osdTaskFlagScheduleAllowed,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagNotUsingSchedule,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagScheduleAllowed,
 osdTaskFlagScheduleAllowed,
};

osqROM0 osqROM1 const osqROM2 uint8 osqROM3 oskTcbAutoStart[osdNumberOfAllTasks] = {
   0x00, /* Task_Protocol */
   0x04, /* ProgrammingMode_InitTask*/
   0x00, /* Task_Backup */
   0x02, /* NormalMode_InitTask*/
   0x00, /* Task_Flash */
   0x00, /* Task_MeasureLive */
   0x00, /* Task_Partialization */
   0x00, /* Task_10_ms */
   0x00, /* Task_Feedback */
   0x00, /* Task_Feedback_P */
   0x00, /* Task_AnalogChannels */
   0x00, /* Task_Main */
   0x00, /* Task_UartParser */
   0x00, /* Task_SendBytes */
};

osqROM0 osqROM1 const osqROM2 osPrioFlagType osqROM3 oskTcbReadyPrioMask[osdNumberOfAllTasks] = {
   0x0001,
   0x4000,
   0x1000,
   0x0800,
   0x0400,
   0x0200,
   0x0100,
   0x0080,
   0x0040,
   0x0020,
   0x0010,
   0x0002,
   0x8000,
   0x4000,
};

#if (osdRTSize > 1)
osqROM0 osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskTcbReadyPrioOffset[osdNumberOfAllTasks] = {
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00,
   0x01,
   0x01,
};
#endif

osqROM0 osqROM1 const osqROM2 osPrioType osqROM3 oskTcbHomePrio[osdNumberOfAllTasks]=
{
   15,
   1,
   3,
   4,
   5,
   6,
   7,
   8,
   9,
   10,
   11,
   14,
   16,
   17,
};

#if CC == BCC2 || CC == ECC2
osqROM0 osqROM1 const osqROM2 osActivationCountType osqROM3 oskTcbAllowedActivations[osdNumberOfAllTasks]=
{
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
};
#endif

#if osdEnableProcedures
osqROM0 osqROM1 const osqROM2 osContextOverlayType osqROM3 oskTcbContextOverlay[osdNumberOfAllTasks] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
};
#endif /* osdEnableProcedures */

/* Resources */

#if ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))
#if (STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions
osqROM0 osqROM1 const osqROM2 osResAccessMaskType osqROM3 oskResAccessMask[osdNumberOfAllResources][1]={
/* sequence of taskNumbers (!not priorities) */
   {    0x8010,   } /* UserInterface_Res*/
  ,{    0xa010,   } /* Engine_Res*/
  ,{    0x8010,   } /* ExtBoardInfo_Res*/
  ,{    0xfffc,   } /* RES_SCHEDULER*/
};

#endif /* (STATUS_LEVEL == EXTENDED_STATUS) && osdEnableAssertions */

osqROM0 osqROM1 const osqROM2 osPrioType osqROM3 oskResCeilingPrio[osdNumberOfAllResources]=
{
   (osPrioType)13,
   (osPrioType)2,
   (osPrioType)12,
   (osPrioType)0,
};

osqROM0 osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResCeilingPrioMask[osdNumberOfTaskResources]=
{
   0x0004,
   0x2000,
   0x0008,
   0x8000,
};

#if (osdRTSize > 1)
osqROM0 osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskResCeilingPrioOffset[osdNumberOfTaskResources]=
{
   0x00,
   0x00,
   0x00,
   0x00,
};
#endif

#endif /*((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))) */

#if (osdNumberOfAllTasks>0)
#if ((osdNumberOfInternalResources > 0)||(osdLib==1))
osqROM0 osqROM1 const osqROM2 osPrioType osqROM3 oskResInternalCeilingPrio[osdNumberOfAllTasks]=
{
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
   (osPrioType) 0,
};

osqROM0 osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResInternalCeilingPrioMask[osdNumberOfAllTasks]=
{
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
};

 #if ((osdRTSize > 1)||(osdLib==1))
osqROM0 osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskResInternalCeilingPrioOffset[osdNumberOfAllTasks]=
{
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
};

#endif
#endif

#else
osqROM0 osqROM1 const osqROM2 osPrioType osqROM3 oskResInternalCeilingPrio[1]={0};
osqROM0 osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResInternalCeilingPrioMask[1]={0};
osqROM0 osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskResInternalCeilingPrioOffset[1]={0};
#endif
/* Counters */

/* Alarms */

#if (osdNumberOfAlarmActivateTask > 0) || (osdNumberOfAlarmSetEvent > 0)
/* task to activate or to event */
osqROM0 osqROM1 const osqROM2 TaskType osqROM3 oskAlarmTask[osdNumberOfAlarms]={
   Task_10_ms, /* ALARM_10_ms*/
   Task_Partialization, /* ALARM_Partialization*/
   Task_SendBytes, /* ALARM_SendBytes*/
   Task_MeasureLive, /* ALARM_MeasureLive*/
   Task_AnalogChannels, /* ALARM_AnalogChannels*/
   Task_Backup, /* ALARM_BackupInit*/
   0, /* 0 for ALARMCALLBACK */
   Task_Main, /* ALARM_Main*/
   Task_UartParser, /* ALARM_Uart_Parser*/
   Task_Feedback, /* ALARM_HalfWave*/
   0, /* 0 for ALARMCALLBACK */
   0, /* 0 for ALARMCALLBACK */
};
#else
 #if osdLib
 osqROM0 osqROM1 const osqROM2 TaskType osqROM3 oskAlarmTask[1]={0};
 #endif
#endif

#if osdNumberOfAlarmSetEvent > 0
osqROM0 osqROM1 const osqROM2 EventMaskType osqROM3 oskAlarmEvent[osdNumberOfAlarms]={
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
   0, /* 0 for ActivateTask or ALARMCALLBACK */
};
#else
 #if osdLib
 osqROM0 osqROM1 const osqROM2 EventMaskType osqROM3 oskAlarmEvent[1]={0};
 #endif
#endif

#if osdNumberOfAlarmCallback > 0
/* alarm callback function */
osqROM0 osAlarmCallbackType osqROM1 const osqROM2 osqROM3 oskAlarmCallback[osdNumberOfAlarms]={
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   CALLBACK_Backup, /* ALARM_ConfirmBackup*/
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   (osAlarmCallbackType) 0, /* 0 for ActivateTask or SetEvent */
   CALLBACK_VLINE_Backup, /* ALARM_Confirm_VLINE_Backup*/
   CALLBACK_BackupCancelVLINE, /* ALARM_Cancel_VLINE_Backup*/
};
#else
 #if osdLib
 osqROM0 osAlarmCallbackType osqROM1 const osqROM2 osqROM3 oskAlarmCallback[1]={(osAlarmCallbackType)0};
 #endif
#endif

#if (osdNumberOfAlarmCallback > 0) || (osdNumberOfAlarmSetEvent > 0)
osqROM0 osqROM1 const osqROM2 uint8 osqROM3 oskAlarmAction[osdNumberOfAlarms]={
   osdAlarmActionActivateTask, /* ALARM_10_ms*/
   osdAlarmActionActivateTask, /* ALARM_Partialization*/
   osdAlarmActionActivateTask, /* ALARM_SendBytes*/
   osdAlarmActionActivateTask, /* ALARM_MeasureLive*/
   osdAlarmActionActivateTask, /* ALARM_AnalogChannels*/
   osdAlarmActionActivateTask, /* ALARM_BackupInit*/
   osdAlarmActionCallback, /* ALARM_ConfirmBackup*/
   osdAlarmActionActivateTask, /* ALARM_Main*/
   osdAlarmActionActivateTask, /* ALARM_Uart_Parser*/
   osdAlarmActionActivateTask, /* ALARM_HalfWave*/
   osdAlarmActionCallback, /* ALARM_Confirm_VLINE_Backup*/
   osdAlarmActionCallback, /* ALARM_Cancel_VLINE_Backup*/
};
#else
 #if (osdNumberOfAlarms == 0) || osdLib
  osqROM0 osqROM1 const osqROM2 uint8 osqROM3 oskAlarmAction[1]={0};
 #endif
#endif

/* static alarm time */
osqROM0 osqROM1 const osqROM2 TickType osqROM3 oskStaticAlarmTime[osdNumberOfAlarms]={
   0, /* no static alarm: ALARM_10_ms*/
   0, /* no static alarm: ALARM_Partialization*/
   0, /* no static alarm: ALARM_SendBytes*/
   0, /* no static alarm: ALARM_MeasureLive*/
   0, /* no static alarm: ALARM_AnalogChannels*/
   0, /* no static alarm: ALARM_BackupInit*/
   0, /* no static alarm: ALARM_ConfirmBackup*/
   0, /* no static alarm: ALARM_Main*/
   0, /* no static alarm: ALARM_Uart_Parser*/
   0, /* no static alarm: ALARM_HalfWave*/
   0, /* no static alarm: ALARM_Confirm_VLINE_Backup*/
   0, /* no static alarm: ALARM_Cancel_VLINE_Backup*/
};

/* static cycle time */
osqROM0 osqROM1 const osqROM2 TickType osqROM3 oskStaticAlarmCycleTime[osdNumberOfAlarms]={
   0, /* no static alarm: ALARM_10_ms */
   0, /* no static alarm: ALARM_Partialization */
   0, /* no static alarm: ALARM_SendBytes */
   0, /* no static alarm: ALARM_MeasureLive */
   0, /* no static alarm: ALARM_AnalogChannels */
   0, /* no static alarm: ALARM_BackupInit */
   0, /* no static alarm: ALARM_ConfirmBackup */
   0, /* no static alarm: ALARM_Main */
   0, /* no static alarm: ALARM_Uart_Parser */
   0, /* no static alarm: ALARM_HalfWave */
   0, /* no static alarm: ALARM_Confirm_VLINE_Backup */
   0, /* no static alarm: ALARM_Cancel_VLINE_Backup */
};

/* Autostart */
osqROM0 osqROM1 const osqROM2 uint8 osqROM3 oskAlarmAutoStart[osdNumberOfAlarms]={
   0x00, /* ALARM_10_ms */
   0x00, /* ALARM_Partialization */
   0x00, /* ALARM_SendBytes */
   0x00, /* ALARM_MeasureLive */
   0x00, /* ALARM_AnalogChannels */
   0x00, /* ALARM_BackupInit */
   0x00, /* ALARM_ConfirmBackup */
   0x00, /* ALARM_Main */
   0x00, /* ALARM_Uart_Parser */
   0x00, /* ALARM_HalfWave */
   0x00, /* ALARM_Confirm_VLINE_Backup */
   0x00, /* ALARM_Cancel_VLINE_Backup */
};



void osInitTimer(void)
{
   osdTCNT0 = 0;                              /* start at time 0 */
   osdTCR0  = osdTimerModeRegValue;           /* clock selection; TCNT clearing selection -> counter not cleared (free running) */
   osdGRA0  = 10;                             /* very short time because a first interrupt is needed; this will care for MinTimerInterval-Init */
   osdGRB0  = 10;                             /* very short time because a first interrupt is needed */
   osdTSR0 &=~(osdTimerZCompareA|osdTimerZCompareB);           /* clear interrupt request */
   osdTIER0|= (osdTimerZCompareA|osdTimerZCompareB);           /* enable interrupt source */
   osdTSTR |=  osdTimerZStart0;              /* start timer */
} /* END OF osInitTimer */



ISR(osTimerInterrupt)
{
   /* compare interrupt */
#if osdTimerHook
   if (osdTSR0 & (osdTimerZCompareC|osdTimerZCompareD|osdTimerZOverflow) & osdTIER0)
   {
      TimerHook();
      return; /*alarm handling - if any - takes place during next interrupt*/
   }
#endif /*osdTimerHook*/
#if (0 == osdMinTimeBetweenTimerIrqs)
      osGetSystemCounter();		/*update OS time*/
#else  /*(osdMinTimeBetweenTimerIrqs > 0)*/
      osNextAllowedTimerInt = osGetSystemCounter()+osdMinTimeBetweenTimerIrqs;
#endif /*(osdMinTimeBetweenTimerIrqs > 0)*/
   osdTSR0  &= ~osdTimerZCompareA;  /* clear timer interrupt request */
   osWorkAlarms();
   osdTIER0 &= ~osdTimerZCompareB;  /* clear auxiliary trigger (in case it was set) */
} /* END OF osTimerInterrupt */

/* END OF C:\PROGETTI\RTC_Dea700_2_STEP_dev\CFG_DishWasher\OSgen\OSEK\H8\tcb.c */

