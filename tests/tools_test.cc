#include "lib/include/hensel_code.h"
#include "lib/include/rational.h"
#include "lib/include/tools.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vector.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

TEST(ToolsTest, RandomInteger) {
  long bound = 30;
  NTL::ZZ number = GenerateNonZeroRandomInteger(NTL::ZZ(bound));

  EXPECT_TRUE((0 <= number) && (number < bound));
}

TEST(ToolsTest, RandomVector) {
  long bound = 50;
  long size1 = 3;
  long size2 = 2;
  std::vector<pie::Rational> v1 = GenerateRandomVector(size1, NTL::ZZ(bound));
  std::vector<pie::Rational> v2 = GenerateRandomVector(size2, NTL::ZZ(bound));

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
  NTL::Vec<NTL::ZZ> v = CreateVectorFromUniqueValue(size, value);

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
              DotProduct(v1, v2).ToString());
  EXPECT_TRUE(expected_dot_product2 == DotProduct(v3, v4));
}

TEST(ToolsTest, RationalProduct) {
  std::vector<pie::Rational> rationals;
  rationals.push_back(pie::Rational(2, 3));
  rationals.push_back(pie::Rational(4, 5));
  rationals.push_back(pie::Rational(6, 7));

  pie::Rational expected_product = pie::Rational(16, 35);

  EXPECT_TRUE(expected_product.ToString() == Product(rationals).ToString());
}

TEST(ToolsTest, IntegerProduct) {
  NTL::Vec<NTL::ZZ> integers;
  NTL::ZZ modulus = NTL::ZZ(64);

  integers.SetLength(3);
  integers[0] = NTL::ZZ(13);
  integers[1] = NTL::ZZ(27);
  integers[2] = NTL::ZZ(42);

  NTL::ZZ expected_product = NTL::ZZ(22);

  EXPECT_EQ(expected_product, Product(integers, modulus));
}