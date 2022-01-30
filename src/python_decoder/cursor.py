from matplotlib.backend_bases import MouseButton
from matplotlib.backend_bases import MouseEvent
import matplotlib.pyplot as plt
import numpy as np

class Cursor(object):
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
            self.txt1.set_position((x,y-0.5))
            self.cursor1x = x
        if self.cursor2On == 1: 
            self.ly2.set_xdata(x)
            self.marker2.set_data([x],[y])
            self.txt2.set_text('t2=%1.6f\nt2-t1=%f' % (x,abs(self.cursor1x-x)))
            self.txt2.set_position((x,y+0.5))
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