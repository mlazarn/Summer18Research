import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
import matplotlib.animation as manimation
import sys

def density(species, lattice, w_0, w, r):
    w_1 = w_0 + w

    count = 0

    for i in range(w):
        if lattice[w_0 - r, w_0 + i] == species:
            count = count + 1
        if lattice[w_1 + r, w_0 + i] == species:
            count = count + 1
        if lattice[w_0 + i, w_0 - r] == species:
            count = count + 1
        if lattice[w_0 + i, w_1 + r] == species:
            count = count + 1

    density = count / (4.0 * w)

    return density

def plotDensities(filename, interfaceDistance, species):
    try:
        lattice = np.genfromtxt(filename, dtype=int, delimiter=',')
        #print(lattice)
    
        size = lattice.shape[0]
        w = size - (2 * interfaceDistance)

        outputData = np.zeros((2, interfaceDistance))

        for r in range(1, interfaceDistance):
            d = density(species, lattice, interfaceDistance, w, r)
            #print('[{}, {}]'.format(str(r), str(d)))
            outputData[0, r - 1] = r  
            outputData[1, r - 1] = density(species, lattice, interfaceDistance, w, r)
    except OSError:
        print("Could not open file \"{0}\"".format(filename))
        raise

    return outputData

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

#def update_progress(progress):
    #barLength = 50;
    #status = ""
    #if isinstance(progress, int):
        #progress = float(progress)
    #if not isinstance(progress, float):
        #progress = 0
        #status = "error: progress var must be float\r\n"
    #if progress < 0:
        #progress = 0
        #status = "Halt...\r\n"
    #if progress >= 1:
        #progress = 1
        #status = "Done...\r\n"
        #block = int(round(barLength*progress))
        #text = "\rPercent: [{0}] {1}% {2}".format( "#"*block + "-"*(barLength-block), progress*100, status)
        #sys.stdout.write(text)
        #sys.stdout.flush()

parser = ap.ArgumentParser()
parser.add_argument('mode', choices=['s', 'm'])
parser.add_argument('target')
parser.add_argument('interface_distance', type=int)
parser.add_argument('--dest_plot', '-d', default='density.png')
parser.add_argument('--dest_csv', '-D', default='density.csv')
parser.add_argument('--dest_movie', '-m', default='density.mp4')

parser.add_argument('--species', '-c', type=int, default=0)

parser.add_argument('--prefix', '-p')

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

marks = ['ro', 'gs', 'b^']
spec=['a', 'b', 'c']

if args.mode == 's':
    print('calculating...')
    try:
        data = plotDensities(args.target, args.interface_distance, args.species)
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

    plt.savefig(args.dest_plot)
    np.savetxt(args.dest_csv, data, delimiter=",")

elif args.mode == 'm':
    if args.swap_interval is None:
        setattr(args, 'swap_interval', args.interval)

    FFMpegWriter = manimation.writers['ffmpeg']
    metadata = dict(title=args.dest_movie, artist=args.author, comment=args.comment)
    writer = FFMpegWriter(fps=args.framerate, metadata=metadata)
    
    print('initializing')

    os.chdir(args.target)
    print(os.getcwd())

    filename = args.prefix + str(args.start) + '.csv'

    r, d = plotDensities(filename, args.interface_distance, args.species)
    #r = data[0]
    #d = data[1]

    fig, ax = plt.subplots()
    l, = ax.plot(r, d, marks[args.species])
    fig.set_tight_layout(True)
    
    ax.set_xlabel(r'$r$')
    ax.set_ylabel(r'$\langle {0} (r) \rangle$'.format(spec[args.species]))
    ax.set_ylim(0.0, 1.0)
    ttl = ax.set_title(r'Density at $t = 0$', loc='left')
    #ax.autoscale()

    print("Writing {}. Please wait".format(args.dest_movie))
    with writer.saving(fig, args.dest_movie, args.dpi):
        #print('writing frame 0')
        writer.grab_frame()

        if args.swap < 1 or args.swap >= args.stop:
            for t in range(args.start + args.interval, args.stop + 1, args.interval):
                try:
                    filename = args.prefix + str(t) + '.csv'
                    #print('calculating frame from {0}'.format(filename))
                    data = plotDensities(filename, args.interface_distance, args.species)
                    #print('setting data')
                    l.set_data(data)
                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
                    #print('drawing frame {0}'.format(str(t)))
                    writer.grab_frame()
                    prog = (1.0 * (t / args.interval)) / ((args.stop - args.start) / args.interval)
                    #update_progress(prog)
                except OSError:
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
        else:
            for t in range(args.start + args.interval, args.swap, args.interval):
                try:
                    filename = args.prefix + str(t) + '.csv'
                    #print('calculating frame {0}'.format(str(t)))
                    data = plotDensities(filename, args.interface_distance, args.species)
                    #print('setting data')
                    l.set_data(data)
                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
                    #print('drawing frame {0}'.format(str(t)))
                    writer.grab_frame()
                    prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
                    #update_progress(prog)
                except OSError:
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
            for t in range(args.swap, args.stop + 1, args.swap_interval):
                try:
                    filename = args.prefix + str(t) + '.csv'
                    #print('calculating frame {0}'.format(str(t)))
                    data = plotDensities(filename, args.interface_distance, args.species)
                    #print('setting data')
                    l.set_data(data)
                    ttl.set_text(r'Density at $t = {}$'.format(str(t)))
                    #print('drawing frame {0}'.format(str(t)))
                    writer.grab_frame()
                    prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
                    #update_progress(prog)
                except OSError:
                    pass
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
    print("Done")
