// Simple canvas visualizer for grid + path JSON
let canvas = document.getElementById('c');
let ctx = canvas.getContext('2d');
let cellSizeSelect = document.getElementById('cellSize');
let fileInput = document.getElementById('file');
let drawBtn = document.getElementById('drawBtn');

let data = null;

fileInput.addEventListener('change', (e) => {
  const f = e.target.files[0];
  if (!f) return;
  const reader = new FileReader();
  reader.onload = () => {
    try {
      data = JSON.parse(reader.result);
      console.log('Loaded', data);
      draw();
    } catch (err) {
      alert('Invalid JSON file');
    }
  };
  reader.readAsText(f);
});

drawBtn.addEventListener('click', draw);

function draw() {
  if (!data) { alert('Please load a JSON file exported by the C++ binary (--export-json)'); return; }
  const cell = parseInt(cellSizeSelect.value || 14, 10);
  const w = data.width, h = data.height;
  canvas.width = w * cell;
  canvas.height = h * cell;
  // background
  ctx.fillStyle = '#061025';
  ctx.fillRect(0,0,canvas.width, canvas.height);

  // draw grid cells
  for (let y=0;y<h;y++) {
    for (let x=0;x<w;x++) {
      const v = data.grid[y][x];
      if (v === 1) {
        ctx.fillStyle = '#a67c00'; // wall
        ctx.fillRect(x*cell, y*cell, cell-1, cell-1);
      } else {
        // empty cell subtle dot
        ctx.fillStyle = '#07233a';
        ctx.fillRect(x*cell, y*cell, cell-1, cell-1);
      }
    }
  }

  // draw path if present
  if (data.path && data.path.length>0) {
    ctx.fillStyle = 'rgba(255,215,0,0.95)';
    for (const p of data.path) {
      ctx.fillRect(p.x*cell, p.y*cell, cell-1, cell-1);
    }
  }

  // draw start & goal
  if (data.start) {
    ctx.fillStyle = 'green';
    ctx.beginPath();
    ctx.arc((data.start.x + 0.5)*cell, (data.start.y+0.5)*cell, Math.max(3, cell/3), 0, Math.PI*2);
    ctx.fill();
  }
  if (data.goal) {
    ctx.fillStyle = 'red';
    ctx.beginPath();
    ctx.arc((data.goal.x + 0.5)*cell, (data.goal.y+0.5)*cell, Math.max(3, cell/3), 0, Math.PI*2);
    ctx.fill();
  }
}
