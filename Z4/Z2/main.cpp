// TP 2023/2024: Zadaća 4, Zadatak 2
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
enum Pol { Musko, Zensko };

class GradjaninBiH {
  std::string ime_i_prezime;
  long long int jmbg;
  GradjaninBiH *prethodni;
  static GradjaninBiH *posljednji;

  static bool ValidanDatum(int dan, int mjesec, int godina);
  static int IzracunajKontrolnuCifru(long long int jmbg);
  static long long int KreirajJMBG(int dan, int mjesec, int godina,
                                   int sifra_regije, int kod);
  static bool JedinstvenJMBG(long long int jmbg);

public:
  GradjaninBiH() {}
  GradjaninBiH(const std::string &ime_i_prezime, long long int jmbg);
  GradjaninBiH(const std::string &ime_i_prezime, int dan, int mjesec,
               int godina, int sifra_regije, ::Pol pol);
  ~GradjaninBiH();
  GradjaninBiH(const GradjaninBiH &) = delete;
  GradjaninBiH &operator=(const GradjaninBiH &) = delete;
  std::string DajImeIPrezime() const { return ime_i_prezime; }
  long long int DajJMBG() const { return jmbg; }
  int DajDanRodjenja() const;
  int DajMjesecRodjenja() const;
  int DajGodinuRodjenja() const;
  int DajSifruRegije() const;
  ::Pol DajPol() const;
  void PromijeniImeIPrezime(const std::string &novo_ime) {
    if (novo_ime.empty()) {
      throw std::logic_error("Ime ne moze biti prazno!");
    }
    ime_i_prezime = novo_ime;
  }
  GradjaninBiH* DajPrethodnog() const { return prethodni; }
  void PostaviPrethodnog(GradjaninBiH* prethodni) {
    this->prethodni = prethodni;
  }
};

GradjaninBiH *GradjaninBiH::posljednji = nullptr;

