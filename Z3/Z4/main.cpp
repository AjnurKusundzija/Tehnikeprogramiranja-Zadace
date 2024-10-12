// TP 2023/2024: Zadaća 3, Zadatak 4
#include <cmath>
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

std::vector<int> Razbrajanje(int N, int M) {
  std::list<int> lista;
  for (int i = 0; i < N; i++) {
    lista.push_back(i + 1);
  }

  std::vector<int> razabrani;
  auto it = lista.begin();
  razabrani.push_back(*it);
  it = lista.erase(it);
  for (int i = 0; i < M - 1; i++) {
    if (++it == lista.end()) {
      it = lista.begin();
    }
  }
  while (!lista.empty()) {
    razabrani.push_back(*it);
    it = lista.erase(it);
    if (it == lista.end()) {
      it = lista.begin();
    }
    for (int i = 0; i < M - 1; i++) {
      if (++it == lista.end()) {
        it = lista.begin();
      }
    }
  }
  return razabrani;
}

int OdabirKoraka(int N, int K) {
  if ((K > N) || (K <= 0)) {
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
  }

  return 0;
}
