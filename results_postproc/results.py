import re
import os

output_filename = "results.csv"
execution_times = {}

for filename in os.listdir("../results"):
    f = os.path.join(filename)
    filename_match = re.search("(.+)_threads_(\d+)_p.txt", filename)
    if os.path.isfile(f) and filename_match:
        n_threads = filename_match.groups()[0]
        n_particles = filename_match.groups()[1]
        with open(filename, "r") as f:
            lines = f.readlines()
        maze_idx = None
        maze_size = None
        for line in lines:
            match = re.search("^Maze (\d+), (\d+x\d+)$", line)
            if match:
                maze_idx = match.groups()[0]
                maze_size = match.groups()[1]
                continue
            match = re.search("^Maze (\d+) average run time:\s*(\d+\.?\d*)$", line)
            if match:
                assert maze_idx == match.groups()[0]
                maze_run_time = match.groups()[1]
                execution_times[(maze_idx, maze_size, n_threads, n_particles)] = maze_run_time

with open("results.csv", "w") as f:
    f.seek(0)
    f.write("maze; maze_size; n_threads; n_particles; execution_time\n")
    for (maze_idx, maze_size, n_threads, n_particles), time in execution_times.items():
        f.write(f"{maze_idx}; {maze_size}; {n_threads}; {n_particles}; {time}\n")

