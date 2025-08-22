extern Adafruit_PWMServoDriver pwm;
extern Adafruit_PWMServoDriver pwm2;
extern Servo s1, s2;

// default functions
void stop(Adafruit_PWMServoDriver pwm, int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, 0);
  pwm.setPWM(g2, 0, 0);
  pwm.setPWM(gp, 0, 0);
}
void forward(Adafruit_PWMServoDriver pwm, int value, int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, value);
  pwm.setPWM(g2, 0, 0);
  pwm.setPWM(gp, 0, value);
}
void backward(Adafruit_PWMServoDriver pwm, int value, int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, 0);
  pwm.setPWM(g2, 0, value);
  pwm.setPWM(gp, 0, value);
}
void setAngleServo(Servo s, int degree) {
  s.write(degree);
}

// main Functions
void driveBase(String command) {
  char FB =  command[0];
  char LR = command[1];
  // y coordinate
  int FBspeed = command.substring(1,3).toInt();
  if (FB == 'B') FBspeed *= -1;
  // x coordinate
  int LRspeed = (command.substring(4,6).toInt() * FB_TO_LR_RATIO).toInt();
  if (LR == 'L') LRspeed *= -1;
  /*
    Instead of using FB and LR seperately
    We can use robot-centric movement
  */
  int leftMotor = FBspeed + LRspeed;
  int rightMotor = FBspeed - LRspeed;
  if (leftMotor >= 0) {
    forward(pwm, leftMotor * DRIVE_BASE_RATIO, M1_IN1, M1_IN2, M1_PWM);
  }
  if (leftMotor < 0) {
    backward(pwm, -leftMotor * DRIVE_BASE_RATIO, M1_IN1, M1_IN2, M1_PWM);
  }
  if (rightMotor >= 0) {
    forward(pwm, rightMotor * DRIVE_BASE_RATIO, M2_IN1, M2_IN2, M2_PWM);
  }
  if (rightMotor < 0) {
    backward(pwm, -rightMotor * DRIVE_BASE_RATIO, M2_IN1, M2_IN2, M2_PWM);
  }
}
void upBothSlide(String command, int state) {
  forward(pwm, state * SLIDE_RATIO, M3_IN1, M3_IN2, M3_PWM);
  forward(pwm, state * SLIDE_RATIO, M4_IN1, M4_IN2, M4_PWM);
}
void downBothSlide(String command, int state) {
  backward(pwm, state * SLIDE_RATIO, M3_IN1, M3_IN2, M3_PWM);
  backward(pwm, state * SLIDE_RATIO, M4_IN1, M4_IN2, M4_PWM);
}
void servoLeft(Servo s, String command) {
  if (command == "X") {
    s.write(90);
  }
  else if (command == "x") {
    s.write(0);
  }
}
void servoRight(Servo s, String command) {
  if (command == "V") {
    s.write(90);
  }
  else if (command == "v") {
    s.write(0);
  }
}
void upFrontSlide(String command) {
  if (command == "U") {
    forward(pwm2, MOTORMAX, M1_IN1, M1_IN2, M1_PWM);
  }
  else if (command == "u") {
    stop(pwm2, M1_IN1, M1_IN2, M1_PWM);
  }
}
void downFrontSlide(String command) {
  if (command == "W") {
    backward(pwm2, MOTORMAX, M1_IN1, M1_IN2, M1_PWM);
  }
  else if (command == "w") {
    stop(pwm2, M1_IN1, M1_IN2, M1_PWM);
  }
}