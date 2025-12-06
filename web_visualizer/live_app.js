let ws = null;
const statusEl = document.getElementById("status");
const canvas = document.getElementById("c");
const ctx = canvas.getContext("2d");

// Load Assets
const imgHome = new Image(); imgHome.src = "assets/home.png";
const imgSchool = new Image(); imgSchool.src = "assets/school.png";

let cellSize = 22; // Slightly larger for retro feel

function drawFrame(frame) {
    const w = frame.width, h = frame.height;
    // Resize with crisp pixel art settings
    canvas.width = w * cellSize;
    canvas.height = h * cellSize;

    ctx.imageSmoothingEnabled = false;

    // Clear background
    ctx.fillStyle = "#050510";
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    for (let y = 0; y < h; y++) {
        for (let x = 0; x < w; x++) {
            const v = frame.cells[y][x];

            let fill = null;
            let stroke = null;

            // Map Values
            if (v === 1) { // Wall
                fill = "#566573";
                stroke = "#2C3E50";
            } else if (v === 2) { // Moving Wall
                fill = "#8E44AD";
            } else if (v === 4) { // Visited
                fill = "#1B4F72"; // Deep Blue
                stroke = "#2874A6";
            } else if (v === 3) { // Path
                fill = "#F1C40F"; // Gold
                // visual "pop" for path
                ctx.shadowColor = "#F39C12";
                ctx.shadowBlur = 10;
            } else {
                // Empty
                fill = "#0B1019";
                stroke = "#1C2833";
            }

            if (fill) {
                ctx.fillStyle = fill;
                ctx.fillRect(x * cellSize, y * cellSize, cellSize, cellSize);
                ctx.shadowBlur = 0; // reset shadow
            }

            if (stroke) {
                ctx.strokeStyle = stroke;
                ctx.lineWidth = 1;
                ctx.strokeRect(x * cellSize, y * cellSize, cellSize, cellSize);
            }
        }
    }

    // Draw Icons (Centered)
    if (frame.start && imgHome.complete) {
        ctx.drawImage(imgHome, frame.start.x * cellSize, frame.start.y * cellSize, cellSize, cellSize);
    }

    if (frame.goal && imgSchool.complete) {
        ctx.drawImage(imgSchool, frame.goal.x * cellSize, frame.goal.y * cellSize, cellSize, cellSize);
    }
}

document.getElementById("connect").onclick = () => {
    if (ws && ws.readyState === WebSocket.OPEN) return;
    ws = new WebSocket("ws://localhost:9002");

    ws.onopen = () => {
        statusEl.textContent = "CONNECTED";
        statusEl.style.color = "#2ECC71";
        console.log("WS connected");
    };

    ws.onmessage = (e) => {
        if (e.data === "END") {
            statusEl.textContent = "MISSION COMPLETE";
            statusEl.style.color = "#F1C40F";
            return;
        }
        try {
            statusEl.textContent = "SCANNING SECTOR...";
            statusEl.style.color = "#3498DB";
            drawFrame(JSON.parse(e.data));
        }
        catch (err) { console.log("Bad frame", err); }
    };

    ws.onclose = () => {
        statusEl.textContent = "DISCONNECTED";
        statusEl.style.color = "#E74C3C";
        console.log("WS closed");
    };
};

document.getElementById("start").onclick = () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) {
        alert("INSERT COIN FIRST (Connect WebSocket)!");
        return;
    }
    const algo = document.getElementById("algo").value;
    const speed = document.getElementById("speed").value;
    const msg = { cmd: "start", algo: algo, speed: speed };
    ws.send(JSON.stringify(msg));

    statusEl.textContent = "SEARCHING FOR SCHOOL...";
};

document.getElementById("stop").onclick = () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    ws.send(JSON.stringify({ cmd: "stop" }));
    statusEl.textContent = "ABORTED";
    statusEl.style.color = "#E74C3C";
};
