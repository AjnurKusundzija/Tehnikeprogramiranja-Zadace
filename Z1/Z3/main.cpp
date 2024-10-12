// TP 2023/2024: Zadaća 1, Zadatak 3
#include <climits>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

long long PretvoriStringUBroj(std::string broj) {
  long long int finalni = 0;
  bool negativan = false;
  int i = 0;
  if (broj.at(0) == '-') {
    negativan = true;
    i++;
  }
  for (; i < broj.length(); i++) {
    if (broj.at(i) < '0' || broj.at(i) > '9') {
      throw std::invalid_argument("greska neka...");
    }
    if (finalni >
        (std::numeric_limits<long long int>::max() - (broj.at(i) - '0')) / 10) {
      throw std::overflow_error("Uneseni string sadrzi prevelik broj!!!");
      throw std::range_error("Broj je prevelik");
    }
    finalni = finalni * 10 + (broj.at(i) - '0');
  }
  return negativan ? -finalni : finalni;
}

std::vector<long long int> BrojeviUStringu(std::string s) {
  std::vector<long long int> brojevi;
  std::string trenutniBroj = "";
  bool JelBroj = true;
  for (int i = 0; i < s.length(); i++) {
    char c = s.at(i);
    if ((c >= '0' && c <= '9') || c == '-') {

      trenutniBroj += c;
      JelBroj = true;
    } else if (JelBroj) {
      if (!(trenutniBroj.size() == 0) &&
          ((trenutniBroj != "-" && trenutniBroj.at(0) == '-') ||
           (trenutniBroj.at(0) >= '0' && trenutniBroj.at(0) <= '9'))) {

        if (i == s.length() || ((s.at(i) < 'a' || s.at(i) > 'z') &&
                                (s.at(i) < 'A' || s.at(i) > 'Z'))) {
          try {
            long long int broj = PretvoriStringUBroj(trenutniBroj);
            brojevi.push_back(broj);
          } catch (std::domain_error &k) {
            // skipamo  ga
          }
        }
      }
      trenutniBroj = "";
      JelBroj = false;
    }
  }

  if (JelBroj && !trenutniBroj.empty()) {
    try {
      long long int broj = PretvoriStringUBroj(trenutniBroj);
      brojevi.push_back(broj);
    } catch (std::domain_error &e) {
    }
    // skip opet
  }
  return brojevi;
}

int main() {
  std::string ulaz;
  std::cout << "Unesite string: ";
  std::getline(std::cin, ulaz);
  try {
    std::vector<long long int> brojevi = BrojeviUStringu(ulaz);
    if (brojevi.empty()) {
      std::cout << "Uneseni string ne sadrzi nijedan broj!\n";

    } else {
      std::cout << "Brojevi unutar stringa: ";

      for (long long int broj : brojevi) {
        std::cout << broj << " ";
      }
      std::cout << std::endl;
    }
  } catch (std::overflow_error &e) {
    std::cout << "PROBLEM: " << e.what() << std::endl;
  } catch (std::range_error &k) {
    std::cout << "Izuzetak s porukom:" << k.what();
  }

  return 0;
}


