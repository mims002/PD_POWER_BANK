#include "common.h"
#include <Arduino.h>
#include <SPI.h>

extern objStoreStruct objStore;

int modeCharge = HIGH;
int modeDischarge = LOW;
int on = LOW;
int off = HIGH;

double dcVoltage;
double usbVoltage;

int dcR = 0;
int usbR = 0;

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

void charger::setUsbV(double v)
{
    double v_read = readV(usbCVoltReadPin, vOffsetUsb,1);
    if(v_read<4) return; //do not run if battery is not connected 

    while (v_read < v)
    {
        if (usbR >= 400)
            break;
        Serial.println(v_read);

        v_read = readV(usbCVoltReadPin, vOffsetUsb,1);

        this->setR(dpot_usbc_cs, ++usbR);
    }

    while (v_read > v)
    {
        if (usbR < 0)
            break;
        Serial.println(v_read);

        v_read = readV(dcVoltReadPin, vOffsetUsb,1);

        this->setR(dpot_usbc_cs, --usbR);
    }
}

double charger::readV(int pin, double ratio, double offset)
{
    int read = analogRead(pin);
    return (read * (3.27 / 4096.0) * ratio + offset);
}

void charger::runState()
{
    Serial.print("Voltage: ");
    Serial.print(this->readV(usbCVoltReadPin,vOffsetUsb,1.6));
    Serial.print(" Current: ");
    Serial.println(this->readCurrent(bat_cur));
    delay(500);
}

double charger::readCurrent(int pin)
{    
    int read = analogRead(pin);
    double v =  this->readV(pin,1.5,-.01);
    v = v - 2.5 +0;
    v = v/.1;
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
