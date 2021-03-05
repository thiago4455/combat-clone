import random
import socket
import struct

def HandleInit(i, b):
    print('INIT')
    return

def HandleTrusted(i, b):
    print(i,b)
    return

def HandleUntrusted(i, b):
    return



server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 4242))
handlers = [HandleInit, HandleTrusted, HandleUntrusted]

while True:
    rand = random.randint(0, 10)
    message, address = server_socket.recvfrom(1024)
    im = message[:12]
    t,i,s = struct.unpack('iII', im)
    b = struct.unpack(f'iII{s}s', message)[-1]
    handlers[t](i,b)
    server_socket.sendto(message, address)
