#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cmath>
#include <libnoise/noise.h>

#include "lwcppgl/display.hpp"

#include "vertex.hpp"

#define CHUNK_SIZE (16)
#define VERTICES_PER_SIDE (CHUNK_SIZE + 1)
#define VERTICES (VERTICES_PER_SIDE * VERTICES_PER_SIDE)
#define INDICES (CHUNK_SIZE * 2 * VERTICES_PER_SIDE + 2 * (CHUNK_SIZE - 1))

class Quadtree {
  public:
    static GLfloat distance;
    Quadtree(GLfloat, GLfloat, GLfloat, GLfloat);
    ~Quadtree();
    GLvoid update(GLfloat, GLfloat, GLfloat);
    GLvoid render();

  private:
    Vertex vs[VERTICES];
    GLfloat box[4];
    Quadtree *children[4];
    GLuint vbo;
    GLuint ibo;
    GLvoid divide();
    GLfloat distance2(GLfloat, GLfloat, GLfloat);
};

#endif // QUADTREE_HPP
