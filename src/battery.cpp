#include "common.h"
#include <Arduino.h>
int pinn = 29;
battery::battery()
{

    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        pinMode(batteryPin[i], INPUT);
    }
    pinMode(enableBatt, OUTPUT);
    pinMode(pinn, OUTPUT);
    digitalWrite(pinn,LOW);
    delay(500);
        digitalWrite(pinn,HIGH);

}

double battery::getBatteryVoltage()
{
    digitalWrite(enableBatt, HIGH);
    delay(50);
    // analogReadAveraging(10);
    
    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        // lastBatteryVoltage[i] = batteryVoltage[i];
        int read = analogRead(batteryPin[i]);
        batteryVoltage[i] = (read * (3.3 / 4095.0) * batteryOffset[i]);
    }


    digitalWrite(enableBatt, LOW);
}

void battery::runState()
{
    this->getBatteryVoltage();
}