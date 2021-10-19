/***********************************************************************/
/*                                                                     */
/*  FILE        :UIPlatformDefines.h                                   */
/*  DATE        :28.02.2006		                                       */
/*  DESCRIPTION :defines generali e indipendenti della piattaforma     */
/*  CPU TYPE    :Generic                                               */
/*  AUTHOR		:Gabriele Merlonghi                                    */
/*                                                                     */
/*  Copyright (c) 2005-2006, Indesit Company, EDT.                     */
/*  Tutti i diritti sono riservati. All rights reserved.               */
/*                                                                     */
/***********************************************************************/
/*
 \\UIPlatform_Modules\General

 ver.     date       author				description
 ------  ----------  ------------------	-----------------------------------------
 1.0.0	28/02/2006	Gabriele Merlonghi  sono state scorporate dal file UISkeletonDefine.h
 le defines più generali che sono indipendenti dalla
 configurazione della piattaforma e del prodotto
 2.0.0	03/09/2007	Gabriele Merlonghi	Riorganizzazione generale della piattaforma visual,
 aggiunte le defines delle combinazioni timers-canali
 */

#ifndef __UIPLATFORM_DEFINES_H
#define __UIPLATFORM_DEFINES_H

/* ------------ DEFINES ------------------- */
//Layers dell'interprete dei microcomandi
#define COMMAND_LAYER_0		0
#define COMMAND_LAYER_1		1
#define COMMAND_LAYER_2		2
#define COMMAND_LAYER_3		3

//Targets
#define TARGET0				0
#define TARGET1				1
#define TARGET2				2
#define TARGET3				3

//Intestazione Ciclo
#define IC_PROGRAMMING		0
#define IC_RUN				1
#define IC_DELAY			2
#define IC_END				3
#define IC_PAUSE			4
#define IC_FAULT			5

//Sub Intestazione Ciclo
#define SIC_GENERIC			0
#define SIC_PIRO			1
#define SIC_AUTOTEST		2
#define SIC_COLLAUDO		3
#define SIC_DEMO			4
#define SIC_MANUAL			5
#define SIC_AUTOMATIC		6
#define SIC_RISING			7
#define SIC_ANTISHOCK		8
#define SIC_ANTIFOAM		9
#define SIC_PUMP_OUT		10
#define SIC_GRILL			13	//per compatibilità con le vecchie UI in cui c'era solo SIC_MANUAL
//Stati Generali del prodotto
#define STATUS_PROGRAMMING	IC_PROGRAMMING
#define STATUS_RUN			IC_RUN
#define STATUS_DELAY		IC_DELAY
#define STATUS_END			IC_END
#define STATUS_PAUSE		IC_PAUSE
#define STATUS_FAULT		IC_FAULT
#define	STATUS_UIOFF		6	// stato di OFF logico nel display: la main è in programming
#define	STATUS_INVALID		7	// invalid data: i dati del pacchetto di stato sono invalidi
#define ID_GENERIC			0
#define ID_SELECTOR			1
#define ID_START			2
#define ID_TEMPERATURE		3
#define ID_CLOCK_HH			4
#define ID_CLOCK_MM			5
#define ID_DURATION_HH		6
#define ID_DURATION_MM		7
#define ID_ENDCOOK_HH		8
#define ID_ENDCOOK_MM		9
#define ID_COUNTER_HH		10
#define ID_COUNTER_MM		11
#define ID_STOP				12
#define ID_MODE				13
#define ID_SELECT			14
#define ID_POWER			15
#define ID_CHILDLOCK		16
#define ID_SWITCH_ONOFF		17
#define ID_ESCAPE			18
#define ID_DELAY_HH			19
#define ID_DELAY_MM			20
#define ID_SETUP			21
#define ID_CONFIRM			22
#define ID_DEMOMODE			23
#define ID_DOORLOCK			24
#define ID_LAMP				25
#define ID_3IN1				26
#define ID_EXTRADRY			27
#define ID_BASKET			28
#define ID_MODE2			29
#define ID_LANGUAGE			30
#define ID_SOUND			31
#define ID_CLOCKENABLE		32
#define ID_PYROLISESAUTO	33
#define ID_REMOTECONTROL	34
#define ID_PHASE_ICON       35
#define ID_PHASE_STRING     36
#define ID_SPIN             37
#define ID_RINSE            38
#define ID_HOWDIRTY         39
#define ID_DRYINGLEVEL      40
#define ID_TIMEDDRY         41
#define ID_HIGHHEAT         42
#define ID_STAINREMOVAL     43
#define ID_PREWASH          44
#define ID_SUPERWASH        45
#define ID_IRONMIN          46
#define ID_RAPID            47
#define ID_MINILOAD         48
#define ID_SOFTRINSE        49
#define ID_EXTRARINSE       50
#define ID_BLEACH           51
#define ID_PRECARE          52
#define ID_POSTCARE         53
#define ID_CONDUCTIVITYSNSOR 	54
#define ID_WASHENHANCE      	55
#define ID_SPINEXCLUSION    	56
#define	ID_WATER_HARDNESS		57
#define ID_NIGHT_OPTION			58
#define	ID_TIME_SHORTENING		59
#define	ID_EXTENSION			60
#define	ID_HOB_POWER_LEVEL		61
#define ID_BOOSTER_TURBO		62
#define ID_DIRECT_SELECT		63
#define ID_SELECT_MODE			64
#define ID_SELECT_PUSH			65
#define ID_DIRECT_SELECT_MODE	66
#define ID_DIRECT_SELECT_PUSH	67
#define ID_CAVITY				68
#define ID_MEMORYSAVE			69
#define ID_MEMORYLOAD			70
#define ID_SUPER			71
#define ID_SUPER_48			72
#define ID_ECO				73
#define ID_HOLIDAY			74
#define ID_AUTOTEST			75
#define ID_TEST				76
#define ID_HTF					77
#define ID_BOOST				79
#define ID_MEMORYSAVE2			80
#define ID_MEMORYLOAD2			81
#define ID_MEMORYSAVE3			82
#define ID_MEMORYLOAD3			83
#define ID_LFDETERGENT			84
#define ID_LFSOFTNER			85
#define ID_LFDILUCONC			86
#define ID_RESET_ALARMS		87
#define ID_WSSUGGESTDETERGENT	88
#define ID_LFCLEANINGDETERGENT  89
#define ID_LFCLEANINGSOFTNER	90	
#define ID_STEAM				91
#define ID_ICEPARTY			92
#define ID_EVERFRESH		93
#define ID_STANDBY			94
#define ID_COOLCAREZONE		95
#define ID_CYCLEVARIABLE_1	96
#define ID_CYCLEVARIABLE_2	97
#define ID_CYCLEVARIABLE_3	98
#define ID_CLECOPLUS		101

