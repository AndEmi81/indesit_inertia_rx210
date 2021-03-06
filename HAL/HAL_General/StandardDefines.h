/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    STDDEFINES	Standard Defines
@{
\par		  Descriptions
\n				It contains Standard Defines
\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti
\n
@}
\ingroup        HAL

\page stddefines Standard defines documentation
\ref STDDEFINES
*/
/**
\n
\par		    Header file
\n			    It contains Standard Defines
\n
\file		      StandardDefines.h
\ingroup	    STDDEFINES
\date		      23/06/2009
\version	    00.00.01
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n              00.00.02
\n              Jan 28,2011
\n              Stefano Ughi
\n              Bug fixed on macro sizeTab(x)
\n
\n 		        00.00.01
\n          	23/06/2009
\n          	Roberto Fioravanti
\n          	- HWKEY20INTERFACE addition
\n
\n 		        00.00.01
\n          	23/06/2009
\n          	Roberto Fioravanti
\n          	- Creation
\n
*/

/**
\addtogroup STDDEFINES
@{
*/

#ifndef __STANDARDDEFINES_H
#define __STANDARDDEFINES_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
\hideinitializer
Mimimum
\retval minimum beetween two values
*/
#define minValue(a,b)       (((a)<=(b))?(a):(b))
#define sizeTab(x)          (sizeof(x)/sizeof(x[0]))

#define TRUE	1						/**<\hideinitializer true*/
#define FALSE	0						/**<\hideinitializer false*/

#define ON  1							/**< \hideinitializer ON */
#define OFF 0							/**< \hideinitializer OFF */

/** Load States */
#define LOAD_ON					1
#define LOAD_OFF				0
/*----------------------------------------------------------------------*/
#define _SET   1           /**< \hideinitializer SET */
#define _RESET 0	          /**< \hideinitializer RESET */


#define _ENABLE_  1				/**< \hideinitializer _ENABLE_ */
#define _DISABLE_ 0				/**< \hideinitializer _DISABLE_*/


#define HI  1							/**< \hideinitializer HI*/
#define LOW 0							/**< \hideinitializer LOW*/

#define INPUT_HI  1   /**< DamperONOFF.c */
#define INPUT_LOW 0   /**< DamperONOFF.c */

/**
   @name Bit Mask
   @{
   Bit mask definition for bit access to the registers.
*/
#define BIT0_MASK   0x01			/**< \hideinitializer bit 0*/
#define BIT1_MASK   0x02			/**< \hideinitializer bit 1*/
#define BIT2_MASK   0x04			/**< \hideinitializer bit 2*/
#define BIT3_MASK   0x08			/**< \hideinitializer bit 3*/
#define BIT4_MASK   0x10			/**< \hideinitializer bit 4*/
#define BIT5_MASK   0x20			/**< \hideinitializer bit 5*/
#define BIT6_MASK   0x40			/**< \hideinitializer bit 6*/
#define BIT7_MASK   0x80			/**< \hideinitializer bit 7*/
#define ALLBIT_MASK 0xFF			/**< \hideinitializer all bit of a byte selected*/
/* @} */

#define BYTE_SIZE	8				/**< \hideinitializer byte size in bits*/

#define SQUARE_ROOT_2   1.414   /** Square root of two  approximate value */


/*Manufacturer*/
#define MCU_FREESCALE				0
#define MCU_RENESAS					1
#define MCU_NXP						2

