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
  std::cout << "params.gamma: " << params.gamma << "\n";
  std::cout << "params.tau: " << params.tau << "\n\n";

  pie::IDGHV idghv = pie::IDGHV(pk, sk);

  auto start = std::chrono::high_resolution_clock::now();
  idghv.KeyGen(params);
  auto stop = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = stop - start;
  double milliseconds = duration.count();
  Section("Key Generation Performance");
  PrintDuration(milliseconds);

  Header("Encryption");

  pie::Rational m1 = pie::Rational(2, 3);
  pie::Rational m2 = pie::Rational(4, 5);
  pie::HenselCode c1 = pie::IDGHVEncrypt(params, idghv.pk, m1);
  pie::HenselCode c2 = pie::IDGHVEncrypt(params, idghv.pk, m2);
  pie::Rational m1_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c1);
  pie::Rational m2_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c2);

  Section("Messages");

  std::cout << "m1: " << m1.ToString() << "\n";
  std::cout << "m2: " << m2.ToString() << "\n";

  Section("Encoding");

  start = std::chrono::high_resolution_clock::now();
  pie::HenselCode h1 = pie::Encode(idghv.pk.g, 1, m1);
  stop = std::chrono::high_resolution_clock::now();
  duration = stop - start;
  milliseconds = duration.count();
  PrintDuration(milliseconds);

  pie::HenselCode h2 = pie::Encode(idghv.pk.g, 1, m2);

  Section("Ciphertexts");

  std::cout << "c1: " << c1.code << "\n";
  std::cout << "c2: " << c2.code << "\n";

  Section("Decrypted Messages");

  std::cout << "m1_r: " << m1_r.ToString() << "\n";
  std::cout << "m2_r: " << m2_r.ToString() << "\n";

  Section("Homomorphic Addition and Multiplication");

  pie::HenselCode c1_plus_c2 = c1 + c2;
  pie::HenselCode c1_times_c2 = c1 * c2;

  std::cout << "c1_plus_c2: " << c1_plus_c2.code << "\n";
  std::cout << "c1_times_c2: " << c1_times_c2.code << "\n";

  Section("Decrypted Homomorphic Results");

  pie::Rational m1_plus_m2_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c1_plus_c2);
  pie::Rational m1_times_m2_r = pie::IDGHVDecrypt(params, idghv.pk, idghv.sk, c1_times_c2);

  std::cout << "m1_plus_m2_r: " << m1_plus_m2_r.ToString() << "\n";
  std::cout << "m1_times_m2_r: " << m1_times_m2_r.ToString() << "\n";

  return 0;
}