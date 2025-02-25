#include <Emakefun_MotorDriver.h>
#include <buttonAPI.h>


Emakefun_MotorDriver MotorDriver = Emakefun_MotorDriver(0x60);
Emakefun_DCMotor RMotor, LMotor, Lift;
Emakefun_Servo Taker1, Taker2;
bool Taker1Status = 0, Taker2Status = 0;
int down = 0;


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

void goMotor(Emakefun_Servo port, uint8_t angle) {
  port.writeServo(angle);
}

void goMotorAtButton(Emakefun_DCMotor port, int speed,
                     Button buttonForward, Button buttonBackward) {
  if (buttonForward) {
    goMotor(port, speed);
  } else if (buttonBackward) {
    goMotor(port, -speed);
  } else {
    goMotor(port, 0);
  }
}

void goMotorAtButton(Emakefun_Servo port, int angleUp, int angleDown,
                     bool &status, Button button) {
  if (button) {
    if (status) {
      goMotor(port, angleUp);
    } else {
      goMotor(port, angleDown);
    }
    status = !status;
  }
}

bool hasTouch(int port) {
  return digitalRead(port);
}

void gamepadMode() {
  Gamepad.read_gamepad(false, 0);
  goMotor(RMotor, rightStick.y());
  goMotor(LMotor, leftStick.y());
  goMotorAtButton(Taker1, 180, 90, Taker1Status, cross);
  goMotorAtButton(Taker2, 0,   90, Taker2Status, cross);
  goMotorAtButton(Lift, 255, rightUp, rightDown);
}


void setup() {
  Serial.begin(115200);
  MotorDriver.begin(50);
  RMotor = *MotorDriver.getMotor(M1);
  LMotor = *MotorDriver.getMotor(M2);
  Lift   = *MotorDriver.getMotor(M4);
  Taker1 = *MotorDriver.getServo(1);
  Taker2 = *MotorDriver.getServo(2);

  goMotor(Taker1, 90); // ┐
  goMotor(Taker2, 90); // ┴ taker to zero position

  gamepadBegin();
}

void loop() {
  gamepadMode();
}
