#include <Emakefun_MotorDriver.h>
#include <buttonAPI.h>


Emakefun_MotorDriver MotorDriver = Emakefun_MotorDriver(0x60);
Emakefun_DCMotor RMotor, LMotor, Lift;

void goMotor(Emakefun_DCMotor port, int speed) {
  port.setSpeed(abs(speed));
  if (speed > 0) {
    port.run(FORWARD);
  } else if (speed != 0) {
    port.run(BACKWARD);
  } else {
    port.run(BRAKE);
  }
}

void goMotorAtButton(Emakefun_DCMotor port, int speed, Button buttonForward, Button buttonBackward) {
  if (buttonForward) {
    goMotor(port, speed);
  } else if (buttonBackward) {
    goMotor(port, -speed);
  } else {
    goMotor(port, 0);
  }
}

void gamepadMode() {
  Gamepad.read_gamepad(false, 0);
  goMotor(RMotor, rightStick.Y);
  goMotor(LMotor, leftStick.Y);
  goMotorAtButton(Lift, 255, rightUp, rightDown);
}


void setup() {
  Serial.begin(115200);
  MotorDriver.begin(50);
  RMotor = *MotorDriver.getMotor(M1);
  LMotor = *MotorDriver.getMotor(M2);
  Lift   = *MotorDriver.getMotor(M4);

  gamepadBegin();
}

void loop() {
  gamepadMode();
}
