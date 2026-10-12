#include <iostream>
#include <stdexcept>

namespace tirkacheva {
  int countDivisible()
  {
    int previous = 0;
    int current = 0;
    int count = 0;
    int elements = 0;
    bool terminated = false;

    if (!(std::cin >> previous)) {
      throw std::runtime_error("Invalid input");
    }

    if (previous == 0) {
      throw std::invalid_argument("Empty sequence");
    }

    elements = 1;

    while (std::cin >> current) {
      if (current == 0) {
        terminated = true;
        break;
      }

      elements++;

      if (current % previous == 0) {
        count++;
      }

      previous = current;
    }

    if (!std::cin.eof() && std::cin.fail()) {
      throw std::runtime_error("Invalid input");
    }

    if (!terminated) {
      throw std::runtime_error("Missing terminating zero");
    }
    if (elements < 2) {
      throw std::invalid_argument("Not enough elements");
    }

    return count;
  }
}

  int main()
  {
    try {
      std::cout << tirkacheva::countDivisible() << "\n";
      return 0;
    } catch (const std::invalid_argument &error) {
      std::cerr << "Error: " << error.what() << "\n";
      return 2;
    } catch (const std::runtime_error &error) {
      std::cerr << "Error: " << error.what() << "\n";
      return 1;
    }
  }
