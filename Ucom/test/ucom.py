# coding:utf-8
# 对于串口的测试可以使用 vspd 建立成对的虚拟串口
# 可以使用 ipython 加载此文件内的函数方便测试

import time
import serial
import math
import random
import socket

# 测试串口图像
def testUartGraph():
    com = serial.Serial('com2', 115200)
    i = 0
    while 1:
        i=i+1
        s = ' 0:%.2f 1:%g 2:%d 3:%d 4:%d 5:10.0\n'%(50*math.sin((i%20)*2*math.pi/20), random.randint(0,100)-50, (i%50), (i%100 -50), (i%50 -10))
        com.write(s.encode())
        time.sleep(0.1)

# 测试 ucom 的多 tcp 客户端的链接
def testMutliTcpClient(port):
    client_1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_1.connect(('127.0.0.1', port))
    client_1.send('client_1'.encode())

    client_2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_2.connect(('127.0.0.1', port))
    client_2.send('client_2'.encode())

    time.sleep(0.5)
    client_1.close()
    client_2.close()

# 测试 ucom 的 tcp 客户端
def testTcpAsServer():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 8051))
    server.listen()
    print("Server start")
    while True:
        connection, address = server.accept()
        print('Connected by', address)
        buf = connection.recv(1)
        connection.send(buf)
        connection.close()