#include "common.h"
#include <Arduino.h>


enum batteryState{
    source,
    sink,
    off
};
battery::battery()
{

    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        pinMode(batteryPin[i], INPUT);
    }
    pinMode(enableBatt, OUTPUT);
}

double battery::getBatteryVoltage()
{
    digitalWrite(enableBatt, HIGH);

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

int battery::chargeLevel(){
    
}
