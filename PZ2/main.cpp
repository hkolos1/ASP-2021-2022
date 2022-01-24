#include <iostream>

using namespace std;

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

template <typename Tip> class NizLista : public Lista<Tip> {
protected:
  int brElemenata;
  int maxKapacitet;
  int trenutniClan;
  Tip *niz = nullptr;
  void Isprazni(Tip *niz) {
    if (niz != nullptr)
      delete[] niz;
    niz = nullptr;
  }
  void PovecajKapacitet(int trenutniKapacitet) {
    Tip *noviNiz = nullptr;
    try {
      noviNiz = new Tip[trenutniKapacitet + 100];
    } catch (...) {
      throw domain_error("Nedovoljno memorije!");
    }
    for (int i = 0; i < trenutniKapacitet; i++)
      noviNiz[i] = niz[i];
    Isprazni(niz);
    niz = noviNiz;
    noviNiz = nullptr;
    maxKapacitet = trenutniKapacitet + 100;
  }

public:
  NizLista() {
    brElemenata = 0;
    trenutniClan = 0;
    PovecajKapacitet(0);
  };
  ~NizLista() {
    Isprazni(niz);
    brElemenata = 0;
    maxKapacitet = 0;
  }
  NizLista(const NizLista &nl) {
    brElemenata = nl.brElemenata;
    maxKapacitet = nl.maxKapacitet;
    trenutniClan = nl.trenutniClan;

    niz = new Tip[maxKapacitet];
    for (int i = 0; i < nl.brElemenata; i++) {
      niz[i] = nl.niz[i];
    }
  }
  NizLista &operator=(const NizLista &nizLista) {
    if (this == &nizLista)
      return *this;

    delete[] niz;
    niz = nullptr;
    maxKapacitet = nizLista.maxKapacitet;
    trenutniClan = nizLista.trenutniClan;
    brElemenata = nizLista.brElemenata;
    try {
      niz = new int[nizLista.maxKapacitet];
    } catch (...) {
      throw domain_error("Nedovoljno memorije!");
    }

    for (int i = 0; i < maxKapacitet; i++) {
      niz[i] = nizLista[i];
    }
    return *this;
  }

  int brojElemenata() const { return brElemenata; };
  Tip &trenutni() {
    if (trenutniClan > brElemenata)
      throw range_error("Lista je prazna");
    return niz[trenutniClan - 1];
  };
  Tip trenutni() const {
    if (trenutniClan > brElemenata)
      throw range_error("Lista je prazna");
    return niz[trenutniClan - 1];
  };
  bool prethodni();
  bool sljedeci();
  void obrisi();
  void dodajIza(const Tip &el);
  void dodajIspred(const Tip &el);
  Tip &operator[](int n) { return niz[n]; };
  Tip operator[](int n) const { return niz[n]; };
  void pocetak() { trenutniClan = 1; };
  void kraj() { trenutniClan = brElemenata; };
};
template <typename Tip> bool NizLista<Tip>::prethodni() {
  if (trenutniClan == 0) {
    return false;
  }
  trenutniClan--;
  return true;
}
template <typename Tip> bool NizLista<Tip>::sljedeci() {
  if (trenutniClan == brElemenata) {
    return false;
  }
  trenutniClan++;
  return true;
}

template <typename Tip> void NizLista<Tip>::obrisi() {
  if (!brElemenata)
    throw domain_error("Lista je vec prazna");
  for (int i = trenutniClan; i < brElemenata - 1; i++)
    niz[i] = niz[i + 1];
  brElemenata--;
  (trenutniClan == brElemenata - 1) ? trenutniClan++ : trenutniClan--;
}

