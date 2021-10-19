/*
Copyright (c) 2004-2014, Indesit Company, I.D.D.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		Fixed.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		  LIB_Fixed.c
\ingroup	LIB_Fixed
\date		  Mar 14, 2014
\version	02.00.00
\author   Alessio Montagnani
\author   Andrea Zoppi
\n
\n
\Par\b	History:
\n
\n    01.00.00
\n    14/03/2014
\n    Alessio Montagnani
\n    Creation
\n
\n    02.00.00
\n    23/05/2014
\n    Andrea Zoppi
\n    Refactoring
*/

#include "LIB_FixedExp.h"


/* Common constants */
const LIB_Fixed_ValueType LIB_Fixed_Const_MIN  = LIB_FIXED_VALUESTRUCT_INIT(LIB_FIXED_FRACTIONAL_MIN, 0);       /**< Minimum value. */
const LIB_Fixed_ValueType LIB_Fixed_Const_MONE = LIB_FIXED_VALUESTRUCT_INIT(-1, (LIB_FIXED_NOTATION_MAX - 2));  /**< -1.0, maximum precision. */
const LIB_Fixed_ValueType LIB_Fixed_Const_ZERO = LIB_FIXED_VALUESTRUCT_INIT( 0, (LIB_FIXED_NOTATION_MAX - 2));  /**<  0.0, maximum precision. */
const LIB_Fixed_ValueType LIB_Fixed_Const_ONE  = LIB_FIXED_VALUESTRUCT_INIT( 1, (LIB_FIXED_NOTATION_MAX - 2));  /**<  1.0, maximum precision. */
const LIB_Fixed_ValueType LIB_Fixed_Const_MAX  = LIB_FIXED_VALUESTRUCT_INIT(LIB_FIXED_FRACTIONAL_MAX, 0);       /**< Maximum value. */


/**
 * \brief   Accumulator value arithmetic right shift.
 * \details Ensures sign extension.
 * \pre \p shift <= \p LIB_FIXED_NOTATION_MAX
 *
 * \param[in] accumulator
 *          Accumulator value to be shifted.
 * \param[in] shift
 *          Right shift amount.
 * \return
 *          Accumulator value shifted to the right with extended sign.
 */
LIB_Fixed_AccumulatorType LIB_Fixed_AccumulatorSAR(LIB_Fixed_AccumulatorType accumulator, LIB_Fixed_NotationType shift)
{
  LIB_Fixed_AccumulatorType result = 0;

  if ( shift < LIB_FIXED_ACCUMULATOR_BIT_SIZE )
  {
    result = (accumulator >> shift);

#if !LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN
    /* Sign extension */
    if ( accumulator < 0 )
    {
      result |= ((((LIB_Fixed_AccumulatorType)1 << shift) - 1) << (LIB_FIXED_ACCUMULATOR_BIT_SIZE - shift));
    }
#endif  /* !LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN */
  }
  return (result);
}


/**
 * \brief   Fractional value arithmetic right shift.
 * \details Ensures sign extension.
 *
 * \param[in] fractional
 *          Fractional value to be shifted.
 * \param[in] shift
 *          Right shift amount.
 * \return
 *          Fractional value shifted to the right with extended sign.
 */
LIB_Fixed_FractionalType LIB_Fixed_FractionalSAR(LIB_Fixed_FractionalType fractional, LIB_Fixed_NotationType shift)
{
#if LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN
  return (fractional >> shift);
#else
  /* Extended sign is mapped correctly when downcasting, thanks to type size constraints */
  LIB_Fixed_AccumulatorType extended = fractional;
  return ((LIB_Fixed_FractionalType)(extended >> shift));
#endif
}


/**
 * \brief   Approximated accumulator overflow.
 * \details Shifts the accumulator value to the right until all significant
 *          bits fit into the fractional value range. Sign is preserved.
 * \warning If there is an overflow, the original pointed values are not
 *          overwritten, so that the overflow can be checked upon return.
 *
 * \param[in,out] accumulatorPtr
 *          Pointer to the accumulator value.
 * \param[in,out] notationPtr
 *          Pointer to the notation value.
 */
