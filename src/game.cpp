#include "game.hpp"

GLfloat Game::camera[3] = { 0.45f, 0.28f, 4.f };
Light Game::light;
GLuint Game::program;
GLuint Game::camera_uniform;
GLuint Game::time_uniform;
GLuint Game::light_color_uniform;
GLuint Game::light_ambient_uniform;
Quadtree *Game::quadtree = NULL;



GLvoid Game::initialize() {
  GLuint perspective_uniform;
  GLuint vertexShader, fragmentShader;
  GLchar *vertexSource, *fragmentSource;
  const GLfloat frustum = 1.f;
  const GLfloat znear = 1.f / 65536.f;
  const GLfloat zfar = 10.f;
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
  camera_uniform = glGetUniformLocation(program, "camera");
  time_uniform = glGetUniformLocation(program, "time");
  light_color_uniform = glGetUniformLocation(program, "light.color");
  light_ambient_uniform = glGetUniformLocation(program, "light.ambient");
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

  // Initialize light.
  light.color[0] = 1.f;
  light.color[1] = 1.f;
  light.color[2] = 1.f;
  light.ambient = 0.1f;

  // Initialize noise.
  Noise::initialize();

  // Initialize quadtree.
  quadtree = new Quadtree(-1.f, -1.f, 1.f, 1.f, 16);
  quadtree->update(camera[0], camera[1], camera[2]);
}



GLvoid Game::update() {
  static GLuint ticks = SDL_GetTicks();
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();
  if (Keyboard::isKeyDown(KEY_W)) {
    quadtree->update(camera[0], camera[1], camera[2]);
    camera[2] -= 0.001f * Quadtree::distance * delta;
  }
  if (Keyboard::isKeyDown(KEY_S)) {
    quadtree->update(camera[0], camera[1], camera[2]);
    camera[2] += 0.001f * Quadtree::distance * delta;
  }

  Quadtree::distance = 65536.0f;
}



GLvoid Game::render() {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);

  glUseProgram(program);
  glUniform1f(time_uniform, (GLfloat)SDL_GetTicks() / 1000.0f);
  glUniform3fv(camera_uniform, 1, camera);
  glUniform3fv(light_color_uniform, 1, light.color);
  glUniform1f(light_ambient_uniform, light.ambient);


  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  quadtree->render();
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
