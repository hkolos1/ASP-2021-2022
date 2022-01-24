#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;
const int vel = 1000;

template <typename Kljuc, typename Vrijednost> class Mapa {
public:
  Mapa(){};

  virtual ~Mapa(){};

  virtual Vrijednost operator[](const Kljuc &indeks) const = 0;

  virtual Vrijednost &operator[](const Kljuc &indeks) = 0;

  virtual int brojElemenata() const = 0;

  virtual void obrisi() = 0;

  virtual void obrisi(const Kljuc &kljuc) = 0;
};

template <typename Kljuc, typename Vrijednost>
class NizMapa : public Mapa<Kljuc, Vrijednost> {

  std::pair<Kljuc, Vrijednost> **niz;
  int velicina, kapacitet;

public:
  NizMapa<Kljuc, Vrijednost>() {
    velicina = 0;
    kapacitet = vel;
    niz = new std::pair<Kljuc, Vrijednost> *[vel];
  }

  NizMapa<Kljuc, Vrijednost>(const NizMapa<Kljuc, Vrijednost> &nova) {
    velicina = nova.velicina;
    kapacitet = vel;
    niz = new std::pair<Kljuc, Vrijednost> *[vel];

    for (int i = 0; i < velicina; i++) {
      this->niz[i] = new std::pair<Kljuc, Vrijednost>(nova.niz[i]->first,
                                                      nova.niz[i]->second);
    }
  }

  NizMapa<Kljuc, Vrijednost> operator=(const NizMapa<Kljuc, Vrijednost> &nova) {
    if (this == &nova)
      return *this;
    unisti();

    velicina = nova.velicina;
    kapacitet = vel;
    niz = new std::pair<Kljuc, Vrijednost> *[vel];

    for (int i = 0; i < velicina; i++) {
      this->niz[i] = new std::pair<Kljuc, Vrijednost>(nova.niz[i]->first,
                                                      nova.niz[i]->second);
    }
    return *this;
  }

  ~NizMapa() override {
    for (int i = 0; i < velicina; i++) {
      delete niz[i];
    }
    delete[] niz;
  }

  Vrijednost operator[](const Kljuc &indeks) const {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == indeks)
        return niz[i]->second;
    }
    return Vrijednost();
  }

  Vrijednost &operator[](const Kljuc &indeks) override {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == indeks)
        return niz[i]->second;
    }
    if (velicina == kapacitet)
      realociraj();
    niz[velicina] = new std::pair<Kljuc, Vrijednost>(indeks, Vrijednost());
    return niz[velicina++]->second;
  }

  void realociraj() {
    std::pair<Kljuc, Vrijednost> **pomocni;
    pomocni = new std::pair<Kljuc, Vrijednost> *[2 * vel];
    for (int i = 0; i < velicina; i++) {
      pomocni[i] =
          new std::pair<Kljuc, Vrijednost>(niz[i]->first, niz[i]->second);
    }
    unisti();
    niz = pomocni;
    kapacitet = 2 * vel;
  }

  int brojElemenata() const { return velicina; }

  void unisti() {
    for (int i = 0; i < velicina; i++) {
      delete niz[i];
    }
    delete[] niz;
  }

  void obrisi() override {
    for (int i = 0; i < velicina; i++) {
      delete niz[i];
    }
    velicina = 0;
  }

  void obrisi(const Kljuc &kljuc) {
    for (int i = 0; i < velicina; i++) {
      if (niz[i]->first == kljuc) {
        delete niz[i];
        if (i != velicina - 1) {
          niz[i] = niz[velicina - 1];
          niz[velicina - 1] = nullptr;
        }
        velicina--;
        return;
      }
    }
    throw std::logic_error("Nema takvog elementa");
  }
};

template <typename Kljuc, typename Vrijednost>
class BinStabloMapa : public Mapa<Kljuc, Vrijednost> {
  int velicina;

  struct Cvor {
    Kljuc key;
    Vrijednost value;
    Cvor *roditelj, *desno, *lijevo;

    Cvor(const Kljuc &k, const Vrijednost &v, Cvor *r) {
      key = k;
      value = v;
      lijevo = nullptr;
      desno = nullptr;
      roditelj = r;
    }
  };

  Cvor *korijen;

  Cvor *trazi(Cvor *cvor, const Kljuc &kljuc) const {
    if (cvor == nullptr || cvor->key == kljuc)
      return cvor;
    if (kljuc < cvor->key)
      return trazi(cvor->lijevo, kljuc);
    return trazi(cvor->desno, kljuc);
  }