void LIB_Fixed_ApproximateAccumulatorOverflow(LIB_Fixed_AccumulatorType *accumulatorPtr, LIB_Fixed_NotationType *notationPtr)
{
  LIB_Fixed_AccumulatorType accumulator = *accumulatorPtr;
  LIB_Fixed_NotationType notation = *notationPtr;

  if ( accumulator < 0 )
  {
    /* Throw LSb away until the fractional part fits */
    while ( LIB_FIXED_ACCUMULATOR_OVERFLOW_MASK != (accumulator & LIB_FIXED_ACCUMULATOR_OVERFLOW_MASK) )
    {
      accumulator >>= 1;

#if !LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN
      accumulator |= LIB_FIXED_ACCUMULATOR_SIGN_MASK;
#endif  /* LIB_FIXED_RIGHT_SHIFT_EXTENDS_SIGN */

      --notation;
    }

    /* Save the result only if shifted and not overflowed, thus it can be checked upon return */
    if ( notation < *notationPtr )
    {
      *accumulatorPtr = accumulator;
      *notationPtr = notation;
    }
  }
  else
  {
    /* Throw LSb away until the fractional part fits */
    while ( 0 != (accumulator & LIB_FIXED_ACCUMULATOR_OVERFLOW_MASK) )
    {
      accumulator >>= 1;
      --notation;
    }

    /* Save the result only if shifted and not overflowed, thus it can be checked upon return */
    if ( notation < *notationPtr )
    {
#if LIB_FIXED_ROUND_AWAY_FROM_ZERO
      accumulator = (((*accumulatorPtr >> (*notationPtr - notation - 1)) + 1) >> 1);  /* floor(x + 0.5) */
#endif  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */

      *accumulatorPtr = accumulator;
      *notationPtr = notation;
    }
  }
}


/**
 * \brief   Rescales to meet maximum precision.
 * \details For non-null values, increases the notation so that the most
 *          significant bit (sign excluded) is 1.
 *
 * \param[in] value
 *          Value to rescale.
 * \return
 *          Rescaled value.
 */
LIB_Fixed_ValueType LIB_Fixed_ToMaximumNotation(LIB_Fixed_ValueType value)
{
  /* Exclude 0 and minimum (maximum negative) */
  if ( 0 != (value.fractional & LIB_FIXED_FRACTIONAL_MAX) )
  {
    /* Shift to the left until the most significant bit is 1 */
    while ( 0 == (value.fractional & (LIB_FIXED_FRACTIONAL_SIGN_MASK / 2)) )
    {
      value.fractional <<= 1;
      ++value.notation;
    }
  }
  return (value);
}


/**
 * \brief   Rescales a fixed point value.
 *
 * \param[in,out] valuePtr
 *          Pointer to the fixed point value.
 * \param[in] notation
 *          Notation of the resulting fractional value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          The result value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fractional value.
 */
LIB_Fixed_ErrorType LIB_Fixed_Rescale(LIB_Fixed_ValueType *valuePtr, LIB_Fixed_NotationType notation)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_AccumulatorType accumulator = 0;
  LIB_Fixed_NotationType shift = 0;

  if ( notation <= LIB_FIXED_NOTATION_MAX )
  {
    if ( notation > valuePtr->notation )
    {
      shift = (notation - valuePtr->notation);
      accumulator = ((LIB_Fixed_AccumulatorType)valuePtr->fractional << shift);
      if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
      {
        valuePtr->fractional = (LIB_Fixed_FractionalType)accumulator;
        valuePtr->notation = notation;
        error = LIB_FIXED_ERROR_OK;
      }
    }
    else if ( notation < valuePtr->notation )
    {
      shift = (valuePtr->notation - notation);
      if ( valuePtr->fractional >= 0 )
      {
#if LIB_FIXED_ROUND_AWAY_FROM_ZERO
        valuePtr->fractional = (((valuePtr->fractional >> (shift - 1)) + 1) >> 1);  /* floor(x + 0.5) */
#else  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */
        valuePtr->fractional >>= shift;
#endif  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */
      }
      else
      {
        valuePtr->fractional = LIB_Fixed_FractionalSAR(valuePtr->fractional, (shift - 1)) / 2;  /* floor(x) */
      }
      valuePtr->notation = notation;
      error = LIB_FIXED_ERROR_OK;
    }
    else
    {
      /* No rescale */
      error = LIB_FIXED_ERROR_OK;
    }
  }
  return (error);
}


