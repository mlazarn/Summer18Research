import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
import matplotlib.animation as manimation
from tqdm import tqdm
import sys

def open_densities(filename, y_min, y_max):
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')[y_min:y_max]

    return csv_data

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

data_0 = open_densities("data/densityTest2/test_0/densities.csv", 0, 3000)
label_0 = r'$\epsilon = 0.1$'
data_1 = open_densities("data/densityTest2/test_1/densities.csv", 0, 3000)
label_1 = r'$\epsilon = 1.25$'
data_2 = open_densities("data/densityTest2/test_2/densities.csv", 0, 3000)
label_2 = r'$\epsilon = 2.5$'
data_3 = open_densities("data/densityTest2/test_3/densities.csv", 0, 900)
label_3 = r'$\epsilon = 10.0$'

fig, ax = plt.subplots()
ax.plot(data_0, 'c-', label=label_0)
ax.plot(data_1, 'm--', label=label_1)
ax.plot(data_2, 'y-.', label=label_2)
ax.plot(data_3, color='forestgreen', linestyle=':', label=label_3)
ax.legend()

ax.grid(which='both', zorder=1)
fig.set_tight_layout(True)

ttl = ax.set_title("Density")
ax.set_xlabel(r'$t$')
ax.set_ylabel(r'$ \rho (t)$')

fig.savefig("densities.png", dpi=200)
