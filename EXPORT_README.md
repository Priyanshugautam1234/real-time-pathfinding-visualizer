A quick note:
We added `web_visualizer/` with index.html and app.js, and a C++ example `export_json_stub.cpp`.
To produce JSON from your real program, integrate the JSON export logic after your pathfinder computes the path.
We suggest using https://github.com/nlohmann/json single-header library (add json.hpp into thirdparty or use package manager).
