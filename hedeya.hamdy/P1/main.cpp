#ifndef MAIN_CPP
#define MAIN_CPP

#include <algorithm>
#include <cstdlib>
#include <iostream>

int main()
{
  std::cout << "Enter an integer (0 to terminate):\n";

  int previous = 0;
  int current = 0;
  int length = 1;
  int longest = 0;

  while (std::cin >> current && current != 0)
  {
    if (current == previous && previous != 0)
    {
      length += 1;
    }
    else
    {
      length = 1;
    }

    longest = std::max(length, longest);
    previous = current;
  }

  std::cout << longest;

  return EXIT_SUCCESS;
}

#endif
