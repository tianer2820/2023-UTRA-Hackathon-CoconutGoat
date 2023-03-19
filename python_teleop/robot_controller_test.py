import time

from robot_controller import RobotController


robot = RobotController()

robot.connect()


while True:
    time.sleep(1)
    robot.drive(0.3, 0)

