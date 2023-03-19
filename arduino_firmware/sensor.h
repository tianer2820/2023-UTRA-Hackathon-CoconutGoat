#if !defined(SENSOR)
#define SENSOR

#include "chassis.h"

#define THRES_INF 850
#define THRES_CM 2

class Sensor{
  public:
    int portIn;
    int portOut;
    int type; // 0 -- infrared; 1 -- ultrasonic

    Sensor(int type, int in, int out = -1){
      portIn = in;
      portOut = out;
      this->type = type;
      pinMode(portIn, INPUT);
      if(out != -1)
          pinMode(portOut, OUTPUT);
    }
    /* return distance in centimeters for ultrasonic, return analog output for infrared */
    long getD(){
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

    /* return digital output for infrared */
    int getD_d(){
      int sensorStatus = digitalRead(portIn);
      return sensorStatus;
    }

    /* return 0 if no collision, return 1 if detected collision */
    int detectCollision(){
      if(type){
        if(getD() < THRES_CM) return 1;
        else return 0;
      } else if(!type){
          if(getD() < THRES_INF) return 1;
          else return 0;
      }
    }
};



void goAroundLeft(Car* car){
    car->drive(0,0);
    leftTurn(90);
    while(sensors[2].detectCollision){
      car->drive(0.2,0);
    }
    car->drive(0,0);
    rightTurn(90);
    car->drive(0.2,0);
    delay(1000);
    
}

void goAroundRight(Car* car){
  
}


#endif
