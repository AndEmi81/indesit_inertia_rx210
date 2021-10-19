/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/





/**
\n
\par		PM_Linkedloads.c
\n          it contains shared functionalities between the different type of linked loads
\file		PM_Linkedloads.c
\ingroup	PM_LinkedLoads
\date		Oct 4, 2010 
\version	00.00.67 
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.67
\n		04/10/2010
\n		Roberto Fioravanti 
\n		changes 
\n 		- creation
*/
#define MODULE_BODY_LINKEDLOADS
/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "LIB_Error.h"
#include "PM_LinkedLoads.h"
#include "DigitalOutputs.h"
#include "OutputManagerExp.h"
#include "LIB_ClassBVar.h"
#include "Lib_Utilities.h"
#include "LIB_ZeroCrossingExp.h"
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#include "LoadsRequestExp.h"
#endif /*  defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* properties */
#if defined (CONFIG_LINKED_LOADS_NUM)
#define LINKED_LOADS_NUM 			CONFIG_LINKED_LOADS_NUM
#endif /* defined (CONFIG_LINKED_LOADS_NUM) */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

/*lint -e676*/
static int8_t LinkedLoads_NextIndex = LINKED_LOADS_NUM-1;	    /* Next index. Range:   from: -1 (or FFh = full)
                                                                                        to LINKED_LOADS_NUM-1 (= empty)  */
/*lint +e676*/

static PM_LinkedLoadsType LinkedLoads[LINKED_LOADS_NUM];

#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
struct Loader TriacRelay_OutputTablePtr;
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */


#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
uint8_t TriacRelay_Ready;
static uint8_t OutputPin_Set[IO_MODULE_MAP_SIZE/8   + IO_MODULE_MAP_SIZE%8];
#endif  /* !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
void PM_LinkedLoads_SetPinValue (LinkedLoads_PinType pin, uint8_t state);
void PM_LinkedLoads_ApplyPinValue (PM_LinkedLoadsType* pLinkedLoad);
uint8_t PM_LinkedLoads_GetPinValue (LinkedLoads_PinType pin);
#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
static uint8_t LinkedLoads_FindIdx(LinkedLoads_LoadType load_id);
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
module initialization: internal structures.

\return 	none

\attention  This method is influenced by the following properties:
\li LINKED_LOADS_PINDRIVINGONTHEFLY further data initialization
\li RELAY_LOAD_ENABLE: loading of data from the pointer PTR_IO_CONFIG_DATA

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
void PM_LinkedLoads_Init(void)
{
    PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*)NULL;
	int8_t idx;
	SAFETY_REGION_START(); /* big safty region because is only in initializazion */
	for(idx = LINKED_LOADS_NUM-1 ; idx >= 0; idx--)
	{
	    pLinkedLoad=&LinkedLoads[idx];
		/* state init */
		pLinkedLoad->state.all = TRIAC_RELAY_INIT_OK;

  		/* load value */
		pLinkedLoad->NewValue = OFF;
		pLinkedLoad->Value = OFF;
		
		/* counter */
		pLinkedLoad->cnt=0;	
		
		/* default load id */
		pLinkedLoad->loadid = LINKED_LOADS_DEFAULT_LOADID;
		/* timings */
		pLinkedLoad->timing[0]=pLinkedLoad->timing[1]=0;

		SAFETY_DATA_ELEM_UPDATE( LinkedLoads, idx );
	}

	/*init linked loads reverse counter*/
	LinkedLoads_NextIndex = LINKED_LOADS_NUM-1; /*MS*/
	SAFETY_DATA_UPDATE(LinkedLoads_NextIndex);
	SAFETY_REGION_END();

#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
	(void)memset(&OutputPin_Set[0], 0, sizeof(OutputPin_Set));
	TriacRelay_Ready = 0;
#endif /*defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)*/
	
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
	(void)BasicLoader(SF_PTR_IO_CONFIG, EEP_OUTPUT_TABPTR_OFFSET, &TriacRelay_OutputTablePtr);
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */

}

