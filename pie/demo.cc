#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "lib/include/tools.h"
#include "pie/helpers.h"
#include <NTL/ZZ.h>
#include <iostream>
#include <string>
#include <unistd.h>

int main() {

  Title("PIE Encoder");

  Header("Rational numbers");

  Section("Initializing rational from NTL::ZZ numerators and denominators");
  
  Rational a;
  a.numerator = NTL::ZZ(3);
  a.denominator = NTL::ZZ(5);

  Rational b = Rational(NTL::ZZ(2), NTL::ZZ(7));

  std::cout << "a: " << a.ToString() << "\n";
  std::cout << "b: " << b.ToString() << "\n\n";

  Section("Initializing rational from long numerators and denominators");

  Rational c = Rational(17, 28);

  std::cout << "c: " << c.ToString() << "\n\n";

  Section("Initializing rational from string");

  Rational d = StringToRational("38/15");

  std::cout << "d: " << d.ToString() << "\n\n";

  Section("Initializing reducible rational => 18/16");

  Rational e = StringToRational("18/16");

  std::cout << "e: " << e.ToString() << "\n\n";

  Section("Addition => a + b + c + d + e");

  Rational r1 = a + b + c + d + e;

  std::cout << "r1: " << r1.ToString() << "\n\n";

  Section("Subtraction => a - b - c - d - e");

  Rational r2 = a - b - c - d - e;

  std::cout << "r2: " << r2.ToString() << "\n\n";

  Section("Multiplication => a * b * c * d * e");

  Rational r3 = a * b * c * d * e;

  std::cout << "r3: " << r3.ToString() << "\n\n";

  Section("Division => a / b / c / d / e");

  Rational r4 = a / b / c / d / e;

  std::cout << "r4: " << r4.ToString() << "\n\n";

  Header("Hensel Codes");

  NTL::ZZ prime = StringToZZ("16966601356614805487");
  HenselCode hensel_code = HenselCode(prime, 1);

  Section("Encode");

  NTL::ZZ h1 = hensel_code.Encode(a);
  NTL::ZZ h2 = hensel_code.Encode(b);
  NTL::ZZ h3 = hensel_code.Encode(c);
  NTL::ZZ h4 = hensel_code.Encode(d);
  NTL::ZZ h5 = hensel_code.Encode(e);

  std::cout << "h1: " << h1 << "\n";
  std::cout << "h2: " << h2 << "\n";
  std::cout << "h3: " << h3 << "\n";
  std::cout << "h4: " << h4 << "\n";
  std::cout << "h5: " << h5 << "\n\n";

  Section("Decode");

  Rational h1_d = hensel_code.Decode(h1);
  Rational h2_d = hensel_code.Decode(h2);
  Rational h3_d = hensel_code.Decode(h3);
  Rational h4_d = hensel_code.Decode(h4);
  Rational h5_d = hensel_code.Decode(h5);

  std::cout << "h1_d: " << h1_d.ToString() << "\n";
  std::cout << "h2_d: " << h2_d.ToString() << "\n";
  std::cout << "h3_d: " << h3_d.ToString() << "\n";
  std::cout << "h4_d: " << h4_d.ToString() << "\n";
  std::cout << "h5_d: " << h5_d.ToString() << "\n\n";

  Section("Addition => h1 + h2");

  NTL::ZZ h6 = hensel_code.Add(h1, h2);

  Rational h6_d = hensel_code.Decode(h6);

  std::cout << "h6: " << h6 << "\n";
  std::cout << "h6_d: " << h6_d.ToString() << "\n\n";

  Section("Subtraction => h1 - h2");

  NTL::ZZ h7 = hensel_code.Sub(h1, h2);

  Rational h7_d = hensel_code.Decode(h7);

  std::cout << "h7: " << h7 << "\n";
  std::cout << "h7_d: " << h7_d.ToString() << "\n\n";

  Section("Multiplication => h1 * h2");

  NTL::ZZ h8 = hensel_code.Mul(h1, h2);

  Rational h8_d = hensel_code.Decode(h8);

  std::cout << "h8: " << h8 << "\n";
  std::cout << "h8_d: " << h8_d.ToString() << "\n\n";

  Section("Division => h1 / h2");

  NTL::ZZ h9 = hensel_code.Div(h1, h2);

  Rational h9_d = hensel_code.Decode(h9);

  std::cout << "h9: " << h9 << "\n";
  std::cout << "h9_d: " << h9_d.ToString() << "\n\n";
  
  return 0;
}