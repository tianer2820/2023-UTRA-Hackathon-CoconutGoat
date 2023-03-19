#include <SoftwareSerial.h>

#include "chassis.h"

// debug flag
#define DEBUG

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


Car *car = NULL;
SoftwareSerial *bluetooth = NULL;

// gyro 
#include "GY521.h"
GY521 sensor(0x68);

uint8_t analog_sensors[ANALOG_SENSOR_N];

void setup()
{
    car = new Car(M_PORT1, M_PORT2, M_PORT3, M_PORT4);
    bluetooth = new SoftwareSerial(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX
    bluetooth->begin(9600);

    #ifdef DEBUG
    Serial.begin(9600);
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
void loop()
{
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
    for (int i = 0; i < ANALOG_SENSOR_N; i++)
    {
        int val = analogRead(analog_sensors[i]);
        float percent = (float)val / 1023.0;
        bluetooth->print("ANA ");
        bluetooth->print(String(i));
        bluetooth->print(" ");
        bluetooth->print(String(percent, 2));
        bluetooth->print("\n");
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

        String args[16];
        int arg_len = 0;
        int cursor = 0;
        int line_len = line.length();
        while (1)
        {
            int idx = line.indexOf(' ', cursor);
            if (idx < 0)
            {
                // no more spaces, copy rest of the string
                args[arg_len] = line.substring(cursor);
                arg_len += 1;
                break;
            }
            String arg = line.substring(cursor, idx);
            args[arg_len] = arg;
            arg_len += 1;
            cursor = idx + 1;
            if (cursor >= line_len)
            {
                break;
            }
        }

        #ifdef DEBUG
        Serial.print("num of args: ");
        Serial.println(arg_len);
        #endif

        // get command
        String command = args[0];

        #ifdef DEBUG
        Serial.print("command is: ");
        Serial.println(command);
        #endif

        if (command.equals("DRV"))
        {
            // need at lease 2 args
            if (arg_len >= 3)
            {
                float forward = args[1].toFloat();
                float turn = args[2].toFloat();
                car->drive(forward, turn);
            }
        }
        else
        {
        }
    }
}
