#include "quadtree.hpp"

GLfloat Quadtree::minDistance = 65536.f;
GLuint Quadtree::vertexmapProgram;
GLuint Quadtree::heightmapProgram;
GLuint Quadtree::normalmapProgram;
GLuint Quadtree::colormapProgram;



Quadtree::Quadtree(GLfloat a1, GLfloat b1, GLfloat a2, GLfloat b2, GLuint level) {
  const GLfloat L = (a2 - a1) / CHUNK_SIZE;

  box[0] = a1;
  box[1] = b1;
  box[2] = a2;
  box[3] = b2;
  this->level = level;

  // Nullify children.
  for (GLuint i = 0; i < 4; i++) {
    children[i] = NULL;
  }

  // Generate spherized front face.
  size_t i = 0;
  for (GLuint b = 0; b < VERTICES_PER_SIDE; b++) {
    for (GLuint a = 0; a < VERTICES_PER_SIDE; a++, i++) {
      vs[i].r = spherize(vec3(a1 + L * a, b1 + L * b, 1.f));
      vs[i].t = vec2((GLfloat)a / (GLfloat)CHUNK_SIZE, (GLfloat)b / (GLfloat)CHUNK_SIZE);
    }
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

  computeVertexmap();
  generateHeightmap();
  generateNormalmap(level);
  generateColormap();

  // Delete heightmap texture.
  glDeleteTextures(1, &heightmap);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);
}



Quadtree::~Quadtree() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteTextures(1, &normalmap);
  glDeleteTextures(1, &colormap);

  for (GLuint i = 0; i < 4; i++) {
    delete children[i];
    children[i] = NULL;
  }
}



GLvoid Quadtree::initialize() {
  vertexmapProgram = Display::shaders("vertexmap.vert", "heightmap.frag");
  heightmapProgram = Display::shaders("heightmap.vert", "heightmap.frag");
  normalmapProgram = Display::shaders("normalmap.vert", "normalmap.frag");
  colormapProgram =  Display::shaders("colormap.vert",  "colormap.frag");
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, normalmap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colormap);

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

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

  } else {
    for (GLuint i = 0; i < 4; i++) {
      children[i]->render();
    }
  }
}



GLvoid Quadtree::computeVertexmap() {
  const GLuint meshPositionUniform = glGetUniformLocation(vertexmapProgram, "meshPosition");
  const GLuint meshLengthUniform =   glGetUniformLocation(vertexmapProgram, "meshLength");
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f,  0.f,
    -1.f, -1.f,  0.f,
     1.f,  1.f,  0.f,
     1.f, -1.f,  0.f
  };
  GLuint fbo;
  GLuint ibo;
  GLuint vbo;
  GLuint vertexmap;
  GLfloat ps[VERTICES];

  // Initialize vertexmap texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &vertexmap);
  glBindTexture(GL_TEXTURE_2D, vertexmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // TODO: Use GL_R32F internal format, GL_RED type; possibly OpenGL 3.0+.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, VERTICES_PER_SIDE, VERTICES_PER_SIDE, 0, GL_RGBA, GL_FLOAT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize framebuffer object, attach vertexmap texture.
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vertexmap, 0);
  glDrawBuffers(1, buffers);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Render vertexmap to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glViewport(0, 0, VERTICES_PER_SIDE, VERTICES_PER_SIDE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(vertexmapProgram);
  glUniform3fv(meshPositionUniform, 1, value_ptr(vec3(box[0], box[1], 1.f)));
  glUniform1f(meshLengthUniform, (box[2] - box[0]));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  glPopAttrib();

  // Read vertexmap pixel data.
  // FIXME: Bottleneck.
  glReadPixels(0, 0, VERTICES_PER_SIDE, VERTICES_PER_SIDE, GL_RED, GL_FLOAT, ps);
  for (size_t i = 0; i < VERTICES; i++) {
    const GLfloat noise = 1.f + ps[i] / 16.f;
    this->vs[i].r *= noise;
  }

  // Unbind framebuffer object, delete buffers and texture.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteTextures(1, &vertexmap);
}



