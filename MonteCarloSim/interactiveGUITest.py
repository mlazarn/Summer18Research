import numpy as np
import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons

# Initializing figure and axis
fig, ax = plt.subplots()
plt.subplots_adjust(left = 0.25, bottom = 0.25)

t = np.arange(0.0, 1.0, 0.001)
a0 = 5
f0 = 3
x0 = 0.0

s = a0 * np.sin(2 * np.pi * f0 * t)
l, = plt.plot(t, s, lw=2, color='red')
ind = ax.axvline(x=x0)
plt.axis([0, 1, -10, 10])

#ttl = ax.set_title('Amplitude = {}'.format(a0))
ttl = ax.set_title('x = {}'.format(x0))
axcolor = 'lightgoldenrodyellow'
axfreq = plt.axes([0.25, 0.1, 0.65, 0.03], facecolor=axcolor)
#axamp = plt.axes([0.25, 0.15, 0.65, 0.03], facecolor=axcolor)
axpos = plt.axes([0.25, 0.15, 0.65, 0.03], facecolor=axcolor)

sfreq = Slider(axfreq, 'Freq', 0.1, 30, valinit=f0)
#samp = Slider(axamp, 'Amp', 0.1, 100, valinit=a0)
spos = Slider(axpos, 'Position', 0.0, 1.0, valinit=x0)


def update(val):
    #amp = samp.val
    x = spos.val
    freq = sfreq.val
    #l.set_ydata(amp * np.sin(2 * np.pi * freq * t))
    l.set_ydata(a0 * np.sin(2 * np.pi * freq * t))
    ind.set_data([x,x], [0,1])
    #ttl.set_text('Amplitude = {}'.format(amp))
    ttl.set_text('x = {0:.3f}'.format(x))
    fig.canvas.draw_idle()

sfreq.on_changed(update)
#samp.on_changed(update)
spos.on_changed(update)

resetax = plt.axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', color=axcolor, hovercolor='0.975')

def reset(event):
    sfreq.reset()
    #samp.reset()
    spos.reset()
button.on_clicked(reset)

printax = plt.axes([0.695, 0.025, 0.1, 0.04])
button2 = Button(printax, 'Print', color=axcolor, hovercolor='0.975')

def printfunc(event):
    print('________________')
    for idx, val in enumerate(y):
        print('{0}, {1:.3f}'.format(idx, val))
    print('________________')
button2.on_clicked(printfunc)

y = []

writeax = plt.axes([0.59, 0.025, 0.1, 0.04])
button3 = Button(writeax, 'Write', color=axcolor, hovercolor='0.975')

def writefunc(event):
    #amp = samp.val
    x = spos.val
    #y.append(amp)
    y.append(x)
    print('appended y[{0}] = {1:.2f}'.format(len(y) - 1, x))
button3.on_clicked(writefunc)

plusax = plt.axes([0.535, 0.025, 0.05, 0.04])
minsax = plt.axes([0.48, 0.025, 0.05, 0.04])
button4 = Button(plusax, '+ 0.1', color=axcolor, hovercolor='0.975')
button5 = Button(minsax, '- 0.1', color=axcolor, hovercolor='0.975')

def plusfunc(event):
    x_curr = spos.val
    x_new = x_curr + 0.1
    if 0.0 <= x_new and 1.0 >= x_new:
        x = x_new
        spos.set_val(x)
        ind.set_data([x,x], [0,1])
        #ttl.set_text('Amplitude = {}'.format(amp))
        ttl.set_text('x = {0:.3f}'.format(x))
        fig.canvas.draw_idle()
def minsfunc(event):
    x_curr = spos.val
    x_new = x_curr - 0.1
    if 0.0 <= x_new and 1.0 >= x_new:
        x = x_new
        spos.set_val(x)
        ind.set_data([x,x], [0,1])
        #ttl.set_text('Amplitude = {}'.format(amp))
        ttl.set_text('x = {0:.3f}'.format(x))
        fig.canvas.draw_idle()
button4.on_clicked(plusfunc)
button5.on_clicked(minsfunc)

rax = plt.axes([0.025, 0.5, 0.15, 0.15], facecolor=axcolor)
radio = RadioButtons(rax, ('red', 'blue', 'green'), active=0)

def colorfunc(label):
    l.set_color(label)
    fig.canvas.draw_idle()
radio.on_clicked(colorfunc)

plt.show()
