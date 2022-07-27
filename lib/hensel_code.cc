#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <math.h>
#include <vector>

namespace pie
{
  HenselCode::HenselCode(const NTL::ZZ &prime_, const long &r_exponent, const NTL::ZZ &code) {
    this->prime = prime_;
    this->r = r_exponent;
    this->modulus = NTL::power(this->prime, this->r);
    this->code = code;
  }

  HenselCode HenselCode::operator+(HenselCode &other) {
    NTL::ZZ code = AddMod(this->code, other.code, this->modulus);
    return HenselCode(this->prime, this->r, code);
  }

  HenselCode HenselCode::operator-(HenselCode &other) {
    NTL::ZZ code = SubMod(this->code, other.code, this->modulus);
    return HenselCode(this->prime, this->r, code);
  }

  HenselCode HenselCode::operator*(HenselCode &other) {
    NTL::ZZ code = MulMod(this->code, other.code, this->modulus);
    return HenselCode(this->prime, this->r, code);
  }

  HenselCode HenselCode::operator/(HenselCode &other) {
    NTL::ZZ other_code_inv = InvMod(other.code, this->modulus);
    NTL::ZZ code = MulMod(this->code, other_code_inv, this->modulus);
    return HenselCode(this->prime, this->r, code);
  }

  HenselCode Encode(const NTL::ZZ &prime_, const long &r_exponent, pie::Rational &m_prime) {
    NTL::ZZ den_inv = InvMod(m_prime.denominator, power(prime_, r_exponent));
    NTL::ZZ hencoded =
        MulMod(m_prime.numerator, den_inv, power(prime_, r_exponent));
    
    HenselCode h = HenselCode(prime_, r_exponent, hencoded);

    return h;
  }

  pie::Rational Decode(const NTL::ZZ &prime_, const long &r_exponent, const HenselCode &hensel_code) {
    int index = 0;
    NTL::ZZ big_n = SqrRoot((prime_ - 1) / 2);
    NTL::Vec<NTL::ZZ> x;
    x.SetLength(2);
    x[index] = power(prime_, r_exponent);
    x[index + 1] = hensel_code.code;

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

    pie::Rational m = pie::Rational(c, d);

    return m;
  }
}