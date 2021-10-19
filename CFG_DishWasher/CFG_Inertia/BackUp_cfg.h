#ifndef __BACKUP_CFG_H
#define __BACKUP_CFG_H

/* RTN check mode */
/*#define RTN_CHECK_WITH_COUNTER*/
#define RTN_CHECK_WITH_SW_ALARM
/* According to linker config. file */
#if defined (FREESCALE_MCF51_AG128)
#define BACKUPAREA_START_ADDR		0x0001BC00
#define BACKUPAREA_SIZE_IN_BYTES    1024
#endif /* defined (FREESCALE_MCF51_AG128) */

#if defined (NXP_LPC12xx_1227)
#define BACKUPAREA_START_ADDR	    0x0001BC00
#define BACKUPAREA_SIZE_IN_BYTES    100
#endif /* defined (NXP_LPC12xx_1227) */

#if defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)
#define BACKUPAREA_START_ADDR	    0x00100000
#define BACKUPAREA_SIZE_IN_BYTES    (1024)
#endif /* defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0) */



#if defined (RTN_CHECK_WITH_COUNTER)
/* Numbers of consecutive reads out of range to enter in backup for RTN */
#define NUM_READ_RTN_OUT_OF_RANGE   20
/* Numbers of consecutive reads in range to exit from backup for RTN */
#define NUM_READ_RTN_IN_RANGE       20
#elif defined (RTN_CHECK_WITH_SW_ALARM)
#define ID_SW_ALARM 0
#define RTN_OUT_OF_RANGE_TOUT_NSEC  5000000     /*nSec*/
#define RTN_IN_RANGE_TOUT_NSEC      5000000     /*nSec*/
#else
#error "You must define one mode for RTN check!"
#endif

#define BACKUPAREA_END_ADDR         (BACKUPAREA_START_ADDR+BACKUPAREA_SIZE_IN_BYTES-1) 

#endif /* __BACKUP_CFG_H */
