#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "lib/include/tools.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZX.h>
#include <NTL/vector.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>

TEST(ToolsTest, RandomInteger) {
  long bound = 30;
  NTL::ZZ number = pie::GenerateNonZeroRandomInteger(NTL::ZZ(bound));

  EXPECT_TRUE((0 <= number) && (number < bound));
}

TEST(ToolsTest, RandomVector) {
  long bound = 50;
  long size1 = 3;
  long size2 = 2;
  std::vector<pie::Rational> v1 = pie::GenerateRandomVector(size1, NTL::ZZ(bound));
  std::vector<pie::Rational> v2 = pie::GenerateRandomVector(size2, NTL::ZZ(bound));

  EXPECT_EQ(size1, v1.size());
  EXPECT_TRUE((0 <= v1[0].numerator) && (v1[0].numerator < bound));
  EXPECT_TRUE((0 <= v1[0].denominator) && (v1[0].denominator < bound));
  EXPECT_TRUE((0 <= v1[1].numerator) && (v1[1].numerator < bound));
  EXPECT_TRUE((0 <= v1[1].denominator) && (v1[1].denominator < bound));
  EXPECT_TRUE((0 <= v1[2].numerator) && (v1[2].numerator < bound));
  EXPECT_TRUE((0 <= v1[2].denominator) && (v1[2].denominator < bound));
  EXPECT_EQ(size2, v2.size());
  EXPECT_TRUE((0 <= v2[0].numerator) && (v2[0].numerator < bound));
  EXPECT_TRUE((0 <= v2[0].denominator) && (v2[0].denominator < bound));
  EXPECT_TRUE((0 <= v2[1].numerator) && (v2[1].numerator < bound));
  EXPECT_TRUE((0 <= v2[1].denominator) && (v2[1].denominator < bound));
}

TEST(ToolsTest, VectorFromUniqueValue) {
  long size = 5;
  NTL::ZZ value = NTL::ZZ(36);
  NTL::Vec<NTL::ZZ> v = pie::CreateVectorFromUniqueValue(size, value);

  EXPECT_EQ(size, v.length());
  EXPECT_EQ(value, v[0]);
  EXPECT_EQ(value, v[1]);
  EXPECT_EQ(value, v[2]);
  EXPECT_EQ(value, v[3]);
  EXPECT_EQ(value, v[4]);
}

TEST(ToolsTest, DotProduct) {
  std::vector<pie::Rational> v1 = {pie::Rational(2, 3), pie::Rational(4, 5), pie::Rational(6, 7)};
  std::vector<pie::Rational> v2 = {pie::Rational(8, 9), pie::Rational(10, 11),
                              pie::Rational(12, 13)};
  pie::Rational expected_dot_product1 = pie::Rational(57056, 27027);

  NTL::Vec<NTL::ZZ> v3;
  v3.SetLength(3);
  v3[0] = NTL::ZZ(2);
  v3[1] = NTL::ZZ(3);
  v3[2] = NTL::ZZ(4);

  NTL::Vec<NTL::ZZ> v4;
  v4.SetLength(3);
  v4[0] = NTL::ZZ(5);
  v4[1] = NTL::ZZ(6);
  v4[2] = NTL::ZZ(7);
  NTL::ZZ expected_dot_product2 = NTL::ZZ(56);

  EXPECT_TRUE(expected_dot_product1.ToString() ==
              pie::DotProduct(v1, v2).ToString());
  EXPECT_TRUE(expected_dot_product2 == pie::DotProduct(v3, v4));
}

TEST(ToolsTest, RationalProduct) {
  std::vector<pie::Rational> rationals;
  rationals.push_back(pie::Rational(2, 3));
  rationals.push_back(pie::Rational(4, 5));
  rationals.push_back(pie::Rational(6, 7));

  pie::Rational expected_product = pie::Rational(16, 35);

  EXPECT_TRUE(expected_product.ToString() == pie::Product(rationals).ToString());
}

TEST(ToolsTest, IntegerProduct) {
  NTL::Vec<NTL::ZZ> integers;
  NTL::ZZ modulus = NTL::ZZ(64);

  integers.SetLength(3);
  integers[0] = NTL::ZZ(13);
  integers[1] = NTL::ZZ(27);
  integers[2] = NTL::ZZ(42);

  NTL::ZZ expected_product = NTL::ZZ(22);

  EXPECT_EQ(expected_product, pie::Product(integers, modulus));
}

TEST(ToolsTest, ProductOfVectorElements) {
  NTL::Vec<NTL::ZZ> v;
  v.SetLength(3);
  v[0] = 53;
  v[1] = 71;
  v[2] = 97;

  NTL::ZZ expected_product = NTL::ZZ(365011);

  EXPECT_EQ(expected_product, pie::Product(v));
}