/**
 * \brief   Clamps the accumulator.
 * \details Saturates fractional value if outside range.
 *
 * \param[in] accumulator
 *          Accumulator value to clamp.
 * \return
 *          Clamped fractional value.
 */
LIB_Fixed_FractionalType LIB_Fixed_ClampAccumulator(LIB_Fixed_AccumulatorType accumulator)
{
  LIB_Fixed_FractionalType fractional;
  if ( accumulator < LIB_FIXED_FRACTIONAL_MIN )
  {
    fractional = LIB_FIXED_FRACTIONAL_MIN;
  }
  else if ( accumulator > LIB_FIXED_FRACTIONAL_MAX )
  {
    fractional = LIB_FIXED_FRACTIONAL_MAX;
  }
  else
  {
    fractional = (LIB_Fixed_FractionalType)accumulator;
  }
  return (fractional);
}


/**
 * \brief   Clamps between bounds.
 * \pre     \p minimum <= \p maximum
 *
 * \param[in] minimum
 *          Lower bound.
 * \param[in] value
 *          Value to be clamped.
 * \param[out] maximum
 *          Upper bound.
 * \return
 *          Clamped value.
 */
LIB_Fixed_ValueType LIB_Fixed_Clamp(LIB_Fixed_ValueType minimum, LIB_Fixed_ValueType value, LIB_Fixed_ValueType maximum)
{
  if ( LIB_Fixed_Cmp(value, minimum) < 0 )
  {
    value = minimum;
  }
  else if ( LIB_Fixed_Cmp(maximum, value) < 0 )
  {
    value = maximum;
  }
  else
  {
    /* value = value */
  }

#if LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  value = LIB_Fixed_ToMaximumNotation(value);
#endif  /* LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  return (value);
}


/**
 * \brief   Clamped accumulator to fixed point.
 * \details Converts an accumulator value into a fixed point value.
 *          Accumulator and notation are clamped.
 * \warning The accumulator is already in the specified notation.
 *          No shifts will occur.
 *
 * \param[in] accumulator
 *          Accumulator value to be converted.
 * \param[in] notation
 *          Notation of the resulting fixed point value.
 * \return
 *          Clamped fixed point value.
 */
LIB_Fixed_ValueType LIB_Fixed_FromClampedAccumulator(LIB_Fixed_AccumulatorType accumulator, LIB_Fixed_NotationType notation)
{
  LIB_Fixed_ValueType value;

  if ( notation <= LIB_FIXED_NOTATION_MAX )
  {
    value.fractional = LIB_Fixed_ClampAccumulator(accumulator);
    value.notation = notation;
  }
  else
  {
    value = LIB_Fixed_Const_ZERO;
  }
  return (value);
}


/**
 * \brief   Integer to fixed point.
 * \details Converts an integral value into a fixed point value.
 *          The integral part is shifted left by the notation.
 * \post    On error, the returned value is zero.
 *
 * \param[in] integral
 *          Integral value to be converted.
 * \param[in] notation
 *          Notation of the resulting fixed point value.
 * \param[out] valuePtr
 *          Returned fixed point value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The integral value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_FromInt(LIB_Fixed_FractionalType integral, LIB_Fixed_NotationType notation, LIB_Fixed_ValueType *valuePtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_AccumulatorType accumulator = ((LIB_Fixed_AccumulatorType)integral << notation);

  *valuePtr = LIB_Fixed_Const_ZERO;

  if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
  {
    valuePtr->fractional = (LIB_Fixed_FractionalType)accumulator;
    valuePtr->notation = notation;
    error = LIB_FIXED_ERROR_OK;
  }

  return (error);
}


/**
 * \brief   Fixed point addition.
 * \post    On error, the returned value is zero.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \param[out] resultPtr
 *          Pointer to the result value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_Add(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_NotationType notation = ((operand1.notation >= operand2.notation) ? operand1.notation : operand2.notation);
  LIB_Fixed_AccumulatorType aligned1 = ((LIB_Fixed_AccumulatorType)operand1.fractional << (notation - operand1.notation));
  LIB_Fixed_AccumulatorType aligned2 = ((LIB_Fixed_AccumulatorType)operand2.fractional << (notation - operand2.notation));
  LIB_Fixed_AccumulatorType accumulator = (aligned1 + aligned2);

  *resultPtr = LIB_Fixed_Const_ZERO;

  if ( (operand1.notation <= LIB_FIXED_NOTATION_MAX) && (operand2.notation <= LIB_FIXED_NOTATION_MAX) )
  {
#if LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS
    LIB_Fixed_ApproximateAccumulatorOverflow(&accumulator, &notation);
#endif  /* LIB_FIXED_APPROXIMATE_OVERFLOWS */

    if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
    {
      resultPtr->fractional = (LIB_Fixed_FractionalType)accumulator;
      resultPtr->notation = notation;
      error = LIB_FIXED_ERROR_OK;
    }
