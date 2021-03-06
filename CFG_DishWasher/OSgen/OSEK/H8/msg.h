/* file: msg.h, automatically generated by GENH8.EXE, Version: 3.05 */
/* from: C:\PROGETTI\RTC_DEA700_2_STEP_DEV\CFG_DISHWASHER\OSGEN\OSEK\H8\DW_DEA700_H8.oil */
/* at  : Thu Sep 06 16:46:04 2012 */
/* Implementation: HitachiH8_3687Group */
/* Version of general code: 3.33a */

#include <vrm.h>
#define osdVrmGenMajRelNum 3
#define osdVrmGenMinRelNum 5
#include <vrm.h>

#ifndef __MSG_1__
#define __MSG_1__

/* first part - included at the beginning of osekcom.h */

/* conformance class */
#define osdCCCA 1
#define osdCCCB 2
#define osdCCC osdCCCB

/* number of all messages */
#define osdNumberOfMsgs 1

/* number of all unqueued messages */
#define osdNumberOfMsgsUnQueuedMsgs 1

/* number of all queued messages */
#define osdNumberOfMsgsQueuedMsgs 0

/* number of all queues with size >= 1 */
#define osdNumberOfMsgsQueues 0

/* number of all receiving tasks */
#define osdNumberOfMsgsRcvTasks 1

/* number of all notifications */
#define osdNumberOfMsgsNotifications 1

/* number of callback notifications */
#define osdNumberOfMsgsCallbackNotifics 0

/* number of flag notifications */
#define osdNumberOfMsgsFlagNotifics 1

/* usage of message resource */
#define osdUseMessageResource 1

/* usage of message status */
#define osdUseMessageStatus 1

#ifndef osdAsmFile

/* user defined structures for messages */
#if osdNumberOfMsgs > 0
#include <umsgtype.h>
#else
extern uint8 ucComDummy;
#endif

/* collection of all user defined structures */
typedef union
{
	PC_CONTROL_PARAM PC_CONTROL_PARAM_obj;
} DataType;

/* a reference to the user defined data will be used */
typedef DataType* DataRefType;

#endif

#else
#ifndef __MSG_2__
#define __MSG_2__
/* second part - included at the end of osekcom.h */

/* externals for messages */
osqROM0 extern osqROM1 const osqROM2 osComMsg osqROM3 RemoteLoadsControl;
osqROM0 extern osqROM1 const osqROM2 osComMsgRom osqROM3 oskMsgRom[osdNumberOfMsgs];

/* externals for unqueued user data (needed for "WithoutCopy") */
extern PC_CONTROL_PARAM RemoteLoadsControlUsrData;

/* access names - global buffers for copies */

/* access names - direct access to message object */
#define RemoteLoadsControl_RxBuffer RemoteLoadsControlUsrData
#define RemoteLoadsControl_TxBuffer RemoteLoadsControlUsrData

/* prototypes for callback functions */

/* defines for flag access */
#define REMOTE_LOADS_CONTROL 0
#if osdNumberOfMsgsFlagNotifics
extern osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[osdNumberOfMsgsFlagNotifics];
#endif

#endif

#endif

#ifndef osrNumberOfMsgsFlagNotifics /*double include preventer*/
# if osdLib
#  define osrNumberOfMsgsFlagNotifics oskNumberOfMsgsFlagNotifics
extern const uint16 oskNumberOfMsgsFlagNotifics;
# else
#  define osrNumberOfMsgsFlagNotifics osdNumberOfMsgsFlagNotifics
# endif
#endif

/* END OF C:\PROGETTI\RTC_Dea700_2_STEP_dev\CFG_DishWasher\OSgen\OSEK\H8\msg.h */

