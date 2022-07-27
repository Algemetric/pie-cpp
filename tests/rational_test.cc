#include "lib/include/rational.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <string>

TEST(RationalTest, RationalConstructor) {
  Rational rational = Rational(NTL::ZZ(8), NTL::ZZ(9));

  EXPECT_EQ(8, rational.numerator);
  EXPECT_EQ(9, rational.denominator);
}

TEST(RationalTest, IrreducibleFraction) {
  Rational rat1 = Rational(NTL::ZZ(8), NTL::ZZ(4));
  Rational rat2 = Rational(NTL::ZZ(9), NTL::ZZ(6));

  EXPECT_EQ(2, rat1.numerator);
  EXPECT_EQ(1, rat1.denominator);
  EXPECT_EQ(3, rat2.numerator);
  EXPECT_EQ(2, rat2.denominator);
}

TEST(RationalTest, ToString) {
  Rational rat1 = Rational(NTL::ZZ(5), NTL::ZZ(3));
  Rational rat2 = Rational(NTL::ZZ(7), NTL::ZZ(4));

  EXPECT_EQ("5/3", rat1.ToString());
  EXPECT_EQ("7/4", rat2.ToString());
}

TEST(RationalTest, Addition) {
  Rational rat1 = Rational(NTL::ZZ(2), NTL::ZZ(3));
  Rational rat2 = Rational(NTL::ZZ(3), NTL::ZZ(5));
  Rational rat3 = rat1 + rat2;

  EXPECT_EQ("19/15", rat3.ToString());
}

TEST(RationalTest, Subtraction) {
  Rational rat1 = Rational(NTL::ZZ(2), NTL::ZZ(3));
  Rational rat2 = Rational(NTL::ZZ(3), NTL::ZZ(5));
  Rational rat3 = rat1 - rat2;

  EXPECT_EQ("1/15", rat3.ToString());
}

TEST(RationalTest, Multiplication) {
  Rational rat1 = Rational(NTL::ZZ(2), NTL::ZZ(3));
  Rational rat2 = Rational(NTL::ZZ(3), NTL::ZZ(5));
  Rational rat3 = rat1 * rat2;

  EXPECT_EQ("2/5", rat3.ToString());
}

TEST(RationalTest, Division) {
  Rational rat1 = Rational(NTL::ZZ(2), NTL::ZZ(3));
  Rational rat2 = Rational(NTL::ZZ(7), NTL::ZZ(11));
  Rational rat3 = rat1 / rat2;

  EXPECT_EQ("22/21", rat3.ToString());
}

TEST(RationalTest, RationalEquality) {
  Rational rat1 = Rational(2, 3);
  Rational rat2 = Rational(2, 3);
  Rational rat3 = Rational(4, 3);

  EXPECT_TRUE(rat1 == rat2);
  EXPECT_FALSE(rat1 == rat3);
  EXPECT_FALSE(rat2 == rat3);
}

TEST(RationalTest, StringToRational) {
  std::string string1 = "2/3";
  std::string string2 = "-3/5";
  std::string string3 = "6/1";
  std::string string4 = "1//2";
  std::string string5 = "1/b";
  std::string string6 = "a/b";
  std::string string7 = "*";

  Rational rational1 = StringToRational(string1);
  Rational rational2 = StringToRational(string2);
  Rational rational3 = StringToRational(string3);
  Rational rational4 = StringToRational(string4);
  Rational rational5 = StringToRational(string5);
  Rational rational6 = StringToRational(string6);
  Rational rational7 = StringToRational(string7);

  EXPECT_EQ(string1, rational1.ToString());
  EXPECT_EQ(string2, rational2.ToString());
  EXPECT_EQ(string3, rational3.ToString());
  EXPECT_EQ("0/1", rational4.ToString());
  EXPECT_EQ("0/1", rational5.ToString());
  EXPECT_EQ("0/1", rational6.ToString());
  EXPECT_EQ("0/1", rational7.ToString());
}