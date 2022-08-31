#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <NTL/ZZXFactoring.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <vector>

TEST(HenselCodeTest, HenselCodeConstructor) {

  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  NTL::ZZ code = NTL::ZZ(172);
  pie::HenselCode h = pie::HenselCode(prime, r, code);

  EXPECT_EQ(prime, h.prime);
  EXPECT_EQ(r, h.r);
  EXPECT_EQ(code, h.code);
}

TEST(HenselCodeTest, Encode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  pie::Rational rational = pie::Rational(4, 9);
  pie::HenselCode h = pie::Encode(prime, r, rational);

  EXPECT_EQ(7544264, h.code);
}

TEST(HenselCodeTest, Decode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  pie::Rational rational = pie::Rational(4, 9);
  pie::HenselCode h = pie::Encode(prime, r, rational);
  pie::Rational rational_decoded = pie::Decode(prime, r, h);

  EXPECT_EQ("4/9", rational_decoded.ToString());
}

TEST(HenselCodeTest, PolyEncode) {
  long b = 257;
  long n = 3;
  pie::Rational rational = pie::Rational(2, 5);
  NTL::ZZX hx = pie::PolyEncode(b, n, rational);

  NTL::ZZX expected_hx;
  expected_hx.SetLength(3);
  expected_hx[0] = NTL::ZZ(102);
  expected_hx[1] = NTL::ZZ(-52);
  expected_hx[2] = NTL::ZZ(-102);

  EXPECT_EQ(expected_hx, hx);
}