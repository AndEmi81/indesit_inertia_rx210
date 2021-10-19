#ifndef __EM_SOFTWARE_TABLE_PRI_H
#define __EM_SOFTWARE_TABLE_PRI_H

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* PTR_ENERGY_METER_DATA.TABLE */
/* Dimensione (word) dell'header di una tabella energetica Table */
#define EM_DATA_TABLE_HEADER_WLEN         (1)
/* Dimensione (word) minima di una tabella energetica Table */
#define EM_DATA_TABLE_MIN_LEN             (EM_DATA_HEADER_COMMON_WLEN + EM_DATA_TABLE_HEADER_WLEN)
/* Offset del parametro "K = Total Number Tables" */
#define EM_DATA_TABLE_NUM_TABLES_OFFSET   (0)
/* Dimensione (byte) minima di una curva (1 word di "N num elementi tabella dk") */
#define EM_DATA_TABLE_CURVE_MIN_LEN       (2)
/* Offset del parametro "N num elementi tabella dk" */
#define EM_DATA_TABLE_NUM_ITEMS_OFFSET    (0)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct
{
  uint8_t         ui8Valid;
  uint16_t        ui16Yvalue; /* valore di Y */
  uint8_t         ui8NItems;  /* numero di campioni (P,V) della curva */
  uint8_t*        pui8Item;   /* punta al primo campione (P,V) della curva (nel file di setting) */
} stTableCurveY_t;

typedef struct
{
  stTableCurveY_t sCurveY1;   /* curva Y1 */
  stTableCurveY_t sCurveY2;   /* curva Y2 */
} stTableSurfaceY_t;

typedef struct
{
  uint8_t         ui8Valid;
  uint16_t        ui16V;      /* Tensione */
  uint16_t        ui16Pw;     /* Potenza */
} stCurveYPoint_t;

typedef struct
{
  stCurveYPoint_t sPoint1;    /* punto 1 sulla curva Y */
  stCurveYPoint_t sPoint2;    /* punto 2 sulla curva Y */
} stCurveYLine_t;

#endif /* __EM_SOFTWARE_TABLE_PRI_H */
