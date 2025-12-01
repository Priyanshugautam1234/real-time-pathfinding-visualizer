# 🚀 Real-Time Pathfinding Visualizer

A high-performance **C++ + WebSocket** powered pathfinding engine with modern real-time visualization using **HTML Canvas**.

---

## 🔍 Overview

This project is an end-to-end real-time pathfinding visualizer that streams algorithm progress directly from a **C++ backend** to a modern **Web UI** using WebSockets.

It supports **BFS**, **Dijkstra**, and **A\*** with:

- Live animated exploration  
- Adjustable speed  
- Dynamic coloring of visited nodes  
- Highlighted final shortest path  

This tool is ideal for:  
✔ Algorithm visualization  
✔ Educational demos  
✔ System design understanding  
✔ Research experiments  

---

## ✨ Features

### 🧠 Algorithms Supported
- **A\*** Pathfinding  
- **Dijkstra’s** Algorithm  
- **Breadth-First Search (BFS)**  

---

## 🎥 Real-Time Visualization
- Live grid rendering via WebSocket frames  
- Clear node states with modern color mapping  
- Smooth animations with auto-throttling  
- Final shortest path highlighted cleanly  

---

## ⚡ High-Performance C++ Engine
- Fast grid computation  
- Dynamic frame throttling  
- No lag even at high frame rates  
- Efficient memory use  

---

## 🎨 Modern Web UI
- Glassmorphism-styled sidebar  
- Canvas-based grid rendering  
- Fully responsive control panel  
- **Connect / Start / Stop / Speed Control** support  

---

## 🧰 Tech Stack

### **🖥️ Backend (C++)**
- C++17  
- Custom grid-based pathfinding engine  
- Algorithms: BFS, Dijkstra, A*  
- nlohmann/json (single-header JSON serializer)  
- Real-time state updates & shortest path computation  

### **🔌 Communication Layer**
- WebSockets (Python `websockets` library)  
- asyncio event loop  
- Live JSON frame streaming from C++ → Browser  
- Optimized frame throttling for smooth performance  

### **🌐 Frontend**
- HTML5 Canvas (real-time grid rendering)  
- Vanilla JavaScript (ES6)  
- Modern Glassmorphism UI with custom CSS  
- Interactive controls: Connect, Start, Stop, Speed, Algorithm  

### **🐍 Python Middleware**
- Python 3  
- WebSocket server  
- Manages C++ process execution  
- Streams frames to multiple clients  
- Handles START/STOP commands  

### **🛠️ Build & Tools**
- MinGW / g++ compiler  
- CMake (project structure)  
- Git & GitHub  
- Windows terminal execution  

---

## 🛠️ How to Run
-1️⃣ Start the WebSocket Server  python web_visualizer/live_server.py
-2️⃣ Open the Live UI  start web_visualizer/live.html
### **3️⃣ Run the Visualizer**
-Select Algorithm → BFS / Dijkstra / A*
-Select Speed → Slow / Medium / Fast
-Press Connect WebSocket
-Press Start Live

---





