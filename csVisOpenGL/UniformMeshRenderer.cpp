#include <csVisOpenGL/UniformMeshRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformMeshRenderer::UniformMeshRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , cull(false) {}

UniformMeshRenderer::~UniformMeshRenderer() {}

void UniformMeshRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformMeshRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (cull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  shader->bind();
  {
    glExtraFunctions->glBindVertexArray(vao->getVao());
    glExtraFunctions->glDrawArrays(GL_TRIANGLES, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void UniformMeshRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void UniformMeshRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void UniformMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions) {
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
}
} // namespace csVisOpenGL
