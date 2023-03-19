
#include <SoftwareSerial.h>
#include <NewPing.h>


#include "sensor.h"
#include "chassis.h"
#include "digital_counter.h"

// debug flag
#define DEBUG

// motor ports
#define M_PORT1 5
#define M_PORT2 6
#define M_PORT3 9
#define M_PORT4 10


#define ANALOG_SENSOR_0 A0
#define ANALOG_SENSOR_1 A1
#define ANALOG_SENSOR_2 A2
#define ANALOG_SENSOR_3 A3
#define ANALOG_SENSOR_4 A6
#define ANALOG_SENSOR_5 A7
#define ANALOG_SENSOR_N 6



// front color
#define FRONT_COLOR 4
#define LEFT_COLOR 7

// sonics
#define SONIC_RIGHT_TRIGGER 12
#define SONIC_LEFT_TRIGGER 13
#define SONIC_FRONT_TRIGGER 11
#define SONIC_LEFT_RECV 8
#define SONIC_FRONT_RECV 3
#define SONIC_RIGHT_RECV 2


Car *car = NULL;
SoftwareSerial *bluetooth = NULL;
DigitalCounter* counters[2];

// gyro 
#include "GY521.h"
GY521 sensor(0x68);

uint8_t analog_sensors[ANALOG_SENSOR_N];

int led = 0;

enum State{
    FRONT,
    LEFT,
    RIGHT,
};
int current_state = FRONT;


class MyTimer
{
private:
    unsigned long start_time;
public:
    MyTimer(){
        start_time = 0;
    }

    void reset(){
        start_time = millis();
    }
    unsigned long get(unsigned long ms){
        return ms - start_time;
    }
};



NewPing* sonic_front;
NewPing* sonic_left;
NewPing* sonic_right;

void setup()
{
    sonic_front = new NewPing(SONIC_FRONT_TRIGGER, SONIC_FRONT_RECV, 100);
    sonic_left = new NewPing(SONIC_LEFT_TRIGGER, SONIC_LEFT_RECV, 100);
    sonic_right = new NewPing(SONIC_RIGHT_TRIGGER, SONIC_RIGHT_RECV, 100);

    current_state = FRONT;
    pinMode(LED_BUILTIN, OUTPUT);

    car = new Car(M_PORT1, M_PORT2, M_PORT3, M_PORT4);

    #ifdef DEBUG
    Serial.begin(115200);
    #endif

    // gyro setup
    Wire.begin();
    delay(100);
    while (sensor.wakeup() == false)
    {
        #ifdef DEBUG
        Serial.print(millis());
        Serial.println("\tCould not connect to GY521");
        #endif
        delay(1000);
    }
    sensor.setAccelSensitivity(2); // 8g
    sensor.setGyroSensitivity(1);  // 500 degrees/s
    sensor.setThrottle();
    // set calibration values from calibration sketch.
    sensor.axe = -0.0457861;
    sensor.aye = 0.0148047;
    sensor.aze = -1.0180640;
    sensor.gxe = -0.7887023;
    sensor.gye = -2.2706871;
    sensor.aze = -0.0302290;


    // analog sensors
    analog_sensors[0] = ANALOG_SENSOR_0;
    analog_sensors[1] = ANALOG_SENSOR_1;
    analog_sensors[2] = ANALOG_SENSOR_2;
    analog_sensors[3] = ANALOG_SENSOR_3;
    analog_sensors[4] = ANALOG_SENSOR_4;
    analog_sensors[5] = ANALOG_SENSOR_5;
    for (int i = 0; i < ANALOG_SENSOR_N; i++)
    {
        pinMode(analog_sensors[i], INPUT);
    }


    
}


unsigned long last_time = millis();
double drift = 0;
float angle = 0;
void loop()
{

    MyTimer my_timer;

    MyTimer last_rotate;
    last_rotate.reset();

    sensor.read();
    drift = sensor.getYaw() - 180;

    // obstacle

    while (1)
    {

        long left = sonic_left->ping_cm();



        // drift for gyro
        unsigned long new_time = millis();
        unsigned long delta = new_time - last_time;
        last_time = new_time;
        drift += 0.7 / (10 * 1000) * delta;

        // gyro
        sensor.read();
        float yaw = sensor.getYaw() - drift;

        if(last_rotate.get(millis()) > 1500){
            // return to the original direction
            car->drive(0, 0.45);
            while (1)
            {
                sensor.read();
                if(sensor.getYaw() - drift > 160 && sensor.getYaw() - drift < 195){
                    car->drive(0, 0);
                    break;
                }
                
            }
            last_rotate.reset();
        }

        


        if(left == 0){
            left = 9999;
        }
        
        if(left >= 15){
            car->drive(-0.5, -0.05);
        }
        else {
            car->drive(0, 0);
            my_timer.reset();
            while (my_timer.get(millis()) < 500)
            {
                sensor.read();
            }
            
            car->drive(0, 0.5);
            my_timer.reset();
            while (my_timer.get(millis()) < 300)
            {
                sensor.read();
            }
            car->drive(0, 0);
            my_timer.reset();
            while (my_timer.get(millis()) < 300)
            {
                sensor.read();
            }
            int new_left = sonic_left->ping_cm();
            car->drive(0, -0.5);
            my_timer.reset();
            while (my_timer.get(millis()) < 500)
            {
                sensor.read();
            }
            car->drive(0, 0);
            int new_right = sonic_left->ping_cm();

            if(new_left == 0){
                new_left = 9999;
            }
            if(new_right == 0){
                new_right = 9999;
            }
            last_rotate.reset();
            if(new_right >= new_left){
                continue;
            } else {
                car->drive(0, 0.5);
                my_timer.reset();
                while (my_timer.get(millis()) < 500)
                {
                    sensor.read();
                }
                car->drive(0, 0);
                continue;
            }
        }
    }
    



}
