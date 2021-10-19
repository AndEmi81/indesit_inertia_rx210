/*
Copyright (c) 2004-2012, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_FWParams.c
\brief   Il file contiene i dati dei  parametri  da FW  vedi CONFIG_PARAM_FROM_FIRMWARE definito da configuratore.
\ingroup PM_MotorGate
\author  Marco Sperandini
*/
#include "Config.h"

/**
\addtogroup PM_MotorGate
*@{*/
#ifdef CONFIG_PM_MOTORGATE

#if defined (CONFIG_PARAM_FROM_FIRMWARE)

const uint16_t tabSpeedRange[6] =
{
    70-1,		/* vTarget <= 69 r.p.m. 	(rifVel = 0) */
    100-1,		/* vTarget <= 99 r.p.m. 	(rifVel = 1) */
    400-1,		/* vTarget <= 399 r.p.m. 	(rifVel = 2) */
    700-1,		/* vTarget <= 699 r.p.m. 	(rifVel = 3) */
    1000-1,		/* vTarget <= 999 r.p.m. 	(rifVel = 4) */
    0xFFFF		/* vTarget  > 999 r.p.m. 	(rifVel = 5) */
};

/*
stepOutOfRange_dn - ratioVelTarget_dn - stepOutOfRange_up - ratioVelTarget_up - maxDev
x_dn: velCurrent < velTarget
x_up: velCurrent > velTarget
*/
const uint8_t tabOutRange[2][ sizeof(tabSpeedRange)/sizeof(tabSpeedRange[0])][5] =
{
    32,0,32,0,64,    	/* 60 Hz - rifVel= 0 */
    32,0,32,0,64,    	/* 60 Hz - rifVel= 1 */
    12,4,12,4,40,    	/* 60 Hz - rifVel= 2 */
    15,16,15,16,46,   	/* 60 Hz - rifVel= 3 */
    70,4,70,4,74,    	/* 60 Hz - rifVel= 4 */
    70,4,70,4,74,		/* 60 Hz - rifVel= 5 */

    80,0,80,0,74,    	/* 50 Hz - rifVel= 0 */
    80,0,255,20,74,    	/* 50 Hz - rifVel= 1 */
    27,4,27,4,74,    	/* 50 Hz - rifVel= 2 */
    15,16,15,16,46,  	/* 50 Hz - rifVel= 3 */
    70,4,70,4,74,    	/* 50 Hz - rifVel= 4 */
    70,4,70,4,74 	 	/* 50 Hz - rifVel= 5 */
};

/* ST ( 6 bytes ) - CD (2 bytes) - CPD (2 bytes)  */
const uint8_t tabPD_Param[2][sizeof(tabSpeedRange)/sizeof(tabSpeedRange[0])][10] =
{
    0x00,0x01,0x02,0x6E,0x97,0x8D, 0x0D,0x0F, 0x0D,0x7F,     /* 60 Hz, rifVel = 0, ST-CD-CPD*/

    0x00,0x01,0x0B,0xC6,0xA7,0xF0, 0x14,0xDA, 0x16,0xF5,     /* 60 Hz, rifVel = 1, ST-CD-CPD*/

    0x00,0x01,0x1B,0xA5,0xE3,0x54, 0x36,0x12, 0x3B,0x05,     /* 60 Hz, rifVel = 2, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x76,0x39, 0x80,0x37,     /* 60 Hz, rifVel = 3, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x76,0x39, 0x80,0x37,     /* 60 Hz, rifVel = 4, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x76,0x39, 0x80,0x37,     /* 60 Hz, rifVel = 5, ST-CD-CPD*/

    0x00,0x01,0x04,0x9B,0xA5,0xE3, 0x04,0xC0, 0x05,0x3A,     /* 50 Hz, rifVel = 0, ST-CD-CPD*/

    0x00,0x01,0x0B,0x6C,0xA7,0xF0, 0x0C,0x03, 0x0D,0x39,     /* 50 Hz, rifVel = 1, ST-CD-CPD*/

    0x00,0x01,0x35,0x3F,0x7C,0xEE, 0x1F,0x26, 0x24,0xA3,     /* 50 Hz, rifVel = 2, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x44,0x1A, 0x49,0xDB,     /* 50 Hz, rifVel = 3, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x44,0x1A, 0x49,0xDB,     /* 50 Hz, rifVel = 4, ST-CD-CPD*/

    0x00,0x01,0x37,0xCE,0xD9,0x17, 0x44,0x1A, 0x49,0xDB,     /* 50 Hz, rifVel = 5, ST-CD-CPD*/
};


const uint8_t tabRebalancePD_Param[10] =
{
    0x00,0x01,0x0B,0xC6,0xA7,0xF0, 0x0B,0xAF, 0x0D,0xCA,    /* rebalance up param */
};
#endif /* CONFIG_PARAM_FROM_FIRMWARE */

#endif /* CONFIG_PM_MOTORGATE */
/**@}*/
