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

  Direction getNextMovementDirection(Point currentPosition, Direction lastMovementDirection, Bounds bounds,
                                     const std::set< Point >& seen)
  {
    Direction direction = UP;

    Point nextPositionLast = getNextPosition(lastMovementDirection, currentPosition);
    Point nextPositionUp = getNextPosition(UP, currentPosition);
    Point nextPositionDown = getNextPosition(DOWN, currentPosition);
    Point nextPositionLeft = getNextPosition(LEFT, currentPosition);
    Point nextPositionRight = getNextPosition(RIGHT, currentPosition);

    if (!seen.count(nextPositionLast) && isInBounds(nextPositionLast, bounds)) {
      return lastMovementDirection;
    }

    if (isInBounds(nextPositionUp, bounds) && !seen.count(nextPositionUp)) {
      direction = UP;
    } else if (isInBounds(nextPositionRight, bounds) && !seen.count(nextPositionRight)) {
      direction = RIGHT;
    } else if (isInBounds(nextPositionDown, bounds) && !seen.count(nextPositionDown)) {
      direction = DOWN;
    } else if (isInBounds(nextPositionLeft, bounds) && !seen.count(nextPositionLeft)) {
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

  void rejectExtraData(std::ifstream& inputFile)
  {
    std::string extra;
    if (inputFile >> extra) {
      throw invalid_matrix_error("Extra data after matrix");
    }
  }

  void fillMatrix(std::ifstream& inputFile, Dimensions dimensions, std::vector< int >& matrix)
  {
    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        if (!(inputFile >> matrix[index])) {
          throw invalid_matrix_error("Number of rows/columns does not match the number of elements in the input");
        }
      }
    }
  }

  void fillMatrix(std::ifstream& inputFile, Dimensions dimensions, int* matrix)
  {
    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        if (!(inputFile >> matrix[index])) {
          throw invalid_matrix_error("Number of rows/columns does not match the number of elements in the input");
        }
      }
    }
  }

  void outputMatrixToFile(Dimensions dimensions, const std::vector< int >& matrix, const std::string& outputFilePath)
  {
    std::ofstream outputFile(outputFilePath);

    if (!outputFile.is_open()) {
      throw std::runtime_error("Output file failed to open");
    }

    outputFile << dimensions.rows << " " << dimensions.columns << " ";

    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = row * dimensions.columns + column;

        outputFile << matrix[index] << " ";
      }
    }
  }

  void outputMatrixToFile(Dimensions dimensions, const int* matrix, const std::string& outputFilePath)
  {
    std::ofstream outputFile(outputFilePath);

    if (!outputFile.is_open()) {
      throw std::runtime_error("Output file failed to open");
    }

    outputFile << dimensions.rows << " " << dimensions.columns << " ";

    for (int row = 0; row < dimensions.rows; ++row) {
      for (int column = 0; column < dimensions.columns; ++column) {
        const std::size_t index = (row * dimensions.columns) + column;

        outputFile << matrix[index] << " ";
      }
    }
  }

  void iterateThroughMatrixSpiral(Dimensions dimensions, int* matrix,
                                  const std::function< void(int&, std::size_t) >& elementFunction)
  {
    std::set< Point > seen = {};

    Bounds bounds{0, dimensions.rows - 1, 0, dimensions.columns - 1};
    Point position{bounds.left, bounds.bottom};

    const std::size_t totalElements = dimensions.rows * dimensions.columns;

    Direction lastMovementDirection = UP;

    for (std::size_t i = 0; i < totalElements; ++i) {
      seen.insert(position);

      const std::size_t index = (position.y * dimensions.columns) + position.x;

      int& element = matrix[index];
      elementFunction(element, i);

      Direction direction = getNextMovementDirection(position, lastMovementDirection, bounds, seen);
      lastMovementDirection = direction;

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

    int NUM_ARG = 0;

    try {
      std::size_t endPosition = 0;

      NUM_ARG = std::stoi(argv[1], &endPosition);
      if (endPosition != std::string(argv[1]).length()) {
        throw std::invalid_argument("First parameter is not a number");
      }
    } catch (const std::invalid_argument&) {
      throw std::invalid_argument("First parameter is not a number");
    }

    if (NUM_ARG > 2 || NUM_ARG < 1) {
      throw std::invalid_argument("First parameter is out of range");
    }

    const bool USE_DYNAMIC_ARRAY = (NUM_ARG == 2);

    const std::string INPUT_FILE_PATH = argv[2];
    const std::string OUTPUT_FILE_PATH = argv[3];

    std::ifstream inputFile(INPUT_FILE_PATH);
    if (!inputFile.is_open()) {
      throw hedeya::invalid_matrix_error("Input file failed to open");
    }

    hedeya::Dimensions dimensions{0, 0};
    if (!(inputFile >> dimensions.rows >> dimensions.columns)) {
      throw hedeya::invalid_matrix_error("Number of rows/columns is not a valid number");
    }

    if (dimensions.rows < 0 || dimensions.columns < 0) {
      throw hedeya::invalid_matrix_error("Number of rows/columns cannot be negative");
    }

    std::size_t totalElements = dimensions.rows * dimensions.columns;

    if (USE_DYNAMIC_ARRAY) {
      std::vector< int > dynamicMatrix(totalElements, 0);

      hedeya::fillMatrix(inputFile, dimensions, dynamicMatrix);
      hedeya::rejectExtraData(inputFile);

      hedeya::iterateThroughMatrixSpiral(dimensions, dynamicMatrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, dynamicMatrix, OUTPUT_FILE_PATH);
    } else {
      if (totalElements > hedeya::MAX_ELEMENTS) {
        throw hedeya::invalid_matrix_error("Matrix exceeds fixed-size array capacity");
      }

      std::array< int, hedeya::MAX_ELEMENTS > staticMatrix{};

      hedeya::fillMatrix(inputFile, dimensions, staticMatrix.data());
      hedeya::rejectExtraData(inputFile);

      hedeya::iterateThroughMatrixSpiral(dimensions, staticMatrix.data(), hedeya::subtractDifferenceFromElement);

      hedeya::outputMatrixToFile(dimensions, staticMatrix.data(), OUTPUT_FILE_PATH);
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
