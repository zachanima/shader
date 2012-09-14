#include "game.hpp"

vec3 Game::camera = vec3(0.25f, 0.25f, 4.f);
Light Game::light;
GLuint Game::program;
GLuint Game::camera_uniform;
GLuint Game::time_uniform;
GLuint Game::light_direction_uniform;
GLuint Game::light_color_uniform;
GLuint Game::light_ambient_uniform;
GLuint Game::light_diffuse_uniform;
Quadtree *Game::quadtree = NULL;



GLvoid Game::initialize() {
  GLuint perspective_uniform;
  GLuint vertexShader, fragmentShader;
  GLchar *vertexSource, *fragmentSource;
  const GLfloat znear = 1.f / 65536.f;
  const GLfloat zfar = 65536.f;
  mat4 projection_matrix = perspective(45.f, (GLfloat)WIDTH / (GLfloat)HEIGHT, znear, zfar);

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
  light_direction_uniform = glGetUniformLocation(program, "light.direction");
  light_color_uniform = glGetUniformLocation(program, "light.color");
  light_ambient_uniform = glGetUniformLocation(program, "light.ambient");
  light_diffuse_uniform = glGetUniformLocation(program, "light.diffuse");
  perspective_uniform = glGetUniformLocation(program, "perspective");
  glUseProgram(program);
  glUniformMatrix4fv(perspective_uniform, 1, GL_FALSE, value_ptr(projection_matrix));
  glUseProgram(0);

  // Initialize light.
  light.direction = vec3(0.f, 0.f, 1.f);
  light.color = vec3(1.f, 1.f, 1.f);
  light.ambient = 0.01f;
  light.diffuse = 0.75f;

  // Initialize noise.
  Noise::initialize();

  // Initialize quadtree.
  quadtree = new Quadtree(-1.f, -1.f, 1.f, 1.f, 16);
  quadtree->update(camera);
}



GLvoid Game::update() {
  static GLuint ticks = SDL_GetTicks();
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();
  if (Keyboard::isKeyDown(KEY_W)) {
    quadtree->update(camera);
    camera.z -= 0.0005f * Quadtree::distance * delta;
  }
  if (Keyboard::isKeyDown(KEY_S)) {
    quadtree->update(camera);
    camera.z += 0.0005f * Quadtree::distance * delta;
  }

  // Debug controls.
  if (Keyboard::isKeyDown(KEY_R)) {
    light.ambient += 0.0001f * delta;
  }
  if (Keyboard::isKeyDown(KEY_F)) {
    light.ambient -= 0.0001f * delta;
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
  glUniform3fv(camera_uniform, 1, value_ptr(camera));

  vec3 normalized_light_direction = normalize(vec3(1.f + sin(SDL_GetTicks() / 1000.f) * 2.f, 0.f, 1.f));
  glUniform3fv(light_direction_uniform, 1, value_ptr(normalized_light_direction));
  glUniform3fv(light_color_uniform, 1, value_ptr(light.color));
  glUniform1f(light_ambient_uniform, light.ambient);
  glUniform1f(light_diffuse_uniform, light.diffuse);


  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  quadtree->render();
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
