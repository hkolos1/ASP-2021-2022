#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename Tip> struct Cvor {
  Tip vrijednost;
  Cvor<Tip> *prethodni = nullptr;
};

template <typename Tip> class Stek {
  Cvor<Tip> *vrhSteka;
  int velicina;

public:
  Stek() {
    vrhSteka = nullptr;
    velicina = 0;
  }
  ~Stek() { brisi(); }

  Stek(const Stek &stek) {
    Cvor<Tip> *vrhSteka = nullptr;
    velicina = 0;
    Cvor<Tip> *novi = stek.vrhSteka;
    Tip *niz = new Tip[stek.velicina];
    int i = 0;
    while (novi != nullptr) {
      niz[i] = novi->vrijednost;
      i++;
      novi = novi->prethodni;
    }
    for (int i = stek.velicina - 1; i >= 0; i--) {
      this->stavi(niz[i]);
    }
    delete[] niz;
  }

  Stek(Tip &&stek) {
    velicina = stek.velicina;
    vrhSteka = stek.vrhSteka;
    stek.vrhSteka = nullptr;
  }

  Stek &operator=(const Stek &stek) {
    if (this == &stek)
      return *this;
    this->brisi();
    Cvor<Tip> *novi = stek.vrhSteka;
    Tip *niz = new Tip[stek.velicina];
    int i = 0;
    while (novi != nullptr) {
      niz[i] = novi->vrijednost;
      i++;
      novi = novi->prethodni;
    }
    for (int i = stek.velicina - 1; i >= 0; i--) {
      this->stavi(niz[i]);
    }
    delete[] niz;

    return *this;
  }
  Stek &operator=(Tip &&stek) {
    if (this == &stek)
      return *this;
    velicina = stek.velicina;
    vrhSteka = stek.vrhSteka;
    stek.vrhSteka = nullptr;
    return *this;
  }
  void brisi();
  void stavi(const Tip &el);
  Tip skini();
  int brojElemenata() const { return velicina; };
  Tip &vrh();
};

template <typename Tip> void Stek<Tip>::brisi() {
  while (velicina != 0) {
    Cvor<Tip> *prethodniClan = vrhSteka->prethodni;
    delete vrhSteka;
    vrhSteka = prethodniClan;
    velicina--;
  }
}

template <typename Tip> void Stek<Tip>::stavi(const Tip &el) {
  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;
  novi->prethodni = vrhSteka;
  vrhSteka = novi;
  velicina++;
}

template <typename Tip> Tip Stek<Tip>::skini() {
  if (velicina == 0)
    throw range_error("Stek je vec prazan!");
  Cvor<Tip> *prethodniClan = vrhSteka->prethodni;
  Tip vrijednostClana = vrhSteka->vrijednost;
  delete vrhSteka;
  vrhSteka = prethodniClan;
  velicina--;
  return vrijednostClana;
}

template <typename Tip> Tip &Stek<Tip>::vrh() {
  if (velicina == 0)
    throw range_error("Izasli ste van opsega!");
  return vrhSteka->vrijednost;
}

void pretraga(Stek<vector<int>> &s, int trazeni) {
  if (s.brojElemenata() == 0) {
    cout << "Nema elementa";
    return;
  }

  vector<int> privremeni = s.vrh();
  int pocetakVektora = 0;
  int krajVektora = privremeni.size() - 1;
  int sredinaVektora;
  while (pocetakVektora <= krajVektora) {
    sredinaVektora = floor((pocetakVektora + krajVektora) / 2);
    if (privremeni[sredinaVektora] < trazeni)
      pocetakVektora = sredinaVektora + 1;
    else if (privremeni[sredinaVektora] > trazeni)
      krajVektora = sredinaVektora - 1;
    else {
      cout << sredinaVektora << " " << s.brojElemenata() - 1;
      return;
    }
  }
  s.skini();
  pretraga(s, trazeni);
  s.stavi(privremeni);
}

void test1() {
  vector<int> v1 = {10, 15, 17};
  vector<int> v2 = {50, 100};
  vector<int> v3 = {777};
  Stek<vector<int>> s;
  s.stavi(v1);
  s.stavi(v2);
  s.stavi(v3);
  pretraga(s, 15);
}

void test2() {
  vector<int> v1, v2, v3, v4;
  Stek<vector<int>> s;

  for (int i = 0; i < 10; i++) {
    v1.push_back(i);
    v2.push_back(i * 2);
    v3.push_back(i * 3);
    v4.push_back(i * 4);
  }
  s.stavi(v1);
  s.stavi(v2);
  s.stavi(v3);
  s.stavi(v4);

  pretraga(s, 5);
}

void test3() {
  Stek<vector<int>> s;
  pretraga(s, 10);
}

int main() {

  test1();
  cout << endl;
  test2();
  cout << endl;
  test3();
  cout << endl;

  return 0;
}