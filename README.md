#  Pathfinding Quest (Real-Time Visualizer)

A high-performance **C++ + WebSocket** powered pathfinding engine with a **Retro Arcade** real-time visualization using **HTML Canvas**.

---

##  Overview

This project is an end-to-end real-time pathfinding visualizer that streams algorithm progress directly from a **C++ backend** to a **Retrowave Web UI** using WebSockets.

It supports **BFS**, **Dijkstra**, and **A\*** with:

- Live animated exploration  
- Adjustable speed (Turbo, Normal, Slow-Mo)  
- Dynamic glowing paths  
- Pixel-art **Home** and **School** icons  

This tool is ideal for:  
✔ Algorithm visualization  
✔ Gamified education  
✔ System design understanding  
✔ Research experiments  

---

##  Features

###  Algorithms Supported
- **A\*** Pathfinding  
- **Dijkstra’s** Algorithm  
- **Breadth-First Search (BFS)**  

---

##  Real-Time Visualization
- Live grid rendering via WebSocket frames  
- **CRT Scanline** effects for retro vibe  
- Smooth animations with auto-throttling  
- **Glowing Gold** shortest path trail  

---

## High-Performance C++ Engine
- Fast grid computation  
- Dynamic frame throttling  
- No lag even at high frame rates  
- Efficient memory use  

---

##  Retro Arcade UI
- **Pathfinding Quest** theme  
- **Pixel-art fonts** ('Press Start 2P')  
- **Mission Control** sidebar  
- **Responsive Design**: Adapts for split-screen and mobile  
- **Connect / Start / Stop** arcade buttons  

---

##  Tech Stack

### ** Backend (C++)**
- C++17  
- Custom grid-based pathfinding engine  
- Algorithms: BFS, Dijkstra, A*  
- nlohmann/json  

### **🔌 Communication Layer**
- WebSockets (Python `asyncio`)  
- Robust non-blocking process management  
- Live JSON frame streaming  

### ** Frontend**
- HTML5 Canvas  
- Vanilla JavaScript  
- CSS3 (Flexbox, Grid, CRT Effects)  
- Responsive Media Queries  

### ** Python Middleware**
- Python 3 Asyncio Server  

---

## 🛠️ How to Run

### **1️⃣ Start the WebSocket Server**
```bash
cd web_visualizer
python live_server.py
```

### **2️⃣ Open the Game**
- Open `http://localhost:8000/live.html`
- Or open `web_visualizer/live.html` directly.

### **3️⃣ Play**  
- Select **Algorithm**  
- Select **Speed**  
- Click **CONNECT** (Insert Coin)  
- Click **START MISSION**
