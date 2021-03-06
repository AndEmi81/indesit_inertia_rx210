/* file: tcb.h, automatically generated by GENH8.EXE, Version: 3.05 */
/* from: C:\PROGETTI\RTC_DEA700_2_STEP_DEV\CFG_DISHWASHER\OSGEN\OSEK\H8\DW_DEA700_H8.oil */
/* at  : Thu Sep 06 16:46:04 2012 */
/* Implementation: HitachiH8_3687Group */
/* Version of general code: 3.33a */

#include <vrm.h>
#define osdVrmGenMajRelNum 3
#define osdVrmGenMinRelNum 5
#include <vrm.h>

#if osdPreTCB
#ifndef _PRE_TCB_H
#define _PRE_TCB_H

#define osdCompiler osdHitachi

#define osdLib 0
#define CC ECC1

#define STATUS_LEVEL EXTENDED_STATUS

#define osdFullPreempt 0
#define osdMixedPreempt 1
#define osdNonPreempt 0

#define osdNumberOfAllTasks 14
#define osdNumberOfBasicTasks 13
#define osdNumberOfExtendedTasks 1
#define osdNumberOfPriorities 18
#define osdNumberOfAllResources 4
#define osdNumberOfTaskResources 4
#define osdNumberOfInterruptResources 0
#define osdNumberOfInternalResources 0
#define osdNumberOfAlarms 12
#define osdNumberOfVarAlarms 12
#define osdNumberOfVarHeapAlarms 12
#define osdNumberOfStatHeapAlarms 0
#define osdNumberOfAlarmCallback 3
#define osdNumberOfAlarmSetEvent 0
#define osdNumberOfAlarmActivateTask 9
#define osdNumberOfCounters 1
#define osdUseCounterAPI 0
#define osdStackCheck 1

#define osdSuppressFilenames 1

#define osdEnableAssertions 0

#define osdORTIDebug 0
#define osdORTIVersion 210

#define osdORTIEnabled 0


typedef uint8 EventMaskType;

#define osdEventMaskTypeSize 1

#define osdTimerHook 1
#define GetHWTimerCounterValue() (osdTCNT0)
#define SetHWTimerCompareValue(n) osdGRA0 = (uint16)(n)
#define TriggerHiResTimerISR() osdTIER0 |= osdTimerZCompareB;
#define osdHWTimerCompType uint16
#define osdHWTimerSemiCount 0x8000
#define osdHWTimerCountMask 0xffff
#define osdEnableProcedures 0

#define osdEnableContextOverlay 0

#define osdNumberOfAppModes 3

#define OSDEFAULTAPPMODE 0x01
#define NormalMode 0x02
#define ProgrammingMode 0x04

#define osdOSEK2_2 1

#define osdOSEKVersionSpec 220

#define osdGetServiceIdMacros 1
#define osdParameterAccessMacros 0

#define osdHitachiH8_3687Group
#define osdEnableIntInSysTimer 1
#define osdE7EmulatorSupport 0
#define osdAdvancedMode 24
#define osdIdleStackSize 23
#define osdSystemStackSize 140
#define osdStartupStackSize 140
#define osdUseHighResolutionTimer 1
#define osdUserDefinedSystemTimer 0 /* generated by general code now */
#define osdTimerModeRegValue 0x3
/* INFORMATION: The resolution of the HW Counter timer is 0.500000 ?s */
/* INFORMATION: The Cycle time of the HW Counter is 32.768000 ms */

/* INFORMATION: The Cycle time of the system counter is 2147.483648 s */

#define osdMinTimeBetweenTimerIrqs 0 /* 0 Ticks */
/* max(number of priority,number of tasks) */

#define osdRTSize 2

#define osdErrorHook 1
#define osdPreTaskHook 0
#define osdPostTaskHook 0
#define osdStartupHook 0
#define osdShutdownHook 0
#endif /* _PRE_TCB_H (double include preventer) */
#endif /* osdPreTCB */

/* Start of invariant part */
#if osdPostTCB
/* (POST because types have to be defined in OSEK.H) */

#if (osdRTSize > 1)
   extern osqRAM1 osqRAM2 osPrioFlagType osqRAM3 osQReadyPrios[osdRTSize];
