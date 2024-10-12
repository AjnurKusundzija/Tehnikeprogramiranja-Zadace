// TP 2023/2024: Zadaća 2, Zadatak 3
#include <algorithm>
#include <climits>
#include <cmath>
#include <deque>
#include <iostream>
#include <vector>

template <typename It1, typename It2, typename IzlazniIt>
IzlazniIt UnijaBlokova(It1 p1, It1 p2, It2 p3, It2 p4, IzlazniIt p5) {
  IzlazniIt p6 = p5;
  for (auto it = p1; it != p2; it++) {
    bool nadjen = false;
    for (auto it2 = p5; it2 != p6; it2++) {
      if (*it == *it2) {
        nadjen = true;
        break;
      }
    }
    if (!nadjen) {
      *p6 = *it;
      ++p6;
    }
  }
  for (auto it = p3; it != p4; it++) {
    bool nadjen = false;
    for (auto it2 = p5; it2 != p6; it2++) {
      if (*it == *it2) {
        nadjen = true;
        break;
      }
    }
    if (!nadjen) {
      *p6 = *it;
      ++p6;
    }
  }
  return p6;
}

int main() {
  int niz[100];
  std::deque<int> dek;
  double rez[200];
  int n, m;
  std::cout << "Unesite broj elemenata niza (max. 100): ";
  std::cin >> n;
  std::cout << "Unesite elemente niza: ";
  for (int i = 0; i < n; i++) {
    std::cin >> *(niz + i);
  }
  std::cout << "Unesite broj elemenata deka: ";
  std::cin >> m;
  std::cout << "Unesite elemente deka: ";
  for (int i = 0; i < m; i++) {
    int elem;
    std::cin >> elem;
    dek.push_back(elem);
  }
  auto it = UnijaBlokova(niz, niz + n, dek.begin(), dek.end(), rez);
  std::cout << "Njihova unija glasi: ";
  for (auto l = rez; l != it; l++) {
    std::cout << *l << " ";
  }
  std::cout << std::endl;

  return 0;
}
