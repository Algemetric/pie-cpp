#include <iostream>
#include <string>
#include <unistd.h>

void Title(const std::string &string) {
  std::cout << "\n================================================\n";
  std::cout << string << "\n";
  std::cout << "================================================\n\n";
}

void Header(const std::string &string) {
  std::cout << string << "\n";
  std::cout << "------------------------------------------------\n\n";
}

void Section(const std::string &string) {
  std::cout << string << ": \n\n";
}

void PrintDuration(double milliseconds) {
  std::cout << milliseconds << " milliseconds" << "\n\n";
}