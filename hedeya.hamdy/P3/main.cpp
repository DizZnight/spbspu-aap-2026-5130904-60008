#ifndef MAIN_CPP
#define MAIN_CPP

#include <array>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace hedeya {
  const int MAX_ELEMENTS = 10000;
  const int INVALID_MATRIX_ERROR = 2;

  class InvalidMatrixError: public std::runtime_error {
  public:
    explicit InvalidMatrixError(const std::string& message = "Invalid matrix"):
      std::runtime_error(message)
    {}
  };

  enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
  };

  struct Bounds {
    int top, bottom, left, right;
  };

  struct Dimensions {
    int rows, columns;
  };

  struct Point {
    int x, y;

    bool operator<(const Point& other) const
    {
      if (x != other.x) {
        return x < other.x;
      }

      return y < other.y;
    }
  };

  Point getNextPosition(Direction direction, Point position);
  bool isInBounds(Point position, Bounds bounds);
  Direction getNextDirection(Point position, Direction last_direction, Bounds bounds, const std::set< Point >& seen);
  void move(Direction direction, Point& position);
  void subtractDifferenceFromElement(int& element, std::size_t i);
  void rejectExtraData(std::ifstream& input_file);
  void fillMatrix(std::ifstream& input_file, Dimensions dimensions, std::vector< int >& matrix);
  void fillMatrix(std::ifstream& input_file, Dimensions dimensions, int* matrix);
  void outputMatrixToFile(Dimensions dimensions, const std::vector< int >& matrix, const std::string& output_file_path);
  void outputMatrixToFile(Dimensions dimensions, const int* matrix, const std::string& output_file_path);
  void iterateSpiral(Dimensions dimensions, int* matrix, const std::function< void(int&, std::size_t) >& element_func);
}

int main(int argc, char* argv[])
{
  try {
    if (argc < 4) {
      throw std::invalid_argument("Not enough arguments");
    }
    if (argc > 4) {
      throw std::invalid_argument("Too many arguments");
    }

    int num_arg = 0;

    try {
      std::size_t end_position = 0;

      num_arg = std::stoi(argv[1], &end_position);
      if (end_position != std::string(argv[1]).length()) {
        throw std::invalid_argument("First parameter is not a number");
      }
    } catch (const std::invalid_argument&) {
      throw std::invalid_argument("First parameter is not a number");
    }

    if (num_arg > 2 || num_arg < 1) {
      throw std::invalid_argument("First parameter is out of range");
    }

    const bool USE_DYNAMIC_ARRAY = (num_arg == 2);

    const std::string INPUT_FILE_PATH = argv[2];
    const std::string OUTPUT_FILE_PATH = argv[3];

    std::ifstream input_file(INPUT_FILE_PATH);
    if (!input_file.is_open()) {
      throw hedeya::InvalidMatrixError("Input file failed to open");
    }

    hedeya::Dimensions dimensions{0, 0};
    if (!(input_file >> dimensions.rows >> dimensions.columns)) {
      throw hedeya::InvalidMatrixError("Number of rows/columns is not a valid number");
    }

    if (dimensions.rows < 0 || dimensions.columns < 0) {
      throw hedeya::InvalidMatrixError("Number of rows/columns cannot be negative");
    }

    const std::size_t total_elements = dimensions.rows * dimensions.columns;

    if (USE_DYNAMIC_ARRAY) {
      std::vector< int > dynamic_matrix(total_elements, 0);

      hedeya::fillMatrix(input_file, dimensions, dynamic_matrix);
      hedeya::rejectExtraData(input_file);

      hedeya::iterateSpiral(dimensions, dynamic_matrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, dynamic_matrix, OUTPUT_FILE_PATH);
    } else {
      if (total_elements > hedeya::MAX_ELEMENTS) {
        throw hedeya::InvalidMatrixError("Matrix exceeds fixed-size array capacity");
      }

      std::array< int, hedeya::MAX_ELEMENTS > static_matrix{};

      hedeya::fillMatrix(input_file, dimensions, static_matrix.data());
      hedeya::rejectExtraData(input_file);

      hedeya::iterateSpiral(dimensions, static_matrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, static_matrix.data(), OUTPUT_FILE_PATH);
    }

    return EXIT_SUCCESS;

  } catch (const hedeya::InvalidMatrixError& error) {
    std::cerr << "Invalid matrix error: " << error.what() << "\n";
    return hedeya::INVALID_MATRIX_ERROR;
  } catch (const std::invalid_argument& error) {
    std::cerr << "Invalid argument error: " << error.what() << "\n";
    return EXIT_FAILURE;
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << "\n";
    return EXIT_FAILURE;
  }
}

