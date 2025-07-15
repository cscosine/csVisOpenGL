#pragma once

#include <QOpenGLExtraFunctions>

#include <cassert>

namespace csVisOpenGL {
class Vao {
  GLuint vao;
  std::map<GLint, GLuint> locationToVbo;
  int count;

  QOpenGLExtraFunctions* glExtraFunctions;

public:
  Vao();
  virtual ~Vao();

  void initialize(QOpenGLExtraFunctions* glExtraFunctions) {
    this->glExtraFunctions = glExtraFunctions;
  }
  GLuint getVao() const {
    return vao;
  }
  int size() const {
    return count;
  }

  void uploadPositions(GLuint location, const float* values, int count);
  void uploadVertexColors(GLuint location, const float* values, int count);
  void uploadNormals(GLuint location, const float* values, int count);
  void uploadTexCoords0(GLuint location, const float* values, int count);

private:
  void createVBO(GLuint location, GLint size, GLenum type, GLboolean normalized);

  template <typename T>
  void uploadVBO(GLuint vbo, const T* values, int count, GLenum usage = GL_STATIC_DRAW) {
    assert(vbo > 0);
    glExtraFunctions->glBindVertexArray(vao);
    {
      glExtraFunctions->glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glExtraFunctions->glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count, values, usage);
      glExtraFunctions->glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glExtraFunctions->glBindVertexArray(0);
  }
};
} // namespace csVisOpenGL
