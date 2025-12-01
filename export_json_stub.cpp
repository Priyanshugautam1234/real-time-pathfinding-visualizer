// export_json_stub.cpp
// This is a helper example to show how to export the grid, start, goal and path as JSON.
// Integrate similar code into your project's main after computing the path.

#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp> // we suggest using nlohmann/json single header

using json = nlohmann::json;

struct Cell { int x; int y; };
int main_demo_export() {
    // Dummy small example grid
    int width = 30, height = 20;
    std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));
    // mark some walls
    for (int x=5;x<25;++x) grid[6][x] = 1;
    json j;
    j["width"] = width;
    j["height"] = height;
    j["grid"] = json::array();
    for (int y=0;y<height;++y) {
        json row = json::array();
        for (int x=0;x<width;++x) row.push_back(grid[y][x]);
        j["grid"].push_back(row);
    }
    j["start"] = { {"x",2}, {"y",2} };
    j["goal"] = { {"x",28}, {"y",18} };
    j["path"] = json::array();
    // example path
    j["path"].push_back({{"x",2},{"y",2}});
    j["path"].push_back({{"x",3},{"y",2}});
    // write to file
    std::ofstream ofs("exported_path.json");
    ofs << j.dump(2) << std::endl;
    return 0;
}
