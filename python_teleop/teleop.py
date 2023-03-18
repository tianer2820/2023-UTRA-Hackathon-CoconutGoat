#!/usr/bin/env python3
import sys
import time
import bluetooth
import socket
import threading
from PyQt6.QtWidgets import QMainWindow, QApplication, QPushButton, QWidget
from PyQt6 import uic, QtGui, QtCore

class KeyboardInterface(QWidget):
    def __init__(self, sock):
        self.sock = sock
        super(KeyboardInterface, self).__init__()
        self.show()

    def keyPressEvent(self, event) -> None:
        if event.key() == QtCore.Qt.Key.Key_W:
            send_forward(self.sock, 1, 0)
        elif event.key() == QtCore.Qt.Key.Key_S:
            send_forward(self.sock, -1, 0)
        elif event.key() == QtCore.Qt.Key.Key_A:
            send_forward(self.sock, 0, -1)
        elif event.key() == QtCore.Qt.Key.Key_D:
            send_forward(self.sock, 0, 1)
    def keyReleaseEvent(self, event):
        send_forward(self.sock, 0, 0)

class MainWindow(QMainWindow):
    def __init__(self, sock):
        super(MainWindow, self).__init__()
        uic.loadUi("mainwindow.ui", self)
        self.sock = sock
        self.forward = self.findChild(QPushButton, "forward")
        self.backward = self.findChild(QPushButton, "backward")
        self.left = self.findChild(QPushButton, "left")
        self.right = self.findChild(QPushButton, "right")
        self.stop = self.findChild(QPushButton, "stop")
        self.forward.clicked.connect(self.forward_exe)
        self.backward.clicked.connect(self.backward_exe)
        self.left.clicked.connect(self.left_exe)
        self.right.clicked.connect(self.right_exe)
        self.stop.clicked.connect(self.stop_exe)
        self.show()

    def forward_exe(self):
        send_forward(self.sock, 1, 0)

    def backward_exe(self):
        send_forward(self.sock, -1, 0)

    def left_exe(self):
        send_forward(self.sock, 0, -1)

    def right_exe(self):
        send_forward(self.sock, 0, 1)

    def stop_exe(self):
        send_forward(self.sock, 0, 0)


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


def send_forward(sock, forward_v, turn_v):
    line = "DRV " + str(forward_v) + " " + str(turn_v) + "\n"
    sock.send(line.encode('utf-8'))


if __name__ == '__main__':
    sock = connect_bluetooth()
    print(sock)
    thread = threading.Thread(target=recv_thread, args=(sock,))
    thread.start()
    window_app = QApplication(sys.argv)
    mainwindowui = MainWindow(sock)
    keyboardui = KeyboardInterface(sock)
    window_app.exec()