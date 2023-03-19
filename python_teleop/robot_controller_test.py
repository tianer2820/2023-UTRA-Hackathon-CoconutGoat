import time

from robot_controller import RobotController


robot = RobotController()

robot.connect()


while True:
    time.sleep(0.2)
    print(robot.analog_sensors)

