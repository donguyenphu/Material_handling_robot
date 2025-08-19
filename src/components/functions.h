extern Adafruit_PWMServoDriver pwm;
extern Adafruit_PWMServoDriver pwm2;
extern Servo s1, s2;

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

void setDefaultServo(Servo s) {
    s.write(0);
}