template <typename Tip> void NizLista<Tip>::dodajIza(const Tip &el) {
  if (brElemenata == maxKapacitet)
    PovecajKapacitet(maxKapacitet);
  if (brElemenata == 0) {
    brElemenata++;
    niz[0] = el;
    trenutniClan = 1;
    return;
  }
  brElemenata++;
  for (int i = brElemenata - 1; i > trenutniClan; i--) {
    niz[i] = niz[i - 1];
  }
  niz[trenutniClan] = el;
}
template <typename Tip> void NizLista<Tip>::dodajIspred(const Tip &el) {
  if (brElemenata == maxKapacitet)
    PovecajKapacitet(maxKapacitet);
  if (brElemenata == 0) {
    brElemenata++;
    niz[0] = el;
    trenutniClan = 1;
    return;
  }
  brElemenata++;
  for (int i = brElemenata - 1; i >= trenutniClan; i--) {
    niz[i] = niz[i - 1];
  }
  niz[trenutniClan - 1] = el;
  trenutniClan++;
}
template <typename Tip> struct Cvor {
  Tip vrijednost;
  Cvor *sljedeci = nullptr;
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  Cvor<Tip> *pocetakListe;
  Cvor<Tip> *trenutniClan;
  int brElemenata;

public:
  JednostrukaLista() {
    pocetakListe = nullptr;
    trenutniClan = pocetakListe;
    brElemenata = 0;
  }
  JednostrukaLista(const JednostrukaLista &l) {
    brElemenata = l.brElemenata;
    pocetakListe = new Cvor<Tip>();
    pocetakListe->vrijednost = l.pocetakListe->vrijednost;
    trenutniClan = pocetakListe;
    Cvor<Tip> *noviTrenutniClan = pocetakListe;
    Cvor<Tip> *StaraLista = l.pocetakListe->sljedeci;
    for (int i = 0; i < brElemenata - 1; i++) {
      noviTrenutniClan->sljedeci = new Cvor<Tip>();
      noviTrenutniClan = noviTrenutniClan->sljedeci;
      if (StaraLista == l.trenutniClan)
        trenutniClan = noviTrenutniClan;
      noviTrenutniClan->vrijednost = StaraLista->vrijednost;
      StaraLista = StaraLista->sljedeci;
    }
  }
  JednostrukaLista &operator=(const JednostrukaLista &l) {
    if (this == &l)
      return *this;
    if (brElemenata > 0) {
      while (true) {
        Cvor<Tip> *zaBrisati = pocetakListe;
        pocetakListe = pocetakListe->sljedeci;
        delete zaBrisati;
        if (pocetakListe == nullptr)
          break;
      }
    }
    brElemenata = l.brElemenata;
    pocetakListe = new Cvor<Tip>();
    pocetakListe->vrijednost = l.pocetakListe->vrijednost;
    trenutniClan = pocetakListe;
    Cvor<Tip> *noviTrenutniClan = pocetakListe;
    Cvor<Tip> *StaraLista = l.pocetakListe->sljedeci;
    for (int i = 0; i < brElemenata - 1; i++) {
      noviTrenutniClan->sljedeci = new Cvor<Tip>();
      noviTrenutniClan = noviTrenutniClan->sljedeci;
      if (StaraLista == l.trenutniClan)
        trenutniClan = noviTrenutniClan;
      noviTrenutniClan->vrijednost = StaraLista->vrijednost;
      StaraLista = StaraLista->sljedeci;
    }
    return *this;
  }

  ~JednostrukaLista() {
    if (brElemenata != 0) {
      while (true) {
        Cvor<Tip> *zaBrisati = pocetakListe;
        pocetakListe = pocetakListe->sljedeci;
        delete zaBrisati;
        if (pocetakListe == nullptr)
          break;
      }
    }
  }
  int brojElemenata() const { return brElemenata; };
  Tip trenutni() const { return trenutniClan->vrijednost; };
  Tip &trenutni() { return trenutniClan->vrijednost; };
  bool prethodni();
  bool sljedeci();
  void obrisi();
  void dodajIza(const Tip &el);
  void dodajIspred(const Tip &el);
  Tip &operator[](int n);
  Tip operator[](int n) const;
  void pocetak() { trenutniClan = pocetakListe; };
  void kraj() {
    while (trenutniClan->sljedeci != nullptr)
      trenutniClan = trenutniClan->sljedeci;
  };
};

template <typename Tip> bool JednostrukaLista<Tip>::prethodni() {
  if (trenutniClan == pocetakListe)
    return false;
  Cvor<Tip> *bivsiTrenutni = trenutniClan;
  trenutniClan = pocetakListe;
  while (trenutniClan->sljedeci != bivsiTrenutni) {
    trenutniClan = trenutniClan->sljedeci;
  }
  return true;
}

