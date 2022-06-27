import time
from matplotlib import pyplot as plt
import numpy as np

import serial
import serial.tools.list_ports

from multiprocessing import Process, Queue
from collections import deque

BUFFER_SIZE = 1024
XY_MODE = True

def find_port():
    '''
    Function to automatically find and return Serial object for CH340 devices

    Raises Exception if port not Found
    '''
    ports = list(filter(lambda p: "CH340" in str(p) or "USB Serial" in str(p), list(serial.tools.list_ports.comports())))

    if ports:
        return  serial.Serial(port=str(ports[0]).split(" ")[0], baudrate=115200)
    else:
        raise Exception(f"COM Port not found: {list(map(lambda p: str(p), list(serial.tools.list_ports.comports())))}")
    
    return None


def fetch(queue: Queue) -> None:
    arduino = find_port()
    while True:
        data = arduino.read_until()
        try:
            if XY_MODE:
                xy_data = list(map(lambda x: float(x.strip()), data.split(b',')))
                queue.put(xy_data)
            else:
                queue.put(int(data))
                # print(data)
        except ValueError:
            continue
        #queue.put(data)

def main(queue: Queue) -> None:

    if XY_MODE:
        x = deque(maxlen=BUFFER_SIZE)
        for i in range(BUFFER_SIZE):
            x.append(0)
    else:
        x = np.linspace(0, BUFFER_SIZE, num=BUFFER_SIZE)

    y = deque(maxlen=BUFFER_SIZE) 
    for i in range(BUFFER_SIZE):
        if XY_MODE:
            y.append(0)
        else:    
            y.append(None)

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    line, = ax.plot([], marker='o', lw=0, markersize=3)
    text = ax.text(0.8,0.5, "")

    ax.set_xlim([0, 3])
    ax.set_ylim([-10,200])

    fig.canvas.draw()   # note that the first draw comes before setting data 

    plt.show(block=False)


    t_start = time.time()
    k=0.
    
    while True:
    #for i in np.arange(1000):

        while not queue.empty():
            if XY_MODE:
                xy_data = queue.get()
                x.append(xy_data[0])
                y.append(xy_data[1])
            else:
                y.append(queue.get())
        
        if not XY_MODE:
            y_adjusted = list(filter(lambda x: x is not None, y))
            if len(y_adjusted) == 0:
                continue

            x_adjusted = x[BUFFER_SIZE - len(y_adjusted):]

            ax.set_xlim(min(x_adjusted),max(x_adjusted)*1.01)
            ax.set_ylim(min(y_adjusted) - max(y_adjusted)*0.01,max(y_adjusted)*1.01)

            line.set_data(x_adjusted, y_adjusted)
        else:
            ax.set_xlim(0,max(x)*1.01)
            ax.set_ylim(min(y) - max(y)*0.05,max(y)*1.05)
            line.set_data(x, y)

        # redraw everything
        fig.canvas.draw()

        fig.canvas.flush_events()
        #alternatively you could use
        #plt.pause(0.000000000001) 
        # however plt.pause calls canvas.draw(), as can be read here:
        #http://bastibe.de/2013-05-30-speeding-up-matplotlib.html


if __name__ == "__main__":
    queue = Queue(maxsize=BUFFER_SIZE)
    main_p = Process(target=main, args=(queue, )) 
    fetch_p = Process(target=fetch, args=(queue, ))

    main_p.start()
    fetch_p.start()

    main_p.join()
    fetch_p.join()