/*Freescale Models*/
#define MODEL_S08GB60				0	/* Motorola */
#define MODEL_S08GB32				1	/* Motorola */
#define MODEL_S08GT32				2	/* Motorola */
#define MODEL_S08GT16				3	/* Motorola */
#define MODEL_S08GT60				4	/* Motorola */
#define MODEL_S08QE128				5	/* Motorola */
#define MODEL_MCF51AG128			6	/* Motorola */
/*Renesas Models */
#define MODEL_H83687				0
#define MODEL_H839064				1
#define MODEL_H839014				2
#define MODEL_R8C27					3
#define MODEL_H836077               4
#define MODEL_H836079               5
#define MODEL_R8C2B					6
#define MODEL_R5F52108				7
#define MODEL_R5F52Z05				8
#define MODEL_H8S					9
#define MODEL_R5F52Z06				10
#define MODEL_R5F51105				11
/* NXP Models */
#define MODEL_LPC1227				0
/* MCU Core Models */
#define CORE_HCS08					0	/* Core Freescale HCS08 */
#define CORE_R8C					1	/* Core Renesas R8C Tiny */
#define CORE_H8						2	/* Core Renesas H8 Tiny */
#define CORE_CFV1					3	/* Core Freescale CFV1 */
#define CORE_CORTEX_M0				4	/* Cortex M0 */
/*Appliance Models*/
#define APPLIANCE_CL				0	/* Cooling */
#define APPLIANCE_DW				1	/* Diswasher */
#define APPLIANCE_WM				2	/* Wash Machine */
#define APPLIANCE_CK				3	/* Cooking */
#define APPLIANCE_DR				4	/* Dryer */
#define APPLIANCE_HO				5	/* Hobs */
#define APPLIANCE_HD                6   /* Hoods */
#define NOTAPPLIANCE         		15 	/* Not Appliance */
/*Unique Code Visual Fw*/
#define	CK_DOUBLE_OVEN		  		15
#define CK_BIG_CAVITY				16
#define	DW_INDESIT_BASIC			26
#define DW_INDESIT_PREMIUM			27

/* Board */
#define BOARD_MAIN					  	0
#define BOARD_DISPLAY				  	1
#define BOARD_EXPANSION			  		2
#define BOARD_COMM_INTERFACE  			4

/* Evolution */
#define HWINTERFACE_RS232			0 /* Indoor Communication Node (HW Key with Indesit Platform Protocol) */
#define HWKEY20INTERFACE			(1) /*  Indoor Communication Node ADM */
#define HWKEY20INTERFACE_DDC2       (2)	/*  Indoor Communication Node ADM DDC2 */

/* Maximum setting file array size */
#define EEPPAGE_512					0
#define EEPPAGE_1024				1
#define EEPPAGE_2048				2
#define EEPPAGE_4096				3
#define EEPPAGE_8192				4
#define EEPPAGE_16384				5
#define EEPPAGE_32768				6
#define EEPPAGE_65536				7

/* crc */
#define CRC__NO						0	  /* CRC not Implemented */
#define CRC__YES						1	  /* CRC Implemented */

/*endianess*/
#define BIG_ENDIAN					0
#define LITTLE_ENDIAN				1

/* Format */
#define S_FORMAT						0	  /* old Ident Format */
#define E_FORMAT						1	  /* new Ident Format */


/*block*/
#define BLOCK_SIZE__64				0
#define BLOCK_SIZE__128				1


/**
Evo	 values
*/
#define EVO_PLATFORM__1_0            0	  
#define EVO_PLATFORM__2_0            1
#define EVO_PLATFORM__3_0            2
#define EVO_PLATFORM__4_0            3
#define EVO_PLATFORM__5_0            4
#define EVO_PLATFORM__6_0            5
#define EVO_PLATFORM__7_0            6


/* Ident32 defines */

/* Ident Version */
#define STD_IDENT32_VER_0			0


/**
Product Config Bitmap Values
*/

#define PRODUCT_CFG_NONE								0
/* white box */
#define PRODUCT_CFG_WBOX_NU								0
#define PRODUCT_CFG_WBOX_PRESENT						BIT0_MASK

/* remote control */
#define PRODUCT_CFG_RC_NONE								0
#define PRODUCT_CFG_RC_CONNECT_CLUSTER					BIT0_MASK
#define PRODUCT_CFG_RC_CONNECT_1PLUS					BIT1_MASK	/* ZiFridge with frame 0xAB/0xAC */


