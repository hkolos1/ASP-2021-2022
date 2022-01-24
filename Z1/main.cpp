#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

template <typename Tip> class Lista {
public:
  Lista(){};
  virtual ~Lista(){};
  virtual int brojElemenata() const = 0;
  virtual Tip &trenutni() = 0;
  virtual Tip trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void obrisi() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &operator[](int n) = 0;
  virtual Tip operator[](int n) const = 0;
};

template <typename Tip> struct Cvor {
  Tip vrijednost;
  Cvor *prethodni = nullptr;
  Cvor *sljedeci = nullptr;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
  Cvor<Tip> *trenutniClan = nullptr;
  Cvor<Tip> *pocetakListe = nullptr;
  Cvor<Tip> *krajListe = nullptr;
  int velicina = 0;

public:
  DvostrukaLista(){};
  ~DvostrukaLista();

  DvostrukaLista(const DvostrukaLista &lista);
  DvostrukaLista(DvostrukaLista &&lista);

  DvostrukaLista &operator=(const DvostrukaLista &lista);
  DvostrukaLista &operator=(DvostrukaLista &&lista);

  int brojElemenata() const { return velicina; };
  Tip &trenutni() { return trenutniClan->vrijednost; };
  Tip trenutni() const { return trenutniClan->vrijednost; };
  bool prethodni();
  bool sljedeci();
  void obrisi();
  void pocetak();
  void kraj();
  void dodajIspred(const Tip &el);
  void dodajIza(const Tip &el);
  Tip &operator[](int n);
  Tip operator[](int n) const;
};
template <typename Tip> DvostrukaLista<Tip>::~DvostrukaLista() {
  while (pocetakListe != nullptr) {
    Cvor<Tip> *rezervni = pocetakListe;
    pocetakListe = pocetakListe->sljedeci;
    delete rezervni;
  }
  trenutniClan = nullptr;
  pocetakListe = nullptr;
  krajListe = nullptr;
  velicina = 0;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &lista) {
  Cvor<Tip> *rezervni = lista.pocetakListe;
  while (rezervni != nullptr) {
    dodajIza(rezervni->vrijednost);
    sljedeci();
    rezervni = rezervni->sljedeci;
  }
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(DvostrukaLista &&lista) {
  pocetakListe = lista.pocetakListe;
  krajListe = lista.krajListe;
  trenutniClan = lista.trenutniClan;
  velicina = lista.velicina;

  lista.velicina = 0;
  lista.pocetakListe = nullptr;
  lista.krajListe = nullptr;
  lista.trenutniClan = nullptr;
}

template <typename Tip>
DvostrukaLista<Tip> &
DvostrukaLista<Tip>::operator=(const DvostrukaLista &lista) {
  if (this == &lista)
    return *this;

  while (trenutniClan != nullptr) {
    obrisi();
  }

  Cvor<Tip> *rezervni = lista.pocetakListe;
  while (rezervni != nullptr) {
    dodajIza(rezervni->vrijednost);
    sljedeci();
    rezervni = rezervni->sljedeci;
  }
  return *this;
}
template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(DvostrukaLista &&lista) {
  if (this == &lista)
    return *this;

  while (trenutniClan != nullptr) {
    obrisi();
  }

  pocetakListe = lista.pocetakListe;
  krajListe = lista.krajListe;
  trenutniClan = lista.trenutniClan;
  velicina = lista.velicina;

  lista.velicina = 0;
  lista.pocetakListe = nullptr;
  lista.krajListe = nullptr;
  lista.trenutniClan = nullptr;

  return *this;
}

template <typename Tip> bool DvostrukaLista<Tip>::prethodni() {
  if (trenutniClan->prethodni == nullptr)
    return false;
  trenutniClan = trenutniClan->prethodni;
  return true;
}

template <typename Tip> bool DvostrukaLista<Tip>::sljedeci() {
  if (trenutniClan->sljedeci == nullptr)
    return false;
  trenutniClan = trenutniClan->sljedeci;
  return true;
}

template <typename Tip> void DvostrukaLista<Tip>::obrisi() {

  if (velicina == 1) {
    delete trenutniClan;
    trenutniClan = nullptr;
    pocetakListe = nullptr;
    velicina = 0;
    return;
  }

  if (trenutniClan->prethodni == nullptr) {
    trenutniClan = trenutniClan->sljedeci;
    delete trenutniClan->prethodni;
    trenutniClan->prethodni = nullptr;
    pocetakListe = trenutniClan;
    velicina--;
    return;
  }

  if (trenutniClan->sljedeci == nullptr) {
    trenutniClan = trenutniClan->prethodni;
    delete trenutniClan->sljedeci;
    trenutniClan->sljedeci = nullptr;
    krajListe = trenutniClan;
    velicina--;
    return;
  }

  Cvor<Tip> *rezervni = trenutniClan->prethodni;
  trenutniClan = trenutniClan->sljedeci;
  delete trenutniClan->prethodni;
  trenutniClan->prethodni = rezervni;
  rezervni->sljedeci = trenutniClan;
  velicina--;
}

template <typename Tip> void DvostrukaLista<Tip>::pocetak() {
  trenutniClan = pocetakListe;
}

template <typename Tip> void DvostrukaLista<Tip>::kraj() {
  trenutniClan = krajListe;
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
  if (trenutniClan == nullptr) {
    Cvor<Tip> *novi = new Cvor<Tip>();
    novi->vrijednost = el;
    novi->prethodni = nullptr;
    novi->sljedeci = nullptr;
    trenutniClan = novi;
    velicina++;
    pocetakListe = novi;
    krajListe = novi;
    return;
  }

  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;

  novi->prethodni = trenutniClan->prethodni;
  novi->sljedeci = trenutniClan;

  if (novi->prethodni != nullptr)
    novi->prethodni->sljedeci = novi;
  else
    pocetakListe = novi;
  trenutniClan->prethodni = novi;

  velicina++;
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIza(const Tip &el) {
  if (trenutniClan == nullptr) {
    Cvor<Tip> *novi = new Cvor<Tip>();
    novi->vrijednost = el;
    novi->prethodni = nullptr;
    novi->sljedeci = nullptr;
    pocetakListe = novi;
    trenutniClan = novi;
    velicina++;
    return;
  }

  Cvor<Tip> *novi = new Cvor<Tip>();
  novi->vrijednost = el;

  novi->prethodni = trenutniClan;
  novi->sljedeci = trenutniClan->sljedeci;

  if (trenutniClan->sljedeci != nullptr)
    trenutniClan->sljedeci->prethodni = novi;
  else
    krajListe = novi;
  trenutniClan->sljedeci = novi;

  velicina++;
}

template <typename Tip> Tip &DvostrukaLista<Tip>::operator[](int n) {
  if (n < 0 || n >= velicina)
    throw std::range_error("Neispravan indeks!");
  Cvor<Tip> *pomocni = pocetakListe;
  int brojac = 0;
  while (brojac != n) {
    pomocni = pomocni->sljedeci;
    brojac++;
  }
  return pomocni->vrijednost;
}

template <typename Tip> Tip DvostrukaLista<Tip>::operator[](int n) const {
  if (n < 0 || n >= velicina)
    throw std::range_error("Neispravan indeks!");
  Cvor<Tip> *pomocni = pocetakListe;
  int brojac = 0;
  while (brojac != n) {
    pomocni = pomocni->sljedeci;
    brojac++;
  }
  return pomocni->vrijednost;
}

template <typename Tip> Tip dajMaksimum(const Lista<Tip> &n) {
  if (!n.brojElemenata())
    throw std::domain_error("Lista je prazna!");

  DvostrukaLista<Tip> nova =
      dynamic_cast<DvostrukaLista<Tip> &>(const_cast<Lista<Tip> &>(n));

  nova.pocetak();
  Tip maksimum = nova.trenutni();

  while (nova.sljedeci())
    if (nova.trenutni() > maksimum)
      maksimum = nova.trenutni();

  return maksimum;
}

int main() { return 0; }
