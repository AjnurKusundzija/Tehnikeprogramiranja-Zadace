#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

template <typename TipEl> class Matrica {
  int br_redova, br_kolona;
  TipEl **elementi;
  char ime_matrice;

  static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
  static void DealocirajMemoriju(TipEl **elementi, int br_redova);
  void KopirajElemente(TipEl **elementi);

public:
  Matrica(int br_redova, int br_kolona, char ime = 0);
  Matrica(const Matrica &m);
  Matrica(Matrica &&m) noexcept;
  ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
  Matrica &operator=(const Matrica &m);
  Matrica &operator=(Matrica &&m) noexcept;

  Matrica(const std::string &ime_dat, bool jelibinarna);

  void SacuvajUTekstualnuDatoteku(const std::string &ime_dat) const;
  void SacuvajUBinarnuDatoteku(const std::string &ime_dat) const;
  void ObnoviIzTekstualneDatoteke(const std::string &ime_dat);
  void ObnoviIzBinarneDatoteke(const std::string &ime_dat);

  template <typename Tip2>
  friend std::istream &operator>>(std::istream &tok, Matrica<Tip2> &m);
  template <typename Tip2>
  friend std::ostream &operator<<(std::ostream &tok, const Matrica<Tip2> &m);
  template <typename Tip2>
  friend Matrica<Tip2> operator+(const Matrica<Tip2> &m1,
                                 const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator-(const Matrica<Tip2> &m1,
                                 const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(const Matrica<Tip2> &m1,
                                 const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(const Matrica<Tip2> &m, Tip2 d);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(Tip2 d, const Matrica<Tip2> &m);

  Matrica &operator+=(const Matrica &m);
  Matrica &operator-=(const Matrica &m);
  Matrica &operator*=(const Matrica &m);
  Matrica &operator*=(TipEl d);

  TipEl *operator[](int i) { return elementi[i]; }
  const TipEl *operator[](int i) const { return elementi[i]; }
  TipEl &operator()(int i, int j);
  const TipEl &operator()(int i, int j) const;

  operator std::string() const;
};

template <typename TipEl>
TipEl **Matrica<TipEl>::AlocirajMemoriju(int br_redova, int br_kolona) {
  TipEl **elementi = new TipEl *[br_redova] {};
  try {
    for (int i = 0; i < br_redova; i++)
      elementi[i] = new TipEl[br_kolona];
  } catch (...) {
    DealocirajMemoriju(elementi, br_redova);
    throw;
  }
  return elementi;
}

template <typename TipEl>
void Matrica<TipEl>::DealocirajMemoriju(TipEl **elementi, int br_redova) {
  if (elementi) {
    for (int i = 0; i < br_redova; i++)
      delete[] elementi[i];
    delete[] elementi;
  }
}

template <typename TipEl>
Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime)
    : br_redova(br_redova), br_kolona(br_kolona),
      elementi(AlocirajMemoriju(br_redova, br_kolona)), ime_matrice(ime) {}

template <typename TipEl>
void Matrica<TipEl>::KopirajElemente(TipEl **elementi) {
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      Matrica::elementi[i][j] = elementi[i][j];
}

template <typename TipEl>
Matrica<TipEl>::Matrica(const Matrica &m)
    : br_redova(m.br_redova), br_kolona(m.br_kolona),
      elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)),
      ime_matrice(m.ime_matrice) {
  KopirajElemente(m.elementi);
}

