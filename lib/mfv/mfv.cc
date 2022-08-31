// #include "lib/include/hensel_code.h"
#include "lib/mfv/include/mfv.h"
#include "lib/include/tools.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <NTL/ZZXFactoring.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <NTL/RR.h>
#include <math.h>
#include <vector>
#include <random>
#include <NTL/matrix.h>

namespace pie
{
  MFVParams::MFVParams() {};

  MFVParams::MFVParams(const NTL::ZZ q, const float sigma, const long b, 
                       const long n, const long w, const long l) {
    this->q = q;
    this->sigma = sigma;
    this->b = b;
    this->n = n;
    this->w = w;
    this->l = l;
                  
  }

  MFVPublicKey::MFVPublicKey() {};

  MFVPrivateKey::MFVPrivateKey() {};

  MFV::MFV() {};

  MFV::MFV(MFVPublicKey pk, MFVPrivateKey sk) {
    this->pk = pk;
    this->sk = sk;
  }

  void MFV::KeyGen(MFVParams params) {
    this->params = params;
    this->f = pie::ComputeModulusPolynomial(this->params.n);

    NTL::ZZ_p::init(this->params.q);

    MFV::GenerateS();
    MFV::GeneratePK();
  }

  void MFV::GenerateS() {
    this->sk.s = pie::RandomPolynomial(-1, 1, this->params.n);
  }

  void MFV::GeneratePK() {
    this->pk.a.SetLength(this->params.n);
    this->pk.e.SetLength(this->params.n);

    NTL::ZZ from = -NTL::ZZ((this->params.q - 1)/2);
    NTL::ZZ to = NTL::ZZ((this->params.q - 1)/2);

    this->pk.a = pie::RandomPolynomial(from, to, this->params.n);
    this->pk.e = pie::NormalPolynomial(this->params.n, 0, this->params.sigma);

    NTL::ZZX as = pie::MulMod(this->pk.a, this->sk.s, this->f);
    NTL::ZZX as_e = (as + this->pk.e);

    // std::cout << "a: " << this->pk.a << "\n";
    // std::cout << "e: " << this->pk.e << "\n";
    // std::cout << "as: " << as << "\n";
    // std::cout << "as_e: " << as_e << "\n";
    // std::cout << "-as_e: " << -as_e << "\n";


    // std::cout << "as_e: " << as_e << "\n\n";
    // std::cout << "-as_e: " << -as_e << "\n\n";

    this->pk.p0 = pie::ModP(-as_e, this->params.q);
  }

  NTL::Vec<NTL::ZZX> MFVEncrypt(MFVParams &params, MFVPublicKey &pk, pie::Rational &m) {
    NTL::ZZX f = ComputeModulusPolynomial(params.n);
    // std::cout << "f: " << f << "\n";

    NTL::ZZX deltab = ComputeDeltaB(params);
    // std::cout << "deltab: " << deltab << "\n";
    NTL::ZZX u = pie::RandomPolynomial(-1, 1, params.n);

    // // overriding u for debuging
    // u[0] = NTL::ZZ(1);
    // u[1] = NTL::ZZ(1);
    // u[2] = NTL::ZZ(0);
    // u[3] = NTL::ZZ(1);
    // //--
    
    // std::cout << "u: " << u << "\n";
    NTL::ZZX e0 = pie::NormalPolynomial(params.n, 0, params.sigma);

    // // overriding e0 for debuging
    // e0[0] = NTL::ZZ(-1);
    // e0[1] = NTL::ZZ(-6);
    // e0[2] = NTL::ZZ(-7);
    // e0[3] = NTL::ZZ(-5);
    // //--

    // std::cout << "e0: " << e0 << "\n";
    NTL::ZZX e1 = pie::NormalPolynomial(params.n, 0, params.sigma);

    // // overriding e1 for debuging
    // e1[0] = NTL::ZZ(-1);
    // e1[1] = NTL::ZZ(-2);
    // e1[2] = NTL::ZZ(-2);
    // e1[3] = NTL::ZZ(0);
    // //--

    // std::cout << "e1: " << e1 << "\n";

    NTL::ZZX mx = pie::PolyEncode(params.b, params.n, m);
    NTL::ZZX c01 = pie::MulMod(deltab, mx, f);
    NTL::ZZX c02 = pie::MulMod(pk.p0, u, f);
    NTL::ZZX c1notq = c01 + c02 + e0;
    NTL::ZZX c1 = pie::ModP(c01 + c02 + e0, params.q);

    // std::cout << "mx: " << mx << "\n";
    // std::cout << "c01: " << c01 << "\n";
    // std::cout << "c02: " << c02 << "\n";
    // std::cout << "c1: " << c1 << "\n\n";
    // std::cout << "c1notq: " << c1notq << "\n\n";

    NTL::ZZX c11 = pie::MulMod(pk.a, u, f);
    NTL::ZZX c2 = pie::ModP(c11 + e1, params.q);

    // std::cout << "c11: " << c11 << "\n";
    // std::cout << "c2: " << c2 << "\n\n";

    NTL::Vec<NTL::ZZX> c;
    c.SetLength(2);
    c[0] = c1;
    c[1] = c2;

    return c;
  }

