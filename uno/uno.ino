#include <RH_ASK.h>
#include <SPI.h>
// #include <Servo.h>
#include "ServoTimer2.h"
#include <AFMotor.h>

#define MAX_SPEED 200
#define MAX_SPEED_OFFSET 20

ServoTimer2 spray_Angle;

AF_DCMotor sprayMotor(1);
AF_DCMotor motorLeft(3);
AF_DCMotor motorRight(4);
RH_ASK rf_driver;

int boat_Joystick_x;
int boat_Joystick_y;
int spray_Joystick_x;
int sprayPosition;
int sprayState;

void setup() {
  Serial.begin(9600);
  setDefult();
  spray_Angle.attach(10);
  spray_Angle.write(sprayPosition);
  if (!rf_driver.init()) {
    Serial.println("RF driver initialization failed");
  }
}

void loop() {
  uint8_t buf[4];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    boat_Joystick_x = buf[0] * 4;
    boat_Joystick_y = buf[1] * 4;
    sprayPosition = buf[2];
    sprayState = buf[3];
  } else {
    setDefult();
  }
  controlMotors();
  doSpray();
}

void setDefult() {
  boat_Joystick_x = 511;
  boat_Joystick_y = 511;
  sprayPosition = 90;
  sprayState = 1;
}

void controlMotors() {
  if (boat_Joystick_x >= 800) {
    moveForward();
  } else if (boat_Joystick_x <= 250) {
    moveBackward();
  } else if (boat_Joystick_y >= 800) {
    turnRight();
  } else if (boat_Joystick_y <= 250) {
    turnLeft();
  } else {
    stopMotors();
  }
}

void moveForward() {
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
  accelerateMotors();
}

void moveBackward() {
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
  accelerateMotors();
}

void turnRight() {
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
  accelerateMotors();
}

void turnLeft() {
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
  accelerateMotors();
}

void accelerateMotors() {
  for (int speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
    motorLeft.setSpeed(speedSet);
    motorRight.setSpeed(speedSet);
    delay(5);
  }
}

void stopMotors() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}

void doSpray() {
  if (sprayState == 0) {
    sprayMotor.run(FORWARD);
    sprayMotor.setSpeed(255);
  } else {
    sprayMotor.run(RELEASE);
  }
}