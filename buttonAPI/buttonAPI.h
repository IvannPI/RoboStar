#include <buttonAPI.cpp>


void begin(void)


class Button {
  public:
    Button(uint16_t);
    operator bool(void);
  protected:
    uint16_t;
};

class Stick {
  public:
    Stick(uint16_t, uint16_t, uint16_t);
    int x(void);
    int y(void);
    operator bool(void);
  protected:
    uint16_t;
    uint16_t;
    uint16_t;
};

Button start;
Button select;

Button padRight;
Button padLeft;
Button padUp;
Button padDown;

Button circle;
Button rectangle;
Button triangle;
Button cross;

Button rightUp;
Button rightDown;
Button leftUp;
Button leftDown;

Stick rightStick;
Stick leftStick;
