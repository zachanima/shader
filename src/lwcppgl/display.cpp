#include "display.hpp"

GLvoid Display::create(GLuint width, GLuint height) {
  const GLuint BPP = 16;
  const GLuint FLAGS = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWACCEL;
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_WM_SetCaption("Game", 0);
  SDL_SetVideoMode(width, height, BPP, FLAGS);
}



GLvoid Display::update() {
  Keyboard::update();
  SDL_GL_SwapBuffers();
}



GLuint Display::shaders(const GLchar *vertexFilename, const GLchar *fragmentFilename) {
  const GLchar *vertexSource = source(vertexFilename);
  const GLchar *fragmentSource = source(fragmentFilename);
  const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, 0);
  glShaderSource(fragmentShader, 1, &fragmentSource, 0);
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glLinkProgram(program);
  return program;
}



GLchar *Display::source(const GLchar *filename) {
  FILE *file;
  long length;
  char *buffer;

  file = fopen(filename, "r");
  if (!file) {
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  length = ftell(file);
  buffer = (char *)malloc(length + 1);
  fseek(file, 0, SEEK_SET);
  if (!fread(buffer, length, 1, file)) {
    return NULL;
  }
  fclose(file);
  buffer[length] = 0;

  return buffer;
}
