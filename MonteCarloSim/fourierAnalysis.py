import os
import sys
import argparse as ap

import numpy as np
from scipy.fftpack import fft
from scipy.fftpack import fftfreq
import matplotlib.pyplot as plt
import matplotlib.animation as manimation
from matplotlib import colors as colors

from tqdm import tqdm

def fourierTransform(array):
    specData = sp.fftpack.fft(array, n=length)
    return specData

def combineData(args):
    targ = args.prefix + '0'
    arr = np.genfromtxt(targ + "/" +  args.filename, dtype=float, delimiter=',')
    stacked = fft(arr , n=(arr.shape[-1] + args.pad))[:,args.idx_0:args.idx_1]

    for run in range(1, args.runs):
        targ = args.prefix + str(run);
        arr = np.genfromtxt(targ + "/" +  args.filename, dtype=float, delimiter=',')

        mean = np.mean(arr, axis=1,keepdims=True)
        offset = np.zeros(mean.shape)
        if args.offset:
            offset = mean
        new_array = fft(arr - offset, n=(arr.shape[-1] + args.pad))[:,args.idx_0:args.idx_1]
        stacked = np.dstack((stacked, new_array))

    if args.abs:
        spec_data = np.mean(np.abs(stacked), axis=2)
    else:
        spec_data = np.mean(np.real(stacked), axis=2)
    freq = fftfreq(arr.shape[-1] + args.pad)[args.idx_0:args.idx_1]
    output = np.vstack((freq, spec_data))
    return output

def plotSpectrograph(args):
    data =  combineData(args)
    freq = data[0,:]
    spec_data = data[1:, :].T

    fig, ax = plt.subplots()
    con = ax.imshow(spec_data, cmap='inferno')
    ax.set_aspect(args.aspect)
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, 0, (args.idx_1 - args.idx_0) - 1, zorder=3)

    ax.set_ylim(0, args.args.idx_0 - 2)
    cb = fig.colorbar(con, ax=ax)
    fig.set_tight_layout(True)

    ax.set_ylabel(r'$\omega$')
    ax.set_xlabel(r'$r$')

    fig.savefig(args.output, dpi=args.dpi)

def plotFreqPlot(args):
    data = combineData(args)
    frequency = data[0,:]
    specData = data[args.position + 1, :]
    print(str(specData.shape))
    #frequency = fftfreq(args.idx_1 - args.idx_0)
    #frequency = np.arange(0, args.idx_1) 

    fig, ax = plt.subplots()
    ax.plot(frequency, specData, linestyle='-', marker='s')

    ax.grid(which='both', zorder=1)
    fig.set_tight_layout(True)
    
    ax.set_xlabel(r'$\omega$')
    ax.set_ylabel(r'$|a(\omega)|$')
    
    if args.write_csv:
        output = np.array([frequency, specData])
        np.savetxt(args.write_csv_dest, output, delimiter=",")

    fig.savefig(args.output)

parser = ap.ArgumentParser()
parser.add_argument('target')
parser.add_argument('filename')
parser.add_argument('output')
parser.add_argument('prefix')
parser.add_argument('idx_1', type=int)
parser.add_argument('runs', type=int)
parser.add_argument('mode')
parser.add_argument('pad', type=int)
parser.add_argument('--idx_0', type=int, default=0)
parser.add_argument('--aspect', '-a', type=float, default=1.0)
parser.add_argument('--position', '-p', type=int)
parser.add_argument('--abs', '-A', action='store_true')
parser.add_argument('--offset', '-o', action='store_true')
parser.add_argument('--write_csv', '-w', action='store_true')
parser.add_argument('--write_csv_dest', '-d', default='spectralData.csv')

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


