import serial
import os
import os.path
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

import logging
import threading
import time

fifo_path = "/home/hiram/sla4.txt"

#print (os.fifo_path.exists(fifo_path))
#if os.fifo_path.exists(fifo_path) is False:
#    os.mkfifo(fifo_path, 0o777)
#print (os.fifo_path.exists(fifo_path))

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

esp_port = serial.Serial('/dev/ttyUSB0', 9600)
received_data = []

def acquire_serial_data():
#    for x in range(100):
    while(True):
        line = esp_port.readline()
        print(line)
        received_data.append(line.decode("utf-8"))

'''
maybe not needed after all
def opener(path, flags):
    return os.open(path, os.O_RDONLY | os.O_NONBLOCK)
'''

def acquire_data():
    data = open('firstReadings.txt', 'r').read()
    lines = data.split('\n')
    #out = open(fifo_path, 'w')
    for line in lines:
        #out.write(str(line) + '\n')
        received_data.append(str(line))
        time.sleep(0.1)
        #print("wrote a line")
    #out.close()

def animate(i):
    '''
    file_descriptor = os.open(fifo_path, os.O_RDONLY | os.O_NONBLOCK) 
    data_file = os.fdopen(file_descriptor)#open(fifo_path, 'r', opener=file_descriptor) 
    data_file.flush()
    print("file descriptor {}".format(file_descriptor))
    graph_data = data_file.read()
    print("graph_data {}".format(graph_data))
    lines = graph_data.split('\n')
    '''
    cols = []
    #data is intended to come already in a list
    #logging.info("Starting animate thread #%s", name)
    sensor_0 = []
    sensor_1 = []
    sensor_2 = []
    sensor_3 = []
    sensor_4 = []
    sensor_5 = []
    sensor_6 = []
    sensor_7 = []
    sensor_8 = []
    sensor_9 = []
    sensor_10= []
    sensor_11= []
   
    x = 0 
    xs = []
    for line in received_data:
        if len(line) > 1:
            sen_0 ,\
	    sen_1 ,\
	    sen_2 ,\
	    sen_3 ,\
	    sen_4 ,\
	    sen_5 ,\
	    sen_6 ,\
	    sen_7 ,\
	    sen_8 ,\
	    sen_9 ,\
	    sen_10,\
	    sen_11 = line.split(',')[2:]
            sensor_0.append(float(sen_0))
            sensor_1.append(float(sen_1))
            sensor_2.append(float(sen_2))
            sensor_3.append(float(sen_3))
            sensor_4.append(float(sen_4))
            sensor_5.append(float(sen_5))
            sensor_6.append(float(sen_6))
            sensor_7.append(float(sen_7))
            sensor_8.append(float(sen_8))
            sensor_9.append(float(sen_9))
            sensor_10.append(float(sen_10))
            sensor_11.append(float(sen_11))
            x = x + 1

    xs = range(0, x)
    ax1.clear()
    ax1.plot(\
	 xs, sensor_0,\
         xs, sensor_1,\
         xs, sensor_2,\
         xs, sensor_3,\
         xs, sensor_4,\
         xs, sensor_5,\
         xs, sensor_6,\
         xs, sensor_7,\
         xs, sensor_8,\
         xs, sensor_9,\
         xs, sensor_10,\
         xs, sensor_11)
    #os.close(file_descriptor)

this func can be a thread later. lets wait for now
def ani_func():
    ani = animation.FuncAnimation(fig, animate, interval=1000)

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,datefmt="%H:%M:%S")
    
    logging.info("Main    : before creating thread")
    serial_read_thread = threading.Thread(target=acquire_serial_data)
    #serial_read_thread = threading.Thread(target=ani_func)
    logging.info("Main    : before running thread")
    serial_read_thread.start()
    logging.info("Main    : wait for the thread to finish")
    #x.join()
    #logging.info("Main    : all done") 
    ani = animation.FuncAnimation(fig, animate, interval=500)
    plt.show()

