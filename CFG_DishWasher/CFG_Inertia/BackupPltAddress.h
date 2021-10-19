
#ifndef __BACKUP_PLT_ADDRESS_CONFIG_H
#define __BACKUP_PLT_ADDRESS_CONFIG_H
#include "PM_SFT_WM_NTCProbe.h"
#include "PM_SFT_WM_DrainPump.h"
#include "PM_SFT_WM_PressSwitch.h"
#include "LIB_ClassBVar.h"

/* Indicatori per individuare le variabili da non ripristinare dopo un PC Control */
/* #define PC_CTRL_NOT_RESTORED_END		((void *) 0) */
/* #define SIZEOF_PC_CTRL_MARKER			0 */

/* Indirizzi Dati Backup
===========================================*/
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) 

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#define PLATFORM_BACKUP_SAFE_VARS			2

#define PLATFORM_BACKUP_SAFE_SIZE									\
	sizeof(Sec_CrcExtVarTable[SEC_ID_persWashNTCProbe]) +			\
	sizeof(Sec_CrcExtVarTable[SEC_ID_sftDrainPumpVirtualTemp])


#define PLATFORM_BACKUP_SAFE_ADDRS									\
	&Sec_CrcExtVarTable[SEC_ID_persWashNTCProbe],					\
	&Sec_CrcExtVarTable[SEC_ID_sftDrainPumpVirtualTemp],			\


#define PLATFORM_BACKUP_SAFE_DIMTABS								\
	sizeof(Sec_CrcExtVarTable[SEC_ID_persWashNTCProbe]),			\
	sizeof(Sec_CrcExtVarTable[SEC_ID_sftDrainPumpVirtualTemp]),		\


#else
#define PLATFORM_BACKUP_SAFE_VARS			0
#define PLATFORM_BACKUP_SAFE_SIZE			0
#define PLATFORM_BACKUP_SAFE_ADDRS
#define PLATFORM_BACKUP_SAFE_DIMTABS
#endif

#define PLATFORM_BACKUP_NUM					(6 + PLATFORM_BACKUP_SAFE_VARS)

#define PLATFORM_BACKUP_SIZE ( PLATFORM_BACKUP_SAFE_SIZE +		\
	sizeof(coolDownCnt) + 										\
	sizeof(persWashNTCProbe) + 									\
	sizeof(sftDrainPumpVirtualTemp) +							\
	sizeof(prsZeroRefLev) +										\
	sizeof(prsCalibOK) +										\
	sizeof(prsCycleInProgr)	)


#define PLATFORM_BACKUP_ADDRS									\
	PLATFORM_BACKUP_SAFE_ADDRS									\
	&coolDownCnt,												\
	&persWashNTCProbe,											\
	&sftDrainPumpVirtualTemp,									\
	&prsZeroRefLev,												\
	&prsCalibOK,												\
	&prsCycleInProgr,											\



/*===========================================*/
#define PLATFORM_BACKUP_DIMTABS									\
	PLATFORM_BACKUP_SAFE_DIMTABS								\
	sizeof(coolDownCnt),										\
	sizeof(persWashNTCProbe),									\
	sizeof(sftDrainPumpVirtualTemp),							\
	sizeof(prsZeroRefLev),										\
	sizeof(prsCalibOK),											\
	sizeof(prsCycleInProgr),									\


#else /* #if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */

#define PLATFORM_BACKUP_NUM		(0)

#define PLATFORM_BACKUP_SIZE 	(0)

#define PLATFORM_BACKUP_ADDRS									\

/*===========================================*/
#define PLATFORM_BACKUP_DIMTABS									\

#endif /* #if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */
#endif /* __BACKUP_PLT_ADDRESS_CONFIG_H */
