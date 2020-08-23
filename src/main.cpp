
#include <Arduino.h>
#include "common.h"
#include "usbC.h"
#include <SPI.h>

objStoreStruct objStore;
usb_pd_ob usb_pd_ob1[CONFIG_USB_PD_PORT_COUNT];

//pin to disable usb c output activates usb c input
unsigned int usbcPowerPin = 35;
//pin to disable dc output activates dc input
unsigned int dcPowerPin = 0;

void setup()
{
    Serial.begin(9600);

    //set up the display
    //objStore.graphics = graphics();

    //set up the battery
    objStore.battery = battery();

    pinMode(usbcPowerPin, OUTPUT);
    analogReadResolution(12);
    analogReadAveraging(8);
}
unsigned long time = 0;
boolean toggle = 0;
void loop()
{

    // if (millis() - objStore.graphics.lastRun > 700)
    // {
    //     //run each state
    //     objStore.graphics.runState();
    //     objStore.graphics.lastRun = millis();
    // }

    objStore.charger.runState();
    // if (millis() - objStore.battery.lastRun > 500)
    // {
    //     //run each state
    //     objStore.battery.runState();
    //     objStore.battery.lastRun = millis();
    // }

    // if (millis() - objStore.usbC.lastRun > 1)
    // {
    //     //run each state
    //     objStore.usbC.runState();
    //     objStore.usbC.lastRun = millis();
    // }

    // toggle = !toggle;
    // objStore.charger.runState();
}