#else
   extern osqRAM1 osqRAM2 osPrioFlagType osqRAM3 osQReadyPrios;
#endif

   extern osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbWaitEventMask[osdNumberOfExtendedTasks];



#if (CC==BCC2) || (CC==ECC2)
   #if (osdNumberOfPriorities > 0)
      extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[osdNumberOfPriorities];
      extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskTail[osdNumberOfPriorities];
      osqROM0 extern osqROM1 const osqROM2 osQEntryCountType osqROM3 oskQMaxActivations[osdNumberOfPriorities];
   #else
      extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskHead[1];
      extern osqTcbRAM1 osqTcbRAM2 osQEntryCountType osqTcbRAM3 osQReadyTaskTail[1];
      osqROM0 extern osqROM1 const osqROM2 osQEntryCountType osqROM3 oskQMaxActivations[1];
   #endif
#else
   #if (osdNumberOfPriorities > 0)
      extern osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQReadyTask[osdNumberOfPriorities];
   #else
      extern osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osQReadyTask[1];
   #endif
#endif


#if (osdNumberOfAllTasks > 0)
   #if osdMixedPreempt
     osqROM0 extern osqROM1 const osqROM2 osTaskPreemptionType osqROM3 oskTcbTaskPreemption[osdNumberOfAllTasks];
   #endif
   #if osdEnableProcedures
     osqROM0 extern osqROM1 const osqROM2 osContextOverlayType osqROM3 oskTcbContextOverlay[osdNumberOfAllTasks];
   #endif

  osqROM0 extern osqROM1 const osqROM2 osTaskFlagType osqROM3 oskTcbTaskFlags[osdNumberOfAllTasks];
#if (osdNumberOfExtendedTasks > 0) || osdLib
   extern osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbSetEventMask[osdNumberOfExtendedTasks];
#endif
#else
   #if osdMixedPreempt
      osqROM0 extern osqROM1 const osqROM2 osTaskPreemptionType osqROM3 oskTcbTaskPreemption[1];
   #endif
   #if osdEnableProcedures
      osqROM0 extern osqROM1 const osqROM2 osContextOverlayType osqROM3 oskTcbContextOverlay[1];
   #endif
   #if STATUS_LEVEL == EXTENDED_STATUS
      osqROM0 extern osqROM1 const osqROM2 osTaskFlagType osqROM3 oskTcbTaskFlags[1];
   #endif
#if (osdNumberOfExtendedTasks > 0) || osdLib
      extern osqTcbRAM1 osqTcbRAM2 EventMaskType osqTcbRAM3 osTcbSetEventMask[1];
#endif                                                 
#endif

#if ((CC == BCC2) || (CC == ECC2))
   #if (osdNumberOfPriorities > 0)
      extern osqROM0 osqPTcbRAM1 osqPTcbRAM2 TaskType osqPTcbRAM3 * osqROM1 const osqROM2 osqROM3 oskQActivationQueues[osdNumberOfPriorities];
   #else
      extern osqROM0 osqPTcbRAM1 osqPTcbRAM2 TaskType osqPTcbRAM3 * osqROM1 const osqROM2 osqROM3 oskQActivationQueues[1];
   #endif

   #if (osdNumberOfAllTasks > 0)
      extern osqTcbRAM1 osqTcbRAM2 osActivationCountType osqTcbRAM3 osTcbActivationCount[osdNumberOfAllTasks];
   #else
      extern osqTcbRAM1 osqTcbRAM2 osActivationCountType osqTcbRAM3 osTcbActivationCount[1];
   #endif
#endif

#if (osdNumberOfAllTasks > 0)
   extern osqTcbRAM1 osqTcbRAM2 osTaskStateType osqTcbRAM3 osTcbTaskState[osdNumberOfAllTasks];
   extern osStackPtrType osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 osTcbSP[osdNumberOfAllTasks];
   #if osdORTIDebug
    extern osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osTcbActualPrio[osdNumberOfAllTasks];
    extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbORTIServiceId[osdNumberOfAllTasks];
   #endif
   #if STATUS_LEVEL == EXTENDED_STATUS
    #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
     extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[osdNumberOfAllTasks];
    #endif
   #endif
   #if osdEnableContextOverlay
    extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbReturnAddressState[osdNumberOfAllTasks];
   #endif

   osqROM0 extern osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackTop[osdNumberOfAllTasks];
