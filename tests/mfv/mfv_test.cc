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

TEST(MFVTest, MFVMultiplyPrime) {

  pie::MFVPublicKey pk;
  pie::MFVPrivateKey sk;
  pie::MFVEvaluationKey evk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk, evk);
  mfv.KeyGen(params);

  NTL::Vec<NTL::ZZX> c1;
  NTL::Vec<NTL::ZZX> c2;
  c1.SetLength(2);
  c2.SetLength(2);

  NTL::ZZX c11;
  NTL::ZZX c12;
  NTL::ZZX c21;
  NTL::ZZX c22;
  c11.SetLength(mfv.params.n);
  c12.SetLength(mfv.params.n);
  c21.SetLength(mfv.params.n);
  c22.SetLength(mfv.params.n);

  c11[0] = NTL::ZZ(358080);
  c11[1] = NTL::ZZ(-456800);
  c11[2] = NTL::ZZ(-814686);
  c11[3] = NTL::ZZ(672921);

  c12[0] = NTL::ZZ(-456708);
  c12[1] = NTL::ZZ(-812076);
  c12[2] = NTL::ZZ(751287);
  c12[3] = NTL::ZZ(313769);

  c21[0] = NTL::ZZ(530549);
  c21[1] = NTL::ZZ(-647093);
  c21[2] = NTL::ZZ(-440751);
  c21[3] = NTL::ZZ(-806081);

  c22[0] = NTL::ZZ(192763);
  c22[1] = NTL::ZZ(-439345);
  c22[2] = NTL::ZZ(-764083);
  c22[3] = NTL::ZZ(729172);

  NTL::ZZX expected_c_11;
  expected_c_11.SetLength(mfv.params.n);
  expected_c_11[0] = NTL::ZZ(796890000000);
  expected_c_11[1] = NTL::ZZ(101870000000);
  expected_c_11[2] = NTL::ZZ(834180000000);
  expected_c_11[3] = NTL::ZZ(-247970000000);

  NTL::ZZX expected_c_12;
  expected_c_12.SetLength(mfv.params.n);
  expected_c_12[0] = NTL::ZZ(-735814);
  expected_c_12[1] = NTL::ZZ(356743);
  expected_c_12[2] = NTL::ZZ(69486);
  expected_c_12[3] = NTL::ZZ(595292);

  NTL::ZZX expected_c_21;
  expected_c_21.SetLength(mfv.params.n);
  expected_c_21[0] = NTL::ZZ(1504200000000);
  expected_c_21[1] = NTL::ZZ(287500000000);
  expected_c_21[2] = NTL::ZZ(-1471400000000);
  expected_c_21[3] = NTL::ZZ(-657700000000);

  NTL::ZZX expected_c_22;
  expected_c_22.SetLength(mfv.params.n);
  expected_c_22[0] = NTL::ZZ(178868);
  expected_c_22[1] = NTL::ZZ(707867);
  expected_c_22[2] = NTL::ZZ(695488);
  expected_c_22[3] = NTL::ZZ(773625);

  NTL::ZZX expected_c_31;
  expected_c_21.SetLength(mfv.params.n);
  expected_c_21[0] = NTL::ZZ(17900000000);
  expected_c_21[1] = NTL::ZZ(-1216000000000);
  expected_c_21[2] = NTL::ZZ(264000000000);
  expected_c_21[3] = NTL::ZZ(-621800000000);

  NTL::ZZX expected_c_32;
  expected_c_22.SetLength(mfv.params.n);
  expected_c_22[0] = NTL::ZZ(636216);
  expected_c_22[1] = NTL::ZZ(41462);
  expected_c_22[2] = NTL::ZZ(-45942);
  expected_c_22[3] = NTL::ZZ(-662343);

  NTL::Vec<NTL::ZZX> cs = MFVMulPrime(mfv.params, c1, c2);

  EXPECT_EQ(expected_c_11, c_11);
  EXPECT_EQ(expected_c_12, c_12);
  EXPECT_EQ(expected_c_21, c_21);
  EXPECT_EQ(expected_c_22, c_22);
  EXPECT_EQ(expected_c_31, c_31);
  EXPECT_EQ(expected_c_32, c_32);
}

