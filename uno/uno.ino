#include <Wire.h>
#include <Servo.h>
#include <AFMotor.h>

#define MAX_SPEED 200
#define MAX_SPEED_OFFSET 20

Servo spray_Angle;
AF_DCMotor sprayMotor(1);
AF_DCMotor motorLeft(3);
AF_DCMotor motorRight(4);


int boat_Joystick_x;
int boat_Joystick_y;
int sprayPosition;
int sprayState;

void setup() {
  setDefult();
  spray_Angle.attach(10);
  spray_Angle.write(90);
  Wire.begin(8);
  Wire.onReceive(receiveData);
}

void loop() {
  spray_Angle.write(sprayPosition);
  controlMotors();
  doSpray();
}

void receiveData() {
  if (Wire.available() >= 3) {
    boat_Joystick_x = Wire.read() * 4;
    boat_Joystick_y = Wire.read() * 4;
    sprayPosition = Wire.read();
    sprayState = Wire.read();
  } else {
    setDefult();
  }
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
    sprayMotor.setSpeed(250);
  } else {
    sprayMotor.run(RELEASE);
  }
}
