#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <new>
#include <stdexcept>

enum Smjer { Desno, Dolje, Dijagonalno };

template <typename TipElemenata> struct Matrica {
  char ime_matrice;
  int br_redova, br_kolona;
  TipElemenata **elementi = nullptr; // VEOMA BITNA INICIJALIZACIJA!!!
};

template <typename TipElemenata>
void UnistiMatricu(Matrica<TipElemenata> &mat) {
  if (!mat.elementi)
    return;
  for (int i = 0; i < mat.br_redova; i++)
    delete[] mat.elementi[i];
  delete[] mat.elementi;
  mat.elementi = nullptr;
}

template <typename TipElemenata>
Matrica<TipElemenata> StvoriMatricu(int br_redova, int br_kolona,
                                    char ime = 0) {
  Matrica<TipElemenata> mat;
  mat.br_redova = br_redova;
  mat.br_kolona = br_kolona;
  mat.ime_matrice = ime;
  mat.elementi = nullptr;

  try {
    mat.elementi = new TipElemenata *[br_redova] {};
    for (int i = 0; i < br_redova; i++) {
      mat.elementi[i] = new TipElemenata[br_kolona]{}; // Dodana inicijalizacija
    }
  } catch (...) {
    if (mat.elementi) {
      for (int i = 0; i < br_redova; i++) {
        delete[] mat.elementi[i];
      }
      delete[] mat.elementi;
    }
    throw;
  }

  return mat;
}

template <typename TipElemenata> void UnesiMatricu(Matrica<TipElemenata> &mat) {
  for (int i = 0; i < mat.br_redova; i++)
    for (int j = 0; j < mat.br_kolona; j++) {
      std::cout << mat.ime_matrice << "(" << i + 1 << "," << j + 1
                << ") = " << std::endl;
      std::cin >> mat.elementi[i][j];
    }
}

template <typename TipElemenata>
void IspisiMatricu(Matrica<TipElemenata> mat, int sirina = 4, int tacnost = 3,
                   bool treba_brisati = false) {
  for (int i = 0; i < mat.br_redova; i++) {
    for (int j = 0; j < mat.br_kolona; j++)
      std::cout << std::setw(sirina) << std::setprecision(tacnost)
                << mat.elementi[i][j];
    std::cout << std::endl;
  }
  if (treba_brisati)
    UnistiMatricu(mat);
}

template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1,
                                  const Matrica<TipElemenata> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
  return m3;
}

template <typename TipElemenata>
Matrica<TipElemenata> ProsiriPremaFunkcijama(
    Matrica<TipElemenata> &mat,
    const std::map<Smjer, std::function<TipElemenata(TipElemenata)>> &m,
    int n) {
  if (n < 1) {
    throw std::domain_error("Besmislen parametar");
  }

  Matrica<TipElemenata> nova_mat =
      StvoriMatricu<TipElemenata>(mat.br_redova, mat.br_kolona);
  for (int i = 0; i < mat.br_redova; i++) {
    for (int j = 0; j < mat.br_kolona; j++) {
      nova_mat.elementi[i][j] = mat.elementi[i][j];
    }
  }
  if (m.empty()) {
    return nova_mat;
  }

  try {
    for (int iteracija = 0; iteracija < n - 1; iteracija++) {

      int redovi = nova_mat.br_redova;
      int kolone = nova_mat.br_kolona;

if(m.count(Dijagonalno)>0 && m.count(Desno)==0 && m.count(Dolje)==0){
     redovi += nova_mat.br_redova;
      kolone += nova_mat.br_kolona;

    
}

      if (m.count(Dolje) > 0) {
        redovi += nova_mat.br_redova;
      }

      if (m.count(Desno) > 0) {
        kolone += nova_mat.br_kolona;
      }

      Matrica<TipElemenata> temp =
          StvoriMatricu<TipElemenata>(redovi, kolone);

      for (int i = 0; i < nova_mat.br_redova; i++) {
        for (int j = 0; j < nova_mat.br_kolona; j++) {
          temp.elementi[i][j] = nova_mat.elementi[i][j];
        }
      }
     if(m.count(Dijagonalno)>0 && m.count(Desno)==0 && m.count(Dolje)==0){
         for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i][j + nova_mat.br_kolona]=nova_mat.elementi[i][j];
                 for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i + nova_mat.br_redova][j] =
               nova_mat.elementi[i][j];
                for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
          temp.elementi[i+nova_mat.br_redova][j+nova_mat.br_kolona]=m.at(Dijagonalno)(nova_mat.elementi[i][j]);
            
             

            
     }
     else if (m.count(Desno) > 0 && m.count(Dolje) > 0 &&
          m.count(Dijagonalno) == 0) {
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i][j + nova_mat.br_kolona] =
                m.at(Desno)(nova_mat.elementi[i][j]);
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i + nova_mat.br_redova][j] =
                m.at(Dolje)(nova_mat.elementi[i][j]);
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i + nova_mat.br_redova][j + nova_mat.br_kolona] =
                nova_mat.elementi[i][j];
      }
      if (m.count(Desno) > 0)
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i][j + nova_mat.br_kolona] =
                m.at(Desno)(nova_mat.elementi[i][j]);

      if (m.count(Dolje) > 0)
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            temp.elementi[i + nova_mat.br_redova][j] =
                m.at(Dolje)(nova_mat.elementi[i][j]);

      if (m.count(Dijagonalno) > 0)
        for (int i = 0; i < nova_mat.br_redova; i++)
          for (int j = 0; j < nova_mat.br_kolona; j++)
            if (m.count(Desno) > 0 && m.count(Dolje) > 0) {
              temp.elementi[i + nova_mat.br_redova][j + nova_mat.br_kolona] =
                  m.at(Dijagonalno)(nova_mat.elementi[i][j]);
              temp.elementi[i + nova_mat.br_redova][j] =
                  m.at(Dolje)(nova_mat.elementi[i][j]);
              temp.elementi[i][j + nova_mat.br_kolona] =
                  m.at(Desno)(nova_mat.elementi[i][j]);

            } else if (m.count(Desno) > 0) {
              temp.elementi[i][j + nova_mat.br_kolona] =
                  m.at(Dijagonalno)(nova_mat.elementi[i][j]);
            } else if (m.count(Dolje) > 0) {
              temp.elementi[i + nova_mat.br_redova][j] =
                  m.at(Dijagonalno)(nova_mat.elementi[i][j]);
            }

      UnistiMatricu(nova_mat);
      nova_mat = temp;
    }
  } catch (...) {
    UnistiMatricu(nova_mat);
    throw;
  }
  return nova_mat;
}

int main() {
  try {
    int br_redova, br_kolona, n;
    std::cout << "Unesi broj redova i kolona matrice:\n";
    std::cin >> br_redova >> br_kolona;
    Matrica<int> mat = StvoriMatricu<int>(br_redova, br_kolona, 'M');

    UnesiMatricu(mat);
    std::cout << "Unesite n: " << std::endl;
    std::cin >> n;
    std::map<Smjer, std::function<int(int)>> m{
        {Smjer::Desno, [](int x) { return x + 1; }},
        {Smjer::Dolje, [](int x) { return x + 2; }},
        {Smjer::Dijagonalno, [](int x) { return x + 3; }}};
    Matrica<int> rez = ProsiriPremaFunkcijama(mat, m, n);
    IspisiMatricu(rez, 6, true);
    UnistiMatricu(rez);
    UnistiMatricu(mat);
  } catch (std::bad_alloc) {
    std::cout << "Problem sa alokacijom.";
  } catch (std::logic_error e) {
    std::cout << e.what();
  }

  return 0;
}