template <typename Tip> bool JednostrukaLista<Tip>::sljedeci() {
  if (trenutniClan->sljedeci == nullptr)
    return false;
  trenutniClan = trenutniClan->sljedeci;
  return true;
}

template <typename Tip> void JednostrukaLista<Tip>::obrisi() {
  if (trenutniClan == nullptr && pocetakListe == nullptr) {
    throw domain_error("Lista je vec prazna");
  }
  if (brElemenata == 1) {
    if (trenutniClan->sljedeci == nullptr) {
      delete trenutniClan;
      trenutniClan = nullptr;
      pocetakListe = nullptr;
      brElemenata--;
      return;
    }
  }
  Cvor<Tip> *zaBrisati = trenutniClan;
  if (trenutniClan == pocetakListe) {
    pocetakListe = trenutniClan->sljedeci;
    trenutniClan = trenutniClan->sljedeci;
  } else if (trenutniClan->sljedeci != nullptr) {
    trenutniClan = trenutniClan->sljedeci;
    Cvor<Tip> *prethodni = pocetakListe;
    while (prethodni->sljedeci != zaBrisati)
      prethodni = prethodni->sljedeci;
    prethodni->sljedeci = trenutniClan;
  } else {
    Cvor<Tip> *prethodni = pocetakListe;
    while (prethodni->sljedeci != zaBrisati)
      prethodni = prethodni->sljedeci;

    trenutniClan = prethodni;
    trenutniClan->sljedeci = nullptr;
  }
  delete zaBrisati;
  brElemenata--;
}
template <typename Tip> void JednostrukaLista<Tip>::dodajIza(const Tip &el) {
  Cvor<Tip> *novi = nullptr;
  try {
    novi = new Cvor<Tip>();
  } catch (...) {
    throw domain_error("Nedovoljno memorije!");
  }
  novi->vrijednost = el;
  novi->sljedeci = nullptr;

  if (pocetakListe == nullptr) {
    pocetakListe = novi;
    trenutniClan = novi;
    brElemenata++;
    return;
  }
  novi->sljedeci = trenutniClan->sljedeci;
  trenutniClan->sljedeci = novi;
  brElemenata++;
}
template <typename Tip> void JednostrukaLista<Tip>::dodajIspred(const Tip &el) {
  Cvor<Tip> *novi = nullptr;
  try {
    novi = new Cvor<Tip>();
  } catch (...) {
    throw domain_error("Nedovoljno memorije!");
  }
  novi->vrijednost = el;
  novi->sljedeci = trenutniClan;

  if (pocetakListe == nullptr) {
    pocetakListe = novi;
    trenutniClan = novi;
    brElemenata++;
    return;
  }
  if (trenutniClan == pocetakListe) {
    novi->sljedeci = pocetakListe;
    pocetakListe = novi;
    brElemenata++;
    return;
  }
  Cvor<Tip> *prethodni = pocetakListe;
  while (prethodni->sljedeci != trenutniClan) {
    prethodni = prethodni->sljedeci;
  }
  prethodni->sljedeci = novi;
  brElemenata++;
}

template <typename Tip> Tip &JednostrukaLista<Tip>::operator[](int n) {
  if (n < 0 || n > brElemenata - 1) {
    throw domain_error("Neispravan index");
  }
  Cvor<Tip> *trazeni = pocetakListe;
  for (int i = 0; i < n; i++)
    trazeni = trazeni->sljedeci;
  return trazeni->vrijednost;
}

template <typename Tip> Tip JednostrukaLista<Tip>::operator[](int n) const {
  if (n < 0 || n > brElemenata - 1) {
    throw domain_error("Neispravan index");
  }
  Cvor<Tip> *trazeni = pocetakListe;
  for (int i = 0; i < n; i++)
    trazeni = trazeni->sljedeci;
  return trazeni->vrijednost;
}

int main() {
  JednostrukaLista<int> lista;
  for (int i(1); i <= 5; i++)
    lista.dodajIspred(i);
  {
    JednostrukaLista<int> lista2(lista);
    JednostrukaLista<int> lista3;
    lista3 = lista;
    lista.obrisi();
    std::cout << lista2.brojElemenata();
    std::cout << " " << lista3.brojElemenata() << " ";
  }
  std::cout << lista.brojElemenata();
}