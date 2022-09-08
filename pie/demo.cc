#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "lib/include/tools.h"
#include "lib/include/helpers.h"
#include <NTL/ZZ.h>
#include <iostream>
#include <string>
#include <unistd.h>

int main() {

  Title("PIE Encoder");

  Header("Rational numbers");

  Section("Initializing rational from NTL::ZZ numerators and denominators");
  
  pie::Rational a;
  a.numerator = NTL::ZZ(3);
  a.denominator = NTL::ZZ(5);

  pie::Rational b = pie::Rational(NTL::ZZ(2), NTL::ZZ(7));

  std::cout << "a: " << a.ToString() << "\n";
  std::cout << "b: " << b.ToString() << "\n\n";

  Section("Initializing rational from long numerators and denominators");

  pie::Rational c = pie::Rational(17, 28);

  std::cout << "c: " << c.ToString() << "\n\n";

  Section("Initializing rational from string");

  pie::Rational d = pie::StringToRational("38/15");

  std::cout << "d: " << d.ToString() << "\n\n";

  Section("Initializing reducible rational => 18/16");

  pie::Rational e = pie::StringToRational("18/16");

  std::cout << "e: " << e.ToString() << "\n\n";

  Section("Addition => a + b + c + d + e");

  pie::Rational r1 = a + b + c + d + e;

  std::cout << "r1: " << r1.ToString() << "\n\n";

  Section("Subtraction => a - b - c - d - e");

  pie::Rational r2 = a - b - c - d - e;

  std::cout << "r2: " << r2.ToString() << "\n\n";

  Section("Multiplication => a * b * c * d * e");

  pie::Rational r3 = a * b * c * d * e;

  std::cout << "r3: " << r3.ToString() << "\n\n";

  Section("Division => a / b / c / d / e");

  pie::Rational r4 = a / b / c / d / e;

  std::cout << "r4: " << r4.ToString() << "\n\n";

  Header("Hensel Codes");

  NTL::ZZ prime = pie::StringToZZ("16966601356614805487");
  long r = 1;

  Section("Encode");

  pie::HenselCode h1 = Encode(prime, r, a);
  pie::HenselCode h2 = Encode(prime, r, b);
  pie::HenselCode h3 = Encode(prime, r, c);
  pie::HenselCode h4 = Encode(prime, r, d);
  pie::HenselCode h5 = Encode(prime, r, e);

  std::cout << "h1: " << h1.code << "\n";
  std::cout << "h2: " << h2.code << "\n";
  std::cout << "h3: " << h3.code << "\n";
  std::cout << "h4: " << h4.code << "\n";
  std::cout << "h5: " << h5.code << "\n\n";

  Section("Decode");

  pie::Rational h1_d = Decode(prime, r, h1);
  pie::Rational h2_d = Decode(prime, r, h2);
  pie::Rational h3_d = Decode(prime, r, h3);
  pie::Rational h4_d = Decode(prime, r, h4);
  pie::Rational h5_d = Decode(prime, r, h5);

  std::cout << "h1_d: " << h1_d.ToString() << "\n";
  std::cout << "h2_d: " << h2_d.ToString() << "\n";
  std::cout << "h3_d: " << h3_d.ToString() << "\n";
  std::cout << "h4_d: " << h4_d.ToString() << "\n";
  std::cout << "h5_d: " << h5_d.ToString() << "\n\n";

  Section("Addition => h1 + h2");

  pie::HenselCode h6 = h1 + h2;

  pie::Rational h6_d = Decode(prime, r, h6);

  std::cout << "h6: " << h6.code << "\n";
  std::cout << "h6_d: " << h6_d.ToString() << "\n\n";

  Section("Subtraction => h1 - h2");

  pie::HenselCode h7 = h1 - h2;

  pie::Rational h7_d = Decode(prime, r, h7);

  std::cout << "h7: " << h7.code << "\n";
  std::cout << "h7_d: " << h7_d.ToString() << "\n\n";

  Section("Multiplication => h1 * h2");

  pie::HenselCode h8 = h1 * h2;

  pie::Rational h8_d = Decode(prime, r, h8);

  std::cout << "h8: " << h8.code << "\n";
  std::cout << "h8_d: " << h8_d.ToString() << "\n\n";

  Section("Division => h1 / h2");

  pie::HenselCode h9 = h1 / h2;

  pie::Rational h9_d = Decode(prime, r, h9);

  std::cout << "h9: " << h9.code << "\n";
  std::cout << "h9_d: " << h9_d.ToString() << "\n\n";

  return 0;
}