
#include <Arduino.h>
#include "common.h"
#include "usbC.h"

objStoreStruct objStore;

void setup()
{
    Serial.begin(9600);


    //set up the display
    //objStore.graphics = graphics();

    //set up the battery
    objStore.battery = battery();
}

void loop()
{
    if (millis() - objStore.graphics.lastRun > 500)
    {
        //run each state
        objStore.graphics.runState();
        objStore.graphics.lastRun = millis();
    }

    if (millis() - objStore.battery.lastRun > 500)
    {
        //run each state
        objStore.battery.runState();
        objStore.battery.lastRun = millis();
    }
}