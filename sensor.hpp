#pragma once

class sensor{
    int portIn;
    int portOut;
    int type; // 0 -- infrared; 1 -- ultrasonic

    sensor(int type, int in, int out = -1);
    /* return distance in centimeters for ultrasonic, return analog output for infrared */
    long getD();

    /* return digital output for infrared */
    int getD_d();
};