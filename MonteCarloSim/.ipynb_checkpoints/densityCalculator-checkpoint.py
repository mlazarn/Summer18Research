import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
import matplotlib.animation as manimation
from tqdm import tqdm
import sys

def movie_args_check(args):
    suffx = 'is a required argument when running in movie mode'

    if not os.path.isdir(args.target):
        raise ValueError('--target must point to a valid directory in movie mode')

    if args.dest_movie[-4:] != '.mp4':
        raise ValueError('--dest_movie must end in .mp4')

    if args.prefix is None:
        raise NameError('--prefix ' + suffx)

    if args.start is None:
        raise NameError('--start ' + suffx)
    elif args.start < 0:
        raise ValueError('--start must be a non-negative number')

    if args.interval is None:
        raise NameError('--interval ' + suffx)
    elif args.interval < 1:
        raise ValueError('--interval must be a positive number')

    if args.stop is None:
        raise NameError('--stop ' + suffx)
    elif args.stop <= args.start:
        raise ValueError('--stop must be larger than --start')

    if args.interface_distance < 1:
        raise ValueError('--interval_distance must be a positive number')

def plot_args_check(args):
    if (not os.path.isfile(args.target)) or args.target[-4:] != '.csv':
        raise ValueError('--target must point to a valid .csv file in single mode')

    if args.dest_plot[-4:] != '.png':
        raise ValueError('--dest_plot must end in .png')
    if args.dest_csv[-4:] != '.csv':
        raise ValueError('--dest_csv must end in .csv')

def open_data(filename, species, y_min, y_max):
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')[ : , y_min:y_max]

    if species == -1:
        output = np.sum(csv_data, axis=0)
    else:
        output = csv_data[species, : ]
    
    return output

def open_densities(filename, y_min, y_max):
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')[y_min:y_max]
    
    return csv_data

def render_data(args):
    species_names = [r'a', r'b', r'c', r'\mathrm{net}']
    marks = ['r.', 'g.', 'b.', 'k.']

    if args.y_unit == 'f':
        title = r'Flux at $t={0}$'
        axis_title = r'$\langle \Phi_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [-0.1, 0.1]
    elif args.y_unit == 'p':
        title = r'Density at $t={0}$'
        axis_title = r'$\langle \rho_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [0.0, 1.0]

    x_ttl = 'a'
    
    if args.x_unit == 'r':
        x_ttl = r'$r$'
    elif args.x_unit == 't':
        x_ttl = r'$t$'

    if args.lims is not None:
        axis_range = args.lims

    print('initializing')

    os.chdir(args.target)
    print(os.getcwd())

    filename = args.prefix + str(args.start) + ".csv"

    data = open_data(filename, args.species, args.y_min, args.y_max)

    if args.binned:
        r = np.genfromtxt("bin_midpoints.csv", dtype=float, delimiter=',')[args.y_min:args.y_max]
    else:
        r = np.arange(args.y_min, args.y_max)

    fig, ax = plt.subplots()
    l, = ax.plot(r, data, marks[args.species], zorder=2)
    if args.binned:
        l.set_linestyle('--')
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, axis_range[0], axis_range[1], zorder=3)
    if args.grid:
        ax.grid(which='both', zorder=1)
    fig.set_tight_layout(True)

    ttl = ax.set_title(title.format(str(args.start)), loc='left')

    ax.set_xlim(r[0], r[-1])
    ax.set_xlabel(x_ttl)
    ax.set_ylim(axis_range)
    ax.set_ylabel(axis_title)

    FFMpegWriter = manimation.writers['ffmpeg']
    metadata = dict(title=args.output, artist=args.author, comment=args.comment)
    writer = FFMpegWriter(fps=args.framerate, metadata=metadata)

    print('writing {}'.format(args.output))

    with writer.saving(fig, args.output, args.dpi):
        if args.swap < 1 or args.swap >= args.stop:
            frames = int( (args.stop - args.start) / args.interval )
            pbar = tqdm(total=frames)
            for t in range(args.start, args.stop, args.interval):
                try:
                    if t > args.start:
                        filename = args.prefix + str(t) + ".csv"
                        data = open_data(filename, args.species, args.y_min, args.y_max)
                        l.set_ydata(data)
                        ttl.set_text(title.format(str(t)))

                    writer.grab_frame()
                    pbar.update()
                except OSError:
                    pbar.update()
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
            pbar.close()
        else:
            if args.swap_interval is None:
                setattr(args, 'swap_interval', args.interval)

            frames = int( ((args.swap - args.start) / args.interval) + ((args.stop - args.swap) / args.swap_interval))
            pbar = tqdm(total=frames)

            for t in range(args.start, args.swap, args.interval):
                try:
                    if t > args.start:
                        filename = args.prefix + str(t) + ".csv"
                        data = open_data(filename, args.species, args.y_min, args.y_max)
                        l.set_ydata(data)
                        ttl.set_text(title.format(str(t)))
                    writer.grab_frame()
                    pbar.update()
                except OSError:
                    pbar.update()
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
            for t in range(args.swap, args.stop, args.swap_interval):
                print("?")
                try:
                    filename = args.prefix + str(t) + ".csv"
                    data = open_data(filename, args.species, args.y_min, args.y_max)
                    l.set_ydata(data)
                    ttl.set_text(title.format(str(t)))
                    writer.grab_frame()
                    pbar.update()
                except OSError:
                    pbar.update()
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
            pbar.close()
    print("Done")

