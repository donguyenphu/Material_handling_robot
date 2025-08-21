#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "components/define.h"
#include "components/functions.h"

/*
  Shield 1: M1: base left, M2: base right, M3: basket, M4: slide back, S1: left, S2: right
  Shield 2: M1: slide front
*/

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x61);

SoftwareSerial hc06(RX_ARDUINO, TX_ARDUINO); // RX, TX

Servo s1, s2;

int stateUp = 0, stateDown = 0;

void setup() {
  Serial.begin(9600);
  hc06.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz cho servo
  pwm2.begin();
  pwm2.setPWMFreq(50); // 50Hz cho servo
  s1.attach(10); /// D10 of Arduino
  s2.attach(9); /// D9 of arduino
  // Init speed all motors
  stop(pwm, M1_IN1, M1_IN2, M1_PWM);
  stop(pwm, M2_IN1, M2_IN2, M2_PWM);
  stop(pwm, M3_IN1, M3_IN2, M3_PWM);
  stop(pwm, M4_IN1, M4_IN2, M4_PWM);
  stop(pwm2, M1_IN1, M1_IN2, M1_PWM);
  // Set default angle for servos
  setAngleServo(s1, 0);
  setAngleServo(s2, 0);
}

/* 
  Joinstick updown+leftright: drive base
  Z (toggle) (lên slide)
  Y (toggle) (xuống slide)
  X (switch state to nhỏ) (servo toggle trái)
  V (switch state to nhỏ) (servo toggle phải)
  U  (switch state to nhỏ) (lên slide trước)
  W (switch state to nhỏ) (xuống slide trước)
*/

void loop() {
  if (hc06.available()) {
    /* Because c was sent byte to byte, use String can cause memory fragmentation 
    -> we read until the end line to get full command */
    String command = hc06.readStringUntil('\n');
    if (command.length() == 6) driveBase(command); /// Format: FxxLxx (F can be B, L can be R)
    if (command == "Z") {
      upBothSlide(command, stateUp);
      stateUp = (stateUp + 1) % 2;
    }
    if (command == "Y") {
      downBothSlide(command, stateDown);
      stateDown = (stateDown + 1) % 2;
    }
    if (command == "X" || command == "x") servoLeft(s1, command);
    if (command == "V" || command == "v") servoRight(s2, command);
    if (command == "U" || command == "u") upFrontSlide(command);
    if (command == "W" || command == "w") downFrontSlide(command);
  }
  /* Send Serial command to hc06 ok */
  if (Serial.available()) {
    char c = Serial.read();
    hc06.write(c);
  }
}
