import asyncio
import websockets
import subprocess
import json
import os

CLIENTS = set()
process = None

EXE_PATH = "Q:/pathfinding_web_visualizer/src/pathfinding.exe"


async def client_handler(ws):
    print("Client connected")
    CLIENTS.add(ws)

    global process

    try:
        async for msg in ws:
            print("Received:", msg)

            try:
                data = json.loads(msg)
            except:
                print("Invalid JSON from client.")
                continue

            cmd = data.get("cmd")

            # ---------------------------
            # START COMMAND
            # ---------------------------
            if cmd == "start":
                algo = data.get("algo", "astar")
                speed = data.get("speed", "fast")

                algo_map = {"bfs": "1", "dijkstra": "2", "astar": "3"}
                speed_map = {"slow": "300", "medium": "120", "fast": "20"}

                algo_val = algo_map.get(algo, "3")
                speed_val = speed_map.get(speed, "20")

                print(f"Launching C++ pathfinding: algo={algo} speed={speed}")

                # kill previous
                if process:
                    try:
                        process.kill()
                    except:
                        pass

                process = subprocess.Popen(
                    [EXE_PATH, "--live", f"--algo={algo_val}", f"--speed={speed_val}"],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True
                )

                asyncio.create_task(stream_frames())

            # ---------------------------
            # STOP COMMAND
            # ---------------------------
            elif cmd == "stop":
                print("Stopping...")
                if process:
                    try:
                        process.kill()
                    except:
                        pass
                process = None

    except Exception as e:
        print("WebSocket error:", e)

    finally:
        CLIENTS.remove(ws)
        print("Client disconnected")


# ------------------------------------------------------------
# STREAM JSON FRAMES FROM C++ → FRONTEND
# ------------------------------------------------------------
async def stream_frames():
    global process
    if not process:
        return

    while True:
        line = process.stdout.readline()
        if not line:
            break

        line = line.strip()

        # JSON frame
        if line.startswith("{") and line.endswith("}"):
            remove = []
            for ws in CLIENTS:
                try:
                    await ws.send(line)
                except:
                    remove.append(ws)

            for ws in remove:
                CLIENTS.remove(ws)

        # END marker for final shortest path frame
        elif line == "END":
            for ws in CLIENTS:
                try:
                    await ws.send("END")
                except:
                    pass


async def main():
    print("WebSocket server running at ws://localhost:9002")
    async with websockets.serve(client_handler, "localhost", 9002, ping_interval=None, ping_timeout=None):
        await asyncio.Future()


if __name__ == "__main__":
    asyncio.run(main())
