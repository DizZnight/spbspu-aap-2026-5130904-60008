#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>
#include <string>

namespace grekova {
const int EXIT_SUCCESS_CODE = 0;
const int EXIT_ARGS_ERROR = 1;
const int EXIT_FILE_ERROR = 2;
const int EXIT_MATRIX_ERROR = 2;
const int MAX_SIZE_ARR = 10000;

int readMatrix(const std::string &filename, int mode, int **&matrix, int &rows,
               int &cols, int *&staticStorage);
void processSpiral(int **matrix, int rows, int cols);
bool readInt(std::ifstream &in, int &val);
void writeMatrixToFile(const std::string &filename, int **matrix, int rows,
                       int cols);
}

int main(int argc, char *argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments.\n";
    return grekova::EXIT_ARGS_ERROR;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments.\n";
    return grekova::EXIT_ARGS_ERROR;
  }

  std::string num = argv[1];

  if (num != "1" && num != "2") {
    bool is_numeric = true;
    for (const char c : num) {
      if (!std::isdigit(c)) {
        is_numeric = false;
        break;
      }
    }

    if (is_numeric) {
      std::cerr << "First parameter is out of range.\n";
      return grekova::EXIT_ARGS_ERROR;
    } else {
      std::cerr << "First parameter is not a number.\n";
      return grekova::EXIT_ARGS_ERROR;
    }
  }

  int mode = (num == "1") ? 1 : 2;

  int **matrix = nullptr;
  int *staticStorage = nullptr;
  int rows = 0, cols = 0;

  int readStatus =
      grekova::readMatrix(argv[2], mode, matrix, rows, cols, staticStorage);
  if (readStatus != grekova::EXIT_SUCCESS_CODE) {
    return readStatus;
  }

  if (rows == 0 && cols == 0) {
    grekova::writeMatrixToFile(argv[3], nullptr, 0, 0);
    return grekova::EXIT_SUCCESS_CODE;
  }

  grekova::processSpiral(matrix, rows, cols);

  grekova::writeMatrixToFile(argv[3], matrix, rows, cols);

  if (mode == 1) {
    delete[] matrix;
    delete[] staticStorage;
  } else {
    for (int i = 0; i < rows; ++i)
      delete matrix[i];
    delete[] matrix;
  }

  return grekova::EXIT_SUCCESS_CODE;
}

bool grekova::readInt(std::ifstream &in, int &val) {
  return static_cast<bool>(in >> val);
}

int grekova::readMatrix(const std::string &filename, int mode, int **&matrix,
                        int &rows, int &cols, int *&staticStorage) {
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    std::cerr << "Cannot open input file.\n";
    return EXIT_MATRIX_ERROR;
  }

  if (!readInt(fin, rows) || !readInt(fin, cols)) {
    std::cerr << "Invalid matrix input or empty file.\n";
    return EXIT_MATRIX_ERROR;
  }

  if (rows < 0 || cols < 0) {
    std::cerr << "Invalid matrix dimensions.\n";
    return EXIT_MATRIX_ERROR;
  }

  if (rows == 0 && cols == 0) {
    matrix = nullptr;
    return EXIT_SUCCESS_CODE;
  }

  if (mode == 1) {
    if (rows * cols > MAX_SIZE_ARR) {
      std::cerr << "Matrix is too large for static array.\n";
      return EXIT_MATRIX_ERROR;
    }

    staticStorage = new int[grekova::MAX_SIZE_ARR];
    matrix = new int *[rows];

    int idx = 0, val = 0;
    for (int i = 0; i < rows; ++i) {
      matrix[i] = &staticStorage[i * cols];
      for (int j = 0; j < cols; ++j) {
        if (!readInt(fin, val)) {
          std::cerr << "Not enough elements in matrix.\n";
          delete[] staticStorage;
          delete[] matrix;
          return EXIT_MATRIX_ERROR;
        }
        staticStorage[idx++] = val;
      }
    }
  } else {
    try {
      matrix = new int *[rows];
      for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
      }
    } catch (const std::bad_alloc &e) {
      std::cerr << "Memory allocation failed.\n";
      if (matrix) {
        for (int i = 0; i < rows; ++i)
          delete[] matrix[i];
        delete[] matrix;
      }
      return EXIT_FILE_ERROR;
    }

    int val = 0;
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (!readInt(fin, val)) {
          std::cerr << "Not enough elements in matrix.\n";
          for (int k = 0; k < rows; ++k)
            delete[] matrix[k];
          delete[] matrix;
          return EXIT_MATRIX_ERROR;
        }
        matrix[i][j] = val;
      }
    }
  }
  return EXIT_SUCCESS_CODE;
}

void grekova::processSpiral(int **matrix, int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    return;

  int top = 0, bottom = rows - 1, left = 0, right = cols - 1, increment = 1;

  while (top <= bottom && left <= right) {

    for (int j = left; j <= right; ++j)
      matrix[bottom][j] += increment++;
    bottom--;

    for (int i = bottom; i >= top; --i)
      matrix[i][right] += increment++;
    right--;

    if (top <= bottom) {
      for (int j = right; j >= left; --j)
        matrix[top][j] += increment++;
      top++;
    }

    if (left <= right) {
      for (int i = top; i <= bottom; ++i)
        matrix[i][left] += increment++;
      left++;
    }
  }
}

void grekova::writeMatrixToFile(const std::string &filename, int **matrix,
                                int rows, int cols) {
  std::ofstream fout(filename);
  if (!fout.is_open()) {
    std::cerr << "Cannot open output file.\n";
    return;
  }

  fout << rows << " " << cols << " ";
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      fout << matrix[i][j] << " ";
    }
  }
}
