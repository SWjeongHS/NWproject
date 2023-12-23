from socket import *
import numpy as np

class SocketInfo():
    HOST = "10.10.20.100"
    PORT = 9190
    BUFSIZE = 10000000000
    ADDR = (HOST, PORT)

a="2022,"#년도
b="142,"#상품
c="02"#지역
i=0
sendserver=f'{a+b+c}'
csock = socket(AF_INET, SOCK_STREAM)
csock.connect(SocketInfo.ADDR)
print("connection is success")

to_server = "P" # Change to string
right_method = to_server.encode("UTF-8")
sent = csock.send(right_method)
print("Send Data : {}, bytes len : {}, bytes : {}".format(to_server, len(right_method), right_method))

to_server = sendserver # Change to string
right_method = to_server.encode("UTF-8")
print("Send Data : {}, bytes len : {}, bytes : {}".format(to_server, len(right_method), right_method))
csock.send(right_method)
while 1:
    commend = csock.recv(SocketInfo.BUFSIZE)
    data = commend.decode("UTF-8")

    print("Contents : {}".format(data))
    print()
csock.close()
exit()
