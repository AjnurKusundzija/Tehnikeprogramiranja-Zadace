// TP 2023/2024: Zadaća 4, Zadatak 4
#include <algorithm>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>


class Datum {
  int dan, mjesec, godina;

  bool JeLValidan(int d, int m, int g) {
    if (m < 1 || m > 12)
      return false;
    if (d < 1)
      return false;
    if (m == 2) {
      bool prest = (g % 4 == 0 && g % 100 != 0) || (g % 400 == 0);
      if (prest)
        return d <= 29;
      else
        return d <= 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) {
      return d <= 30;
    }
    return d <= 31;
  }

public:
  Datum(int d, int m, int g) { Postavi(d, m, g); }

  void Postavi(int d, int m, int g) {
    if (!JeLValidan(d, m, g)) {
      throw std::domain_error("Neispravan datum");
    }
    dan = d;
    mjesec = m;
    godina = g;
  }

  std::tuple<int, int, int> Ocitaj() const { return {dan, mjesec, godina}; }

  void Ispisi() const { std::cout << dan << "/" << mjesec << "/" << godina; }
};

class Vrijeme {
  int sati, minute;

public:
  Vrijeme(int s, int m) { Postavi(s, m); }

  void Postavi(int s, int m) {
    if (s < 0 || s >= 24 || m < 0 || m >= 60) {
      throw std::domain_error("Neispravno vrijeme");
    }

    sati = s;
    minute = m;
  }

  std::pair<int, int> Ocitaj() const { return {sati, minute}; }

  void Ispisi() const {
    if (sati < 10) {
      std::cout << "0" << sati << ":";
    } else {
      std::cout << sati << ":";
    }
    std::cout << std::setw(2) << std::setfill('0') << minute;
  }
};

class Pregled {
  std::string ime_pacijenta;
  Datum datum;
  Vrijeme vrijeme;

public:
  Pregled(const std::string &ime, const Datum &d, const Vrijeme &v)
      : ime_pacijenta(ime), datum(d), vrijeme(v) {}

  Pregled(const std::string &ime, int dan, int mjesec, int godina, int sati,
          int minute)
      : ime_pacijenta(ime), datum(dan, mjesec, godina), vrijeme(sati, minute) {}

  Pregled &PromijeniPacijenta(const std::string &novo_ime) {
    ime_pacijenta = novo_ime;
    return *this;
  }

  Pregled &PromijeniDatum(const Datum &novi_datum) {
    datum = novi_datum;
    return *this;
  }

  Pregled &PromijeniVrijeme(const Vrijeme &novo_vrijeme) {
    vrijeme = novo_vrijeme;
    return *this;
  }

  void PomjeriDanUnaprijed() {
    int d, m, g;
    std::tie(d, m, g) = datum.Ocitaj();
    d++;
    if (d > 31) {
      d = 1;
      m++;
    }
    if (m > 12) {
      m = 1;
      g++;
    }
    datum.Postavi(d, m, g);
  }

  void PomjeriDanUnazad() {
    int d, m, g;
    std::tie(d, m, g) = datum.Ocitaj();
    d--;
    if (d < 1) {
      d = 31;
      m--;
    }
    if (m < 1) {
      m = 12;
      g--;
    }
    datum.Postavi(d, m, g);
  }

  std::string DajImePacijenta() const { return ime_pacijenta; }

  Datum DajDatumPregleda() const { return datum; }

  Vrijeme DajVrijemePregleda() const { return vrijeme; }

  static bool DolaziPrije(const Pregled &p1, const Pregled &p2) {
    int d1, m1, g1, d2, m2, g2;
    std::tie(d1, m1, g1) = p1.datum.Ocitaj();
    std::tie(d2, m2, g2) = p2.datum.Ocitaj();
    if (g1 != g2)
      return g1 < g2;
    if (m1 != m2)
      return m1 < m2;
    if (d1 != d2)
      return d1 < d2;
    int h1, min1, h2, min2;
    std::tie(h1, min1) = p1.vrijeme.Ocitaj();
    std::tie(h2, min2) = p2.vrijeme.Ocitaj();
    if (h1 != h2)
      return h1 < h2;
    return min1 < min2;
  }

