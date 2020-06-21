#include "common.h"
#include <Arduino.h>

battery::battery()
{

    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        pinMode(batteryPin[i], INPUT);
    }
}

double battery::getBatteryVoltage()
{
    analogReadAveraging(10);
    analogReadResolution(12);
    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        // lastBatteryVoltage[i] = batteryVoltage[i];
        int read = analogRead(batteryPin[i]);
        batteryVoltage[i] = (read * (3.26 / 4095.0) * batteryOffset[i]) - 0.1;
    }
}

void battery::runState()
{
    this->getBatteryVoltage();
}