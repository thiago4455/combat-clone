import socket
import struct
import sys
import random

available_users_list = [i for i in range(1,255)]
users_uid = {}



def HandleInit(a, i, s, b):
    uid = struct.unpack('I', b)
    if uid not in users_uid:
        u = available_users_list.pop(random.randint(0,len(available_users_list)-1))
        users_uid[uid] = u
    else:
        u = users_uid[uid]
    print(u)
    server_socket.sendto(struct.pack(f'iIII',0,i,s,u), a)
    return

def HandleTrusted(a, i, s, b):
    print(i,b)
    # print("{0:b}".format(i),b)
    server_socket.sendto(struct.pack(f'iII{s}s',1,i,s,b), a)
    return

def HandleUntrusted(i, b):
    return


server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 4242))
handlers = [HandleInit, HandleTrusted, HandleUntrusted]

while True:
    message, address = server_socket.recvfrom(1024)
    im = message[:12]
    t,i,s = struct.unpack('iII', im)
    b = struct.unpack(f'iII{s}s', message)[-1]
    handlers[t](address,i,s,b)
