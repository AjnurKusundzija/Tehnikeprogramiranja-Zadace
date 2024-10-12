// TP 2023/2024: Zadaća 1, Zadatak 2
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

void Sortiraj(std::vector<double> &prozor) {
  int n = prozor.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (prozor.at(j) > prozor.at(j + 1)) {
        std::swap(prozor.at(j), prozor.at(j + 1));
      }
    }
  }
}

std::vector<std::vector<double>>
MedijanskiFilter(const std::vector<std::vector<double>> &matrica, int N) {
  if (N < 0) {
    throw std::domain_error("Neispravan red filtriranja");
  }
  std::vector<std::vector<double>> rezultat(
      matrica.size(), std::vector<double>(matrica.at(0).size()));
  for (int i = 0; i < matrica.size(); i++) {
    for (int j = 0; j < matrica.at(0).size(); j++) {
      std::vector<double> prozor;
      for (int k = std::max(0, i - N);
           k <= std::min((int)matrica.size() - 1, i + N); k++) {
        for (int l = std::max(0, j - N);
             l <= std::min((int)matrica.at(k).size() - 1, j + N); l++) {
          prozor.push_back(matrica.at(k).at(l));
        }
      }
      Sortiraj(prozor);
      if (prozor.size() % 2 == 1) {
        rezultat.at(i).at(j) = prozor.at(prozor.size() / 2);
      } else {
        rezultat.at(i).at(j) =
            (prozor.at(prozor.size() / 2 - 1) + prozor.at(prozor.size() / 2)) /
            2.0;
      }
    }
  }
  return rezultat;
}

int main() {
  int m, n, N;
  std::cout << "Unesite broj redova i kolona matrice: ";
  std::cin >> m >> n;
  std::cout << "Unesite elemente matrice: ";
  std::vector<std::vector<double>> matrica(m, std::vector<double>(n));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> matrica.at(i).at(j);
    }
  }
  try {

    std::cout << "Unesite red filtriranja: " << std::endl;
    std::cin >> N;
    if (N < 0) {
      std::cout << std::endl;
      std::cout << "GRESKA: Neispravan red filtriranja! ";
      return 0;
    }
    std::cout << "Matrica nakon filtriranja:" << std::endl;
    std::vector<std::vector<double>> rezultat = MedijanskiFilter(matrica, N);
    for (int i = 0; i < rezultat.size(); i++) {
      for (int j = 0; j < rezultat.at(i).size(); j++) {
        std::cout << std::setw(7) << std::fixed << std::setprecision(2)
                  << rezultat.at(i).at(j);
      }
      std::cout << std::endl;
    }
  } catch (const std::domain_error &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

