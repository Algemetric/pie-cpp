#pragma once

#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <vector>

namespace pie
{
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

  HenselCode Encode(const NTL::ZZ &prime_, const long &r_exponent, pie::Rational &m_prime);

  pie::Rational Decode(const NTL::ZZ &prime_, const long &r_exponent, const HenselCode &hensel_code);

  NTL::ZZX PolyEncode(const long &b, const long &n, pie::Rational m);

  pie::Rational PolyDecode(const long &b, const long &n, NTL::ZZX hx);
}