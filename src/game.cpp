#include "game.hpp"

GLuint Game::VAO;
GLuint Game::program;
GLuint Game::time_uniform;
const GLuint Game::INDICES = 13;



GLvoid Game::initialize() {
  const unsigned int ATTR_POSITION = 0, ATTR_COLOR = 1;
  GLuint VBO, IBO;
  GLuint perspective_uniform;
  GLuint vertexShader, fragmentShader;
  GLchar *vertexSource, *fragmentSource;
  GLfloat data[] = {
     0.0f,    0.5f, -1.5f, 1.0f,
     0.5f, -0.366f, -1.5f, 1.0f,
    -0.5f, -0.366f, -1.5f, 1.0f,
     0.0f,    0.5f, -2.0f, 1.0f,
     0.5f, -0.366f, -2.0f, 1.0f,
    -0.5f, -0.366f, -2.0f, 1.0f,
     0.0f,    0.5f, -2.5f, 1.0f,
     0.5f, -0.366f, -2.5f, 1.0f,
    -0.5f, -0.366f, -2.5f, 1.0f,
     1.0f,    0.0f, 0.0f, 1.0f,
     0.0f,    1.0f, 0.0f, 1.0f,
     0.0f,    0.0f, 1.0f, 1.0f,
     1.0f,    0.0f, 0.0f, 1.0f,
     0.0f,    1.0f, 0.0f, 1.0f,
     0.0f,    0.0f, 1.0f, 1.0f,
     1.0f,    0.0f, 0.0f, 1.0f,
     0.0f,    1.0f, 0.0f, 1.0f,
     0.0f,    0.0f, 1.0f, 1.0f,
  };
  GLuint indices[] = { 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 8 };
  const GLfloat frustum = 1.0f;
  const GLfloat znear = 0.5f;
  const GLfloat zfar = 10.0f;
  GLfloat matrix[16];

  // Initialize vertex buffer object.
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(ATTR_POSITION);
  glEnableVertexAttribArray(ATTR_COLOR);
  glVertexAttribPointer(ATTR_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)144);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Initialize index buffer object.
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

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
  matrix[14] = (2 * zfar * znear) / (znear - zfar);
  matrix[11] = -1.0f;
  glUseProgram(program);
  glUniformMatrix4fv(perspective_uniform, 1, GL_FALSE, matrix);
  glUseProgram(0);

  // Initialize vertex array object.
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glEnableVertexAttribArray(ATTR_POSITION);
  glEnableVertexAttribArray(ATTR_COLOR);
  glVertexAttribPointer(ATTR_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)144);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBindVertexArray(0);
}



GLvoid Game::render() {
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glUseProgram(program);
  glBindVertexArray(VAO);

  glUniform1f(time_uniform, (float)SDL_GetTicks() / 1000.0f);
  glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (void *)0);

  glBindVertexArray(0);
  glUseProgram(0);
}
