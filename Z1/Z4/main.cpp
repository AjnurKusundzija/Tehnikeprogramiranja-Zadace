// TP 2023/2024: Zadaća 1, Zadatak 4
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>
std::string ViginereSifriranje(const std::string &poruka, const std::string &kljuc) {
  for (char c : kljuc) {
    if (!std::isalpha(c) || !std::isupper(c)) {
      throw std::domain_error("Neispravan kljuc");
    }
  }

  std::string sifriran = "";
  int kljuc_index = 0;
  for (char slovo : poruka) {
    if (std::isalpha(slovo)) {
      char pomjeri = kljuc.at(kljuc_index % kljuc.length());
      pomjeri = std::toupper(pomjeri) - 'A';
      if (std::isupper(slovo)) {
        sifriran += 'A' + (slovo - 'A' + pomjeri) % 26;

      } else {
        sifriran += 'a' + (slovo - 'a' + pomjeri) % 26;
      }
      kljuc_index++;

    } else {
      sifriran += slovo;
    }
  }
  return sifriran;
}

std::string ViginereDesifriranje(const std::string &sifriran,
                                 const std::string &kljuc) {

  for (char c : kljuc) {
    if (!std::isalpha(c) || !std::isupper(c)) {
      throw std::domain_error("Neispravan kljuc");
    }
  }

  std::string poruka = "";
  int kljuc_index = 0;
  for (char slovo : sifriran) {
    if (std::isalpha(slovo)) {
      char pomjeri = kljuc.at(kljuc_index % kljuc.length());
      pomjeri = std::toupper(pomjeri) - 'A';
      if (std::isupper(slovo)) {
        poruka += 'A' + (slovo - 'A' - pomjeri + 26) % 26;

      } else {
        poruka += 'a' + (slovo - 'a' - pomjeri + 26) % 26;
      }
      kljuc_index++;

    } else {
      poruka += slovo;
    }
  }
  return poruka;
}
bool JeliValidan(std::string kljuc) {
  if (kljuc.empty())
    throw std::domain_error("Neispravan kljuc");
  for (char x : kljuc) {
    if (!std::isalpha(x) || !std::isupper(x))
      return false;
  }
  return true;
}

int main() {
  std::string kljuc;
  while (true) {
    std::cout << "Unesite kljuc: ";
    std::getline(std::cin, kljuc);

    if (JeliValidan(kljuc)) {
      std::cout << "Kljuc prihvacen. ";
      break;
    } else {
      std::cout << "Neispravan kljuc, ponovite unos:" << std::endl;
    }
  }
  std::string poruka;
  std::cout << "Unesite poruku: ";
  std::getline(std::cin, poruka);
  std::string sifriran = ViginereSifriranje(poruka, kljuc);
  std::cout << "Sifrirana poruka: " << sifriran << std::endl << std::endl;
  while (true) {
    std::string pokusaj_kljuca;
    std::cout << "Pokusajte pogoditi kljuc: ";
    std::getline(std::cin, pokusaj_kljuca);
    if (!JeliValidan(pokusaj_kljuca)) {
      std::cout << "Niste pogodili kljuc, probajte ponovo!" << std::endl;
      continue;
    }
    std::string desifrirana_poruka =
        ViginereDesifriranje(sifriran, pokusaj_kljuca);
    if (desifrirana_poruka == poruka) {

      std::cout << "Pokusaj desifriranja: " << desifrirana_poruka << std::endl;

      std::cout << "Bravo, pogodili ste kljuc!" << std::endl;
      break;

    } else {
      std::cout << "Pokusaj desifriranja: " << desifrirana_poruka << std::endl;
      std::cout << "Niste pogodili kljuc, probajte ponovo!" << std::endl;
    }
    try {

    } catch (std::domain_error k) {
      std::cout << k.what() << std::endl;
    }
  }

  return 0;
}
