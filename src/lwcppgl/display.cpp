#include "display.hpp"

int Display::width = 0;
int Display::height = 0;

void Display::create(int width, int height) {
  const int BPP = 16;
  const int FLAGS =
    SDL_OPENGL |
    SDL_GL_DOUBLEBUFFER |
    SDL_HWPALETTE |
    SDL_HWACCEL;
  Display::width = width;
  Display::height = height;

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_WM_SetCaption("Game", 0);
  SDL_SetVideoMode(width, height, BPP, FLAGS);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Display::update() {
  Keyboard::update();
  SDL_GL_SwapBuffers();
}