/**
This methods, called every 10 ms, detects linked loads (de)activation.

\return 	none

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
void PM_LinkedLoads_Manager(void)
{
	uint8_t load_cmd, cfg;
	int8_t idx, maxLoads;
	uint8_t relay_old_status;
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*)NULL;
	
#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
	if (TriacRelay_Ready)
	{
		return;
	}
		
	(void)memset(&OutputPin_Set[0], 0, sizeof(OutputPin_Set));
#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

    /* begin to process the last load */
	idx = LINKED_LOADS_NUM-1; 
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( LinkedLoads_NextIndex );
	maxLoads = LinkedLoads_NextIndex;
	SAFETY_REGION_END();

	while(idx > maxLoads)
	{
		pLinkedLoad = &LinkedLoads[idx];
		
        if((pLinkedLoad->cfg & LINKEDLOADS_DONT_CARE) == 0)
        {/* begin process load */

    		SAFETY_REGION_START();
    		SAFETY_DATA_ELEM_CHECK( LinkedLoads, idx );
			/* current cfg */
			cfg = pLinkedLoad->cfg;

			/* get relay old status */
			relay_old_status = (uint8_t)(pLinkedLoad->Value & LINKED_LOADS_2ON);

			/* by default turn off primary and secondary loads of current value */
			pLinkedLoad->Value &= LINKED_LOADS_CFG_1OFF_2OFF;

			if( pLinkedLoad->NewValue != (pLinkedLoad->Value&0x0F))
			{
				/* reset counter */
				pLinkedLoad->cnt = 0;
				/* activate the request */
				pLinkedLoad->state.all  |= RELAY_REQUEST;
				/* update current value */
				pLinkedLoad->Value = (uint8_t)((pLinkedLoad->NewValue & 0x0F) | (pLinkedLoad->Value & 0xF0));
			}


			/* get cmd */
			if(pLinkedLoad->Value & 0x0F)
			{
				load_cmd=ON;
			}
			else
			{
				load_cmd=OFF;
			}

			if(pLinkedLoad->state.all & RELAY_REQUEST)
			{
				/* process the new request */

				if( pLinkedLoad->primaryLoad != LINKED_LOADS_DUMMY_PIN )
				{
					/* It's not a DigitalDelayed */
					if(cfg & (LINKEDLOADS_RELAYLOAD | LINKEDLOADS_RELAYTRIAC)) /*Not a triac+relay*/
					{
						if(load_cmd)
						{
							if(pLinkedLoad->cnt < pLinkedLoad->timing[0])
							{
							/* first load must be ON */
							#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

								LIB_DIO_SET_LOAD(ON, pLinkedLoad->primaryLoad);

							#endif	/* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
								pLinkedLoad->Value |= LINKED_LOADS_1ON;
							}
							else
							{
								if (!(cfg & LINKEDLOADS_NOSECONDARYLOAD))
								{

								#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
									if (cfg & LINKEDLOADS_RELAYLOAD)
									{
										L_PARAM lparam = { ON };

										if(SET_LOAD_NO_ERROR != LIB_IOM_SetLoad(pLinkedLoad->secondaryLoad & 0x7F, lparam))
										{
											PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_SET_LOAD);
										}

									}
									else
								#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
									{
										LIB_DIO_SET_LOAD(ON, pLinkedLoad->secondaryLoad);
									}

									pLinkedLoad->Value |= LINKED_LOADS_2ON;
									pLinkedLoad->state.all &= (~(RELAY_REQUEST));

								}
							}/*End if timing[0]*/
						}/*End load_cmd=1*/
						else /*if (!load_cmd)*/
						{
							if(pLinkedLoad->cnt < pLinkedLoad->timing[1])
							{

							#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

								if (!(cfg & LINKEDLOADS_NOSECONDARYLOAD))
								{

								#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
									if (cfg & LINKEDLOADS_RELAYLOAD)
									{
										L_PARAM lparam = {0};
										lparam.s.reg = OFF;
										if(SET_LOAD_NO_ERROR != LIB_IOM_SetLoad(pLinkedLoad->secondaryLoad & 0x7F, lparam))
										{
											PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_SET_LOAD);
										}

									}

									else
								#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */

									{

										LIB_DIO_SET_LOAD(OFF, pLinkedLoad->secondaryLoad);

									}
									pLinkedLoad->Value &= (LINKED_LOADS_2OFF);


								}
							#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

							}
							else
							{
								#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
									LIB_DIO_SET_LOAD(OFF, pLinkedLoad->primaryLoad);
								#endif	/* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
								pLinkedLoad->Value &= (LINKED_LOADS_1OFF);
								pLinkedLoad->state.all &= (~(RELAY_REQUEST));
							}
						}
					} /*End not triac+relay*/
					else /*if triac+relay*/
					{
						if(pLinkedLoad->cnt < (pLinkedLoad->timing[0] + pLinkedLoad->timing[1]))
						{
						/* first load must be ON */
						#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

							LIB_DIO_SET_LOAD(ON, pLinkedLoad->primaryLoad);

						#endif	/* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
							pLinkedLoad->Value |= LINKED_LOADS_1ON;
							if(pLinkedLoad->cnt >= pLinkedLoad->timing[0])
							{
								if(!load_cmd)
								{

									LIB_DIO_SET_LOAD(OFF, pLinkedLoad->secondaryLoad);
									pLinkedLoad->Value &= LINKED_LOADS_2OFF;
								}
								else
								{
									LIB_DIO_SET_LOAD(ON, pLinkedLoad->secondaryLoad);
									pLinkedLoad->Value |= LINKED_LOADS_2ON;

								}
							}
						}
						else
						{
						#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

							LIB_DIO_SET_LOAD(OFF, pLinkedLoad->primaryLoad);

						#endif	/* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
							pLinkedLoad->Value &= (LINKED_LOADS_1OFF);
							pLinkedLoad->state.all &= (~(RELAY_REQUEST));
						}

					}
				}
				else
				{
					/* It's digital delayed */
					if(
					/* LOAD OFF->ON , use the first threshold !!!*/
					((load_cmd == ON) && (pLinkedLoad->cnt >= pLinkedLoad->timing[0]))
					||
					/* LOAD ON->OFF , use the second threshold !!!*/
					((load_cmd == OFF) && (pLinkedLoad->cnt >= pLinkedLoad->timing[1]))
					)
					{
						/*set status of the controlled load */
					#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
						LIB_DIO_SET_LOAD(load_cmd, pLinkedLoad->secondaryLoad);
					#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

						if(load_cmd)
						{
							pLinkedLoad->Value |= LINKED_LOADS_2ON;
						}
						/* delete the request */
						pLinkedLoad->state.all &= (~(RELAY_REQUEST));
					}
					else
					{
						if(relay_old_status)
						{
							pLinkedLoad->Value |= LINKED_LOADS_2ON;
						}
					}
				}

				/* increment the counter */
				pLinkedLoad->cnt++;
			}
			else
			{

				if( pLinkedLoad->primaryLoad != LINKED_LOADS_DUMMY_PIN )
				{
					if (!(cfg & LINKEDLOADS_RELAYTRIAC))
					{
					#if (defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY))
						/* Controller must be OFF */
						LIB_DIO_SET_LOAD(OFF, pLinkedLoad->primaryLoad);
					#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
					}
				}

				if (!(cfg & LINKEDLOADS_NOSECONDARYLOAD))
				{
				#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

				#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
					if ((cfg & LINKEDLOADS_RELAYLOAD))
					{
						L_PARAM lparam = {0};
						lparam.s.reg = load_cmd;

						if(SET_LOAD_NO_ERROR != LIB_IOM_SetLoad(pLinkedLoad->secondaryLoad & 0x7F,lparam))
						{
							PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_SET_LOAD);
						}

					}
					else
				#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
					{
						LIB_DIO_SET_LOAD(load_cmd, pLinkedLoad->secondaryLoad);
					}
				#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

					if(load_cmd)
					{
						pLinkedLoad->Value |= LINKED_LOADS_2ON;
					}
				}

				if ((cfg & LINKEDLOADS_RELAYTRIAC))
				{

				#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
					LIB_DIO_SET_LOAD(load_cmd, pLinkedLoad->primaryLoad);
				#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

					if(load_cmd)
					{
						pLinkedLoad->Value |= LINKED_LOADS_1ON;
					}
				}
			}

			if ((cfg & LINKEDLOADS_NOSECONDARYLOAD))
			{
			#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

			#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
				if ((cfg & LINKEDLOADS_RELAYLOAD))
				{
					L_PARAM lparam = {0};
					lparam.s.reg = OFF;

					if(SET_LOAD_NO_ERROR != LIB_IOM_SetLoad(pLinkedLoad->secondaryLoad & 0x7F,lparam))
					{
						PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_SET_LOAD);
					}
				}
				else
			#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
				{
					LIB_DIO_SET_LOAD(OFF, pLinkedLoad->secondaryLoad);
				}
			#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

				pLinkedLoad->Value &= (LINKED_LOADS_2OFF);
			}
			
			if ((cfg & LINKEDLOADS_NOPRIMARYLOAD))
			{
			#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
				pLinkedLoad->Value &= (LINKED_LOADS_1OFF);
			#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
			}

			
		#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
		#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
			if ((cfg & LINKEDLOADS_RELAYLOAD))
			{
				L_PARAM lparam = {0};
				lparam.s.reg = (pLinkedLoad->Value & LINKED_LOADS_2ON) ? ON : OFF;
				LIB_IOM_SetLoad(pLinkedLoad->secondaryLoad, lparam);

			}
			else
		#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
			{
				PM_LinkedLoads_SetPinValue(pLinkedLoad->secondaryLoad, (uint8_t)(pLinkedLoad->Value & (LINKED_LOADS_2ON)));
			}

			if(LinkedLoads[idx].primaryLoad != LINKED_LOADS_DUMMY_PIN)
			{
				PM_LinkedLoads_SetPinValue(pLinkedLoad->primaryLoad, (uint8_t)(pLinkedLoad->Value & (LINKED_LOADS_1ON)));
			}
			
		#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

	        SAFETY_DATA_ELEM_UPDATE( LinkedLoads, idx );
	        SAFETY_REGION_END();

        } /* end process load */

        idx--;

	} /* end while */
	
