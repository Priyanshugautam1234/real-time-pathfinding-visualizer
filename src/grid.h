#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>

struct Cell {
    int row, col;
    bool is_wall;
    bool visited;
    bool in_path;
    bool is_start;
    bool is_goal;
    bool is_moving_wall;
    int move_dir;
};

class Grid {
public:
    int rows, cols;
    std::vector<std::vector<Cell>> cells;

    Grid(int r, int c);

    void display() const;
    void reset();

    void generateRandomWalls(double probability);
    void updateMovingWalls();
};

#endif
