let ws = null;
const statusEl = document.getElementById("status");
const canvas = document.getElementById("c");
const ctx = canvas.getContext("2d");

let cellSize = 18;

function drawFrame(frame) {
    const w = frame.width, h = frame.height;
    canvas.width = w * cellSize;
    canvas.height = h * cellSize;
    ctx.lineWidth = 0.35;

    for (let y = 0; y < h; y++) {
        for (let x = 0; x < w; x++) {
            const v = frame.cells[y][x];
            let col = "#0b1a2b";
            if (v === 1) col = "#d4a72c";   // wall
            if (v === 2) col = "#9b4bff";   // moving wall
            if (v === 4) col = "#4cc9f0";   // visited
            if (v === 3) col = "#ffff33";   // path

            ctx.fillStyle = col;
            ctx.fillRect(x*cellSize, y*cellSize, cellSize, cellSize);

            ctx.strokeStyle = "#00000033";
            ctx.strokeRect(x*cellSize, y*cellSize, cellSize, cellSize);
        }
    }

    if (frame.start) {
        ctx.fillStyle = "lime";
        ctx.beginPath();
        ctx.arc((frame.start.x+0.5)*cellSize, (frame.start.y+0.5)*cellSize, cellSize*0.35, 0, Math.PI*2);
        ctx.fill();
    }

    if (frame.goal) {
        ctx.fillStyle = "red";
        ctx.beginPath();
        ctx.arc((frame.goal.x+0.5)*cellSize, (frame.goal.y+0.5)*cellSize, cellSize*0.35, 0, Math.PI*2);
        ctx.fill();
    }
}

document.getElementById("connect").onclick = () => {
    if (ws && ws.readyState === WebSocket.OPEN) return;
    ws = new WebSocket("ws://localhost:9002");
    ws.onopen = () => { statusEl.textContent = "Connected"; console.log("WS connected"); };
    ws.onmessage = (e) => {
        if (e.data === "END") {
            console.log("Algorithm finished");
            return;
        }
        try { drawFrame(JSON.parse(e.data)); }
        catch (err) { console.log("Bad frame", err); }
    };
    ws.onclose = () => { statusEl.textContent = "Disconnected"; console.log("WS closed"); };
};

document.getElementById("start").onclick = () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) {
        alert("Connect WebSocket first!");
        return;
    }
    const algo = document.getElementById("algo").value;
    const speed = document.getElementById("speed").value;
    const msg = { cmd: "start", algo: algo, speed: speed };
    ws.send(JSON.stringify(msg));
    console.log("Start sent:", msg);
};

document.getElementById("stop").onclick = () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    ws.send(JSON.stringify({ cmd: "stop" }));
    console.log("Stop sent");
};
