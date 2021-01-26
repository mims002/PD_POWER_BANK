#ifndef COMMON_H
#define COMMON_H

#include "usb_pd.h"

class battery
{

public:
    double batteryVoltage[4];
    double lastBatteryVoltage[4];
    unsigned long lastRun = 0;
    int remainingPower = 50;
    int enableBatt = 28;

    int batteryPin[4] = {20, 21, 22, 23};
    double batteryOffset[4] = {
        calcOffset(100000., 20000),
        calcOffset(100000., 27000),
        calcOffset(100000., 47000),
        calcOffset(100000., 200000),
    };

    battery(/* args */);
    void runState();
    int chargeLevel(); //0 stop charger, 1 increase, -1 decrease

    double getBatteryVoltage();

    double calcOffset(double R1, double R2)
    {
        return ((R1 + R2) / R2);
    }
};

class graphics
{

public:
    graphics(/* args */);
    void runState();
    unsigned long lastRun = 0;
    //240x320
    int bottomText = 30;
    int battTh = 10;
    int battTw = 40;
    int padding = 10;
    int battw = 60;
    int batth = 240 - bottomText - battTh;
    int battx = 320 - padding - battw;

    int batty = 0 + battTh;
    int battTx = battx + battw / 2 - battTw / 2;

    int enterStatus = 0;
    int knobStatus = 0;

    void batteryStatus(char *buff);
    void setBatteryLevel(int bat, int percentage);
    void drawBattery(int bat);
    void getVoltCurrent(char *title, float *v, float *c);
    void renderMenu();
    void graphics::set(char *title, float *v, float *c,  int i);
    void graphics::router(int i);
    void setPowerLevel(int offsetY, char *value, int x = 125, int y = 0);
    
};

class usbC
{
public:
    int source;
    unsigned long lastRun = 0;
    usbC();
    void runState();
};

class charger
{
public:
    const int dpot_usbc_cs = 37;
    const int dpot_dc_cs = 38;

    const int con_bat = 0;
    const int con_usbc_input = 0;
    const int dc_swt = 26;
    const int usb_swt = 27;

    const int dc_status_pin = 29;
    const int usb_status_pin = 30;
    const int bat_cur_pin = 16;
    const int usb_cur_pin = 16;
    const int dc_cur_pin = 16;

    void runState();

    int dcVoltReadPin = 39;
    int usbCVoltReadPin = 40;

    float dcV = 0;
    float dcC = 0;

    double vOffsetDC = calcOffset(100400., 14940.);
    double vOffsetUsb = calcOffset(100000., 15000.);

    double calcOffset(double R1, double R2)
    {
        return ((R1 + R2) / R2);
    }

    charger();
    void test(int cs);
    void setV(int readVpin, int readCpin, int fPin, double v, double cur);
    void setR(int pin, int r);
    double readV(int pin, double ratio, double offset);
    void digitalPotWrite(int cs, int address, int value);
    double charger::readCurrent(int pin);
    void charger::checkUsb();
    void charger::checkDc();
    void charger::sinkBatt();
    void charger::startDCIn();
    void charger::startDCOut();

};

struct objStoreStruct
{
    charger charger;
    battery battery;
    graphics graphics;
    usbC usbC;
};

#endif