GLvoid Quadtree::generateHeightmap() {
  const GLuint meshPositionUniform = glGetUniformLocation(heightmapProgram, "meshPosition");
  const GLuint meshLengthUniform =   glGetUniformLocation(heightmapProgram, "meshLength");
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f,  0.f,
    -1.f, -1.f,  0.f,
     1.f,  1.f,  0.f,
     1.f, -1.f,  0.f
  };
  GLuint fbo;
  GLuint ibo;
  GLuint vbo;

  // Initialize heightmap texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &heightmap);
  glBindTexture(GL_TEXTURE_2D, heightmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // TODO: Use GL_R32F internal format, GL_RED type. Possibly OpenGL 3.0+
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize framebuffer object, attach heightmap texture.
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, heightmap, 0);
  glDrawBuffers(1, buffers);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Render heightmap to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(heightmapProgram);
  glUniform3fv(meshPositionUniform, 1, value_ptr(vec3(box[0], box[1], 1.f)));
  glUniform1f( meshLengthUniform,   (box[2] - box[0]));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  glPopAttrib();

  // Unbind framebuffer object, delete buffers.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}



GLvoid Quadtree::generateNormalmap(GLuint level) {
  const GLuint samplerUniform = glGetUniformLocation(normalmapProgram, "sampler");
  const GLuint levelUniform = glGetUniformLocation(normalmapProgram, "level");
  const GLuint textureSizeUniform = glGetUniformLocation(normalmapProgram, "textureSize");
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f, 0.f,  0.f,  1.f,
    -1.f, -1.f, 0.f,  0.f,  0.f,
     1.f,  1.f, 0.f,  1.f,  1.f,
     1.f, -1.f, 0.f,  1.f,  0.f
  };
  GLuint fbo;
  GLuint ibo;
  GLuint vbo;

  // Initialize normalmap texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &normalmap);
  glBindTexture(GL_TEXTURE_2D, normalmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize framebuffer object, attach normalmap texture.
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalmap, 0);
  glDrawBuffers(1, buffers);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Render normalmap to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);
  glClearColor(1.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(normalmapProgram);
  glUniform1i(samplerUniform, 0);
  glUniform1i(levelUniform, level);
  glUniform1f(textureSizeUniform, (GLfloat)TEXTURE_SIZE);
  glBindTexture(GL_TEXTURE_2D, heightmap);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)12);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  glPopAttrib();

  // Unbind framebuffer object, delete buffers.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}



GLvoid Quadtree::generateColormap() {
  const GLuint samplerUniform = glGetUniformLocation(colormapProgram, "sampler");
  const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f, 0.f,  0.f,  1.f,
    -1.f, -1.f, 0.f,  0.f,  0.f,
     1.f,  1.f, 0.f,  1.f,  1.f,
     1.f, -1.f, 0.f,  1.f,  0.f
  };
  GLuint fbo;
  GLuint ibo;
  GLuint vbo;

  // Initialize colormap texture.
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &colormap);
  glBindTexture(GL_TEXTURE_2D, colormap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Initialize framebuffer object, attach normalmap texture.
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colormap, 0);
  glDrawBuffers(1, buffers);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Render normalmap to framebuffer.
  glPushAttrib(GL_VIEWPORT);
  glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);
  glClearColor(1.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(colormapProgram);
  glUniform1i(samplerUniform, 0);
  glBindTexture(GL_TEXTURE_2D, heightmap);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)12);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  glPopAttrib();

  // Unbind framebuffer object, delete buffers.
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
}



GLvoid Quadtree::divide() {
  GLfloat ca = 0.5f * (box[0] + box[2]); // Center A.
  GLfloat cb = 0.5f * (box[1] + box[3]); // Center B.
  children[0] = new Quadtree(box[0], box[1], ca,     cb, level - 1);
  children[1] = new Quadtree(ca,     box[1], box[2], cb, level - 1);
  children[2] = new Quadtree(box[0], cb,     ca,     box[3], level - 1);
  children[3] = new Quadtree(ca,     cb,     box[2], box[3], level - 1);
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
