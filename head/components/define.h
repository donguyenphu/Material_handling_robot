#define RX_ARDUINO 8
#define TX_ARDUINO 7

/* adafruit motor shield v2.0 has pca9685 inside
-> motor can run with 12-bit operation (0-4095)
*/
#define MOTORMIN 0
#define MOTORMAX 4095

#define SERVO1 10
#define SERVO2 9

// For controlling PWM and direction of motor, A motor has 3 main channel: 2 directions pins and 1 PWM pins. To make movement, set direct and set PWM value for the motor

/// motor 1 pins
#define M1_PWM 8
#define M1_IN2 9
#define M1_IN1 10

/// motor 2 pins
#define M2_PWM 13
#define M2_IN2 12
#define M2_IN1 11

///motor 3 pins
#define M3_PWM 2
#define M3_IN2 3
#define M3_IN1 4

/// motor 4 pins
#define M4_PWM 7
#define M4_IN2 6
#define M4_IN1 5

/// max value of left-right joinstick is 60 and forward-backward is 99 -> multiply to the left-right value to equalize the value
#define FB_TO_LR_RATIO 1

/// power drive base gain
#define DRIVE_BASE_RATIO 41

/// both slide power ratio
#define SLIDE_RATIO 4095