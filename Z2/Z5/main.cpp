// TP 2023/2024: Zadaća 2, Zadatak 5
#include <cmath>
#include <deque>
#include <iostream>
#include <new>
#include <vector>

template <typename kont1, typename kont2>

auto **KreirajTablicuMnozenja(kont1 &prvi, kont2 &drugi) {
  int prvi_vel = std::distance(prvi.begin(), prvi.end());
  int drugi_vel = std::distance(drugi.begin(), drugi.end());
  if (prvi_vel != drugi_vel)
    throw std::range_error("Kontejneri nisu iste duzine");

  decltype(prvi.begin()) a = prvi.begin();
  decltype(drugi.begin()) b = drugi.begin();

  auto x = *a, y = *b;
  decltype(x * y) **matrica = nullptr;
  try {
    matrica = new decltype(x * y) *[prvi_vel] {};
  } catch (...) {
    delete matrica;
    throw std::range_error("Nema dovoljno memorije");
  }
  try {
    matrica[0] = new decltype(x * y)[(prvi_vel * (prvi_vel + 1)) / 2]{};

    for (int i = 1; i < prvi_vel; i++) {
      matrica[i] = matrica[i - 1] + i;
    }
  } catch (...) {
    delete[] matrica[0];
    delete[] matrica;
    throw std::range_error("Nema dovoljno memorije");
  }
  for (int i = 0; i < prvi_vel; i++) {
    b = drugi.begin();
    for (int j = 0; j <= i; j++) {
      x = *a;
      y = *b++;
      if (x * y != y * x) {
        delete[] matrica[0];
        delete[] matrica;
        throw std::logic_error("Nije ispunjena pretpostavka o komutativnosti");
      }
      matrica[i][j] = x * y;
    }
    a++;
  }
  return matrica;
}

int main() {
  int duzina;
  std::cout << "Duzina sekvenci: ";
  std::cin >> duzina;
  std::cout << "Elementi prve sekvence: ";

  std::vector<double> prvi(duzina);
  for (auto &element : prvi) {
    std::cin >> element;
  }

  std::cout << "Elementi druge sekvence: ";
  std::vector<double> drugi(duzina);
  for (auto &element : drugi) {
    std::cin >> element;
  }
  std::cout << "Tablica mnozenja: " << std::endl;
  try {
    auto matrica = KreirajTablicuMnozenja(prvi, drugi);
    for (int i = 0; i < duzina; i++) {
      for (int j = 0; j <= i; j++) {
        std::cout << matrica[i][j] << " ";
      }
      std::cout << std::endl;
    }
    delete[] matrica[0];
    delete[] matrica;
  } catch (std::range_error e) {
    std::cout << e.what();
  } catch (std::logic_error e) {
    std::cout << e.what();
  }
  return 0;
}
