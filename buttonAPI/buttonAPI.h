#ifndef BUTTONAPI_H
#define BUTTONAPI_H

#include <PS2X_lib.h>

extern PS2X Gamepad;

void gamepadBegin();
void gamepadUpdate();

class Button {
  public:
    Button(uint16_t buttonID);
    operator bool() const;
  protected:
    uint16_t buttonID;
};

class Stick {
  public:
    Stick(uint16_t X_stickID, uint16_t Y_stickID, uint16_t buttonID);
    int x() const;
    int y() const;
    operator bool() const;
  protected:
    uint16_t X_stickID;
    uint16_t Y_stickID;
    uint16_t buttonID;
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