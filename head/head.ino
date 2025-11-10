#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "components/define.h"
#include "components/functions.h"

/*
  Shield 1: M1: base left, M2: base right, M4: slide back, S1: left, S2: right
  Shield 2: M1: slide front, M3: basket
*/

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60); /// level 1 (attached to arduino uno) adafruit motor shield v2.0
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x61); /// level 2 (attached to level 1) adafruit motor shield v2.0

//To receive and transmit data, rx arduino -> tx hc06 and tx arduino -> rx hc06
SoftwareSerial hc06(RX_ARDUINO, TX_ARDUINO);

Servo s1, s2;

/// state for up and down both slides
int stateUpSlide = 0; 
int stateDownSlide = 0;

/// state for open and close baskets
int stateBasketOpen = 0;
int stateBasketClose = 0;

/// state for up and down slide behind
int stateUpSlide2 = 0; 
int stateDownSlide2 = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600); /// serial baud rate has to be the same as hc06 to read command
  hc06.begin(9600); /// default hc06 baud rate
  pwm.begin();
  // 50Hz for servo, 1600Hz for motor
  pwm.setPWMFreq(1600); 
  pwm2.begin();
  pwm2.setPWMFreq(1600);
  s1.attach(10); /// D10 of Arduino (top gate)
  s2.attach(9); /// D9 of arduino (bottom gate)

  stateUpSlide = 0; 
  stateDownSlide = 0;
  stateBasketOpen = 0;
  stateBasketClose = 0;
  stateUpSlide2 = 0; 
  stateDownSlide2 = 0;
  
  // Initialize speed all motors
  stop(pwm, M1_IN1, M1_IN2, M1_PWM); /// drivebase left
  stop(pwm, M2_IN1, M2_IN2, M2_PWM); /// drivebase right
  stop(pwm2, M3_IN1, M3_IN2, M3_PWM); /// basket
  stop(pwm, M4_IN1, M4_IN2, M4_PWM); /// slide behind
  stop(pwm2, M1_IN1, M1_IN2, M1_PWM); /// slide front

  // Set default angle for servos
  setAngleServo(s1, 180);
  setAngleServo(s2, 0);

  // check for Adafruit Motorshield v2.0 work or not
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C found at 0x");
      Serial.println(addr, HEX);
    }
  }
}
/* 
  Joinstick updown+leftright: drive base
  Z (toggle) (up 2 slides)
  Y (toggle) (down 2 slides)
  X (switch state to nhỏ) (servo toggle left)
  V (switch state to nhỏ) (servo toggle right)
  U  (switch state to nhỏ) (up slide front)
  W (switch state to nhỏ) (down slide front)
  1 (toggle) (open basket)
  2 (toggle) (close basket)
  3 (toggle) (up slide behind)
  4 (toggle) (down slide behind)
*/

void loop() {
  /* command from hc06 to Serial*/
  if (hc06.available()) {
    /* Because c was sent byte to byte, use String can cause memory fragmentation 
    -> we read until the end line to get full command from bluetooth controller */
    String command = hc06.readStringUntil('\n');
    Serial.println(command+" PRESSED");
    if (command.length() == 6) /// Format: FxxLxx (F can be B, L can be R)
    {
      driveBase(command);
    }
    /// Z first turn -> up 2 slides, Z second turn -> stop 2 slides
    if (command == "Z") {
      stateUpSlide = (stateUpSlide + 1) % 2;
      stateDownSlide = 0;
      stateBasketOpen = 0;
      stateBasketClose = 0;
      stateUpSlide2 = 0; 
      stateDownSlide2 = 0;
      upBothSlide(command, stateUpSlide);
    }
    /// Y first turn -> down 2 slides, Y second turn -> stop 2 slides
    if (command == "Y") {
      stateDownSlide = (stateDownSlide + 1) % 2;
      stateUpSlide = 0;
      stateBasketOpen = 0;
      stateBasketClose = 0;
      stateUpSlide2 = 0; 
      stateDownSlide2 = 0;
      downBothSlide(command, stateDownSlide);
    }
    /// first turn,-> open baskets, second turn -> stop baskets 
    if (command == "1") {
      stateBasketOpen = (stateBasketOpen + 1) % 2;
      stateUpSlide = 0; 
      stateDownSlide = 0;
      stateBasketClose = 0;
      stateUpSlide2 = 0; 
      stateDownSlide2 = 0;
      openBaskets(command, stateBasketOpen);
    }
    /// first turn,-> close baskets, second turn -> stop baskets
    if (command == "2") {
      stateBasketClose = (stateBasketClose + 1) % 2;
      stateUpSlide = 0; 
      stateDownSlide = 0;
      stateBasketOpen = 0;
      stateUpSlide2 = 0; 
      stateDownSlide2 = 0;
      closeBaskets(command, stateBasketClose); 
    }
    /// first turn,-> up slide behind, second turn -> stop slide behind
    if (command == "3") {
      stateUpSlide2 = (stateUpSlide2 + 1) % 2;
      stateUpSlide = 0; 
      stateDownSlide = 0;
      stateBasketOpen = 0;
      stateBasketClose = 0;
      stateDownSlide2 = 0;
      upBehindSlide(command, stateUpSlide2);
    }
    /// first turn,-> down slide behind, second turn -> stop slide behind
    if (command == "4") {
      stateDownSlide2 = (stateDownSlide2 + 1) % 2;
      stateUpSlide = 0; 
      stateDownSlide = 0;
      stateBasketOpen = 0;
      stateBasketClose = 0;
      stateUpSlide2 = 0; 
      downBehindSlide(command, stateDownSlide2); 
    }
    // command area
    if (command == "X" || command == "x") servoLeft(s1, command); /// X when open, x when close
    else if (command == "V" || command == "v") servoRight(s2, command); /// V when open, v when close
    else if (command == "U" || command == "u") upFrontSlide(command); /// U when up, u when stop
    else if (command == "W" || command == "w") downFrontSlide(command); /// W when down, w when stop
  }
  /* Send Serial command to hc06 */
  if (Serial.available()) {
    hc06.write(Serial.read());
  }
}
