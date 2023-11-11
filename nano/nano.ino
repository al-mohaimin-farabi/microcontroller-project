#include <Wire.h>

#define S_VRX_PIN A0
#define B_VRX_PIN A1
#define B_VRY_PIN A2
#define SPRAY_SWITCH_PIN 2

int boat_Joystick_x = 511;
int boat_Joystick_y = 511;
int spray_Joystick_x = 511;
int sprayPosition = 89;
int sprayState = 1;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(SPRAY_SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  sprayState = digitalRead(SPRAY_SWITCH_PIN);

  boat_Joystick_x = analogRead(B_VRX_PIN);
  boat_Joystick_y = analogRead(B_VRY_PIN);
  spray_Joystick_x = analogRead(S_VRX_PIN);

  sprayPosition = map(spray_Joystick_x, 0, 1023, 0, 180);

  Wire.beginTransmission(8);
  Wire.write(boat_Joystick_x / 4);
  Wire.write(boat_Joystick_y / 4);
  Wire.write(sprayPosition);
  Wire.write(sprayState);
  // Serial.println(boat_Joystick_y);
  Wire.endTransmission();
}
