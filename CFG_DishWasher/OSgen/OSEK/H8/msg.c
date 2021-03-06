/* file: msg.c, automatically generated by GENH8.EXE, Version: 3.05 */
/* from: C:\PROGETTI\RTC_DEA700_2_STEP_DEV\CFG_DISHWASHER\OSGEN\OSEK\H8\DW_DEA700_H8.oil */
/* at  : Thu Sep 06 16:46:04 2012 */
/* Implementation: HitachiH8_3687Group */
/* Version of general code: 3.33a */

#include <vrm.h>
#define osdVrmGenMajRelNum 3
#define osdVrmGenMinRelNum 5
#include <vrm.h>

#include "osekcom.h"

#if osdLib
osqROM0 osqROM1 const osqROM2 uint16 osqROM3 oskNumberOfMsgsFlagNotifics= osdNumberOfMsgsFlagNotifics;
#endif
#if osdNumberOfMsgsFlagNotifics
osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[osdNumberOfMsgsFlagNotifics];
#else
   #if osdLib
   osqTcbRAM1 osqTcbRAM2 FlagType osqTcbRAM3 osComFlags[1];
   #endif
#endif
/* unqueued user data */
PC_CONTROL_PARAM RemoteLoadsControlUsrData;

/* queued user data */

/* queues with size >= 1 */


/* access names - global buffers for copies */


/* receiving tasks */

osqROM0 osqROM1 const osqROM2 osComTaskPtr osqROM3 oskRcvTasks[osdNumberOfMsgsRcvTasks] =
{
{
	Task_Main, /* task */
	1 /* WithoutCopy flag */
},
};

/* notifications */

osqROM0 osqROM1 const osqROM2 osComNotification osqROM3 oskNotifications[osdNumberOfMsgsNotifications] =
{
{
	osdCOM_ACTION_FLAG,  /* action */
	0,  /* task to activate or post event to */
	0,  /* event */
#if osdNumberOfMsgsCallbackNotifics
	(osComCbFct) 0,  /* callback */
#endif
#if osdNumberOfMsgsFlagNotifics
	REMOTE_LOADS_CONTROL,  /* flag */
#endif
},
};

/* generic RAM part */
#if osdUseMessageResource
osqTcbRAM1 osqTcbRAM2 osComMsgRamGeneric osqTcbRAM3 osMsgRamGeneric[osdNumberOfMsgs];
#endif

/* ROM parts of all messages */

osqROM0 osqROM1 const osqROM2 osComMsgRom osqROM3 oskMsgRom[osdNumberOfMsgs] =
{
{
	sizeof(PC_CONTROL_PARAM),  /* message length */
	0,  /* fifo size - also used to distinguisch queued and unqueued msgs */
	1,  /* number of receivers */
	#if osdUseMessageResource
	&osMsgRamGeneric[0],  /* pointer to generic RAM part */
	#endif
	(osqPROM1 osqPROM2 osComMsgQueue osqPROM3*)(&RemoteLoadsControlUsrData),  /* pointer to data part - message can either be queued or unqueued */
	#if STATUS_LEVEL == EXTENDED_STATUS
	osdMAGIC_MSG,  /* magic number */
	#endif
	&oskRcvTasks[0], /* pointer to list of receiving tasks */
	&oskNotifications[0],  /* pointer to list of notifications */
	1,  /* number of notifications */
	1  /* send message without copying data */
},
};

/* references to the messages used */

osqROM0 osqROM1 const osqROM2 osComMsg osqROM3 RemoteLoadsControl = (osComMsg)(&(oskMsgRom[0]));

#if osdNumberOfMsgs > 0

osqFunc1 void osqFunc2 osInternalStartCOM(void)
{
   /* some auxiliary variables */
   #if osdUseMessageResource
   int i;

   #endif
   /* initialise unqueued messages */
   #if osdUseMessageResource
   for(i=0;i<osdNumberOfMsgs;i++)
   {
      osMsgRamGeneric[i].resource=0;
   }

   #endif
}

#if osdCCC==osdCCCB
#if osdUseMessageResource
osqFunc1 uint8 osqFunc2 __ComResourcesAreTaken(void)
{
   int i;
   for(i=0;i<osdNumberOfMsgs;i++)
   {
      if(osMsgRamGeneric[i].resource!=0)
      {return 1;}
   }
    return 0;
}
#endif
#endif

#else
const uint8 osDummyMsg=0; /* to generate an object file */
void osDummyMsgFunc(void);

void osDummyMsgFunc(void){
   return;
}
#endif /* osdNumberOfMsgs */


/* END OF C:\PROGETTI\RTC_Dea700_2_STEP_dev\CFG_DishWasher\OSgen\OSEK\H8\msg.c */

