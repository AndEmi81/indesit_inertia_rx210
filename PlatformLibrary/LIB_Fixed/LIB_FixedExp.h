/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_FixedExp.h
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		LIB_FixedExp.h
\ingroup	LIB_Fixed
\date		Mar 14, 2014
\version	01.00.00
\author		Alessio Montagnani
\n
\n
\Par\b	History:
\n
*/

#if !defined(_LIB_FIXEDEXP_H_)
#define _LIB_FIXEDEXP_H_


#include <StandardTypes.h>
#include <StandardDefines.h>


/** Rounds away from zero.
 * \details Rounding examples:
 *          - Positive: x =  0.5  =>  y =  1.0 = floor(x + 0.5)
 *          - Negative: x = -0.5  =>  y = -1.0 = floor(x)
 *          - Zero:     x =  0.0  =>  y =  0.0
 */
#if !defined(LIB_FIXED_ROUND_AWAY_FROM_ZERO)
#define LIB_FIXED_ROUND_AWAY_FROM_ZERO                TRUE
#endif


/** Fast automatic sign extension when right shifting.
 * \pre The compiler suppports right shift sign extension.
 */
#if !defined(LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN)
#define LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN            FALSE
#endif

/** Tries to approximate overflows by rounding the needed fractional bits. */
#if !defined(LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS)
#define LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS   TRUE
#endif

/** Clamps the fractional result on overflow.
 * \warning Overflow error is not silenced.
 */
#if !defined(LIB_FIXED_CLAMP_OVERFLOWS)
#define LIB_FIXED_CLAMP_OVERFLOWS                     TRUE
#endif

/** Results are always at the maximum notation. */
#if !defined(LIB_FIXED_MAXIMUM_NOTATION_RESULTS)
#define LIB_FIXED_MAXIMUM_NOTATION_RESULTS            FALSE  /* FIXME: Untested */
#endif

/** Generates floating point conversion functions. */
#if !defined(LIB_FIXED_CONVERT_FLOATS)
#define LIB_FIXED_CONVERT_FLOATS                      FALSE
#endif

/** Enables automatic tests code generation. */
#if !defined(LIB_FIXED_GENERATE_TESTS)
#define LIB_FIXED_GENERATE_TESTS                      FALSE
#endif


/** Type of the internal fixed point representation.
 * \pre Signed type.
 */
typedef int16_t LIB_Fixed_FractionalType;

/** Type of the extended fixed point representation (for the accumulator).
 * \pre Signed type.
 * \pre Fractional and accumulator types are bound by the following rule:
\code{.c}
    sizeof(LIB_Fixed_AccumulatorType) >= (2 * sizeof(LIB_Fixed_FractionalType))
\endcode
 */
typedef int32_t LIB_Fixed_AccumulatorType;

/** Signed type of the extended fixed point representation (for the division accumulator).
 * \pre Fractional and accumulator types are bound by the following rule:
\code{.c}
    sizeof(LIB_Fixed_AccumulatorType) >= (3 * sizeof(LIB_Fixed_FractionalType))
\endcode
 */
typedef int64_t LIB_Fixed_DivAccumulatorType;

/** Type of the notation.
 * \pre Unsigned type.
 */
typedef uint8_t LIB_Fixed_NotationType;

/** Type of the comparison result.
 * \pre Signed type.
 */
typedef LIB_Fixed_AccumulatorType LIB_Fixed_CmpType;

/** Fixed point value. */
typedef struct LIB_Fixed_ValueType {
  LIB_Fixed_FractionalType  fractional;   /**< Fractional part. */
  LIB_Fixed_NotationType    notation;     /**< Notation (2**n). */
} LIB_Fixed_ValueType;

/** Error of fixed point operations. */
typedef enum LIB_Fixed_ErrorType {
  LIB_FIXED_ERROR_OK                  = 0,  /**< No error. */
  LIB_FIXED_ERROR_OVERFLOW            = 1,  /**< Overflow. */
  LIB_FIXED_ERROR_DIVISION_BY_ZERO    = 2,  /**< Division by zero. */
  LIB_FIXED_ERROR_NULL_ARG            = 3,  /**< \p NULL argument. */

  LIB_FIXED_ERROR__LENGTH
} LIB_Fixed_ErrorType;


/** Byte size of a fractional value. */
#define LIB_FIXED_FRACTIONAL_BTYE_SIZE        (sizeof(LIB_Fixed_FractionalType))

