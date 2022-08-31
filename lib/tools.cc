#include "lib/include/tools.h"
#include "lib/include/rational.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vector.h>
#include <NTL/ZZX.h>
#include <NTL/ZZXFactoring.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <random>

namespace pie
{
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

  NTL::ZZ Product(NTL::Vec<NTL::ZZ> v) {
    NTL::ZZ product = NTL::ZZ(1);

    for (long i = 0; i < v.length(); i++) {
      product = product * v[i]; 
    }

    return product;
  }

  NTL::ZZ CRT(NTL::Vec<NTL::ZZ> m, NTL::Vec<NTL::ZZ> r) {
    NTL::ZZ g = Product(m);
    NTL::ZZ g_prime;
    NTL::ZZ g_prime_inverse;
    NTL::ZZ result = NTL::ZZ(0);

    for (long i = 0; i < m.length(); i++) {
      g_prime = g / m[i];
      g_prime_inverse = pie::ModInverse(g_prime, m[i]);
      result = result + (g_prime * g_prime_inverse * r[i]);
    }

    return result % g;
  }

  NTL::ZZ Delta(long i, long j) {
    NTL::ZZ delta;
    if (i == j) {
      delta = NTL::ZZ(1);
    } else {
      delta = NTL::ZZ(0);
    }
    return delta;
  }

  NTL::ZZ ModInverse(NTL::ZZ a, NTL::ZZ m) {
    NTL::ZZ d;
    NTL::ZZ s;
    NTL::ZZ t;
    NTL::XGCD(d, s, t, a, m);
    
    return s % m;
  }

  NTL::ZZ RandomNumberFromRange(const NTL::ZZ &from, const NTL::ZZ &to) {
    NTL::ZZ rnd;

    while (true) {
      rnd = NTL::RandomBnd(to);
      long bit = rand() % 2;
      NTL::ZZ sign = bit == 0 ? NTL::ZZ(1) : NTL::ZZ(-1);
      rnd = rnd * sign;

      if ((rnd >= from) && (rnd <= to)) {
        break;
      }
    }

    return rnd;
  }

  NTL::ZZX ModP(const NTL::ZZX &a, const NTL::ZZ &p) {
    NTL::ZZX b;
    b.SetLength(a.rep.length());

    for (long i = 0; i < b.rep.length(); i++) {
      b[i] = SymMod(a[i], p);
    }

    return b;
  }

  NTL::ZZX MulMod(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f) {
    return NTL::reverse(NTL::MulMod(NTL::reverse(a), NTL::reverse(b), f));
  }

  NTL::ZZX MulModP(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f, const NTL::ZZ &p) {
    return pie::ModP(NTL::MulMod(a, b, f), p);
  }

  NTL::ZZX AddModP(const NTL::ZZX &a, const NTL::ZZX &b, const NTL::ZZX &f, const NTL::ZZ &p) {
    return pie::ModP(a + b, p);
  }

  NTL::ZZX RandomPolynomial(const long &from, const long &to, const long &n) {
    NTL::ZZX a;
    a.SetLength(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(from, to);

    for (long i = 0; i < n; i++) {
      a[i] = NTL::ZZ(distr(gen));
    }

    return a;
  }

  NTL::ZZX RandomPolynomial(const NTL::ZZ &from, const NTL::ZZ &to, const long &n) {
    NTL::ZZX a;
    a.SetLength(n);

    for (long i = 0; i < n; i++) {
      a[i] = pie::RandomNumberFromRange(from, to);
    }

    return a;
  }

  NTL::ZZX NormalPolynomial(const long &n, const long &mean, const float &stddev) {
    NTL::ZZX a;
    a.SetLength(n);

    // std::default_random_engine generator;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, stddev);

    for (long i = 0; i < n; i++) {
      a[i] = NTL::ZZ((int)distribution(gen));
    }

    return a;
  }

  NTL::ZZX ComputeModulusPolynomial(const long &n) {
    NTL::ZZX f;

    f.SetLength(n + 1);
    f[0] = NTL::ZZ(1);
    f[n] = NTL::ZZ(1);

    return f;
  }

  NTL::ZZ SymMod(const NTL::ZZ &a, const NTL::ZZ &m) {
    NTL::ZZ r = a % m;

    if (r <= (m - 1)/2) {
      return r;
    } else {
      return r - m;
    }
  }
}
