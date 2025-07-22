#include <csVisOpenGL/Shader.hpp>
#include <iostream>

namespace csVisOpenGL {

Shader::Shader(QOpenGLShader* vertexShader, QOpenGLShader* fragmentShader) {
  m_shaderProgram.addShader(vertexShader);
  m_shaderProgram.addShader(fragmentShader);
  if (!m_shaderProgram.link()) {
    std::cerr << "Program link error: " << m_shaderProgram.log().toStdString() << std::endl;
    // TODO(sceriani): manage errors here.. maybe a static member that can return null / expected is better
  }
  m_shaderProgram.release();
  this->bindLocations();
}

void Shader::bindLocations() {
  m_vertexLocations.position = m_shaderProgram.attributeLocation("position");
  m_vertexLocations.normal = m_shaderProgram.attributeLocation("normal");
  m_vertexLocations.color = m_shaderProgram.attributeLocation("color");
  m_vertexLocations.texCoord0 = m_shaderProgram.attributeLocation("texCoord0");

  m_uniformLocations.m = m_shaderProgram.uniformLocation("m");
  m_uniformLocations.v = m_shaderProgram.uniformLocation("v");
  m_uniformLocations.p = m_shaderProgram.uniformLocation("p");
  m_uniformLocations.vp = m_shaderProgram.uniformLocation("vp");
  m_uniformLocations.mv = m_shaderProgram.uniformLocation("mv");
  m_uniformLocations.mvp = m_shaderProgram.uniformLocation("mvp");
  m_uniformLocations.uniformColor = m_shaderProgram.uniformLocation("uniformColor");
}
} // namespace csVisOpenGL
