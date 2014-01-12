#!/usr/bin/python
# -*- coding: utf-8 -*-


import serial
import time
import sys
import binascii
import commands
import os

reload(sys)
sys.setdefaultencoding("utf-8")


result="switch "
ss = bytearray(32)
ss[0] = 1
v = sys.argv[1]
if ( v == "0" ):
	ss[1] = 0
	result+="close "
elif ( v == "1" ):
	ss[1] = 1
	result+="open "

s = serial.Serial('/dev/ttyUSB0',9600,timeout=1)

success=0
retry=3
isopen=0
while (retry!=0):
	retry-=1
	s.write(ss)
	#print "send:" + binascii.b2a_hex(ss);
	rr = s.read(32)
	tmp = bytearray(rr)
	#print "recv:" + binascii.b2a_hex(rr);
	if ( len(tmp)==32 and tmp[0] ==2 and tmp[1] == ss[1]):
		success=1
		isopen=tmp[1]
		break

#if ( isopen == 1 ):
#	zh = "espeak -v+f5 \"open\" 2>/dev/null &"
#else:
#	zh = "espeak -v+f5 \"close\"  2>/dev/null &"

if ( success == 1 ): 
	result +="success"
else:
	result +="failure"

result += "\n"
print result
t= time.strftime("%Y-%m-%d %H:%M:%S",time.localtime());
f = open("switch.log","a")
f.write(t+" "+result)
f.close()

#os.system(zh)

sys.exit(0)
