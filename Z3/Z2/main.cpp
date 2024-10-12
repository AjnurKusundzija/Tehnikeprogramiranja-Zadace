// TP 2023/2024: Zadaća 3, Zadatak 2
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::tuple<std::string, int, int> Pozicija;
typedef std::map<std::string, std::set<Pozicija>> IndeksPojmova;

std::string PretvoriUSlova(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}
bool JeliAlfaNum(char c) {
  return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
          (c >= 'a' && c <= 'z'));
}
IndeksPojmova KreirajIndeksPojmova(Knjiga &knjiga) {
  IndeksPojmova index;
  for (const auto &poglavlje : knjiga) {
    const std::string &oznakapoglavlja = poglavlje.first;
    const std::vector<std::string> &stranice = poglavlje.second;
    for (int i = 0; i < stranice.size(); i++) {
      const std::string &stranica = stranice.at(i);
      std::string rijec;
      for (int j = 0; j <= stranica.size(); j++) {
        if (j < stranica.size() && JeliAlfaNum(stranica.at(j))) {
          rijec += stranica.at(j);
        } else if (!rijec.empty()) {
          index[PretvoriUSlova(rijec)].insert(
              std::make_tuple((oznakapoglavlja), i + 1, j - rijec.size() + 1));
          rijec.clear();
        }
      }
    }
  }
  return index;
}

std::set<Pozicija> PretraziIndeksPojmova(const std::string &rijec,
                                         IndeksPojmova &index) {
  if (!std::all_of(rijec.begin(), rijec.end(), JeliAlfaNum)) {
    throw std::domain_error("Neispravna rijec");
  }
  auto it = index.find(PretvoriUSlova(rijec));
  if (it == index.end()) {
    return {};

  } else {
    return it->second;
  }
}
void IspisiIndeksPojmova(IndeksPojmova &index) {
  for (const auto &par : index) {
    const std::string &rijec = par.first;
    const std::set<Pozicija> &pozicije = par.second;
    std::cout << rijec << ": ";
    std::vector<Pozicija> pozicijeVek(pozicije.begin(), pozicije.end());
    for (int i = 0; i < pozicijeVek.size(); i++) {
      const auto &pozicija = pozicijeVek.at(i);
      std::cout << std::get<0>(pozicija) << "/" << std::get<1>(pozicija) << "/"
                << std::get<2>(pozicija);
      if (i != pozicijeVek.size() - 1) {
        std::cout << ", ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
int main() {
  Knjiga knjiga;
  std::string poglavlje;
  while (true) {
    std::cout << std::endl << "Unesite naziv poglavlja: " << std::endl;
    std::getline(std::cin, poglavlje);
    if (poglavlje.empty())
      break;
    std::vector<std::string> stranice;
    while (true) {
      std::cout << std::endl
                << "Unesite sadrzaj stranice " << stranice.size() + 1 << ":";
      std::string stranica;
      std::getline(std::cin, stranica);
      if (stranica.empty())
        break;
      stranice.push_back(stranica);
    }
    knjiga[poglavlje] = stranice;
  }
  IndeksPojmova index = KreirajIndeksPojmova(knjiga);
  std::cout << std::endl << "Kreirani indeks pojmova:" << std::endl;
  IspisiIndeksPojmova(index);
  while (true) {
    std::cout << std::endl << "Unesite rijec: " << std::endl;
    std::string rijec;
    std::getline(std::cin, rijec);
    if (rijec.empty())
      break;
    try {
      std::set<Pozicija> pozicije = PretraziIndeksPojmova(rijec, index);
      if (pozicije.empty()) {
        std::cout << std::endl << "Rijec nije nadjena!";

      } else {
        std::cout << std::endl << "Rijec nadjena na pozicijama: ";
        for (const auto &pozicija : pozicije) {
          std::cout << std::get<0>(pozicija) << "/" << std::get<1>(pozicija)
                    << "/" << std::get<2>(pozicija) << " ";
        }
        std::cout << std::endl;
      }

    } catch (std::domain_error &e) {
      std::cout << e.what() << "!" << std::endl;
    }
  }
  std::cout << std::endl << "Dovidjenja!";
  return 0;
}
