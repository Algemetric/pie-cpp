#include "lib/include/rational.h"
#include "lib/include/tools.h"
#include <NTL/ZZ.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace pie
{
  Rational::Rational() {
    this->numerator = NTL::ZZ(0);
    this->denominator = NTL::ZZ(1);
  }

  Rational::Rational(NTL::ZZ numerator_, NTL::ZZ denominator_) {
    this->numerator = std::move(numerator_);
    this->denominator = std::move(denominator_);
    GCD();
  }

  void Rational::GCD() {
    NTL::ZZ g = NTL::GCD(this->numerator, this->denominator);
    this->numerator = numerator / g;
    this->denominator = denominator / g;
  }

  Rational::Rational(const long num, const long den) {
    NTL::ZZ numerator_ = NTL::ZZ(num);
    NTL::ZZ denominator_ = NTL::ZZ(den);
    NTL::ZZ g = NTL::GCD(numerator_, denominator_);
    this->numerator = numerator_ / g;
    this->denominator = denominator_ / g;
  }

  std::string Rational::ToString() const {
    std::stringstream buffer;
    buffer << this->numerator << "/" << this->denominator;
    return buffer.str();
  }

  Rational Rational::operator+(Rational &other) const {
    Rational result = Rational(this->numerator * other.denominator +
                                  this->denominator * other.numerator,
                              this->denominator * other.denominator);
    result.GCD();
    return result;
  }

  Rational Rational::operator-(Rational &other) const {
    Rational result = Rational(this->numerator * other.denominator -
                                  this->denominator * other.numerator,
                              this->denominator * other.denominator);
    result.GCD();
    return result;
  }

  Rational Rational::operator*(Rational &other) const {
    Rational result = Rational(this->numerator * other.numerator,
                              this->denominator * other.denominator);
    result.GCD();
    return result;
  }

  Rational Rational::operator/(Rational &other) const {
    Rational result = Rational(this->numerator * other.denominator,
                              this->denominator * other.numerator);
    result.GCD();
    return result;
  }

  bool Rational::operator==(Rational &other) const {
    return ((this->numerator == other.numerator) != 0) &&
          ((this->denominator == other.denominator) != 0);
  }

  bool IsStringInteger(std::string string) {
    if (string.empty() ||
        ((isdigit(string[0]) == 0) && (string[0] != '-') && (string[0] != '+')))
      return false;

    char *res;
    strtol(string.c_str(), &res, 10);

    return (*res == 0);
  }

  long StringCharCount(char character, std::string string) {
    return std::count(string.begin(), string.end(), character);
  }

  Rational StringToRational(const std::string &string) {
    std::vector<std::string> sv_;
    std::stringstream rational_ss;
    std::string s;
    NTL::ZZ numerator;
    NTL::ZZ denominator;

    rational_ss << string;

    if (static_cast<int>(IsStringInteger(string)) == 1 &&
        StringCharCount('/', string) == 0) {
      numerator = StringToZZ(string);
      denominator = NTL::ZZ(1);
    } else if (StringCharCount('/', string) == 1) {
      while (std::getline(rational_ss, s, '/')) {
        sv_.push_back(s);
      }

      if (IsStringInteger(sv_[0]) && IsStringInteger(sv_[1])) {
        numerator = StringToZZ(sv_[0]);
        denominator = StringToZZ(sv_[1]);
      } else {
        numerator = NTL::ZZ(0);
        denominator = NTL::ZZ(1);
      }
    } else {
      numerator = NTL::ZZ(0);
      denominator = NTL::ZZ(1);
    }

    Rational rational = Rational(numerator, denominator);

    return rational;
  }
}