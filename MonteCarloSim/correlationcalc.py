import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import argparse as ap
from tqdm import tqdm

def open_data(filename, max_r):
    csv_data = np.genfromtxt(filename, dtype=int, delimiter=',')
    return csv_data[:,:max_r]

def calc_densities(lattice):
    x_size, y_size = lattice.shape
    #x_size = lattice.shape[0]
    
    a_pop = np.count_nonzero(lattice == 0, axis=0)
    b_pop = np.count_nonzero(lattice == 1, axis=0)
    c_pop = np.count_nonzero(lattice == 2, axis=0)
    pop = a_pop + b_pop + c_pop
    
    densities = np.vstack((a_pop, b_pop, c_pop, pop))
    # TODO:
    # -sum up populations
    # - divide by x_size
    return densities / (x_size * 1.0)

def calc_corr_function(lattice, spec, r_min, r_max, sub_d_squared):
    x_size, y_size = lattice.shape
    r_size = r_max - r_min
    r_range = range(r_min, r_max)
    densities = calc_densities(lattice)
    
    corr = np.zeros((r_size, y_size))
    for r in r_range:
        corr_r = np.zeros(y_size)
        for i in range(x_size):
            target_index = i + r
            if (target_index > 0):
                target_index = target_index % x_size
            corr_xr = (lattice[i,:] == spec) & (lattice[target_index,:] == spec)
            corr_r = corr_r + corr_xr.astype(int)
        r_ind = r - r_min
        
        if (sub_d_squared):
            np.put(corr[r_ind,:], range(r_size), (corr_r / (1.0 * x_size)) - (densities[spec,:]*densities[spec,:]))
        else:
            np.put(corr[r_ind,:], range(r_size), corr_r/(1.0 * x_size))
    
    return corr, r_range, densities

def calc_averages(prefix, t_min, t_max, spec, r_min, r_max, print_prog, sub_d_squared=True, calc_avg_den=True, print_densities=False):
    pbar = tqdm(total=t_max-t_min)
    fname = prefix + str(t_min) + ".csv"
    lattice = open_data(fname, r_max)
    avg_corr, r_range, avg_den = calc_corr_function(lattice, spec, r_min, r_max, sub_d_squared)
    
    if print_prog:
        pbar.update()
    if (print_densities):
        density_filename = "density_{0}.tsv".format(t_min)
        np.savetxt(density_filename, avg_den, delimiter=',')
    for t in range(t_min + 1, t_max):
        #print(t)
        fname = prefix + str(t) + ".csv"
        lattice = open_data(fname, r_max)
        corr_t, r_range, den_t = calc_corr_function(lattice, spec, r_min, r_max, sub_d_squared)
        
        if (calc_avg_den):
            avg_den = avg_den + den_t
        if (print_densities):
            density_filename = "density_{0}.tsv".format(t)
            np.savetxt(density_filename, den_t, delimiter=',')
        
        avg_corr = avg_corr + corr_t
        if print_prog:
            pbar.update()
    
    avg_corr = avg_corr / ((t_max - t_min) * 1.0)
    
    if (calc_avg_den):
        avg_den = avg_den / ((t_max - t_min) * 1.0)
        return avg_corr, r_range, avg_den
    else:
        return avg_corr, r_range  
    
def find_roots(data, margin=1.0e-6, starting_idx=0, limit_idx=0):
    datasize = data.shape[-1]
    lim = limit_idx
    if ((starting_idx >= datasize - 2) or starting_idx < 0):
        raise ValueError("starting index of {} is invalid, must be between zero and length of array")
    if (limit_idx <= 0 or limit_idx<=starting_idx):
        lim = datasize
    
    for i in range(starting_idx + 1, lim):
        if ((data[i-1] > 0 and data[i] < 0) or (data[i-1] < 0 and data[i] > 0)):
            return i - 1
        if (abs(data[i]) < margin):
            return i
    raise RuntimeError("could not find a root within specified margin")

def calc_corr_lens(corr_function, max_r, write_output=False):
    correlation_lengths = np.zeros(max_r)
    for i in range(max_r):
        correlation_lengths[i] = find_roots(corr_avg.T[i,:])
    if write_output:
        np.savetxt("correlation_length.tsv", correlation_lengths, delimiter=',')
    return correlation_lengths, np.arange(max_r)

parser = ap.ArgumentParser()
parser.add_argument('path')
parser.add_argument('prefix')

parser.add_argument('start', type=int)
parser.add_argument('stop', type=int)

parser.add_argument('--prog', '-p', action='store_true')

args = parser.parse_args()

os.chdir(args.path)

corr_avg, r_range, den_avg = calc_averages(args.prefix, args.start, args.stop, 0, 0, 256, args.prog)
calc_corr_lens(corr_avg, 256, write_output=True)
np.savetxt("correlation_function.tsv", corr_avg, delimiter=',')
np.savetxt("average_density.tsv", den_avg, delimiter=',')
 
