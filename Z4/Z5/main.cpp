// TP 2023/2024: Zadaća 4, Zadatak 5
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>

bool JeLiCifra(char c) { return c >= '0' && c <= '9'; }

bool JeLiRazmak(char c) { return c == ' ' || c == '\t' || c == '\n'; }

class Student {
  int broj_indeksa;
  std::string godina_studija, ime_prezime, adresa, telefon;
  void UkloniViskaRazmak(std::string &s) {
    int pocetak = 0, kraj = s.size() - 1;
    while (pocetak < s.size() && JeLiRazmak(s[pocetak]))
      pocetak++;
    while (kraj > pocetak && JeLiRazmak(s[kraj]))
      kraj--;

    bool razmak = false;
    std::string finalni;
    for (int i = pocetak; i <= kraj; i++) {
      if (JeLiRazmak(s[i])) {
        if (!razmak) {
          finalni.push_back(' ');
          razmak = true;
        }
      } else {
        finalni.push_back(s[i]);
        razmak = false;
      }
    }
    s = finalni;
  }

  bool JeLiValidanTelefon(const std::string &telefon) {
    int poz_1 = -1, poz_2 = -1;

    for (int i = 0; i < telefon.size(); i++) {
      if (telefon[i] == '/') {
        poz_1 = i;
      } else if (telefon[i] == '-') {
        poz_2 = i;
      }
    }

    if (poz_1 == -1 || poz_2 == -1 || poz_1 > poz_2) {
      return false;
    }

    for (int i = 0; i < telefon.size(); i++) {
      if (i != poz_1 && i != poz_2 && !JeLiCifra(telefon[i]) &&
          !JeLiRazmak(telefon[i])) {
        return false;
      }
    }

    return true;
  }

public:
  Student(int broj_indeksa, std::string godina_studija, std::string ime_prezime,
          std::string adresa, std::string telefon) {
    if (broj_indeksa < 10000 || broj_indeksa > 99999)
      throw std::domain_error("Neispravni parametri");

    std::string ispravne_godine[] = {"1",   "2",   "3",   "1/B", "2/B", "3/B",
                                     "1/M", "2/M", "1/D", "2/D", "3/D"};
    bool ispravna = false;
    for (const std::string &g : ispravne_godine) {
      if (godina_studija == g) {
        ispravna = true;
        break;
      }
    }
    if (!ispravna)
      throw std::domain_error("Neispravni parametri");

    if (!JeLiValidanTelefon(telefon))
      throw std::domain_error("Neispravni parametri");

    UkloniViskaRazmak(ime_prezime);
    UkloniViskaRazmak(adresa);

    this->broj_indeksa = broj_indeksa;
    this->godina_studija = godina_studija;
    this->ime_prezime = ime_prezime;
    this->adresa = adresa;
    this->telefon = telefon;
  }

  int DajIndeks() const { return broj_indeksa; }
  std::string DajGodinuStudija() const {
    if (godina_studija == "1")
      return "1/B";
    if (godina_studija == "2")
      return "2/B";
    if (godina_studija == "3")
      return "3/B";
    return godina_studija;
  }
  std::string DajImePrezime() const { return ime_prezime; }
  std::string DajAdresu() const { return adresa; }
  std::string DajTelefon() const { return telefon; }

  void Ispisi() const {
    std::cout << "Broj indeksa: " << broj_indeksa << std::endl;
    std::cout << "Godina studija: " << DajGodinuStudija() << std::endl;
    std::cout << "Ime i prezime: " << ime_prezime << std::endl;
    std::cout << "Adresa: " << adresa << std::endl;
    std::cout << "Telefon: " << telefon << std::endl;
  }
};

class Laptop {
  int evidencijski;
  std::string naziv, karakteristike;
  Student *zaduzio_ga;

public:
  Laptop(int evidencijski, std::string naziv, std::string karakteristike)
      : evidencijski(evidencijski), naziv(naziv),
        karakteristike(karakteristike), zaduzio_ga(nullptr) {
    if (evidencijski < 0)
      throw std::domain_error("Neispravni parametri");
  }

  int DajEvidencijskiBroj() const { return evidencijski; }
  std::string DajNaziv() const { return naziv; }
  std::string DajKarakteristike() const { return karakteristike; }

  void Zaduzi(Student &student) {
    if (zaduzio_ga)
      throw std::domain_error("Laptop vec zaduzen");
    zaduzio_ga = &student;
  }

  void Razduzi() {
    if (!zaduzio_ga)
      return;
    zaduzio_ga = nullptr;
  }
  bool DaLiJeZaduzen() const { return zaduzio_ga != nullptr; }