#if LIB_FIXED_CLAMP_OVERFLOWS
    else
    {
      *resultPtr = LIB_Fixed_FromClampedAccumulator(accumulator, notation);
    }
#endif  /* LIB_FIXED_CLAMP_OVERFLOWS */
  }

#if LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  *resultPtr = LIB_Fixed_ToMaximumNotation(*resultPtr);
#endif  /* LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  return (error);
}


/**
 * \brief   Fixed point subtraction.
 * \post    On error, the returned value is zero.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \param[out] resultPtr
 *          Pointer to the result value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_Sub(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_NotationType notation = ((operand1.notation >= operand2.notation) ? operand1.notation : operand2.notation);
  LIB_Fixed_AccumulatorType aligned1 = ((LIB_Fixed_AccumulatorType)operand1.fractional << (notation - operand1.notation));
  LIB_Fixed_AccumulatorType aligned2 = ((LIB_Fixed_AccumulatorType)operand2.fractional << (notation - operand2.notation));
  LIB_Fixed_AccumulatorType accumulator = (aligned1 - aligned2);

  *resultPtr = LIB_Fixed_Const_ZERO;

  if ( (operand1.notation <= LIB_FIXED_NOTATION_MAX) && (operand2.notation <= LIB_FIXED_NOTATION_MAX) )
  {
#if LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS
    LIB_Fixed_ApproximateAccumulatorOverflow(&accumulator, &notation);
#endif  /* LIB_FIXED_APPROXIMATE_OVERFLOWS */

    if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
    {
      resultPtr->fractional = (LIB_Fixed_FractionalType)accumulator;
      resultPtr->notation = notation;
      error = LIB_FIXED_ERROR_OK;
    }
#if LIB_FIXED_CLAMP_OVERFLOWS
    else
    {
      *resultPtr = LIB_Fixed_FromClampedAccumulator(accumulator, notation);
    }
#endif  /* LIB_FIXED_CLAMP_OVERFLOWS */
  }

#if LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  *resultPtr = LIB_Fixed_ToMaximumNotation(*resultPtr);
#endif  /* LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  return (error);
}


/**
 * \brief   Fixed point multiplication.
 * \post    On error, the returned value is zero.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \param[out] resultPtr
 *          Pointer to the result value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_Mul(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_NotationType notation = (operand1.notation + operand2.notation);
  LIB_Fixed_AccumulatorType extended1 = operand1.fractional;
  LIB_Fixed_AccumulatorType extended2 = operand2.fractional;
  LIB_Fixed_AccumulatorType accumulator = (extended1 * extended2);

  *resultPtr = LIB_Fixed_Const_ZERO;

  if ( (operand1.notation <= LIB_FIXED_NOTATION_MAX) && (operand2.notation <= LIB_FIXED_NOTATION_MAX) )
  {
#if LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS
    LIB_Fixed_ApproximateAccumulatorOverflow(&accumulator, &notation);
#endif  /* LIB_FIXED_APPROXIMATE_OVERFLOWS */

    if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
    {
      resultPtr->fractional = (LIB_Fixed_FractionalType)accumulator;
      resultPtr->notation = notation;
      error = LIB_FIXED_ERROR_OK;
    }
#if LIB_FIXED_CLAMP_OVERFLOWS
    else
    {
      *resultPtr = LIB_Fixed_FromClampedAccumulator(accumulator, notation);
    }
#endif  /* LIB_FIXED_CLAMP_OVERFLOWS */
  }

#if LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  *resultPtr = LIB_Fixed_ToMaximumNotation(*resultPtr);
#endif  /* LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  return (error);
}


/**
 * \brief   Fixed point division.
 * \post    On error, the returned value is zero.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \param[out] resultPtr
 *          Pointer to the result value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fixed point representation.
 * \retval LIB_FIXED_ERROR_DIVIDE_BY_ZERO
 *          Division by zero.
 */
