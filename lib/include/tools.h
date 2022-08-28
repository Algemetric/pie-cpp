#pragma once

#include "lib/include/rational.h"
#include <NTL/ZZ.h>

namespace pie {
  NTL::ZZ GenerateNonZeroRandomInteger(const NTL::ZZ &upperbound);

  pie::Rational GenerateRandomRational(const NTL::ZZ &upperbound);

  std::vector<pie::Rational> GenerateRandomVector(int size, const NTL::ZZ &upperbound);

  NTL::Vec<NTL::ZZ> CreateVectorFromUniqueValue(long size, const NTL::ZZ &value);

  pie::Rational DotProduct(std::vector<pie::Rational> vector1,
                      std::vector<pie::Rational> vector2);

  pie::Rational Product(std::vector<pie::Rational> rationals);

  NTL::ZZ Product(NTL::Vec<NTL::ZZ> integers, const NTL::ZZ &modulus);

  NTL::ZZ DotProduct(NTL::Vec<NTL::ZZ> vector1, NTL::Vec<NTL::ZZ> vector2);

  NTL::ZZ StringToZZ(const std::string &string);

  NTL::ZZ Product(NTL::Vec<NTL::ZZ> v);

  NTL::ZZ CRT(NTL::Vec<NTL::ZZ> m, NTL::Vec<NTL::ZZ> r);

  NTL::ZZ Delta(long i, long j);

  NTL::ZZ ModInverse(NTL::ZZ a, NTL::ZZ m);
}
