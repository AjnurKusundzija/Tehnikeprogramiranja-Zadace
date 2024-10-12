// TP 2023/2024: Zadaća 5, Zadatak 3
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Spremnik {
protected:
  double tezina;
  std::string naziv_sadrzaja;

public:
  Spremnik(double tezina, const std::string &naziv)
      : tezina(tezina), naziv_sadrzaja(naziv) {}
  virtual ~Spremnik(){};
  double DajTezinu() const { return tezina; }
  const std::string &DajNaziv() const { return naziv_sadrzaja; }
  virtual double DajUkupnuTezinu() const = 0;
  virtual void Ispisi() const = 0;
  virtual Spremnik *DajKopiju() const = 0;
};

class Sanduk : public Spremnik {
  std::vector<double> tezine_predmeta;

public:
  Sanduk(double tezina, const std::string &naziv,
         const std::vector<double> &tezine)
      : Spremnik(tezina, naziv), tezine_predmeta(tezine) {}
  double DajUkupnuTezinu() const override {
    double ukupna_tezina = tezina;
    for (double t : tezine_predmeta)
      ukupna_tezina += t;
    return ukupna_tezina;
  }
  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Sanduk\n";
    std::cout << "Sadrzaj: " << naziv_sadrzaja << "\n";
    std::cout << "Tezine predmeta: ";
    for (double t : tezine_predmeta)
      std::cout << t << " ";
    std::cout << "(kg)\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }
  Spremnik *DajKopiju() const override { return new Sanduk(*this); }
};

class Vreca : public Spremnik {
  double tezina_materije;

public:
  Vreca(double tezina, const std::string &naziv, double tezina_materije)
      : Spremnik(tezina, naziv), tezina_materije(tezina_materije) {}
  double DajUkupnuTezinu() const override { return tezina + tezina_materije; }
  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Vreca\n";
    std::cout << "Sadrzaj: " << naziv_sadrzaja << "\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Tezina pohranjene materije: " << tezina_materije << " (kg)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }
  Spremnik *DajKopiju() const override { return new Vreca(*this); }
};

class Bure : public Spremnik {
  double specificna_tezina, zapremina;

public:
  Bure(double tezina, const std::string &naziv, double spec_tezina,
       double zapremina)
      : Spremnik(tezina, naziv), specificna_tezina(spec_tezina),
        zapremina(zapremina) {}
  double DajUkupnuTezinu() const override {
    return tezina + (specificna_tezina * (zapremina / 1000));
  }
  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Bure\n";
    std::cout << "Sadrzaj: " << naziv_sadrzaja << "\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Specificna tezina tecnosti: " << specificna_tezina
              << " (kg/m^3)\n";
    std::cout << "Zapremina tecnosti: " << zapremina << " (l)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }
  Spremnik *DajKopiju() const override { return new Bure(*this); }
};

