// #include "lib/include/hensel_code.h"
#include "lib/idghv/include/idghv.h"
#include "lib/include/tools.h"
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <math.h>
#include <vector>
#include <random>
#include <NTL/matrix.h>

namespace pie
{
  IDGHVParams::IDGHVParams() {};

  IDGHVParams::IDGHVParams(const long lambda, const long l, const long rho, const long eta, 
                const long gamma, const long tau) {
    this->lambda = lambda;
    this->l = l;
    this->rho = rho;
    this->eta = eta;
    this->gamma = gamma;
    this->tau = tau;
                  
  }

  IDGHVPublicKey::IDGHVPublicKey() {};

  IDGHVPrivateKey::IDGHVPrivateKey() {};

  IDGHV::IDGHV() {};

  IDGHV::IDGHV(IDGHVPublicKey pk, IDGHVPrivateKey sk) {
    this->pk = pk;
    this->sk = sk;
  }

  void IDGHV::KeyGen(IDGHVParams params) {
    this->params = params;
    this->pk.tau = params.tau;

    IDGHV::GeneratePsAndPi();
    IDGHV::GenerateRoughInteger(this->params.gamma - this->params.eta * this->params.l);
    NTL::mul(this->pk.x0, this->q0, this->pi);
    
    IDGHV::GenerateXs();
  }

  void IDGHV::GeneratePsAndPi() {
    this->sk.ps.SetLength(this->params.l);
    pi = 1;

    for (long i = 0; i < this->params.l; i++) {
      NTL::GenPrime(this->sk.ps[i], this->params.eta, 80);
      NTL::mul(this->pi, this->pi, this->sk.ps[i]);
    }
  }

  void IDGHV::GenerateRoughInteger(const long bit_length) {
    NTL::ZZ prime_factor;
    NTL::ZZ bound = NTL::power_ZZ(this->params.lambda, 2);

    while (true) {
      NTL::RandomLen(this->q0, bit_length);
      prime_factor = 2;
      bool rough = true;
      
      while (prime_factor < bound) {
        if (NTL::GCD(prime_factor, this->q0) != 1) {
          rough = false;
        }

        NTL::add(prime_factor, prime_factor, 1);
        NTL::NextPrime(prime_factor, prime_factor);
      }

      if (rough) {
        break;
      }
    }
  }

  void IDGHV::GenerateXs() {
    this->pk.xs.SetLength(this->params.tau);
    this->pk.xs_.SetLength(this->params.l);
    this->pk.big_qs.SetLength(this->params.l);
    this->pk.g = NTL::ZZ(1);
    NTL::Vec<NTL::ZZ> r;
    NTL::Vec<NTL::ZZ> r_;
    NTL::Vec<NTL::ZZ> xp;
    r.SetLength(this->params.l);
    r_.SetLength(this->params.l);
    xp.SetLength(this->params.l);

    for (long i = 0; i < this->params.l; i++) {
      NTL::GenPrime(this->pk.big_qs[i], this->params.rho, 80);
      this->pk.g = this->pk.g * this->pk.big_qs[i];
    }

    for (long i = 0; i < this->params.tau; i++) {
      for (long j = 0; j < this->params.l; j++) {
        NTL::RandomBnd(r[j], NTL::power_ZZ(2,this->params.rho));
        xp[j] = this->pk.big_qs[j] * r[j];
      }
      this->pk.xs[i] = CRT(this->sk.ps, xp);
    }

    for (long i = 0; i < this->params.l; i++) {
      for (long j = 0; j < this->params.l; j++) {
        NTL::RandomBnd(r_[j], NTL::power_ZZ(2,this->params.rho));
         xp[j] = this->pk.big_qs[j] * r_[j] + pie::Delta(i, j);
      }
      this->pk.xs_[i] = CRT(this->sk.ps, xp);
    }

  }

  HenselCode IDGHVEncrypt(IDGHVParams &params, IDGHVPublicKey &pk, pie::Rational &m) {
    pie::HenselCode h = pie::Encode(pk.g, 1, m);
    NTL::Vec<NTL::ZZ> ms;
    ms.SetLength(params.l);

    for (long i = 0; i < params.l; i++) {
      ms[i] = (h.code % pk.big_qs[i]);
    }

    NTL::ZZ first_term = NTL::ZZ(0);
    NTL::ZZ second_term = NTL::ZZ(0);
    NTL::ZZ random_bit;

    for (long i = 0; i < params.l; i++) {
      first_term = first_term + (ms[i] * pk.xs_[i]);
    }

    for (long i = 0; i < params.tau; i++) {
      random_bit = NTL::ZZ(rand() % 2);
      second_term = second_term + (random_bit * pk.xs[i]);
    }

    pie::HenselCode c = pie::HenselCode(pk.x0, 1, (first_term + second_term) % pk.x0);

    return c;
  }

  Rational IDGHVDecrypt(IDGHVParams &params, const IDGHVPublicKey &pk, 
                        const IDGHVPrivateKey &sk, const HenselCode &c) {
    NTL::Vec<NTL::ZZ> hs;
    hs.SetLength(params.l);

    for (long i = 0; i < params.l; i++) {
      hs[i] = (c.code % sk.ps[i]) % pk.big_qs[i];
    }

    NTL::ZZ h = pie::CRT(pk.big_qs, hs);
    pie::HenselCode h_ = pie::HenselCode(pk.g, 1, h);
    pie::Rational m = pie::Decode(pk.g, 1, h_);

    return m;
  }

}