/* Main platform setting file pointers */
#define SF_PTR_SERIAL_NUMBER                            0x0000u
#define SF_PTR_EXTERNAL_BOARD_PARAMETERS                0x0001u
#define SF_PTR_EXP_BOARD                                0x0002u
#define SF_PTR_EXP_BOARD_FACTORY_TEST                   0x0003u
#define SF_PTR_EXP_BOARD_SPECIFIC                       0x0004u
#define SF_PTR_POWER_PROFILE                            0x0005u         /**< Power Profile data		*/
#define SF_PTR_FACTORY_TEST                             0x0006u         /**< Factory Test data */
#define SF_PTR_MAP_LENGTH                               0x0007u
#define SF_PTR_ENERGY_PHASE                             0x0008u         /**< Power Profile Energy Phases data             */
#define SF_PTR_CHECKSUM                                 0x0009u
#define SF_PTR_HW_CONFIG                                0x000Au         /**< Hw config data			*/
#define SF_PTR_HI_STATEMENTS                            0x000Cu
#define SF_PTR_LO_STATEMENTS                            0x000Du
#define SF_PTR_IO_CONFIG                                0x0020u         /**< Setting file pointer I/O Map */
#define SF_PTR_IO_DEVICE                                0x0021u         /**< Setting file pointer I/O configuration parameters  */
#define SF_PTR_LOADS_PROFILE                            0x0022u         /**< Setting file pointer Loads's Pilot Profiles  */
#define SF_PTR_MAIN_REG_PARAM_TABLE                     0x0023u         /**< Main table Regulation values  */
#define SF_PTR_CONDITION_TABLE                          0x0026u         /**< Condition table data  */
#define SF_PTR_FAULT                                    0x0027u         /**< Fault parameters data  */
#define SF_PTR_UP_MODIFIERS                             0x0029u         /**< Microprogram modifiers data */
#define SF_PTR_SAFETY_PARAMETERS                        0x002Au         /**< Safety parameters data  */
#define SF_PTR_ALGORITHM_PARAMETERS                     0x002Bu         /**< Algorithm parameters data  */
#define SF_PTR_APP_MODIFIERS                            0x0034u         /**< APP modifiers data*/
#define SF_PTR_ENERGY_METER                             0x0037u         /**< Energy meter parameters pointer */
#define SF_PTR_STATISTICS                               0x0039u         /**< Statistics data */


/**
   @name SYSTEM_MODE
   @{
   System Mode definition.
*/
/*
** System Mode
                do not bracketing (.)  these defines else HEW IDE generates the following warning
                "C5047 (W) Incompatible redefinition of macro "xxx"
*/

/**
Mode : application running
*/
#define SYSTEM_NORMAL_MODE							1

/* SYSTEM_NORMAL_MODE: submodes */
#define SYSTEM_NO_SUBMODE							0
/* normal sub modes */
#define SYSTEM_NORMAL_AUTOTEST_MODE                 1
#define SYSTEM_NORMAL_COLLAUDO_MODE                 2
#define SYSTEM_NORMAL_DEMO_MODE						3
#define SYSTEM_NORMAL_RESET_LAST_FAULT    			4
#define SYSTEM_NORMAL_RESET_TO_DEFAULT    			5
#define SYSTEM_NORMAL_STANDBY_MODE					6
#define SYSTEM_NORMAL_RESET_STANDBY_MODE            7
#define SYSTEM_NORMAL_FAST_STANDBY_MODE             8
#define SYSTEM_NORMAL_RESET_AUTOTEST_MODE           9
#define SYSTEM_NORMAL_RESET_COLLAUDO_MODE          10
#define SYSTEM_NORMAL_RESET_DEMO_MODE              11

#define SYSTEM_NORMAL_NOCHK_REFRESH					0xAA
#define SYSTEM_NORMAL_NOCHK_STEP2					0xAD
#define SYSTEM_NORMAL_NOCHK_STEP1     				0xDE

/**
Programming setting file mode
*/
#define SYSTEM_PROGRAMMING_MODE						2


/*programming sub modes */
#define SYSTEM_PROGRAMMING_SAVEREG_MODE              1


/**
corrupted setting file mode
*/
#define SYSTEM_UNCONSISTENT_SETTING_FILE_MODE		4

/**
Special mode (a.k.a. Test mode)
*/
#define SYSTEM_SPECIAL_MODE							5

/**
UI Remote Control mode
*/
#define SYSTEM_SET_UI_REMOTE_CONTROL                6

/**
Sleep mode
*/
#define SYSTEM_SLEEP_MODE                           7

/**
Boot Mode
*/
#define SYSTEM_BOOT_MODE                            8

#define SYSTEM_BOOT_SETTING_ENTER_STEP2				0xAC
#define SYSTEM_BOOT_ENTER_STEP2						0xAD
#define SYSTEM_BOOT_SETTING_ENTER_STEP1				0xCE
#define SYSTEM_BOOT_ENTER_STEP1     				0xDE

/* @} */



/*
    Endian Macros
*/

#define Endian32_Swap(value) \
         ((((unsigned int)((value) & 0x000000FF)) << 24) | \
         (((unsigned int)((value) & 0x0000FF00)) << 8) | \
         (((unsigned int)((value) & 0x00FF0000)) >> 8) | \
         (((unsigned int)((value) & 0xFF000000)) >> 24))

