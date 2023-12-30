import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
# mpl.use("TkAgg")


def graphs_fixed_n_particles(df, n_particles):
    for maze_name in df["maze"].unique():
        maze_df = (df.loc[(df["maze"] == maze_name) & ((df["n_particles"] == n_particles) | (df["n_threads"] == 1))]
                   .sort_values("n_threads"))
        maze_size = maze_df["maze_size"].iloc[0]
        maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                     title=f"Maze {maze_size}, {n_particles=}",
                     xticks=np.concatenate((np.array([1]), np.arange(0, 32, 2))))
        plt.savefig(f"plots/maze_{maze_size}_{n_particles=}.png")

def graphs_linear_n_particles(df):
    for maze_name in df["maze"].unique():
        maze_df = (df.loc[(df["maze"] == maze_name) & (df["n_particles"] == df["n_threads"])]
                   .sort_values("n_threads"))
        maze_size = maze_df["maze_size"].iloc[0]
        maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                     title=f"Maze {maze_size}, n_particles=n_threads",
                     xticks=np.concatenate((np.array([1]), np.arange(0, 32, 2))))
        plt.savefig(f"plots/maze_{maze_size}_n_particles=n_threads.png")


def graphs_all_particles_and_speedup(df):
    maze_avg = df.groupby(by=["maze", "maze_size", "n_threads"])["execution_time"].sum().reset_index()
    for maze_name in maze_avg["maze"].unique():
        maze_df = maze_avg.loc[(maze_avg["maze"] == maze_name)].sort_values("n_threads")
        maze_size = maze_df["maze_size"].iloc[0]
        plt.figure()
        maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                     title=f"Maze {maze_size}",
                     xticks=np.concatenate((np.array([1]), np.arange(0, 32, 2))))
        plt.savefig(f"plots/maze_{maze_size}.png")
        plt.close()

        sequential_exec_time = (maze_df.loc[(maze_df["n_threads"] == 1)])["execution_time"].iloc[0]
        fig = plt.figure()
        ax = fig.add_subplot(111)
        speedup = sequential_exec_time / maze_df["execution_time"]
        plt.scatter(maze_df["n_threads"], speedup, color="orange")
        plt.title(f"Speedup maze {maze_size}")
        plt.xticks(np.concatenate((np.array([1]), np.arange(0, 32, 2))))
        for t, s in zip(maze_df["n_threads"], speedup):
            ax.annotate(f"  {s:.2}", xy=(t, s), xytext=(t, s), xycoords='data')
        plt.savefig(f"plots/speedup_maze_{maze_size}.png")
        plt.close()

if __name__ == "__main__":
    df = pd.read_csv("results.csv", sep="; ")
    # graphs_fixed_n_particles(df, n_particles=100)
    # graphs_linear_n_particles(df)
    graphs_all_particles_and_speedup(df)