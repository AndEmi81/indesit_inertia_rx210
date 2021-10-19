/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Feedbacks.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		Feedbacks.c
\ingroup	LIB_Feedbacks
\date		Nov 2, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.01
\n		Apr 18, 2010
\n		stefano.ughi
\n		changes
\n         - in LIB_FDB_Manager() added support for Zero Crossing Voltage signal on I.C.U. pin
\n
\n 		01.00.00
\n		Nov 2, 2010
\n		stefano.ughi
\n		changes
\n          Creation
\n
*/

#define MODULE_BODY_LIB_FEEDBACKS
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include <string.h>
#include <stdlib.h>
#include "Feedbacks.h"
#include "OCDIO_drv.h"
#include "OCTPM_drv.h"
#include "OCIRQ_drv.h"
#include "DigitalInputs.h"
#include "LIB_Error.h"
#include "InputManagerExp.h"
#include "IOSharing.h"
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** All feedbacks are not ready */
#define FDB_ALL_ARE_NOT_READY   ((feedbackBitfield_t)0u)
/** Reading of all feedbacks is enabled */
#define FDB_ALL_READ_ENABLED    ((feedbackBitfield_t)0u)
/** Feedback's value is indeterminate */
#define FDB_INDETERMINATE       0xFF

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*lint -esym( 754,FDB_descriptorType_tag::w ) w not referenced */
/*lint -esym( 754,free ) free not referenced */

/**
Feedback's descriptor
*/
typedef union FDB_descriptorType_tag
{
    /** word's access */
    uint16_t w;
    /** bit's access */
    struct
    {
        /** If '1' the feedback is updated */
        uint8_t active:1;
        /** If '1' feedback read through multiplexer if '0' direct */
        uint8_t readtype:1;
        /** Multiplexer's configuration for feedback's reading */
        uint8_t chsel:3;
        /** Pull-up enable */
        uint8_t pullupEn:1;
        /** Unused */
        uint8_t free:2;
        /** If '1' feedback is linked to a partializated load */
        uint8_t loadP:1;
        /** Pin of the linkage module minus 1 linked to the feedback */
        uint8_t pin:6;
        /** If '1' reading inverted */
        uint8_t actvsense:1;
    } s;
} FDB_descriptorType;

typedef struct
{
    uint8_t cntSamples;
    int8_t cntVal;
    int8_t cntZCmatch;
    unsigned int  ready: 1; /*  indicating if feedback is ready */
    unsigned int  readyWritable: 1;
    unsigned int  zcCompare: 1;
} FDB_StatusType;

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
/** The output of the module */
uint8_t Feedback[N_FEEDBACK];
/** Feedback's Descriptor */
feedbackBitfield_t FeedbackInputDisable;
static FDB_descriptorType FeedbackDescriptor[N_FEEDBACK];
FDB_StatusType FeedbackStatus[N_FEEDBACK];

/**
 * LIB_FDB_FeedbackInitMeasure
 * \param  [in] pos feedback position described inside Setting File
 *
 **/
void LIB_FDB_FeedbackInitMeasure(uint8_t pos)
{
    if ( pos < N_FEEDBACK )
    {
        SAFETY_REGION_START();
        SAFETY_DATA_ELEM_CHECK( FeedbackStatus, pos);

        /* Disable feedback's reading */
        FeedbackStatus[pos].cntSamples = 0;
        FeedbackStatus[pos].cntZCmatch = 0;
        FeedbackStatus[pos].cntVal = 0;
        FeedbackStatus[pos].readyWritable = 0;
        FeedbackStatus[pos].zcCompare = 1;

        SAFETY_DATA_ELEM_UPDATE( FeedbackStatus, pos );
        SAFETY_REGION_END();
    }
    else
    {
        PLT_ERROR_FATAL(PLT_ERROR_FEEDBACK_2);
    }
}

