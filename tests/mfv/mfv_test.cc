#include "lib/mfv/include/mfv.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <vector>

TEST(MFVTest, MFVParams) {

  pie::MFVParams params = pie::MFVParams(1679617, 3.19, 30, 4, 4, 10);

  EXPECT_EQ(52, params.q);
  EXPECT_EQ(37, params.sigma);
  EXPECT_EQ(41, params.b);
  EXPECT_EQ(41, params.n);
  EXPECT_EQ(1558, params.w);
  EXPECT_EQ(900000, params.l);
}

TEST(MFVTest, MFVKeyGen) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk);
  mfv.KeyGen(params);

  EXPECT_EQ(mfv.params.n, mfv.sk.length());
  EXPECT_EQ(mfv.params.n, mfv.pk.length());
}

TEST(MFVTest, MFVEncryption) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk);
  mfv.KeyGen(params);

  pie::Rational m1 = pie::Rational(2, 5);
  pie::Rational m2 = pie::Rational(3, 4);

  NTL::Vec<NTL::ZZX> c1 = pie::MFVEncrypt(mfv.params, mfv.pk, m1);
  NTL::Vec<NTL::ZZX> c2 = pie::MFVEncrypt(mfv.params, mfv.pk, m2);

  pie::Rational m1_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1);
  pie::Rational m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c2);

  EXPECT_EQ(m1.ToString(), m1_r.ToString());
  EXPECT_EQ(m2.ToString(), m2_r.ToString());
}

TEST(MFVTest, MFVHomomorphicAddition) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk);
  mfv.KeyGen(params);

  pie::Rational m1 = pie::Rational(4, 9);
  pie::Rational m2 = pie::Rational(1, 7);

  NTL::Vec<NTL::ZZX> c1 = pie::MFVEncrypt(mfv.params, mfv.pk, m1);
  NTL::Vec<NTL::ZZX> c2 = pie::MFVEncrypt(mfv.params, mfv.pk, m2);

  NTL::Vec<NTL::ZZX> c1_plus_c2 = MFVAdd(mfv.params, c1, c2);

  pie::Rational m1_plus_m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1_plus_c2);

  EXPECT_EQ((m1 + m2).ToString(), m1_plus_m2_r.ToString());
}

