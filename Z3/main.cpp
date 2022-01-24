#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int dajBrojCifara(int broj) {
  int brojac = 0;
  while (broj != 0) {
    broj /= 10;
    brojac++;
  }
  return brojac;
}

int dajCifru(int broj, int redniBrojCifreOdKraja) {
  for (int i = 0; i < redniBrojCifreOdKraja; i++)
    broj /= 10;
  return broj % 10;
}

void radixSort(vector<int> &a) {
  vector<queue<int>> znamenke(10);
  int brojCifara = dajBrojCifara(*max_element(a.begin(), a.end()));
  for (int i = 0; i < brojCifara; i++) {
    for (int j = 0; j < a.size(); j++) {
      znamenke.at(dajCifru(a.at(j), i)).push(a.at(j));
    }
    a.clear();
    for (auto &red : znamenke) {
      while (!red.empty()) {
        a.push_back(red.front());
        red.pop();
      }
    }
  }
}

void stvoriGomilu(vector<int> &a) {
  for (int i = a.size() / 2; i >= 0; i--) {
    for (int j = i; j < a.size() / 2;) {
      int lijevi = j * 2 + 1;
      int desni = j * 2 + 2;

      if (desni < a.size() && a[lijevi] < a[desni])
        lijevi = desni;

      if (a[j] <= a[lijevi]) {
        int temp = a[j];
        a[j] = a[lijevi];
        a[lijevi] = temp;
        j = lijevi;
      } else
        break;
    }
  }
}

void popraviGore(vector<int> &a, int index) {
  while (index != 0 && a[index] > a[(index - 1) / 2]) {
    int temp = a[index];
    a[index] = a[(index - 1) / 2];
    a[(index - 1) / 2] = temp;
    index = (index - 1) / 2;
  }
}

void popraviDolje(vector<int> &a, int index, int velicina) {
  while (!(index >= velicina / 2 && index < velicina)) {
    int lijevi = index * 2 + 1;
    int desni = index * 2 + 2;

    if (desni < velicina && a[desni] > a[lijevi])
      lijevi = desni;
    if (a[index] > a[lijevi])
      break;
    int temp = a[index];
    a[index] = a[lijevi];
    a[lijevi] = temp;
    index = lijevi;
  }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
  a.push_back(umetnuti);
  velicina++;
  popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
  velicina--;
  int temp = a[0];
  a[0] = a[velicina];
  a[velicina] = temp;

  if (velicina != 0)
    popraviDolje(a, 0, velicina);
  return a[velicina];
}

void gomilaSort(vector<int> &a) {
  stvoriGomilu(a);
  int velicina = a.size();
  for (int i = 0; i <= a.size() - 2; i++)
    izbaciPrvi(a, velicina);
}

int main() {
  // std::cout << "Zadaća 3, Zadatak 1";
  return 0;
}
