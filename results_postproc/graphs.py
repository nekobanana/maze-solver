import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
mpl.use("TkAgg")


def graphs_fixed_n_particles(df, n_particles):
    for maze_name in df["maze"].unique():
        maze_df = df.loc[(df["maze"] == maze_name) & ((df["n_particles"] == n_particles) | (df["n_threads"] == 1))]
        maze_size = maze_df["maze_size"].iloc[0]
        maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                     title=f"Maze {maze_size}, {n_particles=}",
                     xticks=np.concatenate((np.array([1]), np.arange(0, 22, 2))))
        plt.show()

def graphs_linear_n_particles(df):
    for maze_name in df["maze"].unique():
        maze_df = df.loc[(df["maze"] == maze_name) & (df["n_particles"] == df["n_threads"])]
        maze_size = maze_df["maze_size"].iloc[0]
        maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                     title=f"Maze {maze_size}, n_particles=n_threads",
                     xticks=np.concatenate((np.array([1]), np.arange(0, 22, 2))))
        plt.show()

if __name__ == "__main__":
    df = pd.read_csv("results.csv", sep="; ")
    graphs_fixed_n_particles(df, n_particles=100)
    # graphs_linear_n_particles(df)
