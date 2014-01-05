#!/usr/bin/python

import serial
import sys

ss = bytearray(32)
ss[0] = 1
v = sys.argv[1]
print v
if ( v == "0" ):
	ss[1] = 2
elif ( v == "1" ):
	ss[1] = 1
print ss[0],ss[1]
s = serial.Serial('/dev/ttyUSB0',9600)
s.write(ss)