#if osdStackCheck
   osqROM0 extern osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackBottom[osdNumberOfAllTasks];
#endif

   osqROM0 extern osTaskFctType osqROM1 const osqROM2 osqROM3 oskTcbTaskStartAddress[osdNumberOfAllTasks];
   osqROM0 extern osqROM1 const osqROM2 osPrioType osqROM3 oskTcbHomePrio[osdNumberOfAllTasks];
   osqROM0 extern osqROM1 const osqROM2 osActivationCountType osqROM3 oskTcbAllowedActivations[osdNumberOfAllTasks];

   osqROM0 extern osqROM1 const osqROM2 osPrioFlagType osqROM3 oskTcbReadyPrioMask[osdNumberOfAllTasks];
   
   #if (osdRTSize > 1)
      osqROM0 extern osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskTcbReadyPrioOffset[osdNumberOfAllTasks];
   #endif

#else
   extern osqTcbRAM1 osqTcbRAM2 osTaskStateType osqTcbRAM3 osTcbTaskState[1];
   extern osStackPtrType osqTcbRAM1 osqTcbRAM2 osqTcbRAM3 osTcbSP[1];
   #if osdORTIDebug
   extern osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osTcbActualPrio[1];
   extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbORTIServiceId[1];
   #endif

   #if STATUS_LEVEL == EXTENDED_STATUS
   #if (osdNumberOfInternalResources > 0) || (osdNumberOfInterruptResources > 0)
   extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbResourceCounter[1];
   #endif
   #endif
   #if osdEnableContextOverlay
   extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osTcbReturnAddressState[1];
   #endif
   osqROM0 extern osTaskFctType osqROM1 const osqROM2 osqROM3 oskTcbTaskStartAddress[1];
   osqROM0 extern osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackTop[1];
#if osdStackCheck
   osqROM0 extern osStackPtrType osqROM1 const osqROM2 osqROM3 oskTcbStackBottom[1];
#endif
   osqROM0 extern osqROM1 const osqROM2 osPrioType osqROM3 oskTcbHomePrio[1];
   osqROM0 extern osqROM1 const osqROM2 osPrioFlagType osqROM3 oskTcbReadyPrioMask[1];
   osqROM0 extern osqROM1 const osqROM2 osActivationCountType osqROM3 oskTcbAllowedActivations[1];
#endif

/* resources */

#if ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed)))
#if (osdNumberOfAllResources > 0)
   extern osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osResSavedPrio[osdNumberOfAllResources];
   #if STATUS_LEVEL == EXTENDED_STATUS
      extern osqTcbRAM1 osqTcbRAM2 osResCounterType osqTcbRAM3 osResOccupationCounter[osdNumberOfAllResources];
   #endif
   #if (STATUS_LEVEL == EXTENDED_STATUS && osdEnableAssertions) || osdORTIDebug
      extern osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osResActualTask[osdNumberOfAllResources];
   #endif
   
   osqROM0 extern osqROM1 const osqROM2 osPrioType osqROM3 oskResCeilingPrio[osdNumberOfAllResources];
   #if (osdRTSize > 1)
      osqROM0 extern osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskResCeilingPrioOffset[osdNumberOfTaskResources];
   #endif
   osqROM0 extern osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResCeilingPrioMask[osdNumberOfTaskResources];
#else
   extern osqTcbRAM1 osqTcbRAM2 osPrioType osqTcbRAM3 osResSavedPrio[1];
   #if STATUS_LEVEL == EXTENDED_STATUS
      extern osqTcbRAM1 osqTcbRAM2 osResCounterType osqTcbRAM3 osResOccupationCounter[1];
   #endif
   #if (STATUS_LEVEL == EXTENDED_STATUS && osdEnableAssertions) || osdORTIDebug
      extern osqTcbRAM1 osqTcbRAM2 TaskType osqTcbRAM3 osResActualTask[1];
   #endif
   
   osqROM0 extern osqROM1 const osqROM2 osPrioType osqROM3 oskResCeilingPrio[1];
   osqROM0 extern osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResCeilingPrioMask[1];
