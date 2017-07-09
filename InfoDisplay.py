import psutil
from time import sleep, strftime
import serial
from sys import argv

boi = serial.Serial(argv[1]);


while True:
    mem = psutil.virtual_memory();
    mem = round((100/(mem.total/mem.used))*10)/10;

    cpu = psutil.cpu_percent();
    boi.write(b"0Time: %s,CPU: %d%%,RAM: %d%%\r" % (bytearray(strftime("%H:%M:%S"), "UTF-8"), cpu, mem));
    sleep(1);
