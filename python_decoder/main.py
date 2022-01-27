
import struct
from turtle import width
from matplotlib.backend_bases import MouseEvent
import serial
import time
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backend_bases import MouseButton
from collections import Counter

MAX_ENTRIES = 200
ENTRY_SIZE = 8


class SnaptoCursor(object):
    def __init__(self, ax, x, y):
        self.ax = ax
        self.ly1 = ax.axvline(x[0],color='r', alpha=0.9)  # the vert line
        self.ly2 = ax.axvline(x[0],color='g', alpha=0.9)  # the vert line
        self.marker1, = ax.plot([x[0]],[y[0]], marker="o", color="crimson", zorder=3)
        self.marker2, = ax.plot([x[0]],[y[0]], marker="o", color="crimson", zorder=3) 
        self.x = x
        self.y = y
        self.cursor1On = 1
        self.cursor2On = 1
        self.cursor1x = 0
        self.cursor2x = 0
        self.txt1 = ax.text(0.7, 0.9, '')
        self.txt2 = ax.text(0.7, 0.9, '')

    def mouse_move(self, event):
        if not event.inaxes: return
        x, y = event.xdata, event.ydata
        # indx = np.searchsorted(self.x, [x])[0]
        # x = self.x[indx]
        # y = self.y[indx]
        if self.cursor1On == 1: 
            self.ly1.set_xdata(x)
            self.marker1.set_data([x],[y])
            self.txt1.set_text('t1=%1.6f' % (x))
            self.txt1.set_position((x,y))
            self.cursor1x = x
        if self.cursor2On == 1: 
            self.ly2.set_xdata(x)
            self.marker2.set_data([x],[y])
            self.txt2.set_text('t2=%1.6f\nt2-t1=%f' % (x,abs(self.cursor1x-x)))
            self.txt2.set_position((x,y))
        self.ax.figure.canvas.draw_idle()
    
    def on_click(self,event):
        if plt.get_current_fig_manager().toolbar.mode != '': return

        if event.button == MouseButton.LEFT:
            if self.cursor1On:
                self.cursor1On = 0
            else:
                self.cursor1On = 1

        if event.button == MouseButton.RIGHT:
            if self.cursor2On:
                self.cursor2On = 0
            else:
                self.cursor2On = 1


ser = serial.Serial('COM4',baudrate=2000000)  # open serial port
ser.timeout = 0.5
ser.write("@".encode())
time.sleep(0.5)
ser.write("!".encode())
mybytearray = bytearray()
mybytearray  = ser.read(MAX_ENTRIES*ENTRY_SIZE)
print(mybytearray)
print(len(mybytearray))
ser.close()  

if len(mybytearray) > 0:
    f = open('./capture.bin', 'wb')
    f.write(mybytearray)
    f.close()

f = open("./capture.bin", "rb")
mybytearray = bytearray()

IRQs = []
data = []
for i in range(MAX_ENTRIES-1):
    data.append(struct.unpack("IHBB", f.read(ENTRY_SIZE)))
    IRQs.append(data[i][2])

IRQLabels = list(Counter(IRQs).keys())
IRQLabels.sort()

print(IRQLabels)

IRQList = []
timeList = []
IRQStates = []

for i in range(len(IRQLabels)):
    l = []
    IRQList.append(l)
    IRQStates.append(int(i*2))

print(IRQList)   
    

for i in range(MAX_ENTRIES-1):
    timeList.append(data[i][0]*0.001 + data[i][1]*0.000001)
    for j in range(len(IRQLabels)):
        IRQList[j].append(IRQStates[j])
        if data[i][2] == IRQLabels[j]:
            if data[i][3] == 0:
                IRQStates[j] = 1 + 2*j
            else:
                IRQStates[j] = 0 + 2*j
        IRQList[j].append(IRQStates[j])
    timeList.append(data[i][0]*0.001 + data[i][1]*0.000001)

fig, ax = plt.subplots()
cursor1 = SnaptoCursor(ax,timeList,IRQList[0])
cid1=  plt.connect('motion_notify_event', cursor1.mouse_move)
cid2 = plt.connect('button_press_event', cursor1.on_click)

labels = ["IRQ1 (TIM16 highest)", "IRQ2 (TIM6 medium)", "IRQ3 (TIM7 lowest)"]

for i in range(len(IRQLabels)):
    ax.plot(timeList,IRQList[i],marker='o',linewidth=2, label=labels[i])
ax.set_ylim([-1, len(IRQLabels)*2 + 1])
ax.legend()
ax.grid()
plt.show()
