#include <algorithm>
#include <iostream>
#include <vector>

namespace osipov {
int main() {
  int cifra = 0;
  std::vector<int> posledovatelnost;
  std::vector<int> otvet;
  int streak = 1;

  while (true) {
    if (!(std::cin >> cifra)) {
      std::cerr << "its not a number\n";
      return 1;
    }

    if (cifra == 0) {
      break;
    }

    posledovatelnost.push_back(cifra);
  }

  if (posledovatelnost.empty()) {
    std::cout << 0 << "\n";
    return 0;
  }

  unsigned long i = 0;
  while (i < posledovatelnost.size()) {
    if ((i + 1) == posledovatelnost.size()) {
      otvet.push_back(streak);
      std::cout << *std::max_element(otvet.begin(), otvet.end()) << "\n";
      return 0;
    }

    if (posledovatelnost[i] <= posledovatelnost[i + 1]) {
      streak++;
      i++;
    } else {
      otvet.push_back(streak);
      streak = 1;
      i++;
    }
  }

  return 0;
}
}
