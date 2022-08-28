#pragma once

#include "lib/include/hensel_code.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <vector>

namespace pie
{
  struct IDGHVParams {
    long lambda;
    long l;
    long rho;
    long eta;
    long gamma;
    long tau;

    IDGHVParams();

    IDGHVParams(const long lambda, const long rho, const long eta, 
                const long gamma, const long l, const long tau);
  };

  struct IDGHVPublicKey {
    NTL::ZZ x0;
    NTL::Vec<NTL::ZZ> big_qs;
    NTL::ZZ g;
    NTL::Vec<NTL::ZZ> xs;
    NTL::Vec<NTL::ZZ> xs_;
    long tau;

    IDGHVPublicKey();

    // IDGHVPublicKey(const NTL::ZZ x0, const NTL::Vec<NTL::ZZ> big_qs, 
    //             const NTL::Vec<NTL::ZZ> xs, NTL::Vec<NTL::ZZ> xs_,
    //             const long tau);
  };

  struct IDGHVPrivateKey {
    NTL::Vec<NTL::ZZ> ps;

    IDGHVPrivateKey();

    // IDGHVPrivateKey(const NTL::Vec<NTL::ZZ> ps);
  };

  class IDGHV {
  public:
    IDGHVParams params;
    IDGHVPublicKey pk;
    IDGHVPrivateKey sk;
    // NTL::Vec<NTL::ZZ> ps;
    NTL::ZZ pi;
    NTL::ZZ q0;
    // NTL::ZZ x0;
    // NTL::Vec<NTL::ZZ> xs;
    // NTL::Vec<NTL::ZZ> xs_;
    // NTL::Vec<NTL::ZZ> big_qs;

    IDGHV();

    IDGHV(IDGHVPublicKey pk, IDGHVPrivateKey sk);

    // IDGHV(IDGHVPublicKey pk, IDGHVPrivateKey sk);

    void KeyGen(IDGHVParams params);

    void GeneratePsAndPi();

    void GenerateRoughInteger(const long bit_length);

    // void GenerateXs(NTL::Vec<NTL::ZZ> &xs, NTL::Vec<NTL::ZZ> &xs_, 
    //                      NTL::Vec<NTL::ZZ> &big_qs, NTL::ZZ &q0, NTL::ZZ &pi,
    //                      long &rho, NTL::ZZ &g);
    void GenerateXs();

    // pie::Rational Decrypt();
  };

  HenselCode IDGHVEncrypt(IDGHVParams &params, IDGHVPublicKey &pk, pie::Rational &m);

  Rational IDGHVDecrypt(IDGHVParams &params, const IDGHVPublicKey &pk, 
                        const IDGHVPrivateKey &sk, const HenselCode &c);
}