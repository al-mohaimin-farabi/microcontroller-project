#include <RH_ASK.h>
#include <SPI.h>

#define S_VRX_PIN A0
#define B_VRX_PIN A1
#define B_VRY_PIN A2
#define SPRAY_SWITCH_PIN 2

RH_ASK rf_driver;


int boat_Joystick_x = 511;
int boat_Joystick_y = 511;
int spray_Joystick_x = 511;
int sprayPosition = 89;
int sprayState = 1;

void setup() {
  rf_driver.init();
  pinMode(SPRAY_SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  sprayState = digitalRead(SPRAY_SWITCH_PIN);

  boat_Joystick_x = analogRead(B_VRX_PIN);
  boat_Joystick_y = analogRead(B_VRY_PIN);
  spray_Joystick_x = analogRead(S_VRX_PIN);

  sprayPosition = map(spray_Joystick_x, 0, 1023, 0, 180);

  uint8_t buf[4];

  buf[0] = sprayPosition;
  buf[1] = sprayState;
  buf[2] = boat_Joystick_x / 4;
  buf[3] = boat_Joystick_y / 4;


  rf_driver.send(buf, sizeof(buf));
  rf_driver.waitPacketSent();
}