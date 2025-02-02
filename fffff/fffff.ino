#include <Emakefun_MotorDriver.h>
#include <PS2X_lib.h>  //for v1.6
#include <MatrixLaserSensor.h>
#include <MatrixColorSensor.h>

MatrixLaser Laser;
MatrixColor MXColor1;

PS2X gamepad;
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
  goMotorAtAnalog(RMotor, 255, PSS_RY, "right speed");
  goMotorAtAnalog(LMotor, 255, PSS_LY, "left speed" );
  goMotorAtButton(Taker1, 180, 90,  Taker1Status, PSB_TRIANGLE, "taker up",   "taker zero");
  goMotorAtButton(Taker2, 90,  0,   Taker2Status, PSB_TRIANGLE, "taker up",   "taker zero");
  goMotorAtButton(Taker1, 0,   90,  Taker1Status, PSB_CROSS   , "taker down", "taker zero");
  goMotorAtButton(Taker2, 90,  180, Taker2Status, PSB_CROSS   , "taker down", "taker zero");
  goMotorAtButton(Lift, 255, PSB_R1, PSB_R2, "lift up", "lift down");
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

  goMotor(Taker1, 100); // ┐
  goMotor(Taker2, 0);   // ┴ taker to zero position

  gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);


  if (Laser.begin()){
    Serial.println("Matirx Laser Sensor activated");
  }
  else{
    Serial.println("Matirx Laser Sensor activation failed");
  }
  if (MXColor1.begin()){
    Serial.println("Matirx Color Sensor activated");
  }
  else{
    Serial.println("Matirx Color Sensor activation failed");
  }
  MXColor1.setLight(true, true, 0);
  MXColor1.setGamma(true);

}

void loop() {
  gamepadMode();
  /*dis = Laser.getDistance();
  Serial.println(dis);
  Serial.print("R=");
  Serial.println(MXColor1.getColor(R));
  Serial.print("G=");
  Serial.println(MXColor1.getColor(G));
  Serial.print("B=");
  Serial.println(MXColor1.getColor(B));
  Serial.print("C=");
  Serial.println(MXColor1.getColor(C));
  Serial.print("M=");
  Serial.println(MXColor1.getColor(M));
  Serial.print("Y=");
  Serial.println(MXColor1.getColor(Y));
  Serial.print("K=");
  Serial.println(MXColor1.getColor(K));
  delay(1000);*/  
}
