#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <vector>

TEST(HenselCodeTest, HenselCodeConstructor) {

  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  NTL::ZZ code = NTL::ZZ(172);
  HenselCode h = HenselCode(prime, r, code);

  EXPECT_EQ(prime, h.prime);
  EXPECT_EQ(r, h.r);
  EXPECT_EQ(code, h.code);
}

TEST(HenselCodeTest, Encode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  Rational rational = Rational(4, 9);
  HenselCode h = Encode(prime, r, rational);

  EXPECT_EQ(7544264, h.code);
}

TEST(HenselCodeTest, Decode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  Rational rational = Rational(4, 9);
  HenselCode h = Encode(prime, r, rational);
  Rational rational_decoded = Decode(prime, r, h);

  EXPECT_EQ("4/9", rational_decoded.ToString());
}