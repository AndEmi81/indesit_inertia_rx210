/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			Implementation of all
\n
\file		MainActivityList.h
\ingroup	MainDebug
\date		12/01/2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.00
\n		12/01/2011
\n		Roberto Fioravanti
\n		changes
\n 		- Creation
\n
\n
*/
#ifndef __MAIN_ACTIVITY_H
#define __MAIN_ACTIVITY_H


/* ******** */
/* Includes */
/* ******** */
#include "autoconf.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef void Main_ActivityCallbackType (void);


typedef struct Main_ActivityStruct Main_ActivityType;
struct Main_ActivityStruct{
    Main_ActivityType*              pNext;
    Main_ActivityCallbackType*      pCallback;
#if defined(CONFIG_MAIN_ACTIVITY_EXECUTE_COUNT)
    uint32_t                        count;
#endif
};



typedef struct Main_ActivityCBStruct
{
    Main_ActivityType*              pActivityList;
    Main_ActivityType*              pCurrentActivity;
#if defined(CONFIG_MAIN_ACTIVITY_LIST_EXECUTE_COUNT)
    uint32_t                        count;
#endif
} Main_ActivityList_Type;




/* ********* */
/* Variables */
/* ********* */

/* Global */


/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
void Main_ActivityList_Init ( Main_ActivityList_Type* pActivityCB );
void Main_ActivityProcess(Main_ActivityList_Type* pActivityCB);

void Main_CreateActivity ( Main_ActivityList_Type* pActivityCB, 
                          Main_ActivityType* pActivity, 
                          Main_ActivityCallbackType*  pCallback,
                          Main_ActivityCallbackType*  pInit);
/* Local Function */




/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Local Function */



#endif /* __MAIN_ACTIVITY_H */
/* *********** */
/* End of file */
/* *********** */
