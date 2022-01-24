#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

template <typename Tip> class Grana;

template <typename Tip> class Cvor;

template <typename Tip> class GranaIterator;

template <typename Tip> class UsmjereniGraf {
public:
  UsmjereniGraf(int BrojCvorova){};
  virtual ~UsmjereniGraf(){};
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvrova(int n) = 0;
  virtual void dodajGranu(int polazniCvor, int dolazniCvor,
                          float tezinaGrane) = 0;
  virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;
  virtual float dajTezinuGrane(int polazniCvor, int dolazniCvor) const = 0;
  virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                                  float tezina) = 0;
  virtual bool postojiGrana(int polazniCvor, int dolazniCvor) const = 0;
  virtual void postaviOznakuCvora(int n, Tip oznakaCvora) = 0;
  virtual Tip dajOznakuCvora(int n) const = 0;
  virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                  Tip oznaka) = 0;
  virtual Tip dajOznakuGrane(int polazniCvor, int dolazniCvor) const = 0;
  virtual GranaIterator<Tip> dajGranePocetak() = 0;
  virtual GranaIterator<Tip> dajGraneKraj() = 0;
  virtual GranaIterator<Tip> dajSljedecuGranu(int polazniCvor,
                                              int dolazniCvor) = 0;
  Grana<Tip> dajGranu(int polazniCvor, int dolazniCvor) {
    return Grana<Tip>(this, polazniCvor, dolazniCvor);
  };
  Cvor<Tip> dajCvor(int n) { return Cvor<Tip>(this, n); }
};

template <typename Tip> class Cvor {
  int n;
  UsmjereniGraf<Tip> *graph;

public:
  Cvor(UsmjereniGraf<Tip> *g, int n) {
    this->n = n;
    graph = g;
  }
  Tip dajOznaku() const { return graph->dajOznakuCvora(n); };
  void postaviOznaku(Tip oznaka) { graph->postaviOznakuCvora(n, oznaka); };
  int dajRedniBroj() const { return n; };
};

template <typename Tip> class Grana {
  UsmjereniGraf<Tip> *graph;
  int polazniCvor;
  int dolazniCvor;

public:
  Grana(UsmjereniGraf<Tip> *graph, int polazniCvor, int dolazniCvor) {
    this->graph = graph;
    this->polazniCvor = polazniCvor;
    this->dolazniCvor = dolazniCvor;
  }
  float dajTezinu() const;
  void postaviTezinu(float tezina);
  Tip dajOznaku() const;
  void postaviOznaku(Tip oznaka);
  Cvor<Tip> dajPolazniCvor() const;
  Cvor<Tip> dajDolazniCvor() const;
};

template <typename Tip> float Grana<Tip>::dajTezinu() const {
  return graph->dajTezinuGrane(polazniCvor, dolazniCvor);
}

template <typename Tip> void Grana<Tip>::postaviTezinu(float tezina) {
  graph->postaviTezinuGrane(polazniCvor, dolazniCvor, tezina);
}

template <typename Tip> Tip Grana<Tip>::dajOznaku() const {
  return graph->dajOznakuGrane(polazniCvor, dolazniCvor);
}

template <typename Tip> void Grana<Tip>::postaviOznaku(Tip oznaka) {
  graph->postaviOznakuGrane(polazniCvor, dolazniCvor, oznaka);
}

template <typename Tip> Cvor<Tip> Grana<Tip>::dajPolazniCvor() const {
  return graph->dajCvor(polazniCvor);
}

template <typename Tip> Cvor<Tip> Grana<Tip>::dajDolazniCvor() const {
  return graph->dajCvor(dolazniCvor);
}

template <typename Tip> class GranaIterator {
  UsmjereniGraf<Tip> *graph;
  int polazniCvor;
  int dolazniCvor;

public:
  GranaIterator(UsmjereniGraf<Tip> *graph, int polazniCvor, int dolazniCvor) {
    this->graph = graph;
    this->polazniCvor = polazniCvor;
    this->dolazniCvor = dolazniCvor;
  };
  Grana<Tip> operator*();
  bool operator==(const GranaIterator &it) const;
  bool operator!=(const GranaIterator &it) const;
  GranaIterator<Tip> &operator++();
  GranaIterator<Tip> &operator++(int);
};

template <typename Tip> Grana<Tip> GranaIterator<Tip>::operator*() {
  return Grana<Tip>(graph, polazniCvor, dolazniCvor);
}

template <typename Tip>
bool GranaIterator<Tip>::operator==(const GranaIterator &it) const {
  bool prvi = graph == it.graph;
  bool drugi = polazniCvor == it.polazniCvor;
  bool treci = dolazniCvor == it.dolazniCvor;
  return (prvi && drugi && treci);
}

