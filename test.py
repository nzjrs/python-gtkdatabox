import random

import sys
sys.path.insert(0, ".libs")

import gobject
import gtk.gdk
import gtkdatabox

def on_timeout(plot):
    plot.update([random.random(), random.random(), 2.0*random.random()])
    return True

w = gtk.Window()
plot = gobject.new(gtkdatabox.DataboxPlot, num_plots=3)
w.add(plot)

grid = gtkdatabox.DataboxGrid(10,10,gtk.gdk.Color(127,127,127),1)
plot.graph_add(grid)

gobject.timeout_add(200, on_timeout, plot)

plot.show()
w.show_all()
gtk.main()

