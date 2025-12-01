# Web Visualizer for Route Planning Project

This folder contains a lightweight web visualizer for your C++ route planning project.

## How it works

1. Build and run the C++ binary and export the computed path and map as JSON:
   ```
   ./your_binary --export-json output.json
   ```

2. Open `web_visualizer/index.html` in your browser and load the exported `output.json` file.

## JSON format expected
```json
{
  "width": 80,
  "height": 40,
  "grid": [[0,1,0,...], ...],
  "start": {"x": 1, "y": 2},
  "goal": {"x": 78, "y": 35},
  "path": [{"x":2,"y":2}, {"x":3,"y":2}, ...]
}
```

The C++ changes to export this JSON are added as `export_json_stub.cpp` as a helper — you can integrate into your project's main logic.
