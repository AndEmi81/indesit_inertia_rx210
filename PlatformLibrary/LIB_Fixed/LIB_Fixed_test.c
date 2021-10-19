
#include "LIB_FixedExp.h"

#if LIB_FIXED_GENERATE_TESTS

#if LIB_FIXED_CONVERT_FLOATS
#include <float.h>
#include <math.h>
#endif


#if !LIB_FIXED_ROUND_AWAY_FROM_ZERO
#error "LIB_FIXED_ROUND_AWAY_FROM_ZERO must be TRUE for this test suite"
#endif


bool_t LIB_Fixed__test(void)
{
  volatile bool_t current = TRUE;
  volatile bool_t result = TRUE;

  current = LIB_Fixed_AccumulatorSAR__test();
  result = (result && current);

  current = LIB_Fixed_FractionalSAR__test();
  result = (result && current);

  current = LIB_Fixed_ApproximateAccumulatorOverflow__test();
  result = (result && current);

  current = LIB_Fixed_Rescale__test();
  result = (result && current);

  current = LIB_Fixed_FromInt__test();
  result = (result && current);

  current = LIB_Fixed_Add__test();
  result = (result && current);

  current = LIB_Fixed_Sub__test();
  result = (result && current);

  current = LIB_Fixed_Mul__test();
  result = (result && current);

  current = LIB_Fixed_Div__test();
  result = (result && current);

  current = LIB_Fixed_Sqr__test();
  result = (result && current);

  current = LIB_Fixed_Cmp__test();
  result = (result && current);

  current = LIB_Fixed_Round__test();
  result = (result && current);

#if LIB_FIXED_CONVERT_FLOATS

  current = LIB_Fixed_FromFloat__test();
  result = (result && current);

  current = LIB_Fixed_ToFloat__test();
  result = (result && current);

#endif  /* LIB_FIXED_CONVERT_FLOATS */

  return (result);
}


bool_t LIB_Fixed_AccumulatorSAR__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_AccumulatorType accumulator;
    LIB_Fixed_NotationType shift;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_AccumulatorType accumulator;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { (0), (0) }, { (0) } },
    { { (0), (10) }, { (0) } },
    { { (0), (20) }, { (0) } },
    { { (-1), (1) }, { (-1) } },

    { { ( 0x00123400), (9) }, { ( 0x00123400 / (1 << 9)) } },
    { { (-0x00123400), (9) }, { (-0x00123400 / (1 << 9)) } },

    { { ( 0x00123400), (2) }, { ( 0x00123400 / (1 << 2)) } },
    { { (-0x00123400), (2) }, { (-0x00123400 / (1 << 2)) } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.accumulator = LIB_Fixed_AccumulatorSAR(vi.accumulator, vi.shift);

    if ( vo.accumulator != vor.accumulator )
    {
      result = FALSE;
    }
  }

  return (result);
}


bool_t LIB_Fixed_FractionalSAR__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_FractionalType fractional;
    LIB_Fixed_NotationType shift;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_FractionalType fractional;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { (0), (0) }, { (0) } },
    { { (0), (10) }, { (0) } },
    { { (0), (20) }, { (0) } },
    { { (-1), (1) }, { (-1) } },

    { { ( 0x0AE0), (9) }, { ( 0x0AE0 / (1 << 9)) } },
    { { (-0x0AE0), (9) }, { (-0x0AE0 / (1 << 9)) } },

    { { ( 0x0AE0), (2) }, { ( 0x0AE0 / (1 << 2)) } },
    { { (-0x0AE0), (2) }, { (-0x0AE0 / (1 << 2)) } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vi.fractional = LIB_Fixed_FractionalSAR(vi.fractional, vi.shift);

    if ( vo.fractional != vor.fractional )
    {
      result = FALSE;
    }
  }

  return (result);
}


