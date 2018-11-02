import os
import sys
import argparse as ap

import numpy as np
from scipy.fftpack import fft
from scipy.fftpack import fftfreq
from scipy import interpolate as spInt
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
    num_cols = spec_data.shape[-1]
    print(spec_data.shape)

    print(args.aspect)
    fig, ax = plt.subplots()
    con = ax.imshow(spec_data, cmap='inferno')
    #con = ax.imshow(spec_data[::-1,:], cmap='inferno')
    #, extent=[ -0.5, num_cols -0.5, 0, freq[-1] ])
    ax.set_aspect(spec_data.shape[1] / (1.0*spec_data.shape[0]))
    if len(args.vlines) > 0:
        ax.axvline(args.vlines)
        #ax.vlines(args.vlines, 0, (args.idx_1 - args.idx_0) - 1, zorder=3)

    ax.set_ylim(0, args.idx_1 - 1)
    cb = fig.colorbar(con, ax=ax)
    fig.set_tight_layout(True)

    ax.set_ylabel(r'$\omega$')
    ax.set_xlabel(r'$r$')

    if args.write_csv:
        np.savetxt(args.write_csv_dest, data, delimiter=",")

    fig.savefig(args.output, dpi=args.dpi)

def plotHWHM(args):
    data = combineData(args)
    freq = data[0,:]
    spec_data = data[1:, :]
    n = spec_data.shape[0]
    frequencies = np.arange(0, freq[-1] + (freq[-1]/freq.shape[-1]), freq[-1]/freq.shape[-1])
    half_widths = np.empty(n)
    for i in range(n):
        raw_ampl = spec_data[i, :]
        adjusted_ampl = raw_ampl - (np.max(raw_ampl) / 2)
        spline = spInt.InterpolatedUnivariateSpline(freq, adjusted_ampl)
        roots = spline.roots()
        FW = roots[-1] - roots[-2]
        half_widths[i] = FW / 2

    x = np.arange(n)
    fig, ax = plt.subplots()
    ax.plot(x, half_widths, linestyle='-', marker='s')

    ax.grid(which='both', zorder=1)
    if len(args.vlines) > 0:
        ax.axvline(args.vlines)
    fig.set_tight_layout(True)
    
    ax.set_xlabel(r'$\omega$')
    ax.set_ylabel(r'$|a(\omega)|$')
    
    if args.write_csv:
        output = np.array([x, half_widths])
        np.savetxt(args.write_csv_dest, output, delimiter=",")

    fig.savefig(args.output)
        

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
    if len(args.vlines) > 0:
        ax.axvline(args.vlines)
        #ax.vlines(args.vlines, 0, np.max(specData), zorder=3)
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
parser.add_argument('--write_csv_dest', '-d', default='output.csv')

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
elif args.mode == 'h':
    plotHWHM(args)


