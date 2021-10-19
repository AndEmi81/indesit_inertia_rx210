
/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\n
\par		    IO_Driver -
\n			    Contains target dependant typedef and defines for drivers

Definizioni, enum e tipi generali.
\file    IO_driver.h

\date    05/mar/09
\version 1.0.0
\author  stefano.ughi
\addtogroup IOGenerl IO General
\ingroup Drivers
\n
\n
\par\b	History:
\n
\n 		        00.00.14
\n          	28/10/2009
\n          	Roberto Fioravanti
\n          	changes
\n            - IO_ModuleIOMapType added to allow the ModuleIOMap definition.
\n						- #defines BEGIN_MODULE_IO_MAP(n) and END_MODULE_IO_MAP() added. This helps the definition of the ModuleIOMap table in config.c file.
\n						- extern declaration for ModuleIOmap[];
\n 
\n 		        00.00.01
\n          	05/03/2009
\n          	stefano.ughi
\n          	Prima versione
@{*/

#ifndef __IODRIVER_H
#define __IODRIVER_H

/* ******** */
/* Includes */
/* ******** */

#include "HAL_Interface.h" 
#include  HAL_CFG
/* need of these header file because of callback.h : if callback prototypes will became standard they culd be removed */

#include "StandardTypes.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Configurazione del canale per modalità input capture su fronte di salita */
#define IC_ON_RISING_EDGE	  1
/** Configurazione del canale per modalità input capture su fronte di discesa */
#define IC_ON_FALLING_EDGE    2
/** Configurazione del canale per modalità input capture con entrambi i fronti */
#define IC_ON_BOTH_EDGE		  3
/** Configurazione del canale per modalità compare software only */
#define OC_SW_ONLY		      4
/** Configurazione del canale per modalità toggle output sul compare  */
#define OC_TOGGLE_OUTPUT	  5
/** Configurazione del canale per modalità reset output sul compare  */
#define OC_RESET_OUTPUT		  6
/** Configurazione del canale per modalità set output sul compare  */
#define OC_SET_OUTPUT         7
/** Configurazione del canale per modalità PWM Edge Aligned: normally high e reset sul compare */
#define PWM_EDGE_HIGH_TRUE    8
/** Configurazione del canale per modalità PWM Edge Aligned: normally low e set sul compare */
#define PWM_EDGE_LOW_TRUE	  9
/** Configurazione del canale per modalità PWM Center Aligned: normally high e reset sul compare */
#define PWM_CENTER_HIGH_TRUE 10
/** Configurazione del canale per modalità PWM Center Aligned: normally low e set sul compare */
#define PWM_CENTER_LOW_TRUE  11
/** Inverte la sensibilità sul fronte de canale */
#define TOGGLE_EDGE          12


/** Device type */
enum IO_DEVICETYPE{
	STANDARD=0,
	CHANNEL,
	RANDOM
};
/** Driver type */
enum IO_DRIVERMODUS{
	SYNCRON=0,
	ASYNCRON,
	BOTH
};

/** General Errors's Def. IO_ErrorType */
enum eIO_ERROR{	
	IO_E_OK                 =0,               /**< Success */	
	IO_E_BUSY,                                /**< Function is blocked */
	IO_E_WORKING,                             /**< Working */
	IO_E_UNKNOWN_MODE, 	                      /**< The requested command is not supported */	
	IO_E_FCN_SUSPENDED=16,                    /**< Misura sospesa da un’altra funzione */
	IO_E_PARAM_IGNORED,  	                    /**< Il parametro passato alla funzione è stato ignorato */
	IO_E_INVALID_CHANNEL_ID, 	                /**< Il descrittore del canale non è valido,drivers tipo channel */
	IO_E_INVALID_VALUE,		                    /**< Il valore del parametro passato è invalido */
	IO_E_INVALID_SIZE,		                    /**< La dimensione del parametro passato è invalida */
	IO_E_INVALID_POSITION,	                  /**< La posizione a partire dalla quale debbo leggere/scrivere i dati non è valida, es eeprom o flash */
	IO_E_INVALID_NOTIF_TYPE,                  /**< Il tipo della notifica che la funzione deve dare non è valido */
	IO_E_UNEXPECTED_CONDITION,                /**< Si è verificata una condizione inaspettata  */
	IO_E_ALREADY_PENDING,                     /**< Already Pending */
	IO_E_NOT_QUEUED,                          /**< Not queued */
	IO_E_FAIL,                                /**< Failed */
	IO_E_TIMEOUT_EXPIRED,                     /**< Timeout expired */


	IO_E_CHECK_PP,                            /**< Communication Platform: error in CHK o CRC*/
	IO_E_ACK_PP,                              /**< Communication Platform: missing ACK */
    IO_E_ACK_BUS,                             /**< Communication Platform: missing ACK (I2C) */
    IO_E_OV_AL,                               /**< Communication Platform: Overrun or Arbitration Lost */


    IO_E_OUT_OF_MEMORY=32,					/**< Flash: position+(size-1) out of max address avaible */
    IO_E_OUT_OF_BOUNDS,						/**< Flash: params out of table's limits */
    IO_E_NOT_BLANK,							/**< Flash: data flash not blank */
    IO_E_PROGRAMMING,						/**< Flash: errors during erase/write operations */
    IO_E_VERIFY,							/**< Flash: errors during verify operation */
    IO_E_ERASE_FORBIDDEN                    /**< Flash: on the selected block erase operation is forbidden*/
};

/** Definizione delle notifiche globali */
enum eIO_NOTIFICATION
{
    IO_N_ERROR =0,                               /**< Transfer error */
    IO_N_READ_DATA_INIT,                      /**< Address match Read */
    IO_N_WRITE_DATA_INIT,                     /**< Address match Write */
    IO_N_TRANSFER_DONE,                       /**< Transfer done */
    IO_N_TX_DONE,							  /**< Tx done */  
    IO_N_ACTION,                              /**< Decode */
    IO_N_COMPLETED                      	  /**< Completed */
    
      
};

/** Stato del driver */
enum IO_DEVICE_STATE{
	IO_STATE_IDLE,
	IO_STATE_ACTIVE,
	IO_STATE_UNKNOWN
};

enum IO_DEVICES{
  DEVICE_NULL=0,
	DEVICE_SWDMA
};

/* Options */ 
enum IO_OPTIONS{
  IO_MASTER_BIT,
  IO_CLOSE_BIT,
  IO_CLOSE_TX_BIT
};

#define IO_CLOSE        (1<<(IO_OptionSizeType)IO_CLOSE_BIT)
#define IO_OPT_MASTER   (1<<(IO_OptionSizeType)IO_MASTER_BIT)
#define IO_CLOSE_TX     (1<<(IO_OptionSizeType)IO_CLOSE_TX_BIT)

/* Digital IO State */
enum DIO_STATE_PIN
{
    DIO_STATE_PIN_IS_UNKNOWN,
    DIO_STATE_PIN_IS_INPUT,
    DIO_STATE_PIN_IS_OUTPUT,
    DIO_STATE_PIN_PULLUP_IS_ON,
    DIO_STATE_PIN_PULLUP_IS_OFF,
    DIO_STATE_PIN_PULLUP_IS_NOT_PRESENT,
    DIO_STATE_PIN_PULLDOWN_IS_ON,
    DIO_STATE_PIN_PULLDOWN_IS_OFF,
    DIO_STATE_PIN_PULLDOWN_IS_NOT_PRESENT,
    DIO_STATE_PIN_DRIVESTRENGTH_IS_ON,
    DIO_STATE_PIN_DRIVESTRENGTH_IS_OFF,
    DIO_STATE_PIN_DRIVESTRENGTH_IS_NOT_PRESENT,
    DIO_STATE_PIN_SLEWRATE_IS_ON,
    DIO_STATE_PIN_SLEWRATE_IS_OFF,
    DIO_STATE_PIN_SLEWRATE_IS_NOT_PRESENT,
    DIO_STATE_PIN_PASSIVEFILTER_IS_ON,
    DIO_STATE_PIN_PASSIVEFILTER_IS_OFF,
    DIO_STATE_PIN_PASSIVEFILTER_IS_NOT_PRESENT,
    DIO_STATE_PIN_INTRPT_IS_ON,
    DIO_STATE_PIN_INTRPT_IS_OFF,
    DIO_STATE_PIN_INTRPT_IS_NOT_PRESENT,
    DIO_STATE_PIN_INTRPT_FLG_IS_ON,
    DIO_STATE_PIN_INTRPT_FLG_IS_OFF,
    DIO_STATE_PIN_INTRPT_FLG_IS_NOT_PRESENT,
    DIO_STATE_PIN_INTRPT_EDGE_IS_RISING,
    DIO_STATE_PIN_INTRPT_EDGE_IS_FALLING,
    DIO_STATE_PIN_INTRPT_EDGE_IS_NOT_PRESENT,
    DIO_STATE_PIN_DIGITAL_FLT_IS_NOT_PRESENT,
    DIO_STATE_PIN_DIGITAL_FLT_IS_ON,
    DIO_STATE_PIN_DIGITAL_FLT_IS_OFF,
    DIO_STATE_PORT_INTRPT_IS_NOT_PRESENT,
    DIO_STATE_PORT_INTRPT_IS_ON,
    DIO_STATE_PORT_INTRPT_IS_OFF,
    DIO_STATE_PORT_DMA_IS_NOT_PRESENT,
    DIO_STATE_PORT_DMA_IS_ON,
    DIO_STATE_PORT_DMA_IS_OFF,
    DIO_STATE_DETECT_MODE_INTRPT_IS_NOT_PRESENT,
    DIO_STATE_DETECT_MODE_INTRPT_IS_LEVEL,
    DIO_STATE_DETECT_MODE_INTRPT_IS_EDGE

    
};


#define BEGIN_IO_MAP(IoMapName, ItemsNum)    const IO_ModuleIOMapType IoMapName[ItemsNum]={
  
#define END_IO_MAP()       };

/* ModuleIoMap */
#define BEGIN_MODULE_IO_MAP(n)    const IO_ModuleIOMapType ModuleIOmap[n]={
  
#define END_MODULE_IO_MAP()       };

/* FlashMap, Flash Erase/Write Operations Availability */
#define BEGIN_MCU_FLASH_MAP(n)    const FLS_MapType FlashMap[n]={
  
#define END_MCU_FLASH_MAP()       };

/* FlashMap, Flash Erase Operations Availability */
#define BEGIN_MCU_FLASH_MAP_ERASE(n)    const uint8_t FlashMapEraseForbidden[n]={

#define END_MCU_FLASH_MAP_ERASE()      };

#define BEGIN_HW_CNTR_CFG(n)    const IO_HwCntrCfgType IO_HwCntrCfg[n]={
    
#define END_HW_CNTR_CFG(n)      };

/** Reset Sources defines vars typedef RST_Type 
Keep in mind that last bit of vars is busy so if you define RST_Type as uint8_t 
you can't have a define greater than 127 */
#define RST_UNKNOWN                     0
#define RST_BY_POWER_ON                 1
#define RST_BY_PIN                      2
#define RST_BY_WATCHDOG                 3
#define RST_BY_LOW_VOLT_DETECT          4
#define RST_BY_LOSS_OF_CLOCK            5
#define RST_BY_ILLEGAL_OPCODE           6
#define RST_BY_ILLEGAL_ADDR_ACCESS      7
#define RST_BY_USER                     8
#define RST_BY_DEBUGGER                 9
#define RST_BY_RAM_ERR                  10

#define NUM_RST_SOURCE                  11 /* Reset Sources above */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* Flash Map */
#if defined(HAL_FLS_SUPPORT)
typedef struct FlS_Map_tag {
    FLS_positionType StartAddress; /**< Start Address Flash Block */
    FLS_positionType EndAddress;   /**< End Address Flash Block */
}FLS_MapType;
#endif /* defined(HAL_FLS_SUPPORT) */


/* Reset source type */
typedef uint8_t RST_Type;

/* Lvd types (Low Voltage Detector) */
typedef uint8_t LVDR_TripPointType;
typedef uint8_t LVDW_TripPointType;

typedef uint8_t         IO_ErrorType;       /**< Tipo errore. Cfr. #eIO_ERROR per i codici */
typedef uint8_t         IO_DeviceType;
typedef uint8_t         IO_ChannelType;
typedef uint8_t         IO_DeviceStateType;
typedef uint8_t         IO_ModeType;
typedef uint8_t         IO_ValueType;
typedef uint8_t*        IO_MemPtrType;
typedef IO_ValueType    IO_NotificationType;
typedef uint16_t        IO_LenType;
typedef uint8_t         PortIdType; 
typedef uint8_t         PinNumberType; 
typedef bool8_t          PinValueType;
typedef uint8_t			IO_PositionType;

/*--------------------------------- Start Hardware Timers ---------------------------------------*/
typedef uint16_t        IO_TickResType;		/* tick resolution in [nSec]*/
typedef uint32_t        IO_TimingValueType; /* [nSec]*/
/*---------------------------------- End Hardware Timers ----------------------------------------*/

/* TPM Driver */
typedef uint8_t		TPM_TimerNum_t;
typedef uint8_t 	TPM_TimerFreq_t;	/* NOTA: il tipo dipende dall'unità di misura in cui viene passata la frequenza */
typedef uint8_t 	TPM_TimerPrescaler_t;
typedef uint16_t 	TPM_CntModValue_t;
typedef uint16_t 	TPM_ChannelValue_t;
typedef uint8_t   	TPM_ChannelConfig_t;
typedef bool8_t	 	TPM_State_t;


/* SCI Driver */
typedef uint8_t		SCI_PortNum_t;
typedef uint16_t 	SCI_BRDivisor_t;
typedef uint8_t   SCI_Data_t;
typedef bool8_t	 	SCI_State_t;
/* Bus Communication Platform */
typedef uint8_t IO_OptionType;                  /**< Tipo Opzione */

typedef uint8_t IO_OptionSizeType;              /**< Tipo Opzione */

typedef uint16_t       IO_PacketSizeType;         /**< Dimensione di un pacchetto */
typedef IO_MemPtrType IO_BufferType;             /**< Tipo puntatore a buffer  */
typedef uint16_t       IO_BufferSizeType;         /**< Tipo dimensione buffer   */
typedef IO_ValueType  I2C_StatusType;

/** Slave Manager */
typedef struct 
{    
    uint8_t * pData;    /**< puntatore ai dati */
    uint8_t   size;     /**< dimensione in byte */
}IO_BufferInfoType_t;


typedef IO_ErrorType (* Comm_Notification)(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);

typedef struct CommDriverParam_tag
{      
  Comm_Notification    		callback;           /* completed action callback */
  IO_PacketSizeType      len;                /* */
  IO_MemPtrType           buffer;
  IO_OptionType          option;
  I2C_SlaveAddressType    slaveAddress;
} CommDriverParamType;

typedef struct 
{  
  Comm_Notification          eventCB;						/**< callback pointer for channel events */
  IO_MemPtrType              pMaxRxData;
  IO_MemPtrType              rxbuffer;
  IO_ChannelType             id;                /**< channel id */  
}IO_PhyInitSyncType;
typedef struct InOutDriverParam_tag
{      
  PortIdType              PortId; 
  PinNumberType           PinNumber; 
  PinValueType            PinValue;
} InOutDriverParamType;

typedef struct TPMDriverParam_tag {
  TPM_TimerNum_t        	  TimerNum;           /* Timer ID */
  TPM_TimerFreq_t			      Frequency;			    /* Timer frequency source*/
  TPM_TimerPrescaler_t  	  Prescaler;         	/* Timer frequency prescaler */
  TPM_CntModValue_t       	CounterModuloValue; /* Upper counter value */
  TPM_ChannelValue_t    	  ChannelValue; 		  /* Upper channel value */
  TPM_ChannelConfig_t       Config;							/* Channel config */
  TPM_State_t          		  State;				      /* Counter state */
}TPMDriverParam_t;

typedef struct SCIDriverParam_tag {
  SCI_PortNum_t        	  PortNum;            /* SCI Port ID */
  SCI_BRDivisor_t			    BRDivisor;			    /* Baud Rate Divisor */
  SCI_Data_t							Data;								/* Data to Tx or from Rx */
  SCI_State_t          		State;				      /* SCI port state */
}SCIDriverParam_t;



#define pnt_To_IO_ChannelType(pVoid) (*((IO_ChannelType*)(pVoid)))



/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
void IO_ErrorHook (IO_DeviceType device, IO_ErrorType error);
/*@}*/

#endif /* __IODRIVER_H */

/* *********** */
/* End of file */
/* *********** */