#define	mask_IDFunc		0x7F00
#define mask_toMain		0x8000
#define mask_updown		0x0008
#define mask_push		0x0010
#define mask_indirect	0x0020
#define mask_target		0x00C0
#define mask_uniqueID	((word)(mask_IDFunc|mask_push|mask_indirect|mask_target)) //è escluso solo il flag toMain e il flag updown
#define mask_target0	0x0000	//seleziona applicazione0 (nel forno corrisponde alla main cavity)
#define mask_target1	0x0040	//seleziona applicazione1 (nel forno corrisponde alla top cavity)
#define ID_func_regulPLUS_T0(ID)	(((word)ID<<8)|mask_target0) //è escluso solo il flag toMain
#define ID_func_regulPLUS_T1(ID)	(((word)ID<<8)|mask_target1) //è escluso solo il flag toMain
#define ID_func_regulMINUS_T0(ID)	(((word)ID<<8)|mask_target0|mask_updown) //è escluso solo il flag toMain
#define ID_func_regulMINUS_T1(ID)	(((word)ID<<8)|mask_target1|mask_updown) //è escluso solo il flag toMain
//Comandi sulla funzione start
#define DONT_CARE_COND		1
#define START_COND			2
#define RESET_COND			3
#define PAUSE_COND			4
#define RESET_FAULT			5
#define RESTART_COND		6

//Oggetti visual
#define TYP_ICON			0
#define TYP_BAR				1
#define TYP_DIGIT			2
#define TYP_STRING			3

//Gestore degli eventi
enum
{
	EVENT_NULL = 0,
	EVENT_FUNCTION_NOT_ACTIVE,
	EVENT_FUNCTION_DISABLED,
	EVENT_FUNCTION_REGULATED,
	EVENT_FUNCTION_UNCHANGEABLE,
	EVENT_NO_ANY_FUNCTION,
	EVENT_FL_DISABLE,
	EVENT_KEY_SEQUENCE_ON,
	EVENT_FUNCTION_FIRST_NOT_MODIFIED,
	EVENT_FUNCTION_NOT_ENABLE
};

//Basetempi
#define BASETIME_SHORT 10		//multiplo della base tempi corta
#define BASETIME_LONG 500		//multiplo della base tempi lunga
//Definizioni di uso generale
#define	FALSE	0
#define	TRUE	1

#define ON		1
#define OFF		0

#define BYTE_SIZE	8
#define WORD_SIZE	16

#ifndef NULL
#define NULL	((void *) 0)
#endif

//Manufacturer
#define MCU_FREESCALE				0
#define MCU_RENESAS					1

