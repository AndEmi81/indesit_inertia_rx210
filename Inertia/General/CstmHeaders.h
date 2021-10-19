/**
\defgroup	CstmHeaders Modulo Custom Headers
\ingroup	ApplGeneral

Il modulo contiene gli include degli header files necessari all'applicazione.
*/

/**
Header file del modulo Custom Headers

\file		CstmHeaders.h
\ingroup	CstmHeaders
\date		23/07/2008
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __CSTMHEADERS_H
#define __CSTMHEADERS_H

/* ******** */
/* Includes */
/* ******** */

/* Standard */
#include "string.h"


#include "Config.h"
#include "LIB_Utilities.h"
#include "LIB_SettingExp.h"
#include "LIB_FixedExp.h"

#include "CstmDefines.h"
#include "CstmTypes.h"
#include "CstmFuncLib.h"
#include "CstmGlobal.h"
#include "CmnErr.h"
/* Header moduli Application Platform */

#include "APP_callback.h"
#include "APP_Interpreter.h"
#include "APP_JumpIf.h"
#include "APP_Loader.h"
#include "APP_SetState.h"
#include "APP_FuncLib.h"
#include "APP_StandBy.h"
#include "APP_ActivateAlgorithm.h"
#include "APP_Fault.h"

#include "HighLevel_Io_manager.h"
#include "DW_Regulations.h"
#include "DW_Hound.h"
#include "DW_Supervisor.h"
#include "DW_Safety.h"
#include "DW_Fault.h"
#include "DW_Statistics.h"
/* Header moduli Applicazione */

/* Microprogrammi */
#include "LoadsControl.h"
#include "Maintain.h"
#include "PumpControl.h"
#include "Drain.h"
#include "ExecuteAlgorithm.h"
#include "7literManager.h"
#include "AlternateControl.h"
#include "Heat.h"
#include "SetAlarm.h"
#include "SetProfile.h"
#include "SparklingAid.h"
#include "Load.h"
#include "LoadRestore.h"
#include "Regenerate.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __CSTMHEADERS_H */

/* *********** */
/* End of file */
/* *********** */
