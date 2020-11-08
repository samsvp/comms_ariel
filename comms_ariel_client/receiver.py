#!/usr/bin/env python3

"""
Sample code to receive protobuf's messages
"""

import socket
import proto.comms_ariel_pb2

UDP_IP = "192.168.0.151"
UDP_PORT = 7000

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print(data)
    drone_msg = proto.comms_ariel_pb2.DroneToUSVMessage()
    drone_msg.ParseFromString(data)
    print(drone_msg)