/*
#if TYPE_LONGLONG

#define Endian64_Swap(value)                                \
                (((((UInt64)value)<<56) & 0xFF00000000000000ULL)  | \
                 ((((UInt64)value)<<40) & 0x00FF000000000000ULL)  | \
                 ((((UInt64)value)<<24) & 0x0000FF0000000000ULL)  | \
                 ((((UInt64)value)<< 8) & 0x000000FF00000000ULL)  | \
                 ((((UInt64)value)>> 8) & 0x00000000FF000000ULL)  | \
                 ((((UInt64)value)>>24) & 0x0000000000FF0000ULL)  | \
                 ((((UInt64)value)>>40) & 0x000000000000FF00ULL)  | \
                 ((((UInt64)value)>>56) & 0x00000000000000FFULL))
#endif // TYPE_LONGLONG 
*/

/*
CW: 	__LITTLE_ENDIAN__ 	defined if the Compiler allocates in Little Endian order
		__BIG_ENDIAN__ 		defined if the Compiler allocates in Little Endian order

EWARM: 	__LITTLE_ENDIAN__ :
An integer that reflects the --endian option and is defined to 1 when the byte 
order is little-endian. The symbol is defined to 0 when the byte order is big-endian.

GNURX:	__RX_LITTLE_ENDIAN__	defined if Little Endian
		__RX_BIG_ENDIAN__		defined if big endian

A new macro is defined __ENDIANESS_LITTLE__ that wraps __LITTLE_ENDIAN__.
*/
#if defined (__CWCC__)
#if defined (__LITTLE_ENDIAN__)
#define __ENDIANESS_LITTLE__	(1)
#else
#define __ENDIANESS_LITTLE__	(0)
#endif

#elif defined (__IAR_SYSTEMS_ICC__)
#define __ENDIANESS_LITTLE__	(__LITTLE_ENDIAN__)
#elif defined(__GNUC__)
	#ifdef __RX_LITTLE_ENDIAN__
		#define __ENDIANESS_LITTLE__	(1)
	#else
		#define __ENDIANESS_LITTLE__	(0)
	#endif
#else

/* HEW */
#define __ENDIANESS_LITTLE__	(0)
#endif

/* L to L/N & B to B/N */

/* no op */
#if (__ENDIANESS_LITTLE__)
    #define EndianS16_LToL(value)               (value)
    #define EndianS16_LToN(value)               (value)
	#define EndianS16_NToL(value)               (value)
    #define EndianU16_LToL(value)               (value)
    #define EndianU16_LToN(value)               (value)
	#define EndianU16_NToL(value)               (value)
    #define EndianS32_LToL(value)               (value)
    #define EndianS32_LToN(value)               (value)
    #define EndianU32_LToL(value)               (value)
    #define EndianU32_LToN(value)               (value)
/*    #define EndianS64_ToL(value)               (value)
    #define EndianS64_ToN(value)               (value)
    #define EndianU64_ToL(value)               (value)
    #define EndianU64_ToN(value)               (value)*/
#else

    #define EndianS16_BToB(value)               (value)
    #define EndianS16_BToN(value)               (value)
	#define EndianS16_NToB(value)               (value)
    #define EndianU16_BToB(value)               (value)
    #define EndianU16_BToN(value)               (value)
	#define EndianU16_NToB(value)               (value)
    #define EndianS32_BToB(value)               (value)
    #define EndianS32_BToN(value)               (value)
    #define EndianU32_BToB(value)               (value)
    #define EndianU32_BToN(value)               (value)
/*    #define EndianS64_ToB(value)               (value)
    #define EndianS64_ToN(value)               (value)
    #define EndianU64_ToB(value)               (value)
    #define EndianU64_ToN(value)               (value)*/
#endif 
/*
    Map native to actual
*/
#if (__ENDIANESS_LITTLE__)
    #define EndianS16_LToB(value)               (signed short int)Endian16_Swap((unsigned short int)(value))
	#define EndianS16_BToN(value)				(signed short int)Endian16_Swap((unsigned short int)(value))
	#define EndianS16_NToB(value)               (signed short int)Endian16_Swap((unsigned short int)(value))
	#define EndianU16_BToN(value)               Endian16_Swap((value))
    #define EndianU16_LToB(value)               Endian16_Swap((value))
	#define EndianU16_NToB(value)               Endian16_Swap((value))
    #define EndianS32_LToB(value)               Endian32_Swap((value))
    #define EndianU32_LToB(value)               Endian32_Swap((value))
