#ifndef MAIN_CPP
#define MAIN_CPP

#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace hedeya {
  const int MAX_ELEMENTS = 10000;
  const int INVALID_MATRIX_ERROR = 2;

  class invalid_matrix_error: public std::runtime_error {
   public:
    explicit invalid_matrix_error(const std::string& message = "Invalid matrix"):
      std::runtime_error(message)
    {
    }
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
      if (x != other.x)
        return x < other.x;
      return y < other.y;
    }
  };

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
    return ((position.x >= bounds.left) && (position.x <= bounds.right) && (position.y >= bounds.top)
            && (position.y <= bounds.bottom));
  }

  Direction getNextMovementDirection(Point current_position, Direction last_movement_direction,
                                     Bounds bounds, const std::set< Point >& seen)
  {
    Direction direction = UP;

    Point next_position_last = getNextPosition(last_movement_direction, current_position);
    Point next_position_up = getNextPosition(UP, current_position);
    Point next_position_down = getNextPosition(DOWN, current_position);
    Point next_position_left = getNextPosition(LEFT, current_position);
    Point next_position_right = getNextPosition(RIGHT, current_position);

    if (!seen.count(next_position_last) && isInBounds(next_position_last, bounds)) {
      return last_movement_direction;
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
      throw invalid_matrix_error("Extra data after matrix");
    }
  }

  void fillMatrix(std::ifstream& input_file, Dimensions dimensions, std::vector< int >& matrix)
  {
    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        if (!(input_file >> matrix[index])) {
          throw invalid_matrix_error("Number of rows/columns does not match the number of elements in the input");
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
          throw invalid_matrix_error("Number of rows/columns does not match the number of elements in the input");
        }
      }
    }
  }

  void outputMatrixToFile(Dimensions dimensions, const std::vector< int >& matrix,
                          const std::string& output_file_path)
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

  void iterateThroughMatrixSpiral(Dimensions dimensions, int* matrix,
                                  const std::function< void(int&, std::size_t) >& element_function)
  {
    std::set< Point > seen = {};

    Bounds bounds{0, dimensions.rows - 1, 0, dimensions.columns - 1};
    Point position{bounds.left, bounds.bottom};

    const std::size_t total_elements = dimensions.rows * dimensions.columns;

    Direction last_movement_direction = UP;

    for (std::size_t i = 0; i < total_elements; ++i) {
      seen.insert(position);

      const std::size_t index = (position.y * dimensions.columns) + position.x;

      int& element = matrix[index];
      element_function(element, i);

      Direction direction = getNextMovementDirection(position, last_movement_direction, bounds, seen);
      last_movement_direction = direction;

      move(direction, position);
    }
  }
}  // namespace hedeya

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
      throw hedeya::invalid_matrix_error("Input file failed to open");
    }

    hedeya::Dimensions dimensions{0, 0};
    if (!(input_file >> dimensions.rows >> dimensions.columns)) {
      throw hedeya::invalid_matrix_error("Number of rows/columns is not a valid number");
    }

    if (dimensions.rows < 0 || dimensions.columns < 0) {
      throw hedeya::invalid_matrix_error("Number of rows/columns cannot be negative");
    }

    std::size_t total_elements = dimensions.rows * dimensions.columns;

    if (USE_DYNAMIC_ARRAY) {
      std::vector< int > dynamic_matrix(total_elements, 0);

      hedeya::fillMatrix(input_file, dimensions, dynamic_matrix);
      hedeya::rejectExtraData(input_file);

      hedeya::iterateThroughMatrixSpiral(dimensions, dynamic_matrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, dynamic_matrix, OUTPUT_FILE_PATH);
    } else {
      if (total_elements > hedeya::MAX_ELEMENTS) {
        throw hedeya::invalid_matrix_error("Matrix exceeds fixed-size array capacity");
      }

      std::array< int, hedeya::MAX_ELEMENTS > static_matrix{};

      hedeya::fillMatrix(input_file, dimensions, static_matrix.data());
      hedeya::rejectExtraData(input_file);

      hedeya::iterateThroughMatrixSpiral(dimensions, static_matrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, static_matrix.data(), OUTPUT_FILE_PATH);
    }

    return EXIT_SUCCESS;

  } catch (const hedeya::invalid_matrix_error& error) {
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

#endif