bool_t LIB_Fixed_ApproximateAccumulatorOverflow__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_AccumulatorType accumulator;
    LIB_Fixed_NotationType notation;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_AccumulatorType accumulator;
    LIB_Fixed_NotationType notation;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
   { { ( 0x00001234),  (0) }, { ( 0x00001234), (0) } },

   { { ( 0x12340000), (16) }, { ( 0x000048D0), (2) } },
   { { ( 0x00123400),  (8) }, { ( 0x000048D0), (2) } },

   { { (-0x12340000), (16) }, { (-0x000048D0), (2) } },
   { { (-0x00123400),  (8) }, { (-0x000048D0), (2) } },

   { { ( 0x1234F000), (16) }, { ( 0x000048D4), (2) } },
   { { ( 0x001234F0),  (8) }, { ( 0x000048D4), (2) } },

   { { (-0x1234F000), (16) }, { (-0x000048D4), (2) } },
   { { (-0x001234F0),  (8) }, { (-0x000048D4), (2) } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.accumulator = vi.accumulator;
    vo.notation = vi.notation;
    LIB_Fixed_ApproximateAccumulatorOverflow(&vo.accumulator, &vo.notation);

    if ( vo.accumulator != vor.accumulator )
    {
      result = FALSE;
    }
    if ( vo.notation != vor.notation )
    {
      result = FALSE;
    }
  }

  return (result);
}


