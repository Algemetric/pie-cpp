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
  };

  struct IDGHVPrivateKey {
    NTL::Vec<NTL::ZZ> ps;

    IDGHVPrivateKey();
  };

  class IDGHV {
  public:
    IDGHVParams params;
    IDGHVPublicKey pk;
    IDGHVPrivateKey sk;
    NTL::ZZ pi;
    NTL::ZZ q0;

    IDGHV();

    IDGHV(IDGHVPublicKey pk, IDGHVPrivateKey sk);

    void KeyGen(IDGHVParams params);

    void GeneratePsAndPi();

    void GenerateRoughInteger(const long bit_length);

    void GenerateXs();
  };

  HenselCode IDGHVEncrypt(IDGHVParams &params, IDGHVPublicKey &pk, pie::Rational &m);

  Rational IDGHVDecrypt(IDGHVParams &params, const IDGHVPublicKey &pk, 
                        const IDGHVPrivateKey &sk, const HenselCode &c);
}