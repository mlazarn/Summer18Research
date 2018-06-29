import csv
import os
import matplotlib.pyplot as plt
from matplotlib import colors
import matplotlib.animation as manimation
import numpy as np
import argparse as ap
import sys

def update(ttl, im, latt, title='t=0'):
    im.set_data(latt)
    ttl.set_text(title)

def new_frame(pfx, ttl, im, t):
    try:
        filename = pfx + str(t) + '.csv'
        lattice = np.genfromtxt(filename, dtype=int, delimiter=',')
        title = 't=' + str(t)
        update(ttl, im, lattice, title=title)
    except OSError:
        print("Could not open file \"{0}\"".format(filename))
        raise
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
parser.add_argument('path')
parser.add_argument('prefix')

parser.add_argument('start', type=int)
parser.add_argument('interval', type=int)
parser.add_argument('stop', type=int)

parser.add_argument('--output', '-o', default='movie.mp4')

parser.add_argument('--swap', '-w', type=int, default=-1)
parser.add_argument('--swap_interval', '-W', type=int, default=-1)

parser.add_argument('--framerate', '-f', type=int, default=15)
parser.add_argument('--author', '-a', default='user')
parser.add_argument('--comment', '-c')

parser.add_argument('--dpi', '-d', type=int, default=100)

args = parser.parse_args()

FFMpegWriter = manimation.writers['ffmpeg']
metadata = dict(title=args.output, artist=args.author, comment=args.comment)
writer = FFMpegWriter(fps=args.framerate, metadata=metadata)

cmap = colors.ListedColormap(['r', 'g', 'b', 'k'])
bounds = [0, 1, 2, 3, 4]
norm = colors.BoundaryNorm(bounds, cmap.N)

fig, ax = plt.subplots()
fig.set_tight_layout(True)

os.chdir(args.path)

lattice = np.genfromtxt(args.prefix + str(args.start) + '.csv', dtype=int, delimiter=',')

im = ax.imshow(lattice, interpolation='nearest', cmap=cmap, norm=norm)
ttl = ax.set_title('t=0', loc='left')

print("writing {}. Please wait.".format(args.output))
with writer.saving(fig, args.output, args.dpi):

    #print("writing frame 0")
    writer.grab_frame()
    
    if args.swap < 1 or args.swap > args.stop:
        for t in range(args.start + args.interval, args.stop + 1, args.interval):
            try:
                new_frame(args.prefix, ttl, im, t)
                #print("writing frame {0}".format(str(t)))
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
                new_frame(args.prefix, ttl, im, t)
                #print("writing frame {0}".format(str(t)))
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
                new_frame(args.prefix, ttl, im, t)
                #print("writing frame {0}".format(str(t)))
                writer.grab_frame()
                prog = (1.0 * (t / args.swap_interval)) / ((args.stop - args.start) / args.swap_interval)
                #update_progress(prog)
            except OSError:
                pass
            except:
                #print("Unexpected error:", sys.exc_info()[0])
                raise

print("done")