#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
	TriacRelay_Ready = 1;		
#endif	/* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
}


/**
Function for output manager. It allows to make a driving request for the load_id.

\param [in] load_on desired state of the linked load
    \li 0: deactivation
    \li 1: activation
\param [in] load_id	 load id of the load

\return 	none

\attention  This method is influenced by the following properties:
\li LOAD_DONT_CARE if (load_on & 0x80) load won't be driven
\li LINKED_LOADS_PINDRIVINGONTHEFLY: 
\li CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT

\attention load_on format:
        cfg = high nibble = load_on & LOAD_CFG_MASK
        val = low nibble = load_on & LOAD_REG_MASK

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
void PM_LinkedLoads_SetLoad(uint8_t load_on, uint8_t load_id)
{

	uint8_t new_load_value;
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*)NULL;
	uint8_t linkedLoadIdx;
	bool_t isDontCare;
	


#if     !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
	uint8_t set_all_loads = 0;
#endif  /*!defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

	/* search for load id */
	/*MS pLinkedLoad = LinkedLoads_Find2(load_id); */
	linkedLoadIdx = LinkedLoads_FindIdx(load_id);
	if(linkedLoadIdx == 0xFF)
	{
		    /*raise error Load not found*/
	    PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND);
		return;
	}
	/*Load found!*/
	pLinkedLoad = &LinkedLoads[linkedLoadIdx];

	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( LinkedLoads, linkedLoadIdx );

	if(!(pLinkedLoad->state.all & TRIAC_RELAY_INIT_OK))
	{
		SAFETY_REGION_END();
	    /* not initialized */
	    PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_NOT_INIT);
		return;
	}
					
    /* load dont care management */
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
	isDontCare = (LIB_IOM_FindLoadOnSwitchRelay(load_id) & (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE | LOAD_SR_SAME_POS)) == (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE);