  Cvor *dodaj(Cvor *&cvor, const Kljuc &kljuc, Cvor *rod) {
    if (cvor == nullptr) {
      cvor = new Cvor(kljuc, Vrijednost(), rod);
      velicina++;
      return cvor;
    } else if (kljuc < cvor->key)
      return dodaj(cvor->lijevo, kljuc, cvor);
    else
      return dodaj(cvor->desno, kljuc, cvor);
  }

public:
  BinStabloMapa() : Mapa<Kljuc, Vrijednost>() {
    velicina = 0;
    korijen = nullptr;
  }

  int brojElemenata() const { return velicina; }

  ~BinStabloMapa() { obrisiStablo(korijen); }

  BinStabloMapa(const BinStabloMapa<Kljuc, Vrijednost> &bsmapa)
      : korijen(nullptr), velicina(0) {
    kopirajStablo(korijen, bsmapa.korijen, nullptr);
  }

  BinStabloMapa<Kljuc, Vrijednost> &
  operator=(const BinStabloMapa<Kljuc, Vrijednost> &bsmapa) {
    if (this == &bsmapa)
      return *this;
    obrisiStablo(korijen);
    korijen = nullptr;
    velicina = 0;
    kopirajStablo(korijen, bsmapa.korijen, nullptr);
    return *this;
  }

  Vrijednost operator[](const Kljuc &indeks) const {
    Cvor *cvor = trazi(korijen, indeks);
    if (cvor != nullptr)
      return cvor->value;
    return Vrijednost();
  }

  Vrijednost &operator[](const Kljuc &indeks) {
    Cvor *cvor = trazi(korijen, indeks);
    if (cvor != nullptr)
      return cvor->value;
    cvor = dodaj(korijen, indeks, nullptr);
    return cvor->value;
  }

  void obrisi() {
    obrisiStablo(korijen);
    korijen = nullptr;
    velicina = 0;
  }

  void obrisi(const Kljuc &kljuc) {
    Cvor *del = trazi(korijen, kljuc);
    if (del == nullptr)
      throw std::logic_error("Kljuc ne postoji");
    if (del->lijevo == nullptr && del->desno == nullptr) {
      if (del == korijen)
        korijen = nullptr;
      else if (del == del->roditelj->lijevo)
        del->roditelj->lijevo = nullptr;
      else
        del->roditelj->desno = nullptr;
      delete del;
    } else if (del->lijevo != nullptr && del->desno != nullptr) {
      Cvor *pom = del->lijevo;
      while (pom->desno != nullptr)
        pom = pom->desno;
      pom->desno = del->desno;
      if (del->desno != nullptr)
        del->desno->roditelj = pom;
      pom->roditelj->desno = pom->lijevo;
      if (pom->lijevo != nullptr)
        pom->lijevo->roditelj = pom->roditelj;
      if (del == korijen) {
        korijen = pom;
      } else if (del == del->roditelj->lijevo) {
        del->roditelj->lijevo = pom;
        pom->roditelj = del->roditelj;
      } else {
        del->roditelj->desno = pom;
        pom->roditelj = del->roditelj;
      }
      if (del == korijen)
        pom->lijevo = nullptr;
      else {
        pom->lijevo = del->lijevo;
        del->lijevo->roditelj = pom;
      }
      delete del;
    } else {
      Cvor *dijete = del->lijevo;
      if (dijete == nullptr)
        dijete = del->desno;
      if (del == korijen)
        korijen = dijete;
      else if (del == del->roditelj->lijevo)
        del->roditelj->lijevo = dijete;
      else
        del->roditelj->desno = dijete;
      dijete->roditelj = del->roditelj;
      delete del;
    }
    velicina--;
  }

  void obrisiStablo(Cvor *cvor) {
    if (cvor == nullptr)
      return;
    obrisiStablo(cvor->lijevo);
    obrisiStablo(cvor->desno);
    delete cvor;
  }

  void kopirajStablo(Cvor *&cvor1, Cvor *cvor2, Cvor *roditelj1) {
    if (cvor2 == nullptr)
      return;
    cvor1 = new Cvor(cvor2->key, cvor2->value, roditelj1);
    kopirajStablo(cvor1->lijevo, cvor2->lijevo, cvor1);
    kopirajStablo(cvor1->desno, cvor2->desno, cvor1);
    velicina++;
  }
};

int main() {
  /*BinStabloMapa<int, int> binarno;
  NizMapa<int, int> niz;

  clock_t vrijeme1 = clock();

  for (int i = 0; i < 10000; i++) {
    binarno[rand() % 100] = i;
  }
  clock_t vrijeme2 = clock();

  cout << "Vrijeme izvrsavanja: " << vrijeme2 - vrijeme1 << endl;

  vrijeme1 = clock();

  for (int i = 0; i < 10000; i++) {
    binarno[rand() % 100] = i;
  }
  vrijeme2 = clock();

  cout << "Vrijeme izvrsavanja: " << vrijeme2 - vrijeme1 << endl;*/

  return 0;
}
