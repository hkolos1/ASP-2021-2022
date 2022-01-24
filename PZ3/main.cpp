#include <iostream>
#include <stdexcept>

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
    throw range_error("Stek je prazan!");
  Cvor<Tip> *prethodniClan = vrhSteka->prethodni;
  Tip vrijednostClana = vrhSteka->vrijednost;
  delete vrhSteka;
  vrhSteka = prethodniClan;
  velicina--;
  return vrijednostClana;
}
template <typename Tip> Tip &Stek<Tip>::vrh() {
  if (velicina == 0)
    throw range_error("Van opsega!");
  return vrhSteka->vrijednost;
}

int main() {
  std::cout << "Pripremna Zadaca Za Vjezbu 3, Zadatak 1";
  return 0;
}
