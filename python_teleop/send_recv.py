import bluetooth
import socket
import threading


def connect_bluetooth() -> socket.socket:
    print("searching devices...")
    devices = None
    while True:
        devices = bluetooth.discover_devices(lookup_names=True, duration=4)
        if len(devices) > 0:
            print("devices found:")
            for dev in devices:
                addr = dev[0]
                name = dev[1]
                print("Name: {}, addr: {}".format(name, addr))
            break
        else:
            print("no device found, retry...")
            continue

    # auto find target device
    target_addr = None
    target_name = None
    for device in devices:
        addr = device[0]
        name = device[1]
        if name == 'HC-06':
            # found
            target_addr = addr
            target_name = name
            break

    if target_addr is None:
        print("HC-06 not found")
        return
    else:
        print("HC-06 is found, auto connect...")

    print("Connecting to \"{}\" on {}".format(target_name, target_addr))
    sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
    target_channel = 1
    sock.connect((target_addr, target_channel))

    print("Bluetooth platform Connected")
    return sock


def recv_thread(sock: socket.socket):
    while True:
        c = sock.recv(1)
        print(c)