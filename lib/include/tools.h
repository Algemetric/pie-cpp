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

  NTL::ZZ RandomNumberFromRange(const NTL::ZZ &from, const NTL::ZZ &to);

  NTL::ZZX MulMod(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f);

  NTL::ZZX ModP(const NTL::ZZX &a, const NTL::ZZ &p);

  NTL::ZZX MulModP(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f, const NTL::ZZ &p);

  NTL::ZZX AddModP(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f, const NTL::ZZ &p);

  NTL::ZZX RandomPolynomial(const long &from, const long &to, const long &n);

  NTL::ZZX RandomPolynomial(const NTL::ZZ &from, const NTL::ZZ &to, const long &n);

  NTL::ZZX NormalPolynomial(const long &n, const long &mean, const float &stddev);

  NTL::ZZX ComputeModulusPolynomial(const long &n);

  NTL::ZZ SymMod(const NTL::ZZ &a, const NTL::ZZ &m);
}
