#pragma once

#include <QOpenGLShaderProgram>

namespace csVisOpenGL {
class Shader {
public:
  struct VertexLocations {
    GLint position;
    GLint normal;
    GLint color;
    GLint texCoord0;
  };

  struct UniformLocations {
    GLint m;
    GLint v;
    GLint p;
    GLint vp;
    GLint mv;
    GLint mvp;
    GLint uniformColor;
  };

public:
  Shader(QOpenGLShader *vertexShader, QOpenGLShader *fragmentShader);

  QOpenGLShaderProgram &getProgram() { return m_shaderProgram; }

  const VertexLocations &getVertexLocations() const { return m_vertexLocations; }
  const UniformLocations &getUniformLocations() const { return m_uniformLocations; }

  bool bind() const { return m_shaderProgram.bind(); }
  void release() const { m_shaderProgram.release(); }

private:
  void bindLocations();

  VertexLocations m_vertexLocations;
  UniformLocations m_uniformLocations;
  mutable QOpenGLShaderProgram m_shaderProgram;
};
} // namespace csVisOpenGL
