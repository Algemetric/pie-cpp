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

  MFVEvaluationKey::MFVEvaluationKey() {};

  MFV::MFV() {};

  MFV::MFV(MFVPublicKey pk, MFVPrivateKey sk, MFVEvaluationKey evk) {
    this->pk = pk;
    this->sk = sk;
    this->evk = evk;
  }

  void MFV::KeyGen(MFVParams params) {
    this->params = params;
    this->f = pie::ComputeModulusPolynomial(this->params.n);

    NTL::ZZ_p::init(this->params.q);

    MFV::GenerateS();
    MFV::GeneratePK();
    MFV::GenerateEVK();
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

    this->pk.p0 = pie::ModP(-as_e, this->params.q);
  }

  void MFV::GenerateEVK() {
    this->evk.evk1.SetLength(this->params.l + 1);
    this->evk.evk2.SetLength(this->params.l + 1);

    NTL::ZZX a;
    NTL::ZZX e;
    NTL::ZZX s2;
    NTL::ZZX wx;
    NTL::ZZX wis2;
    wx.SetLength(1);

    NTL::ZZX as_e;

    NTL::ZZ from = -NTL::ZZ((this->params.q - 1)/2);
    NTL::ZZ to = NTL::ZZ((this->params.q - 1)/2);
    
    for (long i = 0; i < this->params.l + 1; i++) {
      a = pie::RandomPolynomial(from, to, this->params.n);
      e = pie::NormalPolynomial(this->params.n, 0, this->params.sigma);

      s2 = pie::MulMod(this->sk.s, this->sk.s, this->f);
      
      wx[0] = NTL::ZZ(NTL::power_ZZ(params.w, i));

      wis2 = pie::MulMod(wx, s2, this->f);

      this->evk.evk1[i] = pie::ModP(-pie::MulMod(a, this->sk.s, this->f) + e + wis2, this->params.q);
      this->evk.evk2[i] = a;
    }

    this->evk.evk1 = pie::Reverse(this->evk.evk1);
    this->evk.evk2 = pie::Reverse(this->evk.evk2);
  }

  NTL::Vec<NTL::ZZX> MFVEncrypt(MFVParams &params, MFVPublicKey &pk, pie::Rational &m) {
    NTL::ZZX f = ComputeModulusPolynomial(params.n);

    NTL::ZZX deltab = ComputeDeltaB(params);
    NTL::ZZX u = pie::RandomPolynomial(-1, 1, params.n);
    
    NTL::ZZX e0 = pie::NormalPolynomial(params.n, 0, params.sigma);

    NTL::ZZX e1 = pie::NormalPolynomial(params.n, 0, params.sigma);

    NTL::ZZX mx = pie::PolyEncode(params.b, params.n, m);
    NTL::ZZX c01 = pie::MulMod(deltab, mx, f);
    NTL::ZZX c02 = pie::MulMod(pk.p0, u, f);
    NTL::ZZX c1notq = c01 + c02 + e0;
    NTL::ZZX c1 = pie::ModP(c01 + c02 + e0, params.q);

    NTL::ZZX c11 = pie::MulMod(pk.a, u, f);
    NTL::ZZX c2 = pie::ModP(c11 + e1, params.q);

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
    }

    return deltab;
  }

  NTL::ZZX AuxPolynomial(MFVParams &params, const NTL::ZZX &a, NTL::ZZX &f) {
    NTL::ZZX ax;
    ax.SetLength(2);
    ax[0] = NTL::ZZ(1);
    ax[1] = NTL::ZZ(-params.b);

    NTL::ZZX b = pie::MulMod(ax, a, f); 
    NTL::ZZX c;
    c.SetLength(params.n);

    NTL::RR qr;
    NTL::conv(qr, params.q);
    
    for (long i = 0; i < params.n; i++) {
      NTL::RR bi;
      conv(bi, b[i]);
      NTL::conv(c[i], NTL::round(bi / qr));
    }

    return c;
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

  NTL::Vec<NTL::ZZX> MFVMulPrime(MFVParams &params, const NTL::Vec<NTL::ZZX> &c1, 
                         const NTL::Vec<NTL::ZZX> &c2) {
    
    NTL::ZZX f = ComputeModulusPolynomial(params.n);
    NTL::Vec<NTL::ZZX> cs;
    cs.SetLength(3);

    NTL::ZZX c_11 = pie::MulMod(c1[0], c2[0], f);
    cs[0] = pie::AuxPolynomial(params, c_11, f);

    NTL::ZZX c_21 = pie::MulMod(c1[0], c2[1], f) + pie::MulMod(c1[1], c2[0], f);
    cs[1] = pie::AuxPolynomial(params, c_21, f);

    NTL::ZZX c_31 = pie::MulMod(c1[1], c2[1], f);
    cs[2] = pie::AuxPolynomial(params, c_31, f);

    return cs;
  }

  NTL::Vec<NTL::ZZX> MFVRelinearize(MFVParams &params, MFVEvaluationKey &evk, 
                         const NTL::Vec<NTL::ZZX> &cprime) {
    NTL::Vec<NTL::ZZX> c; 
    c.SetLength(2);                         
    NTL::ZZX f = ComputeModulusPolynomial(params.n);                      
    NTL::ZZX c2prime = cprime[2];
    NTL::ZZ m;
    NTL::ZZX y;
    y.SetLength(params.l + 1);
    NTL::Vec<NTL::ZZX> c2primes;
    long size = cprime[2].rep.length();
    c2primes.SetLength(size);
    NTL::ZZ wz = NTL::ZZ(params.w);

    for (long i = 0; i < size; i++) {
      m = c2prime[i];
      y[0] = pie::SymMod(m, wz);
      
      for (long j = 1; j <= params.l; j++) {
        m = (m - y[j - 1] * NTL::power(wz, j - 1));
        y[j] = pie::SymMod(m /NTL::power(wz, j), wz);
      }
      y = NTL::reverse(y);
      c2primes[i] = y;
    }

    NTL::Vec<NTL::ZZX> cprimes = pie::RotatedPolynomial(c2primes, params.l + 1, size);

    NTL::ZZX c11 = cprime[0];
    for (long i = 0; i <= params.l; i++) {

      c11 = c11 + pie::MulMod(evk.evk1[i], cprimes[i], f);
    }
    c[0] = c11;

    NTL::ZZX c22 = cprime[1];
    for (long i = 0; i <= params.l; i++) {
      c22 = c22 + pie::MulMod(evk.evk2[i], cprimes[i], f);
    }
    
    c[1] = c22;

    return c;
  }

  NTL::Vec<NTL::ZZX> MFVMul(MFVParams &params, MFVEvaluationKey &evk,
                            const NTL::Vec<NTL::ZZX> &c1, const NTL::Vec<NTL::ZZX> &c2) {

    NTL::Vec<NTL::ZZX> cprime = MFVMulPrime(params, c1, c2);
    NTL::Vec<NTL::ZZX> c = MFVRelinearize(params, evk, cprime);

    return c;
  }

  NTL::Vec<NTL::ZZX> RotatedPolynomial(const NTL::Vec<NTL::ZZX> &cs, 
                             const long &columns, const long &rows) {
    NTL::Vec<NTL::ZZX> as;
    as.SetLength(columns);

    for (long i = 0; i < columns; i++) {
      as[i].SetLength(rows);
      for (long j = 0; j < rows; j++) {
        as[i][j] = cs[j][i];
      }
    }

    return as;
  }
}