TEST(ToolsTest, CRTOfVectorElements) {
  NTL::Vec<NTL::ZZ> m;
  m.SetLength(3);
  m[0] = 53;
  m[1] = 71;
  m[2] = 97;

  NTL::Vec<NTL::ZZ> r;
  r.SetLength(3);
  r[0] = 32;
  r[1] = 45;
  r[2] = 76;

  NTL::ZZ expected_crt = NTL::ZZ(200478);

  EXPECT_EQ(expected_crt, pie::CRT(m, r));
}

TEST(ToolsTest, SymMod) {
  NTL::ZZ a = NTL::ZZ(458);
  NTL::ZZ b = NTL::ZZ(-22);
  NTL::ZZ c = NTL::ZZ(-131);
  NTL::ZZ d = NTL::ZZ(129);
  NTL::ZZ e = NTL::ZZ(80);

  NTL::ZZ p = NTL::ZZ(257);

  EXPECT_EQ(NTL::ZZ(-56), pie::SymMod(a, p));
  EXPECT_EQ(NTL::ZZ(-22), pie::SymMod(b, p));
  EXPECT_EQ(NTL::ZZ(126), pie::SymMod(c, p));
  EXPECT_EQ(NTL::ZZ(-128), pie::SymMod(d, p));
  EXPECT_EQ(NTL::ZZ(80), pie::SymMod(e, p));
}

TEST(ToolsTest, PolySymMod) {
  NTL::ZZX a;
  a.SetLength(4);

  NTL::ZZ p = NTL::ZZ(257);

  a[0] = NTL::ZZ(399);
  a[1] = NTL::ZZ(-131);
  a[2] = NTL::ZZ(285);
  a[3] = NTL::ZZ(67);

  NTL::ZZX b = pie::ModP(a, p);

  EXPECT_EQ(NTL::ZZ(-115), b[0]);
  EXPECT_EQ(NTL::ZZ(126), b[1]);
  EXPECT_EQ(NTL::ZZ(28), b[2]);
  EXPECT_EQ(NTL::ZZ(67), b[3]);
}

TEST(ToolsTest, ReverseVecPolynomial) {
  NTL::Vec<NTL::ZZX> a;
  a.SetLength(2);

  NTL::ZZX a1;
  NTL::ZZX a2;
  a1.SetLength(4);
  a2.SetLength(4);

  a1[0] = NTL::ZZ(2);
  a1[1] = NTL::ZZ(3);
  a1[2] = NTL::ZZ(4);
  a1[3] = NTL::ZZ(5);

  a2[0] = NTL::ZZ(6);
  a2[1] = NTL::ZZ(7);
  a2[2] = NTL::ZZ(8);
  a2[3] = NTL::ZZ(9);

  a[0] = a1;
  a[1] = a2;

  EXPECT_EQ(2, a[0][0]);
  EXPECT_EQ(3, a[0][1]);
  EXPECT_EQ(4, a[0][2]);
  EXPECT_EQ(5, a[0][3]);

  a = pie::Reverse(a);

  EXPECT_EQ(6, a[0][0]);
  EXPECT_EQ(7, a[0][1]);
  EXPECT_EQ(8, a[0][2]);
  EXPECT_EQ(9, a[0][3]);

  EXPECT_EQ(2, a[1][0]);
  EXPECT_EQ(3, a[1][1]);
  EXPECT_EQ(4, a[1][2]);
  EXPECT_EQ(5, a[1][3]);

}

TEST(ToolsTest, ReverseVecZZ) {
  NTL::Vec<NTL::ZZ> a;
  a.SetLength(8);

  a[0] = NTL::ZZ(2);
  a[1] = NTL::ZZ(3);
  a[2] = NTL::ZZ(4);
  a[3] = NTL::ZZ(5);

  a[4] = NTL::ZZ(6);
  a[5] = NTL::ZZ(7);
  a[6] = NTL::ZZ(8);
  a[7] = NTL::ZZ(9);

  EXPECT_EQ(2, a[0]);
  EXPECT_EQ(3, a[1]);
  EXPECT_EQ(4, a[2]);
  EXPECT_EQ(5, a[3]);
  EXPECT_EQ(6, a[4]);
  EXPECT_EQ(7, a[5]);
  EXPECT_EQ(8, a[6]);
  EXPECT_EQ(9, a[7]);

  a = pie::Reverse(a);

  EXPECT_EQ(9, a[0]);
  EXPECT_EQ(8, a[1]);
  EXPECT_EQ(7, a[2]);
  EXPECT_EQ(6, a[3]);
  EXPECT_EQ(5, a[4]);
  EXPECT_EQ(4, a[5]);
  EXPECT_EQ(3, a[6]);
  EXPECT_EQ(2, a[7]);

}

TEST(ToolsTest, CreateVecZZXFromList) {
  NTL::ZZX a = pie::CreateVecZZXFromList({389048, 400519, 204647, 777223});

  EXPECT_EQ(NTL::ZZ(389048), a[0]);
  EXPECT_EQ(NTL::ZZ(400519), a[1]);
  EXPECT_EQ(NTL::ZZ(204647), a[2]);
  EXPECT_EQ(NTL::ZZ(777223), a[3]);
}