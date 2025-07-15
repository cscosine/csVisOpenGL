#include <csVisOpenGL/UniformLineRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

UniformLineRenderer::UniformLineRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffuseUniform)
    , lineWidth(1) {}

UniformLineRenderer::~UniformLineRenderer() {}

void UniformLineRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void UniformLineRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(lineWidth);

  shader->bind();
  {
    glExtraFunctions->glBindVertexArray(vao->getVao());
    glDrawArrays(GL_LINES, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void UniformLineRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void UniformLineRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void UniformLineRenderer::setLines(const Eigen::Matrix3Xf& positions) {
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
}

} // namespace csVisOpenGL
