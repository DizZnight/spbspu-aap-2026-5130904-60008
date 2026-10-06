#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main()
{
  int max1 = 0;
  int max2 = 0;
  int num = 0;
  int size = 0;
  try
  {
    while (std::cin >> num && num != 0)
    {
      size++;
      if (size == 1)
      {
        max1 = num;
      }
      else if (num > max1)
      {
        max2 = max1;
        max1 = num;
      }
      else if (num > max2 || size == 2)
      {
        max2 = num;
      }
    }
    if (std::cin.fail() && !std::cin.eof())
    {
      throw std::invalid_argument("Invalid_argument");
    }
    if (size < 2)
    {
      throw std::out_of_range("Not_enough_values");
    }
  }
  catch (const std::invalid_argument &ex)
  {
    std::cerr << ex.what() << "\n";
    std::exit(1);
  }
  catch (const std::out_of_range &ex)
  {
    std::cerr << ex.what() << "\n";
    std::exit(2);
  }
  std::cout << max2 << "\n";
  return 0;
}
