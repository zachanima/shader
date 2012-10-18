#include "game.hpp"

Camera Game::camera;
Light Game::light;
GLuint Game::program;
GLuint Game::viewUniform;
Quadtree *Game::quadtree = NULL;



GLvoid Game::initialize() {
  const GLfloat FOV = 45.f;
  const GLfloat ASPECT = (GLfloat)WIDTH / (GLfloat)HEIGHT;
  const GLfloat ZNEAR = 1.f / 65536.f;
  const GLfloat ZFAR = 65536.f;
  const mat4 model = mat4(
    vec4(1.f, 0.f, 0.f, 0.f),
    vec4(0.f, 1.f, 0.f, 0.f),
    vec4(0.f, 0.f, 1.f, 0.f),
    vec4(0.f, 0.f, 0.f, 1.f)
  );
  const mat4 projection = perspective(FOV, ASPECT, ZNEAR, ZFAR);
  GLuint modelUniform;
  GLuint projectionUniform;
  GLuint normalmapUniform;
  GLuint colormapUniform;

  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize uniforms.
  camera.positionUniform = glGetUniformLocation(program, "camera");
  light.directionUniform = glGetUniformLocation(program, "light.direction");
  light.colorUniform =     glGetUniformLocation(program, "light.color");
  light.ambientUniform =   glGetUniformLocation(program, "light.ambient");
  light.diffuseUniform =   glGetUniformLocation(program, "light.diffuse");
  modelUniform =           glGetUniformLocation(program, "model");
  viewUniform =            glGetUniformLocation(program, "view");
  projectionUniform =      glGetUniformLocation(program, "projection");
  normalmapUniform =       glGetUniformLocation(program, "normalmap");
  colormapUniform =        glGetUniformLocation(program, "colormap");
  
  // Initialize camera.
  camera.position = vec3(0.f, 0.f, 4.f);

  // Initialize light.
  light.direction = vec3(0.f, 0.f, 1.f);
  light.color = vec3(1.f, 1.f, 1.f);
  light.ambient = 0.01f;
  light.diffuse = 0.75f;

  // Initialize quadtree class.
  Quadtree::initialize();

  // Initialize quadtree member.
  quadtree = new Quadtree(-1.f, -1.f, 1.f, 1.f, 16);
  quadtree->update(camera.position);

  // Apply uniforms.
  glUseProgram(program);
  glUniform1i(normalmapUniform, 0);
  glUniform1i(colormapUniform, 1);
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(model));
  glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, value_ptr(projection));
  glUseProgram(0);
}



GLvoid Game::update() {
  static GLuint ticks = SDL_GetTicks();
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  quadtree->update(camera.position);
  if (Keyboard::isKeyDown(KEY_W)) { offsetOrientation(vec3( 1.f, 0.f,  0.f), 0.00025f * delta); }
  if (Keyboard::isKeyDown(KEY_S)) { offsetOrientation(vec3(-1.f, 0.f,  0.f), 0.00025f * delta); }
  if (Keyboard::isKeyDown(KEY_A)) { offsetOrientation(vec3( 0.f, 0.f, -1.f), 0.0005f * delta); }
  if (Keyboard::isKeyDown(KEY_D)) { offsetOrientation(vec3( 0.f, 0.f,  1.f), 0.0005f * delta); }

  if (Keyboard::isKeyDown(KEY_R)) {
    vec3 direction = vec3(0.f, 0.f, -1.f) * camera.orientation;
    direction = normalize(direction);
    direction *= 0.0005f * Quadtree::minDistance * delta;
    camera.position += direction;
    camera.target += direction;
  }
  if (Keyboard::isKeyDown(KEY_F)) { camera.position.z += 0.0005f * Quadtree::minDistance * delta; }

  Quadtree::minDistance = 65536.f;
}



GLvoid Game::render() {
  const vec3 lightDirection = normalize(vec3(1.f + sin(SDL_GetTicks() / 1000.f) * 2.f, 1.f + cos(SDL_GetTicks() / 1000.f) * 2.f, -2.f));
  mat4 view = mat4_cast(camera.orientation);
  view = translate(view, -camera.position);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);

  glUseProgram(program);

  glUniform3fv(camera.positionUniform, 1, value_ptr(camera.position));
  glUniformMatrix4fv(viewUniform,      1, GL_FALSE, value_ptr(view));
  glUniform3fv(light.directionUniform, 1, value_ptr(lightDirection));
  glUniform3fv(light.colorUniform,     1, value_ptr(light.color));
  glUniform1f( light.ambientUniform,      light.ambient);
  glUniform1f( light.diffuseUniform,      light.diffuse);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  quadtree->render();
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}



GLvoid Game::offsetOrientation(const vec3 axis, GLfloat angle) {
  const GLfloat scalar = cosf(angle / 2.f);

  vec3 naxis = normalize(axis);
  naxis = naxis * sinf(angle / 2.f);

  quat offset(scalar, naxis.x, naxis.y, naxis.z);

  mat4 cameraMatrix = glm::lookAt(camera.position, camera.target, vec3(0.f, 1.f, 0.f));
  quat view = quat_cast(cameraMatrix);
  quat invView = conjugate(view);

  quat world = invView * offset * view;
  camera.orientation = world * camera.orientation;

  camera.orientation = normalize(camera.orientation);
}
