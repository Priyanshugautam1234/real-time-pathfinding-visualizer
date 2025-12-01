#include "grid.h"
#include "algorithms.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include "json.hpp"
#include <string>

using json = nlohmann::json;
using std::string;

int SPEED_MS = 150;
bool LIVE_OUTPUT = false;

json exportToJson(Grid &grid, std::pair<int,int> start, std::pair<int,int> goal) {
    json j;
    j["width"]  = grid.cols;
    j["height"] = grid.rows;
    j["grid"] = json::array();
    for (int r = 0; r < grid.rows; r++) {
        json row = json::array();
        for (int c = 0; c < grid.cols; c++) row.push_back(grid.cells[r][c].is_wall ? 1 : 0);
        j["grid"].push_back(row);
    }
    j["start"] = { {"x", start.second}, {"y", start.first} };
    j["goal"]  = { {"x", goal.second}, {"y", goal.first} };
    j["path"] = json::array();
    for (int r = 0; r < grid.rows; r++)
        for (int c = 0; c < grid.cols; c++)
            if (grid.cells[r][c].in_path) j["path"].push_back({ {"x", c}, {"y", r} });
    return j;
}

int main(int argc, char** argv) {
    bool live_mode = false;
    string algo_arg = "";
    string speed_arg = "";
    double wall_prob = 0.08; // default wall density
    const int MAX_ATTEMPTS = 6; // how many retries to try to find a solvable map

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--live") live_mode = true;
        else if (arg.rfind("--algo=",0) == 0) algo_arg = arg.substr(7);
        else if (arg.rfind("--speed=",0) == 0) speed_arg = arg.substr(8);
        else if (arg.rfind("--walls=",0) == 0) {
            try { wall_prob = std::stod(arg.substr(8)); } catch(...) {}
        }
    }

    LIVE_OUTPUT = live_mode;

    system("chcp 65001 > nul");

    // grid size
    Grid grid(25, 60);

    std::pair<int,int> start = {0,0};
    std::pair<int,int> goal  = {grid.rows - 1, grid.cols - 1};

    // If live, slightly reduce wall density for nicer visuals (you can change)
    if (live_mode) {
        if (wall_prob < 0.01) wall_prob = 0.06; // fallback
        else wall_prob = wall_prob; // if user provided --walls=...
    }

    int algo = 3; // default A*
    if (live_mode) {
        // allow either numeric or textual args
        if (algo_arg == "1" || algo_arg == "bfs") algo = 1;
        if (algo_arg == "2" || algo_arg == "dijkstra") algo = 2;
        if (algo_arg == "3" || algo_arg == "astar") algo = 3;

        // speed_arg may be numeric (ms) or friendly string
        if (!speed_arg.empty()) {
            try {
                int s = std::stoi(speed_arg);
                SPEED_MS = s;
            } catch(...) {
                if (speed_arg == "slow") SPEED_MS = 800;
                else if (speed_arg == "medium") SPEED_MS = 250;
                else if (speed_arg == "fast") SPEED_MS = 80;
                else SPEED_MS = 120;
            }
        } else {
            SPEED_MS = 120;
        }

        std::cout << "[LIVE] algo=" << algo << " speed_ms=" << SPEED_MS << " wall_prob=" << wall_prob << std::endl;
    } else {
        std::cout << "\nSelect Algorithm:\n1. BFS\n2. Dijkstra\n3. A*\nChoice: ";
        std::cin >> algo;
        std::cout << "\nSelect Speed:\n1. Slow\n2. Medium\n3. Fast\nChoice: ";
        int sp; std::cin >> sp;
        if (sp==1) SPEED_MS=1500; else if (sp==2) SPEED_MS=500; else SPEED_MS=120;
    }

    bool success = false;
    int attempt = 0;

    // Try generating maps until algorithm finds a path (useful for live demos)
    while (attempt < MAX_ATTEMPTS && !success) {
        attempt++;
        // reset grid cleanly
        grid = Grid(grid.rows, grid.cols);

        // place start/goal before making walls so generateRandomWalls will avoid them
        grid.cells[start.first][start.second].is_start = true;
        grid.cells[goal.first][goal.second].is_goal = true;

        // create walls
        srand((unsigned)time(NULL) + attempt * 7);
        grid.generateRandomWalls(wall_prob);

        std::cout << "Attempt " << attempt << ": generating map (walls=" << wall_prob << ")..." << std::endl;

        // Run algorithm in "test" mode to check solvability quickly:
        // we use the same functions — they stream frames if LIVE_OUTPUT true.
        // Temporarily turn off LIVE_OUTPUT for quick test runs when not live, but for live keep streaming.
        bool prev_live = LIVE_OUTPUT;
        if (!live_mode) LIVE_OUTPUT = false; // ensure console mode doesn't spam
        if (algo == 1) success = bfs(grid, start, goal, SPEED_MS);
        else if (algo == 2) success = dijkstra(grid, start, goal, SPEED_MS);
        else success = astar(grid, start, goal, SPEED_MS);
        LIVE_OUTPUT = prev_live;

        if (!success) {
            std::cout << "No path found on attempt " << attempt << ". Retrying with new map...\n\n";
        } else {
            std::cout << "Path found on attempt " << attempt << ".\n";
            break;
        }
    }

    if (!success) {
        std::cout << "✘ No path found after " << MAX_ATTEMPTS << " attempts. Lower wall density or try manual map.\n";
    } else {
        std::cout << "\n✔ Path Found!\n";
    }

    // Export final grid (if path found exports path)
    json exported = exportToJson(grid, start, goal);
    std::ofstream out("exported_path.json");
    out << exported.dump(4);
    out.close();

    if (!live_mode) {
        std::cout << "\nJSON exported to: exported_path.json\nPress Enter to exit...";
        std::cin.ignore(); std::cin.get();
    }

    return 0;
}
