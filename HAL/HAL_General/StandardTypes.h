/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    STDTYPES	Standard Types
@{
\par		    Description
It contains standard types definition for Platform 2 projects
\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti
@}
\ingroup        HAL

\page stdtypes Standard types documentation
\ref STDTYPES        
*/
/**
\n
\par		    Header file
\n			    Contiene le definizioni dei tipi relativi allo SkeletonBase \n
e allo Skeleton Applicazione
\n
\file		    StandardTypes.h
\ingroup	    STDTYPES
\date		    25/02/2009
\version	    00.00.01
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n 
\n 		        00.00.12
\n          	xx/09/2009
\n          	Roberto Fioravanti
\n          	changes
\n            - aggiornata Flash_Data_Struct 
\n
\n 		        00.00.01
\n          	25/02/2009
\n          	Roberto Fioravanti
\n          	Prima versione
\n
*/

/**
\addtogroup STDTYPES
@{*/

#ifndef __STANDARDTYPES_H
#define __STANDARDTYPES_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined(__GNUC__) && defined(__RX_BIG_ENDIAN__)
/* Normally, GCC allocates bitfields from either the least significant or most
significant bit in the underlying type, such that bitfields happen to be
allocated from lowest address to highest address. Specifically, big-endian
targets allocate the MSB first, where little-endian targets allocate the LSB first.*/
#define __BIT_ORDER_MSB_FIRST__
#else
#define __BIT_ORDER_LSB_FIRST__
#endif



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/*lint -esym(754,w) */


#if defined(RENESAS_300H_39079)
typedef   unsigned char		PORT_STRUCT;
#endif


/* Generic */
#if 0 /* obsolete types */
typedef unsigned char BOOL;
typedef unsigned char BYTE;
typedef signed short SHORT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#endif

/**
  @name Standard ISO-Posix types definitions
  @{
*/
typedef signed short int    	int16_t;
typedef unsigned short int		uint16_t;
typedef unsigned long int		uint32_t;
typedef signed long int 		int32_t;
typedef unsigned long long		uint64_t;
typedef signed long long 		int64_t;
typedef unsigned char 			uint8_t;
typedef signed char				int8_t;
typedef unsigned char 			bool8_t; /* to be used where required 1 byte boolean type */
typedef unsigned int 			bool_t;  /* to be used for temporary variables/values */
	/* the generic bool_t allows compiler a better size optimization */



/* @} */


/* Counters */
typedef struct 
{
	struct{
		uint8_t Enable  :1;
		uint8_t UpDown  :1;
		uint8_t FreeR   :1;
		uint8_t Opcode  :5;
	} settings;
      
	uint8_t value;
} COUNTER;

/*Loader*/
typedef struct Loader{

	uint8_t *Data;
	uint16_t DataLenght;
} Loader_Struct;


/** struttura dati per gli ingressi digitali */
typedef union 
{  
    /** accesso all'intera struttura */
	uint16_t w;
	/** accesso a bit */
	struct
	{
		/** se '1' la lettura dell'ingresso ?? abilitata */
		uint8_t active:1;
		/** se '1' ingresso multiplexato altrimenti semplice */
		uint8_t readtype:1;
		/** ?? la potenza di 2 del n?? di letture effettuate prima di
		avere la nuova lettura */
		uint8_t filterPot2:3;  
		/** se l'ingresso ?? multiplexato rappresenta la combinazione
		da dare al multiplexer per ottenere l'ingresso */
		uint8_t chsel:3;
		/** pin associato all'ingresso */
		uint8_t pin:6;
		/** Se '1' l'ingresso deve essere letto in modo invertito */
		uint8_t inversion:1;
		/** Se '1' l'ingresso deve essere configurato con pull-up */        
		uint8_t pullupEn:1;
	}s;
}DIGINPUTPIN;



/*TIME INPUTS PIN*/
typedef union 
{
	unsigned short w;
	struct
	{
		unsigned char Enabled   :1;
		unsigned char NSamples	:7;
		unsigned char Delta		:4; 
		unsigned char MulConf	:3;
		unsigned char ReadType  :1; /* non ?? nella struttura editor
									?? nella inizializzazione dell'IO_Manager */
		
	} s;
}TIME_INPUT_STRUCT;




/*
** *******************************************************************
**	USER-DEFINED STRUCTURED DATA TYPES
*/



