#include <Arduino.h>
#include <PS2X_lib.h>

PS2X Gamepad;
#define PS2_DAT 12
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 13
#define pressures false
#define rumble    false

void begin() {
  Gamepad.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
}


class Button {
  public:
    Button(uint16_t buttonID) { buttonID = buttonID; }
    operator bool() { return Gamepad.Button(buttonID); }
  protected:
    uint16_t buttonID;
};

class Stick {
  public:
    Stick(uint16_t X_stickID, uint16_t Y_stickID, uint16_t buttonID) {
      X_stickID = X_stickID;
      Y_stickID = Y_stickID;
      buttonID  = buttonID;
    }
    int x() { return Gamepad.Analog(X_stickID) - 127.5; }
    int y() { return Gamepad.Analog(Y_stickID) - 127.5; }
    operator bool() { return Gamepad.Button(buttonID); }
  protected:
    uint16_t X_stickID;
    uint16_t Y_stickID;
    uint16_t buttonID;
};


Button start     = Button(PSB_START);
Button select    = Button(PSB_SELECT);

Button padRight  = Button(PSB_PAD_RIGHT);
Button padLeft   = Button(PSB_PAD_LEFT);
Button padUp     = Button(PSB_PAD_UP);
Button padDown   = Button(PSB_PAD_DOWN);

Button circle    = Button(PSB_CIRCLE);
Button rectangle = Button(PSB_SQUARE);
Button triangle  = Button(PSB_TRIANGLE);
Button cross     = Button(PSB_CROSS);

Button rightUp   = Button(PSB_R1);
Button rightDown = Button(PSB_R2);
Button leftUp    = Button(PSB_L1);
Button leftDown  = Button(PSB_L2);

Stick rightStick = Stick(PSS_RX, PSS_RY, PSB_R3);
Stick leftStick  = Stick(PSS_LX, PSS_LY, PSB_L3);
