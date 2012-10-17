#ifndef LWCPPGL_KEYBOARD_HPP
#define LWCPPGL_KEYBOARD_HPP

#include <SDL/SDL.h>

enum {
  KEY_ESCAPE,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_A,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_W,
  KEYS
};

class Keyboard {
  public:
    static void update();
    static bool isKeyDown(int code);

    static bool key[KEYS];
};

#endif // LWCPPGL_KEYBOARD_HPP
