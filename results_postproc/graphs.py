import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv", sep="; ")
n_particles = 100
for maze_name in df["maze"].unique():
    maze_df = df.loc[(df["maze"] == maze_name) & (df["n_particles"] == n_particles)]
    maze_size = maze_df["maze_size"].iloc[0]
    maze_df.plot(x="n_threads", y="execution_time", kind="scatter",
                 title=f"Maze {maze_size}, {n_particles=}",
                 xticks=np.concatenate((np.array([1]), np.arange(0, 20, 2))))
    plt.show()
