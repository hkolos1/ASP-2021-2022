#include <iostream>
#include <stdexcept>

using namespace std;

int fib2_0(int n, int prvi = 0, int drugi = 1, int sljedeci = 0,
           int brojac = 0) {
  sljedeci = prvi + drugi;
  prvi = drugi;
  drugi = sljedeci;
  brojac++;
  if (n - 1 == brojac)
    return sljedeci;
  return fib2_0(n, prvi, drugi, sljedeci, brojac);
}

int nzd(int x, int y) {
  if (0 == y)
    return x;
  return nzd(y, x % y);
}
int main() {
  std::cout << fib2_0(10) << std::endl;
  std::cout << nzd(140, 10) << std::endl;
  return 0;
}
