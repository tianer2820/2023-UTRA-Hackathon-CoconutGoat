#!/usr/bin/env python3
import sys
import time
import bluetooth
import socket
import threading
from PyQt6.QtWidgets import QMainWindow, QApplication, QPushButton, QWidget
from PyQt6 import uic, QtGui, QtCore
from robot_controller import RobotController, data_recv_thread
forward = 0
right = 0

class KeyboardInterface(QWidget):
    def __init__(self, sock):
        self.sock = sock
        super(KeyboardInterface, self).__init__()
        self.show()

    def keyPressEvent(self, event) -> None:
        if event.isAutoRepeat():
            return
        global forward, right
        if event.key() == QtCore.Qt.Key.Key_W:
            forward = forward + 1
        if event.key() == QtCore.Qt.Key.Key_S:
            forward = forward - 1
        if event.key() == QtCore.Qt.Key.Key_A:
            right = right - 1
        if event.key() == QtCore.Qt.Key.Key_D:
            right = right + 1
        print("Key press: forward: {}, right: {}".format(forward, right))
        robotcontroller.drive(forward, right)
    def keyReleaseEvent(self, event):
        global forward, right
        if event.key() == QtCore.Qt.Key.Key_W:
            forward = forward - 1
        if event.key() == QtCore.Qt.Key.Key_S:
            forward = forward + 1
        if event.key() == QtCore.Qt.Key.Key_A:
            right = right + 1
        if event.key() == QtCore.Qt.Key.Key_D:
            right = right - 1
        print("Key release: forward: {}, right: {}".format(forward, right))
        robotcontroller.drive(forward, right)

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
        robotcontroller.drive(1, 0)

    def backward_exe(self):
        robotcontroller.drive(-1, 0)

    def left_exe(self):
        robotcontroller.drive(0, -1)

    def right_exe(self):
        robotcontroller.drive(0, 1)

    def stop_exe(self):
        robotcontroller.drive(0, 0)

if __name__ == '__main__':
    global robotcontroller
    robotcontroller = RobotController()
    robotcontroller.connect()
    window_app = QApplication(sys.argv)
    window_app.exec()