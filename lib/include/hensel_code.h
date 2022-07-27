#pragma once

#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <vector>

class HenselCode {
public:
  NTL::ZZ prime;
  int r;
  NTL::ZZ modulus;
  NTL::ZZ code;

  HenselCode(const NTL::ZZ &prime_, const long &r_exponent, const NTL::ZZ &code);

  HenselCode operator+(HenselCode &other);

  HenselCode operator-(HenselCode &other);

  HenselCode operator*(HenselCode &other);

  HenselCode operator/(HenselCode &other);
};

HenselCode Encode(const NTL::ZZ &prime_, const long &r_exponent, Rational &m_prime);

Rational Decode(const NTL::ZZ &prime_, const long &r_exponent, const HenselCode &hensel_code);