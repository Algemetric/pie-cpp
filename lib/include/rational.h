#pragma once

#include <NTL/ZZ.h>
#include <string>

namespace pie
{
  struct Rational {
    NTL::ZZ numerator;
    NTL::ZZ denominator;

    Rational();

    Rational(NTL::ZZ numerator_, NTL::ZZ denominator_);

    Rational(long num, long den);

    void GCD();

    std::string ToString() const;

    Rational operator+(Rational &ohter) const;

    Rational operator-(Rational &ohter) const;

    Rational operator*(Rational &other) const;

    Rational operator/(Rational &other) const;

    bool operator==(Rational &other) const;
  };

  bool IsStringInteger(std::string string);

  long StringCharCount(char character, std::string string);

  Rational StringToRational(const std::string &string);
}