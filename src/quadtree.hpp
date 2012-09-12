#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "lwcppgl/display.hpp"

#include "vertex.hpp"

#define CHUNK_SIZE (16)
#define VERTICES_PER_SIDE (CHUNK_SIZE + 1)
#define VERTICES (VERTICES_PER_SIDE * VERTICES_PER_SIDE)
#define INDICES (CHUNK_SIZE * 2 * VERTICES_PER_SIDE + 2 * (CHUNK_SIZE - 1))

class Quadtree {
  public:
    Quadtree(GLdouble, GLdouble, GLdouble, GLdouble);
    ~Quadtree();
    GLvoid update(GLdouble, GLdouble, GLdouble);
    GLvoid render();

  private:
    Vertex vs[VERTICES];
    GLdouble box[4];
    GLuint vao;
};

#endif // QUADTREE_HPP
