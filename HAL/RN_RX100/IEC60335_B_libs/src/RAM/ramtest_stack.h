/*  DISCLAIMER:                                                               */
/*  We (Renesas Electronics Europe Limited) do not warrant that the Software   */
/*  is free from claims by a third party of copyright, patent, trademark,     */
/*  trade secret or any other intellectual property infringement.             */
/*                                                                            */
/*  Under no circumstances are we liable for any of the following:            */
/*                                                                            */
/*  1. third-party claims against you for losses or damages;                  */
/*  2. loss of, or damage to, your records or data; or                        */
/*  3. economic consequential damages (including lost profits or savings)     */
/*     or incidental damages, even if we are informed of their possibility.   */
/*                                                                            */
/*  We do not warrant uninterrupted or error free operation of the Software.  */
/*  We have no obligation to provide service, defect, correction, or any      */
/*  maintenance for the Software. We have no obligation to supply any         */
/*  Software updates or enhancements to you even if such are or later become  */
/*  available.                                                                */
/*                                                                            */
/*  IF YOU DOWNLOAD OR USE THIS SOFTWARE YOU AGREE TO THESE TERMS.            */
/*                                                                            */
/*  THERE ARE NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE IMPLIED        */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.       */

/***********************************************************************************
FILE NAME  		ramtest_stack.h
DESCRIPTION		This is designed for use on a RX6xx series device.
				RAM memory test of stack.
				The actual memory test is provided by a different module.
				
				NOTE: These functions are not re-entrant.
				Only 1 of these functions must be called at any one time.
				
Copyright   : 2012 Renesas Electronics Europe Ltd.
All Rights Reserved
***********************************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
23-July-2009 RTE PJI
26-Nov-2009	VDE (Version 1.0)
***********************************************************************************/

#ifndef _RAMTEST_STACK_H_INCLUDED_
#define _RAMTEST_STACK_H_INCLUDED_

/***********************************************************************************
 User Includes
***********************************************************************************/
#include "misratypes.h"

/***********************************************************************************
 Typedefs
***********************************************************************************/
/*Function Pointer used to link this module with a particular memory test function.*/
typedef bool_t(*TEST_FUNC)(const uint32_t, const uint32_t, void* const); 

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
bool_t RamTest_Stack_User(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewUSP,
						const TEST_FUNC fpTest_Func);
						
bool_t RamTest_Stack_Int(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewISP,
						const TEST_FUNC fpTest_Func);
						
bool_t RamTest_Stacks(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewISP,
						const uint32_t ui32_NewUSP,
						const TEST_FUNC fpTest_Func);
					

#endif
