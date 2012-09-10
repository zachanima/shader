#ifndef MAIN_HPP
#define MAIN_HPP

#define GL_GLEXT_PROTOTYPES

#include <cstdio>
#include <SDL/SDL_opengl.h>
#include <GL/glext.h>

#include "lwcppgl/display.hpp"
#include "lwcppgl/keyboard.hpp"

#define W (1280)
#define H (720)

GLvoid initialize();
GLvoid render();
int main(int, char **);
char *filetobuffer(const char *);

#endif // MAIN_HPP
