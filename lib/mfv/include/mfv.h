#pragma once

#include "lib/include/hensel_code.h"
#include <NTL/ZZ.h>
#include <NTL/ZZXFactoring.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <NTL/vector.h>
#include <vector>

namespace pie
{
  struct MFVParams {
    NTL::ZZ q;
    float sigma;
    long b;
    long n;
    long w;
    long l;

    MFVParams();

    MFVParams(const NTL::ZZ q, const float sigma, const long b, 
              const long n, const long w, const long l);
  };

  struct MFVPublicKey {
    NTL::ZZX a;
    NTL::ZZX e;
    NTL::ZZX p0;

    MFVPublicKey();
  };

  struct MFVPrivateKey {
    NTL::ZZX s;

    MFVPrivateKey();
  };

  class MFV {
  public:
    MFVParams params;
    MFVPublicKey pk;
    MFVPrivateKey sk;
    NTL::ZZ e0;
    NTL::ZZ e1;
    NTL::ZZX f;

    MFV();

    MFV(MFVPublicKey pk, MFVPrivateKey sk);

    void KeyGen(MFVParams params);

    void GenerateS();

    void GeneratePK();
  };

  NTL::ZZX ComputeDeltaB(MFVParams &params);

  NTL::Vec<NTL::ZZX> MFVEncrypt(MFVParams &params, MFVPublicKey &pk, pie::Rational &m);

  Rational MFVDecrypt(MFVParams &params, 
                      const MFVPrivateKey &sk, const NTL::Vec<NTL::ZZX> &c);

  NTL::Vec<NTL::ZZX> MFVAdd(MFVParams &params, const NTL::Vec<NTL::ZZX> &c1, 
                         const NTL::Vec<NTL::ZZX> &c2);

  NTL::Vec<NTL::ZZX> MFVMul(MFVParams &params, const NTL::Vec<NTL::ZZX> &c1, 
                         const NTL::Vec<NTL::ZZX> &c2);
}