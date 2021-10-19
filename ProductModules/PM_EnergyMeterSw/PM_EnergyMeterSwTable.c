
#include "PM_EnergyMeterSwTable.h"
#include "Config.h"

#if defined(CONFIG_EMS_TLOAD_ITEMS_MAX)
#include "PM_EnergyMeterSwTable_pri.h"

#include <string.h> /* memset */



/* ******************* */
/* Function prototypes */
/* ******************* */


/**
  This function find, in the table, the Y (Temperature) curves including the Yx value

\param  [in]      stTable_t*          psTable     [pointer to data table struct]
\param  [in]      uint16_t            ui16Yx      [the value of temperature]
\param  [out]     stTableSurfaceY_t*  psSurfaceY  [pointer to Y surface]
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
STATIC eEMS_Err TableFindYCurves (const stTable_t* psTable, uint16_t ui16Yx,
                                  stTableSurfaceY_t* psSurfaceY);


/**
  This function computes the power based on Vin and a Y curve

\param  [in]      stTable_t*        psTable     [pointer to data table struct]
\param  [in]      uint16_t          ui16Vin     [the value of Vin]
\param  [in]      stTableCurveY_t*  psCurveY    [pointer to Y curve]
\param  [out]     uint16_t*         pui16Power  [pointer to the power calculated]
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
STATIC eEMS_Err TableCalibSingola (const stTable_t* psTable, uint16_t ui16Vin,
                                   const stTableCurveY_t* psCurveY, uint16_t* pui16Power);


/**
  This function computes the power based on Yx, Vin and the curves Y1 and Y2

\param  [in]      stTable_t*          psTable     [pointer to data table struct]
\param  [in]      uint16_t            ui16Yx      [the value of temperature]
\param  [in]      uint16_t            ui16Vin     [the value of Vin]
\param  [in]      stTableSurfaceY_t*  psSurfaceY  [pointer to Y surface]
\param  [out]     uint16_t*           pui16Power  [pointer to the power calculated]
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
STATIC eEMS_Err TableCalibDoppia (const stTable_t* psTable, uint16_t ui16Yx, uint16_t ui16Vin,
                                  const stTableSurfaceY_t* psSurfaceY, uint16_t* pui16Power);


/**
  This function Init/Reset a Y curve

\param  [in]      none
\param  [out]     stTableCurveY_t*  psCurveY    [pointer to Y curve]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
STATIC void CurveInit (stTableCurveY_t* psCurveY);


STATIC void LineInit (stCurveYLine_t* psYLine);

STATIC void PointInit (stCurveYPoint_t* psYPoint);

STATIC void PointSet (uint16_t ui16V, uint16_t ui16Pw,
                      stCurveYPoint_t* psYPoint);


STATIC eEMS_Err TableCalibDoppia_Punti (uint16_t ui16Yx,
                                        uint16_t ui16Y1, uint16_t ui16P1,
                                        uint16_t ui16Y2, uint16_t ui16P2,
                                        uint16_t* pui16Power);

STATIC eEMS_Err TableCalibDoppia_PuntoRetta (uint16_t ui16Yx, uint16_t ui16Vin,
                                             uint16_t ui16Y1, uint16_t ui16P1,
                                             uint16_t ui16Y2, const stCurveYLine_t* psY2Line,
                                             uint16_t* pui16Power);

STATIC eEMS_Err TableCalibDoppia_Rette (uint16_t ui16Yx, uint16_t ui16Vin,
                                        uint16_t ui16Y1, const stCurveYLine_t* psY1Line,
                                        uint16_t ui16Y2, const stCurveYLine_t* psY2Line,
                                        uint16_t* pui16Power);

STATIC eEMS_Err CalcoloPuntoSuRetta (uint16_t ui16X,
                                     uint16_t ui16X1, uint16_t ui16Z1,
                                     uint16_t ui16X2, uint16_t ui16Z2,
                                     uint16_t* pui16Z);

STATIC void CurveSet (uint16_t ui16Y, const uint8_t* pui8Curves, uint8_t ui8CurveNItems,
                      stTableCurveY_t* psCurveY);


STATIC eEMS_Err CurveYFindPoints (const stTable_t* psTable, const stTableCurveY_t* psCurveY,
                                  uint16_t ui16Vin, stCurveYLine_t* psYLine);



/* ****************** */
/* Exported functions */
/* ****************** */


