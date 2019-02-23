# coding:utf-8
import time
import serial
import math
import random

com = serial.Serial('com2', 115200)
i = 0

# test ucom chart
while 1:
	i=i+1
	str = ' 0:%.2f 1:%g 2:%d 3:%d 4:%d 5:10.0\n'%(50*math.sin((i%20)*2*math.pi/20), random.randint(0,100)-50, (i%50), (i%100 -50), (i%50 -10))
	com.write(str.encode())
	time.sleep(0.1)