  Student &DajKodKogaJe() const {
    if (!zaduzio_ga)
      throw std::domain_error("Laptop nije zaduzen");
    return *zaduzio_ga;
  }

  Student *DajPokKodKogaJe() const { return zaduzio_ga; }

  void Ispisi() const {
    std::cout << "Evidencijski broj: " << evidencijski << std::endl;
    std::cout << "Naziv: " << naziv << std::endl;
    std::cout << "Karakteristike: " << karakteristike << std::endl;
  }
};

class Administracija {
  std::map<int, Student *> studenti;
  std::map<int, Laptop *> laptopi;

  void Kopiraj(const Administracija &a) {
    for (const auto &p : a.studenti)
      studenti[p.first] = new Student(*p.second);
    for (const auto &p : a.laptopi)
      laptopi[p.first] = new Laptop(*p.second);
  }

  void Brisi() {
    for (const auto &p : studenti)
      delete p.second;
    for (const auto &p : laptopi)
      delete p.second;
  }

public:
  Administracija() {}

  Administracija(const Administracija &a) { Kopiraj(a); }

  Administracija(Administracija &&a) noexcept {
    studenti = std::move(a.studenti);
    laptopi = std::move(a.laptopi);
  }

  Administracija &operator=(const Administracija &a) {
    if (this == &a)
      return *this;
    Brisi();
    Kopiraj(a);
    return *this;
  }

  Administracija &operator=(Administracija &&a) noexcept {
    if (this == &a)
      return *this;
    Brisi();
    studenti = std::move(a.studenti);
    laptopi = std::move(a.laptopi);
    return *this;
  }

  ~Administracija() { Brisi(); }

  void RegistrirajNovogStudenta(int broj_indeksa, std::string godina_studija,
                                std::string ime_prezime, std::string adresa,
                                std::string telefon) {
    if (studenti.count(broj_indeksa))
      throw std::domain_error("Student s tim indeksom vec postoji");
    studenti[broj_indeksa] =
        new Student(broj_indeksa, godina_studija, ime_prezime, adresa, telefon);
  }
  void RegistrirajNoviLaptop(int evidencijski, const std::string &naziv,
                             const std::string &karakteristike) {
    if (laptopi.count(evidencijski))
      throw std::domain_error("Laptop s tim evidencijskim brojem vec postoji");
    laptopi[evidencijski] = new Laptop(evidencijski, naziv, karakteristike);
  }

  Student &NadjiStudenta(int broj_indeksa) {
    if (!studenti.count(broj_indeksa))
      throw std::domain_error("Student nije nadjen");
    return *studenti[broj_indeksa];
  }

  Laptop &NadjiLaptop(int evidencijski) {
    if (!laptopi.count(evidencijski))
      throw std::domain_error("Laptop nije nadjen");
    return *laptopi[evidencijski];
  }

  void IzlistajStudente() const {
    for (const auto &p : studenti) {
      p.second->Ispisi();
      std::cout << std::endl;
    }
  }

  void IzlistajLaptope() const {
    for (const auto &p : laptopi) {
      p.second->Ispisi();
      if (p.second->DaLiJeZaduzen()) {
        std::cout << "Zaduzio(la): " << p.second->DajKodKogaJe().DajImePrezime()
                  << " (" << p.second->DajKodKogaJe().DajIndeks() << ")"
                  << std::endl
                  << std::endl;
      }
    }
  }

  void ZaduziLaptop(int broj_indeksa, int evidencijski) {
    if (!studenti.count(broj_indeksa))
      throw std::domain_error("Student nije nadjen");
    if (!laptopi.count(evidencijski))
      throw std::domain_error("Laptop nije nadjen");

    auto &student = *studenti[broj_indeksa];
    for (const auto &p : laptopi) {
      if (p.second->DaLiJeZaduzen() && &p.second->DajKodKogaJe() == &student)
        throw std::domain_error("Student je vec zaduzio laptop");
    }

    laptopi[evidencijski]->Zaduzi(student);
  }

  int NadjiSlobodniLaptop() const {
    for (const auto &p : laptopi) {
      if (!p.second->DaLiJeZaduzen())
        return p.first;
    }
    throw std::domain_error("Nema slobodnih laptopa");
  }

  void RazduziLaptop(int evidencijski) {
    if (!laptopi.count(evidencijski))
      throw std::domain_error("Laptop nije nadjen");
    if (!laptopi[evidencijski]->DaLiJeZaduzen())
      throw std::domain_error("Laptop nije zaduzen");
    laptopi[evidencijski]->Razduzi();
  }

