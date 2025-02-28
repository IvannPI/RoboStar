#include <Emakefun_MotorDriver.h>
#include <PS2X_lib.h>  //for v1.6


PS2X Gamepad;
#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

Emakefun_MotorDriver MotorDriver = Emakefun_MotorDriver(0x60);
Emakefun_DCMotor RMotor, LMotor, Lift;
Emakefun_Servo Taker1, Taker2;
bool Taker1Status = 0, Taker2Status = 0;


void goMotor(Emakefun_DCMotor port, int speed, char debug[] = "") {
  port.setSpeed(abs(speed));
  if (speed > 0) {
    port.run(FORWARD);
  } else if (speed != 0) {
    port.run(BACKWARD);
  } else {
    port.run(BRAKE);
  }
  if (debug != "") {
    Serial.println(debug);
  }
}

void goMotor(Emakefun_Servo port, uint8_t angle, char debug[] = "") {
  port.writeServo(angle);
  if (debug != "") {
    Serial.println(debug);
  }
}

void goMotorAtButton(Emakefun_DCMotor port, int speed,
                     uint16_t buttonForward, uint16_t buttonBackward,
                     char debugForward[] = "", char debugBackward[] = "") {
  if (Gamepad.ButtonDataByte()) {
    if (Gamepad.Button(buttonForward)) {
      goMotor(port, speed, debugForward);
    } else if (Gamepad.Button(buttonBackward)) {
      goMotor(port, -speed, debugBackward);
    }
  } else {
    goMotor(port, 0);
  }
}

void goMotorAtButton(Emakefun_Servo port, uint8_t angleUp, uint8_t angleDown, bool &status,
                     uint16_t button, char debugUp[] = "", char debugDown[] = "") {
  if (Gamepad.ButtonDataByte() && Gamepad.ButtonPressed(button)) {
    if (status) {
      goMotor(port, angleUp, debugUp);
    } else {
      goMotor(port, angleDown, debugDown);
    }
    status = !status;
  }
}

void goMotorAtAnalog(Emakefun_DCMotor port, int speed, uint16_t analog, char debug[] = "") {
  long angle = map(127.5 - Gamepad.Analog(analog), -127.5, 127.5, -speed, speed);
  goMotor(port, angle);
  if (debug != "" && angle) {
    Serial.print(debug);
    Serial.print(':');
    Serial.println(angle);
  }
}


void AutoMode() {
  goMotor(RMotor, 255);
  delay(700);
  goMotor(LMotor, 255);
  delay(600);
  goMotor(RMotor, 0);
  goMotor(LMotor, 0);
  goMotor(Taker1, 180);
  goMotor(Taker2, 0);
  delay(800);
  
  goMotor(Taker1, 90); // ┐
  goMotor(Taker2, 90); // ┴ taker to zero position
  goMotor(LMotor, -255);
  delay(700);
  goMotor(RMotor, -255);
  delay(500);
  goMotor(RMotor, 255);
  delay(750);
  goMotor(LMotor, 255);
  delay(1300);
  goMotor(RMotor, 0);
  goMotor(LMotor, 0);
}

void gamepadMode() {
  Gamepad.read_gamepad(false, 0);
  goMotorAtAnalog(RMotor, 255, PSS_RY, "right speed");
  goMotorAtAnalog(LMotor, 255, PSS_LY, "left speed" );
  goMotorAtButton(Taker1, 180, 90,  Taker1Status, PSB_CROSS   , "taker down", "taker zero");
  goMotorAtButton(Taker2, 0,   90,  Taker2Status, PSB_CROSS   , "taker down", "taker zero");
  goMotorAtButton(Lift, 255, PSB_R2, PSB_R1, "lift down", "lift up");
  delay(50);
}


void setup() {
  Serial.begin(115200);
  MotorDriver.begin(50);
  RMotor = *MotorDriver.getMotor(M1);
  LMotor = *MotorDriver.getMotor(M2);
  Lift   = *MotorDriver.getMotor(M3);
  Taker1 = *MotorDriver.getServo(1);
  Taker2 = *MotorDriver.getServo(2);

  Gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  goMotor(Taker1, 90); // ┐
  goMotor(Taker2, 90); // ┴ taker to zero position


  do {
    Gamepad.read_gamepad(false, 0);
    delay(50);
  } while (!Gamepad.ButtonPressed(PSB_START));

  AutoMode();
}

void loop() {
  gamepadMode();
}
