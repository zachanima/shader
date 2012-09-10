#include "main.hpp"

GLuint triangleVBO;
GLuint triangleIBO;
GLuint triangleVAO;
GLuint shaderProgram;
GLchar *vertexSource, *fragmentSource;
GLuint vertexShader, fragmentShader;
GLuint time_uniform;
GLuint perspective_uniform;
const unsigned int attribute_position = 0;
const unsigned int attribute_color = 1;
const unsigned int INDICES = 13;
float data[] = {
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



GLvoid initialize() {
  // Initialize vertex buffer object.
  glGenBuffers(1, &triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(attribute_position);
  glEnableVertexAttribArray(attribute_color);
  glVertexAttribPointer(attribute_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, 0, (void *)144);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

  // Initialize index buffer object.
  GLuint indices[] = { 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 8 };
  glGenBuffers(1, &triangleIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

  // Initialize shaders.
  vertexSource = filetobuffer("shader.vert");
  fragmentSource = filetobuffer("shader.frag");
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertexShader, 1, (const GLchar **)&vertexSource, 0);
  glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentSource, 0);
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Initialize uniforms.
  time_uniform = glGetUniformLocation(shaderProgram, "time");
  perspective_uniform = glGetUniformLocation(shaderProgram, "perspective");
  const float frustum = 1.0f;
  const float znear = 0.5f;
  const float zfar = 10.0f;
  float matrix[16];
  memset(matrix, 0, sizeof(float) * 16);
  matrix[0] = frustum / ((float)W / (float)H);
  matrix[5] = frustum;
  matrix[10] = (zfar + znear) / (znear - zfar);
  matrix[14] = (2 * zfar * znear) / (znear - zfar);
  matrix[11] = -1.0f;
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(perspective_uniform, 1, GL_FALSE, matrix);
  glUseProgram(0);

  // Initialize vertex array object.
  glGenVertexArrays(1, &triangleVAO);
  glBindVertexArray(triangleVAO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glEnableVertexAttribArray(attribute_position);
  glEnableVertexAttribArray(attribute_color);
  glVertexAttribPointer(attribute_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, 0, (void *)144);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIBO);
  glBindVertexArray(0);
}



GLvoid render() {
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glUseProgram(shaderProgram);
  glBindVertexArray(triangleVAO);

  glUniform1f(time_uniform, (float)SDL_GetTicks() / 1000.0f);
  glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (void *)0);

  glBindVertexArray(0);
  glUseProgram(0);
}



int main(int argc, char **argv) {
  Display::create(W, H);
  initialize();

  while (!Keyboard::isKeyDown(KEY_ESCAPE)) {
    render();
    Display::update();
  }

  return 0;
}



char *filetobuffer(const char *filename) {
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
