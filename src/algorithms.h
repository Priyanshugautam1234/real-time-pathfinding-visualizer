#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "grid.h"
#include <utility>

bool bfs(Grid &grid, std::pair<int,int> start, std::pair<int,int> goal, int speed_ms);
bool dijkstra(Grid &grid, std::pair<int,int> start, std::pair<int,int> goal, int speed_ms);
bool astar(Grid &grid, std::pair<int,int> start, std::pair<int,int> goal, int speed_ms);

#endif
