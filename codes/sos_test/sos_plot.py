'''
By Mudit Aggarwal at TI-CEPD, NSUT
To plot curve for Scienec on a Stick Project
'''

from matplotlib import pyplot as plt
import numpy as np

import serial
import serial.tools.list_ports

from multiprocessing import Process, Queue
from collections import deque

BUFFER_SIZE = 1024

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
            queue.put(int(data))
            # print(data)
        except ValueError:
            continue
        #queue.put(data)

def main(queue: Queue) -> None:
    x = np.linspace(0, BUFFER_SIZE, num=BUFFER_SIZE)

    y = deque(maxlen=BUFFER_SIZE) 
    for i in range(BUFFER_SIZE):
        y.append(None)

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)

    line, = ax.plot([], marker='o', lw=0, markersize=3)

    ax.set_xlim([0, 3])
    ax.set_ylim([-10,200])

    fig.suptitle("Science on a Stick", fontsize=15)
    ax.set_xlabel("Time unit", fontsize=12)
    ax.set_ylabel("Voltage unit", fontsize=12)

    ax.grid()

    fig.canvas.draw()   # note that the first draw comes before setting data 

    plt.show(block=False)
    
    while True:

        while not queue.empty():
            y.append(queue.get())
        
        y_adjusted = list(filter(lambda x: x is not None, y))
        if len(y_adjusted) == 0:
            continue

        x_adjusted = x[BUFFER_SIZE - len(y_adjusted):]

        ax.set_xlim(min(x_adjusted),max(x_adjusted)*1.01)
        ax.set_ylim(min(y_adjusted) - max(y_adjusted)*0.01,max(y_adjusted)*1.01)

        line.set_data(x_adjusted, y_adjusted)

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
