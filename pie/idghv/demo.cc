#include "lib/include/hensel_code.h"
#include "lib/idghv/include/idghv.h"
#include "lib/include/helpers.h"
#include <NTL/ZZ.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>

int main() {

  Title("PIE + IDGHV");

  Header("Key Generation");

  Section("Parameters");

  pie::IDGHVPublicKey pk;
  pie::IDGHVPrivateKey sk;
  
  pie::IDGHVParams params = pie::IDGHVParams(52, 37, 41, 1558, 900000, 661);
  // pie::IDGHVParams params = pie::IDGHVParams(50, 6, 60, 4248, 530000000, 661);

  std::cout << "params.lambda: " << params.lambda << "\n";
  std::cout << "params.l: " << params.l << "\n";
  std::cout << "params.rho: " << params.rho << "\n";
  std::cout << "params.eta: " << params.eta << "\n";
  std::cout << "params.gamma: " << params.gamma << "\n\n";
  std::cout << "params.tau: " << params.tau << "\n\n";

  pie::IDGHV idghv = pie::IDGHV(pk, sk);
  // std::cout << "idghv.lambda: " << idghv.params.lambda << "\n";
  // long lambda = 52;
  // long l = 37;
  // long rho = 41;
  // long eta = 1558;
  // long gamma = 900000;

  auto start = std::chrono::high_resolution_clock::now();
  idghv.KeyGen(params);
  auto stop = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = stop - start;
  double milliseconds = duration.count();
  PrintDuration(milliseconds);

  // pie::Rational m = pie::Rational(2, 3);;
  // m.SetLength(3);
  // m[0] = NTL::ZZ(1);
  // m[1] = NTL::ZZ(2);
  // m[2] = NTL::ZZ(3);

  std::cout << "g: " << idghv.pk.g << "\n";

  pie::Rational m = pie::Rational(2, 3);
  pie::HenselCode c = pie::IDGHVEncrypt(params, idghv.pk, m);
  pie::Rational m_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c);

  std::cout << "m: " << m.ToString() << "\n";
  std::cout << "m_r: " << m_r.ToString() << "\n";

  // std::cout << "pk.g: " << pk.g << "\n";
  // std::cout << "pk.big_qs[0]: " << pk.big_qs[0] << "\n";

  // pie::HenselCode h = pie::IDGHVEncrypt(params, pk, m);

  // std::cout << "h: " << h.code << "\n";

  // std::cout << "idghv.lambda: " << idghv.params.lambda << "\n";
  // std::cout << "idghv.l: " << idghv.params.l << "\n";
  // std::cout << "idghv.rho: " << idghv.params.rho << "\n";
  // std::cout << "idghv.eta: " << idghv.params.eta << "\n";
  // std::cout << "idghv.gamma: " << idghv.params.gamma << "\n\n";
  // std::cout << "idghv.tau: " << idghv.params.tau << "\n\n";

  return 0;
}