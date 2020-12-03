/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Arduino.h>
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "common.h"
#include <WString.h>
#include "usb_pd.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 36
#define TFT_CS 34
#define TFT_PWR 33
#define TFT_RST 35
#define batteryWidth 60
#define batteryHeight 30

int batteryY = 5;
int batteryPercentY = 20;
boolean insideMenu = false;
extern objStoreStruct objStore;
extern struct usb_pd_ob usb_pd_ob1[CONFIG_USB_PD_PORT_COUNT];
uint16_t batteryColor = ILI9341_WHITE;
int pos = 0;
int menuId = 0;

enum spaces
{
  padding = 10,
  divider1 = 55,
  divider2 = divider1 + 23 * 3 + padding,

};

enum powerOffset
{
  power,
  flow,
  time
};

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// If using the breakout, change pins as desired
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

graphics::graphics()
{
  tft.begin();
  tft.setRotation(1);

  tft.fillScreen(ILI9341_BLACK);
  yield();

  tft.fillScreen(ILI9341_BLACK);

  batteryColor = ILI9341_WHITE;

  this->drawBattery(0);
  this->drawBattery(1);
  this->drawBattery(2);
  this->drawBattery(3);

  tft.drawLine(0, divider1, 240, divider1, ILI9341_WHITE);
  this->setPowerLevel(power, "Power", padding);
  this->setPowerLevel(flow, "Flow", padding);
  this->setPowerLevel(time, "Time", padding);

  tft.drawLine(0, divider2, 240, divider2, ILI9341_WHITE);
  tft.setCursor(padding, divider2 + padding);
  tft.print("USB C-PD");

  this->setPowerLevel(4, "Power", padding, 8);

  tft.setSPISpeed(90 * 1000000);
  digitalWrite(TFT_PWR, HIGH);
}

enum BatteryStatus
{
  voltage,
};

void graphics::setPowerLevel(int offsetY, char *value, int x, int y)
{
  // char str[8];
  // memset(str, 0, 8);
  // memset(str, 'A', 7);

  // sprintf(str, "%d/%dw", objStore.battery.remainingPower, 600);
  tft.setCursor(x, offsetY * 23 + y + divider1 + padding);
  tft.print(value);
}
void graphics::runState()
{
  //enter menu
  if (this->enterStatus || this->knobStatus != 0)
  {
    pos += knobStatus / 4;
    Serial.printf("POS: %d\n", pos);
    this->knobStatus = 0;
    this->renderMenu();
  }
  //update battery

  // if (i == 0)
  //   batteryColor = ILI9341_RED;
  // else if (i == 1)
  //   batteryColor = ILI9341_BLUE;
  // else
  //   batteryColor = ILI9341_WHITE;
  char str[5];
  memset(str, 0, 5);
  memset(str, ' ', 4);
  double battLevel = objStore.battery.batteryVoltage[3] / 16.8 * 100;
  int offset = battLevel > 99 ? 0 : battLevel > 9 ? 1 : 2;
  sprintf(str + offset, "%1.0f%%", battLevel);

  batteryStatus(str);

  // setBatteryLevel(i, objStore.battery.batteryVoltage[i] / (4.2 * (i + 1)) * 100);

  // this->setPowerLevel(power, "100/100w");
  // this->setPowerLevel(flow, "-50w");
  // this->setPowerLevel(time, "20h");

  // tft.setCursor(padding + 120, divider2 + padding);
  // tft.setTextSize(2);
  // if (usb_pd_ob1[0].mode == 2)
  //   this->setPowerLevel(3, "SOURCE ", 125, 8);
  // else if (usb_pd_ob1[0].mode == 1)
  //   this->setPowerLevel(3, "SINK   ", 125, 8);
  // else if (usb_pd_ob1[0].mode == 0)
  //   this->setPowerLevel(3, "Removed", 125, 8);

  // char str[13];
  // memset(str, 0, 13);
  // memset(str, ' ', 12);
  // sprintf(str, "%.0fv/%.1fa", usb_pd_ob1[0].supply_voltage / 1000., usb_pd_ob1[0].max_ma / 1000.);

  // this->setPowerLevel(4, str, 125, 8);

  // tft.setTextSize(1);
}

void graphics::renderMenu()
{

  if (insideMenu && this->enterStatus)
  {
    if (menuId == 0)
      tft.fillRect(0, 0, this->battx, 240, ILI9341_BLACK);
    //go into new menu
    switch (pos)
    { //dc output

    case 0:
    case 1:
      int16_t x, y;
      uint16_t w, h;

      tft.setFont(&FreeSansBold12pt7b);
      tft.setCursor(10, 25);

      tft.println("Voltage");

      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(10, 55);

      tft.getTextBounds("12v", 10, 55, &x, &y, &w, &h);
      tft.fillRect(x-5,y-5,w+10,h+10,ILI9341_DARKGREY);

      tft.println("12v");

      tft.setFont(&FreeSansBold12pt7b);
      tft.setCursor(10, 100);
      tft.println("Current");

      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(10, 130);
      tft.println("12a");

      tft.setFont(&FreeSansBold12pt7b);
      tft.setCursor(10, 200);
      tft.println("Start");

      tft.setCursor(100, 200);
      tft.println("Cancel");

      break;
    }
  }
  else
  {
    String menuOptions[] = {
        "Set DC Output",
        "Set DC Input",
        "Set Max Battery",
        "Set Battery",
        "Set Battery",
        "Go Back",
    };
    int len = sizeof(menuOptions) / sizeof(menuOptions[0]);

    if (pos < 0)
      pos = len - 1;
    else if (pos >= len)
      pos = 0;

    tft.fillRect(0, 0, this->battx, 240, ILI9341_BLACK);
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextSize(1);
    int i;
    for (i = 0; i < len; i++)
    {

      tft.setCursor(15, 35 * i + this->battTh + 24);
      if (pos == i)
      {
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.fillRect(10, 35 * i + this->battTh, this->battx - 35, 36, ILI9341_WHITE);
      }
      else
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

      tft.println(menuOptions[i]);
      tft.drawLine(10, 35 * i + this->battTh, this->battx - 25, 35 * i + this->battTh, ILI9341_WHITE);
    }

    tft.drawLine(10, 35 * i + this->battTh, this->battx - 25, 35 * i + this->battTh, ILI9341_WHITE);
  }

  this->enterStatus = 0;
  insideMenu = true;
}
void graphics::batteryStatus(char *buff)
{
  tft.setFont(&FreeSans12pt7b);
  tft.setCursor(battx + 7, batty + batth + battTh + 10);
  tft.setTextColor(batteryColor, ILI9341_BLACK);
  tft.fillRect(battx, batty + batth, 60, 45, ILI9341_BLACK);
  tft.print(buff);
}

void graphics::drawBattery(int bat)
{
  tft.drawRoundRect(battx, batty, battw, batth, 5, ILI9341_WHITE);
  tft.drawRoundRect(battTx, 0, battTw, battTh, 3, ILI9341_WHITE);
}

void graphics::setBatteryLevel(int bat, int percentage)
{

  tft.fillRect(bat * batteryWidth + 5,
               batteryY + 5,
               (batteryWidth - 20) * (percentage / 100.0),
               batteryHeight - 10,
               batteryColor);
}
