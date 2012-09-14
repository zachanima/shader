#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cmath>
#include <glm/glm.hpp>

#include "lwcppgl/display.hpp"

#include "noise.hpp"
#include "vertex.hpp"

using namespace glm;

#define CHUNK_SIZE (32)
#define VERTICES_PER_SIDE (CHUNK_SIZE + 1)
#define VERTICES (VERTICES_PER_SIDE * VERTICES_PER_SIDE)
#define INDICES (CHUNK_SIZE * 2 * VERTICES_PER_SIDE + 2 * (CHUNK_SIZE - 1))

class Quadtree {
  public:
    static GLfloat distance;
    Quadtree(GLfloat, GLfloat, GLfloat, GLfloat, GLuint);
    ~Quadtree();
    GLvoid update(vec3);
    GLvoid render();

  private:
    Vertex vs[VERTICES];
    GLfloat box[4];
    Quadtree *children[4];
    GLuint level;
    GLuint vbo;
    GLuint ibo;
    GLvoid divide();
    GLfloat distance2(vec3);
};

#endif // QUADTREE_HPP
