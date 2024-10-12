// TP 2023/2024: Zadaća 2, Zadatak 2
#include <algorithm>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


template <typename T, typename U, typename F>
auto GeneraliziraniKroneckerovProizvod(T a, U b, F f) {

  if (a.empty() || b.empty()) {
    throw std::domain_error("Parametri nemaju formu matrice");
  }
  int m1 = a.size(), n1 = a[0].size();
  for (int i = 1; i < m1; i++) {
    if (a[i].size() != n1) {
      throw std::domain_error("Prvi parametar nema formu matrice.");
    }
  }
  int m2 = b.size(), n2 = b[0].size();
  for (int i = 1; i < m2; i++) {
    if (b[i].size() != n2) {
      throw std::domain_error("Drugi parametar nema formu matrice.");
    }
  }

  using tiprezultata = decltype(a[0][0] * b[0][0]);
  std::vector<std::vector<tiprezultata>> rezultat(
      m1 * m2, std::vector<tiprezultata>(n1 * n2));

  for (int i = 0; i < m1; i++) {
    for (int j = 0; j < n1; j++) {
      for (int p = 0; p < m2; p++) {
        for (int q = 0; q < n2; q++) {
          try {
            rezultat[i * m2 + p][j * n2 + q] = f(a[i][j], b[p][q]);
          } catch (...) {
            throw std::runtime_error("Neocekivani problemi pri racunanju");
          }
        }
      }
    }
  }
  return rezultat;
}

int main() {
  int m1, n1, m2, n2;
  std::cout << "Unesite dimenzije prve matrice: ";
  std::cin >> m1 >> n1;
  std::vector<std::deque<double>> a(m1, std::deque<double>(n1));
  std::cout << "Unesite elemente prve matrice: ";

  for (auto &red : a) {
    for (auto &elem : red) {
      std::cin >> elem;
    }
  }
  std::cout << "Unesite dimenzije druge matrice: ";
  std::cin >> m2 >> n2;
  std::deque<std::vector<double>> b(m2, std::vector<double>(n2));
  std::cout << "Unesite elemente druge matrice: ";
  for (auto &red : b) {
    for (auto &elem : red) {
      std::cin >> elem;
    }
  }
  auto rezultat = GeneraliziraniKroneckerovProizvod(
      a, b, [](double a, double b) { return a * b; });
  double max = std::abs(rezultat[0][0]);
  for (auto &red : rezultat) {
    for (auto &elem : red) {
      if (std::abs(elem) > max) {
        max = std::abs(elem);
      }
    }
  }
  int sirina = std::to_string((int)max).length() + 2;

  std::cout << "Njihov Kroneckerov proizvod glasi:" << std::endl;

  for (int i = 0; i < m1 * m2; i++) {
    for (int j = 0; j < n1 * n2; j++) {
      std::cout << std::setw(sirina) << rezultat[i][j];
    }
    std::cout << std::endl;
  }

  return 0;
}
