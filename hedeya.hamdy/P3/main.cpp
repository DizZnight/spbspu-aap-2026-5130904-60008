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
};

enum Direction getNextMovementDirection(Point currentPosition, Bounds bounds, Point previousPosition);
void move(enum Direction direction, Point &position);
int getNextPosition(Direction direction, Point position);
void printMatrix(const std::vector<std::vector<int>>& matrix);

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    int rows = 0, columns = 0;
    input >> rows >> columns;

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(columns, 0));

    for (int row = 0; row < rows; ++row){
        for (int column = 0; column < columns; ++column){
            input >> matrix[row][column];
        }
    }

    Bounds bounds{0, rows - 1, 0, columns - 1}; // top, bottom, left, right

    Point position{bounds.left, bounds.bottom}; // x, y
    Point previousPosition{position.x, position.y}; // x, y

    int count = 1;

    for(int i = 0; i < rows * columns; ++i)
    {
        matrix[position.y][position.x] -= count;

        enum Direction direction = getNextMovementDirection(position, bounds, previousPosition);

        previousPosition = position;
        move(direction, position);

        count += 1;
    }

    printMatrix(matrix);

    return EXIT_SUCCESS;
}

enum Direction getNextMovementDirection(Point currentPosition, Bounds bounds, Point previousPosition)
{
    enum Direction direction = UP;

    if(currentPosition.y > bounds.top && getNextPosition(UP, currentPosition) != previousPosition.y){
        direction = UP;
    }
    else if(currentPosition.x < bounds.right && getNextPosition(RIGHT, currentPosition) != previousPosition.x){
        direction = RIGHT;
    }
    else if(currentPosition.y < bounds.bottom && getNextPosition(DOWN, currentPosition) != previousPosition.y){
        direction = DOWN;
    }
    else if(currentPosition.x > bounds.left && getNextPosition(LEFT, currentPosition) != previousPosition.x){
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

int getNextPosition(Direction direction, Point position)
{
    switch (direction) {
        case UP:    return position.y - 1;
        case RIGHT: return position.x + 1;
        case DOWN:  return position.y + 1;
        case LEFT:  return position.x - 1;
    }
}

void printMatrix(const std::vector<std::vector<int>>& matrix)
{
    for (const std::vector<int>& row : matrix) {
        for (const int element : row) {
            std::cout << element << "\n";
        }
    }
}

#endif