  void Ispisi() const {
    std::cout << std::left << std::setw(30) << std::setfill(' ')
              << ime_pacijenta << " ";

    datum.Ispisi();
    std::cout << " ";
    vrijeme.Ispisi();
    std::cout << std::endl;
  }
};

class Pregledi {
  std::vector<std::unique_ptr<Pregled>> pregledi;

public:
  Pregledi() = default;

  Pregledi(const std::initializer_list<Pregled> &lista) {
    for (const auto &pregled : lista) {
      pregledi.push_back(std::make_unique<Pregled>(pregled));
    }
  }

  Pregledi(const Pregledi &p) {
    for (const auto &pregled : p.pregledi) {
      pregledi.push_back(std::make_unique<Pregled>(*pregled));
    }
  }

  Pregledi(Pregledi &&p) noexcept = default;

  Pregledi &operator=(const Pregledi &p) {
    if (this != &p) {
      pregledi.clear();
      for (const auto &pregled : p.pregledi) {
        pregledi.push_back(std::make_unique<Pregled>(*pregled));
      }
    }
    return *this;
  }

  Pregledi &operator=(Pregledi &&p) noexcept {
    if (this != &p) {
      pregledi = std::move(p.pregledi);
    }
    return *this;
  }

  void RegistrirajPregled(const std::string &ime, const Datum &d,
                          const Vrijeme &v) {
    pregledi.push_back(std::make_unique<Pregled>(ime, d, v));
  }

  void RegistrirajPregled(const std::string &ime, int dan, int mjesec,
                          int godina, int sati, int minute) {
    pregledi.push_back(
        std::make_unique<Pregled>(ime, dan, mjesec, godina, sati, minute));
  }

  void RegistrirajPregled(std::unique_ptr<Pregled> pregled) {
    pregledi.push_back(std::move(pregled));
  }

  void RegistrirajPregled(std::shared_ptr<Pregled> pregled) {
    pregledi.push_back(std::make_unique<Pregled>(*pregled));
  }

  int DajBrojPregleda() const { return pregledi.size(); }

  int DajBrojPregledaNaDatum(const Datum &d) const {
    return std::count_if(pregledi.begin(), pregledi.end(),
                         [&](const std::unique_ptr<Pregled> &pregled) {
                           return pregled->DajDatumPregleda().Ocitaj() ==
                                  d.Ocitaj();
                         });
  }

  Pregled &DajNajranijiPregled() {
    if (pregledi.empty())
      throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(pregledi.begin(), pregledi.end(),
                              [](const std::unique_ptr<Pregled> &p1,
                                 const std::unique_ptr<Pregled> &p2) {
                                return Pregled::DolaziPrije(*p1, *p2);
                              });
  }

  Pregled DajNajranijiPregled() const {
    if (pregledi.empty())
      throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(pregledi.begin(), pregledi.end(),
                              [](const std::unique_ptr<Pregled> &p1,
                                 const std::unique_ptr<Pregled> &p2) {
                                return Pregled::DolaziPrije(*p1, *p2);
                              });
  }

  void IsprazniKolekciju() { pregledi.clear(); }

  void ObrisiNajranijiPregled() {
    if (pregledi.empty())
      throw std::range_error("Prazna kolekcija");
    auto najraniji = std::min_element(pregledi.begin(), pregledi.end(),
                                      [](const std::unique_ptr<Pregled> &p1,
                                         const std::unique_ptr<Pregled> &p2) {
                                        return Pregled::DolaziPrije(*p1, *p2);
                                      });
    pregledi.erase(najraniji);
  }

  void ObrisiPregledePacijenta(const std::string &ime) {
    pregledi.erase(std::remove_if(pregledi.begin(), pregledi.end(),
                                  [&](const std::unique_ptr<Pregled> &pregled) {
                                    return pregled->DajImePacijenta() == ime;
                                  }),
                   pregledi.end());
  }

  void IspisiPregledeNaDatum(const Datum &d) const {
    std::vector<Pregled *> pregledi_na_datum;
    for (const auto &pregled : pregledi) {
      if (pregled->DajDatumPregleda().Ocitaj() == d.Ocitaj()) {
        pregledi_na_datum.push_back(pregled.get());
      }
    }
    std::sort(pregledi_na_datum.begin(), pregledi_na_datum.end(),
              [](Pregled *p1, Pregled *p2) {
                return Pregled::DolaziPrije(*p1, *p2);
              });
    for (auto pregled : pregledi_na_datum) {
      pregled->Ispisi();
    }
  }