class Skladiste {
  std::vector<std::shared_ptr<Spremnik>> spremnici;

public:
  Skladiste(){};
  Skladiste(const Skladiste &s) {
    for (int i = 0; i < s.spremnici.size(); i++) {
      spremnici.push_back(
          std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
    }
  }
  Skladiste &operator=(const Skladiste &s) {
    if (&s == this)
      return *this;
    spremnici.clear();
    for (int i = 0; i < s.spremnici.size(); i++) {
      spremnici.push_back(
          std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
    }
    return *this;
  }
  Spremnik *DodajSanduk(double tezina, const std::string &naziv,
                        const std::vector<double> &tezine) {
    auto sanduk = std::make_shared<Sanduk>(tezina, naziv, tezine);
    spremnici.push_back(sanduk);
    return sanduk.get();
  }
  Spremnik *DodajVrecu(double tezina, const std::string &naziv,
                       double tezina_materije) {
    auto vreca = std::make_shared<Vreca>(tezina, naziv, tezina_materije);
    spremnici.push_back(vreca);
    return vreca.get();
  }
  Spremnik *DodajBure(double tezina, const std::string &naziv,
                      double spec_tezina, double zapremina) {
    auto bure = std::make_shared<Bure>(tezina, naziv, spec_tezina, zapremina);
    spremnici.push_back(bure);
    return bure.get();
  }
  Spremnik *DodajSpremnik(Spremnik *spremnik, bool pripadnost) {
    if (pripadnost) {
      spremnici.push_back(std::shared_ptr<Spremnik>(spremnik));
      return spremnik;
    } else {
      auto kopija = std::shared_ptr<Spremnik>(spremnik->DajKopiju());
      spremnici.push_back(kopija);
      return kopija.get();
    }
  }
  void BrisiSpremnik(Spremnik *spremnik) {
    for (auto it = spremnici.begin(); it != spremnici.end(); it++) {
      if (it->get() == spremnik) {
        spremnici.erase(it);
        break;
      }
    }
  }
  Spremnik &DajNajlaksi() {
    if (spremnici.empty())
      throw std::range_error("Skladiste je prazno");
    auto najlaksi = spremnici[0].get();
    for (int i = 1; i < spremnici.size(); i++) {
      if (spremnici[i]->DajTezinu() < najlaksi->DajTezinu())
        najlaksi = spremnici[i].get();
    }
    return *najlaksi;
  }
  Spremnik &DajNajtezi() {
    if (spremnici.empty())
      throw std::range_error("Skladiste je prazno");
    auto najtezi = spremnici[0].get();
    for (int i = 1; i < spremnici.size(); i++) {
      if (spremnici[i]->DajTezinu() > najtezi->DajTezinu())
        najtezi = spremnici[i].get();
    }
    return *najtezi;
  }
  int BrojPreteskih(int max_tezina) const {
    int broj = 0;
    for (int i = 0; i < spremnici.size(); i++) {
      if (spremnici[i]->DajUkupnuTezinu() > max_tezina)
        broj++;
    }
    return broj;
  }
  void IzlistajSkladiste() const {
    std::vector<std::shared_ptr<Spremnik>> kopija(spremnici);
    for (int i = 0; i < kopija.size(); i++) {
      for (int j = i + 1; j < kopija.size(); j++) {
        if (kopija[i]->DajUkupnuTezinu() < kopija[j]->DajUkupnuTezinu()) {
          std::swap(kopija[i], kopija[j]);
        }
      }
    }
    for (int i = 0; i < kopija.size(); i++) {
      kopija[i]->Ispisi();
      std::cout <<std::endl;
    }
  }
  void UcitajIzDatoteke(const std::string &ime_datoteke) {
    std::ifstream ulaz(ime_datoteke);
    if (!ulaz)
      throw std::logic_error("Trazena datoteka ne postoji");

    spremnici.clear();
    std::string unos;
    while (std::getline(ulaz, unos)) {
      if (unos.empty())
        continue;

      char tip = unos[0];
      std::string naziv = unos.substr(2);

      if (tip == 'S') {
        if (!std::getline(ulaz, unos))
          throw std::logic_error("Problemi pri citanju datoteke");
        std::istringstream ss(unos);
        double tezina;
        int broj_predmeta;
        if (!(ss >> tezina >> broj_predmeta) && broj_predmeta < 0)
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        std::vector<double> tezine(broj_predmeta);
        for (int i = 0; i < broj_predmeta; i++) {
          if (!(ss >> tezine[i]))
            throw std::logic_error("Datoteka sadrzi besmislene podatke");
        }
        if (!ss.eof())
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        DodajSanduk(tezina, naziv, tezine);
      } else if (tip == 'V') {
        if (!std::getline(ulaz, unos))
          throw std::logic_error("Problemi pri citanju datoteke");
        double tezina, tezina_materije;
        std::istringstream ss(unos);
        if (!(ss >> tezina >> tezina_materije))
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        if ( !ss.eof())
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        DodajVrecu(tezina, naziv, tezina_materije);
      } else if (tip == 'B') {
        if (!std::getline(ulaz, unos))
          throw std::logic_error("Problemi pri citanju datoteke");
        double tezina, spec_tezina, zapremina;
        std::istringstream ss(unos);
        if (!(ss >> tezina >> spec_tezina >> zapremina))
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        if (ss.fail() && !ss.eof())
          throw std::logic_error("Datoteka sadrzi besmislene podatke");
        DodajBure(tezina, naziv, spec_tezina, zapremina);
      } else {
        throw std::logic_error("Datoteka sadrzi besmislene podatke");
      }
    }
    if (ulaz.bad())
      throw std::logic_error("Problemi pri citanju datoteke");
  }
};

int main() {
  Skladiste s;
  try {
    s.UcitajIzDatoteke("ROBA.TXT");
    s.IzlistajSkladiste();

  } catch (std::exception &k) {
    std::cout << k.what() << "!" << std::endl;
  }
}
