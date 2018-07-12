import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
import matplotlib.animation as manimation
from tqdm import tqdm
import sys

#def density(species, lattice, w_0, w, r, topology=0):
#    w_1 = w_0 + w
#
#    count = 0
#
#    for i in range(w):
#        if lattice[w_0 - r, w_0 + i] == species:
#            count = count + 1
#        if lattice[w_1 + r, w_0 + i] == species:
#            count = count + 1
#        if lattice[w_0 + i, w_0 - r] == species:
#            count = count + 1
#        if lattice[w_0 + i, w_1 + r] == species:
#            count = count + 1
#
#    density = count / (4.0 * w)
#
#    return density
#
#def density_annular(species, lattice, lattice_width, y):
#    count = 0
#
#    for x in range(lattice_width):
#        if lattice[x, y] == species : count = count + 1
#
#    density = count / (1.0 * lattice_width)
#
#    return density
#
#def plot_densities(filename, interfaceDistance, species):
#    try:
#        lattice = np.genfromtxt(filename, dtype=int, delimiter=',')
#        #print(lattice)
#    
#        size = lattice.shape[0]
#        w = size - (2 * interfaceDistance)
#
#        outputData = np.zeros((2, interfaceDistance))
#
#        for r in range(interfaceDistance):
#            d = density(species, lattice, interfaceDistance, w, r + 1)
#
#            #print('[{}, {}]'.format(str(r), str(d)))
#            outputData[0, r - 1] = r  
#            outputData[1, r - 1] = d
#    except OSError:
#        print("Could not open file \"{0}\"".format(filename))
#        raise
#
#    return outputData
#
#def plot_densities_annular(filename, interface_distance, max_radius, species):
#    try:
#        lattice = np.genfromtxt(filename, dtype=int, delimiter=',')
#    
#        lattice_width = lattice.shape[0]
#
#        outputData = np.zeros((2, max_radius))
#
#        for r in range(max_radius):
#            y = interface_distance + r
#            d = density_annular(species, lattice, lattice_width, y)
#            #print('[{}, {}]'.format(str(r), str(d)))
#            outputData[0, r - 1] = r + 1 
#            outputData[1, r - 1] = d
#    except OSError:
#        print("Could not open file \"{0}\"".format(filename))
#        raise
#
#    return outputData
#
#def plot_current(filename, species):
#    try:
#        if species == -1:
#            array = np.genfromtxt(filename, dtype=int, delimiter=',')[0]
#            current = array[0] + array[1] + array[2]
#        else:
#            current = np.genfromtxt(filename, dtype=int, delimiter=',')[species]
#        return current
#    except OSError:
#        print("Could not open file \"{0}\"".format(filename))
#        raise
    
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

#def update_arrays(prefix, species, t, y_min, y_max):

    #data = open_data(filename, species, y_min=y_min, y_max=y_max)

    #return data

def render_data(args):
    species_names = [r'a', r'b', r'c', r'{net}']
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

    if args.lims is not None:
        axis_range = args.lims

    print('initializing')

    os.chdir(args.target)
    print(os.getcwd())

    data = open_data(args.prefix, args.species, args.start, args.y_min, args.y_max)
    r = np.arange(args.y_min, args.y_max)

    fig, ax = plt.subplots()
    l, = ax.plot(r, data, marks[args.species], zorder=1)
    if len(args.vlines) > 0:
        ax.vlines(args.vlines, axis_range[0], axis_range[1], zorder=2)
    fig.set_tight_layout(True)

    ttl = ax.set_title(title.format(str(args.start)), loc='left')

    ax.set_xlim(r[0], r[-1])
    ax.set_xlabel(r'$r$')
    ax.set_ylim(axis_range)
    ax.set_ylabel(axis_title)

    FFMpegWriter = manimation.writers['ffmpeg']
    metadata = dict(title=args.output, artist=args.author, comment=args.comment)
    writer = FFMpegWriter(fps=args.framerate, metadata=metadata)
    
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
parser.add_argument('unit', choices=['f', 'p'])
parser.add_argument('y_min', type=int)
parser.add_argument('y_max', type=int)

parser.add_argument('--species', '-c', type=int, default=0)

parser.add_argument('--prefix', '-p')

parser.add_argument('--lims', '-l', type=float, nargs=2)
parser.add_argument('--vlines', '-v', type=int, nargs='+')

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

#max_radius = args.max_radius
#if args.topology == 1 and args.max_radius == -1 and args.mode != 'cm':
    #max_radius = args.interface_distance

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

