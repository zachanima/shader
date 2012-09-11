#include "quadtree.hpp"

Quadtree::Quadtree(GLdouble a1, GLdouble b1, GLdouble a2, GLdouble b2) {
  const GLuint ATTR_POSITION = 0;
  const GLdouble L = (a2 - a1) / CHUNK_SIZE;
  GLuint vbo, ibo;

  box[0] = a1;
  box[1] = b1;
  box[2] = a2;
  box[3] = b2;

  GLuint is[INDICES];
  size_t v = 0;
  size_t i = 0;
  for (size_t b = 0; b < CHUNK_SIZE; b++) {
    for (size_t a = 0; a <= CHUNK_SIZE; a++) {
      vs[v].r[0] = a1 + L * (a + 0); vs[v].r[1] = b1 + L * (b + 1); vs[v].r[2] = 1.; is[i++] = v; v++;
      vs[v].r[0] = a1 + L * (a + 0); vs[v].r[1] = b1 + L * (b + 0); vs[v].r[2] = 1.; is[i++] = v; v++;
    }
    // TODO: Make degenerate triangle split triangle strip.
    is[i++] = v - 1;
    is[i++] = v;
  }

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);

  // Initialize vertex array object.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(ATTR_POSITION);
  glVertexAttribPointer(ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindVertexArray(0);
}



Quadtree::~Quadtree() {
  glDeleteVertexArrays(1, &vao);
}



GLvoid Quadtree::update(GLdouble x, GLdouble y, GLdouble z) {
}



GLvoid Quadtree::render() {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);
  glBindVertexArray(0);
}
