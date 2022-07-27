#pragma once

#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <vector>

class HenselCode {
public:
  NTL::ZZ prime;
  int r;
  NTL::ZZ modulus;

  HenselCode(const NTL::ZZ &prime_, const long &r_exponent);

  NTL::ZZ Encode(Rational &m_prime) const;

  Rational Decode(NTL::ZZ &hencoded) const;

  NTL::Vec<NTL::ZZ> EncodeVector(std::vector<Rational> vector) const;

  std::vector<Rational> DecodeVector(NTL::Vec<NTL::ZZ> &vencoded);

  NTL::ZZ Add(NTL::ZZ &h1, NTL::ZZ &h2);

  NTL::ZZ Sub(NTL::ZZ &h1, NTL::ZZ &h2);

  NTL::ZZ Mul(NTL::ZZ &h1, NTL::ZZ &h2);

  NTL::ZZ Div(NTL::ZZ &h1, NTL::ZZ &h2);
};