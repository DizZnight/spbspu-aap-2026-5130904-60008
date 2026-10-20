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

namespace hedeya{
    const int MAX_ELEMENTS = 10000;
    const int MAX_ROWS = 100;
    const int MAX_COLS = 100;

    enum Direction {
            UP,
            DOWN,
            LEFT,
            RIGHT,
        };

    struct Bounds {
        int top, bottom, left, right;
    };

    struct Point {
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

    void printMatrix(int rows, int columns, const std::vector<std::vector<int>>& matrix)
    {
        std::cout << rows << " " << columns << " ";
        for (const std::vector<int>& row : matrix) {
            for (const int element : row) {
                std::cout << element << " ";
            }
        }
    }

    void printMatrix(int rows, int columns, const int matrix[][MAX_COLS])
    {
        std::cout << rows << " " << columns << " ";
        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                std::cout << matrix[row][column] << " ";
            }
        }
    }

    void outputMatrixToFile(int rows, int columns, const std::vector<std::vector<int>>& matrix, std::ofstream& outputFile)
    {
        outputFile << rows << " " << columns << " ";
        for (const std::vector<int>& row : matrix) {
            for (const int element : row) {
                outputFile << element << " ";
            }
        }
    }

    void outputMatrixToFile(int rows, int columns, const int matrix[][MAX_COLS], std::ofstream& outputFile)
{
    outputFile << rows << " " << columns << " ";
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            outputFile << matrix[row][column] << " ";
        }
    }
}
}

int main(int argc, char* argv[])
{
    try
    {
        if(argc < 3){
            throw std::invalid_argument("3 Arguments were expected: num, input, output");
            return EXIT_FAILURE;
        }

        int num = std::stoi(argv[1]);
        if(num != 1 && num != 2){
            throw std::invalid_argument("Num must be either 1 for a static array or 2 for a dynamic array");
            return EXIT_FAILURE;
        }

        bool useDynamicArray = (num == 2);

        std::string inputFilePath = argv[2];
        std::string outputFilePath = argv[3];

        std::ifstream input(inputFilePath);

        int rows = 0;
        int columns = 0;
        input >> rows >> columns;

        int staticMatrix[MAX_ROWS][MAX_COLS] = {0};
        std::vector<std::vector<int>> dynamicMatrix;

    if (useDynamicArray) {
            dynamicMatrix.assign(rows, std::vector<int>(columns, 0));
        } else {
            if ((rows * columns > MAX_ELEMENTS) || rows > MAX_ROWS || columns > MAX_COLS) {
                throw std::range_error("Number of elements, rows or columns exceeds the maximum, try using a dynamic array instead");
            }
        }

        for (int row = 0; row < rows; ++row){
            for (int column = 0; column < columns; ++column){

            if (useDynamicArray) {
                input >> dynamicMatrix[row][column];
            } else {
                input >> staticMatrix[row][column];
            }
            }
        }

        Bounds bounds{0, rows - 1, 0, columns - 1}; // top, bottom, left, right

        Point position{bounds.left, bounds.bottom}; // x, y
        Point previousPosition{position.x, position.y}; // x, y

        std::set<Point> seen = {};

        int difference = 1;

        for(int i = 0; i < rows * columns; ++i)
        {
            seen.insert(position);

            int& element = useDynamicArray ? dynamicMatrix[position.y][position.x] : staticMatrix[position.y][position.x];
            element -= difference;

            enum Direction direction = getNextMovementDirection(position, bounds, seen);

            move(direction, position);

            difference += 1;
        }

        if(useDynamicArray){
            printMatrix(rows, columns, dynamicMatrix);
        } else {
            printMatrix(rows, columns, staticMatrix);
        }


        std::ofstream outputFile(outputFilePath);

        if (!outputFile.is_open()) {
            throw std::runtime_error("Output file failed to open");
            return EXIT_FAILURE;
        }

        if(useDynamicArray){
            outputMatrixToFile(rows, columns, dynamicMatrix, outputFile);
        } else {
            outputMatrixToFile(rows, columns, staticMatrix, outputFile);
        }


        return EXIT_SUCCESS;
    }
    catch (const std::invalid_argument& error) {
        std::cerr << "Invalid argument error: " << error.what() << "\n";
    }
    catch (const std::range_error& error) {
        std::cerr << "Range error: " << error.what() << "\n";
    }
}

#endif