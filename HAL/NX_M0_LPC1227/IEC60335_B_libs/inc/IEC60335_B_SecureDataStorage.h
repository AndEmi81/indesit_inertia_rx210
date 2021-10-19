/****************************************************************************
 *   $Id:: IEC60335_B_SecureDataStorage.h 7460 2011-06-01 11:53:27Z gerritd#$
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for secure data storage.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/

#ifndef __CB_SDS_
#define __CB_SDS_


/*! \file IEC60335_B_SecureDataStorage.h
	\todo describe interrupt behaviour. 
*/

/*! \brief typedef for a critical floating point variable with 64 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_FLOAT64
{
	FLOAT64_ data;		/*!< Data content of the critical variable */
	FLOAT64_ mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_FLOAT64;

/*! \brief typedef for a critical floating point variable with 32 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_FLOAT32
{
	FLOAT32 data;		/*!< Data content of the critical variable */
	FLOAT32 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_FLOAT32;

/*! \brief typedef for a critical unsigned integer with 64 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_UINT64
{
	UINT64 data;		/*!< Data content of the critical variable */
	UINT64 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_UINT64;

/*! \brief typedef for a critical signed integer with 64 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_INT64
{
	INT64 data;			/*!< Data content of the critical variable */
	INT64 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_INT64;

/*! \brief typedef for a critical unsigned integer with 32 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_UINT32
{
	UINT32 data;		/*!< Data content of the critical variable */
	UINT32 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_UINT32;

/*! \brief typedef for a critical signed integer with 32 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_INT32
{
	INT32 data;			/*!< Data content of the critical variable */
	INT32 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_INT32;

/*! \brief typedef for a critical unsigned integer with 16 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_UINT16
{
	UINT16 data;		/*!< Data content of the critical variable */
	UINT16 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_UINT16;

/*! \brief typedef for a critical signed integer with 16 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_INT16
{
	INT16 data;			/*!< Data content of the critical variable */
	INT16 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_INT16;

/*! \brief typedef for a critical unsigned integer with 8 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_UINT8
{
	UINT8 data;			/*!< Data content of the critical variable */
	UINT8 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_UINT8;

/*! \brief typedef for a critical signed integer with 8 bits.
	
	This variable has to be checked with the macro \a IEC60335_CriticalDataCheck(criticalVar)
	\see IEC60335_CriticalDataCheck(criticalVar)

	\attention Do only use the macros to read or write the varible.
	\see IEC60335_CriticalDataRead(criticalVar)
	\see IEC60335_CriticalDataWrite(criticalVar, value)

	\attention To instance such a critical variable, the macro \a IEC60335_CriticalDataInitialise(value)
	has to be used to initialise the struct.
*/
typedef struct tag_secured_INT8
{
	INT8 data;			/*!< Data content of the critical variable */
	INT8 mirror;		/*!< The complement of the data content of the critical variable */
} type_secured_INT8;

/*! \brief Macro to check critical variables.
	
	\param criticalVar 
*/
#define IEC60335_CriticalDataCheck(criticalVar)\
		((criticalVar.data == ~(criticalVar.mirror))? (IEC60335_testPassed) : (IEC60335_testFailed) )

/*! \brief Macro to read critical variables.
	
	\param criticalVar 
*/
#define IEC60335_CriticalDataRead(criticalVar) (criticalVar.data)

/*! \brief Macro to write critical variables.
	
	\param criticalVar 
	\param value
*/
#define IEC60335_CriticalDataWrite(criticalVar, value)\
		{\
			criticalVar.data = value;\
			criticalVar.mirror = ~value;\
		}

/*! \brief Macro to initialise critical variables.
	
	\todo describe, that it is neccesarry to initialise every type!
	\param value
*/
#define IEC60335_CriticalDataInitialise(value) ({value, ~value)}

#endif
