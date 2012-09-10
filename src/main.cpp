#include "main.hpp"

GLuint triangleVBO;
GLuint triangleIBO;
GLuint shaderProgram;
GLchar *vertexSource, *fragmentSource;
GLuint vertexShader, fragmentShader;
const unsigned int shaderAttribute = 0;
const unsigned int NUM_OF_VERTICES_IN_DATA = 6;
const unsigned int NUM_OF_INDICES_IN_DATA = 8;
float data[6][3] = {
  {  0.f,   0.5f, 0.f },
  { -0.5f, -0.5f, 0.f },
  {  0.5f, -0.5f, 0.f },
  {  0.5f,  1.f,  0.f },
  {  0.f,   0.f,  0.f },
  {  1.f,   0.f,  0.f }
};



GLvoid initialize() {
  glGenBuffers(1, &triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, NUM_OF_VERTICES_IN_DATA * 3 * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(shaderAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

  GLuint indices[] = { 0, 1, 2, 2, 3, 3, 4, 5 };
  glGenBuffers(1, &triangleIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_OF_INDICES_IN_DATA * sizeof(GLuint), indices, GL_STATIC_DRAW);
  // delete indices;

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
  glBindAttribLocation(shaderProgram, shaderAttribute, "position");
  glLinkProgram(shaderProgram);
}



GLvoid render() {
  glUseProgram(shaderProgram);
  glClearColor(0., 0., 0., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawElements(GL_TRIANGLE_STRIP, NUM_OF_INDICES_IN_DATA, GL_UNSIGNED_INT, (void *)0);
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
