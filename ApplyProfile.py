#!/usr/bin/env python3

import serial
from sys import argv, exit
from json import loads

# Note. If the key you need isn't here, you can get the code from https://www.arduino.cc/en/Reference/KeyboardModifiers and use that instead.

keyCodes = {
        "LCTRL": "128",
        "LSHIFT": "129",
        "LALT": "130",
        "RETURN": "176",
        "RIGHT": "215",
        "LEFT": "216",
        "DOWN": "217",
        "UP": "218",
        "BACKSPACE": "219",
        "TAB": "179",
        "ESC": "177"
}

if len(argv) < 3:
    print("Usage: %s </path/to/serialPort> </path/to/.acprofile> [slot number (1-5)]" % argv[0]);
    exit(1);

print("Opening serial port %s" % argv[1]);
try:
    boi = serial.Serial(argv[1]);
except:
    print("Failed to open serial port!");
    exit(1);

print("Trying to read profile %s" % argv[2]);
try:
    profile = loads(open(argv[2], 'r').read());
    fc = profile["profile"];
except:
    print("Failed to read profile!");
    exit(1);

if len(profile["name"]) > 12:
    profile["name"] = profile["name"][:11];

try:
    slot = int(argv[3])-1;
except:
    slot = profile["defaultSlot"]-1;

if slot < 0 or slot > 4:
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

boi.write(b"1 %d %s %d %s\r" % (slot, bytearray(fc, "UTF-8"), len(profile["name"]), bytearray(profile["name"], "UTF-8")));
