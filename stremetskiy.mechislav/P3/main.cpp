#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

namespace stremetskiy
{
  void readRowAndCol(std::string, int *, int *);
  void fillArr(std::string, int *, int, int);
  int **fillArr(std::string, int, int);
  void writeResult(std::string, int);
  constexpr int INVALID_INPUT = 1;
  constexpr int INVALID_FILE = 2;
  constexpr int SIZE_ARRSTAT = 100;
}

int main()
{
  int num = 0;
  int count = 0;

  std::string input = "";
  std::string output = "";
  std::string extra = "";
  std::string all_line = "";

  int row = 0;
  int col = 0;

  std::cout << "\nInput 3 val: num of var,input and output files\n";
  try
  {
    std::getline(std::cin, all_line);
    std::istringstream iss(all_line);
    if (!(iss >> num))
    {
      throw std::invalid_argument("First value is not a number\n");
    }
    if (!(iss >> input >> output))
    {
      throw std::range_error("Not enough arguments\n");
    }

    if (iss >> extra)
    {
      throw std::range_error("too many arguments\n");
    }

    stremetskiy::readRowAndCol(input, &row, &col);
    if (num == 1)
    {
      int array[stremetskiy::SIZE_ARRSTAT]
               [stremetskiy::SIZE_ARRSTAT] = {0};
      stremetskiy::fillArr(input, array[0], row, col);

      for (int i = 1; i < (row - 1); i++)
      {
        for (int j = 1; j < (col - 1); j++)
        {
          if (array[i][j] > array[i - 1][j] &&
              array[i][j] > array[i + 1][j] &&
              array[i][j] > array[i][j - 1] &&
              array[i][j] > array[i][j + 1])
          {
            count++;
          }
        }
      }
    }
    else if (num == 2)
    {
      int *const *const array = stremetskiy::fillArr(input, row, col);

      for (int i = 1; i < (row - 1); i++)
      {
        for (int j = 1; j < (col - 1); j++)
        {
          if (array[i][j] > array[i - 1][j] &&
              array[i][j] > array[i + 1][j] &&
              array[i][j] > array[i][j - 1] &&
              array[i][j] > array[i][j + 1])
          {
            count++;
          }
        }
      }

      for (int i = 0; i < row; i++)
      {
        delete[] array[i];
      }
      delete[] array;
    }
    else
    {
      throw std::invalid_argument("First value is out of range\n");
    }
  }
  catch (const std::range_error &ex)
  {
    std::cerr << "Invalid input: " << ex.what();
    return stremetskiy::INVALID_INPUT;
  }
  catch (const std::invalid_argument &ex)
  {
    std::cerr << "Invalid input: " << ex.what();
    return stremetskiy::INVALID_INPUT;
  }
  catch (const std::logic_error &ex)
  {
    std::cerr << "Input file problem: " << ex.what();
    return stremetskiy::INVALID_FILE;
  }

  stremetskiy::writeResult(output, count);

  return 0;
}

namespace stremetskiy
{
  void readRowAndCol(std::string file, int *row, int *col)
  {
    std::ifstream in(file);

    int val = 0;

    if (in.is_open())
    {
      if (!(in >> val))
      {
        throw std::logic_error("Invalid file content\n");
      }
      *row = val;
      if (!(in >> val))
      {
        throw std::logic_error("Invalid file content\n");
      }
      *col = val;
    }
    else
    {
      throw std::logic_error("input file cannot be opened\n");
    }
  }

  void fillArr(std::string file, int arr[], int row, int col)
  {
    std::ifstream in(file);

    int val = 0;

    if (in.is_open())
    {
      in >> val;
      in >> val;

      for (int i = 0; i < row; i++)
      {
        for (int j = 0; j < col; j++)
        {
          if (!(in >> val))
          {
            throw std::logic_error("Invalid file content\n");
          }
          *(arr + i * stremetskiy::SIZE_ARRSTAT + j) = val;
        }
      }
    }
    else
    {
      throw std::logic_error("input file cannot be opened\n");
    }
  }

  int **fillArr(std::string file, int row, int col)
  {
    std::ifstream in(file);

    int val = 0;

    if (in.is_open())
    {
      in >> val;
      in >> val;

      int **const result_arr = new int *[row];
      for (int i = 0; i < row; i++)
      {
        result_arr[i] = new int[col];
      }

      for (int i = 0; i < row; i++)
      {
        for (int j = 0; j < col; j++)
        {
          if (!(in >> val))
          {
            throw std::logic_error("Invalid file content\n");
          }
          result_arr[i][j] = val;
        }
      }
      return result_arr;
    }
    else
    {
      throw std::logic_error("input file cannot be opened\n");
    }
  }

  void writeResult(std::string file, int result)
  {
    std::ofstream out(file);

    if (out.is_open())
    {
      out << result;
    }
    else
    {
      throw std::logic_error("output file cannot be opened\n");
    }
  }
}
