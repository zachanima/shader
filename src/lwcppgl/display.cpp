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

  glFlush();
  SDL_GL_SwapBuffers();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  Display::orthogonal();
}

void Display::orthogonal() {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, width, height, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
}

void Display::perspective() {
  const double FOV = 45.0;
  const double ASPECT = static_cast<double>(width) / static_cast<double>(height);
  const double ZNEAR = 0.0000152587890625;
  const double ZFAR = 65536.;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(FOV, ASPECT, ZNEAR, ZFAR);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);
}
