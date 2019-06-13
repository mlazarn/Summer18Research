import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
from scipy.fftpack import fft
from scipy.fftpack import fftfreq
from scipy.stats import linregress as linregress
from scipy.signal import find_peaks as fpeaks
from scipy.signal import peak_widths as peak_widths
from scipy.fftpack import fftshift
from scipy import interpolate as spInt
import matplotlib.animation as manimation
from tqdm import tqdm
import sys

def open_data(filename):
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')

    return csv_data

def get_fwhm(data):
    freq = fftfreq(256)
    #freq = fftshift(fftfreq(256))
    # normalizing by 2.0 / 256
    specdat = np.abs(fft(data[0:256]) * (2.0 / 256))
    #specdat[0] = specdat[2]
    #specdat = fftshift(specdat)
    
    adjusted_dat = specdat - (np.max(specdat) / 2)
    argMax = np.argmax(specdat)
    spline = spInt.InterpolatedUnivariateSpline(freq, adjusted_dat)
    roots = spline.roots()
    try:
        for j in range(roots.shape[-1] - 1):
            HWL = roots[-2]
            HWR = roots[-1]
            if roots[j] < freq[argMax] and roots[j + 1] > freq[argMax]:
                HWL = roots[j]
                HWR = roots[j+1]
        FW = HWR - HWL
        return FW
    except IndexError:
        FW = np.nan
    #half_width = FW / 2
    #return half_width

def calculate_corr_len(args):
    print('initializing')

    os.chdir(args.dir)
    print(os.getcwd())

    filename = args.prefix + "{}/{}".format(0, args.target)
    data = open_data(filename)

    print('opening files')

    for i in range(1, args.runs):
        filename = args.prefix + "{}/{}".format(i, args.target)
        data = data + open_data(filename)
    data = data/(1.0 * args.runs)

    print('writing autocorrelation')

    np.savetxt(args.output, data, delimiter=',')

    c_len = np.zeros(data.shape[1])

    print('calculating correlation lengths')

    for i in range(0, data.shape[1]):
        c_len[i] = 1/get_fwhm(data[i])
    
    print('writing correlation lengths')

    np.savetxt(args.corr_len_output, c_len, delimiter=',')

    print('done!')

parser = ap.ArgumentParser()
parser.add_argument('dir')
parser.add_argument('prefix')
parser.add_argument('target')
parser.add_argument('output')
#parser.add_argument('y_unit', choices=['f', 'p'])
#parser.add_argument('x_unit', choices=['r', 't'])
#parser.add_argument('y_min', type=int)
#parser.add_argument('y_max', type=int)
parser.add_argument('runs', type=int)

parser.add_argument('--corr_len_output', '-c', default='corr_len.csv')

#parser.add_argument('--species', '-c', type=int, default=0)

#parser.add_argument('--lims', '-l', type=float, nargs=2)
#parser.add_argument('--vlines', '-v', type=int, nargs='+', default=[])

#parser.add_argument('--grid', '-g', action='store_true')

#parser.add_argument('--binned', '-b', action='store_true')

#parser.add_argument('--start', '-s', type=int)
#parser.add_argument('--interval', '-i', type=int)
#parser.add_argument('--stop', '-S', type=int)

#parser.add_argument('--swap', '-w', type=int, default=-1)
#parser.add_argument('--swap_interval', '-I', type=int)

#parser.add_argument('--framerate', '-f', type=int, default=15)
#parser.add_argument('--author', '-a', default='user')
#parser.add_argument('--comment', '-C')

#parser.add_argument('--dpi', type=int, default=100)

#args = parser.parse_args()

#plt.rc('text', usetex=True)
#plt.rc('font', family='serif')

#spec=['a', 'b', 'c']

args = parser.parse_args()

calculate_corr_len(args)
