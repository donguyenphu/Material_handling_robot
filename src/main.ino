#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <components/define.h>
#include <components/functions.h>

/*
  Shield 1: M1: base left, M2: base right, M3: basket, M4: slide back, S1: left, S2: right
  Shield 2: M1: slide front
 */

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x61);

SoftwareSerial bluetooth(RX_ARDUINO, TX_ARDUINO); // RX, TX

Servo s1, s2;

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz cho servo
  pwm2.begin();
  pwm2.setPWMFreq(50) // 50Hz cho servo
  s1.attach(10); /// D10 of Arduino
  s2.attach(9); /// D9 of arduino

  stop(pwm, M1_IN1, M1_IN2, M1_PWM);
  stop(pwm, M2_IN1, M2_IN2, M2_PWM);
  stop(pwm, M3_IN1, M3_IN2, M3_PWM);
  stop(pwm, M4_IN1, M4_IN2, M4_PWM);
  stop(pwm2, M1_IN1, M1_IN2, M1_PWM);
  setDefaultServo(s1);
  setDefaultServo(s2);
}

void loop() {
  driveBase();
  slideFront();
  slideBack();
  openBasket();
  tiltBasket();
}
