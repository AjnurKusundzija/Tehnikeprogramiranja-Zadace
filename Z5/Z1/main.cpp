// TP 2023/2024: Zadaća 5, Zadatak 1
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

class Razlomak {

  long long int brojnik;
  long long int nazivnik;

  static long long int NZD(long long int br, long long int naz) {
    while (naz != 0) {
      long long int temp = naz;
      naz = br % naz;
      br = temp;
    }
    return br;
  }

  void skrati() {
    long long int nzd = NZD(brojnik, nazivnik);
    brojnik /= nzd;
    nazivnik /= nzd;
    if (nazivnik < 0) {
      brojnik = -brojnik;
      nazivnik = -nazivnik;
    }
  }

  static void ProvjeriOpseg(long long int x, long long int y) {
    if (x > 0 && y > 0 && x > std::numeric_limits<long long int>::max() / y) {
      throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }
    if (x > 0 && y < 0 && y < std::numeric_limits<long long int>::min() / x) {
      throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }
    if (x < 0 && y > 0 && x < std::numeric_limits<long long int>::min() / y) {
      throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }
    if (x < 0 && y < 0 && x < std::numeric_limits<long long int>::max() / y) {
      throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }
  }

public:
  Razlomak(long long int br, long long int naz) {
    if (naz == 0) {
      throw std::invalid_argument("Nekorektan razlomak");
    }
    brojnik = br;
    nazivnik = naz;
    skrati();
  }

  Razlomak(long long int br) : Razlomak(br, 1) {}

  Razlomak() : Razlomak(0, 1) {}

  long long int DajBrojnik() const { return brojnik; }
  long long int DajNazivnik() const { return nazivnik; }

  Razlomak operator+(const Razlomak &other) const {
    long long int novi_brojnik =
        brojnik * other.nazivnik + other.brojnik * nazivnik;
    long long int novi_nazivnik = nazivnik * other.nazivnik;
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(other.brojnik, nazivnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    ProvjeriOpseg(novi_brojnik, 1);
    ProvjeriOpseg(novi_nazivnik, 1);
    return Razlomak(novi_brojnik, novi_nazivnik);
  }

  operator long double() const {
    return static_cast<long double>(brojnik) /
           static_cast<long double>(nazivnik);
  }

  Razlomak operator-(const Razlomak &other) const {
    long long int novi_brojnik =
        brojnik * other.nazivnik - other.brojnik * nazivnik;
    long long int novi_nazivnik = nazivnik * other.nazivnik;
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(other.brojnik, nazivnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    ProvjeriOpseg(novi_brojnik, 1);
    ProvjeriOpseg(novi_nazivnik, 1);
    return Razlomak(novi_brojnik, novi_nazivnik);
  }

  Razlomak operator*(const Razlomak &other) const {
    ProvjeriOpseg(brojnik, other.brojnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    return Razlomak(brojnik * other.brojnik, nazivnik * other.nazivnik);
  }

  Razlomak operator/(const Razlomak &other) const {
    if (other.brojnik == 0) {
      throw std::invalid_argument("Dijeljenje s nulom");
    }
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(nazivnik, other.brojnik);
    return Razlomak(brojnik * other.nazivnik, nazivnik * other.brojnik);
  }

  Razlomak &operator*=(const Razlomak &other) {
    ProvjeriOpseg(brojnik, other.brojnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    brojnik *= other.brojnik;
    nazivnik *= other.nazivnik;
    skrati();
    return *this;
  }

  Razlomak &operator+=(const Razlomak &other) {
    long long int novi_brojnik =
        brojnik * other.nazivnik + other.brojnik * nazivnik;
    long long int novi_nazivnik = nazivnik * other.nazivnik;
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(other.brojnik, nazivnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    ProvjeriOpseg(novi_brojnik, 1);
    ProvjeriOpseg(novi_nazivnik, 1);
    brojnik = novi_brojnik;
    nazivnik = novi_nazivnik;
    skrati();
    return *this;
  }

  Razlomak &operator-=(const Razlomak &other) {
    long long int novi_brojnik =
        brojnik * other.nazivnik - other.brojnik * nazivnik;
    long long int novi_nazivnik = nazivnik * other.nazivnik;
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(other.brojnik, nazivnik);
    ProvjeriOpseg(nazivnik, other.nazivnik);
    ProvjeriOpseg(novi_brojnik, 1);
    ProvjeriOpseg(novi_nazivnik, 1);
    brojnik = novi_brojnik;
    nazivnik = novi_nazivnik;
    skrati();
    return *this;
  }

  Razlomak &operator/=(const Razlomak &other) {
    if (other.brojnik == 0) {
      throw std::invalid_argument("Dijeljenje s nulom");
    }
    ProvjeriOpseg(brojnik, other.nazivnik);
    ProvjeriOpseg(nazivnik, other.brojnik);
    brojnik *= other.nazivnik;
    nazivnik *= other.brojnik;
    skrati();
    return *this;
  }

  bool operator==(const Razlomak &other) const {
    return brojnik == other.brojnik && nazivnik == other.nazivnik;
  }

  bool operator!=(const Razlomak &other) const { return !(*this == other); }

  bool operator<(const Razlomak &other) const {
    return brojnik * other.nazivnik < other.brojnik * nazivnik;
  }

  bool operator<=(const Razlomak &other) const {
    return *this < other || *this == other;
  }

  bool operator>(const Razlomak &other) const { return !(*this <= other); }

  bool operator>=(const Razlomak &other) const { return !(*this < other); }

  Razlomak operator+() const { return *this; }

  Razlomak operator-() const { return Razlomak(-brojnik, nazivnik); }

  Razlomak &operator++() {
    ProvjeriOpseg(brojnik, nazivnik);
    brojnik += nazivnik;
    skrati();
    return *this;
  }

  Razlomak operator++(int) {
    ProvjeriOpseg(brojnik, nazivnik);
    Razlomak temp = *this;
    brojnik += nazivnik;
    skrati();
    return temp;
  }

  Razlomak &operator--() {
    ProvjeriOpseg(brojnik, nazivnik);
    brojnik -= nazivnik;
    skrati();
    return *this;
  }

  Razlomak operator--(int) {
    ProvjeriOpseg(brojnik, nazivnik);
    Razlomak temp = *this;
    brojnik -= nazivnik;
    skrati();
    return temp;
  }

  friend std::ostream &operator<<(std::ostream &cout, const Razlomak &r) {
    cout << r.brojnik;
    if (r.nazivnik != 1) {
      cout << '/' << r.nazivnik;
    }
    return cout;
  }

  friend std::istream &operator>>(std::istream &cin, Razlomak &r) {
    long long int br, naz = 1;
    char c;
    cin >> br;
    if (cin.peek() == EOF || std::isspace(cin.peek())) {
      r.brojnik = br;
      r.nazivnik = 1;
    } else if (cin.peek() == '/') {
      cin >> c;
      if (cin.peek() == '\n') {
        cin.setstate(std::ios::failbit);
        return cin;
      }
      cin >> naz;
      if (cin.peek() != EOF && !std::isspace(cin.peek())) {
        cin.setstate(std::ios::failbit);
        return cin;
      }
      long long int nzd = r.NZD(br, naz);
      r.brojnik = br / nzd;
      r.nazivnik = naz / nzd;
    } else {
      cin.setstate(std::ios::failbit);
    }
    return cin;
  }
};

int main() { return 0; }