def render_single_frame(args):
    species_names = [r'a', r'b', r'c', r'\mathrm{net}']
    marks = ['r.', 'g.', 'b.', 'k.']

    if args.y_unit == 'f':
        title = r'Flux'
        axis_title = r'$\langle \Phi_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [-0.1, 0.1]
    elif args.y_unit == 'p':
        title = r'Density'
        axis_title = r'$\langle \rho_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [0.0, 1.0]

    if args.x_unit == 'r':
        x_axis_title = r'$r$'
    elif args.x_unit == 't':
        x_axis_title = r'$t$'

    if args.lims is not None:
        axis_range = args.lims

    print('initializing')

    data = open_densities(args.target, args.y_min, args.y_max)

    if args.binned:
        r = np.genfromtxt("bin_midpoints.csv", dtype=float, delimiter=',')[args.y_min:args.y_max]
    else:
        r = np.arange(args.y_min, args.y_max)

    fig, ax = plt.subplots()
    l, = ax.plot(r, data, marks[args.species], zorder=2)
    if args.binned:
        l.set_linestyle('--')
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, axis_range[0], axis_range[1], zorder=3)
    if args.grid:
        ax.grid(which='both', zorder=1)
    fig.set_tight_layout(True)

    ttl = ax.set_title(title, loc='left')

    ax.set_xlim(r[0], r[-1])
    ax.set_xlabel(x_axis_title)
    ax.set_ylim(axis_range)
    ax.set_ylabel(axis_title)

    fig.savefig(args.output, dpi=args.dpi)

parser = ap.ArgumentParser()
parser.add_argument('mode', choices=['s', 'a'])
parser.add_argument('target')
parser.add_argument('output')
parser.add_argument('y_unit', choices=['f', 'p'])
parser.add_argument('x_unit', choices=['r', 't'])
parser.add_argument('y_min', type=int)
parser.add_argument('y_max', type=int)

parser.add_argument('--species', '-c', type=int, default=0)

parser.add_argument('--prefix', '-p')

parser.add_argument('--lims', '-l', type=float, nargs=2)
parser.add_argument('--vlines', '-v', type=int, nargs='+', default=[])

parser.add_argument('--grid', '-g', action='store_true')

parser.add_argument('--binned', '-b', action='store_true')

parser.add_argument('--start', '-s', type=int)
parser.add_argument('--interval', '-i', type=int)
parser.add_argument('--stop', '-S', type=int)

parser.add_argument('--swap', '-w', type=int, default=-1)
parser.add_argument('--swap_interval', '-I', type=int)

parser.add_argument('--framerate', '-f', type=int, default=15)
parser.add_argument('--author', '-a', default='user')
parser.add_argument('--comment', '-C')

parser.add_argument('--dpi', type=int, default=100)

args = parser.parse_args()

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

spec=['a', 'b', 'c']

if args.mode == 's':
    render_single_frame(args)
elif args.mode == 'a':
    render_data(args)
