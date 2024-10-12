// TP 2023/2024: Zadaća 5, Zadatak 2
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Berza {
  std::vector<int> cijene;
  int min;
  int max;

public:
  Berza(int x, int y);
  explicit Berza(int y);
  void RegistrirajCijenu(int cijena);
  int DajBrojRegistriranihCijena() const;
  void BrisiSve();
  int DajMinimalnuCijenu() const;
  int DajMaksimalnuCijenu() const;
  bool operator!() const;
  int DajBrojCijenaVeciOd(int vrijednost) const;
  void Ispisi() const;
  int operator[](int indeks) const;
  Berza &operator++();
  Berza operator++(int);
  Berza &operator--();
  Berza operator--(int);
  Berza operator-() const;
  Berza &operator+=(int y);
  Berza &operator+=(const Berza &a);
  Berza &operator-=(int y);
  Berza &operator-=(const Berza &a);

  friend Berza operator+(const Berza &b1, const Berza &b2);
  friend Berza operator-(const Berza &b1, const Berza &b2);
  friend Berza operator+(const Berza &a, int y);
  friend Berza operator-(const Berza &a, int y);
  friend Berza operator+(int y, const Berza &a);
  friend Berza operator-(int y, const Berza &a);
  friend bool operator==(const Berza &b1, const Berza &b2);
  friend bool operator!=(const Berza &b1, const Berza &b2);
};

Berza::Berza(int x, int y) : min(x), max(y) {

  if (x < 0 || y < 0 || x > y) {
    throw std::range_error("Ilegalne granicne cijene");
  }
}
Berza::Berza(int y) : min{}, max(y) {
  if (y < 0) {
    throw std::range_error("Ilegalne granicne cijene");
  }
}

void Berza::RegistrirajCijenu(int cijena) {
  if (cijena < min || cijena > max) {
    throw std::range_error("Ilegalna cijena");
  }
  cijene.push_back(cijena);
}

int Berza::DajBrojRegistriranihCijena() const { return cijene.size(); }
void Berza::BrisiSve() { cijene.clear(); }
int Berza::DajMinimalnuCijenu() const {
  if (cijene.size() == 0) {
    throw std::range_error("Nema registriranih cijena");
  }
  return *std::min_element(std::begin(cijene), std::end(cijene));
}
int Berza::DajMaksimalnuCijenu() const {
  if (cijene.size() == 0) {
    throw std::range_error("Nema registriranih cijena");
  }
  return *std::max_element(std::begin(cijene), std::end(cijene));
}
bool Berza::operator!() const { return cijene.empty(); }

int Berza::DajBrojCijenaVeciOd(int vrijednost) const {
  if (cijene.size() == 0) {
    throw std::range_error("Nema registriranih cijena");
  }
  return std::count_if(
      std::begin(cijene), std::end(cijene),
      std::bind(std::greater<int>(), std::placeholders::_1, vrijednost));
}
void Berza::Ispisi() const {
  if (cijene.size() == 0) {
    throw std::range_error("Nema registriranih cijena");
  }
  std::vector<int> temp = cijene;
  std::sort(std::begin(temp), std::end(temp), std::greater<int>());
  std::for_each(std::begin(temp), std::end(temp), [](int cijena) {
    std::cout << std::fixed << std::setprecision(2) << cijena / 100.
              << std::endl;
  });
}
int Berza::operator[](int indeks) const {
  if (indeks < 1 || indeks > cijene.size()) {
    throw std::range_error("Neispravan indeks");
  }
  return cijene[indeks - 1];
}