#endif

#endif  /* ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))) */

#if (osdNumberOfInternalResources > 0)
   osqROM0 extern osqROM1 const osqROM2 osPrioType osqROM3 oskResInternalCeilingPrio[osdNumberOfAllTasks];
   osqROM0 extern osqROM1 const osqROM2 osPrioFlagType osqROM3 oskResInternalCeilingPrioMask[osdNumberOfAllTasks];
   #if (osdRTSize > 1)
      osqROM0 extern osqROM1 const osqROM2 osPrioOffsetType osqROM3 oskResInternalCeilingPrioOffset[osdNumberOfAllTasks];
   #endif
#endif
/* alarms */

#if (osdNumberOfAlarms > 0)
   extern osqTcbRAM1 osqTcbRAM2 TickType osqTcbRAM3 osAlarmTime[osdNumberOfAlarms];
   #ifndef osdUseGeneratedFastAlarm
      extern osqTcbRAM1 osqTcbRAM2 uint8 osqTcbRAM3 osAlarmActivated[osdNumberOfAlarms];
      extern osqTcbRAM1 osqTcbRAM2 osHeapType osqTcbRAM3 osAlarmHeap[osdNumberOfAlarms+1];    /* Minimum Heap */
   #endif

   #if (osdNumberOfVarAlarms > 0)
   extern osqTcbRAM1 osqTcbRAM2 TickType osqTcbRAM3 osAlarmCycleTime[osdNumberOfVarAlarms];
   #endif

   osqROM0 extern osqROM1 const osqROM2 AlarmBaseType osqROM3 oskAlarmDefinitions;

   #if (osdNumberOfAlarms > 0)   
      osqROM0 extern osqROM1 const osqROM2 TickType osqROM3 oskStaticAlarmTime[osdNumberOfAlarms];
      osqROM0 extern osqROM1 const osqROM2 TickType osqROM3 oskStaticAlarmCycleTime[osdNumberOfAlarms];
   #endif

   #ifndef osdUseGeneratedFastAlarm
      osqROM0 extern osqROM1 const osqROM2 TaskType osqROM3 oskAlarmTask[osdNumberOfAlarms];
      #if (osdNumberOfAlarmSetEvent > 0)
        osqROM0 extern osqROM1 const osqROM2 EventMaskType osqROM3 oskAlarmEvent[osdNumberOfAlarms];
      #endif
      #if (osdNumberOfAlarmSetEvent > 0) || (osdNumberOfAlarmCallback > 0)
        osqROM0 extern osqROM1 const osqROM2 uint8 osqROM3 oskAlarmAction[osdNumberOfAlarms];
      #endif
      #if (osdNumberOfAlarmCallback > 0)
        osqROM0 extern osAlarmCallbackType osqROM1 const osqROM2 osqROM3 oskAlarmCallback[osdNumberOfAlarms];
      #endif
   #endif
#endif

#ifdef osdTestMacros
   #define osTestMacrosPreTaskHook  osPreTaskHook
   #define osTestMacrosPostTaskHook osPostTaskHook
#else
#if osdLib
   #define osInterPreTaskHook  osPreTaskHook
   #define osInterPostTaskHook osPostTaskHook
#else
   #define PreTaskHook  osPreTaskHook
   #define PostTaskHook osPostTaskHook
#endif
#endif

