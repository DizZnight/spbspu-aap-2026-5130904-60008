#ifndef MAIN_CPP
#define MAIN_CPP

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <set>
#include <variant>
#include <functional>

namespace hedeya
{
    const int MAX_ELEMENTS = 10000;
    const int MAX_ROWS = 100;
    const int MAX_COLS = 100;

    enum Direction
    {
            UP,
            DOWN,
            LEFT,
            RIGHT,
        };

    struct Bounds
    {
        int top, bottom, left, right;
    };

    struct Dimensions
    {
        int rows, columns;
    };

    struct Point
    {
        int x, y;

        bool operator<(const Point& other) const {
            if (x != other.x) return x < other.x;
            return y < other.y;
        }
    };

    Point getNextPosition(Direction direction, Point position);
    enum Direction getNextMovementDirection(Point currentPosition, Bounds bounds, const std::set<Point>& seen);
    void move(enum Direction direction, Point &position);

    void printMatrix(int rows, int columns, const std::vector<std::vector<int>>& matrix);
    void printMatrix(int rows, int columns, const int matrix[][MAX_COLS]);

    void outputMatrixToFile(int rows, int columns, const std::vector<std::vector<int>>& matrix, std::ofstream& outputFile);
    void outputMatrixToFile(int rows, int columns, const int matrix[][MAX_COLS], std::ofstream& outputFile);

    void fillMatrix(std::ifstream& inputFile, Dimensions& dimensions, std::vector<std::vector<int>> &matrix);
    void fillMatrix(std::ifstream& inputFile, Dimensions& dimensions, int (&matrix)[MAX_ROWS][MAX_COLS]);

    void subtractDifferenceFromElement(int &element, int i);

    void iterateThroughMatrixSpiral(Dimensions dimensions, std::vector<std::vector<int>> &matrix, const std::function<void(int&, int)>& elementFunction);
    void iterateThroughMatrixSpiral(Dimensions dimensions, int (&matrix)[MAX_ROWS][MAX_COLS], const std::function<void(int&, int)>& elementFunction);

    Point getNextPosition(Direction direction, Point position)
    {
        switch (direction) {
            case UP:    return Point{position.x, position.y - 1};
            case RIGHT: return Point{position.x + 1, position.y};
            case DOWN:  return Point{position.x, position.y + 1};
            case LEFT:  return Point{position.x - 1, position.y};
        }
        return position;
    }

    enum Direction getNextMovementDirection(Point currentPosition, Bounds bounds, const std::set<Point>& seen)
    {
        enum Direction direction = UP;

        if(currentPosition.y != bounds.top && !seen.count(getNextPosition(UP, currentPosition))){
            direction = UP;
        }
        else if(currentPosition.x != bounds.right && !seen.count(getNextPosition(RIGHT, currentPosition))){
            direction = RIGHT;
        }
        else if(currentPosition.y != bounds.bottom && !seen.count(getNextPosition(DOWN, currentPosition))){
            direction = DOWN;
        }
        else if(currentPosition.x != bounds.left && !seen.count(getNextPosition(LEFT, currentPosition))){
            direction = LEFT;
        }

        return direction;
    }

    void move(enum Direction direction, Point &position)
    {
        switch (direction) {
            case UP:    position.y -= 1; break;
            case RIGHT: position.x += 1; break;
            case DOWN:  position.y += 1; break;
            case LEFT:  position.x -= 1; break;
        }
    }

    void outputMatrixToFile(Dimensions dimensions, const std::vector<std::vector<int>>& matrix, const std::string& outputFilePath)
    {
        std::ofstream outputFile(outputFilePath);

        if (!outputFile.is_open()) {
            throw std::runtime_error("Output file failed to open");
        }

        outputFile << dimensions.rows << " " << dimensions.columns << " ";
        for (const std::vector<int>& row : matrix) {
            for (const int element : row) {
                outputFile << element << " ";
            }
        }
    }

    void outputMatrixToFile(Dimensions dimensions, const int matrix[][MAX_COLS], const std::string& outputFilePath)
    {
        std::ofstream outputFile(outputFilePath);

        if (!outputFile.is_open()) {
            throw std::runtime_error("Output file failed to open");
        }

        outputFile << dimensions.rows << " " << dimensions.columns << " ";
        for (int row = 0; row < dimensions.rows; ++row) {
            for (int column = 0; column < dimensions.columns; ++column) {
                outputFile << matrix[row][column] << " ";
            }
        }
    }

