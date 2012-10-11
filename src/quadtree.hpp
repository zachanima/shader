#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "lwcppgl/display.hpp"

#include "vertex.hpp"

#define CHUNK_SIZE (16)
#define VERTICES_PER_SIDE (CHUNK_SIZE + 1)
#define VERTICES (VERTICES_PER_SIDE * VERTICES_PER_SIDE)
#define INDICES (CHUNK_SIZE * 2 * VERTICES_PER_SIDE + 2 * (CHUNK_SIZE - 1))

using namespace glm;

class Quadtree {
  public:
    static GLfloat minDistance;
    static GLuint heightmapProgram;
    static GLuint normalmapProgram;
    Quadtree(GLfloat, GLfloat, GLfloat, GLfloat, GLuint);
    ~Quadtree();
    static GLvoid initialize();
    GLvoid update(vec3);
    GLvoid render();

  private:
    Vertex vs[VERTICES];
    GLfloat box[4];
    Quadtree *children[4];
    GLuint level;
    GLuint vbo; // Vertex buffer object.
    GLuint ibo; // Index buffer object.
    GLuint texture;
    GLuint normalmap;
    GLvoid divide();
    const GLfloat distance2(vec3);
    const static vec3 spherize(vec3);
};

#endif // QUADTREE_HPP
