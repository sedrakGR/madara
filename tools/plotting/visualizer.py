# import the submodules and give them shorter aliases
import madara
import madara.knowledge as engine
import madara.transport as transport
import sys
import numpy as np
import math
from itertools import groupby
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Button
from matplotlib import style
from mpl_toolkits.mplot3d import Axes3D


cnt = 1
pause = False
def visualizer(x, y, z, u, v, w):


    # matplotlib customizations and set subplots
    style.use('fivethirtyeight')
    fig = plt.figure()
    ax1 = fig.add_subplot(111, projection = '3d')
    # ax2 = fig.add_subplot(2,1,2)

    # instantiate and shorten KB and KR
    kb = engine.KnowledgeBase()
    kr = engine.KnowledgeRecord()

    # pause = False

    def ori_conv(z):
        ang = math.degrees(z)
        x = math.cos(ang)
        y = math.sin(ang)
        z = 0
        return x, y, z

    def onClick(event):
        global pause
        pause ^= True

    settings = engine.CheckpointSettings()
    # settings.prefixes('.gams.frames.p1_base_footprint')
    settings.filename = ("/media/sedrak/OS/SET/projects/"
                            "SAFE_reliability_rc_v3.4.0__pikachu-16-0004__2018-08-16-09-47-39.stk")

    # # Read in if binary file 
    # kb.load_context(settings.filename) 

    reader = engine.CheckpointReader(settings)
    # map = dict(list(reader))
    # print map.values()[2].toi()

    prefix1 = '.gams.frames.p1_base_footprint'
    prefix2 = '.gams.frames.p1_map'
    suffix = '.origin'


    # Check to see if an axis has all same elements (makes plotting faster) 
    def checkEqual3(lst):
        return lst[1:] == lst[:-1]  

    def animate(i):
        global cnt
        # Plot the 3d
        if not pause: 
            ax1.clear()
            x1 = x[:cnt]
            y1 = y[:cnt]     
            Axes3D.plot(ax1, x1, y1, z)
            u, v, w1 = ori_conv(w[cnt])
            ax1.quiver(x[cnt], y[cnt], z, u, v, w1, length=0.01, normalize=True)
            ax1.set_title(prefix1)
            ax1.set_xlabel('X')
            ax1.set_ylabel('Y')
            ax1.set_zlabel('Altitude (m)')
            cnt = cnt + 1 
            # print x[cnt] , y[cnt]  

    # Get KRs and make into list
    # x = []
    # y = []
    # z = []
    # u = []
    # v = []
    # w = []

    # for key, value in reader: 
    #     if value.is_double_type() or value.is_integer_type() and suffix in key and prefix1 in key:
    #         x.append(value.retrieve_index(0).to_string())
    #         y.append(value.retrieve_index(1).to_string())
    #         z.append(value.retrieve_index(2).to_string())   
    #         u.append(value.retrieve_index(3).to_string())
    #         v.append(value.retrieve_index(4).to_string())
    #         w.append(value.retrieve_index(5).to_string())
    #         print(value.retrieve_index(0).to_string(), value.retrieve_index(1).to_string(), value.retrieve_index(2).to_string(),
    #                 value.retrieve_index(3).to_string(), value.retrieve_index(4).to_string(), value.retrieve_index(5).to_string())

    # print np.column_stack([x, y, z])

    # Call fn to find if all same elements
    same_z = checkEqual3(z)
    same_u = checkEqual3(u)
    same_v = checkEqual3(v)

    if same_z:
        x = [float(x) for x in x]
        y = [float(y) for y in y]
        w = [float(w) for w in w]
        z = 0
        u = 0
        v = 0
    else: 
        x = [float(x) for x in x]
        y = [float(y) for y in y]
        z = [float(z) for z in z]

    # Animate plot 
    # Args: interval = interval to plot (ms)
    ani = animation.FuncAnimation(fig, animate, interval=10)
    axpause = plt.axes([0.7, 0.05, 0.1, 0.075])
    bpause = Button(axpause,'Pause/Play')
    bpause.on_clicked(onClick)

    return plt.show()