LIB_Fixed_ErrorType LIB_Fixed_Div(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2, LIB_Fixed_ValueType *resultPtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;
  LIB_Fixed_DivAccumulatorType divaccumulator = 0;
  LIB_Fixed_AccumulatorType accumulator = 0;
  LIB_Fixed_NotationType notation = (operand1.notation + operand2.notation);
  LIB_Fixed_NotationType overnotation = 0;

  *resultPtr = LIB_Fixed_Const_ZERO;

  if ( 0 != operand2.fractional )
  {
    if ( (operand1.notation <= LIB_FIXED_NOTATION_MAX) && (operand2.notation <= LIB_FIXED_NOTATION_MAX) )
    {
      accumulator = ((LIB_Fixed_AccumulatorType)operand1.fractional << operand2.notation);

#if LIB_FIXED_ROUND_AWAY_FROM_ZERO
      if ( ((operand1.fractional & LIB_FIXED_FRACTIONAL_SIGN_MASK) != (operand2.fractional & LIB_FIXED_FRACTIONAL_SIGN_MASK)) && (operand2.notation > 0) )
      {
        accumulator += ((LIB_Fixed_AccumulatorType)1 << (operand2.notation - 1));  /* (x + 0.5) */
      }
#endif  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */

      divaccumulator = ((LIB_Fixed_DivAccumulatorType)accumulator << operand2.notation);
      divaccumulator /= operand2.fractional;

      /* In the following, it is safe to truncate the least significant bits or have wrong sign extension bits */
      overnotation = (notation + operand2.notation);
      if ( overnotation > (LIB_FIXED_ACCUMULATOR_BIT_SIZE - 1) )
      {
        overnotation -= (LIB_FIXED_ACCUMULATOR_BIT_SIZE - 1);
        divaccumulator >>= overnotation;
        notation -= overnotation;
      }
      accumulator = (LIB_Fixed_AccumulatorType)divaccumulator;  /* sign always mapped correctly */

#if LIB_FIXED_APPROXIMATE_ACCUMULATOR_OVERFLOWS
      LIB_Fixed_ApproximateAccumulatorOverflow(&accumulator, &notation);
#endif  /* LIB_FIXED_APPROXIMATE_OVERFLOWS */

      if ( (accumulator >= LIB_FIXED_FRACTIONAL_MIN) && (accumulator <= LIB_FIXED_FRACTIONAL_MAX) )
      {
        resultPtr->fractional = (LIB_Fixed_FractionalType)accumulator;
        resultPtr->notation = notation;
        error = LIB_FIXED_ERROR_OK;
      }
#if LIB_FIXED_CLAMP_OVERFLOWS
      else
      {
        *resultPtr = LIB_Fixed_FromClampedAccumulator(accumulator, notation);
      }
#endif  /* LIB_FIXED_CLAMP_OVERFLOWS */
    }
  }
  else
  {
    error = LIB_FIXED_ERROR_DIVISION_BY_ZERO;
  }

#if LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  *resultPtr = LIB_Fixed_ToMaximumNotation(*resultPtr);
#endif  /* LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  return (error);
}


/**
 * \brief   Fixed point square.
 * \post    On error, the returned value is zero.
 *
 * \param[in] operand
 *          Operand.
 * \param[out] resultPtr
 *          Pointer to the result value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The result value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_Sqr(LIB_Fixed_ValueType operand, LIB_Fixed_ValueType *resultPtr)
{
  return LIB_Fixed_Mul(operand, operand, resultPtr);
}


/**
 * \brief   Fixed point comparison.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \return
 *          Signed comparison value.
 * \retval 0
 *          \p operand1 == \p operand2.
 * \retval positive
 *          \p operand1 > \p operand2.
 * \retval negative
 *          \p operand1 < \p operand2
 */
LIB_Fixed_CmpType LIB_Fixed_Cmp(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2)
{
  LIB_Fixed_AccumulatorType aligned1 = ((LIB_Fixed_AccumulatorType)operand1.fractional << operand2.notation);
  LIB_Fixed_AccumulatorType aligned2 = ((LIB_Fixed_AccumulatorType)operand2.fractional << operand1.notation);
  return ((LIB_Fixed_CmpType)(aligned1 - aligned2));
}


