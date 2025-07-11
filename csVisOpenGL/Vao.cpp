#include "Vao.h"

namespace csVisOpenGL {

Vao::Vao()
    : vao(0)
    , count(-1) {}

Vao::~Vao() {
  if (vao != 0) {
    for (auto it : locationToVbo)
      glExtraFunctions->glDeleteBuffers(1, &it.second);
    glExtraFunctions->glDeleteVertexArrays(1, &vao);
  }
  vao = 0;
}

void Vao::uploadPositions(GLuint location, const float* values, int count) {
  if (locationToVbo.find(location) == locationToVbo.end())
    createVBO(location, 3, GL_FLOAT, false);
  uploadVBO(locationToVbo[location], values, count * 3);
  this->count = count;
}

void Vao::uploadVertexColors(GLuint location, const float* values, int count) {
  if (locationToVbo.find(location) == locationToVbo.end())
    createVBO(location, 4, GL_FLOAT, false);
  uploadVBO(locationToVbo[location], values, count * 4);
  this->count = count;
}

void Vao::uploadNormals(GLuint location, const float* values, int count) {
  if (locationToVbo.find(location) == locationToVbo.end())
    createVBO(location, 3, GL_FLOAT, true);
  uploadVBO(locationToVbo[location], values, count * 3);
  this->count = count;
}

void Vao::uploadTexCoords0(GLuint location, const float* values, int count) {
  if (locationToVbo.find(location) == locationToVbo.end())
    createVBO(location, 2, GL_FLOAT, false);
  uploadVBO(locationToVbo[location], values, count * 2);
  this->count = count;
}

void Vao::createVBO(GLuint location, GLint size, GLenum type, GLboolean normalized) {
  // Create (if it does not exist) and bind the VAO
  if (vao == 0)
    glExtraFunctions->glGenVertexArrays(1, &vao);
  glExtraFunctions->glBindVertexArray(vao);
  {
    // Create the VBO
    GLuint vbo;
    glExtraFunctions->glGenBuffers(1, &vbo);

    // Setup the VBO
    glExtraFunctions->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glExtraFunctions->glVertexAttribPointer(location, size, type, normalized, 0, 0);
    glExtraFunctions->glEnableVertexAttribArray(location);
    glExtraFunctions->glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Remember its index
    locationToVbo[location] = vbo;
  }

  // Unbind VAO
  glExtraFunctions->glBindVertexArray(0);
}
} // namespace csVisOpenGL