/** Bit size of a fractional value. */
#define LIB_FIXED_FRACTIONAL_BIT_SIZE         (8 * LIB_FIXED_FRACTIONAL_BTYE_SIZE)

/** Keeps only the sign of a fractional value. */
#define LIB_FIXED_FRACTIONAL_SIGN_MASK        (1 << (LIB_FIXED_FRACTIONAL_BIT_SIZE - 1))

/** Minimum fractional value. */
#define LIB_FIXED_FRACTIONAL_MIN              (-LIB_FIXED_FRACTIONAL_SIGN_MASK)

/** Maximum fractional value. */
#define LIB_FIXED_FRACTIONAL_MAX              (LIB_FIXED_FRACTIONAL_SIGN_MASK - 1)


/** Byte size of a notation value. */
#define LIB_FIXED_NOTATION_BTYE_SIZE          (sizeof(LIB_Fixed_NotationType))

/** Bit size of a notation value. */
#define LIB_FIXED_NOTATION_BIT_SIZE           (8 * LIB_FIXED_NOTATION_BTYE_SIZE)

/** Maximum notation value. */
#define LIB_FIXED_NOTATION_MAX                (LIB_FIXED_FRACTIONAL_BIT_SIZE - 1)

/** The result can have any notation. */
#define LIB_FIXED_NOTATION_ANY                (~(LIB_Fixed_NotationType)0)


/** Byte size of an extended value. */
#define LIB_FIXED_ACCUMULATOR_BTYE_SIZE       (sizeof(LIB_Fixed_AccumulatorType))

/** Bit size of an extended value. */
#define LIB_FIXED_ACCUMULATOR_BIT_SIZE        (8 * LIB_FIXED_ACCUMULATOR_BTYE_SIZE)

/** Keeps only the sign of an extended value. */
#define LIB_FIXED_ACCUMULATOR_SIGN_MASK       (1 << (LIB_FIXED_ACCUMULATOR_BIT_SIZE - 1))

/** Minimum extended value. */
#define LIB_FIXED_ACCUMULATOR_MIN             (-LIB_FIXED_ACCUMULATOR_SIGN_MASK)

/** Maximum extended value. */
#define LIB_FIXED_ACCUMULATOR_MAX             (LIB_FIXED_ACCUMULATOR_SIGN_MASK - 1)

/** Mask to detect overflows on the accumulator.
 * \deatils Exposes only the most significant bits and the fractional sign bit.
 */
#define LIB_FIXED_ACCUMULATOR_OVERFLOW_MASK   (~((1 << LIB_FIXED_NOTATION_MAX) - 1))


/** Static initialization of a \p LIB_Fixed_ValueStruct object. */
#define LIB_FIXED_VALUESTRUCT_INIT(fractional, notation) \
  { ((fractional) << (notation)) , (notation) }

/** Initializes a fractional value from a floating point value.
 * \details Can be used with constant floating point computations at compile time.
 */
#define LIB_FIXED_FRACTIONAL_FROMFLOAT(floating, notation) \
  ((LIB_Fixed_FractionalType)((floating) * ((LIB_Fixed_AccumulatorType)1 << (notation))))


extern const LIB_Fixed_ValueType LIB_Fixed_Const_MIN;
extern const LIB_Fixed_ValueType LIB_Fixed_Const_MONE;
extern const LIB_Fixed_ValueType LIB_Fixed_Const_ZERO;
extern const LIB_Fixed_ValueType LIB_Fixed_Const_ONE;
extern const LIB_Fixed_ValueType LIB_Fixed_Const_MAX;


