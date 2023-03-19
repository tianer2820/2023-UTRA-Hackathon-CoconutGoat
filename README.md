# 2023-UTRA-Hackathon-CoconutGoat

This is the robot code for the 2023 UTRA Hackathon from team CoconutGoat.

## Directory structure

**arduino_blackline**: simple code to follow a black tape on ground using two lightness sensors

**arduino_blackline_obstacle**: arduino_blackline, but will stop when obstacle detected (using ultrasonic sensor)

**arduino_bluetooth_test**: debug test code for HC-06 bluetooth module

**arduino_bruteforce**: run straight line at full speed. A backup plan if nothing else works

**arduino_counter_test**: We painted white stripes on the wheels of the robot and try to use that as an encoder. This is the test code for that.

**arduino_firmware**: The final obstacle avoidance code

**arduino_motor_test**: motor hardware test code

**arduino_mpu_test**: test code for mpu6050 module

**python_bluetooth_test**: python test code using the pybluez lib

**python_teleop**: a qt gui to control the robot using bluetooth

Note: the HC-06 module we used induce random noise when transmitting, so in the end we removed it and used only arduino code.

