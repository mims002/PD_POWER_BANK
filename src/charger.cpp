#include "common.h"
#include <Arduino.h>
#include <SPI.h>

extern objStoreStruct objStore;

charger::charger()
{
    // set the slaveSelectPin as an output:
    pinMode(dpot_bat_cs, OUTPUT);
    digitalWrite(dpot_bat_cs, HIGH);

    pinMode(dpot_usbc_cs, OUTPUT);
    digitalWrite(dpot_usbc_cs, HIGH);

    pinMode(dpot_dc_cs, OUTPUT);
    digitalWrite(dpot_dc_cs, HIGH);

    pinMode(con_dc_input, OUTPUT);
    digitalWrite(con_dc_input, LOW);

    // initialize SPI:
}

void charger::runState()
{
    //check if dc is in input mode 
    //check current draw

    //check if usbc is in input mode 


    //check if dc is in output mode 
    //check current draw

    //check if battery should be charging

    //check if battery should be in power mode



    SPI.begin();
    digitalWrite(con_dc_input, HIGH);
    digitalPotWrite(dpot_bat_cs, 1, 254);

    test(dpot_bat_cs);
}

void charger::test(int cs)
{
    // go through the six channels of the digital pot:
    int channel0 = 0x0;
    int channel1 = 0x1;

    // change the resistance on this channel from min to max:
    for (int level = 240; level < 255; level++)
    {
        digitalPotWrite(cs, channel0, level);
        delay(500);
    }
    // // change the resistance on this channel from min to max:
    // for (int level = 0; level < 255; level++)
    // {
    //     digitalPotWrite(cs, channel1, level);
    //     delay(500);
    // }

    // wait a second at the top:
    delay(100);
    // change the resistance on this channel from max to min:
    for (int level = 0; level < 255; level++)
    {
        digitalPotWrite(cs, channel0, 255 - level);
        delay(500);
    }
    // // change the resistance on this channel from max to min:
    // for (int level = 0; level < 255; level++)
    // {
    //     this->digitalPotWrite(cs, channel1, 255 - level);
    //     delay(500);
    // }
}

void charger::digitalPotWrite(int cs, int address, int value)
{
    // gain control of the SPI port
    // and configure settings
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    // take the SS pin low to select the chip:
    digitalWrite(cs, LOW);

    unsigned int fomatted = address << 4;
    Serial.print(fomatted, BIN);
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

    objStore.battery.runState();
    //run each state
    objStore.graphics.runState();
}
