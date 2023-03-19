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


Car *car = NULL;
SoftwareSerial *bluetooth = NULL;
DigitalCounter* counters[2];

// gyro 
#include "GY521.h"
GY521 sensor(0x68);

uint8_t analog_sensors[ANALOG_SENSOR_N];

int led = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    car = new Car(M_PORT1, M_PORT2, M_PORT3, M_PORT4);
    bluetooth = new SoftwareSerial(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX
    bluetooth->begin(115200);

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
    // float pitch = sensor.getPitch();
    // float roll = sensor.getRoll();
    float yaw = sensor.getYaw() - drift;
    bluetooth->print("GYR ");
    bluetooth->print(String(yaw, 2));
    bluetooth->print("\n");

    #ifdef DEBUG
    Serial.println(yaw);
    #endif


    // Analog sensors
    // for (int i = 0; i < ANALOG_SENSOR_N; i++)
    // {
    //     int val = analogRead(analog_sensors[i]);
    //     float percent = (float)val / 1023.0;
    //     bluetooth->print("ANA ");
    //     bluetooth->print(String(i));
    //     bluetooth->print(" ");
    //     bluetooth->print(String(percent, 2));
    //     bluetooth->print("\n");
    // }


    // Digital counters
    counters[0]->update();
    counters[1]->update();
    if(counters[0]->is_changed()){
        bluetooth->print("CNT 0 ");
        bluetooth->print(String(counters[0]->get_count()));
        bluetooth->print("\n");
        counters[0]->clear_changed();
    }
    if(counters[1]->is_changed()){
        bluetooth->print("CNT 1 ");
        bluetooth->print(String(counters[1]->get_count()));
        bluetooth->print("\n");
        counters[1]->clear_changed();
    }



    // read command
    String line;
    if (bluetooth->available() > 0)
    {
        // PARSE COMMAND

        line = bluetooth->readStringUntil('\n');

        #ifdef DEBUG
        Serial.println("Line received:");
        Serial.println(line);
        Serial.print("length = ");
        Serial.print(line.length());
        #endif

        String args[2];
        int idx = line.indexOf(' ');
        if(idx >= 0 && idx < line.length()){
            String forward = line.substring(0, idx);
            String turn = line.substring(idx+1);

            float forward_f = forward.toFloat();
            float turn_f = turn.toFloat();
            car->drive(forward_f, turn_f);
        }
    }
}
