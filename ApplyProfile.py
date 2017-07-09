#!/usr/bin/env python3

import serial
from sys import argv, exit

# Note. If the key you need isn't here, you can get the code from https://www.arduino.cc/en/Reference/KeyboardModifiers and use that instead.

keyCodes = {
        "LCTRL": "128",
        "LSHIFT": "129",
        "LALT": "130",
        "RETURN": "176",
        "RIGHT": "215",
        "LEFT": "216",
        "DOWN": "217",
        "UP": "218"
}

if len(argv) < 3:
    print("Usage: %s </path/to/serialPort> </path/to/.acprofile> [slot number (1-2)]" % argv[0]);
    exit(1);

print("Opening serial port %s" % argv[1]);
try:
    boi = serial.Serial(argv[1]);
except:
    print("Failed to open serial port!");
    exit(1);

print("Trying to read profile %s" % argv[2]);
try:
    fc = open(argv[2], 'r').read().replace("\n","");
except:
    print("Failed to read profile!");
    exit(1);

try:
    slot = int(argv[3]);
except:
    slot = 1;

if slot < 1 or slot > 2:
    print("Invalid slot number!");
    exit(1);

print("Parsing profile (Part 1)");
fc = fc.split(",");
for k,i in enumerate(fc):
    if len(i) == 1:
        fc[k] = str(ord(i[0]));

print("Parsing profile (Part 2)");
fc = " ".join(fc);
for i in keyCodes:
    fc = fc.replace(i, keyCodes[i]);

boi.write(b"1 %d %s\r" % (slot, bytearray(fc, "UTF-8")));
