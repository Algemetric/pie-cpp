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

  // IDGHVPublicKey::IDGHVPublicKey(const NTL::ZZ x0, const NTL::Vec<NTL::ZZ> big_qs, 
  //               const NTL::Vec<NTL::ZZ> xs, NTL::Vec<NTL::ZZ> xs_,
  //               const long tau) {
  //   this->x0 = x0;
  //   this->big_qs = big_qs;
  //   this->xs = xs;
  //   this->xs_ = xs_;
  //   this->tau = tau;
  // }

  // IDGHVPrivateKey::IDGHVPrivateKey(const NTL::Vec<NTL::ZZ> ps) {
  //   this->ps = ps;
  // }

  IDGHV::IDGHV() {};

  IDGHV::IDGHV(IDGHVPublicKey pk, IDGHVPrivateKey sk) {
    this->pk = pk;
    this->sk = sk;
  }

  void IDGHV::KeyGen(IDGHVParams params) {
    // std::cout << "Entered KeyGen\n";
    this->params = params;
    // std::cout << "params.tau: " << params.tau << "\n";
    this->pk.tau = params.tau;

    IDGHV::GeneratePsAndPi();
    // std::cout << "GeneratePsAndPi done\n";
    IDGHV::GenerateRoughInteger(this->params.gamma - this->params.eta * this->params.l);
    NTL::mul(this->pk.x0, this->q0, this->pi);
    // std::cout << "GeneraGenerateRoughInteger done\n";
    
    // IDGHV::GenerateXs(this->pk.xs, this->pk.xs_, this->pk.big_qs, this->q0, this->pi,
    //                   this->params.rho, this->pk.g);
    IDGHV::GenerateXs();
    // std::cout << "GenerateXs done\n";
  }

  void IDGHV::GeneratePsAndPi() {
    // std::cout << "Entered GeneratePsAndPi done\n";
    // std::cout << "this->params.l: " << this->params.l << "\n";
    this->sk.ps.SetLength(this->params.l);
    pi = 1;

    for (long i = 0; i < this->params.l; i++) {
      NTL::GenPrime(this->sk.ps[i], this->params.eta, 80);
      NTL::mul(this->pi, this->pi, this->sk.ps[i]);
    }
  }

  void IDGHV::GenerateRoughInteger(const long bit_length) {
    // std::cout << "Entered GenerateRoughInteger done\n";
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

    // generate Qi's and compute g (the product of Qi's)
    for (long i = 0; i < this->params.l; i++) {
      NTL::GenPrime(this->pk.big_qs[i], this->params.rho, 80);
      this->pk.g = this->pk.g * this->pk.big_qs[i];
      // std::cout << "this->pk.big_qs[i]: " << this->pk.big_qs[i] << "\n";
    }

    // std::cout << "Generating Qi's done\n";

    // compute the xi's
    for (long i = 0; i < this->params.tau; i++) {
      for (long j = 0; j < this->params.l; j++) {
        NTL::RandomBnd(r[j], NTL::power_ZZ(2,this->params.rho));
        xp[j] = this->pk.big_qs[j] * r[j];
      }
      this->pk.xs[i] = CRT(this->sk.ps, xp);
    }

    // std::cout << "Generating xi's done\n";

    // compute the x_i's
    for (long i = 0; i < this->params.l; i++) {
      for (long j = 0; j < this->params.l; j++) {
        NTL::RandomBnd(r_[j], NTL::power_ZZ(2,this->params.rho));
         xp[j] = this->pk.big_qs[j] * r_[j] + pie::Delta(i, j);
      }
      this->pk.xs_[i] = CRT(this->sk.ps, xp);
    }

    // std::cout << "Generating x_i's done\n";
  }

  HenselCode IDGHVEncrypt(IDGHVParams &params, IDGHVPublicKey &pk, pie::Rational &m) {
    pie::HenselCode h = pie::Encode(pk.g, 1, m);
    // std::cout << "(encryption) h: " << h.code << "\n";
    NTL::Vec<NTL::ZZ> ms;
    ms.SetLength(params.l);

    // std::cout << "After initial vars\n";

    for (long i = 0; i < params.l; i++) {
      ms[i] = (h.code % pk.big_qs[i]);
      // std::cout << "ms[" << i << "]: " << ms[i] << "\n";
    }

    // std::cout << "After first loop\n";

    NTL::ZZ first_term = NTL::ZZ(0);
    NTL::ZZ second_term = NTL::ZZ(0);
    NTL::ZZ random_bit;

    for (long i = 0; i < params.l; i++) {
      first_term = first_term + (ms[i] * pk.xs_[i]);
    }

    // std::cout << "After second loop\n";

    for (long i = 0; i < params.tau; i++) {
      random_bit = NTL::ZZ(rand() % 2);
      // std::cout << "random_bit: " << random_bit << "\n";
      second_term = second_term + (random_bit * pk.xs[i]);
    }

    // std::cout << "After third loop\n";

    pie::HenselCode c = pie::HenselCode(pk.x0, 1, (first_term + second_term) % pk.x0);

    // std::cout << "After computing c\n";

    return c;
  }

  Rational IDGHVDecrypt(IDGHVParams &params, const IDGHVPublicKey &pk, 
                        const IDGHVPrivateKey &sk, const HenselCode &c) {
    NTL::Vec<NTL::ZZ> hs;
    hs.SetLength(params.l);

    // std::cout << "After Decryption initialization\n";

    for (long i = 0; i < params.l; i++) {
      hs[i] = (c.code % sk.ps[i]) % pk.big_qs[i];
      // std::cout << "hs[" << i << "]: " << hs[i] << "\n";
    }

    // std::cout << "After loop\n";

    // std::cout << "hs size: " << hs.length() << "\n";
    // std::cout << "pk.big_qs size: " << pk.big_qs.length() << "\n";

    NTL::ZZ h = pie::CRT(pk.big_qs, hs);
    // std::cout << "(decryption) h: " << h << "\n";
    // std::cout << "After CRT\n";
    pie::HenselCode h_ = pie::HenselCode(pk.g, 1, h);
    // std::cout << "After Hensel code initialization\n";
    pie::Rational m = pie::Decode(pk.g, 1, h_);
    // std::cout << "After Hensel decode\n";

    return m;
  }

}