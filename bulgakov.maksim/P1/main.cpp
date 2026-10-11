#include <iostream>
#include <stdexcept>

void func(int variable, int &max_variable, int &count_max_variable);


int main()
{
  int variable = 0;
  int max_variable = 0;
  int count_max_variable = 1;
  int total_count = 0;

  int return_code = 0;


  while (std::cin >> variable)
  {
    if (variable == 0)
    {
      break;
    }

    if (total_count == 0)
    {
      max_variable = variable;
      count_max_variable = 1;
    }
    else
    {
      func(variable, max_variable, count_max_variable);
    }

    total_count++;
  }



  try
  {
    if (!std::cin.eof() && std::cin.fail())
    {
      throw std::invalid_argument("Input error: not an int!");
    }
    if (total_count == 0)
    {
      throw std::range_error("Sequence is too short!");
    }

    std::cout << "Count max variable: " << count_max_variable << "\n";
  }


  catch (const std::invalid_argument& e)
  {
    std::cerr << "Invalid argument!" << e.what() << "\n";
    return_code = 1;
  }


  catch (const std::range_error& e)
  {
    std::cerr << "Range error!" << e.what() << "\n";
    return_code = 2;
  }


  return return_code;
}



void func (int variable, int &max_variable, int &count_max_variable)
{
  if (variable > max_variable)
  {
    max_variable = variable;
    count_max_variable = 1;
  }
  else if(variable == max_variable)
  {
    count_max_variable++;
  }
}