Berza &Berza::operator++() {
  std::transform(std::begin(cijene), std::end(cijene), std::begin(cijene),
                 [this](int cijena) {
                   if (cijena + 100 > max) {
                     throw std::range_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return cijena + 100;
                 });

  return *this;
}
Berza Berza::operator++(int) {
  Berza kopija(*this);
  ++(*this);
  return kopija;
}

Berza &Berza::operator--() {
  std::transform(std::begin(cijene), std::end(cijene), std::begin(cijene),
                 [this](int cijena) {
                   if (cijena - 100 < min) {
                     throw std::range_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return cijena - 100;
                 });

  return *this;
}

Berza Berza::operator-() const {
  Berza nova(*this);
  std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(),
                 [this](int cijena) { return max + min - cijena; });
  return nova;
}

Berza &Berza::operator+=(int y) {
  *this = *this + y;
  return *this;
}

Berza &Berza::operator+=(const Berza &a) {
  if (min != a.min || max != a.max) {
    throw std::domain_error("Nesaglasni operandi");
  }
  if (cijene.size() != a.cijene.size()) {
    throw std::domain_error("Nesaglasni operandi");
  }
  std::transform(cijene.begin(), cijene.end(), a.cijene.begin(), cijene.begin(),
                 [this](int cijena1, int cijena2) {
                   int nova_cijena = cijena1 + cijena2;
                   if (nova_cijena < min || nova_cijena > max) {
                     throw std::domain_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return nova_cijena;
                 });
  return *this;
}

Berza &Berza::operator-=(int y) {
  *this = *this - y;
  return *this;
}

Berza &Berza::operator-=(const Berza &a) {
  if (min != a.min || max != a.max) {
    throw std::domain_error("Nesaglasni operandi");
  }
  if (cijene.size() != a.cijene.size()) {
    throw std::domain_error("Nesaglasni operandi");
  }
  std::transform(cijene.begin(), cijene.end(), a.cijene.begin(), cijene.begin(),
                 [this](int cijena1, int cijena2) {
                   int nova_cijena = cijena1 - cijena2;
                   if (nova_cijena < min || nova_cijena > max) {
                     throw std::domain_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return nova_cijena;
                 });
  return *this;
}

Berza operator+(const Berza &b1, const Berza &b2) {
  if (b1.min != b2.min || b1.max != b2.max) {
    throw std::domain_error("Nesaglasni operandi");
  }
  Berza nova(b1.min, b1.max);
  if (b1.cijene.size() != b2.cijene.size()) {
    throw std::domain_error("Nesaglasni operandi");
  }
  std::transform(
      b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(),
      std::back_inserter(nova.cijene), [&nova](int cijena1, int cijena2) {
        int nova_cijena = cijena1 + cijena2;
        if (nova_cijena < nova.min || nova_cijena > nova.max) {
          throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        }
        return nova_cijena;
      });
  return nova;
}

Berza operator-(const Berza &b1, const Berza &b2) {
  if (b1.min != b2.min || b1.max != b2.max) {
    throw std::domain_error("Nesaglasni operandi");
  }
  Berza nova(b1.min, b1.max);
  if (b1.cijene.size() != b2.cijene.size()) {
    throw std::domain_error("Nesaglasni operandi");
  }
  std::transform(
      b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(),
      std::back_inserter(nova.cijene), [&nova](int cijena1, int cijena2) {
        int nova_cijena = cijena1 - cijena2;
        if (nova_cijena < nova.min || nova_cijena > nova.max) {
          throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        }
        return nova_cijena;
      });
  return nova;
}

Berza operator+(const Berza &a, int y) {
  Berza nova(a);
  std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(),
                 [y, &nova](int cijena) {
                   if (cijena + y > nova.max || cijena + y < nova.min) {
                     throw std::domain_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return cijena + y;
                 });
  return nova;
}
Berza Berza::operator--(int) {
  Berza kopija(*this);
  --(*this);
  return kopija;
}
Berza operator-(const Berza &a, int y) {
  Berza nova(a);
  std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(),
                 [y, &nova](int cijena) {
                   if (cijena - y > nova.max || cijena - y < nova.min) {
                     throw std::domain_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return cijena - y;
                 });
  return nova;
}

Berza operator+(int y, const Berza &a) { return a + y; }

Berza operator-(int y, const Berza &a) {
  Berza nova(a);
  std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(),
                 [y, &nova](int cijena) {
                   if (y - cijena > nova.max || y - cijena < nova.min) {
                     throw std::domain_error(
                         "Prekoracen dozvoljeni opseg cijena");
                   }
                   return y - cijena;
                 });
  return nova;
}

bool operator==(const Berza &b1, const Berza &b2) {
  return b1.cijene.size() == b2.cijene.size() &&
         std::equal(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin());
}

bool operator!=(const Berza &b1, const Berza &b2) {
  return !std::equal(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin());
}

int main() { return 0; }