LIB_Fixed_AccumulatorType LIB_Fixed_AccumulatorSAR(LIB_Fixed_AccumulatorType accumulator, LIB_Fixed_NotationType shift);
LIB_Fixed_FractionalType LIB_Fixed_FractionalSAR(LIB_Fixed_FractionalType fractional, LIB_Fixed_NotationType shift);
void LIB_Fixed_ApproximateAccumulatorOverflow(LIB_Fixed_AccumulatorType *accumulatorPtr, LIB_Fixed_NotationType *notationPtr);
LIB_Fixed_ValueType LIB_Fixed_ToMaximumNotation(LIB_Fixed_ValueType value);
LIB_Fixed_ErrorType LIB_Fixed_Rescale(LIB_Fixed_ValueType *valuePtr, LIB_Fixed_NotationType notation);
LIB_Fixed_FractionalType LIB_Fixed_ClampAccumulator(LIB_Fixed_AccumulatorType accumulator);
LIB_Fixed_ValueType LIB_Fixed_Clamp(LIB_Fixed_ValueType minimum, LIB_Fixed_ValueType value, LIB_Fixed_ValueType maximum);
LIB_Fixed_ValueType LIB_Fixed_FromClampedAccumulator(LIB_Fixed_AccumulatorType accumulator, LIB_Fixed_NotationType notation);
LIB_Fixed_ErrorType LIB_Fixed_FromInt(LIB_Fixed_FractionalType integral, LIB_Fixed_NotationType notation, LIB_Fixed_ValueType *valuePtr);
LIB_Fixed_ErrorType LIB_Fixed_Add(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr);
LIB_Fixed_ErrorType LIB_Fixed_Sub(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr);
LIB_Fixed_ErrorType LIB_Fixed_Mul(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr);
LIB_Fixed_ErrorType LIB_Fixed_Div(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr);
LIB_Fixed_ErrorType LIB_Fixed_Sqr(LIB_Fixed_ValueType operand, LIB_Fixed_ValueType *resultPtr);
LIB_Fixed_CmpType LIB_Fixed_Cmp(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2);
LIB_Fixed_ValueType LIB_Fixed_Max(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2);
LIB_Fixed_ValueType LIB_Fixed_Min(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2);
LIB_Fixed_ValueType LIB_Fixed_Abs(LIB_Fixed_ValueType operand);
LIB_Fixed_FractionalType LIB_Fixed_Round(LIB_Fixed_ValueType operand);
LIB_Fixed_ValueType LIB_Fixed_Lerp(LIB_Fixed_ValueType minimum, LIB_Fixed_ValueType ratio, LIB_Fixed_ValueType maximum);
LIB_Fixed_ValueType LIB_Fixed_LerpY
(
  LIB_Fixed_ValueType x1, LIB_Fixed_ValueType y1,
  LIB_Fixed_ValueType x2, LIB_Fixed_ValueType y2,
  LIB_Fixed_ValueType x
);
#if LIB_FIXED_CONVERT_FLOATS
LIB_Fixed_ErrorType LIB_Fixed_FromFloat(float floating, LIB_Fixed_NotationType notation, LIB_Fixed_ValueType *valuePtr);
float LIB_Fixed_ToFloat(LIB_Fixed_ValueType value);
#endif  /* LIB_FIXED_CONVERT_FLOATS */


#if LIB_FIXED_GENERATE_TESTS
bool_t LIB_Fixed__test(void);
bool_t LIB_Fixed_AccumulatorSAR__test(void);
bool_t LIB_Fixed_FractionalSAR__test(void);
bool_t LIB_Fixed_ApproximateAccumulatorOverflow__test(void);
/* TODO: bool_t LIB_Fixed_ToMaximumNotation__test(void); */
bool_t LIB_Fixed_Rescale__test(void);
/* TODO: bool_t LIB_Fixed_ClampAccumulator__test(void); */
/* TODO: bool_t LIB_Fixed_Clamp__test(void); */
/* TODO: bool_t LIB_Fixed_FromClampedAccumulator__test(void); */
bool_t LIB_Fixed_FromInt__test(void);
bool_t LIB_Fixed_Add__test(void);
bool_t LIB_Fixed_Sub__test(void);
bool_t LIB_Fixed_Mul__test(void);
bool_t LIB_Fixed_Div__test(void);
bool_t LIB_Fixed_Sqr__test(void);
bool_t LIB_Fixed_Cmp__test(void);
/* TODO: bool_t LIB_Fixed_Max__test(void); */
/* TODO: bool_t LIB_Fixed_Min__test(void); */
/* TODO: bool_t LIB_Fixed_Abs__test(void); */
bool_t LIB_Fixed_Round__test(void);
/* TODO: bool_t LIB_Fixed_Lerp__test(void); */
/* TODO: bool_t LIB_Fixed_LerpY__test(void); */
#if LIB_FIXED_CONVERT_FLOATS
bool_t LIB_Fixed_FromFloat__test(void);
bool_t LIB_Fixed_ToFloat__test(void);
#endif  /* LIB_FIXED_CONVERT_FLOATS */
#endif  /* LIB_FIXED_GENERATE_TESTS */

#endif  /* _LIB_FIXEDEXP_H_ */
