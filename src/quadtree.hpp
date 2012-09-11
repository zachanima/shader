#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "lwcppgl/display.hpp"

#include "vertex.hpp"

#define CHUNK_SIZE (16)
#define VERTICES (CHUNK_SIZE * CHUNK_SIZE * 6)
#define INDICES (VERTICES)

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