bool_t LIB_Fixed_Rescale__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType value;
    LIB_Fixed_NotationType notation;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType value;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), ( 0) }, ( 0) }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK }, },
    { { { ( 0x0AE0), ( 4) }, ( 4) }, { { ( 0x0AE0), ( 4) }, LIB_FIXED_ERROR_OK }, },

    { { { ( 0x0AE0), ( 4) }, ( 0) }, { { ( 0x00AE), ( 0) }, LIB_FIXED_ERROR_OK }, },
    { { { (-0x0AE0), ( 4) }, ( 0) }, { { (-0x00AE), ( 0) }, LIB_FIXED_ERROR_OK }, },

    { { { ( 0x0AE0), ( 8) }, ( 0) }, { { ( 0x000B), ( 0) }, LIB_FIXED_ERROR_OK }, },
    { { { (-0x0AE0), ( 8) }, ( 0) }, { { (-0x000B), ( 0) }, LIB_FIXED_ERROR_OK }, },

    { { { ( 0x0230), ( 4) }, ( 8) }, { { ( 0x0230 << 4), ( 8) }, LIB_FIXED_ERROR_OK }, },
    { { { (-0x0230), ( 4) }, ( 8) }, { { (-0x0230 << 4), ( 8) }, LIB_FIXED_ERROR_OK }, },

    { { { ( 0x0230), ( 4) }, (12) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW }, },
    { { { (-0x0230), ( 4) }, (12) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW }, },

    { { { ( 0x0AE0), ( 4) }, (LIB_FIXED_NOTATION_MAX + 1) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW }, },
    { { { ( 0x0AE0), ( 4) }, (LIB_FIXED_NOTATION_MAX + 1) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW }, },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.value = vi.value;
    vo.error = LIB_Fixed_Rescale(&vo.value, vi.notation);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.value.fractional != vor.value.fractional )
      {
        result = FALSE;
      }
      if ( vo.value.notation != vor.value.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_FromInt__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_FractionalType integral;
    LIB_Fixed_NotationType notation;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType value;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { ( 0x0000), (0) }, { { ( 0x0000), (0) }, LIB_FIXED_ERROR_OK  } },

    { { ( 0x1234), (0) }, { { ( 0x1234), (0) }, LIB_FIXED_ERROR_OK  } },
    { { (-0x1234), (0) }, { { (-0x1234), (0) }, LIB_FIXED_ERROR_OK  } },

    { { ( 0x0034), (4) }, { { ( 0x0034 << 4), (4) }, LIB_FIXED_ERROR_OK  } },
    { { (-0x0034), (4) }, { { (-0x0034 << 4), (4) }, LIB_FIXED_ERROR_OK  } },
    { { ( 0x0034), (8) }, { { ( 0x0034 << 8), (8) }, LIB_FIXED_ERROR_OK  } },
    { { (-0x0034), (8) }, { { (-0x0034 << 8), (8) }, LIB_FIXED_ERROR_OK  } },

    { { ( 0x00AE), (LIB_FIXED_NOTATION_MAX) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { (-0x00AE), (LIB_FIXED_NOTATION_MAX) }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_FromInt(vi.integral, vi.notation, &vo.value);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.value.fractional != vor.value.fractional )
      {
        result = FALSE;
      }
      if ( vo.value.notation != vor.value.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Add__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand1;
    LIB_Fixed_ValueType operand2;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType result;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), (0) }, { ( 0x0000), (0) } }, { { ( 0x0000), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1234), (0) }, { ( 0x0000), (0) } }, { { ( 0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { ( 0x4321), (0) } }, { { ( 0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0000), (0) } }, { { (-0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { (-0x4321), (0) } }, { { (-0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { ( 0x4321), (0) } }, { { ( 0x1234 + 0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { (-0x4321), (0) } }, { { (-0x1234 - 0x4321), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (8) }, { ( 0x0043), (0) } }, { { ( 0x1200 + 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x0043), (0) } }, { { (-0x1200 - 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (8) } }, { { ( 0x1200 + 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0012), (0) }, { (-0x4300), (8) } }, { { (-0x1200 - 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0120), (4) }, { ( 0x0430), (4) } }, { { ( 0x0120 + 0x0430), (4) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0120), (4) }, { (-0x0430), (4) } }, { { (-0x0120 - 0x0430), (4) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0012), (0) }, { (-0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { { (-0x0001), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { { (-0x0001), (0) }, LIB_FIXED_ERROR_OK } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_Add(vi.operand1, vi.operand2, &vo.result);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.result.fractional != vor.result.fractional )
      {
        result = FALSE;
      }
      if ( vo.result.notation != vor.result.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Sub__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand1;
    LIB_Fixed_ValueType operand2;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType result;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { (0x0000), (0) }, { (0x0000), (0) } }, { { ( 0x0000), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1234), (0) }, { ( 0x0000), (0) } }, { { ( 0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { ( 0x4321), (0) } }, { { (-0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0000), (0) } }, { { (-0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { (-0x4321), (0) } }, { { ( 0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { ( 0x4321), (0) } }, { { ( 0x1234 - 0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { (-0x4321), (0) } }, { { (-0x1234 + 0x4321), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (8) }, { ( 0x0043), (0) } }, { { ( 0x1200 - 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x0043), (0) } }, { { (-0x1200 + 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (8) } }, { { ( 0x1200 - 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0012), (0) }, { (-0x4300), (8) } }, { { (-0x1200 + 0x4300), (8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0120), (4) }, { ( 0x0430), (4) } }, { { ( 0x0120 - 0x0430), (4) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0120), (4) }, { (-0x0430), (4) } }, { { (-0x0120 + 0x0430), (4) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0012), (0) }, { (-0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_Sub(vi.operand1, vi.operand2, &vo.result);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.result.fractional != vor.result.fractional )
      {
        result = FALSE;
      }
      if ( vo.result.notation != vor.result.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Mul__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand1;
    LIB_Fixed_ValueType operand2;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType result;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), (0) }, { ( 0x0000), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { ( 0x0000), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { ( 0x4321), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0000), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0000), (0) }, { (-0x4321), (0) } }, { { (0x0000), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x0001), (0) }, { ( 0x0001), (0) } }, { { ( 0x0001), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { ( 0x0001), (0) } }, { { ( 0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), (0) }, { ( 0x4321), (0) } }, { { ( 0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0001), (0) } }, { { (-0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), (0) }, { (-0x4321), (0) } }, { { (-0x4321), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x7FFF), (15) }, { ( 0x0001), ( 0) } }, { { ( 0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x7FFF), (15) }, { (-0x0001), ( 0) } }, { { (-0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x7FFF), (15) }, { ( 0x0001), ( 0) } }, { { (-0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x7FFF), (15) }, { (-0x0001), ( 0) } }, { { ( 0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), ( 0) }, { ( 0x7FFF), (15) } }, { { ( 0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), ( 0) }, { (-0x7FFF), (15) } }, { { (-0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), ( 0) }, { ( 0x7FFF), (15) } }, { { (-0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), ( 0) }, { (-0x7FFF), (15) } }, { { ( 0x7FFF), (15) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x8000), (15) }, { ( 0x0001), ( 0) } }, { { ( 0x8000), (15) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x8000), (15) }, { (-0x0001), ( 0) } }, { { ( 0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x8000), (15) }, { ( 0x0001), ( 0) } }, { { (-0x8000), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x8000), (15) }, { (-0x0001), ( 0) } }, { { ( 0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), ( 0) }, { ( 0x8000), (15) } }, { { ( 0x8000), (15) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), ( 0) }, { (-0x8000), (15) } }, { { (-0x8000), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), ( 0) }, { ( 0x8000), (15) } }, { { ( 0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), ( 0) }, { (-0x8000), (15) } }, { { ( 0x4000), (14) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x8000), (15) }, { ( 0x8000), (15) } }, { { (0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x8000), (15) }, { (-0x8000), (15) } }, { { (0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x8000), (15) }, { ( 0x8000), (15) } }, { { (0x4000), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x8000), (15) }, { (-0x8000), (15) } }, { { (0x4000), (14) }, LIB_FIXED_ERROR_OK } },

    { { { (-0x0001), (0) }, { (-0x0001), (0) } }, { { ( 0x0001), (0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { (-0x0001), (0) } }, { { (-0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), (0) }, { ( 0x4321), (0) } }, { { (-0x4321), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { (-0x0001), (0) } }, { { ( 0x1234), (0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), (0) }, { (-0x4321), (0) } }, { { ( 0x4321), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (8) }, { ( 0x0003), ( 0) } }, { { (0x3600), (8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x0003), ( 0) } }, { { (0x3600), (8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0012), (0) }, { ( 0x3000), (12) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0012), (0) }, { (-0x3000), (12) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0120), (4) }, { ( 0x0300), ( 8) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0120), (4) }, { (-0x0300), ( 8) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1200), (8) }, { ( 0x3000), (12) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x3000), (12) } }, { { (0x6C00), (9) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (8) }, { ( 0x0043), (0) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x0043), (0) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (8) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0012), (0) }, { (-0x4300), (8) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0120), (4) }, { ( 0x0430), (4) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0120), (4) }, { (-0x0430), (4) } }, { { (0x4B60), (4) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0012), (0) }, { (-0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_Mul(vi.operand1, vi.operand2, &vo.result);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.result.fractional != vor.result.fractional )
      {
        result = FALSE;
      }
      if ( vo.result.notation != vor.result.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Div__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand1;
    LIB_Fixed_ValueType operand2;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType result;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), (0) }, { ( 0x0000), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_DIVISION_BY_ZERO } },
    { { { ( 0x1234), (0) }, { ( 0x0000), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_DIVISION_BY_ZERO } },
    { { { ( 0x0000), (0) }, { ( 0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0000), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_DIVISION_BY_ZERO } },
    { { { ( 0x0000), (0) }, { (-0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x0001), (0) }, { ( 0x0001), (0) } }, { { ( 0x0001), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { ( 0x0001), (0) } }, { { ( 0x1234), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), (0) }, { ( 0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { ( 0x0001), (0) } }, { { (-0x1234), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0001), (0) }, { (-0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },

    { { { (-0x0001), (0) }, { (-0x0001), (0) } }, { { ( 0x0001), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1234), (0) }, { (-0x0001), (0) } }, { { (-0x1234), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), (0) }, { ( 0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1234), (0) }, { (-0x0001), (0) } }, { { ( 0x1234), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), (0) }, { (-0x4321), (0) } }, { { ( 0x0000), ( 0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (8) }, { ( 0x0003), ( 0) } }, { { (0x0600), ( 8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x0003), ( 0) } }, { { (0x0600), ( 8) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0012), (0) }, { ( 0x3000), (12) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0012), (0) }, { (-0x3000), (12) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x0120), (4) }, { ( 0x0300), ( 8) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0120), (4) }, { (-0x0300), ( 8) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },
    { { { ( 0x1200), (8) }, { ( 0x3000), (12) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1200), (8) }, { (-0x3000), (12) } }, { { (0x6000), (12) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x1200), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0043), (0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0012), (0) }, { (-0x4300), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { ( 0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { ( 0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
    { { { (-0x0120), (LIB_FIXED_NOTATION_MAX + 1) }, { (-0x0430), (LIB_FIXED_NOTATION_MAX + 1) } }, { { (0), (0) }, LIB_FIXED_ERROR_OVERFLOW } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_Div(vi.operand1, vi.operand2, &vo.result);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.result.fractional != vor.result.fractional )
      {
        result = FALSE;
      }
      if ( vo.result.notation != vor.result.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Sqr__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType result;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), ( 0) } }, { { (0), (0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x0001), ( 0) } }, { { (1 <<  0), ( 0) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0001), ( 0) } }, { { (1 <<  0), ( 0) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x0010), ( 4) } }, { { (1 <<  8), ( 8) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0010), ( 4) } }, { { (1 <<  8), ( 8) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x0100), ( 8) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x0100), ( 8) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x1000), (12) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x1000), (12) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x4000), (14) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x4000), (14) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },

    { { { ( 0x7FFF), (15) } }, { { ( 0x7FFE), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x7FFF), (15) } }, { { ( 0x7FFE), (15) }, LIB_FIXED_ERROR_OK } },
    { { { (-0x8000), (15) } }, { { (1 << 14), (14) }, LIB_FIXED_ERROR_OK } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_Sqr(vi.operand, &vo.result);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.result.fractional != vor.result.fractional )
      {
        result = FALSE;
      }
      if ( vo.result.notation != vor.result.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_Cmp__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand1;
    LIB_Fixed_ValueType operand2;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_CmpType cmp;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), (0) }, { ( 0x0000), (0) } }, { ( 0) } },
    { { { ( 0x1234), (0) }, { ( 0x1234), (0) } }, { ( 0) } },
    { { { (-0x1234), (0) }, { (-0x1234), (0) } }, { ( 0) } },
    { { { ( 0x4321), (0) }, { ( 0x4321), (0) } }, { ( 0) } },
    { { { (-0x4321), (0) }, { (-0x4321), (0) } }, { ( 0) } },

    { { { ( 0x1234), (4) }, { ( 0x1234), (4) } }, { ( 0) } },
    { { { (-0x1234), (4) }, { (-0x1234), (4) } }, { ( 0) } },
    { { { ( 0x4321), (4) }, { ( 0x4321), (4) } }, { ( 0) } },
    { { { (-0x4321), (4) }, { (-0x4321), (4) } }, { ( 0) } },

    { { { ( 0x1234), (8) }, { ( 0x1234), (8) } }, { ( 0) } },
    { { { (-0x1234), (8) }, { (-0x1234), (8) } }, { ( 0) } },
    { { { ( 0x4321), (8) }, { ( 0x4321), (8) } }, { ( 0) } },
    { { { (-0x4321), (8) }, { (-0x4321), (8) } }, { ( 0) } },

    { { { ( 0x1234), (0) }, { ( 0x0000), (0) } }, { ( 1) } },
    { { { ( 0x0000), (0) }, { ( 0x4321), (0) } }, { (-1) } },
    { { { (-0x1234), (0) }, { ( 0x0000), (0) } }, { (-1) } },
    { { { ( 0x0000), (0) }, { (-0x4321), (0) } }, { ( 1) } },
    { { { ( 0x1234), (0) }, { ( 0x4321), (0) } }, { (-1) } },
    { { { (-0x1234), (0) }, { (-0x4321), (0) } }, { ( 1) } },

    { { { ( 0x1200), (8) }, { ( 0x0043), (0) } }, { (-1) } },
    { { { (-0x1200), (8) }, { (-0x0043), (0) } }, { ( 1) } },
    { { { ( 0x0012), (0) }, { ( 0x4300), (8) } }, { (-1) } },
    { { { (-0x0012), (0) }, { (-0x4300), (8) } }, { ( 1) } },
    { { { ( 0x0120), (4) }, { ( 0x0430), (4) } }, { (-1) } },
    { { { (-0x0120), (4) }, { (-0x0430), (4) } }, { ( 1) } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { ( 0) } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { ( 0) } },

    { { { (LIB_FIXED_FRACTIONAL_MAX), (0) }, { (LIB_FIXED_FRACTIONAL_MIN), (0) } }, { ( 1) } },
    { { { (LIB_FIXED_FRACTIONAL_MIN), (0) }, { (LIB_FIXED_FRACTIONAL_MAX), (0) } }, { (-1) } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.cmp = LIB_Fixed_Cmp(vi.operand1, vi.operand2);
    vo.cmp = ((vo.cmp < 0) ? (-1) : ((vo.cmp > 0) ? (1) : (0)));

    if ( vo.cmp != vor.cmp )
    {
      result = FALSE;
    }
  }

  return (result);
}


