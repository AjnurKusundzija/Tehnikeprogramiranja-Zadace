#include <algorithm>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

auto KubnaInterpolacija(std::vector<std::pair<double, double>> tacke) {
  std::sort(std::begin(tacke), std::end(tacke));
  int vel = tacke.size();
  if (vel < 4) {
    throw std::domain_error("Nedovoljan broj cvorova");
  }
  for (int i = 0; i < vel - 1; i++) {
    if (tacke.at(i + 1).first == tacke.at(i).first) {
      throw std::domain_error("Neispravni cvorovi");
    }
  }

  return [tacke, vel](double k) {
    if ((tacke.at(0).first > k) || (tacke.at(vel - 1).first < k)) {
      throw std::range_error("Argument izvan opsega");
    }
    if (k == tacke.at(vel - 1).first) {
      return tacke.at(vel - 1).second;
    }

    double s = 0;

    int mjesto = 0;
    for (int i = 0; i < vel - 1; i++) {
      if (k == tacke.at(i).first) {
        return tacke.at(i).second;
      }
      if (k == tacke.at(i + 1).first) {
        return tacke.at(i + 1).second;
      }
      if (tacke.at(i).first <= k && k < tacke.at(i + 1).first) {
        mjesto = i;
        break;
      }
    }
    if (mjesto == 0)
      mjesto = 1;
    if (mjesto == vel - 2)
      mjesto--;

    for (int i = mjesto - 1; i <= mjesto + 2; i++) {
      double p = 1;
      for (int j = mjesto - 1; j <= mjesto + 2; j++) {
        if (j != i) {
          p *=
              (k - tacke.at(j).first) / (tacke.at(i).first - tacke.at(j).first);
        }
      }
      s += tacke.at(i).second * p;
    }
    return s;
  };
}

auto KubnaInterpolacija(std::function<double(double)> funkcija, double min,
                        double max, double razlika) {
  if (min > max || razlika <= 0 || razlika > max) {
    /* std::cout << min << "\n";
     std::cout << max << "\n";
     std::cout << razlika << "\n";*/
    throw std::domain_error("Nekorektni parametri");
  }
  std::vector<std::pair<double, double>> tacke;
  for (double i = min; i < max || std::abs(i - max) < 1e-5; i += razlika) {
    tacke.push_back(std::make_pair(i, funkcija(i)));
  }
  return KubnaInterpolacija(tacke);
}
double f(double x) { return x * x + sin(x) + log(x + 1); }

int main() {
  std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): "
            << std::endl;
  int odabir;
  std::cin >> odabir;
  if (odabir == 1) {
    std::cout << "Unesite broj cvorova: " << std::endl;
    try {
      int broj_cvorova;

      std::cin >> broj_cvorova;

      std::vector<std::pair<double, double>> cvorovi(broj_cvorova);
      std::cout << "Unesite cvorove kao parove x y: " << std::endl;
      for (int i = 0; i < broj_cvorova; i++) {
        std::cin >> cvorovi.at(i).first >> cvorovi.at(i).second;
      }

      auto Kubna = KubnaInterpolacija(cvorovi);
      while (true) {
        std::cout << "Unesite argument (ili \"kraj\" za kraj): " << std::endl;
        double x;
        std::cin >> x;
        if (!std::cin)
          break;

        try {
          double y = Kubna(x);
          std::cout << "f(" << x << ") = " << y << std::endl;

        } catch (std::range_error &e) {
          std::cout << e.what() << "!" << std::endl;
        }
      }
    } catch (std::domain_error &l) {
      std::cout << l.what();
    }

  } else if (odabir == 2) {
    std::cout << "Unesite krajeve intervala i korak: " << std::endl;
    try {
      double pocetak, kraj, korak;

      std::cin >> pocetak >> kraj >> korak;
      auto Kubna = KubnaInterpolacija(f, pocetak, kraj, korak);

      while (true) {
        std::cout << "Unesite argument (ili \"kraj\" za kraj): " << std::endl;
        double x;
        std::cin >> x;
        if (!std::cin)
          break;

        try {
          double y = Kubna(x);
          std::cout << "f(" << x << ") = " << f(x) << " fapprox(" << x
                    << ") = " << y << std::endl;
        } catch (std::exception &e) {
          std::cout << "Argument izvan opsega!" << std::endl;
        }
      }
    } catch (std::exception &z) {
      std::cout << z.what();
    }
  }
  return 0;
}
