import socket
import struct
import sys
import random
import time as _time
import itertools


available_users_list = [i for i in range(1,255)]
users_data = [[0,0,0,0] for i in range(1,255)] # Address, timestamp, room, index
users_uid = {}
rooms = [[]]
next_available_room = 0

def time():
    return round(_time.time() * 1000)


def HandleInit(a, i, ts, s, b):
    global next_available_room 
    uid = struct.unpack('I', b)
    if uid not in users_uid:
        u = available_users_list.pop(random.randint(0,len(available_users_list)-1))
        users_uid[uid] = u
        users_data[u][0] = a
        users_data[u][1] = time()
        users_data[u][2] = next_available_room
        users_data[u][3] = len(rooms[next_available_room])
        rooms[next_available_room].append(u)
        if len(rooms[next_available_room]) > 1:
            next_available_room += 1
            rooms.append([])
    else:
        u = users_uid[uid]
    print(users_data[u][1], u, users_data[u][2])
    for pl in rooms[users_data[u][2]]:
        _s = len(rooms[users_data[u][2]])
        print([(ru, users_data[ru][3]) for ru in rooms[users_data[u][2]]])
        server_socket.sendto( struct.pack(f'iIIIII{"BB"*_s}',0,i,ts,s,pl,_s,*itertools.chain.from_iterable([(ru, users_data[ru][3]) for ru in rooms[users_data[u][2]]]) ), users_data[pl][0])
    return

def HandleTrusted(a, i,ts, s, b):
    u = i >> 24
    print(u, i,b)
    users_data[u][1] = time()
    # print("{0:b}".format(i),b)
    server_socket.sendto(struct.pack(f'iIII{s}s',1,i,ts,s,b), a)
    return

def HandleUntrusted(a, i, ts, s, b):
    u = i >> 24
    # print(u, i,b)
    users_data[u][1] = time()
    print("Recived:",u,i,b,users_data[u][2],rooms[users_data[u][2]],rooms)
    for p in rooms[users_data[u][2]]:
        print("Sent:", i,b, "to:", p)
        server_socket.sendto(struct.pack(f'iIII{s}s',2,i,ts,s,b), users_data[p][0])

    return


server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 4242))
handlers = [HandleInit, HandleTrusted, HandleUntrusted]

while True:
    message, address = server_socket.recvfrom(1024)
    im = message[:16]
    t,i,ts,s = struct.unpack('iIII', im)
    b = struct.unpack(f'iIII{s}s', message)[-1]
    handlers[t](address,i,ts,s,b)
    # for i in range(len(users_data)):
    #     if users_data[i][1] != 0 and users_data[i][1] < time() - 5000:
    #         rooms[users_data[i][2]] = []
    #         users_data[i] = [0,0,0]
    #         available_users_list.append(i)
    #         for k, v in users_uid.items():
    #             if v == i:
    #                 del users_uid[k]
    #                 break