  void IspisiSvePreglede() const {
    std::vector<Pregled *> svi_pregledi;
    for (const auto &pregled : pregledi) {
      svi_pregledi.push_back(pregled.get());
    }
    std::sort(svi_pregledi.begin(), svi_pregledi.end(),
              [](Pregled *p1, Pregled *p2) {
                return Pregled::DolaziPrije(*p1, *p2);
              });
    for (auto pregled : svi_pregledi) {
      pregled->Ispisi();
    }
  }
};

int main() {
  try {
    Pregledi pregledi;

    while (true) {
      std::cout << "1. Registriraj Pregled\n";
      std::cout << "2. Daj Broj Pregleda\n";
      std::cout << "3. Daj Broj Pregleda Na Datum\n";
      std::cout << "4. Daj Najraniji Pregled\n";
      std::cout << "5. Obrisi Najraniji Pregled\n";
      std::cout << "6. Obrisi Preglede Pacijenta\n";
      std::cout << "7. Ispisi Preglede Na Datum\n";
      std::cout << "8. Ispisi Sve Preglede\n";
      std::cout << "9. Isprazni Kolekciju\n";
      std::cout << "10. Izlaz\n";
      std::cout << "Unesite izbor: ";
      int izbor;
      std::cin >> izbor;
      std::cin.ignore(10000, '\n');
      if (izbor == 10) {
        std::cout << "Kraj programa.\n";
        break;
      }
      switch (izbor) {
      case 1: {
        std::string ime_pacijenta;
        int dan, mjesec, godina, sati, minute;
        std::cout << "Unesite ime pacijenta: ";
        std::getline(std::cin, ime_pacijenta);
        std::cout << "Unesite datum i vrijeme pregleda (dan mjesec godina sati "
                     "minute): ";
        std::cin >> dan >> mjesec >> godina >> sati >> minute;
        std::cin.ignore(10000, '\n');
        try {
          pregledi.RegistrirajPregled(ime_pacijenta, dan, mjesec, godina, sati,
                                      minute);
        } catch (const std::range_error &e) {
          std::cout << e.what() << std::endl;
        }
        break;
      }
      case 2:
        std::cout << "Ukupan broj pregleda: " << pregledi.DajBrojPregleda()
                  << std::endl;
        break;
      case 3: {
        int dan, mjesec, godina;
        std::cout << "Unesite datum (dan mjesec godina): ";
        std::cin >> dan >> mjesec >> godina;
        std::cin.ignore(10000, '\n');
        std::cout << "Broj pregleda na datum: "
                  << pregledi.DajBrojPregledaNaDatum(Datum(dan, mjesec, godina))
                  << std::endl;
        break;
      }
      case 4: {
        try {
          std::cout << "Najraniji pregled: ";
          pregledi.DajNajranijiPregled().Ispisi();
        } catch (const std::domain_error &e) {
          std::cout << e.what() << std::endl;
        }
        break;
      }
      case 5: {
        try {
          pregledi.ObrisiNajranijiPregled();
          std::cout << "Najraniji pregled je obrisan.\n";
        } catch (const std::range_error &e) {
          std::cout << e.what() << std::endl;
        }
        break;
      }
      case 6: {
        std::string ime_pacijenta;
        std::cout << "Unesite ime pacijenta: ";
        std::getline(std::cin, ime_pacijenta);
        pregledi.ObrisiPregledePacijenta(ime_pacijenta);
        std::cout << "Pregledi za pacijenta " << ime_pacijenta
                  << " su obrisani.\n";
        break;
      }
      case 7: {
        int dan, mjesec, godina;
        std::cout << "Unesite datum: ";
        std::cin >> dan >> mjesec >> godina;
        std::cin.ignore(10000, '\n');
        pregledi.IspisiPregledeNaDatum(Datum(dan, mjesec, godina));
        break;
      }
      case 8:
        std::cout << "Svi pregledi:\n";
        pregledi.IspisiSvePreglede();
        break;
      case 9:
        pregledi.IsprazniKolekciju();
        std::cout << "Kolekcija ispraznjena.\n";
        break;
      default:
        std::cout << "Nepoznata opcija. Pokusajte ponovo.\n";
      }
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