#else
	isDontCare = (load_on & LOAD_CFG_DONT_CARE) != 0;
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */

	if (isDontCare)
    {
        pLinkedLoad->cfg |= LINKEDLOADS_DONT_CARE;
    }
    else
    {
        pLinkedLoad->cfg &= ~(LINKEDLOADS_DONT_CARE);
    }
    
    /* update configuration based on load_on cfg nibble value */				
	if((load_on & LOAD_CFG_MASK) == LINKED_LOADS_1ON)
	{
	    pLinkedLoad->cfg |= LINKEDLOADS_NOSECONDARYLOAD;
		load_on = (uint8_t)!(load_on & 0x0F);
	}
	else
	{
	    pLinkedLoad->cfg &= ~(LINKEDLOADS_NOSECONDARYLOAD);
	}
	
	if((load_on & LOAD_CFG_MASK) == LINKED_LOADS_2ON)
	{
	    pLinkedLoad->cfg |= LINKEDLOADS_NOPRIMARYLOAD;
	}
	else
	{
	    pLinkedLoad->cfg &= ~(LINKEDLOADS_NOPRIMARYLOAD);
	}
	
	
	
	/*update new value of the load from the val field of load_on argument */
	pLinkedLoad->NewValue = (uint8_t)(load_on & LOAD_REG_MASK);
	
	if(!goodZCSignal())     /*TODO: must be available good ZC signal*/
	{
	    /* get new value keeping on the current configuration */
		new_load_value = (uint8_t)((pLinkedLoad->NewValue & LOAD_REG_MASK) | (pLinkedLoad->Value & LOAD_CFG_MASK));
		
		/* check if new value is changed */                  
		if (pLinkedLoad->Value != new_load_value)
		{
		    /* load value is changed */
			pLinkedLoad->Value = new_load_value;
		
		#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
			set_all_loads = 1;
			(void)memset(&OutputPin_Set[0], 0, 8);
		#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
		
		}
        
        /* delete request */		
		pLinkedLoad->state.all &= (~(RELAY_REQUEST));
		
		/* update the configuration field in the current value */
		if(load_on & LOAD_REG_MASK)
		{
			pLinkedLoad->Value |= LINKED_LOADS_CFG_1ON_2ON;
		}
		else
		{
			pLinkedLoad->Value &= LINKED_LOADS_CFG_1OFF_2OFF;
		}
		
		
	#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)
		/* recycle new_load_value to get the new value for the connected load */
		new_load_value = (pLinkedLoad->Value & LINKED_LOADS_2ON) ? ON : OFF;
		
		
		/* begin of secondary load management */
	#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
	
	    if( pLinkedLoad->cfg & LINKEDLOADS_RELAYLOAD )
		{
		    PM_LinkedLoadsType* pLinkedLoad2 = LinkedLoads_Find2(pLinkedLoad->secondaryLoad);
			if(!pLinkedLoad2)
			{
			  SAFETY_REGION_END();
			  PLT_ERROR_WARNING(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND2);
			  return;
			}

			SAFETY_DATA_ELEM_CHECK(LinkedLoads, pLinkedLoad->secondaryLoad);

			if(new_load_value)
			{
				pLinkedLoad2->Value |= LINKED_LOADS_CFG_1ON_2ON;				
			}
			else
			{
				pLinkedLoad2->Value &= LINKED_LOADS_CFG_1OFF_2OFF;
			}
			
			LIB_DIO_SET_LOAD(new_load_value, pLinkedLoad2->secondaryLoad);
			
			if( pLinkedLoad2->primaryLoad != LINKED_LOADS_DUMMY_PIN )			
			{
			    /* not a DD load */
				LIB_DIO_SET_LOAD(new_load_value,pLinkedLoad2->primaryLoad);
			}

			SAFETY_DATA_ELEM_UPDATE(LinkedLoads, pLinkedLoad->secondaryLoad);

		}
		else
	#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
	    
		{
			LIB_DIO_SET_LOAD(new_load_value, pLinkedLoad->secondaryLoad);
		}
		
		/* end of secondary load management */
		
		/* primary load management: the primary load state follows the secondary load one */	
		if( pLinkedLoad->primaryLoad != LINKED_LOADS_DUMMY_PIN )
		{
			LIB_DIO_SET_LOAD(new_load_value, pLinkedLoad->primaryLoad);
		}
		
	#else /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
		
		if (set_all_loads)
		{
		    int8_t i = (int8_t)(LinkedLoads_NextIndex + 1);
			/* ascending order processing beacause in case of Relay + Load  pilotType, the relay + load 
			must be processed before the linked load */
			
			
			while(i < LINKED_LOADS_NUM)
			{
			#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
				
				if (LinkedLoads[i].cfg & LINKEDLOADS_RELAYLOAD)
				{
				    PM_LinkedLoadsType* pLinkedLoad2 = NULL;
        			pLinkedLoad2 = LinkedLoads_Find2(LinkedLoads[i].secondaryLoad);
        			
        			if(!pLinkedLoad2)
        			{
        			    /* not found */
						PLT_ERROR_WARNING(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND2);
			            return;
        			}
        			
					if(LinkedLoads[i].Value & LINKED_LOADS_2ON)
					{
						pLinkedLoad2->Value |= LINKED_LOADS_CFG_1ON_2ON;
					}
					else
					{
						pLinkedLoad2->Value &= LINKED_LOADS_CFG_1OFF_2OFF;
					}
				}
				else
			#endif
				{
					PM_LinkedLoads_SetPinValue(LinkedLoads[i].secondaryLoad, (uint8_t)(LinkedLoads[i].Value & (LINKED_LOADS_2ON)));
				}
				if(LinkedLoads[i].primaryLoad != LINKED_LOADS_DUMMY_PIN)
				{
					PM_LinkedLoads_SetPinValue(LinkedLoads[i].primaryLoad, (uint8_t)(LinkedLoads[i].Value & (LINKED_LOADS_1ON)));
				}
				
				i++;
			}
		}
		PM_LinkedLoads_ApplyPinValue(pLinkedLoad);
		#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */
			
	}
	SAFETY_DATA_ELEM_UPDATE( LinkedLoads, linkedLoadIdx );
	SAFETY_REGION_END();
}



