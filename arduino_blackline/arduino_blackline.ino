#include <SoftwareSerial.h>

#include "chassis.h"
#include "digital_counter.h"

// debug flag
// #define DEBUG

// motor ports
#define M_PORT1 5
#define M_PORT2 6
#define M_PORT3 9
#define M_PORT4 10

// bluetooth ports
#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3

#define ANALOG_SENSOR_0 A0
#define ANALOG_SENSOR_1 A1
#define ANALOG_SENSOR_2 A2
#define ANALOG_SENSOR_3 A3
#define ANALOG_SENSOR_4 A6
#define ANALOG_SENSOR_5 A7
#define ANALOG_SENSOR_N 6

// wheel encoder
#define LEFT_ENCODER 12
#define RIGHT_ENCODER 11

// front color
#define FRONT_COLOR 4
#define LEFT_COLOR 7


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


void setup()
{
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

    // wheel encoders
    counters[0] = new DigitalCounter(LEFT_ENCODER);
    counters[1] = new DigitalCounter(RIGHT_ENCODER);
    
}


unsigned long last_time = millis();
double drift = 0;
float angle = 0;
void loop()
{
    digitalWrite(LED_BUILTIN, led);
    led = !led;

    // drift for gyro
    unsigned long new_time = millis();
    unsigned long delta = new_time - last_time;
    last_time = new_time;
    drift += 0.5 / (10 * 1000) * delta;

    // gyro
    sensor.read();
    float yaw = sensor.getYaw() - drift;


    #ifdef DEBUG
    Serial.println(yaw);
    #endif


    // Analog sensors
    // for (int i = 0; i < ANALOG_SENSOR_N; i++)
    // {
    //     int val = analogRead(analog_sensors[i]);
    //     float percent = (float)val / 1023.0;
    // }


    // Digital counters
    counters[0]->update();
    counters[1]->update();



    // black line
    if(digitalRead(LEFT_COLOR)){
        car->drive(0, 0.3);
    } else if (digitalRead(FRONT_COLOR)){
        car->drive(0, -0.3);
    } else {
        car->drive(-0.3, 0);
    }

}
