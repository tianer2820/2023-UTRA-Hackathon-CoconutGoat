//
//    FILE: GY521_pitch_roll_yaw.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo pitch roll yaw
//    DATE: 2020-08-06

// Use this lib: https://github.com/RobTillaart/GY521/releases/tag/0.3.9

#include "GY521.h"

GY521 sensor(0x68);

uint32_t counter = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println(__FILE__);
    Serial.print("GY521_LIB_VERSION: ");
    Serial.println(GY521_LIB_VERSION);

    Wire.begin();

    delay(100);
    while (sensor.wakeup() == false)
    {
        Serial.print(millis());
        Serial.println("\tCould not connect to GY521");
        delay(1000);
    }
    sensor.setAccelSensitivity(2); // 8g
    sensor.setGyroSensitivity(1);  // 500 degrees/s

    sensor.setThrottle();
    Serial.println("start...");

    // set calibration values from calibration sketch.
    sensor.axe = -0.0457861;
    sensor.aye = 0.0148047;
    sensor.aze = -1.0180640;
    sensor.gxe = -0.7887023;
    sensor.gye = -2.2706871;
    sensor.aze = -0.0302290;


    // second calibration

    // sensor.read();
    // float pitch1 = sensor.getPitch();
    // float roll1 = sensor.getRoll();
    // float yaw1 = sensor.getYaw();
    // delay(2000);
    // sensor.read();
    // float pitch2 = sensor.getPitch();
    // float roll2 = sensor.getRoll();
    // float yaw2 = sensor.getYaw();

    // Serial.println("2 sec drift: ");
    // Serial.println(yaw2 - yaw1);

    // delay(1000);

}

unsigned long last_time = millis();
double drift = 0;
void loop()
{
    unsigned long new_time = millis();
    unsigned long delta = new_time - last_time;
    last_time = new_time;
    drift += 0.7 / (10 * 1000) * delta;

    sensor.read();
    float pitch = sensor.getPitch();
    float roll = sensor.getRoll();
    float yaw = sensor.getYaw() - drift;

    if (counter % 10 == 0)
    {
        Serial.println("\nCNT\tPITCH\tROLL\tYAW");
    }

    Serial.print(counter);
    Serial.print('\t');
    Serial.print(pitch, 3);
    Serial.print('\t');
    Serial.print(roll, 3);
    Serial.print('\t');
    Serial.print(yaw, 3);
    Serial.println();

    counter++;
}

// -- END OF FILE --
