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

def open_data(prefix, species, t, y_min, y_max):
    filename = prefix + str(t) + '.csv'
    csv_data = np.genfromtxt(filename, dtype=float, delimiter=',')[ : , y_min:y_max]

    if species == -1:
        output = np.sum(csv_data, axis=0)
    else:
        output = csv_data[species, : ]

    return output

def render_data(args):
    species_names = [r'a', r'b', r'c', r'/text{net}']
    marks = ['r.', 'g.', 'b.', 'k.']

    if args.unit == 'f':
        title = r'Flux at $t={0}$'
        axis_title = r'$\langle \Phi_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [-0.1, 0.1]
    elif args.unit == 'p':
        title = r'Density at $t={0}$'
        axis_title = r'$\langle \rho_{0} \rangle (r)$'.format(species_names[args.species])
        if args.lims is None:
            axis_range = [0.0, 1.0]
    elif args.unit == 'r':
        title = r'Reactions at $t={0}'
        axis_title = r'$n$'
        if args.lims is None:
            axis_range = [0, 64]

    if args.lims is not None:
        axis_range = args.lims

    print('initializing')

    os.chdir(args.target)
    print(os.getcwd())

    data = open_data(args.prefix, args.species, args.start, args.y_min, args.y_max)

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
    ax.set_xlabel(r'$r$')
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

            for t in range(args.start, args.stop + 1, args.interval):
                try:
                    if t > args.start:
                        data = open_data(args.prefix, args.species, t, args.y_min, args.y_max)
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
                        data = open_data(args.prefix, args.species, t, args.y_min, args.y_max)
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
            for t in range(args.swap, args.stop + 1, args.swap_interval):
                try:
                    data = open_data(args.prefix, args.species, t, args.y_min, args.y_max)
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
parser.add_argument('--vlines', '-v', type=int, nargs='+')

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
    print('calculating...')
    try:
        data = np.array([])
        if args.topology == 1:
            data = plot_densities_annular(args.target, args.interface_distance, max_radius, args.species)
        else:
            data = plot_densities(args.target, args.interface_distance, args.species)
    except OSError:
        quit()

    r = data[0]
    d = data[1]

    print('plotting')
    fig = plt.figure()
    l, = plt.plot(r, d, marks[args.species])

    plt.xlabel(r'$r$')
    plt.ylabel(r'$\langle {0} (r) \rangle$'.format(spec[args.species]))
    plt.title(r'Density')

    print('writing')
    
    target_dir = args.target.split('/')[:-1]
    if len(target_dir) > 0:
        path = ''
        for directory in target_dir[:-1]:
            path = path + directory + '/'
        path = path + target_dir[-1]
        os.chdir(path)

    plt.savefig(args.output + '.png')
    np.savetxt(args.output + '.csv', data, delimiter=",")

elif args.mode == 'a':
    render_data(args)
