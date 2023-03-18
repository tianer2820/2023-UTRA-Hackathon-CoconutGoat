#include <SoftwareSerial.h>

#include "chassis.h"

// motor ports
#define M_PORT1 5
#define M_PORT2 6
#define M_PORT3 9
#define M_PORT4 10

// bluetooth ports
#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 3

Car *car = NULL;
SoftwareSerial *bluetooth = NULL;

void setup()
{
    car = new Car(M_PORT1, M_PORT2, M_PORT3, M_PORT4);
    bluetooth = new SoftwareSerial(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX
    bluetooth->begin(9600);
    Serial.begin(9600);
}

void loop()
{
    delay(5);

    // read line
    String line;
    if (bluetooth->available() > 0)
    {
        // PARSE COMMAND

        line = bluetooth->readStringUntil('\n');

        Serial.println("Line received:");
        Serial.println(line);
        Serial.print("length = ");
        Serial.print(line.length());

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

        Serial.print("num of args: ");
        Serial.println(arg_len);

        // get command
        String command = args[0];

        Serial.print("command is: ");
        Serial.println(command);

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