/**
 * \brief   Fixed point maximum.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \return
 *          Maximum operand.
 */
LIB_Fixed_ValueType LIB_Fixed_Max(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2)
{
  LIB_Fixed_AccumulatorType aligned1 = ((LIB_Fixed_AccumulatorType)operand1.fractional << operand2.notation);
  LIB_Fixed_AccumulatorType aligned2 = ((LIB_Fixed_AccumulatorType)operand2.fractional << operand1.notation);
  return ((aligned1 >= aligned2) ? operand1 : operand2);
}


/**
 * \brief   Fixed point maximum.
 *
 * \param[in] operand1
 *          First operand.
 * \param[in] operand2
 *          Second operand.
 * \return
 *          Maximum operand.
 */
LIB_Fixed_ValueType LIB_Fixed_Min(LIB_Fixed_ValueType operand1, LIB_Fixed_ValueType operand2)
{
  LIB_Fixed_AccumulatorType aligned1 = ((LIB_Fixed_AccumulatorType)operand1.fractional << operand2.notation);
  LIB_Fixed_AccumulatorType aligned2 = ((LIB_Fixed_AccumulatorType)operand2.fractional << operand1.notation);
  return ((aligned1 <= aligned2) ? operand1 : operand2);
}


/**
 * \brief   Fixed point absolute value.
 * \warning A fractional value equal to \p LIB_FIXED_FRACTIONAL_MIN
 *          is converted into \p LIB_FIXED_FRACTIONAL_MAX.
 *
 * \param[in] operand
 *          Operand.
 * \return
 *          Absolute value of the operand.
 */
LIB_Fixed_ValueType LIB_Fixed_Abs(LIB_Fixed_ValueType operand)
{
  if ( operand.fractional < 0 )
  {
    if ( operand.fractional != LIB_FIXED_FRACTIONAL_MIN )
    {
      operand.fractional = -operand.fractional;
    }
    else
    {
      operand.fractional = LIB_FIXED_FRACTIONAL_MAX;
    }
  }
  return (operand);
}


/**
 * \brief   Rounds to integer.
 * \details Rounds away from zero:
 *          - \p operand < 0 --> floor(operand)
 *          - \p operand >= 0 --> floor(operand + 0.5)
 *
 * \param[in] operand
 *          Operand.
 * \return
 *          Fixed point rounded to integer.
 */
LIB_Fixed_FractionalType LIB_Fixed_Round(LIB_Fixed_ValueType operand)
{
  LIB_Fixed_FractionalType integral = 0;

  if ( operand.notation > 0 )
  {
    /* Round away from zero */
    if ( operand.fractional >= 0 )
    {
#if LIB_FIXED_ROUND_AWAY_FROM_ZERO
      integral = (((operand.fractional >> (operand.notation - 1)) + 1) >> 1);  /* floor(x + 0.5) */
#else  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */
      integral = (operand.fractional >> operand.notation);
#endif  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */
    }
    else
    {
      integral = LIB_Fixed_FractionalSAR(operand.fractional, (operand.notation - 1)) / 2;  /* floor(x) */
    }
  }
  else
  {
    /* Already integral */
    integral = operand.fractional;
  }

  return (integral);
}


/**
 * \brief   Linear interpolation with ratio.
 *
 * \param[in] x1
 *          First value.
 * \param[in] ratio
 *          Interpolation ratio between \p x1 (\p ratio == 0) and \p x2 (\ratio == 1).
 * \param[out] x2
 *          Second value.
 * \return
 *          Interpolated value.
 */
LIB_Fixed_ValueType LIB_Fixed_Lerp(LIB_Fixed_ValueType x1, LIB_Fixed_ValueType ratio, LIB_Fixed_ValueType x2)
{
  /* Maximum used as temporary variable */
  LIB_Fixed_Sub(x2, x1, &x2);     /* x2 = delta */
  LIB_Fixed_Mul(ratio, x2, &x2);  /* x2 = adjusted delta */
  LIB_Fixed_Add(x1, x2, &x2);     /* x2 = adjusted output */
  return (x2);
}


