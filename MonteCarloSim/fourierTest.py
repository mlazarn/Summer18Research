import numpy as np
import scipy.fftpack as fft
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.animation as manimation
from matplotlib import colors as colors

T = 4
dt = 0.01
t = np.arange(0.0, T, dt)

s_t = np.sin( 2.0 * np.pi * t) + 0.5*np.sin( 20.0 * np.pi * t)
d_t = np.exp(-t) * np.sin( 2.0 * np.pi * t)

N = int(T // dt)
N_Pad = 1 * N
#d_pad = np.abs(fft.fft(d_t, n=N_Pad))
s_omega = np.abs(fft.fft(s_t, n=N_Pad))
d_omega = np.abs(fft.fft(d_t, n=N_Pad))
freqs = fft.fftfreq(N_Pad, dt)
#freqs_pad = fft.fftfreq(N_Pad, dt)

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

fig, ax = plt.subplots()

l_1 = ax.plot(freqs[:N_Pad//2 - 1], d_omega[:N_Pad//2 - 1], 'o')
l_2 = ax.plot(freqs[:N_Pad//2 - 1], s_omega[:N_Pad//2 - 1], 's-')
#l_2 = ax.plot(freqs_pad, d_pad, 's')


ax.grid(which='both', zorder=1)
fig.set_tight_layout(True)

ax.set_xlabel(r'$\omega$')
ax.set_ylabel(r'$|a(\omega)|$')

plt.show()
