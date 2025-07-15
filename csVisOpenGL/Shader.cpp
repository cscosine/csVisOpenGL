#include <csVisOpenGL/Shader.hpp>
#include <iostream>

namespace csVisOpenGL {

Shader* Shader::createShader(QOpenGLShader* vertexShader, QOpenGLShader* fragmentShader) {
  Shader* shader = new Shader();
  shader->shaderProgram.addShader(vertexShader);
  shader->shaderProgram.addShader(fragmentShader);
  if (!shader->shaderProgram.link()) {
    std::cerr << "Program link error: " << shader->shaderProgram.log().toStdString() << std::endl;
    return nullptr;
  }
  shader->shaderProgram.release();
  shader->bindLocations();
  return shader;
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
