#include "game.hpp"

Camera Game::camera;
Light Game::light;
GLuint Game::program;
Quadtree *Game::quadtree = NULL;



GLvoid Game::initialize() {
  GLuint perspectiveUniform;
  GLuint samplerUniform;
  const GLfloat FOV = 45.f;
  const GLfloat ASPECT = (GLfloat)WIDTH / (GLfloat)HEIGHT;
  const GLfloat ZNEAR = 1.f / 65536.f;
  const GLfloat ZFAR = 65536.f;
  const mat4 projectionMatrix = perspective(FOV, ASPECT, ZNEAR, ZFAR);

  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize uniforms.
  camera.positionUniform = glGetUniformLocation(program, "camera");
  light.directionUniform = glGetUniformLocation(program, "light.direction");
  light.colorUniform =     glGetUniformLocation(program, "light.color");
  light.ambientUniform =   glGetUniformLocation(program, "light.ambient");
  light.diffuseUniform =   glGetUniformLocation(program, "light.diffuse");
  perspectiveUniform =     glGetUniformLocation(program, "perspective");
  samplerUniform =         glGetUniformLocation(program, "sampler");
  
  // Initialize camera.
  camera.position = vec3(0.05f, -0.0f, 4.f);

  // Initialize light.
  light.direction = vec3(0.f, 0.f, 1.f);
  light.color = vec3(1.f, 1.f, 1.f);
  light.ambient = 0.1f;
  light.diffuse = 0.75f;

  // Initialize quadtree class.
  Quadtree::initialize();

  // Initialize quadtree member.
  quadtree = new Quadtree(-1.f, -1.f, 1.f, 1.f, 16);
  quadtree->update(camera.position);

  // Apply perspective uniform.
  glUseProgram(program);
  glUniform1i(samplerUniform, 0);
  glUniformMatrix4fv(perspectiveUniform, 1, GL_FALSE, value_ptr(projectionMatrix));
  glUseProgram(0);
}



GLvoid Game::update() {
  static GLuint ticks = SDL_GetTicks();
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  quadtree->update(camera.position);
  if (Keyboard::isKeyDown(KEY_W)) { camera.position.z -= 0.0005f * Quadtree::minDistance * delta; }
  if (Keyboard::isKeyDown(KEY_S)) { camera.position.z += 0.0005f * Quadtree::minDistance * delta; }

  // Debug controls.
  if (Keyboard::isKeyDown(KEY_R)) { light.ambient += 0.0001f * delta; }
  if (Keyboard::isKeyDown(KEY_F)) { light.ambient -= 0.0001f * delta; }

  Quadtree::minDistance = 65536.f;
}



GLvoid Game::render() {
  const vec3 lightDirection = normalize(vec3(1.f + sin(SDL_GetTicks() / 1000.f) * 2.f, 1.f + cos(SDL_GetTicks() / 1000.f) * 2.f, 1.f));

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);

  glUseProgram(program);

  glUniform3fv(camera.positionUniform, 1, value_ptr(camera.position));
  glUniform3fv(light.directionUniform, 1, value_ptr(lightDirection));
  glUniform3fv(light.colorUniform,     1, value_ptr(light.color));
  glUniform1f( light.ambientUniform,      light.ambient);
  glUniform1f( light.diffuseUniform,      light.diffuse);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  quadtree->render();
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
