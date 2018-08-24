import os
import sys
import argparse as ap

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as manimation
from matplotlib import colors as colors

from tqdm import tqdm

def combineData(args):
    targ = args.prefix + '0'
    stacked = np.genfromtxt(targ + '/spectralData.csv', dtype=float, delimiter=',')[:,1:50]#[::-1,:]

    for run in range(1, args.runs):
        targ = args.prefix + str(run);
        new_array = np.genfromtxt(targ + '/spectralData.csv', dtype=float, delimiter=',')[:,1:50]#[::-1,:]
        stacked = np.dstack((stacked, new_array))

    spec_data = np.mean(stacked, axis=2)
    return spec_data

def plotSpectrograph(args):
    spec_data =  combineData(args).T

    fig, ax = plt.subplots()
    con = ax.imshow(spec_data, cmap='inferno')
    cb = fig.colorbar(con, ax=ax)
    ax.set_aspect(5)
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, 0, 49, zorder=3)

    ax.set_ylim(0, 48)
    fig.set_tight_layout(True)

    ax.set_ylabel(r'$\omega$')
    ax.set_xlabel(r'$r$')

    fig.savefig(args.output, dpi=args.dpi)

def plotFreqPlot(args):
    specData = combineData(args)[args.position, :]
    frequency = np.arange(1, 50) / args.steps

    fig, ax = plt.subplots()
    ax.plot(frequency, specData, linestyle='-', marker='s')

    ax.grid(which='both', zorder=1)
    fig.set_tight_layout(True)
    
    ax.set_xlabel(r'$\omega$')
    ax.set_ylabel(r'$|a(\omega)|$')
    
    fig.savefig(args.output)

parser = ap.ArgumentParser()
parser.add_argument('target')
parser.add_argument('prefix')
parser.add_argument('runs', type=int)
parser.add_argument('steps', type=int)
parser.add_argument('mode')
parser.add_argument('output')
parser.add_argument('--position', '-p', type=int)

parser.add_argument('--vlines', '-v', type=int, nargs='+', default=[])
parser.add_argument('--dpi', type=int, default=100)

args = parser.parse_args()

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

os.chdir(args.target)
print(os.getcwd())

if args.mode == 's':
    plotSpectrograph(args)
elif args.mode == 'f':
    plotFreqPlot(args)

