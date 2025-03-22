#ifndef BUTTONAPI_H
#define BUTTONAPI_H

#include <PS2X_lib.h>

static PS2X Gamepad;

extern void gamepadBegin();
static unsigned long lastMillis;
static void gamepadUpdate();

class Button {
  public:
    Button(uint16_t buttonID);
    operator bool() const;
  protected:
    uint16_t buttonID;
};

class Axis {
  public:
    Axis(uint16_t axisID);
    operator int() const;
  protected:
    uint16_t axisID;
};

class Stick {
  public:
    Stick(uint16_t X_stickID, uint16_t Y_stickID, uint16_t buttonID);
    operator bool() const;
    int x;
    int y;
  protected:
    Axis X_stick;
    Axis Y_stick;
    Button button;
};

extern Button start;
extern Button select;

extern Button padRight;
extern Button padLeft;
extern Button padUp;
extern Button padDown;

extern Button circle;
extern Button rectangle;
extern Button triangle;
extern Button cross;

extern Button rightUp;
extern Button rightDown;
extern Button leftUp;
extern Button leftDown;

extern Stick rightStick;
extern Stick leftStick;

#endif