/* MAIN IDENT STRUCT */
#if defined (__ICCARM__) 
#if (__LITTLE_ENDIAN__)
#pragma bitfields=disjoint_types
#endif 
#endif



#if defined(__BIT_ORDER_MSB_FIRST__)
#if defined(RN_RX200) || defined(RN_RX100)
typedef struct IdentHeaderStruct2{
	uint8_t 	Model			:4;
	uint8_t 	Manufacturer	:4;

	uint8_t 	Board			:4;
	uint8_t 	Appliance		:4;

	uint8_t 	free			:1;
	uint8_t 	block128		:1;		/* if '0' write block 64 else 128 */
	uint8_t 	newFormat		:1;		/* if '0' standard format else extended format */
	uint8_t 	littleEndian	:1;		/* if '0' MSB first (Big Endian) else LSB first (Little Endian) */
	uint8_t 	crcReady		:1;		/* if '1' CRC implemented */
	uint8_t 	Evo				:3;
}IdentHeaderType;

#else
#error "not supported yet."
#endif
#else

typedef struct IdentHeaderStruct2{
    uint8_t 	Manufacturer	:4;
	uint8_t 	Model			:4;

	uint8_t 	Appliance		:4;
	uint8_t 	Board			:4;
	
	uint8_t 	Evo				:3;
	uint8_t 	crcReady		:1;		/* if '1' CRC implemented */
	uint8_t 	littleEndian	:1;		/* if '0' MSB first (Big Endian) else LSB first (Little Endian) */
	uint8_t 	newFormat		:1;		/* if '0' standard format else extended format */
	uint8_t 	block128		:1;		/* if '0' write block 64 else 128 */
	uint8_t 	free			:1;

}IdentHeaderType;

#endif




#if defined (__ICCARM__) 
#if (__LITTLE_ENDIAN__)
#pragma bitfields=default /* Restores to default setting. */
#endif 
#endif


typedef struct IdentTableStruct{
    IdentHeaderType header;

	uint8_t 	FWversion[3];
	uint16_t 	RAMaddress;				/* microcontroller RAM Start address */
	uint16_t 	EEPaddress;				/* setting start address */
	uint16_t 	LUTaddress;				/* lookup table start address */

#if defined(__BIT_ORDER_LSB_FIRST__)
	uint8_t 	pageSize		:3;		/* num. di shift a sinistra del valore 512 */
	uint8_t 	pageNum			:5;		/* num. pagine destinate al file di setting - 1 */
#else
	uint8_t 	pageNum			:5;		/* num. pagine destinate al file di setting - 1 */
	uint8_t 	pageSize		:3;		/* num. di shift a sinistra del valore 512 */
#endif
	uint8_t 	LUTsize;
	uint16_t	BackUpAddress;
	uint16_t	StatisticsAddress;		
	uint16_t	BackupSize;
	uint16_t	StatisticsSize;
	uint16_t	SkeletonRAMAddress;
	uint16_t	ApplicationRAMAddress;		 		
	uint16_t	LastFaultAddress;
	uint16_t	CurrFaultAddress;
	uint16_t	FCTaddress;
	uint16_t	StatisticAbsSize;
	uint16_t	StatisticRelSize;
	uint16_t	PDMaddress;
	uint8_t		ProductCfg;
}IdentTableStruct_t;

/*
 * Ident table 32-bit data structure
 */
typedef struct STD_IdentTableStruct32bit_s
{
	/* RAM */
	uint32_t		RamAddr;		/*RAM address */
	uint32_t		RamSize;		/*Ram size */
	uint32_t		LktAddr;		/*Lkt address */
	uint32_t		WBoxAddr;		/*WBox address */
	uint32_t		PDMAddr;		/*PDM address */
	uint32_t		EepAddr;		/*Setting address */
	uint32_t		BckAddr;		/*Backup address */
	uint32_t		StrFileAddr;	/*String file address */
}STD_IdentTableStruct32bitType;

/*
 * Ident table 16-bit data structure
 */
typedef struct STD_IdentTableStruct16bit_s
{
	/* RAM */
	uint16_t		LktSize;		/*Lkt size in bytes */
	uint16_t		WboxSize;		/*Wbox size in bytes */
	uint16_t		EepSize;		/*Eep size in bytes */
	uint16_t		BckSize;		/*Bck size in bytes */
	uint16_t		UCVFW;			/*Unique code Visual fw */
	uint16_t		StrFileSize;	/*String file size in bytes */
}STD_IdentTableStruct16bitType;


