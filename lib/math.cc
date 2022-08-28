#include "lib/include/math.h"
#include <NTL/ZZ.h>
#include <math.h>

namespace pie {
  void SumMul(NTL::ZZ &result, const NTL::ZZ &op1, const NTL::ZZ &op2, const long &limit) {
    result = 1;
    for (long i = 0; i < limit; i++) {
      NTL::mul(result, result, op1 * op2);
    }
  }
}