#include "quadtree.hpp"

GLfloat Quadtree::distance = 0.f;



Quadtree::Quadtree(GLfloat a1, GLfloat b1, GLfloat a2, GLfloat b2, GLuint level) {
  const GLfloat L = (a2 - a1) / CHUNK_SIZE;

  box[0] = a1;
  box[1] = b1;
  box[2] = a2;
  box[3] = b2;

  children[0] = NULL;
  children[1] = NULL;
  children[2] = NULL;
  children[3] = NULL;

  this->level = level - 1;

  // Generate front face.
  size_t v = 0;
  for (size_t b = 0; b < VERTICES_PER_SIDE; b++) {
    for (size_t a = 0; a < VERTICES_PER_SIDE; a++) {
      vs[v].r[0] = a1 + L * a; vs[v].r[1] = b1 + L * b; vs[v].r[2] = 1.; v++;
    }
  }

  // Spherize front face.
  for (GLuint v = 0; v < VERTICES; v++) {
    const GLfloat x2 = vs[v].r[0] * vs[v].r[0];
    const GLfloat y2 = vs[v].r[1] * vs[v].r[1];
    const GLfloat z2 = vs[v].r[2] * vs[v].r[2];
    vs[v].r[0] *= sqrt(1.f - y2 / 2.f - z2 / 2.f + y2 * z2 / 3.f);
    vs[v].r[1] *= sqrt(1.f - x2 / 2.f - z2 / 2.f + x2 * z2 / 3.f);
    vs[v].r[2] *= sqrt(1.f - x2 / 2.f - y2 / 2.f + x2 * y2 / 3.f);
  }

  // Apply noise.
  for (GLuint v = 0; v < VERTICES; v++) {
    const GLfloat noise = Noise::noise(vs[v].r[0] * 2.f, vs[v].r[1] * 2.f, vs[v].r[2] * 2.f) / 64.f + 1.f;
    vs[v].r[0] *= noise;
    vs[v].r[1] *= noise;
    vs[v].r[2] *= noise;
  }

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  GLuint is[INDICES];
  const GLuint DEGENERATES = 2 * VERTICES_PER_SIDE;
  for (GLuint i = 0; i < INDICES; i += DEGENERATES + 2) {
    const GLuint ROW = i / DEGENERATES * VERTICES_PER_SIDE;
    for (GLuint j = 0; j < DEGENERATES; j += 2) {
      is[i + j + 0] = ROW + VERTICES_PER_SIDE + j / 2;
      is[i + j + 1] = ROW + j / 2;
    }
    if (i + DEGENERATES < INDICES) {
      is[i + DEGENERATES + 0] = ROW + VERTICES_PER_SIDE - 1;
      is[i + DEGENERATES + 1] = ROW + DEGENERATES;
    }
  }
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);
}



Quadtree::~Quadtree() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}



GLvoid Quadtree::update(GLfloat x, GLfloat y, GLfloat z) {
  bool split = distance2(x, y, z) < (box[2] - box[0]) * (box[2] - box[0]) * 4.f; 

  if (split) {
    if (children[0] == NULL && level > 0) {
      divide();
    }
    if (children[0] != NULL) {
      children[0]->update(x, y, z);
      children[1]->update(x, y, z);
      children[2]->update(x, y, z);
      children[3]->update(x, y, z);
    }
  } else if (children[0] != NULL) {
    delete children[0];
    delete children[1];
    delete children[2];
    delete children[3];
    children[0] = NULL;
    children[1] = NULL;
    children[2] = NULL;
    children[3] = NULL;
  }

  if (distance2(x, y, z) < distance * distance) {
    distance = sqrt(distance2(x, y, z));
  }
}



GLvoid Quadtree::render() {
  if (children[0] == NULL) {
    const GLuint ATTR_POSITION = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(ATTR_POSITION);
    glVertexAttribPointer(ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);
  } else {
    for (GLuint i = 0; i < 4; i++) {
      children[i]->render();
    }
  }
}



GLvoid Quadtree::divide() {
  GLfloat ca = 0.5f * (box[0] + box[2]); // Center A.
  GLfloat cb = 0.5f * (box[1] + box[3]); // Center B.
  children[0] = new Quadtree(box[0], box[1], ca,     cb, level);
  children[1] = new Quadtree(ca,     box[1], box[2], cb, level);
  children[2] = new Quadtree(box[0], cb,     ca,     box[3], level);
  children[3] = new Quadtree(ca,     cb,     box[2], box[3], level);
}



GLfloat Quadtree::distance2(GLfloat x, GLfloat y, GLfloat z) {
  return
    powf(x - 0.5f * vs[0].r[0] - 0.5f * vs[VERTICES - 1].r[0], 2) +
    powf(y - 0.5f * vs[0].r[1] - 0.5f * vs[VERTICES - 1].r[1], 2) +
    powf(z - 0.5f * vs[0].r[2] - 0.5f * vs[VERTICES - 1].r[2], 2);
}
