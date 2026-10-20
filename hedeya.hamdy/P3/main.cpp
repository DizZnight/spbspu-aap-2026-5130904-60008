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
void outputMatrixToFile(int rows, int columns, const std::vector<std::vector<int>>& matrix, std::ofstream& outputFile);

int main(int argc, char* argv[])
{
    if(argc < 3){
        return EXIT_FAILURE;
    }

    int num = 2; //std::stoi(argv[1]);
    if(num != 1 && num != 2){
        return EXIT_FAILURE;
    }

    bool useDynamicArray = (num == 2);

    std::string inputFilePath = "TestCase1.txt"; //argv[2];
    std::string outputFilePath = "Output.txt"; //argv[3];

    std::ifstream input(inputFilePath);

    int rows = 0;
    int columns = 0;
    input >> rows >> columns;

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(columns, 0));

    //if(useDynamicArray){
    //    std::vector<std::vector<int>> matrix(rows, std::vector<int>(columns, 0));
    //} else {
    //    std::array<std::array<int, columns>, rows> matrix = {0};
    //}

    for (int row = 0; row < rows; ++row){
        for (int column = 0; column < columns; ++column){
            input >> matrix[row][column];
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
        matrix[position.y][position.x] -= difference;

        enum Direction direction = getNextMovementDirection(position, bounds, seen);

        move(direction, position);

        difference += 1;
    }

    printMatrix(rows, columns, matrix);

    std::ofstream outputFile(outputFilePath);

    if (!outputFile.is_open()) {
        return EXIT_FAILURE;
    }

    outputMatrixToFile(rows, columns, matrix, outputFile);

    return EXIT_SUCCESS;
}

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

void outputMatrixToFile(int rows, int columns, const std::vector<std::vector<int>>& matrix, std::ofstream& outputFile)
{
    outputFile << rows << " " << columns << " ";
    for (const std::vector<int>& row : matrix) {
        for (const int element : row) {
            outputFile << element << " ";
        }
    }
}

#endif