  void PrikaziZaduzenja() const {
    bool ima_zaduzenja = false;
    for (const auto &p : laptopi) {
      if (p.second->DaLiJeZaduzen()) {
        std::cout << "Student " << p.second->DajKodKogaJe().DajImePrezime()
                  << " (" << p.second->DajKodKogaJe().DajIndeks()
                  << ") zaduzio/la laptop broj " << p.first << std::endl;
        ima_zaduzenja = true;
      }
    }
    if (!ima_zaduzenja)
      std::cout << "Nema zaduzenja" << std::endl;
  }
};

void InterakcijaSKorisnikom() {
  std::cout << "Odaberite jednu od sljedecih opcija: " << std::endl;
  std::cout << "1 - RegistrirajNovogStudenta" << std::endl;
  std::cout << "2 - RegistrirajNoviLaptop" << std::endl;
  std::cout << "3 - IzlistajStudente" << std::endl;
  std::cout << "4 - IzlistajLaptope" << std::endl;
  std::cout << "5 - NadjiSlobodniLaptop" << std::endl;
  std::cout << "6 - ZaduziLaptop" << std::endl;
  std::cout << "7 - RazduziLaptop" << std::endl;
  std::cout << "8 - PrikaziZaduzenja" << std::endl;
  std::cout << "K - Kraj programa" << std::endl;
}

int main() {
  Administracija admin;
  char opcija;

  do {
    InterakcijaSKorisnikom();
    std::cin >> opcija;

    try {
      switch (opcija) {
      case '1': {
        int broj_indeksa;
        std::string godina_studija, ime_prezime, adresa, telefon;
        std::cout << "Unesite broj indeksa: " << std::endl;
        std::cin >> broj_indeksa;
        std::cout
            << "Unesite godinu studija (formata A/X, gdje je A godina studija, "
               "a X prima vrijednosti B,M,D, zavisno od studija): "
            << std::endl;
        std::cin >> godina_studija;
        std::cin.ignore(10000, '\n');
        std::cout << "Unesite ime i prezime studenta: " << std::endl;
        std::getline(std::cin, ime_prezime);
        std::cout << "Unesite adresu studenta: " << std::endl;
        std::getline(std::cin, adresa);
        std::cout << "Unesite broj telefona (formata x/x-x): " << std::endl;
        std::getline(std::cin, telefon);
        admin.RegistrirajNovogStudenta(broj_indeksa, godina_studija,
                                       ime_prezime, adresa, telefon);
        break;
      }
      case '2': {
        int evidencijski;
        std::string naziv, karakteristike;
        std::cout << "Unesite evidencijski broj laptopa " << std::endl;
        std::cin >> evidencijski;
        std::cin.ignore(10000, '\n');
        std::cout << "Unesite naziv laptopa " << std::endl;
        std::getline(std::cin, naziv);
        std::cout << "Unesite karakteristike laptopa: ";
        std::getline(std::cin, karakteristike);
        admin.RegistrirajNoviLaptop(evidencijski, naziv, karakteristike);
        std::cout << std::endl << "Laptop uspjesno registrovan!" << std::endl;
        break;
      }
      case '3': {
        admin.IzlistajStudente();
        break;
      }
      case '4': {
        admin.IzlistajLaptope();
        break;
      }
      case '5': {
        int slobodni_laptop = admin.NadjiSlobodniLaptop();
        std::cout << "Prvi slobodni laptop ima evidencijski broj: "
                  << slobodni_laptop << std::endl;
        break;
      }
      case '6': {
        int broj_indeksa, evidencijski;
        std::cout << "Unesite broj indeksa studenta: ";
        std::cin >> broj_indeksa;
        std::cout << "Unesite evidencijski broj laptopa: ";
        std::cin >> evidencijski;
        admin.ZaduziLaptop(broj_indeksa, evidencijski);
        break;
      }
      case '7': {
        int evidencijski;
        std::cout << "Unesite evidencijski broj laptopa: ";
        std::cin >> evidencijski;
        admin.RazduziLaptop(evidencijski);
        break;
      }
      case '8': {
        admin.PrikaziZaduzenja();
        break;
      }
      case 'K': {
        break;
      }
      default: {
        std::cout << "Nepoznata opcija!" << std::endl;
        break;
      }
      }
    } catch (const std::exception &e) {
      std::cout << "Izuzetak: " << e.what() << "!" << std::endl;
    }

  } while (opcija != 'K');

  return 0;
}
