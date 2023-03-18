#include "sensor.hpp"


sensor::sensor(int type, int in, int out = -1){
    portIn = in;
    portOut = out;
    this->type = type;
    pinMode(portIn, INPUT);
    if(out != -1)
        pinMode(portOut, OUTPUT);
}


long sensor::getD(){
    if(type == 1){
        digitalWrite(portOut, LOW);
        delayMicroseconds(5);
        digitalWrite(portOut, HIGH);
        delayMicroseconds(10);
        digitalWrite(portOut, LOW);

        long duration = pulseIn(portIn, HIGH);
        long cm = (duration/2) / 29.1;
        return cm;
    } else if (type == 0){
        int sensorStatus = analogRead(portIn);
        return sensorStatus;
    }
}

int sensor::getD_d(){
    int sensorStatus = digitalRead(portIn);
    return sensorStatus;
    // if (sensorStatus == 1) // Check if the pin high or not
    // {
    //     // if the pin is high turn off the onboard Led
    //     // Serial.print(sensorStatus);
    //     Serial.println("Motion Detected!"); // print Motion Detected! on the serial monitor window
    // }
    // else  {
    //     //else turn on the onboard LED
    //     // Serial.print(sensorStatus);
    //     Serial.println("Motion Ended!"); // print Motion Ended! on the serial monitor window
    // }
}
