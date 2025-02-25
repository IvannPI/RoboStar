#include <Arduino.h>
#include "buttonAPI.h"
#include <PS2X_lib.h>

PS2X Gamepad;

#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

void gamepadBegin() {
  Gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  gamepadUpdate();
}

unsigned long lastMillis = 0;
void gamepadUpdate() {
  if (millis() - lastMillis >= 50) {
    lastMillis = millis();
    Gamepad.read_gamepad(false, 0);
  }
}

Button::Button(uint16_t buttonID) : buttonID(buttonID) {}
Button::operator bool() const { gamepadUpdate(); return Gamepad.Button(buttonID); }

Stick::Stick(uint16_t X_stickID, uint16_t Y_stickID, uint16_t buttonID)
  : X_stickID(X_stickID), Y_stickID(Y_stickID), buttonID(buttonID) {}

int Stick::x() const { gamepadUpdate(); 127.5 - return Gamepad.Analog(X_stickID); }
int Stick::y() const { gamepadUpdate(); 127.5 - return Gamepad.Analog(Y_stickID); }
Stick::operator bool() const { gamepadUpdate(); return Gamepad.Button(buttonID); }

Button start(PSB_START);
Button select(PSB_SELECT);

Button padRight(PSB_PAD_RIGHT);
Button padLeft(PSB_PAD_LEFT);
Button padUp(PSB_PAD_UP);
Button padDown(PSB_PAD_DOWN);

Button circle(PSB_CIRCLE);
Button rectangle(PSB_SQUARE);
Button triangle(PSB_TRIANGLE);
Button cross(PSB_CROSS);

Button rightUp(PSB_R1);
Button rightDown(PSB_R2);
Button leftUp(PSB_L1);
Button leftDown(PSB_L2);

Stick rightStick(PSS_RX, PSS_RY, PSB_R3);
Stick leftStick(PSS_LX, PSS_LY, PSB_L3);