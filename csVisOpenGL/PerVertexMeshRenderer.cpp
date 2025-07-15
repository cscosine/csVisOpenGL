#include <csVisOpenGL/PerVertexMeshRenderer.hpp>

#include <csVisOpenGL/Camera.hpp>
#include <csVisOpenGL/Shader.hpp>
#include <csVisOpenGL/Vao.hpp>

namespace csVisOpenGL {

PerVertexMeshRenderer::PerVertexMeshRenderer()
    : Renderer(ShaderFactory::ShaderNames::DiffusePerVertex)
    , cull(false) {}

PerVertexMeshRenderer::~PerVertexMeshRenderer() {}

void PerVertexMeshRenderer::initializeConcrete() {
  setUniformColor(Eigen::Vector4f::Ones().eval());
}

void PerVertexMeshRenderer::drawConcrete(const Camera& camera, const Eigen::Matrix4f& m) {
  if (cull)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  shader->bind();
  {
    glExtraFunctions->glBindVertexArray(vao->getVao());
    glDrawArrays(GL_TRIANGLES, 0, vao->size());
    glExtraFunctions->glBindVertexArray(0);
  }
  shader->release();
}

void PerVertexMeshRenderer::setUniformColor(const Eigen::Vector4f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), color(3));
  shader->release();
}

void PerVertexMeshRenderer::setUniformColor(const Eigen::Vector3f& color) {
  shader->bind();
  glExtraFunctions->glUniform4f(shader->getUniformLocations().uniformColor, color(0), color(1), color(2), 1.0f);
  shader->release();
}

void PerVertexMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix3Xf& colors) {
  Eigen::Matrix4Xf c = Eigen::Matrix4Xf::Ones(4, colors.cols());
  c.topLeftCorner(3, colors.cols()) = colors;
  setTriangles(positions, c);
}

void PerVertexMeshRenderer::setTriangles(const Eigen::Matrix3Xf& positions, const Eigen::Matrix4Xf& colors) {
  assert(positions.cols() == colors.cols());
  vao->uploadPositions(shader->getVertexLocations().position, positions.data(), positions.cols());
  vao->uploadVertexColors(shader->getVertexLocations().color, colors.data(), colors.cols());
}
} // namespace csVisOpenGL