    void fillMatrix(std::ifstream& inputFile, Dimensions& dimensions, std::vector<std::vector<int>> &matrix)
    {
        int rows = dimensions.rows;
        int columns = dimensions.columns;

        for (int row = 0; row < rows; ++row){
            for (int column = 0; column < columns; ++column){
                inputFile >> matrix[row][column];
            }
        }
    }

    void fillMatrix(std::ifstream& inputFile, Dimensions& dimensions, int (&matrix)[MAX_ROWS][MAX_COLS])
    {
        int rows = dimensions.rows;
        int columns = dimensions.columns;

        for (int row = 0; row < rows; ++row){
            for (int column = 0; column < columns; ++column){
                inputFile >> matrix[row][column];
            }
        }
    }

    void subtractDifferenceFromElement(int &element, int i)
    {
        int difference = i + 1;
        element -= difference;
    }

    void iterateThroughMatrixSpiral(Dimensions dimensions, std::vector<std::vector<int>> &matrix, const std::function<void(int&, int)>&& elementFunction)
    {
        std::set<Point> seen = {};

        Bounds bounds{0, dimensions.rows - 1, 0, dimensions.columns - 1};
        Point position{bounds.left, bounds.bottom};

        for(int i = 0; i < dimensions.rows * dimensions.columns; ++i)
            {
                seen.insert(position);

                int& element = matrix[position.y][position.x];
                elementFunction(element, i);

                enum Direction direction = getNextMovementDirection(position, bounds, seen);

                move(direction, position);
            }
    }

    void iterateThroughMatrixSpiral(Dimensions dimensions, int (&matrix)[MAX_ROWS][MAX_COLS], const std::function<void(int&, int)>& elementFunction)
    {
        std::set<Point> seen = {};

        Bounds bounds{0, dimensions.rows - 1, 0, dimensions.columns - 1};
        Point position{bounds.left, bounds.bottom};

        for(int i = 0; i < dimensions.rows * dimensions.columns; ++i)
            {
                seen.insert(position);

                int& element = matrix[position.y][position.x];
                elementFunction(element, i);

                enum Direction direction = getNextMovementDirection(position, bounds, seen);

                move(direction, position);
            }
    }
}

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 4)
        {
            throw std::invalid_argument("Exactly 3 arguments are expected: num, input, output");
        }

        int num = std::stoi(argv[1]);
        if(num != 1 && num != 2)
        {
            throw std::invalid_argument("Num must be either 1 for a static array or 2 for a dynamic array");
        }

        bool useDynamicArray = (num == 2);

        std::string inputFilePath = argv[2];
        std::string outputFilePath = argv[3];

        std::ifstream inputFile(inputFilePath);

        hedeya::Dimensions dimensions{0, 0};
        inputFile >> dimensions.rows >> dimensions.columns;

        int staticMatrix[hedeya::MAX_ROWS][hedeya::MAX_COLS] = {0};
        std::vector<std::vector<int>> dynamicMatrix;

        if (useDynamicArray)
        {
            dynamicMatrix.assign(dimensions.rows, std::vector<int>(dimensions.columns, 0));

            hedeya::fillMatrix(inputFile, dimensions, dynamicMatrix);
            hedeya::iterateThroughMatrixSpiral(dimensions, dynamicMatrix, hedeya::subtractDifferenceFromElement);
            hedeya::outputMatrixToFile(dimensions, dynamicMatrix, outputFilePath);
        } else {
            if (
            (dimensions.rows * dimensions.columns > hedeya::MAX_ELEMENTS)
            || dimensions.rows > hedeya::MAX_ROWS
            || dimensions.columns > hedeya::MAX_COLS)
            {
                throw std::range_error("Number of elements, rows, or columns exceeds the allowed maximum, try using a dynamic array instead");
            }

            hedeya::fillMatrix(inputFile, dimensions, staticMatrix);
            hedeya::iterateThroughMatrixSpiral(dimensions, staticMatrix, hedeya::subtractDifferenceFromElement);
            hedeya::outputMatrixToFile(dimensions, staticMatrix, outputFilePath);
        }

        return EXIT_SUCCESS;
    }
    catch(const std::invalid_argument& error)
    {
        std::cerr << "Invalid argument error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
    catch(const std::range_error& error)
    {
        std::cerr << "Range error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
}

#endif