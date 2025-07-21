#pragma once

#include <QOpenGLExtraFunctions>

#include <cassert>

namespace csVisOpenGL {
class Vao {
public:
  Vao();
  virtual ~Vao();

  void initialize(std::shared_ptr<QOpenGLExtraFunctions> const& glExtraFunctions);

  GLuint getVao() const {
    return m_vao;
  }
  int size() const {
    return m_count;
  }

  void uploadPositions(GLuint location, const float* values, int count);
  void uploadVertexColors(GLuint location, const float* values, int count);
  void uploadNormals(GLuint location, const float* values, int count);
  void uploadTexCoords0(GLuint location, const float* values, int count);

private:
  GLuint m_vao;
  std::map<GLint, GLuint> m_locationToVbo;
  int m_count;

  std::shared_ptr<QOpenGLExtraFunctions> m_glExtraFunctions;
};
} // namespace csVisOpenGL
