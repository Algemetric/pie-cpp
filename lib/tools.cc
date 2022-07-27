#include "lib/include/tools.h"
#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vector.h>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>

NTL::ZZ GenerateNonZeroRandomInteger(const NTL::ZZ &upperbound) {
  NTL::ZZ number;
  while (true) {
    number = NTL::RandomBnd(upperbound);
    if ((number != 0) != 0) {
      break;
    }
  }
  return number;
}

pie::Rational GenerateRandomRational(const NTL::ZZ &upperbound) {
  NTL::ZZ numerator = GenerateNonZeroRandomInteger(upperbound);
  NTL::ZZ denominator = GenerateNonZeroRandomInteger(upperbound);

  return pie::Rational(numerator, denominator);
}

std::vector<pie::Rational> GenerateRandomVector(const int size,
                                           const NTL::ZZ &upperbound) {
  NTL::ZZ numerator;
  NTL::ZZ denominator;
  std::vector<pie::Rational> vector;

  for (int i = 0; i < size; i++) {
    numerator = GenerateNonZeroRandomInteger(upperbound);
    denominator = GenerateNonZeroRandomInteger(upperbound);
    vector.push_back(pie::Rational(numerator, denominator));
  }

  return vector;
}

NTL::Vec<NTL::ZZ> CreateVectorFromUniqueValue(long size, const NTL::ZZ &value) {
  NTL::Vec<NTL::ZZ> vector;
  vector.SetLength(size);

  for (long i = 0; i < size; i++) {
    vector[i] = value;
  }

  return vector;
}

pie::Rational DotProduct(std::vector<pie::Rational> vector1,
                    std::vector<pie::Rational> vector2) {
  pie::Rational result = pie::Rational(NTL::ZZ(0), NTL::ZZ(1));
  pie::Rational mul;

  for (long i = 0; i < vector1.size(); i++) {
    mul = vector1[i] * vector2[i];
    result = result + mul;
  }

  return result;
}

NTL::ZZ DotProduct(NTL::Vec<NTL::ZZ> vector1, NTL::Vec<NTL::ZZ> vector2) {
  NTL::ZZ result = NTL::ZZ(0);

  for (long i = 0; i < vector1.length(); i++) {
    result += vector1[i] * vector2[i];
  }

  return result;
}

pie::Rational Product(std::vector<pie::Rational> rationals) {
  long num = 1;
  long den = 1;
  pie::Rational product = pie::Rational(num, den);

  for (long i = 0; i < rationals.size(); i++) {
    product = product * rationals[i];
  }

  return product;
}

NTL::ZZ Product(NTL::Vec<NTL::ZZ> integers, const NTL::ZZ &modulus) {
  NTL::ZZ product = NTL::ZZ(1);

  for (long i = 0; i < integers.length(); i++) {
    product = NTL::MulMod(product, integers[i], modulus);
  }

  return product;
}

NTL::ZZ StringToZZ(const std::string &string) {
  NTL::ZZ zz_;
  std::stringstream ss_(string);
  ss_ >> zz_;
  return zz_;
}