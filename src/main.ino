#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <PS2X_lib.h>

PS2X ps2x;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);


#define SERVOMIN 150 // giá trị xung nhỏ
#define SERVOMAX 600 // giá trị xung lớn

#define MOTORMIN 0
#define MOTORMAX 4094

#define SERVO1 0
#define SERVO2 1

#define M1_PWM 8
#define M1_IN2 9
#define M1_IN1 10

#define M2_PWM 13
#define M2_IN2 11
#define M2_IN1 11

#define M3_PWM 2
#define M3_IN2 3
#define M3_IN1 4

#define M4_PWM 7
#define M4_IN2 6
#define M4_IN1 5

#define PS2_CLK 11 /// clock
#define PS2_CS 10 /// chip select
#define PS2_CMD 9 /// command
#define PS2_DAT 8 /// data

#define pressures false
#define rumble true

void stop(int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, 0);
  pwm.setPWM(g2, 0, 0);
  pwm.setPWM(gp, 0, 0);
}

void forward(int value, int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, value);
  pwm.setPWM(g2, 0, 0);
  pwm.setPWM(gp, 0, value);
}

void backward(int value, int g1, int g2, int gp) {
  pwm.setPWM(g1, 0, 0);
  pwm.setPWM(g2, 0, value);
  pwm.setPWM(gp, 0, value);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(1600); // 50Hz cho servo
  stop(M1_IN1, M1_IN2, M1_PWM);
  stop(M2_IN1, M2_IN2, M2_PWM);
  stop(M3_IN1, M3_IN2, M3_PWM);
  stop(M4_IN1, M4_IN2, M4_PWM);

  int error = -1;
  for (int i = 0; i < 10; i++)  // thử kết nối với tay cầm ps2 trong 10 lần
  {
    delay(1000);  // đợi 1 giây
    // cài đặt chân và các chế độ: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_CS, PS2_DAT, pressures, rumble);
    Serial.print(".");
  }

  switch (error)  // kiểm tra lỗi nếu sau 10 lần không kết nối được
  {
    case 0:
      Serial.println(" Ket noi tay cam PS2 thanh cong");
      break;
    case 1:
      Serial.println(" LOI: Khong tim thay tay cam, hay kiem tra day ket noi vơi tay cam ");
      break;
    case 2:
      Serial.println(" LOI: khong gui duoc lenh");
      break;
    case 3:
      Serial.println(" LOI: Khong vao duoc Pressures mode ");
      break;
  }
}

void loop() {
  ps2x.read_gamepad(false, false);  // gọi hàm để đọc tay điều khiển
  
  // các trả về giá trị TRUE (1) khi nút được giữ
  if (ps2x.Button(PSB_START))  // nếu nút Start được giữ, in ra Serial monitor
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))  // nếu nút Select được giữ, in ra Serial monitor
    Serial.println("Select is being held");

  if (ps2x.Button(PSB_PAD_UP))  // tương tự như trên kiểm tra nút Lên (PAD UP)
  {
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);  // đọc giá trị analog ở nút này, xem nút này được bấm mạnh hay nhẹ
  }
  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }
  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }
  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
  }
  if (ps2x.NewButtonState()) {  // Trả về giá trị TRUE khi nút được thay đổi trạng thái (bật sang tắt, or tắt sang bật)
    if (ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if (ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if (ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if (ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
    if (ps2x.Button(PSB_TRIANGLE))
      Serial.println("△ pressed");
  }
  //△□○×
  if (ps2x.ButtonPressed(PSB_CIRCLE))  // Trả về giá trị TRUE khi nút được ấn (từ tắt sang bật)
    Serial.println("○ just pressed");
  if (ps2x.NewButtonState(PSB_CROSS))  // Trả về giá trị TRUE khi nút được thay đổi trạng thái
    Serial.println("× just changed");
  if (ps2x.ButtonReleased(PSB_SQUARE))  //  Trả về giá trị TRUE khi nút được ấn (từ tắt sang bật)
    Serial.println("□ just released");

  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { // các trả về giá trị TRUE khi nút được giữ
    // Đọc giá trị 2 joystick khi nút L1 hoặc R1 được giữ
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY));  // đọc trục Y của joystick bên trái. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
  }
  delay(50);

}