/**
Returns the maximum value for for the specified timingid

\param [in] timingid
    \li 0: first delay 
    \li 1: second delay
    
    IDs of timings are in the same orders of those passed in \ref PM_LinkedLoads_RelayLoad_NewLoad,
    \ref PM_LinkedLoads_RelayTriac_NewLoad, \ref PM_LinkedLoads_TriacRelay_NewLoad, \ref PM_LinkedLoads_DigitalDelayed_NewLoad.
    
    For the meaning of the timings, see \ref PM_LinkedLoads_RelayLoad_NewLoad,
    \ref PM_LinkedLoads_RelayTriac_NewLoad, \ref PM_LinkedLoads_TriacRelay_NewLoad, \ref PM_LinkedLoads_DigitalDelayed_NewLoad.
    
     
\return 	uint8_t
\retval     maximum value for timingid

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
uint8_t PM_LinkedLoads_GetMaxTiming(uint8_t timingid)
{
	uint8_t max = 0;
	int8_t idx = LINKED_LOADS_NUM-1;
	
	if(timingid >= PM_LINKEDLOADS_NUM_OF_TIMINGS)
	{
	    return 0;
	}
	
	while(idx > LinkedLoads_NextIndex)
	{
		if(LinkedLoads[idx].timing[timingid] >= max)
		{
			max = LinkedLoads[idx].timing[timingid];
		}
		
		idx--;
	}
	
	return max;
}

/**
Returns timing value for for specified linked load and its status

\param [in] loadPos		- Load position of the linked load requested
\param [in] status		- Load status
    
     
\return 	uint8_t
\retval     maximum delay value

\author     Alessandro Sabbatini
\date		11/09/2013
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t PM_LinkedLoads_GetTiming(uint8_t loadPos, uint8_t status)
{
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*) NULL;
	uint8_t max = 0;

	/* search for load id */
	pLinkedLoad = LinkedLoads_Find2(loadPos);

	if (pLinkedLoad)
	{
		/* Digital Delayed - show delay according to load status */
		if(pLinkedLoad->primaryLoad == (LinkedLoads_LoadType)LINKED_LOADS_DUMMY_PIN )
		{
			max = (status ? pLinkedLoad->timing[0] : pLinkedLoad->timing[1]);
		}
		/* Triac + Relay */
		else if ( (pLinkedLoad->cfg & ( LINKEDLOADS_RELAYLOAD | LINKEDLOADS_RELAYTRIAC)) == 0)
		{
			max = (status ? 0 : ( pLinkedLoad->timing[0] + pLinkedLoad->timing[1]));
		}
		/* Relay + Load / Relay + Triac */
		else
		{
			max = 0;
		}
	}
		
	return max;
}



