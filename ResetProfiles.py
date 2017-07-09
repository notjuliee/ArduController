#!/usr/bin/env python3

import serial
from sys import argv, exit

if len(argv) < 2:
    print("Usage: %s </path/to/serialPort>" % argv[0]);
    exit(1);

print("Opening serial port %s" % argv[1]);
try:
    boi = serial.Serial(argv[1]);
except:
    print("Failed to open serial port!");
    exit(1);

boi.write(b"2");
print("Please reboot ArduBoy to finish reset");
