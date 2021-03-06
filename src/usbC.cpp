#include <Wire.h>
#include "tcpm_driver.h"
#include "usb_pd.h"
#include <Arduino.h>
#include "common.h"
const int PROGMEM usb_pd_int_pin = 37;

extern usb_pd_ob usb_pd_ob1[CONFIG_USB_PD_PORT_COUNT];

int debug_led_current_state = 0;

int pd_source_cap_current_index = 0;

char str[30];

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
    {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},
};

int last_time = 0;
int dsp_interval = 0;
int voltage[] = {5000, 8000, 1200, 15000, 19000};
int selected = 0;

usbC::usbC()
{
  return;
  pinMode(usb_pd_int_pin, INPUT_PULLUP);

  Wire.begin();
  Wire.setClock(100000);

  pd_set_max_voltage(voltage[0]);

  tcpm_init(0);
  delay(50);
  pd_init(0);
  delay(50);
}

void usbC::runState()
{
  int reset = 0;

 

    // memset(str, ' ', 30);
    // sprintf(str, "Voltage: %lu Current: %lu\n\n", usb_pd_ob1[0].supply_voltage, usb_pd_ob1[0].max_ma);
    // usb_serial_write(str, 30);
    // dsp_interval = millis();

    // memset(str, ' ', 30);
    // sprintf(str, "Mode: %d\n\n",usb_pd_ob1[0].mode);
    // usb_serial_write(str, 30);

    // memset(str, ' ', 30);
    // sprintf(str, "Current state: %d\n\n", getLastState(0));
    // usb_serial_write(str, 30);
  


  if (LOW == digitalRead(usb_pd_int_pin))
  {
    tcpc_alert(0);
  }

  pd_run_state_machine(0, reset);

  




}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps)
{

  //digitalWrite(debug_led_pin, HIGH);
  usb_pd_ob1[port].src_cap_curr = cnt - 1;
  usb_pd_ob1[port].src_cap_cnt = cnt;
  usb_pd_ob1[port].src_caps = src_caps;

  uint32_t mv;
  uint32_t ma;

  pd_extract_pdo_power(*(src_caps + cnt - 1), &ma, &mv);

  pd_set_max_voltage(20000);
  

}
