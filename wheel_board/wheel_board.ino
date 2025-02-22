#include <Emakefun_MotorDriver.h>
#include <PS2X_lib.h>
#include <MatrixLaserSensor.h>

MatrixLaser Laser;

PS2X Gamepad;
#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

Emakefun_MotorDriver MotorDriver = Emakefun_MotorDriver(0x60);
Emakefun_DCMotor RMotor, LMotor, UMotor, DMotor;


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

void goRobot(int speed, int rotate) {
  V_R = rotate ==  speed || (speed - abs(rotate)) / (speed - rotate);
  V_L = rotate == -speed || (speed - abs(rotate)) / (speed + rotate);
  V_UD = (V_R - V_L) / 2;
  goMotor(RMotor, V_R);
  goMotor(LMotor, V_L);
  goMotor(UMotor,  V_UD);
  goMotor(DMotor, -V_UD);
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


void gamepadMode() {
  Gamepad.read_gamepad(false, 0);
  /*if (Gamepad.Analog(PSS_RX) == 127) {
    // position
    goMotorAtAnalog(RMotor, 255, PSS_LY);
    goMotorAtAnalog(LMotor, 255, PSS_LY);
    goMotorAtAnalog(UMotor, 255, PSS_LX);
    goMotorAtAnalog(DMotor, 255, PSS_LX);
  } else {
    // rotate
    goMotorAtAnalog(RMotor,  255, PSS_RX);
    goMotorAtAnalog(LMotor, -255, PSS_RX);
    goMotorAtAnalog(UMotor,  255, PSS_RX);
    goMotorAtAnalog(DMotor, -255, PSS_RX);
  }*/
  goRobot(Gamepad.Analog(PSS_RY), Gamepad.Analog(PSS_LX)); 
  delay(50);
}


void setup() {
  Serial.begin(115200);

  MotorDriver.begin(50);
  RMotor = *MotorDriver.getMotor(M1);
  LMotor = *MotorDriver.getMotor(M2);
  UMotor = *MotorDriver.getMotor(M3);
  DMotor = *MotorDriver.getMotor(M4);

  Gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
}

void loop() {
  gamepadMode();
}
