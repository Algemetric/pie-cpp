#pragma once

#include <NTL/ZZ.h>

namespace pie {
  void SumMul(NTL::ZZ &result, const NTL::ZZ &op1, const NTL::ZZ &op2);
}