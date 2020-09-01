
#include <Arduino.h>
#include "common.h"
#include "usbC.h"
#include <SPI.h>
#include <Encoder.h>

objStoreStruct objStore;
usb_pd_ob usb_pd_ob1[CONFIG_USB_PD_PORT_COUNT];


//pin to disable usb c output activates usb c input
unsigned int usbcPowerPin = 35;
//pin to disable dc output activates dc input
unsigned int dcPowerPin = 0;

int leftPin = 9;
int rightPin = 8;
int enterPin = 7;

Encoder knob(leftPin,rightPin);
long knobPosition=0;

int enterStatus=0;

void updateLeft();

void setup()
{
    Serial.begin(9600);

    // //set up the display
    // objStore.graphics = graphics();

    // //set up the battery
    // objStore.battery = battery();

    pinMode(usbcPowerPin, OUTPUT);
    // analogReadResolution(12);
    // analogReadAveraging(8);
    pinMode(leftPin, INPUT_PULLUP);
    pinMode(rightPin, INPUT_PULLUP);
    pinMode(enterPin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(enterPin),updateLeft, RISING);
}

void updateLeft(){
    objStore.graphics.enterStatus=1;
    Serial.println(digitalRead(enterPin));
}

unsigned long time = 0;
boolean toggle = 0;
void loop()
{
    
    long newPos = knob.read();

    if (abs(newPos)>=4 ){
        objStore.graphics.knobStatus = newPos;
        Serial.println(knobPosition);
        knob.write(0);
        // Serial.printf("Right: %d\nLeft:%d\nPush:%d\n\n", digitalRead(rightPin), digitalRead(leftPin), digitalRead(enterPin));
        // leftStatus=0;
    }
    if (millis() - objStore.graphics.lastRun > 10)
    {
        //run each state
        objStore.graphics.runState();
        objStore.graphics.lastRun = millis();
    }

    // objStore.charger.runState();
    if (millis() - objStore.battery.lastRun > 10)
    {
        //run each state
        objStore.battery.runState();
        objStore.battery.lastRun = millis();
    }

    // if (millis() - objStore.usbC.lastRun > 1)
    // {
    //     //run each state
    //     objStore.usbC.runState();
    //     objStore.usbC.lastRun = millis();
    // }

    // toggle = !toggle;
    // objStore.charger.runState();
}