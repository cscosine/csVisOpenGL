#include <csVisOpenGL/renderers/UniformPointRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformPointRenderer::UniformPointRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , pointSize(1)
    , smoothPoints(false) {}

UniformPointRenderer::~UniformPointRenderer() {}

void UniformPointRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformPointRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glPointSize(pointSize);

  shader->bind();
  {
    if (smoothPoints)
      glEnable(GL_POINT_SMOOTH);
    else
      glDisable(GL_POINT_SMOOTH);

    glExtraFunctions->glBindVertexArray(vao->getVao());
    glExtraFunctions->glDrawArrays(GL_POINTS, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void UniformPointRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void UniformPointRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void UniformPointRenderer::setPoints(const Eigen::Matrix3Xf& positions) {
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
}

} // namespace csVisOpenGL
