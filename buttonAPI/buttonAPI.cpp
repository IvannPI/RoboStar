#include <Arduino.h>
#include "buttonAPI.h"
#include <PS2X_lib.h>

static PS2X Gamepad;

#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

static void gamepadBegin() {
  Gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  gamepadUpdate();
}

static unsigned long lastMillis = 0;
static void gamepadUpdate() {
  if (millis() - lastMillis >= 50) {
    lastMillis = millis();
    Gamepad.read_gamepad(false, 0);
  }
}


namespace {
  class Button {
    public:
      Button(uint16_t buttonID) {
        this->buttonID = buttonID;
      }
      Button(const Button &button) {
        this->buttonID = button.buttonID;
      }
      operator bool() const {
        gamepadUpdate();
        return Gamepad.Button(this->buttonID );
      }
    protected:
      uint16_t buttonID;
  };
}

namespace {
  class Axis {
    public:
      Axis(uint16_t axisID) {
        this->axisID = axisID;
      }
      Axis(const Axis &axis) {
        this->axisID = axis.axisID;
      }
      operator int() const {
        gamepadUpdate();
        return Gamepad.Analog(this->axisID) - 127.5;
      }
    protected:
      uint16_t axisID;
  };
}

namespace {
  class Stick {
    public:
      Stick(uint16_t X_stickID, uint16_t Y_stickID, uint16_t buttonID) {
        this->X_stick = Axis(X_stickID);
        this->Y_stick = Axis(Y_stickID);
        this->button = Button(buttonID);
      }
      Stick(const Stick &stick) {
        this->X_stick = stick.X_stick;
        this->Y_stick = stick.Y_stick;
        this->button = stick.button; 
      }
      operator bool() const {
        return (bool)button;
      }
      int x = X_stick;
      int y = Y_stick;
    protected:
      Axis X_stick;
      Axis Y_stick;
      Button button;
  };
}


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
