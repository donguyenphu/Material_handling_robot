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

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60); /// level 1 (attached to arduino uno) adafruit motor shield v2.0
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x61); /// level 2 (attached to level 1) adafruit motor shield v2.0

SoftwareSerial hc06(RX_ARDUINO, TX_ARDUINO); // RX, TX

Servo s1, s2;

int stateUp = 0, stateDown = 0; /// state for up and down both slides

void setup() {
  Serial.begin(9600);
  hc06.begin(9600); /// default hc06 baud rate
  pwm.begin();
  pwm.setPWMFreq(50); // 50Hz for servo
  pwm2.begin();
  pwm2.setPWMFreq(50); // 50Hz for servo
  s1.attach(10); /// D10 of Arduino (top gate)
  s2.attach(9); /// D9 of arduino (bottom gate)

  // Initialize speed all motors
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
  Z (toggle) (up 2 slides)
  Y (toggle) (down 2 slides)
  X (switch state to nhỏ) (servo toggle left)
  V (switch state to nhỏ) (servo toggle right)
  U  (switch state to nhỏ) (up slide front)
  W (switch state to nhỏ) (down slide front)
*/

void loop() {
  /* command from hc06 to Serial*/
  if (hc06.available()) {
    /* Because c was sent byte to byte, use String can cause memory fragmentation 
    -> we read until the end line to get full command */
    String command = hc06.readStringUntil('\n');
    if (command.length() == 6) driveBase(command); /// Format: FxxLxx (F can be B, L can be R)
    if (command == "Z") { /// Z first turn -> up 2 slides, Z second turn -> stop 2 slides
      stateUp = (stateUp + 1) % 2;
      upBothSlide(command, stateUp);
    }
    if (command == "Y") { /// Y first turn -> down 2 slides, Y second turn -> stop 2 slides
      stateDown = (stateDown + 1) % 2;
      downBothSlide(command, stateDown);
    }
    if (command == "X" || command == "x") servoLeft(s1, command); /// X when open, x when close
    if (command == "V" || command == "v") servoRight(s2, command); /// V when open, v when close
    if (command == "U" || command == "u") upFrontSlide(command); /// U when up, u when stop
    if (command == "W" || command == "w") downFrontSlide(command); /// W when down, w when stop
  }
  /* Send Serial command to hc06 */
  if (Serial.available()) {
    char c = Serial.read();
    hc06.write(c);
  }
}
