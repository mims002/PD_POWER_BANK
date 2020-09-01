#include "common.h"
#include <Arduino.h>

enum batteryState
{
    source,
    sink,
    off
};
extern objStoreStruct objStore;
double max_current = .300;
double max_voltage = 16.8;
double user_max_voltage = 16.8;

battery::battery()
{

    for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    {
        pinMode(batteryPin[i], INPUT);
    }
    pinMode(enableBatt, OUTPUT);
    batteryVoltage[3] = 0;
}

double battery::getBatteryVoltage()
{
    digitalWrite(enableBatt, HIGH);

    // for (int i = 0; i < sizeof(batteryPin) / sizeof(int); i++)
    // {
    //     // lastBatteryVoltage[i] = batteryVoltage[i];
    //     int read = analogRead(batteryPin[i]);
    //     batteryVoltage[i] = (read * (3.3 / 4095.0) * batteryOffset[i]);
    // }

    batteryVoltage[3] = ((int)(batteryVoltage[3]*10 + 2) % 169) /10.;
    digitalWrite(enableBatt, LOW);
}

void battery::runState()
{
    this->getBatteryVoltage();
}

int battery::chargeLevel()
{
    //check to see
    if (batteryVoltage[4] == user_max_voltage)
        return 0;
    if (batteryVoltage[4] < user_max_voltage && objStore.charger.readCurrent(objStore.charger.bat_cur_pin) < max_current)
        return 1;

    return -1;
}
