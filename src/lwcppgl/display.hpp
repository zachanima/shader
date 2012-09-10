#ifndef LWCPPGL_DISPLAY_HPP
#define LWCPPGL_DISPLAY_HPP

#define GL_GLEXT_PROTOTYPES

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glext.h>


#include "keyboard.hpp"

class Display {
  public:
    static void create(int, int);
    static void update();
    static void orthogonal();
    static void perspective();

  private:
    static int width;
    static int height;
};

#endif // LWCPPGL_DISPLAY_HPP