//Freescale Models
#define MODEL_S08GB60				0	// Motorola
#define MODEL_S08GB32				1	// Motorola
#define MODEL_S08GT32				2	// Motorola
#define MODEL_S08GT16				3	// Motorola
#define MODEL_S08GT60				4	// Motorola
#define MODEL_S08QE128			5	// Motorola
//Renesas Models
#define MODEL_H83687				0	// Renesas
#define MODEL_H839064				1	// Renesas
#define MODEL_H839014				2	// Renesas
#define MODEL_R8C27					3	// Renesas
#define MODEL_H836077               4   // Renesas
#define MODEL_H836079               5   // Renesas
#define MODEL_R8C2B					6	// Renesas
//MCU Core Models
#define CORE_HCS08					0	// Core Freescale HCS08
#define CORE_R8C					1	// Core Renesas R8C Tiny
#define CORE_H8						2	// Core Renesas H8 Tiny
//Appliance Models
#define APPLIANCE_CL				0	// Cooling
#define APPLIANCE_DW				1	// Diswasher
#define APPLIANCE_WM				2	// Wash Machine
#define APPLIANCE_CK				3	// Coking
#define APPLIANCE_DR				4	// Dryer
#define APPLIANCE_HO				5	// Hobs
#define NOTAPPLIACE         15 //Not Appliance
//Unique Code Visual Fw
#define	CK_DOUBLE_OVEN				15
#define CK_BIG_CAVITY				16
#define	DW_INDESIT_BASIC			26
#define DW_INDESIT_PREMIUM			27

//Board
#define BOARD_MAIN					  0
#define BOARD_DISPLAY				  1
#define BOARD_EXPANSION			  2
#define BOARD_COMM_INTERFACE  3

//Evolution
#define HWINTERFACE_RS232			0 // Indoor Communication Node (HW Key with Indesit Platfrom Protocol)


//Maximum setting file array size
#define EEPPAGE_512					0
#define EEPPAGE_1024				1
#define EEPPAGE_2048				2
#define EEPPAGE_4096				3
#define EEPPAGE_8192				4
#define EEPPAGE_16384				5
#define EEPPAGE_32768				6
#define EEPPAGE_65536				7

//Maximum Indent Table Global lenght
#define IDT_GLOBAL_LEN				48

#define VALUE_MAX_IN_MIN_COUNTER14	136
#define TIMEOUT_F12_DISPL			5
#define TIMEOUT_F12_DEFAULT			300000	//timeout di default del fault di comunicazione espresso in ms
//tabella delle frequenze discrete ammesse
#define F10MHZ	10		//indica una frequenza di clock impostata a circa/uguale 10 MHz
#define F16MHZ	16		//indica una frequenza di clock impostata a circa/uguale 16 MHz
#define F20MHZ	20		//indica una frequenza di clock impostata a circa/uguale 20 MHz
//tabella delle possibili combinazioni di impiego dei timers
#define	TIM1CH0	1
#define	TIM1CH1	2
#define	TIM2CH0	3
#define	TIM2CH1	4
#define	TIM2CH2	5
#define	TIM2CH3	6
#define	TIM2CH4	7

//tabella degli operatori logici utilizzati nella valutazione delle condizioni sulle funzioni
enum
{
	OP_UGUALE=0,
	OP_DIVERSO,
	OP_MAGGIORE,
	OP_MINORE
};

//tabella dei connettivi logici utilizzati nella valutazione delle condizioni sulle funzioni
enum
{
	CN_OR=0,
	CN_AND
};

#define I2C_ADDR_MAIN	0x90    /* I2C address of the main contro board */
#define I2C_ADDR_UI 	0x9E    /* I2C address of the user interface board */
#define I2C_ADDR_KEY	0x96    /* I2C address of the external communication key */

/* ------------ MACROS -------------------- */
/* Calcolano correttamente il valore con cui caricare i timers */
#define TIMER_SH(ms)	(((ms)/BASETIME_SHORT)-1)	//da un tempo in millisecondi genera il valore a cui occorre inizializzare il timer short
#define TIMER_LN(ms)	(((ms)/BASETIME_LONG)-1)	//da un tempo in millisecondi genera il valore a cui occorre inizializzare il timer long
/* Definiscono il bit i-esimo da utilizzare come argomento nei metodi */
/* Possono essere utilizzati anche più bits posti in OR */
#define MASK8(i)		((byte)1<<i)	//genera una maschera a byte con un bit nella posizione i-esima
#define MASK16(i)		((word)1<<i)	//genera una maschera a word con un bit nella posizione i-esima
#define minValue(a,b) (((a)<=(b))?(a):(b))
#define maxValue(a,b) (((a)>(b))?(a):(b))

#define	TAKE_HIGHER_BYTE(VAL)	((byte)((VAL>>8) & 0xFF))
#define	TAKE_LOWER_BYTE(VAL)	((byte)(VAL & 0xFF))

/* Generic Macro */
#define LOBYTE(value)			((byte)((word)(value) & 0x00FF))
#define HIBYTE(value)			((byte)((word)(value) >> 8))
#define LONIBBLE(value)			((byte)((byte)(value) & 0x0F))
#define HINIBBLE(value)			((byte)((byte)(value) >> 4))
#define SWAP_WORD(value)		(((word)LOBYTE(value) << 8) | (word)HIBYTE(value))

/* ------------ ERRORS -------------------- */

#endif //__UIPLATFORM_DEFINES_H