#else
    #define EndianS16_BToL(value)				Endian16_Swap((value))
	#define EndianS16_LToN(value)				Endian16_Swap((value))
	#define EndianS16_NToL(value)				Endian16_Swap((value))
    #define EndianU16_BToL(value)				Endian16_Swap((value))
    #define EndianU16_LToN(value)               Endian16_Swap((value))
	#define EndianU16_NToL(value)               Endian16_Swap((value))
    #define EndianS32_BToL(value)				Endian32_Swap((value))
    #define EndianU32_BToL(value)				Endian32_Swap((value))
	#define EndianU32_LToN(value)               Endian32_Swap((value))
#endif 


/**
\param [in] p (uint8_t) is the bit position in the byte of the least significant bit in the bit field.
The bit position p is 0 for the least significant bit in the byte,
7 (15 or 31) for the most significant bit. 0 <= p <= 7 (15 or 31).
\param [in] len (uint8_t) is the size in bits of the bit field. 1 <= len <= 8-p (1 <= len <= 15-p or 1 <= len <= 31-p).
\param [in] val is a the encoded value.

@{
*/
#define BFMASK(p, len) \
	(((1U<<(len))-1) << (p))

#define BF_8_GET(val, p, len) \
	(((uint8_t)(val) >> (p)) & ((1U<<(len))-1))
#define BF_16_GET(val, p, len) \
	(((uint16_t)(val) >> (p)) & ((1U<<(len))-1))
#define BF_32_GET(val, p, len) \
	(((uint32_t)(val) >> (p)) & ((1U<<(len))-1))
/*@*/


/*
get displ of fields in bytes
@{
*/

#define MBGET_MAX_DISPL(N, NameOfStruct, field) \
  MBGET_MAX_DISPL##N (NameOfStruct, field)

