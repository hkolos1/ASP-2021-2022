#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip> struct Cvor {
  Tip vrijednost;
  Cvor<Tip> *sljedeci = nullptr;
  Cvor<Tip> *prethodni = nullptr;
};

template <typename Tip> class DvostraniRed {
  Cvor<Tip> *vrhReda;
  Cvor<Tip> *krajReda;
  int velicina;

public:
  DvostraniRed() {
    vrhReda = nullptr;
    krajReda = nullptr;
    velicina = 0;
  }

  ~DvostraniRed() { brisi(); }

  DvostraniRed(const DvostraniRed &red) {
    velicina = 0;
    vrhReda = nullptr;
    krajReda = nullptr;
    Cvor<Tip> *novi = red.vrhReda;
    while (novi != nullptr) {
      this->staviNaVrh(novi->vrijednost);
      novi = novi->sljedeci;
    }
  }

  DvostraniRed(DvostraniRed &&red) {
    velicina = red.velicina;
    vrhReda = red.vrhReda;
    krajReda = red.krajReda;
    red.vrhReda = nullptr;
    red.krajReda = nullptr;
    red.velicina = 0;
  }

  DvostraniRed &operator=(const DvostraniRed &red) {
    if (this == &red)
      return *this;
    this->brisi();

    Cvor<Tip> *novi = red.vrhReda;
    while (novi != nullptr) {
      this->staviNaVrh(novi->vrijednost);
      novi = novi->sljedeci;
    }
    return *this;
  }

  DvostraniRed &operator=(DvostraniRed &&red) {
    if (this == &red)
      return *this;
    this->brisi();
    velicina = red.velicina;
    vrhReda = red.vrhReda;
    krajReda = red.krajReda;
    red.vrhReda = nullptr;
    red.krajReda = nullptr;
    red.velicina = 0;
    return *this;
  }
  void staviNaCelo(const Tip &el);
  Tip skiniSaCela();
  Tip &vrh();
  void brisi();
  void staviNaVrh(const Tip &el);
  Tip skiniSaVrha();
  int brojElemenata() const { return velicina; };
  Tip &celo();
};

template <typename Tip> void DvostraniRed<Tip>::brisi() {
  while (velicina != 0) {
    Cvor<Tip> *sljedeciClan = vrhReda->sljedeci;
    delete vrhReda;
    vrhReda = sljedeciClan;
    velicina--;
  }
  vrhReda = nullptr;
  krajReda = nullptr;
  velicina = 0;
}

template <typename Tip> void DvostraniRed<Tip>::staviNaVrh(const Tip &el) {
  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;
  if (krajReda != nullptr) {
    krajReda->sljedeci = novi;
    novi->prethodni = krajReda;
  } else {
    vrhReda = novi;
  }
  krajReda = novi;
  velicina++;
}

template <typename Tip> Tip DvostraniRed<Tip>::skiniSaVrha() {
  if (velicina == 0)
    throw range_error("Red je vec prazan!");

  Cvor<Tip> *sljedeciClan = krajReda->prethodni;
  Tip vrijednostClana = krajReda->vrijednost;
  delete krajReda;
  krajReda = sljedeciClan;
  velicina--;

  if (velicina == 0) {
    vrhReda = nullptr;
    krajReda = nullptr;

  } else {
    krajReda->sljedeci = nullptr;
  }
  return vrijednostClana;
}

template <typename Tip> Tip &DvostraniRed<Tip>::celo() {
  if (velicina == 0)
    throw range_error("Izasli ste van opsega!");
  return vrhReda->vrijednost;
}

template <typename Tip> void DvostraniRed<Tip>::staviNaCelo(const Tip &el) {
  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;
  novi->sljedeci = vrhReda;
  if (vrhReda != nullptr)
    vrhReda->prethodni = novi;
  vrhReda = novi;
  if (krajReda == nullptr)
    krajReda = novi;
  velicina++;
}

template <typename Tip> Tip DvostraniRed<Tip>::skiniSaCela() {
  if (velicina == 0)
    throw range_error("Red je vec prazan!");
  Cvor<Tip> *brisati = vrhReda;
  Tip vrijednost = vrhReda->vrijednost;
  vrhReda = vrhReda->sljedeci;
  if (vrhReda != nullptr)
    vrhReda->prethodni = nullptr;
  delete brisati;
  brisati = nullptr;
  velicina--;
  if (velicina == 0) {
    vrhReda = nullptr;
    krajReda = nullptr;
  }
  return vrijednost;
}

template <typename Tip> Tip &DvostraniRed<Tip>::vrh() {
  if (velicina == 0)
    throw range_error("Izasli ste van opsega!");
  return krajReda->vrijednost;
}

bool testNaCelo() {
  DvostraniRed<int> red;
  for (int i = 0; i < 10; i++) {
    red.staviNaCelo(i);
    if (red.celo() != i)
      return false;
  }
  return true;
}

bool testSaCela() {
  DvostraniRed<int> red;
  int broj;
  for (int i = 0; i < 14; i++) {
    red.staviNaCelo(i);
    broj = red.skiniSaCela();
    if (broj != i)
      return false;
  }
  return red.brojElemenata() == 0;
}

bool test3() {
  DvostraniRed<double> red;
  for (double i = 0; i < 13; i += 1.7) {
    red.staviNaCelo(i);
    if (fabs(red.celo() - i) > 0.0000001)
      return false;
  }
  return true;
}

int main() {
  // std::cout << "Zadaća 2, Zadatak 1";
  bool t1 = testNaCelo();
  bool t2 = testSaCela();
  bool t3 = test3();

  if (t1 && t2 && t3)
    cout << "OK!" << endl;
  else if (!t1 && t2 && t3)
    cout << "TestNaCelo NOTOK" << endl;
  else if (t1 && !t2 && t3)
    cout << "TestSaCela NOTOK" << endl;
  else if (t1 && t2 && !t3)
    cout << "Test3 NOTOK" << endl;
  else
    cout << "NOTOK" << endl;
  return 0;
}
