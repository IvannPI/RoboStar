#include <Emakefun_MotorDriver.h>
#include <PS2X_lib.h>  //for v1.6

PS2X gamepad;
#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

Emakefun_MotorDriver MotorDriver = Emakefun_MotorDriver(0x60);
Emakefun_DCMotor RMotor, LMotor;
Emakefun_Servo Lift1, Lift2;
bool Lift1Status = 0, Lift2Status = 0;


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
  if (gamepad.ButtonDataByte()) {
    if (gamepad.Button(buttonForward)) {
      goMotor(port, speed, debugForward);
    } else if (gamepad.Button(buttonBackward)) {
      goMotor(port, -speed, debugBackward);
    }
  } else {
    goMotor(port, 0);
  }
}

void goMotorAtButton(Emakefun_Servo port, uint8_t angleUp, uint8_t angleDown, bool &status,
                     uint16_t button, char debugUp[] = "", char debugDown[] = "") {
  if (gamepad.ButtonDataByte() && gamepad.ButtonPressed(button)) {
    if (status) {
      goMotor(port, angleUp, debugUp);
    } else {
      goMotor(port, angleDown, debugDown);
    }
    status = !status;
  }
}

void goMotorAtAnalog(Emakefun_DCMotor port, int speed, uint16_t analog, char debug[] = "") {
  long angle = map(127.5 - gamepad.Analog(analog), -127.5, 127.5, -speed, speed);
  goMotor(port, angle);
  if (debug != "" && angle) {
    Serial.print(debug);
    Serial.print(':');
    Serial.println(angle);
  }
}

bool hasTouch(int port, char debug[] = "") {
  return digitalRead(port);
  if (debug != "") {
    Serial.println(debug);
  }
}

void gamepadMode() {
  gamepad.read_gamepad(false, 0);
  goMotorAtAnalog(RMotor, 100, PSS_RY, "right speed");
  goMotorAtAnalog(LMotor, 100, PSS_LY, "left speed" );
  goMotorAtButton(Lift1, 100, 0, Lift1Status, PSB_TRIANGLE, "lift up", "lift down");
  goMotorAtButton(Lift2, 0, 100, Lift2Status, PSB_TRIANGLE, "lift up", "lift down");
  delay(50);
}


void setup() {
  Serial.begin(115200);
  MotorDriver.begin(50);
  RMotor = *MotorDriver.getMotor(M1);
  LMotor = *MotorDriver.getMotor(M2);
  Lift1  = *MotorDriver.getServo(1);
  Lift2  = *MotorDriver.getServo(2);

  goMotor(Lift1, 100);
  goMotor(Lift2, 0);  

  gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
}

void loop() {
  gamepadMode();
}
