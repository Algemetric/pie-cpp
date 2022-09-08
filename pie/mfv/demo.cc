#include "lib/include/hensel_code.h"
#include "lib/mfv/include/mfv.h"
#include "lib/include/helpers.h"
#include "lib/include/tools.h"
#include <NTL/ZZ.h>
#include <NTL/ZZXFactoring.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <NTL/RR.h>
#include <NTL/xdouble.h>
#include <NTL/quad_float.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>

int main() {

  Title("PIE + MFV");

  pie::MFVPublicKey pk;
  pie::MFVPrivateKey sk;
  pie::MFVEvaluationKey evk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk, evk);
  mfv.KeyGen(params);

  std::cout << "mfv.params.q: " << mfv.params.q << "\n";
  std::cout << "mfv.params.sigma: " << mfv.params.sigma << "\n";
  std::cout << "mfv.params.b: " << mfv.params.b << "\n";
  std::cout << "mfv.params.n: " << mfv.params.n << "\n";
  std::cout << "mfv.params.w: " << mfv.params.w << "\n";
  std::cout << "mfv.params.l: " << mfv.params.l << "\n\n";

  std::cout << "mfv.pk.p0: " << mfv.pk.p0 << "\n";
  std::cout << "mfv.pk.a: " << mfv.pk.a << "\n";
  std::cout << "mfv.sk.s: " << mfv.sk.s << "\n\n";

  Header("Messages");

  pie::Rational m1 = pie::Rational(2, 5);
  pie::Rational m2 = pie::Rational(3, 4);

  std::cout << "m1: " << m1.ToString() << "\n";
  std::cout << "m2: " << m2.ToString() << "\n\n";

  Header("Polynomial Encoder");

  NTL::ZZX m1x = pie::PolyEncode(mfv.params.b, mfv.params.n, m1);
  NTL::ZZX m2x = pie::PolyEncode(mfv.params.b, mfv.params.n, m2);

  pie::Rational m1x_r = pie::PolyDecode(mfv.params.b, mfv.params.n, m1x);
  pie::Rational m2x_r = pie::PolyDecode(mfv.params.b, mfv.params.n, m2x);

  std::cout << "m1x: " << m1x << "\n";
  std::cout << "m2x: " << m2x << "\n\n";

  std::cout << "m1x_r: " << m1x_r.ToString() << "\n";
  std::cout << "m2x_r: " << m2x_r.ToString() << "\n\n";

  Header("Ciphertexts");

  NTL::Vec<NTL::ZZX> c1 = pie::MFVEncrypt(mfv.params, mfv.pk, m1);
  NTL::Vec<NTL::ZZX> c2 = pie::MFVEncrypt(mfv.params, mfv.pk, m2);

  std::cout << "c1: " << c1 << "\n";
  std::cout << "c2: " << c2 << "\n\n";

  NTL::Vec<NTL::ZZX> c1_plus_c2 = pie::MFVAdd(mfv.params, c1, c2);

  Header("Decrypted ciphertexts");

  pie::Rational m1_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1);
  pie::Rational m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c2);

  pie::Rational m1_plus_m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1_plus_c2);

  std::cout << "m1_r: " << m1_r.ToString() << "\n";
  std::cout << "m2_r: " << m2_r.ToString() << "\n\n";

  std::cout << "m1_plus_m2_r: " << m1_plus_m2_r.ToString() << "\n";
  
  return 0;
}