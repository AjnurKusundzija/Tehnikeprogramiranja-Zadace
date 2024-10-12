// TP 2023/2024: Zadaća 1, Zadatak 1
#include <array>
#include <climits>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

enum class TretmanNegativnih {
  IgnorirajZnak,
  Odbaci,
  TretirajKao0,
  PrijaviGresku
};

int najmanji(int a, int b) { return (a < b) ? a : b; }

int BrojKvadrata(int n) {

  if (n < 0) {
    throw std::domain_error("Broj mora biti nenegativan");
  }
  if (n == 0) {
    return 0;
  }
  std::vector<int> br(n + 1, INT_MAX);
  br.at(0) = 0;
  for (int i = 1; i <= sqrt(n); i++) {
    int kvadrat = i * i;
    for (int j = kvadrat; j <= n; j++) {
      br.at(j) = najmanji(br.at(j), br.at(j - kvadrat) + 1);
    }
  }

  return br.at(n);
}

std::array<std::vector<int>, 5>
RazvrstajPoBrojuKvadrata(std::vector<int> brojevi, TretmanNegativnih tretman) {
  std::array<std::vector<int>, 5> rezultat;

  for (int i = 0; i < brojevi.size(); i++) {
    int temp = brojevi.at(i);
    if (brojevi.at(i) < 0) {
      if (tretman == TretmanNegativnih::IgnorirajZnak) {
        brojevi.at(i) = -brojevi.at(i);
      } else if (tretman == TretmanNegativnih::Odbaci) {
        continue;
      } else if (tretman == TretmanNegativnih::TretirajKao0) {
        brojevi.at(i) = 0;
      } else if (tretman == TretmanNegativnih::PrijaviGresku) {
        throw std::domain_error(
            "Nije predvidjeno razvrstavanje negativnih brojeva");
      }
    }
    int brojKvadrata = BrojKvadrata(brojevi.at(i));

    if (brojKvadrata <= 4) {
      rezultat.at(brojKvadrata).push_back(temp);
    }
  }

  return rezultat;
}

int main() {
  std::vector<int> brojevi;
  int broj;
  bool negativan = false;
  std::cout << "Unesite brojeve (bilo koji ne-broj oznacava kraj):"
            << std::endl;

  while (std::cin >> broj) {
    if (broj < 0) {
      std::cout << "Nije podrzano razvrstavanje negativnih brojeva!"
                << std::endl;
      negativan = true;
      break;
    }
    brojevi.push_back(broj);
  }
  std::cin.clear();
  std::cin.ignore(10000, '\n');
  if (!negativan) {
    TretmanNegativnih tretman = TretmanNegativnih::PrijaviGresku;
    try {
      std::array<std::vector<int>, 5> rezultat =
          RazvrstajPoBrojuKvadrata(brojevi, tretman);
      std::cout << "Rezultati razvrstavanja po broju kvadrata:\n";
      for (int i = 0; i < rezultat.size(); i++) {
        if (!rezultat.at(i).empty()) {
          std::cout << i << ": ";
          for (int j = 0; j < rezultat.at(i).size(); j++) {
            std::cout << rezultat.at(i).at(j) << " ";
          }
          std::cout << "\n";
        }
      }
    } catch (std::domain_error &e) {
      std::cout << e.what() << std::endl;
    }
  }

  return 0;
}