/**
Returns the load state given the load id

\param [in] load_id load id of the load you want to know th state
     
\return 	uint8_t
\retval     bitmap give by any combination of \ref LL_1ON and \ref LL_2ON 
            

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
uint8_t PM_LinkedLoads_GetLoadState(LinkedLoads_LoadType load_id)
{
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*) NULL;
	uint8_t state = 0;

	/* search for load id */
	pLinkedLoad = LinkedLoads_Find2(load_id);

	if (!pLinkedLoad)
	{
		/* Linked load not found - it's OFF by its definition */
		PLT_ERROR_WARNING(PLT_ERROR_LINKEDLOADS_LOAD_NOT_FOUND);
		return 0;
	}

	/*Load found!*/

#if defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

	if (pLinkedLoad->Value & LINKED_LOADS_1ON)
	{
		state |= LL_1ON;
	}

	if (pLinkedLoad->Value & LINKED_LOADS_2ON)
	{
		state |= LL_2ON;
	}

#else /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

	if(PM_LinkedLoads_GetPinValue(pLinkedLoad->primaryLoad))
	{
		state |= LL_1ON;
	}

#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)

	cfg = pLinkedLoad->cfg & LINKEDLOADS_RELAYLOAD;

	if(cfg)
	{
		if(pLinkedLoad->Value & LINKED_LOADS_2ON)
		{
			state |= LL_2ON;
		}

	}
	else
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
	if(PM_LinkedLoads_GetPinValue(pLinkedLoad->secondaryLoad))
	{
		state |= LL_2ON;
	}

