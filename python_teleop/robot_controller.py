import bluetooth
import socket
import threading
import time

def data_recv_thread(controller: 'RobotController'):
    while controller.running:
        if(controller.bluetooth_socket is None):
            time.sleep(0.05)
            continue
        
        buff = ''
        while True:
            c = controller.bluetooth_socket.recv(1)
            if c != b'\n':
                buff += c.decode('utf-8')
            else:
                break

        buff = buff.strip()
        args = buff.split(' ')

        if len(args) == 0:
            continue

        command = args[0]
        if command == 'GYR':
            if len(args) >= 2:
                controller.gyr_angle = float(args[1])
        else:
            pass



class RobotController:
    def __init__(self) -> None:
        self.running = True
        self.bluetooth_socket = None
        self.gyr_angle = 0

        self.recv_thread = threading.Thread(target=data_recv_thread, args=(self,))
        self.recv_thread.start()

    def __del__(self):
        self.running = False
        self.recv_thread.join()

    def connect(self):
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

        # find target device
        target_addr = None
        target_name = None
        for device in devices:
            addr = device[0]
            name = device[1]
            if name == 'HC-06':
                target_addr = addr
                target_name = name
                break
        if target_addr is None:
            print("HC-06 not found")
            return
        else:
            print("HC-06 found")

        print("Connecting to \"{}\" on {}".format(target_name, target_addr))
        sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
        target_channel = 1
        sock.connect((target_addr, target_channel))
        print("Bluetooth platform Connected")

        self.bluetooth_socket = sock

    def disconnect(self):
        if self.bluetooth_socket is not None:
            self.bluetooth_socket.close()
            self.bluetooth_socket = None
    
    def drive(self, forward: float, turn: float):
        line = "DRV {} {}\n".format(forward, turn)
        self.bluetooth_socket.send(line.encode('utf-8'))