bool GradjaninBiH::ValidanDatum(int dan, int mjesec, int godina) {
  int broj_dana[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (godina % 4 == 0 && (godina % 100 != 0 || godina % 400 == 0))
    broj_dana[1] = 29;
  if (mjesec < 1 || mjesec > 12)
    return false;
  if (dan < 1 || dan > broj_dana[mjesec - 1])
    return false;
  if (godina > 2021)
    return false;
  return true;
}

int GradjaninBiH::IzracunajKontrolnuCifru(long long int jmbg) {
  int cifre[13];
  for (int i = 12; i >= 0; i--) {
    cifre[i] = jmbg % 10;
    jmbg /= 10;
  }
  int kontrolna_cifra =
      11 - (7 * (cifre[0] + cifre[6]) + 6 * (cifre[1] + cifre[7]) +
            5 * (cifre[2] + cifre[8]) + 4 * (cifre[3] + cifre[9]) +
            3 * (cifre[4] + cifre[10]) + 2 * (cifre[5] + cifre[11])) %
               11;
  if (kontrolna_cifra == 11)
    kontrolna_cifra = 0;
  if (kontrolna_cifra == 10)
    throw std::logic_error("JMBG nije validan");
  return kontrolna_cifra;
}

long long int GradjaninBiH::KreirajJMBG(int dan, int mjesec, int godina,
                                        int sifra_regije, int kod) {
  long long int jmbg = dan * 1e11 + mjesec * 1e9 + (godina % 1000) * 1e6 +
                       sifra_regije * 1e4 + kod * 1e1;
  int kontrolna_cifra = IzracunajKontrolnuCifru(jmbg);
  return jmbg + kontrolna_cifra;
}

bool GradjaninBiH::JedinstvenJMBG(long long int jmbg) {
  for (GradjaninBiH *it = posljednji; it != nullptr; it = it->prethodni) {
    if (it->jmbg == jmbg)
      return false;
  }
  return true;
}

GradjaninBiH::GradjaninBiH(const std::string &ime_i_prezime, long long int jmbg)
    : ime_i_prezime(ime_i_prezime), jmbg(jmbg) {
  if (ime_i_prezime.empty())
    throw std::logic_error("Ime ne moze biti prazno!");

  int cifre[13];
  long long int temp_jmbg = jmbg;
  for (int i = 12; i >= 0; i--) {
    cifre[i] = temp_jmbg % 10;
    temp_jmbg /= 10;
  }

  int dan = cifre[0] * 10 + cifre[1];
  int mjesec = cifre[2] * 10 + cifre[3];
  int godina = cifre[4] * 100 + cifre[5] * 10 + cifre[6];
  if (godina >= 900) {
    godina += 1000;
  } else if (godina < 100) {
    godina += 2000;
  } else {
    godina += 1000;
  }

  if (!ValidanDatum(dan, mjesec, godina) ||
      cifre[12] != IzracunajKontrolnuCifru(jmbg))
    throw std::logic_error("JMBG nije validan");

  if (!JedinstvenJMBG(jmbg))
    throw std::logic_error("Vec postoji gradjanin sa istim JMBG");

  prethodni = posljednji;
  posljednji = this;
}

GradjaninBiH::GradjaninBiH(const std::string &ime_i_prezime, int dan,
                           int mjesec, int godina, int sifra_regije, ::Pol pol)
    : ime_i_prezime(ime_i_prezime) {
  if (ime_i_prezime.empty())
    throw std::logic_error("Ime ne moze biti prazno!");
  if (!ValidanDatum(dan, mjesec, godina) || sifra_regije < 0 ||
      sifra_regije >= 100)
    throw std::logic_error("Neispravni podaci");

  int kod_pocetak, kod_kraj;

  if (pol == Musko) {
    kod_pocetak = 0;
    kod_kraj = 499;
  } else {
    kod_pocetak = 500;
    kod_kraj = 999;
  }

  for (int kod = kod_pocetak; kod <= kod_kraj; kod++) {
    try {
      long long int eventualni_jmbg =
          KreirajJMBG(dan, mjesec, godina, sifra_regije, kod);
      if (JedinstvenJMBG(eventualni_jmbg)) {
        jmbg = eventualni_jmbg;
        prethodni = posljednji;
        posljednji = this;
        return;
      }
    } catch (std::logic_error &) {
      continue;
    }
  }

  throw std::logic_error("Neispravni podaci");
}

GradjaninBiH::~GradjaninBiH() {
  if (this == posljednji) {
    posljednji = prethodni;
  } else {
    for (GradjaninBiH *it = posljednji; it != nullptr; it = it->prethodni) {
      if (it->prethodni == this) {
        it->prethodni = prethodni;
        break;
      }
    }
  }
}

int GradjaninBiH::DajDanRodjenja() const {
  return (jmbg / 100000000000LL) % 100;
}

int GradjaninBiH::DajMjesecRodjenja() const {
  return (jmbg / 1000000000LL) % 100;
}

int GradjaninBiH::DajGodinuRodjenja() const {
  int godina = (jmbg / 1000000LL) % 1000;
  if (godina >= 900) {
    godina += 1000;
  } else if (godina < 100) {
    godina += 2000;
  } else {
    godina += 1000;
  }
  return godina;
}

int GradjaninBiH::DajSifruRegije() const { return (jmbg / 10000LL) % 100; }

::Pol GradjaninBiH::DajPol() const {
  int kod = (jmbg / 10LL) % 1000;
  if (kod < 500) {
    return Musko;
  } else {
    return Zensko;
  }
}
/*
void IspisiListu(GradjaninBiH* glava) {
  while (glava) {
    std::cout << glava->DajImeIPrezime() << " JMBG: " << glava->DajJMBG()
              << std::endl;
    glava = glava->DajPrethodnog();
  }
}*/

int main() {

  int broj_gradjana;
  std::cout << "Unesite broj gradjana:" << std::endl;
  std::cin >> broj_gradjana;

  GradjaninBiH **gradjani = new GradjaninBiH *[broj_gradjana];
  int broj_unesenih = 0;

  for (int i = 0; i < broj_gradjana; ++i) {
    std::string ime_i_prezime;
    int dan, mjesec, godina, sifra_regije, pol;

    bool validan = false;
    while (!validan) {
      std::cout << "Unesite podatke za " << (i + 1)
                << ". gradjanina:" << std::endl;
      std::cout << "Ime i prezime: ";
      std::cin.ignore(10000, '\n');
      std::getline(std::cin, ime_i_prezime);
      std::cout << "Datum rodjenja: ";
      std::cin >> dan >> mjesec >> godina;
      std::cout << "Sifra regije: ";
      std::cin >> sifra_regije;
      std::cout << "Pol ( 1-Musko, 2-Zensko ): ";
      std::cin >> pol;

      try {
        gradjani[broj_unesenih] =
            new GradjaninBiH(ime_i_prezime, dan, mjesec, godina, sifra_regije,
                             static_cast<Pol>(pol - 1));
        broj_unesenih++;
        validan = true;
      } catch (std::logic_error &e) {
        std::cout << "Neispravni podaci. Molimo ponovite unos." << std::endl
                  << std::endl;
      }
    }
  }

  std::sort(gradjani, gradjani + broj_unesenih,
            [](GradjaninBiH *a, GradjaninBiH *b) {
              if (a->DajGodinuRodjenja() != b->DajGodinuRodjenja())
                return a->DajGodinuRodjenja() < b->DajGodinuRodjenja();
              if (a->DajMjesecRodjenja() != b->DajMjesecRodjenja())
                return a->DajMjesecRodjenja() < b->DajMjesecRodjenja();
              return a->DajDanRodjenja() < b->DajDanRodjenja();
            });

  std::cout << "Gradjani sortirani po starosti:" << std::endl;
  for (int i = 0; i < broj_unesenih; i++) {
    std::cout << gradjani[i]->DajImeIPrezime() << " "
              << gradjani[i]->DajDanRodjenja() << "."
              << gradjani[i]->DajMjesecRodjenja() << "."
              << gradjani[i]->DajGodinuRodjenja() << " JMBG: " << std::setw(13)
              << std::setfill('0') << gradjani[i]->DajJMBG() << std::endl;
  }

  for (int i = 0; i < broj_unesenih; i++) {
    delete gradjani[i];
  }
  delete[] gradjani;
}
