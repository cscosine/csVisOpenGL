#include "PerVertexLineRenderer.h"

#include "Camera.h"
#include "Shader.h"
#include "Vao.h"

namespace csVisOpenGL {

PerVertexLineRenderer::PerVertexLineRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffusePerVertex)
    , lineWidth(1) {}

PerVertexLineRenderer::~PerVertexLineRenderer() {}

void PerVertexLineRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void PerVertexLineRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  glLineWidth(lineWidth);

  shader->bind();
  {
    glExtraFunctions->glBindVertexArray(vao->getVao());
    glExtraFunctions->glDrawArrays(GL_LINES, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void PerVertexLineRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void PerVertexLineRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void PerVertexLineRenderer::setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors) {
  assert(positions.cols() == colors.cols());
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
  vao->uploadVertexColors(shader->getVertexLocations().color, colors.data(), colors.cols());
}

void PerVertexLineRenderer::setLines(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setLines(positions, c);
}
} // namespace csVisOpenGL
