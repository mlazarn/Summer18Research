import os
import sys
import argparse as ap

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as manimation
from matplotlib import colors as colors

from tqdm import tqdm

def plotSpectrograph(args):
    targ = args.prefix + '0'
    stacked = np.genfromtxt(targ + "/" +  args.filename, dtype=float, delimiter=',')[:,1:args.height].T#[::-1,:]

    for run in range(1, args.runs):
        targ = args.prefix + str(run);
        new_array = np.genfromtxt(targ + "/" + args.filename, dtype=float, delimiter=',')[:,1:args.height].T#[::-1,:]
        stacked = np.dstack((stacked, new_array))

    spec_data = np.mean(stacked, axis=2)

    fig, ax = plt.subplots()
    con = ax.imshow(spec_data, cmap='inferno')
    ax.set_aspect(5)
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, 0, args.height - 1, zorder=3)

    ax.set_ylim(0, args.height - 2)
    cb = fig.colorbar(con, ax=ax)
    fig.set_tight_layout(True)

    ax.set_ylabel(r'$\omega$')
    ax.set_xlabel(r'$r$')

    fig.savefig(args.output, dpi=args.dpi)

parser = ap.ArgumentParser()
parser.add_argument('target')
parser.add_argument('filename')
parser.add_argument('output')
parser.add_argument('prefix')
parser.add_argument('height', type=int)
parser.add_argument('runs', type=int)

parser.add_argument('--vlines', '-v', type=int, nargs='+', default=[])
parser.add_argument('--dpi', type=int, default=100)

args = parser.parse_args()

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

os.chdir(args.target)
print(os.getcwd())

plotSpectrograph(args)

