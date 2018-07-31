import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
import matplotlib.animation as manimation
from tqdm import tqdm
import sys

def open_densities(filename, y_min=0, y_max=-1):
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')
    return csv_data

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

data = open_densities('data/carryingCapacityTest/densities.csv')
means = np.mean(data, axis=1)

mobilities = open_densities('data/carryingCapacityTest/mobilities.csv')

fig, ax = plt.subplots()
ax.plot(mobilities, means, color='forestgreen', linestyle='-', marker='s')

ax.set_ylim(0, 1)

ax.grid(which='both', zorder=1)
fig.set_tight_layout(True)

ax.set_xlabel(r'$\epsilon_m$')
ax.set_ylabel(r'$\widetilde{\rho}$')

fig.savefig("carryingCap2.png")
