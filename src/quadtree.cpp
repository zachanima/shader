#include "quadtree.hpp"

GLfloat Quadtree::minDistance = 65536.f;
GLuint Quadtree::generatorProgram;



Quadtree::Quadtree(GLfloat a1, GLfloat b1, GLfloat a2, GLfloat b2, GLuint level) {
  const GLfloat L = (a2 - a1) / CHUNK_SIZE;

  box[0] = a1;
  box[1] = b1;
  box[2] = a2;
  box[3] = b2;

  for (GLuint i = 0; i < 4; i++) {
    children[i] = NULL;
  }

  this->level = level - 1;

  // Generate front face.
  GLuint v = 0;
  for (GLuint b = 0; b < VERTICES_PER_SIDE; b++) {
    for (GLuint a = 0; a < VERTICES_PER_SIDE; a++) {
      vs[v].r = vec3(a1 + L * a, b1 + L * b, 1.f);
      vs[v].t = vec2((GLfloat)a / (GLfloat)CHUNK_SIZE, (GLfloat)b / (GLfloat)CHUNK_SIZE);
      v++;
    }
  }

  // Spherize front face, apply noise.
  for (GLuint v = 0; v < VERTICES; v++) {
    vs[v].r = spherize(vs[v].r);
    const GLfloat noise = Noise::noise(vs[v].r) / 16.f + 1.f;
    vs[v].r *= noise;
  }

  // Compute indices.
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

  // Compute normals.
  for (GLuint i = 0; i < INDICES - 2; i++) {
    const GLuint i0 = is[i+0];
    const GLuint i1 = is[i+1];
    const GLuint i2 = is[i+2];
    vec3 v1;
    vec3 v2;
    if (i % 2 == 0) {
      v1 = vs[i1].r - vs[i0].r;
      v2 = vs[i2].r - vs[i0].r;
    } else {
      v1 = vs[i2].r - vs[i0].r;
      v2 = vs[i1].r - vs[i0].r;
    }
    vec3 n = normalize(cross(v1, v2));
    vs[i0].n += n;
    vs[i1].n += n;
    vs[i2].n += n;
    if (i > 0 && i % (VERTICES_PER_SIDE * 2 + 2) == VERTICES_PER_SIDE * 2 - 3) {
      i += 4; // Skip degenerate triangles.
    }
  }
  for (GLuint v = 0; v < VERTICES; v++) {
    vs[v].n = normalize(vs[v].n);
  }

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);

  // Initialize texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize framebuffer object, attach texture.
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, buffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Initialize framebuffer vertex buffer object.
  GLuint fvbo;
  const GLfloat fvs[] = {
    -1.f,  1.f, 0.f,
    -1.f, -1.f, 0.f,
     1.f,  1.f, 0.f,
     1.f, -1.f, 0.f
  };
  glGenBuffers(1, &fvbo);
  glBindBuffer(GL_ARRAY_BUFFER, fvbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), fvs, GL_STATIC_DRAW);

  // Initialize framebuffer index buffer object.
  GLuint fibo;
  const GLushort fis[] = { 0, 1, 2, 3 };
  glGenBuffers(1, &fibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), fis, GL_STATIC_DRAW);

  // Initialize framebuffer uniforms.
  const GLuint meshPositionUniform = glGetUniformLocation(generatorProgram, "meshPosition");
  const GLuint meshLengthUniform =   glGetUniformLocation(generatorProgram, "meshLength");

  // Render to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, 256, 256);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(generatorProgram);
  glUniform3fv(meshPositionUniform, 1, value_ptr(vec3(a1, b1, 1.f)));
  glUniform1f( meshLengthUniform,   (a2 - a1));
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, fvbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fibo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glPopAttrib();

  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &fvbo);
  glDeleteBuffers(1, &fibo);
}



Quadtree::~Quadtree() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}



GLvoid Quadtree::initialize() {
  generatorProgram = Display::shaders("heightmap.vert", "heightmap.frag");
}



GLvoid Quadtree::update(vec3 camera) {
  bool split = distance2(camera) < (box[2] - box[0]) * (box[2] - box[0]) * 4.f;

  if (split) {
    if (children[0] == NULL && level > 0) {
      divide();
    }
    if (children[0] != NULL) {
      for (GLuint i = 0; i < 4; i++) {
        children[i]->update(camera);
      }
    }

  } else if (children[0] != NULL) {
    for (GLuint i = 0; i < 4; i++) {
      delete children[i];
      children[i] = NULL;
    }
  }

  if (distance2(camera) < minDistance * minDistance) {
    minDistance = sqrt(distance2(camera));
  }
}



GLvoid Quadtree::render() {
  if (children[0] == NULL) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, r));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, t));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, n));

    glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

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



const GLfloat Quadtree::distance2(vec3 r) {
  return
    powf(r.x - 0.5f * vs[0].r.x - 0.5f * vs[VERTICES - 1].r.x, 2) +
    powf(r.y - 0.5f * vs[0].r.y - 0.5f * vs[VERTICES - 1].r.y, 2) +
    powf(r.z - 0.5f * vs[0].r.z - 0.5f * vs[VERTICES - 1].r.z, 2);
}



const vec3 Quadtree::spherize(vec3 r) {
  const vec3 r2 = r * r;
  r.x *= sqrt(1.f - r2.y / 2.f - r2.z / 2.f + r2.y * r2.z / 3.f);
  r.y *= sqrt(1.f - r2.x / 2.f - r2.z / 2.f + r2.x * r2.z / 3.f);
  r.z *= sqrt(1.f - r2.x / 2.f - r2.y / 2.f + r2.x * r2.y / 3.f);
  return r;
}
