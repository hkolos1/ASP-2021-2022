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

int main() {
  try {
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
    g->dodajGranu(0, 1, 2);
    g->dodajGranu(1, 0, 2.5);
    g->dodajGranu(1, 1, 0);
    g->dodajGranu(0, 0, 3);
    for (GranaIterator<bool> iterator = g->dajGranePocetak();
         iterator != g->dajGraneKraj(); ++iterator)
      cout << (*iterator).dajPolazniCvor().dajRedniBroj() << " "
           << (*iterator).dajDolazniCvor().dajRedniBroj() << " "
           << (*iterator).dajTezinu() << endl;

  } catch (const char izuzetak[]) {
    cout << izuzetak << endl;
  }

  return 0;
}
