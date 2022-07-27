#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <math.h>
#include <vector>

HenselCode::HenselCode(const NTL::ZZ &prime_, const long &r_exponent) {
  this->prime = prime_;
  this->r = r_exponent;
  this->modulus = NTL::power(this->prime, this->r);
}

NTL::ZZ HenselCode::Encode(Rational &m_prime) const {
  NTL::ZZ den_inv = InvMod(m_prime.denominator, power(this->prime, this->r));
  NTL::ZZ hencoded =
      MulMod(m_prime.numerator, den_inv, power(this->prime, this->r));

  return hencoded;
}

Rational HenselCode::Decode(NTL::ZZ &hencoded) const {
  int index = 0;
  NTL::ZZ big_n = SqrRoot((prime - 1) / 2);
  NTL::Vec<NTL::ZZ> x;
  x.SetLength(2);
  x[index] = power(this->prime, this->r);
  x[index + 1] = hencoded;

  NTL::Vec<NTL::ZZ> y;
  y.SetLength(2);
  y[index] = NTL::ZZ(0);
  y[index + 1] = NTL::ZZ(1);

  NTL::Vec<NTL::ZZ> z;
  z.SetLength(2);
  z[index] = NTL::ZZ(1);
  z[index + 1] = NTL::ZZ(0);
  NTL::ZZ q;
  index = 1;

  while ((x[index] > big_n) != 0) {
    x.SetLength(x.length() + 2);
    y.SetLength(y.length() + 2);
    z.SetLength(z.length() + 2);
    q = x[index - 1] / x[index];
    x[index + 1] = x[index - 1] - q * x[index];
    y[index + 1] = y[index - 1] - q * y[index];
    z[index + 1] = z[index - 1] - q * z[index];
    index++;
  }

  NTL::ZZ c = NTL::power(NTL::ZZ(-1), (index + 1)) * x[index];
  NTL::ZZ d = NTL::power(NTL::ZZ(-1), (index + 1)) * y[index];

  Rational m = Rational(c, d);

  return m;
}

NTL::Vec<NTL::ZZ> HenselCode::EncodeVector(std::vector<Rational> vector) const {
  NTL::Vec<NTL::ZZ> vencoded;
  vencoded.SetLength(vector.size());

  for (long i = 0; i < vencoded.length(); i++) {
    vencoded[i] = Encode(vector[i]);
  }

  return vencoded;
}

std::vector<Rational> HenselCode::DecodeVector(NTL::Vec<NTL::ZZ> &vencoded) {
  std::vector<Rational> vector;
  Rational rational;

  for (long i = 0; i < vencoded.length(); i++) {
    rational = Decode(vencoded[i]);
    vector.push_back(rational);
  }

  return vector;
}

NTL::ZZ HenselCode::Add(NTL::ZZ &h1, NTL::ZZ &h2) {
  return AddMod(h1, h2, this->modulus);
}

NTL::ZZ HenselCode::Sub(NTL::ZZ &h1, NTL::ZZ &h2) {
  return SubMod(h1, h2, this->modulus);
}

NTL::ZZ HenselCode::Mul(NTL::ZZ &h1, NTL::ZZ &h2) {
  return MulMod(h1, h2, this->modulus);
}

NTL::ZZ HenselCode::Div(NTL::ZZ &h1, NTL::ZZ &h2) {
  NTL::ZZ h2_inv = InvMod(h2, this->modulus);
  return MulMod(h1, h2_inv, this->modulus);
}