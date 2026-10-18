#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <cstdlib>

namespace ajdarbekov {

  const std::string NOT_EN_ARG = "Not enough arguments";
  const std::string TOO_MANY_ARG = "Too many arguments";
  const std::string FIRST_OUT_RANGE = "First parameter is out of range";
  const std::string FIRST_NOT_NUMBER = "First parameter is not a number";
  const std::string INCOR_FILE_NAME = "Incorrect file name";
  const std::string NOT_MAT_IN_FILE = "No matrix found in the file";
  const int ARG_EXIT_CODE = 1;
  const int MAT_EXIT_CODE = 2;

  struct Matrix {
    int line, col;
    int* start;
  };

  void inputdata(int mode, std::string fin, std::string fout);
  void spiral(Matrix mat);
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << ajdarbekov::NOT_EN_ARG;
    return ajdarbekov::ARG_EXIT_CODE;
  }
  if (argc > 4) {
    std::cerr << ajdarbekov::TOO_MANY_ARG;
    return ajdarbekov::ARG_EXIT_CODE;
  }

  const std::string num_param = argv[1];
  int mode = 0;

  if (num_param == "1") {
    mode = 1;
  } else if (num_param == "2") {
    mode = 2;
  } else {
    bool is_numeric = true;
    for (const char c : num_param) {
      if (!std::isdigit(c)) {
        is_numeric = false;
        break;
      }
    }
    if (is_numeric) {
      std::cerr << ajdarbekov::FIRST_OUT_RANGE;
    } else {
      std::cerr << ajdarbekov::FIRST_NOT_NUMBER;
    }
    return ajdarbekov::ARG_EXIT_CODE;
  }

  try {
    ajdarbekov::inputdata(mode, argv[2], argv[3]);
  } catch (const std::invalid_argument& ex) {
    std::cerr << ex.what();
    return ajdarbekov::MAT_EXIT_CODE;
  }

  return 0;
}

void ajdarbekov::inputdata(int mode, std::string fin, std::string fout) {
  std::ifstream infile(fin);
  std::ofstream outfile(fout);
  if (!infile.is_open() or !outfile.is_open()) {
    std::cerr << ajdarbekov::INCOR_FILE_NAME;
    std::exit(ajdarbekov::ARG_EXIT_CODE);
  }

  int line = 0, col = 0;
  const int BUFFER = 10000;
  int arr[BUFFER];
  bool has_matrices = false;
  ajdarbekov::Matrix mat{ 0, 0, nullptr };

  while (true) {
    infile >> line;
    if (infile.eof()) {
      if (!has_matrices) {
        throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
      }
      break;
    }

    if (infile.fail()) {
      throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
    }

    if (!(infile >> col)) {
      throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
    }

    has_matrices = true;

    if (line < 0 or col < 0) {
      throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
    }

    if (line == 0 and col == 0) {
      outfile << line << " " << col << "\n";
      continue;
    }

    const int quan_elem = col * line;
    mat.line = line;
    mat.col = col;

    if (mode == 1) {
      if (quan_elem > BUFFER) {
        throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
      }
      mat.start = arr;
    } else {
      mat.start = new int[quan_elem];
    }

    for (int i = 0; i < quan_elem; ++i) {
      if (!(infile >> mat.start[i])) {
        if (mode == 2) {
          delete[] mat.start;
        }
        throw std::invalid_argument(ajdarbekov::NOT_MAT_IN_FILE);
      }
    }

    ajdarbekov::spiral(mat);

    outfile << line << " " << col;
    for (int i = 0; i < quan_elem; ++i) {
      outfile << " " << mat.start[i];
    }
    outfile << "\n";

    if (mode == 2) {
      delete[] mat.start;
    }
  }
}

void ajdarbekov::spiral(ajdarbekov::Matrix mat) {
  int top = 0, bottom = mat.line - 1, left = 0, right = mat.col - 1, n = 1;

  while (top <= bottom and left <= right) {
    int* cursor = mat.start + (top * mat.col + left);

    for (int i = left; i <= right; ++i) {
      *(cursor) -= n;
      ++cursor;
      ++n;
    }
    ++top;
    
    if (top <= bottom) {
      cursor = mat.start + (top * mat.col + right);
      for (int i = top; i <= bottom; ++i) {
        *(cursor) -= n;
        cursor += mat.col;
        ++n;
      }
    }
    --right;

    if (top <= bottom and left <= right) {
      cursor = mat.start + (bottom * mat.col + right);
      for (int i = right; i >= left; --i) {
        *(cursor) -= n;
        --cursor;
        ++n;
      }
    }
    --bottom;

    if (top <= bottom && left <= right) {
      cursor = mat.start + (bottom * mat.col + left);
      for (int i = bottom; i >= top; --i) {
        *(cursor) -= n;
        cursor -= mat.col;
        ++n;
      }
    }
    ++left;
  }
}