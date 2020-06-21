#ifndef COMMON_H
#define COMMON_H

class battery
{

public:
    double batteryVoltage[4];
    double lastBatteryVoltage[4];
    unsigned long lastRun = 0;

    int batteryPin[4] = {21, 20, 17, 16};
    double batteryOffset[4] = {
        calcOffset(1015000.0, 940000.0),
        calcOffset(1008000.0, 508000.0),
        calcOffset(998000.0, 300000.0),
        calcOffset(995000.0, 239000.0),
    };

    battery(/* args */);
    void runState();

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

    void batteryStatus(int bat, int yOffset, char *buff);
    void setBatteryLevel(int bat, int percentage);
    void drawBattery(int bat);
};

struct objStoreStruct
{
    battery battery;
    graphics graphics;
};

#endif