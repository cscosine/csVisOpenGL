#include <csVisOpenGL/Shader.hpp>
#include <iostream>

namespace csVisOpenGL {

Shader::Shader(QOpenGLShader* vertexShader, QOpenGLShader* fragmentShader) {
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  if (!shaderProgram.link()) {
    std::cerr << "Program link error: " << shaderProgram.log().toStdString() << std::endl;
    // TODO(sceriani): manage errors here.. maybe a static member that can return null / expected is better
  }
  shaderProgram.release();
  this->bindLocations();
}

void Shader::bindLocations() {
  vertexLocations.position = shaderProgram.attributeLocation("position");
  vertexLocations.normal = shaderProgram.attributeLocation("normal");
  vertexLocations.color = shaderProgram.attributeLocation("color");
  vertexLocations.texCoord0 = shaderProgram.attributeLocation("texCoord0");

  uniformLocations.m = shaderProgram.uniformLocation("m");
  uniformLocations.v = shaderProgram.uniformLocation("v");
  uniformLocations.p = shaderProgram.uniformLocation("p");
  uniformLocations.vp = shaderProgram.uniformLocation("vp");
  uniformLocations.mv = shaderProgram.uniformLocation("mv");
  uniformLocations.mvp = shaderProgram.uniformLocation("mvp");
  uniformLocations.uniformColor = shaderProgram.uniformLocation("uniformColor");
}
} // namespace csVisOpenGL
