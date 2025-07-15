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

private:
  VertexLocations vertexLocations;
  UniformLocations uniformLocations;
  mutable QOpenGLShaderProgram shaderProgram;

  Shader() {};

public:
  static Shader* createShader(QOpenGLShader* vertexShader, QOpenGLShader* fragmentShader);

  QOpenGLShaderProgram& getProgram() {
    return shaderProgram;
  }

  const VertexLocations& getVertexLocations() const {
    return vertexLocations;
  }
  const UniformLocations& getUniformLocations() const {
    return uniformLocations;
  }

  bool bind() const {
    return shaderProgram.bind();
  }
  void release() const {
    shaderProgram.release();
  }

private:
  void bindLocations();
};
} // namespace csVisOpenGL