#if osdLib
   #define osrRTSize                     oskRTSize 
   #define osrNumberOfPriorities         oskNumberOfPriorities
   #define osrNumberOfAllTasks           oskNumberOfAllTasks
   #define osrNumberOfAllResources       oskNumberOfAllResources
   #define osrNumberOfTaskResources      oskNumberOfTaskResources
   #define osrNumberOfInterruptResources oskNumberOfInterruptResources
   #define osrNumberOfInternalResources  oskNumberOfInternalResources
   #define osrNumberOfExtendedTasks      oskNumberOfExtendedTasks
   #define osrNumberOfAlarms             oskNumberOfAlarms
   #define osrNumberOfVarAlarms          oskNumberOfVarAlarms
   #define osrNumberOfVarHeapAlarms      oskNumberOfVarHeapAlarms
   #define osrNumberOfStatHeapAlarms     oskNumberOfStatHeapAlarms

   extern const osRTSizeType               oskRTSize;
   extern const osTaskIndexType            oskNumberOfAllTasks;
   extern const osTaskIndexType            oskNumberOfExtendedTasks;
   extern const osPrioType                 oskNumberOfPriorities;
   extern const osResourceIndexType        oskNumberOfAllResources;
   extern const osResourceIndexType        oskNumberOfTaskResources;
   extern const osResourceIndexType        oskNumberOfInterruptResources;
   extern const osResourceIndexType        oskNumberOfInternalResources;

   extern const osAlarmIndexType           oskNumberOfAlarms;
   extern const osAlarmIndexType           oskNumberOfVarAlarms;
   extern const osAlarmIndexType           oskNumberOfVarHeapAlarms;
   extern const osAlarmIndexType           oskNumberOfStatHeapAlarms;

   extern const TickType                   oskMaxAllowedValue;
   extern const TickType                   oskTicksPerBase;
   extern const TickType                   oskMinCycle;
   extern const uint8                      oskAlarmAutoStart[];

   extern const uint8                      oskTcbAutoStart[];


   extern const osVersionVariantCodingType oskLibVersionVariant;
   /* in library variant provide prototypes for interHook-functions */
   extern void osInterPreTaskHook(void);
   extern void osInterPostTaskHook(void);
   extern void osInterStartupHook(void);
   extern void osInterShutdownHook(StatusType Error);
   extern void osInterErrorHook(StatusType Error);

#else
   #define osrRTSize                      osdRTSize 
   #define osrNumberOfPriorities          osdNumberOfPriorities 
   #define osrNumberOfAllTasks            osdNumberOfAllTasks
   #define osrNumberOfAllResources        osdNumberOfAllResources
   #define osrNumberOfTaskResources       osdNumberOfTaskResources
   #define osrNumberOfInterruptResources  osdNumberOfInterruptResources
   #define osrNumberOfInternalResources   osdNumberOfInternalResources
   #define osrNumberOfExtendedTasks       osdNumberOfExtendedTasks
   #define osrNumberOfAlarms              osdNumberOfAlarms
   #define osrNumberOfVarAlarms           osdNumberOfVarAlarms
   #define osrNumberOfVarHeapAlarms       osdNumberOfVarHeapAlarms
   #define osrNumberOfStatHeapAlarms      osdNumberOfStatHeapAlarms
   /* in source code variant map hook routines directly to user functions */
   #define osInterStartupHook             StartupHook
   #define osInterShutdownHook            ShutdownHook
   #define osInterErrorHook               ErrorHook
#endif

extern osqStack1 osqStack2 osStackDataType osqStack3 osSystemStack[osdSystemStackSize];
extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskSystemStackTop;
extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskSystemStackBottom;

extern osqStack1 osqStack2 osStackDataType osqStack3 osIdleStack[osdIdleStackSize];
extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskIdleStackTop;
extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskIdleStackBottom;

extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskStartupStackTop;
extern osqROM0 osqROM1 const osqROM2 osStackPtrType osqROM3 oskStartupStackBottom;


#endif /* osdPostTCB */
/* End of invariant part */
#if osdPostTCB
#ifndef _POST_TCB_H
#define _POST_TCB_H

/* Tasks */
#define Task_Protocol ((TaskType)0)
#define ProgrammingMode_InitTask ((TaskType)1)
#define Task_Backup ((TaskType)2)
#define NormalMode_InitTask ((TaskType)3)
#define Task_Flash ((TaskType)4)
#define Task_MeasureLive ((TaskType)5)
#define Task_Partialization ((TaskType)6)
#define Task_10_ms ((TaskType)7)
#define Task_Feedback ((TaskType)8)
#define Task_Feedback_P ((TaskType)9)
#define Task_AnalogChannels ((TaskType)10)
#define Task_Main ((TaskType)11)
#define Task_UartParser ((TaskType)12)
#define Task_SendBytes ((TaskType)13)


/* Resources */