/*---------------------------------------------
 This function init a Table
 ----------------------------------------------*/
eEMS_Err TableInit (const uint8_t* pui8Table, uint8_t ui8TableWLen, stTable_t* psTable)
{


#if CHECK_TABLE
  if ((pui8Table == NULL) || (ui8TableWLen < EM_DATA_TABLE_HEADER_WLEN) || (psTable == NULL))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* parse N curves */
  psTable->ui8NCurves = pui8Table[EM_DATA_TABLE_NUM_TABLES_OFFSET];

  ui8TableWLen -= EM_DATA_TABLE_HEADER_WLEN;

  /*
   * Imposta gli estremi delle curve
   */

  if ((psTable->ui8NCurves == 0) || (ui8TableWLen == 0))
  {
    psTable->ui8NCurves     = 0;
    /*lint -epp*/
    psTable->pui8Curves     = NULL;
    /*lint +epp*/
    psTable->ui8CurvesWLen  = 0;
  }
  else
  { /* (psTable->ui8NCurves > 0) && (ui8TableWLen > 0) */
    psTable->pui8Curves     = (uint8_t*)&pui8Table[2 * EM_DATA_TABLE_HEADER_WLEN];
    psTable->ui8CurvesWLen  = ui8TableWLen;


  }

  return EMS_E_OK;
}


/*---------------------------------------------
 This function calculates the power consumption
 with Tables.
 ----------------------------------------------*/