#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */

	return state;
}


#if !defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY)

void PM_LinkedLoads_SetPinValue (LinkedLoads_PinType pin, uint8_t state)
{
	uint8_t lindex = (uint8_t)((pin) / 8);
	uint8_t pin_to_set = (uint8_t)((pin) % 8);
	
	if (state)
	{
		OutputPin_Set[lindex] |= (1 << pin_to_set);
	}
	
}

uint8_t PM_LinkedLoads_GetPinValue (LinkedLoads_PinType pin)
{	
	
	uint8_t lindex = (uint8_t)(pin / 8);
	
	pin = (uint8_t)(pin % 8);
	
	if (((OutputPin_Set[lindex] >> pin) & 0x01))
	{
		return ON;
	}
	else
	{
		return OFF;
	}
}

void PM_LinkedLoads_ApplyPinValue (PM_LinkedLoadsType* pLinkedLoad)
{	
    if(!pLinkedLoad)
    {
        return;
    }
    
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
	if(!(pLinkedLoad->cfg & LINKEDLOADS_RELAYLOAD))
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
	{
		LIB_DIO_SET_LOAD(PM_LinkedLoads_GetPinValue(pLinkedLoad->secondaryLoad), pLinkedLoad->secondaryLoad);
	}			
	if (pLinkedLoad->primaryLoad != LINKED_LOADS_DUMMY_PIN)
	{
		LIB_DIO_SET_LOAD(PM_LinkedLoads_GetPinValue(pLinkedLoad->primaryLoad),pLinkedLoad->primaryLoad); 	
	}
}





/**
Linked loads driver. Apply pin values
   
\return 	none

\attention  avilable if LINKED_LOADS_PINDRIVINGONTHEFLY is defined as 0

\author     Roberto Fioravanti
\date		04/10/2010
\version	1.0.0

  
\note		\par \b Nota 1:
			documentation by Roberto Fioravanti
*/
void PM_LinkedLoads_Driver(void)
{

	if (TriacRelay_Ready)
	{
		
		int8_t idx = LINKED_LOADS_NUM-1; 
		
		while(idx > LinkedLoads_NextIndex)
		{
    
            if( LinkedLoads[idx].cfg & LINKEDLOADS_DONT_CARE)
            {
                idx--;
                continue;
            }
			
    		PM_LinkedLoads_ApplyPinValue(&LinkedLoads[idx]);	
    		idx--;
		}
		
		TriacRelay_Ready = 0;		
	}
}