/**
 * \brief   Linear interpolation of Y coordinate between two points.
 * \pre     \p x1 != \p x2
 *
 * \param[in] x1
 *          X coordinate of the first point
 * \param[in] y1
 *          Y coordinate of the first point
 * \param[in] x2
 *          X coordinate of the second point
 * \param[in] y2
 *          Y coordinate of the second point
 * \param[in] x
 *          X offset from \p x1 (function input).
 * \return
 *          Interpolated Y (function output).
 */
LIB_Fixed_ValueType LIB_Fixed_LerpY
(
  LIB_Fixed_ValueType x1, LIB_Fixed_ValueType y1,
  LIB_Fixed_ValueType x2, LIB_Fixed_ValueType y2,
  LIB_Fixed_ValueType x
)
{
#if !LIB_FIXED_MAXIMUM_NOTATION_RESULTS
  /* Increase precision anyway */
  x1 = LIB_Fixed_ToMaximumNotation(x1);
  y1 = LIB_Fixed_ToMaximumNotation(y1);
  x2 = LIB_Fixed_ToMaximumNotation(x2);
  y2 = LIB_Fixed_ToMaximumNotation(y2);
  x  = LIB_Fixed_ToMaximumNotation(x);
#endif  /* !LIB_FIXED_MAXIMUM_NOTATION_RESULTS */

  LIB_Fixed_Sub(y2, y1, &y2);  /* x2 = delta_x */
  LIB_Fixed_Sub(x2, x1, &x2);  /* y2 = delta_y */
  LIB_Fixed_Div(y2, x2, &x2);  /* x2 = (delta_y / delta_x) = ratio */
  LIB_Fixed_Mul(x2,  x, &y2);  /* y2 = (delta_y / delta_x) * x = adjusted delta */
  LIB_Fixed_Add(y1, y2, &y2);  /* y2 = adjusted output */
  return (y2);
}


#if LIB_FIXED_CONVERT_FLOATS

/**
 * \brief   Floating point to fixed point.
 * \details Converts a floating point value into a fixed point value.
 * \post    On error, the returned value is zero.
 *
 * \param[in] floating
 *          Floating piont value to be converted.
 * \param[in] notation
 *          Notation of the resulting fixed point value.
 * \param[out] valuePtr
 *          Returned fixed point value.
 * \return
 *          Error code.
 * \retval LIB_FIXED_ERROR_OK
 *          Returned value is valid.
 * \retval LIB_FIXED_ERROR_OVERFLOW
 *          The converted value cannot fit into the fixed point representation.
 */
LIB_Fixed_ErrorType LIB_Fixed_FromFloat(float floating, LIB_Fixed_NotationType notation, LIB_Fixed_ValueType *valuePtr)
{
  LIB_Fixed_ErrorType error = LIB_FIXED_ERROR_OVERFLOW;

  *valuePtr = LIB_Fixed_Const_ZERO;

  floating *= ((LIB_Fixed_AccumulatorType)1 << notation);
  if ( (floating >= LIB_FIXED_FRACTIONAL_MIN) && (floating <= LIB_FIXED_FRACTIONAL_MAX) )
  {
#if LIB_FIXED_ROUND_AWAY_FROM_ZERO
    if ( floating > 0 )
    {
      floating += 0.5f;
    }
#endif  /* LIB_FIXED_ROUND_AWAY_FROM_ZERO */

    if ( (floating >= LIB_FIXED_FRACTIONAL_MIN) && (floating <= LIB_FIXED_FRACTIONAL_MAX) )
    {
      resultPtr->fractional = (LIB_Fixed_FractionalType)floating;
      resultPtr->notation = notation;
      error = LIB_FIXED_ERROR_OK;
    }
#if LIB_FIXED_CLAMP_OVERFLOWS
    else
    {
      *resultPtr = LIB_Fixed_FromClampedAccumulator((LIB_Fixed_AccumulatorType)(floating * ((LIB_Fixed_AccumulatorType)1 << notation)), notation);
    }
#endif  /* LIB_FIXED_CLAMP_OVERFLOWS */
  }

  return (error);
}


/**
 * \brief   Fixed point to floating point.
 *
 * \param[in] value
 *          Fixed piont value to be converted.
 * \return
 *          Floating point representation.
 */
float LIB_Fixed_ToFloat(LIB_Fixed_ValueType value)
{
  return ((float)value.fractional / ((LIB_Fixed_AccumulatorType)1 << value.notation));
}

#endif  /* LIB_FIXED_CONVERT_FLOATS */

