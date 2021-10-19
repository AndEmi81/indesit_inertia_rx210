#ifndef __EM_SOFTWARE_TABLE_H
#define __EM_SOFTWARE_TABLE_H

#include "StandardTypes.h"
#include "PM_EnergyMeterSw.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define TABLE_APPROX_VIN    1


#define CHECK_TABLE         1



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct
{
  uint8_t   ui8NCurves;     /* total number of curves */
  uint8_t*  pui8Curves;     /* pointer to first of all curves into Table (in the Settings File) */
  uint8_t   ui8CurvesWLen;  /* size (word) of all curves */
} stTable_t;



#if defined(CONFIG_EMS_TLOAD_ITEMS_MAX)
/* ******************* */
/* Function prototypes */
/* ******************* */

/**
  This function init a Table

\param  [in]      uint8_t*    pui8Table     [pointer to load data table into settings file]
\param  [in]      uint8_t     ui8TableWLen  [size (word) of load data table]
\param  [out]     stTable_t*  psTable       [pointer to data table struct]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
eEMS_Err TableInit (const uint8_t* pui8Table, uint8_t ui8TableWLen, stTable_t* psTable);


/**
  This function calculates the power consumption, with Tables, based on Yx and Vin

\param  [in]      stTable_t*  psTable     [pointer to data table struct]
\param  [in]      uint16_t    ui16Yx      [the value of temperature]
\param  [in]      uint16_t    ui16Vin     [the value of Vin]
\param  [out]     uint16_t*   pui16Power  [pointer to the power calculated]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\bug    [none]

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
eEMS_Err TablePower (const stTable_t* psTable, uint16_t ui16Yx, uint16_t ui16Vin, uint16_t* pui16Power);

#endif /* ENABLE_TABLE_LOAD */

#endif /* __EM_SOFTWARE_TABLE_H */