template <typename Tip>
bool GranaIterator<Tip>::operator!=(const GranaIterator &it) const {
  return !(*this == it);
}

template <typename Tip> GranaIterator<Tip> &GranaIterator<Tip>::operator++() {
  if (polazniCvor == -1 && dolazniCvor == -1)
    throw domain_error("Nedozvoljena operacija!");

  GranaIterator grana = graph->dajSljedecuGranu(polazniCvor, dolazniCvor);
  polazniCvor = grana.polazniCvor;
  dolazniCvor = grana.dolazniCvor;
  return *this;
}

template <typename Tip>
GranaIterator<Tip> &GranaIterator<Tip>::operator++(int) {
  GranaIterator privremeni(*this);
  ++(*this);
  return privremeni;
}

template <typename Tip> class MatricaGraf : public UsmjereniGraf<Tip> {
  struct Ogranak {
    Tip oznaka;
    float tezina;
    bool ima;
  };
  vector<vector<Ogranak>> matrica;
  vector<Tip> oznake;

public:
  MatricaGraf(int n) : UsmjereniGraf<Tip>(n) {
    oznake.resize(n, Tip());
    Ogranak none;
    none.ima = false;
    matrica.resize(n);
    for (int i = 0; i < n; i++) {
      matrica.at(i).resize(n);
      fill(matrica.at(i).begin(), matrica.at(i).end(), none);
    }
  }

  int dajBrojCvorova() const { return matrica.size(); };
  void postaviBrojCvrova(int n);
  void dodajGranu(int polazniCvor, int dolazniCvor, float tezinaGrane);
  void obrisiGranu(int polazniCvor, int dolazniCvor);
  void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina);
  float dajTezinuGrane(int polazniCvor, int dolazniCvor) const;
  bool postojiGrana(int polazniCvor, int dolazniCvor) const;
  void postaviOznakuCvora(int n, Tip oznakaCvora);
  Tip dajOznakuCvora(int n) const { return oznake.at(n); };
  void postaviOznakuGrane(int polazniCvor, int dolazniCvor, Tip oznaka);
  Tip dajOznakuGrane(int polazniCvor, int dolazniCvor) const {
    return matrica.at(polazniCvor).at(dolazniCvor).oznaka;
  };
  GranaIterator<Tip> dajGranePocetak();
  GranaIterator<Tip> dajGraneKraj();
  GranaIterator<Tip> dajSljedecuGranu(int polazniCvor, int dolazniCvor);
};

template <typename Tip> void MatricaGraf<Tip>::postaviBrojCvrova(int n) {
  if (n < matrica.size())
    throw range_error("Broj cvorova se ne moze smanjiti!");
  oznake.resize(n, Tip());
  Ogranak none;
  none.ima = false;
  matrica.resize(n);
  for (int i = 0; i < n; i++) {
    matrica.at(i).resize(n);
    fill(matrica.at(i).begin(), matrica.at(i).end(), none);
  }
  vector<Ogranak> prazan(n, none);
  matrica.resize(n, prazan);
}

template <typename Tip>
void MatricaGraf<Tip>::dodajGranu(int polazniCvor, int dolazniCvor,
                                  float tezinaGrane) {
  Ogranak ogranak;
  ogranak.tezina = tezinaGrane;
  ogranak.ima = true;
  matrica.at(polazniCvor).at(dolazniCvor) = ogranak;
}

template <typename Tip>
void MatricaGraf<Tip>::obrisiGranu(int polazniCvor, int dolazniCvor) {
  matrica.at(polazniCvor).at(dolazniCvor).ima = false;
}

template <typename Tip>
void MatricaGraf<Tip>::postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                                          float tezina) {
  matrica.at(polazniCvor).at(dolazniCvor).tezina = tezina;
}

template <typename Tip>
float MatricaGraf<Tip>::dajTezinuGrane(int polazniCvor, int dolazniCvor) const {
  return matrica.at(polazniCvor).at(dolazniCvor).tezina;
}

template <typename Tip>
bool MatricaGraf<Tip>::postojiGrana(int polazniCvor, int dolazniCvor) const {
  return matrica.at(polazniCvor).at(dolazniCvor).ima;
}

template <typename Tip>
void MatricaGraf<Tip>::postaviOznakuCvora(int n, Tip oznakaCvora) {
  oznake.at(n) = oznakaCvora;
}

template <typename Tip>
void MatricaGraf<Tip>::postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                          Tip oznaka) {
  matrica.at(polazniCvor).at(dolazniCvor).oznaka = oznaka;
}

template <typename Tip> GranaIterator<Tip> MatricaGraf<Tip>::dajGranePocetak() {
  GranaIterator<Tip> iteratr(this, 0, -1);
  return ++iteratr;
}