eEMS_Err TablePower (const stTable_t* psTable, uint16_t ui16Yx, uint16_t ui16Vin, uint16_t* pui16Power)
{
  stTableSurfaceY_t sSurface;
  uint8_t ui8Result;


#if CHECK_TABLE
  if ((psTable == NULL) || (pui16Power == NULL))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

 #if TABLE_APPROX_VIN
  ui16Vin >>= 8;
 #endif

  /* init curves */
  CurveInit(&sSurface.sCurveY1);
  CurveInit(&sSurface.sCurveY2);

  /* cerca le curve che includono Yx */
  ui8Result = TableFindYCurves(psTable, ui16Yx, &sSurface);
  if ((ui8Result != EMS_E_OK) || (sSurface.sCurveY1.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* curva Y1 valida */

  /* effettua le calibrazioni */
  if (sSurface.sCurveY2.ui8Valid == 0)
  { /* curva Y2 non valida: calibrazione singola con Vin e curva Y1 */
    return TableCalibSingola(psTable, ui16Vin, &sSurface.sCurveY1, pui16Power);
  }
  else
  { /* curva Y2 valida: calibrazione doppia con Vin, Yx e le curve Y1 e Y2 */
    return TableCalibDoppia(psTable, ui16Yx, ui16Vin, &sSurface, pui16Power);
  }
}


/* ****************** */
/* Static functions */
/* ****************** */


/*------------------------------------
 Init/Reset a Y curve.
 -------------------------------------*/
void CurveInit (stTableCurveY_t* psCurveY)
{
#if CHECK_TABLE
  if (psCurveY == NULL)
  {
    return;
  }
#endif

  memset(psCurveY, 0, sizeof(stTableCurveY_t));
}


/**
  This function set a Y curve

\param  [in]      uint16_t          ui16Y         [value of Y]
\param  [in]      uint8_t*          pui8Curves    [pointer to current Y curve data area]
\param  [in]      uint8_t           ui8CurveBytes [size of current Y curve data area]
\param  [out]     stTableCurveY_t*  psCurveY      [pointer to Y curve]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
STATIC void CurveSet (uint16_t ui16Y, const uint8_t* pui8Curves, uint8_t ui8CurveNItems,
                      stTableCurveY_t* psCurveY)
{
#if CHECK_TABLE
  if ((psCurveY == NULL) || (pui8Curves == NULL))
  {
    return;
  }
#endif

  psCurveY->ui8Valid   = 1;
  psCurveY->ui16Yvalue = ui16Y;
  psCurveY->ui8NItems  = ui8CurveNItems;
  /*lint -epp*/
  psCurveY->pui8Item   = ((psCurveY->ui8NItems > 0) ? ((uint8_t*)pui8Curves + 2) : NULL);
  /*lint -epp*/
}


/*--------------------------------------
 Find, in the table, the Y (Temperature)
 curves including the Yx value.
 -------------------------------------*/
eEMS_Err TableFindYCurves (const stTable_t* psTable, uint16_t ui16Yx, stTableSurfaceY_t* psSurfaceY)
{
  uint16_t ui16CurvesLen;     /* dimensione (byte) corrente dell'area dati contenente le curve */
  uint8_t ui8CurveIdx;        /* indice della curva corrente */
  const uint8_t* pui8Curves;  /* puntatore all'area dati della curva corrente */
  uint8_t ui8CurveNItems;     /* numero di elementi nella curva corrente */
  uint8_t ui8CurveBytes;      /* dimensione dell'area dati contenente gli elementi Y e (P,V)i della curva corrente */
  uint16_t ui16CurrentY;      /* valore di Y per la curva corrente "Curva in funzione del dato dk" */

#if CHECK_TABLE
  if ((psTable == NULL) || (psSurfaceY == NULL))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* Recupera il puntatore alle curve */
  pui8Curves = psTable->pui8Curves;
#if CHECK_TABLE
  if (pui8Curves == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* Recupera la dimensione totale dell'area contenente le curve */
  ui16CurvesLen = 2 * ((uint16_t)(psTable->ui8CurvesWLen));

  /* ciclo su tutte le curve: si suppone che le curve siano memorizzate per valori di Y crescenti. */
  for (ui8CurveIdx = 0; ((ui16CurvesLen > 0) && (ui8CurveIdx < psTable->ui8NCurves)); ++ui8CurveIdx)
  {
    /* recupera il numero di elementi della curva corrente "N = num elementi tabella dk". */
    ui8CurveNItems = pui8Curves[EM_DATA_TABLE_NUM_ITEMS_OFFSET];

    /* pui8Curves punta a Y */
    pui8Curves += EM_DATA_TABLE_CURVE_MIN_LEN;

    ui16CurvesLen -= EM_DATA_TABLE_CURVE_MIN_LEN;

    /* calcola la dimensione (bytes) dell'area contenente gli elementi Y e (P,V)i della curva.
     * Una curva � composta da 1 word per il valore di Y, N word per i valori di potenza,
     * N byte per i valori di tensione (+ un evetuale byte unused se N � dispari). */
    ui8CurveBytes = 2 + (3 * ui8CurveNItems) + (ui8CurveNItems % 2);

#if CHECK_TABLE
    /*
     * Verifica la dimensione dell'area contenente la curva corrente:
     * - non deve essere superiore alla dimensione corrente dell'area contenente le curve
     * - deve essere multiplo di 2
     */
    if ((ui8CurveBytes > ui16CurvesLen) || ((ui8CurveBytes % 2) != 0))
    {
      return EMS_E_INVALID_VALUE;
    }
#endif

    /* Recupera il valore di Y per la curva corrente */
    ui16CurrentY = *((uint16_t *)pui8Curves);

    /*
     * Confronta Yx con il valore di Y della curva corrente
     */

    if (ui16Yx == ui16CurrentY)
    {
      /* Trovato il valore esatto: imposta la curva Y1 ed esci */
      CurveSet(ui16Yx, pui8Curves, ui8CurveNItems, &(psSurfaceY->sCurveY1));

      break;
    }

    if (ui16Yx < ui16CurrentY)
    {
      /* Il valore Yx � inferiore al valore di Y della curva corrente: fa le opportune impostazioni ed esci */

      if (psSurfaceY->sCurveY1.ui8Valid == 1)
      {
        /* E' gia' stata impostata la curva Y1: imposta la curva Y2 */
        CurveSet(ui16CurrentY, pui8Curves, ui8CurveNItems, &(psSurfaceY->sCurveY2));
      }
      else
      {
        /* Non e' stata ancora impostata la curva Y1: impostala */
        CurveSet(ui16CurrentY, pui8Curves, ui8CurveNItems, &(psSurfaceY->sCurveY1));
      }

      break;
    }

    /* ui16Yx > ui16CurrentY */

    /* Il valore Yx � superiore al valore di Y della curva corrente: imposta la curva Y1 */
    CurveSet(ui16CurrentY, pui8Curves, ui8CurveNItems, &(psSurfaceY->sCurveY1));

    /* Aggiorna gli estremi della curva successiva */
    pui8Curves += ui8CurveBytes;
    ui16CurvesLen -= (uint16_t)ui8CurveBytes;
  }

  return EMS_E_OK;
}


/**
  This function Init/Reset a Y point

\param  [in]      none
\param  [out]     stCurveYPoint_t* psYPoint   [pointer to Y point]
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
STATIC void PointInit (stCurveYPoint_t* psYPoint)
{
#if CHECK_TABLE
  if (psYPoint == NULL)
  {
    return;
  }
#endif

  memset(psYPoint, 0, sizeof(stCurveYPoint_t));
}


/**
  This function set a Y point

\param  [in]      uint16_t          ui16V      [value of V]
\param  [in]      uint16_t          ui16Pw     [value of P]
\param  [out]     stCurveYPoint_t*  psYPoint   [pointer to Y point]
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
STATIC void PointSet (uint16_t ui16V, uint16_t ui16Pw,
                      stCurveYPoint_t* psYPoint)
{
#if CHECK_TABLE
  if (psYPoint == NULL)
  {
    return;
  }
#endif

  psYPoint->ui8Valid  = 1;
  psYPoint->ui16V     = ui16V;
  psYPoint->ui16Pw    = ui16Pw;
}


/**
  This function Init/Reset a Y line

\param  [in]      none
\param  [out]     stCurveYLine_t* psYLine   [pointer to Y line]
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
STATIC void LineInit (stCurveYLine_t* psYLine)
{
#if CHECK_TABLE
  if (psYLine == NULL)
  {
    return;
  }
#endif

  /* init points */
  PointInit(&(psYLine->sPoint1));
  PointInit(&(psYLine->sPoint2));
}


/**
  This function find, in the Y (Temperature) curve, the line including the Vin value

\param  [in]      stTable_t*        psTable   [pointer to data table struct]
\param  [in]      stTableCurveY_t*  psCurveY  [pointer to Y curve]
\param  [in]      uint16_t          ui16Vin   [the value of Vin]
\param  [out]     stCurveYLine_t*   psYLine   [pointer to Y line]
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
STATIC eEMS_Err CurveYFindPoints (const stTable_t* psTable, const stTableCurveY_t* psCurveY,
                                  uint16_t ui16Vin, stCurveYLine_t* psYLine)
{
  uint16_t* pui16PwItem;    /* punta al primo campione P della curva (nel file di setting) */
  uint8_t*  pui8VItem;      /* punta al primo campione V della curva (nel file di setting) */
  uint8_t   ui8ItemIdx;     /* indice del campione corrente nella curva */
  uint16_t  ui16CurrentV;   /* valore di V per la curva corrente */
  uint16_t  ui16CurrentPw;  /* valore di P per la V corrente */


#if CHECK_TABLE
  if ((psTable == NULL) || (psCurveY == NULL) || (psYLine == NULL))
  {
    return EMS_E_INVALID_VALUE;
  }

  if (psCurveY->ui8Valid == 0)
  {
    return EMS_E_INVALID_VALUE;
  }

  if (psCurveY->pui8Item == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
#endif


  /* Imposta i puntatori ai campioni della curva */
  pui16PwItem = (uint16_t*)(psCurveY->pui8Item);
  pui8VItem = (uint8_t*)(&(pui16PwItem[psCurveY->ui8NItems]));

  /*
   * ciclo su tutti i campioni: si suppone che i campioni siano memorizzati per valori di V crescenti. */
  for (ui8ItemIdx = 0; ui8ItemIdx < psCurveY->ui8NItems; ++ui8ItemIdx)
  {
    /* Recupera la potenza per il campione corrente */
    ui16CurrentPw = pui16PwItem[ui8ItemIdx];

    /* Recupera la tensione per il campione corrente */
   #if TABLE_APPROX_VIN
    /* ui16Vin � espressa su 8 bit */
    ui16CurrentV = (uint16_t)(pui8VItem[ui8ItemIdx]);
   #else
    /* ui16Vin � espressa su 16 bit mentre il valore di V � espresso su 8 bit, moltiplico quest'ultimo per 2^8 */
    ui16CurrentV = ((uint16_t)(pui8VItem[ui8ItemIdx])) << 8;
   #endif




    /*
     * Confronta Vin con il valore di V corrente
     */

    if (ui16Vin == ui16CurrentV)
    {
      /* Trovato il valore esatto: imposta il punto P1 della retta ed esci */
      PointSet(ui16Vin, ui16CurrentPw, &(psYLine->sPoint1));



      break;
    }

    if (ui16Vin < ui16CurrentV)
    {
      /* Il valore Vin � inferiore al valore di V corrente: fa le opportune impostazioni ed esci */

      if (psYLine->sPoint1.ui8Valid == 1)
      {
        /* E' gia' stato impostato il punto P1 della retta: imposta il punto P2 */
        PointSet(ui16CurrentV, ui16CurrentPw, &(psYLine->sPoint2));


      }
      else
      {
        /* Non e' stata ancora impostato il punto P1 della retta: impostalo */
        PointSet(ui16CurrentV, ui16CurrentPw, &(psYLine->sPoint1));


      }

      break;
    }

    /* ui16Vin > ui16CurrentV */

    /* Il valore Vin � superiore al valore di V corrente: imposta il punto P1 */
    PointSet(ui16CurrentV, ui16CurrentPw, &(psYLine->sPoint1));


    /* confronta il campione successivo */
  }

  return EMS_E_OK;
}


/**
  This function computes the value of Z for a line, based on X, one point (X1,Z) and one point (X2,Z2)
                                          (where X1 < X < X2)

\param  [in]      uint16_t    ui16X
\param  [in]      uint16_t    ui16X1
\param  [in]      uint16_t    ui16Z1
\param  [in]      uint16_t    ui16X2
\param  [in]      uint16_t    ui16Z2
\param  [out]     uint16_t*   pui16Z
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
STATIC eEMS_Err CalcoloPuntoSuRetta (uint16_t ui16X,
                                     uint16_t ui16X1, uint16_t ui16Z1,
                                     uint16_t ui16X2, uint16_t ui16Z2,
                                     uint16_t* pui16Z)
{
  int32_t i32Z;


#if CHECK_TABLE
  if (pui16Z == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }

  /* Verifica che i valori di X, X1 e X2 siano validi */
  if ((ui16X <= ui16X1) || (ui16X >= ui16X2))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* X1 < X < X2 */

  if (ui16Z1 == ui16Z2)
  {
    /* � una retta a Z costante */
    *pui16Z = ui16Z1;

    /* Z == Z1 == Z2 */
  }
  else
  {
    /* Z1 != Z2 */
#if 0
    *pui16Z = ui16Z1 + ((ui16Z2 - ui16Z1) * (ui16X - ui16X1) / (ui16X2 - ui16X1));
#else
    i32Z     = (int32_t)ui16Z2 - (int32_t)ui16Z1;
    i32Z    *= (int32_t)ui16X  - (int32_t)ui16X1;
    i32Z    /= (int32_t)ui16X2 - (int32_t)ui16X1;
    i32Z    += (int32_t)ui16Z1;
    *pui16Z  = (uint16_t)i32Z;

    /* min(Z1,Z2) < Z < max(Z1,Z2) */
#endif


  }

  return EMS_E_OK;
}