bool_t LIB_Fixed_Round__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType operand;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_FractionalType integral;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { ( 0x0000), ( 0) } }, { ( 0x0000) } },

    { { { ( 0x1234), ( 0) } }, { ( 0x1234) } },
    { { { ( 0x1234), ( 4) } }, { ( 0x0123) } },
    { { { ( 0x1234), ( 8) } }, { ( 0x0012) } },
    { { { ( 0x1234), (12) } }, { ( 0x0001) } },

    { { { (-0x1234), ( 0) } }, { (-0x1234) } },
    { { { (-0x1234), ( 4) } }, { (-0x0123) } },
    { { { (-0x1234), ( 8) } }, { (-0x0012) } },
    { { { (-0x1234), (12) } }, { (-0x0001) } },

    { { { ( 0x123F), ( 0) } }, { ( 0x123F) } },
    { { { ( 0x123F), ( 4) } }, { ( 0x0124) } },
    { { { ( 0x123F), ( 8) } }, { ( 0x0012) } },
    { { { ( 0x123F), (12) } }, { ( 0x0001) } },

    { { { (-0x123F), ( 0) } }, { (-0x123F) } },
    { { { (-0x123F), ( 4) } }, { (-0x0124) } },
    { { { (-0x123F), ( 8) } }, { (-0x0012) } },
    { { { (-0x123F), (12) } }, { (-0x0001) } },
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.integral = LIB_Fixed_Round(vi.operand);

    if ( vo.integral != vor.integral )
    {
      result = FALSE;
    }
  }

  return (result);
}