template <typename Tip> GranaIterator<Tip> MatricaGraf<Tip>::dajGraneKraj() {
  GranaIterator<Tip> iterator(this, -1, -1);
  return iterator;
}

template <typename Tip>
GranaIterator<Tip> MatricaGraf<Tip>::dajSljedecuGranu(int polazniCvor,
                                                      int dolazniCvor) {
  for (int i = polazniCvor; i < matrica.size(); i++) {
    for (int j = 0; j < matrica.size(); j++) {
      if (i == polazniCvor && j <= dolazniCvor)
        continue;
      else if (matrica.at(i).at(j).ima)
        return GranaIterator<Tip>(this, i, j);
    }
  }
  return GranaIterator<Tip>(this, -1, -1);
}

template <typename Tip> class ListaGraf : public UsmjereniGraf<Tip> {
  struct Ogranak {
    Tip oznaka;
    float tezina;
    bool ima;
  };
  vector<vector<Ogranak>> matrica;
  vector<Tip> oznake;

public:
  ListaGraf(int n) : UsmjereniGraf<Tip>(n) { postaviBrojCvrova(n); };
  int dajBrojCvorova() const { return matrica.size(); };
  void postaviBrojCvrova(int n);
  void dodajGranu(int polazniCvor, int dolazniCvor, float tezinaGrane);
  void obrisiGranu(int polazniCvor, int dolazniCvor);
  void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina);
  float dajTezinuGrane(int polazniCvor, int dolazniCvor) const;
  bool postojiGrana(int polazniCvor, int dolazniCvor) const;
  void postaviOznakuCvora(int n, Tip oznakaCvora);
  Tip dajOznakuCvora(int n) const { return oznake.at(n); };
  void postaviOznakuGrane(int polazniCvor, int dolazniCvor, Tip oznaka);
  Tip dajOznakuGrane(int polazniCvor, int dolazniCvor) const {
    return matrica.at(polazniCvor).at(dolazniCvor).oznaka;
  };
  GranaIterator<Tip> dajSljedecuGranu(int polazniCvor, int dolazniCvor);
  GranaIterator<Tip> dajGranePocetak();
  GranaIterator<Tip> dajGraneKraj();
};

template <typename Tip> void ListaGraf<Tip>::postaviBrojCvrova(int n) {
  if (n < matrica.size())
    throw range_error("Broj cvorova se ne moze smanjiti!");
  Ogranak none;
  none.ima = false;
  for (int i = 0; i < matrica.size(); i++) {
    matrica.at(i).resize(n);
    fill(matrica.at(i).begin(), matrica.at(i).end(), none);
  }
  vector<Ogranak> prazan(n, none);
  matrica.resize(n, prazan);
  oznake.resize(n);
}

template <typename Tip>
void ListaGraf<Tip>::dodajGranu(int polazniCvor, int dolazniCvor,
                                float tezinaGrane) {
  Ogranak ogranak;
  ogranak.tezina = tezinaGrane;
  ogranak.ima = true;
  matrica.at(polazniCvor).at(dolazniCvor) = ogranak;
}

template <typename Tip>
void ListaGraf<Tip>::obrisiGranu(int polazniCvor, int dolazniCvor) {
  matrica.at(polazniCvor).at(dolazniCvor).ima = false;
}

template <typename Tip>
void ListaGraf<Tip>::postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                                        float tezina) {
  matrica.at(polazniCvor).at(dolazniCvor).tezina = tezina;
}

template <typename Tip>
float ListaGraf<Tip>::dajTezinuGrane(int polazniCvor, int dolazniCvor) const {
  return matrica.at(polazniCvor).at(dolazniCvor).tezina;
}

template <typename Tip>
bool ListaGraf<Tip>::postojiGrana(int polazniCvor, int dolazniCvor) const {
  return matrica.at(polazniCvor).at(dolazniCvor).ima;
}

template <typename Tip>
void ListaGraf<Tip>::postaviOznakuCvora(int n, Tip oznakaCvora) {
  oznake.at(n) = oznakaCvora;
}

template <typename Tip>
void ListaGraf<Tip>::postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                        Tip oznaka) {
  matrica.at(polazniCvor).at(dolazniCvor).oznaka = oznaka;
}

template <typename Tip>
GranaIterator<Tip> ListaGraf<Tip>::dajSljedecuGranu(int polazniCvor,
                                                    int dolazniCvor) {
  for (int i = polazniCvor; i < matrica.size(); i++) {
    for (int j = 0; j < matrica.size(); j++) {
      if (i == polazniCvor && j <= dolazniCvor)
        continue;
      else if (matrica.at(i).at(j).ima)
        return GranaIterator<Tip>(this, i, j);
    }
  }
  return GranaIterator<Tip>(this, -1, -1);
}

