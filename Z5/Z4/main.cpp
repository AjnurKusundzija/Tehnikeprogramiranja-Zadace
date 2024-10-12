#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Spremnik {
protected:
  double tezina;
  std::string naziv;

public:
  Spremnik(double t, std::string s) : tezina(t), naziv(s) {}
  double DajTezinu() const { return tezina; }
  virtual double DajUkupnuTezinu() const = 0;
  virtual void Ispisi() const = 0;
  virtual ~Spremnik() {}
  virtual Spremnik *DajKopiju() const = 0;
};

class Sanduk : public Spremnik {
  std::vector<double> tezine_predmeta;

public:
  Sanduk(double tezina, std::string naziv, std::vector<double> tezine_predmeta)
      : Spremnik(tezina, naziv), tezine_predmeta(tezine_predmeta) {}

  double DajUkupnuTezinu() const override {
    double ukupna_tezina = tezina;
    for (double t : tezine_predmeta)
      ukupna_tezina += t;
    return ukupna_tezina;
  }

  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Sanduk\n";
    std::cout << "Sadrzaj: " << naziv << "\n";
    std::cout << "Tezine predmeta: ";
    for (double t : tezine_predmeta)
      std::cout << t << " ";
    std::cout << "(kg)\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }

  virtual Spremnik *DajKopiju() const override { return new Sanduk(*this); }
};

class Vreca : public Spremnik {
  double tezina_materije;

public:
  Vreca(double tezina, std::string naziv, double tezina_materije)
      : Spremnik(tezina, naziv), tezina_materije(tezina_materije) {}

  double DajUkupnuTezinu() const override { return tezina + tezina_materije; }

  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Vreca\n";
    std::cout << "Sadrzaj: " << naziv << "\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Tezina pohranjene materije: " << tezina_materije << " (kg)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }

  virtual Spremnik *DajKopiju() const override { return new Vreca(*this); }
};

class Bure : public Spremnik {
  double gustina_tecnosti;
  double zapremina;

public:
  Bure(double tezina, std::string naziv, double gustina_tecnosti,
       double zapremina)
      : Spremnik(tezina, naziv), gustina_tecnosti(gustina_tecnosti),
        zapremina(zapremina) {}

  double DajUkupnuTezinu() const override {
    return tezina + (gustina_tecnosti * (zapremina / 1000));
  }

  void Ispisi() const override {
    std::cout << "Vrsta spremnika: Bure\n";
    std::cout << "Sadrzaj: " << naziv << "\n";
    std::cout << "Vlastita tezina: " << tezina << " (kg)\n";
    std::cout << "Specificna tezina tecnosti: " << gustina_tecnosti
              << " (kg/m^3)\n";
    std::cout << "Zapremina tecnosti: " << zapremina << " (l)\n";
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)\n";
  }

  virtual Spremnik *DajKopiju() const override { return new Bure(*this); }
};

class PolimorfniSpremnik {
  Spremnik *spremnik;

public:
  PolimorfniSpremnik() : spremnik(nullptr) {}

  PolimorfniSpremnik(const Spremnik &spremnik)
      : spremnik(spremnik.DajKopiju()) {}

  PolimorfniSpremnik(const PolimorfniSpremnik &a) {
    if (a.spremnik)
      spremnik = a.spremnik->DajKopiju();
  }

  PolimorfniSpremnik(PolimorfniSpremnik &&a) noexcept : spremnik(a.spremnik) {
    a.spremnik = nullptr;
  }

  PolimorfniSpremnik &operator=(PolimorfniSpremnik a) {
    std::swap(spremnik, a.spremnik);
    return *this;
  }

  ~PolimorfniSpremnik() { delete spremnik; }

  double DajTezinu() const {
    if (!spremnik)
      throw std::logic_error("Nespecificiran spremnik");
    return spremnik->DajTezinu();
  }

  double DajUkupnuTezinu() const {
    if (!spremnik)
      throw std::logic_error("Nespecificiran spremnik");
    return spremnik->DajUkupnuTezinu();
  }

  void Ispisi() const {
    if (!spremnik)
      throw std::logic_error("Nespecificiran spremnik");
    spremnik->Ispisi();
  }
};

int main() { return 0; }