#if LIB_FIXED_CONVERT_FLOATS

bool_t LIB_Fixed_FromFloat__test(void)
{
  typedef struct TestVectorInputType
  {
    float floating;
    LIB_Fixed_NotationType notation;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    LIB_Fixed_ValueType value;
    LIB_Fixed_ErrorType error;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { (0.0f), (0) }, { { (0), (0) }, LIB_FIXED_ERROR_OK } },

    /* TODO */
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.error = LIB_Fixed_FromFloat(vi.floating, vi.notation, &vo.value);

    if ( vo.error != vor.error )
    {
      result = FALSE;
    }
    else if ( LIB_FIXED_ERROR_OK == vo.error )
    {
      if ( vo.value.fractional != vor.value.fractional )
      {
        result = FALSE;
      }
      if ( vo.value.notation != vor.value.notation )
      {
        result = FALSE;
      }
    }
    else
    {
      /* Ok */
    }
  }

  return (result);
}


bool_t LIB_Fixed_ToFloat__test(void)
{
  typedef struct TestVectorInputType
  {
    LIB_Fixed_ValueType value;
  } TestVectorInputType;
  typedef struct TestVectorOutputType
  {
    float floating;
  } TestVectorOutputType;
  typedef struct TestVectorType
  {
    TestVectorInputType i;
    TestVectorOutputType o;
  } TestVectorType;

  static const TestVectorType tests[] =
  {
    { { { (0), (0) } }, { (0.0f) } },

    /* TODO */
  };

  TestVectorInputType vi;
  TestVectorOutputType vo;
  TestVectorOutputType vor;
  uint8_t n;
  bool_t result = TRUE;
  static uint8_t ntest = 255;

  for ( n = 0; n < (sizeof(tests) / sizeof(TestVectorType)); ++n )
  {
    vi = tests[n].i;
    vo = tests[n].o;
    vor = vo;

    if ( n == ntest )
    {
      n = ntest;
    }

    vo.floating = LIB_Fixed_ToFloat(vi.value);

    if ( fabs(vo.floating - vor.floating) < 0.001f )
    {
      result = FALSE;
    }
  }

  return (result);
}

#endif  /* LIB_FIXED_CONVERT_FLOATS */

#endif  /* LIB_FIXED_GENERATE_TESTS */