/**
LIB_FDB_AreFeedbackReady
This function returns TRUE if all active feedbacks are ready otherwise FALSE.

\return 	bool_t

\retval		TRUE all active feedbacks are ready\n
            FALSE at least one feedback is not ready

\author 	Stefano Ughi
\date		Nov. 08, 2010
\version	1.0.0

*/
bool_t LIB_FDB_AreFeedbackReady(void)
{
    uint8_t i;

    for ( i = 0; i < N_FEEDBACK ; i++ )
    {
        if ( FeedbackDescriptor[i].s.active )
        {
            if ( FeedbackStatus[i].ready  == 0 )
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/** TODO: LIB_FDB_InitSync doc */
void LIB_FDB_InitSync(void)
{
    int i;

    SAFETY_REGION_START();

    FeedbackInputDisable = FDB_ALL_READ_ENABLED;

    (void)memset(&FeedbackDescriptor[0], 0, sizeof(FeedbackDescriptor));

    (void)memset(&FeedbackStatus[0], 0, sizeof(FDB_StatusType));

    for(i=0; i<N_FEEDBACK; i++)
    {
        SAFETY_DATA_ELEM_UPDATE( Feedback, i );
        SAFETY_DATA_ELEM_UPDATE( FeedbackStatus,i);
        SAFETY_DATA_ELEM_UPDATE( FeedbackDescriptor, i);
    }
    SAFETY_DATA_UPDATE( FeedbackInputDisable );
    SAFETY_REGION_END();
}

/**
LIB_FDB_GetFeedbackVal
This function returns feedback's value on the position "pos"

\param  [in]    pos feedback's position, the position is that of setting's file.

\return     uint8_t

\retval     FDB_INPUT_LOW
\retval     FDB_INPUT_HI
\retval     FDB_INPUT_FREQ_OPP_PHASE
\retval     FDB_INPUT_FREQ_PHASE

\author 	Stefano Ughi
\date		Nov. 08, 2010
\version	1.0.0

*/
uint8_t LIB_FDB_GetFeedbackVal(uint8_t pos)
{
    return Feedback[pos];
}

/**
LIB_FDB_GetFeedbackInput
This function, fixed feedback's position, returns the value of the pin linked to the feedback.

\param  [in] pos feedback's position, the position is that of setting's file.

\return 	bool_t

\retval		TRUE pin is high
\retval     FALSE pin is low

\author 	Stefano Ughi
\date		Nov. 08, 2010
\version	1.0.0

*/
bool_t LIB_FDB_GetFeedbackInput(uint8_t pos)
{
    return LIB_DIO_GET_INPUT(FeedbackDescriptor[pos].s.pin);
}

/**
 * LIB_FDB_IsFeedbackReady
This function returns TRUE if feedback in position "pos" was acquired at least once.

\param  [in]    pos feedback's position, the position is that of setting's file.

\return 	bool_t

\retval		TRUE   feedback in position "pos" was acquired at least once
\retval		FALSE  feedback in position "pos" was not acquired at least once

*/
bool_t LIB_FDB_IsFeedbackReady(uint8_t pos)
{
    return (bool_t)FeedbackStatus[pos].ready;
}

/**
 * LIB_FDB_RegisterFeedback
\n Setup of a single feedback channel.
\n This function is called inside LIB_IOM_InputInitialization()  at cold reset.

\attention \b FeedbackDescriptor[] (global) used W

\param  [in]	pos Ram Position Conversion Result vs vector Feedback[]
\param  [in]	pin pin of the linkage module minus 1 linked to the feedback
\param  [in]	readtype '0' -> direct feedback \n
						 '!0' -> multiplexed feedback
\param  [in]	pConfigData pointer to the description feedback area


\author 	stefano.ughi
\date		Nov 05, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			if feedack is not multiplexed actvsense and pullupEn are set in LIB_IOM_InputInitialization()
*/
void LIB_FDB_RegisterFeedback( uint8_t pos, uint8_t pin, uint8_t readtype, const uint8_t *const pConfigData )
{
    FDB_descriptorType *pFeedbackpin;

    if ( pos < N_FEEDBACK )
    {
        SAFETY_REGION_START();
        SAFETY_DATA_ELEM_CHECK( FeedbackDescriptor, pos);

        pFeedbackpin = &FeedbackDescriptor[pos];
        /* multiplexed feedback or direct ?  */
        pFeedbackpin->s.readtype = readtype;
        pFeedbackpin->s.pin = pin;
        /* feedback linked to partializated load ? */
        pFeedbackpin->s.loadP = ((feedbackLLIparams_t*)pConfigData)->bit.partialized;

#if defined __MULTI_H
        /* multiplexer's configuration for feedback's reading */
        pFeedbackpin->s.chsel = ((feedbackMpxLLIparams_t*)pConfigData)->bit.mpxCfg;
        /* inversion of reading ? */
        pFeedbackpin->s.actvsense = ((feedbackMpxLLIparams_t*)pConfigData)->bit.inverted;
        /* pull-up ? */
        pFeedbackpin->s.pullupEn = ((feedbackMpxLLIparams_t*)pConfigData)->bit.pullUp;
#endif  /* #if defined __MULTI_H */

        /* Feedback's reading always active */
        pFeedbackpin->s.active = 1;

        SAFETY_DATA_ELEM_UPDATE( FeedbackDescriptor, pos);
        SAFETY_REGION_END();
    }
    else
    {
        PLT_ERROR_FATAL(PLT_ERROR_FEEDBACK_1);
    }
}

/**
LIB_FDB_Manager
\n Feedbacks's Manager.
\n This function performs feedbacks's acquisition.
\n The output of the function is Feedback[], possibles value for Feedback[] are:
\n \li FDB_INPUT_LOW
\n \li FDB_INPUT_HI
\n \li FDB_INPUT_FREQ_OPP_PHASE
\n \li FDB_INPUT_FREQ_PHASE

\param  [in]    loadType    if '0' read feedbacks linked to partialized loads \n
			                if '1' read feedbacks linked to non partialized loads.
\param  [in]	zcIsAvailable   if '0' zero crossing voltage not available \n
                                if '1' zero crossing voltage available

\n Below the scheme of feedback's update
\n <b>
\n \li case zcIsAvailable=FALSE
\n
\n -FeedbackVal[i]:
\n
\n       low
\n -------------|    (-FDB_NO_ZC_LIMIT_FOR_LEVEL)
\n        no decision
\n |-----------------------| (-FDB_NO_ZC_LIMIT_FOR_FREQ)
\n           freq
\n |-----------------|  0
\n           freq
\n |-----------------------| (+FDB_NO_ZC_LIMIT_FOR_FREQ)
\n           no decision
\n |------------|    (FDB_NO_ZC_LIMIT_FOR_LEVEL)
\n            high
\n |---------------
\n
\n however with signals whose properties is not known acquisition might lead to indeterminate state.
\n To avoid indeterminate state FDB_NO_ZC_LIMIT_FOR_FREQ could be set @ a value greater than FEEDBACK_SAMPLES
\n in this way
\n       low
\n -------------|    (-FDB_NO_ZC_LIMIT_FOR_LEVEL)
\n           freq
\n |-----------------|  0
\n           freq
\n |------------|    (FDB_NO_ZC_LIMIT_FOR_LEVEL)
\n            high
\n |---------------
\n
\n \li case zcIsAvailable=TRUE
\n
\n -FeedbackZCmatch[i]:
\n
\n   freq not in phase
\n ---------------------|  (-FDB_ZC_LIMIT_FOR_FREQ)
\n     no decision
\n |----------------|  0
\n       no decision
\n |--------------------|  (FDB_ZC_LIMIT_FOR_FREQ)
\n      freq in phase
\n |---------------------
\n
\n
\n
\n -FeedbackVal[i]:
\n
\n         low
\n -------------------------|  (-FDB_ZC_LIMIT_FOR_LEVEL)
\n      no decision
\n |--------------------|  0
\n      no decision
\n |-------------------------|  (+FDB_ZC_LIMIT_FOR_LEVEL)
\n       high
\n |-------------------------
\n
\n
\n </b>

\author 	Stefano Ughi
\date		Nov. 08, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by Stefano Ughi

*/
void LIB_FDB_Manager(bool_t loadType, bool_t zcIsAvailable)
{
    feedbackBitfield_t feedbackMask;
    uint8_t i;
    uint8_t tmpFeedbackValue;
    bool_t pinval;
    bool_t zcSyncEdge;
#if defined(ZCV_CH_ID)
    ICU_EdgeModeType mode;
#endif


    for ( i = 0; i < N_FEEDBACK ; i++ )
    {
        /* SAFETY REGION START */
        /* check consistency FeedbackDescriptor, FeedbackStatus, Feedbacks
         *
         */

        SAFETY_REGION_START();

        SAFETY_DATA_ELEM_CHECK( Feedback, i);
        SAFETY_DATA_ELEM_CHECK( FeedbackStatus, i);
        SAFETY_DATA_ELEM_CHECK( FeedbackDescriptor, i);
        SAFETY_DATA_CHECK( FeedbackInputDisable );

        /* Check if the feedback is active and is of the loadType to read */
        if ( FeedbackDescriptor[i].s.active && (FeedbackDescriptor[i].s.loadP == loadType) )
        {
#ifdef __MULTI_H
        	bool_t activeSense;

            if (FeedbackDescriptor[i].s.readtype)
            {
                /* This is a Multiplexed Feedback */

                if ( FeedbackDescriptor[i].s.pullupEn )
                {
                    (void)OCDIO_SetPinPullUpSync(FeedbackDescriptor[i].s.pin);
                }
                else
                {
                    (void)OCDIO_ResetPinPullUpSync(FeedbackDescriptor[i].s.pin);
                }
                activeSense = (bool_t)(FeedbackDescriptor[i].s.actvsense?1:0);
                LIB_IOM_Set_IODir_ActiveSense(activeSense,FeedbackDescriptor[i].s.pin);

                /* Select multiplexer's channel */
                PM_setInMux(FeedbackDescriptor[i].s.chsel);
            }

#endif  /* #ifdef __MULTI_H */

            /* Get the Feedback value according with inversion bit */
            pinval = LIB_DIO_GET_INPUT( FeedbackDescriptor[i].s.pin );

            /* Count the values */
            if (pinval)
            {
                FeedbackStatus[i].cntVal++;
            }
            else
            {
                FeedbackStatus[i].cntVal--;
            }

            /* If Zero Crossing Voltage is available */
            if ( TRUE == zcIsAvailable )
            {
                /* Check the matching with the ZC signal */
                zcSyncEdge = LIB_ZC_GetCurrentEdge();

                /*
                   I am waiting for positive edge so I am on the low level of zero crossing and I read '0'
                   or
                   I am waiting for negative edge so I am on the high level of zero crossing and I read '1'
                */
                if (((IRQ_ON_POSITIVE_EDGE == zcSyncEdge) && (FALSE == pinval)) ||
                        ((IRQ_ON_NEGATIVE_EDGE == zcSyncEdge) && (TRUE == pinval)))
                {
                    /* Match -> feedback in phase with zero crossing voltage */
                    FeedbackStatus[i].cntZCmatch++;
                }
                else
                {
                    /* No match -> feedback in phase opposition respect zero crossing voltage */
                    FeedbackStatus[i].cntZCmatch--;
                }
            }

            if ( FALSE == zcIsAvailable )
            {
                FeedbackStatus[i].zcCompare = 0;
            }

            /* Update num. samples Counter  */
            FeedbackStatus[i].cntSamples++;

            /* Check if time to decide feedback's state */
            if ( FeedbackStatus[i].cntSamples >= FEEDBACK_SAMPLES )
            {
                /* No value for feedbacks */
                tmpFeedbackValue = FDB_INDETERMINATE;

                if ( FeedbackStatus[i].zcCompare == 0 )
                {
                    /* Zero Crossing Voltage Not Available */
                    if ( FeedbackStatus[i].cntVal >= FDB_NO_ZC_LIMIT_FOR_LEVEL)
                    {
                        /* feedback is high */
                        tmpFeedbackValue = FDB_INPUT_HI;
                    }
                    else if ( FeedbackStatus[i].cntVal <= -FDB_NO_ZC_LIMIT_FOR_LEVEL)
                    {
                        /* feedback is low */
                        tmpFeedbackValue = FDB_INPUT_LOW;
                    }
                    else if ( abs(FeedbackStatus[i].cntVal) <= FDB_NO_ZC_LIMIT_FOR_FREQ)
                    {
                        /* frequency on feedback */
                        tmpFeedbackValue = FDB_INPUT_FREQ_PHASE;
                    }
                    else
                    {
                        /* nothing to do - do not modify ! */
                    }
                }
                else
                {
                    /* Zero Crossing Voltage Available */
                	if ( FeedbackStatus[i].cntVal >= FDB_ZC_LIMIT_FOR_LEVEL )
					{
							/* high value on feedback */
							tmpFeedbackValue = FDB_INPUT_HI;
					}
					else if ( FeedbackStatus[i].cntVal <= -FDB_ZC_LIMIT_FOR_LEVEL )
					{
						/* low value on feedback */
						tmpFeedbackValue = FDB_INPUT_LOW;
					}
					else if ( FeedbackStatus[i].cntZCmatch >= FDB_ZC_LIMIT_FOR_FREQ )
                    {
                        /* frequency in phase respect zero crossing voltage on feedback */
                        tmpFeedbackValue = FDB_INPUT_FREQ_PHASE;
                    }
                    else if ( FeedbackStatus[i].cntZCmatch <= -FDB_ZC_LIMIT_FOR_FREQ )
                    {
                        /* frequency in opposition phase respect zero crossing voltage on feedback */
                        tmpFeedbackValue = FDB_INPUT_FREQ_OPP_PHASE;
                    }
                    else
                    {
                        /* no decision ! */
                    }
                }

                feedbackMask = (feedbackBitfield_t)(1u<<i);
                /* Process tmpFeedbackValue and update Feedback[] if feedback is not disabled */
                if ( ( FeedbackInputDisable & feedbackMask ) == 0 )
                {
                    if ( tmpFeedbackValue != FDB_INDETERMINATE )
                    {
                        Feedback[i] = tmpFeedbackValue;
                    }
                }

                FeedbackStatus[i].ready 			= 1;
                FeedbackStatus[i].readyWritable 	= 1;
                FeedbackStatus[i].zcCompare 		= 1;
                FeedbackStatus[i].cntZCmatch       	= 0;
                FeedbackStatus[i].cntVal 			= 0;
                FeedbackStatus[i].cntSamples 		= 0;

            }

        }

        SAFETY_DATA_ELEM_UPDATE( Feedback, i);
        SAFETY_DATA_ELEM_UPDATE( FeedbackStatus, i);
        SAFETY_DATA_ELEM_UPDATE( FeedbackDescriptor, i);
        SAFETY_DATA_UPDATE( FeedbackInputDisable );

        SAFETY_REGION_END();
        /* SAFETY REGION END */
    }

}

/**
LIB_FDB_IsFeedbackWritableReady
This function returns TRUE if feedback in position "pos" was acquired.
\n The function acts on FeedbackReadyWritable.

\param  [in]    pos feedback's position, the position is that of setting's file.

\return 	bool_t

\retval		TRUE
\retval		FALSE
*/

bool_t LIB_FDB_IsFeedbackWritableReady(uint8_t pos)
{
    return (bool_t)FeedbackStatus[pos].readyWritable ;
}

/**
This function returns a bitmap of all feedbacks acquired.

\param  	none

\return 	feedbackBitfield_t

\retval		FeedbackReadyWritable bitmap


\author 	Alessandro Sabbatini
\date		Dec. 12, 2012
\version	1.0.0
*/
feedbackBitfield_t LIB_FDB_GetFeedbackWritableBitMap(void)
{
	uint8_t i;
	feedbackBitfield_t res = 0;

	for (i = 0; i < N_FEEDBACK; i++)
	{
		if (FeedbackStatus[i].readyWritable) {
			res |= ( ((feedbackBitfield_t)1) << i );
		}
	}

	return res;
}

/* *********** */
/* End of file */
/* *********** */

