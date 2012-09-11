#include "game.hpp"

GLuint Game::vao;
GLuint Game::program;
GLuint Game::time_uniform;
Quadtree *Game::quadtree = NULL;



GLvoid Game::initialize() {
  GLuint perspective_uniform;
  GLuint vertexShader, fragmentShader;
  GLchar *vertexSource, *fragmentSource;
  const GLfloat frustum = 1.0f;
  const GLfloat znear = 0.5f;
  const GLfloat zfar = 10.0f;
  GLfloat matrix[16];

  // Initialize shaders.
  vertexSource = source("shader.vert");
  fragmentSource = source("shader.frag");
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertexShader, 1, (const GLchar **)&vertexSource, 0);
  glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentSource, 0);
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // Initialize uniforms.
  time_uniform = glGetUniformLocation(program, "time");
  perspective_uniform = glGetUniformLocation(program, "perspective");
  memset(matrix, 0, sizeof(GLfloat) * 16);
  matrix[0] = frustum / ((GLfloat)WIDTH / (GLfloat)HEIGHT);
  matrix[5] = frustum;
  matrix[10] = (zfar + znear) / (znear - zfar);
  matrix[14] = (2.f * zfar * znear) / (znear - zfar);
  matrix[11] = -1.f;
  glUseProgram(program);
  glUniformMatrix4fv(perspective_uniform, 1, GL_FALSE, matrix);
  glUseProgram(0);

  // Initialize quadtree.
  quadtree = new Quadtree(-1.f, -1.f, 1.f, 1.f);
}



GLvoid Game::render() {
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glUseProgram(program);
  glUniform1f(time_uniform, (GLfloat)SDL_GetTicks() / 1000.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  quadtree->render();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
