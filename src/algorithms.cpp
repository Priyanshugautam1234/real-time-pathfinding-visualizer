#include "algorithms.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <limits>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

extern bool LIVE_OUTPUT;
int FRAME_CNT = 0;

vector<pair<int,int>> directions = {
    {1,0}, {-1,0}, {0,1}, {0,-1}
};

static bool isValid(Grid &g, int r, int c) {
    return r >= 0 && c >= 0 && r < g.rows && c < g.cols && !g.cells[r][c].is_wall;
}

static int compute_skip(int speed_ms) {
    if (speed_ms <= 0) return 1;
    int v = 300 / max(10, speed_ms);
    return max(1, v);
}

void sendFrame(Grid &grid, int speed_ms, bool forceSend = false) {

    if (!LIVE_OUTPUT) return;

    FRAME_CNT++;

    if (!forceSend) {
        int skip = compute_skip(speed_ms);
        if (FRAME_CNT % skip != 0) return;
    }

    json frame;
    frame["width"]  = grid.cols;
    frame["height"] = grid.rows;

    frame["cells"] = json::array();

    for (int r = 0; r < grid.rows; r++) {
        json row = json::array();
        for (int c = 0; c < grid.cols; c++) {
            const auto &cell = grid.cells[r][c];
            int v = 0;

            if (cell.is_wall) v = 1;
            if (cell.is_moving_wall) v = 2;
            if (cell.in_path) v = 3;
            else if (cell.visited) v = 4;

            row.push_back(v);
        }
        frame["cells"].push_back(row);
    }

    for (int r = 0; r < grid.rows; r++)
        for (int c = 0; c < grid.cols; c++) {
            if (grid.cells[r][c].is_start)
                frame["start"] = { {"x", c}, {"y", r} };
            if (grid.cells[r][c].is_goal)
                frame["goal"] = { {"x", c}, {"y", r} };
        }

    cout << frame.dump() << endl;
    cout.flush();

    this_thread::sleep_for(chrono::milliseconds(max(10, speed_ms)));
}

/////////////////////////////////////////////////////
// BFS
/////////////////////////////////////////////////////
bool bfs(Grid &grid, pair<int,int> start, pair<int,int> goal, int speed_ms) {

    queue<pair<int,int>> q;
    unordered_map<int,int> parent;
    auto key = [&](int r,int c){ return r * grid.cols + c; };

    q.push(start);
    grid.cells[start.first][start.second].visited = true;

    bool found = false;

    while (!q.empty()) {
        auto [r,c] = q.front(); q.pop();

        if (r == goal.first && c == goal.second) { found = true; break; }

        for (auto [dr,dc] : directions) {
            int nr = r + dr, nc = c + dc;
            if (isValid(grid,nr,nc) && !grid.cells[nr][nc].visited) {

                grid.cells[nr][nc].visited = true;
                parent[key(nr,nc)] = key(r,c);
                q.push({nr,nc});
            }
        }

        grid.updateMovingWalls();
        sendFrame(grid, speed_ms);
    }

    if (!found) return false;

    int r = goal.first, c = goal.second;
    while (!(r == start.first && c == start.second)) {
        grid.cells[r][c].in_path = true;
        int p = parent[key(r,c)];
        r = p / grid.cols;
        c = p % grid.cols;
    }
    grid.cells[start.first][start.second].in_path = true;

    sendFrame(grid, 0, true);
    cout << "END" << endl;
    cout.flush();

    return true;
}

/////////////////////////////////////////////////////
// Dijkstra
/////////////////////////////////////////////////////
bool dijkstra(Grid &grid, pair<int,int> start, pair<int,int> goal, int speed_ms) {

    auto key = [&](int r, int c){ return r * grid.cols + c; };

    vector<int> dist(grid.rows * grid.cols, INT_MAX);
    unordered_map<int,int> parent;

    using Node = pair<int, pair<int,int>>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[key(start.first, start.second)] = 0;
    pq.push({0, start});

    while (!pq.empty()) {

        auto [cost, pos] = pq.top(); pq.pop();
        auto [r, c] = pos;

        if (grid.cells[r][c].visited) continue;
        grid.cells[r][c].visited = true;

        if (r == goal.first && c == goal.second) break;

        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;

            if (!isValid(grid,nr,nc)) continue;

            int nk = key(nr,nc);
            int ck = key(r,c);

            int newCost = cost + 1;

            if (newCost < dist[nk]) {
                dist[nk] = newCost;
                parent[nk] = ck;
                pq.push({newCost, {nr,nc}});
            }
        }

        grid.updateMovingWalls();
        sendFrame(grid, speed_ms);
    }

    if (dist[key(goal.first, goal.second)] == INT_MAX) return false;

    int r = goal.first, c = goal.second;
    while (!(r == start.first && c == start.second)) {
        grid.cells[r][c].in_path = true;
        int p = parent[key(r,c)];
        r = p / grid.cols;
        c = p % grid.cols;
    }

    grid.cells[start.first][start.second].in_path = true;

    sendFrame(grid, 0, true);
    cout << "END" << endl;
    cout.flush();

    return true;
}

/////////////////////////////////////////////////////
// A*
/////////////////////////////////////////////////////
bool astar(Grid &grid, pair<int,int> start, pair<int,int> goal, int speed_ms) {

    auto key = [&](int r, int c){ return r * grid.cols + c; };

    vector<int> gCost(grid.rows * grid.cols, INT_MAX);
    vector<int> fCost(grid.rows * grid.cols, INT_MAX);
    unordered_map<int,int> parent;

    auto heuristic = [&](int r, int c){
        return abs(r - goal.first) + abs(c - goal.second);
    };

    using Node = pair<int, pair<int,int>>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    int startKey = key(start.first, start.second);

    gCost[startKey] = 0;
    fCost[startKey] = heuristic(start.first, start.second);

    pq.push({fCost[startKey], start});

    while (!pq.empty()) {

        auto [f, pos] = pq.top(); pq.pop();
        int r = pos.first, c = pos.second;

        if (grid.cells[r][c].visited) continue;

        grid.cells[r][c].visited = true;

        if (r == goal.first && c == goal.second) break;

        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;

            if (!isValid(grid,nr,nc)) continue;

            int nk = key(nr,nc);
            int ck = key(r,c);

            int tentative = gCost[ck] + 1;

            if (tentative < gCost[nk]) {
                parent[nk] = ck;
                gCost[nk] = tentative;
                fCost[nk] = tentative + heuristic(nr,nc);
                pq.push({fCost[nk], {nr,nc}});
            }
        }

        grid.updateMovingWalls();
        sendFrame(grid, speed_ms);
    }

    int r = goal.first, c = goal.second;
    if (gCost[key(r,c)] == INT_MAX) return false;

    while (!(r == start.first && c == start.second)) {
        grid.cells[r][c].in_path = true;
        int p = parent[key(r,c)];
        r = p / grid.cols;
        c = p % grid.cols;
    }

    grid.cells[start.first][start.second].in_path = true;

    sendFrame(grid, 0, true);
    cout << "END" << endl;
    cout.flush();

    return true;
}
