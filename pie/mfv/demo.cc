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

  NTL::ZZX ax;
  NTL::ZZX bx;
  NTL::ZZX fx;
  NTL::ZZX cx;
  // long n = 3;

  ax.SetLength(3);
  bx.SetLength(3);
  fx.SetLength(4);

  ax[0] = NTL::ZZ(2);
  ax[1] = NTL::ZZ(3);
  ax[2] = NTL::ZZ(5);

  bx[0] = NTL::ZZ(7);
  bx[1] = NTL::ZZ(8);
  bx[2] = NTL::ZZ(11);

  fx[0] = NTL::ZZ(1);
  fx[3] = NTL::ZZ(1);

  cx = pie::MulMod(ax, bx, fx);

  std::cout << "ax: " << ax << "\n";
  std::cout << "bx: " << bx << "\n";
  std::cout << "cx: " << cx << "\n\n";

  NTL::ZZ q = NTL::ZZ(5);
  NTL::ZZ_p::init(q);
  NTL::ZZ_pX cx_mod;

  NTL::conv(cx_mod, cx);

  std::cout << "cx_mod: " << cx_mod << "\n\n";

  NTL::ZZX dx = pie::MulModP(ax, bx, fx, q);

  std::cout << "dx: " << dx << "\n\n";

  NTL::ZZX ex = ax + bx;
  NTL::ZZX ex_mod = pie::ModP(ex, q);

  std::cout << "ex: " << ex << "\n";
  std::cout << "ex_mod: " << ex_mod << "\n\n";

  NTL::ZZX ex_reverse = NTL::reverse(ex);

  std::cout << "ex_reverse: " << ex_reverse << "\n\n";

  pie::Rational m = pie::Rational(2, 3);
  long b = 30;
  long n = 4;

  NTL::ZZX hx = pie::PolyEncode(b, n, m);
  pie::Rational m_r = pie::PolyDecode(b, n, hx);

  std::cout << "m: " << m.ToString() << "\n";
  std::cout << "hx: " << hx << "\n";
  std::cout << "m_r: " << m_r.ToString() << "\n\n";

  pie::MFVPublicKey pk;
  pie::MFVPrivateKey sk;
  pie::MFVParams params = pie::MFVParams(NTL::ZZ(1679617), 3.19, 30, 4, 4, 10);
  pie::MFV mfv = pie::MFV(pk, sk);
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

  // overriding parameters for debugging
  // mfv.pk.p0[0] = NTL::ZZ(588891);
  // mfv.pk.p0[1] = NTL::ZZ(-647834);
  // mfv.pk.p0[2] = NTL::ZZ(-272039);
  // mfv.pk.p0[3] = NTL::ZZ(252907);

  // mfv.pk.a[0] = NTL::ZZ(-606804);
  // mfv.pk.a[1] = NTL::ZZ(-41028);
  // mfv.pk.a[2] = NTL::ZZ(-231016);
  // mfv.pk.a[3] = NTL::ZZ(483920);

  // mfv.sk.s[0] = NTL::ZZ(1);
  // mfv.sk.s[1] = NTL::ZZ(0);
  // mfv.sk.s[2] = NTL::ZZ(0);
  // mfv.sk.s[3] = NTL::ZZ(-1);

  // std::cout << "pk.p0: " << mfv.pk.p0 << "\n";
  // std::cout << "pk.a: " << mfv.pk.a << "\n";
  // std::cout << "sk.s: " << mfv.sk.s << "\n\n";

  //-- 

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
  NTL::Vec<NTL::ZZX> c1_times_c2 = pie::MFVMul(mfv.params, c1, c2);

  Header("Decrypted ciphertexts");

  pie::Rational m1_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1);
  pie::Rational m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c2);

  pie::Rational m1_plus_m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1_plus_c2);
  pie::Rational m1_times_m2_r = pie::MFVDecrypt(mfv.params, mfv.sk, c1_times_c2);

  std::cout << "m1_r: " << m1_r.ToString() << "\n";
  std::cout << "m2_r: " << m2_r.ToString() << "\n\n";

  std::cout << "m1_plus_m2_r: " << m1_plus_m2_r.ToString() << "\n";
  std::cout << "m1_times_m2_r: " << m1_times_m2_r.ToString() << "\n\n";
  
  // Header("Key Generation");

  // Section("Parameters");

  // pie::MFVPublicKey pk;
  // pie::MFVPrivateKey sk;
  
  // pie::MFVParams params = pie::MFVParams(52, 37, 41, 1558, 900000, 661);
  // // pie::MFVParams params = pie::MFVParams(50, 6, 60, 4248, 530000000, 661);

  // std::cout << "params.lambda: " << params.lambda << "\n";
  // std::cout << "params.l: " << params.l << "\n";
  // std::cout << "params.rho: " << params.rho << "\n";
  // std::cout << "params.eta: " << params.eta << "\n";
  // std::cout << "params.gamma: " << params.gamma << "\n";
  // std::cout << "params.tau: " << params.tau << "\n\n";

  // pie::MFV mfv = pie::MFV(pk, sk);

  // auto start = std::chrono::high_resolution_clock::now();
  // mfv.KeyGen(params);
  // auto stop = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double, std::milli> duration = stop - start;
  // double milliseconds = duration.count();
  // Section("Key Generation Performance");
  // PrintDuration(milliseconds);

  // Header("Encryption");

  // pie::Rational m1 = pie::Rational(2, 3);
  // pie::Rational m2 = pie::Rational(4, 5);
  // pie::HenselCode c1 = pie::MFVEncrypt(params, mfv.pk, m1);
  // pie::HenselCode c2 = pie::MFVEncrypt(params, mfv.pk, m2);
  // pie::Rational m1_r = pie::MFVDecrypt(params, mfv.pk, mfv.sk, c1);
  // pie::Rational m2_r = pie::MFVDecrypt(params, mfv.pk, mfv.sk, c2);

  // Section("Messages");

  // std::cout << "m1: " << m1.ToString() << "\n";
  // std::cout << "m2: " << m2.ToString() << "\n";

  // Section("Ciphertexts");

  // std::cout << "c1: " << c1.code << "\n";
  // std::cout << "c2: " << c2.code << "\n";

  // Section("Decrypted Messages");

  // std::cout << "m1_r: " << m1_r.ToString() << "\n";
  // std::cout << "m2_r: " << m2_r.ToString() << "\n";

  // Section("Homomorphic Addition and Multiplication");

  // pie::HenselCode c1_plus_c2 = c1 + c2;
  // pie::HenselCode c1_times_c2 = c1 * c2;

  // std::cout << "c1_plus_c2: " << c1_plus_c2.code << "\n";
  // std::cout << "c1_times_c2: " << c1_times_c2.code << "\n";

  // Section("Decrypted Homomorphic Results");

  // pie::Rational m1_plus_m2_r = pie::MFVDecrypt(params, mfv.pk, mfv.sk, c1_plus_c2);
  // pie::Rational m1_times_m2_r = pie::MFVDecrypt(params, mfv.pk, mfv.sk, c1_times_c2);

  // std::cout << "m1_plus_m2_r: " << m1_plus_m2_r.ToString() << "\n";
  // std::cout << "m1_times_m2_r: " << m1_times_m2_r.ToString() << "\n";

  return 0;
}