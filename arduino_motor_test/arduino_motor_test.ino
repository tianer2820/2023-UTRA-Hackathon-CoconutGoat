#include "chassis.h"

#define M1 10
#define M2 9
#define M3 6
#define M4 5


void setup(){
    pinMode(13, OUTPUT);



    

}
Car car(M1, M2, M3, M4);

float speed = 0;
float speed_inc = 0.05;

void loop(){
   speed += speed_inc;

    if(speed > 1){
        speed_inc = -0.05;
    } else if (speed < -1){
        speed_inc = 0.05;
    }

    car.drive(speed, 0);
    // car.drive(-1, 0);
    delay(50);
}