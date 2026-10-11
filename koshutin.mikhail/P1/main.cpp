#include <iostream>
namespace koshutin
{
int countElements()
{
  int a = 0;
  int num_of_elements = 0;
  int b = 0;
  int c = 0;

  try {
    if (!(std::cin >> b)) {
      throw 1;
    }
    if (b == 0) {
      std::cout << num_of_elements << "\n";
      return 0;
    }

    if (!(std::cin >> a)) {
      throw 1;
    }
    if (a == 0) {
      std::cout << num_of_elements << "\n";
      return 0;
    }

    for (;;) {
      if (!(std::cin >> c)) {
        throw 1;
      }
      if (c == 0) {
        break;
      }

      if (a < b and a > c) {
        num_of_elements += 1;
      }

      b = a;
      a = c;
    }
    std::cout << num_of_elements << "\n";
  } catch (int thr) {
    if (thr == 1) {
      std::cerr << "Invalid Input" << "\n";
      return 1;
    }
  }

  return 0;
}

}
int main()
{
  return koshutin::countElements();
}