/*------------------------------------
 computes the power based on Vin and
 a Y curve
 -------------------------------------*/
eEMS_Err TableCalibSingola (const stTable_t* psTable, uint16_t ui16Vin,
                            const stTableCurveY_t* psCurveY, uint16_t* pui16Power)
{
  stCurveYLine_t sLineY; /* retta su Y */
  uint8_t aui8Result;


#if CHECK_TABLE
  if (pui16Power == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* init lines*/
  LineInit(&sLineY);

  /* cerca sulla curva Y la retta che include Vin */
  aui8Result = CurveYFindPoints(psTable, psCurveY, ui16Vin, &sLineY);
  if ((aui8Result != EMS_E_OK) || (sLineY.sPoint1.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* trovato almeno un punto sulla curva Y */

  if (sLineY.sPoint2.ui8Valid == 0)
  {
    /* trovato un punto sulla curva Y */
    *pui16Power = sLineY.sPoint1.ui16Pw;

    return EMS_E_OK;
  }
  else
  {
    /* trovata una retta sulla curva Y */
    return CalcoloPuntoSuRetta(ui16Vin,
                               sLineY.sPoint1.ui16V, sLineY.sPoint1.ui16Pw,
                               sLineY.sPoint2.ui16V, sLineY.sPoint2.ui16Pw,
                               pui16Power);
  }
}


/**
  This function computes the power based on Yx, one power (P1) on the curve Y1 and one power (P1) on the curve Y2

\param  [in]      uint16_t    ui16Yx      [the value of Yx]
\param  [in]      uint16_t    ui16Y1      [the value of Y1]
\param  [in]      uint16_t    ui16P1      [the value of power on Y1]
\param  [in]      uint16_t    ui16Y2      [the value of Y2]
\param  [in]      uint16_t    ui16P2      [the value of power on Y2]
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
STATIC eEMS_Err TableCalibDoppia_Punti (uint16_t ui16Yx,
                                        uint16_t ui16Y1, uint16_t ui16P1,
                                        uint16_t ui16Y2, uint16_t ui16P2,
                                        uint16_t* pui16Power)
{


  return CalcoloPuntoSuRetta(ui16Yx,
                             ui16Y1, ui16P1,
                             ui16Y2, ui16P2,
                             pui16Power);
}


/**
  This function computes the power based on Yx, Vin, one power (P1) on the curve Y1 and one line on the curve Y2

\param  [in]      uint16_t          ui16Yx      [the value of Yx]
\param  [in]      uint16_t          ui16Vin     [the value of Vin]
\param  [in]      uint16_t          ui16Y1      [the value of Y1]
\param  [in]      uint16_t          ui16P1      [the value of power on Y1]
\param  [in]      uint16_t          ui16Y2      [the value of Y2]
\param  [out]     stCurveYLine_t*   psYL2ine    [pointer to Y2 line]
\param  [out]     uint16_t*         pui16Power  [pointer to the power calculated]
\param  [in,out]  none

\attention \b

\return   eEMS_Err - error code

\retval   [none]

\warning  [none]

\author   Intecs
\date     08-09-2011
\version  1.0.0

\note   \par \b Nota 1:
      documentation by ${Intecs}
*/
STATIC eEMS_Err TableCalibDoppia_PuntoRetta (uint16_t ui16Yx, uint16_t ui16Vin,
                                             uint16_t ui16Y1, uint16_t ui16P1,
                                             uint16_t ui16Y2, const stCurveYLine_t* psY2Line,
                                             uint16_t* pui16Power)
{
  uint16_t ui16P2x;   /* punto sulla retta Y2 */



#if CHECK_TABLE
  if (psY2Line == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }

  if ((psY2Line->sPoint1.ui8Valid == 0) || (psY2Line->sPoint2.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* calcola il valore di potenza Px2, sulla retta Y2, corrispondente a Vin */
  if (EMS_E_OK != CalcoloPuntoSuRetta(ui16Vin,
                                      psY2Line->sPoint1.ui16V, psY2Line->sPoint1.ui16Pw,
                                      psY2Line->sPoint2.ui16V, psY2Line->sPoint2.ui16Pw,
                                      &ui16P2x))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* calcola il valore di potenza basato su Yx e sui punti (Y1,P1) e (Y2,P2x) */
  return TableCalibDoppia_Punti(ui16Yx,
                                ui16Y1, ui16P1,
                                ui16Y2, ui16P2x,
                                pui16Power);
}


/**
  This function computes the power based on Yx, Vin, one line on the curve Y1 and one line on the curve Y2

\param  [in]      uint16_t          ui16Yx      [the value of Yx]
\param  [in]      uint16_t          ui16Vin     [the value of Vin]
\param  [in]      uint16_t          ui16Y1      [the value of Y1]
\param  [out]     stCurveYLine_t*   psYL1ine    [pointer to Y1 line]
\param  [in]      uint16_t          ui16Y2      [the value of Y2]
\param  [out]     stCurveYLine_t*   psYL2ine    [pointer to Y2 line]
\param  [out]     uint16_t*         pui16Power  [pointer to the power calculated]
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
STATIC eEMS_Err TableCalibDoppia_Rette (uint16_t ui16Yx, uint16_t ui16Vin,
                                        uint16_t ui16Y1, const stCurveYLine_t* psY1Line,
                                        uint16_t ui16Y2, const stCurveYLine_t* psY2Line,
                                        uint16_t* pui16Power)
{
  uint16_t ui16P1x;   /* punto sulla retta Y1 */



#if CHECK_TABLE
  if (psY1Line == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }

  if ((psY1Line->sPoint1.ui8Valid == 0) || (psY1Line->sPoint2.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* calcola il valore di potenza P1x, sulla retta Y1, corrispondente a Vin */
  if (EMS_E_OK != CalcoloPuntoSuRetta(ui16Vin,
                                      psY1Line->sPoint1.ui16V, psY1Line->sPoint1.ui16Pw,
                                      psY1Line->sPoint2.ui16V, psY1Line->sPoint2.ui16Pw,
                                      &ui16P1x))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* calcola il valore di potenza basato su Yx, Vin sul punto (Y1,P1) e sulla retta Y2 */
  return TableCalibDoppia_PuntoRetta(ui16Yx, ui16Vin,
                                     ui16Y1, ui16P1x,
                                     ui16Y2, psY2Line,
                                     pui16Power);
}


/*--------------------------------------
 Computes the power based on Yx, Vin
 and the curves Y1 and Y2.
 -------------------------------------*/
eEMS_Err TableCalibDoppia (const stTable_t* psTable, uint16_t ui16Yx, uint16_t ui16Vin,
                           const stTableSurfaceY_t* psSurfaceY, uint16_t* pui16Power)
{
  stCurveYLine_t sLineY1; /* retta su Y1 */
  stCurveYLine_t sLineY2; /* retta su Y2 */
  uint8_t aui8Result[2];


#if CHECK_TABLE
  if (psSurfaceY == NULL)
  {
    return EMS_E_INVALID_VALUE;
  }
#endif

  /* Y1 < Yx < Y2 */

  /* init lines */
  LineInit(&sLineY1);
  LineInit(&sLineY2);

  /* cerca sulla curva Y1 la retta che include Vin */
  aui8Result[0] = CurveYFindPoints(psTable, &(psSurfaceY->sCurveY1), ui16Vin, &sLineY1);
  if ((aui8Result[0] != EMS_E_OK) || (sLineY1.sPoint1.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* cerca sulla curva Y2 la retta che include Vin */
  aui8Result[1] = CurveYFindPoints(psTable, &(psSurfaceY->sCurveY2), ui16Vin, &sLineY2);
  if ((aui8Result[1] != EMS_E_OK) || (sLineY2.sPoint1.ui8Valid == 0))
  {
    return EMS_E_INVALID_VALUE;
  }

  /* trovato almeno un valore su Y1 (sLineY1.sPoint1) e un valore su Y2 (sLineY2.sPoint1) */

  if ((sLineY1.sPoint2.ui8Valid == 0) && (sLineY2.sPoint2.ui8Valid == 0))
  {
    /* trovato un solo valore su Y1 e un solo valore su Y2 */
    return TableCalibDoppia_Punti(ui16Yx,
                                  psSurfaceY->sCurveY1.ui16Yvalue, sLineY1.sPoint1.ui16Pw,
                                  psSurfaceY->sCurveY2.ui16Yvalue, sLineY2.sPoint1.ui16Pw,
                                  pui16Power);
  }
  else if ((sLineY1.sPoint2.ui8Valid != 0) && (sLineY2.sPoint2.ui8Valid == 0))
  {
    /* trovati due valori su Y1 e un solo valore su Y2 */
    return TableCalibDoppia_PuntoRetta(ui16Yx, ui16Vin,
                                       psSurfaceY->sCurveY2.ui16Yvalue, sLineY2.sPoint1.ui16Pw,
                                       psSurfaceY->sCurveY1.ui16Yvalue, &sLineY1,
                                       pui16Power);
  }
  else if ((sLineY1.sPoint2.ui8Valid == 0) && (sLineY2.sPoint2.ui8Valid != 0))
  {
    /* trovato un solo valore su Y1 e due valori su Y2 */
    return TableCalibDoppia_PuntoRetta(ui16Yx, ui16Vin,
                                       psSurfaceY->sCurveY1.ui16Yvalue, sLineY1.sPoint1.ui16Pw,
                                       psSurfaceY->sCurveY2.ui16Yvalue, &sLineY2,
                                       pui16Power);
  }
  else /* if ((sLineY1.sPoint2.ui8Valid != 0) && (sLineY2.sPoint2.ui8Valid != 0)) */
  {
    /* trovati due valori su Y1 e due valori su Y2 */
    return TableCalibDoppia_Rette(ui16Yx, ui16Vin,
                                  psSurfaceY->sCurveY1.ui16Yvalue, &sLineY1,
                                  psSurfaceY->sCurveY2.ui16Yvalue, &sLineY2,
                                  pui16Power);
  }
}

#endif /* ENABLE_TABLE_LOAD */
