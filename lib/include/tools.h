#pragma once

#include "lib/include/rational.h"
#include <NTL/ZZ.h>

NTL::ZZ GenerateNonZeroRandomInteger(const NTL::ZZ &upperbound);

Rational GenerateRandomRational(const NTL::ZZ &upperbound);

std::vector<Rational> GenerateRandomVector(int size, const NTL::ZZ &upperbound);

NTL::Vec<NTL::ZZ> CreateVectorFromUniqueValue(long size, const NTL::ZZ &value);

Rational DotProduct(std::vector<Rational> vector1,
                    std::vector<Rational> vector2);

Rational Product(std::vector<Rational> rationals);

NTL::ZZ Product(NTL::Vec<NTL::ZZ> integers, const NTL::ZZ &modulus);

NTL::ZZ DotProduct(NTL::Vec<NTL::ZZ> vector1, NTL::Vec<NTL::ZZ> vector2);

NTL::ZZ StringToZZ(const std::string &string);