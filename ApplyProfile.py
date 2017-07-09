import serial
from sys import argv

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

boi = serial.Serial(argv[1]);
fc = open(argv[2], 'r').read().replace("\n","");

fc = fc.split(",");

for k,i in enumerate(fc):
    if len(i) == 1:
        fc[k] = str(ord(i[0]));

fc = " ".join(fc);

for i in keyCodes:
    fc = fc.replace(i, keyCodes[i]);

boi.write(b"1 %s\r" % bytearray(fc, "UTF-8"));