#define MBGET_MAX_DISPL1(NameOfStruct, field) \
  (&((NameOfStruct *)0)->field##0)


#define MBGET_MAX_DISPL2(NameOfStruct, field) \
  (&((NameOfStruct *)0)->field##0)>(&((NameOfStruct *)0)->field##1)?(&((NameOfStruct *)0)->field##0):(&((NameOfStruct *)0)->field##1)

#define MBGET_MAX_DISPL3(NameOfStruct, field) \
  (&((NameOfStruct *)0)->field##0)>(&((NameOfStruct *)0)->field##2)?(&((NameOfStruct *)0)->field##0):(&((NameOfStruct *)0)->field##2)

#define MBGET_MAX_DISPL4(NameOfStruct, field) \
  (&((NameOfStruct *)0)->field##0)>(&((NameOfStruct *)0)->field##4)?(&((NameOfStruct *)0)->field##0):(&((NameOfStruct *)0)->field##4)

/* @} */

/*
macros: getting bitfields value for byte-by-byte structure layout.
byte-by-byte structure layout imposes a rule on the naming of fields making up a
multi-byte quantity in these structures: the fields must all be named with a common name
followed by a digit 0..N, where myfield0 is the least significant byte of the multi-byte
quantity, myfield1 the next most significant byte, and myfieldN is the most significant byte.

Eg. struct{
		uint8_t onebytefield0;

		uint8_t twobytesfield0;
		uint8_t twobytesfield1;

		uint8_t threebytesfield0;
		uint8_t threebytesfield1;
		uint8_t threebytesfield2;

		uint8_t fourbytesfield0;
		uint8_t fourbytesfield1;
		uint8_t fourbytesfield2;
		uint8_t fourbytesfield3;

	};

@{
*/
/*
\param N number of bytes
\param structp pointer to struct
\param field field name
*/
#define MBGET(N, structp, field) \
  MBGET_##N (structp, field)

#define MBGET_1(structp, field) \
  ((structp)->field##0)

#define MBGET_2(structp, field) \
  ((structp)->field##0 | ((structp)->field##1<<8))

#define MBGET_3(structp, field) \
  ((structp)->field##0 | ((structp)->field##1<<8) | \
  ((structp)->field##2<<16))

#define MBGET_4(structp, field) \
 ((structp)->field##0 | ((structp)->field##1<<8) | \
 ((structp)->field##2<<16)|((structp)->field##3<<24))


#define MASK_8_FROM_VAL(p, len, val) 		(((uint8_t)(val) << (p)) & BFMASK(p,len))
/* @} */

/*
set bitfields
@{
 */
/*
\param N number of bytes
\param structp pointer to struct
\param field field name
\param val value to set
*/
#define MBSET(N, structp, field, val) \
  MBSET_##N (structp, field, val)

#define MBSET_1(structp, field, val) \
  ((structp)->field##0 = (val) & 0xff)

#define MBSET_2(structp, field, val) \
  ((structp)->field##0 = (val) & 0xff, \
   (structp)->field##1 = ((val) >> 8) & 0xff)

#define MBSET_3(structp, field, val) \
  ((structp)->field##0 = (val) & 0xff, \
   (structp)->field##1 = ((val) >> 8) & 0xff, \
   (structp)->field##2 = ((val) >> 16) & 0xff)

#define MBSET_4(structp, field, val) \
  ((structp)->field##0 = (val) & 0xff, \
   (structp)->field##1 = ((val) >> 8) & 0xff, \
   (structp)->field##2 = ((val) >> 16) & 0xff, \
   (structp)->field##3 = ((val) >> 24) & 0xff)

/* @} */

/** ident32 macros
@{
*/
#define STD_IDENT32_STRINGFILE_ADDRESS_DEFAULT					0
#define STD_IDENT32_STRINGFILE_SIZE_DEFAULT						0
#define STD_IDENT32_STRINGFILE_NUM_OF_PAGES_DEFAULT				0
#define STD_IDENT32_UCVFW_DEFAULT								0

/* header */
#define STD_IDENT32_HDR_MCU(model, manufacturer)				(MASK_8_FROM_VAL(4, 4, model)|MASK_8_FROM_VAL(0, 4, manufacturer))
#define STD_IDENT32_HDR_BOARD_APPLIANCE(board, appliance)		(MASK_8_FROM_VAL(4, 4, board)|MASK_8_FROM_VAL(0, 4, appliance))
#define STD_IDENT32_HDR_INFO(writeBlockSize, endianness, errorcheck, idtversion)		\
								(MASK_8_FROM_VAL(6, 1, writeBlockSize)|					\
								MASK_8_FROM_VAL(4, 1, endianness)|						\
								MASK_8_FROM_VAL(3, 1, errorcheck)|						\
								MASK_8_FROM_VAL(0, 3, idtversion))

/* fw version */
#define STD_IDENT32_BODY_FWVERSION_XX(XX)				(MASK_8_FROM_VAL(0, 8, XX))
#define STD_IDENT32_BODY_FWVERSION_YY(YY)				(MASK_8_FROM_VAL(0, 8, YY))
#define STD_IDENT32_BODY_FWVERSION_ZZ(ZZ)				(MASK_8_FROM_VAL(0, 8, ZZ))


/* fw version */
#define STD_IDENT32_BODY_PRJCODE(code)					(MASK_8_FROM_VAL(0, 8, code))


/* product configuration bitmap */
/*
-white box:
-remote control
*/
#define STD_IDENT32_BODY_PRODUCT_CFG0(wbox, rc)				(MASK_8_FROM_VAL(1, 3, rc)|MASK_8_FROM_VAL(0, 1, wbox))
#define STD_IDENT32_BODY_PRODUCT_CFG1()						(MASK_8_FROM_VAL(0, 1, 0))
/*@} ident32 macros*/


/*
SF_PTR_CHECKSUM macros
@{
*/
#define STD_SF_PTR_CHECKSUM_CHK_VAL(ptr)									MBGET(2,(ptr), chk)
#define STD_SF_PTR_CHECKSUM_CRC_VAL(ptr)									MBGET(2,(ptr), crc)
/*@} SF_PTR_CHECKSUM macros*/

/*
@{
*/

#ifdef CONFIG_PL_DEBUG
	void pltErrorHandler(int err);
	/**
	 * Generic assert in CONFIG_PLT_DEBUG, ensure platform consistency
	 */
	#define PLT_ASSERT(exp)  if(!(exp)) { pltErrorHandler(exp); }
#else
	#define PLT_ASSERT(exp)  {}
#endif
/*@}*/

#define 	BOOT_SETTING_START_KEY	0xF145A92Cu
#define 	BOOT_START_KEY			0xE078F445u
#define		BOOT_SIGNATURE			0xABCD4321u


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


#endif	/* __STANDARDDEFINES_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