template <typename TipEl>
Matrica<TipEl>::Matrica(Matrica &&m) noexcept
    : br_redova(m.br_redova), br_kolona(m.br_kolona), elementi(m.elementi),
      ime_matrice(m.ime_matrice) {
  m.br_redova = 0;
  m.elementi = nullptr;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator=(const Matrica &m) {
  if (this != &m) {
    TipEl **novi_prostor = AlocirajMemoriju(m.br_redova, m.br_kolona);
    DealocirajMemoriju(elementi, br_redova);
    elementi = novi_prostor;
    br_redova = m.br_redova;
    br_kolona = m.br_kolona;
    ime_matrice = m.ime_matrice;
    KopirajElemente(m.elementi);
  }
  return *this;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator=(Matrica &&m) noexcept {
  if (this != &m) {
    std::swap(br_redova, m.br_redova);
    std::swap(br_kolona, m.br_kolona);
    std::swap(ime_matrice, m.ime_matrice);
    std::swap(elementi, m.elementi);
  }
  return *this;
}

template <typename TipEl>
Matrica<TipEl>::Matrica(const std::string &ime_dat, bool jelibinarna)
    : elementi(nullptr), br_redova(0), br_kolona(0) {
  if (jelibinarna) {
    ObnoviIzBinarneDatoteke(ime_dat);
  } else {
    ObnoviIzTekstualneDatoteke(ime_dat);
  }
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUBinarnuDatoteku(const std::string &ime_dat) const {
  std::ofstream datoteka(ime_dat, std::ios::binary);
  if (!datoteka)
    throw std::logic_error("Problemi sa upisom u datoteku");
  datoteka.write(reinterpret_cast<const char *>(&br_redova), sizeof(br_redova));
  datoteka.write(reinterpret_cast<const char *>(&br_kolona), sizeof(br_kolona));
  for (int i = 0; i < br_redova; i++) {
    datoteka.write(reinterpret_cast<const char *>(elementi[i]),
                   br_kolona * sizeof(TipEl));
  }
  if (!datoteka)
    throw std::logic_error("Problemi sa upisom u datoteku");
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(
    const std::string &ime_dat) const {
  std::ofstream datoteka(ime_dat);
  if (!datoteka)
    throw std::logic_error("Problemi sa upisom u datoteku");
  for (int i = 0; i < br_redova; i++) {
    for (int j = 0; j < br_kolona; j++) {
      datoteka << elementi[i][j];
      if (j != br_kolona - 1)
        datoteka << ",";
    }
    datoteka << std::endl;
  }
  if (!datoteka)
    throw std::logic_error("Problemi sa upisom u datoteku");
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(const std::string &ime_dat) {
  std::ifstream datoteka(ime_dat);
  if (!datoteka)
    throw std::logic_error("Trazena datoteka ne postoji");

  // Prvi prolaz za određivanje broja redova i kolona
  int broj_redova = 0;
  int broj_kolona = -1;
  std::string linija;
  while (std::getline(datoteka, linija)) {
    std::istringstream iss(linija);
    std::string element_str;
    int trenutni_broj_kolona = 0;

    while (std::getline(iss, element_str, ',')) {
      std::istringstream elem_stream(element_str);
      TipEl element;
      if (!(elem_stream >> element)) {
        throw std::logic_error("Datoteka sadrzi besmislene podatke");
      }
      trenutni_broj_kolona++;
    }

    if (broj_kolona == -1) {
      broj_kolona = trenutni_broj_kolona;
    } else if (trenutni_broj_kolona != broj_kolona) {
      throw std::logic_error("Datoteka sadrzi besmislene podatke");
    }
    broj_redova++;
  }

  if (broj_redova == 0 || broj_kolona == 0) {
    throw std::logic_error("Datoteka sadrzi besmislene podatke");
  }

  // Resetovanje ifstream-a za drugi prolaz
  datoteka.clear();
  datoteka.seekg(0, std::ios::beg);

  // Drugi prolaz za stvarno čitanje elemenata
  if (br_redova != broj_redova || br_kolona != broj_kolona) {
    DealocirajMemoriju(elementi, br_redova);
    br_redova = broj_redova;
    br_kolona = broj_kolona;
    elementi = AlocirajMemoriju(br_redova, br_kolona);
  }

  int red = 0;
  while (std::getline(datoteka, linija)) {
    std::istringstream iss(linija);
    std::string element_str;
    int kolona = 0;

    while (std::getline(iss, element_str, ',')) {
      std::istringstream elem_stream(element_str);
      TipEl element;
      if (!(elem_stream >> element)) {
        throw std::logic_error("Datoteka sadrzi besmislene podatke");
      }
      elementi[red][kolona] = element;
      kolona++;
    }
    red++;
  }

  if (!datoteka.eof()) {
    throw std::logic_error("Problemi pri citanju datoteke");
  }
}
template <typename TipEl>
void Matrica<TipEl>::ObnoviIzBinarneDatoteke(const std::string &ime_dat) {
  std::ifstream datoteka(ime_dat, std::ios::binary);
  if (!datoteka)
    throw std::logic_error("Trazena datoteka ne postoji");

  int novi_br_redova, novi_br_kolona;
  datoteka.read(reinterpret_cast<char *>(&novi_br_redova),
                sizeof(novi_br_redova));
  datoteka.read(reinterpret_cast<char *>(&novi_br_kolona),
                sizeof(novi_br_kolona));
  if (!datoteka)
    throw std::logic_error("Problemi pri citanju datoteke");

  DealocirajMemoriju(elementi, br_redova);
  br_redova = novi_br_redova;
  br_kolona = novi_br_kolona;
  elementi = AlocirajMemoriju(br_redova, br_kolona);

  for (int i = 0; i < br_redova; i++) {
    datoteka.read(reinterpret_cast<char *>(elementi[i]),
                  br_kolona * sizeof(TipEl));
    if (!datoteka)
      throw std::logic_error("Problemi pri citanju datoteke");
  }
}

template <typename TipEl> TipEl &Matrica<TipEl>::operator()(int i, int j) {
  if (i < 1 || i > br_redova || j < 1 || j > br_kolona)
    throw std::range_error("Neispravan indeks");
  return elementi[i - 1][j - 1];
}

template <typename TipEl>
const TipEl &Matrica<TipEl>::operator()(int i, int j) const {
  if (i < 1 || i > br_redova || j < 1 || j > br_kolona)
    throw std::range_error("Neispravan indeks");
  return elementi[i - 1][j - 1];
}

template <typename TipEl>
std::istream &operator>>(std::istream &tok, Matrica<TipEl> &m) {
  for (int i = 0; i < m.br_redova; i++) {
    for (int j = 0; j < m.br_kolona; j++) {
      if (&tok == &std::cin)
        std::cout << "(" << i + 1 << "," << j + 1 << ") = ";
      tok >> m.elementi[i][j];
    }
  }
  return tok;
}

template <typename TipEl>
std::ostream &operator<<(std::ostream &tok, const Matrica<TipEl> &m) {
  for (int i = 0; i < m.br_redova; i++) {
    for (int j = 0; j < m.br_kolona; j++) {
      tok << m.elementi[i][j];
      if (j != m.br_kolona - 1)
        tok << "";
    }
    tok << std::endl;
  }
  return tok;
}

template <typename TipEl>
Matrica<TipEl> operator+(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator-(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator*(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_kolona != m2.br_redova)
    throw std::domain_error("Matrice nisu saglasne za mnozenje");
  Matrica<TipEl> m3(m1.br_redova, m2.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m2.br_kolona; j++) {
      m3.elementi[i][j] = TipEl();
      for (int k = 0; k < m1.br_kolona; k++)
        m3.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
    }
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator*(const Matrica<TipEl> &m, TipEl d) {
  Matrica<TipEl> m2(m.br_redova, m.br_kolona);
  for (int i = 0; i < m.br_redova; i++)
    for (int j = 0; j < m.br_kolona; j++)
      m2.elementi[i][j] = m.elementi[i][j] * d;
  return m2;
}

template <typename TipEl>
Matrica<TipEl> operator*(TipEl d, const Matrica<TipEl> &m) {
  return m * d;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator+=(const Matrica &m) {
  if (br_redova != m.br_redova || br_kolona != m.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] += m.elementi[i][j];
  return *this;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator-=(const Matrica &m) {
  if (br_redova != m.br_redova || br_kolona != m.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] -= m.elementi[i][j];
  return *this;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator*=(const Matrica &m) {
  if (br_kolona != m.br_redova)
    throw std::domain_error("Matrice nisu saglasne za mnozenje");
  Matrica<TipEl> m1 = *this;
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < m.br_kolona; j++) {
      elementi[i][j] = TipEl();
      for (int k = 0; k < br_kolona; k++)
        elementi[i][j] += m1.elementi[i][k] * m.elementi[k][j];
    }
  return *this;
}

template <typename TipEl> Matrica<TipEl> &Matrica<TipEl>::operator*=(TipEl d) {
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] *= d;
  return *this;
}

template <typename TipEl> Matrica<TipEl>::operator std::string() const {
  using std::to_string;
  std::string s = "{";
  for (int i = 0; i < br_redova; i++) {
    s += "{";
    for (int j = 0; j < br_kolona; j++) {
      s += to_string(elementi[i][j]);
      if (j != br_kolona - 1)
        s += ",";
    }
    s += "}";
    if (i != br_redova - 1)
      s += ",";
  }
  return s += "}";
}

int main() {
  try {
    int br_redova, br_kolona;
    std::cout << "Unesite broj redova i kolona za matricu: ";
    std::cin >> br_redova >> br_kolona;

    Matrica<double> m(br_redova, br_kolona);
    std::cout << "Unesite elemente matrice:" << std::endl;
    std::cin >> m;

    std::string ime_tekstualne_datoteke, ime_binarne_datoteke;
    std::cout << "Unesite ime tekstualne datoteke: ";
    std::cin >> ime_tekstualne_datoteke;
    std::cout << "Unesite ime binarne datoteke: ";
    std::cin >> ime_binarne_datoteke;

    m.SacuvajUTekstualnuDatoteku(ime_tekstualne_datoteke);
    m.SacuvajUBinarnuDatoteku(ime_binarne_datoteke);

    Matrica<double> m1(ime_tekstualne_datoteke, false);
    Matrica<double> m2(ime_binarne_datoteke, true);

    std::cout << "Ucitana matrica iz tekstualne datoteke:" << std::endl;
    std::cout << m1;

    std::cout << "Ucitana matrica iz binarne datoteke:" << std::endl;
    std::cout << m2;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}