import os
import sys

import numpy as np
import matplotlib.pyplot as plt
import scipy as sp
import matplotlib.animation as manimation

from tqdm import tqdm

def generate_array(prefix, timesteps, width):
    array = np.zeros(timesteps + 1, width)
    for t in range(timesteps + 1):
        filename = prefix + str(t) + ".csv"
        array[i] = np.genfromtxt(filename, dtype=float, delimiter=',')[0, :]
    return array

def take_fft(array, timesteps, width):
    array_transpose = a.T
    dims = array_transpose.shape()
    fft_array = np.zeros(dims)
    
    for x in range(width):


