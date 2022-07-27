#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <vector>

TEST(HenselCodeTest, HenselCodeConstructor) {

  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  HenselCode hensel_code = HenselCode(prime, r);

  EXPECT_EQ(prime, hensel_code.prime);
  EXPECT_EQ(r, hensel_code.r);
}

TEST(HenselCodeTest, Encode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  HenselCode hensel_code = HenselCode(prime, r);
  Rational rational = Rational(NTL::ZZ(4), NTL::ZZ(9));
  NTL::ZZ h = hensel_code.Encode(rational);

  EXPECT_EQ(7544264, h);
}

TEST(HenselCodeTest, Decode) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  HenselCode hensel_code = HenselCode(prime, r);
  Rational rational = Rational(NTL::ZZ(4), NTL::ZZ(9));
  NTL::ZZ h = hensel_code.Encode(rational);
  Rational rational_decoded = hensel_code.Decode(h);

  EXPECT_EQ("4/9", rational_decoded.ToString());
}

TEST(HenselCodeTest, EncodeVector) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  HenselCode hensel_code = HenselCode(prime, r);

  std::vector<Rational> v = {Rational(NTL::ZZ(2), NTL::ZZ(3)),
                             Rational(NTL::ZZ(4), NTL::ZZ(5)),
                             Rational(NTL::ZZ(6), NTL::ZZ(7))};
  NTL::Vec<NTL::ZZ> vh = hensel_code.EncodeVector(v);

  EXPECT_EQ(v.size(), vh.length());
  EXPECT_EQ(11316396, vh[0]);
  EXPECT_EQ(6789838, vh[1]);
  EXPECT_EQ(14549652, vh[2]);
}

TEST(HenselCodeTest, DecodeVector) {
  NTL::ZZ prime = NTL::ZZ(257);
  int r = 3;
  HenselCode hensel_code = HenselCode(prime, r);

  std::vector<Rational> v = {Rational(NTL::ZZ(2), NTL::ZZ(3)),
                             Rational(NTL::ZZ(4), NTL::ZZ(5)),
                             Rational(NTL::ZZ(6), NTL::ZZ(7))};
  NTL::Vec<NTL::ZZ> vh = hensel_code.EncodeVector(v);
  std::vector<Rational> v_decoded = hensel_code.DecodeVector(vh);

  EXPECT_EQ(v.size(), v_decoded.size());
  EXPECT_EQ("2/3", v_decoded[0].ToString());
  EXPECT_EQ("4/5", v_decoded[1].ToString());
  EXPECT_EQ("6/7", v_decoded[2].ToString());
}