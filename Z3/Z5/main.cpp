// TP 2023/2024: Zadaća 3, Zadatak 5
#include <cmath>
#include <iostream>
#include <new>
#include <stdexcept>
#include <vector>

struct Distrikt {
  int broj_distrikta;
  Distrikt *sljedeci;
};

std::vector<int> Razbrajanje(int N, int M) {
  Distrikt *prvi = new Distrikt{1, nullptr};
  Distrikt *trenutni = prvi;
  try {
    for (int i = 2; i <= N; i++) {
      trenutni->sljedeci = new Distrikt{i, nullptr};
      trenutni = trenutni->sljedeci;
    }
    trenutni->sljedeci = prvi;
    int ostalo = N;
    std::vector<int> razabrani;
    while (true) {
      Distrikt *izbaceni_distrikt = prvi;
      trenutni->sljedeci = izbaceni_distrikt->sljedeci;
      prvi = prvi->sljedeci;
      razabrani.push_back(izbaceni_distrikt->broj_distrikta);
      delete izbaceni_distrikt;
      ostalo--;
      if (ostalo == 0)
        break;
      for (int i = 1; i < M; i++) {
        prvi = prvi->sljedeci;
        trenutni = trenutni->sljedeci;
      }
    }
    return razabrani;
  } catch (...) {
    while (prvi != nullptr) {
      Distrikt *sljedeci = prvi->sljedeci;
      delete prvi;
      prvi = sljedeci;
    }
    throw;
  }
}

int OdabirKoraka(int N, int K) {
  if (N == 1)
    return 1;
  if ((K > N) || (K <= 0) || (N == 0)) {
    throw std::domain_error(
        "Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni "
        "broj bloka ne moze biti veci od broja blokova");
  }
  for (int M = 1; M <= N; M++) {
    std::vector<int> razbrajanje = Razbrajanje(N, M);
    if (razbrajanje.at(razbrajanje.size() - 1) == K) {
      return M;
    }
  }
  return 0;
}
int main() {

  try {

    std::cout << "Unesite broj distrikta u gradu: ";
    int N;
    std::cin >> N;
    int K;
    std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
    std::cin >> K;
    int korak = OdabirKoraka(N, K);
    std::cout << "Trazeni korak: " << korak << std::endl;
  } catch (std::domain_error &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Neka greska bilo sta!";
  }
  return 0;
}