namespace hedeya {
  Point getNextPosition(Direction direction, Point position)
  {
    switch (direction) {
    case UP:
      return Point{position.x, position.y - 1};
    case RIGHT:
      return Point{position.x + 1, position.y};
    case DOWN:
      return Point{position.x, position.y + 1};
    case LEFT:
      return Point{position.x - 1, position.y};
    }
    return position;
  }

  bool isInBounds(Point position, Bounds bounds)
  {
    const bool is_x_in_bounds = position.x >= bounds.left && position.x <= bounds.right;
    const bool is_y_in_bounds = position.y >= bounds.top && position.y <= bounds.bottom;
    return is_x_in_bounds && is_y_in_bounds;
  }

  Direction getNextDirection(Point position, Direction last_direction, Bounds bounds, const std::set< Point >& seen)
  {
    Direction direction = UP;

    const Point next_position_last = getNextPosition(last_direction, position);
    const Point next_position_up = getNextPosition(UP, position);
    const Point next_position_down = getNextPosition(DOWN, position);
    const Point next_position_left = getNextPosition(LEFT, position);
    const Point next_position_right = getNextPosition(RIGHT, position);

    if (!seen.count(next_position_last) && isInBounds(next_position_last, bounds)) {
      return last_direction;
    }

    if (isInBounds(next_position_up, bounds) && !seen.count(next_position_up)) {
      direction = UP;
    } else if (isInBounds(next_position_right, bounds) && !seen.count(next_position_right)) {
      direction = RIGHT;
    } else if (isInBounds(next_position_down, bounds) && !seen.count(next_position_down)) {
      direction = DOWN;
    } else if (isInBounds(next_position_left, bounds) && !seen.count(next_position_left)) {
      direction = LEFT;
    }

    return direction;
  }

  void move(Direction direction, Point& position)
  {
    switch (direction) {
    case UP:
      position.y -= 1;
      break;
    case RIGHT:
      position.x += 1;
      break;
    case DOWN:
      position.y += 1;
      break;
    case LEFT:
      position.x -= 1;
      break;
    }
  }

  void subtractDifferenceFromElement(int& element, std::size_t i)
  {
    element -= i + 1;
  }

  void rejectExtraData(std::ifstream& input_file)
  {
    std::string extra;
    if (input_file >> extra) {
      throw InvalidMatrixError("Extra data after matrix");
    }
  }

  void fillMatrix(std::ifstream& input_file, Dimensions dimensions, std::vector< int >& matrix)
  {
    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        if (!(input_file >> matrix[index])) {
          throw InvalidMatrixError("Number of rows/columns does not match the number of elements in the input");
        }
      }
    }
  }

  void fillMatrix(std::ifstream& input_file, Dimensions dimensions, int* matrix)
  {
    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        if (!(input_file >> matrix[index])) {
          throw InvalidMatrixError("Number of rows/columns does not match the number of elements in the input");
        }
      }
    }
  }

  void outputMatrixToFile(Dimensions dimensions, const std::vector< int >& matrix, const std::string& output_file_path)
  {
    std::ofstream output_file(output_file_path);

    if (!output_file.is_open()) {
      throw std::runtime_error("Output file failed to open");
    }

    output_file << dimensions.rows << " " << dimensions.columns << " ";

    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = row * dimensions.columns + column;

        output_file << matrix[index] << " ";
      }
    }
  }

  void outputMatrixToFile(Dimensions dimensions, const int* matrix, const std::string& output_file_path)
  {
    std::ofstream output_file(output_file_path);

    if (!output_file.is_open()) {
      throw std::runtime_error("Output file failed to open");
    }

    output_file << dimensions.rows << " " << dimensions.columns << " ";

    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        output_file << matrix[index] << " ";
      }
    }
  }

  void iterateSpiral(Dimensions dimensions, int* matrix, const std::function< void(int&, std::size_t) >& element_func)
  {
    std::set< Point > seen = {};

    const Bounds bounds{0, dimensions.rows - 1, 0, dimensions.columns - 1};
    Point position{bounds.left, bounds.bottom};

    const std::size_t total_elements = dimensions.rows * dimensions.columns;

    Direction last_direction = UP;

    for (std::size_t i = 0; i < total_elements; ++i) {
      seen.insert(position);

      const std::size_t index = (position.y * dimensions.columns) + position.x;

      int& element = matrix[index];
      element_func(element, i);

      const Direction direction = getNextDirection(position, last_direction, bounds, seen);
      last_direction = direction;

      move(direction, position);
    }
  }
}

#endif