#endif /* defined(CONFIG_LINKED_LOADS_PINDRIVINGONTHEFLY) */


/* local functions */
/*
searches the first occurrence of load_id in the linked loads table
*/
PM_LinkedLoadsType* LinkedLoads_Find2(LinkedLoads_LoadType load_id)
{
	int8_t idx;
	PM_LinkedLoadsType* pLL = (PM_LinkedLoadsType*) NULL;

	for (idx = LINKED_LOADS_NUM - 1; idx > LinkedLoads_NextIndex; idx--)
	{
		pLL = &LinkedLoads[idx];

		if (pLL->loadid == load_id)
		{
			break;
		}
	}

	if (idx <= LinkedLoads_NextIndex)
	{
		pLL = (PM_LinkedLoadsType*) NULL;
	}

	return pLL;
}

static uint8_t LinkedLoads_FindIdx(LinkedLoads_LoadType load_id)
{
	uint8_t idx;

	for (idx = LINKED_LOADS_NUM - 1; idx > LinkedLoads_NextIndex; idx--)
	{
		if (LinkedLoads[idx].loadid == load_id)
		{
			break;
		}
	}

	if (idx <= LinkedLoads_NextIndex)
	{
		idx = 0xFF;
	}

	return idx;
}


/*
searches the first occurrence of load_id in the linked loads table
*/
PM_LinkedLoadsType* LinkedLoads_FindSecondaryLoad(LinkedLoads_LoadType load_id)
{
	int8_t idx;
	PM_LinkedLoadsType* pLL = (PM_LinkedLoadsType*) NULL;

	for (idx = LINKED_LOADS_NUM - 1; idx > LinkedLoads_NextIndex; idx--)
	{
		pLL = &LinkedLoads[idx];

		if (pLL->secondaryLoad == load_id)
		{
			break;
		}
	}

	if (idx <= LinkedLoads_NextIndex)
	{
		pLL = (PM_LinkedLoadsType*) NULL;
	}

	return pLL;
}


/*
inserts a new linked load if not present in the table and if the table is not full
*/
void PM_LinkedLoads_NewLoad(LinkedLoads_LoadType primaryload,
		LinkedLoads_LoadType secondaryload, uint8_t load_id,
		LinkedLoads_TimingType firstdelay, LinkedLoads_TimingType seconddelay,
		uint8_t cfg)
{
	PM_LinkedLoadsType* pLinkedLoad = (PM_LinkedLoadsType*) NULL;

	if (LinkedLoads_NextIndex < 0)
	{
		/* no room */
		PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_NO_ROOM);
		return;
	}

	pLinkedLoad = LinkedLoads_Find2(load_id);

	if (pLinkedLoad)
	{
		/* load id already inserted */
		PLT_ERROR_FATAL(PLT_ERROR_LINKEDLOADS_LOAD_ALREADY_PRESENT);
		return;
	}

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(LinkedLoads_NextIndex);
	SAFETY_DATA_ELEM_CHECK( LinkedLoads, LinkedLoads_NextIndex );
	/* position of the current load */
	pLinkedLoad = &LinkedLoads[LinkedLoads_NextIndex];

	/* update load_id */
	pLinkedLoad->loadid = load_id;

	/*Update loads linked with the load id */
	pLinkedLoad->primaryLoad = primaryload;
	pLinkedLoad->secondaryLoad = secondaryload;
	pLinkedLoad->cfg = cfg;

	/* Updates timings */
	pLinkedLoad->timing[0] = firstdelay;
	pLinkedLoad->timing[1] = seconddelay;

	SAFETY_DATA_ELEM_UPDATE( LinkedLoads, LinkedLoads_NextIndex );
	/* move Triac_Relay_Multi_MasterIndex to the next position: FFh means array is full */
	LinkedLoads_NextIndex--;

	SAFETY_DATA_UPDATE(LinkedLoads_NextIndex);
	SAFETY_REGION_END();
}

/* *********** */
/* End of file */
/* *********** */

