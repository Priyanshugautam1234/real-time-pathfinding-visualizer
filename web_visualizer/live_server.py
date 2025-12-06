import asyncio
import websockets
import json
import os

CLIENTS = set()
current_task = None
current_process = None

EXE_PATH = "Q:/pathfinding_web_visualizer/src/pathfinding.exe"


async def client_handler(ws):
    print("Client connected", flush=True)
    CLIENTS.add(ws)

    global current_task, current_process

    try:
        async for msg in ws:
            print("Received:", msg, flush=True)

            try:
                data = json.loads(msg)
            except:
                print("Invalid JSON from client.", flush=True)
                continue

            cmd = data.get("cmd")

            # ---------------------------
            # START COMMAND
            # ---------------------------
            if cmd == "start":
                algo = data.get("algo", "astar")
                speed = data.get("speed", "fast")

                algo_map = {"bfs": "1", "dijkstra": "2", "astar": "3"}
                speed_map = {"slow": "180", "medium": "80", "fast": "20"}

                algo_val = algo_map.get(algo, "3")
                speed_val = speed_map.get(speed, "20")

                print(f"Launching C++ pathfinding: algo={algo} speed={speed}", flush=True)

                if not os.path.exists(EXE_PATH):
                    print(f"Error: Executable not found at {EXE_PATH}", flush=True)
                    return

                # cleanup previous
                if current_task:
                    current_task.cancel()
                    try:
                        await current_task
                    except asyncio.CancelledError:
                        pass
                    current_task = None

                if current_process:
                    try:
                        current_process.kill()
                        await current_process.wait()  # Ensure it is dead
                    except:
                        pass
                    current_process = None

                # Start Async Subprocess
                try:
                    process = await asyncio.create_subprocess_exec(
                        EXE_PATH, "--live", f"--algo={algo_val}", f"--speed={speed_val}",
                        stdout=asyncio.subprocess.PIPE,
                        stderr=None, # error to console
                    )
                    current_process = process
                    
                    # Start streaming task
                    current_task = asyncio.create_task(stream_frames(process))
                except Exception as e:
                    print(f"Failed to start process: {e}", flush=True)

            # ---------------------------
            # STOP COMMAND
            # ---------------------------
            elif cmd == "stop":
                print("Stopping...", flush=True)
                if current_task:
                    current_task.cancel()
                    current_task = None
                
                if current_process:
                    try:
                        current_process.kill()
                        await current_process.wait()
                    except:
                        pass
                    current_process = None

    except Exception as e:
        print("WebSocket error:", e, flush=True)

    finally:
        CLIENTS.remove(ws)
        print("Client disconnected", flush=True)


# ------------------------------------------------------------
# STREAM JSON FRAMES FROM C++ → FRONTEND
# ------------------------------------------------------------
async def stream_frames(process):
    print("Stream task started.", flush=True)
    try:
        while True:
            # Async Read - Allows other events to process!
            line = await process.stdout.readline()
            
            if not line:
                break

            line_str = line.decode('utf-8').strip()

            # JSON frame
            if line_str.startswith("{") and line_str.endswith("}"):
                remove = []
                # Copy set for safety
                for ws in list(CLIENTS):
                    try:
                        await ws.send(line_str)
                    except:
                        remove.append(ws)

                for ws in remove:
                    CLIENTS.remove(ws)

            # END marker
            elif line_str == "END":
                for ws in list(CLIENTS):
                    try:
                        await ws.send("END")
                    except:
                        pass

    except asyncio.CancelledError:
        print("Stream task cancelled.", flush=True)
        raise
    except Exception as e:
        print(f"Stream error: {e}", flush=True)
    finally:
        print("Stream task ended.", flush=True)


async def main():
    print("WebSocket server running at ws://localhost:9002", flush=True)
    async with websockets.serve(client_handler, "localhost", 9002, ping_interval=None, ping_timeout=None):
        await asyncio.Future()


if __name__ == "__main__":
    asyncio.run(main())
