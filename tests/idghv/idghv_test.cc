#include "lib/idghv/include/idghv.h"
#include "gtest/gtest.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <vector>

TEST(IDGHVTest, IDGHVParams) {

  pie::IDGHVParams params = pie::IDGHVParams(52, 37, 41, 1558, 900000, 661);

  EXPECT_EQ(52, params.lambda);
  EXPECT_EQ(37, params.l);
  EXPECT_EQ(41, params.rho);
  EXPECT_EQ(1558, params.eta);
  EXPECT_EQ(900000, params.gamma);
  EXPECT_EQ(661, params.tau);
}

TEST(IDGHVTest, IDGHVKeyGen) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::IDGHVParams params = pie::IDGHVParams(52, 37, 41, 1558, 900000, 661);
  pie::IDGHV idghv = pie::IDGHV(pk, sk);
  idghv.KeyGen(params);

  EXPECT_EQ(idghv.params.lambda, params.lambda);
  EXPECT_EQ(idghv.params.l, params.l);
  EXPECT_EQ(idghv.params.rho, params.rho);
  EXPECT_EQ(idghv.params.eta, params.eta);
  EXPECT_EQ(idghv.params.gamma, params.gamma);
  EXPECT_EQ(idghv.params.tau, params.tau);

  EXPECT_EQ(idghv.pk.tau, params.tau);
  EXPECT_EQ(idghv.pk.big_qs.length(), params.l);
  EXPECT_EQ(idghv.pk.xs.length(), params.tau);
  EXPECT_EQ(idghv.pk.xs_.length(), params.l);

  for (long i = 0; i < params.l; i++) {
    EXPECT_EQ(NTL::NumBits(idghv.pk.big_qs[i]), params.rho);
  }
}

TEST(IDGHVTest, IDGHVEncrypt) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::IDGHVParams params = pie::IDGHVParams(52, 37, 41, 1558, 900000, 661);
  pie::IDGHV idghv = pie::IDGHV(pk, sk);
  idghv.KeyGen(params);

  pie::Rational m = pie::Rational(2, 3);
  pie::HenselCode c = pie::IDGHVEncrypt(params, idghv.pk, m);
  pie::Rational m_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c);

  EXPECT_EQ(m.ToString(), m_r.ToString());
}

TEST(IDGHVTest, IDGHVHomomorphism) {

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  pie::IDGHVParams params = pie::IDGHVParams(52, 37, 41, 1558, 900000, 661);
  pie::IDGHV idghv = pie::IDGHV(pk, sk);
  idghv.KeyGen(params);

  pie::Rational m1 = pie::Rational(2, 3);
  pie::Rational m2 = pie::Rational(5, 11);
  pie::HenselCode c1 = pie::IDGHVEncrypt(params, idghv.pk, m1);
  pie::HenselCode c2 = pie::IDGHVEncrypt(params, idghv.pk, m2);

  pie::HenselCode c1_plus_c2 = c1 + c2;
  pie::HenselCode c1_times_c2 = c1 * c2;

  pie::Rational m1_plus_m2_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c1_plus_c2);
  pie::Rational m1_times_m2_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c1_times_c2);

  EXPECT_EQ((m1 + m2).ToString(), m1_plus_m2_r.ToString());
  EXPECT_EQ((m1 * m2).ToString(), m1_times_m2_r.ToString());
}