/*
 * Ident table 08-bit data structure
 */
typedef struct STD_IdentTableStruct08bit_s
{
	/* RAM */
	uint8_t		EepPagesNum;		/*num of pages for Eep */
	uint8_t		StrFileNumPages;	/*num of pages String file */
}STD_IdentTableStruct08bitType;


/*
 * Ident table 08-bit data structure
 */
typedef struct STD_IdentTableStructHdr_s
{
	uint8_t		Mcu0;			/*	BE
									7-4: 	model
									3-0: 	manufacturer
								*/
	uint8_t		Board_Applicance0;
								/*	BE
									7-4: 	board
									3-0: 	appliance type
								*/
	uint8_t		Info0;			/*  BE
									7: 		unused
									6: 		write block size
									5: 		unused
									4: 		endianness
									3: 		error checking
									2-0: 	version
								*/

	/* Fw version */
	uint8_t 	FwVersionXX0;	/* version */
	uint8_t 	FwVersionYY0;	/* verification */
	uint8_t 	FwVersionZZ0;	/* modification */


	/* Product configuration */
	uint8_t 	ProductCfg1;	/* Product configuration MSB */
	uint8_t 	ProductCfg0;	/* Product configuration LSB */

	/* Project code */
	uint8_t 	prjCode0;		/* Project code */

	/* description of subsequent blocks */
	uint8_t		numof32bitdata;
	uint8_t		numof16bitdata;
	uint8_t		numof08bitdata;

}STD_IdentTableStructHdrType;


/*
 * Ident table structure
 */
typedef struct STD_IdentTableStruct32_s{
	/* header */
	STD_IdentTableStructHdrType 	hdr;

	/*32-bit data block*/
	STD_IdentTableStruct32bitType	data32bit;
	/*16-bit data block*/
	STD_IdentTableStruct16bitType	data16bit;
	/*08-bit data block*/
	STD_IdentTableStruct08bitType	data08bit;
}STD_ExtendedIdentTableType;






/** I2CexpAddr + ExpansionCode identificano in modo univoco l'Expansion */
typedef union {
	uint8_t code;
	struct{		
	    uint8_t type:4;	    /* bits 3-0 */
	    uint8_t family:4;   /* bits 7-4 */
	}field;
}exIdentifier_t;

typedef struct{
    uint8_t I2CexpAddr;
	uint8_t code;
}idTypeExpansion_s;

typedef union{
    uint16_t all;
    struct {
	uint8_t I2CexpAddr;
	uint8_t code;
	}field;
}idTypeExpansion_t;

typedef struct{
    void *const pBufDataForExp;
    void *const pBufExpInfo;
    const uint8_t sizeBufDataForExp;
    const uint8_t sizeBufExpInfo;    
}vectorsRefExp_t;

typedef uint8_t SysMode_Type;
typedef uint8_t SysSubMode_Type;

/** Fault code/subcode structure */
typedef struct sft_fault_codes_s {

	uint8_t code;		/** Fault Code */
	uint8_t subCode;	/** Fault Subcode */

} sft_fault_codes_t;

/** Safety module fault state structure; used as interface of Product Safety
 *  to application
 *
 *  fault:		code+subcode
 *  staus:		Status (bit 0-6) + Status change notification (bit 7)		*/
typedef struct sft_module_faults_s
{
	sft_fault_codes_t fault;
	uint8_t	status;
} sft_module_faults_t;

/** Data type for Safety Action array */
typedef uint32_t sft_safety_action_bmp_t;


/* Need to avoid RAM allocation of const void* array[] vars */
typedef void *voidPtr;


/*
SF_PTR_CHECKSUM
 * */
typedef struct STD_SettingSignature_s
{
	unsigned char chk1;				/**< Modified chk - 2 bytes, unsigned , BE	*/
	unsigned char chk0;				/**<                  */
	unsigned char crc1;				/**< CRC16 - 2 bytes, unsigned , BE	*/
	unsigned char crc0;				/**<                  */
}
STD_SettingSignatureType;

typedef struct boot_short_info_s {
	uint32_t 	signature;
	uint8_t		*shortInfo;
} boot_short_info_t;


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


#endif	/* __STANDARDTYPES_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
