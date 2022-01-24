#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

unsigned int hashF(string ulaz, int max) {
  int prosti = 6971;
  for (int i = 0; i < ulaz.size(); i++)
    prosti ^= ((prosti << 5)) + ulaz.at(i) + (prosti >> 2);

  return (prosti & 0x7FFFFFFF) % max;
}

unsigned int ukradenaHashFunkcija(std::string ulaz, unsigned int max) {
  unsigned int suma = 5381; // poboljsava distribuciju

  for (int i(0); i < ulaz.length(); i++)
    suma = suma * 33 + ulaz[i];
  return suma % max;
}

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
  virtual ~Mapa(){};
  virtual int brojElemenata() const = 0;
  virtual TipVrijednosti &operator[](TipKljuca kljuc) = 0;
  virtual TipVrijednosti operator[](TipKljuca kljuc) const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  int n; // Broj elemenata
  int kapacitet;
  pair<TipKljuca, TipVrijednosti> *elementi = nullptr;

public:
  NizMapa() {
    elementi = nullptr;
    kapacitet = 100;
    n = 0;
  }

  NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &mapa);
  NizMapa(NizMapa<TipKljuca, TipVrijednosti> &&mapa);
  NizMapa &operator=(const NizMapa<TipKljuca, TipVrijednosti> &mapa);
  NizMapa &operator=(NizMapa<TipKljuca, TipVrijednosti> &&mapa);

  ~NizMapa() { this->obrisi(); }

  TipVrijednosti &operator[](TipKljuca kljuc);
  TipVrijednosti operator[](TipKljuca kljuc) const;
  void obrisi();
  void obrisi(const TipKljuca &kljuc);
  int brojElemenata() const { return n; };
};

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi() {
  delete[] elementi;
  elementi = nullptr;
  n = 0;
  kapacitet = 100;
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  if (n == 0)
    throw domain_error("Mapa je prazna");

  int pozicija = -1;
  for (int i = 0; i < n; i++) {
    if (elementi[i].first == kljuc) {
      pozicija = i;
      break;
    }
  }

  if (pozicija == -1)
    throw domain_error("Element ne postoji u ovoj mapi!");

  for (; pozicija < n - 1; pozicija++) {
    elementi[pozicija].first = elementi[pozicija + 1].first;
    elementi[pozicija].second = elementi[pozicija + 1].second;
  }
  n--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) {
  if (n == 0) // Ako je prazna mapa
  {
    elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];
    elementi[0].first = kljuc;
    elementi[0].second = TipVrijednosti();
    n++;
    return elementi[0].second;
  }
  // Pretrazi mapu
  for (int i = 0; i < n; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;

  // Element nije nadjen
  if (n == kapacitet) // Ako je puna mapa
  {
    kapacitet *= 5;
    pair<TipKljuca, TipVrijednosti> *novaMapa =
        new pair<TipKljuca, TipVrijednosti>[kapacitet];
    for (int i = 0; i < n; i++) {
      novaMapa[i].first = elementi[i].first;
      novaMapa[i].second = elementi[i].second;
    }
    delete[] elementi;
    elementi = novaMapa;
    novaMapa = nullptr;
  }

  // Ubacivanje kljuca
  elementi[n].first = kljuc;
  elementi[n].second = TipVrijednosti();
  n++;
  return elementi[n - 1].second;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti
NizMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) const {
  for (int i = 0; i < n; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;
  return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(
    const NizMapa<TipKljuca, TipVrijednosti> &mapa) {
  if (&mapa == this)
    return;

  n = mapa.n;
  kapacitet = mapa.kapacitet;
  elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];

  for (int i = 0; i < n; i++) {
    elementi[i].first = mapa.elementi[i].first;
    elementi[i].second = mapa.elementi[i].second;
  }
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(
    NizMapa<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return;
  n = mapa.n;
  kapacitet = mapa.kapacitet;
  swap(mapa.elementi, elementi);
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &
NizMapa<TipKljuca, TipVrijednosti>::operator=(
    const NizMapa<TipKljuca, TipVrijednosti> &mapa) {
  if (&mapa == this)
    return *this;

  delete[] elementi;
  elementi = new pair<TipKljuca, TipVrijednosti>[mapa.kapacitet];
  n = mapa.n;
  kapacitet = mapa.kapacitet;
  for (int i = 0; i < n; i++) {
    elementi[i].first = mapa.elementi[i].first;
    elementi[i].second = mapa.elementi[i].second;
  }
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &
NizMapa<TipKljuca, TipVrijednosti>::operator=(
    NizMapa<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return *this;

  n = mapa.n;
  kapacitet = mapa.kapacitet;
  swap(mapa.elementi, elementi);

  return *this;
}

template <typename TipKljuca, typename TipVrijednosti> struct Cvor {
  TipKljuca kljuc;
  TipVrijednosti vrijednost;
  Cvor *lijevi;
  Cvor *desni;
  Cvor *roditelj;
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  Cvor<TipKljuca, TipVrijednosti> *korijen;
  int n;

  void zbrisi(Cvor<TipKljuca, TipVrijednosti> *&k) {
    Cvor<TipKljuca, TipVrijednosti> *sljedeci = k;
    if (sljedeci == nullptr)
      return;
    zbrisi(k->lijevi);
    zbrisi(k->desni);
    delete sljedeci;
  }

  void kopiraj(Cvor<TipKljuca, TipVrijednosti> *&prvi,
               Cvor<TipKljuca, TipVrijednosti> &drugi) {
    if (drugi == nullptr)
      return;

    Cvor<TipKljuca, TipVrijednosti> *pomocni = drugi;

    prvi = new Cvor<TipKljuca, TipVrijednosti>();
    prvi->kljuc = pomocni->kljuc;
    prvi->vrijednost = pomocni->vrijednost;
    prvi->lijevi = nullptr;
    prvi->desni = nullptr;

    kopiraj(prvi->lijevi, pomocni->lijevi);
    kopiraj(prvi->desni, pomocni->desni);
    if (prvi->lijevi != nullptr)
      prvi->lijevi->roditelj = prvi;
    if (prvi->desni != nullptr)
      prvi->desni->roditelj = prvi;
    n++;
  }

public:
  BinStabloMapa() {
    korijen = nullptr;
    n = 0;
  }

  BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo);
  BinStabloMapa<TipKljuca, TipVrijednosti> &
  operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo);

  ~BinStabloMapa() {
    zbrisi(korijen);
    korijen = nullptr;
    n = 0;
  };
  int brojElemenata() const { return n; };
  TipVrijednosti &operator[](TipKljuca kljuc);
  TipVrijednosti operator[](TipKljuca kljuc) const;
  void obrisi();
  void obrisi(const TipKljuca &kljuc);
};
template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(
    const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
  korijen = nullptr;
  n = 0;
  kopiraj(korijen, stablo.korijen);
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &
BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(
    const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo) {
  if (this == &stablo)
    return *this;
  zbrisi(korijen);
  n = 0;
  korijen = nullptr;
  kopiraj(korijen, stablo.korijen);
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi() {
  zbrisi(korijen);
  korijen = nullptr;
  n = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  if (n == 0)
    throw std::domain_error("Mapa je prazna!");

  Cvor<TipKljuca, TipVrijednosti> *trazeni;
  Cvor<TipKljuca, TipVrijednosti> *sljedeci = korijen;
  while (sljedeci != nullptr && kljuc != sljedeci->kljuc) {
    if (kljuc < sljedeci->kljuc)
      sljedeci = sljedeci->lijevi;
    else
      sljedeci = sljedeci->desni;
  }
  trazeni = sljedeci;

  if (trazeni == nullptr)
    throw std::domain_error("Kljuc ne postoji!");

  Cvor<TipKljuca, TipVrijednosti> *roditeljCvor = trazeni->roditelj;

  Cvor<TipKljuca, TipVrijednosti> *prvi = nullptr;
  Cvor<TipKljuca, TipVrijednosti> *drugi = nullptr;
  Cvor<TipKljuca, TipVrijednosti> *privremeni = nullptr;

  if (trazeni->lijevi == nullptr)
    prvi = trazeni->desni;
  else if (trazeni->desni == nullptr)
    prvi = trazeni->lijevi;
  else {
    drugi = prvi;
    prvi = trazeni->lijevi;
    privremeni = prvi->desni;
    while (privremeni != nullptr) {
      drugi = prvi;
      prvi = privremeni;
      privremeni = prvi->desni;
    }
    if (drugi != trazeni) {
      drugi->desni = prvi->lijevi;
      prvi->lijevi = trazeni->lijevi;
    }
    prvi->desni = trazeni->desni;
  }

  if (roditeljCvor == nullptr)
    korijen = prvi;
  else {
    if (trazeni == roditeljCvor->lijevi)
      roditeljCvor->lijevi = prvi;
    else
      roditeljCvor->desni = prvi;
  }

  if (prvi != nullptr)
    prvi->roditelj = roditeljCvor;

  delete trazeni;
  trazeni = nullptr;
  n--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti
BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) const {
  Cvor<TipKljuca, TipVrijednosti> *trazeni;

  Cvor<TipKljuca, TipVrijednosti> *sljedeci = korijen;
  while (sljedeci != nullptr && kljuc != sljedeci->kljuc) {
    if (kljuc < sljedeci->kljuc)
      sljedeci = sljedeci->lijevi;
    else
      sljedeci = sljedeci->desni;
  }
  trazeni = sljedeci;

  if (trazeni == nullptr)
    return TipVrijednosti();
  else
    return trazeni->vrijednost;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) {
  Cvor<TipKljuca, TipVrijednosti> *trazeni;
  {
    Cvor<TipKljuca, TipVrijednosti> *sljedeci = korijen;
    while (sljedeci != nullptr && kljuc != sljedeci->kljuc) {
      if (kljuc < sljedeci->kljuc)
        sljedeci = sljedeci->lijevi;
      else
        sljedeci = sljedeci->desni;
    }
    trazeni = sljedeci;
  }
  if (trazeni == nullptr) {
    trazeni = new Cvor<TipKljuca, TipVrijednosti>();
    trazeni->kljuc = kljuc;
    trazeni->lijevi = nullptr;
    trazeni->desni = nullptr;
    trazeni->vrijednost = TipVrijednosti();

    Cvor<TipKljuca, TipVrijednosti> *prvi = korijen;
    Cvor<TipKljuca, TipVrijednosti> *drugi = nullptr;

    while (prvi != nullptr) {
      drugi = prvi;
      if (trazeni->kljuc < prvi->kljuc)
        prvi = prvi->lijevi;
      else
        prvi = prvi->desni;
    }

    if (drugi == nullptr)
      korijen = trazeni;
    else {
      if (trazeni->kljuc < drugi->kljuc)
        drugi->lijevi = trazeni;
      else
        drugi->desni = trazeni;
    }
    trazeni->roditelj = drugi;
    n++;
  }
  return trazeni->vrijednost;
}

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
  int n; // Broj elemenata
  int kapacitet;
  pair<TipKljuca, TipVrijednosti> *elementi = nullptr;
  unsigned int (*hashFunkcija)(TipKljuca, unsigned int) = nullptr;

public:
  HashMapa() {
    elementi = nullptr;
    kapacitet = 100;
    n = 0;
    elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];
  }

  HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &mapa);
  HashMapa(HashMapa<TipKljuca, TipVrijednosti> &&mapa);
  HashMapa &operator=(const HashMapa<TipKljuca, TipVrijednosti> &mapa);
  HashMapa &operator=(HashMapa<TipKljuca, TipVrijednosti> &&mapa);

  ~HashMapa() { delete[] elementi; }

  TipVrijednosti &operator[](TipKljuca kljuc);
  TipVrijednosti operator[](TipKljuca kljuc) const;
  void obrisi();
  void obrisi(const TipKljuca &kljuc);
  int brojElemenata() const { return n; };
  void definisiHashFunkciju(unsigned int (*funkcija)(TipKljuca, unsigned int)) {
    hashFunkcija = funkcija;
  };
};

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi() {
  delete[] elementi;
  elementi = nullptr;
  n = 0;
  kapacitet = 100;
  elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  if (n == 0)
    throw domain_error("Mapa je prazna");

  int pozicija = -1;
  for (int i = 0; i < kapacitet; i++) {
    if (elementi[i].first == kljuc) {
      pozicija = i;
      break;
    }
  }

  if (pozicija == -1)
    throw domain_error("Element ne postoji u ovoj mapi!");

  elementi[pozicija].first = TipKljuca();
  elementi[pozicija].second = TipVrijednosti();
  n--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
HashMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) {
  if (!hashFunkcija)
    throw std::logic_error("Niste definisali hash funkciju!");
  int i;
  for (i = 0; i < kapacitet; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;

  int index = hashFunkcija(kljuc, kapacitet);

  while (index < kapacitet &&
         elementi[index].first !=
             TipKljuca()) // Trazi se slobodno mjesto za novog clana
    index++;

  if (index == kapacitet) {
    pair<TipKljuca, TipVrijednosti> *novi =
        new pair<TipKljuca, TipVrijednosti>[kapacitet + 100];
    for (int i = 0; i < kapacitet; i++)
      novi[i] = elementi[i];

    delete[] elementi;
    elementi = novi;
    kapacitet += 100;
  }
  elementi[index].first = kljuc;
  n++;
  return elementi[index].second;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti
HashMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) const {
  for (int i = 0; i < kapacitet; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;
  return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(
    const HashMapa<TipKljuca, TipVrijednosti> &mapa) {
  if (&mapa == this)
    return;

  n = mapa.n;
  hashFunkcija = mapa.hashFunkcija;
  kapacitet = mapa.kapacitet;
  elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];

  for (int i = 0; i < kapacitet; i++) {
    elementi[i].first = mapa.elementi[i].first;
    elementi[i].second = mapa.elementi[i].second;
  }
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(
    HashMapa<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return;
  n = mapa.n;
  kapacitet = mapa.kapacitet;
  hashFunkcija = mapa.hashFunkcija;
  swap(mapa.elementi, elementi);
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &
HashMapa<TipKljuca, TipVrijednosti>::operator=(
    const HashMapa<TipKljuca, TipVrijednosti> &mapa) {
  if (&mapa == this)
    return *this;

  delete[] elementi;
  elementi = new pair<TipKljuca, TipVrijednosti>[mapa.kapacitet];
  n = mapa.n;
  hashFunkcija = mapa.hashFunkcija;
  kapacitet = mapa.kapacitet;
  for (int i = 0; i < kapacitet; i++) {
    elementi[i].first = mapa.elementi[i].first;
    elementi[i].second = mapa.elementi[i].second;
  }
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &
HashMapa<TipKljuca, TipVrijednosti>::operator=(
    HashMapa<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return *this;

  n = mapa.n;
  hashFunkcija = mapa.hashFunkcija;
  kapacitet = mapa.kapacitet;
  swap(mapa.elementi, elementi);

  return *this;
}

string generisiRandomString(int duzina) {
  string privremeni;
  string karakteri =
      "ABCDEFGHIJKLMNOPQRSTVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  srand(time(NULL));
  for (int i = 0; i < duzina; i++)
    privremeni.push_back(rand() % 61);

  return privremeni;
}

int main() {
  HashMapa<string, int> hashMapa;
  BinStabloMapa<string, int> binarnaMapa;
  NizMapa<string, int> nizMapa;

  vector<string> randomKljucevi(10000);
  vector<int> randomVrijednosti(10000);
  srand(time(NULL));
  for (int i = 0; i < 10000; i++) {
    randomKljucevi.at(i) = generisiRandomString(rand() % 15);
    randomVrijednosti.at(i) = rand() % 1000;
  }

  hashMapa.definisiHashFunkciju(ukradenaHashFunkcija);

  clock_t vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    hashMapa[randomKljucevi.at(i)] = randomVrijednosti.at(i);
  }

  clock_t vrijeme2 = clock();
  cout << "Ubacivanje: " << endl;
  cout << "Hash mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    binarnaMapa[randomKljucevi.at(i)] = randomVrijednosti.at(i);
  }
  vrijeme2 = clock();
  cout << "Binarno stablo mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    nizMapa[randomKljucevi.at(i)] = randomVrijednosti.at(i);
  }

  vrijeme2 = clock();
  cout << "Niz mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  // HashMapa kod ubacivanja elemenata ima losije vrijeme izvrsavanja
  // u odnosu na Niz i BinarnoStablo

  cout << "Pristup elementima: " << endl;

  vrijeme1 = clock();

  for (int i = 0; i < 10000; i++) {
    hashMapa[randomKljucevi.at(i)];
  }

  vrijeme1 = clock();
  cout << "Hash mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  vrijeme2 = clock();
  for (int i = 0; i < 10000; i++) {
    binarnaMapa[randomKljucevi.at(i)];
  }

  vrijeme2 = clock();
  cout << "Binarna mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  vrijeme1 = clock();

  for (int i = 0; i < 10000; i++) {
    nizMapa[randomKljucevi.at(i)];
  }

  vrijeme2 = clock();
  cout << "Niz mapa: " << (vrijeme2 - vrijeme1) << " ms" << endl;

  // Identicna situacija je i kod pristupa elemenata

  return 0;
}
