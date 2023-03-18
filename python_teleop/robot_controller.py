import bluetooth
import socket

class RobotController:
    def __init__(self) -> None:
        self.bluetooth_socket = None

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


