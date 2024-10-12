// TP 2023/2024: Zadaća 5, Zadatak 6
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
void SortirajBinarnuDatoteku(
    const char ime_datoteke[],
    std::function<bool(T, T)> kriterij = std::less<T>()) {
  std::fstream datoteka(ime_datoteke,
                        std::ios::in | std::ios::out | std::ios::binary);
  if (!datoteka) {
    throw std::logic_error("Datoteka ne postoji");
  }

  datoteka.seekg(0, std::ios::end);
  auto velicina = datoteka.tellg();
  datoteka.seekg(0, std::ios::beg);

  if (velicina % sizeof(T) != 0) {
    datoteka.close();
    throw std::logic_error("Problemi u pristupu datoteci");
  }

  int broj_elemenata = velicina / sizeof(T);

  
  bool je_li_true = true;
  bool je_li_false = true;
  for (int i = 0; i < broj_elemenata - 1; ++i) {
    T prvi, drugi;
    datoteka.seekg(i * sizeof(T));
    datoteka.read(reinterpret_cast<char *>(&prvi), sizeof(T));
    datoteka.read(reinterpret_cast<char *>(&drugi), sizeof(T));
    if (!kriterij(prvi, drugi)) {
      je_li_true = false;
    }
    if (kriterij(prvi, drugi)) {
      je_li_false = false;
    }
    if (!je_li_true && !je_li_false)
      break;
  }

  if (je_li_true) {
    datoteka.close();
    return;
  }

  if (je_li_false) {
    for (int i = 0; i < broj_elemenata / 2; ++i) {
      T prvi, zadnji;
      datoteka.seekg(i * sizeof(T));
      datoteka.read(reinterpret_cast<char *>(&prvi), sizeof(T));
      datoteka.seekg((broj_elemenata - 1 - i) * sizeof(T));
      datoteka.read(reinterpret_cast<char *>(&zadnji), sizeof(T));
      datoteka.seekp(i * sizeof(T));
      datoteka.write(reinterpret_cast<const char *>(&zadnji), sizeof(T));
      datoteka.seekp((broj_elemenata - 1 - i) * sizeof(T));
      datoteka.write(reinterpret_cast<const char *>(&prvi), sizeof(T));
    }
    datoteka.close();
    return;
  }

 
  for (int i = 0; i < broj_elemenata - 1; ++i) {
    for (int j = i + 1; j < broj_elemenata; ++j) {
      T prvi, drugi;
      datoteka.seekg(i * sizeof(T));
      datoteka.read(reinterpret_cast<char *>(&prvi), sizeof(T));
      datoteka.seekg(j * sizeof(T));
      datoteka.read(reinterpret_cast<char *>(&drugi), sizeof(T));
      if (kriterij(drugi, prvi)) {
        datoteka.seekp(i * sizeof(T));
        datoteka.write(reinterpret_cast<const char *>(&drugi), sizeof(T));
        datoteka.seekp(j * sizeof(T));
        datoteka.write(reinterpret_cast<const char *>(&prvi), sizeof(T));
      }
    }
  }

  datoteka.close();
}


void KreirajDatoteku(const char ime[]) {
  std::ofstream datoteka(ime, std::ios::binary);
  if (!datoteka) {
    throw std::logic_error("Ne moze se kreirati");
  }
  std::vector<int> brojevi = {5, 3, 4, 3, 7, 8, 9, 10};
  datoteka.write(reinterpret_cast<const char *>(&(*brojevi.begin())),
                 brojevi.size() * sizeof(int));
  datoteka.close();
}
void IspisiSadrzaj(const char ime[]) {
  std::ifstream datoteka(ime, std::ios::binary);
  if (!datoteka) {
    throw std::logic_error("Ne postoji datoteka");
  }
  std::cout << "Sadrzaj:";
  int broj;
  while (datoteka.read(reinterpret_cast<char *>(&broj), sizeof(int))) {
    std::cout << broj << " ";
  }
  std::cout << std::endl;
  datoteka.close();
}

int main() {
  const char *ime = "test.dat";
  KreirajDatoteku(ime);
  IspisiSadrzaj(ime);
  try {
    SortirajBinarnuDatoteku<int>(ime);

  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  IspisiSadrzaj(ime);

  return 0;
}