#define UserInterface_Res ((ResourceType)0)
#define Engine_Res ((ResourceType)1)
#define ExtBoardInfo_Res ((ResourceType)2)
#define RES_SCHEDULER ((ResourceType)3)


#if ((!defined(osdGetResourceNotUsed)) || (!defined(osdReleaseResourceNotUsed))) && osdEnableAssertions
osqROM0 extern osqROM1 const osqROM2 osResAccessMaskType osqROM3 oskResAccessMask[osdNumberOfAllResources][1];


#endif

/* Counter */

#define SystemTimer 0

#define OSMAXALLOWEDVALUE_SystemTimer OSMAXALLOWEDVALUE
#define OSTICKSPERBASE_SystemTimer OSTICKSPERBASE
#define OSMINCYCLE_SystemTimer OSMINCYCLE


/* Events */

#define EVENT_I2C_PROTOCOL ((EventMaskType)(0x01))/* AUTO */
#define EVENT_UART_PROTOCOL ((EventMaskType)(0x02))/* AUTO */
/* Alarms */

#define OSTICKDURATION 500UL

/* Maximum possible allowed value of the system counter in ticks */
#define OSMAXALLOWEDVALUE 0x3fffffffUL

/* Minimum allowed number of ticks for a cyclic alarm */
#define OSMINCYCLE 1

/* Number of ticks required to reach specific unit of the system counter */
#define OSTICKSPERBASE 1

#define USEC(x) ( ((TickType)(((((uint32)(x)) * 2UL ) + ( 1UL  / 2 )) / 1UL )))/*	valid range for x: 0..536870911	*/
#define MSEC(x) ( ((TickType)(((((uint32)(x)) * 2000UL ) + ( 1UL  / 2 )) / 1UL )))/*	valid range for x: 0..536870	*/
#define SEC(x) ( ((TickType)(((((uint32)(x)) * 2000000UL ) + ( 1UL  / 2 )) / 1UL )))/*	valid range for x: 0..536	*/
#define ALARM_10_ms ((AlarmType)0)
#define ALARM_Partialization ((AlarmType)1)
#define ALARM_SendBytes ((AlarmType)2)
#define ALARM_MeasureLive ((AlarmType)3)
#define ALARM_AnalogChannels ((AlarmType)4)
#define ALARM_BackupInit ((AlarmType)5)
#define ALARM_ConfirmBackup ((AlarmType)6)
#define ALARM_Main ((AlarmType)7)
#define ALARM_Uart_Parser ((AlarmType)8)
#define ALARM_HalfWave ((AlarmType)9)
#define ALARM_Confirm_VLINE_Backup ((AlarmType)10)
#define ALARM_Cancel_VLINE_Backup ((AlarmType)11)


#if (osdNumberOfAlarms > 0)
osqROM0 extern osqROM1 const osqROM2 uint8 osqROM3 oskAlarmAutoStart[osdNumberOfAlarms];
#endif

#if (osdNumberOfAllTasks > 0)
osqROM0 extern osqROM1 const osqROM2 uint8 osqROM3 oskTcbAutoStart[osdNumberOfAllTasks];
#endif

/* prototypes for alarm callback functions */
void CALLBACK_Backup(void);
void CALLBACK_VLINE_Backup(void);
void CALLBACK_BackupCancelVLINE(void);

/* ISRs */

#define osdIsrZC_InterruptCat 2
#define osdIsrIsr_I2CCat 2
#define osdIsrStartupCodeCat 1
#define osdIsrUartRx_InterruptCat 2
#define osdIsrIsr_Tpm2OverflowCat 2
#define osdIsrIsr_NMICat 1


/* externals for assertions */
#if osdEnableProcedures
#if osdEnableAssertions
#if osdNumberOfAllTasks > 0 
   extern osqBOOL1 osqBOOL2 osBoolArrayBaseType osqBOOL3 osIsInProcedure[osdNumberOfAllTasks];
#endif
#endif
#endif


#endif /* _POST_TCB_H (double include preventer) */
#endif /* osdPostTCB */


/* END OF C:\PROGETTI\RTC_Dea700_2_STEP_dev\CFG_DishWasher\OSgen\OSEK\H8\tcb.h */