#    data = np.array([])
#    if args.topology == 1:
#        data = plot_densities_annular(filename, args.interface_distance, max_radius, args.species)
#    else:
#        data = plot_densities(filename, args.interface_distance, args.species)
#
#    #r, d = plot_densities(filename, args.interface_distance, args.species)
#    r = data[0]
#    d = data[1]
#
#    fig, ax = plt.subplots()
#    l, = ax.plot(r, d, marks[args.species])
#    fig.set_tight_layout(True)
#    
#    ax.set_xlabel(r'$r$')
#    ax.set_ylabel(r'$\langle {0} (r) \rangle$'.format(spec[args.species]))
#    ax.set_ylim(0.0, 1.0)
#    #ax.autoscale()
#
#    print("Writing {}. Please wait".format(args.dest_movie))
#    with writer.saving(fig, args.dest_movie, args.dpi):
#        #print('writing frame 0')
#        writer.grab_frame()
#
#        if args.swap < 1 or args.swap >= args.stop:
#
#            frames = int( (args.stop - args.start) / args.interval )
#            pbar = tqdm(total=frames)
#
#            for t in range(args.start + args.interval, args.stop + 1, args.interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame from {0}'.format(filename))
#                    if args.topology == 1:
#                        data = plot_densities_annular(filename, args.interface_distance, max_radius, args.species)
#                    else:
#                        data = plot_densities(filename, args.interface_distance, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.interval)) / ((args.stop - args.start) / args.interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#
#            pbar.close()
#        else:
#            if args.swap_interval is None:
#                setattr(args, 'swap_interval', args.interval)
#
#            frames = int( ((args.swap - args.start) / args.interval) + ((args.stop - args.swap) / args.swap_interval))
#            pbar = tqdm(total=frames)
#
#            for t in range(args.start + args.interval, args.swap, args.interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame {0}'.format(str(t)))
#                    data = np.array([])
#                    if args.topology == 1:
#                        data = plot_densities_annular(filename, args.interface_distance, max_radius, args.species)
#                    else:
#                        data = plot_densities(filename, args.interface_distance, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#            for t in range(args.swap, args.stop + 1, args.swap_interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame {0}'.format(str(t)))
#
#                    data = np.array([])
#                    if args.topology == 1:
#                        data = plot_densities_annular(filename, args.interface_distance, max_radius, args.species)
#                    else:
#                        data = plot_densities(filename, args.interface_distance, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#            pbar.close()
#    print("Done")
#elif args.mode == 'cm':
#    FFMpegWriter = manimation.writers['ffmpeg']
#    metadata = dict(title=args.dest_movie, artist=args.author, comment=args.comment)
#    writer = FFMpegWriter(fps=args.framerate, metadata=metadata)
#    
#    print('initializing')
#
#    os.chdir(args.target)
#    print(os.getcwd())
#
#    filename = args.prefix + str(args.start) + '.csv'
#
#    data = plot_current(filename, args.species)
#
#    #r, d = plot_densities(filename, args.interface_distance, args.species)
#    r = data[0]
#    d = data[1]
#
#    fig, ax = plt.subplots()
#    l, = ax.plot(r, d, marks[args.species])
#    fig.set_tight_layout(True)
#    
#    ax.set_xlabel(r'$x$')
#    ax.set_ylabel(r'$\J_{0} (x) $'.format(spec[args.species]))
#    ax.set_ylim(-args.max_radius, args.max_radius )
#    ttl = ax.set_title(r'Current at $t = 0$', loc='left')
#    #ax.autoscale()
#
#    print("Writing {}. Please wait".format(args.dest_movie))
#    with writer.saving(fig, args.dest_movie, args.dpi):
#        #print('writing frame 0')
#        writer.grab_frame()
#
#        if args.swap < 1 or args.swap >= args.stop:
#
#            frames = int( (args.stop - args.start) / args.interval )
#            pbar = tqdm(total=frames)
#
#            for t in range(args.start + args.interval, args.stop + 1, args.interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame from {0}'.format(filename))
#                    data = plot_current(filename, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Current at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.interval)) / ((args.stop - args.start) / args.interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#
#            pbar.close()
#        else:
#            if args.swap_interval is None:
#                setattr(args, 'swap_interval', args.interval)
#
#            frames = int( ((args.swap - args.start) / args.interval) + ((args.stop - args.swap) / args.swap_interval))
#            pbar = tqdm(total=frames)
#
#            for t in range(args.start + args.interval, args.swap, args.interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame {0}'.format(str(t)))
#                    data = np.array([])
#                    data = plot_current(filename, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Current at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#            for t in range(args.swap, args.stop + 1, args.swap_interval):
#                try:
#                    filename = args.prefix + str(t) + '.csv'
#                    #print('calculating frame {0}'.format(str(t)))
#
#                    data = np.array([])
#                    data = plot_current(filename, args.species)
#                    #print('setting data')
#                    l.set_data(data)
#                    ttl.set_text(r'Current at $t = {}$'.format(str(t)))
#                    #print('drawing frame {0}'.format(str(t)))
#                    writer.grab_frame()
#                    pbar.update()
#                    #prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
#                    #update_progress(prog)
#                except OSError:
#                    pbar.update()
#                    pass
#                except:
#                    print("Unexpected error:", sys.exc_info()[0])
#                    raise
#            pbar.close()
#    print("Done")
