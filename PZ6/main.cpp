#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

template <typename Tip> void bubble_sort(Tip *niz, int vel) {
  for (int i = 0; i < vel - 1; i++) {
    for (int j = 0; j < vel - i - 1; j++) {
      if (niz[j] > niz[j + 1])
        swap(niz[j], niz[j + 1]);
    }
  }
}

template <typename Tip> void selection_sort(Tip *niz, int vel) {
  int index = -1;
  for (int i = 0; i < vel; i++) {
    Tip min = niz[i];
    for (int j = i + 1; j < vel; j++) {
      if (niz[j] < min) {
        min = niz[j];
        index = j;
      }
    }
    if (index != i && index != -1) {
      swap(niz[i], niz[index]);
      index = -1;
    }
  }
}

template <typename Tip> void quick_sort(Tip *niz, int vel) {
  int min = 0;
  if (vel - 1 <= min)
    return;
  swap(niz[min], niz[(min + vel) / 2]);
  for (int i = 1; i < vel; i++)
    if (niz[i] < niz[0])
      swap(niz[++min], niz[i]);
  swap(niz[0], niz[min]);
  quick_sort(niz, min);
  quick_sort(niz + min + 1, vel - min - 1);
}

template <typename Tip> void merge_sort(Tip *niz, int vel) {
  if (vel > 1) {
    int i = 0;
    int mid = vel / 2;
    Tip *left = new Tip[mid];
    Tip *right = new Tip[vel - mid];

    for (i = 0; i < mid; i++)
      left[i] = niz[i];
    for (i = mid; i < vel; i++)
      right[i - mid] = niz[i];

    merge_sort(left, mid);
    merge_sort(right, vel - mid);
    int j = 0;
    int k = 0;
    for (i = 0; j < mid && k < vel - mid; i++) {
      if (left[j] < right[k])
        niz[i] = left[j++];
      else
        niz[i] = right[k++];
    }
    for (; j < mid; i++)
      niz[i] = left[j++];
    for (; k < vel - mid; k++)
      niz[i++] = right[k];
    delete[] left;
    delete[] right;
  }
}

void ucitaj(string filename, int *&niz, int &vel) {
  ifstream ulaz(filename.c_str());
  int kapacitet = 100;
  niz = new int[kapacitet];
  int unos;
  int i = 0;
  while (ulaz >> unos) {
    if (i == kapacitet - 1) {
      kapacitet *= 10;
      int *noviNiz = new int[kapacitet];
      for (int i = 0; i < kapacitet / 10; i++)
        noviNiz[i] = niz[i];
      delete[] niz;
      niz = noviNiz;
      noviNiz = nullptr;
    }
    niz[i] = unos;
    i++;
  }
  vel = i;
  ulaz.close();
}

void generisi(string filename, int vel) {
  ofstream izlaz(filename.c_str(), ios::trunc);
  for (int i = 0; i < vel; i++)
    izlaz << rand() % 1000 << " ";
  izlaz.close();
}

void funkcija() {
  int *niz;
  int velicina_niza = 100;
  int odabir = 0;
  for (;;) {
    cout << "Odaberite ponudjenu opciju: " << endl;
    cout << "1 - Generisi datoteku" << endl;
    cout << "2 - Ucitaj datoteku u niz" << endl;
    cout << "3 - Da li je niz sortiran?" << endl;
    cout << "4 - BubbleSort" << endl;
    cout << "5 - SelectionSort" << endl;
    cout << "6 - QuickSort" << endl;
    cout << "7 - MergeSort" << endl;
    cout << "8 - Zapisi niz u datoteku" << endl;
    cin >> odabir;

    switch (odabir) {
    case 1: {
      generisi("brojevi.txt", 100);
      break;
    }
    case 2: {
      ucitaj("brojevi.txt", niz, velicina_niza);
      break;
    }
    case 3: {
      bool sortiran = true;
      for (int i = 0; i < velicina_niza - 1; i++) {
        if (niz[i] > niz[i + 1]) {
          sortiran = false;
          break;
        }
      }
      if (sortiran) {
        cout << "OK" << endl;
      } else {
        cout << "NOK" << endl;
      }
      break;
    }
    case 4: {
      clock_t vrijeme1 = clock();
      bubble_sort(niz, velicina_niza);
      clock_t vrijeme2 = clock();
      cout << "Vrijeme izvrsavanja BubbleSort je " << vrijeme2 - vrijeme1
           << " sekundi." << endl;
      break;
    }

    case 5: {
      clock_t vrijeme1 = clock();
      selection_sort(niz, velicina_niza);
      clock_t vrijeme2 = clock();
      cout << "Vrijeme izvrsavanja SelectionSort je " << vrijeme2 - vrijeme1
           << " sekundi." << endl;
      break;
    }

    case 6: {
      clock_t vrijeme1 = clock();
      quick_sort(niz, velicina_niza);
      clock_t vrijeme2 = clock();
      cout << "Vrijeme izvrsavanja QuickSort je " << vrijeme2 - vrijeme1
           << " sekundi." << endl;
      break;
    }

    case 7: {
      clock_t vrijeme1 = clock();
      merge_sort(niz, velicina_niza);
      clock_t vrijeme2 = clock();
      cout << "Vrijeme izvrsavanja MergeSort je " << vrijeme2 - vrijeme1
           << " sekundi." << endl;
      break;
    }

    case 8: {
      ofstream izlaz("ispis.txt", ios::trunc);
      for (int i = 0; i < velicina_niza; i++)
        izlaz << niz[i] << endl;
      break;
    }
    default: {
      cout << "Unesena je pogresna vrijednost!" << endl;
    }
    }
  }
}
int main() {
  funkcija();
  return 0;
}