  Rational MFVDecrypt(MFVParams &params, 
                      const MFVPrivateKey &sk, const NTL::Vec<NTL::ZZX> &c) {
    NTL::ZZX f = ComputeModulusPolynomial(params.n);
    NTL::ZZX cr1 = pie::MulMod(c[1], sk.s, f); 
    NTL::ZZX cr2 = pie::ModP(c[0] + cr1, params.q); 

    // std::cout << "c[1]: " << c[1] << "\n";
    // std::cout << "sk.s: " << sk.s << "\n";
    // std::cout << "cr1: " << cr1 << "\n";
    // std::cout << "cr2: " << cr2 << "\n";

    NTL::ZZX cr3;
    cr3.SetLength(2);
    cr3[0] = NTL::ZZ(1);
    cr3[1] = NTL::ZZ(-params.b);

    NTL::ZZX cr4 = pie::MulMod(cr3, cr2, f); 
    NTL::ZZX mx;
    mx.SetLength(params.n);

    NTL::RR qr;
    NTL::conv(qr, params.q);
    
    for (long i = 0; i < params.n; i++) {
      NTL::RR cr4i;
      conv(cr4i, cr4[i]);
      NTL::conv(mx[i], NTL::round(cr4i / qr));
    }

    NTL::ZZX cr5 = pie::ModP(mx, NTL::ZZ(params.b));

    // std::cout << "(decryption) mx: " << mx << "\n";
    // std::cout << "(decryption) cr5: " << cr5 << "\n\n";
    
    pie::Rational m = pie::PolyDecode(params.b, params.n, cr5);

    return m;
  }

  NTL::ZZX ComputeDeltaB(MFVParams &params) {
    NTL::ZZX deltab;
    deltab.SetLength(params.n);

    NTL::Vec<NTL::RR> delta;
    delta.SetLength(params.n);

    NTL::RR br = NTL::RR(params.b);
    NTL::RR qr;
    NTL::conv(qr, params.q);

    delta[0] = -qr/(power(br, params.n) + 1);

    for (long i = 1; i < params.n; i++) {
      delta[i] = delta[0] * power(br, i);
    }

    for (long i = 0; i < params.n; i++) {
      NTL::conv(deltab[i], NTL::round(delta[i]));
      // std::cout << "deltab[i]: " << deltab[i] << "\n";
    }

    return deltab;
  }

  NTL::Vec<NTL::ZZX> MFVAdd(MFVParams &params, const NTL::Vec<NTL::ZZX> &c1, 
                         const NTL::Vec<NTL::ZZX> &c2) {
    NTL::Vec<NTL::ZZX> c;
    c.SetLength(2);
    NTL::ZZX f = ComputeModulusPolynomial(params.n);

    c[0] = pie::AddModP(c1[0], c2[0], f, params.q);
    c[1] = pie::AddModP(c1[1], c2[1], f, params.q);

    return c;
  }

  NTL::Vec<NTL::ZZX> MFVMul(MFVParams &params, const NTL::Vec<NTL::ZZX> &c1, 
                         const NTL::Vec<NTL::ZZX> &c2) {
    NTL::Vec<NTL::ZZX> c;
    c.SetLength(2);
    NTL::ZZX f = ComputeModulusPolynomial(params.n);

    c[0] = pie::MulModP(c1[0], c2[0], f, params.q);
    c[1] = pie::MulModP(c1[1], c2[1], f, params.q);

    return c;
  }

  // Rational MFVDecrypt(MFVParams &params, const MFVPublicKey &pk, 
  //                       const MFVPrivateKey &sk, const HenselCode &c) {
  //   NTL::Vec<NTL::ZZ> hs;
  //   hs.SetLength(params.l);

  //   for (long i = 0; i < params.l; i++) {
  //     hs[i] = (c.code % sk.ps[i]) % pk.big_qs[i];
  //   }

  //   NTL::ZZ h = pie::CRT(pk.big_qs, hs);
  //   pie::HenselCode h_ = pie::HenselCode(pk.g, 1, h);
  //   pie::Rational m = pie::Decode(pk.g, 1, h_);

  //   return m;
  // }

}