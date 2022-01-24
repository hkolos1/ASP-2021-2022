#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Tip> struct Cvor {
  Tip vrijednost;
  Cvor<Tip> *sljedeci = nullptr;
};

template <typename Tip> class Red {
  Cvor<Tip> *vrhReda;
  Cvor<Tip> *krajReda;
  int velicina;

public:
  Red() {
    vrhReda = nullptr;
    krajReda = nullptr;
    velicina = 0;
  }

  ~Red() { brisi(); }

  Red(const Red &red) {
    velicina = 0;
    vrhReda = nullptr;
    krajReda = nullptr;
    Cvor<Tip> *novi = red.vrhReda;
    while (novi != nullptr) {
      this->stavi(novi->vrijednost);
      novi = novi->sljedeci;
    }
  }

  Red(Red &&red) {
    velicina = red.velicina;
    vrhReda = red.vrhReda;
    krajReda = red.krajReda;
    red.vrhReda = nullptr;
    red.krajReda = nullptr;
    red.velicina = 0;
  }

  Red &operator=(const Red &red) {
    if (this == &red)
      return *this;
    this->brisi();

    Cvor<Tip> *novi = red.vrhReda;
    while (novi != nullptr) {
      this->stavi(novi->vrijednost);
      novi = novi->sljedeci;
    }
    return *this;
  }

  Red &operator=(Red &&red) {
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

  void brisi();
  void stavi(const Tip &el);
  Tip skini();
  int brojElemenata() const { return velicina; };
  Tip &celo();
};

template <typename Tip> void Red<Tip>::brisi() {
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

template <typename Tip> void Red<Tip>::stavi(const Tip &el) {
  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;
  if (krajReda != nullptr) // red nije prazan?
    krajReda->sljedeci = novi;
  else
    vrhReda = novi;
  krajReda = novi;
  velicina++;
}

template <typename Tip> Tip Red<Tip>::skini() {
  if (velicina == 0)
    throw range_error("Red je vec prazan!");

  Cvor<Tip> *sljedeciClan = vrhReda->sljedeci;
  Tip vrijednostClana = vrhReda->vrijednost;
  delete vrhReda;
  vrhReda = sljedeciClan;
  velicina--;
  if (velicina == 0) {
    vrhReda = nullptr;
    krajReda = nullptr;
  }
  return vrijednostClana;
}

template <typename Tip> Tip &Red<Tip>::celo() {
  if (velicina == 0)
    throw range_error("Izasli ste van opsega!");
  return vrhReda->vrijednost;
}
// Testiranje metode brojElemenata
bool testBrojElemenata() {
  Red<int> red;
  for (int i = 0; i < 15; i++)
    red.stavi(i);
  return red.brojElemenata() == 15;
}
// Testiranje metode brisi()
bool testBrisi() {
  Red<int> red;
  for (int i = 0; i < 15; i++)
    red.stavi(i);
  red.brisi();
  return red.brojElemenata() == 0;
}
// Testiranje metode skini()
bool testSkini() {
  Red<int> red;
  for (int i = 0; i < 15; i++) {
    red.stavi(i);
    if (red.skini() != i)
      return false;
  }
  return (red.brojElemenata() == 0);
}
// Testiranje metode celo()
bool testCelo() {
  Red<int> red;
  for (int i = 0; i < 15; i++)
    red.stavi(i);
  if (red.celo() != 0)
    return false;
  red.celo() = 10;
  if (red.celo() != 10)
    return false;
  return true;
}

int main() {
  if (testBrojElemenata() && testBrisi() && testSkini() && testCelo())
    cout << "OK" << endl;
  else
    cout << "NOTOK";
  return 0;
}
