#ifndef LWCPPGL_DISPLAY_HPP
#define LWCPPGL_DISPLAY_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "keyboard.hpp"

class Display {
  public:
    static void create(int, int);
    static void update();

  private:
    static int width;
    static int height;
};

#endif // LWCPPGL_DISPLAY_HPP
