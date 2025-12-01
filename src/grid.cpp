#include "grid.h"
#include <iostream>
#include <cstdlib>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define PURPLE  "\033[35m"
#define BROWN   "\033[38;5;94m"

Grid::Grid(int r, int c) : rows(r), cols(c) {
    cells.resize(rows, std::vector<Cell>(cols));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cells[i][j] = { i, j, false, false, false, false, false, false, 1 };
}

void Grid::display() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            const auto& cell = cells[i][j];
            if (cell.is_start) std::cout << GREEN << "S " << RESET;
            else if (cell.is_goal) std::cout << RED << "G " << RESET;
            else if (cell.is_moving_wall) std::cout << PURPLE << "@ " << RESET;
            else if (cell.is_wall) std::cout << BROWN << "█ " << RESET;
            else if (cell.in_path) std::cout << YELLOW << "* " << RESET;
            else if (cell.visited) std::cout << RED << ". " << RESET;
            else std::cout << "· ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Grid::reset() {
    for (auto &row : cells)
        for (auto &cell : row) {
            cell.visited = false;
            cell.in_path = false;
        }
}

void Grid::generateRandomWalls(double prob) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            cells[r][c].is_wall = false;

    int safe = 3;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (cells[r][c].is_start || cells[r][c].is_goal) continue;
            if (r <= safe && c <= safe) continue;
            if (r >= rows - safe - 1 && c >= cols - safe - 1) continue;
            double random = (double)rand() / RAND_MAX;
            if (random < prob) cells[r][c].is_wall = true;
        }
    }

    for (int r = 0; r < rows; r++) cells[r][1].is_wall = false;
    for (int c = 0; c < cols; c++) cells[rows - 2][c].is_wall = false;
}

void Grid::updateMovingWalls() {
    std::vector<std::vector<Cell>> next = cells;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Cell &cell = cells[r][c];
            if (!cell.is_moving_wall) continue;
            int newC = c + cell.move_dir;
            if (newC < 0 || newC >= cols || cells[r][newC].is_wall) {
                cell.move_dir *= -1;
                newC = c + cell.move_dir;
            }
            next[r][c].is_moving_wall = false;
            next[r][newC].is_moving_wall = true;
            next[r][newC].move_dir = cell.move_dir;
        }
    }
    cells = next;
}
