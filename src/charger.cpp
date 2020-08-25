#include "common.h"
#include <Arduino.h>
#include <SPI.h>
#include <map>

extern objStoreStruct objStore;

int modeCharge = HIGH;
int modeDischarge = LOW;
int on = LOW;
int off = HIGH;

double dcVoltage;
double usbVoltage;

int dcR = 0;
int usbR = 0;

enum status
{
    source,
    sink
};

struct BuckBoost
{
    status status = source;
    int power = off;
    double max_cur = 0;
    double v = 0;
    int cur_pin = 0;
    int swt_pin = 0;
    int dpot_cs = 0;
    int v_pin = 0;
    int r = 0;
} dc, usb;
charger::charger()
{

    pinMode(dpot_usbc_cs, OUTPUT);
    digitalWrite(dpot_usbc_cs, HIGH);

    pinMode(dpot_dc_cs, OUTPUT);
    digitalWrite(dpot_dc_cs, HIGH);

    pinMode(dpot_dc_cs, OUTPUT);
    digitalWrite(dpot_dc_cs, HIGH);

    pinMode(dc_swt, OUTPUT);
    digitalWrite(dc_swt, on);

    pinMode(usb_swt, OUTPUT);
    digitalWrite(usb_swt, on);

    pinMode(dc_mode, OUTPUT);
    digitalWrite(dc_mode, modeDischarge);

    pinMode(usb_mode, OUTPUT);
    digitalWrite(usb_mode, modeDischarge);

    pinMode(bat_cur, INPUT);

    // initialize SPI:
    SPI.begin();
}

void charger::setV(int readVpin, int readCpin, int fPin, double v, double cur)
{
    //turn off the power
    digitalWrite(fPin, off);
}

double charger::readV(int pin, double ratio, double offset)
{
    int read = analogRead(pin);
    return (read * (3.27 / 4096.0) * ratio + offset);
}

void charger::runState()
{
    if (charging)
    {
        //check voltage and current
    }
}

void charger::checkDc()
{
    if (!dc.power) //turns it off first
        digitalWrite(dc_swt, off);

    //set the correct state
    digitalWrite(dc_mode, source);

    if (dc_mode == source)
    {
        double cur = readCurrent(dc_cur_pin);
        if (cur >= dc.max_cur)
            digitalWrite(dc_swt, off);

        double v = readV(dcVoltReadPin, vOffsetDC, 0);
        if (v >= dc.v)
            digitalWrite(dc_swt, off);

        if (v < dc.v - 1)
        //increase power
    }
}

void charger::sinkBatt()
{
    if (dc.status == source && usb.status == sink)
        return; //not charging

    BuckBoost *sinked;
    if (dc.status == sink)
        sinked = &dc;
    if (usb.status == sink && (dc.v * dc.max_cur) < (usb.v * usb.max_cur))
        sinked = &usb;

    //turn on sinked
    //check volage and current
    int chargeLevel = objStore.battery.chargeLevel();
    if (chargeLevel == 0) //turn on or off batt
        digitalWrite(sinked->swt_pin, off);
    if (chargeLevel > 0 && (sinked->max_cur - 0.2) > this->readCurrent(sinked->cur_pin))
    {
        //increase charge Level
        setR(sinked->dpot_cs, ++(sinked->r));
    }
    while (chargeLevel = objStore.battery.chargeLevel() < 0 || (sinked->max_cur - 0.2) < this->readCurrent(sinked->cur_pin))
    {
        //decrease charge Level
        setR(sinked->dpot_cs, --(sinked->r));
    }

   
    //update charger current
    //balance
}

double charger::readCurrent(int pin)
{
    int read = analogRead(pin);
    double v = this->readV(pin, 1.5, -.01);
    v = v - 2.5 + 0;
    v = v / .1;
    return v;
}

void charger::setR(int pin, int r)
{
    // go through the six channels of the digital pot:
    int channel0 = 0x0;
    int channel1 = 0x1;

    this->digitalPotWrite(pin, channel0, r >= 255 ? 255 : r);
    this->digitalPotWrite(pin, channel1, r >= 255 ? r - 255 : 0);
}

void charger::digitalPotWrite(int cs, int address, int value)
{
    // gain control of the SPI port
    // and configure settings
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    // take the SS pin low to select the chip:
    digitalWrite(cs, LOW);

    unsigned int fomatted = address << 4;
    // Serial.print(fomatted, BIN);
    Serial.print("   Value: ");
    Serial.println(value);
    //  send in the address and value via SPI:
    delay(10);
    //  send in the address and value via SPI:
    SPI.transfer(address << 4);
    SPI.transfer(value);

    delay(10);

    // take the SS pin high to de-select the chip:
    digitalWrite(cs, HIGH);
    // release control of the SPI port
    SPI.endTransaction();
}
