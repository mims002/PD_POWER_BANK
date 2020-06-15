#include <Wire.h>
#include "tcpm_driver.h"
#include "usb_pd.h"
#include <Arduino.h>

const int PROGMEM usb_pd_int_pin = 20;
const int PROGMEM debug_led_pin = 13;
const int PROGMEM pushbutton_pin = 2;
int pushbutton_last_state, pushbutton_current_state, pushbutton_last_time, pushbutton_current_time;
int debug_led_current_state = 0;
int pd_source_cap_current_index = 0, pd_source_cap_max_index = 0;

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
    {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},
};
// USB-C Specific - TCPM end 1

void setup()
{
  pinMode(usb_pd_int_pin, INPUT_PULLUP);
  pinMode(debug_led_pin, OUTPUT);
  pinMode(pushbutton_pin, INPUT_PULLUP);
  pushbutton_last_state = 1;
  pushbutton_last_time = 0;
  digitalWrite(debug_led_pin, LOW);

  Wire.begin();
  Wire.setClock(100000);

  tcpm_init(0);
  delay(50);
  pd_init(0);
  delay(50);
}

void loop()
{
  int reset = 0;
  pushbutton_current_state = digitalRead(pushbutton_pin);
  pushbutton_current_time = millis();

  if (((pushbutton_current_time - pushbutton_last_time) > 5000) && getLastState(0) <= 8 && *tcpc_config[0].drv->stored_vbus ==1)
  {
    if (debug_led_current_state)
    {
      digitalWrite(debug_led_pin, LOW);
      debug_led_current_state = 0;
    }
    else
    {
      digitalWrite(debug_led_pin, HIGH);
      debug_led_current_state = 1;
    }
    if (pd_source_cap_current_index < pd_source_cap_max_index)
    {
      pd_source_cap_current_index++;
    }
    else
    {
      pd_source_cap_current_index = 0;
    }
    //pd[0].task_state = PD_STATE_SOFT_RESET);
    reset = 1;

    pushbutton_last_time = pushbutton_current_time;
  }
  pushbutton_last_state = pushbutton_current_state;

  if (LOW == digitalRead(usb_pd_int_pin))
  {
    tcpc_alert(0);

  }

  // /* V BUS  */
  // char str[35];
  // memset(str, ' ', 35);
  // sprintf(str, "\n\n\nConnected Vbus: %d\n\n\n", *tcpc_config[0].drv->stored_vbus);
  // usb_serial_write(str, 35);

  pd_run_state_machine(0, reset);

  // For some reason, a delay of 4 ms seems to be best
  // My guess is that spamming the I2C bus too fast causes problems

  delay(4);
}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps)
{

  //digitalWrite(debug_led_pin, HIGH);
  pd_source_cap_max_index = cnt - 1;

  uint32_t mv;
  uint32_t ma;
  uint32_t pdo;

  pd_find_pdo_index(port, PD_MAX_VOLTAGE_MV, &pdo);
  pd_extract_pdo_power(pdo, &ma, &mv);

  char str[50];
  memset(str, ' ', 50);
  sprintf(str, "Source Power %lu : %lu Type: %lu\n\n", mv, ma, ((*(src_caps+cnt-1) >> 10) & 0x3FF) *50);
  usb_serial_write(str, 50);

  
}