template <typename Tip> GranaIterator<Tip> ListaGraf<Tip>::dajGranePocetak() {
  GranaIterator<Tip> iteratr(this, 0, -1);
  return ++iteratr;
}

template <typename Tip> GranaIterator<Tip> ListaGraf<Tip>::dajGraneKraj() {
  GranaIterator<Tip> iterator(this, -1, -1);
  return iterator;
}

template <typename Tip>
void bfs(UsmjereniGraf<Tip> *graf, vector<Cvor<Tip>> &obilazak,
         Cvor<Tip> cvor) {
  bool bio = false;
  for (int i = 0; i < obilazak.size(); i++) {
    if (cvor.dajRedniBroj() == obilazak.at(i).dajRedniBroj()) {
      bio = true;
      break;
    }
  }

  if (!bio)
    obilazak.push_back(cvor);
  for (int i = 0; i < graf->dajBrojCvorova(); i++) {
    if (!bio)
      bfs(graf, obilazak, graf->dajCvor(i));
  }
}

template <typename Tip>
void dfs(UsmjereniGraf<Tip> *graf, vector<Cvor<Tip>> &obilazak,
         Cvor<Tip> cvor) {
  bool bio = false;
  for (int i = 0; i < obilazak.size(); i++) {
    if (cvor.dajRedniBroj() == obilazak.at(i).dajRedniBroj()) {
      bio = true;
      break;
    }
  }
  if (!bio)
    obilazak.push_back(cvor);
  for (int i = 0; i < graf->dajBrojCvorova(); i++) {
    if (graf->postojiGrana(cvor.dajRedniBroj(), i) && !bio) {
      dfs(graf, obilazak, graf->dajCvor(i));
    }
  }
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

template <typename TipKljuca, typename TipVrijednost> struct CvorMapa {
  TipKljuca kljuc;
  TipVrijednost vrijednost;
  CvorMapa *sljedeci = nullptr;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
  int n; // Broj elemenata
  int kapacitet;
  CvorMapa<TipKljuca, TipVrijednosti> **nizCvorova;
  pair<TipKljuca, TipVrijednosti> *elementi = nullptr;
  unsigned int (*hashFunkcija)(TipKljuca, unsigned int) = nullptr;

public:
  HashMapaLan() {
    elementi = nullptr;
    kapacitet = 100;
    n = 0;
    elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];
  }

  HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa);
  HashMapaLan(HashMapaLan<TipKljuca, TipVrijednosti> &&mapa);
  HashMapaLan &operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa);
  HashMapaLan &operator=(HashMapaLan<TipKljuca, TipVrijednosti> &&mapa);

  ~HashMapaLan() { delete[] elementi; }

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
void HashMapaLan<TipKljuca, TipVrijednosti>::obrisi() {
  delete[] elementi;
  elementi = nullptr;
  n = 0;
  kapacitet = 100;
  elementi = new pair<TipKljuca, TipVrijednosti>[kapacitet];
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapaLan<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
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
HashMapaLan<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) {
  if (!hashFunkcija)
    throw std::logic_error("Niste definisali hash funkciju!");
  int i;
  for (i = 0; i < kapacitet; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;

  int index = hashFunkcija(kljuc, kapacitet);

  while (index < kapacitet &&
         elementi[index].first !=
             TipKljuca()) // Trazimo slobodno mjesto za novi clan
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
HashMapaLan<TipKljuca, TipVrijednosti>::operator[](TipKljuca kljuc) const {
  for (int i = 0; i < kapacitet; i++)
    if (elementi[i].first == kljuc)
      return elementi[i].second;
  return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapaLan<TipKljuca, TipVrijednosti>::HashMapaLan(
    const HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
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
HashMapaLan<TipKljuca, TipVrijednosti>::HashMapaLan(
    HashMapaLan<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return;
  n = mapa.n;
  kapacitet = mapa.kapacitet;
  hashFunkcija = mapa.hashFunkcija;
  swap(mapa.elementi, elementi);
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapaLan<TipKljuca, TipVrijednosti> &
HashMapaLan<TipKljuca, TipVrijednosti>::operator=(
    const HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
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
HashMapaLan<TipKljuca, TipVrijednosti> &
HashMapaLan<TipKljuca, TipVrijednosti>::operator=(
    HashMapaLan<TipKljuca, TipVrijednosti> &&mapa) {
  if (&mapa == this)
    return *this;

  n = mapa.n;
  hashFunkcija = mapa.hashFunkcija;
  kapacitet = mapa.kapacitet;
  swap(mapa.elementi, elementi);

  return *this;
}

int main() {}
